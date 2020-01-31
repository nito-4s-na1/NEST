#include "listmodel.h"

ListModel::ListModel(QObject *parent):QAbstractTableModel(parent)
{
}
ListModel::~ListModel()
{
    while(!list.isEmpty()) {
        delete list.takeFirst();
    }
}

// 列数を返す。
int ListModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

// 行数を返す。
int ListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return list.count();
}

// データを返す。
QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    switch (index.column()) {
    case 0:
        return list[index.row()]->getName();
    case 1:
        return list[index.row()]->getAge();
    case 2:
        return list[index.row()]->getAge();
    case 3:
        return list[index.row()]->getAge();
    case 4:
        return list[index.row()]->getAge();
    case 5:
        return list[index.row()]->getAge();
    case 6:
        return list[index.row()]->getAge();
    default:
        return QVariant();
    }
}

// ヘッダを返す。
QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }
    if(orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("No.");
        case 1:
            return tr("Time");
        case 2:
            return tr("Source");
        case 3:
            return tr("destination");
        case 4:
            return tr("Protocol");
        case 5:
            return tr("Length");
        case 6:
            return tr("Info");
        default:
            return QVariant();
        }
    } else {
        return QVariant(section+1);
    }
}

// データの追加
void ListModel::add(Parson *parson)
{
    beginInsertRows(QModelIndex(), list.count(), list.count());
    list.append(parson);
    endInsertRows();
}
