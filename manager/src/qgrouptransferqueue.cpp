#include "qgrouptransferqueue.h"

#include <QMouseEvent>

#include "common.h"
#include "extern.h"

QGroupTransferQueue::QGroupTransferQueue(QWidget *parent)
	: QTreeView(parent)
{
	m_statusIcon[Waiting]=QIcon(":/images/waiting.png");
	m_statusIcon[Transfering]=QIcon(":/images/goup.png");
	m_statusIcon[Failed]=QIcon(":/images/error1.png");

	QStringList labels;
	labels<<tr("Source")<<tr("Type")<<tr("Size")<<tr("Target");

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

QGroupTransferQueue::~QGroupTransferQueue()
{

}

void QGroupTransferQueue::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

void QGroupTransferQueue::startSynchronize()
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

void QGroupTransferQueue::appendItem(const QGroupTransferQueue::TransferQueueItem& item)
{
	int index=m_pModel->rowCount();
	insertItem(index, item);
}

void QGroupTransferQueue::insertItem(int index, const QGroupTransferQueue::TransferQueueItem& item)
{
	index=index>0? index:0;
	index=index<m_pModel->rowCount()? index:m_pModel->rowCount();

	m_pModel->insertRow(index);

	m_pModel->itemFromIndex(m_pModel->index(index, 0))->setEditable(false);
	m_pModel->setData(m_pModel->index(index, 0), 
		item.sourceHost.hostName+":"+appendPathName(item.source, item.file.name, item.isTop));

	QVariant var;
	var.setValue(item);
	m_pModel->setData(m_pModel->index(index, 0), var, ItemDataRole);

	m_pModel->itemFromIndex(m_pModel->index(index, 1))->setEditable(false);
	m_pModel->setData(m_pModel->index(index, 1), item.file.typeDesc);

	m_pModel->itemFromIndex(m_pModel->index(index, 2))->setEditable(false);
	m_pModel->itemFromIndex(m_pModel->index(index, 2))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(m_pModel->index(index, 2), fileSize(item.file.size));

	m_pModel->itemFromIndex(m_pModel->index(index, 3))->setEditable(false);
	m_pModel->setData(m_pModel->index(index, 3),
		item.targetHost.hostName+":"+appendPathName(item.target, item.file.name, item.isTop));

	setItemStatus(index, Waiting);

	emit synchronize();
}

void QGroupTransferQueue::removeItem(int row)
{
	if (row>=0 && row<m_pModel->rowCount())
	{
		m_pModel->removeRow(row);
	}
}

void QGroupTransferQueue::setItemStatus(int row, QGroupTransferQueue::TransferStatus status)
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
		m_pModel->setData(m_pModel->index(row, 0), status, StatusRole);

		QIcon icon;

		TransferQueueItem item=qvariant_cast<TransferQueueItem>(m_pModel->data(m_pModel->index(row, 0), ItemDataRole));

		icon=addIconToIcon(m_statusIcon[status], fileIcon(item.file.type), 0);

		m_pModel->setData(m_pModel->index(row, 0), icon, Qt::DecorationRole);

		if (status == Waiting)
		{
			emit synchronize();
		}
	}
}

int QGroupTransferQueue::itemStatus(int row)
{
	int status=InvalidStatus;
	if (row >= 0 && row < taskCount())
	{
		status=m_pModel->data(m_pModel->index(row, 0), StatusRole).toInt();
	}

	return status;
}

int QGroupTransferQueue::fetchTask(QGroupTransferQueue::TransferQueueItem& item)
{
	int row=-1;

	int nRows=m_pModel->rowCount();
	for (int i=0; i<nRows; i++)
	{
		if (m_pModel->data(m_pModel->index(i, 0), StatusRole).toInt() == Waiting)
		{
			row=i;

			item=qvariant_cast<TransferQueueItem>(m_pModel->data(m_pModel->index(row, 0), ItemDataRole));

			setItemStatus(i, Transfering);

			break;
		}
	}

	return row;
}

QString QGroupTransferQueue::appendPathName(const QString path, const QString name, bool isTop)
{
	return isTop? name:appendPath(path, name);
}

void QGroupTransferQueue::mousePressEvent(QMouseEvent* event)
{
	QTreeView::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		emit mouseRightButtonPress(event->pos());
	}
}

QModelIndexList QGroupTransferQueue::selectedRows()
{
	QItemSelectionModel* pModel=selectionModel();
	if (pModel == NULL)
	{
		return QModelIndexList();
	}

	return pModel->selectedRows();
}

int QGroupTransferQueue::taskCount()
{
	return m_pModel->rowCount();
}
