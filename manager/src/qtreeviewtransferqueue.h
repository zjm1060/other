#ifndef QTREEVIEWTRANSFERQUEUE_H
#define QTREEVIEWTRANSFERQUEUE_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QIcon>
#include <QFileIconProvider>

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

	typedef struct _transferQueueItem
	{
		int direction;

		QString source;
		QString name;
		bool isTop;

		QString target;

		int format;
		quint64 size;

		int type;
		QString typeDesc;
	}TransferQueueItem;

public:
	void clear();

	void appendItem(const TransferQueueItem& item);
	void removeItem(int row);
	void setItemStatus(int row, QTreeViewTransferQueue::TransferStatus status);
	int itemStatus(int row);

	int fetchTask(TransferQueueItem& item);	//返回行号,-1表示无任务

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
	QStandardItemModel*		m_pModel;

	QIcon	m_icon[DirectionEnd];
	QIcon	m_statusIcon[StatusEnd];

	QFileIconProvider	m_provider;
};

#endif // QTREEVIEWTRANSFERQUEUE_H
