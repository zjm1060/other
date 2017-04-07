#include "qtreeviewgrouptransferqueue.h"

#include <QMouseEvent>

#include "common.h"
#include "qtreeviewgrouptransferdelegate.h"
#include "qtreeviewsyncdbfile.h"
#include "qreceivertransferfile.h"

QTreeViewGroupTransferQueue::QTreeViewGroupTransferQueue(QWidget *parent)
	: QTreeView(parent)
{
	QStringList labels;
	labels<<tr("Name")<<tr("Type")<<tr("Status")<<tr("Process")<<tr("Target");

	m_pModel=new QTreeModel(labels.size(), this);
	m_pModel->setHorizontalHeaderLabels(labels);

//	setRootIsDecorated(false);
//	setAlternatingRowColors(true);
	setModel(m_pModel);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	setItemDelegate(new QTreeViewGroupTransferDelegate(this));

	setColumnWidth(NameTitle, 180);
	setColumnWidth(TypeTitle, 80);
	setColumnWidth(StatusTitle, 80);
	setColumnWidth(ProcessTitle, 150);
	setColumnWidth(TargetTitle, 250);

	showAbsolutePath(true);
}

QTreeViewGroupTransferQueue::~QTreeViewGroupTransferQueue()
{
	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);

	QAbstractItemDelegate* pDelegate=itemDelegate();
	if (pDelegate != NULL)
	{
		setItemDelegate(NULL);
		SAFE_DELETE_CLASS(pDelegate);
	}
}

void QTreeViewGroupTransferQueue::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

void QTreeViewGroupTransferQueue::showAbsolutePath(bool show)
{
	m_bShowAbsolutePath=show;
}

QModelIndex QTreeViewGroupTransferQueue::appendPeerItem(const QTreeViewGroupTransferQueue::TransferQueuePeerItem& peer)
{
	int nNumRow=m_pModel->rowCount();
	m_pModel->insertRow(nNumRow);

	QModelIndex index=m_pModel->index(nNumRow, NameTitle);

	QString strTitle;
	if (!peer.target.hostName.isEmpty())
	{
		strTitle=peer.target.hostName;
	}
	else
	{
		Server server;
		QServerManager::findFirstServer(peer.target, &server);
		strTitle=server.addr;
	}

	m_pModel->setData(index, strTitle, Qt::DisplayRole);
	m_pModel->setData(index, ServerRole, TransferRole);
	m_pModel->setData(index, qVariantFromValue(peer), TransferDataRole);
	m_pModel->setData(index, Waiting, TransferStatusRole);

	return index;
}

QModelIndex QTreeViewGroupTransferQueue::appendBackupItem(const TransferQueueBackupItem& item)
{
	int row=m_pModel->rowCount();
	m_pModel->insertRow(row);

	QModelIndex index=m_pModel->index(row, NameTitle);

	m_pModel->setData(index, tr("Backup"), Qt::DisplayRole);
	m_pModel->setData(index, BackUpRole, TransferRole);
	m_pModel->setData(index, qVariantFromValue(item), TransferDataRole);

	index=m_pModel->index(row, ProcessTitle, index.parent());
	m_pModel->setData(index, 0, TransferProcessRole);

	index=m_pModel->index(row, TargetTitle, index.parent());
	m_pModel->setData(index, item.target, Qt::DisplayRole);

	setItemTransferStatus(row, Waiting);

	return index;
}

void QTreeViewGroupTransferQueue::appendCommandItem(const QTreeViewGroupTransferQueue::TransferQueueCommandItem& item, const QModelIndex& parent)
{
	int nRow=m_pModel->rowCount(parent);
	insertCommandItem(nRow, item, parent);
}

void QTreeViewGroupTransferQueue::insertCommandItem(int row, const QTreeViewGroupTransferQueue::TransferQueueCommandItem& item, const QModelIndex& parent)
{
	if (row>m_pModel->rowCount(parent))
	{
		row=m_pModel->rowCount(parent);
	}

	m_pModel->insertRow(row, parent);

	QModelIndex index=m_pModel->index(row, NameTitle, parent);
	m_pModel->setData(index, item.name, Qt::DisplayRole);
	m_pModel->setData(index, CommandRole, TransferRole);
	m_pModel->setData(index, qVariantFromValue(item), TransferDataRole);

	index=m_pModel->index(row, TypeTitle, parent);
	m_pModel->setData(index, tr("Command"), Qt::DisplayRole);

	index=m_pModel->index(row, ProcessTitle, parent);
	m_pModel->setData(index, 0, TransferProcessRole);

	setItemTransferStatus(row, Waiting, parent);
}

void QTreeViewGroupTransferQueue::appendItem(const QTreeViewGroupTransferQueue::TransferQueueFileItem& item, int status, const QModelIndex& parent)
{
	int nRow=m_pModel->rowCount(parent);
	insertItem(nRow, item, status, parent);
}

void QTreeViewGroupTransferQueue::insertItem(int row, const QTreeViewGroupTransferQueue::TransferQueueFileItem& item, int status, const QModelIndex& parent)
{
	if (row>m_pModel->rowCount(parent))
	{
		row=m_pModel->rowCount(parent);
	}

	m_pModel->insertRow(row, parent);

	QModelIndex index=m_pModel->index(row, NameTitle, parent);
	m_pModel->setData(index, m_bShowAbsolutePath? QReceiverTransferFile::appendPathName(item.source, item.file.name, item.isTop):item.file.name, Qt::DisplayRole);
	m_pModel->setData(index, FileRole, TransferRole);
	m_pModel->setData(index, qVariantFromValue(item), TransferDataRole);

	index=m_pModel->index(row, TypeTitle, parent);
	m_pModel->setData(index, item.file.typeDesc, Qt::DisplayRole);

	index=m_pModel->index(row, StatusTitle, parent);
	m_pModel->setData(index, status, Qt::DisplayRole);

	index=m_pModel->index(row, ProcessTitle, parent);
	m_pModel->setData(index, 0, TransferProcessRole);

	index=m_pModel->index(row, TargetTitle, parent);
	m_pModel->setData(index, QReceiverTransferFile::appendPathName(item.target, item.file.name, item.isTop), Qt::DisplayRole);

	setItemTransferStatus(row, Waiting, parent);
}

void QTreeViewGroupTransferQueue::removeItem(int row, const QModelIndex& parent)
{
	if (row>=0 && row<m_pModel->rowCount(parent))
	{
		m_pModel->removeRow(row, parent);

		if (m_pModel->rowCount(parent) == 0)
		{
			removeItem(parent.row(), parent.parent());
		}
	}
}

void QTreeViewGroupTransferQueue::setItemTransferStatus(int row, QTreeViewGroupTransferQueue::TransferStatus status, const QModelIndex& parent)
{
	if (status >= StatusEnd || row >= m_pModel->rowCount(parent) || row < 0)
	{
		return;
	}

	QModelIndex index=m_pModel->index(row, NameTitle, parent);
	int nRole=m_pModel->data(index, TransferRole).toInt();
	if (nRole == FileRole || nRole == BackUpRole || nRole == CommandRole)
	{
		if (status == Finished)
		{
			removeItem(row, parent);
		}
		else
		{
			m_pModel->setData(index, status, TransferStatusRole);

			update(index);

			if (status == Waiting)
			{
				emit synchronize();
			}
		}
	}
	else if (!parent.isValid() && nRole == ServerRole)
	{
		m_pModel->setData(index, status, TransferStatusRole);

		switch (status)
		{
		case Waiting:
			break;
		case Connecting:
			break;
		case Connected:
			break;
		case Unconneced:
			{
				int nNumRows=m_pModel->rowCount(index);
				for (int i=0; i<nNumRows; i++)
				{
					setItemTransferStatus(i, Failed, index);
				}
			}
			break;
		default:
			return;
		}

		update(index);
	}
}

int QTreeViewGroupTransferQueue::itemTransferStatus(int row, const QModelIndex& parent)
{
	int status=InvalidStatus;
	if (row >= 0 && row < taskCount(parent))
	{
		QModelIndex index=m_pModel->index(row, NameTitle, parent);
		status=m_pModel->data(index, TransferStatusRole).toInt();
	}

	return status;
}

void QTreeViewGroupTransferQueue::addItemProcess(int row, quint64 size, const QModelIndex& parent)
{
	if (row >= 0 && row < m_pModel->rowCount(parent))
	{
		QModelIndex index=m_pModel->index(row, NameTitle, parent);

		index=m_pModel->index(row, ProcessTitle, parent);
		size=m_pModel->data(index, TransferProcessRole).toULongLong()+size;
		m_pModel->setData(index, size, TransferProcessRole);

		update(index);
	}
}

void QTreeViewGroupTransferQueue::setItemProcess(int row, quint64 size, const QModelIndex& parent)
{
	if (row >= 0 && row < m_pModel->rowCount(parent))
	{
		QModelIndex index=m_pModel->index(row, NameTitle, parent);

		index=m_pModel->index(row, ProcessTitle, parent);
		m_pModel->setData(index, size, TransferProcessRole);

		update(index);
	}
}

QModelIndex QTreeViewGroupTransferQueue::fetchTask(int& role, QVariant& var, int& status)
{
	QModelIndex index;

	int nRows=m_pModel->rowCount();
	for (int i=0; i<nRows; i++)
	{
		QModelIndex pindex=m_pModel->index(i, NameTitle);
		
		int nTransferRole=m_pModel->data(pindex, TransferRole).toInt();
		if (nTransferRole != BackUpRole)
		{
			int nChildRows=m_pModel->rowCount(pindex);
			for (int j=0; j<nChildRows; j++)
			{
				QModelIndex cindex=m_pModel->index(j, NameTitle, pindex);
				if (m_pModel->data(cindex, TransferStatusRole).toInt() == Waiting)
				{
					index=cindex;

					role=m_pModel->data(cindex, TransferRole).toInt();
					if (role == FileRole)
					{
						QGroupTransferQueue::TransferQueueItem item;

						TransferQueuePeerItem peer=qvariant_cast<TransferQueuePeerItem>(m_pModel->data(pindex, TransferDataRole));
						TransferQueueFileItem file=qvariant_cast<TransferQueueFileItem>(m_pModel->data(index, TransferDataRole));

						item.isSlient=peer.isSlient;
						item.sourceHost=peer.source;
						item.isTop=file.isTop;
						item.format=file.format;
						item.source=file.source;
						item.file=file.file;
						item.targetHost=peer.target;
						item.target=file.target;

						var=qVariantFromValue(item);

						cindex=m_pModel->index(j, StatusTitle, pindex);
						status=m_pModel->data(cindex, Qt::DisplayRole).toInt();
						Q_ASSERT(status>=0 && status<ModifiedStatusEnd);
					}
					else if (role == CommandRole)
					{
						var=m_pModel->data(index, TransferDataRole);
					}

					setItemTransferStatus(j, Transfering, pindex);

					break;
				}
			}
		}
		else
		{
			role=BackUpRole;
			index=pindex;

			var=m_pModel->data(pindex, TransferDataRole);
		}

		if (index.isValid())
		{
			break;
		}
	}

	return index;
}

void QTreeViewGroupTransferQueue::mousePressEvent(QMouseEvent* event)
{
	QTreeView::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		emit mouseRightButtonPressed(event->pos());
	}
}

QModelIndexList QTreeViewGroupTransferQueue::selectedRows()
{
	QItemSelectionModel* pModel=selectionModel();
	if (pModel == NULL)
	{
		return QModelIndexList();
	}

	return pModel->selectedRows();
}

int QTreeViewGroupTransferQueue::taskCount(const QModelIndex& parent)
{
	return m_pModel->rowCount(parent);
}

QModelIndex QTreeViewGroupTransferQueue::index(int row, const QModelIndex& parent)
{
	if (row >=0 && row < m_pModel->rowCount(parent))
	{
		return m_pModel->index(row, 0, parent);
	}

	return QModelIndex();
}
