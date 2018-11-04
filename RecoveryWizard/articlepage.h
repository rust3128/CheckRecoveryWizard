#ifndef ARTICLEPAGE_H
#define ARTICLEPAGE_H

#include <QWizardPage>

namespace Ui {
class ArticlePage;
}

class ArticlePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ArticlePage(QWidget *parent = 0);
    ~ArticlePage();

private:
    Ui::ArticlePage *ui;
};

#endif // ARTICLEPAGE_H
