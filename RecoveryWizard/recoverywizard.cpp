#include "recoverywizard.h"
#include "ui_recoverywizard.h"
#include "pagelist.h"
#include "passconv.h"
#include "loggingcategories.h"
#include "viewscriptdialog.h"
#include "lostcheckexecute.h"
#include "checktype.h"



#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>
#include <QThread>
#include <QBoxLayout>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>


RecoveryWizard::RecoveryWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::RecoveryWizard)
{
    ui->setupUi(this);

    initLostCheckFuel();
    append_ASale.clear();

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
    connect(button(QWizard::BackButton),&QAbstractButton::clicked,this,&RecoveryWizard::signalCheckDublicateArticles);
    connect(button(QWizard::BackButton),&QAbstractButton::clicked,this,&RecoveryWizard::slotClearFuelSumm);
    connect(this,&RecoveryWizard::signalFinishWiz,this,&RecoveryWizard::slotInsetLogs);

    connect(connPage,&ConnectionsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
    connect(connPage,&ConnectionsPage::signalConnRecord,this,&RecoveryWizard::slotGetConnRecord);

    connect(termPage,&TerminalsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
    connect(termPage,&TerminalsPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(shiftPage,&ShiftsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
    connect(shiftPage,&ShiftsPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(fuelPage,&FuelPage::sendInfo,this,&RecoveryWizard::slotGetPageData);

    connect(fuelPage,&FuelPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(finalPage,&FinalPage::signalGenerateScript,this,&RecoveryWizard::slotGenerateScript);
    connect(finalPage,&FinalPage::signalViewSql,this,&RecoveryWizard::slotViewSql);
    connect(finalPage,&FinalPage::signalExecScript,this,&RecoveryWizard::slotExecuteSql);
    connect(finalPage,&FinalPage::signalSaveScript,this,&RecoveryWizard::slotSaveScript);

    connect(this,&RecoveryWizard::signalSendConnRec,articlePage,&ArticlePage::slotGetConnRecord);

    connect(articlePage,&ArticlePage::signalSetCommonData,this,&RecoveryWizard::slotSetCommonData);
    connect(articlePage,&ArticlePage::signalSendArticlesData,this,&RecoveryWizard::slotGetArticlesData);
    connect(articlePage,&ArticlePage::signalSendPaytypeArticles,this,&RecoveryWizard::slotGetPaytypeArticles);
    connect(articlePage,&ArticlePage::sendInfo,this,&RecoveryWizard::slotGetPageData);
//    connect(articlePage,&ArticlePage::signalCheckArticles,this,&RecoveryWizard::slotCheckArticles);




}

RecoveryWizard::~RecoveryWizard()
{
    delete ui;
}

void RecoveryWizard::slotGetConnRecord(QSqlRecord rec)
{
    recConnect = rec;
    emit signalSendConnRec(rec);
}

void RecoveryWizard::slotSetCommonData()
{
    initLostCheckArticles();
    initMposCheck();
    append_ASale.clear();

    QHashIterator<QString, QVariant> i(lostCheckFuel);
    while (i.hasNext()) {
        i.next();
        if(lostCheckArticle.contains(i.key()))
            lostCheckArticle[i.key()] = i.value();
        if(mposCheck.contains(i.key()))
            mposCheck[i.key()] = i.value();
    }
    lostCheckArticle["CHECKNUMBER"] = lostCheckFuel["NUM_CHECK"];
    mposCheck["NUMBERCHECK"] = lostCheckFuel["NUM_CHECK"];
    lostCheckArticle["DATEOP"] = lostCheckFuel["DAT"];
}

void RecoveryWizard::slotGetArticlesData(ArticleInfo info)
{
//    info.showMembers();
    QString key = "ARTICLE_ID";
    if(lostCheckArticle.contains(key))
        lostCheckArticle[key] = info.getArticleID();
    else
        qInfo(logInfo()) <<"Invalid KEY" << key ;

    key = "GARTICLE_ID";
    if (lostCheckArticle.contains(key))
        lostCheckArticle[key] = info.getArticleID();
    else
        qInfo(logInfo()) <<"Invalid KEY" << key ;

    key = "PRICE";
    if (lostCheckArticle.contains(key))
        lostCheckArticle[key] = info.getPrice();
    else
        qInfo(logInfo()) <<"Invalid KEY" << key ;

    key = "AMOUNT";
    if(lostCheckArticle.contains(key))
        lostCheckArticle[key] = info.getAmount();
    else
        qInfo(logInfo()) <<"Invalid KEY" << key ;

    key = "SUMMA";
    if(lostCheckArticle.contains(key))
        lostCheckArticle[key] = info.getSumm();
    else
        qInfo(logInfo()) <<"Invalid KEY" << key ;

    key = "DISCOUNT_SUMMA";
    if(lostCheckArticle.contains(key))
        lostCheckArticle[key] = info.getDiscount();
    else
        qInfo(logInfo()) <<"Invalid KEY" << key ;

    append_ASale.append(lostCheckArticle);

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

void RecoveryWizard::initLostCheckArticles()
{
    lostCheckArticle.insert("TERMINAL_ID", 0);
    lostCheckArticle.insert("ASALE_ID", "GEN_ID(GEN_ASALES, 1)");
    lostCheckArticle.insert("SHIFT_ID", 0);
    lostCheckArticle.insert("ARTICLE_ID", 0);
    lostCheckArticle.insert("GARTICLE_ID", 0);
    lostCheckArticle.insert("CODE", 0);
    lostCheckArticle.insert("DATEOP", 0);
    lostCheckArticle.insert("PRICE", 0.00);
    lostCheckArticle.insert("AMOUNT", 0.00);
    lostCheckArticle.insert("SUMMA", 0.00);
    lostCheckArticle.insert("DISCOUNT_SUMMA", 0.00);
    lostCheckArticle.insert("DISCOUNT_PROC", 0.00);
    lostCheckArticle.insert("CHECKNUMBER", 0);
    lostCheckArticle.insert("PAYTYPE_ID", 0);
    lostCheckArticle.insert("INFO_CODE", 0);
    lostCheckArticle.insert("INFO_TEXT", "");
    lostCheckArticle.insert("POS_ID", 0);
    lostCheckArticle.insert("ZNUMBER", 0);
    lostCheckArticle.insert("OPERATOR_ID", 0);
    lostCheckArticle.insert("DEPT_ID", 0);
    lostCheckArticle.insert("NUM_CHECK_RETURN", "NULL");
    lostCheckArticle.insert("DISCOUNT_TYPE", 0);
    lostCheckArticle.insert("POSTRANSACTION_ID", 0);
    lostCheckArticle.insert("SHARE_ID", 0);
    lostCheckArticle.insert("BARCODE", "");
    lostCheckArticle.insert("MPOSCHECK_ID", ":CHECK_ID");
    lostCheckArticle.insert("PAYTYPE_ID2", 0);
    lostCheckArticle.insert("SUMMA2", 0.00);
    lostCheckArticle.insert("DISCOUNT_SUMMA2", 0.00);
    lostCheckArticle.insert("MPOSCHECK_ID_RETURN", 0);
    lostCheckArticle.insert("BONUSCARD", "");

}

void RecoveryWizard::initMposCheck()
{
    mposCheck.insert("TERMINAL_ID", 0);
    mposCheck.insert("MPOSCHECK_ID", ":CHECK_ID");
    mposCheck.insert("POS_ID", 0);
    mposCheck.insert("SHIFT_ID", 0);
    mposCheck.insert("ZNUMBER", 0);
    mposCheck.insert("DAT", 0);
    mposCheck.insert("OPERATOR_ID", 0);
    mposCheck.insert("PAYTYPE_ID", 0);
    mposCheck.insert("PAYTYPE_ID2", 0);
    mposCheck.insert("SALE_TYPE", 1);
    mposCheck.insert("NUMBERCHECK", 0);
    mposCheck.insert("NUM_CHECK_RETURN", 0);
    mposCheck.insert("MPOSCHECK_ID_RETURN", 0);
    mposCheck.insert("COMBINE_TYPE", 0);
    mposCheck.insert("PREORDER_TRN_ID", 0);

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

//    qInfo(logInfo()) << "LostChek set: " << lostCheckFuel[key];
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



void RecoveryWizard::slotGenerateScript(int checkType)
{
    script.clear();
//    endScript.clear();
    mposCheck["COMBINE_TYPE"] = checkType;

    script << "CREATE OR ALTER PROCEDURE TMP_LOST_CHECK ";
    script << "AS ";
    script << "DECLARE VARIABLE CHECK_ID T_INT;";
    script << "BEGIN ";
    script << "CHECK_ID = GEN_ID(GEN_MPOSCHECKS, 1);";

    for(int i=0; i<append_ASale.size();++i){
        script << "EXECUTE PROCEDURE APPEND_ASALE (";
        script << QString("/*TERMINAL_ID - терминал*/                           %1,").arg(append_ASale[i].value("TERMINAL_ID").toString());
        script << QString("/*ASALE_ID - продажа*/                               %1,").arg(append_ASale[i].value("ASALE_ID").toString());
        script << QString("/*SHIFT_ID - № смены*/                               %1,").arg(append_ASale[i].value("SHIFT_ID").toString());
        script << QString("/*ARTICLE_ID -  код в базе*/                         %1,").arg(append_ASale[i].value("ARTICLE_ID").toString());
        script << QString("/*GARTICLE_ID -  код в общем кодификаторе*/          %1,").arg(append_ASale[i].value("GARTICLE_ID").toString());
        script << QString("/*CODE - код в кассе - брать из ARTICLES*/           %1,").arg(append_ASale[i].value("CODE").toString());
        script << QString("/*DAT - дата/времz продажи*/                        '%1',").arg(append_ASale[i].value("DATEOP").toString());
        script << QString("/*PRICE - цена*/                                     %1,").arg(append_ASale[i].value("PRICE").toString());
        script << QString("/*AMOUNT - количество*/                              %1,").arg(append_ASale[i].value("AMOUNT").toString());
        script << QString("/*SUMMA - сумма*/                                    %1,").arg(append_ASale[i].value("SUMMA").toString());
        script << QString("/*DISCOUNT_SUMMA - сумма скидки со знаком +*/        %1,").arg(append_ASale[i].value("DISCOUNT_SUMMA").toString());
        script << QString("/*DISCOUNT_PROC - процент скидки*/                   %1,").arg(append_ASale[i].value("DISCOUNT_PROC").toString());
        script << QString("/*CHECKNUMBER - номер чека*/                         %1,").arg(append_ASale[i].value("CHECKNUMBER").toString());
        script << QString("/*PAYTYPE_ID - вид оплаты*/                          %1,").arg(append_ASale[i].value("PAYTYPE_ID").toString());
        script << QString("/*INFO_CODE - код клиента, только по смарт карте*/   %1,").arg(append_ASale[i].value("INFO_CODE").toString());
        script << QString("/*INFO_TEXT - название клиента (номер карты)*/      '%1',").arg(append_ASale[i].value("INFO_TEXT").toString());
        script << QString("/*POS_ID - не трогать*/                              %1,").arg(append_ASale[i].value("POS_ID").toString());
        script << QString("/*ZNUMBER - є Z-отчета*/                             %1,").arg(append_ASale[i].value("ZNUMBER").toString());
        script << QString("/*OPERATOR_ID - код оператора*/                      %1,").arg(append_ASale[i].value("OPERATOR_ID").toString());
        script << QString("/*DEPT_ID*/                                          %1,").arg(append_ASale[i].value("DEPT_ID").toString());
        script << QString("/*NUM_CHECK_RETURN - чек, по которому возврат */     %1,").arg(append_ASale[i].value("NUM_CHECK_RETURN").toString());
        script << QString("/*DISCOUNT_TYPE*/                                    %1,").arg(append_ASale[i].value("DISCOUNT_TYPE").toString());
        script << QString("/*POSTRANSACTION_ID*/                                %1,").arg(append_ASale[i].value("POSTRANSACTION_ID").toString());
        script << QString("/*SHARE_ID*/                                         %1,").arg(append_ASale[i].value("SHARE_ID").toString());
        script << QString("/*BARCODE - штрихкод*/                              '%1',").arg(append_ASale[i].value("BARCODE").toString());
        script << QString("/*MPOSCHECK_ID*/                                     %1,").arg(append_ASale[i].value("MPOSCHECK_ID").toString());
        script << QString("/*PAYTYPE_ID2 - вид оплаты*/                         %1,").arg(append_ASale[i].value("PAYTYPE_ID2").toString());
        script << QString("/*SUMMA2 - сумма*/                                   %1,").arg(append_ASale[i].value("SUMMA2").toString());
        script << QString("/*DISCOUNT_SUMMA2 - сумма скидки со знаком +*/       %1,").arg(append_ASale[i].value("DISCOUNT_SUMMA2").toString());
        script << QString("/*MPOSCHECK_ID_RETURN*/                              %1,").arg(append_ASale[i].value("MPOSCHECK_ID_RETURN").toString());
        script << QString("/*BONUSCARD*/                                       '%1'").arg(append_ASale[i].value("BONUSCARD").toString());
        script << ");";
    }

    if(!append_ASale.isEmpty()) {
        script << "EXECUTE PROCEDURE APPEND_MPOSCHECK(";
        script << QString("/*TERMINAL_ID - терминал*/                           %1,").arg(mposCheck.value("TERMINAL_ID").toString());
        script << QString("/*MPOSCHECK_ID*/                                     %1,").arg(mposCheck.value("MPOSCHECK_ID").toString());
        script << QString("/*POS_ID - не трогать*/                              %1,").arg(mposCheck.value("POS_ID").toString());
        script << QString("/*SHIFT_ID - № смены*/                               %1,").arg(mposCheck.value("SHIFT_ID").toString());
        script << QString("/*ZNUMBER - № Z-отчета*/                             %1,").arg(mposCheck.value("ZNUMBER").toString());
        script << QString("/*DAT - дата/время продажи*/                        '%1',").arg(mposCheck.value("DAT").toString());
        script << QString("/*OPERATOR_ID - код оператора*/                      %1,").arg(mposCheck.value("OPERATOR_ID").toString());
        script << QString("/*PAYTYPE_ID - вид оплаты*/                          %1,").arg(mposCheck.value("PAYTYPE_ID").toString());
        script << QString("/*PAYTYPE_ID2 - вид оплаты*/                         %1,").arg(mposCheck.value("PAYTYPE_ID2").toString());
        script << QString("/*SALE_TYPE - не трогать */                          %1,").arg(mposCheck.value("SALE_TYPE").toString());
        script << QString("/*NUM_CHECK - номер чека */                          %1,").arg(mposCheck.value("NUMBERCHECK").toString());
        script << QString("/*NUM_CHECK_RETURN - номер чека возврата*/           %1,").arg(mposCheck.value("NUM_CHECK_RETURN").toString());
        script << QString("/*MPOSCHECK_ID_RETURN*/                              %1,").arg(mposCheck.value("MPOSCHECK_ID_RETURN").toString());
        script << QString("/*COMBYNE_TYPE - совместный*/                        %1,").arg(mposCheck.value("COMBINE_TYPE").toString());
        script << QString("/*PREORDER_TRN_ID - совместный*/                     %1").arg(mposCheck.value("PREORDER_TRN_ID").toString());
        script << ");";

    }


    if(checkType != ARTICLE_CHECK)
    {
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
    }
    script << "END;";

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
//    emit signalFinishWiz();


}

void RecoveryWizard::slotSaveScript()
{
    QString curPath = QDir::currentPath()+"//LostCheck";
    QString fileNameLost = QString("Check_%1_%2.sql").arg(lostCheckFuel.value("TERMINAL_ID").toString()).arg(lostCheckFuel.value("NUM_CHECK").toString());

    QDir dir(curPath);
    if(!dir.exists()) {
        dir.mkdir(curPath);
    }
    dir.cd(curPath);

//    MyFileDialog *fileDialog = new MyFileDialog();
//    fileDialog->show();
//    fileDialog->exec();
//    QString fileName = fileDialog->getSaveFileName(this,"Сохранить скрипт",curPath+"//"+fileNameLost,"SQL file (*.sql);;Все файлы (*.*)");

    QString fileName = QFileDialog::getSaveFileName(this,"Сохранить скрипт",curPath+"//"+fileNameLost,
                                           "SQL file (*.sql);;Все файлы (*.*)");

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream qout(&file);

    QListIterator<QString> i(script);
    while (i.hasNext()) {
        qout << i.next() << endl;
    }
    QListIterator<QString> e(endScript);
    while (e.hasNext()) {
        qout << e.next() << endl;
    }

    file.close();
    emit signalFinishWiz();
}

void RecoveryWizard::slotGetPaytypeArticles(int payTypeID)
{
    mposCheck["PAYTYPE_ID"] = lostCheckArticle["PAYTYPE_ID"] = payTypeID;
}

void RecoveryWizard::slotCheckArticles()
{
    emit signalCheckDublicateArticles();
}

void RecoveryWizard::slotInsetLogs()
{
    float summ = lostCheckFuel["SUMMA"].toFloat() - lostCheckFuel["DISCOUNTSUMMA"].toFloat();
    for(int i=0; i<append_ASale.size();++i){
        summ = summ + append_ASale[i].value("SUMMA").toFloat() - append_ASale[i].value("DISCOUNT_SUMMA").toFloat();
    }

    QString strToLog;
    QTextStream in(&strToLog, QIODevice::WriteOnly | QIODevice::Text);
    QListIterator<QString> i(script);
    while (i.hasNext()) {
        in << i.next() << endl;
    }
    QListIterator<QString> e(endScript);
    while (e.hasNext()) {
        in << e.next() << endl;
    }


    int paytypeID = lostCheckFuel["PAYTYPE_ID"].toInt();
    if(paytypeID<1)
        paytypeID = mposCheck["PAYTYPE_ID"].toInt();

    QSqlDatabase dblite = QSqlDatabase::database("options");
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    QSqlQuery q = QSqlQuery(dblite);
    QSqlQuery qc = QSqlQuery(dbcenter);
    qc.prepare("SELECT name FROM paytypes WHERE paytype_id = :paytypeID");
    qc.bindValue(":paytypeID", paytypeID);
    qc.exec();
    qc.next();



    q.prepare("INSERT INTO `logs`(`date`,`connName`,`terminalID`,`shiftID`,`posID`,`numcheck`,`paytype`,`summ`,`script`) "
                         "VALUES (:date, :connName, :terminalID, :shiftID, :posID, :numcheck, :paytype, :summ, :script)");
    q.bindValue(":date",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    q.bindValue(":connName",recConnect.value("conn_name").toString());
    q.bindValue(":terminalID", lostCheckFuel["TERMINAL_ID"].toInt());
    q.bindValue(":shiftID",lostCheckFuel["SHIFT_ID"].toInt());
    q.bindValue(":posID",lostCheckFuel["POS_ID"].toInt());
    q.bindValue(":numcheck",lostCheckFuel["NUM_CHECK"].toInt());
    q.bindValue(":paytype",qc.value(0).toString());
    q.bindValue(":summ",summ);
    q.bindValue(":script",strToLog);

    if(!q.exec()) qInfo(logInfo()) << "NO LOGGING" << q.lastError().text();

//    qInfo(logInfo()) << "типа сохраняли..." << q.lastQuery() << endl << q.executedQuery();

}

void RecoveryWizard::slotClearFuelSumm()
{
    lostCheckFuel["SUMMA"]=0;
    lostCheckFuel["DISCOUNTSUMMA"]=0;
}
