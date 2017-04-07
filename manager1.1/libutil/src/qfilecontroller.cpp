#include "qfilecontroller.h"

#include "qreceiverparser.h"

QFileController::QFileController(QObject *parent)
	: QViewController(parent)
{
}

QFileController::~QFileController()
{

}

void QFileController::recvRoot(quint32 usrID, QString root)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvRoot(root);
	}
}

void QFileController::recvPath(quint32 usrID, QString path, int flag, QVector<FileInfo> fileInfo)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvPath(path, flag, fileInfo);
	}
}

void QFileController::recvDownloadDir(quint32 usrID, QString path, int queryFlag, QString savePath, int flag, QVector<FileInfo> files)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDownloadDir(path, queryFlag, savePath, flag, files);
	}
}

void QFileController::recvDownloadSymLink(quint32 usrID, int queryFlag, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDownloadSymLink(queryFlag, flag);
	}
}

void QFileController::recvUploadDirAndSymlink(quint32 usrID, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadDirAndSymlink(flag);
	}
}

void QFileController::recvDrives(quint32 usrID, QVector<FileInfo> fileInfo)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDrives(fileInfo);
	}
}

void QFileController::recvSymLink(quint32 usrID, int queryFlag, FileInfo info, QString target)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvSymLink(queryFlag, info, target);
	}
}

void QFileController::recvFileInfo(quint32 usrID, QString file, int flag, FileInfo info, QString savePath)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvFileInfo(file, flag, info, savePath);
	}
}

void QFileController::recvDownloadFileInfo(quint32 usrID, QString file, int queryFlag, QString savePath, int format, quint64 size, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDownloadFileInfo(file, queryFlag, savePath, format, size, flag);
	}
}

void QFileController::recvTransferFileInfo(quint32 usrID, int queryFlag, int flag, int size)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvTransferFileInfo(queryFlag, flag, size);
	}
}

void QFileController::recvUploadFile(quint32 usrID, int queryFlag, int flag, quint64 size)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadFile(queryFlag, flag, size);
	}
}

void QFileController::recvRename(quint32 usrID, int flag, QString dir, QString old, QString now)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvRename(flag, dir, old, now);
	}
}

void QFileController::recvNewDirectory(quint32 usrID, int flag, FileInfo file)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvNewDirectory(flag, file);
	}
}

void QFileController::recvDelete(quint32 usrID, int flag, QVector<QString> files)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDelete(flag, files);
	}
}

void QFileController::recvInterruptTransfer(quint32 usrID, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvInterruptTransfer(flag);
	}
}

void QFileController::recvConnectToTarget(quint32 usrID, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvConnectToTarget(flag);
	}
}

void QFileController::recvUploadDirToTarget(quint32 usrID, int flag, QString sourcePath, QString targetPath, QVector<FileInfo> files)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadDirToTarget(flag, sourcePath, targetPath, files);
	}
}

void QFileController::recvUploadSlientDirToTarget(quint32 usrID, int flag, QString sourcePath, QString targetPath, QVector<FileInfo> files)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadSlientDirToTarget(flag, sourcePath, targetPath, files);
	}
}

void QFileController::recvUploadSymlinkToTarget(quint32 usrID, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadSymlinkToTarget(flag);
	}
}

void QFileController::recvUploadFileToTarget(quint32 usrID, int flag, quint64 size)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadFileToTarget(flag, size);
	}
}

void QFileController::recvInterruptTransferToTarget(quint32 usrID, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvInterruptTransferToTarget(flag);
	}
}

void QFileController::recvUploadSlientFile(quint32 usrID, int flag, quint64 size)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadSlientFile(flag, size);
	}
}

void QFileController::recvBackup(quint32 usrID, int flag, int finished, int success)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvBackup(flag, finished, success);
	}
}

void QFileController::recvSyncAssociationConfigure(quint32 usrID, SyncCommands commands)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvSyncAssociationConfigure(commands);
	}
}

void QFileController::recvWriteSyncAssociationConfigure(quint32 usrID, int flag)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvWriteSyncAssociationConfigure(flag);
	}
}

void QFileController::recvCommandKillProcessFail(quint32 usrID, QString processName, quint32 processId, QString path)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCommandKillProcessFail(processName, processId, path);
	}
}

void QFileController::recvCommandCreateProcessFail(quint32 usrID, QString processName)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCommandCreateProcessFail(processName);
	}
}

void QFileController::recvCommandFoundProcessesFail(quint32 usrID)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCommandFoundProcessesFail();
	}
}

void QFileController::recvCommandFoundCommandFail(quint32 usrID)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCommandFoundCommandFail();
	}
}

void QFileController::recvCommandExecCommand(quint32 usrID, int flag, QString command)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCommandExecCommand(flag, command);
	}
}

void QFileController::recvCommandFinish(quint32 usrID)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCommandFinish();
	}
}
