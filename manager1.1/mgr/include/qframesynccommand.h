#ifndef QFRAMESYNCCOMMAND_H
#define QFRAMESYNCCOMMAND_H

#include <QFrame>
#include <QTreeView>
#include <QMouseEvent>

#include "ui_command.h"
#include "qlinkbutton.h"
#include "qnumberdelegate.h"
#include "qutilreadwritexmlfile.h"
#include "qtreeviewassociationfiles.h"
#include "qclientmanager.h"
#include "qabstractreceiver.h"
#include "qlistviewcontrolledserver.h"

class QFrameSyncCommand : public QFrame, public QAbstractReceiver
{
	Q_OBJECT

public:
	QFrameSyncCommand(QWidget *parent=0);
	~QFrameSyncCommand();

public:
	enum TitleLabel
	{
		NameTitle,
		RunningTitle,
		TerminalTitle,
		SizeTitle,
		TypeTitle,
		ModifiedTitle,
		PathTitle
	};

	enum ItemRoleType
	{
		ProgramRole,
		CommandRole
	};

	enum UserData
	{
		ItemRole=Qt::UserRole,		//角色类型
		FileRole,					//本地文件信息
		AssociateRole				//关联信息
	};

public:
	void refresh();

	void setClient(const ServerInfo& info);

	void addProgramItem(const SyncProgram& program);
	void addCommandItem(const SyncCommand& command);
	void addCommandItem(const QString& command, const QVector<SyncMonitorFile>& files=QVector<SyncMonitorFile>());

	QModelIndexList selectedRows() const;

	void setCommands(const SyncCommands& commands);
	SyncCommands commands() const;

	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

public:
	void queryCommandSetting();
	void queryWriteCommandSetting(const SyncCommands& commands);

public slots:
	void connected(QClient* client);

	void onClickedBtnSave();
	void onClickedBtnRefresh();
	void onClickedBtnAddCommand();
	void onClickedBtnDeleteCommand();
	void onClickedBtnMoveUp();
	void onClickedBtnMoveDown();
	void onClickedBtnAddFiles();
	void onClickedBtnDeleteFiles();

	void currentChanged(const QModelIndex& index);

	void dataChanged();		//数据改变，需要保存

	void onClickedItem(const QModelIndex& index);
	void itemChanged(QStandardItem* item);

	void associationFileChanged(QVector<SyncMonitorFile> files);

	void currentCommandItemChanged(const QModelIndex& current, const QModelIndex& previous);
	void fileItemsSelectedChanged(const QItemSelection& selected, const QItemSelection& deselected);

public:
	virtual void recvSyncAssociationConfigure(SyncCommands commands);
	virtual void recvWriteSyncAssociationConfigure(int flag);

signals:
	void unconnected();		//为建立连接
	void serverQuit();		//服务器退出

private:
	void addDefaultCommand();

	void interchangeRows(int firstRow, int secondRow);

	bool enquireSaveChanged();

	void setMenuState(bool loading);

private:
	Ui::FrameCommand*	m_ui;

	QLinkButton*	m_pButtonRefresh;

	QListViewControlledServer*	m_pListViewServers;

	QTreeView*				m_pTreeViewCommand;
	QStandardItemModel*		m_pModel;
	QNumberDelegate*		m_pNumberDelegate;

	QTreeViewAssociationFiles*	m_pTreeViewFiles;

private:
	QClient*			m_pClient;
	QClientManager*		m_pClientManager;

	ServerInfo	m_server;

	QString		m_strWorkspaceCommand;
	QString		m_strWorkspace;

	quint32		m_nUsrID;		//用户ID，用于查询目录信息
};

#endif // QFRAMESYNCCOMMAND_H
