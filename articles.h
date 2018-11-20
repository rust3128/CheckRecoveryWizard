#ifndef ARTICLES_H
#define ARTICLES_H

#include <QString>

class Articles
{
public:
    Articles();
    void setColParam(int _colParam);
    void setID(int _articleID);
    void setShortName(QString _shortName);
    void setAmount(float _amount);
    void setPrice(float _price);
    int getColParam();
    int getID();
    QString getShortName();
    float getAmount();
    float getPrice();
private:
    int colParam;           //Количетсво параметров. необходимое для последующего создания модели
    int artileID;           //Код товара
    QString shortname;      //Наименование
    float amount;           //Количество
    float price;            //Цена
};

#endif // ARTICLES_H
