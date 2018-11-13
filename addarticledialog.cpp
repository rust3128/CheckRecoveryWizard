#include "addarticledialog.h"
#include "ui_addarticledialog.h"


AddArticleDialog::AddArticleDialog(Articles ar, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArticleDialog)
{
    ui->setupUi(this);
    m_articles = ar;
    ui->labelArticleInfo->setText(m_articles.getShortName());
    ui->lineEditPrice->setText(QString::number(m_articles.getPrice(),'f',2));
    ui->lineEditSumm->setText(QString::number(ui->spinBoxAmount->value() * ui->lineEditPrice->text().toFloat(),'f',2));
}

AddArticleDialog::~AddArticleDialog()
{
    delete ui;
}

QStringList AddArticleDialog::getGoodsData()
{
    return m_goodsData;
}

void AddArticleDialog::on_spinBoxAmount_valueChanged(double amount)
{
    ui->lineEditSumm->setText(QString::number(amount * ui->lineEditPrice->text().toFloat(),'f',2));
}

void AddArticleDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Save:
        m_goodsData << ui->spinBoxAmount->text()
                    << ui->lineEditPrice->text()
                    << ui->lineEditSumm->text();
        this->accept();
        break;
    case QDialogButtonBox::Close:
        this->reject();
        break;
    default:
        break;
    }
}
