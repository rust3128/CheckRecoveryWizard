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

}

void Articles::setID(int _articleID)
{
    m_artileID = _articleID;
}

void Articles::setName(QString _name)
{
    m_name = _name;
}

void Articles::setShortName(QString _shortName)
{
    m_shortname = _shortName;
}

void Articles::setAmount(float _amount)
{
    m_amount = _amount;
}

void Articles::setPrice(float _price)
{
    m_price = _price;
}

int Articles::getID()
{
    return m_artileID;
}

QString Articles::getName()
{
    return m_name;
}

QString Articles::getShortName()
{
    return m_shortname;
}

float Articles::getAmount()
{
    return m_amount;
}
float Articles::getPrice()
{
    return m_price;
}

