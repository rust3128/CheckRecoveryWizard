#include "connectionspage.h"
#include "ui_connectionspage.h"
#include "loggingcategories.h"

ConnectionsPage::ConnectionsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ConnectionsPage)
{
    ui->setupUi(this);

    this->registerField("connName", ui->listView);



}

ConnectionsPage::~ConnectionsPage()
{
    delete ui;
}

bool ConnectionsPage::validatePage()
{




    QSqlDatabase dblite = QSqlDatabase::database("options");
    QSqlQuery *q = new QSqlQuery(dblite);

    q->prepare("SELECT * FROM connections WHERE conn_id=:connID");
    q->bindValue(":connID",modelConnections->data(modelConnections->index(ui->listView->currentIndex().row(),0)).toInt());
    if(!q->exec()) qCritical(logCritical()) << "Class:"
                                           << metaObject()->className() << "Ошибка при получении параметров подключения."
                                           << q->lastError().text();
    q->next();


/// Создание подключения к центральной базе

    QSqlDatabase dbcentr = QSqlDatabase::addDatabase("QIBASE","central");

    dbcentr.setHostName(q->value("conn_host").toString());
    dbcentr.setDatabaseName(q->value("conn_db").toString());
    dbcentr.setUserName(q->value("conn_user").toString());
    dbcentr.setPassword(q->value("conn_pass").toString());

    if(!dbcentr.open()) {
        qCritical(logCritical()) << "Не возможно подключится к центральной базе данных" << dbcentr.lastError().text();
        ui->labelInfo->setText(QString("Ошибка подключения.\nНе возможно открыть базу данных!\n%1\nПроверьте настройку подключения.")
                               .arg(dbcentr.lastError().text()));
        return false;
    } else {

    ui->labelInfo->clear();

    this->setField("connName", modelConnections->data(modelConnections->index(ui->listView->currentIndex().row(),1)).toString());
    return true;
    }
}

void ConnectionsPage::initializePage()
{
    createModel();
    createUI();
}



void ConnectionsPage::createModel()
{
    QSqlDatabase dblite = QSqlDatabase::database("options");
    modelConnections = new QSqlTableModel(this,dblite);

    modelConnections->setTable("connections");

    if(!modelConnections->select()) qCritical(logCritical()) << "Class:"
                                                            << metaObject()->className() << "Ошибка при создании модели подключений."
                                                            << modelConnections->lastError().text();

}

void ConnectionsPage::createUI()
{
    ui->listView->setModel(modelConnections);
    ui->listView->setModelColumn(1);
}

