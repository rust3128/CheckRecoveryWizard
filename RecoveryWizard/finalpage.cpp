#include "finalpage.h"
#include "ui_finalpage.h"
#include <QRadioButton>

FinalPage::FinalPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::FinalPage)
{
    ui->setupUi(this);

    ui->radioButtonExecute->setStyleSheet(
                "QRadioButton::indicator::checked { image: url(:/Icons/checked.png);}"
                "QRadioButton::indicator::unchecked { image: url(:/Icons/unchecked.png);}"
                );
    ui->radioButtonSave->setStyleSheet(
                "QRadioButton::indicator::checked { image: url(:/Icons/checked.png);}"
                "QRadioButton::indicator::unchecked { image: url(:/Icons/unchecked.png);}"
                );

    ui->checkBoxView->setStyleSheet(
                "QCheckBox::indicator:unchecked {image: url(:/Icons/unchecked-checkbox.png);}"
                "QCheckBox::indicator:checked {image: url(:/Icons/checked-checkbox.png);}"
                );


    this->setFinalPage(true);
}

FinalPage::~FinalPage()
{
    delete ui;
}

bool FinalPage::validatePage()
{
    emit signalGenerateScript();
    if(ui->checkBoxView->isChecked())
        emit signalViewSql();
    if(ui->radioButtonExecute->isChecked())
        emit signalExecScript();
    return true;
}


