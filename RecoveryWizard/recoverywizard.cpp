#include "recoverywizard.h"
#include "ui_recoverywizard.h"
#include "pagelist.h"
#include "loggingcategories.h"


#include <QAbstractButton>
#include <QMessageBox>



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


    disconnect( button( QWizard::CancelButton ), &QAbstractButton::clicked, this, &QDialog::reject );
    connect(button(QWizard::CancelButton),&QAbstractButton::clicked,this,&RecoveryWizard::cancelWizard);


    connect(connPage,&ConnectionsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);

    connect(termPage,&TerminalsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
    connect(termPage,&TerminalsPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(shiftPage,&ShiftsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
    connect(shiftPage,&ShiftsPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

    connect(fuelPage,&FuelPage::sendInfo,this,&RecoveryWizard::slotGetPageData);

    connect(fuelPage,&FuelPage::signalSendCheckData,this,&RecoveryWizard::slotSetLostCheckData);

//    connect(fuelPage,&FuelPage::signalSendCheckData,this,&RecoveryWizard::slotGetPageData);

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

void RecoveryWizard::slotGetPageData(int row, QString info)
{
    emit signalSendCheckInfo(row,info);
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

