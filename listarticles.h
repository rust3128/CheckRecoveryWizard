#ifndef LISTARTICLES_H
#define LISTARTICLES_H

#include "articles.h"

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QSqlRecord>


class ListArticles : public QObject
{
    Q_OBJECT
public:
    explicit ListArticles(QSqlRecord rec, int terminal_D, int shift, QObject *parent = nullptr);

signals:
    void signalSendArticlesList(QVector<Articles>);
    void finish();

public slots:
    void createListGoods();
private:

private:
    int m_terminalID;
    int m_shiftID;
    QVector<Articles> goods;
    QSqlRecord connRec;

};

#endif // LISTARTICLES_H
