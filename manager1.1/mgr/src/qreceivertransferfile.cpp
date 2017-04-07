#include "qreceivertransferfile.h"

#include <QDir>
#include <QMessageBox>

#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qinfomanager.h"
#include "qtreeviewtransferqueue.h"

#define MAX_TICK_NUMBER		20

QReceiverTransferFile::QReceiverTransferFile(QClient* client, QObject *parent/* =0 */)
	: QObject(parent)
	,m_nDynamicID(QReceiverParser::AllUser)
	,m_pClient(client)
	,m_nOverWriteFlag(QMessageBox::No)
{
	qRegisterMetaType<QReceiverTransferFile::TransferQueueItem>("QReceiverTransferFile::TransferQueueItem");

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_nTick=0;
	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QReceiverTransferFile::~QReceiverTransferFile()
{
	finished();

	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
	m_nUsrID=QReceiverParser::AllUser;
}

void QReceiverTransferFile::finished()
{
	m_timer.stop();

	QReceiverParser::instance()->unregisterInterface(m_nDynamicID);
	m_nDynamicID=QReceiverParser::AllUser;

	m_nOverWriteFlag=QMessageBox::No;
}

void QReceiverTransferFile::transfer(const TransferQueueItem& task)
{
	m_nTick=0;
	m_timer.start();

	QReceiverParser::instance()->unregisterInterface(m_nDynamicID);
	m_nDynamicID=QReceiverParser::instance()->registerInterface(this);

	m_task=task;

	if (m_task.direction == QTreeViewTransferQueue::Download)
	{
		downloads();
	}
	else if (m_task.direction == QTreeViewTransferQueue::Upload)
	{
		uploads();
	}
}

QString QReceiverTransferFile::appendPathName(const QString& path, const QString& name, bool isTop)
{
	return isTop? name:appendPath(path, name);
}

QString QReceiverTransferFile::findSavePath(const QString& path, const QString& name, bool isTop)
{
	QString strSavePath;

	if (isTop)
	{
		QString strName;
		if (name == "/")
		{
			strName="root";
		}
		else if (name.endsWith(":/"))
		{
			strName=name.left(name.size()-2);
		}

		strSavePath=tr("Drive")+"("+strName+")";
	}
	else
	{
		strSavePath=appendPathName(path, name);
	}

	return strSavePath;
}

int QReceiverTransferFile::findFileFormat(const QString& name, int format)
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

void QReceiverTransferFile::uploads()
{
	QString strPath=appendPathName(m_task.source, m_task.name, m_task.isTop);
	QString strSavePath=findSavePath(m_task.target, m_task.name, m_task.isTop);

	if (m_task.type == Drive || m_task.type == Dir || m_task.type == HideDir)
	{
		emit sendTip(Upload, tr("Upload the directory \"%1\" ...").arg(strPath));

		queryUploadDir(m_task.type, strSavePath);
	}
	else
	{
		emit sendTip(Upload, tr("Upload the file \"%1\" ...").arg(strPath));

		queryFileInfo(strSavePath);
	}
}

void QReceiverTransferFile::queryUploadDir(int type, const QString& savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	m_nTick=0;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<type<<savePath;

		buffer=pMsg->query(QueryUploadDir, m_nDynamicID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryUploadSymlink(int type, const QString& file, const QString& savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	QFileInfo info(file);
	if (!info.isSymLink())
	{
		sendTip(Error, tr("Local: Failed to upload the symlink file \"%1\" . It is not a symlink file").arg(file));

		emit uploadSymlinkNoSymLink();

		return;
	}

	m_nTick=0;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<type<<savePath<<info.symLinkTarget();

		buffer=pMsg->query(QueryUploadSymlink, m_nDynamicID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryUploadFile(const QString& file, int format, const QString&savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	m_nTick=0;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		int nFormat=findFileFormat(file, format);

		stream<<savePath<<file<<nFormat;

		buffer=pMsg->query(QueryUploadFile, m_nDynamicID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryInterruptTransfer()
{
	sendTip(Information, tr("Query to interrupt file transfer."));

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer;

		buffer=pMsg->query(QueryInterruptTransfer, m_nDynamicID);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::downloads()
{
	QString strPath=appendPathName(m_task.source, m_task.name, m_task.isTop);
	QString strSavePath=findSavePath(m_task.target, m_task.name, m_task.isTop);

	if (m_task.type == Dir || m_task.type == HideDir)
	{
		queryDownloadDir(strPath, strSavePath);
	}
	else
	{
		QFileInfo fileInfo(strSavePath);
		if (fileInfo.exists())
		{
			if (m_nOverWriteFlag!=QMessageBox::NoAll && m_nOverWriteFlag!=QMessageBox::YesAll)
			{
				m_timer.stop();

				QMessageBox::StandardButton button=QMessageBox::warning(NULL, tr("Warning"),
					tr("Are you sure to overwrite the file \"%1\" ?").arg(strSavePath),
					QMessageBox::No | QMessageBox::NoAll | QMessageBox::Yes | QMessageBox::YesAll | QMessageBox::Cancel, QMessageBox::No);

				m_nOverWriteFlag=button;

				m_timer.start();
			}

			if (m_nOverWriteFlag==QMessageBox::NoAll || m_nOverWriteFlag==QMessageBox::No)
			{
				QString strFilePath=fileInfo.absolutePath();
				QString strBaseName=fileInfo.baseName();
				QString strSuffix=fileInfo.completeSuffix();
				int i=0;
				while (QFile::exists(strSavePath=appendPath(strFilePath, QString("%1[%2].%3").arg(strBaseName).arg(i).arg(strSuffix))))
				{
					i++;
				}

				emit downloadFileRenamed(strSavePath);
			}
		}

		if (m_task.type == File || m_task.type == HideFile)
		{
			int nFormat=findFileFormat(m_task.name, m_task.format);

			queryDownloadFile(strPath, nFormat, strSavePath);
		}
		else if ((m_task.type & SymLink) != 0 || (m_task.type & SymLink) != 0)
		{
			queryDownloadSymLink(strPath, strSavePath);
		}
	}
}

void QReceiverTransferFile::queryDownloadFile(const QString& file, int format, const QString& savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	m_nTick=0;

	emit sendTip(Download, tr("Download \"%1\" ...").arg(file));

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<file<<format<<savePath;

		buffer=pMsg->query(QueryDownloadFile, m_nDynamicID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryDownloadSymLink(const QString& symLink, const QString& savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	m_nTick=0;

	emit sendTip(Download, tr("Download the symbol link file \"%1\" ...").arg(symLink));

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<symLink<<savePath;

		buffer=pMsg->query(QueryDownloadSymLink, m_nDynamicID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryRoot()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRoot, m_nUsrID);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryDrives()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryDrive, m_nUsrID);
		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryPath(const QString& path, int sort)
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<path<<sort<<QStringList()<<QDir::NoFilter;

		buffer=pMsg->query(QueryPath, m_nUsrID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryDownloadDir(const QString& path, const QString& savePath)
{
	if (m_nDynamicID == QReceiverParser::AllUser)
	{
		return;
	}

	m_nTick=0;

	sendTip(Download, tr("Download the folder %1 -> %2 ...").arg(path).arg(savePath));

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<path<<savePath;

		buffer=pMsg->query(QueryDownloadDir, m_nDynamicID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryFileInfo(const QString& file)
{
	Q_ASSERT(!file.isEmpty());

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<file;

		buffer=pMsg->query(QueryFileInfo, m_nUsrID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryRename(const QString& location, const QString& old, const QString& now)
{
	emit sendTip(Information, tr("Remote: Rename %1 to %2 in location %3").arg(old).arg(now).arg(location));

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<location<<old<<now;

		buffer=pMsg->query(QueryRename, m_nUsrID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryNewDirectory(const QString& location)
{
	emit sendTip(Information, tr("Remote: Create a new directory -> \"%1\"").arg(location));

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<location;

		buffer=pMsg->query(QueryNewDirectory, m_nUsrID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::queryDelete(const QString& location, const QVector<QString>& files)
{
	for (int i=0; i<files.size(); i++)
	{
		emit sendTip(Information, tr("Remote: Delete %1 ...").arg(appendPath(location, files[i])));
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<location<<files;

		buffer=pMsg->query(QueryDelete, m_nUsrID, buffer);

		sendMessage(buffer, MSG_FILE_TRANSFER);
	}
}

void QReceiverTransferFile::recvRoot(const QString& root)
{
	emit receiveRoot(root);
}

void QReceiverTransferFile::recvDrives(const QVector<FileInfo>& drives)
{
	emit receiveDrives(drives);
}

void QReceiverTransferFile::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	if (flag == QueryPathValid)
	{
		emit folderCatalog(path, files);
	}
	else if (flag == QueryPathNoExist)
	{
		emit sendTip(Warning, tr("Remote: \"%1\" -> The directory doesn't existed!").arg(path));

		emit folderCatalogFailed(path);
	}
}

void QReceiverTransferFile::recvFileInfo(const QString& file, int flag, const FileInfo& info, const QString& savePath)
{
	Q_UNUSED(info);
	Q_UNUSED(file);

	QString strPath=appendPathName(m_task.source, m_task.name, m_task.isTop);
	QString strSavePath=findSavePath(m_task.target, m_task.name, m_task.isTop);

	m_nTick=0;

	if (flag == QueryFileValid)
	{
		if (m_nOverWriteFlag != QMessageBox::NoAll && m_nOverWriteFlag != QMessageBox::YesAll)
		{
			m_timer.stop();

			QMessageBox::StandardButton button=QMessageBox::warning(NULL, tr("Warning"), tr("Are you sure to overwrite the file?"),
				QMessageBox::No | QMessageBox::NoAll | QMessageBox::Yes | QMessageBox::YesAll | QMessageBox::Cancel, QMessageBox::No);

			m_nOverWriteFlag=button;

			m_timer.start();
		}

		if (m_nOverWriteFlag==QMessageBox::NoAll || m_nOverWriteFlag == QMessageBox::No)
		{
			strSavePath=savePath;

			emit uploadFileRenamed(strSavePath);
		}
		else if (m_nOverWriteFlag == QMessageBox::YesAll || m_nOverWriteFlag == QMessageBox::Yes)
		{
			strSavePath=findSavePath(m_task.target, m_task.name, m_task.isTop);
		}
		else if (m_nOverWriteFlag == QMessageBox::Cancel)
		{
			sendTip(Information, tr("Local: Cancel to transfer the file")+" \""+strPath+"\" .");

			emit uploadFileCanceled();

			return;
		}
	}

	if (m_task.direction == QTreeViewTransferQueue::Upload)
	{
		if ((m_task.type & SymLink) != 0 || (m_task.type & SymLink) != 0)
		{
			queryUploadSymlink(m_task.type, strPath, strSavePath);
		}
		else if (m_task.type == File || m_task.type == HideFile)
		{
			int nFormat=findFileFormat(m_task.name, m_task.format);

			queryUploadFile(strPath, nFormat, strSavePath);
		}
	}
}

void QReceiverTransferFile::recvDownloadFileInfo(const QString& file, int queryFlag, const QString& savePath, int format, quint64 size, int flag)
{
	Q_UNUSED(format);
	Q_UNUSED(savePath);

	switch (queryFlag)
	{
	case QueryFileValid:
		{
			if (flag == QueryFileCreateSuccess)
			{
				m_nTick=0;

				emit sendTip(Information, tr("Local: Create the file \"%1\" successfully.").arg(file));

				emit downloadFileCreateSuccess(size);
			}
			else if (flag == QueryFileCreateFail)
			{
				emit sendTip(Error, tr("Local: Failed to create the file \"%1\" !").arg(file));

				emit downloadFileCreateFailed();
			}
		}
		break;
	case QueryFileNoExist:
		{
			emit sendTip(Error, tr("Remote: The file \"%1\" doesn't exist!").arg(file));

			emit downloadFileNoExist();
		}
		break;
	case QueryFileUnreadable:
		{
			emit sendTip(Error, tr("Remote: The file \"%1\" is unreadable!").arg(file));

			emit downloadFileUnreadable();
		}
		break;
	case QueryFileOpenFail:
		{
			emit sendTip(Error, tr("Remote: Failed to open the file \"%1\" !").arg(file));

			emit downloadFileOpenFail();
		}
	default:
		qDebug("Receive invalid QueryDownloadFileInfo flag!");
		Q_ASSERT(false);
		break;
	}
}

void QReceiverTransferFile::recvTransferFileInfo(int queryFlag, int flag, int size)
{
	QString strSourceName=appendPathName(m_task.source, m_task.name, m_task.isTop);

	switch (queryFlag)
	{
	case QueryFileReadSuccess:
		{
			switch (flag)
			{
			case QueryFileWriteSuccess:
				{
					m_nTick=0;

					emit downloadFileWriteSuccess(size);
				}
				break;
			case QueryFileWriteFail:
				{
					emit sendTip(Error, tr("Local: A write error! Failed to download the file \"%1\" !").arg(strSourceName));

					emit downloadFileWriteFailed();
				}
				break;
			case QueryFileWriteLost:
				{
					emit sendTip(Error, tr("Local: A write error! Failed to download the file \"%1\" !").arg(strSourceName));

					emit downloadFileWriteLost();
				}
				break;
			default:
				break;
			}
		}
		break;
	case QueryFileReadFinish:
		{
			switch (flag)
			{
			case QueryFileRenameFail:
				{
					emit sendTip(Error, tr("Local: Rename error! Failed to download the file \"%1\" !").arg(strSourceName));

					emit downloadFileRenamedFailed();
				}
				break;
			case QueryFileChangePermissionFail:
				{
					emit sendTip(Success, tr("Download the file \"%1\" successfully .").arg(strSourceName));
					emit sendTip(Warning, tr("Local: Failed to change permissions!"));

					emit downloadFileChangePermissionFailed();
				}
				break;
			case QueryFileTransferFinish:
				{
					emit sendTip(Success, tr("Download the file \"%1\" successfully.").arg(strSourceName));

					emit downloadFileTransferFinished();
				}
				break;
			case QueryFileWriteLost:
				{
					emit sendTip(Warning, tr("Local: A write error! Failed to download the file \"%1\" !").arg(strSourceName));

					emit downloadFileWriteLost();
				}
				break;
			}
		}
		break;
	case QueryFileReadLost:
		{
			emit sendTip(Error, tr("Remote: A read error! Failed to download the file \"%1\" !").arg(strSourceName));

			emit downloadFileReadLost();
		}
		break;
	case QueryFileReadFail:
		{
			emit sendTip(Error, tr("Remote: A read error! Failed to download the file \"%1\" !").arg(strSourceName));

			emit downloadFileReadFailed();
		}
		break;
	default:
		qDebug("Receive invalid transfer file info status!");
		Q_ASSERT(false);
		break;
	}
}

void QReceiverTransferFile::recvDownloadDir(const QString& path, int queryFlag, const QString& savePath, int flag, const QVector<FileInfo>& files)
{
	if (queryFlag ==QueryDirValid && flag == QueryMakePathSuccess)
	{
		emit sendTip(Success, tr("Download the directory \"%1\" successfully.").arg(path));

		emit downloadDirSuccess(path, savePath, files, m_task.direction, m_task.format);
	}
	else
	{
		if (queryFlag == QueryDirNoExist)
		{
			emit sendTip(Error, tr("Remote: The directory \"%1\" doesn't existed!").arg(path));
		}
		else if (flag == QueryDirMakePathFailed)
		{
			emit sendTip(Error, tr("Local: Failed to create the directory \"%1\"!").arg(savePath));
		}

		emit downloadDirFailed();
	}
}

void QReceiverTransferFile::recvDownloadSymLink(int queryFlag, int flag)
{
	QString strSourceName=appendPathName(m_task.source, m_task.name, m_task.isTop);

	switch (queryFlag)
	{
	case QueryFileNoExist:
		{
			emit sendTip(Error, tr("Remote: \"%1\" doesn't exist!").arg(strSourceName));

			emit downloadFileNoExist();
		}
		break;
	case QueryFileUnreadable:
		{
			emit sendTip(Error, tr("Remote: \"%1\" is unreadable!").arg(strSourceName));

			emit downloadFileUnreadable();
		}
		break;
	case QuerySymLinkNoLink:
		{
			emit sendTip(Error, tr("Remote: \"%1\" is an invalid symbol link file!").arg(strSourceName));

			emit downloadFileSymLinkNoLink();
		}
		break;
	case QueryFileValid:
		{
			if (flag == QueryCreateSymLinkSuccess)
			{
				emit sendTip(Success, tr("Download the symbol link file \"%1\" successfully.").arg(strSourceName));

				emit downloadFileTransferFinished();
			}
			else if (flag == QueryCreateSymLinkFail)
			{
				emit sendTip(Error, tr("Local: Failed to create the symbol link file \"%1\"!").arg(strSourceName));

				emit downloadFileCreateFailed();
			}
			else if (flag == QueryFileChangePermissionFail)
			{
				emit sendTip(Success, tr("Download the symbol link file \"%1\" successfully.").arg(strSourceName));
				emit sendTip(Warning, tr("Local: Failed to change permissions!"));

				emit downloadFileChangePermissionFailed();
			}
		}
		break;
	default:
		qDebug("Invalid symbol link flag!");
		Q_ASSERT(false);
		break;
	}
}

void QReceiverTransferFile::recvUploadDirAndSymlink(int flag)
{
	QString strPath=appendPathName(m_task.source, m_task.name, m_task.isTop);
	QString strSavePath=findSavePath(m_task.target, m_task.name, m_task.isTop);

	switch (flag)
	{
	case QueryMakePathFail:
		{
			emit sendTip(Error, tr("Remote: Failed to create the directory \"%1\" . May be insufficient permissions.").arg(strSavePath));

			emit uploadDirFailed();
		}
		break;
	case QueryMakePathSuccess:
		{
			QDir dir(strPath);

			QVector<FileInfo> files;

			QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot, 
				QDir::DirsFirst|QDir::Name|QDir::IgnoreCase);
			entryInfoList(list, files, false);

			TransferQueueItem task=m_task;
			task.source=strPath;
			task.isTop=false;
			task.target=strSavePath;

			emit uploadDirSuccess(task, files);
		}
		break;
	case QueryCreateSymLinkFail:
		{
			emit sendTip(Error, tr("Remote: Failed to create the symbol link file \"%1\" . May be insufficient permissions").arg(strSavePath));

			emit uploadSymLinkFailed();
		}
		break;
	case QueryCreateSymLinkSuccess:
		{
			emit sendTip(Success, tr("Upload the symbol link file \"%1\" successfully.").arg(strPath));

			emit uploadSymLinkSuccess();
		}
		break;
	default:
		break;
	}
}

void QReceiverTransferFile::recvUploadFile(int queryFlag, int flag, quint64 size)
{
	QString strPath=appendPathName(m_task.source, m_task.name, m_task.isTop);
	QString strSavePath=findSavePath(m_task.target, m_task.name, m_task.isTop);

	switch (queryFlag)
	{
	case QueryFileOpenSuccess:
		{
			m_nTick=0;
			if (flag == QueryFileOpenFail)
			{
				emit sendTip(Error, tr("Local: Failed to open the file \"%1\" !").arg(strPath));

				emit uploadFileReadFailed();
			}
		}
		break;
	case QueryFileOpenFail:
		{
			sendTip(Error, tr("Remote: Failed to create the file \"%1\" !").arg(strSavePath));

			emit uploadFileCreateFailed();
		}
		break;
	case QueryFileWriteSuccess:
		{
			if (flag == QueryFileReadLost)
			{
				emit sendTip(Warning, tr("Local: A read error! Failed to upload the file \"%1\"").arg(strPath));

				emit uploadFileReadLost();
			}
			else if (flag == QueryFileReadFail)
			{
				emit sendTip(Warning, tr("Local: A read error! Failed to upload the file \"%1\"").arg(strPath));

				emit uploadFileReadFailed();
			}
			else if (flag == QueryFileReadSuccess)
			{
				m_nTick=0;

				emit uploadFileReadSuccess(size);
			}
			else if (flag == QueryFileReadFinish)
			{
				m_nTick=0;

				emit sendTip(Information, tr("Local: Finished to transfer the file \"%1\" .").arg(strPath));
			}
			else if (flag == QueryFileSendFlowControl)
			{
				m_nTick=0;
			}
		}
		break;
	case QueryFileWriteFail:
		{
			emit sendTip(Error, tr("Remote: A write error! Failed to upload \"%1\" !").arg(strPath));

			emit uploadFileWriteFailed();
		}
		break;
	case QueryFileRenameFail:
		{
			emit sendTip(Error, tr("Remote: A rename error! Failed to upload \"%1\" !").arg(strPath));

			emit uploadFileRenameFailed();
		}
		break;
	case QueryFileWriteLost:
		{
			emit sendTip(Error, tr("Remote: A write error! Failed to upload \"%1\" !").arg(strPath));

			emit uploadFileWriteLost();
		}
		break;
	case QueryFileWriteFinish:
	case QueryFileTransferFinish:
		{
			emit sendTip(Success, tr("Upload the file \"%1\" successfully.").arg(strPath));

			emit uploadFileFinished();
		}
		break;
	case QueryFileChangePermissionFail:
		{
			emit sendTip(Success, tr("Upload the file \"%1\" successfully.").arg(strPath));
			emit sendTip(Success, tr("Remote: Failed to change permissions."));

			emit uploadFileChangePermissionFailed();
		}
		break;
	default:
		break;
	}
}

void QReceiverTransferFile::recvInterruptTransfer(int flag)
{
	if (flag == QueryFileRemoveFail)
	{
		QString strSavePath=findSavePath(m_task.target, m_task.name, m_task.isTop);
		emit sendTip(Warning, tr("Remote: Failed to remove the file \"%1\" .").arg(strSavePath));

		emit interruptTransferFailed();
	}

	emit interruptTransferSuccess();
}

void QReceiverTransferFile::recvRename(int flag, const QString& dir, const QString& old, const QString& now)
{
	switch (flag)
	{
	case QueryRenameFail:
		{
			emit sendTip(Error, tr("Remote: Failed to rename! \"%1\" may be not exists, in use or insufficient privileges!").arg(appendPath(dir, old)));

			emit renameFailed(dir, old, now);
		}
		break;
	case QueryRenamePathLost:
		{
			emit sendTip(Error, tr("Remote: Failed to rename! \"%1\" does not exist!").arg(appendPath(dir, old)));

			emit renamePathLost(dir, old, now);
		}
		break;
	case QueryRenameSuccess:
		{
			emit sendTip(Success, tr("Remote: Rename \"%1\" to \"%2\" successfully.").arg(appendPath(dir, old)).arg(appendPath(dir, now)));

			emit renameSuccess(dir, old, now);
		}
		break;
	default:
		break;
	}
}

void QReceiverTransferFile::recvNewDirectory(int flag, const FileInfo& file)
{
	switch (flag)
	{
	case QueryNewFail:
		{
			emit sendTip(Error, tr("Remote: Failed to create a new directory! May be insufficient privileges!"));

			emit newDirFailed();
		}
		break;
	case QueryNewSuccess:
		{
			emit sendTip(Success, tr("Remote: Create a new directory successfully."));

			emit newDirSuccess(file);
		}
		break;
	case QueryDirNoExist:
		{
			emit sendTip(Error, tr("Remote: Failed to create a directory! The path is not existed!"));

			emit newDirPathNoExist();
		}
	default:
		qDebug("Invalid create directory op status");
		break;
	}
}

void QReceiverTransferFile::recvDelete(int flag, const QVector<QString>& files)
{
	switch (flag)
	{
	case QueryDelFail:
		{
			for (int i=0; i<files.size(); i++)
			{
				emit sendTip(Warning, 
					tr("Remote: Failed to delete \"%1\"! May be in use, or insufficient privileges!").arg(files[i]));
			}

			emit deleteFailed();
		}
		break;
	case QueryDelSuccess:
		{
			emit sendTip(Success, tr("Remote: Delete file(s) or folder(s) successfully!"));

			emit deleteSuccess();
		}
		break;
	default:
		qDebug("Invalid delete op status");
		break;
	}
}

void QReceiverTransferFile::timeout()
{
	m_nTick++;

	if (m_nTick > MAX_TICK_NUMBER)
	{
		emit transferTimeout();
	}
}

void QReceiverTransferFile::sendMessage(const QByteArray& buffer, int msgType)
{
	Q_ASSERT_NO_NULL(m_pClient);

	bool bRtn=m_pClient->sendMessage(buffer, msgType);
	if (!bRtn)
	{
		emit sendTip(Error, tr("Disconnected to %1 -> IP=%2, port=%3!").arg(m_pClient->serverAddr().toString())
			.arg(m_pClient->serverAddr().toString()).arg(m_pClient->port()));

		if (m_nDynamicID != QReceiverParser::AllUser)
		{
			emit sendMessageFailed();
		}
	}
}

