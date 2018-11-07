#include "fuelpage.h"
#include "ui_fuelpage.h"
#include "loggingcategories.h"
#include "pagelist.h"



FuelPage::FuelPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::FuelPage)
{
    ui->setupUi(this);
    this->registerField("give*", ui->lineEditGive);
    this->registerField("fuelID*", ui->comboBoxFuels, "currentIndex", SIGNAL(activated(int)));
    this->registerField("trkID*", ui->comboBoxTRK, "currentIndex", SIGNAL(activated(int)));
    this->registerField("paytypeID*", ui->comboBoxPaytype, "currentIndex", SIGNAL(activated(int)));

}

FuelPage::~FuelPage()
{
    delete ui;
}

void FuelPage::initializePage()
{
    emit sendInfo("","Параметры топливного чека",true);

    createModelFuels();
    createModelPaytypes();
    modelTrk = new QSqlQueryModel();

    createUI();
}

int FuelPage::nextId() const
{

//    qInfo(logInfo()) << Q_FUNC_INFO  << "Select Fuel" << field("checkFuel").toBool();
    if(field("checkFuel").toBool()) {
        return FINAL_PAGE;
    } else {
        return ARTICLES_DATA_PAGE;
    }
}

bool FuelPage::validatePage()
{

    emit sendInfo("Топливо",modelFuels->data(modelFuels->index(field("fuelID").toInt(),2)).toString(),false);
    emit signalSendCheckData("FUEL_ID",modelFuels->data(modelFuels->index(field("fuelID").toInt(),1)).toInt());
    emit signalSendCheckData("TANK_ID",modelFuels->data(modelFuels->index(field("fuelID").toInt(),0)).toInt());

    emit sendInfo("Колонка",modelTrk->data(modelTrk->index(field("trkID").toInt(),0)).toString(),false);
    emit sendInfo("Пистолет",modelTrk->data(modelTrk->index(field("trkID").toInt(),1)).toString(),false);
    emit signalSendCheckData("DISPENSER_ID",modelTrk->data(modelTrk->index(field("trkID").toInt(),0)).toInt());
    emit signalSendCheckData("TRK_ID",modelTrk->data(modelTrk->index(field("trkID").toInt(),1)).toInt());

    emit sendInfo("Вид оплаты",modelPaytypes->data(modelPaytypes->index(field("paytypeID").toInt(),1)).toString(),false);
    emit signalSendCheckData("PAYTYPE_ID",modelPaytypes->data(modelPaytypes->index(field("paytypeID").toInt(),0)).toInt());

    if(showClients) {
        signalSendCheckData("INFO_CODE",ui->lineEditClientCode->text().toInt());
        signalSendCheckData("INFO_TEXT",ui->lineEditClientInfo->text().trimmed());
    }
    emit sendInfo("Получено", ui->lineEditGive->text().trimmed(),false);
    emit signalSendCheckData("GIVE",ui->lineEditGive->text().toDouble());

    emit sendInfo("Цена", ui->lineEditPrice->text().trimmed(),false);
    emit signalSendCheckData("PRICE", ui->lineEditPrice->text().toDouble());

    emit sendInfo("Сумма",ui->lineEditSum->text().trimmed(),false);
    emit signalSendCheckData("SUMMA",ui->lineEditSum->text().toDouble());

    emit sendInfo("Скидка",ui->lineEditDiscount->text().trimmed(),false);
    emit signalSendCheckData("DISCOUNTSUMMA",ui->lineEditDiscount->text().toDouble());

    return true;
}



void FuelPage::createUI()
{
    ui->comboBoxFuels->setModel(modelFuels);
    ui->comboBoxFuels->setModelColumn(2);
    ui->comboBoxFuels->setCurrentIndex(-1);
}

void FuelPage::createModelFuels()
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    modelFuels = new QSqlQueryModel();
    QString strSQL = QString("select t.TANK_ID, t.FUEL_ID, f.NAME, t.color from tanks t "
                         "LEFT JOIN fuels f ON f.FUEL_ID=t.FUEL_ID "
                         "where t.TERMINAL_ID=%1 and t.ISACTIVE='T' "
                         "order by t.FUEL_ID")
            .arg(field("terminalID").toInt());
    modelFuels->setQuery(strSQL,dbcenter);

}

void FuelPage::createModelTrk(int tankID)
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    modelTrk->clear();
    QString strSQL = QString("select k.DISPENSER_ID AS TRK, k.TRK_ID as KRAN from TRKS k "
                             "where k.TERMINAL_ID=%1 and k.TANK_ID=%2 and k.ISACTIVE='T' "
                             "order by k.DISPENSER_ID")
            .arg(field("terminalID").toInt())
            .arg(tankID);
    modelTrk->setQuery(strSQL,dbcenter);
    ui->comboBoxTRK->setModel(modelTrk);
    ui->comboBoxTRK->setModelColumn(0);
    ui->comboBoxTRK->setCurrentIndex(-1);
}

void FuelPage::createModelPaytypes()
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    modelPaytypes = new QSqlQueryModel();
    modelPaytypes->setQuery("SELECT p.PAYTYPE_ID, p.NAME, p.DLLNAME FROM PAYTYPES p WHERE p.PAYTYPE_ID>0 and p.ISACTIVE='T'",dbcenter);
    ui->comboBoxPaytype->setModel(modelPaytypes);
    ui->comboBoxPaytype->setModelColumn(1);
    ui->comboBoxPaytype->setCurrentIndex(-1);
    showClientsInfo(false);
}

void FuelPage::showClientsInfo(bool sh)
{
    if(!sh){
        ui->labelClient->hide();
        ui->labelInfo->hide();
        ui->lineEditClientCode->hide();
        ui->lineEditClientInfo->hide();
    } else {
        ui->labelClient->show();
        ui->labelInfo->show();
        ui->lineEditClientCode->show();
        ui->lineEditClientInfo->show();
    }
}

void FuelPage::setPrice(int tankID)
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    QSqlQuery *q = new QSqlQuery(dbcenter);
    q->prepare("SELECT t.price FROM TANKSALDOS t "
               "WHERE t.terminal_id=:terminalID and t.shift_id=:shiftID and t.tank_ID=:tankID");
    q->bindValue(":terminalID",field("terminalID").toInt());
    q->bindValue(":shiftID",field("shiftID").toInt());
    q->bindValue(":tankID",tankID);
    q->exec();
    q->next();
    ui->lineEditPrice->setStyleSheet("background-color: #aaff7f;");
    ui->lineEditPrice->setText(q->value(0).toString());
}


void FuelPage::on_comboBoxFuels_activated(int idx)
{
    int tankID = modelFuels->data(modelFuels->index(idx,0)).toInt();
    ui->lineEditTanks->setStyleSheet("background-color: #aaff7f;");
    ui->lineEditTanks->setText(QString::number(tankID));
    createModelTrk(tankID);
    setPrice(tankID);
}

void FuelPage::on_comboBoxTRK_activated(int idx)
{
    ui->lineEditKran->setStyleSheet("background-color: #aaff7f;");
    ui->lineEditKran->setText(modelTrk->data(modelTrk->index(idx,1)).toString());
}

void FuelPage::on_comboBoxPaytype_activated(int idx)
{
    QString dllName = modelPaytypes->data(modelPaytypes->index(idx,2)).toString();
    if(dllName.toLower() == "unipos" || dllName.toLower() == "clientsdb" ) {
        showClients =true;
    } else {
        showClients =false;
    }
    showClientsInfo(showClients);
}

void FuelPage::on_lineEditGive_textChanged(const QString &arg1)
{
    ui->lineEditSum->setStyleSheet("background-color: #aaff7f;");
    ui->lineEditSum->setText(QString::number(ui->lineEditPrice->text().toDouble()*arg1.toDouble(),'f',2));

}

void FuelPage::on_lineEditPrice_textChanged(const QString &arg1)
{
    ui->lineEditSum->setStyleSheet("background-color: #aaff7f;");
    ui->lineEditSum->setText(QString::number(ui->lineEditGive->text().toDouble()*arg1.toDouble(),'f',2));

}
