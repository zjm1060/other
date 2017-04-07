#include "qtreeviewtransferqueue.h"

#include <QMouseEvent>

#include "common.h"
#include "extern.h"

QTreeViewTransferQueue::QTreeViewTransferQueue(QWidget *parent)
	: QTreeView(parent)
{
	m_icon[Download]=QIcon(":/images/godown.png");
	m_icon[Upload]=QIcon(":/images/goup.png");

	m_statusIcon[Waiting]=QIcon(":/images/waiting.png");
	m_statusIcon[Failed]=QIcon(":/images/error1.png");

	QStringList labels;
	labels<<tr("Name")<<tr("Type")<<tr("Size")<<tr("Target");

	m_pModel=new QStandardItemModel(0, labels.size(), this);
	m_pModel->setHorizontalHeaderLabels(labels);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pModel);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	setColumnWidth(0, 200);
	setColumnWidth(1, 80);
	setColumnWidth(2, 50);
	setColumnWidth(3, 150);
}

QTreeViewTransferQueue::~QTreeViewTransferQueue()
{

}

void QTreeViewTransferQueue::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

void QTreeViewTransferQueue::startSynchronize()
{
	bool bSync=false;

	int rows=taskCount();
	if (rows > 0)
	{
		for (int i=0; i<rows; i++)
		{
			if (itemStatus(i) == Waiting)
			{
				bSync=true;
				break;
			}
		}
	}

	if (bSync)
	{
		emit synchronize();
	}
}

void QTreeViewTransferQueue::appendItem(const TransferQueueItem& item)
{
	if (item.direction < 0 || item.direction >= DirectionEnd)
	{
		return;
	}

	int index=m_pModel->rowCount();
	m_pModel->insertRow(index);

	m_pModel->itemFromIndex(m_pModel->index(index, 0))->setEditable(false);
	m_pModel->setData(m_pModel->index(index, 0), appendPathName(item.source, item.name, item.isTop));
	m_pModel->setData(m_pModel->index(index, 0), item.source, Qt::UserRole);
	m_pModel->setData(m_pModel->index(index, 0), item.name, Qt::UserRole+1);
	m_pModel->setData(m_pModel->index(index, 0), item.isTop, Qt::UserRole+2);
	m_pModel->setData(m_pModel->index(index, 0), item.direction, Qt::UserRole+3);

	m_pModel->itemFromIndex(m_pModel->index(index, 1))->setEditable(false);
	m_pModel->setData(m_pModel->index(index, 1), item.typeDesc);
	m_pModel->setData(m_pModel->index(index, 1), item.type, Qt::UserRole);
	m_pModel->setData(m_pModel->index(index, 1), item.format, Qt::UserRole+1);
	m_pModel->setData(m_pModel->index(index, 1), Waiting, Qt::UserRole+2);

	m_pModel->itemFromIndex(m_pModel->index(index, 2))->setEditable(false);
	m_pModel->itemFromIndex(m_pModel->index(index, 2))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(m_pModel->index(index, 2), fileSize(item.size));
	m_pModel->setData(m_pModel->index(index, 2), item.size, Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(index, 3))->setEditable(false);
	m_pModel->setData(m_pModel->index(index, 3), appendPathName(item.target, item.name, item.isTop));
	m_pModel->setData(m_pModel->index(index, 3), item.target, Qt::UserRole);

	setItemStatus(index, Waiting);

	emit synchronize();
}

void QTreeViewTransferQueue::removeItem(int row)
{
	if (row>=0 && row<m_pModel->rowCount())
	{
		m_pModel->removeRow(row);
	}
}

void QTreeViewTransferQueue::setItemStatus(int row, QTreeViewTransferQueue::TransferStatus status)
{
	if (status >= StatusEnd || row >= m_pModel->rowCount())
	{
		return;
	}

	if (status == Finished)
	{
		m_pModel->removeRow(row);
	}
	else
	{
		m_pModel->setData(m_pModel->index(row, 1), status, Qt::UserRole+2);

		QIcon icon;
		int direction=m_pModel->data(m_pModel->index(row, 0), Qt::UserRole+3).toInt();
		if (direction == Upload)
		{
			QString source=m_pModel->data(m_pModel->index(row, 0), Qt::UserRole).toString();
			QString name=m_pModel->data(m_pModel->index(row, 0), Qt::UserRole+1).toString();
			bool isTop=m_pModel->data(m_pModel->index(row, 0), Qt::UserRole+2).toBool();

			icon=addIconToIcon(m_icon[direction], m_provider.icon(QFileInfo(appendPathName(source, name, isTop))), 0);
		}
		else
		{
			int type=m_pModel->data(m_pModel->index(row, 1), Qt::UserRole).toInt();

			icon=addIconToIcon(m_icon[direction], fileIcon(type), 0);
		}

		icon=addIconToIcon(m_statusIcon[status], icon, 1);

		m_pModel->setData(m_pModel->index(row, 0), icon, Qt::DecorationRole);

		if (status == Waiting)
		{
			emit synchronize();
		}
	}
}

int QTreeViewTransferQueue::itemStatus(int row)
{
	int status=InvalidStatus;
	if (row >= 0 && row < taskCount())
	{
		status=m_pModel->data(m_pModel->index(row, 1), Qt::UserRole+2).toInt();
	}

	return status;
}

int QTreeViewTransferQueue::fetchTask(QTreeViewTransferQueue::TransferQueueItem& item)
{
	int row=-1;

	int nRows=m_pModel->rowCount();
	for (int i=0; i<nRows; i++)
	{
		if (m_pModel->data(m_pModel->index(i, 1), Qt::UserRole+2).toInt() == Waiting)
		{
			row=i;

			item.source=m_pModel->data(m_pModel->index(i, 0), Qt::UserRole).toString();
			item.name=m_pModel->data(m_pModel->index(i, 0), Qt::UserRole+1).toString();
			item.isTop=m_pModel->data(m_pModel->index(i, 0), Qt::UserRole+2).toBool();
			item.direction=m_pModel->data(m_pModel->index(i, 0), Qt::UserRole+3).toInt();

			item.typeDesc=m_pModel->data(m_pModel->index(i, 1), Qt::EditRole).toString();
			item.type=m_pModel->data(m_pModel->index(i, 1), Qt::UserRole).toInt();
			item.format=m_pModel->data(m_pModel->index(i, 1), Qt::UserRole+1).toInt();

			setItemStatus(i, Transfering);

			item.size=m_pModel->data(m_pModel->index(i, 2), Qt::UserRole).toULongLong();

			item.target=m_pModel->data(m_pModel->index(i, 3), Qt::UserRole).toString();

			break;
		}
	}

	return row;
}

QString QTreeViewTransferQueue::appendPathName(const QString path, const QString name, bool isTop)
{
	return isTop? name:appendPath(path, name);
}

void QTreeViewTransferQueue::mousePressEvent(QMouseEvent* event)
{
	QTreeView::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		emit mouseRightButtonPress(event->pos());
	}
}

QModelIndexList QTreeViewTransferQueue::selectedRows()
{
	QItemSelectionModel* pModel=selectionModel();
	if (pModel == NULL)
	{
		return QModelIndexList();
	}

	return pModel->selectedRows();
}

int QTreeViewTransferQueue::taskCount()
{
	return m_pModel->rowCount();
}
