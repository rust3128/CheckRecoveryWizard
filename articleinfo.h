#ifndef ARTICLEINFO_H
#define ARTICLEINFO_H


class ArticleInfo
{
private:
    float m_amount;
    float m_price;
    float m_summ;
    float m_discount;

public:
    ArticleInfo();
    void setAmount(float value) {m_amount = value;}
    void setPrice(float value) {m_price = value;}
    void setDiscount(float value) {m_discount = value;}

    float getAmount() {return m_amount;}
    float getPrice() {return m_price;}
    float getSumm() {return m_amount * m_price;}
    float getDiscount() {return m_discount;}
};

#endif // ARTICLEINFO_H
