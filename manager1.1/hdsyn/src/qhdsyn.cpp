#include "qhdsyn.h"

#include <QPixmapCache>

#include "qprocesssocket.h"
#include "qmessageparser.h"
#include "qdeafactory.h"
#include "qglobalinfo.h"
#include "qreceiverparser.h"
#include "qndc.h"

QHdsyn::QHdsyn(QObject *parent)
	: QObject(parent)
	,m_pDlgLogin(NULL)
	,m_pMainWnd(NULL)
{
	m_pView=new QViewController(this);
	QProcessSocket::instance()->setView(m_pView);

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);
	QProcessSocket::instance()->setUser(m_nUsrID);

	init();

	connect(QInfoManager::instance().get(), SIGNAL(appendTip(QInfoManager::InfoType, const QString)), 
		this, SLOT(appendTip(QInfoManager::InfoType, const QString)));
	connect(QGlobalInfo::instance(), SIGNAL(logout(const QString)), this, SLOT(forceLogout(const QString)));

	openLoginWnd();
	QProcessSocket::instance()->connectToHost();
}

QHdsyn::~QHdsyn()
{
	QProcessSocket::instance()->close();

	QPixmapCache::clear();
}

void QHdsyn::init()
{
	//加载数据加密模块
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	QDEAFactory::instance()->loadLibrary("libdead1", "CreateDEAInstance");
#else
	QDEAFactory::instance()->loadLibrary("libdea1", "CreateDEAInstance");
#endif
#else
#ifdef QT_DEBUG
	QDEAFactory::instance()->loadLibrary("libdead", "CreateDEAInstance");
#else
	QDEAFactory::instance()->loadLibrary("libdea", "CreateDEAInstance");
#endif
#endif

	//加载消息模块
#ifdef	Q_OS_WIN
#ifdef QT_DEBUG
	QMessageParser::instance()->loadLibrary("libmsgd1", "CreateMessageInstance");
#else
	QMessageParser::instance()->loadLibrary("libmsg1", "CreateMessageInstance");
#endif
#else
#ifdef QT_DEBUG
	QMessageParser::instance()->loadLibrary("libmsgd", "CreateMessageInstance");
#else
	QMessageParser::instance()->loadLibrary("libmsg", "CreateMessageInstance");
#endif
#endif
	
	//加载文件比较模块
#ifdef	Q_OS_WIN
#ifdef QT_DEBUG
	QFileCompareFactory::instance()->loadLibrary("libfcd1", "CreateFileCompareInstance");
#else
	QFileCompareFactory::instance()->loadLibrary("libfc1", "CreateFileCompareInstance");
#endif
#else
#ifdef QT_DEBUG
	QFileCompareFactory::instance()->loadLibrary("libfcd", "CreateFileCompareInstance");
#else
	QFileCompareFactory::instance()->loadLibrary("libfc", "CreateFileCompareInstance");
#endif
#endif

//	QProcessSocket::instance()->connectToHost();
}

void QHdsyn::openLoginWnd()
{
	if (!QGlobalInfo::instance()->isLogin())
	{
		if (m_pMainWnd != NULL)
		{
			m_pMainWnd->close();
			m_pMainWnd=NULL;
		}

		if (m_pDlgLogin == NULL)
		{
			m_pDlgLogin=new QDlgLogin;

			Q_ASSERT_NO_NULL(m_pDlgLogin);

			connect(m_pDlgLogin, SIGNAL(logged()), this, SLOT(logged()));
		}
		m_pDlgLogin->show();

		m_pDlgLogin->setInputFocus();
	}
}

void QHdsyn::appendTip(QInfoManager::InfoType type, const QString text)
{
	if (m_pDlgLogin != NULL)
	{
		m_pDlgLogin->appendTip(type, text);
	}
}

void QHdsyn::setServerList(const QHash<QString, QPair<ServerInfo, int> >& servers)
{
	QList<QPair<ServerInfo, int> > listServers=servers.values();
	QGlobalInfo::instance()->setServerList(listServers);

	if (m_pMainWnd != NULL)
	{
		m_pMainWnd->setServerList(listServers);
	}
}

void QHdsyn::setLocalServer(const ServerInfo& info)
{
	QGlobalInfo::instance()->setLocal(info);
}

void QHdsyn::recvLocalServer(int flag, const ServerInfo& info)
{
	if (flag == QuerySucceed)
	{
		setLocalServer(info);

		logged();
	}
}

void QHdsyn::logged()
{
	if (QGlobalInfo::instance()->isLogin())
	{
		SAFE_DELETE_CLASS(m_pDlgLogin);

		QProcessSocket::instance()->queryRemoteServerList();

		if (m_pMainWnd == NULL)
		{
			m_pMainWnd=new QWndGroupManager;
			m_pMainWnd->show();
		}
	}
}

void QHdsyn::recvLocalServerQuit()
{
	QProcessSocket::instance()->close();

	ServerInfo info;
	info.count=0;

	setLocalServer(info);
	openLoginWnd();
}

void QHdsyn::recvInformStatusChanged(quint64 sign)
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
		}
	}
}

void QHdsyn::recvRemoteServerList(int flag, const QList<ServerInfo>& servers)
{
	if (flag == QuerySucceed)
	{
		QMutableHashIterator<QString, QPair<ServerInfo, int> > it(m_servers);
		while (it.hasNext())
		{
			it.next();

			QPair<ServerInfo, int>& peer=it.value();
			peer.second++;
		}

		QListIterator<ServerInfo> iter(servers);
		while (iter.hasNext())
		{
			const ServerInfo& server=iter.next();

			bool bExisted=false;
			if (m_servers.contains(server.hostName))
			{
				QHash<QString, QPair<ServerInfo, int> >::iterator it=m_servers.find(server.hostName);
				while (it != m_servers.end() && it.key() == server.hostName)
				{
					if (server == it.value().first)
					{
						bExisted=true;

						it.value().second=0;

						break;
					}

					it++;
				}
			}

			if (!bExisted)
			{
				m_servers.insertMulti(server.hostName, QPair<ServerInfo, int>(server, 0));
			}
		}
		
		setServerList(m_servers);
	}
}

void QHdsyn::forceLogout(const QString user)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("The user account <%1> was deleted!").arg(user));

	logout(user);
}

void QHdsyn::logout(const QString& user)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("The user <%1> logout.").arg(user));

	QNdc::instance()->pop();

	QGlobalInfo::instance()->setLogin(false);

	openLoginWnd();
}
