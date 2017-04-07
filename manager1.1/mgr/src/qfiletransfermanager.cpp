#include "qfiletransfermanager.h"

#include "common.h"
#include "qmessageparser.h"

QFileTransferManager::QFileTransferManager(QClient* client, QWidget *parent)
	: QFrame(parent)
{
	memset(m_pFrameFileTransfer, 0, sizeof(QFrameTransferFile*)*ButtonEnd);
	m_pClient=client;

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("H9000 Config Files"), ButtonConfigFiles);
	m_pButtonList->addButton(tr("H9000 Pictures"), ButtonPictures);
	m_pButtonList->addButton(tr("H9000 DB Define files"), ButtonDBDefine);
	m_pButtonList->addButton(tr("H9000 Data Files"), ButtonDatFiles);
	m_pButtonList->addButton(tr("H9000 IEC104 Files"), ButtonIEC104Files);
	m_pButtonList->addButton(tr("H9000 Sound Files"), ButtonSoundFiles);
	m_pButtonList->addButton(tr("File System"), ButtonFileSystem);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonConfigFiles);
}

QFileTransferManager::~QFileTransferManager()
{

}

void QFileTransferManager::buttonClicked(int id)
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
				m_pFrameFileTransfer[ButtonConfigFiles]=new QFrameTransferFile(m_pClient, H9000_DEF_PATH, this);

				m_pFrameFileTransfer[ButtonConfigFiles]->setTitleText(tr("Transfer H9000 config files"));
				m_pFrameFileTransfer[ButtonConfigFiles]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonConfigFiles]->setThemePixmap(findPixmap(":/images/config.png"));
				m_pFrameFileTransfer[ButtonConfigFiles]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_DEF_PATH));

				m_pFrameFileTransfer[ButtonConfigFiles]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonConfigFiles]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonConfigFiles]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameFileTransfer[ButtonConfigFiles]);
			}
			m_pFrameFileTransfer[ButtonConfigFiles]->show();
		}
		break;
	case ButtonPictures:
		{
			if (m_pFrameFileTransfer[ButtonPictures] == NULL)
			{
				m_pFrameFileTransfer[ButtonPictures]=new QFrameTransferFile(m_pClient, H9000_DSP_PATH, this);

				m_pFrameFileTransfer[ButtonPictures]->setTitleText(tr("Transfer H9000 picture files"));
				m_pFrameFileTransfer[ButtonPictures]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonPictures]->setThemePixmap(findPixmap(":/images/picture.png"));
				m_pFrameFileTransfer[ButtonPictures]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_DSP_PATH));

				m_pFrameFileTransfer[ButtonPictures]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonPictures]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonPictures]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameFileTransfer[ButtonPictures]);
			}
			m_pFrameFileTransfer[ButtonPictures]->show();
		}
		break;
	case ButtonDBDefine:
		{
			if (m_pFrameFileTransfer[ButtonDBDefine] == NULL)
			{
				m_pFrameFileTransfer[ButtonDBDefine]=new QFrameTransferFile(m_pClient, H9000_DATABASE_PATH, this);

				m_pFrameFileTransfer[ButtonDBDefine]->setTitleText(tr("Transfer H9000 database define files"));
				m_pFrameFileTransfer[ButtonDBDefine]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonDBDefine]->setThemePixmap(findPixmap(":/images/dbfile.png"));
				m_pFrameFileTransfer[ButtonDBDefine]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_DATABASE_PATH));

				m_pFrameFileTransfer[ButtonDBDefine]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonDBDefine]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonDBDefine]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameFileTransfer[ButtonDBDefine]);
			}
			m_pFrameFileTransfer[ButtonDBDefine]->show();
		}
		break;
	case ButtonDatFiles:
		{
			if (m_pFrameFileTransfer[ButtonDatFiles] == NULL)
			{
				m_pFrameFileTransfer[ButtonDatFiles]=new QFrameTransferFile(m_pClient, H9000_DAT_PATH, this);

				m_pFrameFileTransfer[ButtonDatFiles]->setTitleText(tr("Transfer H9000 data files"));
				m_pFrameFileTransfer[ButtonDatFiles]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonDatFiles]->setThemePixmap(findPixmap(":/images/dat.png"));
				m_pFrameFileTransfer[ButtonDatFiles]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_DAT_PATH));

				m_pFrameFileTransfer[ButtonDatFiles]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonDatFiles]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonDatFiles]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameFileTransfer[ButtonDatFiles]);
			}
			m_pFrameFileTransfer[ButtonDatFiles]->show();
		}
		break;
	case ButtonIEC104Files:
		{
			if (m_pFrameFileTransfer[ButtonIEC104Files] == NULL)
			{
				m_pFrameFileTransfer[ButtonIEC104Files]=new QFrameTransferFile(m_pClient, H9000_IEC104_PATH, this);

				m_pFrameFileTransfer[ButtonIEC104Files]->setTitleText(tr("Transfer H9000 IEC 104 files"));
				m_pFrameFileTransfer[ButtonIEC104Files]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonIEC104Files]->setThemePixmap(findPixmap(":/images/iec.png"));
				m_pFrameFileTransfer[ButtonIEC104Files]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_IEC104_PATH));

				m_pFrameFileTransfer[ButtonIEC104Files]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonIEC104Files]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonIEC104Files]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameFileTransfer[ButtonIEC104Files]);
			}
			m_pFrameFileTransfer[ButtonIEC104Files]->show();
		}
		break;
	case ButtonSoundFiles:
		{
			if (m_pFrameFileTransfer[ButtonSoundFiles] == NULL)
			{
				m_pFrameFileTransfer[ButtonSoundFiles]=new QFrameTransferFile(m_pClient, H9000_VOICE_PATH, this);

				m_pFrameFileTransfer[ButtonSoundFiles]->setTitleText(tr("Transfer H9000 sound files"));
				m_pFrameFileTransfer[ButtonSoundFiles]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonSoundFiles]->setThemePixmap(findPixmap(":/images/voice.png"));
				m_pFrameFileTransfer[ButtonSoundFiles]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_VOICE_PATH));

				m_pFrameFileTransfer[ButtonSoundFiles]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonSoundFiles]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonSoundFiles]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameFileTransfer[ButtonSoundFiles]);
			}
			m_pFrameFileTransfer[ButtonSoundFiles]->show();
		}
		break;
	case ButtonFileSystem:
		{
			if (m_pFrameFileTransfer[ButtonFileSystem] == NULL)
			{
				m_pFrameFileTransfer[ButtonFileSystem]=new QFrameTransferFile(m_pClient, "", this);

				m_pFrameFileTransfer[ButtonFileSystem]->setTitleText(tr("Transfer other kinds of files"));
				m_pFrameFileTransfer[ButtonFileSystem]->setDefaultDescText(tr("Select files you want to commit."));
				m_pFrameFileTransfer[ButtonFileSystem]->setThemePixmap(findPixmap(":/images/filesystem.png"));
				m_pFrameFileTransfer[ButtonFileSystem]->setDefaultLocalLocation(getAppParentDir());

				m_pFrameFileTransfer[ButtonFileSystem]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameFileTransfer[ButtonFileSystem]->setFrameShape(QFrame::Box);
				m_pFrameFileTransfer[ButtonFileSystem]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameFileTransfer[ButtonFileSystem]);
			}
			m_pFrameFileTransfer[ButtonFileSystem]->show();
		}
		break;
	default:
		break;
	}
}
