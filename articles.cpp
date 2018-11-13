#include "articles.h"

//Articles::Articles(int _articleID =0, QString _name = "", QString _shortName ="", float _amount=0, float _price=0)
//{
//    m_artileID = _articleID;
//    m_name = _name;
//    m_shortname = _shortName;
//    m_amount = _amount;
//    m_price = _price;
//}
Articles::Articles()
{
    setColParam(4);
}

void Articles::setColParam(int _colParam)
{
    colParam = _colParam;
}

int Articles::getColParam()
{
    return colParam;
}

void Articles::setID(int _articleID)
{
    artileID = _articleID;
}

//void Articles::setName(QString _name)
//{
//    m_name = _name;
//}

void Articles::setShortName(QString _shortName)
{
    shortname = _shortName;
}

void Articles::setAmount(float _amount)
{
    amount = _amount;
}

void Articles::setPrice(float _price)
{
    price = _price;
}

int Articles::getID()
{
    return artileID;
}

//QString Articles::getName()
//{
//    return m_name;
//}

QString Articles::getShortName()
{
    return shortname;
}

float Articles::getAmount()
{
    return amount;
}
float Articles::getPrice()
{
    return price;
}

