#include "widget.h"
#include "deviceselect.h"
#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QTimer>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    QCoreApplication::setApplicationName("NEST");
    MainWindow mw;
    //QTimer::singleShot(0,select);
    Login l;
    DeviceSelect ds;
    QPixmap pixmap("://ArtWork/NEST_Font_Logo.png");
    QSplashScreen splash(pixmap,Qt::WindowStaysOnTopHint);
    splash.setPixmap(pixmap.scaled(500,500,Qt::KeepAspectRatio));
    splash.show();
    a.processEvents();
    splash.showMessage("Now Loading...",Qt::AlignBottom,Qt::black);
    //QTimer::singleShot(5000,&splash,&QWidget::close);
    splash.finish(&mw);
    if(l.exec()){
        if(ds.exec()){
            QString na=ds.comb->currentText();
            if(ds.chek[0]->checkState()==Qt::Checked)mw.getTCP();
            if(ds.chek[1]->checkState()==Qt::Checked)mw.getUDP();
            if(ds.chek[2]->checkState()==Qt::Checked)mw.getDNS();
            if(ds.chek[3]->checkState()==Qt::Checked)mw.getARP();
            if(ds.chek[4]->checkState()==Qt::Checked)mw.getICMP();
            if(ds.chek[5]->checkState()==Qt::Checked)mw.getDHCP();
            if(mw.zikkoutyu==false){
                std::string name=na.toStdString();

                mw.setUsername(l.getUsername());
                mw.setPassword(l.getPassword());

                mw.m.add_pcapdev(name);
                mw.devname=name;

                int s = socket(AF_INET, SOCK_STREAM, 0);

                struct ifreq ifr;
                ifr.ifr_addr.sa_family = AF_INET;
                strncpy(ifr.ifr_name, mw.devname.c_str(),IFNAMSIZ-1);
                ioctl(s, SIOCGIFADDR, &ifr);
                int fclose(s);
                struct sockaddr_in addr;
                memcpy( &addr, &ifr.ifr_ifru.ifru_addr, sizeof(struct sockaddr_in) );
                mw.thisip=inet_ntoa(addr.sin_addr);
                qDebug()<<QString::fromStdString(mw.thisip)<<"aaa";
                mw.m.start();
                mw.et.start();
                mw.zikkoutyu=true;
            }
            mw.show();
        }
    }
    return a.exec();
}

