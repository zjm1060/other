#ifndef QTREEITEM_H
#define QTREEITEM_H

#include <QList>
#include <QVector>
#include <QVariant>

class QTreeItem
{
public:
	QTreeItem(QTreeItem* parent=0);
    QTreeItem(int childCount, int column, QTreeItem *parent=0);
    ~QTreeItem();

public:
	bool insertChild(int row, QTreeItem* child);
    void appendChild(QTreeItem* child);

	void removeChildren(int row, int count);

    QTreeItem* child(int row);

    int childCount() const;
	void setChildCount(int count);

    int columnCount() const;
	void setColumnCount(int count);

	void setData(int column, const QVariant& value, int role=Qt::DisplayRole);
    QVariant data(int column, int role=Qt::DisplayRole) const;

    int row() const;
    QTreeItem* parent();

private:
    QList<QTreeItem*> m_childItems;
    QVector<QMap<int, QVariant> > m_itemData;

    QTreeItem*	m_pParent;
};

#endif
