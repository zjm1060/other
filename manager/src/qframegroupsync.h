#ifndef QFRAMEGROUPSYNC_H
#define QFRAMEGROUPSYNC_H

#include <QFrame>
#include <QStandardItemModel>
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

#include "ui_syncgroupfile.h"
#include "qtipsdelegate.h"
#include "qgrouptransferqueue.h"
#include "qframeremotefilesystem.h"
#include "qserverdelegate.h"
#include "qtransferinstructor.h"

class QFrameGroupSync : public QFrame, public Ui::FrameGroupSyncFile
{
	Q_OBJECT

public:
	QFrameGroupSync(int kind, const QString subPath, QWidget* parent=0);
	~QFrameGroupSync();

public:
	enum TransferFormat
	{
		TextFormat=0,
		BinaryFormat,
		AutoFormat,
		FormatEnd
	};

public:
	void setTitleText(const QString& title);
	void setDefaultDescText(const QString& desc);

	void setThemePixmap(const QPixmap& pixmap);
	void showTipsPixmap(bool show);

public:
	void appendTip(int flag, const QString& tip);

protected:
	virtual void finished();

private slots:
	void enterInvalidChar(bool invalid, const QChar& ch);
	void focusStatusChanged(bool has);

	void mouseRightButtonPress(QPoint pos);

	void clicked(const QModelIndex& index);
	void doubleClicked(const QModelIndex& index);

	void actionTriggered(int index);

	void synchronize();
	void recvMessage(QTransferInstructor::MessageType type, const QString& message);
	void addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files);

	void mouseRightButtonPressTransferQueue(QPoint pos);

private:
	void addServer(int target, const QServerManager::ServerInfo& serverInfo);

	void doubleClicked(int target, const QModelIndex& index);

	void transfer(int row, const QGroupTransferQueue::TransferQueueItem& task);
	void fetchTask();

private:
	enum enumListView
	{
		ListViewSource,
		ListViewTarget,
		ListViewEnd
	};

	QListView*	m_pListView[ListViewEnd];
	QStandardItemModel* m_pModel[ListViewEnd];

	QServerDelegate* m_pServerDelegate;

	QFrameRemoteFileSystem*		m_pFrameFileSystem;

	QGroupTransferQueue*	m_pViewTransferQueue;
	QTransferInstructor*	m_pTransferInstructor;

	QStandardItemModel*		m_pModelTips;
	QTipsDelegate*			m_pDelegate;

	QString		m_strDefaultDesc;		//默认提示信息描述

	int			m_nKind;
	QString		m_strSubPath;

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

	enum pixmapType
	{
		PixmapServer,
		PixmapMultiServer,
		PixmapServerQuit,
		PixmapSelect,
		PixmapEnd
	};

	QPixmap	m_pixmap[PixmapEnd];
};

#endif // QFRAMEGROUPSYNC_H
