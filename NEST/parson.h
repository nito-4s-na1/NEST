#ifndef PARSON_H
#define PARSON_H

#include <QObject>
#include <QString>

class Parson : public QObject
{
    Q_OBJECT
public:
    explicit Parson(QObject *parent = nullptr);
    Parson(const QString &name, const int age, QObject *parent = nullptr);

signals:

public slots:

private:
    QString name;
    int age;
    int No,Time,Souece,Destination,Protocol;
public:
    QString &getName();
    int getAge();
};

#endif // PARSON_H
