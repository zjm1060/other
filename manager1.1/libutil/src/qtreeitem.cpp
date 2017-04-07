#include <QStringList>

#include "qtreeitem.h"

#include "define.h"

QTreeItem::QTreeItem(QTreeItem *parent)
{
	m_pParent=parent;
}

QTreeItem::QTreeItem(int childCount, int column, QTreeItem *parent)
{
	childCount=childCount>0? childCount:0;
	column=column>0? column:0;

	m_pParent=parent;

	m_itemData.resize(column);

	for (int i=0; i<childCount; i++)
	{
		appendChild(new QTreeItem(0, column, this));
	}
}

QTreeItem::~QTreeItem()
{
	setChildCount(0);
}

void QTreeItem::appendChild(QTreeItem* item)
{
	m_childItems.append(item);
}

bool QTreeItem::insertChild(int row, QTreeItem* child)
{
	if (child == NULL)
	{
		return false;
	}

	m_childItems.insert(row, child);

	return true;
}

void QTreeItem::removeChildren(int row, int count)
{
	QMutableListIterator<QTreeItem*> it(m_childItems);
	int nIndex=0;

	QTreeItem* pItem;
	while (it.hasNext())
	{
		pItem=it.next();

		if (nIndex >= row)
		{
			it.remove();

			pItem->setChildCount(0);

			SAFE_DELETE_CLASS(pItem);
		}

		if ((++nIndex) >= row+count)
		{
			break;
		}
	}
}

QTreeItem* QTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int QTreeItem::childCount() const
{
    return m_childItems.count();
}

void QTreeItem::setChildCount(int count)
{
	if (count < 0 || m_childItems.size() == count)
	{
		return;
	}

	if (m_childItems.size() < count)
	{
		int nIndex=m_childItems.size()-1;
		for (;nIndex<count; nIndex++)
		{
			appendChild(new QTreeItem(0, m_pParent->columnCount(), m_pParent));
		}
	}
	else
	{
		QMutableListIterator<QTreeItem*> it(m_childItems);
		QTreeItem* pItem;
		int nIndex=0;
		while (it.hasNext())
		{
			pItem=it.next();
			if (nIndex >= count)
			{
				it.remove();

				if (pItem != NULL)
				{
					pItem->m_pParent=NULL;
					pItem->setChildCount(0);

					SAFE_DELETE_CLASS(pItem);
				}
			}

			nIndex++;
		}
	}
}

int QTreeItem::columnCount() const
{
    return m_itemData.count();
}

void QTreeItem::setData(int column, const QVariant& value, int role)
{
	if (column<0 || column > m_itemData.size())
	{
		return;
	}

	QMap<int, QVariant>& mapItemData=m_itemData[column];

	mapItemData.insert(role, value);
}

QVariant QTreeItem::data(int column, int role) const
{
	if (column<0 || column >= m_itemData.size())
	{
		return QVariant();
	}

	const QMap<int, QVariant>& mapItemData=m_itemData[column];

    return mapItemData.value(role);
}

QTreeItem* QTreeItem::parent()
{
    return m_pParent;
}

int QTreeItem::row() const
{
    if (m_pParent != NULL)
	{
        return m_pParent->m_childItems.indexOf(const_cast<QTreeItem*>(this));
	}

    return 0;
}
