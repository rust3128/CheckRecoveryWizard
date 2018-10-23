#ifndef RECOVERYWIZARD_H
#define RECOVERYWIZARD_H

#include <QWizard>

namespace Ui {
class RecoveryWizard;
}

class RecoveryWizard : public QWizard
{
    Q_OBJECT

signals:
    void signalHideWiz();
private slots:
    void cancelWizard();
public:
    explicit RecoveryWizard(QWidget *parent = 0);
    ~RecoveryWizard();

private:
    Ui::RecoveryWizard *ui;
};

#endif // RECOVERYWIZARD_H
