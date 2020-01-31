#include "parson.h"

Parson::Parson(QObject *parent) : QObject(parent)
{

}

Parson::Parson(const QString &name, const int age, QObject *parent):Parson(parent)
{
    this->name = name;
    this->age = age;
}


QString &Parson::getName()
{
    return this->name;
}

int Parson::getAge()
{
    return this->age;
}
