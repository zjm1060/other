#ifndef QABSTRACTFILETRANSFER_H
#define QABSTRACTFILETRANSFER_H

#include <QFrame>
#include <QStandardItemModel>
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

#include "ui_transferfile.h"
#include "qtipsdelegate.h"
#include "qtreeviewtransferqueue.h"
#include "qframefilesystem.h"
#include "qimessage.h"

class QAbstractFileTransfer : public QFrame, public Ui::FrameTransferFile
{
	Q_OBJECT

public:
	QAbstractFileTransfer(QWidget *parent=0);
	~QAbstractFileTransfer();

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

public slots:
	void appendTip(int flag, const QString tip);

protected:
	virtual void transfer(int row, const QReceiverTransferFile::TransferQueueItem& task)=0;
	virtual void cancel()=0;

	virtual void gotoRemoteHome()=0;
	virtual void gotoRemoteLocation(const QString& location)=0;

	virtual void queryRename(const QString& location, const QString& old, const QString& now)=0;
	virtual void queryNewDirectory(const QString& location)=0;
	virtual void queryDelete(const QString& location, const QVector<QString>& files)=0;

	virtual void finished();

protected:
	void editItem(int machine, const QModelIndex index);

private slots:
	void localEditTextChangedLocation(const QString& location);
	void enterPressedLocal(QString location);
	void clickedToolButtonLocal(int type);
	void localFocusStatusChanged(bool has);

	void remoteEditTextChangedLocation(const QString& location);
	void enterPressedRemote(QString location);
	void clickedToolButtonRemote(int type);
	void remoteFocusStatusChanged(bool has);

	void localLocationCurrentIndexChanged(const QString& location);
	void doubleClickedLocalLocation(const QModelIndex& index);
	void backspacePressLocal();
	void refreshPressLocal();
	void mouseRightButtonPressLocal(QPoint pos);
	void localDataChanged(QModelIndex index, QString old, QString now);
	void localNewDirectory();
	void localDelDirectory();

	void remoteLocationCurrentIndexChanged(const QString& location);
	void doubleClickedRemoteLocation(const QModelIndex& index);
	void backspacePressRemote();
	void refreshPressRemote();
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
	int		m_nRefreshFlag;		//刷新标志,传输完成后刷新列表

protected:
	QActionGroup*	m_pGroupFormat;

private:
	enum ActionType
	{
		ActionDownload=0,
		ActionUpload,
		ActionOpen,
		ActionEdit,
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

#endif // QABSTRACTFILETRANSFER_H
