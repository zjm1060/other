#include "qsyncgroupcommitmanager.h"

#include "common.h"
#include "qmessageparser.h"
#include "qglobalinfo.h"

QSyncGroupCommitManager::QSyncGroupCommitManager(QWidget *parent)
	: QFrame(parent)
{
	memset(m_pFrame, 0, sizeof(QFrameSyncGroupCommitFile*)*ButtonEnd);

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(170);

	m_pButtonList->addButton(tr("H9000 Config Files"), ButtonConfigFiles);
	m_pButtonList->addButton(tr("H9000 Pictures"), ButtonPictures);
	m_pButtonList->addButton(tr("H9000 DB Define files"), ButtonDBDefine);
	m_pButtonList->addButton(tr("H9000 Data Files"), ButtonDatFiles);
	if (parseConfig(getAppParentDir()+INI_FILE, "commit/xld", true).toBool())
	{
		m_pButtonList->addButton(tr("XLD IEC104 Files"), ButtonXLDIEC104Files);
	}
	if (parseConfig(getAppParentDir()+INI_FILE, "commit/xjb", true).toBool())
	{
		m_pButtonList->addButton(tr("XJB IEC104 Files"), ButtonXJBIEC104Files);
	}

	m_pButtonList->addButton(tr("H9000 Sound Files"), ButtonSoundFiles);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonConfigFiles);
}

QSyncGroupCommitManager::~QSyncGroupCommitManager()
{

}

void QSyncGroupCommitManager::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	for (int i=0; i<ButtonEnd; i++)
	{
		if (m_pFrame[i] != NULL)
		{
			m_pFrame[i]->refreshServerList(servers);
		}
	}
}

void QSyncGroupCommitManager::buttonClicked(int id)
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
				m_pFrame[id]=new QFrameSyncGroupCommitFile("QTreeViewSyncConfFile", true, this);

				m_pFrame[id]->setTitleText(tr("Commit the modified H9000 configure files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/config.png"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->refreshServerList(QGlobalInfo::instance()->serverList());
			m_pFrame[id]->show();
		}
		break;
	case ButtonPictures:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupCommitFile("QTreeViewSyncDspFile", true, this);

				m_pFrame[id]->setTitleText(tr("Commit the modified H9000 dsp files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/picture.png"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->refreshServerList(QGlobalInfo::instance()->serverList());
			m_pFrame[id]->show();
		}
		break;
	case ButtonDBDefine:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupCommitFile("QTreeViewSyncDbFile", true, this);

				m_pFrame[id]->setTitleText(tr("Commit the modified H9000 database define files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/dbfile.png"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->refreshServerList(QGlobalInfo::instance()->serverList());
			m_pFrame[id]->show();
		}
		break;
	case ButtonDatFiles:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupCommitFile("QTreeViewSyncDatFile", true, this);

				m_pFrame[id]->setTitleText(tr("Commit the modified H9000 data files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/dat.png"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->refreshServerList(QGlobalInfo::instance()->serverList());
			m_pFrame[id]->show();
		}
		break;
	case ButtonXLDIEC104Files:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupCommitFile("QTreeViewSyncXLDIEC104File", true, this);

				m_pFrame[id]->setTitleText(tr("Commit the modified H9000 XLD IEC104 files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/iec.png"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->refreshServerList(QGlobalInfo::instance()->serverList());
			m_pFrame[id]->show();
		}
		break;
	case ButtonXJBIEC104Files:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupCommitFile("QTreeViewSyncXJBIEC104File", true, this);

				m_pFrame[id]->setTitleText(tr("Commit the modified H9000 XJB IEC104 files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/iec.png"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->refreshServerList(QGlobalInfo::instance()->serverList());
			m_pFrame[id]->show();
		}
		break;
	case ButtonSoundFiles:
		{
			if (m_pFrame[id] == NULL)
			{
				m_pFrame[id]=new QFrameSyncGroupCommitFile("QTreeViewSyncVoiceFile", true, this);

				m_pFrame[id]->setTitleText(tr("Commit the modified H9000 voice files"));
				m_pFrame[id]->setThemePixmap(findPixmap(":/images/voice.png"));

				m_pFrame[id]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrame[id]->setFrameShape(QFrame::Box);
				m_pFrame[id]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrame[id]);
			}

			m_pFrame[id]->refreshServerList(QGlobalInfo::instance()->serverList());
			m_pFrame[id]->show();
		}
		break;
	default:
		break;
	}
}

