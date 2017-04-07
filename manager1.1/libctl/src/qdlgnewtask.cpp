#include "qdlgnewtask.h"

#include <QPixmap>
#include <QStringList>
#include <QDataStream>
#include <QDir>

#include "ui_newtask.h"
#include "common.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"

QDlgNewTask::QDlgNewTask(QClient* client, QWidget *parent)
	: QDialog(parent)
	,m_bQueryComputer(false)
	,m_bQueryHomePath(true)
{
	m_pClient=client;

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_ui=new Ui_DialogNewTask;
	m_ui->setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	m_ui->labelIcon->setPixmap(findPixmap(":/images/process.png"));
	m_ui->labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
	m_ui->labelInfoIcon->hide();
	
	m_ui->pushButtonOk->setEnabled(false);

	connect(m_ui->lineEditNewTask, SIGNAL(textChanged(const QString&)), this, SLOT(textChangedNewTask(const QString&)));

	m_ui->toolButtonBrowser->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(m_ui->toolButtonBrowser, SIGNAL(toggled(bool)), this, SLOT(onToggledExpandButton(bool)));

	m_pFrameFileSystem=new QFrameFileSystem(this);
	m_pFrameFileSystem->treeViewFileSystem()->setEditable(false);
	m_ui->verticalLayout->addWidget(m_pFrameFileSystem);

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
	connect(m_pFrameFileSystem->treeViewFileSystem(), SIGNAL(doubleClicked(const QModelIndex &)), 
		this, SLOT(doubleClickedLocalLocation(const QModelIndex&)));
	connect(m_pFrameFileSystem->treeViewFileSystem(), SIGNAL(backspaceKeyPress()), 
		this, SLOT(backspaceKeyPress()));
	connect(m_pFrameFileSystem->treeViewFileSystem(), SIGNAL(refreshKeyPress()), 
		this, SLOT(refreshKeyPress()));

 	m_ui->lineEditNewTask->installEventFilter(this);

	bool bExpand=parseConfig(getAppParentDir()+INI_FILE, "newtask/expand", false).toBool();
	if (bExpand)
	{
		m_ui->toolButtonBrowser->toggle();
	}
	else
	{
		onToggledExpandButton(bExpand);
	}
}

QDlgNewTask::~QDlgNewTask()
{
	m_ui->lineEditNewTask->removeEventFilter(this);

	codeConfig(getAppParentDir()+INI_FILE, "newtask/expand", m_ui->toolButtonBrowser->isChecked());

	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	SAFE_DELETE_CLASS(m_ui);
}

QStringList QDlgNewTask::task()
{
	return parseCommand(m_ui->lineEditNewTask->text());
}

void QDlgNewTask::queryPath(const QString& path)
{
	if (path.isEmpty())
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		int nSortColumn=m_pFrameFileSystem->treeViewFileSystem()->sortColumn();
		int nSort=SortName;
		switch (nSortColumn)
		{
		case QTreeViewFileSystem::NameTitle:
			nSort=SortName;
			break;
		case QTreeViewFileSystem::SizeTitle:
			nSort=SortSize;
			break;
		case QTreeViewFileSystem::TypeTitle:
			nSort=SortType;
			break;
		case QTreeViewFileSystem::ModifiedTitle:
			nSort=SortTime;
			break;
		default:
			break;
		}
		stream<<path<<nSort<<QStringList()<<QDir::NoFilter;

		buffer=pMsg->query(QueryPath, m_nUsrID, buffer);

		m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QDlgNewTask::queryRoot()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRoot, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QDlgNewTask::queryDrives()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryDrive, m_nUsrID);
		m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QDlgNewTask::onToggledExpandButton(bool bExpand)
{
	m_ui->groupBoxBrowser->setVisible(bExpand);

	if (bExpand)
	{
		m_ui->toolButtonBrowser->setIcon(QIcon(":/images/contract.png"));

		gotoRemoteHome();
	}
	else
	{
		m_ui->toolButtonBrowser->setIcon(QIcon(":/images/expand.png"));
	}
}

bool QDlgNewTask::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == m_ui->lineEditNewTask)
	{
		if (event->type() == QEvent::FocusIn)
		{
			textChangedNewTask(m_ui->lineEditNewTask->text());

			m_ui->pushButtonOk->setDefault(true);
		}
	}
	
	return false;
}

QStringList QDlgNewTask::parseCommand(const QString& text) const
{
	QStringList arguments;

	int nLeft=0, nRight=0;
	bool bQuot=false;
	while (nRight < text.size())
	{
		if (!bQuot && (text[nRight] == QChar(' ') || text[nRight] == QChar('\t')))
		{
			if (nLeft < nRight)
			{
				QString argument=text.mid(nLeft, nRight-nLeft);
				arguments<<text.mid(nLeft, nRight-nLeft);
			}

			nLeft=nRight+1;
		}

		if (text[nRight] == QChar('\"'))
		{
			if (nLeft < nRight)
			{
				QString argument=text.mid(nLeft, nRight-nLeft);
				arguments<<text.mid(nLeft, nRight-nLeft);
			}

			nLeft=nRight+1;
			bQuot=!bQuot;
		}

		nRight++;
	}

	if (nLeft < nRight)
	{
		QString argument=text.mid(nLeft, nRight-nLeft);
		arguments<<text.mid(nLeft, nRight-nLeft);
	}

	return arguments;
}

void QDlgNewTask::textChangedNewTask(const QString& text)
{
	QString strText=text.trimmed();
	
	if (!strText.isEmpty() && !isUrl(strText))
	{
		const QStringList list=parseCommand(strText);

		foreach (QString argument, list)
		{
			QChar ch;
			if (hasInvalidChar(argument, &ch))
			{
				m_ui->labelInfoIcon->show();
				m_ui->labelInfo->setText(QString(ch)+" "+tr("is an invalid character in your task name."));
				m_ui->pushButtonOk->setEnabled(false);

				return;
			}
		}
	}
	
	m_ui->labelInfoIcon->hide();
	m_ui->labelInfo->setText(tr("Enter your task."));
	m_ui->pushButtonOk->setEnabled(!strText.isEmpty());
}

void QDlgNewTask::editLocationTextChanged(const QString& location)
{
	QString strLocation=QDir::fromNativeSeparators(location);

	if (!location.isEmpty() && m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
	{
		QChar ch;
		if (!hasInvalidChar(location, &ch))
		{
			m_ui->labelInfoIcon->hide();
			m_ui->labelInfo->setText(tr("Enter your target file folder."));
		}
		else
		{
			m_ui->labelInfoIcon->show();
			m_ui->labelInfo->setText(QString(ch)+" "+tr("is an invalid character in your address."));
		}
	}
	else
	{
		m_ui->labelInfoIcon->hide();
		m_ui->labelInfo->setText(tr("Enter your target file folder."));
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
	if (flag == QueryPathValid)
	{
		if (m_bQueryHomePath)
		{
			m_bQueryHomePath=false;
		}

		m_pFrameFileSystem->setLocation(path, false);
		m_pFrameFileSystem->setFileInfo(files);
	}
	else if (flag == QueryPathNoExist)
	{
		if (m_bQueryHomePath)
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
					m_ui->labelInfoIcon->show();
					m_ui->labelInfo->setText(tr("Try to open an non-existent path."));
				}
			}
		}
	}
}

void QDlgNewTask::recvDrives(const QVector<FileInfo>& drives)
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

void QDlgNewTask::recvSymLink(int flag, const FileInfo& info, const QString& symLink)
{
	switch (flag)
	{
	case QueryFileNoExist:
		break;
	case QueryFileUnreadable:
		break;
	case QueryFileNoSymLink:
		break;
	case QueryFileValid:
		{
			if ((info.type & File) != 0 || (info.type & HideFile) != 0)
			{
				m_ui->lineEditNewTask->setText(symLink);
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

		if (nType == File || nType == HideFile || nType == SymLinkFile || nType == SymLinkHideFile)
		{	
			m_ui->lineEditNewTask->setText("\""+appendPath(m_pFrameFileSystem->location(), strFileName)+"\"");
			return;
		}

		if (nType == Dir || nType == HideDir || nType == SymLinkDir || nType == SymLinkHideDir)
		{
			strLocation=appendPath(m_pFrameFileSystem->location(), strFileName);
		}
		else if ((nType & Drive) != 0)
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
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<symLink;

		buffer=pMsg->query(QuerySymLink, m_nUsrID, buffer);

		m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
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
	m_bQueryHomePath=true;

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

		m_ui->pushButtonOk->setDefault(false);
	}
}

void QDlgNewTask::backspaceKeyPress()
{
	clickedToolButton(QFrameFileSystem::ButtonUp);
}

void QDlgNewTask::refreshKeyPress()
{
	locationCurrentIndexChanged(m_pFrameFileSystem->comboBoxFileSystem()->currentLocation());
}
