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
    }
    return m_shiftNumOK;
}

int ShiftsPage::nextId()
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

    ui->comboBoxPoss->setModel(modelPoss);
    ui->comboBoxPoss->setModelColumn(1);
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
    modelPoss = new QSqlQueryModel();
    modelPoss->setQuery(strSQL,dbcenter);
    if(modelPoss->rowCount()==0){
        qCritical(logCritical()) <<  QString("Class: %1 Metod: %2. Не возможно создать модель смен.")
                                             .arg(this->metaObject()->className())
                                             .arg(Q_FUNC_INFO);
        return;
    }
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
//            shiftDate = modelShifts->data(modelShifts->index(i,2)).toDate();
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
    connect(shDlg,&SelectShiftDialog::sendShiftID,this,&ShiftsPage::slotGetTerminals);
    shDlg->exec();
}

void ShiftsPage::slotGetTerminals(int terminalID)
{
    ui->lineEditShiftID->setText(QString::number(terminalID));
    ui->lineEditShiftID->setFocus();
}

