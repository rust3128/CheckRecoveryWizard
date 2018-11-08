#include "recoverywizard.h"
#include "ui_recoverywizard.h"
#include "pagelist.h"
#include "passconv.h"
#include "loggingcategories.h"
#include "viewscriptdialog.h"
#include "lostcheckexecute.h"



#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>
#include <QThread>
#include <QBoxLayout>


RecoveryWizard::RecoveryWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::RecoveryWizard)
{
    ui->setupUi(this);

    initLostCheckFuel();

    connPage = new ConnectionsPage();
    termPage = new TerminalsPage();
    shiftPage = new ShiftsPage();
    fuelPage = new FuelPage();
    finalPage = new FinalPage();
    articlePage = new ArticlePage();

    this->setPage(CONNECTIONS_PAGE, connPage);
    this->setPage(TERMINALS_PAGE, termPage);
    this->setPage(SHIFTS_PAGE, shiftPage);
    this->setPage(FUELDATA_PAGE, fuelPage);
    this->setPage(FINAL_PAGE, finalPage);
    this->setPage(ARTICLES_DATA_PAGE, articlePage);

//    QPushButton butFinish = this->findChild<QPushButton>("qt_wizard_finish");
//    QObject::connect(butFinish,SIGNAL(clicked()),this,SLOT(slotExecuteSql()));
//    connect(this->button(QWizard::FinishButton),&QAbstractButton::clicked,this,&RecoveryWizard::slotExecuteSql);


    disconnect( button( QWizard::CancelButton ), &QAbstractButton::clicked, this, &QDialog::reject );
    connect(button(QWizard::CancelButton),&QAbstractButton::clicked,this,&RecoveryWizard::cancelWizard);


    connect(connPage,&ConnectionsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);

    connect(termPage,&TerminalsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
    connect(termPage,&TerminalsPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(shiftPage,&ShiftsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
    connect(shiftPage,&ShiftsPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(fuelPage,&FuelPage::sendInfo,this,&RecoveryWizard::slotGetPageData);

    connect(fuelPage,&FuelPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(finalPage,&FinalPage::signalGenerateScript,this,&RecoveryWizard::slotGenerateScript);
    connect(finalPage,&FinalPage::signalViewSql,this,&RecoveryWizard::slotViewSql);
    connect(finalPage,&FinalPage::signalExecScript,this,&RecoveryWizard::slotExecuteSql);

//    QBoxLayout *mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(m_lineEdit = new QLineEdit);
//    mainLayout->addStretch(1);
//    this->setLayout(mainLayout);
//    m_lineEdit->hide();
//    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/Icons/wizlogo.jpg"));

}

RecoveryWizard::~RecoveryWizard()
{
    delete ui;
}

void RecoveryWizard::initLostCheckFuel()
{
    lostCheckFuel.insert("TERMINAL_ID",0);
    lostCheckFuel.insert("SHIFT_ID",0);
    lostCheckFuel.insert("DISPENSER_ID",-1);
    lostCheckFuel.insert("TRK_ID",0);
    lostCheckFuel.insert("TANK_ID",0);
    lostCheckFuel.insert("FUEL_ID",-1);
    lostCheckFuel.insert("GIVE",0);
    lostCheckFuel.insert("ORDERED",0);
    lostCheckFuel.insert("SUMMA",0);
    lostCheckFuel.insert("CASH",0);
    lostCheckFuel.insert("DISCOUNTSUMMA",0);
    lostCheckFuel.insert("PAYTYPE_ID",-1);
    lostCheckFuel.insert("NUM_CHECK",0);
    lostCheckFuel.insert("NUM_CHECK_RETURN",0);
    lostCheckFuel.insert("TRANSACTION_ID",0);
    lostCheckFuel.insert("SEC",0);
    lostCheckFuel.insert("ISLAST","T");
    lostCheckFuel.insert("INFO_CODE",-1);
    lostCheckFuel.insert("INFO_TEXT","");
    lostCheckFuel.insert("POS_ID",1);
    lostCheckFuel.insert("ZNUMBER",0);
    lostCheckFuel.insert("OPERATOR_ID",-1);
    lostCheckFuel.insert("SALEORDER_ID","GEN_ID(GEN_SALEORDERS, 1)");
    lostCheckFuel.insert("PRICE",0);
    lostCheckFuel.insert("ISBEFOREPAY","F");
    lostCheckFuel.insert("POSTRANSACTION_ID",0);
    lostCheckFuel.insert("POSTRNRETURN_ID",0);
    lostCheckFuel.insert("SHARE_ID",0);
    lostCheckFuel.insert("MPOSCHECK_ID",":CHECK_ID");
    lostCheckFuel.insert("PAYTYPE_ID2",1);
    lostCheckFuel.insert("SUMMA2",0.00);
    lostCheckFuel.insert("DISCOUNTSUMMA2",0.00);
    lostCheckFuel.insert("DAT",0);
    lostCheckFuel.insert("GOV_NUMBER","");
    lostCheckFuel.insert("BONUSCARD","");
}


void RecoveryWizard::cancelWizard()
{
    if( QMessageBox::question( this, "Завершить восстановление",
                               "Восстановление чека не завршено. Отметь восстановление?",
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes ) {
        // allow cancel
        emit signalHideWiz();
        reject();
    }

}

//void RecoveryWizard::on_RecoveryWizard_currentIdChanged(int id)
//{

//    switch (id) {
//    case CONNECTIONS_PAGE:
////        qInfo(logInfo()) << Q_FUNC_INFO << "case CONNECTIONS_PAGE:" << "Current ID changed:ROW" << infoRow << "Text" << infoText;
//        break;
//    case TERMINALS_PAGE:
////        qInfo(logInfo()) << Q_FUNC_INFO << "case TERMINALS_PAGE:" << "Current ID changed:ROW" << infoRow << "Text" << infoText;
////        emit signalSendCheckInfo(infoRow,infoText);
//        break;
//    default:
//        break;
//    }


//}

void RecoveryWizard::slotGetPageData(QString name, QString info, bool isSpan)
{
    emit signalSendCheckInfo(name, info, isSpan);
}

void RecoveryWizard::slotSetLostCheckData(QString key, QVariant data)
{
    if(lostCheckFuel.contains(key)){
        lostCheckFuel[key] = data;
    } else {
        qCritical(logCritical()) <<  QString("Class: %1 Metod: %2. Не верный ключ ХЕШ данных для процедуры: "+key+".")
                                     .arg(this->metaObject()->className())
                                     .arg(Q_FUNC_INFO);
    }

    qInfo(logInfo()) << "LostChek set: " << lostCheckFuel[key];
//    QHashIterator<QString, QVariant> i(lostCheckFuel);
//    while (i.hasNext()) {
//        i.next();
//        qInfo(logInfo()) << i.key() << ": " << i.value();
//    }

}

void RecoveryWizard::slotViewSql()
{

    ViewScriptDialog *viewScript = new ViewScriptDialog(script+endScript);
    viewScript->exec();
}



void RecoveryWizard::slotGenerateScript()
{
    script.clear();
//    endScript.clear();

    script << "CREATE OR ALTER PROCEDURE TMP_LOST_CHECK ";
    script << "AS ";
    script << "DECLARE VARIABLE CHECK_ID T_INT;";
    script << "BEGIN ";
    script << "CHECK_ID = GEN_ID(GEN_MPOSCHECKS, 1);";
    script << "EXECUTE PROCEDURE APPEND_FSALE(";
    script << QString("/*TERMINAL_ID - терминал*/                           %1,").arg(lostCheckFuel.value("TERMINAL_ID").toString());
    script << QString("/*SHIFT_ID - № смены*/                               %1,").arg(lostCheckFuel.value("SHIFT_ID").toString());
    script << QString("/*DISPENSER_ID - номер колонки*/                     %1,").arg(lostCheckFuel.value("DISPENSER_ID").toString());
    script << QString("/*TRK_ID - номер пистолета*/                         %1,").arg(lostCheckFuel.value("TRK_ID").toString());
    script << QString("/*TANK_ID - номер резервуара */                      %1,").arg(lostCheckFuel.value("TANK_ID").toString());
    script << QString("/*FUEL_ID - код топлива */                           %1,").arg(lostCheckFuel.value("FUEL_ID").toString());
    script << QString("/*GIVE - залито */                                   %1,").arg(lostCheckFuel.value("GIVE").toString());
    script << QString("/*ORDERED - заказано */                              %1,").arg(lostCheckFuel.value("ORDERED").toString());
    script << QString("/*SUMMA - сумма */                                   %1,").arg(lostCheckFuel.value("SUMMA").toString());
    script << QString("/*CASH - не трогать */                               %1,").arg(lostCheckFuel.value("CASH").toString());
    script << QString("/*DISCOUNTSUMMA - сумма скидки НЕ отрицательна */    %1,").arg(lostCheckFuel.value("DISCOUNTSUMMA").toString());
    script << QString("/*PAYTYPE_ID - вид оплаты*/                          %1,").arg(lostCheckFuel.value("PAYTYPE_ID").toString());
    script << QString("/*NUM_CHECK - номер чека */                          %1,").arg(lostCheckFuel.value("NUM_CHECK").toString());
    script << QString("/*NUM_CHECK_RETURN - номер чека возврата*/           %1,").arg(lostCheckFuel.value("NUM_CHECK_RETURN").toString());
    script << QString("/*TRANSACTION_ID - номер транзакции*/                %1,").arg(lostCheckFuel.value("TRANSACTION_ID").toString());
    script << QString("/*SEC - врем€ заправки в секундах*/                  %1,").arg(lostCheckFuel.value("SEC").toString());
    script << QString("/*ISLAST - не трогать */                            '%1',").arg(lostCheckFuel.value("ISLAST").toString());
    script << QString("/*INFO_CODE - код клиента*/                          %1,").arg(lostCheckFuel.value("INFO_CODE").toString());
    script << QString("/*INFO_TEXT - название клиента (номер карты)*/      '%1',").arg(lostCheckFuel.value("INFO_TEXT").toString());
    script << QString("/*POS_ID - не трогать*/                              %1,").arg(lostCheckFuel.value("POS_ID").toString());
    script << QString("/*ZNUMBER - № Z-отчета*/                             %1,").arg(lostCheckFuel.value("ZNUMBER").toString());
    script << QString("/*OPERATOR_ID - код оператора*/                      %1,").arg(lostCheckFuel.value("OPERATOR_ID").toString());
    script << QString("/*SALEORDER_ID - не трогать */                       %1,").arg(lostCheckFuel.value("SALEORDER_ID").toString());
    script << QString("/*PRICE - цена*/                                     %1,").arg(lostCheckFuel.value("PRICE").toString());
    script << QString("/*ISBEFOREPAY - предоплата*/                        '%1',").arg(lostCheckFuel.value("ISBEFOREPAY").toString());
    script << QString("/*POSTRANSACTION_ID*/                                %1,").arg(lostCheckFuel.value("POSTRANSACTION_ID").toString());
    script << QString("/*POSTRETURN_ID*/                                    %1,").arg(lostCheckFuel.value("POSTRNRETURN_ID").toString());
    script << QString("/*SHARE_ID*/                                         %1,").arg(lostCheckFuel.value("SHARE_ID").toString());
    script << QString("/*MPOSCHECK_ID*/                                     %1,").arg(lostCheckFuel.value("MPOSCHECK_ID").toString());
    script << QString("/*PAYTYPE_ID2 - вид оплаты*/                         %1,").arg(lostCheckFuel.value("PAYTYPE_ID2").toString());
    script << QString("/*SUMMA2 - сумма */                                  %1,").arg(lostCheckFuel.value("SUMMA2").toString());
    script << QString("/*DISCOUNTSUMMA2 - сумма скидки не отрицательная*/   %1,").arg(lostCheckFuel.value("DISCOUNTSUMMA2").toString());
    script << QString("/*DAT - дата/времz продажи*/                        '%1',").arg(lostCheckFuel.value("DAT").toString());
    script << QString("/*GOV_NUMBER*/                                      '%1',").arg(lostCheckFuel.value("GOV_NUMBER").toString());
    script << QString("/*BONUSCARD*/                                       '%1');").arg(lostCheckFuel.value("BONUSCARD").toString());
    script << "END ";

    endScript << "EXECUTE PROCEDURE TMP_LOST_CHECK;";
    endScript << "DROP PROCEDURE TMP_LOST_CHECK;";
    endScript << "COMMIT WORK;";
}

void RecoveryWizard::slotExecuteSql()
{
    qInfo(logInfo()) << "Начинаем выполнять скрипт";

    QHash<QString,QString> connAzs;

    QSqlDatabase dbCenter = QSqlDatabase::database("central");
    QSqlQuery *q = new QSqlQuery(dbCenter);
    q->prepare("SELECT c.SERVER_NAME, c.DB_NAME, c.CON_PASSWORD FROM CONNECTIONS c WHERE c.TERMINAL_ID=:term_id AND c.CONNECT_ID=2");
    q->bindValue(":term_id",lostCheckFuel.value("TERMINAL_ID"));

    if(!q->exec()){
        QString errSQL = QString("%1 Не возможно получить данные о подключении к АЗС.\nПричина: %2")
                .arg(Q_FUNC_INFO)
                .arg(q->lastError().text());
        QMessageBox::critical(this,"Ошибка",errSQL);
        qCritical(logCritical()) << errSQL;
        return;
    }
    q->next();
    connAzs.insert("SERVER_NAME",q->value("SERVER_NAME").toString());
    connAzs.insert("DB_NAME",q->value("DB_NAME").toString());
    connAzs.insert("PASSWORD",passConv(q->value("CON_PASSWORD").toString()));


    LostCheckExecute *lsExec = new LostCheckExecute();
    QThread *thread = new QThread(this);
    progExecDlg = new ProgressExecuteDialog();
    lsExec->setScript(script);
    lsExec->setTerminalID(lostCheckFuel.value("TERMINAL_ID").toInt());
    lsExec->setConnData(connAzs);


    lsExec->moveToThread(thread);
    connect(thread,&QThread::started,lsExec,&LostCheckExecute::slotScriptExecute);
    connect(thread,&QThread::started,this,&RecoveryWizard::slotStartExecute);

    connect(lsExec,&LostCheckExecute::signalCurrentTask,progExecDlg,&ProgressExecuteDialog::setCurrentStatus);
    connect(lsExec,&LostCheckExecute::signalTaskStatus,progExecDlg,&ProgressExecuteDialog::setResultStatus);


    connect(lsExec,&LostCheckExecute::finished,this,&RecoveryWizard::slotFinisExecute,Qt::DirectConnection);
    connect(lsExec,&LostCheckExecute::finished,thread,&QThread::quit);
    connect(lsExec,&LostCheckExecute::finished,lsExec,&LostCheckExecute::deleteLater);
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);

    thread->start();

}

void RecoveryWizard::slotStartExecute()
{
    emit signalFinishWiz();
    progExecDlg->exec();

}

void RecoveryWizard::slotFinisExecute()
{
//    QMessageBox msgBox;
//    msgBox.setWindowTitle("Информация");
//    msgBox.setText("Результат выполнения скрипта.");
//    msgBox.setInformativeText(message);
//    if(isValid) {
//        msgBox.setIcon(QMessageBox::Information);
//    } else {
//        msgBox.setIcon(QMessageBox::Critical);
//    }
//    msgBox.exec();
////    emit signalFinishWiz();


}
