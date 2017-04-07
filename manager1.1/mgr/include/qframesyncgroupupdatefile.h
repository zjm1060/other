#ifndef QFRAMESYNCGROUPUPDATEFILE_H
#define QFRAMESYNCGROUPUPDATEFILE_H

#include <QFrame>
#include <QPushButton>
#include <QSignalMapper>
#include <QAction>
#include <QMenu>

#include "ui_syncgroupupdatefile.h"
#include "qtreeviewupdatedbfile.h"
#include "qlistviewsyncfilerequestserver.h"
#include "qlinkbutton.h"
#include "qreadwritexmlfile.h"

class QFrameSyncGroupUpdateFile : public QFrame, public Ui::FrameSyncGroupUpdateFile
{
	Q_OBJECT

public:
	QFrameSyncGroupUpdateFile(const QString& treeview, QWidget *parent=0);
	~QFrameSyncGroupUpdateFile();

public:
	void refreshWorkspace();

public:
	void setTitleText(const QString& title);
	void setDefaultDescText(const QString& desc);

	void setThemePixmap(const QPixmap& pixmap);
	void showTipsPixmap(bool show);

signals:
	void sync(const QString& workspace, QTreeModel* model);

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event);

private slots:
	void appendTip(int flag, const QString& tip);

	void readWorkspace();
	void currentTextChanged();

	void configureChanged(const QMap<QString, QString>& configure);

	void mouseRightButtonPressed(QPoint pos);
	void actionTriggered(int index);

	void clicked(const QModelIndex& index);
	void doubleClicked(const QModelIndex& index);

	void onClickedBtnWorkspace();
	void onClickedBtnInstantUpdate();
	void onClickedBtnSync();

	void refreshFileViewFinished();

	void serversLoaded(QVector<QPair<QString, QString> > servers);

	void selectedChanged();

	void modified();	//对比窗口保存修改
	void closing();		//对比窗口关闭

private:
	void writeConfigure();

	void setMenuState(bool refresh);

private:
	enum ActionType
	{
		ActionInstantUpdate,
		ActionManualUpdate,
		ActionSelectOrDeselect,
		ActionSelectNone,
		ActionRefresh,
		ActionEnd
	};

	QMenu*		m_pMenu;
	QAction*	m_pActions[ActionEnd];

	QSignalMapper*	m_pSignalMapper;

private:
	QString		m_strDefaultDesc;		//默认提示信息描述

	QListViewSyncFileRequestServer*	m_pListViewServers;		//请求同步端主机列表
	QLinkButton*	m_pButtonWorkspace;			//同步接收方工作目录
	QTreeViewUpdateFile*	m_pTreeViewFiles;	//同步接收方文件列表

	QPushButton*	m_pButtonUpdate;
	QPushButton*	m_pButtonSync;

	QMonitorConfigureFile*	m_pMonitor;
	QMap<QString, QString>	m_configure;	//接收端配置信息

	bool	m_bRefreshing;

private:
	QMap<QModelIndex, QObject*>		m_mapWnd;	//主窗口与当前
};

class QSyncFileGroupReadCommonFolderRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QSyncFileGroupReadCommonFolderRunnable(const QString& file,QObject* parent=0);
	virtual ~QSyncFileGroupReadCommonFolderRunnable();

signals:
	void finished(QList<QString> folders);

public:
	virtual void run();

private:
	QString	m_strFile;
};

#endif // QFRAMESYNCGROUPUPDATEFILE_H
