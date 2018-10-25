#ifndef CONNECTIONSPAGE_H
#define CONNECTIONSPAGE_H

#include <QWizardPage>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>

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
    void sendInfo(int, QString);
protected:
    bool validatePage();

private:
    Ui::ConnectionsPage *ui;
    QSqlTableModel *modelConnections;
private:
    void createModel();
    void createUI();

};

#endif // CONNECTIONSPAGE_H
