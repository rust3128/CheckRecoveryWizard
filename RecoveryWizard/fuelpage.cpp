#include "fuelpage.h"
#include "ui_fuelpage.h"

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
