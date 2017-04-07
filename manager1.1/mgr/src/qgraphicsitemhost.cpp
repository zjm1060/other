#include "qgraphicsitemhost.h"

#include <QPainter>
#include <qmath.h>

#include "common.h"
#include "qservermanager.h"
#include "qglobalinfo.h"

QString g_hostPixmapKey[QGraphicsItemHost::IconTypeEnd]=
{
	":/images/host.png",
	":/images/host_quit.png",
	":/images/connect.png",
	":/images/disconnect.png"
};

QGraphicsItemHost::QGraphicsItemHost(quint64 uid, QAbstractGraphicsItem* parent)
	: QAbstractGraphicsItem(uid, parent)
	,m_pWndManager(NULL)
{
	memset(m_bFlags, 0, _countof(m_bFlags)*sizeof(bool));
	setFlag(Connectable, 0);

	setPos(0.0, 0.0);
	setStatus(StatusOnline);
}

QGraphicsItemHost::~QGraphicsItemHost()
{
}

#ifdef Q_OS_SOLARIS
void QGraphicsItemHost::loadPixmap()
{
	for (int i=0; i<QGraphicsItemHost::IconTypeEnd; i++)
	{
		findPixmap(g_hostPixmapKey[i]);
	}
}
#endif

void QGraphicsItemHost::setFlag(int func, bool flag)
{
	Q_ASSERT(func < FunctionEnd);

	m_bFlags[func]=flag;
	if (func == Connectable && flag && m_common.status == StatusOffline)
	{
		setTickCount(TickZero);
	}
}

bool QGraphicsItemHost::flag(int func) const
{
	Q_ASSERT(func < FunctionEnd);

	return m_bFlags[func];
}

void QGraphicsItemHost::setServerInfo(const ServerInfo& info)
{
	m_info=info;
}

const ServerInfo& QGraphicsItemHost::serverInfo()
{
	return m_info;
}

void QGraphicsItemHost::setTickCount(TickType tick)
{
	QAbstractGraphicsItem::setTickCount(tick);

	if (m_common.status == StatusOffline)
	{
		setFlag(Connectable, false);
	}
}

QRectF QGraphicsItemHost::iconBoundingRect() const
{
	QRectF r=rect();

	int nHeight=0;
	QPixmap pix=pixmap(m_common.status);
	if (!pix.isNull())
	{
		nHeight=pix.height();
	}

	return QRectF(r.left(), r.top(), r.width(), nHeight);
}

QRectF QGraphicsItemHost::rect() const
{
	Q_ASSERT(m_common.status < StatusEnd);

	QRectF r=QAbstractGraphicsItem::rect();

	int nWidth=0, nHeight=0;
	QPixmap pix=pixmap(m_common.status);
	if (!pix.isNull())
	{
		nWidth=pix.width();
		nHeight=pix.height();
	}

	nWidth=(nWidth>r.width()? nWidth-r.width():0);

	if (flag(Connectable))
	{
		pix=pixmap(IconConnectable);
		if (!pix.isNull())
		{
			nHeight+=pix.height();
		}
	}
	else
	{
		pix=pixmap(IconDisconnected);
		if (!pix.isNull())
		{
			nHeight+=pix.height();
		}
	}

	r.adjust(-nWidth/2, -nHeight/2, nWidth/2, nHeight/2);

	return r;
}

QPixmap QGraphicsItemHost::pixmap(int type) const
{
	Q_ASSERT(type < IconTypeEnd);

	return findPixmap(g_hostPixmapKey[type]);
}

void QGraphicsItemHost::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	QRectF r=rect();

	QPixmap pix=pixmap(m_common.status);
	painter->drawPixmap(r.left()+(r.width()-pix.width())/2, r.top(), pix);

	QPixmap pixm=m_bFlags[Connectable]? pixmap(IconConnectable):pixmap(IconDisconnected);
	painter->drawPixmap(r.left()+(r.width()-pix.width())/2, r.top()+pix.height(), pixm);

	painter->restore();

	QAbstractGraphicsItem::paint(painter, option, widget);
}

void QGraphicsItemHost::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	Q_UNUSED(event);

	if (m_pWndManager == NULL)
	{
		if (flag(Connectable))
		{
			Server server;
			QServerManager::checkConnect(QGlobalInfo::instance()->local(), m_info, NULL, &server);

			m_pWndManager=new QWndManager(m_info.hostName+"-"+m_info.usrName, server.addr, server.port);

			connect(m_pWndManager, SIGNAL(onClosed(QWidget*)), this, SLOT(wndClosed(QWidget*)));

			m_pWndManager->show();
		}
	}
	else
	{
		m_pWndManager->raise();
		m_pWndManager->activateWindow();
	}
}

void QGraphicsItemHost::wndClosed(QWidget* widget)
{
	if (widget == NULL)
	{
		return;
	}

	if (widget == m_pWndManager)
	{
		SAFE_DELETE_CLASS(m_pWndManager);
	}
}

void QGraphicsItemHost::closeWindow()
{
	if (m_pWndManager != NULL)
	{
		m_pWndManager->close();
	}
}

void QGraphicsItemHost::reconnect()
{
	if (m_pWndManager != NULL)
	{
		m_pWndManager->reconnect();
	}
}
