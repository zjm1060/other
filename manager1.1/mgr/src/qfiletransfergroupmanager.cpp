#include "qfiletransfergroupmanager.h"

#include "common.h"
#include "qmessageparser.h"
#include "qreadwritexmlfile.h"
#include "qlistviewsyncfileserver.h"
#include "qglobalinfo.h"

QFileTransferGroupManager::QFileTransferGroupManager(QWidget *parent)
	: QFrame(parent)
	,m_bChanged(false)
{
	memset(m_pFrameFileTransfer, 0, sizeof(QFrameFileTransferGroup*)*ButtonEnd);

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(170);

	m_pButtonList->addButton(tr("H9000 Config Files"), ButtonConfigFiles);
	m_pButtonList->addButton(tr("H9000 Pictures"), ButtonPictures);
	m_pButtonList->addButton(tr("H9000 DB Define files"), ButtonDBDefine);
	m_pButtonList->addButton(tr("H9000 Data Files"), ButtonDatFiles);
	m_pButtonList->addButton(tr("H9000 IEC104 Files"), ButtonIEC104Files);
	m_pButtonList->addButton(tr("H9000 Sound Files"), ButtonSoundFiles);
	m_pButtonList->addButton(tr("File System"), ButtonFileSystem);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	QReadServersFromXmlFile reader(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE));
	if (reader.read())
	{
		m_addedServers=reader.result();
	}

	m_pButtonList->click(ButtonConfigFiles);
}

QFileTransferGroupManager::~QFileTransferGroupManager()
{
	if (m_bChanged)
	{
		QWriteServersToXmlFile writer(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE), m_addedServers);
		writer.write();
	}
}

void QFileTransferGroupManager::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	for (int i=0; i<ButtonEnd; i++)
	{
		if (m_pFrameFileTransfer[i] != NULL)
		{
			m_pFrameFileTransfer[i]->setServerList(servers);
		}
	}
}

void QFileTransferGroupManager::addServer(const Server& server)
{
	m_bChanged=true;

	m_addedServers.append(server);

	for (int i=0; i<ButtonEnd; i++)
	{
		if (m_pFrameFileTransfer[i] != NULL)
		{
			m_pFrameFileTransfer[i]->setAddedServerList(m_addedServers);
		}
	}
}

void QFileTransferGroupManager::removeServers(const QList<Server>& servers)
{
	m_bChanged=true;

	QListIterator<Server> it(servers);
	while (it.hasNext())
	{
		const Server& server=it.next();

		QMutableListIterator<Server> iter(m_addedServers);
		while (iter.hasNext())
		{
			const Server& s=iter.next();
			if (server.addr == s.addr && server.port == s.port)
			{
				iter.remove();
				break;
			}
		}
	}

	for (int i=0; i<ButtonEnd; i++)
	{
		if (m_pFrameFileTransfer[i] != NULL)
		{
			m_pFrameFileTransfer[i]->setAddedServerList(m_addedServers);
		}
	}
}

void QFileTransferGroupManager::buttonClicked(int id)
{
	for (int i=0; i<ButtonEnd; i++)
	{
		if (i == id)
		{
			continue;
		}

		SAFE_HIDE_WIDGET(m_pFrameFileTransfer[i]);
	}

	switch (id)
	{
	case ButtonConfigFiles:
		{
			if (m_pFrameFileTransfer[ButtonConfigFiles] == NULL)
			{
				m_pFrameFileTransfer[ButtonConfigFiles]=new QFrameFileTransferGroup("config", H9000_DEF_PATH, this);

				m_pFrameFileTransfer[ButtonConfigFiles]->setTitleText(tr("Transfer H9000 config files"));
				m_pFrameFileTransfer[ButtonConfigFiles]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonConfigFiles]->setThemePixmap(findPixmap(":/images/config.png"));

				m_pFrameFileTransfer[ButtonConfigFiles]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonConfigFiles]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonConfigFiles]->setFrameShadow(QFrame::Raised);

				m_pFrameFileTransfer[ButtonConfigFiles]->setServerList(QGlobalInfo::instance()->serverList());
				m_pFrameFileTransfer[ButtonConfigFiles]->setAddedServerList(m_addedServers);

				connect(m_pFrameFileTransfer[ButtonConfigFiles], SIGNAL(addServer(const Server&)), this, SLOT(addServer(const Server&)));
				connect(m_pFrameFileTransfer[ButtonConfigFiles], SIGNAL(removeServers(const QList<Server>&)), this, SLOT(removeServers(const QList<Server>&)));

				m_layout->addWidget(m_pFrameFileTransfer[ButtonConfigFiles]);
			}
			m_pFrameFileTransfer[ButtonConfigFiles]->show();
		}
		break;
	case ButtonPictures:
		{
			if (m_pFrameFileTransfer[ButtonPictures] == NULL)
			{
				m_pFrameFileTransfer[ButtonPictures]=new QFrameFileTransferGroup("dsp", H9000_DSP_PATH, this);

				m_pFrameFileTransfer[ButtonPictures]->setTitleText(tr("Transfer H9000 picture files"));
				m_pFrameFileTransfer[ButtonPictures]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonPictures]->setThemePixmap(findPixmap(":/images/picture.png"));

				m_pFrameFileTransfer[ButtonPictures]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonPictures]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonPictures]->setFrameShadow(QFrame::Raised);

				m_pFrameFileTransfer[ButtonPictures]->setServerList(QGlobalInfo::instance()->serverList());
				m_pFrameFileTransfer[ButtonPictures]->setAddedServerList(m_addedServers);

				connect(m_pFrameFileTransfer[ButtonPictures], SIGNAL(addServer(const Server&)), this, SLOT(addServer(const Server&)));
				connect(m_pFrameFileTransfer[ButtonPictures], SIGNAL(removeServers(const QList<Server>&)), this, SLOT(removeServers(const QList<Server>&)));

				m_layout->addWidget(m_pFrameFileTransfer[ButtonPictures]);
			}
			m_pFrameFileTransfer[ButtonPictures]->show();
		}
		break;
	case ButtonDBDefine:
		{
			if (m_pFrameFileTransfer[ButtonDBDefine] == NULL)
			{
				m_pFrameFileTransfer[ButtonDBDefine]=new QFrameFileTransferGroup("database", H9000_DATABASE_PATH, this);

				m_pFrameFileTransfer[ButtonDBDefine]->setTitleText(tr("Transfer H9000 database define files"));
				m_pFrameFileTransfer[ButtonDBDefine]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonDBDefine]->setThemePixmap(findPixmap(":/images/dbfile.png"));

				m_pFrameFileTransfer[ButtonDBDefine]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonDBDefine]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonDBDefine]->setFrameShadow(QFrame::Raised);

				m_pFrameFileTransfer[ButtonDBDefine]->setServerList(QGlobalInfo::instance()->serverList());
				m_pFrameFileTransfer[ButtonDBDefine]->setAddedServerList(m_addedServers);

				connect(m_pFrameFileTransfer[ButtonDBDefine], SIGNAL(addServer(const Server&)), this, SLOT(addServer(const Server&)));
				connect(m_pFrameFileTransfer[ButtonDBDefine], SIGNAL(removeServers(const QList<Server>&)), this, SLOT(removeServers(const QList<Server>&)));

				m_layout->addWidget(m_pFrameFileTransfer[ButtonDBDefine]);
			}
			m_pFrameFileTransfer[ButtonDBDefine]->show();
		}
		break;
	case ButtonDatFiles:
		{
			if (m_pFrameFileTransfer[ButtonDatFiles] == NULL)
			{
				m_pFrameFileTransfer[ButtonDatFiles]=new QFrameFileTransferGroup("dat", H9000_DAT_PATH, this);

				m_pFrameFileTransfer[ButtonDatFiles]->setTitleText(tr("Transfer H9000 date files"));
				m_pFrameFileTransfer[ButtonDatFiles]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonDatFiles]->setThemePixmap(findPixmap(":/images/dat.png"));

				m_pFrameFileTransfer[ButtonDatFiles]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonDatFiles]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonDatFiles]->setFrameShadow(QFrame::Raised);

				m_pFrameFileTransfer[ButtonDatFiles]->setServerList(QGlobalInfo::instance()->serverList());
				m_pFrameFileTransfer[ButtonDatFiles]->setAddedServerList(m_addedServers);

				connect(m_pFrameFileTransfer[ButtonDatFiles], SIGNAL(addServer(const Server&)), this, SLOT(addServer(const Server&)));
				connect(m_pFrameFileTransfer[ButtonDatFiles], SIGNAL(removeServers(const QList<Server>&)), this, SLOT(removeServers(const QList<Server>&)));

				m_layout->addWidget(m_pFrameFileTransfer[ButtonDatFiles]);
			}
			m_pFrameFileTransfer[ButtonDatFiles]->show();
		}
		break;
	case ButtonIEC104Files:
		{
			if (m_pFrameFileTransfer[ButtonIEC104Files] == NULL)
			{
				m_pFrameFileTransfer[ButtonIEC104Files]=new QFrameFileTransferGroup("iec104", H9000_IEC104_PATH, this);

				m_pFrameFileTransfer[ButtonIEC104Files]->setTitleText(tr("Transfer H9000 IEC 104 files"));
				m_pFrameFileTransfer[ButtonIEC104Files]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonIEC104Files]->setThemePixmap(findPixmap(":/images/iec.png"));

				m_pFrameFileTransfer[ButtonIEC104Files]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonIEC104Files]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonIEC104Files]->setFrameShadow(QFrame::Raised);

				m_pFrameFileTransfer[ButtonIEC104Files]->setServerList(QGlobalInfo::instance()->serverList());
				m_pFrameFileTransfer[ButtonIEC104Files]->setAddedServerList(m_addedServers);

				connect(m_pFrameFileTransfer[ButtonIEC104Files], SIGNAL(addServer(const Server&)), this, SLOT(addServer(const Server&)));
				connect(m_pFrameFileTransfer[ButtonIEC104Files], SIGNAL(removeServers(const QList<Server>&)), this, SLOT(removeServers(const QList<Server>&)));

				m_layout->addWidget(m_pFrameFileTransfer[ButtonIEC104Files]);
			}
			m_pFrameFileTransfer[ButtonIEC104Files]->show();
		}
		break;
	case ButtonSoundFiles:
		{
			if (m_pFrameFileTransfer[ButtonSoundFiles] == NULL)
			{
				m_pFrameFileTransfer[ButtonSoundFiles]=new QFrameFileTransferGroup("voice", H9000_VOICE_PATH, this);

				m_pFrameFileTransfer[ButtonSoundFiles]->setTitleText(tr("Transfer H9000 sound files"));
				m_pFrameFileTransfer[ButtonSoundFiles]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonSoundFiles]->setThemePixmap(findPixmap(":/images/voice.png"));

				m_pFrameFileTransfer[ButtonSoundFiles]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonSoundFiles]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonSoundFiles]->setFrameShadow(QFrame::Raised);

				m_pFrameFileTransfer[ButtonSoundFiles]->setServerList(QGlobalInfo::instance()->serverList());
				m_pFrameFileTransfer[ButtonSoundFiles]->setAddedServerList(m_addedServers);

				connect(m_pFrameFileTransfer[ButtonSoundFiles], SIGNAL(addServer(const Server&)), this, SLOT(addServer(const Server&)));
				connect(m_pFrameFileTransfer[ButtonSoundFiles], SIGNAL(removeServers(const QList<Server>&)), this, SLOT(removeServers(const QList<Server>&)));

				m_layout->addWidget(m_pFrameFileTransfer[ButtonSoundFiles]);
			}
			m_pFrameFileTransfer[ButtonSoundFiles]->show();
		}
		break;
	case ButtonFileSystem:
		{
			if (m_pFrameFileTransfer[ButtonFileSystem] == NULL)
			{
				m_pFrameFileTransfer[ButtonFileSystem]=new QFrameFileTransferGroup("filesytem", "", this);

				m_pFrameFileTransfer[ButtonFileSystem]->setTitleText(tr("Transfer other kinds of files"));
				m_pFrameFileTransfer[ButtonFileSystem]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonFileSystem]->setThemePixmap(findPixmap(":/images/filesystem.png"));

				m_pFrameFileTransfer[ButtonFileSystem]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonFileSystem]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonFileSystem]->setFrameShadow(QFrame::Raised);

				m_pFrameFileTransfer[ButtonFileSystem]->setServerList(QGlobalInfo::instance()->serverList());
				m_pFrameFileTransfer[ButtonFileSystem]->setAddedServerList(m_addedServers);

				connect(m_pFrameFileTransfer[ButtonFileSystem], SIGNAL(addServer(const Server&)), this, SLOT(addServer(const Server&)));
				connect(m_pFrameFileTransfer[ButtonFileSystem], SIGNAL(removeServers(const QList<Server>&)), this, SLOT(removeServers(const QList<Server>&)));

				m_layout->addWidget(m_pFrameFileTransfer[ButtonFileSystem]);
			}
			m_pFrameFileTransfer[ButtonFileSystem]->show();
		}
		break;
	default:
		break;
	}
}
