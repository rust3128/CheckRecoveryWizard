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
//    ui->tableWidget->setEnabled(false);
}
void MainWindow::slotShowWiz()
{
    ui->splitter->show();
    ui->action->setEnabled(false);
    ui->tableWidget->clear();
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
void MainWindow::slotCheckInfoUpdate(QString name, QString value, bool isSpan)
{
    int row = ui->tableWidget->rowCount();
    int col = 1;
    QBrush br = QBrush(QColor("#aaff7f"));
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    if(isSpan) {
        QTableWidgetItem *item = new QTableWidgetItem();
        ui->tableWidget->setSpan(row,0,1,2);
        item->setTextAlignment(Qt::AlignVCenter);
        item->setTextAlignment(Qt::AlignHCenter);
        col = 0;
        br = QBrush(QColor(0,255,255));
        item->setText(value);
        ui->tableWidget->setItem(row,col,item);
        item->setBackground(br);
    }
    QTableWidgetItem *itemName = new QTableWidgetItem();
    QTableWidgetItem *itemValue = new QTableWidgetItem();
    itemName->setText(name);
    itemValue->setText(value);
//    qInfo(logInfo()) << Q_FUNC_INFO << "Row" << row << "value " << value;
    itemValue->setBackground(br);
    ui->tableWidget->setItem(row,0,itemName);
    ui->tableWidget->setItem(row,col,itemValue);

}

