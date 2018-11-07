#include "lostcheckexecute.h"
#include "loggingcategories.h"

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

      if(!dbAzs.open()){
          QString errSQL = QString("%1 Не подключится к базе данных АЗС.\nПричина: %2")
                  .arg(Q_FUNC_INFO)
                  .arg(dbAzs.lastError().text());
          qCritical(logCritical()) << errSQL;
          emit finished(false,errSQL);
          return;
      }
      QSqlQuery *q = new QSqlQuery(dbAzs);
      QString strSQL;
      strSQL.clear();
      QListIterator<QString> i(script);
      while(i.hasNext()){
          strSQL += i.next();
      }
      if(!q->exec(strSQL)) {
          QString errSQL = QString("%1 Не удалось выполнить процедуру восстановления чека.\nПричина: %2")
                  .arg(Q_FUNC_INFO)
                  .arg(q->lastError().text());
          qCritical(logCritical()) << errSQL;
          emit finished(false,errSQL);
          return;
      }
      q->exec("EXECUTE PROCEDURE TMP_LOST_CHECK;");
      q->exec("DROP PROCEDURE TMP_LOST_CHECK;");
      q->exec("COMMIT WORK;");
      emit finished(true,"Чек успешно восстановлен.");

}

