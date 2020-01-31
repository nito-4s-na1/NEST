#ifndef DATALIST_H
#define DATALIST_H

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
#include <QtGui>
#include <QtWidgets>
#include <QBoxLayout>
#include "table.h"

namespace Ui {
class DataList;
}

class DataList : public QWidget
{
    Q_OBJECT

public:
    explicit DataList(QWidget *parent = nullptr);
    ~DataList();
    QBoxLayout *bl;
    void setUsername(QString);
    void setPassword(QString);
    void DBList();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DataList *ui;
    QListWidget* listWidget;
    QLineEdit* lineEdit;
    QSqlDatabase db;
    QStringListModel *datamodel;
    QListView *listview;
    QStandardItem *item=NULL;
    QList<QStandardItem *> listItem;
    QString selectedName;
    table *t;
    QString str;
    QString username;
    QString password;

    void addListItem();

};

#endif // DATALIST_H
