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
private slots:
    void on_pushButtonView_clicked();

private:
    Ui::FinalPage *ui;
};

#endif // FINALPAGE_H
