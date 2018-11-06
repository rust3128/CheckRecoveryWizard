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
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Параметр"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Значение"));


//    ui->tableWidget->setEnabled(false);
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
    ui->tableWidget->clear();
    recWiz = new RecoveryWizard();
    connect(recWiz, &RecoveryWizard::signalHideWiz,this,&MainWindow::slotHideWiz);
    connect(recWiz, &RecoveryWizard::signalSendCheckInfo,this,&MainWindow::slotCheckInfoUpdate);
    ui->verticalLayout->addWidget(recWiz);
    recWiz->show();

    slotShowWiz();

}
void MainWindow::slotCheckInfoUpdate(QString name, QString value, bool isSpan)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());


    QTableWidgetItem *itemName = new QTableWidgetItem(name);
    QTableWidgetItem *itemValue = new QTableWidgetItem(value);

    if(isSpan) {
        itemValue->setTextAlignment(Qt::AlignVCenter);
        itemValue->setTextAlignment(Qt::AlignHCenter);
        itemValue->setBackgroundColor(QColor(0,255,255));
        ui->tableWidget->setSpan(row,0,1,2);
        ui->tableWidget->setItem(row,0,itemValue);
    } else {
        itemValue->setBackgroundColor(QColor("#aaff7f"));
        ui->tableWidget->setItem(row,0,itemName);
        ui->tableWidget->setItem(row,1,itemValue);
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(ui->tableWidget->verticalHeader()->minimumSectionSize());
}

