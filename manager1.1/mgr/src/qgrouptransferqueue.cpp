#include "qgrouptransferqueue.h"

#include <QMouseEvent>

#include "common.h"
#include "qgrouptransferqueuedelegate.h"
#include "qreceivertransferfile.h"

QGroupTransferQueue::QGroupTransferQueue(QWidget *parent)
	: QTreeView(parent)
{
	m_statusIcon[Waiting]=QIcon(":/images/waiting.png");
	m_statusIcon[Transfering]=QIcon(":/images/goup.png");
	m_statusIcon[Failed]=QIcon(":/images/error1.png");

	QStringList labels;
	labels<<tr("Name")<<tr("Type")<<tr("Process")<<tr("Target");

	m_pModel=new QStandardItemModel(0, labels.size(), this);
	m_pModel->setHorizontalHeaderLabels(labels);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pModel);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	setItemDelegate(new QGroupTransferQueueDelegate(this));

	setColumnWidth(QGroupTransferQueueDelegate::NameTitle, 200);
	setColumnWidth(QGroupTransferQueueDelegate::TypeTitle, 80);
	setColumnWidth(QGroupTransferQueueDelegate::ProcessTitle, 150);
	setColumnWidth(QGroupTransferQueueDelegate::TargetTitle, 150);
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

void QGroupTransferQueue::insertItem(int row, const QGroupTransferQueue::TransferQueueItem& item)
{
	if (row>m_pModel->rowCount())
	{
		row=m_pModel->rowCount();
	}

	m_pModel->insertRow(row);

	QVariant var;
	var.setValue(item);

	m_pModel->itemFromIndex(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle), 
		item.sourceHost.hostName+"@"+QReceiverTransferFile::appendPathName(item.source, item.file.name, item.isTop));
	m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle), var, Qt::UserRole);
	m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle), Waiting, Qt::UserRole+1);

	m_pModel->itemFromIndex(m_pModel->index(row, QGroupTransferQueueDelegate::TypeTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::TypeTitle), item.file.typeDesc);

	m_pModel->itemFromIndex(m_pModel->index(row, QGroupTransferQueueDelegate::ProcessTitle))->setEditable(false);
	m_pModel->itemFromIndex(m_pModel->index(row, QGroupTransferQueueDelegate::ProcessTitle))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::ProcessTitle), 0, Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(row, QGroupTransferQueueDelegate::TargetTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::TargetTitle), 
		item.targetHost.hostName+"@"+QReceiverTransferFile::appendPathName(item.target, item.file.name, item.isTop));

	setItemStatus(row, Waiting);

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
		m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle), status, Qt::UserRole+1);

		QIcon icon;

		TransferQueueItem item=qvariant_cast<TransferQueueItem>(m_pModel->data(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle), Qt::UserRole));

		FileInfo info;
		info.name=item.file.name;
		info.type=item.file.type;
		
		icon=addIconToIcon(m_statusIcon[status], fileIcon(info), Qt::BottomRightCorner);

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
		status=m_pModel->data(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle), Qt::UserRole+1).toInt();
	}

	return status;
}

void QGroupTransferQueue::addItemProcess(int row, quint64 size)
{
	if (row >= 0 && row < m_pModel->rowCount())
	{
		size=m_pModel->data(m_pModel->index(row, QGroupTransferQueueDelegate::ProcessTitle), Qt::UserRole).toULongLong()+size;
		m_pModel->setData(m_pModel->index(row, QGroupTransferQueueDelegate::ProcessTitle), size, Qt::UserRole);
	}
}

int QGroupTransferQueue::fetchTask(QGroupTransferQueue::TransferQueueItem& item)
{
	int row=-1;

	int nRows=m_pModel->rowCount();
	for (int i=0; i<nRows; i++)
	{
		if (m_pModel->data(m_pModel->index(i, QGroupTransferQueueDelegate::NameTitle), Qt::UserRole+1).toInt() == Waiting)
		{
			row=i;

			item=qvariant_cast<TransferQueueItem>(m_pModel->data(m_pModel->index(row, QGroupTransferQueueDelegate::NameTitle), Qt::UserRole));

			setItemStatus(i, Transfering);

			break;
		}
	}

	return row;
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

bool operator==(const QGroupTransferQueue::TransferQueueItem& source, const QGroupTransferQueue::TransferQueueItem& target)
{
	if (source.sourceHost != target.sourceHost || source.source != target.source || source.file != target.file || source.targetHost != target.targetHost
		|| source.target != target.target)
	{
		return false;
	}

	return true;
}

bool operator!=(const QGroupTransferQueue::TransferQueueItem& source, const QGroupTransferQueue::TransferQueueItem& target)
{
	return !(source==target);
}
