#ifndef QTREEVIEWFILESYSTEM_H
#define QTREEVIEWFILESYSTEM_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QFileIconProvider>
#include <QPoint>

#include "extern.h"

class QTreeViewFileSystem : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewFileSystem(QWidget *parent=0);
	~QTreeViewFileSystem();

public:
	void insertFileInfo(const QFileInfoList& files, bool isDrive=false);
	void insertFileInfo(const QVector<FileInfo>& files, bool isDrive=false);

	QModelIndex appendFileInfo(const QFileInfo& file, bool isDrive);
	QModelIndex appendFileInfo(const FileInfo& file, bool isDrive=false);

	int sortColumn();

	QModelIndexList selectedRows();

	void setEditable(bool editable);

public:
	void clear();

	QString filename(const QModelIndex& index);
	int type(const QModelIndex& index);
	QString typeDesc(const QModelIndex& index);
	quint64 size(const QModelIndex& index);

	void setFileName(const QModelIndex& index, const QString& name);
	void setFileName(int row, const QString& name);

	void setCurrentEdit(const QModelIndex& index);		//index 为数据源索引(非排序后索引)

Q_SIGNALS:
	void mouseRightButtonPress(QPoint pos);
	void dataChanged(QModelIndex index, QString old, QString now);
	void layoutChanged();

protected:
	virtual void mousePressEvent(QMouseEvent* event);

private slots:
	void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
	QStandardItemModel*		m_pModel;
	QSortFilterProxyModel*	m_pSortModel;

	QFileIconProvider		m_fileIconProvider;

	bool	m_bEditable;
};

#endif // QTREEVIEWFILESYSTEM_H
