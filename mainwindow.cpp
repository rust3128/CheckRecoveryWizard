#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggingcategories.h"
#include "ConnectionDialog/connectiondialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    slotHideWiz();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotShowWiz()
{
    ui->splitter->show();
}

void MainWindow::slotHideWiz()
{
    ui->splitter->hide();
}

void MainWindow::on_actionExit_triggered()
{
    qInfo(logInfo()) << "Завершение работы.";
    this->close();
}

void MainWindow::on_actionClientsList_triggered()
{
    ConnectionDialog *connDlg = new ConnectionDialog();
    connDlg->exec();
}

void MainWindow::on_action_triggered()
{
    recWiz = new RecoveryWizard();

    connect(recWiz, &RecoveryWizard::signalHideWiz,this,&MainWindow::slotHideWiz);

    ui->verticalLayout->addWidget(recWiz);
    recWiz->show();
    slotShowWiz();

}
