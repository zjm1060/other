#ifndef QFRAMESYNCGROUPCOMMITFILE_H
#define QFRAMESYNCGROUPCOMMITFILE_H

#include <QFile>
#include <QTimer>
#include <QList>
#include <QRunnable>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

#include "ui_syncgroupcommitfile.h"
#include "qabstractreceiver.h"
#include "qtreeviewsynccommit.h"
#include "qlistviewsyncfileserver.h"
#include "qlistviewsyncfilerequestserver.h"
#include "qtransferinstructor.h"
#include "qtreeviewgrouptransferqueue.h"
#include "qlinkbutton.h"

class QFrameSyncGroupCommitFile : public QFrame, public Ui::FrameSyncGroupCommitFile
{
	Q_OBJECT

public:
	QFrameSyncGroupCommitFile(const QString& treeview, bool enableRestrict, QWidget *parent=0);
	~QFrameSyncGroupCommitFile();

public:
	void setTitleText(const QString& title);
	void setDefaultDescText(const QString& desc);

	void setThemePixmap(const QPixmap& pixmap);
	void showTipsPixmap(bool show);

public:
	void refreshFileSystem();
	void refreshServerList(const QList<QPair<ServerInfo, int> >& servers);

	void setRestrictFilterEnabled(bool enable);

public slots:
	void appendTip(int flag, const QString& tip);

protected:
	void transferFile(const QModelIndex& index, int role, const QVariant& item);

	virtual void cancel();
	virtual void finished();

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event);

private slots:
	void readCommonFolder();
	void currentTextChanged();
	void refreshModelFinished();

	void actionTriggered(int index);

	void onClickedBtnOk();
	void onClickedBtnCancel();
	void onClickedBtnSelectAll();
	void onClickedBtnSelectNone();
	void onClickedBtnRefresh();
	void onClickedBtnWorkspace();
	void onClickedBtnShowLog();

	void doubleClicked(const QModelIndex& index);

	void mouseRightButtonPressed(QPoint pos);

	void selectedChanged();

	void recvMessage(QTransferInstructor::MessageType type, const QString& message);
	void addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files);
	void transferedFileContents(const QGroupTransferQueue::TransferQueueItem& task, quint64 size);
	void backupProcess(int finished, int success);

	void closing();		//对比窗口关闭

private:
	void fetchTask();
	int findFileFormat(const QString& name, int format) const;

	void syncFinished();

private:
	void appendTransferTip(int flag, const QString& tip);

	void setItemsCheckState(int state);

private:
	void saveBackupRecord(const QString& filename, QVector<QPair<FileInfo, int> >& files);

	bool writeConfigure();

	void logplus(int flag, const QString& tip);
	
private:
	QString		m_strDefaultDesc;		//默认提示信息描述

	QPair<int, QString>	m_transferTip;	//传输消息

	QListViewSyncFileServer*	m_pListViewServers;		//同步接收方列表
	bool		m_bSync;		//是否可以同步文件

	QLinkButton*	m_pButtonWorkspace;	//同步发送方工作目录
	QMap<QString, QString>	m_configure;	//发送端配置信息

	QLinkButton*	m_pButtonRefresh;

	QTreeViewSyncCommit*	m_pTreeView;	//同步发送方文件列表

	QTreeViewGroupTransferQueue*	m_pTreeViewQueue;

	QString		m_strPath;		//本地上传目录
	QString		m_strSavePath;	//远端保存目录

	QModelIndex	m_index;		//当前同步文件位于同步树列表索引

	QGroupTransferQueue::TransferQueueItem m_task;

	QTransferInstructor*	m_pTransferInstructor;

private:
	QMap<QModelIndex, QObject*>		m_mapWnd;	//主窗口与当前

private:
	enum ActionType
	{
		ActionSync,
		ActionCancel,
		ActionDiff,
		ActionSelectAll,
		ActionSelectNone,
		ActionShowLog,
		ActionRefresh,
		ActionDeleteAll,
		ActionRetransmit,
		ActionEnd
	};

	QMenu*		m_pMenu;
	QMenu*		m_pMenuQueue;		//传输队列菜单
	QAction*	m_pActions[ActionEnd];

	QSignalMapper*	m_pSignalMapper;
};

class QWriteBackupToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteBackupToXmlFile(const QString& filename, const QString& path, const QVector<QPair<FileInfo, int> >& files);
	~QWriteBackupToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "folder";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	QString		m_strPath;
	 QVector<QPair<FileInfo, int> > m_files;
};

#endif // QFRAMESYNCGROUPCOMMITFILE_H
