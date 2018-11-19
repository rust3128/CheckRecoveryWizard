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
#include <QSqlRecord>


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
    void signalSendConnRec(QSqlRecord);



private slots:
    void cancelWizard();
    void slotGetPageData(QString name, QString info, bool isSpan);
    void slotSetLostCheckData(QString key, QVariant data);
    void slotViewSql();
    void slotExecuteSql();
    void slotStartExecute();
    void slotFinisExecute();
    void slotGenerateScript();
    void slotGetConnRecord(QSqlRecord rec);
    void slotSetCommonData();               //Заполняем повторяющиеся данные
    void slotGetArticlesData(ArticleInfo info);


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
    QHash<QString, QVariant> lostCheckArticle;
    QHash<QString, QVariant> mposCheck;
    QStringList script;

    QVector<QHash<QString,QVariant>> append_ASale;


    QStringList endScript;
    QStringList beginScript;
    QSqlRecord recConnect;

private:
    void initLostCheckFuel();
    void initLostCheckArticles();
    void initMposCheck();

};

#endif // RECOVERYWIZARD_H
