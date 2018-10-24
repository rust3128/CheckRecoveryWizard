#include "connectionspage.h"
#include "ui_connectionspage.h"
#include "loggingcategories.h"

ConnectionsPage::ConnectionsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ConnectionsPage)
{
    ui->setupUi(this);

    createModel();
    createUI();

    this->registerField("connID", ui->listView);



}

ConnectionsPage::~ConnectionsPage()
{
    delete ui;
}

bool ConnectionsPage::validatePage()
{
    this->setField("connID", modelConnections->data(modelConnections->index(ui->listView->currentIndex().row(),0)).toInt());
    return true;
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

