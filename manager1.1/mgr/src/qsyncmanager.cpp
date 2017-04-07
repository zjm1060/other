#include "qsyncmanager.h"

#include "common.h"
#include "qmessageparser.h"

QSyncManager::QSyncManager(QClient* client, QWidget *parent)
	: QFrame(parent)
	,m_pFrameDatabaseSync(NULL)
{
	m_pClient=client;

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("H9000 Config Files"), ButtonConfigFiles);
	m_pButtonList->addButton(tr("H9000 Pictures"), ButtonPictures);
	m_pButtonList->addButton(tr("H9000 DB Define files"), ButtonDBDefine);
	m_pButtonList->addButton(tr("H9000 Interlock"), ButtonInterlock);
	m_pButtonList->addButton(tr("H9000 Calculation"), ButtonCalculation);
	m_pButtonList->addButton(tr("H9000 Sound Files"), ButtonSoundFiles);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonConfigFiles);
}

QSyncManager::~QSyncManager()
{

}

void QSyncManager::buttonClicked(int id)
{
	switch (id)
	{
	case ButtonConfigFiles:
		{
			SAFE_HIDE_WIDGET(m_pFrameDatabaseSync);
		}
		break;
	case ButtonPictures:
		{
			SAFE_HIDE_WIDGET(m_pFrameDatabaseSync);
		}
		break;
	case ButtonDBDefine:
		{
			if (m_pFrameDatabaseSync == NULL)
			{
				m_pFrameDatabaseSync=new QFrameSyncFile(m_pClient, this);

				m_pFrameDatabaseSync->setTitleText(tr("Synchronize H9000 database define files"));
				m_pFrameDatabaseSync->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameDatabaseSync->setThemePixmap(findPixmap(":/images/dbfile.png"));

				m_pFrameDatabaseSync->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameDatabaseSync->setFrameShape(QFrame::Box);
				m_pFrameDatabaseSync->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameDatabaseSync);
			}
			m_pFrameDatabaseSync->show();
		}
		break;
	case ButtonInterlock:
		{
			SAFE_HIDE_WIDGET(m_pFrameDatabaseSync);
		}
		break;
	case ButtonCalculation:
		{
			SAFE_HIDE_WIDGET(m_pFrameDatabaseSync);
		}
		break;
	case ButtonSoundFiles:
		{
			SAFE_HIDE_WIDGET(m_pFrameDatabaseSync);
		}
		break;
	default:
		break;
	}
}
