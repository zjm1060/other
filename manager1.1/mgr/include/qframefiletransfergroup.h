#ifndef QFRAMEFILETRANSFERGROUP_H
#define QFRAMEFILETRANSFERGROUP_H

#include <QFrame>
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

#include "ui_transferfilegroup.h"
#include "qtreeviewgrouptransferqueue.h"
#include "qframeremotefilesystem.h"
#include "qtransferinstructor.h"
#include "qlistviewcontrolledserver.h"
#include "qlistviewfilereceiver.h"
#include "qlistviewprompt.h"
#include "qlinkbutton.h"

class QFrameFileTransferGroup : public QFrame, public Ui::FrameFileTransferGroup
{
	Q_OBJECT

public:
	QFrameFileTransferGroup(const QString& record, const QString& subPath, QWidget* parent=0);
	~QFrameFileTransferGroup();

public:
	void setTitleText(const QString& title);
	void setDefaultDescText(const QString& desc);

	void setThemePixmap(const QPixmap& pixmap);
	void showTipsPixmap(bool show);

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);
	void setAddedServerList(const QList<Server>& servers);

signals:
	void addServer(const Server& server);
	void removeServers(const QList<Server>& servers);

protected:
	virtual void finished();

private slots:
	void enterInvalidChar(bool invalid, const QChar& ch);
	void focusStatusChanged(bool has);

	void onClickedButtonAdd();
	void onClickedButtonRemove();

	void selectChanged(bool removable);

	void mouseRightButtonPress(QPoint pos);

	void currentChanged(const QModelIndex& index);

	void actionTriggered(int index);

	void synchronize();
	void recvMessage(QTransferInstructor::MessageType type, const QString& message);
	void addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files);
	void transferedFileContents(const QGroupTransferQueue::TransferQueueItem& task, quint64 size);

	void mouseRightButtonPressTransferQueue(QPoint pos);

private:
	void transfer(QModelIndex index, const QGroupTransferQueue::TransferQueueItem& task);
	void fetchTask();

private:
	QListViewControlledServer*	m_pListViewServers;

	QLinkButton*		m_pLinkButtonRemove;
	QLinkButton*		m_pLinkButtonAdd;

	QListViewFileReceiver*	m_pListViewReveiver;	//接收端列表

	QFrameRemoteFileSystem*		m_pFrameFileSystem;

	QTreeViewGroupTransferQueue*	m_pTreeViewTransferQueue;
	QListViewPrompt*				m_pListViewPrompt;

	QModelIndex	m_index;		//当前同步文件位于同步树列表索引
	QGroupTransferQueue::TransferQueueItem m_task;

	QTransferInstructor*	m_pTransferInstructor;

	QString		m_strDefaultDesc;		//默认提示信息描述

private:
	bool	m_bSync;			//是否正在同步
	int		m_nRow;				//当前同步行

private:
	QActionGroup*	m_pGroupFormat;

private:
	enum ActionType
	{
		ActionUpload,
		ActionOpen,
		ActionDelete,
		ActionRefresh,
		ActionNewDirectory,
		ActionRename,
		ActionRetransmit,
		ActionDeleteTask,
		ActionDeleteAllTask,
		ActionEnd
	};

	QMenu*		m_pMenu;
	QMenu*		m_pMenuFormat;
	QMenu*		m_pMenuTask;
	QAction*	m_pActions[ActionEnd];

	QAction*	m_pActionsFormat[FormatEnd];

	QSignalMapper*	m_pSignalMapper;
};

#endif // QFRAMEFILETRANSFERGROUP_H
