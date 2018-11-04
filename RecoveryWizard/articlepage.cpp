#include "articlepage.h"
#include "ui_articlepage.h"

ArticlePage::ArticlePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ArticlePage)
{
    ui->setupUi(this);
}

ArticlePage::~ArticlePage()
{
    delete ui;
}
