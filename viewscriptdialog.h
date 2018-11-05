#ifndef VIEWSCRIPTDIALOG_H
#define VIEWSCRIPTDIALOG_H

#include <QDialog>



namespace Ui {
class ViewScriptDialog;
}

class ViewScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewScriptDialog(QStringList script, QWidget *parent = 0);
    ~ViewScriptDialog();

private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::ViewScriptDialog *ui;

};

#endif // VIEWSCRIPTDIALOG_H
