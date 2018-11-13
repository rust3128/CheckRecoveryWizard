#ifndef ADDARTICLEDIALOG_H
#define ADDARTICLEDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "articles.h"

namespace Ui {
class AddArticleDialog;
}

class AddArticleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddArticleDialog(Articles ar, QWidget *parent = 0);
    ~AddArticleDialog();
    QStringList getGoodsData();

private slots:
    void on_spinBoxAmount_valueChanged(double amount);
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::AddArticleDialog *ui;
    Articles m_articles;
    QStringList m_goodsData;
};

#endif // ADDARTICLEDIALOG_H
