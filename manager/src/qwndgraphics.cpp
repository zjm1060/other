#include "qwndgraphics.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QHostAddress>
#include <QMouseEvent>

#include "common.h"
#include "qgraphicsitemrouter.h"
#include "qgraphicsitemswitch.h"
#include "qgraphicsitemhost.h"
#include "qgraphicsitemlink.h"
#include "qlogicaltopo.h"

QWndGraphics::QWndGraphics(QWidget* parent)
:QGraphicsView(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

	m_pScene=new QGraphicsScene(this);
	setScene(m_pScene);

	m_pAbstractItem=new QAbstractGraphicsItem;
	m_pAbstractItem->setOrigin(0,0);

	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	setViewportUpdateMode(FullViewportUpdate);

	setBackgroundBrush(QBrush(Qt::gray));
}

QWndGraphics::~QWndGraphics()
{
	SAFE_DELETE_CLASS(m_pAbstractItem);
}

void QWndGraphics::setRouters(QVector<QPair<quint32, int> > routers)
{
	qDebug("QWndGraphics: Add %d routes to scene", routers.size());

	QAbstractGraphicsItem* pRouter;
	QAbstractGraphicsItem* pItem=m_pAbstractItem;
	QAbstractGraphicsItem* pLink;

	for (int i=0; i<routers.size(); i++)
	{
		if (!pItem->contains(routers[i].first))
		{
			pRouter=new QGraphicsItemRouter;

			if (pItem != m_pAbstractItem && routers[i].first !=INVALID_IP_ADDRESS)
			{
				pLink=new QGraphicsItemLink;
			}
			else
			{
				pLink=NULL;
			}
			
			pItem->addItem(routers[i].first, pRouter, pLink);

			m_pScene->addItem(pRouter);
			if (pLink != NULL)
			{
				m_pScene->addItem(pLink);
			}

			if (routers[i].first ==INVALID_IP_ADDRESS)
			{
				pRouter->hide();
			}

			pItem=pRouter;
		}
		else
		{
			pItem=pItem->item(routers[i].first);

			pItem->setTickCount(QAbstractGraphicsItem::TickZero);
		}
	}

	m_pAbstractItem->adjust();

	QRectF r=m_pAbstractItem->boundingVolume();
	r.adjust(-60, -60, 60, 40);
	m_pScene->setSceneRect(r);

	m_pScene->invalidate();
}

QAbstractGraphicsItem* QWndGraphics::findRouter(quint32 router, QAbstractGraphicsItem* item)
{
	Q_ASSERT_NO_NULL(item);

	if (item->contains(router))
	{
		return item->item(router);
	}
	else
	{
		QAbstractGraphicsItem* pItem;

		QList<quint32> keys=item->items();
		QListIterator<quint32> it(keys);
		quint32 key;
		while (it.hasNext())
		{
			key=it.next();

			pItem=item->item(key);
			if (pItem->type() == GraphicsItemRouter)
			{
				pItem=findRouter(router, pItem);
				if (pItem != NULL)
				{
					return pItem;
				}
			}
		}

		return NULL;
	}
}

void QWndGraphics::setAlives(quint32 route, quint32 gateway, QVector<quint32> alives)
{
	QGraphicsItemRouter* pRouter;
	QGraphicsItemSwitch* pSwitch;
	QAbstractGraphicsItem* pItem;
	QAbstractGraphicsItem* pLink;

	qDebug("QWndGraphics: Find router %s in the scene.", QHostAddress(route).toString().toLocal8Bit().data());
	pRouter=qgraphicsitem_cast<QGraphicsItemRouter*>(findRouter(route, m_pAbstractItem));

	if (pRouter == NULL)
	{
		return;
	}

	if (!pRouter->contains(gateway))
	{
		pItem=new QGraphicsItemSwitch;

		pLink=NULL;
		if (pRouter->isVisible())
		{
			pLink=new QGraphicsItemLink;
		}
		
		pRouter->addItem(gateway, pItem, pLink);
		qDebug("QWndGraphics: Add switch %s to scene", QHostAddress(gateway).toString().toLocal8Bit().data());

		if ((gateway != INVALID_IP_ADDRESS) && (gateway&0xff) != 0)
		{
			pItem->setTag(QHostAddress(gateway).toString());
		}

		m_pScene->addItem(pItem);
		m_pScene->addItem(pLink);
	}

	qDebug("QWndGraphics: Add hosts to scene.");

	pSwitch=qgraphicsitem_cast<QGraphicsItemSwitch*>(pRouter->item(gateway));
	if (pSwitch != NULL)
	{
		for (int i=0; i<alives.size(); i++)
		{
			if (alives[i] == gateway)
			{
				continue;
			}

			if (!pSwitch->contains(alives[i]))
			{
				pItem=new QGraphicsItemHost;
				pLink=new QGraphicsItemLink;
				pSwitch->addItem(alives[i], pItem, pLink);
				qDebug("QWndGraphics: Add host %s to scene", QHostAddress(alives[i]).toString().toLocal8Bit().data());

				pItem->setTag(QHostAddress(alives[i]).toString());

				m_pScene->addItem(pItem);
				m_pScene->addItem(pLink);
			}

			pItem=pSwitch->item(alives[i]);
			pItem->setTickCount(QAbstractGraphicsItem::TickZero);
		}

		if (!alives.isEmpty())
		{
			pRouter->setTickCount(QAbstractGraphicsItem::TickZero);
			pSwitch->setTickCount(QAbstractGraphicsItem::TickZero);
		}
	}

	m_pAbstractItem->adjust();

	QRectF r=m_pAbstractItem->boundingVolume();
	r.adjust(-60, -60, 60, 60);
	m_pScene->setSceneRect(r);

	m_pScene->invalidate();
}

void QWndGraphics::checkAlives()
{
	m_pAbstractItem->setTickCountAll(QAbstractGraphicsItem::TickInc);

	m_pScene->invalidate();
}

void QWndGraphics::disconnected()
{
	m_pAbstractItem->setTickCountAll(QAbstractGraphicsItem::TickTop);

	m_pScene->invalidate();
}

bool QWndGraphics::addServer(const QServerManager::ServerInfo& info)
{
	QList<QString> keys=info.servers.keys();

	QAbstractGraphicsItem* pItem;
	QGraphicsItemHost*	pHost;

	bool bReprobe=false;
	QHostAddress address;
	foreach (QString key, keys)
	{
		const QVector<QServerManager::Server>& servers=info.servers[key];
		for (int i=0; i<servers.size(); i++)
		{
			address=QHostAddress(servers[i].addr);
			if (address == QHostAddress::LocalHost)
			{
				continue;
			}

			pItem=m_pAbstractItem->findServer(address.toIPv4Address());
			if (pItem == NULL)
			{
				bReprobe=true;
				continue;
			}

			pHost=qgraphicsitem_cast<QGraphicsItemHost*>(pItem);
			if (pHost != NULL)
			{
				pHost->setTickCount(QAbstractGraphicsItem::TickZero);

				if (pHost->flag(QGraphicsItemHost::IconConnectable) != 1)
				{
					pHost->setFlag(QGraphicsItemHost::IconConnectable, 1);

					pHost->setData(QGraphicsItemHost::DataHostName, info.hostName);
					pHost->setData(QGraphicsItemHost::DataIPAddress, servers[i].addr);
					pHost->setData(QGraphicsItemHost::DataPort, servers[i].port);

					pHost->setTag(servers[i].addr+"\n"+info.hostName);
				}
			}
		}
	}

	m_pScene->invalidate();

	return !bReprobe;
}

void QWndGraphics::removeServer(const QServerManager::ServerInfo& info)
{
	QList<QString> keys=info.servers.keys();

	QAbstractGraphicsItem* pItem;
	QGraphicsItemHost*	pHost;

	foreach (QString key, keys)
	{
		const QVector<QServerManager::Server>& servers=info.servers[key];
		for (int i=0; i<servers.size(); i++)
		{
			pItem=m_pAbstractItem->findServer(QHostAddress(servers[i].addr).toIPv4Address());
			if (pItem == NULL)
			{
				continue;
			}

			pHost=qgraphicsitem_cast<QGraphicsItemHost*>(pItem);
			if (pHost != NULL)
			{
				pHost->setFlag(QGraphicsItemHost::IconConnectable, 0);
			}
		}
	}

	m_pScene->invalidate();
}

void QWndGraphics::closeEvent()
{
	m_pAbstractItem->closeEvent();
}

void QWndGraphics::reconnect()
{
	if (m_pAbstractItem != NULL)
	{
		m_pAbstractItem->reconnect();
	}
}
