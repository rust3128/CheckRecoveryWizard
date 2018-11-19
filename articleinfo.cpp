#include "articleinfo.h"
#include <QDebug>

ArticleInfo::ArticleInfo()
{
    m_articleID=0;
    m_amount=0;
    m_price=0;
    m_discount=0;
}

void ArticleInfo::showMembers()
{
    qDebug() << "ArticleID" << m_articleID << endl
             << "Amount" << m_amount << endl
             << "Price" << m_price << endl
             << "Summ"  << getSumm() << endl
             << "Discount" << m_discount << endl;
}
