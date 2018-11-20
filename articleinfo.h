#ifndef ARTICLEINFO_H
#define ARTICLEINFO_H


class ArticleInfo
{
private:
    int m_articleID;        //Код товара
    float m_amount;         //Количество
    float m_price;          //Цена
    float m_summ;           //Сумма
    float m_discount;       //Скидка

public:
    ArticleInfo();
    void setAmount(float value) {m_amount = value;}
    void setPrice(float value) {m_price = value;}
    void setDiscount(float value) {m_discount = value;}
    void setArticleID(int value) {m_articleID = value;}

    float getAmount() {return m_amount;}
    float getPrice() {return m_price;}
    float getSumm() {return m_amount * m_price;}
    float getDiscount() {return m_discount;}
    int getArticleID() {return m_articleID;}
    void showMembers();
};

#endif // ARTICLEINFO_H
