#include "vektormodel.h"
#include "articles.h"

VektorModel::VektorModel(const QVector<Articles> vek)
{
    ar = vek;
}

QVariant VektorModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) { return QVariant(); }
        Articles a = ar[index.row()];
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return a.getID();
        case 1: return a.getShortName();
        case 2: return a.getAmount();
        case 3: return QString::number(a.getPrice(),'f',2);
        default: break;
        }
      break;


    case Qt::TextAlignmentRole:
        if(index.column() == 2 || index.column() == 3 )
            return Qt::AlignRight;
        break;
    default:
        break;
    }

    return QVariant();

}

QVariant VektorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case 0:
        return tr( "Гл.Код" );
    case 1:
        return tr( "Наименование" );
    case 2:
        return tr( "Доступно" );
    case 3:
        return tr( "Цена" );
    }

    return QVariant();
}

int VektorModel::rowCount(const QModelIndex &parent) const
{
    return ar.size();
}

int VektorModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}
