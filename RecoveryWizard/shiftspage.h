#ifndef SHIFTSPAGE_H
#define SHIFTSPAGE_H

#include <QWizardPage>
#include <QSqlQueryModel>
#include <QSqlError>

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
private slots:
    void on_lineEditShiftID_textChanged(const QString &arg1);
    void on_toolButton_clicked();
    void slotGetTerminals(int terminalID);

private:
    Ui::ShiftsPage *ui;
    QSqlQueryModel *modelShifts;
private:
    void createUI();
    void createModelShifts();
};

#endif // SHIFTSPAGE_H
