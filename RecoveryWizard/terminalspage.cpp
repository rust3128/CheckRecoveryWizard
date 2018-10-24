#include "terminalspage.h"
#include "ui_terminalspage.h"
#include "loggingcategories.h"

#include <QMessageBox>

TerminalsPage::TerminalsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::TerminalsPage)
{
    ui->setupUi(this);
    createUI();

}

TerminalsPage::~TerminalsPage()
{
    delete ui;
}

void TerminalsPage::createUI()
{
    ui->labelTerminalName->setText("Терминал не указан.");
    ui->labelOnlineStatus->hide();
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


/// Создание подключения к центральной базе

    QSqlDatabase dbcentr = QSqlDatabase::addDatabase("QIBASE","central");

    dbcentr.setHostName(q->value("conn_host").toString());
    dbcentr.setDatabaseName(q->value("conn_db").toString());
    dbcentr.setUserName(q->value("conn_user").toString());
    dbcentr.setPassword(q->value("conn_pass").toString());

    if(!dbcentr.open()) {
        qCritical(logCritical()) << "Не возможно подключится к центральной базе данных" << dbcentr.lastError().text();
        QMessageBox::critical(0,"Ошибка подключения", QString("Не возможно открыть базу данных!\n%1\nПроверьте настройку подключения.")
                .arg(dbcentr.lastError().text()),
                QMessageBox::Ok);
    }


    emit sendInfo(0,q->value("conn_name").toString());


}

bool TerminalsPage::validatePage()
{
    if(ui->lineEditTerminal->text().trimmed().size()< 4){
        ui->labelTerminalName->setText("Не верный номер терминала.");
        ui->labelTerminalName->setStyleSheet("color: red;");
        return false;
    }

    return true;
}

void TerminalsPage::createModelTerminals()
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    modelTerminals = new QSqlQueryModel();
    modelTerminals->setQuery("SELECT t.terminal_id, TRIM(t.name) AS name, c.SERVER_NAME, c.DB_NAME, c.CON_PASSWORD FROM TERMINALS t "
                             "LEFT JOIN CONNECTIONS c ON t.TERMINAL_ID = c.TERMINAL_ID "
                             "WHERE t.TERMINALTYPE=3 and t.ISACTIVE='T' and c.CONNECT_ID=2 "
                             "ORDER BY t.TERMINAL_ID",dbcenter);
}



