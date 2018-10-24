#include "terminalspage.h"
#include "ui_terminalspage.h"
#include "loggingcategories.h"

TerminalsPage::TerminalsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::TerminalsPage)
{
    ui->setupUi(this);

}

TerminalsPage::~TerminalsPage()
{
    delete ui;
}

void TerminalsPage::initializePage()
{
//    this->setTitle("Выбрана база c ID="+field("connID").toString());


    QSqlDatabase dblite = QSqlDatabase::database("options");
    QSqlQuery *q = new QSqlQuery(dblite);

    q->prepare("SELECT * FROM connections WHERE conn_id=:connID");
    q->bindValue(":connID",field("connID").toInt());
    if(!q->exec()) qCritical(logCritical()) << "Class:"
                                           << metaObject()->className() << "Ошибка при получении параметров подключения."
                                           << q->lastError().text();
    q->next();

    qInfo(logInfo()) << "Conn Name " << q->value("conn_name").toString();
    emit sendInfo(0,q->value("conn_name").toString());


}
