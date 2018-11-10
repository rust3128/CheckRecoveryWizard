#include "listarticles.h"
#include "loggingcategories.h"

ListArticles::ListArticles(int terminal_D, int shift, QObject *parent) : QObject(parent)
{
    m_terminalID = terminal_D;
    m_shiftID = shift;
}

void ListArticles::createListGoods()
{
    Articles ar;
    QSqlDatabase dbth = QSqlDatabase::database("connThread");
    QSqlQuery *q = new QSqlQuery(dbth);
    if(!dbth.open()) {
        qCritical(logCritical()) << Q_FUNC_INFO << "Не могу подключится кцентральной базе"
                                 << endl << dbth.lastError().text();
        return;
    }


    QString strSQL = QString("SELECT A.GARTICLE_ID, GA.NAME, GA_SHORTNAME, SL.AMOUNT, "
                                 "(SELECT FIRST 1 NEWPRICE FROM HISTORY_PRICES HP "
                                  "WHERE HP.TERMINAL_ID = SL.TERMINAL_ID "
                                  "AND HP.GARTICLE_ID = A.GARTICLE_ID "
                                  "AND HP.DATEOP < SH.DATCLOSE "
                                  "ORDER BY HP.DATEOP DESC) AS PRICE "
                               "FROM GET_ASALDOS (%1, %2, NULL, 0) AS SL "
                               "INNER JOIN ARTICLES A ON A.TERMINAL_ID = SL.TERMINAL_ID AND A.ARTICLE_ID = SL.ARTICLE_ID "
                               "LEFT JOIN SHIFTS SH ON SH.TERMINAL_ID = SL.TERMINAL_ID AND SH.SHIFT_ID = SL.SHIFT_ID "
                               "LEFT JOIN GARTICLES GA ON GA.GARTICLE_ID = A.GARTICLE_ID "
                               "WHERE SL.AMOUNT > 0")
                .arg(m_terminalID)
                .arg(m_shiftID);
    q->exec(strSQL);
    while (q->next()){
        ar.setID(q->value("GARTICLE_ID").toInt());
        ar.setName(q->value("NAME").toString().trimmed());
        ar.setShortName(q->value("GA_SHORTNAME").toString().trimmed());
        ar.setAmount(q->value("AMOUNT").toFloat());
        ar.setPrice(q->value("PRICE").toFloat());
        goods.append(ar);
    }


}
