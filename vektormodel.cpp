#include "vektormodel.h"
#include "articles.h"

VektorModel::VektorModel(const QVector<Articles> vek)
{
    ar = vek;
}
//отображение данных модели
QVariant VektorModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) { return QVariant(); }
        Articles a = ar[index.row()];
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return a.getID();                           //1-й столбец ID
        case 1: return a.getShortName();                    //2-й столбец Наименование
        case 2: return a.getAmount();                       //3-й столбец Количетсво
        case 3: return QString::number(a.getPrice(),'f',2); //4-й столбец Цена
        default: break;
        }
      break;

    //выравниваем 2-й и 3-й столбез по правому краю
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
    //Cоздаем заголовки столбцов модели
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
    //Количетсво строк
    return ar.size();
}

int VektorModel::columnCount(const QModelIndex &parent) const
{
    //Количество столбцов
    Articles a;
    return a.getColParam();
}
