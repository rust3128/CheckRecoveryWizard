#include "mainwindow.h"
#include "loggingcategories.h"
#include "database.h"
#include <QApplication>
#include <QFile>
#include <QLibraryInfo>
#include <QTranslator>


// Умный указатель на файл логирования
QScopedPointer<QFile>   m_logFile;

// Объявляение обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Устанавливаем файл логирования,
    // внимательно сверьтесь с тем, какой используете путь для файла
    m_logFile.reset(new QFile("CheckRecovery.log"));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);

    QTranslator *qt_translator = new QTranslator();
    if(qt_translator->load(":/translate/qtbase_ru.qm"))
        a.installTranslator(qt_translator);

//#ifndef QT_NO_TRANSLATION
//    QString translatorFileName = QLatin1String("qt_");
//    translatorFileName += QLocale::system().name();
//    QTranslator *translator = new QTranslator(&a);
//    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
//        a.installTranslator(translator);
//    else
//        qWarning(logWarning()) << "Не удалось загрузить языковый файл.";
//#endif


    if(!connectOptions()){
        qInfo(logInfo()) << "Аварийное Завершение работы.";
        return 1;
    }

    qInfo(logInfo()) << "Запуск програмы.";
    MainWindow w;
    w.show();

    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    QTextStream console(stdout);


    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
#ifdef QT_DEBUG
    case QtInfoMsg:     out << "INF "; console << "Info: " << msg << endl; break;
    case QtDebugMsg:    out << "DBG "; console << "Debug: " << msg << endl; break;
    case QtWarningMsg:  out << "WRN "; console << "Warning: " << msg << endl; break;
    case QtCriticalMsg: out << "CRT "; console << "Critical: " << msg << endl; break;
    case QtFatalMsg:    out << "FTL "; console << "Fatality: " << msg << endl; break;
#else
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
#endif

    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Очищаем буферизированные данные
    console.flush();
}
