#ifndef DATABASE_H
#define DATABASE_H

#include "loggingcategories.h"

#include <QFile>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>

#define DATABASE_NAME "./CheckRecovery.opt"
#define DATABASE_HOSTNAME "OptionsHost"
#define DATABASE_SQL "./SQL//database.sql"


static bool createDatabase()
{
    bool result = false;
    qInfo(logInfo()) << "Создаем структуру таблиц базы данных.";
    QString allFile;

    QFile file(DATABASE_SQL);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qInfo(logInfo()) << "SQL файл успешно открыт.";
        QTextStream in(&file);
        allFile = in.readAll();
        QStringList temp = allFile.split(";");
        for(int i=0;i<temp.size();++i){
            qInfo(logInfo()) << temp.at(i);
        }

        result = true;
    } else {
        qInfo(logInfo()) << "Не возможно открыть SQL файл.";
        result = false;
    }

    return result;
}

static bool openDatabase()
{
    bool result = false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","options");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);

    if(db.open()){
        qInfo(logInfo()) << "Файл настроек открыт успешно";
        result = true;
    } else {
        qCritical(logCritical()) << "Не удалось открыть файл настроек приложения.";
        result = false;
    }

    return result;
}

static bool createOptions()
{
    bool result = false;

    if(openDatabase()) {
        if(createDatabase()){
            result = true;
        } else {
            result = false;
        }
    } else {
        qCritical(logCritical()) << "Не удалось воссановить базу данных приложения.";
        result = false;
    }
    return result;
}


static bool connectOptions()
{
    bool result;
    if(QFile(DATABASE_NAME).exists()){
        qInfo(logInfo()) << "Открываем файл настроек приложения.";
        result = openDatabase();
    } else {
        qInfo(logInfo()) << "Отсутсвует файл настроек приложения";
        result = createOptions();
    }

    return result;
}



#endif // DATABASE_H
