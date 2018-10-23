#ifndef TERMINALSPAGE_H
#define TERMINALSPAGE_H

#include <QWizardPage>

namespace Ui {
class TerminalsPage;
}

class TerminalsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TerminalsPage(QWidget *parent = 0);
    ~TerminalsPage();

private:
    Ui::TerminalsPage *ui;
};

#endif // TERMINALSPAGE_H
