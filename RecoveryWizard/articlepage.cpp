#include "articlepage.h"
#include "ui_articlepage.h"

ArticlePage::ArticlePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ArticlePage)
{
    ui->setupUi(this);
    ui->tableView->hide();
}

ArticlePage::~ArticlePage()
{
    delete ui;
}

void ArticlePage::initializePage()
{

//    QSqlDatabase dbcenter = QSqlDatabase::database("central");
//    modelArticles = new QSqlQueryModel();
//    QString strSQL = QString("SELECT SL.TERMINAL_ID, A.GARTICLE_ID, GA.NAME, GA_SHORTNAME, SL.AMOUNT, "
//                             "(SELECT FIRST 1 NEWPRICE FROM HISTORY_PRICES HP "
//                              "WHERE HP.TERMINAL_ID = SL.TERMINAL_ID "
//                              "AND HP.GARTICLE_ID = A.GARTICLE_ID "
//                              "AND HP.DATEOP < SH.DATCLOSE "
//                              "ORDER BY HP.DATEOP DESC) AS PRICE "
//                           "FROM GET_ASALDOS (%1, %2, NULL, 0) AS SL "
//                           "INNER JOIN ARTICLES A ON A.TERMINAL_ID = SL.TERMINAL_ID AND A.ARTICLE_ID = SL.ARTICLE_ID "
//                           "LEFT JOIN SHIFTS SH ON SH.TERMINAL_ID = SL.TERMINAL_ID AND SH.SHIFT_ID = SL.SHIFT_ID "
//                           "LEFT JOIN GARTICLES GA ON GA.GARTICLE_ID = A.GARTICLE_ID "
//                           "WHERE SL.AMOUNT > 0")
//            .arg(field("terminalID").toInt())
//            .arg(field("shiftID").toInt());

//    modelArticles->setQuery(strSQL,dbcenter);
//    ui->tableView->setModel(modelArticles);


}

bool ArticlePage::validatePage()
{
    return false;
}




