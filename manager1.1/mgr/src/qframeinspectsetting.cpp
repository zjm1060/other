#include "qframeinspectsetting.h"

#include <QDir>

#include "common.h"
#include "qservermanager.h"

QFrameInspectSetting::QFrameInspectSetting(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);

	m_pButtonRefresh=new QLinkButton(tr("Refresh"), this);
	horizontalLayoutButtons->addWidget(m_pButtonRefresh);

	connect(m_pButtonRefresh, SIGNAL(clicked()), this, SLOT(refresh()));

	m_pListViewServers=new QListViewControlledServer(this);
	horizontalLayoutSource->addWidget(m_pListViewServers);

	connect(m_pListViewServers, SIGNAL(currentChanged(const QModelIndex&)), this, SLOT(currentChanged(const QModelIndex&)));

	m_pTreeViewSetting=new QTreeViewInspectSetting(this);
	horizontalLayoutInspect->addWidget(m_pTreeViewSetting);

	connect(m_pTreeViewSetting, SIGNAL(enableInspect(bool)), this, SLOT(setInspectEnabled(bool)));

	setInspectEnabled(false);
	connect(pushButtonInspect, SIGNAL(clicked()), this, SLOT(inspect()));
}

QFrameInspectSetting::~QFrameInspectSetting()
{
	
}

void QFrameInspectSetting::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	m_pListViewServers->refreshServerList(servers);
}

void QFrameInspectSetting::currentChanged(const QModelIndex& index)
{
	if (m_pListViewServers == NULL || !index.isValid())
	{
		return;
	}

	ServerInfo info=m_pListViewServers->serverInfoFromIndex(index);
	m_pTreeViewSetting->setClient(info);
}

void QFrameInspectSetting::setInspectEnabled(bool enabled)
{
	if (pushButtonInspect != NULL)
	{
		pushButtonInspect->setEnabled(enabled);
	}

	if (m_pButtonRefresh != NULL)
	{
		m_pButtonRefresh->setEnabled(enabled);
	}
}

void QFrameInspectSetting::inspect()
{
	ServerInfo info;
	if (m_pListViewServers->selectedServer(info))
	{
		QTreeViewInspectSetting::InspectItem items=m_pTreeViewSetting->writeConfigure();

		emit inspect(info, items);
	}
}

void QFrameInspectSetting::refresh()
{
	if (m_pListViewServers != NULL && m_pTreeViewSetting != NULL)
	{
		ServerInfo info;
		if (m_pListViewServers->selectedServer(info))
		{
			m_pTreeViewSetting->setClient(info);
		}
	}
}
