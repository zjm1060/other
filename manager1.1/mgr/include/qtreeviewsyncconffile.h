#ifndef QTREEVIEWSYNCCONFFILE_H
#define QTREEVIEWSYNCCONFFILE_H

#include <QSortFilterProxyModel>

#include "qtreeviewsynccommit.h"

class QSortFilterSyncConfFileModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	QSortFilterSyncConfFileModel(QObject* parent=0);
	~QSortFilterSyncConfFileModel();

protected:
	virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};

class QTreeViewSyncConfFile : public QTreeViewSyncCommit
{
	Q_OBJECT

public:
	QTreeViewSyncConfFile(QWidget* parent=0);
	~QTreeViewSyncConfFile();

public:
	virtual QString record() const
	{
		return "config";
	}

	virtual QString commonFolder() const
	{
		return H9000_DEF_PATH;
	}

public:
	virtual void setItemCheckState(int row, int state, const QModelIndex& parent=QModelIndex());

protected:
	virtual void checkDiff();

	virtual void setCustomItemDelegate();
	virtual void setTreeModel(QTreeModel* model);

	virtual QModelIndex mapToSourceItem(const QModelIndex& index);

private:
	QSortFilterSyncConfFileModel*	m_pSortModel;
};

class QSyncConfFileRunnable : public QTreeViewSyncCommitRunnable
{
	Q_OBJECT

public:
	QSyncConfFileRunnable(const QString& workspace, const QString& history, const QString& record, bool checkable, bool enableRestrict, QObject* parent=0);
	virtual ~QSyncConfFileRunnable();

protected:
	virtual void check();
};

#endif // QTREEVIEWSYNCCONFFILE_H
