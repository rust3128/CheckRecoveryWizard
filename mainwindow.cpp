#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggingcategories.h"

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
