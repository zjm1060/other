#include "qdlgnewtask.h"

#include <QPixmap>
#include <QStringList>
#include <QDataStream>
#include <QDir>

#include "common.h"
#include "qmessageparser.h"
#include "qmessagefiletransfer.h"
#include "qfiletransferparser.h"

QDlgNewTask::QDlgNewTask(QClient* client, QWidget *parent)
	: QDialog(parent),
	m_bQueryComputer(false)
{
	m_pClient=client;

	m_nUsrID=QFileTransferParser::instance()->registerInterface(this);

	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	labelIcon->setPixmap(QPixmap(":/images/process.png"));
	labelInfoIcon->setPixmap(QPixmap(":/images/error.png"));
	labelInfoIcon->hide();

	connect(lineEditNewTask, SIGNAL(textChanged(const QString&)), this, SLOT(textChangedNewTask(const QString&)));

	toolButtonBrowser->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(toolButtonBrowser, SIGNAL(toggled(bool)), this, SLOT(onToggledExpandButton(bool)));

	bool bExpand=parseConfig(getAppParentDir()+INI_FILE, "newtask/expand", false).toBool();
	if (bExpand)
	{
		toolButtonBrowser->toggle();
	}
	else
	{
		onToggledExpandButton(bExpand);
	}

	m_pFrameFileSystem=new QFrameFileSystem(this);
	m_pFrameFileSystem->treeViewFileSystem()->setEditable(false);
	verticalLayout->addWidget(m_pFrameFileSystem);

	connect(m_pFrameFileSystem, SIGNAL(clickedToolButton(int)), this, SLOT(clickedToolButton(int)));
	connect(m_pFrameFileSystem, SIGNAL(currentIndexChanged(QString)),
		this, SLOT(locationCurrentIndexChanged(QString)));

	connect(m_pFrameFileSystem->comboBoxFileSystem(), SIGNAL(editTextChanged(const QString&)),
		this, SLOT(editLocationTextChanged(const QString&)));
	connect(m_pFrameFileSystem->comboBoxFileSystem(), SIGNAL(enterPressed(QString)),
		this, SLOT(enterPressed(QString)));
	connect(m_pFrameFileSystem->comboBoxFileSystem(), SIGNAL(focusStatusChanged(bool)),
		this, SLOT(focusStatusChanged(bool)));
	connect(m_pFrameFileSystem->treeViewFileSystem(), SIGNAL(doubleClicked(const QModelIndex &)), 
		this, SLOT(doubleClickedRemoteLocation(const QModelIndex&)));

 	lineEditNewTask->installEventFilter(this);
}

QDlgNewTask::~QDlgNewTask()
{
	lineEditNewTask->removeEventFilter(this);

	codeConfig(getAppParentDir()+INI_FILE, "newtask/expand", toolButtonBrowser->isChecked());

	QFileTransferParser::instance()->unregisterInterface(m_nUsrID);
}

QString QDlgNewTask::text()
{
	return lineEditNewTask->text();
}

void QDlgNewTask::queryPath(const QString& strAddr)
{
	if (!strAddr.isEmpty())
	{
		QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
		if (pMsg != NULL)
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			int nSortColumn=m_pFrameFileSystem->treeViewFileSystem()->sortColumn();
			int nSort=0;
			switch (nSortColumn)
			{
			case 0:
				nSort=QMessageFileTransfer::SortName;
				break;
			case 1:
				nSort=QMessageFileTransfer::SortSize;
				break;
			case 2:
				nSort=QMessageFileTransfer::SortType;
				break;
			case 3:
				nSort=QMessageFileTransfer::SortTime;
				break;
			default:
				break;
			}
			stream<<strAddr<<nSort;

			buffer=pMsg->query(QMessageFileTransfer::QueryPath, m_nUsrID, buffer);

			m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		}
	}
}

void QDlgNewTask::queryRoot()
{
	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageFileTransfer::QueryRoot, m_nUsrID);

		m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
	}
}

void QDlgNewTask::queryDrives()
{
	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageFileTransfer::QueryDrive, m_nUsrID);
		m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
	}
}

void QDlgNewTask::onToggledExpandButton(bool bExpand)
{
	groupBoxBrowser->setVisible(bExpand);

	if (bExpand)
	{
		toolButtonBrowser->setIcon(QIcon(":/images/contract.png"));

		queryDrives();
	}
	else
	{
		toolButtonBrowser->setIcon(QIcon(":/images/expand.png"));
	}
}

bool QDlgNewTask::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == lineEditNewTask)
	{
		if (event->type() == QEvent::FocusIn)
		{
			textChangedNewTask(lineEditNewTask->text());

			pushButtonOk->setDefault(true);
		}
	}
	
	return false;
}

void QDlgNewTask::textChangedNewTask(const QString& text)
{
	if (!text.isEmpty())
	{
		QChar ch;
		if (!hasInvalidChar(text, &ch))
		{
			labelInfoIcon->hide();
			labelInfo->setText(tr("Enter your task name."));
			pushButtonOk->setEnabled(true);
		}
		else
		{
			labelInfoIcon->show();
			labelInfo->setText(QString(ch)+" "+tr("is an invalid character in your task name."));
			pushButtonOk->setEnabled(false);
		}
	}
	else
	{
		labelInfoIcon->hide();
		labelInfo->setText(tr("Enter your task name."));
		pushButtonOk->setEnabled(false);
	}
}

void QDlgNewTask::editLocationTextChanged(const QString& location)
{
	QString strLocation=QDir::fromNativeSeparators(location);

	if (!location.isEmpty() && m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
	{
		QChar ch;
		if (!hasInvalidChar(location, &ch))
		{
			labelInfoIcon->hide();
			labelInfo->setText(tr("Enter your target file folder."));
		}
		else
		{
			labelInfoIcon->show();
			labelInfo->setText(QString(ch)+" "+tr("is an invalid character in your address."));
		}
	}
	else
	{
		labelInfoIcon->hide();
		labelInfo->setText(tr("Enter your target file folder."));
	}
}

void QDlgNewTask::recvRoot(const QString& root)
{
	m_strRoot=root;

	m_pFrameFileSystem->setLocation(root, false);

	setLocation(root);
}

void QDlgNewTask::setLocation(const QString& location)
{
	if (location.isEmpty())
	{
		return;
	}

	gotoRemoteLocation(location);
}

void QDlgNewTask::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	if (flag == QMessageFileTransfer::QueryPathValid)
	{
		m_pFrameFileSystem->setLocation(path, false);
		m_pFrameFileSystem->setFileInfo(files);

		m_pFrameFileSystem->comboBoxFileSystem()->setFocus();
	}
	else if (flag == QMessageFileTransfer::QueryPathNoExist)
	{
		QString strLocation=m_pFrameFileSystem->location();

		if (strLocation != path)
		{
			queryPath(strLocation);
		}
		else
		{
			if (strLocation != m_strRoot)
			{
				queryPath(m_strRoot);
			}
			else
			{
				labelInfoIcon->show();
				labelInfo->setText(tr("Try to open an non-existent path."));
			}
		}
	}
}

void QDlgNewTask::recvDrive(const QVector<FileInfo>& drives)
{
	if (m_bQueryComputer)
	{
		m_bQueryComputer=false;

		m_pFrameFileSystem->setFileInfo(drives, QComboBoxFileSystem::Computer);
		QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->itemText(QComboBoxFileSystem::Computer);
		m_pFrameFileSystem->setLocation(strLocation, false);
	}
	else
	{
		m_pFrameFileSystem->setDrives(drives);

		queryRoot();
	}
}

void QDlgNewTask::recvSymLink(int flag, FileInfo info, const QString& symLink)
{
	switch (flag)
	{
	case QMessageFileTransfer::QueryFileNoExist:
		break;
	case QMessageFileTransfer::QueryFileUnreadable:
		break;
	case QMessageFileTransfer::QuerySymLinkNoLink:
		break;
	case QMessageFileTransfer::QueryFileValid:
		{
			if ((info.type & QMessageFileTransfer::File) != 0 || (info.type & QMessageFileTransfer::HideFile) != 0)
			{
				lineEditNewTask->setText(symLink);
			}
			else
			{
				setLocation(symLink);
			}
		}
		break;
	default:
		qDebug("Receive symbol link error!");
		break;
	}
}

void QDlgNewTask::doubleClickedRemoteLocation(const QModelIndex& index)
{
	if (!index.isValid())
	{
		return;
	}

	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
	QString strFileName=pTreeViewFileSystem->filename(index);
	QString strLocation;

	if (strFileName == "..")
	{
		strLocation=getParentDir(m_pFrameFileSystem->location());
	}
	else
	{
		int nType=pTreeViewFileSystem->type(index);

		if (nType == QMessageFileTransfer::File || nType == QMessageFileTransfer::HideFile)
		{
			lineEditNewTask->setText(appendPath(m_pFrameFileSystem->location(), strFileName));
			return;
		}
		else if (nType == QMessageFileTransfer::SymLinkFile || nType == QMessageFileTransfer::SymLinkHideFile)
		{
			querySymLink(appendPath(m_pFrameFileSystem->location(), strFileName));
			return;
		}

		if (nType == QMessageFileTransfer::Dir || nType == QMessageFileTransfer::HideDir
			|| nType == QMessageFileTransfer::SymLinkDir || nType == QMessageFileTransfer::SymLinkHideDir)
		{
			strLocation=appendPath(m_pFrameFileSystem->location(), strFileName);
		}
		else if ((nType & QMessageFileTransfer::Drive) != 0)
		{
			strLocation=strFileName;
		}
	}

	if (!strLocation.isEmpty())
	{
		setLocation(strLocation);
	}
}

void QDlgNewTask::querySymLink(const QString& symLink)
{
	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<symLink;

		buffer=pMsg->query(QMessageFileTransfer::QuerySymLink, m_nUsrID, buffer);

		m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
	}
}

void QDlgNewTask::clickedToolButton(int type)
{
	switch (type)
	{
	case QFrameFileSystem::ButtonHome:
		{
			gotoRemoteHome();
		}
		break;
	case QFrameFileSystem::ButtonUp:
		{
			QString strLocation=m_pFrameFileSystem->location();
			if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) != -1)
			{
				break;
			}

			strLocation=m_pFrameFileSystem->comboBoxFileSystem()->parentLocation(strLocation);
			gotoRemoteLocation(strLocation);
		}
		break;
	default:
		break;
	}
}

void QDlgNewTask::gotoRemoteHome()
{
	m_pFrameFileSystem->clear();

	queryDrives();
}

void QDlgNewTask::gotoRemoteLocation(const QString& location)
{

	int itemType=m_pFrameFileSystem->comboBoxFileSystem()->topItemType(location);
	switch (itemType)
	{
	case QComboBoxFileSystem::OtherItem:
		{
			queryPath(location);
		}
		break;
	case QComboBoxFileSystem::Computer:
		{
			m_bQueryComputer=true;
			queryDrives();
		}
		break;
	default:
		break;
	}
}

void QDlgNewTask::locationCurrentIndexChanged(QString location)
{
	setLocation(location);
}

void QDlgNewTask::enterPressed(QString location)
{
	setLocation(location);
}

void QDlgNewTask::focusStatusChanged(bool has)
{
	if (has)
	{
		editLocationTextChanged(m_pFrameFileSystem->comboBoxFileSystem()->lineEdit()->text());

		pushButtonOk->setDefault(false);
	}
}
