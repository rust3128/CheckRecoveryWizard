#include "lostcheckexecute.h"
#include "loggingcategories.h"
#include "taskliststatus.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

LostCheckExecute::LostCheckExecute(QObject *parent) : QObject(parent)
{

}

void LostCheckExecute::slotScriptExecute()
{



      QSqlDatabase dbAzs = QSqlDatabase::addDatabase("QIBASE",QString("azs"+QString::number(terminalID)));

      dbAzs.setHostName(conn.value("SERVER_NAME"));
      dbAzs.setDatabaseName(conn.value("DB_NAME"));
      dbAzs.setUserName("SYSDBA");
      dbAzs.setPassword(conn.value("PASSWORD"));
      emit signalCurrentTask(CONNECT_DB_AZS);
      if(!dbAzs.open()){
          QString errSQL = QString("Не возможно подключится к базе данных АЗС.\nПричина: %2")
                  .arg(dbAzs.lastError().text());
          qCritical(logCritical()) << errSQL;
          emit signalTaskStatus(CONNECT_DB_AZS, false,errSQL);
          emit finished();
          return;
      }
      emit signalTaskStatus(CONNECT_DB_AZS,true);
      emit signalCurrentTask(EXECUTE_SQL);
      QSqlQuery *q = new QSqlQuery(dbAzs);
      QString strSQL;
      strSQL.clear();
      QListIterator<QString> i(script);
      while(i.hasNext()){
          strSQL += i.next();
      }
      if(!q->exec(strSQL)) {
          QString errSQL = QString("Не удалось выполнить процедуру восстановления чека.\nПричина: %2")
                  .arg(q->lastError().text());
          qCritical(logCritical()) << errSQL;
          emit signalTaskStatus(EXECUTE_SQL,false,errSQL);
          emit finished();
          return;
      }
      q->exec("EXECUTE PROCEDURE TMP_LOST_CHECK;");
      q->exec("DROP PROCEDURE TMP_LOST_CHECK;");
      q->exec("COMMIT WORK;");
      emit signalTaskStatus(EXECUTE_SQL,true);
      emit finished();

}

