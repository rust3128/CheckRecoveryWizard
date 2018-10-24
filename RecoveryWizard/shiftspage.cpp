#include "shiftspage.h"
#include "ui_shiftspage.h"

ShiftsPage::ShiftsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ShiftsPage)
{
    ui->setupUi(this);
}

ShiftsPage::~ShiftsPage()
{
    delete ui;
}
