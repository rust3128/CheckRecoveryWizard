#ifndef RECOVERYWIZARD_H
#define RECOVERYWIZARD_H

#include "connectionspage.h"
#include "terminalspage.h"
#include "shiftspage.h"
#include "fuelpage.h"
#include "finalpage.h"
#include "articlepage.h"

#include <QWizard>

namespace Ui {
class RecoveryWizard;
}

class RecoveryWizard : public QWizard
{
    Q_OBJECT

signals:
    void signalHideWiz();
    void signalSendCheckInfo(QString,QString,bool);

private slots:
    void cancelWizard();
    void slotGetPageData(QString name, QString info, bool isSpan);
    void slotSetLostCheckData(QString key, QVariant data);
    void slotViewSql();
public:
    explicit RecoveryWizard(QWidget *parent = 0);
    ~RecoveryWizard();

private:
    Ui::RecoveryWizard *ui;
    TerminalsPage *termPage;
    ConnectionsPage *connPage;
    ShiftsPage *shiftPage;
    FuelPage *fuelPage;
    FinalPage *finalPage;
    ArticlePage *articlePage;

    QHash<QString, QVariant> lostCheckFuel;
    QStringList script;
//    QStringList endScript;

private:
    void initLostCheckFuel();
    void generateScript();
};

#endif // RECOVERYWIZARD_H
