#include "progressexecutedialog.h"
#include "ui_progressexecutedialog.h"
#include "taskliststatus.h"

ProgressExecuteDialog::ProgressExecuteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressExecuteDialog)
{
    ui->setupUi(this);
    prInd = new QProgressIndicator();
    ui->horizontalLayoutPI->addWidget(prInd);

    prInd->setColor(Qt::darkMagenta);
    ui->labelMessageConnect->clear();
    ui->labelMessageExecute->clear();
    ui->pushButton->hide();
    prInd->startAnimation();
}

ProgressExecuteDialog::~ProgressExecuteDialog()
{
    delete ui;
}

void ProgressExecuteDialog::setCurrentStatus(int status)
{
    switch (status) {
    case CONNECT_DB_AZS:
        ui->labelMessageConnect->setText("Подключение к базе данных АЗС...");
        break;
    case EXECUTE_SQL:
        ui->labelMessageExecute->setText("Выполнение процедуры посстановления чека...");
    default:
        break;
    }

}

void ProgressExecuteDialog::setResultStatus(int task, bool isComplite, QString mess)
{
    switch (task) {
    case CONNECT_DB_AZS:
        if(isComplite){
            ui->labelMessageConnect->setText("Подключились к базе данных АЗС.");
        } else {
            ui->labelMessageConnect->setText(mess);
            ui->labelTitle->setStyleSheet("color: rgb(255, 0, 0);");
            ui->labelTitle->setText("Ошибка!");
        }
        break;
    case EXECUTE_SQL:
        if(isComplite){
            ui->labelMessageExecute->setStyleSheet("color: rgb(0, 255, 0);");
            ui->labelMessageExecute->setText("Чек успешно восстановлен!");
            ui->labelTitle->clear();

        } else {
            ui->labelMessageExecute->setText(mess);

            ui->labelTitle->setStyleSheet("color: rgb(255, 0, 0);");
            ui->labelTitle->setText("Ошибка!");
        }
        ui->labelMessageConnect->clear();
        break;
    default:
        break;
    }
    prInd->stopAnimation();
    prInd->hide();
    ui->pushButton->show();
}

void ProgressExecuteDialog::on_pushButton_clicked()
{
    this->reject();
}
