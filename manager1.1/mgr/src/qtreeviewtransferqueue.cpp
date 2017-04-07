#include "qtreeviewtransferqueue.h"

#include <QMouseEvent>

#include "common.h"
#include "qutildef.h"
#include "qtransferqueuedelegate.h"

QTreeViewTransferQueue::QTreeViewTransferQueue(QWidget *parent)
	: QTreeView(parent)
{
	qRegisterMetaType<QReceiverTransferFile::TransferQueueItem>("QReceiverTransferFile::TransferQueueItem");

	m_icon[Download]=QIcon(":/images/godown.png");
	m_icon[Upload]=QIcon(":/images/goup.png");

	m_statusIcon[Failed]=QIcon(":/images/error1.png");

	QStringList labels;
	labels<<tr("Name")<<tr("Type")<<tr("Process")<<tr("Target");

	m_pModel=new QStandardItemModel(0, labels.size(), this);
	m_pModel->setHorizontalHeaderLabels(labels);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pModel);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	setItemDelegate(new QTransferQueueDelegate(this));

	setColumnWidth(QTransferQueueDelegate::NameTitle, 200);
	setColumnWidth(QTransferQueueDelegate::TypeTitle, 80);
	setColumnWidth(QTransferQueueDelegate::ProcessTitle, 150);
	setColumnWidth(QTransferQueueDelegate::TargetTitle, 150);

	setItemsExpandable(true);
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

void QTreeViewTransferQueue::appendItem(const QReceiverTransferFile::TransferQueueItem& item)
{
	insertItem(m_pModel->rowCount(), item);
}

void QTreeViewTransferQueue::insertItem(int row, const QReceiverTransferFile::TransferQueueItem& item)
{
	Q_ASSERT(item.direction >=0 && item.direction < DirectionEnd);
	
	if (row>m_pModel->rowCount())
	{
		row=m_pModel->rowCount();
	}

	m_pModel->insertRow(row);

	QVariant var;
	var.setValue(item);

	m_pModel->itemFromIndex(m_pModel->index(row, QTransferQueueDelegate::NameTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::NameTitle), QReceiverTransferFile::appendPathName(item.source, item.name, item.isTop));
	m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::NameTitle), var, Qt::UserRole);
	m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::NameTitle), Waiting, Qt::UserRole+1);

	m_pModel->itemFromIndex(m_pModel->index(row, QTransferQueueDelegate::TypeTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::TypeTitle), item.typeDesc);

	m_pModel->itemFromIndex(m_pModel->index(row, QTransferQueueDelegate::ProcessTitle))->setEditable(false);
	m_pModel->itemFromIndex(m_pModel->index(row, QTransferQueueDelegate::ProcessTitle))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::ProcessTitle), 0, Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(row, QTransferQueueDelegate::TargetTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::TargetTitle), QReceiverTransferFile::appendPathName(item.target, item.name, item.isTop));

	setItemStatus(row, Waiting);

	emit synchronize();
}

void QTreeViewTransferQueue::removeItem(int row)
{
	if (row>=0 && row<m_pModel->rowCount())
	{
		m_pModel->removeRow(row);
	}
}

void QTreeViewTransferQueue::setItemFileName(int row, const QString& name)
{
	if (row >= 0 && row < taskCount())
	{
		QVariant var=m_pModel->data(m_pModel->index(row, QTransferQueueDelegate::NameTitle), Qt::UserRole);
		QReceiverTransferFile::TransferQueueItem item=var.value<QReceiverTransferFile::TransferQueueItem>();

		m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::TargetTitle), QReceiverTransferFile::appendPathName(item.target, name, item.isTop));
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
		QVariant var=m_pModel->data(m_pModel->index(row, QTransferQueueDelegate::NameTitle), Qt::UserRole);
		QReceiverTransferFile::TransferQueueItem item=var.value<QReceiverTransferFile::TransferQueueItem>();

		m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::NameTitle), status, Qt::UserRole+1);

		QIcon icon;
		int direction=item.direction;
		FileInfo info;
		info.name=item.name;
		info.type=item.type;
		info.size=item.size;

		icon=addIconToIcon(m_icon[direction], fileIcon(info), Qt::BottomRightCorner);

		if (status == Failed)
		{
			icon=addIconToIcon(m_statusIcon[status], icon, Qt::TopLeftCorner);
		}

		m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::NameTitle), icon, Qt::DecorationRole);

		if (status == Waiting)
		{
			emit synchronize();
		}
	}
}

QString QTreeViewTransferQueue::itemFileName(int row)
{
	QString strFileName;

	if (row >= 0 && row < taskCount())
	{
		strFileName=m_pModel->data(m_pModel->index(row, QTransferQueueDelegate::TargetTitle)).toString();
	}

	return strFileName;
}

int QTreeViewTransferQueue::itemStatus(int row)
{
	int status=InvalidStatus;
	if (row >= 0 && row < taskCount())
	{
		status=m_pModel->data(m_pModel->index(row, QTransferQueueDelegate::NameTitle), Qt::UserRole+1).toInt();
	}

	return status;
}

void QTreeViewTransferQueue::addItemProcess(int row, quint64 size)
{
	if (row >= 0 && row < taskCount())
	{
		size=m_pModel->data(m_pModel->index(row, QTransferQueueDelegate::ProcessTitle), Qt::UserRole).toULongLong()+size;
		m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::ProcessTitle), size, Qt::UserRole);
	}
}

void QTreeViewTransferQueue::setItemFileSize(int row, quint64 size)
{
	if (row >= 0 && row < taskCount())
	{
		QVariant var=m_pModel->data(m_pModel->index(row, QTransferQueueDelegate::NameTitle), Qt::UserRole);
		QReceiverTransferFile::TransferQueueItem item=var.value<QReceiverTransferFile::TransferQueueItem>();

		item.size=size;

		var.setValue(item);

		m_pModel->setData(m_pModel->index(row, QTransferQueueDelegate::NameTitle), var, Qt::UserRole);
	}
}

int QTreeViewTransferQueue::fetchTask(QReceiverTransferFile::TransferQueueItem& item)
{
	int row=-1;

	int nRows=m_pModel->rowCount();
	for (int i=0; i<nRows; i++)
	{
		if (itemStatus(i) == Waiting)
		{
			row=i;

			item=m_pModel->data(m_pModel->index(row, QTransferQueueDelegate::NameTitle), Qt::UserRole).value<QReceiverTransferFile::TransferQueueItem>();

			setItemStatus(i, Transfering);

			break;
		}
	}

	return row;
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
