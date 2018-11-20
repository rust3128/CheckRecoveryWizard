#include "articles.h"

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

