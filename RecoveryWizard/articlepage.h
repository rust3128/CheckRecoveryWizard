#ifndef ARTICLEPAGE_H
#define ARTICLEPAGE_H
#include "articles.h"
#include "vektormodel.h"
#include <QWizardPage>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include <QSqlRecord>


namespace Ui {
class ArticlePage;
}

class ArticlePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ArticlePage(QWidget *parent = 0);
    ~ArticlePage();
signals:

public slots:
    void slotGetConnRecord(QSqlRecord rec);

private slots:
    void slotGetArticlesList(QVector<Articles> ls);
    void slotStartArticlesList();
    void slotFinishArticlesList();

protected:
    void initializePage();
    bool validatePage();

//    int nextId() const;
private:
    Ui::ArticlePage *ui;
    Articles ar;
    QVector<Articles> goods;
    QSqlRecord recrodConn;
    VektorModel *modelArticles;
};

#endif // ARTICLEPAGE_H
