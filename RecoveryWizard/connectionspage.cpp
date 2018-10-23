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
}

ConnectionsPage::~ConnectionsPage()
{
    delete ui;
}

void ConnectionsPage::createModel()
{
    QSqlDatabase dblite = QSqlDatabase::database("options");
    modelConnections = new QSqlTableModel(this,dblite);

    modelConnections->setTable("connections");

    if(!modelConnections->select()) qCritical(logCritical()) << "Class:"
                                                            << metaObject()->className() << "Ошибка при создании модели."
                                                            << modelConnections->lastError().text();

}

void ConnectionsPage::createUI()
{
    ui->listView->setModel(modelConnections);
    ui->listView->setModelColumn(1);
}

