#ifndef QWNDGROUPMANAGER_H
#define QWNDGROUPMANAGER_H

#include <QMainWindow>
#include <QAction>
#include <QMap>

#include "qfiletransfergroupmanager.h"
#include "qsyncgroupcommitmanager.h"
#include "qsyncgroupupdatemanager.h"
#include "qsyncgroupreleasemanager.h"
#include "qinspectmanager.h"
#include "qconfiguregroupmanager.h"
#include "qmainwndprivilege.h"
#include "qreceiverprivilege.h"

class QWndGroupManager : public QMainWindow
{
	Q_OBJECT

public:
	QWndGroupManager(QWidget *parent=0);
	~QWndGroupManager();

	enum ActionType
	{
		ActionFileTransfer,
		ActionCommitSync,
		ActionUpdateSync,
		ActionFileRelease,
		ActionInspect,
		ActionConfigure,
		ActionPrivileges,
		ActionEnd
	};

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

signals:
	void onClosed(QWidget* widget);

private slots:
	void onActionToggled(QAction* action);

	void onToggledActionPrivalege();
	void onClosedPrivelege(QWidget* widget);

	void labelSettingChanged(const QMap<QString, QString>& configure);

private slots:
	void modifyPasswordSuccess(const QString usrName, const PrivilegeUser user);
	void modifyPasswordUserNonexistent(const QString usrName);

private:
	void loadConfigure();

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
	void init();

private:
	QActionGroup*	m_pGroup;

	QAction*	m_pAction[ActionEnd];
	QToolBar*	m_pBar;

	QVBoxLayout* m_pLayout;

	QReceiverPrivilege*	m_pReceiver;

	QFileTransferGroupManager*		m_pFileTransferGroupManager;
	QSyncGroupCommitManager*		m_pSyncGroupCommitManager;
	QSyncGroupUpdateManager*		m_pSyncGroupUpdateManager;
	QSyncGroupReleaseManager*		m_pSyncGroupReleaseManager;
	QInspectManager*				m_pInspectManager;
	QConfigureGroupManager*			m_pConfigureManager;
	QMainWndPrivilege*				m_pPrivilegeManager;
};

#endif // QWNDGROUPMANAGER_H
