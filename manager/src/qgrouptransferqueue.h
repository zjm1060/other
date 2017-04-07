#ifndef QGROUPTRANSFERQUEUE_H
#define QGROUPTRANSFERQUEUE_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QIcon>

#include "qservermanager.h"
#include "qframeremotefilesystem.h"

class QGroupTransferQueue : public QTreeView
{
	Q_OBJECT

public:
	QGroupTransferQueue(QWidget *parent);
	~QGroupTransferQueue();

public:
	enum TransferStatus
	{
		InvalidStatus=-1,
		Waiting=0,
		Transfering,
		Failed,
		Finished,
		StatusEnd
	};

	typedef struct _transferQueueItem
	{
		QServerManager::ServerInfo sourceHost;
		bool isTop;
		int format;

		QString source;

		QFrameRemoteFileSystem::TransferFileItem	file;

		QServerManager::ServerInfo	targetHost;
		QString target;
	}TransferQueueItem;

public:
	void clear();

	void appendItem(const QGroupTransferQueue::TransferQueueItem& item);
	void insertItem(int index, const QGroupTransferQueue::TransferQueueItem& item);
	void removeItem(int row);
	void setItemStatus(int row, QGroupTransferQueue::TransferStatus status);
	int itemStatus(int row);

	int fetchTask(QGroupTransferQueue::TransferQueueItem& item);	//返回行号,-1表示无任务

	QModelIndexList selectedRows();
	int taskCount();

	void startSynchronize();

public:
	static QString appendPathName(const QString path, const QString name, bool isTop);

Q_SIGNALS:
	void synchronize();
	void mouseRightButtonPress(QPoint pos);

protected:
	virtual void mousePressEvent(QMouseEvent* event);

private:
	enum UserDataRole
	{
		ItemDataRole=Qt::UserRole,
		StatusRole,
		RoleEnd
	};

private:
	QStandardItemModel*		m_pModel;

	QIcon	m_statusIcon[StatusEnd];
};

Q_DECLARE_METATYPE(QGroupTransferQueue::TransferQueueItem)

#endif // QGROUPTRANSFERQUEUE_H
