#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggingcategories.h"
#include "ConnectionDialog/connectiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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
