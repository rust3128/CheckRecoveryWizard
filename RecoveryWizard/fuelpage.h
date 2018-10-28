#ifndef FUELPAGE_H
#define FUELPAGE_H

#include <QWizardPage>
#include <QVariant>

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
    void sendInfo(int, QString);
    void signalSendCheckData(QString, QVariant);

protected:
    void initializePage();
private:
    Ui::FuelPage *ui;
private:
    void sendDataTo();
};

#endif // FUELPAGE_H
