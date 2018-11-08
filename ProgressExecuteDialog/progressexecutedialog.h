#ifndef PROGRESSEXECUTEDIALOG_H
#define PROGRESSEXECUTEDIALOG_H

#include <QDialog>
#include <ProgressIndicator/QProgressIndicator.h>

namespace Ui {
class ProgressExecuteDialog;
}

class ProgressExecuteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressExecuteDialog(QWidget *parent = 0);
    ~ProgressExecuteDialog();

public slots:
    void setCurrentStatus(int status);
    void setResultStatus(int task, bool isComplite, QString mess);
private slots:
    void on_pushButton_clicked();

private:
    Ui::ProgressExecuteDialog *ui;
    QProgressIndicator *prInd;
};

#endif // PROGRESSEXECUTEDIALOG_H
