#include "qlistviewfilereceiver.h"

#include "common.h"
#include "qdlgbrowsefolders.h"
#include "qdlgeditfolder.h"

QListViewFileReceiver::QListViewFileReceiver(const QString& xmlfile, const QString& record, const QString& subPath, QWidget *parent/* =0 */)
	: QListView(parent)
	,m_pModel(NULL)
	,m_strSubPath(subPath)
{
	setResizeMode(QListView::Adjust);
	setMovement(QListView::Static);
	setViewMode(QListView::IconMode);
	setGridSize(QSize(96, 80));
	setWordWrap(true);

	QStringList labels;
	labels<<tr("title");

	m_pModel=new QStandardItemModel(0, labels.size(), this);
	m_pModel->setHorizontalHeaderLabels(labels);

	setModel(m_pModel);

	m_pDelegate=new QServerDelegate;
	setItemDelegate(m_pDelegate);

	setSelectionMode(QAbstractItemView::ExtendedSelection);
	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	m_pMonitor=new QMonitorFileReceiver(xmlfile, record, this);
	connect(m_pMonitor, SIGNAL(changed(const QMap<QString, QListViewFileReceiver::FileReceiver>&)), 
		this, SLOT(changed(const QMap<QString, QListViewFileReceiver::FileReceiver>&)));

	m_pMonitor->load();
}

QListViewFileReceiver::~QListViewFileReceiver()
{
	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);

	setItemDelegate(NULL);
	SAFE_DELETE_CLASS(m_pDelegate);

	if (m_pMonitor != NULL)
	{
		m_pMonitor->save(m_receivers);
		SAFE_DELETE_CLASS(m_pMonitor);
	}
}

void QListViewFileReceiver::paintEvent(QPaintEvent *event)
{
	QListView::paintEvent(event);
}

QListViewFileReceiver::FileReceiver& QListViewFileReceiver::findReceiver(const ServerInfo& info)
{
	Q_ASSERT(info.count > 0);

	QList<QString> lstMac=info.servers.keys();
	foreach (QString strMac, lstMac)
	{
		if (m_receivers.contains(strMac))
		{
			return m_receivers[strMac];
		}
	}

	QString strKey=lstMac.first();
	FileReceiver& receiver=m_receivers[strKey];
	receiver.name=strKey;
	receiver.select=false;

	return m_receivers[strKey];
}

bool QListViewFileReceiver::contains(const ServerInfo& info)
{
	Q_ASSERT(info.count > 0);

	QList<QString> lstMac=info.servers.keys();
	foreach (QString strMac, lstMac)
	{
		if (m_receivers.contains(strMac))
		{
			return true;
		}
	}

	return false;
}

void QListViewFileReceiver::addReceiver(const QPair<ServerInfo, int>& info)
{
	const ServerInfo& serverInfo=info.first;
	if (serverInfo.count == 0)
	{
		return;
	}

	FileReceiver& receiver=findReceiver(info.first);

	QVariant var;
	QStandardItem* pItem=NULL;

	int nRow;
	bool bExist=false;
	for (nRow=0; nRow<m_pModel->rowCount(); nRow++)
	{
		pItem=m_pModel->item(nRow);
		if (pItem != NULL)
		{
			QModelIndex index=m_pModel->index(nRow, 0);

			int nType=m_pModel->data(index, ItemTypeRole).toInt();
			switch (nType)
			{
			case OriginItem:
				{
					ServerInfo server=qVariantValue<ServerInfo>(m_pModel->data(index, ItemUserDataRole));
					if (info.first == server)
					{
						bExist=true;
					}
				}
				break;
			case AddedItem:
				{
					Server server=qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole));
					QMapIterator<QString, QVector<Server> > it(info.first.servers);
					while (it.hasNext())
					{
						it.next();

						const QVector<Server>& servers=it.value();
						for (int i=0; i<servers.size(); i++)
						{
							const Server& s=servers[i];
							if (s.addr == server.addr)
							{
								bExist=true;
								break;
							}
						}

						if (bExist)
						{
							break;
						}
					}
				}
				break;
			default:
				break;
			}
		}

		if (bExist)
		{
			break;
		}
	}

	if (!bExist)
	{
		m_pModel->insertRow(nRow);
	}

	QPixmap pixmap=(info.second>0? findPixmap(":/images/host_quit.png"):findPixmap(":/images/host.png"));
	if (receiver.select && info.second == 0)
	{
		pixmap=addPixmapToPixmap(findPixmap(":/images/checkmark.png"), pixmap, Qt::BottomRightCorner);
	}

	QModelIndex index=m_pModel->index(nRow, 0);
	pItem=m_pModel->itemFromIndex(index);
	pItem->setEditable(false);
	m_pModel->setData(index, QIcon(pixmap), Qt::DecorationRole);
	m_pModel->setData(index, serverInfo.hostName, Qt::DisplayRole);

	var.setValue(serverInfo);
	m_pModel->setData(index, OriginItem, ItemTypeRole);
	m_pModel->setData(index, var, ItemUserDataRole);

	m_pModel->setData(index, info.second==0, ServerOnlineRole);	//是否在线

	m_pModel->setData(index, info.second==0? receiver.select:false, ServerSelectRole);

	QString strDefault=receiver.workspace;

	if (strDefault.isEmpty() || !isCommonDriveMode(strDefault, serverInfo.home))
	{
		strDefault=appendPath(serverInfo.home, m_strSubPath);
	}

	m_pModel->setData(index, strDefault, ServerSaveRole);

	setToolTip(index, serverInfo, strDefault);
}

void QListViewFileReceiver::setToolTip(const QModelIndex& index, const ServerInfo& info, const QString& savePath)
{
	QStandardItem* pItem=m_pModel->itemFromIndex(m_pModel->index(index.row(), 0));
	if (pItem == NULL)
	{
		return;
	}

	QString strHtmlText;

	if (!info.hostName.isEmpty())
	{
		strHtmlText+="<font color=#A0A0A0>"+tr("Host Name:")+"</font>"
			+"<font color=#4B0082>"+info.hostName+"</font>";
	}

	if (info.count > 0)
	{
		strHtmlText+="<dt><font color=#A0A0A0>"+tr("Network:")+"</font>";
		QMapIterator<QString, QVector<Server> > it(info.servers);
		while (it.hasNext())
		{
			it.next();

			const QVector<Server>& servers=it.value();
			foreach (Server server, servers)
			{
				strHtmlText+="<dd><font color=#A0A0A0>"+server.addr+"</font></dd>";
			}
		}
		strHtmlText+="</dt>";
	}
	
	strHtmlText+="<dt><font color=#A0A0A0>"+tr("Upload Path:")+"</font>"
		+"<dd><font color=#0000CD>"+savePath+"</font></dd></dt>";

	pItem->setToolTip(strHtmlText);
}

void QListViewFileReceiver::setToolTip(const QModelIndex& index, const Server& server, const QString& savePath)
{
	QStandardItem* pItem=m_pModel->itemFromIndex(m_pModel->index(index.row(), 0));
	if (pItem == NULL)
	{
		return;
	}

	QString strHtmlText;
	strHtmlText+="<dt><font color=#A0A0A0>"+tr("Network:")+"</font>";
	strHtmlText+="<dd><font color=#A0A0A0>"+server.addr+"</font></dd>";
	strHtmlText+="</dt>";

	strHtmlText+="<dt><font color=#A0A0A0>"+tr("Upload Path:")+"</font>"
		+"<dd><font color=#0000CD>"+savePath+"</font></dd></dt>";

	pItem->setToolTip(strHtmlText);
}

bool QListViewFileReceiver::addReceiver(const Server& server)
{
	QStandardItem* pItem=NULL;

	int nRow;
	bool bExist=false;
	for (nRow=0; nRow<m_pModel->rowCount(); nRow++)
	{
		pItem=m_pModel->item(nRow);
		if (pItem != NULL)
		{
			QModelIndex index=m_pModel->index(nRow, 0);

			int nType=m_pModel->data(index, ItemTypeRole).toInt();
			switch (nType)
			{
			case OriginItem:
				{
					ServerInfo info=qVariantValue<ServerInfo>(m_pModel->data(index, ItemUserDataRole));
					QMapIterator<QString, QVector<Server> > it(info.servers);
					while (it.hasNext())
					{
						it.next();

						const QVector<Server>& servers=it.value();
						for (int i=0; i<servers.size(); i++)
						{
							const Server& s=servers[i];
							if (s.addr == server.addr && s.port == server.port)
							{
								bExist=true;

								setCurrentIndex(index);
								scrollTo(index);
								break;
							}
						}

						if (bExist)
						{
							break;
						}
					}
				}
				break;
			case AddedItem:
				{
					Server s=qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole));
					
					if (s.addr == server.addr)
					{
						bExist=true;

						setCurrentIndex(index);
						scrollTo(index);
					}
				}
				break;
			default:
				break;
			}
		}

		if (bExist)
		{
			return false;
		}
	}

	FileReceiver& receiver=m_receivers[server.addr];
	if (receiver.name.isEmpty())
	{
		receiver.name=server.addr;
		receiver.select=false;
	}

	QVariant var;

	m_pModel->insertRow(nRow);

	QPixmap pixmap=findPixmap(":/images/host.png");
	if (receiver.select)
	{
		pixmap=addPixmapToPixmap(findPixmap(":/images/checkmark.png"), pixmap, Qt::BottomRightCorner);
	}

	QModelIndex index=m_pModel->index(nRow, 0);
	pItem=m_pModel->itemFromIndex(index);
	pItem->setEditable(false);
	m_pModel->setData(index, QIcon(pixmap), Qt::DecorationRole);
	m_pModel->setData(index, server.addr, Qt::DisplayRole);

	var.setValue(server);
	m_pModel->setData(index, AddedItem, ItemTypeRole);
	m_pModel->setData(index, var, ItemUserDataRole);

	m_pModel->setData(index, true, ServerOnlineRole);	//是否在线
	m_pModel->setData(index, receiver.select, ServerSelectRole);

	QString strDefault=receiver.workspace;

	if (strDefault.isEmpty())
	{
		strDefault=appendPath("/home/ems/h9000/", m_strSubPath);
	}

	m_pModel->setData(index, strDefault, ServerSaveRole);

	setToolTip(index, server, strDefault);

	return true;
}

void QListViewFileReceiver::changed(const QMap<QString, QListViewFileReceiver::FileReceiver>& receivers)
{
	m_receivers=receivers;

	QStandardItem* pItem=NULL;

	for (int nRow=0; nRow<m_pModel->rowCount(); nRow++)
	{
		pItem=m_pModel->item(nRow);
		if (pItem != NULL)
		{
			QModelIndex index=m_pModel->index(nRow, 0);

			int nType=m_pModel->data(index, ItemTypeRole).toInt();
			switch (nType)
			{
			case OriginItem:
				{
					ServerInfo server=qVariantValue<ServerInfo>(m_pModel->data(index, ItemUserDataRole));
					if (contains(server))
					{
						QListViewFileReceiver::FileReceiver receiver=findReceiver(server);

						bool bOnline=m_pModel->data(index, ServerOnlineRole).toBool();	//是否在线

						QPixmap pixmap=(!bOnline? findPixmap(":/images/host_quit.png"):findPixmap(":/images/host.png"));
						if (receiver.select)
						{
							pixmap=addPixmapToPixmap(findPixmap(":/images/checkmark.png"), pixmap, Qt::BottomRightCorner);
						}

						QModelIndex index=m_pModel->index(nRow, 0);
						m_pModel->setData(index, QIcon(pixmap), Qt::DecorationRole);

						m_pModel->setData(index, receiver.select, ServerSelectRole);

						QString strDefault=receiver.workspace;

						if (strDefault.isEmpty())
						{
							strDefault=appendPath(server.home, m_strSubPath);
						}

						m_pModel->setData(index, strDefault, ServerSaveRole);

						setToolTip(index, server, strDefault);
					}
				}
				break;
			case AddedItem:
				{
					Server server=qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole));
					if (m_receivers.contains(server.addr))
					{
						QListViewFileReceiver::FileReceiver receiver=m_receivers[server.addr];

						QPixmap pixmap=findPixmap(":/images/host.png");
						if (receiver.select)
						{
							pixmap=addPixmapToPixmap(findPixmap(":/images/checkmark.png"), pixmap, Qt::BottomRightCorner);
						}

						QModelIndex index=m_pModel->index(nRow, 0);
						m_pModel->setData(index, QIcon(pixmap), Qt::DecorationRole);

						m_pModel->setData(index, receiver.select, ServerSelectRole);

						QString strDefault=receiver.workspace;

						if (strDefault.isEmpty())
						{
							strDefault=appendPath("/home/ems/h9000/", m_strSubPath);
						}

						m_pModel->setData(index, strDefault, ServerSaveRole);

						setToolTip(index, server, strDefault);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void QListViewFileReceiver::selectAll()
{
	if (m_pModel == NULL)
	{
		return;
	}

	bool bNeedSelet=false;

	int nRowCount=m_pModel->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, 0);

		bool bOnline=m_pModel->data(index, ServerOnlineRole).toBool();
		if (!bOnline)
		{
			continue;
		}

		if (!m_pModel->data(index, ServerSelectRole).toBool())
		{
			bNeedSelet=true;

			m_pModel->setData(index, true, ServerSelectRole);

			QPixmap pixmap=bOnline? findPixmap(":/images/host.png"):findPixmap(":/images/host_quit.png");
			pixmap=addPixmapToPixmap(findPixmap(":/images/checkmark.png"), pixmap, Qt::BottomRightCorner);

			m_pModel->setData(index, QIcon(pixmap), Qt::DecorationRole);

			int nType=m_pModel->data(index, ItemTypeRole).toInt();
			switch (nType)
			{
			case OriginItem:
				{
					ServerInfo info=
						qvariant_cast<ServerInfo>(m_pModel->data(index, ItemUserDataRole));

					if (contains(info))
					{
						QListViewFileReceiver::FileReceiver& receiver=findReceiver(info);
						receiver.select=true;
					}
				}
				break;
			case AddedItem:
				{
					Server server=qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole));

					if (m_receivers.contains(server.addr))
					{
						m_receivers[server.addr].select=true;
					}
				}
				break;
			default:
				break;
			}
		}
	}

	if (m_pMonitor != NULL && bNeedSelet)
	{
		m_pMonitor->save(m_receivers);
	}

	emit selectedChanged();
}

void QListViewFileReceiver::selectNone()
{
	if (m_pModel == NULL)
	{
		return;
	}

	bool bNeedSelet=false;

	int nRowCount=m_pModel->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, 0);
		if (m_pModel->data(index, ServerSelectRole).toBool())
		{
			if (m_pModel->data(index, ServerSelectRole).toBool())
			{
				bNeedSelet=true;

				m_pModel->setData(index, false, ServerSelectRole);

				bool bOnline=m_pModel->data(index, ServerOnlineRole).toBool();
				QPixmap pixmap=bOnline? findPixmap(":/images/host.png"):findPixmap(":/images/host_quit.png");

				m_pModel->setData(index, QIcon(pixmap), Qt::DecorationRole);

				int nType=m_pModel->data(index, ItemTypeRole).toInt();
				switch (nType)
				{
				case OriginItem:
					{
						ServerInfo info=
							qvariant_cast<ServerInfo>(m_pModel->data(index, ItemUserDataRole));

						if (contains(info))
						{
							QListViewFileReceiver::FileReceiver& receiver=findReceiver(info);
							receiver.select=false;
						}
					}
					break;
				case AddedItem:
					{
						Server server=qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole));

						if (m_receivers.contains(server.addr))
						{
							m_receivers[server.addr].select=false;
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

	if (m_pMonitor != NULL && bNeedSelet)
	{
		m_pMonitor->save(m_receivers);
	}

	emit selectedChanged();
}

void QListViewFileReceiver::clicked(const QModelIndex& index)
{
	bool bSelect=m_pModel->data(index, ServerSelectRole).toBool();
	int nType=m_pModel->data(index, ItemTypeRole).toInt();
	bool bOnline=m_pModel->data(index, ServerOnlineRole).toBool();
	if (!bOnline && !bSelect)
	{
		return;
	}

	if (bSelect)
	{
		QRect rect=visualRect(index);
		QPoint pt=mapFromGlobal(QCursor::pos());

		QRect r=m_pDelegate->toolIconRect(rect);
		if (r.contains(pt))
		{
			switch (nType)
			{
			case OriginItem:
				{
					ServerInfo info=
						qvariant_cast<ServerInfo>(m_pModel->data(index, ItemUserDataRole));

					QString strInitPath=m_pModel->data(index, ServerSaveRole).toString();
					QDlgBrowseFolders dialog(info, strInitPath, m_strSubPath, this);
					dialog.setModal(true);
					if (dialog.exec() != 0)
					{
						strInitPath=dialog.folderPath();
						m_pModel->setData(index, strInitPath, ServerSaveRole);

						if (contains(info))
						{
							QListViewFileReceiver::FileReceiver& receiver=findReceiver(info);
							receiver.workspace=strInitPath;
							receiver.select=!bSelect;

							if (m_pMonitor != NULL)
							{
								m_pMonitor->save(m_receivers);
							}
						}

						setToolTip(index, info, strInitPath);
					}
				}
				break;
			case AddedItem:
				{
					Server server=qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole));
					
					QString strInitPath=m_pModel->data(index, ServerSaveRole).toString();
					QDlgEditFolder dialog(QPixmap(""), strInitPath, this);
					dialog.setModal(true);
					if (dialog.exec() != 0)
					{
						strInitPath=dialog.text();
						m_pModel->setData(index, strInitPath, ServerSaveRole);

						if (m_receivers.contains(server.addr))
						{
							m_receivers[server.addr].workspace=strInitPath;
							m_receivers[server.addr].select=!bSelect;

							if (m_pMonitor != NULL)
							{
								m_pMonitor->save(m_receivers);
							}
						}

						setToolTip(index, server, strInitPath);
					}
				}
				break;
			default:
				break;
			}

			return;
		}
	}
	
	switch (nType)
	{
	case OriginItem:
		{
			ServerInfo info=
				qvariant_cast<ServerInfo>(m_pModel->data(index, ItemUserDataRole));

			if (contains(info))
			{
				QListViewFileReceiver::FileReceiver& receiver=findReceiver(info);
				receiver.select=!bSelect;
			}
		}
		break;
	case AddedItem:
		{
			Server server=qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole));

			if (m_receivers.contains(server.addr))
			{
				m_receivers[server.addr].select=!bSelect;
			}
		}
		break;
	default:
		break;
	}
	
	QPixmap pixmap=bOnline? findPixmap(":/images/host.png"):findPixmap(":/images/host_quit.png");
	if (!bSelect)
	{
		pixmap=addPixmapToPixmap(findPixmap(":/images/checkmark.png"), pixmap, Qt::BottomRightCorner);
	}

	m_pModel->setData(index, QIcon(pixmap), Qt::DecorationRole);
	m_pModel->setData(index, !bSelect, ServerSelectRole);

	if (m_pMonitor != NULL)
	{
		m_pMonitor->save(m_receivers);
	}

	update(index);

	emit selectedChanged();
}

QList<Server> QListViewFileReceiver::removeSelectedReceiver()
{
	QList<Server> servers;

	QModelIndexList indexes=selectionModel()->selectedRows();
	while (!indexes.isEmpty())
	{
		QListIterator<QModelIndex> it(indexes);
		while (it.hasNext())
		{
			const QModelIndex& index=it.next();
			int nType=m_pModel->data(index, ItemTypeRole).toInt();
			if (nType == AddedItem)
			{
				Server server=qvariant_cast<Server>(m_pModel->data(index, ItemUserDataRole));
				servers.append(server);

				m_pModel->removeRow(index.row());

				m_receivers.remove(server.addr);

				break;
			}
		}

		if (!it.hasNext())
		{
			break;
		}

		indexes=selectionModel()->selectedRows();
	}

	if (m_pMonitor != NULL)
	{
		m_pMonitor->save(m_receivers);
	}

	return servers;
}

int QListViewFileReceiver::nextSelected(int row, ServerInfo& info) const
{
	int nRowCount=m_pModel->rowCount();
	for (int nRow=row+1; nRow<nRowCount; nRow++)
	{
		const QModelIndex&index=m_pModel->index(nRow, 0);
		if (m_pModel->data(index, ServerSelectRole).toBool())
		{
			int nType=m_pModel->data(index, ItemTypeRole).toInt();
			if (nType == OriginItem)
			{
				info=qVariantValue<ServerInfo>(m_pModel->data(index, ItemUserDataRole));
			}
			else if (nType == AddedItem)
			{
				info.hostName="";
				info.usrName="";
				info.home="";

				info.servers.clear();
				info.count=1;
				info.servers["FF:FF:FF:FF:FF:FF"].append(qVariantValue<Server>(m_pModel->data(index, ItemUserDataRole)));
			}

			return nRow;
		}
	}

	return -1;
}

bool QListViewFileReceiver::isAddedItem(int row) const
{
	return false; //临时

	if (m_pModel == NULL)
	{
		return false;
	}

	const QModelIndex& index=m_pModel->index(row, 0);
	int nType=m_pModel->data(index, ItemTypeRole).toInt();

	return nType==AddedItem;
}

QString QListViewFileReceiver::savePath(int row) const
{
	QString strPath;

	if (m_pModel != NULL)
	{
		const QModelIndex& index=m_pModel->index(row, 0);
		strPath=m_pModel->data(index, ServerSaveRole).toString();
	}

	return strPath;
}

void QListViewFileReceiver::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

void QListViewFileReceiver::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	Q_UNUSED(selected);
	Q_UNUSED(deselected);

	bool bRemovable=false;

	QModelIndexList indexes=selectionModel()->selectedRows();
	QListIterator<QModelIndex> it(indexes);
	while (it.hasNext())
	{
		const QModelIndex& index=it.next();
		int nType=m_pModel->data(index, ItemTypeRole).toInt();
		if (nType == AddedItem)
		{
			bRemovable=true;
		}
	}

	emit selectChanged(bRemovable);

	viewport()->update();
}

bool QListViewFileReceiver::hasSelected() const
{
	bool hasSelect=false;
	for (int i=0; i<m_pModel->rowCount(); i++)
	{
		QModelIndex index=m_pModel->index(i, 0);
		hasSelect=m_pModel->data(index, ServerSelectRole).toBool();
		if (hasSelect)
		{
			bool bOnline=m_pModel->data(index, ServerOnlineRole).toBool();
			if (bOnline)
			{
				return true;
			}
		}
	}

	return false;
}

bool operator==(const QListViewFileReceiver::FileReceiver& source, const QListViewFileReceiver::FileReceiver& target)
{
	if (source.name == target.name && source.workspace == target.workspace && source.select == target.select)
	{
		return true;
	}

	return false;
}

QMonitorFileReceiver::QMonitorFileReceiver(const QString& filename, const QString& type, QObject* watched, QObject* parent/* =0 */)
:QAbstractMonitor(filename, watched, parent)
,m_strRecord(type)
{
	qRegisterMetaType<QMap<QString, QListViewFileReceiver::FileReceiver> >("QMap<QString, QListViewFileReceiver::FileReceiver>");
}

QMonitorFileReceiver::~QMonitorFileReceiver()
{

}

bool QMonitorFileReceiver::loadFile()
{
	bool bChanged=false;

	m_mutex.lock();

	QReadFileRecevierFromXmlFile reader(m_strFileName, m_strRecord);
	if (reader.read())
	{
		QMap<QString, QListViewFileReceiver::FileReceiver> receivers=reader.result();
		if (receivers.size() != m_receivers.size() || receivers != m_receivers)
		{
			m_receivers=receivers;

			bChanged=true;
		}
	}

	m_mutex.unlock();

	if (m_bFirst || bChanged)
	{
		if (m_bFirst)
		{
			m_bFirst=false;
		}

		emit changed(m_receivers);
	}

	return bChanged;
}

void QMonitorFileReceiver::save(const QMap<QString, QListViewFileReceiver::FileReceiver>& receivers)
{
	QMutexLocker locker(&m_mutex);

	QWriteFileRecevierToXmlFile writer(m_strFileName, m_strRecord, receivers.values().toVector());
	writer.write(false);
}

QReadFileRecevierFromXmlFile::QReadFileRecevierFromXmlFile(const QString& filename, const QString& type)
:QAbstractReadXmlFile(filename)
,m_strType(type)
{

}

QReadFileRecevierFromXmlFile::~QReadFileRecevierFromXmlFile()
{

}

void QReadFileRecevierFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "transfer", readTransferNode);
}

void QReadFileRecevierFromXmlFile::readTransferNode(QXmlStreamReader& reader)
{
	readNextNode(reader, m_strType, readTypeNode);
}

void QReadFileRecevierFromXmlFile::readTypeNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "receivers", readReceiversNode);
}

void QReadFileRecevierFromXmlFile::readReceiversNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "receiver", readReceiverNode);
}

void QReadFileRecevierFromXmlFile::readReceiverNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "receiver");

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("name") && attributes.hasAttribute("workspace") && attributes.hasAttribute("select"))
	{
		QListViewFileReceiver::FileReceiver receiver;
		receiver.name=attributes.value("name").toString();
		receiver.workspace=attributes.value("workspace").toString();
		receiver.select=(attributes.value("select").toString()=="true");
		if (!receiver.name.isEmpty())
		{
			m_results.insert(receiver.name, receiver);
		}
	}

	readItemFinished(reader);
}

QWriteFileRecevierToXmlFile::QWriteFileRecevierToXmlFile(const QString& filename, const QString& type, const QVector<QListViewFileReceiver::FileReceiver>& receivers)
:QAbstractWriteXmlFile(filename)
,m_strType(type)
,m_receivers(receivers)
{

}

QWriteFileRecevierToXmlFile::~QWriteFileRecevierToXmlFile()
{

}

void QWriteFileRecevierToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	QDomElement node=findChildElement(doc, parent, m_strType);
	node=findChildElement(doc, node, "receivers");

	QDomElement child = node.firstChildElement();
	while (!child.isNull())
	{
		QDomElement nd=child;

		child = child.nextSiblingElement();

		node.removeChild(nd);
	}

	for (int i=0; i<m_receivers.size(); i++)
	{
		QDomElement child=doc.createElement("receiver");

		QListViewFileReceiver::FileReceiver& receiver=m_receivers[i];

		child.setAttribute("name", receiver.name);
		child.setAttribute("workspace", receiver.workspace);
		child.setAttribute("select", receiver.select? "true":"false");

		node.appendChild(child);
	}
}

QDomElement QWriteFileRecevierToXmlFile::findChildElement(QDomDocument& doc, QDomElement& parent, const QString& tag)
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

