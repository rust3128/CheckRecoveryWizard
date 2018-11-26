#include "viewscriptdialog.h"
#include "ui_viewscriptdialog.h"

#include "SQLHighlighter.h"

ViewScriptDialog::ViewScriptDialog(QStringList script, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewScriptDialog)
{
    ui->setupUi(this);

//#if defined(Q_OS_LINUX)
//    ui->sqlEdit->setFont(QFont("Source Code Pro"));
//#endif
    ui->sqlEdit->setFont(QFont("monospace"));


//    SQLHighlighter* highlighter =
      new SQLHighlighter(ui->sqlEdit->document());
//    highlighter->setDocument(ui->sqlEdit->document());

    ui->sqlEdit->append(script.join("\n"));

}

ViewScriptDialog::~ViewScriptDialog()
{
    delete ui;
}

void ViewScriptDialog::on_pushButtonClose_clicked()
{
    this->reject();
}
