#ifndef QABSTRACTSYNC_H
#define QABSTRACTSYNC_H

#include <QFrame>
#include <QStandardItemModel>
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

#include "ui_syncfile.h"
#include "qtipsdelegate.h"
#include "qtreeviewtransferqueue.h"
#include "qframefilesystem.h"

class QAbstractSync : public QFrame, public Ui::FrameSyncFile
{
	Q_OBJECT

public:
	QAbstractSync(QWidget *parent=0);
	~QAbstractSync();

public:
	void setTitleText(const QString& title);
	void setDefaultDescText(const QString& desc);

	void setThemePixmap(const QPixmap& pixmap);
	void showTipsPixmap(bool show);

	void setDefaultLocalLocation(const QString& location);

public:
	void refreshLocalDrives();
	void refreshLocalFileSystem();

	void setLocation(int machine, const QString& location);

public:
	void refresh(int machine);

public:
	void appendTip(int flag, const QString& tip);

protected:
	virtual void transfer(int row, const QTreeViewTransferQueue::TransferQueueItem& task)=0;
	virtual void cancel()=0;

	virtual void gotoRemoteHome()=0;
	virtual void gotoRemoteLocation(const QString& location)=0;

	virtual void queryRename(const QString& location, int row, const QString& old, const QString& now)=0;
	virtual void queryNewDirectory(const QString& location)=0;
	virtual void queryDelete(const QString& location, const QStringList& files)=0;

	virtual void finished();

private slots:
	void localEditTextChangedLocation(const QString& location);
	void enterPressedLocal(QString location);
	void clickedToolButtonLocal(int type);
	void localFocusStatusChanged(bool has);

	void remoteEditTextChangedLocation(const QString& location);
	void enterPressedRemote(QString location);
	void clickedToolButtonRemote(int type);
	void remoteFocusStatusChanged(bool has);

	void localLocationCurrentIndexChanged(QString location);
	void doubleClickedLocalLocation(const QModelIndex& index);
	void mouseRightButtonPressLocal(QPoint pos);
	void localDataChanged(QModelIndex index, QString old, QString now);
	void localNewDirectory();
	void localDelDirectory();

	void remoteLocationCurrentIndexChanged(QString location);
	void doubleClickedRemoteLocation(const QModelIndex& index);
	void mouseRightButtonPressRemote(QPoint pos);
	void remoteDataChanged(QModelIndex index, QString old, QString now);
	void remoteNewDirectory();
	void remoteDelDirectory();

	void actionTriggered(int index);

	void synchronize();

	void mouseRightButtonPressTransferQueue(QPoint pos);

protected:
	enum MachineType
	{
		LocalMachine=0,
		RemoteMachine,
		MachineEnd
	};

protected:
	QFrameFileSystem*		m_pFrameFileSystem[MachineEnd];

	QTreeViewTransferQueue*	m_pViewTransferQueue;

	QStandardItemModel*		m_pModelTips;
	QTipsDelegate*			m_pDelegate;

	QString		m_strDefaultDesc;		//默认提示信息描述
	QString		m_strLocalHome;			//本地主目录
 	QString		m_strRemoteHome;		//远程主目录

protected:
	bool	m_bSync;			//是否正在同步

protected:
	enum TransferFormat
	{
		TextFormat=0,
		BinaryFormat,
		AutoFormat,
		FormatEnd
	};
	QActionGroup*	m_pGroupFormat;

private:
	enum ActionType
	{
		ActionDownload=0,
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

private:
	enum PopupMenuOwner
	{
		InvalidOwner=-1,
		LocalFileSystemOwner=0,
		RemoteFileSystemOwner,
		TransferQueueOwner,
		OwnerEnd
	};

	int m_nPopupMenuOwner;		//当前触发菜单操作的窗口
};

#endif // QABSTRACTSYNC_H
