#include "articlepage.h"
#include "ui_articlepage.h"
#include "listarticles.h"
#include "loggingcategories.h"
#include <QThread>
#include <QTime>
#include <QDebug>

ArticlePage::ArticlePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ArticlePage)
{
    ui->setupUi(this);
    ui->tableView->hide();
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
    QVectorIterator<Articles> a(goods);



    while(a.hasNext()){
        ar = a.next();
        qDebug() << ar.getID() << ar.getName() << ar.getShortName() << ar.getAmount() << ar.getPrice();
    }
}

void ArticlePage::slotStartArticlesList()
{
    qInfo(logInfo()) << "Начали получать список товаров" << QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->tableView->hide();
}

void ArticlePage::slotFinishArticlesList()
{
    qInfo(logInfo()) << "Закончили получать список товаров" << QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->frameProgress->hide();
    ui->tableView->show();

    modelArticles = new VektorModel(goods);
    ui->tableView->setModel(modelArticles);
}

void ArticlePage::slotGetConnRecord(QSqlRecord rec)
{
    recrodConn = rec;
}

bool ArticlePage::validatePage()
{
    return false;
}




