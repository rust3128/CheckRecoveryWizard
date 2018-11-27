#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggingcategories.h"
#include "ConnectionDialog/connectiondialog.h"
#include "RecoveryWizard/terminalspage.h"
#include "ProgressExecuteDialog/progressexecutedialog.h"
#include "LogsDialog/logsdialog.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createUI();
    slotHideWiz();

    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
//    ui->tableWidget->horizontalHeader()->setVisible(true);
//    ui->tableWidget->horizontalHeaderItem(0)->setText("Параметр");
//    ui->tableWidget->horizontalHeaderItem(1)->setText("Значение");
//    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Параметр"));
//    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Значение"));



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
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
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
    connect(recWiz, &RecoveryWizard::signalFinishWiz,recWiz, &RecoveryWizard::deleteLater);
    connect(recWiz, &RecoveryWizard::signalFinishWiz,this, &MainWindow::slotHideWiz);
    connect(recWiz,&RecoveryWizard::signalCheckDublicateArticles,this,&MainWindow::slotCheckDublicateArticles);
    ui->verticalLayout->addWidget(recWiz);
    recWiz->show();

    slotShowWiz();

}
void MainWindow::slotCheckInfoUpdate(QString name, QString value, bool isSpan)
{

    if(!(name == "Код" || name == "Наименов." || name == "Цена" || name == "Сумма" || name == "Скидка"))
    {
        for(int i=0; i<ui->tableWidget->rowCount();++i){
            if(ui->tableWidget->item(i,0)->text().trimmed() == name || ui->tableWidget->item(i,0)->text().trimmed() == value ) {
                (isSpan) ? ui->tableWidget->item(i,0)->setText(value) : ui->tableWidget->item(i,1)->setText(value);
                return;
            }
        }
    }



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

void MainWindow::slotCheckDublicateArticles()
{
    QString it;
    int rowCount = ui->tableWidget->rowCount();
    for(int i=0; i<rowCount;++i){
        it = ui->tableWidget->item(i,0)->text().trimmed();
        if(it == "Товары"){
            for(int j = rowCount-1; i<j ;--j){
                ui->tableWidget->removeRow(j);
            }
            break;
        }
    }

}



void MainWindow::on_actionLogs_triggered()
{
    LogsDialog *logsDlg = new LogsDialog();
    this->setCentralWidget(logsDlg);
    logsDlg->exec();
}
