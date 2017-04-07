#ifndef QFRAMESYNCGROUPUPDATE_H
#define QFRAMESYNCGROUPUPDATE_H

#include <QFrame>
#include <QHBoxLayout>

#include "qframesyncgroupupdatefile.h"
#include "qframesyncgrouptransferfile.h"

class QFrameSyncGroupUpdate : public QFrame
{
	Q_OBJECT

public:
	QFrameSyncGroupUpdate(const QString& record, const QString& subpath, const QString& treeview, QWidget *parent=0);
	~QFrameSyncGroupUpdate();

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

	void setTitleText(const QString& title);
	void setThemePixmap(const QPixmap& pixmap);

	void setSyncTitleText(const QString& desc);

private slots:
	void sync(const QString& workspace, QTreeModel* model);
	void goBack();

private:
	QHBoxLayout*		m_pLayout;

	QFrameSyncGroupUpdateFile*		m_pFrameUpdate;
	QFrameSyncGroupTransferFile*	m_pFrameCommit;

	QStack<QWidget*>	m_stack;	//¥∞ø⁄œ‘ æ∂—’ª

	QPixmap		m_pixmap;

	QString		m_strRecord;
	QString		m_strSubPath;

	QString		m_strTreeView;
	QString		m_strDesc;
};

#endif // QFRAMESYNCGROUPUPDATE_H
