#include "articlepage.h"
#include "ui_articlepage.h"
#include "listarticles.h"
#include "pagelist.h"
#include "loggingcategories.h"
#include "addarticledialog.h"
#include <QThread>
#include <QTime>
#include <QDebug>
#include <QTableWidgetItem>

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


void ArticlePage::initializePage()
{
//    qInfo(logInfo()) << Q_FUNC_INFO << recrodConn;

    ListArticles *lsArticles = new ListArticles(recrodConn, field("terminalID").toInt(),field("shiftID").toInt());
    QThread *thread = new QThread();
    lsArticles->moveToThread(thread);

    connect(thread,&QThread::started,this,&ArticlePage::slotStartArticlesList);
    connect(thread,&QThread::started,lsArticles,&ListArticles::createListGoods);


    connect(lsArticles,&ListArticles::signalSendArticlesList,this,&ArticlePage::slotGetArticlesList,Qt::DirectConnection);
    connect(lsArticles,&ListArticles::finish,thread,&QThread::quit);

    connect(lsArticles,&ListArticles::finish,lsArticles,&ListArticles::deleteLater);
    connect(lsArticles,&ListArticles::finish,this,&ArticlePage::slotFinishArticlesList);

    connect(thread,&QThread::finished,thread,&QThread::deleteLater);

    thread->start();
}

void ArticlePage::slotGetArticlesList(QVector<Articles> ls)
{
    goods = ls;
//    QVectorIterator<Articles> a(goods);
//    while(a.hasNext()){
//        ar = a.next();
//        qDebug() << ar.getID() << ar.getShortName() << ar.getAmount() << ar.getPrice();
//    }

}

void ArticlePage::slotStartArticlesList()
{
    qInfo(logInfo()) << "Начали получать список товаров" << QTime::currentTime().toString("hh:mm:ss.zzz");

}

void ArticlePage::slotFinishArticlesList()
{
    qInfo(logInfo()) << "Закончили получать список товаров" << QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->frameProgress->hide();
    ui->groupBoxAdd->show();

    modelArticles = new VektorModel(goods);

    ui->tableView->setModel(modelArticles);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->minimumSectionSize());

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

    typedef ArticleInfo arIn;
    qRegisterMetaType<arIn>("articleAmount");

    ArticleInfo ai;


    for(int i=0;i<ui->tableWidget->rowCount();++i){
        ai.setArticleID(ui->tableWidget->item(i,0)->data(Qt::DisplayRole).toFloat());
        ai.setAmount(ui->tableWidget->item(i,2)->data(Qt::DisplayRole).toFloat());
        ai.setPrice(ui->tableWidget->item(i,3)->data(Qt::DisplayRole).toFloat());
        ai.setDiscount(ui->tableWidget->item(i,5)->data(Qt::DisplayRole).toFloat());
        emit signalSendArticlesData(ai);
    }

    return true;
}

void ArticlePage::createUI()
{
    ui->labelError->clear();


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

void ArticlePage::on_pushButtonAdd_clicked()
{
    ui->groupBoxArticles->show();
}

void ArticlePage::on_tableView_doubleClicked(const QModelIndex &idx)
{

    AddArticleDialog *addArtDlg = new AddArticleDialog(goods.at(idx.row()));
    int dlgCode = addArtDlg->exec();


    if(dlgCode == QDialog::Accepted) {
        ui->labelError->clear();
        arInfo = addArtDlg->getGoodsData();
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        Articles ar = goods.at(idx.row());

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
