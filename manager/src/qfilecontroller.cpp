#include "qfilecontroller.h"

#include "qfiletransferparser.h"

QFileController::QFileController(QObject *parent)
	: QObject(parent)
{
}

QFileController::~QFileController()
{

}

void QFileController::recvRoot(quint32 usrID, QString root)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvRoot(root);
	}
}

void QFileController::recvPath(quint32 usrID, QString path, int flag, QVector<FileInfo> fileInfo)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvPath(path, flag, fileInfo);
	}
}

void QFileController::recvDir(quint32 usrID, QString path, int flag, QVector<FileInfo> fileInfo)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDir(path, flag, fileInfo);
	}
}

void QFileController::recvDrive(quint32 usrID, QVector<FileInfo> fileInfo)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDrive(fileInfo);
	}
}

void QFileController::recvFileInfo(quint32 usrID, int flag, qint64 fileSize)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvFileInfo(flag, fileSize);
	}
}

void QFileController::recvFileData(quint32 usrID, int flag, QByteArray buffer)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvFileData(flag, buffer);
	}
}

void QFileController::recvSymLinkInfo(quint32 usrID, int flag, FileInfo info, QString symLink)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvSymLink(flag, info, symLink);
	}
}

void QFileController::recvUpload(quint32 usrID, int flag)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUpload(flag);
	}
}

void QFileController::recvUploadFile(quint32 usrID, int flag)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvUploadFile(flag);
	}
}

void QFileController::recvRename(quint32 usrID, int row, QString old, QString now, int flag)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvRename(row, old, now, flag);
	}
}

void QFileController::recvNewDirectory(quint32 usrID, int flag, FileInfo file)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvNewDirectory(flag, file);
	}
}

void QFileController::recvDelete(quint32 usrID, int flag)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDelete(flag);
	}
}

void QFileController::recvConnectToHost(quint32 usrID, int flag)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvConnectToHost(flag);
	}
}

void QFileController::recvSourceUpload(quint32 usrID, int flag, QByteArray buffer)
{
	QAbstractFileTransfer* pInterface=QFileTransferParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvSourceUpload(flag, buffer);
	}
}
