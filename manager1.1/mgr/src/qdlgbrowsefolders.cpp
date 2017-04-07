#include "qdlgbrowsefolders.h"

#include <QPixmap>
#include <QStringList>
#include <QDataStream>
#include <QDir>

#include "common.h"
#include "qmessageparser.h"

QDlgBrowseFolders::QDlgBrowseFolders(const ServerInfo& info, const QString& initPath, const QString& subPath, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	labelIcon->setPixmap(findPixmap(":/images/folder.png"));
	labelDescIcon->hide();

	m_pixmap[QFrameRemoteFileSystem::Warning]=findPixmap(":/images/warning.png");
	m_pixmap[QFrameRemoteFileSystem::Error]=findPixmap(":/images/error.png");

	m_pFrameFileSystem=new QFrameRemoteFileSystem(initPath, subPath, this);
	verticalLayout->addWidget(m_pFrameFileSystem);

	connect(m_pFrameFileSystem, SIGNAL(enterInvalidChar(bool, const QChar&)), this, SLOT(enterInvalidChar(bool, const QChar&)));
	connect(m_pFrameFileSystem, SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPress(QPoint)));
	connect(m_pFrameFileSystem, SIGNAL(focusStatusChanged(bool)), this, SLOT(focusStatusChanged(bool)));
	connect(m_pFrameFileSystem, SIGNAL(tip(int, const QString&)), this, SLOT(tip(int, const QString&)));

	//弹出菜单
	m_pMenu=new QMenu(this);
	m_pActions[ActionOpen]=new QAction(tr("&Open"), m_pMenu);
	m_pActions[ActionDelete]=new QAction(QIcon(":/images/delete.png"), tr("&Delete"), m_pMenu);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("&Refresh"), m_pMenu);
	m_pActions[ActionNewDirectory]=new QAction(QIcon(":/images/newdirectory.png"), tr("&New Directory"), m_pMenu);
	m_pActions[ActionRename]=new QAction(tr("Rename"), m_pMenu);

	m_pMenu->addAction(m_pActions[ActionOpen]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionDelete]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionNewDirectory]);
	m_pMenu->addAction(m_pActions[ActionRename]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionRefresh]);

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));

	m_pFrameFileSystem->setClient(info);
}

QDlgBrowseFolders::~QDlgBrowseFolders()
{
}

QString QDlgBrowseFolders::text()
{
	QString strText;
	if (m_pFrameFileSystem != NULL)
	{
		strText=m_pFrameFileSystem->currentLocationText();
	}

	return strText;
}

QString QDlgBrowseFolders::folderPath()
{
	QString strFolder=text();

	if (m_pFrameFileSystem != NULL)
	{
		QVector<FileInfo> items=m_pFrameFileSystem->selectedItems();
		for (int i=0; i<items.size(); i++)
		{
			const FileInfo& item=items[i];
			if (isDir(item.type))
			{
				strFolder=appendPath(strFolder, item.name);
			}
		}
	}

	return strFolder;
}

void QDlgBrowseFolders::setFilters(const QStringList& nameFilters, QDir::Filters filters)
{
	if (m_pFrameFileSystem != NULL)
	{
		m_pFrameFileSystem->setFilters(nameFilters, filters);
	}
}

void QDlgBrowseFolders::enterInvalidChar(bool invalid, const QChar& ch)
{
	if (invalid)
	{
		tip(QFrameRemoteFileSystem::Error, QString(ch)+" "+tr("is an invalid character in your remote location."));
	}
	else
	{
		tip(QFrameRemoteFileSystem::Information, tr("Enter your target folder path."));

		pushButtonOk->setEnabled(m_pFrameFileSystem->canNewDir());
	}
}

void QDlgBrowseFolders::mouseRightButtonPress(QPoint pos)
{
	m_pActions[ActionOpen]->setEnabled(m_pFrameFileSystem->canOpen());
	m_pActions[ActionDelete]->setEnabled(m_pFrameFileSystem->canDelete());
	m_pActions[ActionRename]->setEnabled(m_pFrameFileSystem->canRename());
	m_pActions[ActionNewDirectory]->setEnabled(m_pFrameFileSystem->canNewDir());

	m_pMenu->popup(pos);
}

void QDlgBrowseFolders::focusStatusChanged(bool has)
{
	if (has)
	{
		m_pFrameFileSystem->editTextChangedLocation(m_pFrameFileSystem->locationText());
	}
}

void QDlgBrowseFolders::actionTriggered(int index)
{
	switch (index)
	{
	case ActionOpen:
		{
			m_pFrameFileSystem->doubleClickedLocation();
		}
		break;
	case ActionDelete:
		{
			m_pFrameFileSystem->delDirectory();
		}
		break;
	case ActionRefresh:
		{
			m_pFrameFileSystem->refresh();
		}
		break;
	case ActionNewDirectory:
		{
			m_pFrameFileSystem->newDirectory();
		}
		break;
	case ActionRename:
		{
 			m_pFrameFileSystem->rename();
		}
		break;
	default:
		break;
	}
}

void QDlgBrowseFolders::tip(int type, const QString& tip)
{
	if (type == QFrameRemoteFileSystem::Information)
	{
		labelDescIcon->hide();
	}
	else
	{
		labelDescIcon->setPixmap(m_pixmap[type]);
		labelDescIcon->show();
	}

	labelDesc->setText(tip);
}
