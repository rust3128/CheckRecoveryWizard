#ifndef LOSTCHECKEXECUTE_H
#define LOSTCHECKEXECUTE_H

#include <QObject>
#include <QHash>

class LostCheckExecute : public QObject
{
    Q_OBJECT
public:
    explicit LostCheckExecute(QObject *parent = nullptr);
    void setScript(QStringList scr)
    {
        script = scr;
    }
    void setTerminalID(int term)
    {
        terminalID =term;
    }
    void setConnData(QHash<QString, QString> hash)
    {
        conn = hash;
    }
signals:
    void signalCurrentTask(int);
    void signalTaskStatus(int, bool, QString mess = nullptr);
    void finished();

public slots:
   void slotScriptExecute();
private:
    QStringList script;
    int terminalID;
    QHash<QString, QString> conn;
};

#endif // LOSTCHECKEXECUTE_H
