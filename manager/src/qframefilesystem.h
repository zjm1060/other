#ifndef QFRAMEFILESYSTEM_H
#define QFRAMEFILESYSTEM_H

#include <QFrame>
#include <QSignalMapper>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "qcomboboxfilesystem.h"
#include "qtreeviewfilesystem.h"

class QFrameFileSystem : public QFrame
{
	Q_OBJECT

public:
	QFrameFileSystem(QWidget *parent);
	~QFrameFileSystem();

	enum ToolButtonType
	{
		ButtonHome=0,
		ButtonUp,
		ButtonEnd
	};

public:
	void setDrives(const QFileInfoList& drives);
	void setDrives(const QVector<FileInfo>& drives);

	void setFileInfo(const QFileInfoList& files, int itemType=-1);
	void setFileInfo(const QVector<FileInfo>& files, int itemType=-1);

	void setLocation(const QString& location, bool test=true);
	QString location() const;

	void clear();

public:
	QComboBoxFileSystem*	comboBoxFileSystem()	{ return m_pComboBoxFileSystem; }
	QTreeViewFileSystem*	treeViewFileSystem() { return m_pTreeViewFileSystem; }

Q_SIGNALS:
	void clickedToolButton(int type);
	void currentIndexChanged(QString location);

private:
	void init();

private slots:
	void itemPressed(QModelIndex index);

private:
	QToolButton*	m_pToolButton[ButtonEnd];
	QSignalMapper*	m_pSignalMapper;

	QComboBoxFileSystem*	m_pComboBoxFileSystem;
	QTreeViewFileSystem*	m_pTreeViewFileSystem;

	QHBoxLayout*	m_pHorizontalLayout;
	QVBoxLayout*	m_pVerticalLayout;
};

#endif // QFRAMEFILESYSTEM_H
