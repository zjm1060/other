#ifndef QTREEVIEWFILESYSTEM_H
#define QTREEVIEWFILESYSTEM_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QPoint>
#include <QRunnable>

#include "qutildef.h"
#include "qfileicondelegate.h"
#include "qnumberdelegate.h"
#include "libctlglobal.h"

class CTL_EXPORT QSortFilterFileSystemModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	QSortFilterFileSystemModel(QObject* parent=0);
	~QSortFilterFileSystemModel();

protected:
	virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};

class CTL_EXPORT QTreeViewFileSystem : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewFileSystem(QWidget *parent=0);
	~QTreeViewFileSystem();

public:
	enum TitleLabel
	{
		NameTitle,
		SizeTitle,
		TypeTitle,
		ModifiedTitle
	};

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
	FileInfo fileInfo(const QModelIndex& index);

	void setFileName(const QModelIndex& index, const QString& name);
	void setFileName(int row, const QString& name);

	void setCurrentEdit(const QModelIndex& index);		//index 为数据源索引(非排序后索引)

Q_SIGNALS:
	void mouseRightButtonPress(QPoint pos);
	void dataChanged(QModelIndex index, QString old, QString now);
	void backspaceKeyPress();
	void deleteKeyPress();
	void refreshKeyPress();

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);

private slots:
	void insertFileInfoFinished(QStandardItemModel* model);
	void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
	void setSourceModel(QStandardItemModel* model);

private:
	QStandardItemModel*		m_pModel;
	QSortFilterFileSystemModel*	m_pSortModel;

	QFileIconDelegate*		m_pIconDelegate;
	QNumberDelegate*		m_pNumberDelegate;

	bool	m_bEditable;
};

class CTL_EXPORT QTreeViewFileSystemRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QTreeViewFileSystemRunnable(const QVector<FileInfo>& files, bool isEditable, bool isDrive=false,QObject* parent=0);
	QTreeViewFileSystemRunnable(const QFileInfoList& files, bool isEditable, bool isDrive=false,QObject* parent=0);
	virtual ~QTreeViewFileSystemRunnable();

public:
	virtual void run();

signals:
	void finished(QStandardItemModel* model);

private:
	QStandardItemModel*	m_pModel;
	QVector<FileInfo>	m_files_1;
	QFileInfoList		m_files_2;

	bool m_isDrive;
	bool m_bEditable;
};

#endif // QTREEVIEWFILESYSTEM_H
