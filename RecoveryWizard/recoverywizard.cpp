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
    connPage = new ConnectionsPage();
    termPage = new TerminalsPage();
    this->setPage(CONNECTIONS_PAGE, connPage);
    this->setPage(TERMINALS_PAGE, termPage);

    disconnect( button( QWizard::CancelButton ), &QAbstractButton::clicked, this, &QDialog::reject );
    connect(button(QWizard::CancelButton),&QAbstractButton::clicked,this,&RecoveryWizard::cancelWizard);
    connect(termPage,&TerminalsPage::sendInfo,this,&RecoveryWizard::slotGetPageData);
}

RecoveryWizard::~RecoveryWizard()
{
    delete ui;
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

void RecoveryWizard::on_RecoveryWizard_currentIdChanged(int id)
{
    switch (id) {
    case TERMINALS_PAGE:
        emit signalSendCheckInfo(infoRow,infoText);
        break;
    default:
        break;
    }
}

void RecoveryWizard::slotGetPageData(int row, QString info)
{
    infoRow=row;
    infoText=info;
}

