#ifndef QFRAMEFILESYSTEMPANEL_H
#define QFRAMEFILESYSTEMPANEL_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QStandardItemModel;
class QSortFilterProxyModel;
QT_END_NAMESPACE

class Ui_FrameFileSystem;
class QDiskDelegate;

#include "qutildef.h"

class QFrameFileSystemPanel : public QFrame
{
	Q_OBJECT

public:
	QFrameFileSystemPanel(QWidget *parent=0);
	~QFrameFileSystemPanel();

public:
	enum LogicalDiskTitle
	{
		NameTitle,
		FileSystemTitle
	};

public:
	void setLogicalDisk(const QVector<LogicalDisk>& disk);

private:
	void showLogicalDisk();

private:
	Ui_FrameFileSystem*		m_ui;

	QVector<LogicalDisk>	m_vecLogicalDisk;

	QStandardItemModel*		m_pModelComputer;
	QSortFilterProxyModel*	m_pSortModel;
	QDiskDelegate*			m_pDelegate;
};

#endif // QFRAMEFILESYSTEMPANEL_H
