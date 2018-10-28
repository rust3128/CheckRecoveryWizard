#include "fuelpage.h"
#include "ui_fuelpage.h"
#include "loggingcategories.h"


FuelPage::FuelPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::FuelPage)
{
    ui->setupUi(this);
}

FuelPage::~FuelPage()
{
    delete ui;
}

void FuelPage::initializePage()
{
    sendDataTo();
}

void FuelPage::sendDataTo()
{
    emit sendInfo(2,field("shiftID").toString());
    emit signalSendCheckData("SHIFT_ID", field("shiftID").toInt());

    emit sendInfo(3,field("posID").toString());
    qInfo(logInfo()) << "POS ID=" << field("posID").toString();
    emit signalSendCheckData("POS_ID",field("posID").toInt());
}
