#include "shiftspage.h"
#include "ui_shiftspage.h"
#include "loggingcategories.h"
#include "pagelist.h"
#include "SelectShiftDialog/selectshiftdialog.h"

ShiftsPage::ShiftsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ShiftsPage)
{
    ui->setupUi(this);
    this->registerField("shiftID*",ui->lineEditShiftID);
    this->registerField("numCheck*", ui->lineEditNumCheck);
    this->registerField("checkFuel", ui->radioButtonFuel);
    this->registerField("checkFuelArticles",ui->radioButtonFuelArticles);
    this->registerField("checkArticles",ui->radioButtonArticles);

}

ShiftsPage::~ShiftsPage()
{
    delete ui;
}

void ShiftsPage::initializePage()
{
    createModelShifts();
    createModelPoss();
    createUI();
}


bool ShiftsPage::validatePage()
{

    QString strSQL;
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    QSqlQuery *q = new QSqlQuery(dbcenter);
    if(ui->radioButtonFuel->isChecked() || ui->radioButtonFuelArticles->isChecked()){
        strSQL=QString("SELECT COUNT(*) FROM saleorders s "
                       "WHERE s.terminal_id=%1 AND s.shift_id=%2 AND s.num_check=%3")
                .arg(field("terminalID").toInt())
                .arg(field("shiftID").toInt())
                .arg(field("numCheck").toInt());
    } else {
        strSQL=QString("SELECT COUNT(*) FROM asales s "
                       "WHERE s.terminal_id=%1 AND s.shift_id=%2 AND s.numbercheck=%3")
                .arg(field("terminalID").toInt())
                .arg(field("shiftID").toInt())
                .arg(field("numCheck").toInt());
    }

    if(!q->exec(strSQL)) {
        qCritical(logCritical()) <<  QString("Class: %1 Metod: %2. Не возможно получить информации о наличии чека.")
                                             .arg(this->metaObject()->className())
                                             .arg(Q_FUNC_INFO);
        ui->labelDublicateNumCheck->setText("Не возможно получить информации о наличии чека.");
        return false;
    }
    q->next();
    if(q->value(0) != 0){
        ui->labelDublicateNumCheck->setText(QString("Чек с № %1 уже есть в базе данных АЗС.")
                                            .arg(ui->lineEditNumCheck->text()));
        return false;
    } else {
        ui->labelDublicateNumCheck->clear();
    }
    if(possID<0){
        ui->labelPossInfo->setText("Не выбран номер кассы.");
        return false;
    }

    if(ui->radioButtonFuel->isChecked()){
        this->setField("checkFuel",true);
    } else {
        this->setField("checkFuel",false);
    }

    if(ui->radioButtonFuelArticles->isChecked()){
        this->setField("chekFuelArticles",true);
    } else {
        this->setField("chekFuelArticles",false);
    }

    if(ui->radioButtonArticles->isChecked()){
        this->setField("checkArticles",true);
    } else {
        this->setField("checkArticles",false);
    }

    qInfo(logInfo()) << "Select Fuel?" << field("checkFuel").toBool();

    setField("shiftID",ui->lineEditShiftID->text().toInt());
    emit sendInfo(2,ui->lineEditShiftID->text().trimmed());
    emit signalSendCheckData("SHIFT_ID",ui->lineEditShiftID->text().toInt());

    emit sendInfo(3,QString::number(possID));
    emit signalSendCheckData("POS_ID",possID);

    emit sendInfo(4,ui->dateTimeEdit->text());
    emit signalSendCheckData("DAT",ui->dateTimeEdit->dateTime().toString("yyyy/MM/dd hh:mm:ss"));

    emit sendInfo(5,ui->lineEditNumCheck->text());
    emit signalSendCheckData("NUM_CHECK",ui->lineEditNumCheck->text().toInt());

    return m_shiftNumOK;
}

int ShiftsPage::nextId() const
{
    if(ui->radioButtonFuel->isChecked() || ui->radioButtonFuelArticles->isChecked()){
        return FUELDATA_PAGE;
    } else {
        return ARTICLES_DATA_PAGE;
    }

}

void ShiftsPage::createUI()
{
    ui->lineEditShiftID->setValidator(new QIntValidator(0,99999,ui->lineEditShiftID));
    ui->lineEditNumCheck->setValidator(new QIntValidator(0,999999999,ui->lineEditNumCheck));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->labelShiftData->clear();
    ui->labelDublicateNumCheck->clear();
    ui->labelPossInfo->clear();

    possID=-1;
    ui->comboBoxPoss->setModel(this->modelPoss);
    ui->comboBoxPoss->setModelColumn(1);
    ui->comboBoxPoss->setCurrentIndex(possID);

}

void ShiftsPage::createModelShifts()
{
    QString strSQL = QString("select s.SHIFT_ID, s.ZNUMBER, s.DATOPEN, s.DATCLOSE, s.OPERATOR_ID  from SHIFTS s "
                             "where s.TERMINAL_ID=%1 "
                             "order by s.SHIFT_ID DESC")
            .arg(field("terminalID").toInt());
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    modelShifts = new QSqlQueryModel();
    modelShifts->setQuery(strSQL,dbcenter);
    if(modelShifts->rowCount()==0){
        qCritical(logCritical()) <<  QString("Class: %1 Metod: %2. Не возможно создать модель смен.")
                                             .arg(this->metaObject()->className())
                                             .arg(Q_FUNC_INFO);
        return;
    }
    while(modelShifts->canFetchMore())
        modelShifts->fetchMore();
}

void ShiftsPage::createModelPoss()
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    QString strSQL = QString("SELECT p.pos_id, p.name FROM poss p where p.terminal_id=%1")
            .arg(field("terminalID").toInt());
    this->modelPoss = new QSqlQueryModel(this);
    this->modelPoss->setQuery(strSQL,dbcenter);
    if(modelPoss->rowCount()==0){
        qCritical(logCritical()) <<  QString("Class: %1 Metod: %2. Не возможно создать модель смен.")
                                             .arg(this->metaObject()->className())
                                             .arg(Q_FUNC_INFO);

    }
    qInfo(logInfo()) << "Индекс при создали моделей касс "<< ui->comboBoxPoss->currentIndex();
}



void ShiftsPage::on_lineEditShiftID_textChanged(const QString &arg1)
{
    if(ui->lineEditShiftID->text().isEmpty()){
        ui->labelShiftData->clear();
        return;
    }
    int shift = arg1.toInt();
    for(int i =0; i<modelShifts->rowCount();++i){
        if(shift == modelShifts->data(modelShifts->index(i,0)).toInt()){
            ui->labelShiftData->setText("Смена № "+modelShifts->data(modelShifts->index(i,0)).toString() +
                    "\nНачало смены: "+modelShifts->data(modelShifts->index(i,2)).toDateTime().toString("dd.MM.yyyy hh.mm") +
                    "\nZ-отчет № "+modelShifts->data(modelShifts->index(i,1)).toString());
            ui->dateTimeEdit->setDate(modelShifts->data(modelShifts->index(i,2)).toDate());
            m_shiftNumOK=true;
            return;
        }
    }
    ui->labelShiftData->setText("Не верный номер смены.");
    m_shiftNumOK=false;
}

void ShiftsPage::on_toolButton_clicked()
{
    SelectShiftDialog *shDlg = new SelectShiftDialog(modelShifts);
    connect(shDlg,&SelectShiftDialog::sendShiftID,this,&ShiftsPage::slotGetShifts);
    shDlg->exec();
}

void ShiftsPage::slotGetShifts(int shiftID)
{
    ui->lineEditShiftID->setText(QString::number(shiftID));
    ui->lineEditShiftID->setFocus();
}


void ShiftsPage::on_comboBoxPoss_activated(int idx)
{
    QModelIndex indexModel=modelPoss->index(idx,0,QModelIndex());
    possID = modelPoss->data(indexModel, Qt::DisplayRole).toInt();
    ui->labelPossInfo->clear();
}
