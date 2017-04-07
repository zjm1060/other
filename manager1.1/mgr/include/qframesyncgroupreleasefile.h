#ifndef QFRAMESYNCGROUPRELEASEFILE_H
#define QFRAMESYNCGROUPRELEASEFILE_H

#include <QFrame>
#include <QPushButton>
#include <QSignalMapper>
#include <QAction>
#include <QMenu>

#include "qlinkbutton.h"
#include "qtreeviewupdatefile.h"
#include "qreadwritexmlfile.h"

class QFrameSyncGroupReleaseFile : public QFrame
{
	Q_OBJECT

public:
	QFrameSyncGroupReleaseFile(const QString& treeview, bool enableRestrict, QWidget *parent=0);
	~QFrameSyncGroupReleaseFile();

public:
	void refreshWorkspace();
	QString workspace() const;

	bool hasSelected() const;
	QVector<QPair<DiffFile, FileInfo> > selectedItems();

	QString record() const;

	void setRestrictFilterEnabled(bool enable);
	void setLocalLastModifiedEnabled(bool enable);

public:
	bool isLoaded() const;

signals:
	void loaded(bool state);
	void selectedChanged();
	void sendTip(int type, QString tip);

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event);

private slots:
	void readWorkspace();
	void currentTextChanged();

	void mouseRightButtonPressed(QPoint pos);
	void actionTriggered(int index);

	void configureChanged(const QMap<QString, QString>& configure);

	void doubleClicked(const QModelIndex& index);

	void onClickedBtnWorkspace();

	void refreshFileViewFinished();

private:
	void setMenuState(bool refresh);

	void writeConfigure();

private:
	enum ActionType
	{
		ActionSelectOrDeselect,
		ActionSelectAll,
		ActionSelectNone,
		ActionRefresh,
		ActionEnd
	};

	QMenu*		m_pMenu;
	QAction*	m_pActions[ActionEnd];

	QSignalMapper*	m_pSignalMapper;

private:
	QLinkButton*	m_pButtonWorkspace;			//同步接收方工作目录
	QTreeViewUpdateFile*	m_pTreeViewFiles;	//同步接收方文件列表

	QPushButton*	m_pButtonRefresh;

	QMonitorConfigureFile*	m_pMonitor;
	QMap<QString, QString>	m_configure;	//接收端配置信息

	bool	m_bRefreshing;
};

#endif // QFRAMESYNCGROUPRELEASEFILE_H
