#ifndef VEKTORMODEL_H
#define VEKTORMODEL_H

#include "articles.h"
#include <QObject>
#include <QAbstractTableModel>
#include <QVariant>



class VektorModel : public QAbstractTableModel
{
    Q_OBJECT
    QVector<Articles>  ar;
public:
    VektorModel(const QVector<Articles> vek);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
};

#endif // VEKTORMODEL_H
