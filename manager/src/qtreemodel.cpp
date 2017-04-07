#include <QtGui>

#include "qtreeitem.h"
#include "qtreemodel.h"

#include "define.h"

QTreeModel::QTreeModel(int column, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_pRoot=new QTreeItem(0, column);
}

QTreeModel::~QTreeModel()
{
	m_pRoot->setChildCount(0);

    SAFE_DELETE_CLASS(m_pRoot);
}

void QTreeModel::setVerticalHeaderLabels(const QStringList& labels)
{
	if (m_pRoot != NULL)
	{
		for (int i=0; i<labels.size(); i++)
		{
			m_pRoot->setData(i, labels[i]);
		}
	}
}

QVariant QTreeModel::data(const QModelIndex &index, int role) const
{
	QTreeItem* pItem=itemFromIndex(index);
	if (pItem != NULL && pItem->columnCount() > index.column())
	{
		return pItem->data(index.column(), role);
	}

	return QVariant();
}

Qt::ItemFlags QTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
	{
        return 0;
	}

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant QTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
        return m_pRoot->data(section);
	}

    return QVariant();
}

QModelIndex QTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
	{
        return QModelIndex();
	}

	QTreeItem* pParentItem=parent.isValid()? itemFromIndex(parent):m_pRoot;

	if (pParentItem == NULL)
	{
		return QModelIndex();
	}

    QTreeItem* pChildItem = pParentItem->child(row);
	return pChildItem!=NULL? createIndex(row, column, pChildItem):QModelIndex();
}

QModelIndex QTreeModel::parent(const QModelIndex &index) const
{
    QTreeItem* pChildItem = itemFromIndex(index);
	if (pChildItem == NULL)
	{
		return QModelIndex();
	}

    QTreeItem* pParentItem = pChildItem->parent();

    if (pParentItem == m_pRoot)
	{
        return QModelIndex();
	}

    return createIndex(pParentItem->row(), 0, pParentItem);
}

int QTreeModel::rowCount(const QModelIndex &parent) const
{
    QTreeItem* pItem=parent.isValid()? itemFromIndex(parent):m_pRoot;

	return pItem!=NULL? pItem->childCount():0;;
}

int QTreeModel::columnCount(const QModelIndex &parent) const
{
	QTreeItem* pItem=parent.isValid()? itemFromIndex(parent):m_pRoot;

	return pItem!=NULL? pItem->columnCount():0;
}

bool QTreeModel::setRowCount(int rows, const QModelIndex& index)
{
	if (rows < 0)
	{
		return false;
	}

	QTreeItem* pItem=index.isValid()? itemFromIndex(index):m_pRoot;

	if (pItem == NULL)
	{
		return false;
	}

	int nCount=pItem->childCount();
	if (rows == nCount)
	{
		return true;
	}
	else if (rows < nCount)
	{
		return removeRows(rows, nCount-rows, index);
	}
	else
	{
		return insertRows(rows, nCount-rows, index);
	}

	return true;
}

bool QTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
	QTreeItem* pParentItem=parent.isValid()? itemFromIndex(parent):m_pRoot;

	if (pParentItem == NULL)
	{
		return false;
	}

	if ((count < 1) || (row < 0) || (row > pParentItem->childCount()))
	{
		return false;
	}

	beginInsertRows(parent, row, row+count-1);

	bool bStatus=false;
	for (int i=0; i<count; i++)
	{
		bStatus=pParentItem->insertChild(row+i, createItem(pParentItem));
	}

	endInsertRows();

	return bStatus;
}

bool QTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	QTreeItem* pParentItem=parent.isValid()? itemFromIndex(parent):m_pRoot;

	if (pParentItem == NULL)
	{
		return false;
	}

	if ((count < 1) || (row < 0) || (row+count > pParentItem->childCount()))
	{
		return false;
	}

	beginRemoveRows(parent, row, row+count-1);

	pParentItem->removeChildren(row, count);

	endRemoveRows();

	return true;
}

QTreeItem*	QTreeModel::createItem(QTreeItem* parent) const
{
	int nColumn=(parent!=NULL? parent->columnCount():0);

	return new QTreeItem(0, nColumn, parent);
}

QTreeItem*	QTreeModel::itemFromIndex(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return NULL;
	}

	return static_cast<QTreeItem*>(index.internalPointer());
}

bool QTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	QTreeItem* pItem=itemFromIndex(index);
	if (pItem == NULL)
	{
		return false;
	}

	pItem->setData(index.column(), value, role);

	return true;
}
