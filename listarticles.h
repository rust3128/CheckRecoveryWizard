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
    // Сигнал дя отправки данных в основной поток для дальнейшей их опбработки
    void signalSendArticlesList(QVector<Articles>);
    //Сигнал об окончании выборки данных
    void finish();

public slots:
    // Слот для начала выбоки данных, вызываемый из основного потока
    void createListGoods();

private:
    int m_terminalID;               //Номер магазина, параметр для запроса
    int m_shiftID;                  //Номер смены, параметр для запроса
    QVector<Articles> goods;        //Вектор в котором будем хранить результаты запроса и передадим в основной поток
    QSqlRecord connRec;             //Запись выбранная из другой модели содержащщая параметры подключения к базе данных

};

#endif // LISTARTICLES_H
