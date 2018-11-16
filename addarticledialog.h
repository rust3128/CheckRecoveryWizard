#ifndef ADDARTICLEDIALOG_H
#define ADDARTICLEDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include "articles.h"
#include "articleinfo.h"

namespace Ui {
class AddArticleDialog;
}

class AddArticleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddArticleDialog(Articles ar, QWidget *parent = 0);
    ~AddArticleDialog();
    ArticleInfo getGoodsData();

private slots:
    void on_spinBoxAmount_valueChanged(double amount);
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_lineEditDiscount_textChanged(const QString &discont);

private:
    Ui::AddArticleDialog *ui;
    Articles m_articles;
//    QStringList m_goodsData;
    ArticleInfo arInfo;
};

#endif // ADDARTICLEDIALOG_H
