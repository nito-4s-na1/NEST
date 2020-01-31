#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QtGui>
#include <QListWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QListWidget>
#include <QSqlTableModel>
#include <QStringList>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QTableView>
#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <drilldownchart.h>
#include <drilldownslice.h>

namespace Ui {
class table;
}

class table : public QWidget
{
    Q_OBJECT

public:
    explicit table(QWidget *parent = nullptr);
    ~table();
    void setStr(QString);
    void setUsername(QString);
    void setPassword(QString);
    void DBList();
    int protocolcounttcp;
    int protocolcountdns;
    int protocolcountudp;
    int protocolcountarp;
    int protocolcounticmp;
    int protocolcountdhcp;


public slots:
    void on_pushButton_clicked();
    void pie_graph();

private:
    Ui::table *ui;
    QSqlDatabase db;
    QString strr;
    QString username;
    QString password;
};

#endif // TABLE_H
