#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "parson.h"

class ListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = nullptr);

    virtual ~ListModel() override;

    // 列数を返す。
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // 行数を返す。
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // データを返す。
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // ヘッダを返す。
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // データの追加
    void add(Parson *parson);
private:
    // 実際のデータの配列
    QVector<Parson*> list;
};

#endif // LISTMODEL_H
