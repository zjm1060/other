#include "qframesyncgroupupdate.h"

#include "common.h"
#include "qmessageparser.h"
#include "qglobalinfo.h"

QFrameSyncGroupUpdate::QFrameSyncGroupUpdate(const QString& record, const QString& subpath, const QString& treeview, QWidget *parent/* =0 */)
	: QFrame(parent)
	,m_pFrameUpdate(NULL)
	,m_pFrameCommit(NULL)
	,m_strRecord(record)
	,m_strSubPath(subpath)
	,m_strTreeView(treeview)
{
	m_pLayout=new QHBoxLayout(this);
	setLayout(m_pLayout);

	m_pFrameUpdate=new QFrameSyncGroupUpdateFile(treeview, this);
	m_pLayout->addWidget(m_pFrameUpdate);

	connect(m_pFrameUpdate, SIGNAL(sync(const QString&, QTreeModel*)), this, SLOT(sync(const QString&, QTreeModel*)));
}

QFrameSyncGroupUpdate::~QFrameSyncGroupUpdate()
{

}

void QFrameSyncGroupUpdate::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (m_pFrameCommit != NULL)
	{
		m_pFrameCommit->setServerList(servers);
	}
}

void QFrameSyncGroupUpdate::setTitleText(const QString& title)
{
	if (m_pFrameUpdate != NULL)
	{
		m_pFrameUpdate->setTitleText(title);
	}
}

void QFrameSyncGroupUpdate::setThemePixmap(const QPixmap& pixmap)
{
	m_pixmap=pixmap;

	if (m_pFrameUpdate != NULL)
	{
		m_pFrameUpdate->setThemePixmap(pixmap);
	}
}

void QFrameSyncGroupUpdate::setSyncTitleText(const QString& desc)
{
	m_strDesc=desc;
	if (m_pFrameCommit != NULL)
	{
		m_pFrameCommit->setTitleText(desc);
	}
}

void QFrameSyncGroupUpdate::sync(const QString& workspace, QTreeModel* model)
{
	m_stack.push(m_pFrameUpdate);
	SAFE_HIDE_WIDGET(m_pFrameUpdate);

	if (m_pFrameCommit == NULL)
	{
		m_pFrameCommit=new QFrameSyncGroupTransferFile(m_strRecord, m_strSubPath, m_strTreeView, this);
		m_pLayout->addWidget(m_pFrameCommit);

		m_pFrameCommit->setTitleText(m_strDesc);

		m_pFrameCommit->setThemePixmap(m_pixmap);
		setServerList(QGlobalInfo::instance()->serverList());

		connect(m_pFrameCommit, SIGNAL(goBack()), this, SLOT(goBack()));
	}

	if (m_pFrameCommit != NULL)
	{
		m_pFrameCommit->setWorkspace(workspace);
		m_pFrameCommit->setTreeModel(model);

		m_pFrameCommit->show();
	}
}

void QFrameSyncGroupUpdate::goBack()
{
	QWidget* pWidget=m_stack.pop();

	QWidget* pWidgetSender=(QWidget*)sender();
	SAFE_HIDE_WIDGET(pWidgetSender);

	if (pWidget != NULL)
	{
		pWidget->show();
	}
}
