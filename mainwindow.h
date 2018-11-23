#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RecoveryWizard/recoverywizard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotShowWiz();                 //отображение мастера
    void slotHideWiz();                 //Скрытие мастера
    void slotCheckInfoUpdate(QString name, QString value, bool isSpan);
    void slotCheckDublicateArticles();

private slots:
    void on_actionExit_triggered();
    void on_actionClientsList_triggered();
    void on_action_triggered();


    void on_actionLogs_triggered();

private:
    Ui::MainWindow *ui;
    RecoveryWizard *recWiz;
private:
    void createUI();
};

#endif // MAINWINDOW_H
