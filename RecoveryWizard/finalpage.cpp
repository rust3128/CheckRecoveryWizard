#include "finalpage.h"
#include "ui_finalpage.h"
#include <QRadioButton>
#include <checktype.h>

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
    int checkType;

    if(field("checkFuel").toBool()) checkType = FUEL_CHECK;
    if(field("checkFuelArticles").toBool()) checkType = COMBAIN_CHECK;
    if(field("checkArticles").toBool()) checkType = ARTICLE_CHECK;

    emit signalGenerateScript(checkType);

    if(ui->checkBoxView->isChecked())
        emit signalViewSql();
    if(ui->radioButtonExecute->isChecked())
        emit signalExecScript();
    if(ui->radioButtonSave->isChecked())
        emit signalSaveScript();
    return true;
}


