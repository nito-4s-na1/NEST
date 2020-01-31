#include "table.h"
#include "ui_table.h"

table::table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::table)
{
    QWidget::setFixedSize(1030,600);
    ui->setupUi(this);
    setWindowTitle("History Data");

}
void table::DBList(){
    QTableView * table=new QTableView;
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    //db.setPassword(password);
    db.open();

    QSqlQueryModel * model=new QSqlQueryModel;
    QSqlQuery * query=new QSqlQuery;
    if(query->prepare("select id,pgp_sym_decrypt(time,'"+password+"') as time,pgp_sym_decrypt(source,'"+password+"') as source,"
                      "pgp_sym_decrypt(destination,'"+password+"') as destination,cast(pgp_sym_decrypt(length,'"+password+"') as int) as length,"
                      "pgp_sym_decrypt(protocol,'"+password+"') as protocol,pgp_sym_decrypt(inout,'"+password+"') as in_out from "+strr)){
        query->exec();
    }
//    if(query->prepare("select * from "+str)){
//        //query->bindValue(const tablename);
//        query->exec();
//    }
    else{
        qWarning() << (query->lastError());
    }

    model->setQuery(*query);

    table->setSortingEnabled(true);
    table->verticalHeader()->setVisible(false);
    QSortFilterProxyModel * proxyModel=new QSortFilterProxyModel;
    proxyModel->setSourceModel(model);
    table->setModel(proxyModel);
    table->horizontalHeader()->setDefaultSectionSize(140);
    table->horizontalHeader()->setStretchLastSection(true);
    QBoxLayout* box = new QBoxLayout(QBoxLayout::TopToBottom);
    box->addWidget(table);
    QPushButton *button=new QPushButton(QIcon ("://ArtWork/Pi_graph.png"),"Graph");
    box->addWidget(button);
    QObject::connect(button,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));

    setLayout(box);

    protocolcountdns=0;
    protocolcounttcp=0;
    protocolcountudp=0;
    if(query->prepare("select pgp_sym_decrypt(protocol,'"+password+"') from "+strr)){
        if (query->exec()) {
            while (query->next()) {
               if(query->value(0).toString()=="TCP"){
                       protocolcounttcp +=1 ;
               }
               if(query->value(0).toString()=="DNS"){
                       protocolcountdns +=1;
               }
               if(query->value(0).toString()=="UDP"){
                   protocolcountudp +=1;
               }
               if(query->value(0).toString()=="ARP"){
                   protocolcountarp +=1;
               }
               if(query->value(0).toString()=="ICMP"){
                   protocolcounticmp +=1;
               }
               if(query->value(0).toString()=="DHCP"){
                   protocolcountdhcp +=1;
               }
            }
        }else {
                qWarning() << query->lastError();
                qInfo() << query->lastQuery() << query->boundValues();
        }
    } else {
        qWarning() << query->lastError();
    }
    db.close();
}

void table::on_pushButton_clicked(){
    pie_graph();
}

table::~table()
{
    delete ui;
}

void table::pie_graph(){
    DrilldownChart *chartp = new DrilldownChart();
    chartp->setTheme(QChart::ChartThemeLight);
    chartp->setAnimationOptions(QChart::AllAnimations);
    chartp->legend()->setVisible(true);
    chartp->legend()->setAlignment(Qt::AlignRight);

    QPieSeries *yearSeries = new QPieSeries(this);
    yearSeries->setName("count - All");

    /*const QStringList months = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };*/
    const QStringList names = {
        "TCP","DNS","UDP","ARP","ICMP","DHCP"
    };


    for (const QString &name : names) {
        QPieSeries *series = new QPieSeries(this);
        series->setName("count " + name);

       // for (const QString &month : months)
         //   *series << new DrilldownSlice(QRandomGenerator::global()->bounded(1000), month, yearSeries);//rannsuu



        if(name=="TCP"){
            *yearSeries << new DrilldownSlice(protocolcounttcp,name, series);
        }
        if(name=="DNS"){
            *yearSeries << new DrilldownSlice(protocolcountdns,name, series);
        }
        if(name=="UDP"){
            *yearSeries << new DrilldownSlice(protocolcountudp,name, series);
        }
        if(name=="ARP"){
            *yearSeries << new DrilldownSlice(protocolcountarp,name, series);
        }
        if(name=="ICMP"){
            *yearSeries << new DrilldownSlice(protocolcounticmp,name, series);
        }
        if(name=="DHCP"){
            *yearSeries << new DrilldownSlice(protocolcountdhcp,name, series);
        }
        QObject::connect(series, &QPieSeries::clicked, chartp, &DrilldownChart::handleSliceClicked);
    }

    //QObject::connect(yearSeries, &QPieSeries::clicked, chartp, &DrilldownChart::handleSliceClicked);

    chartp->changeSeries(yearSeries);

    QChartView *chartView=new QChartView(chartp);
    chartView->resize(500,300);
    chartView->show();
}

void table::setStr(QString a){
    strr=a;
}
void table::setUsername(QString a){
    username=a;
}
void table::setPassword(QString a){
    password=a;
}
