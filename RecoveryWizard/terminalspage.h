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
    void sendInfo(int, QString);
protected:
    void initializePage();
private:
    Ui::TerminalsPage *ui;
};

#endif // TERMINALSPAGE_H
