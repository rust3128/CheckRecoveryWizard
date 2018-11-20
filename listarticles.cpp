#include "listarticles.h"
#include "loggingcategories.h"

ListArticles::ListArticles(QSqlRecord rec, int terminal_D, int shift, QObject *parent) : QObject(parent)
{
    //Получаем входные данные
    m_terminalID = terminal_D;
    m_shiftID = shift;
    connRec =rec;
}

void ListArticles::createListGoods()
{
    //Создаем объект для хранения одной записи
    Articles ar;

    //регистрируем тип для передачи его через механизм сигнал слот.
    typedef QVector<Articles> vek;
    qRegisterMetaType<vek>("vektor");

    //Из Асистента
    //"Соединение может использоваться только внутри создавшего его потока. Перемещение соединений между потоками и создание запросов в другой поток не поддерживается.
    //Поэтому создаем новое соединение

    QSqlDatabase dbth = QSqlDatabase::addDatabase("QIBASE","thcentr");

    dbth.setHostName(connRec.value("conn_host").toString());
    dbth.setDatabaseName(connRec.value("conn_db").toString());
    dbth.setUserName(connRec.value("conn_user").toString());
    dbth.setPassword(connRec.value("conn_pass").toString());


    if(!dbth.open()) {
        qCritical(logCritical()) << Q_FUNC_INFO << "Не могу подключится к центральной базе"
                                 << endl << dbth.lastError().text();
        return;
    }
    //Привязыем запрос к соединению
    QSqlQuery q = QSqlQuery(dbth);

    //Формируем строку с запросом.
    //Такого типа запросы предпочитаю создавать ввиде строки и тестировать их в непосредственно в менеджере базы данных

    QString strSQL = QString("SELECT A.GARTICLE_ID, GA.SHORTNAME, SL.AMOUNT, "
                                 "(SELECT FIRST 1 NEWPRICE FROM HISTORY_PRICES HP "
                                  "WHERE HP.TERMINAL_ID = SL.TERMINAL_ID "
                                  "AND HP.GARTICLE_ID = A.GARTICLE_ID "
                                  "AND HP.DATEOP < SH.DATCLOSE "
                                  "ORDER BY HP.DATEOP DESC) AS PRICE "
                               "FROM GET_ASALDOS (%1, %2, NULL, 0) AS SL "
                               "INNER JOIN ARTICLES A ON A.TERMINAL_ID = SL.TERMINAL_ID AND A.ARTICLE_ID = SL.ARTICLE_ID "
                               "LEFT JOIN SHIFTS SH ON SH.TERMINAL_ID = SL.TERMINAL_ID AND SH.SHIFT_ID = SL.SHIFT_ID "
                               "LEFT JOIN GARTICLES GA ON GA.GARTICLE_ID = A.GARTICLE_ID "
                               "WHERE SL.AMOUNT > 0 "
                               "ORDER BY A.GARTICLE_ID" )
                .arg(m_terminalID)
                .arg(m_shiftID);
    //Выполняем запрос
    if(!q.exec(strSQL)) {
        qInfo(logInfo()) << "Errog goodlist" << q.lastError().text();
        emit finish();
    }
    //Цикл получения записей, и добавления их в вектор
    while (q.next()){
        ar.setID(q.value("GARTICLE_ID").toInt());
        ar.setShortName(q.value("SHORTNAME").toString().trimmed());
        ar.setAmount(q.value("AMOUNT").toFloat());
        ar.setPrice(q.value("PRICE").toFloat());
        goods.append(ar);
    }
    //Передаем результат в основной поток
    emit signalSendArticlesList(goods);
    //Поток закончил работу
    emit finish();
}
