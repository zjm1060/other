#include "qhshow.h"

#include <QSignalMapper>
#include <QMenu>
#include <QIcon>
#include <QPixmapCache>
#include <QStylePainter>
#include <QStyleOptionDockWidgetV2>
#include <QApplication>
#include <QMouseEvent>

#include "common.h"
#include "qpaneltools.h"

#define HSHOW_FRAME_HEIGHT	42

QVector<QHShow*>		QHShow::g_sWidgets;

QHShow::QHShow(const QRect& geometry, QWidget *parent/* =0 */)
	: QDockWidget(parent)
	,m_geometry(geometry)
	,m_pTrayIcon(NULL)
	,m_pMenu(NULL)
	,m_pSignalMapper(NULL)
	,m_bFocus(false)
	,m_bShow(false)
{
	memset(m_pActions, 0, sizeof(QAction*)*ActionEnd);

	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
//	setAttribute(Qt::WA_TranslucentBackground, true);

	createActions();

//	setWindowOpacity(0.9);
	setIcon(QIcon(":/images/hshow.png"));
	setWindowTitle(tr("HShow"));

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setGeometry(0, 0, geometry.width(), HSHOW_FRAME_HEIGHT);

	m_pPanel=new QPanelTools;
	setWidget(m_pPanel);

	connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(focusChanged(QWidget*, QWidget*)));

	showWidget();
}

QHShow::~QHShow()
{
	QPixmapCache::clear();
}

QHShow*	QHShow::createWidget(const QRect& geometry)
{
	QHShow* pWidget=NULL;

	pWidget=new QHShow(geometry);
	g_sWidgets.append(pWidget);

	if (pWidget != NULL)
	{
		pWidget->show();
	}

	return pWidget;
}

void QHShow::setIcon(const QIcon& icon)
{
	setWindowIcon(icon);

	if (m_pTrayIcon != NULL)
	{
		m_pTrayIcon->setIcon(icon);
		m_pTrayIcon->show();
	}
}

void QHShow::createActions()
{
	m_pMenu=new QMenu(this);

//	m_pActions[ActionConfigure]=new QAction(QIcon(":/images/configure.png"), tr("&Configure"), this);
	m_pActions[ActionExit]=new QAction(tr("E&xit"), this);

//	m_pMenu->addAction(m_pActions[ActionConfigure]);
//	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionExit]);
	
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));

	if (QSystemTrayIcon::isSystemTrayAvailable())
	{
		m_pTrayIcon = new QSystemTrayIcon(this);
		m_pTrayIcon->setContextMenu(m_pMenu);

		connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	}
}

void QHShow::actionTriggered(int index)
{
	switch (index)
	{
//	case ActionConfigure:
//		break;
	case ActionExit:
		{
			qApp->quit();
		}
		break;
	default:
		break;
	}
}

void QHShow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	 case QSystemTrayIcon::Trigger:
	 case QSystemTrayIcon::DoubleClick:
		 {
			 for (int i=0; i<g_sWidgets.size(); i++)
			 {
				 g_sWidgets[i]->showWidget();
			 }
		 }
		 break;
	 case QSystemTrayIcon::MiddleClick:
		 break;
	 default:
		 break;
	}
}

void QHShow::paintEvent(QPaintEvent* event)
{
	QDockWidget::paintEvent(event);

	QPainter p(this);

	QPixmap pixmap=findPixmap(":/images/frame.png");
	if (!pixmap.isNull())
	{
		QRect r=geometry();
		pixmap=pixmap.scaledToHeight(r.height());

		int nWidth=pixmap.width()/3;

		QPixmap leftPixmap=pixmap.copy(0, 0, nWidth, pixmap.height());
		QPixmap midPixmap=pixmap.copy(nWidth, 0, nWidth, pixmap.height());
		QPixmap rightPixmap=pixmap.copy(pixmap.width()-nWidth, 0, nWidth, pixmap.height());

		QRect rect(r.left(), r.top(), nWidth, r.height());
		p.drawPixmap(rect, leftPixmap);

		rect.moveLeft(nWidth);
		rect.setWidth(r.width()-nWidth*2);
		p.drawTiledPixmap(rect, midPixmap);

		rect=QRect(r.right()-nWidth, r.top(), nWidth, r.height());
		p.drawPixmap(rect, rightPixmap);
	}
}

void QHShow::showWidget()
{
	m_bShow=true;

	move(m_geometry.left(), m_geometry.top());

	repaint();
}

void QHShow::hideWidget()
{
	m_bShow=false;

	move(m_geometry.left(), m_geometry.top()-HSHOW_FRAME_HEIGHT+2);
}

void QHShow::enterEvent(QEvent* event)
{
	showWidget();

	QDockWidget::enterEvent(event);
}

void QHShow::leaveEvent(QEvent* event)
{
	if (!m_bFocus)
	{
		hideWidget();
	}

	QDockWidget::leaveEvent(event);
}

void QHShow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		if (m_pMenu != NULL)
		{
			m_pMenu->popup(event->pos());

			return;
		}
	}

	QDockWidget::mousePressEvent(event);
}

void QHShow::focusChanged(QWidget* old, QWidget* now)
{
	Q_UNUSED(old);

	m_bFocus=(now == NULL? false:true);

	if (!m_bFocus)
	{
		for (int i=0; i<g_sWidgets.size(); i++)
		{
			g_sWidgets[i]->hideWidget();
		}
	}
}
