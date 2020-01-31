#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <packetmachine.hpp>
#include <string>
//#include "listmodel.h"
#include "packetlistmodel.h"
#include "deviceselect.h"
#include<pcap.h>
#include <sstream>
#include <QElapsedTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QSplashScreen>
#include <QWizard>
#include <QLabel>
#include <QTableView>
#include <QSqlQueryModel>
#include <QListWidget>
#include <QSqlTableModel>
#include <QStringList>
#include <QStandardItemModel>
#include <QModelIndex>
#include <datalist.h>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <drilldownchart.h>
#include <drilldownslice.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    pm::Machine m;
    QSqlDatabase db;
    QElapsedTimer et;
    QElapsedTimer et2;
    std::string devname;
    std::string thisip;
    bool gratoma;
    bool zikkoutyu;

    unsigned long int counter;
    void getTCP();
    void getUDP();
    void getDNS();
    void getARP();
    void getICMP();
    void getDHCP();
    ListModel* getModel();
    void setUsername(QString);
    void setPassword(QString);
    DataList *D;
    int protocolcounttcp;
    int protocolcountdns;
    int protocolcountudp;
    int protocolcountarp;
    int protocolcounticmp;
    int protocolcountdhcp;



  public slots:

  private slots:
    void select();/*
    void about();
    void aboutQt();*/
    void start();
    void stop();
    void restart();
    void graph_plot();
    void data_history();
    //! toolbar end

    void database_connection();
    //! click_action end

private:

    void createActions();
    void createMenu();
    void createToolBar();
    MainWindow *findMainWindow(const QString &fileName) const;

    QString username;
    QString password;

    Ui::MainWindow *ui;
    QtCharts::QChartView *chartView;
    DeviceSelect *ds;
    ListModel *model;
    //!Menu
    QMenu *fileMenu;
    QMenu *capMenu;
    QMenu *statisMenu;
    QMenu *HistoryMenu;
    QMenu *helpMenu;
    //!Menu end
    //! ToolBar
    QToolBar *ToolBar;
    //! ToolBar end
    QActionGroup *alignmentGroup;

    //! menu action
    QAction *exitAct;
    QAction *startCap,*stopCap,*restartCap;
    QAction *graph;//statistics Menu
    QAction *history;
    QAction *aboutAct,*aboutQtAct;//about Menu
    QLabel *infoLabel;//assist label
    //!menu action end
};

#endif // MAINWINDOW_H
