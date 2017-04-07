#ifndef QNONTOPOMAINWND_H
#define QNONTOPOMAINWND_H

#include <QMainWindow>
#include <QScrollArea>

#include "qlistgroupwidget.h"
#include "qheartbeat.h"
#include "qservermanager.h"
#include "qwndgroupmanager.h"

class QNonTopoMainWnd : public QMainWindow
{
	Q_OBJECT

public:
	QNonTopoMainWnd();
	~QNonTopoMainWnd();

public:
	void startServers();
	void startHeartBeat();

public:
	virtual bool eventFilter(QObject* watched, QEvent* event);

public slots:
	void itemDoubleClicked(const QString& title, const QModelIndex& index);

	void managerDestroy(QWidget* widget);

	void remoteServerComing(const QServerManager::ServerInfo& serverInfo);
	void remoteServerQuit(const QServerManager::ServerInfo& serverInfo);

	void localNetStatusChanged(bool connecting);

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
	void reconnect(const QString& title, const QModelIndex& index);
	void serverQuit(QStandardItemModel* model, const QModelIndex& index);

	bool canConnected(const QServerManager::ServerInfo& info, QServerManager::Server& server) const;

private:
	QScrollArea*			m_pScrollArea;
	QListGroupWidget*		m_pListGroupWidget;

	QWndGroupManager*		m_pWndGroupManager;

	QHeartBeat*		m_pHeartBeat;

	QMap<QModelIndex, QWidget*>	m_mapWidget;

	enum iconType
	{
		IconWorkgroup,
		IconServer,
		IconMultiServer,
		IconServerQuit,
		IconEnd
	};

	QIcon	m_icon[IconEnd];
};

#endif // QNONTOPOMAINWND_H
