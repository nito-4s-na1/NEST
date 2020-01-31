#include "datalist.h"
#include "ui_datalist.h"


DataList::DataList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataList)
{
    setWindowTitle("Data");
    setGeometry(300,300,300,300);

    listWidget=new QListWidget();
    listWidget->setSortingEnabled(true);

//    lineEdit = new QLineEdit("項目1");
//    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(addListItem()));
//            //エンターが押されたらリストに項目追加
    /*db connection*/

}
//void DataList::addListItem()
//{
//    listWidget->addItem(lineEdit->text());
//    listWidget->sortItems(Qt::AscendingOrder);
//        //文字コードの値が小さい順に並べ替え
//}
DataList::~DataList()
{
    delete ui;
}

void DataList::on_pushButton_clicked()
{
    QListWidgetItem *pItem = listWidget->currentItem();
    str = pItem->text();
    t = new table;
    t->setStr(str);
    t->setPassword(password);
    t->setUsername(username);
    t->DBList();
    t->show();

}

void DataList::DBList(){
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("testuser");
    db.setPassword("kinako");
    if(db.open()) qDebug() << "db connection";
    else qDebug() << "connection error";
    /*db*/
    QStringList Tables;
    QSqlQuery query(db);
    if (query.prepare("select tablename from pg_tables where tablename not like 'pg_%' and "
                      "schemaname not like 'infomation_%' and tableowner='"+username+"'")) {
        if (query.exec()) {
            while (query.next()) {
                Tables << query.value(0).toString();
            }
        } else {
            qWarning() << query.lastError();
            qInfo() << query.lastQuery() << query.boundValues();
        }
    } else {
        qWarning() << query.lastError();
    }

    QStringList list;
        list << Tables;
    listWidget->addItems(Tables);

//    QListWidgetItem *pItem = listWidget->currentItem();
//    QString str = pItem->text();
//    qDebug() << str;

    QBoxLayout* box = new QBoxLayout(QBoxLayout::TopToBottom);
    box->addWidget(listWidget);
//    box->addWidget(lineEdit);
    QPushButton *button=new QPushButton("Open");
//    QPushButton *button2=new QPushButton("Delete");
    box->addWidget(button);
//    box->addWidget(button2);
    QObject::connect(button,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
//    QObject::connect(button2,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked_2()));
    setLayout(box);

//    QStandardItem * item=NULL;
//    QList<QStandardItem *> listItem;


    //connect database

     //

    db.close();
}

void DataList::setUsername(QString a){
    username=a;
}
void DataList::setPassword(QString a){
    password=a;
}
