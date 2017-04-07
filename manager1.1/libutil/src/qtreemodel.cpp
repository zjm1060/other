#include <QtGui>

#include "qtreeitem.h"
#include "qtreemodel.h"

#include "define.h"

QTreeModel::QTreeModel(int column, QObject *parent)
    : QAbstractItemModel(parent)
{
	qRegisterMetaType<Qt::Orientation>("Qt::Orientation");

    m_pRoot=new QTreeItem(0, column);
	m_pItems.insert(qHash(m_pRoot), m_pRoot);
}

QTreeModel::~QTreeModel()
{
	m_pRoot->setChildCount(0);
	m_pItems.clear();

    SAFE_DELETE_CLASS(m_pRoot);
}

void QTreeModel::setHorizontalHeaderLabels(const QStringList& labels)
{
	if (m_pRoot != NULL)
	{
		for (int i=0; i<labels.size(); i++)
		{
			m_pRoot->setData(i, labels[i]);
		}
	}
}

const QStringList QTreeModel::horizontalHeaderLabels() const
{
	QStringList labels;

	if (m_pRoot != NULL)
	{
		for (int i=0; i<columnCount(); i++)
		{
			labels.append(m_pRoot->data(i).toString());
		}
	}

	return labels;
}

QVariant QTreeModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid())
	{
		QTreeItem* pItem=itemFromIndex(index);
		if (pItem != NULL && pItem->columnCount() > index.column())
		{
			return pItem->data(index.column(), role);
		}
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

	QTreeItem* pParentItem=itemFromIndex(parent);

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

    if (pParentItem == NULL || pParentItem == m_pRoot)
	{
        return QModelIndex();
	}

    return createIndex(pParentItem->row(), 0, pParentItem);
}

int QTreeModel::rowCount(const QModelIndex &parent) const
{
    QTreeItem* pItem=itemFromIndex(parent);

	return pItem!=NULL? pItem->childCount():0;
}

int QTreeModel::columnCount(const QModelIndex &parent) const
{
	QTreeItem* pItem=itemFromIndex(parent);

	return pItem!=NULL? pItem->columnCount():0;
}

bool QTreeModel::setRowCount(int rows, const QModelIndex& index)
{
	if (rows < 0)
	{
		return false;
	}

	QTreeItem* pItem=itemFromIndex(index);

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
	if ((count <= 0) || (row < 0))
	{
		return false;
	}

	QTreeItem* pParentItem=itemFromIndex(parent);

	if (pParentItem == NULL)
	{
		return false;
	}

	row=row < pParentItem->childCount()? row:pParentItem->childCount();

	beginInsertRows(parent, row, row+count-1);

	bool bStatus=true;
	QTreeItem* pItem;
	for (int i=0; i<count; i++)
	{
		pItem=createItem(pParentItem);
		bStatus&=pParentItem->insertChild(row+i, pItem);

		m_pItems.insertMulti(qHash(pItem), pItem);
	}

	endInsertRows();

	return bStatus;
}

bool QTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if ((count <= 0) || (row < 0))
	{
		return false;
	}

	QTreeItem* pParentItem=itemFromIndex(parent);

	if (pParentItem == NULL)
	{
		return false;
	}

	count=count>pParentItem->childCount()-row? count:pParentItem->childCount()-row;

	beginRemoveRows(parent, row, row+count-1);

	for (int i=0; i<count; i++)
	{
		removePersistItem(row+i, parent);
	}

	pParentItem->removeChildren(row, count);

	endRemoveRows();

	return true;
}

bool QTreeModel::removeRow(int row, const QModelIndex &parent)
{
	if (row < 0 || row>=rowCount(parent))
	{
		return false;
	}

	QTreeItem* pParentItem=itemFromIndex(parent);

	if (pParentItem == NULL)
	{
		return false;
	}

	beginRemoveRows(parent, row, row);

	removePersistItem(row, parent);

	pParentItem->removeChildren(row, 1);

	endRemoveRows();

	return true;
}

void QTreeModel::removePersistItem(int row, const QModelIndex& parent)
{
	QModelIndex cindex=index(row, 0, parent);

	QTreeItem* pItem=itemFromIndex(cindex);
	if (pItem != NULL)
	{
		int nNumRow=pItem->childCount();
		for (int i=0; i<nNumRow; i++)
		{
			removePersistItem(i, cindex);
		}

		quint32 nKey=qHash(pItem);
		QHash<quint32, QTreeItem*>::iterator it=m_pItems.find(nKey);
		while (it != m_pItems.end() && it.key() == nKey)
		{
			if (it.value() == pItem)
			{
				m_pItems.erase(it);

				break;
			}

			it++;
		}
	}
}

bool QTreeModel::hasChildren(const QModelIndex& parent) const
{
	QTreeItem* pParentItem=itemFromIndex(parent);
	if (pParentItem != NULL)
	{
		return pParentItem->childCount()>0;
	}

	return false;
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
		return m_pRoot;
	}

	QTreeItem* pItem=static_cast<QTreeItem*>(index.internalPointer());
	QList<QTreeItem*> items=m_pItems.values(qHash(pItem));
	if (!items.contains(pItem))
	{
		return NULL;
	}

	return pItem;
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
