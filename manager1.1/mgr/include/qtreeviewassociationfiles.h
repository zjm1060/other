#ifndef QTREEVIEWASSOCIATIOINFILES_H
#define QTREEVIEWASSOCIATIOINFILES_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QPoint>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QSortFilterProxyModel>

#include "qutildef.h"
#include "qutilreadwritexmlfile.h"

class QTreeViewAssociationFilesDelegate;
class QTreeViewAssociationFiles : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewAssociationFiles(QWidget *parent=0);
	~QTreeViewAssociationFiles();

public:
	enum TitleLabel
	{
		NameTitle,
		SizeTitle,
		TypeTitle,
		ModifiedTitle,
		PathTitle
	};

	enum UserData
	{
		FileDataRole=Qt::UserRole		//文件数据
	};

public:
	void insertFiles(const QVector<SyncMonitorFile>& files);

	QModelIndex appendFileInfo(const SyncMonitorFile& file);

	int sortColumn();

	QModelIndexList selectedRows();
	void removeRows(const QModelIndexList& items);

	void setExclusive(bool exclusive);

public:
	void clear();

	SyncMonitorFile fileInfo(const QModelIndex& index) const;
	QVector<SyncMonitorFile> fileInfos() const;

Q_SIGNALS:
	void mouseRightButtonPress(QPoint pos);
	void deleteKeyPress();
	void refreshKeyPress();
	void changed(QVector<SyncMonitorFile> files);

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);

private slots:
	void itemChanged(QStandardItem* item);

private:
	QSortFilterProxyModel*		m_pSortModel;
	QStandardItemModel*			m_pModel;

	bool	m_bExclusive;	//是否排他

	QTreeViewAssociationFilesDelegate*		m_pDelegate;
};

class QTreeViewAssociationFilesDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QTreeViewAssociationFilesDelegate(QObject* parent=0);
	~QTreeViewAssociationFilesDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void drawItem(QPainter* painter, const QStyleOptionViewItem& option, const QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // QTREEVIEWASSOCIATIOINFILES_H
