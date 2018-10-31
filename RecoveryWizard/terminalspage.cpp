#include "terminalspage.h"
#include "ui_terminalspage.h"
#include "loggingcategories.h"
#include "SelectTerminalDialog/selectterminaldialog.h"


#include <QMessageBox>
#include <QTcpSocket>
#include <QIntValidator>

TerminalsPage::TerminalsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::TerminalsPage)
{
    ui->setupUi(this);
    registerField("terminalID*",ui->lineEditTerminal);
    ui->lineEditTerminal->setValidator(new QIntValidator(0,65536,ui->lineEditTerminal));
}

TerminalsPage::~TerminalsPage()
{
    delete ui;
}



void TerminalsPage::initializePage()
{

    ui->labelTerminalName->clear();
    ui->labelOnlineStatus->clear();
    createModelTerminals();

}



bool TerminalsPage::validatePage()
{
    if(ui->lineEditTerminal->text().trimmed().size()< 4){
        ui->labelTerminalName->setStyleSheet("color: red;");
        return false;
    }

    QTcpSocket *tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(modelTerminals->data(modelTerminals->index(idx,2)).toString(),3050);
    if(tcpSocket->waitForConnected(30000)){
        qInfo(logInfo()) << "Проверка доступности. Сервер: " << modelTerminals->data(modelTerminals->index(idx,2)).toString() <<  "FireBird доступен.";
        ui->labelOnlineStatus->setStyleSheet("color: rgb(0, 170, 0);font: 75 14pt 'Noto Sans'");
        ui->labelOnlineStatus->setText("АЗС на связи!");
        setField("terminalID",ui->lineEditTerminal->text().trimmed());
        emit sendInfo(1,ui->lineEditTerminal->text().trimmed());
        emit signalSendCheckData("TERMINAL_ID",ui->lineEditTerminal->text().toInt());
        return true;
    } else {
        qInfo(logInfo()) << "Проверка доступности. Сервер: " << modelTerminals->data(modelTerminals->index(idx,2)).toString() <<  "FireBird НЕ доступен.";
        ui->labelOnlineStatus->setStyleSheet("color: red;font: 75 14pt 'Noto Sans'");
        ui->labelOnlineStatus->setText("Отсутствует связь с АЗС!\nВосстановление чека не возможно!");
        return false;
    }

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






void TerminalsPage::on_lineEditTerminal_textChanged(const QString &term)
{

    QString strTermInfo = "Не верный номер терминала!";
    ui->labelOnlineStatus->clear();
    if(term.length()<4 || term.length()>5)
    {
        ui->labelTerminalName->setText(strTermInfo);
    }
    else
    {
        int terminal = term.toInt();
        idx = -1;
        for(int i=0;i<modelTerminals->rowCount();++i)
        {
            if(terminal == modelTerminals->data(modelTerminals->index(i,0)).toInt())
            {
                idx=i;

                break;
            }
        }
        if(idx>=0)
        {
            strTermInfo=modelTerminals->data(modelTerminals->index(idx,1)).toString();

        }
        ui->labelTerminalName->setText(strTermInfo);
    }

}

void TerminalsPage::on_toolButtonSelectTerminal_clicked()
{
    SelectTerminalDialog *selectTermDlg = new SelectTerminalDialog(modelTerminals);
    connect(selectTermDlg,&SelectTerminalDialog::sendTerminalID,this,&TerminalsPage::getSelectedTerminal);
    selectTermDlg->exec();
}

void TerminalsPage::getSelectedTerminal(int termID)
{
    ui->lineEditTerminal->setText(QString::number(termID));
}
