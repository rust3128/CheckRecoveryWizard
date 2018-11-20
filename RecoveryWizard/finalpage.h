#ifndef FINALPAGE_H
#define FINALPAGE_H

#include <QWizardPage>

namespace Ui {
class FinalPage;
}

class FinalPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit FinalPage(QWidget *parent = 0);
    ~FinalPage();
protected:
    bool validatePage();
signals:
    void signalViewSql();
    void signalExecScript();
    void signalGenerateScript(int);
    void signalSaveScript();
private slots:

private:
    Ui::FinalPage *ui;
};

#endif // FINALPAGE_H
