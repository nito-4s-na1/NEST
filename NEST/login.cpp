#include "login.h"
#include "ui_login.h"
#include "QDebug"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{

    ui->setupUi(this);
    setWindowTitle("Log In");
    //this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
}


Login::~Login()
{
    delete ui;
}

//bool Login::angou(){
//    return false;
//}
void Login::on_cancel_button_clicked()
{
    //qDebug() << "Cancel clicked.";
    //QCoreApplication::quit();

    reject();
    exit(EXIT_FAILURE);
}

bool Login::LoginJudge(QString Usr,QString Pass){
    QString Name="tanaka";
    QString PWord="123456";
    QString PassJudge="false";
    QString True="true";

//    if(Usr==Name&&Pass==PWord){
//        return  true;
//    }
//    else{
//        return false;
//    }
    /*connect to DB*/
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    //db.setPassword("postgres");
    bool ok = db.open();
    if(ok != true) QMessageBox::critical(this,"Connection","Connection Failed!\nPlease set up the database.");
    QSqlQuery query(db);
    if (query.prepare("select (password = crypt(?, password)) from user_data where name=?")) {
        query.addBindValue(Pass);
        query.addBindValue(Usr);
        if (query.exec()) {
            while (query.next()) {
                PassJudge = query.value(0).toString();
            }
        } else {
            qWarning() << query.lastError();
            qInfo() << query.lastQuery() << query.boundValues();
        }
    } else {
        qWarning() << query.lastError();
    }
    db.close();
    if(PassJudge.toUtf8().data() == True){
        return true;
    }
    else {
        QMessageBox::critical(this,"","The Username or Password is Incorrect.");
        return false;
    }
}

void Login::on_login_button_clicked()
{
    UN=ui->lineEdit_username->text();
    PW=ui->lineEdit_password->text();
    if(UN.isEmpty() or PW.isEmpty()) QMessageBox::critical(this,"","Not entered.");
    else if(LoginJudge(UN,PW)==true){
        accept();
    }
}

void Login::on_register_button_clicked()
{
    QString UN=ui->lineEdit_username->text();
    QString PW=ui->lineEdit_password->text();
    if(UN.isEmpty() or PW.isEmpty()) QMessageBox::critical(this,"","Not entered.");
    else {
        /*check db*/
        db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName("localhost");
        db.setDatabaseName("postgres");
        db.setUserName(UN);
        db.setPassword(PW);
        bool ok1 = db.open();
        if(ok1 != true) QMessageBox::critical(this,"Error","The Username you entered does not exist.\nPlease set up the database.");
        else {
            db.close();//check finish
            /*connect to DB*/
            db = QSqlDatabase::addDatabase("QPSQL");
            db.setHostName("localhost");
            db.setDatabaseName("postgres");
            db.setUserName("postgres");
            bool ok2 = db.open();
            if(ok2 != true) QMessageBox::critical(this,"Connection","Connection Failed!");
            QSqlQuery query(db);
            if (query.exec("create table user_data(name text primary key not null, password text not null)")) {
                if (!query.exec()) {
                    qWarning() << query.lastError();
                    qInfo() << query.lastQuery() << query.boundValues();
                }
            } else {
                qWarning() << query.lastError();
            }
            if (query.prepare("insert into user_data(name, password) values (?,(crypt(?,gen_salt('md5'))))")) {
                query.addBindValue(UN);
                query.addBindValue(PW);
                if (query.exec()) {
                    qInfo() << query.lastInsertId().toLongLong() << "added";
                    QMessageBox::information(this,"","Registered user.");
                } else {
                    qWarning() <<query.lastError();
                    qInfo() << query.lastQuery() << query.boundValues();
                    QMessageBox::critical(this,"","The UserName you entered is not unique.");
                }
            } else {
                qWarning() << query.lastError();
            }
            db.close();//connect finish
        }
    }
}
QString Login::getUsername(){
    return  UN;
}
QString Login::getPassword(){
    return PW;
}
