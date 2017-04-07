#include "qlistviewcontrolledserver.h"

#include "common.h"
#include "qreceivertransferfile.h"
#include "qglobalinfo.h"

QListViewControlledServer::QListViewControlledServer(QWidget *parent)
	: QListViewHorizontalIcon(parent)
{
	m_pixmap[HostIcon]=findPixmap(":/images/host.png").scaled(QSize(LIST_VIEW_ICON_SIZE, LIST_VIEW_ICON_SIZE));
	m_pixmap[HostQuitIcon]=findPixmap(":/images/host_quit.png").scaled(QSize(LIST_VIEW_ICON_SIZE, LIST_VIEW_ICON_SIZE));
	m_pixmap[CheckIcon]=findPixmap(":/images/checkmark.png").scaled(QSize(LIST_VIEW_MARK_ICON_SIZE, LIST_VIEW_MARK_ICON_SIZE));

	setSelectionMode(QAbstractItemView::SingleSelection);

	QStringList labels;
	labels<<tr("Server");

	m_pModel=new QStandardItemModel(0, labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	setModel(m_pModel);

	setMouseTracking(true);
}

QListViewControlledServer::~QListViewControlledServer()
{
	setModel(NULL);

	SAFE_DELETE_CLASS(m_pModel);
}

void QListViewControlledServer::entered(const QModelIndex& index)
{
	if (m_pModel == NULL)
	{
		return;
	}

	if (index.isValid())
	{
		bool bSelect=m_pModel->data(index, ItemUserDataSelectRole).toBool();
		QString strMessage;
		if (bSelect)
		{
			if (m_pModel->data(index, ItemUserDataStatusRole).toInt() > 0)
			{
				strMessage=tr("The remote process has been exited!");

				emit sendTips(QReceiverTransferFile::Warning, strMessage);
			}
			else
			{
				emit sendTips(QReceiverTransferFile::Information, "");
			}
		}
		else
		{
			strMessage=tr("Click the left mouse button to switch to the sender.");

			emit sendTips(QReceiverTransferFile::Information, strMessage);
		}
	}
	else
	{
		emit sendTips(QReceiverTransferFile::Information, "");
	}
}

void QListViewControlledServer::mouseMoveEvent(QMouseEvent* event)
{
	QModelIndex index=indexAt(event->pos());

	entered(index);

	QListViewHorizontalIcon::mouseMoveEvent(event);
}

ServerInfo QListViewControlledServer::serverInfoFromIndex(const QModelIndex& index) const
{
	return qvariant_cast<ServerInfo>(m_pModel->data(index, ItemUserDataRole));
}

bool QListViewControlledServer::selectedServer(ServerInfo& info) const
{
	bool bSelect=false;

	int nRowCount=m_pModel->rowCount();

	for (int i=0; i<nRowCount; i++)
	{
		QModelIndex index=m_pModel->index(i, 0);
		bSelect=m_pModel->data(index, ItemUserDataSelectRole).toBool();
		if (bSelect)
		{
			info=serverInfoFromIndex(index);

			break;
		}
	}

	return bSelect;
}

void QListViewControlledServer::refreshServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRowCount=m_pModel->rowCount();

	QListIterator<QPair<ServerInfo, int> > it(servers);
	while (it.hasNext())
	{
		const QPair<ServerInfo, int>& peer=it.next();
		const ServerInfo& info=peer.first;

		if (info.count == 0)
		{
			continue;
		}

		bool bExist=false;
		for (int i=0; i<nRowCount; i++)
		{
			QModelIndex index=m_pModel->index(i, 0);
			ServerInfo inf=serverInfoFromIndex(index);

			if (info == inf)
			{
				bExist=true;

				m_pModel->setData(index, qVariantFromValue(info), ItemUserDataRole);
				m_pModel->setData(index, peer.second, ItemUserDataStatusRole);

				updateIcon(index);
				
				break;
			}
		}

		if (bExist)
		{
			continue;
		}

		int nRow=m_pModel->rowCount();

		m_pModel->insertRow(nRow);
		const QModelIndex&index=m_pModel->index(nRow, 0);
		m_pModel->itemFromIndex(index)->setEditable(false);

		QString strText=info.hostName;

		QString strTip=info.hostName;
		bool bTipFinished=false;

		QMapIterator<QString, QVector<Server> > iter(info.servers);
		while (iter.hasNext())
		{
			iter.next();

			const QVector<Server>& servers=iter.value();
			for (int i=0; i<servers.size(); i++)
			{
				const Server& server=servers[i];

				if (!bTipFinished)
				{
					bTipFinished=true;

					strText+="\n"+server.addr;
				}
				
				strTip+="\n"+server.addr;
			}
		}

		m_pModel->setData(index, strText, Qt::EditRole);

		m_pModel->setData(index, qVariantFromValue(info), ItemUserDataRole);
		m_pModel->setData(index, peer.second, ItemUserDataStatusRole);

		m_pModel->itemFromIndex(index)->setToolTip(strTip);

		setServerItemSelectStatus(index, false);
	}

	if (nRowCount == 0)
	{
		if (m_pModel->rowCount() > 0)
		{
			ServerInfo server=QGlobalInfo::instance()->local();
			nRowCount=m_pModel->rowCount();
			for (int i=0; i<nRowCount; i++)
			{
				ServerInfo info=serverInfoFromIndex(m_pModel->index(i, 0));
				if (info == server)
				{
					setCurrentIndex(m_pModel->index(i, 0));

					break;
				}
			}
		}
	}

	updateListView();
}

void QListViewControlledServer::setServerItemSelectStatus(const QModelIndex& index, bool select)
{
	if (m_pModel == NULL || !index.isValid())
	{
		return;
	}

	m_pModel->setData(index, select, ItemUserDataSelectRole);

	updateIcon(index);
}

int QListViewControlledServer::serverStatus(const QModelIndex& index) const
{
	if (m_pModel != NULL && index.isValid())
	{
		return m_pModel->data(index, ItemUserDataStatusRole).toInt();
	}

	return 1;
}

void QListViewControlledServer::updateIcon(const QModelIndex& index)
{
	QPixmap pixmap;
	if (m_pModel->data(index, ItemUserDataStatusRole).toInt() == 0)
	{
		pixmap=m_pixmap[HostIcon];
	}
	else
	{
		pixmap=m_pixmap[HostQuitIcon];
	}

	if (m_pModel->data(index, ItemUserDataSelectRole).toBool())
	{
		pixmap= addPixmapToPixmap(m_pixmap[CheckIcon], pixmap, Qt::BottomRightCorner);
	}

	m_pModel->setData(index, pixmap, Qt::DecorationRole);
}

void QListViewControlledServer::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous);

	if (m_pModel == NULL)
	{
		return;
	}

	if (serverStatus(current) > 0)
	{
		return;
	}

	setServerItemSelectStatus(m_indexSelected, false);
	setServerItemSelectStatus(current, true);

	m_indexSelected=current;

	emit currentChanged(current);
}
