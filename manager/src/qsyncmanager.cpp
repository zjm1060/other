#include "qsyncmanager.h"

#include "common.h"
#include "qmessageparser.h"

QSyncManager::QSyncManager(QClient* client, QWidget *parent)
	: QFrame(parent)
{
	memset(m_pFrameSync, 0, sizeof(QFrameSyncFile*)*ButtonEnd);
	m_pClient=client;

	m_layout=new QHBoxLayout(this);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("Configure File"), ButtonConfigureFile);
	m_pButtonList->addButton(tr("Picture"), ButtonPicture);
	m_pButtonList->addButton(tr("Database"), ButtonDatabase);
	m_pButtonList->addButton(tr("Interlock"), ButtonInterlock);
	m_pButtonList->addButton(tr("Calculate"), ButtonCalculate);
	m_pButtonList->addButton(tr("Voice"), ButtonVoice);
	m_pButtonList->addButton(tr("File System"), ButtonFileSystem);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonConfigureFile);
}

QSyncManager::~QSyncManager()
{

}

void QSyncManager::buttonClicked(int id)
{
	switch (id)
	{
	case ButtonConfigureFile:
		{
			if (m_pFrameSync[ButtonConfigureFile] == NULL)
			{
				m_pFrameSync[ButtonConfigureFile]=new QFrameSyncFile(m_pClient, H9000_DEF_PATH, this);

				m_pFrameSync[ButtonConfigureFile]->setTitleText(tr("Synchronize configure files"));
				m_pFrameSync[ButtonConfigureFile]->setDefaultDescText(tr("Select files you want to synchronize."));
				m_pFrameSync[ButtonConfigureFile]->setThemePixmap(QPixmap(":/images/config.png"));
				m_pFrameSync[ButtonConfigureFile]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_DEF_PATH));

				m_pFrameSync[ButtonConfigureFile]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameSync[ButtonConfigureFile]->setFrameShape(QFrame::Box);
				m_pFrameSync[ButtonConfigureFile]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSync[ButtonConfigureFile]);
			}
			m_pFrameSync[ButtonConfigureFile]->show();
		}
		break;
	case ButtonPicture:
		{
			if (m_pFrameSync[ButtonPicture] == NULL)
			{
				m_pFrameSync[ButtonPicture]=new QFrameSyncFile(m_pClient, H9000_DSP_PATH, this);

				m_pFrameSync[ButtonPicture]->setTitleText(tr("Synchronize picture files"));
				m_pFrameSync[ButtonPicture]->setDefaultDescText(tr("Select files you want to synchronize."));
				m_pFrameSync[ButtonPicture]->setThemePixmap(QPixmap(":/images/picture.png"));
				m_pFrameSync[ButtonPicture]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_DSP_PATH));

				m_pFrameSync[ButtonPicture]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameSync[ButtonPicture]->setFrameShape(QFrame::Box);
				m_pFrameSync[ButtonPicture]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSync[ButtonPicture]);
			}
			m_pFrameSync[ButtonPicture]->show();
		}
		break;
	case ButtonDatabase:
		{
			if (m_pFrameSync[ButtonDatabase] == NULL)
			{
				m_pFrameSync[ButtonDatabase]=new QFrameSyncFile(m_pClient, H9000_DATABASE_PATH, this);

				m_pFrameSync[ButtonDatabase]->setTitleText(tr("Synchronize database files"));
				m_pFrameSync[ButtonDatabase]->setDefaultDescText(tr("Select files you want to synchronize."));
				m_pFrameSync[ButtonDatabase]->setThemePixmap(QPixmap(":/images/dbfile.png"));
				m_pFrameSync[ButtonDatabase]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_DATABASE_PATH));

				m_pFrameSync[ButtonDatabase]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameSync[ButtonDatabase]->setFrameShape(QFrame::Box);
				m_pFrameSync[ButtonDatabase]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSync[ButtonDatabase]);
			}
			m_pFrameSync[ButtonDatabase]->show();
		}
		break;
	case ButtonInterlock:
		{
			if (m_pFrameSync[ButtonInterlock] == NULL)
			{
				m_pFrameSync[ButtonInterlock]=new QFrameSyncFile(m_pClient, H9000_INTERLOCK_PATH, this);

				m_pFrameSync[ButtonInterlock]->setTitleText(tr("Synchronize interlock files"));
				m_pFrameSync[ButtonInterlock]->setDefaultDescText(tr("Select files you want to synchronize."));
				m_pFrameSync[ButtonInterlock]->setThemePixmap(QPixmap(":/images/lock.png"));
				m_pFrameSync[ButtonInterlock]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_INTERLOCK_PATH));

				m_pFrameSync[ButtonInterlock]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameSync[ButtonInterlock]->setFrameShape(QFrame::Box);
				m_pFrameSync[ButtonInterlock]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSync[ButtonInterlock]);
			}
			m_pFrameSync[ButtonInterlock]->show();
		}
		break;
	case ButtonCalculate:
		{
			if (m_pFrameSync[ButtonCalculate] == NULL)
			{
				m_pFrameSync[ButtonCalculate]=new QFrameSyncFile(m_pClient, H9000_CALC_PATH, this);

				m_pFrameSync[ButtonCalculate]->setTitleText(tr("Synchronize calculate files"));
				m_pFrameSync[ButtonCalculate]->setDefaultDescText(tr("Select files you want to synchronize."));
				m_pFrameSync[ButtonCalculate]->setThemePixmap(QPixmap(":/images/calc.png"));
				m_pFrameSync[ButtonCalculate]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_CALC_PATH));

				m_pFrameSync[ButtonCalculate]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameSync[ButtonCalculate]->setFrameShape(QFrame::Box);
				m_pFrameSync[ButtonCalculate]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSync[ButtonCalculate]);
			}
			m_pFrameSync[ButtonCalculate]->show();
		}
		break;
	case ButtonVoice:
		{
			if (m_pFrameSync[ButtonVoice] == NULL)
			{
				m_pFrameSync[ButtonVoice]=new QFrameSyncFile(m_pClient, H9000_VOICE_PATH, this);

				m_pFrameSync[ButtonVoice]->setTitleText(tr("Synchronize voice files"));
				m_pFrameSync[ButtonVoice]->setDefaultDescText(tr("Select files you want to synchronize."));
				m_pFrameSync[ButtonVoice]->setThemePixmap(QPixmap(":/images/voice.png"));
				m_pFrameSync[ButtonVoice]->setDefaultLocalLocation(appendPath(getAppParentDir(), H9000_VOICE_PATH));

				m_pFrameSync[ButtonVoice]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameSync[ButtonVoice]->setFrameShape(QFrame::Box);
				m_pFrameSync[ButtonVoice]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSync[ButtonVoice]);
			}
			m_pFrameSync[ButtonVoice]->show();
		}
		break;
	case ButtonFileSystem:
		{
			if (m_pFrameSync[ButtonFileSystem] == NULL)
			{
				m_pFrameSync[ButtonFileSystem]=new QFrameSyncFile(m_pClient, "", this);

				m_pFrameSync[ButtonFileSystem]->setTitleText(tr("Synchronize any files"));
				m_pFrameSync[ButtonFileSystem]->setDefaultDescText(tr("Select files you want to synchronize."));
				m_pFrameSync[ButtonFileSystem]->setThemePixmap(QPixmap(":/images/filesystem.png"));
				m_pFrameSync[ButtonFileSystem]->setDefaultLocalLocation(getAppParentDir());

				m_pFrameSync[ButtonFileSystem]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameSync[ButtonFileSystem]->setFrameShape(QFrame::Box);
				m_pFrameSync[ButtonFileSystem]->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSync[ButtonFileSystem]);
			}
			m_pFrameSync[ButtonFileSystem]->show();
		}
		break;
	default:
		break;
	}

	for (int i=0; i<ButtonEnd; i++)
	{
		if (i == id)
		{
			continue;
		}

		SAFE_HIDE_WIDGET(m_pFrameSync[i]);
	}
}
