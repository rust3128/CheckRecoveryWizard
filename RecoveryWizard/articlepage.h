#ifndef ARTICLEPAGE_H
#define ARTICLEPAGE_H
#include "articles.h"
#include "articleinfo.h"
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
    void signalSetCommonData();
public slots:
    void slotGetConnRecord(QSqlRecord rec);

private slots:
    void slotGetArticlesList(QVector<Articles> ls);
    void slotStartArticlesList();
    void slotFinishArticlesList();

    void on_pushButtonAdd_clicked();

    void on_tableView_doubleClicked(const QModelIndex &idx);

    void on_pushButtonDelete_clicked();



//    int nextId() const;
private:
    Ui::ArticlePage *ui;
    Articles ar;
    QVector<Articles> goods;
    QSqlRecord recrodConn;
    ArticleInfo arInfo;
    VektorModel *modelArticles;
    float summArticles=0;
    float discountArticles=0;
private:
    void createUI();

    // QWizardPage interface
public:
    int nextId() const;
    void initializePage();
    bool validatePage();
};

#endif // ARTICLEPAGE_H
