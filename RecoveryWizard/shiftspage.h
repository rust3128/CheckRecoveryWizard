#ifndef SHIFTSPAGE_H
#define SHIFTSPAGE_H

#include <QWizardPage>

namespace Ui {
class ShiftsPage;
}

class ShiftsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ShiftsPage(QWidget *parent = 0);
    ~ShiftsPage();

private:
    Ui::ShiftsPage *ui;
};

#endif // SHIFTSPAGE_H
