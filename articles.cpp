#include "articles.h"

Articles::Articles(int articleID, QString name, QString shortname, float amount, float price)
{
    m_artileID = articleID;
    m_name = name;
    m_shortname = shortname;
    m_amount = amount;
    m_price = price;
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
