#include "qsyncgroupupdatemanager.h"

#include "common.h"
#include "qmessageparser.h"

QSyncGroupUpdateManager::QSyncGroupUpdateManager(QWidget *parent)
	: QFrame(parent)
{
	memset(m_pFrame, 0, sizeof(QFrameSyncGroupUpdate*)*ButtonEnd);

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(170);

	m_pButtonList->addButton(tr("H9000 Config Files"), ButtonConfigFiles);
	m_pButtonList->addButton(tr("H9000 Pictures"), ButtonPictures);
	m_pButtonList->addButton(tr("H9000 DB Define files"), ButtonDBDefine);
	m_pButtonList->addButton(tr("H9000 Data Files"), ButtonDatFiles);
	if (parseConfig(getAppParentDir()+INI_FILE, "update/xld", true).toBool())
	{
		m_pButtonList->addButton(tr("XLD IEC104 Files"), ButtonXLDIEC104Files);
	}
	if (parseConfig(getAppParentDir()+INI_FILE, "update/xjb", true).toBool())
	{
		m_pButtonList->addButton(tr("XJB IEC104 Files"), ButtonXJBIEC104Files);
	}
	m_pButtonList->addButton(tr("H9000 Sound Files"), ButtonSoundFiles);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonConfigFiles);
}

QSyncGroupUpdateManager::~QSyncGroupUpdateManager()
{

}

void QSyncGroupUpdateManager::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	for (int i=0; i<ButtonEnd; i++)
	{
		if (m_pFrame[i] != NULL)
		{
			m_pFrame[i]->setServerList(servers);
		}
	}
}

void QSyncGroupUpdateManager::buttonClicked(int id)
{
	for (int i=0; i<ButtonEnd; i++)
	{
		if (i == id)
		{
			continue;
		}

		SAFE_HIDE_WIDGET(m_pFrame[i]);
	}

	switch (id)
	{
	case ButtonConfigFiles:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupUpdate("config", H9000_DEF_PATH, "QTreeViewUpdateConfFile", this);

				m_pFrame[id]->setTitleText(tr("Update H9000 configure files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/config.png"));

				m_pFrame[id]->setSyncTitleText(tr("Synchronize H9000 configure files"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->show();
		}
		break;
	case ButtonPictures:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupUpdate("dsp", H9000_DSP_PATH, "QTreeViewUpdateDspFile", this);

				m_pFrame[id]->setTitleText(tr("Update H9000 dsp files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/picture.png"));

				m_pFrame[id]->setSyncTitleText(tr("Synchronize H9000 dsp files"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->show();
		}
		break;
	case ButtonDBDefine:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupUpdate("database", H9000_DATABASE_PATH, "QTreeViewUpdateDbFile", this);

				m_pFrame[id]->setTitleText(tr("Update H9000 database define files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/dbfile.png"));

				m_pFrame[id]->setSyncTitleText(tr("Synchronize H9000 real-time database files"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->show();
		}
		break;
	case ButtonDatFiles:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupUpdate("dat", H9000_DAT_PATH, "QTreeViewUpdateDatFile", this);

				m_pFrame[id]->setTitleText(tr("Update H9000 data files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/dat.png"));

				m_pFrame[id]->setSyncTitleText(tr("Synchronize H9000 data files"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->show();
		}
		break;
	case ButtonXLDIEC104Files:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupUpdate("iec104-xld", H9000_IEC104_PATH, "QTreeViewUpdateXLDIEC104File", this);

				m_pFrame[id]->setTitleText(tr("Update H9000 XLD IEC104 files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/iec.png"));

				m_pFrame[id]->setSyncTitleText(tr("Synchronize H9000 XLD IEC104 files"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->show();
		}
		break;
	case ButtonXJBIEC104Files:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupUpdate("iec104-xjb", H9000_IEC104_PATH, "QTreeViewUpdateXJBIEC104File", this);

				m_pFrame[id]->setTitleText(tr("Update H9000 XJB IEC104 files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/iec.png"));

				m_pFrame[id]->setSyncTitleText(tr("Synchronize H9000 XJB IEC104 files"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->show();
		}
		break;
	case ButtonSoundFiles:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupUpdate("voice", H9000_VOICE_PATH, "QTreeViewUpdateVoiceFile", this);

				m_pFrame[id]->setTitleText(tr("Update H9000 voice files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/voice.png"));

				m_pFrame[id]->setSyncTitleText(tr("Synchronize H9000 voice files"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->show();
		}
		break;
	default:
		break;
	}
}

