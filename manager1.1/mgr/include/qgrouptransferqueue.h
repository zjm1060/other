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
		bool isSlient;

		ServerInfo sourceHost;
		bool isTop;
		int format;

		QString source;

		TransferFileItem	file;

		ServerInfo	targetHost;
		QString target;
	}TransferQueueItem;

public:
	void clear();

	void appendItem(const QGroupTransferQueue::TransferQueueItem& item);
	void insertItem(int row, const QGroupTransferQueue::TransferQueueItem& item);
	void removeItem(int row);
	void setItemStatus(int row, QGroupTransferQueue::TransferStatus status);
	int itemStatus(int row);
	void addItemProcess(int row, quint64 size);

	int fetchTask(QGroupTransferQueue::TransferQueueItem& item);	//返回行号,-1表示无任务

	QModelIndexList selectedRows();
	int taskCount();

	void startSynchronize();

Q_SIGNALS:
	void synchronize();
	void mouseRightButtonPress(QPoint pos);

protected:
	virtual void mousePressEvent(QMouseEvent* event);

private:
	QStandardItemModel*		m_pModel;

	QIcon	m_statusIcon[StatusEnd];
};

bool operator==(const QGroupTransferQueue::TransferQueueItem& source, const QGroupTransferQueue::TransferQueueItem& target);
bool operator!=(const QGroupTransferQueue::TransferQueueItem& source, const QGroupTransferQueue::TransferQueueItem& target);

Q_DECLARE_METATYPE(QGroupTransferQueue::TransferQueueItem)

#endif // QGROUPTRANSFERQUEUE_H
