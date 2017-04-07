#include "qgraphicsitemhost.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <qmath.h>

#include "common.h"

QGraphicsItemHost::QGraphicsItemHost(QAbstractGraphicsItem* parent)
	: QAbstractGraphicsItem(parent),
	m_pWndManager(NULL)
{
	m_common.pixmap[StatusOnline]=QPixmap(":/images/host.png");
	m_common.pixmap[StatusOffline]=QPixmap(":/images/host_quit.png");

	memset(m_nFlags, 0, _countof(m_nFlags));
	setFlag(IconConnectable, 0);

	setOrigin(0, 0);
	setStatus(StatusOnline);
}

QGraphicsItemHost::~QGraphicsItemHost()
{
	if (m_pWndManager != NULL)
	{
		SAFE_DELETE_CLASS(m_pWndManager);
	}
}

void QGraphicsItemHost::setFlag(QGraphicsItemHost::IconType flag, int status)
{
	m_nFlags[flag]=status;

	switch (flag)
	{
	case IconConnectable:
		{
			switch (status)
			{
			case 0:
				m_pixmap[IconConnectable]=QPixmap(":/images/disconnect.png");
				break;
			case 1:
				m_pixmap[IconConnectable]=QPixmap(":/images/connect.png");
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

int QGraphicsItemHost::flag(IconType type)
{
	Q_ASSERT(type <= IconTypeEnd);

	return m_nFlags[type];
}

void QGraphicsItemHost::setTickCount(TickType tick)
{
	QAbstractGraphicsItem::setTickCount(tick);

	if (m_common.status == StatusOffline)
	{
		setFlag(IconConnectable, 0);
	}

	if (m_pWndManager != NULL && flag(IconConnectable) == 1)
	{
		if (!m_pWndManager->isConnected())
		{
			m_pWndManager->reconnect();
		}
	}
}

QRectF QGraphicsItemHost::rect() const
{
	QRectF r=QAbstractGraphicsItem::rect();

	r.adjust(-8, -8, 8, 8);

	return r;
}

void QGraphicsItemHost::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	QRectF r=rect();

	painter->drawPixmap(r.left()+(r.width()-m_common.pixmap[m_common.status].width())/2, r.top(), m_common.pixmap[m_common.status]);

	QAbstractGraphicsItem::paint(painter, option, widget);

	if ((option->state & QStyle::State_Selected) != 0)
	{
		drawBounding(painter);
	}

	painter->drawPixmap(r.left(), r.bottom()-16, 16, 16, m_pixmap[IconConnectable]);

	painter->restore();
}

void QGraphicsItemHost::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	Q_UNUSED(event);

	if (m_pWndManager == NULL)
	{
		if (flag(IconConnectable) == 1)
		{
			QString strHostName=data(DataHostName).toString();
			QString strAddress=data(DataIPAddress).toString();
			quint16 nPort=(quint16)data(DataPort).toUInt();

			m_pWndManager=new QWndManager(strHostName, strAddress, nPort);
			int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
			int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
			m_pWndManager->setMinimumSize(nMinWidth, nMinHeight);

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

void QGraphicsItemHost::closeEvent()
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
