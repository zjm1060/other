#include "qinspectmanager.h"

#include "common.h"
#include "qmessageparser.h"
#include "qglobalinfo.h"

QInspectManager::QInspectManager(QWidget *parent)
	: QFrame(parent)
	,m_pFrameSetting(NULL)
	,m_pFrameInspect(NULL)
{
	m_pLayout=new QHBoxLayout(this);
	setLayout(m_pLayout);

	m_pFrameSetting=new QFrameInspectSetting(this);
	m_pLayout->addWidget(m_pFrameSetting);

	connect(m_pFrameSetting, SIGNAL(inspect(ServerInfo, QTreeViewInspectSetting::InspectItem)),
		this, SLOT(inspect(ServerInfo, QTreeViewInspectSetting::InspectItem)));
}

QInspectManager::~QInspectManager()
{

}

void QInspectManager::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (m_pFrameSetting != NULL)
	{
		m_pFrameSetting->setServerList(servers);
	}
}

void QInspectManager::inspect(ServerInfo info, QTreeViewInspectSetting::InspectItem items)
{
	SAFE_HIDE_WIDGET(m_pFrameSetting);

	if (m_pFrameInspect == NULL)
	{
		m_pFrameInspect=new QFrameInspect(this);
		m_pLayout->addWidget(m_pFrameInspect);

		connect(m_pFrameInspect, SIGNAL(goBack()), this, SLOT(goBack()));
	}

	m_pFrameInspect->setInspectItems(info, items);

	m_pFrameInspect->show();
}

void QInspectManager::goBack()
{
	SAFE_HIDE_WIDGET(m_pFrameInspect);

	m_pFrameSetting->show();
}
