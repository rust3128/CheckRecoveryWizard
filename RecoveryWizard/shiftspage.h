#ifndef SHIFTSPAGE_H
#define SHIFTSPAGE_H

#include <QWizardPage>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class ShiftsPage;
}

class ShiftsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ShiftsPage(QWidget *parent = 0);
    ~ShiftsPage();

protected:
    void initializePage();
    bool validatePage();
    int nextId();
private slots:
    void on_lineEditShiftID_textChanged(const QString &arg1);
    void on_toolButton_clicked();
    void slotGetTerminals(int terminalID);

private:
    Ui::ShiftsPage *ui;
    QSqlQueryModel *modelShifts;
    bool m_shiftNumOK;
private:
    void createUI();
    void createModelShifts();
};

#endif // SHIFTSPAGE_H
