#ifndef CONNECTIONSPAGE_H
#define CONNECTIONSPAGE_H

#include <QWizardPage>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


namespace Ui {
class ConnectionsPage;
}

class ConnectionsPage : public QWizardPage
{
    Q_OBJECT


public:
    explicit ConnectionsPage(QWidget *parent = 0);
    ~ConnectionsPage();


signals:
    void sendInfo(QString, QString, bool);
    void signalConnRecord(QSqlRecord);
protected:
    bool validatePage();
    void initializePage();

private slots:
    void on_listView_clicked(const QModelIndex &idx);

private:
    Ui::ConnectionsPage *ui;
    QSqlTableModel *modelConnections;
    int connID;
    QSqlRecord recConn;
private:
    void createModel();
    void createUI();


};

#endif // CONNECTIONSPAGE_H
