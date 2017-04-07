#ifndef QMAINWND_H
#define QMAINWND_H

#include <QMainWindow>
#include <QScrollArea>
#include <QActionGroup>
#include <QAction>
#include <QToolBar>

#include "qheartbeat.h"
#include "qservermanager.h"
#include "qwndgraphics.h"
#include "qlogicaltopo.h"
#include "qwndmanager.h"
#include "qwndgroupmanager.h"

class QMainWnd : public QMainWindow
{
	Q_OBJECT

public:
	QMainWnd();
	~QMainWnd();

public:
	void startServers();
	void startHeartBeat();

public slots:
	void wndClosed(QWidget* widget);

	void remoteServerComing(const QServerManager::ServerInfo& info);
	void remoteServerQuit(const QServerManager::ServerInfo& info);

	void localNetStatusChanged(bool connecting);

	void onActionToggled(QAction* action);

	void setAlives(quint32 route, quint32 gateway, QVector<quint32> alives);

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
 	void reconnect(QWndManager* manager, const QServerManager::ServerInfo& info);

	bool canConnected(const QServerManager::ServerInfo& info, QServerManager::Server& server) const;

private:
	QActionGroup*	m_pGroup;

	enum ActionType
	{
		ActionWorkgroup,
		ActionLocalHost,
		ActionEnd
	};

	QAction*	m_pAction[ActionEnd];
	QToolBar*	m_pBar;

	QWndManager*		m_pWndManager;
	QWndGroupManager*	m_pWndGroupManager;

private:
	QHeartBeat*		m_pHeartBeat;

	QWndGraphics*	m_pWndGraphics;
	QLogicalTopo*	m_pLogicalTopo;
};

#endif // QMAINWND_H
