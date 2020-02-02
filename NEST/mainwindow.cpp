#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "chart.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
//QString s="testdesu";

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("NEST");
    model = new ListModel();
    ui->tableView->setModel(model);
    zikkoutyu=false;
    this->setVisible(false);
    createActions();
    createMenu();
    createToolBar();
    //et.start();
    counter=0;
    startCap->setDisabled(true);
    stopCap->setDisabled(false);
    restartCap->setDisabled(false);
    gratoma=false;
    chartView=nullptr;
    this->setAttribute(Qt::WA_DeleteOnClose,true);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
    delete ds;
}
void MainWindow::select(){
    DeviceSelect dev(this);
    if(dev.exec()){
        QString na=dev.comb->currentText();
        if(dev.chek[0]->checkState()==Qt::Checked)getTCP();
        if(dev.chek[1]->checkState()==Qt::Checked)getUDP();
        if(dev.chek[2]->checkState()==Qt::Checked)getDNS();
        if(dev.chek[3]->checkState()==Qt::Checked)getARP();
        if(dev.chek[4]->checkState()==Qt::Checked)getICMP();
        if(dev.chek[5]->checkState()==Qt::Checked)getDHCP();
        if(zikkoutyu==false){
            std::string name=na.toStdString();
            m.add_pcapdev(name);
            m.start();
            zikkoutyu=true;
        }
    }
}

void MainWindow::database_connection(){
    QApplication::setOverrideCursor(Qt::WaitCursor);

    std::ostringstream s1;
    std::ostringstream s2;
    std::ostringstream s3;
    std::ostringstream date;

    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);
    date << tm.tm_year+1900 << "_" << tm.tm_mon+1 << "_" << tm.tm_mday << "_" << tm.tm_hour << "_" << tm.tm_min << "_" << tm.tm_sec;
    s1 << "create table " << "packetcapture_" << date.str() << "(id serial primary key,time bytea,source bytea,destination bytea,length bytea,protocol bytea,inout bytea)";
    s2 << "insert into " << "packetcapture_" << date.str() << "(time,source,destination,length,protocol,inout) values ((pgp_sym_encrypt(?,?)),(pgp_sym_encrypt(?,?)),(pgp_sym_encrypt(?,?)),(pgp_sym_encrypt(?,?)),(pgp_sym_encrypt(?,?)),(pgp_sym_encrypt(?,?)))";
    QString CreateTable=QString::fromStdString(s1.str());
    QString InsertDB=QString::fromStdString(s2.str());
    /*connect to DB*/
          db = QSqlDatabase::addDatabase("QPSQL");
          db.setHostName("localhost");
          db.setDatabaseName("postgres");
          db.setUserName(username);
          db.setPassword(password);
          bool ok = db.open();
          if(ok != true) QMessageBox::critical(this,"Data Insert","Data Insert Faild!");

        QSqlQuery query(db);
        /*create table*/
        if (query.exec(CreateTable)){
            if (query.exec()) {
               qInfo() << query.lastInsertId().toLongLong() << "added";
            }
            else {
                qWarning() <<query.lastError();
                qInfo() << query.lastQuery() << query.boundValues();
            }
        }
        else {
            qWarning() << query.lastError();
        }
        /*insert into db*/
        //for (auto &num:model->list){//パケットクラス配列の内容の表示
        for(int i=0;i<model->getSizeofList();i++){
            if (query.prepare(InsertDB)){
                       //パケット取得(getPacket)->そのパケットのステータス配列取得(getstatusvector)->各要素アクセスという形でアクセスしています
                query.addBindValue(model->getPacket(i)->getStatusVector()[0]);//time
                query.addBindValue(password);
                query.addBindValue(model->getPacket(i)->getStatusVector()[1]);//source
                query.addBindValue(password);
                query.addBindValue(model->getPacket(i)->getStatusVector()[2]);//dest
                query.addBindValue(password);
                query.addBindValue(model->getPacket(i)->getStatusVector()[3]);//length
                query.addBindValue(password);
                query.addBindValue(model->getPacket(i)->getStatusVector()[4]);//protocol
                query.addBindValue(password);
                query.addBindValue(model->getPacket(i)->getStatusVector()[5]);//inout
                query.addBindValue(password);
                /*query.addBindValue(num->source);
                query.addBindValue(num->dest);
                query.addBindValue(num->length);
                query.addBindValue(num->protocol);*/
                if (query.exec()) {
                   qInfo() << query.lastInsertId().toLongLong() << "added";
                }
                else {
                    qWarning() <<query.lastError();
                    qInfo() << query.lastQuery() << query.boundValues();
                }
            }
            else {
                qWarning() << query.lastError();
            }
        }
        //count
        protocolcountdns=0;
        protocolcounttcp=0;
        protocolcountudp=0;
        s3 << "select pgp_sym_decrypt(protocol,'" << password.toUtf8().constData() << "') from packetcapture_"<< date.str();
        QString SelectTable=QString::fromStdString(s3.str());
        if(query.prepare(SelectTable)){
            if (query.exec()) {
                while (query.next()) {
                   if(query.value(0).toString()=="TCP"){
                           protocolcounttcp +=1 ;
                   }
                   if(query.value(0).toString()=="DNS"){
                           protocolcountdns +=1;
                   }
                   if(query.value(0).toString()=="UDP"){
                       protocolcountudp +=1;
                   }
                   if(query.value(0).toString()=="ARP"){
                       protocolcountarp +=1;
                   }
                   if(query.value(0).toString()=="ICMP"){
                       protocolcounticmp +=1;
                   }
                   if(query.value(0).toString()=="DHCP"){
                       protocolcountdhcp +=1;
                   }
                }
            }else {
                    qWarning() << query.lastError();
                    qInfo() << query.lastQuery() << query.boundValues();
            }
        } else {
            qWarning() << query.lastError();
        }
        db.close();
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        QMessageBox::information(this,"Data Insert","Data Insert Completed!");
}

ListModel* MainWindow::getModel(){
    return this->model;
}
void MainWindow::getTCP(){
    m.on("TCP", [&](const pm::Property& p) {    //イベント "UDP"のコールバック関数
        const auto& s3=p.pkt_size();
        std::ostringstream s1;
        std::ostringstream s2;
        //double time=(et.elapsed())/(double)1000;

        QElapsedTimer timer;
        timer.start();

        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        std::ostringstream date;
        date << tm.tm_year+1900 << "_" << tm.tm_mon+1 << "_" << tm.tm_mday << "_" << tm.tm_hour << "_" << tm.tm_min << "_" << tm.tm_sec;
        const std::string dates=date.str();

        //qDebug() << time;
        counter += 1;
        const std::string ss3=std::to_string(s3);
            //ストリームにパケットの情報を書きこむ
        s1<<p["IPv4.src"];
        s2<<p["IPv4.dst"];
        const std::string ss1=s1.str();
        const std::string ss2=s2.str();

        QString inout;
        if(s1.str()==thisip) inout="OUT";
        else if(s2.str()==thisip) inout="IN";
        else inout="none";

        model->add(new Packet(counter,tr(dates.c_str()),tr(ss1.c_str()),tr(ss2.c_str()),
                tr("TCP"),tr(ss3.c_str()),inout));
        ui->tableView->scrollToBottom();
     });
}
void MainWindow::getUDP(){
    m.on("UDP", [&](const pm::Property& p) {    //イベント "UDP"のコールバック関数
        const auto& s3=p.pkt_size();
        std::ostringstream s1;
        std::ostringstream s2;

        QElapsedTimer timer;
        timer.start();

        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        std::ostringstream date;
        date << tm.tm_year+1900 << "_" << tm.tm_mon+1 << "_" << tm.tm_mday << "_" << tm.tm_hour << "_" << tm.tm_min << "_" << tm.tm_sec;
        const std::string dates=date.str();

        //double time=(et.elapsed())/(double)1000;
        counter+=1;
        const std::string ss3=std::to_string(s3);
            //ストリームにパケットの情報を書きこむ
        s1<<p["IPv4.src"];
        s2<<p["IPv4.dst"];
        const std::string ss1=s1.str();
        const std::string ss2=s2.str();

        QString inout;
        if(s1.str()==thisip) inout="OUT";
        else if(s2.str()==thisip) inout="IN";
        else inout="none";

        model->add(new Packet(counter,tr(dates.c_str()),tr(ss1.c_str()),tr(ss2.c_str()),
                tr("UDP"),tr(ss3.c_str()),inout));
        ui->tableView->scrollToBottom();
     });
}
void MainWindow::getDNS(){
    m.on("DNS", [&](const pm::Property& p) {    //イベント "UDP"のコールバック関数
        const auto& s3=p.pkt_size();
        std::ostringstream s1;
        std::ostringstream s2;

        QElapsedTimer timer;
        timer.start();
        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        std::ostringstream date;
        date << tm.tm_year+1900 << "_" << tm.tm_mon+1 << "_" << tm.tm_mday << "_" << tm.tm_hour << "_" << tm.tm_min << "_" << tm.tm_sec;
        const std::string dates=date.str();

        //double time=(et.elapsed())/(double)1000;
        counter+=1;
        const std::string ss3=std::to_string(s3);
            //ストリームにパケットの情報を書きこむ
        s1<<p["IPv4.src"];
        s2<<p["IPv4.dst"];
        const std::string ss1=s1.str();
        const std::string ss2=s2.str();

        QString inout;
        if(s1.str()==thisip) inout="OUT";
        else if(s2.str()==thisip) inout="IN";
        else inout="none";

        model->add(new Packet(counter,tr(dates.c_str()),tr(ss1.c_str()),tr(ss2.c_str()),
                tr("DNS"),tr(ss3.c_str()),inout));
        ui->tableView->scrollToBottom();
     });
}

void MainWindow::getARP(){
    m.on("ARP", [&](const pm::Property& p) {    //イベント "UDP"のコールバック関数
        const auto& s3=p.pkt_size();
        std::ostringstream s1;
        std::ostringstream s2;

        //QElapsedTimer timer;
        //timer.start();
        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        std::ostringstream date;
        date << tm.tm_year+1900 << "_" << tm.tm_mon+1 << "_" << tm.tm_mday << "_" << tm.tm_hour << "_" << tm.tm_min << "_" << tm.tm_sec;
        const std::string dates=date.str();

        //double time=(et.elapsed())/(double)1000;
        counter+=1;
        const std::string ss3=std::to_string(s3);
            //ストリームにパケットの情報を書きこむ
        s1<<p["IPv4.src"];
        s2<<p["IPv4.dst"];
        const std::string ss1=s1.str();
        const std::string ss2=s2.str();

        QString inout;
        if(s1.str()==thisip) inout="OUT";
        else if(s2.str()==thisip) inout="IN";
        else inout="none";

        model->add(new Packet(counter,tr(dates.c_str()),tr(ss1.c_str()),tr(ss2.c_str()),
                tr("ARP"),tr(ss3.c_str()),inout));
        ui->tableView->scrollToBottom();
     });
}

void MainWindow::getICMP(){
    m.on("ICMP", [&](const pm::Property& p) {    //イベント "UDP"のコールバック関数
        const auto& s3=p.pkt_size();
        std::ostringstream s1;
        std::ostringstream s2;

        //QElapsedTimer timer;
        //timer.start();
        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        std::ostringstream date;
        date << tm.tm_year+1900 << "_" << tm.tm_mon+1 << "_" << tm.tm_mday << "_" << tm.tm_hour << "_" << tm.tm_min << "_" << tm.tm_sec;
        const std::string dates=date.str();

        //double time=(et.elapsed())/(double)1000;
        counter+=1;
        const std::string ss3=std::to_string(s3);
            //ストリームにパケットの情報を書きこむ
        s1<<p["IPv4.src"];
        s2<<p["IPv4.dst"];
        const std::string ss1=s1.str();
        const std::string ss2=s2.str();

        QString inout;
        if(s1.str()==thisip) inout="OUT";
        else if(s2.str()==thisip) inout="IN";
        else inout="none";

        model->add(new Packet(counter,tr(dates.c_str()),tr(ss1.c_str()),tr(ss2.c_str()),
                tr("ICMP"),tr(ss3.c_str()),inout));
        ui->tableView->scrollToBottom();
     });
}

void MainWindow::getDHCP(){
    m.on("DHCP", [&](const pm::Property& p) {    //イベント "UDP"のコールバック関数
        const auto& s3=p.pkt_size();
        std::ostringstream s1;
        std::ostringstream s2;

        //QElapsedTimer timer;
        //timer.start();
        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        std::ostringstream date;
        date << tm.tm_year+1900 << "_" << tm.tm_mon+1 << "_" << tm.tm_mday << "_" << tm.tm_hour << "_" << tm.tm_min << "_" << tm.tm_sec;
        const std::string dates=date.str();

        //double time=(et.elapsed())/(double)1000;
        counter+=1;
        const std::string ss3=std::to_string(s3);
            //ストリームにパケットの情報を書きこむ
        s1<<p["IPv4.src"];
        s2<<p["IPv4.dst"];
        const std::string ss1=s1.str();
        const std::string ss2=s2.str();

        QString inout;
        if(s1.str()==thisip) inout="OUT";
        else if(s2.str()==thisip) inout="IN";
        else inout="none";

        model->add(new Packet(counter,tr(dates.c_str()),tr(ss1.c_str()),tr(ss2.c_str()),
                tr("DHCP"),tr(ss3.c_str()),inout));
        ui->tableView->scrollToBottom();
     });
}
void MainWindow::start(){

    gratoma=false;
    ui->tableView->reset();
    model = new ListModel();
    ui->tableView->setModel(model);
    m.start();
    counter=0;
    et.restart();
    zikkoutyu=true;
    startCap->setDisabled(true);
    stopCap->setDisabled(false);
    restartCap->setDisabled(false);
}
void MainWindow::stop(){
    m.halt();
    gratoma=true;
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Unsaved packets"));
    msgBox.setText(tr("Do you save captured packets or not?\n"
                      "If you don't save that, lose captured data."));
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes,tr("Save"));
    msgBox.setButtonText(QMessageBox::No,tr("No"));
    int res=msgBox.exec();
    if(res==QMessageBox::Yes){
        database_connection();
    }
    else{
        msgBox.close();
    }
    startCap->setDisabled(false);
    stopCap->setDisabled(true);
    restartCap->setDisabled(true);
}
void MainWindow::restart(){
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Unsaved packets"));
    msgBox.setText(tr("Do you save captured packets or not?\n"
                      "If you don't save that, lose captured data."));
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes,tr("Save"));
    msgBox.setButtonText(QMessageBox::No,tr("Discard"));
    msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
    int res=msgBox.exec();
    if(res==QMessageBox::Yes){
        database_connection();
        ui->tableView->reset();
        model = new ListModel();
        ui->tableView->setModel(model);
        m.start();
        counter=0;
        et.restart();
        zikkoutyu=true;
        startCap->setDisabled(true);
        stopCap->setDisabled(false);
        restartCap->setDisabled(false);
    }
    else if (res==QMessageBox::No){
        ui->tableView->reset();
        model = new ListModel();
        ui->tableView->setModel(model);
        m.start();
        counter=0;
        et.restart();
        zikkoutyu=true;
        startCap->setDisabled(true);
        stopCap->setDisabled(false);
        restartCap->setDisabled(false);
    }
    else{
        msgBox.close();
    }

}

void MainWindow::graph_plot(){
        qDebug()<<"DDD";

        if(chartView==nullptr){
            qDebug()<<"BBB";
            et2.start();
            Chart *chart = new Chart;
            chart->setTitle("Packet amount per sec");
            chart->legend()->hide();

            chart->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);
            chart->mw=this;


            chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            //chartView->setAttribute(Qt::WA_DeleteOnClose,true);

             //ui->verticalLayout->addWidget(chartView,Qt::AlignVCenter);

            chartView->resize(500,500);
            chartView->setWindowTitle("Real Time Graph");
            chartView->show();
        }
        else if(!chartView->isVisible()){

            qDebug()<<"CCC";
            et2.start();
            Chart *chart = new Chart;
            chart->setTitle("Packet amount per seconds");
            chart->legend()->hide();

            chart->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);
            chart->mw=this;


            chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            //chartView->setAttribute(Qt::WA_DeleteOnClose,true);

            //ui->verticalLayout->addWidget(chartView,Qt::AlignVCenter);
            chartView->resize(500,500);
            chartView->show();
    }

}
//void MainWindow::pie_graph(){
//    DrilldownChart *chartp = new DrilldownChart();
//    chartp->setTheme(QChart::ChartThemeLight);
//    chartp->setAnimationOptions(QChart::AllAnimations);
//    chartp->legend()->setVisible(true);
//    chartp->legend()->setAlignment(Qt::AlignRight);

//    QPieSeries *yearSeries = new QPieSeries(this);
//    yearSeries->setName("count - All");

//    /*const QStringList months = {
//        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
//    };*/
//    const QStringList names = {
//        "TCP","DNS","UDP","ARP","ICMP","DHCP"
//    };


//    for (const QString &name : names) {
//        QPieSeries *series = new QPieSeries(this);
//        series->setName("count " + name);

//       // for (const QString &month : months)
//         //   *series << new DrilldownSlice(QRandomGenerator::global()->bounded(1000), month, yearSeries);//rannsuu



//        if(name=="TCP"){
//            *yearSeries << new DrilldownSlice(protocolcounttcp,name, series);
//        }
//        if(name=="DNS"){
//            *yearSeries << new DrilldownSlice(protocolcountdns,name, series);
//        }
//        if(name=="UDP"){
//            *yearSeries << new DrilldownSlice(protocolcountudp,name, series);
//        }
//        if(name=="ARP"){
//            *yearSeries << new DrilldownSlice(protocolcountarp,name, series);
//        }
//        if(name=="ICMP"){
//            *yearSeries << new DrilldownSlice(protocolcounticmp,name, series);
//        }
//        if(name=="DHCP"){
//            *yearSeries << new DrilldownSlice(protocolcountdhcp,name, series);
//        }
//        QObject::connect(series, &QPieSeries::clicked, chartp, &DrilldownChart::handleSliceClicked);
//    }

//    //QObject::connect(yearSeries, &QPieSeries::clicked, chartp, &DrilldownChart::handleSliceClicked);

//    chartp->changeSeries(yearSeries);

//    QChartView *chartView=new QChartView(chartp);
//    chartView->resize(500,300);
//    chartView->show();
//}
void MainWindow::data_history(){
    D = new DataList;
    D->setUsername(username);
    D->setPassword(password);
    D->DBList();
    D->show();
}/*
void MainWindow::about()
{
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Menu</b> example shows how to create "
               "menu-bar menus and context menus."));
}

MainWindow *MainWindow::findMainWindow(const QString &fileName) const
{
    QString canonicalFilePath=QFileInfo(fileName).canonicalFilePath();
    foreach(QWidget *widget,QApplication::topLevelWidgets()){
        MainWindow *mainWin=qobject_cast<MainWindow *>(widget);
        return mainWin;
    }
    return 0;
}

void MainWindow::aboutQt()
{
    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}*/

void MainWindow::createActions()
{
    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    exitAct->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    //exitAct->setIcon();
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    startCap=new QAction(QIcon("://ArtWork/NEST_Logo.png"),tr("Start Capturing"),this);
    //startCap->setShortcut(QKeySequence::FindPrevious);
    startCap->setStatusTip(tr("Start Capturing"));
    connect(startCap,&QAction::triggered,this,&MainWindow::start);

    stopCap=new QAction(QIcon("://ArtWork/Stop_icon.png"),tr("Stop Capturing"),this);
    //stopCap->setShortcut(QKeySequence::FindPrevious);
    stopCap->setStatusTip(tr("Stop Captuering"));
    connect(stopCap,&QAction::triggered,this,&MainWindow::stop);

    restartCap=new QAction(QIcon("://ArtWork/Restart_icon.png"),tr("Restart Capturing"),this);
    //findPrevAct->setShortcut(QKeySequence::FindPrevious);
    restartCap->setStatusTip(tr("Restart Capturing"));
    connect(restartCap,&QAction::triggered,this,&MainWindow::restart);

    graph=new QAction(QIcon("://ArtWork/Graph_icon.png"),tr("Show Graph"),this);
    //graph->setShortcut(QKeySequence::FindPrevious);
    graph->setStatusTip(tr("Show Graph"));
    connect(graph,&QAction::triggered,this,&MainWindow::graph_plot);

    history=new QAction(QIcon("://ArtWork/Database.png"),tr("Data History"),this);
    history->setStatusTip(tr("Data History"));
    connect(history,&QAction::triggered,this,&MainWindow::data_history);

    /*aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);*/
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);

    capMenu = menuBar()->addMenu(tr("&Capture"));
    capMenu->addAction(startCap);
    capMenu->addAction(stopCap);
    capMenu->addAction(restartCap);

    statisMenu = menuBar()->addMenu(tr("&Statistics"));
    statisMenu->addAction(graph);

    HistoryMenu = menuBar()->addMenu(tr("&History"));
    HistoryMenu->addAction(history);

    /*helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);*/
}

void MainWindow::createToolBar(){
    ToolBar = addToolBar(tr("&ToolBar"));
    ToolBar->addAction(startCap);
    ToolBar->addAction(stopCap);
    ToolBar->addAction(restartCap);
    ToolBar->addAction(graph);
    ToolBar->addAction(history);
}

void MainWindow::setUsername(QString a){
    username=a;
}
void MainWindow::setPassword(QString a){
    password=a;
}
