#ifndef RECOVERYWIZARD_H
#define RECOVERYWIZARD_H

#include "connectionspage.h"
#include "terminalspage.h"
#include "shiftspage.h"
#include "fuelpage.h"
#include "finalpage.h"
#include "articlepage.h"
#include "ProgressExecuteDialog/progressexecutedialog.h"

#include <QWizard>
#include <QLineEdit>


namespace Ui {
class RecoveryWizard;
}

class RecoveryWizard : public QWizard
{
    Q_OBJECT

signals:
    void signalHideWiz();
    void signalSendCheckInfo(QString,QString,bool);
    void signalFinishWiz();



private slots:
    void cancelWizard();
    void slotGetPageData(QString name, QString info, bool isSpan);
    void slotSetLostCheckData(QString key, QVariant data);
    void slotViewSql();
    void slotExecuteSql();
    void slotStartExecute();
    void slotFinisExecute();
    void slotGenerateScript();
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
    ProgressExecuteDialog *progExecDlg;

    QHash<QString, QVariant> lostCheckFuel;
    QStringList script;
    QStringList endScript;
//    QLineEdit *m_lineEdit;

private:
    void initLostCheckFuel();

};

#endif // RECOVERYWIZARD_H
