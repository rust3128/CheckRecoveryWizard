#ifndef ARTICLEPAGE_H
#define ARTICLEPAGE_H

#include <QWizardPage>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>

namespace Ui {
class ArticlePage;
}

class ArticlePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ArticlePage(QWidget *parent = 0);
    ~ArticlePage();
protected:
    void initializePage();
    bool validatePage();

//    int nextId() const;
private:
    Ui::ArticlePage *ui;
    QSqlQueryModel *modelArticles;
};

#endif // ARTICLEPAGE_H
