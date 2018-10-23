#include "recoverywizard.h"
#include "ui_recoverywizard.h"
#include "pagelist.h"
#include "connectionspage.h"
#include "terminalspage.h"

#include <QAbstractButton>
#include <QMessageBox>


RecoveryWizard::RecoveryWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::RecoveryWizard)
{
    ui->setupUi(this);

    this->setPage(CONNECTIONS_PAGE, new ConnectionsPage());
    this->setPage(TERMINALS_PAGE, new TerminalsPage());

    disconnect( button( QWizard::CancelButton ), &QAbstractButton::clicked, this, &QDialog::reject );
    connect(button(QWizard::CancelButton),&QAbstractButton::clicked,this,&RecoveryWizard::cancelWizard);
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
