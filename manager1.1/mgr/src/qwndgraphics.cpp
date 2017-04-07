#include "qwndgraphics.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QHostAddress>
#include <QMouseEvent>
#include <QPixmapCache>

#ifndef QT_NO_OPENGL
#include <QtOpenGL/QtOpenGL>
#endif

#include "common.h"
#include "qinfomanager.h"
#include "qreceiverparser.h"
#include "qprocesssocket.h"
#include "qglobalinfo.h"
#include "qmessageboxlogout.h"
#include "qndc.h"

QWndGraphics::QWndGraphics(QWidget* parent)
:QGraphicsView(parent)
,m_pLoginPanel(NULL)
,m_bLeftBtnPressed(false)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);
	QProcessSocket::instance()->setUser(m_nUsrID);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setAttribute(Qt::WA_DeleteOnClose);

	setViewportUpdateMode(FullViewportUpdate);

#ifdef Q_OS_WIN
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
#endif

	setBackgroundBrush(QBrush(Qt::black));

#ifndef QT_NO_OPENGL
	setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif

	setMinimumSize(640, 480);

	m_pScene=new QWndGraphicsScene(this);
	setScene(m_pScene);

	connect(m_pScene, SIGNAL(logout()), this, SLOT(logout()));
	connect(m_pScene, SIGNAL(centerOn(QPointF)), this, SLOT(setCenterPos(QPointF)));

	connect(QGlobalInfo::instance(), SIGNAL(logout(const QString)), this, SLOT(forceLogout(const QString)));

	openLoginWnd();

	connect(QInfoManager::instance().get(), SIGNAL(refresh()), this, SLOT(refresh()));
}

QWndGraphics::~QWndGraphics()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
	SAFE_DELETE_CLASS(m_pLoginPanel);
}

void QWndGraphics::wheelEvent(QWheelEvent* event)
{
	Q_UNUSED(event);
}

void QWndGraphics::mouseMoveEvent(QMouseEvent* event)
{
	if (QGlobalInfo::instance()->isLogin() && m_bLeftBtnPressed)
	{
		m_ptEnd=event->pos();

		QPointF pt=mapToScene(rect().center());
		QPointF point=mapToScene(m_ptEnd)-mapToScene(m_ptStart);
		centerOn(pt.x()-point.x(), pt.y()-point.y());

		m_ptStart=m_ptEnd;

		return;
	}

	QGraphicsView::mouseMoveEvent(event);
}

void QWndGraphics::mousePressEvent(QMouseEvent* event)
{
	if (QGlobalInfo::instance()->isLogin())
	{
		if (event->button() == Qt::LeftButton)
		{
			m_bLeftBtnPressed=true;
			m_ptStart=m_ptEnd=event->pos();
		}
	}

	QGraphicsView::mousePressEvent(event);
}

void QWndGraphics::mouseReleaseEvent(QMouseEvent* event)
{
	if (QGlobalInfo::instance()->isLogin())
	{
		m_bLeftBtnPressed=false;
		m_ptStart=m_ptEnd=QPoint();
	}

	QGraphicsView::mouseReleaseEvent(event);
}

void QWndGraphics::openLoginWnd()
{
	Q_ASSERT_NO_NULL(m_pScene);
	if (!QGlobalInfo::instance()->isLogin())
	{
		m_pScene->clear();

		if (m_pLoginPanel == NULL)
		{
			m_pLoginPanel=new QLoginGraphicsScene;

			Q_ASSERT_NO_NULL(m_pLoginPanel);

			connect(m_pLoginPanel->m_pWidgetLogin, SIGNAL(logged()), this, SLOT(logged()));
		}

		m_pScene->addItem(m_pLoginPanel);

		QRectF r=sceneRect();
		QRectF rt=m_pLoginPanel->boundingRect();

		if (rt.isValid())
		{
			m_pLoginPanel->setPos(r.left()+(r.width()-rt.width())/2, r.top()+(r.height()-rt.height())/2);
		}

		centerOn(m_pLoginPanel);

		m_pLoginPanel->m_pWidgetLogin->setInputFocus();
	}
}

void QWndGraphics::logged()
{
	if (QGlobalInfo::instance()->isLogin())
	{
		m_pScene->removeItem(m_pLoginPanel);

		m_pScene->logged();

		QProcessSocket::instance()->queryRemoteServerList();
		QProcessSocket::instance()->queryTopology();

		m_pScene->invalidate();
	}
}

void QWndGraphics::logout()
{
	QMessageBoxLogout dlg(findPixmap(":/images/logout.png").scaled(64, 64), this);
	dlg.setModal(true);
	if (dlg.exec() == QMessageBox::AcceptRole)
	{
		logout(QGlobalInfo::instance()->user().name);
	}
}

void QWndGraphics::forceLogout(const QString user)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("The user account <%1> was deleted!").arg(user));

	logout(user);
}

void QWndGraphics::logout(const QString& user)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("The user <%1> logout.").arg(user));

	QNdc::instance()->pop();

	QGlobalInfo::instance()->setLogin(false);

	openLoginWnd();

	invalidateScene();
}

void QWndGraphics::setCenterPos(QPointF pt)
{
	centerOn(pt);

	invalidateScene();
}

void QWndGraphics::refresh()
{
	if (m_pScene != NULL)
	{
		m_pScene->invalidate();
	}
}

void QWndGraphics::setLocalServer(const ServerInfo& info)
{
	if (m_pScene != NULL)
	{
		m_pScene->setLocalServer(info, mapToScene(QCursor::pos()).toPoint());
	}
}

void QWndGraphics::closeEvent()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	if (m_pScene != NULL)
	{
		m_pScene->closeEvent();
	}

	QPixmapCache::clear();
}

void QWndGraphics::recvLocalServer(int flag, const ServerInfo& info)
{
	if (flag == QuerySucceed)
	{
		setLocalServer(info);

		Q_ASSERT_NO_NULL(m_pScene);
		logged();
	}
}

void QWndGraphics::recvLocalServerQuit()
{
	QProcessSocket::instance()->close();

	ServerInfo info;
	info.count=0;

	setLocalServer(info);
}

void QWndGraphics::recvInformStatusChanged(quint64 sign)
{
	if ((sign & ((quint64)1<<UTIL_TYPE_LOCAL_SERVER)) != 0 || (sign & (quint64)1<<UTIL_TYPE_NETWORK_ADAPTER) != 0)
	{
		QProcessSocket::instance()->queryLocalServer();
	}
	else
	{
		if (QGlobalInfo::instance()->isLogin())
		{
			if ((sign & ((quint64)1<<UTIL_TYPE_REMOTE_SERVER)) != 0)
			{
				QProcessSocket::instance()->queryRemoteServerList();
			}

			if ((sign & ((quint64)1<<UTIL_TYPE_TOPOLOGY)) != 0)
			{
				QProcessSocket::instance()->queryTopology();
			}
		}
	}
}

void QWndGraphics::recvRemoteServerList(int flag, const QList<ServerInfo>& servers)
{
	if (flag == QuerySucceed)
	{
		if (m_pScene != NULL)
		{
			m_pScene->setRemoteServerList(servers);
		}
	}
}

void QWndGraphics::recvTopology(int flag, const GraphTopo& graph)
{
	if (flag == QuerySucceed)
	{
		if (m_pScene != NULL)
		{
			m_pScene->setTopologyGraph(graph);
		}
	}
}
