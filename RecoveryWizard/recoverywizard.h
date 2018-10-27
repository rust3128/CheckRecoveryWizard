#ifndef RECOVERYWIZARD_H
#define RECOVERYWIZARD_H

#include "connectionspage.h"
#include "terminalspage.h"
#include "shiftspage.h"
#include "fuelpage.h"
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
    void on_RecoveryWizard_currentIdChanged();
    void slotGetPageData(int row, QString info);
    void slotSetLostCheckData(QString key, QVariant data);
public:
    explicit RecoveryWizard(QWidget *parent = 0);
    ~RecoveryWizard();

private:
    Ui::RecoveryWizard *ui;
    TerminalsPage *termPage;
    ConnectionsPage *connPage;
    ShiftsPage *shifPage;
    FuelPage *fuelPage;
    int infoRow;
    QString infoText;
    QHash<QString, QVariant> lostCheckFuel;
private:
    void initLostCheckFuel();
};

#endif // RECOVERYWIZARD_H
