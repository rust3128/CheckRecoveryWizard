#ifndef CONNECTIONSPAGE_H
#define CONNECTIONSPAGE_H

#include <QWizardPage>
#include <QSqlTableModel>
#include <QSqlError>

namespace Ui {
class ConnectionsPage;
}

class ConnectionsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConnectionsPage(QWidget *parent = 0);
    ~ConnectionsPage();

private:
    Ui::ConnectionsPage *ui;
    QSqlTableModel *modelConnections;
private:
    void createModel();
    void createUI();
};

#endif // CONNECTIONSPAGE_H
