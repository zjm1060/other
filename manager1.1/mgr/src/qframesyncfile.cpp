#include "qframesyncfile.h"

#include <QDir>
#include <QMessageBox>

#include "define.h"
#include "common.h"
#include "qutildef.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qglobalinfo.h"

#define MAX_TICK_NUMBER		20

QFrameSyncFile::QFrameSyncFile(QClient* client, QWidget *parent)
	: QFrame(parent)
	,m_nDynamicID(QReceiverParser::AllUser)
	,m_nRow(-1)
	,m_bStatusTransfer(false)
{
	qRegisterMetaType<QList<QString> >("QList<QString>");
	qRegisterMetaType<QList<DiffFile> >("QList<QTreeViewSyncFile::DiffFile>");

	setupUi(this);

	m_pTreeView=new QTreeViewSyncFile(this);
	verticalLayoutSyncFile->addWidget(m_pTreeView);

	connect(m_pTreeView, SIGNAL(refreshModelFinished()), this, SLOT(refreshModelFinished()));

	connect(comboBoxCommonFolder, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));

	connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(onClickedBtnOk()));
	connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(onClickedBtnCancel()));
	connect(pushButtonRefresh, SIGNAL(clicked()), this, SLOT(onClickedBtnRefresh()));
	connect(pushButtonSelectAll, SIGNAL(clicked()), this, SLOT(onClickedBtnAll()));
	connect(pushButtonInverse, SIGNAL(clicked()), this, SLOT(onClickedBtnInverse()));

	labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
	showTipsPixmap(false);

	m_pClient=client;
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_nTick=0;
	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	refreshFileSystem();
}

QFrameSyncFile::~QFrameSyncFile()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
	QReceiverParser::instance()->unregisterInterface(m_nDynamicID);
}

void QFrameSyncFile::setTitleText(const QString& title)
{
	labelTitle->setText(title);
}

void QFrameSyncFile::setDefaultDescText(const QString& desc)
{
	m_strDefaultDesc=desc;

	labelDesc->setText(desc);
}

void QFrameSyncFile::setThemePixmap(const QPixmap& icon)
{
	labelIcon->setPixmap(icon);
}

void QFrameSyncFile::showTipsPixmap(bool show)
{
	if (show)
	{
		labelInfoIcon->show();
	}
	else
	{
		labelInfoIcon->hide();
	}
}

void QFrameSyncFile::refreshFileSystem()
{
	pushButtonCancel->setEnabled(false);
	pushButtonOK->setEnabled(false);
	pushButtonRefresh->setEnabled(false);
	pushButtonSelectAll->setEnabled(false);
	pushButtonInverse->setEnabled(false);

	QSyncFileReadCommonFolderRunnable* pRunnable=new QSyncFileReadCommonFolderRunnable(appendPath(getAppParentDir(), 
		QString("/ini/data/localhost/%1/database.xml").arg(m_pClient->serverAddr().toString()).arg(getHostName())));
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QList<QString>)), this, SLOT(readCommonFolderFinished(QList<QString>)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QFrameSyncFile::readCommonFolderFinished(QList<QString> folders)
{
	comboBoxCommonFolder->clear();

	if (!folders.isEmpty())
	{
		foreach (QString folder, folders)
		{
			comboBoxCommonFolder->addItem(fileIcon(Dir), folder);
		}
	}
	else
	{
		comboBoxCommonFolder->addItem(fileIcon(Dir), appendPath(getAppParentDir(), H9000_DATABASE_PATH));
	}
}

void QFrameSyncFile::currentIndexChanged(int index)
{
	if (index < 0 || index >= comboBoxCommonFolder->count())
	{
		return;
	}

	pushButtonCancel->setEnabled(false);
	pushButtonOK->setEnabled(false);
	pushButtonRefresh->setEnabled(false);
	pushButtonSelectAll->setEnabled(false);
	pushButtonInverse->setEnabled(false);
	comboBoxCommonFolder->setEnabled(false);

	QString strAttr=comboBoxCommonFolder->currentText();
	if (m_pTreeView != NULL)
	{
		m_pTreeView->clear();
	}

	QSyncFileReadFolderDiffRunnable* pRunnable=
		new QSyncFileReadFolderDiffRunnable(appendPath(getAppParentDir(), SYNC_H9000_DATABASE_FILE_LOCAL_RECORD_FILE), strAttr);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QList<DiffFile>)), 
		this, SLOT(diffFolderFinished(QList<DiffFile>)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QFrameSyncFile::diffFolderFinished(QList<DiffFile> files)
{
	if (m_pTreeView == NULL)
	{
		return;
	}

	m_pTreeView->setDiffFile(files);
}

void QFrameSyncFile::refreshModelFinished()
{
	pushButtonCancel->setEnabled(false);
	pushButtonOK->setEnabled(true);
	pushButtonRefresh->setEnabled(true);
	pushButtonSelectAll->setEnabled(true);
	pushButtonInverse->setEnabled(true);

	comboBoxCommonFolder->setEnabled(true);
}

void QFrameSyncFile::onClickedBtnOk()
{
	pushButtonCancel->setEnabled(true);
	pushButtonOK->setEnabled(false);
	pushButtonRefresh->setEnabled(false);
	pushButtonSelectAll->setEnabled(false);
	pushButtonInverse->setEnabled(false);
	comboBoxCommonFolder->setEnabled(false);

	m_pTreeView->setItemsCheckable(false);

	int nRowCount=m_pTreeView->rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		if (m_pTreeView->isItemChecked(i))
		{
			m_pTreeView->setItemTransferStatus(i, QTreeViewSyncFile::Waiting);
		}
	}

	m_strPath=comboBoxCommonFolder->currentText();

	m_nRow=-1;
	m_nDynamicID=QReceiverParser::AllUser;
	queryRoot();
}

void QFrameSyncFile::onClickedBtnAll()
{
	m_pTreeView->setSortingEnabled(false);

	int nRowCount=m_pTreeView->rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		m_pTreeView->setItemCheckStatus(i, true);
	}

	m_pTreeView->setSortingEnabled(true);
}

void QFrameSyncFile::onClickedBtnInverse()
{
	m_pTreeView->setSortingEnabled(false);

	int nRowCount=m_pTreeView->rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		bool bCheck=m_pTreeView->isItemChecked(i);
		m_pTreeView->setItemCheckStatus(i, !bCheck);
	}

	m_pTreeView->setSortingEnabled(true);
}

void QFrameSyncFile::onClickedBtnRefresh()
{
	currentIndexChanged(comboBoxCommonFolder->currentIndex());
}

void QFrameSyncFile::onClickedBtnCancel()
{
	cancel();

	onClickedBtnRefresh();
}

void QFrameSyncFile::syncFinished()
{
	QReceiverParser::instance()->unregisterInterface(m_nDynamicID);
	m_nDynamicID=QReceiverParser::AllUser;

	pushButtonCancel->setEnabled(false);
	pushButtonOK->setEnabled(true);
	pushButtonRefresh->setEnabled(true);
	pushButtonSelectAll->setEnabled(true);
	pushButtonInverse->setEnabled(true);
	comboBoxCommonFolder->setEnabled(true);

	m_pTreeView->setItemsCheckable(true);
}

void QFrameSyncFile::recvRoot(const QString& root)
{
	m_strSavePath=appendPath(root, QString("%1%2@%3/%4").arg("ini/data/").arg(m_pClient->serverAddr().toString()).arg(getHostName())).arg("database");

	queryUploadDir(Dir, m_strSavePath);
}

void QFrameSyncFile::recvUploadDirAndSymlink(int flag)
{
	switch (flag)
	{
	case QueryMakePathFail:
		{
			m_bStatusTransfer=false;
		}
		break;
	case QueryMakePathSuccess:
		{
			m_bStatusTransfer=true;
		}
		break;
	case QueryCreateSymLinkFail:
		{
			m_bStatusTransfer=false;
		}
		break;
	case QueryCreateSymLinkSuccess:
		{
			m_bStatusTransfer=true;
		}
		break;
	default:
		break;
	}

	fetchTask();
}

void QFrameSyncFile::recvUploadFile(int queryFlag, int flag, quint64 size)
{
	switch (queryFlag)
	{
	case QueryFileOpenSuccess:
		{
			m_nTick=0;
			if (flag == QueryFileOpenFail)
			{
				m_bStatusTransfer=false;

				fetchTask();
			}
		}
		break;
	case QueryFileOpenFail:
		{
			m_bStatusTransfer=false;

			fetchTask();
		}
		break;
	case QueryFileWriteSuccess:
		{
			if (flag == QueryFileReadLost)
			{
				m_bStatusTransfer=true;

				fetchTask();
			}
			else if (flag == QueryFileReadFail)
			{
				m_bStatusTransfer=false;

				fetchTask();
			}
			else if (flag == QueryFileReadSuccess)
			{
				m_nTick=0;

				m_pTreeView->addTransferredSize(m_nRow, size);
			}
			else if (flag == QueryFileReadFinish)
			{
				m_nTick=0;

				m_pTreeView->addTransferredSize(m_nRow, size);
			}
			else if (flag == QueryFileSendFlowControl)
			{
				m_nTick=0;
			}
		}
		break;
	case QueryFileWriteFail:
	case QueryFileRenameFail:
		{
			m_bStatusTransfer=false;

			fetchTask();
		}
		break;
	case QueryFileWriteLost:
		{
			m_bStatusTransfer=false;

			fetchTask();
		}
		break;
	case QueryFileWriteFinish:
		{
			m_bStatusTransfer=true;

			fetchTask();
		}
		break;
	case QueryFileChangePermissionFail:
		{
			m_bStatusTransfer=true;

			fetchTask();
		}
		break;
	default:
		break;
	}
}

void QFrameSyncFile::recvInterruptTransfer(int flag)
{
	Q_UNUSED(flag);

	fetchTask();
}

void QFrameSyncFile::transferFile(int row, const DiffFile& task)
{
	if (row == -1)
	{
		//Í¬²½Íê±Ï
		finished();
		return;
	}

	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	m_nRow=row;
	m_task=task;

	m_bStatusTransfer=false;

 	m_nTick=0;

	if (m_timer.isActive())
	{
		m_timer.start();
	}

	uploads();
}

int QFrameSyncFile::findFileFormat(const QString& name, int format) const
{
	int nFormat=format;
	if (nFormat == AutoFormat)
	{
		if (isTextFormat(name))
		{
			nFormat=TextFormat;
		}
		else
		{
			nFormat=BinaryFormat;
		}
	}

	return nFormat;
}

void QFrameSyncFile::uploads()
{
	QString strPath=appendPath(m_strPath, m_task.path);
	QString strSavePath=appendPath(m_strSavePath, m_task.path);

	if ((m_task.type & SymLink) != 0 || (m_task.type & SymLink) != 0)
	{
		queryUploadSymlink(m_task.type, strPath, strSavePath);
	}
	else if (m_task.type == File || m_task.type == HideFile)
	{
		int nFormat=findFileFormat(m_task.path, AutoFormat);

		queryUploadFile(strPath, nFormat, strSavePath);
	}
}

void QFrameSyncFile::fetchTask()
{
	QReceiverParser::instance()->unregisterInterface(m_nDynamicID);
	m_nDynamicID=QReceiverParser::instance()->registerInterface(this);

	m_pTreeView->setItemTransferStatus(m_nRow, m_bStatusTransfer? QTreeViewSyncFile::Finished:QTreeViewSyncFile::Failed);
	m_nRow=m_pTreeView->fetchTask(m_task);

	transferFile(m_nRow, m_task);
}

void QFrameSyncFile::queryRoot()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRoot, m_nUsrID);

		if (!m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER))
		{
			cancel();
		}
	}
}

void QFrameSyncFile::queryUploadDir(int type, const QString& savePath)
{
	m_nTick=0;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<type<<savePath;

		buffer=pMsg->query(QueryUploadDir, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			cancel();
		}
	}
}

void QFrameSyncFile::queryUploadSymlink(int type, const QString& file, const QString& savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	QFileInfo info(file);
	if (!info.isSymLink())
	{
		fetchTask();

		return;
	}

	m_nTick=0;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<type<<savePath<<info.symLinkTarget();

		buffer=pMsg->query(QueryUploadSymlink, m_nDynamicID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			cancel();
		}
	}
}

void QFrameSyncFile::queryUploadFile(const QString& file, int format, const QString&savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	m_nTick=0;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		int nFormat=findFileFormat(file, format);

		stream<<savePath<<file<<nFormat;

		buffer=pMsg->query(QueryUploadFile, m_nDynamicID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			cancel();
		}
	}
}

void QFrameSyncFile::queryInterruptTransfer()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer;

		buffer=pMsg->query(QueryInterruptTransfer, m_nDynamicID);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			cancel();
		}
	}
}

void QFrameSyncFile::cancel()
{
	queryInterruptTransfer();

	QReceiverParser::instance()->unregisterInterface(m_nDynamicID);
	m_nDynamicID=QReceiverParser::AllUser;

	m_timer.stop();

	finished();
}

void QFrameSyncFile::timeout()
{
	m_nTick++;

	if (m_nTick > MAX_TICK_NUMBER)
	{
		cancel();
	}
}

void QFrameSyncFile::finished()
{
	m_timer.stop();

	syncFinished();
}

QSyncFileReadCommonFolderRunnable::QSyncFileReadCommonFolderRunnable(const QString& file,QObject* parent/* =0 */)
:QObject(parent)
,m_strFile(file)
{

}

QSyncFileReadCommonFolderRunnable::~QSyncFileReadCommonFolderRunnable()
{

}

void QSyncFileReadCommonFolderRunnable::run()
{
	QFile file(m_strFile);
	if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		emit finished(m_folders);
		return;
	}

	QXmlStreamReader reader(&file);
	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isStartElement())
		{
			if (reader.name() == "root" && reader.attributes().value("version") == "1.0")
			{
				readRoot(reader);
			}
			else
			{
				reader.raiseError(QObject::tr("The file is not an xml version 1.0 file."));
			}
		}
	}

	emit finished(m_folders);
}

void QSyncFileReadCommonFolderRunnable::readRoot(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "root");

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement())
			break;

		if (reader.isStartElement())
		{
			if (reader.name() == "folder")
			{
				readFolder(reader);
			}
			else
			{
				readUnknownElement(reader);
			}
		}
	}
}

void QSyncFileReadCommonFolderRunnable::readFolder(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "folder");

	QString strPath=reader.attributes().value("path").toString();
	if (!strPath.isEmpty())
	{
		m_folders.append(strPath);
	}

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement())
			break;

		if (reader.isStartElement())
		{
			readUnknownElement(reader);
		}
	}
}

void QSyncFileReadCommonFolderRunnable::readUnknownElement(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement())
			break;

		if (reader.isStartElement())
		{
			readUnknownElement(reader);
		}
	}
}

QSyncFileReadFolderDiffRunnable::QSyncFileReadFolderDiffRunnable(const QString& file, const QString& attr,QObject* parent/* =0 */)
:QObject(parent)
,m_strFile(file)
,m_strAttr(attr)
{

}

QSyncFileReadFolderDiffRunnable::~QSyncFileReadFolderDiffRunnable()
{

}

void QSyncFileReadFolderDiffRunnable::run()
{
	QFileInfoList files;

	QDir dir(m_strAttr);
	if (dir.exists())
	{
		files=dir.entryInfoList(QDir::Files);
	}

	QFile file(m_strFile);
	if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QXmlStreamReader reader(&file);
		while (!reader.atEnd())
		{
			reader.readNext();

			if (reader.isStartElement())
			{
				if (reader.name() == "root" && reader.attributes().value("version") == "1.0")
				{
					readRoot(reader);
				}
				else
				{
					reader.raiseError(QObject::tr("The file is not an xml version 1.0 file."));
				}
			}
		}	
	}

	QList<DiffFile> diffFiles;
	QListIterator<QFileInfo> it(files);
	while (it.hasNext())
	{
		const QFileInfo& info=it.next();

		DiffFile diffFile;
		diffFile.path=info.fileName();
		diffFile.type=entryInfoFileType(info);
		diffFile.size=info.size();
		diffFile.modified=info.lastModified();
		diffFile.status=QTreeViewSyncFile::Added;

		if (m_files.contains(diffFile.path))
		{
			DiffFile& diff=m_files[diffFile.path];
			if (diff.status == QTreeViewSyncFile::NonVersioned)
			{
				diffFile.status=QTreeViewSyncFile::NonVersioned;
			}
			else
			{
				if (diffFile.size != diff.size || diffFile.modified != diff.modified)
				{
					diffFile.status=QTreeViewSyncFile::Modified;
				}
				else
				{
					diffFile.status=QTreeViewSyncFile::Unmodified;
				}
			}

			m_files.remove(diffFile.path);
		}

		diffFiles.append(diffFile);
	}

	if (!m_files.isEmpty())
	{
		QMapIterator<QString, DiffFile> iter(m_files);
		while (iter.hasNext())
		{
			iter.next();

			DiffFile diffFile=iter.value();
			if (diffFile.status != QTreeViewSyncFile::Deleted)
			{
				diffFile.status=QTreeViewSyncFile::Deleted;

				diffFiles.append(diffFile);
			}
		}
	}

	emit finished(diffFiles);
}

void QSyncFileReadFolderDiffRunnable::readRoot(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "root");

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement())
			break;

		if (reader.isStartElement())
		{
			if (reader.name() == "folder" && reader.attributes().value("path") == m_strAttr)
			{
				readFolder(reader);
			}
			else
			{
				readUnknownElement(reader);
			}
		}
	}
}

void QSyncFileReadFolderDiffRunnable::readFolder(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "folder" && reader.attributes().value("path") == m_strAttr);

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement())
			break;

		if (reader.isStartElement())
		{
			if (reader.name() == "file")
			{
				readFile(reader);
			}
			else
			{
				readUnknownElement(reader);
			}
		}
	}
}

void QSyncFileReadFolderDiffRunnable::readFile(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "file");

	QXmlStreamAttributes attributes=reader.attributes();
	DiffFile file;

	if (!attributes.hasAttribute("path") || !attributes.hasAttribute("size") || !attributes.hasAttribute("type")
		|| !attributes.hasAttribute("modified") || !attributes.hasAttribute("status"))
	{
		return;
	}

	file.path=attributes.value("path").toString();
	if (file.path.isEmpty())
	{
		return;
	}

	file.size=attributes.value("size").toString().toULongLong();
	file.type=attributes.value("type").toString().toInt();
	file.modified=QDateTime::fromString(attributes.value("modified").toString(), FILE_MODIFIED_DATETIME_FORMAT);
	file.status=attributes.value("status").toString().toInt();
	if (file.status < 0 || file.status >= QTreeViewSyncFile::ModifiedStatusEnd)
	{
		return;
	}

	m_files.insert(file.path, file);
}

void QSyncFileReadFolderDiffRunnable::readUnknownElement(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement())
			break;

		if (reader.isStartElement())
		{
			readUnknownElement(reader);
		}
	}
}
