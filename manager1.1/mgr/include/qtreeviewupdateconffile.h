#ifndef QTREEVIEWUPDATECONFFILE_H
#define QTREEVIEWUPDATECONFFILE_H

#include <QSortFilterProxyModel>

#include "qtreeviewupdatefile.h"

class QSortFilterUpdateFileModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	QSortFilterUpdateFileModel(QObject* parent=0);
	~QSortFilterUpdateFileModel();

protected:
	virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;

private:
	int code(int status) const;
};

class QTreeViewUpdateConfFile : public QTreeViewUpdateFile
{
	Q_OBJECT

public:
	QTreeViewUpdateConfFile(QWidget *parent=0);
	~QTreeViewUpdateConfFile();

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
	virtual void checkModified(const QString& workspace, const QString& hostname, const QString& address);

public:
	virtual QModelIndex mapToSourceItem(const QModelIndex& index);
	virtual QModelIndex mapFromSourceItem(const QModelIndex& index);

protected:
	virtual void check();

	virtual void setCustomItemDelegate();
	virtual void setTreeModel(QTreeModel* model);

private:
	QSortFilterUpdateFileModel*	m_pSortModel;
};

class QDelegateSortUpdateFile : public QUpdateFileDelegate
{
	Q_OBJECT

public:
	QDelegateSortUpdateFile(QObject* parent=0);
	~QDelegateSortUpdateFile();

public:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

class QUpdateConfFileRunnable : public QUpdateFileRunnable
{
	Q_OBJECT

public:
	QUpdateConfFileRunnable(const QString& workspace, const QString& catagory, 
		const QString& hostname, const QString& address, bool restrictFilter, bool localLastModified, bool immediate=false, QObject* parent=0);
	virtual ~QUpdateConfFileRunnable();

protected:
	virtual bool readFiles(bool immediate);
};

#endif // QTREEVIEWUPDATECONFFILE_H
