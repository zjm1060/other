#include "qtreeviewinspect.h"

#include <QMouseEvent>
#include <QDir>

#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"
#include "qviewcontroller.h"

QTreeViewInspect::QTreeViewInspect(QWidget *parent)
	: QTreeView(parent)
	,m_pClient(NULL)
	,m_pModel(NULL)
	,m_bInpecting(false)
	,m_bReference(false)
	,m_nNumItems(0)
{
	QSharedControllerPtr pController(new QViewController);
	m_pClientManager=new QClientManager(pController, this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(connectFailed(QClient*)), this, SLOT(unconnected(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));

	m_pReceiver=new QReceiverInspect;
	connect(m_pReceiver, SIGNAL(cscsDataSourceLost()), this, SLOT(cscsDataSourceLost()));
	connect(m_pReceiver, SIGNAL(cscsDataSourceNoExisted()), this, SLOT(cscsDataSourceNoExisted()));
	connect(m_pReceiver, SIGNAL(recvCscsStationEigenvalue(int, QByteArray)), this, SLOT(recvCscsStationEigenvalue(int, QByteArray)));
	connect(m_pReceiver, SIGNAL(recvCscsGroupEigenvalue(int, int, QByteArray)), this, SLOT(recvCscsGroupEigenvalue(int, int, QByteArray)));
	connect(m_pReceiver, SIGNAL(recvCscsRecordEigenvalue(int, int, int, QByteArray)), this, SLOT(recvCscsRecordEigenvalue(int, int, int, QByteArray)));

	m_pDelegate=new QInspectDelegate(this);
	setItemDelegate(m_pDelegate);

	setHeaderHidden(true);
	setWordWrap(true);
}

QTreeViewInspect::~QTreeViewInspect()
{
	SAFE_DELETE_CLASS(m_pClientManager);

	SAFE_DELETE_CLASS(m_pReceiver);

	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);
}

void QTreeViewInspect::setInspectItems(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items)
{
	QTreeViewInspectRunnable* pRunnable=new QTreeViewInspectRunnable(info, items);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QTreeModel*, int)), this, SLOT(addInspectItemsFinished(QTreeModel*, int)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);

	emit sendMessage(tr("Loading..."));
}

void QTreeViewInspect::addInspectItemsFinished(QTreeModel* model, int count)
{
	setModel(model);
	if (m_pModel != NULL)
	{
		SAFE_DELETE_CLASS(m_pModel);
	}

	m_pModel=model;
	m_nNumItems=count;

	adjustSize();

	emit addItemsFinished();
	emit sendMessage(tr("Loaded."));
}

void QTreeViewInspect::restore(const QModelIndex& pindex/* =QModelIndex */)
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRowCount=m_pModel->rowCount(pindex);
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, NameTitle, pindex);
		m_pModel->setData(index, Initialized, StatusRole);

		restore(index);
	}
}

void QTreeViewInspect::inspect(bool init/* =false */)
{
	if (m_bInpecting || m_pModel == NULL)
	{
		return;
	}

	if (init)
	{
		restore();

		collapseAll();
	}

	m_bInpecting=true;
	m_index=QModelIndex();
	m_bPause=false;

	m_nCurItem=0;
	m_nNumNormal=0;
	m_nNumSkip=0;
	
	nextTask();
}

void QTreeViewInspect::pause(bool state)
{
	m_bPause=state;

	if (!m_bPause)
	{
		nextTask();
	}
}

void QTreeViewInspect::cancel()
{
	m_pClient=NULL;
	m_pClientManager->removeAll();

	m_bInpecting=false;
}

void QTreeViewInspect::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!m_bInpecting)
	{
		return;
	}

	if (m_pModel == NULL || !index.isValid())
	{
		return;
	}

	m_pModel->setData(index, value, role);
	update(m_pModel->index(index.row(), StatusTitle, index.parent()));
}

int QTreeViewInspect::childrenCount(const QModelIndex& index)
{
	if (m_pModel == NULL)
	{
		return 0;
	}

	int nCount=(m_pModel->data(index, ItemRole).toInt() == ChildItem? m_pModel->rowCount(index):0);
	for (int nIndex=0; nIndex<m_pModel->rowCount(index); nIndex++)
	{
		QModelIndex cindex=m_pModel->index(nIndex, NameTitle, index);
		nCount+=childrenCount(cindex);
	}

	return nCount;
}

void QTreeViewInspect::nextTask()
{
	if (m_pModel == NULL)
	{
		return;
	}

	if (m_bPause || !m_bInpecting)
	{
		return;
	}

	if (!m_index.isValid() && m_pModel->rowCount() > 0)
	{
		m_index=m_pModel->index(0, NameTitle);
	}
	else
	{
		int nStatus=m_pModel->data(m_index, StatusRole).toInt();

		bool bSkip=false;
		if (nStatus == Unconnected)
		{
			int nNumItems=m_pModel->rowCount(m_index);

			int nRowCount=m_pModel->rowCount(m_index);
			for (int nRow=0; nRow<nRowCount; nRow++)
			{
				QModelIndex index=m_pModel->index(nRow, NameTitle, m_index);
				setData(index, Skip, StatusRole);
				setData(index, ServiceQuit, CauseRole);

				nNumItems+=childrenCount(index);
			}

			m_nCurItem+=nNumItems;
			m_nNumSkip+=nNumItems;

			expand(m_index);

			bSkip=true;
		}
		else if (nStatus == Skip)
		{
			int nCause=m_pModel->data(m_index, CauseRole).toInt();
			if (nCause == H9000NotRunning || nCause == NoReferenceSource || nCause == ServiceQuit)
			{
				int nNumItems=m_pModel->rowCount(m_index);

				int nRowCount=m_pModel->rowCount(m_index);
				for (int nRow=0; nRow<nRowCount; nRow++)
				{
					QModelIndex index=m_pModel->index(nRow, NameTitle, m_index);

					nNumItems+=childrenCount(index);
				}

				m_nCurItem+=nNumItems;
				m_nNumSkip+=nNumItems;

				bSkip=true;
			}
		}
		
		if (!bSkip && m_pModel->rowCount(m_index) > 0)
		{
			m_index=m_pModel->index(0, NameTitle, m_index);
		}
		else if (m_pModel->rowCount(m_index.parent()) > m_index.row()+1)
		{
			m_index=m_pModel->index(m_index.row()+1, NameTitle, m_index.parent());
		}
		else
		{
			while (true)
			{
				m_index=m_index.parent();

				if (!m_index.isValid())
				{
					break;
				}

				bool bExpand=false;
				int nNumRows=m_pModel->rowCount(m_index);
				for (int nRow=0; nRow<nNumRows; nRow++)
				{
					QModelIndex index=m_pModel->index(nRow, NameTitle, m_index);
					int nState=m_pModel->data(index, StatusRole).toInt();
					if ((nState != Completed && nState != Match) || (m_pModel->rowCount(index) != 0 && isExpanded(index)))
					{
						bExpand=true;

						break;
					}
				}

				if (!bExpand)
				{
					collapse(m_index);
				}

				if (m_pModel->rowCount(m_index.parent()) > m_index.row()+1)
				{
					m_index=m_pModel->index(m_index.row()+1, NameTitle, m_index.parent());

					break;
				}
			} 
		}
	}

	if (!m_index.isValid())
	{
		m_pClient=NULL;
		m_pClientManager->removeAll();

		m_bInpecting=false;

		emit finished(m_nNumNormal, m_nNumSkip, m_nNumItems);

		return;
	}

	scrollTo(m_index);

	int nRole=m_pModel->data(m_index, ItemRole).toInt();
	switch (nRole)
	{
	case PeerItem:
		{
			if (m_pClient != NULL)
			{
				QClient* pClient=m_pClient;
				m_pClient=NULL;
				m_pClientManager->remove(pClient);
			}
			
			m_bReference=qVariantValue<bool>(m_pModel->data(m_index, ComparedRole));

			ServerInfo info=qVariantValue<ServerInfo>(m_pModel->data(m_index, PeerInfoRole));
			QClient* pClient=m_pClientManager->addClient(info);

			setData(m_index, Connecting, StatusRole);

			emit sendMessage(tr("Connecting to %1 ...").arg(info.hostName));

			m_pClientManager->connectToHost(pClient);
		}
		break;
	case GroupItem:
		{
			m_nMsg=m_pModel->data(m_index, MsgParamRole).toInt();

			emit sendMessage(tr("Inspect: %1").arg(m_pModel->data(m_index, Qt::DisplayRole).toString()));

			nextTask();
		}
		break;
	case ChildItem:
		{
			m_nCurItem++;
			emit progress(m_nCurItem, 0, m_nNumItems);

			m_nAck=m_pModel->data(m_index, MsgParamRole).toInt();
			QByteArray buffer=m_pModel->data(m_index, DataRole).toByteArray();

			if (m_pClient != NULL && m_pReceiver != NULL)
			{
				setData(m_index, Collecting, StatusRole);

				if (!m_pReceiver->queryDataSource(m_pClient, m_nMsg, m_nAck, buffer))
				{
					m_nNumSkip++;

					setData(m_index, Skip, StatusRole);
					setData(m_index, ServiceQuit, CauseRole);

					nextTask();
				}
			}
			else if (!m_bReconnecting)
			{
				m_nNumSkip++;

				setData(m_index, Skip, StatusRole);
				setData(m_index, ServiceQuit, CauseRole);

				nextTask();
			}
		}
		break;
	default:
		break;
	}
}

void QTreeViewInspect::connected(QClient* client)
{
	m_pClient=client;
	m_bReconnecting=false;

	QModelIndex index=m_index;
	while (index.isValid())
	{
		if (m_pModel->data(index, ItemRole).toInt() == PeerItem)
		{
			setData(index, Connected, StatusRole);

			break;
		}

		index=index.parent();
	}

	emit sendMessage(tr("Connection succeed."));

	nextTask();
}

void QTreeViewInspect::unconnected(QClient* client)
{
	if (m_pClient != NULL)
	{
		m_pClient = NULL;
		m_bReconnecting=true;

		QModelIndex index=m_index;
		while (index.isValid())
		{
			if (m_pModel->data(index, ItemRole).toInt() == PeerItem)
			{
				setData(index, Reconnecting, StatusRole);

				break;
			}

			index=index.parent();
		}

		emit sendMessage(tr("Reconnecting..."));

		m_pClientManager->reconnect(client);
	}
	else
	{
		m_bReconnecting=false;
		if (!m_pClientManager->contains(client))
		{
			return;
		}

		m_pClientManager->remove(client);

		int nItemRole=m_pModel->data(m_index, ItemRole).toInt();
		if (nItemRole == PeerItem)
		{
			setData(m_index, Unconnected, StatusRole);

			emit sendMessage(tr("Connection failed."));
		}
		else
		{
			QModelIndex index=m_index.parent();
			while (index.isValid())
			{
				if (m_pModel->data(index, ItemRole).toInt() == PeerItem)
				{
					setData(index, Unconnected, StatusRole);

					break;
				}

				index=index.parent();
			}

			emit sendMessage(tr("Disconnection."));

			int nStatus=m_pModel->data(m_index, StatusRole).toInt();
			if (nStatus == Initialized || nStatus == Collecting)
			{
				setData(m_index, Skip, StatusRole);
				setData(m_index, ServiceQuit, CauseRole);

				m_nNumSkip++;
			}
		}

		nextTask();
	}
}

void QTreeViewInspect::disconnected(QClient* client)
{
	unconnected(client);
}

void QTreeViewInspect::cscsDataSourceLost()
{
	setData(m_index, m_bReference? Failed:Skip, StatusRole);
	setData(m_index, H9000NotRunning, CauseRole);

	if (!m_bReference)
	{
		m_nNumSkip++;
	}
	
	nextTask();
}

void QTreeViewInspect::cscsDataSourceNoExisted()
{
	setData(m_index, Failed, StatusRole);
	setData(m_index, DataSourceNoExisted, CauseRole);

	nextTask();
}

void QTreeViewInspect::recvCscsStationEigenvalue(int stn, QByteArray buffer)
{
	Q_UNUSED(stn);

	recvCscsEigenvalue(buffer);
}

void QTreeViewInspect::recvCscsGroupEigenvalue(int stn, int group, QByteArray buffer)
{
	Q_UNUSED(stn);
	Q_UNUSED(group);

	recvCscsEigenvalue(buffer);
}

void QTreeViewInspect::recvCscsRecordEigenvalue(int stn, int group, int datatype, QByteArray buffer)
{
	Q_UNUSED(stn);
	Q_UNUSED(group);
	Q_UNUSED(datatype);

	recvCscsEigenvalue(buffer);
}

void QTreeViewInspect::recvCscsEigenvalue(QByteArray buffer)
{
	QByteArray reference=m_pModel->data(m_index, DataRole).toByteArray();
	if (m_bReference)
	{
		m_data.insert(reference, buffer);

		m_nNumNormal++;
		setData(m_index, Completed, StatusRole);
	}
	else
	{
		setData(m_index, Verifying, StatusRole);

		if (!m_data.contains(reference))
		{
			setData(m_index, Skip, StatusRole);
			setData(m_index, NoReferenceSource, CauseRole);

			m_nNumSkip++;
		}
		else
		{
			if (m_data[reference] == buffer)
			{
				m_nNumNormal++;
				setData(m_index, Match, StatusRole);
			}
			else
			{
				setData(m_index, Mismatch, StatusRole);
			}
		}
	}

	nextTask();
}

void QTreeViewInspect::resizeEvent(QResizeEvent* event)
{
	QSize size=event->size();
	int nWidth=size.width()*2/5;
	nWidth=(nWidth>200? 200:nWidth);

	setColumnWidth(NameTitle, size.width()-nWidth);
	setColumnWidth(StatusTitle, nWidth);

	QTreeView::resizeEvent(event);
}

QTreeViewInspectRunnable::QTreeViewInspectRunnable(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items, QObject* parent/* =0 */)
:QObject(parent)
,m_info(info)
,m_items(items)
,m_pModel(NULL)
,m_nNumItems(0)
{

}

QTreeViewInspectRunnable::~QTreeViewInspectRunnable()
{

}

void QTreeViewInspectRunnable::run()
{
	m_pModel=new QTreeModel(QTreeViewInspect::TitleLabelEnd);
	if (m_pModel != NULL)
	{
		bool bCompare=false;
		for (int i=0; i<m_items.children.size(); i++)
		{
			if (m_items.children[i].msgParam == MSG_CSCS)
			{
				bCompare=true;
				break;
			}
		}

		if (bCompare)
		{
			addInspectGroup(m_info, m_items, true);
		}

		QList<QPair<ServerInfo, int> > servers=QGlobalInfo::instance()->serverList();

		QListIterator<QPair<ServerInfo, int> > it(servers);
		while (it.hasNext())
		{
			const QPair<ServerInfo, int>& peer=it.next();
			if (peer.second == 0)
			{
				addInspectGroup(peer.first, m_items);
			}
		}
	}

	emit finished(m_pModel, m_nNumItems);
}

void QTreeViewInspectRunnable::addInspectItem(const QTreeViewInspectSetting::InspectItem& item, const QModelIndex& parent, bool isGroup, const QByteArray& buffer)
{
	for (int i=0; i<item.children.size(); i++)
	{
		const QTreeViewInspectSetting::InspectItem& citem=item.children[i];

		m_pModel->insertRow(i, parent);

		QModelIndex index=m_pModel->index(i, QTreeViewInspect::NameTitle, parent);
		m_pModel->setData(index, citem.desc, Qt::DisplayRole);
		m_pModel->setData(index, isGroup? QTreeViewInspect::GroupItem:QTreeViewInspect::ChildItem, QTreeViewInspect::ItemRole);
		m_pModel->setData(index, citem.msgParam, QTreeViewInspect::MsgParamRole);

		QByteArray buf=buffer;

		if (!isGroup)
		{
			Q_WRITE_ONLY_DATA_STREAM_DEFINED(stream, buf);
			stream<<citem.id;
			m_pModel->setData(index, buf, QTreeViewInspect::DataRole);

			m_nNumItems++;
		}

		m_pModel->setData(index, QTreeViewInspect::Initialized, QTreeViewInspect::StatusRole);

		addInspectItem(citem,index, false, buf);
	}
}

void QTreeViewInspectRunnable::addInspectGroup(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items, bool compared/* =false */)
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRow=m_pModel->rowCount();

	m_pModel->insertRow(nRow);
	QModelIndex pindex=m_pModel->index(nRow, QTreeViewInspect::NameTitle);
	m_pModel->setData(pindex, QTreeViewInspect::PeerItem, QTreeViewInspect::ItemRole);
	m_pModel->setData(pindex, qVariantFromValue(info), QTreeViewInspect::PeerInfoRole);
	m_pModel->setData(pindex, compared, QTreeViewInspect::ComparedRole);
	m_pModel->setData(pindex, QTreeViewInspect::Initialized, QTreeViewInspect::StatusRole);

	addInspectItem(items, pindex, true);
}
