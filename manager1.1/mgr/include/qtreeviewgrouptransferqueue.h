#ifndef QTREEVIEWGROUPTRANSFERQUEUE_H
#define QTREEVIEWGROUPTRANSFERQUEUE_H

#include <QTreeView>
#include <QIcon>

#include "qservermanager.h"
#include "qframeremotefilesystem.h"
#include "qtreemodel.h"
#include "qgrouptransferqueue.h"

class QTreeViewGroupTransferQueue : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewGroupTransferQueue(QWidget *parent);
	~QTreeViewGroupTransferQueue();

public:
	enum TitleLabel
	{
		NameTitle,
		TypeTitle,
		StatusTitle,
		ProcessTitle,
		TargetTitle
	};

	enum TransferStatus
	{
		InvalidStatus=-1,
		Waiting=0,
		Transfering,
		BackingUp,
		Failed,
		Skip,
		Finished,
		Connecting,
		Connected,
		Unconneced,
		StatusEnd
	};

	enum TransferRoleType
	{
		ServerRole,
		FileRole,
		BackUpRole,		//备份
		CommandRole		//执行命令
	};

	enum UserDataRole
	{
		TransferRole=Qt::UserRole,
		TransferDataRole,
		TransferStatusRole,
		TransferProcessRole
	};

	typedef struct _transferQueuePeerItem
	{
		bool isSlient;		//true: 发送端不接收客户端回应

		ServerInfo source;	//发送端
		ServerInfo target;	//接收端
	}TransferQueuePeerItem;

	typedef struct _transferQueueFileItem
	{
		bool isTop;
		int format;

		QString source;

		TransferFileItem	file;			//用于传输单个文件

		QString target;
	}TransferQueueFileItem;

	typedef struct _transferQueueBackupItem 
	{
		QString source;
		QString target;

		QVector<FileInfo>	files;	//用于备份
	}TransferQueueBackupItem;

	typedef struct _transferQueueCommandItem 
	{
		QString name;	//命令名称
		QString workspace;

		QVector<FileInfo>	files;		//提交的文件
	}TransferQueueCommandItem;

public:
	void clear();

	QModelIndex appendPeerItem(const TransferQueuePeerItem& peer);
	QModelIndex appendBackupItem(const TransferQueueBackupItem& item);

	void appendItem(const QTreeViewGroupTransferQueue::TransferQueueFileItem& item, int status, const QModelIndex& parent);
	void insertItem(int row, const QTreeViewGroupTransferQueue::TransferQueueFileItem& item, int status, const QModelIndex& parent);
	void removeItem(int row, const QModelIndex& parent);

	void appendCommandItem(const QTreeViewGroupTransferQueue::TransferQueueCommandItem& item, const QModelIndex& parent);
	void insertCommandItem(int row, const QTreeViewGroupTransferQueue::TransferQueueCommandItem& item, const QModelIndex& parent);

	void setItemTransferStatus(int row, QTreeViewGroupTransferQueue::TransferStatus status, const QModelIndex& parent=QModelIndex());
	int itemTransferStatus(int row, const QModelIndex& parent=QModelIndex());
	void addItemProcess(int row, quint64 size, const QModelIndex& parent);
	void setItemProcess(int row, quint64 size, const QModelIndex& parent);

	QModelIndex fetchTask(int& role, QVariant& var, int& status);	//无效表示无任务

	QModelIndexList selectedRows();
	int taskCount(const QModelIndex& parent=QModelIndex());

	QModelIndex index(int row, const QModelIndex& parent=QModelIndex());

public:
	void showAbsolutePath(bool show);

Q_SIGNALS:
	void synchronize();
	void mouseRightButtonPressed(QPoint pos);

protected:
	virtual void mousePressEvent(QMouseEvent* event);

private:
	QTreeModel*		m_pModel;

	bool	m_bShowAbsolutePath;
};

Q_DECLARE_METATYPE(QTreeViewGroupTransferQueue::TransferQueuePeerItem)
Q_DECLARE_METATYPE(QTreeViewGroupTransferQueue::TransferQueueFileItem)
Q_DECLARE_METATYPE(QTreeViewGroupTransferQueue::TransferQueueBackupItem)
Q_DECLARE_METATYPE(QTreeViewGroupTransferQueue::TransferQueueCommandItem)

#endif // QTREEVIEWGROUPTRANSFERQUEUE_H
