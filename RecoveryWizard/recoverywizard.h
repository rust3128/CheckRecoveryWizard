#ifndef RECOVERYWIZARD_H
#define RECOVERYWIZARD_H

#include "connectionspage.h"
#include "terminalspage.h"
#include "shiftspage.h"
#include <QWizard>

namespace Ui {
class RecoveryWizard;
}

class RecoveryWizard : public QWizard
{
    Q_OBJECT

signals:
    void signalHideWiz();
    void signalSendCheckInfo(int,QString);
private slots:
    void cancelWizard();
    void on_RecoveryWizard_currentIdChanged(int id);
    void slotGetPageData(int row, QString info);

public:
    explicit RecoveryWizard(QWidget *parent = 0);
    ~RecoveryWizard();

private:
    Ui::RecoveryWizard *ui;
    TerminalsPage *termPage;
    ConnectionsPage *connPage;
    ShiftsPage *shifPage;
    int infoRow;
    QString infoText;
};

#endif // RECOVERYWIZARD_H
