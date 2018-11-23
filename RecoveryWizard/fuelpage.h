#ifndef FUELPAGE_H
#define FUELPAGE_H

#include <QWizardPage>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class FuelPage;
}

class FuelPage : public QWizardPage
{
    Q_OBJECT


public:
    explicit FuelPage(QWidget *parent = 0);
    ~FuelPage();

signals:
    void sendInfo(QString, QString, bool);
    void signalSendCheckData(QString, QVariant);

protected:
    void initializePage();
    bool validatePage();
    int nextId() const;
private slots:
    void on_comboBoxFuels_activated(int idx);
    void on_comboBoxTRK_activated(int idx);
    void on_comboBoxPaytype_activated(int idx);
    void on_lineEditGive_textChanged(const QString &arg1);
    void on_lineEditPrice_textChanged(const QString &arg1);

private:
    Ui::FuelPage *ui;
    QSqlQueryModel *modelFuels;
    QSqlQueryModel *modelTrk;
    QSqlQueryModel *modelPaytypes;
    bool showClients;

private:
    void createUI();
    void createModelFuels();
    void createModelTrk(int tankID);
    void createModelPaytypes();
    void showClientsInfo(bool sh);
    void setPrice(int tankID);

};

#endif // FUELPAGE_H
