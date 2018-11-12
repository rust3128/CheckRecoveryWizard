#include "vektormodel.h"
#include "articles.h"

VektorModel::VektorModel(const QVector<Articles> vek)
{
    ar = vek;
}

QVariant VektorModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) { return QVariant(); }
    if ( role == Qt::DisplayRole)  {

        Articles a = ar[index.row()];

        switch (index.column()) {
        case 0: return a.getID();
        case 1: return a.getName();
        case 2: return a.getShortName();
        case 3: return a.getAmount();
        case 4: return a.getPrice();
        default: break;
        }

    }
    else
    {
        return QVariant();
    }
}

int VektorModel::rowCount(const QModelIndex &parent) const
{
    return ar.size();
}

int VektorModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}
