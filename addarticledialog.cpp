#include "addarticledialog.h"
#include "ui_addarticledialog.h"
#include "loggingcategories.h"
#include <QToolTip>


AddArticleDialog::AddArticleDialog(Articles ar, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArticleDialog)
{
    ui->setupUi(this);
    m_articles = ar;
    ui->labelArticleInfo->setText(m_articles.getShortName());
    ui->lineEditPrice->setText(QString::number(m_articles.getPrice(),'f',2));
    ui->lineEditSumm->setText(QString::number(ui->spinBoxAmount->value() * ui->lineEditPrice->text().toFloat(),'f',2));

    // Валидатор чисел с плавающей запятой от 5 до 15 с 2 знаками после запятой
//    QDoubleValidator* validator =  new QDoubleValidator( 0.00, 99999999.0, 2 );
//    validator->setNotation( QDoubleValidator::StandardNotation );
//    ui->lineEditDiscount->setValidator(validator);
//    ui->lineEditPrice->setValidator(validator);
//    ui->lineEditSumm->setValidator(validator);
    ui->spinBoxAmount->setValue(0.00);
    ui->lineEditDiscount->setText(QString::number(0,'f',2));
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
}

AddArticleDialog::~AddArticleDialog()
{
    delete ui;
}

ArticleInfo AddArticleDialog::getGoodsData()
{
    return arInfo;
}

void AddArticleDialog::on_spinBoxAmount_valueChanged(double amount)
{
    ui->lineEditSumm->setText(QString::number(amount * ui->lineEditPrice->text().toFloat(),'f',2));
    (amount>0) ? ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true) :
                 ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
}

void AddArticleDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Save:
        arInfo.setAmount(ui->spinBoxAmount->value());
        arInfo.setPrice(ui->lineEditPrice->text().toFloat());
        arInfo.setDiscount(ui->lineEditDiscount->text().toFloat());
        this->accept();
        break;
    case QDialogButtonBox::Close:
        this->reject();
        break;
    default:
        break;
    }
}

void AddArticleDialog::on_lineEditDiscount_textChanged(const QString &discont)
{
//    ui->lineEditDiscount->setToolTip("Только положительное значение.");

    if(discont.toFloat()<0) {
        QToolTip::showText(ui->lineEditDiscount->mapToGlobal(QPoint()), ui->lineEditDiscount->toolTip());
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
    }

//    QToolTip::showText(ui->lineEditDiscount->mapToGlobal(QPoint()), ui->lineEditDiscount->toolTip());
}
