#ifndef LISTARTICLES_H
#define LISTARTICLES_H

#include "articles.h"

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>


class ListArticles : public QObject
{
    Q_OBJECT
public:
    explicit ListArticles(int terminal_D, int shift, QObject *parent = nullptr);

signals:

public slots:

private:
    void createListGoods();
private:
    int m_terminalID;
    int m_shiftID;
    QVector<Articles> goods;

};

#endif // LISTARTICLES_H
