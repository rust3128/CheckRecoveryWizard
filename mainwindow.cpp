#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggingcategories.h"
#include "ConnectionDialog/connectiondialog.h"
#include "RecoveryWizard/terminalspage.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createUI();
    slotHideWiz();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Параметр"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Значение"));
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(ui->tableWidget->verticalHeader()->minimumSectionSize());
    ui->tableWidget->setEnabled(false);
}
void MainWindow::slotShowWiz()
{
    ui->splitter->show();
    ui->action->setEnabled(false);
}

void MainWindow::slotHideWiz()
{
    ui->splitter->hide();
    ui->action->setEnabled(true);
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
    connect(recWiz, &RecoveryWizard::signalSendCheckInfo,this,&MainWindow::slotCheckInfoUpdate);
    ui->verticalLayout->addWidget(recWiz);
    recWiz->show();
    slotShowWiz();

}
void MainWindow::slotCheckInfoUpdate(int row, QString value)
{
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(value);
    ui->tableWidget->setItem(row,1,item);
}

