#include "articlepage.h"
#include "ui_articlepage.h"
#include "listarticles.h"
#include "pagelist.h"
#include "checktype.h"
#include "loggingcategories.h"
#include "addarticledialog.h"
#include <QThread>
#include <QTime>
#include <QDebug>
#include <QTableWidgetItem>
#include <QRegExp>


ArticlePage::ArticlePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ArticlePage)
{
    ui->setupUi(this);
    createUI();

}

ArticlePage::~ArticlePage()
{
    delete ui;
}

//Выбор данных происходит при переходе на определенную странцу QWizardPage
void ArticlePage::initializePage()
{
//    emit signalCheckArticles();


    if(field("checkArticles").toBool()) {
        ui->groupBoxPaytype->show();
       this->registerField("paytypeArtileID*", ui->comboBoxPaytype, "currentIndex", SIGNAL(activated(int)));
        createModelPaytypes();
    }

    //Создаем объект класса и передаем ему параметры
    ListArticles *lsArticles = new ListArticles(recrodConn, field("terminalID").toInt(),field("shiftID").toInt());
    //Создаем поток в которм будут производиться наша выборка
    QThread *thread = new QThread();
    //Перемещаем объект класса в поток
    lsArticles->moveToThread(thread);

    ////Сигналы и слоты для взаимидействия с потоком

    //при старте потока выполняем некоторые действия в текущем потоке.
    //В моем случае на просто засекаю начало выбоки данных
    connect(thread,&QThread::started,this,&ArticlePage::slotStartArticlesList);
    //При старте потока начинаем выборку данных
    connect(thread,&QThread::started,lsArticles,&ListArticles::createListGoods);

    //Передача результирующего объекта QVertor из дочернего потока в основной
    connect(lsArticles,&ListArticles::signalSendArticlesList,this,&ArticlePage::slotGetArticlesList,Qt::DirectConnection);
    //Окончание работы потока по завершению выбрки данных
    connect(lsArticles,&ListArticles::finish,thread,&QThread::quit);
    //Удаляем объект в потоке
    connect(lsArticles,&ListArticles::finish,lsArticles,&ListArticles::deleteLater);
    //Вы полняем действия по в основном потоке после завершения дочернего
    connect(lsArticles,&ListArticles::finish,this,&ArticlePage::slotFinishArticlesList);
    //Прощаемся с дочерним потоком
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);

    //Запускаем поток
    thread->start();
}

void ArticlePage::slotGetArticlesList(QVector<Articles> ls)
{
    //Получаем вектор с результами из потока
    goods = ls;
}

void ArticlePage::slotStartArticlesList()
{
    qInfo(logInfo()) << "Начали получать список товаров" << QTime::currentTime().toString("hh:mm:ss.zzz");

}

void ArticlePage::slotFinishArticlesList()
{
    //на основании полученного вектора с данными создаем модель данных типа VectorModel и выводим ее в QTableView

    qInfo(logInfo()) << "Закончили получать список товаров" << QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->frameProgress->hide();
    ui->groupBoxAdd->show();

    modelArticles = new VektorModel(goods);
    proxyModel = new QSortFilterProxyModel();
    proxyModel->setSourceModel(modelArticles);

    ui->tableView->setModel(modelArticles);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->hideColumn(2);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->resizeColumnsToContents();
    //Минимальная высота строк в QTableView
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->minimumSectionSize());

    emit sendInfo("","Товары",true);
}

void ArticlePage::slotGetConnRecord(QSqlRecord rec)
{
    recrodConn = rec;
}

bool ArticlePage::validatePage()
{
    if(ui->tableWidget->rowCount()== 0){
        ui->labelError->setText("Не указан товар для добаления в чек.");
        return false;
    }

    if(summArticles <= 0){
        ui->labelError->setText("Не верная сумма товаров в чеке.");
        return false;
    }

    emit signalSetCommonData();
    if(ui->comboBoxPaytype->isVisible()){
        emit signalSendPaytypeArticles(modelPaytypes->data(modelPaytypes->index(field("paytypeArtileID").toInt(),0)).toInt());
    }

    typedef ArticleInfo arIn;
    qRegisterMetaType<arIn>("articleAmount");

    ArticleInfo ai;

    for(int i=0;i<ui->tableWidget->rowCount();++i){
        ai.setArticleID(ui->tableWidget->item(i,0)->data(Qt::DisplayRole).toFloat());
        ai.setAmount(ui->tableWidget->item(i,2)->data(Qt::DisplayRole).toFloat());
        ai.setPrice(ui->tableWidget->item(i,3)->data(Qt::DisplayRole).toFloat());
        ai.setDiscount(ui->tableWidget->item(i,5)->data(Qt::DisplayRole).toFloat());
        emit signalSendArticlesData(ai);
        emit sendInfo("","Товар № "+QString::number(i+1),true);
        emit sendInfo("Код",QString::number(ai.getArticleID()),false);
        emit sendInfo("Наименов.",ui->tableWidget->item(i,1)->data(Qt::DisplayRole).toString(),false);
        emit sendInfo("Цена",QString::number(ai.getAmount(),'f',2),false);
        emit sendInfo("Сумма",QString::number(ai.getSumm(),'f',2),false);
        emit sendInfo("Скидка",QString::number(ai.getDiscount(),'f',2),false);
    }

    return true;
}

void ArticlePage::createUI()
{
    ui->labelError->clear();
    ui->groupBoxPaytype->hide();

    ui->labelSumm->setText(QString("<table width=100% border=1 cellpadding=4>"
                                   "<tr align=left valign=top>"
                                   "<td align=right width=33%>Итого: %1 грн.</td>"
                                   "<td align=right width=33%>Скидка: %2 грн.</td>"
                                   "<td align=right>Итого со скидкой: %3 грн.</td>"
                                   "</tr>"
                                   "</table>")
                           .arg(QString::number(summArticles,'f',2))
                           .arg(QString::number(discountArticles,'f',2))
                           .arg(QString::number(summArticles - discountArticles,'f',2)));


    ui->groupBoxArticles->hide();
    ui->groupBoxAdd->hide();
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Гл.код"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Наименование"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Кол-во"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Цена"));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Сумма"));
    ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Скидка"));
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(ui->tableWidget->verticalHeader()->minimumSectionSize());
}

void ArticlePage::createModelPaytypes()
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    modelPaytypes = new QSqlQueryModel();
    modelPaytypes->setQuery("SELECT p.PAYTYPE_ID, p.NAME, p.DLLNAME FROM PAYTYPES p WHERE p.ARTICLETYPE>0 and p.ISACTIVE='T'",dbcenter);
    ui->comboBoxPaytype->setModel(modelPaytypes);
    ui->comboBoxPaytype->setModelColumn(1);
    ui->comboBoxPaytype->setCurrentIndex(-1);
}

void ArticlePage::on_pushButtonAdd_clicked()
{
    ui->groupBoxArticles->show();
}

void ArticlePage::on_tableView_doubleClicked(const QModelIndex &idx)
{
    QModelIndex sourceIdx = idx;
//    AddArticleDialog *addArtDlg = new AddArticleDialog(goods.at(idx.row()));
    if(!ui->lineEditFind->text().isEmpty()){
        sourceIdx = proxyModel->mapToSource(idx);
    }
    AddArticleDialog *addArtDlg = new AddArticleDialog(goods.at(sourceIdx.row()));
    int dlgCode = addArtDlg->exec();


    if(dlgCode == QDialog::Accepted) {
        ui->labelError->clear();
        arInfo = addArtDlg->getGoodsData();
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        Articles ar = goods.at(sourceIdx.row());

        ui->tableWidget->setItem(row,0, new QTableWidgetItem(QString::number(arInfo.getArticleID())));
        ui->tableWidget->setItem(row,1, new QTableWidgetItem(ar.getShortName()));
        ui->tableWidget->setItem(row,2, new QTableWidgetItem(QString::number(arInfo.getAmount(),'f',2)));
        ui->tableWidget->setItem(row,3, new QTableWidgetItem(QString::number(arInfo.getPrice(),'f',2)));
        ui->tableWidget->setItem(row,4, new QTableWidgetItem(QString::number(arInfo.getSumm(),'f',2)));
        ui->tableWidget->setItem(row,5, new QTableWidgetItem(QString::number(arInfo.getDiscount(),'f',2)));
        ui->tableWidget->resizeColumnsToContents();
        summArticles += arInfo.getSumm();
        discountArticles += arInfo.getDiscount();
        ui->labelSumm->setText(QString("<table width=100% border=1 cellpadding=4>"
                                       "<tr align=left valign=top>"
                                       "<td align=right width=33%>Итого: %1 грн.</td>"
                                       "<td align=right width=33%>Скидка: %2 грн.</td>"
                                       "<td align=right>Итого со скидкой: %3 грн.</td>"
                                       "</tr>"
                                       "</table>")
                               .arg(QString::number(summArticles,'f',2))
                               .arg(QString::number(discountArticles,'f',2))
                               .arg(QString::number(summArticles - discountArticles,'f',2)));
        ui->groupBoxArticles->hide();

    }
}

void ArticlePage::on_pushButtonDelete_clicked()
{
    int selectedRow = ui->tableWidget->selectionModel()->selectedRows().first().row();
//    qInfo(logInfo()) << "Current row" << selectedRow;
    summArticles -= ui->tableWidget->item(selectedRow,4)->text().toFloat();
    discountArticles -= ui->tableWidget->item(selectedRow,5)->text().toFloat();
    ui->tableWidget->removeRow(selectedRow);
    ui->labelSumm->setText(QString("<table width=100% border=1 cellpadding=4>"
                                   "<tr align=left valign=top>"
                                   "<td align=right width=33%>Итого: %1 грн.</td>"
                                   "<td align=right width=33%>Скидка: %2 грн.</td>"
                                   "<td align=right>Итого со скидкой: %3 грн.</td>"
                                   "</tr>"
                                   "</table>")
                           .arg(QString::number(summArticles,'f',2))
                           .arg(QString::number(discountArticles,'f',2))
                           .arg(QString::number(summArticles - discountArticles,'f',2)));
    ui->groupBoxArticles->hide();
}
int ArticlePage::nextId() const
{
    return FINAL_PAGE;
}

void ArticlePage::on_lineEditFind_textChanged()
{
    int columFind;
    ui->tableView->setModel(proxyModel);
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp(ui->lineEditFind->text(),Qt::CaseInsensitive,syntax);
    if(ui->radioButtonName->isChecked()) columFind = 1;
    if(ui->radioButtonArticles->isChecked()) columFind =0;
    if(ui->radioButtonPrice->isChecked()) columFind = 3;
    proxyModel->setFilterKeyColumn(columFind);
    proxyModel->setFilterRegExp(regExp);


}
