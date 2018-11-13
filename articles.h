#ifndef ARTICLES_H
#define ARTICLES_H

#include <QString>

class Articles
{
public:
//    Articles(int _articleID, QString _name, QString _shortName, float _amount, float _price );
    Articles();
    void setColParam(int _colParam);
    void setID(int _articleID);
//    void setName(QString _name);
    void setShortName(QString _shortName);
    void setAmount(float _amount);
    void setPrice(float _price);
    int getColParam();
    int getID();
//    QString getName();
    QString getShortName();
    float getAmount();
    float getPrice();
private:
    int colParam;
    int artileID;
//    QString m_name;
    QString shortname;
    float amount;
    float price;
};

#endif // ARTICLES_H
