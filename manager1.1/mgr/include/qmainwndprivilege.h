#ifndef QMAINWNDPRIVILEGE_H
#define QMAINWNDPRIVILEGE_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QScrollArea>
#include <QCloseEvent>
#include <QStandardItemModel>
#include <QSignalMapper>
#include <QAction>

#include "qreceiverprivilege.h"
#include "qtreemodel.h"
#include "qframeprivileges.h"

class QMainWndPrivilege : public QMainWindow
{
	Q_OBJECT

public:
	QMainWndPrivilege(QWidget *parent=0);
	~QMainWndPrivilege();

signals:
	void onClosed(QWidget* widget);

protected:
	virtual void closeEvent(QCloseEvent* event);

private slots:
	void clickedGroupsItem(const QModelIndex& index);
	void doubleClickedClassifiedItem(const QModelIndex& index);

	void actionTriggered(int index);

	void privilegesChanged(bool changed);

private slots:
	void recvPrivileges(const PrivilegeGroups groups);
	void createExistedGroup(const QString name, const PrivilegeGroups groups);
	void createGroupSuccess(const QString name, const PrivilegeGroups groups);
	void deleteGroupSuccess(const QString groupName, const PrivilegeGroups groups);
	void createUserSuccess(const QString groupName, const QString userName, const PrivilegeGroups groups);
	void createExistedUser(const QString groupName, const QString userName, const PrivilegeGroups groups);
	void createUserGroupNonexistent(const QString groupName, const QString usrName, const PrivilegeGroups groups);
	void deleteUserSuccess(const QString userName, const PrivilegeGroups groups);
	void modifyPrivilegeSuccess(const QString groupName, const QString usrName, const PrivilegeGroups groups);
	void modifyPrivilegeUserNonexistent(const QString usrName, const PrivilegeGroups groups);

	void onClickedBtnCreateGroup();
	void onClickedBtnDeleteGroup();
	void onClickedBtnCreateUser();
	void onClickedBtnDeleteUser();
	void onClickedBtnSaveUserPrivileges();

private:
	void setMenuState();

	void setGroups(const QString& groupName, const QString& userName, const PrivilegeGroups& groups);

private:
	enum TreeViewUserDataRole
	{
		TreeViewItemRole=Qt::UserRole,
		DateTimeRole,
		PrivilegeUserRole,
		PeerItemRole
	};

	enum TreeViewItemType
	{
		TreeViewGroupsItem,
		TreeViewGroupItem,
		TreeViewUserItem
	};

private:
	enum ActionType
	{
		ActionAddGroup,
		ActionDeleteGroup,
		ActionAddUser,
		ActionDeleteUser,
		ActionSave,
		ActionRefresh,
		ActionEnd
	};

	QAction*	m_pActions[ActionEnd];
	QSignalMapper*	m_pSignalMapper;

private:
	QReceiverPrivilege*	m_pReceiver;

	QSplitter*	m_pSplitter;

	QTreeView*	m_pTreeViewGroups;
	QTreeView*	m_pTreeViewClassified;

	QTreeModel*	m_pTreeModelGroups;
	QStandardItemModel* m_pModelClassified;

	QScrollArea*	m_pScrollAreaPrivileges;
	QFramePrivileges*	m_pFramePrivileges;
};

Q_DECLARE_METATYPE(QModelIndex)

#endif // QMAINWNDPRIVILEGE_H
