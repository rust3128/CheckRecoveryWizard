#ifndef FUELPAGE_H
#define FUELPAGE_H

#include <QWizardPage>

namespace Ui {
class FuelPage;
}

class FuelPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit FuelPage(QWidget *parent = 0);
    ~FuelPage();

private:
    Ui::FuelPage *ui;
};

#endif // FUELPAGE_H
