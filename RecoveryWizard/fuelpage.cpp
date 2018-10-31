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
    emit sendInfo(6,"Параметры топлива");
}


