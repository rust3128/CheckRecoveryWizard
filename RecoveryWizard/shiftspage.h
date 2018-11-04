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
signals:
    void sendInfo(int, QString);
    void signalSendCheckData(QString, QVariant);
protected:
    void initializePage();
    bool validatePage();
    int nextId() const;
private slots:
    void on_lineEditShiftID_textChanged(const QString &arg1);
    void on_toolButton_clicked();
    void slotGetShifts(int shiftID);
    void on_comboBoxPoss_activated(int idx);

//    void on_comboBoxPoss_currentIndexChanged(int idx);

private:
    Ui::ShiftsPage *ui;
    QSqlQueryModel *modelShifts;
    QSqlQueryModel *modelPoss;
    bool m_shiftNumOK;
    int possID;
private:
    void createUI();
    void createModelShifts();
    void createModelPoss();

};

#endif // SHIFTSPAGE_H
