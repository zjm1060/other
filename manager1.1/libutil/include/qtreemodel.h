#ifndef QTREEMODEL_H
#define QTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QHash>

#include "global.h"

class QTreeItem;

class UTIL_EXPORT QTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    QTreeModel(int column, QObject *parent = 0);
    ~QTreeModel();

public:
	void setHorizontalHeaderLabels(const QStringList& labels);
	const QStringList horizontalHeaderLabels() const;

	bool setRowCount(int rows, const QModelIndex& index=QModelIndex());

	virtual bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
	virtual bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());

	bool removeRow(int row, const QModelIndex &parent=QModelIndex());

public:
    virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

	virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	QTreeItem*	createItem(QTreeItem* parent) const;
	QTreeItem*	itemFromIndex(const QModelIndex &index) const;

	void removePersistItem(int row, const QModelIndex& parent=QModelIndex());

private:
    QTreeItem*	m_pRoot;

	QHash<quint32, QTreeItem*> m_pItems;
};

#endif
