#ifndef ARTICLES_H
#define ARTICLES_H

#include <QString>

class Articles
{
public:
//    Articles(int _articleID, QString _name, QString _shortName, float _amount, float _price );
    Articles();
    void setID(int _articleID);
    void setName(QString _name);
    void setShortName(QString _shortName);
    void setAmount(float _amount);
    void setPrice(float _price);
    int getID();
    QString getName();
    QString getShortName();
    float getAmount();
    float getPrice();
private:
    int m_artileID;
    QString m_name;
    QString m_shortname;
    float m_amount;
    float m_price;
};

#endif // ARTICLES_H
