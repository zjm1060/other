#include "qlistviewsyncfileserver.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include "common.h"
#include "qdlgedithost.h"
#include "qtipsdelegate.h"

QListViewSyncFileServer::QListViewSyncFileServer(const QString& record, QWidget *parent/* =0 */)
	: QListViewHorizontalIcon(parent)
	,m_bRemovable(false)
	,m_bEnabled(true)
	,m_bInit(false)
	,m_strRecord(record)
{
	m_pixHostQuit=findPixmap(":/images/host_quit.png").scaled(QSize(LIST_VIEW_ICON_SIZE, LIST_VIEW_ICON_SIZE));
	m_pixHost=findPixmap(":/images/host.png").scaled(QSize(LIST_VIEW_ICON_SIZE, LIST_VIEW_ICON_SIZE));
	m_pixCheck=findPixmap(":/images/checkmark.png").scaled(QSize(LIST_VIEW_MARK_ICON_SIZE, LIST_VIEW_MARK_ICON_SIZE));
	m_pixRemove=findPixmap(":/images/removemark.png").scaled(QSize(LIST_VIEW_LARGE_MARK_ICON_SIZE, LIST_VIEW_LARGE_MARK_ICON_SIZE));

	setSelectionMode(QAbstractItemView::ContiguousSelection);

	QStringList labels;
	labels<<tr("Server");

	m_pModel=new QStandardItemModel(0, labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	setModel(m_pModel);

	setMouseTracking(true);
//	connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(entered(const QModelIndex&)));

	QReadSelectionsFromXmlFile selector(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE), m_strRecord);
	if (selector.read())
	{
		m_selections=selector.result();
	}

	m_pMonitor=new QMonitorServersFile(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE), this);
	connect(m_pMonitor, SIGNAL(changed(const QList<Server>&)), this, SLOT(changed(const QList<Server>&)));

	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClickListViewItem(const QModelIndex&)));
}

QListViewSyncFileServer::~QListViewSyncFileServer()
{
	setModel(NULL);

	SAFE_DELETE_CLASS(m_pModel);

	saveServers();
}

void QListViewSyncFileServer::entered(const QModelIndex& index)
{
	if (m_pModel == NULL || !m_bEnabled)
	{
		emit sendTips(QReceiverTransferFile::Information, "");
		return;
	}

	if (!m_bRemovable)
	{
		if (index.isValid())
		{
			int nType=m_pModel->data(index, ItemRole).toInt();
			switch (nType)
			{
			case OriginItem:
				{
					bool bSelect=m_pModel->data(index, SelectedRole).toBool();
					if (bSelect)
					{
						emit sendTips(QReceiverTransferFile::Information, tr("Click the left mouse button to cancel the choice."));
					}
					else
					{
						emit sendTips(QReceiverTransferFile::Information, tr("Click the left mouse button to select the file receiving end."));
					}
				}
				break;
			case AddedItem:
				{
					bool bSelect=m_pModel->data(index, SelectedRole).toBool();
					QString strMessage;
					if (bSelect)
					{
						strMessage=tr("Click the left mouse button to cancel the choice.");
					}
					else
					{
						strMessage=tr("Click the left mouse button to select the file receiving end.");
					}

					strMessage+=tr("And double-click the right mouse button to enter the removal mode.");
					emit sendTips(QReceiverTransferFile::Information, strMessage);
				}
				break;
			case AddOp:
				{
					emit sendTips(QReceiverTransferFile::Information, tr("Click the left mouse button to add a new file receiving end."));
				}
				break;
			}
		}
		else
		{
			if (hasMantualServer())
			{
				emit sendTips(QReceiverTransferFile::Information, tr("Double-click the right mouse button to enter the removal mode."));
			}
			else
			{
				emit sendTips(QReceiverTransferFile::Information, "");
			}
		}
	}
	else
	{
		emit sendTips(QReceiverTransferFile::Information, tr("Double-click the right mouse button to exit the removal mode."));
	}
}

bool QListViewSyncFileServer::hasMantualServer() const
{
	return !m_servers.isEmpty();
}

void QListViewSyncFileServer::changed(const QList<Server>& servers)
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRowCount=m_pModel->rowCount()-1;
	for (int nRow=nRowCount-1; nRow>=0; nRow--)
	{
		QModelIndex index=m_pModel->index(nRow, 0);
		int nType=m_pModel->data(index, ItemRole).toInt();
		if (nType == AddedItem)
		{
			m_pModel->removeRow(nRow);
		}
	}

	m_servers=servers;

	if (m_pModel != NULL)
	{
		foreach (Server server, m_servers)
		{
			if (!findServer(server))
			{
				addMantualServerItemToServerView(m_pModel->rowCount()>0? m_pModel->rowCount()-1:0, server);
			}
		}
	}
}

QModelIndex QListViewSyncFileServer::findItem(const ServerInfo& info)
{
	if (m_pModel == NULL)
	{
		return QModelIndex();
	}

	int nRowCount=m_pModel->rowCount();
	int nRow=0;
	for (; nRow<nRowCount; nRow++)
	{
		const QModelIndex& index=m_pModel->index(nRow, 0);
		int nType=m_pModel->data(index, ItemRole).toInt();
		switch (nType)
		{
		case OriginItem:
			{
				ServerInfo server=qVariantValue<ServerInfo>(m_pModel->data(index, UserDataRole));

				if (info == server)
				{
					return index;
				}
			}
			break;
		case AddedItem:
			{
				Server server=qVariantValue<Server>(m_pModel->data(index, UserDataRole));

				QMapIterator<QString, QVector<Server> > it(info.servers);
				while (it.hasNext())
				{
					it.next();

					const QVector<Server>& servers=it.value();
					for (int i=0; i<servers.size(); i++)
					{
						const Server& s=servers[i];
						if (s.addr == server.addr)
						{
							return index;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}

	return QModelIndex();
}

bool QListViewSyncFileServer::isSelectedItem(const ServerInfo& info)
{
	Q_ASSERT(info.count > 0);

	QList<QString> lstMac=info.servers.keys();
	foreach (QString strMac, lstMac)
	{
		if (m_selections.contains(strMac))
		{
			return true;
		}
	}

	return false;
}

void QListViewSyncFileServer::refreshServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (m_pModel == NULL || !m_bEnabled)
	{
		return;
	}

	QListIterator<QPair<ServerInfo, int> > it(servers);
	while (it.hasNext())
	{
		const QPair<ServerInfo, int>& peer=it.next();
		const ServerInfo& info=peer.first;

		if (info.count == 0)
		{
			continue;
		}

		QModelIndex index=findItem(info);
		if (!index.isValid())
		{
			int nRow=m_pModel->rowCount();
			if (m_bInit && nRow > 0)
			{
				nRow--;
			}

			m_pModel->insertRow(nRow);
			index=m_pModel->index(nRow, 0);
			m_pModel->itemFromIndex(index)->setEditable(false);

			setServerItemSelectStatus(index, isSelectedItem(info), peer.second);
		}

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
		m_pModel->setData(index, OriginItem, ItemRole);

		m_pModel->setData(index, qVariantFromValue(info), UserDataRole);
		m_pModel->setData(index, peer.second, StatusRole);

		m_pModel->itemFromIndex(index)->setToolTip(strTip);
	}

	if (!m_bInit)
	{
		m_bInit=true;

		int nRow=m_pModel->rowCount()-1;

		QListIterator<Server> iter(m_servers);
		while (iter.hasNext())
		{
			const Server& server=iter.next();

			addMantualServerItemToServerView(++nRow, server);
		}

		m_pModel->insertRow(++nRow);
		const QModelIndex&index=m_pModel->index(nRow, 0);
		m_pModel->itemFromIndex(index)->setEditable(false);
		m_pModel->setData(index, findPixmap(":/images/addserver.png").scaled(QSize(LIST_VIEW_ICON_SIZE, LIST_VIEW_ICON_SIZE)), Qt::DecorationRole);
		m_pModel->setData(index, AddOp, ItemRole);

		if (m_pMonitor != NULL)
		{
			m_pMonitor->load();
		}
	}

	updateListView();
}

void QListViewSyncFileServer::setServerItemSelectStatus(const QModelIndex& index, bool select, int tick)
{
	if (!m_bEnabled)
	{
		return;
	}

	m_pModel->setData(index, select, SelectedRole);

	if (!m_bRemovable)
	{
		const QPixmap& pix=tick>0? m_pixHostQuit:m_pixHost;
		m_pModel->setData(index, select? addPixmapToPixmap(m_pixCheck, pix, Qt::BottomRightCorner):pix, Qt::DecorationRole);
	}
	else
	{
		setServerItemRemovable(index, m_bRemovable);
	}

	emit selectedChanged();
}

void QListViewSyncFileServer::addMantualServerItemToServerView(int row, const Server& server)
{
	m_pModel->insertRow(row);
	const QModelIndex&index=m_pModel->index(row, 0);
	m_pModel->itemFromIndex(index)->setEditable(false);

	QString strText=server.addr;
	m_pModel->setData(index, strText, Qt::EditRole);
	m_pModel->setData(index, AddedItem, ItemRole);
	m_pModel->setData(index, qVariantFromValue(server), UserDataRole);

	setServerItemSelectStatus(index, m_selections.contains(server.addr));
}

void QListViewSyncFileServer::onClickListViewItem(const QModelIndex& index)
{
	if (m_pModel == NULL || !m_bEnabled)
	{
		return;
	}

	if (!m_bRemovable)
	{
		int nType=m_pModel->data(index, ItemRole).toInt();
		switch (nType)
		{
		case OriginItem:
			{
				ServerInfo info=qVariantValue<ServerInfo>(m_pModel->data(index, UserDataRole));
				bool bSelect=m_pModel->data(index, SelectedRole).toBool();
				int nTick=m_pModel->data(index, StatusRole).toInt();
				setServerItemSelectStatus(index, !bSelect, nTick);
				if (bSelect)
				{
					QList<QString> lstKeys=info.servers.keys();
					foreach (QString key, lstKeys)
					{
						m_selections.remove(key);
					}
				}
				else
				{
					QList<QString> lstKeys=info.servers.keys();
					if (!lstKeys.isEmpty())
					{
						m_selections.insert(lstKeys.first());
					}
				}
			}
			break;
		case AddedItem:
			{
				Server server=qVariantValue<Server>(m_pModel->data(index, UserDataRole));
				bool bSelect=m_pModel->data(index, SelectedRole).toBool();
				setServerItemSelectStatus(index, !bSelect);
				if (bSelect)
				{
					m_selections.remove(server.addr);
				}
				else
				{
					m_selections.insert(server.addr);
				}
			}
			break;
		case AddOp:
			{
				addServerItemToServerView();
			}
			break;
		}

		entered(index);

		saveServers();
	}
}

bool QListViewSyncFileServer::findServer(const Server& server)
{
	int nRowCount=m_pModel->rowCount();
	int nRow=0;
	for (; nRow<nRowCount; nRow++)
	{
		const QModelIndex&index=m_pModel->index(nRow, 0);
		int nType=m_pModel->data(index, ItemRole).toInt();
		switch (nType)
		{
		case OriginItem:
			{
				ServerInfo info=qVariantValue<ServerInfo>(m_pModel->data(index, UserDataRole));

				QMapIterator<QString, QVector<Server> > it(info.servers);
				while (it.hasNext())
				{
					it.next();

					const QVector<Server>& servers=it.value();
					for (int i=0; i<servers.size(); i++)
					{
						const Server& s=servers[i];
						if (s.addr == server.addr)
						{
							setCurrentIndex(index);
							scrollTo(index);
							return true;
						}
					}
				}
			}
			break;
		case AddedItem:
			{
				Server s=qVariantValue<Server>(m_pModel->data(index, UserDataRole));
				if (s.addr == server.addr)
				{
					setCurrentIndex(index);
					scrollTo(index);
					return true;
				}
			}
			break;
		default:
			break;
		}
	}

	return false;
}

void QListViewSyncFileServer::addServerItemToServerView()
{
	QDlgEditHost dialog(this);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		Server server=dialog.host();

		if (findServer(server))
		{
			return;
		}

		m_servers.append(server);

		int nRow=m_pModel->rowCount()-1;
		addMantualServerItemToServerView(nRow, server);

		const QModelIndex&index=m_pModel->index(nRow, 0);
		setCurrentIndex(index);
		scrollTo(index);

		saveServers();

		updateListView();
	}
}

bool QListViewSyncFileServer::isAddedItem(int row) const
{
	return false;	//ÁÙÊ±

	if (m_pModel->rowCount() <= row)
	{
		return false;
	}

	const QModelIndex&index=m_pModel->index(row, 0);
	if (m_pModel->data(index, ItemRole).toInt() == AddedItem)
	{
		return true;
	}

	return false;
}

bool QListViewSyncFileServer::hasSelected() const
{
	int nRowCount=m_pModel->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		const QModelIndex&index=m_pModel->index(nRow, 0);
		int nType=m_pModel->data(index, ItemRole).toInt();
		if (nType != AddOp)
		{
			if (m_pModel->data(index, SelectedRole).toBool())
			{
				return true;
			}
		}
	}

	return false;
}

int QListViewSyncFileServer::nextSelected(int row, ServerInfo& info) const
{
	int nRowCount=m_pModel->rowCount();
	for (int nRow=row+1; nRow<nRowCount; nRow++)
	{
		const QModelIndex&index=m_pModel->index(nRow, 0);
		int nType=m_pModel->data(index, ItemRole).toInt();
		if (nType != AddOp)
		{
			if (m_pModel->data(index, SelectedRole).toBool())
			{
				if (nType == OriginItem)
				{
					info=qVariantValue<ServerInfo>(m_pModel->data(index, UserDataRole));
				}
				else if (nType == AddedItem)
				{
					info.hostName="";
					info.usrName="";
					info.home="";

					info.servers.clear();
					info.count=1;
					info.servers["FF:FF:FF:FF:FF:FF"].append(qVariantValue<Server>(m_pModel->data(index, UserDataRole)));
				}

				return nRow;
			}
		}
	}

	return -1;
}

void QListViewSyncFileServer::setServerItemsEnabled(bool enabled)
{
	m_bEnabled=enabled;
}

void QListViewSyncFileServer::mouseReleaseEvent(QMouseEvent* event)
{
	if (!m_bEnabled || event->button() == Qt::RightButton)
	{
		event->accept();
	}
	else
	{
		if (!m_bRemovable)
		{
			QListView::mouseReleaseEvent(event);
		}
		else
		{
			QModelIndex index=indexAt(event->pos());
			int nType=m_pModel->data(index, ItemRole).toInt();
			if (nType == AddedItem)
			{
				QRect rect=visualRect(index);

				rect=QRect(rect.left()+(rect.width()-LIST_VIEW_ICON_SIZE)/2, rect.top(), LIST_VIEW_LARGE_MARK_ICON_SIZE, LIST_VIEW_LARGE_MARK_ICON_SIZE);
				if (rect.contains(event->pos()))
				{
					Server s=qVariantValue<Server>(m_pModel->data(index, UserDataRole));

					m_pModel->removeRow(index.row());

					QMutableListIterator<Server> it(m_servers);
					while (it.hasNext())
					{
						const Server& server=it.next();
						if (s.addr == server.addr)
						{
							it.remove();

							break;
						}
					}

					if (!hasMantualServer())
					{
						m_bRemovable=false;
					}

					saveServers();
					return;
				}
			}
		}
	}
}

void QListViewSyncFileServer::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (!m_bEnabled)
	{
		event->accept();
		return;
	}

	if (event->button() == Qt::RightButton && hasMantualServer())
	{
		setServerItemsRemoveable(!m_bRemovable);

		entered(QModelIndex());

		event->accept();
	}
	else
	{
		QListView::mouseDoubleClickEvent(event);
	}
}

void QListViewSyncFileServer::mouseMoveEvent(QMouseEvent* event)
{
	QModelIndex index=indexAt(event->pos());

	entered(index);

	QListViewHorizontalIcon::mouseMoveEvent(event);
}

void QListViewSyncFileServer::setServerItemsRemoveable(bool removable)
{
	if (removable && m_servers.isEmpty())
	{
		return;
	}

	m_bRemovable=removable;

	int nRowCount=m_pModel->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		const QModelIndex&index=m_pModel->index(nRow, 0);

		int nType=m_pModel->data(index, ItemRole).toInt();
		if (nType == AddedItem)
		{
			setServerItemRemovable(index, removable);
		}
	}
}

void QListViewSyncFileServer::setServerItemRemovable(const QModelIndex& index, bool removable)
{
	bool bSelect=m_pModel->data(index, SelectedRole).toBool();

	QPixmap pix=bSelect? addPixmapToPixmap(m_pixCheck, m_pixHost, Qt::BottomRightCorner):m_pixHost;
	m_pModel->setData(index, removable? addPixmapToPixmap(m_pixRemove, pix, Qt::TopLeftCorner):pix, Qt::DecorationRole);
}

void QListViewSyncFileServer::saveServers()
{
	if (m_pMonitor != NULL)
	{
		m_pMonitor->save(m_servers);
	}

	QWriteSelectionsToXmlFile selector(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE), m_strRecord, m_selections.toList().toVector());
	selector.write(false);
}

QReadSelectionsFromXmlFile::QReadSelectionsFromXmlFile(const QString& filename, const QString& type)
:QAbstractReadXmlFile(filename)
,m_strType(type)
{

}

QReadSelectionsFromXmlFile::~QReadSelectionsFromXmlFile()
{

}

void QReadSelectionsFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "commit", readCommitNode);
}

void QReadSelectionsFromXmlFile::readCommitNode(QXmlStreamReader& reader)
{
	readNextNode(reader, m_strType, readTypeNode);
}

void QReadSelectionsFromXmlFile::readTypeNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "selections", readSelectionsNode);
}

void QReadSelectionsFromXmlFile::readSelectionsNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "selection", readSelectionNode);
}

void QReadSelectionsFromXmlFile::readSelectionNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "selection");

	QXmlStreamAttributes attributes=reader.attributes();

	QString strName;
	if (attributes.hasAttribute("name"))
	{
		strName=attributes.value("name").toString();
		if (!strName.isEmpty())
		{
			m_results.insert(strName);
		}
	}

	readItemFinished(reader);
}

QWriteSelectionsToXmlFile::QWriteSelectionsToXmlFile(const QString& filename, const QString& type, const QVector<QString>& selections)
:QAbstractWriteXmlFile(filename)
,m_strType(type)
,m_selections(selections)
{

}

QWriteSelectionsToXmlFile::~QWriteSelectionsToXmlFile()
{

}

void QWriteSelectionsToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	QDomElement node=findChildElement(doc, parent, m_strType);
	node=findChildElement(doc, node, "selections");

	QDomElement child = node.firstChildElement();
	while (!child.isNull())
	{
		QDomElement nd=child;

		child = child.nextSiblingElement();

		node.removeChild(nd);
	}

	for (int i=0; i<m_selections.size(); i++)
	{
		QDomElement child=doc.createElement("selection");

		child.setAttribute("name", m_selections[i]);

		node.appendChild(child);
	}
}

QDomElement QWriteSelectionsToXmlFile::findChildElement(QDomDocument& doc, QDomElement& parent, const QString& tag)
{
	QDomElement child = parent.firstChildElement();
	while (!child.isNull())
	{
		if (child.tagName() == tag)
		{
			return child;
		}

		child = child.nextSiblingElement();
	}

	child=doc.createElement(tag);
	parent.appendChild(child);

	return child;
}

