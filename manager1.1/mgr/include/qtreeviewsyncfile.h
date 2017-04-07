#ifndef QTREEVIEWSYNCFILE_H
#define QTREEVIEWSYNCFILE_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QPoint>
#include <QRunnable>

#include "qutildef.h"
#include "qsyncfiledelegate.h"
#include "qfilecomparefactory.h"

class QTreeViewSyncFile : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewSyncFile(QWidget *parent=0);
	~QTreeViewSyncFile();

public:
	enum TitleLabel
	{
		NameTitle,
		SizeTitle,
		TypeTitle,
		ModifiedTitle,
		StatusTitle,
		ProcessTitle
	};

	enum DiffFileModifiedStatus
	{
		Unmodified,		//未修改的
		Modified,		//已修改的
		Added,			//添加的
		Deleted,		//删除的
		NonVersioned,	//已存在，但被忽略的
		ModifiedStatusEnd
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

	enum DiffFileUserData
	{
		UserChecked=Qt::UserRole,
		UserCheckable,
		UserTransferStatus,
		UserData
	};

public:
	void setDiffFile(const QList<DiffFile>& files);
	void setItemTransferStatus(int row, int status);
	int itemTransferStatus(int row) const;

	void addTransferredSize(int row, quint64 size);

	int rowCount() const;

	void setItemsCheckable(bool checkable);
	bool isItemChecked(int row) const;
	void setItemCheckStatus(int row, bool check);

	QModelIndexList selectedRows();

	int fetchTask(DiffFile& item);	//返回行号,-1表示无任务
	DiffFile fetchItem(const QModelIndex& index);

public:
	void clear();

Q_SIGNALS:
	void refreshKeyPress();

	void refreshModelFinished();
	void mouseRightButtonPressed(QPoint pos);

private slots:
	void finished(QStandardItemModel* model);

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);

private:
	QStandardItemModel*	m_pModel;
	QSyncFileDelegate*	m_pDelegate;

	bool m_bCheckable;
};

class QTreeViewSyncFileRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QTreeViewSyncFileRunnable(const QList<DiffFile>& files, bool checkable,QObject* parent=0);
	virtual ~QTreeViewSyncFileRunnable();

public:
	virtual void run();

signals:
	void finished(QStandardItemModel* model);

private:
	QStandardItemModel*	m_pModel;
	QList<DiffFile> m_files;

	bool m_bCheckable;
};

#endif // QTREEVIEWSYNCFILE_H
