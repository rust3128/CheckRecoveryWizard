#include "terminalspage.h"
#include "ui_terminalspage.h"

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
