#include "articlepage.h"
#include "ui_articlepage.h"
#include "listarticles.h"
#include "loggingcategories.h"
#include "addarticledialog.h"
#include <QThread>
#include <QTime>
#include <QDebug>

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
    return false;
}

void ArticlePage::createUI()
{
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
        QStringList dlgData = addArtDlg->getGoodsData();
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        Articles ar = goods.at(idx.row());
        ui->tableWidget->setItem(row,0, new QTableWidgetItem(QString::number(ar.getID())));
        ui->tableWidget->setItem(row,1, new QTableWidgetItem(ar.getShortName()));
        ui->tableWidget->setItem(row,2, new QTableWidgetItem(dlgData.at(0)));
        ui->tableWidget->setItem(row,3, new QTableWidgetItem(dlgData.at(1)));
        ui->tableWidget->setItem(row,4, new QTableWidgetItem(dlgData.at(2)));
    }
}
