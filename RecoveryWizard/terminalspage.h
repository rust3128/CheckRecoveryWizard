#ifndef TERMINALSPAGE_H
#define TERMINALSPAGE_H

#include <QWizardPage>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

namespace Ui {
class TerminalsPage;
}

class TerminalsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TerminalsPage(QWidget *parent = 0);
    ~TerminalsPage();
signals:
    void sendInfo(QString, QString, bool);
    void signalSendCheckData(QString, QVariant);
protected:
    void initializePage();
    bool validatePage();
private slots:
    void on_lineEditTerminal_textChanged(const QString &term);
    void on_toolButtonSelectTerminal_clicked();
    void getSelectedTerminal(int termID);

private:
    Ui::TerminalsPage *ui;
    int idx;
    QSqlQueryModel *modelTerminals;
private:
    void createModelTerminals();


};

#endif // TERMINALSPAGE_H
