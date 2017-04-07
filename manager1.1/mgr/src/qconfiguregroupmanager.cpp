#include "qconfiguregroupmanager.h"

#include <QDir>
#include <QMessageBox>

#include "common.h"
#include "qservermanager.h"
#include "qglobalinfo.h"

QConfigureGroupManager::QConfigureGroupManager(QWidget *parent)
	: QFrame(parent)
	,m_pFrameSettings(NULL)
	,m_pFrameCommand(NULL)
{
	m_pLayout=new QHBoxLayout(this);
	setLayout(m_pLayout);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("Basic Settings"), ButtonBasicSettings);
	m_pButtonList->addButton(tr("Association Setting"), ButtonSyncAssociate);

	m_pLayout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonBasicSettings);
}

QConfigureGroupManager::~QConfigureGroupManager()
{
	
}

void QConfigureGroupManager::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (m_pFrameCommand != NULL)
	{
		m_pFrameCommand->setServerList(servers);
	}
}

void QConfigureGroupManager::buttonClicked(int id)
{
	switch (id)
	{
	case ButtonBasicSettings:
		{
			SAFE_HIDE_WIDGET(m_pFrameCommand);

			if (m_pFrameSettings == NULL)
			{
				m_pFrameSettings=new QFrameBasicSettings(this);
				m_pFrameSettings->setFrameShadow(QFrame::Plain);

				connect(m_pFrameSettings, SIGNAL(labelSettingChanged(const QMap<QString, QString>&)),
					this, SIGNAL(labelSettingChanged(const QMap<QString, QString>&)));

				m_pLayout->addWidget(m_pFrameSettings);
			}

			m_pFrameSettings->show();
		}
		break;
	case ButtonSyncAssociate:
		{
			SAFE_HIDE_WIDGET(m_pFrameSettings);

			if (m_pFrameCommand == NULL)
			{
				m_pFrameCommand=new QFrameSyncCommand(this);
				m_pFrameCommand->setFrameShadow(QFrame::Plain);

				m_pFrameCommand->setServerList(QGlobalInfo::instance()->serverList());

				m_pLayout->addWidget(m_pFrameCommand);
			}

			m_pFrameCommand->show();
		}
		break;
	default:
		break;
	}
}
