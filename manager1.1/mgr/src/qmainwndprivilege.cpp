#include "qmainwndprivilege.h"

#include <QToolBar>
#include <QVBoxLayout>
#include <QMessageBox>

#include "common.h"
#include "qimessage.h"
#include "qinfomanager.h"
#include "qdlgcreategroup.h"
#include "qdlgcreateuser.h"
#include "qutildef.h"

QMainWndPrivilege::QMainWndPrivilege(QWidget *parent/* =0 */)
	: QMainWindow(parent)
	,m_pModelClassified(NULL)
{
	setWindowTitle(tr("User Accounts and Privilege Management"));
	setWindowIcon(QIcon(":/images/user_icon.png"));

	m_pReceiver=new QReceiverPrivilege;
	connect(m_pReceiver, SIGNAL(receivePrivileges(const PrivilegeGroups)), this, SLOT(recvPrivileges(const PrivilegeGroups)));
	connect(m_pReceiver, SIGNAL(createExistedGroup(const QString, const PrivilegeGroups)),
		this, SLOT(createExistedGroup(const QString, const PrivilegeGroups)));
	connect(m_pReceiver, SIGNAL(createGroupSuccess(const QString, const PrivilegeGroups)),
		this, SLOT(createGroupSuccess(const QString, const PrivilegeGroups)));
	connect(m_pReceiver, SIGNAL(deleteGroupSuccess(const QString, const PrivilegeGroups)), 
		this, SLOT(deleteGroupSuccess(const QString, const PrivilegeGroups)));

	connect(m_pReceiver, SIGNAL(createUserSuccess(const QString, const QString, const PrivilegeGroups)),
		this, SLOT(createUserSuccess(const QString, const QString, const PrivilegeGroups)));
	connect(m_pReceiver, SIGNAL(createExistedUser(const QString, const QString, const PrivilegeGroups)),
		this, SLOT(createExistedUser(const QString, const QString, const PrivilegeGroups)));
	connect(m_pReceiver, SIGNAL(createUserGroupNonexistent(const QString, const QString, const PrivilegeGroups)), 
		this, SLOT(createUserGroupNonexistent(const QString, const QString, const PrivilegeGroups)));
	connect(m_pReceiver, SIGNAL(deleteUserSuccess(const QString, const PrivilegeGroups)), 
		this, SLOT(deleteUserSuccess(const QString, const PrivilegeGroups)));

	connect(m_pReceiver, SIGNAL(modifyPrivilegeSuccess(const QString, const QString, const PrivilegeGroups)), 
		this, SLOT(modifyPrivilegeSuccess(const QString, const QString, const PrivilegeGroups)));
	connect(m_pReceiver, SIGNAL(modifyPrivilegeUserNonexistent(const QString, const PrivilegeGroups)), 
		this, SLOT(modifyPrivilegeUserNonexistent(const QString, const PrivilegeGroups)));

	int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
	int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
	setMinimumSize(nMinWidth, nMinHeight);

	QToolBar* pToolBar=addToolBar(tr("Tools"));
	pToolBar->setMovable(false);
	pToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

	m_pActions[ActionAddGroup]=new QAction(QIcon(":/images/group_add.png"), tr("New Group"), pToolBar);
	m_pActions[ActionDeleteGroup]=new QAction(QIcon(":/images/group_delete.png"), tr("Delete Group"), pToolBar);
	m_pActions[ActionAddUser]=new QAction(QIcon(":/images/user_add.png"), tr("New User"), pToolBar);
	m_pActions[ActionDeleteUser]=new QAction(QIcon(":/images/user_delete.png"), tr("Delete User"), pToolBar);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("Refresh"), pToolBar);
	m_pActions[ActionSave]=new QAction(QIcon(":/images/save.png"), tr("Save"), pToolBar);

	pToolBar->addAction(m_pActions[ActionAddGroup]);
	pToolBar->addAction(m_pActions[ActionDeleteGroup]);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pActions[ActionAddUser]);
	pToolBar->addAction(m_pActions[ActionDeleteUser]);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pActions[ActionSave]);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pActions[ActionRefresh]);

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));

	m_pSplitter=new QSplitter(Qt::Horizontal);
	setCentralWidget(m_pSplitter);

	m_pTreeViewGroups=new QTreeView;
	m_pTreeViewGroups->setHeaderHidden(true);
	m_pTreeViewGroups->setExpandsOnDoubleClick(false);
	m_pSplitter->addWidget(m_pTreeViewGroups);

	connect(m_pTreeViewGroups, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickedGroupsItem(const QModelIndex&)));

	m_pTreeModelGroups=new QTreeModel(1);
	m_pTreeViewGroups->setModel(m_pTreeModelGroups);

	m_pTreeViewClassified=new QTreeView;
	m_pTreeViewClassified->setAlternatingRowColors(true);
	m_pTreeViewClassified->setRootIsDecorated(false);
	m_pSplitter->addWidget(m_pTreeViewClassified);

	connect(m_pTreeViewClassified, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClickedClassifiedItem(const QModelIndex&)));

	m_pScrollAreaPrivileges=new QScrollArea;
	QVBoxLayout* pLayout=new QVBoxLayout;
	m_pScrollAreaPrivileges->setLayout(pLayout);

	m_pFramePrivileges=new QFramePrivileges;
	connect(m_pFramePrivileges, SIGNAL(privilegesChanged(bool)), this, SLOT(privilegesChanged(bool)));

	pLayout->addWidget(m_pFramePrivileges);
	m_pSplitter->addWidget(m_pScrollAreaPrivileges);

	m_pSplitter->setStretchFactor(1, 2);
	m_pSplitter->setStretchFactor(2, 2);

	m_pScrollAreaPrivileges->setVisible(false);

	m_pReceiver->queryPrivileges();

	setMenuState();
}

QMainWndPrivilege::~QMainWndPrivilege()
{
	SAFE_DELETE_CLASS(m_pReceiver);
	SAFE_DELETE_CLASS(m_pTreeModelGroups);
}

void QMainWndPrivilege::closeEvent(QCloseEvent* event)
{
	emit onClosed(this);

	event->accept();
}

void QMainWndPrivilege::createExistedGroup(const QString name, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Error, 
		tr("Failed to create the user group <%1>! The user group already exists!").arg(name));

	setGroups(name, "", groups);
}

void QMainWndPrivilege::createGroupSuccess(const QString name, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("Create the user group <%1> successfully.").arg(name));

	setGroups(name, "", groups);
}

void QMainWndPrivilege::deleteGroupSuccess(const QString groupName, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("Delete the user group <%1> successfully.").arg(groupName));

	setGroups("", "", groups);
}

void QMainWndPrivilege::setGroups(const QString& groupName, const QString& userName, const PrivilegeGroups& groups)
{
	Q_ASSERT_NO_NULL(m_pTreeModelGroups);

	m_pTreeModelGroups->setRowCount(0);

	QModelIndex tindex;

	m_pTreeModelGroups->insertRow(0);
	QModelIndex index=m_pTreeModelGroups->index(0, 0);
	m_pTreeModelGroups->setData(index, tr("Users and groups"), Qt::DisplayRole);
	m_pTreeModelGroups->setData(index, QIcon(":/images/groupmgr.png"), Qt::DecorationRole);
	m_pTreeModelGroups->setData(index, TreeViewGroupsItem, TreeViewItemRole);

	if (groupName.isEmpty())
	{
		tindex=index;
	}

	QMapIterator<QString, PrivilegeGroup> it(groups.groups);
	int nNumGroup=-1;
	while (it.hasNext())
	{
		it.next();

		const PrivilegeGroup& group=it.value();

		m_pTreeModelGroups->insertRow(++nNumGroup, index);
		QModelIndex cindex=m_pTreeModelGroups->index(nNumGroup, 0, index);

		m_pTreeModelGroups->setData(cindex, group.name, Qt::DisplayRole);
		m_pTreeModelGroups->setData(cindex, QIcon(":/images/group.png"), Qt::DecorationRole);
		m_pTreeModelGroups->setData(cindex, TreeViewGroupItem, TreeViewItemRole);
		m_pTreeModelGroups->setData(cindex, group.datetime, DateTimeRole);

		if (groupName == group.name && userName.isEmpty())
		{
			tindex=cindex;
		}

		QMapIterator<QString, PrivilegeUser> iter(group.usrs);
		int nNumUser=-1;
		while (iter.hasNext())
		{
			iter.next();

			const PrivilegeUser& usr=iter.value();

			m_pTreeModelGroups->insertRow(++nNumUser, cindex);
			QModelIndex ccindex=m_pTreeModelGroups->index(nNumUser, 0, cindex);

			if (groupName == group.name && userName == usr.name)
			{
				tindex=ccindex;
			}

			m_pTreeModelGroups->setData(ccindex, usr.name, Qt::DisplayRole);
			m_pTreeModelGroups->setData(ccindex, QIcon(":/images/user.png"), Qt::DecorationRole);
			m_pTreeModelGroups->setData(ccindex, TreeViewUserItem, TreeViewItemRole);
			m_pTreeModelGroups->setData(ccindex, qVariantFromValue(usr), PrivilegeUserRole);
		}
	}

	m_pTreeViewGroups->expand(tindex);
	if (tindex.isValid())
	{
		m_pTreeViewGroups->expand(tindex.parent());
	}

	m_pTreeViewGroups->setCurrentIndex(tindex);
	m_pTreeViewGroups->scrollTo(tindex);
	clickedGroupsItem(tindex);

	setMenuState();
}

void QMainWndPrivilege::createUserSuccess(const QString groupName, const QString userName, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("Create the user <%1> in the group <%2> successfully.").arg(userName).arg(groupName));

	setGroups(groupName, userName, groups);
}
void QMainWndPrivilege::createExistedUser(const QString groupName, const QString userName, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Error, 
		tr("Failed to create the user <%1>! The user already exists in the group <%2>!").arg(userName).arg(groupName));

	setGroups(groupName, userName, groups);
}

void QMainWndPrivilege::createUserGroupNonexistent(const QString groupName, const QString userName, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Error, 
		tr("Failed to create the user <%1>! The user group <%2> does not exist!").arg(userName).arg(groupName));

	setGroups("", "", groups);
}

void QMainWndPrivilege::deleteUserSuccess(const QString userName, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Important, tr("Delete the user <%1> successfully.").arg(userName));

	setGroups("", "", groups);
}

void QMainWndPrivilege::recvPrivileges(const PrivilegeGroups groups)
{
	setGroups("", "", groups);
}

void QMainWndPrivilege::modifyPrivilegeSuccess(const QString groupName, const QString usrName, const PrivilegeGroups groups)
{
	setGroups(groupName, usrName, groups);
}

void QMainWndPrivilege::modifyPrivilegeUserNonexistent(const QString usrName, const PrivilegeGroups groups)
{
	QInfoManager::instance()->append(QInfoManager::Important, 
		tr("Failed to modify the user <%1> privileges! The user does not exist!").arg(usrName));

	setGroups("", "", groups);
}

void QMainWndPrivilege::clickedGroupsItem(const QModelIndex& index)
{
	Q_ASSERT_NO_NULL(m_pTreeModelGroups);
	Q_ASSERT_NO_NULL(m_pTreeViewClassified);

	int nType=m_pTreeModelGroups->data(index, TreeViewItemRole).toInt();

	if (nType != TreeViewUserItem)
	{
		m_pTreeViewClassified->setVisible(true);
		m_pScrollAreaPrivileges->setVisible(false);
	}
	else
	{
		m_pTreeViewClassified->setVisible(false);
		m_pScrollAreaPrivileges->setVisible(true);
	}

	m_pActions[ActionSave]->setEnabled(false);

	switch (nType)
	{
	case TreeViewGroupsItem:
		{
			QStringList titles;
			titles<<tr("Group Name")<<tr("Date created");
			QStandardItemModel* pModel=new QStandardItemModel(0, titles.size());
			pModel->setHorizontalHeaderLabels(titles);

			m_pTreeViewClassified->setModel(pModel);
			SAFE_DELETE_CLASS(m_pModelClassified);

			m_pModelClassified=pModel;
			
			int nRowCount=m_pTreeModelGroups->rowCount(index);
			for (int row=0; row<nRowCount; row++)
			{
				QModelIndex cindex=m_pTreeModelGroups->index(row, 0, index);
				QString strGroupName=m_pTreeModelGroups->data(cindex, Qt::DisplayRole).toString();
				QIcon icon=qvariant_cast<QIcon>(m_pTreeModelGroups->data(cindex, Qt::DecorationRole));
				QString strDateTime=m_pTreeModelGroups->data(cindex, DateTimeRole).toString();

				m_pModelClassified->insertRow(row);
				m_pModelClassified->itemFromIndex(m_pModelClassified->index(row, 0))->setEditable(false);
				m_pModelClassified->setData(m_pModelClassified->index(row, 0), strGroupName);
				m_pModelClassified->setData(m_pModelClassified->index(row, 0), icon, Qt::DecorationRole);
				m_pModelClassified->setData(m_pModelClassified->index(row, 0), qVariantFromValue(cindex), PeerItemRole);

				m_pModelClassified->itemFromIndex(m_pModelClassified->index(row, 1))->setEditable(false);
				m_pModelClassified->setData(m_pModelClassified->index(row, 1), QDateTime::fromString(strDateTime, STANDARD_DATETIME_FORMAT));
			}

			m_pTreeViewClassified->setColumnWidth(0, 150);
			m_pTreeViewClassified->setColumnWidth(1, 200);
		}
		break;
	case TreeViewGroupItem:
		{
			QStringList titles;
			titles<<tr("User Name")<<tr("Password")<<tr("Role")<<tr("Date created");
			QStandardItemModel* pModel=new QStandardItemModel(0, titles.size());
			pModel->setHorizontalHeaderLabels(titles);

			m_pTreeViewClassified->setModel(pModel);
			SAFE_DELETE_CLASS(m_pModelClassified);

			m_pModelClassified=pModel;

			int nRowCount=m_pTreeModelGroups->rowCount(index);
			for (int row=0; row<nRowCount; row++)
			{
				QModelIndex cindex=m_pTreeModelGroups->index(row, 0, index);
				QIcon icon=qvariant_cast<QIcon>(m_pTreeModelGroups->data(cindex, Qt::DecorationRole));
				PrivilegeUser usr=qvariant_cast<PrivilegeUser>(m_pTreeModelGroups->data(cindex, PrivilegeUserRole));

				m_pModelClassified->insertRow(row);
				m_pModelClassified->itemFromIndex(m_pModelClassified->index(row, 0))->setEditable(false);
				m_pModelClassified->setData(m_pModelClassified->index(row, 0), usr.name);
				m_pModelClassified->setData(m_pModelClassified->index(row, 0), icon, Qt::DecorationRole);
				m_pModelClassified->setData(m_pModelClassified->index(row, 0), qVariantFromValue(cindex), PeerItemRole);

				m_pModelClassified->itemFromIndex(m_pModelClassified->index(row, 1))->setEditable(false);
				m_pModelClassified->setData(m_pModelClassified->index(row, 1), usr.passwd);

				m_pModelClassified->itemFromIndex(m_pModelClassified->index(row, 2))->setEditable(false);

				if (usr.privileges.contains(PrivilegeRole) && (usr.privileges[PrivilegeRole]&AdministratorRole) != 0)
				{
					m_pModelClassified->setData(m_pModelClassified->index(row, 2), tr("Administrator"));
				}
				else
				{
					m_pModelClassified->setData(m_pModelClassified->index(row, 2), tr("General User"));
				}

				m_pModelClassified->itemFromIndex(m_pModelClassified->index(row, 3))->setEditable(false);
				m_pModelClassified->setData(m_pModelClassified->index(row, 3), QDateTime::fromString(usr.datetime, STANDARD_DATETIME_FORMAT));
			}

			m_pTreeViewClassified->setColumnWidth(0, 150);
			m_pTreeViewClassified->setColumnWidth(1, 200);
			m_pTreeViewClassified->setColumnWidth(2, 150);
			m_pTreeViewClassified->setColumnWidth(3, 200);
		}
		break;
	case TreeViewUserItem:
		{
			Q_ASSERT_NO_NULL(m_pFramePrivileges);

			PrivilegeUser usr=qvariant_cast<PrivilegeUser>(m_pTreeModelGroups->data(index, PrivilegeUserRole));
			m_pFramePrivileges->setPrivileges(usr);
		}
		break;
	default:
		break;
	}

	m_pTreeViewGroups->expand(index);

	setMenuState();
}

void QMainWndPrivilege::doubleClickedClassifiedItem(const QModelIndex& index)
{
	Q_ASSERT_NO_NULL(m_pModelClassified);
	Q_ASSERT_NO_NULL(m_pTreeViewGroups);

	QModelIndex tindex=qvariant_cast<QModelIndex>(m_pModelClassified->data(m_pModelClassified->index(index.row(), 0, index.parent()), PeerItemRole));

	m_pTreeViewGroups->expand(tindex);
	m_pTreeViewGroups->expand(tindex.parent());
	m_pTreeViewGroups->setCurrentIndex(tindex);
	m_pTreeViewGroups->scrollTo(tindex);

	clickedGroupsItem(tindex);
}

void QMainWndPrivilege::actionTriggered(int index)
{
	switch (index)
	{
	case ActionAddGroup:
		onClickedBtnCreateGroup();
		break;
	case ActionDeleteGroup:
		onClickedBtnDeleteGroup();
		break;
	case ActionAddUser:
		onClickedBtnCreateUser();
		break;
	case ActionDeleteUser:
		onClickedBtnDeleteUser();
		break;
	case ActionSave:
		onClickedBtnSaveUserPrivileges();
		break;
	case ActionRefresh:
		m_pReceiver->queryPrivileges();
		break;
	default:
		break;
	}
}

void QMainWndPrivilege::privilegesChanged(bool changed)
{
	m_pActions[ActionSave]->setEnabled(changed);
}

void QMainWndPrivilege::onClickedBtnSaveUserPrivileges()
{
	Q_UNUSED(m_pReceiver);
	PrivilegeUser user=m_pFramePrivileges->privileges();
	if (user.name.isEmpty() || user.passwd.isEmpty())
	{
		return;
	}

	m_pReceiver->queryModifyUserPrivileges(user);
}

void QMainWndPrivilege::onClickedBtnCreateUser()
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	QModelIndex index=m_pTreeViewGroups->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	int nType=m_pTreeModelGroups->data(index, TreeViewItemRole).toInt();
	if (nType == TreeViewGroupItem)
	{
		QString strGroupName=m_pTreeModelGroups->data(index, Qt::DisplayRole).toString();
		
		QDlgCreateUser dialog(findPixmap(":/images/user_icon.png"), this);
		dialog.setModal(true);
		if (dialog.exec() != 0)
		{
			PrivilegeUser user=dialog.user();

			m_pReceiver->queryCreateUser(strGroupName, user);
		}
	}
}

void QMainWndPrivilege::onClickedBtnDeleteUser()
{
	QModelIndex index=m_pTreeViewGroups->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	int nType=m_pTreeModelGroups->data(index, TreeViewItemRole).toInt();
	if (nType == TreeViewUserItem)
	{
		QString strUserName=m_pTreeModelGroups->data(index, Qt::DisplayRole).toString();
		if (!strUserName.isEmpty() && strUserName != HMAN_PRIVALEGE_DEFAULT_USER_NAME)
		{
			QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
				tr("Are you sure to delete the user <%1>?").arg(strUserName), QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

			if (button == QMessageBox::Ok)
			{
				m_pReceiver->queryDeleteUser(strUserName);
			}
		}
	}
}

void QMainWndPrivilege::onClickedBtnDeleteGroup()
{
	QModelIndex index=m_pTreeViewGroups->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	int nType=m_pTreeModelGroups->data(index, TreeViewItemRole).toInt();
	if (nType == TreeViewGroupItem)
	{
		QString strGroupName=m_pTreeModelGroups->data(index, Qt::DisplayRole).toString();
		if (!strGroupName.isEmpty() && strGroupName != HMAN_PRIVALEGE_DEFAULT_GROUP_NAME)
		{
			QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
				tr("Are you sure to delete the user group <%1> and all the users of the group?").arg(strGroupName), 
				QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

			if (button == QMessageBox::Ok)
			{
				m_pReceiver->queryDeleteGroup(strGroupName);
			}
		}
	}
}

void QMainWndPrivilege::onClickedBtnCreateGroup()
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	QDlgCreateGroup dialog(findPixmap(":/images/user_icon.png"), this);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		QString name=dialog.group();

		m_pReceiver->queryCreateGroup(name);
	}
}

void QMainWndPrivilege::setMenuState()
{
	Q_ASSERT_NO_NULL(m_pTreeViewGroups);

	QModelIndex index=m_pTreeViewGroups->currentIndex();
	if (!index.isValid())
	{
		for (int i=0; i<ActionEnd; i++)
		{
			if (m_pActions[i] != NULL)
			{
				m_pActions[i]->setEnabled(false);
			}
		}

		return;
	}

	m_pActions[ActionAddGroup]->setEnabled(true);

	int nType=m_pTreeModelGroups->data(index, TreeViewItemRole).toInt();
	if (nType == TreeViewGroupItem && m_pTreeModelGroups->data(index, Qt::DisplayRole).toString() != HMAN_PRIVALEGE_DEFAULT_GROUP_NAME)
	{
		m_pActions[ActionDeleteGroup]->setEnabled(true);
	}
	else
	{
		m_pActions[ActionDeleteGroup]->setEnabled(false);
	}

	if (nType == TreeViewGroupItem)
	{
		m_pActions[ActionAddUser]->setEnabled(true);
	}
	else
	{
		m_pActions[ActionAddUser]->setEnabled(false);
	}

	if (nType == TreeViewUserItem && m_pTreeModelGroups->data(index, Qt::DisplayRole).toString() != HMAN_PRIVALEGE_DEFAULT_USER_NAME)
	{
		m_pActions[ActionDeleteUser]->setEnabled(true);
	}
	else
	{
		m_pActions[ActionDeleteUser]->setEnabled(false);
	}

	m_pActions[ActionRefresh]->setEnabled(true);
}
