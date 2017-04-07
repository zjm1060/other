#ifndef QWNDMANAGER_H
#define QWNDMANAGER_H

#include <QMainWindow>
#include <QAction>

#include "qclient.h"
#include "qviewcontroller.h"
#include "qsystemmanager.h"
#include "qprocessmanager.h"
#include "qsyncmanager.h"

class QWndManager : public QMainWindow
{
	Q_OBJECT

public:
	QWndManager(const QString& hostname, const QString& ipAddr, quint16 port, QWidget *parent=0);
	~QWndManager();

	enum ActionType
	{
		ActionSystemMgr=0,
		ActionProcessMgr,
//		ActionRealTimeLib,
		ActionSynchronize,
		ActionNetwork,
		ActionResourceMgr,
		ActionDatabase,
		ActionEnd
	};

public:
	void reconnect();
	void reconnect(const QString& addr, quint16 port);

	bool isConnected();

	QSystemManager* systemManager();
	QResourceManager* resourceManager();
	QProcessManager* processManager();

signals:
	void onClosed(QWidget* widget);

private slots:
	void clientQuit();
	void clientRecvMessage(QConnection* sender, QByteArray buffer);
	void clientStateChanged(QAbstractSocket::SocketState socketState);

	void onActionToggled(QAction* action);

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
	void init();
	void connectToHost();
	void setTitleBar(bool bConnected);

private:
	QActionGroup*	m_pGroup;

	QAction*	m_pAction[ActionEnd];
	QToolBar*	m_pBar;

	QClient*	m_pClient;
	QString		m_strPeerName;

	QVBoxLayout* m_pLayout;

	QViewController*	m_pController;

	QSystemManager*		m_pSystemManager;
	QProcessManager*	m_pProcessManager;
	QResourceManager*	m_pResourceManager;
	QSyncManager*		m_pSyncManager;
};

#endif // QWNDMANAGER_H
