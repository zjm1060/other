#ifndef QTREEVIEWTRANSFERQUEUE_H
#define QTREEVIEWTRANSFERQUEUE_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QIcon>
#include <QVariant>

#include "qreceivertransferfile.h"

class QTreeViewTransferQueue : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewTransferQueue(QWidget *parent);
	~QTreeViewTransferQueue();

public:
	enum TransferDirection
	{
		Download=0,
		Upload,
		DirectionEnd
	};

	enum TransferStatus
	{
		InvalidStatus=-1,
		Waiting=0,
		Transfering,
		Failed,
		Finished,
		StatusEnd
	};

public:
	void clear();

	void appendItem(const QReceiverTransferFile::TransferQueueItem& item);
	void insertItem(int row, const QReceiverTransferFile::TransferQueueItem& item);
	void removeItem(int row);
	void setItemFileName(int row, const QString& name);
	void setItemStatus(int row, QTreeViewTransferQueue::TransferStatus status);
	void addItemProcess(int row, quint64 size);
	void setItemFileSize(int row, quint64 size);
	QString itemFileName(int row);
	int itemStatus(int row);

	int fetchTask(QReceiverTransferFile::TransferQueueItem& item);	//返回行号,-1表示无任务

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

	QIcon	m_icon[DirectionEnd];
	QIcon	m_statusIcon[StatusEnd];
};

#endif // QTREEVIEWTRANSFERQUEUE_H
