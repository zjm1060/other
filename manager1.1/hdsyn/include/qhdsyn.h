#ifndef QHDSYN_H
#define QHDSYN_H

#include <QObject>
#include <QMap>

#include "qviewcontroller.h"
#include "qdlglogin.h"
#include "qwndgroupmanager.h"

class QHdsyn : public QObject, public QAbstractReceiver
{
	Q_OBJECT

public:
	QHdsyn(QObject *parent=0);
	~QHdsyn();

public:
	void setServerList(const QHash<QString, QPair<ServerInfo, int> >& servers);

public:
	void setLocalServer(const ServerInfo& info);

public:
	void openLoginWnd();

public:
	virtual void recvLocalServer(int flag, const ServerInfo& info);
	virtual void recvLocalServerQuit();
	virtual void recvInformStatusChanged(quint64 sign);
	virtual void recvRemoteServerList(int flag, const QList<ServerInfo>& servers);

public slots:
	void logged();

	void logout(const QString& user);
	void forceLogout(const QString user);

	void appendTip(QInfoManager::InfoType type, const QString text);

private:
	void init();

private:
	QHash<QString, QPair<ServerInfo, int> >	m_servers;

private:
	quint32		m_nUsrID;		//用户ID,用于接收消息

	QViewController*	m_pView;

	QDlgLogin*			m_pDlgLogin;
	QWndGroupManager*	m_pMainWnd;
};

#endif // QHDSYN_H
