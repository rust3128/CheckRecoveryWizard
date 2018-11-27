#include "logsdialog.h"
#include "ui_logsdialog.h"
#include "loggingcategories.h"
#include <QSqlDatabase>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <QClipboard>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include "SQLHighlighter.h"

LogsDialog::LogsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogsDialog)
{
    ui->setupUi(this);
    createUI();
#if defined(Q_OS_LINUX)
    ui->sqlEdit->setFont(QFont("Source Code Pro"));
#endif
}

LogsDialog::~LogsDialog()
{
    delete ui;
}

void LogsDialog::createUI()
{
    QSqlDatabase dblite = QSqlDatabase::database("options");

    modelLogs = new QSqlTableModel(this,dblite);
    modelLogs->setTable("logs");

    modelLogs->setHeaderData(1,Qt::Horizontal,"Дата операции");
    modelLogs->setHeaderData(2,Qt::Horizontal,"Подключение");
    modelLogs->setHeaderData(3,Qt::Horizontal,"Терминал");
    modelLogs->setHeaderData(4,Qt::Horizontal,"Смена");
    modelLogs->setHeaderData(5,Qt::Horizontal,"Касса");
    modelLogs->setHeaderData(6,Qt::Horizontal,"№ Чека");
    modelLogs->setHeaderData(7,Qt::Horizontal,"Вид оплаты");
    modelLogs->setHeaderData(8,Qt::Horizontal,"Сумма");

    modelLogs->select();
    ui->tableView->setModel(modelLogs);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(9);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->minimumSectionSize());

    ui->groupBoxSQL->hide();
}

void LogsDialog::on_pushButtonClose_clicked()
{
    this->reject();
}

void LogsDialog::on_tableView_doubleClicked(const QModelIndex &idx)
{
    ui->sqlEdit->clear();
    ui->groupBoxSQL->show();
    ui->tableView->setEnabled(false);
    new SQLHighlighter(ui->sqlEdit->document());
    strSQL = modelLogs->data(modelLogs->index(idx.row(),9)).toString();
    ui->sqlEdit->setPlainText(strSQL);
    ui->sqlEdit->selectAll();
    curIdx = idx;

}

void LogsDialog::on_toolButtonSaveAs_clicked()
{
    QString curPath = QDir::currentPath()+"//SQL";
    QString sqlFileName = modelLogs->data(modelLogs->index(curIdx.row(),4)).toString();
    sqlFileName += "_"+modelLogs->data(modelLogs->index(curIdx.row(),5)).toString();
    sqlFileName += "_"+modelLogs->data(modelLogs->index(curIdx.row(),6)).toString()+".sql";

    QDir dir(curPath);
    if(!dir.exists()) {
        dir.mkdir(curPath);
    }
    dir.cd(curPath);
//    qInfo(logInfo()) << "QDir" << dir.current() << "QString" << dir.canonicalPath();
    QString fileName = QFileDialog::getSaveFileName(this,"Сохранить скрипт",curPath+"//"+sqlFileName,
                                           "SQL file (*.sql);;Все файлы (*.*)");

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << "/* Скрипт восстановления чека "+modelLogs->data(modelLogs->index(curIdx.row(),6)).toString() +
                  " АЗС " + modelLogs->data(modelLogs->index(curIdx.row(),4)).toString() +
                  " Смена " + modelLogs->data(modelLogs->index(curIdx.row(),5)).toString() + " */" << endl;
        stream << modelLogs->data(modelLogs->index(curIdx.row(),7)).toString()+";" << endl;
        stream << "commit work;" << endl;
        if (stream.status() != QTextStream::Ok){
            qCritical(logCritical()) << "Ошибка записи в файл";
            QMessageBox::information(this,"Сохранение файла",
                                         "Не удалось сохранить файл.");
        } else {
            QMessageBox::information(this,"Сохранение файла",
                                         "Файл восстановления успешно сохранен.");
        }
    }
    on_toolButtonClose_clicked();
}

void LogsDialog::on_toolButtonClose_clicked()
{
    ui->groupBoxSQL->hide();
    ui->tableView->setEnabled(true);
}

void LogsDialog::on_toolButtonCopy_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strSQL);
    QMessageBox::information(this,"Копирование",
                             "Скрипт успешно скопирован в буфер обмена.");
    on_toolButtonClose_clicked();
}
