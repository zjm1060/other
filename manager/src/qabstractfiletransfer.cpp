#include "qabstractfiletransfer.h"

QAbstractFileTransfer::QAbstractFileTransfer()
{

}

QAbstractFileTransfer::~QAbstractFileTransfer()
{
}

void QAbstractFileTransfer::recvRoot(const QString& root)
{
	Q_UNUSED(root);
}

void QAbstractFileTransfer::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	Q_UNUSED(path);
	Q_UNUSED(flag);
	Q_UNUSED(files);
}

void QAbstractFileTransfer::recvDir(const QString& path, int flag, const QVector<FileInfo>& files)
{
	Q_UNUSED(path);
	Q_UNUSED(flag);
	Q_UNUSED(files);
}

void QAbstractFileTransfer::recvDrive(const QVector<FileInfo>& drives)
{
	Q_UNUSED(drives);
}

void QAbstractFileTransfer::recvFileInfo(int flag, quint64 size)
{
	Q_UNUSED(flag);
	Q_UNUSED(size);
}

void QAbstractFileTransfer::recvFileData(int flag, const QByteArray& buffer)
{
	Q_UNUSED(flag);
	Q_UNUSED(buffer);
}

void QAbstractFileTransfer::recvSymLink(int flag, FileInfo info, const QString& symLink)
{
	Q_UNUSED(flag);
	Q_UNUSED(info);
	Q_UNUSED(symLink);
}

void QAbstractFileTransfer::recvUpload(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractFileTransfer::recvUploadFile(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractFileTransfer::recvRename(int row, QString old, QString now, int flag)
{
	Q_UNUSED(row);
	Q_UNUSED(old);
	Q_UNUSED(now);
	Q_UNUSED(flag);
}

void QAbstractFileTransfer::recvNewDirectory(int flag, const FileInfo& file)
{
	Q_UNUSED(flag);
	Q_UNUSED(file);
}

void QAbstractFileTransfer::recvDelete(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractFileTransfer::recvConnectToHost(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractFileTransfer::recvSourceUpload(int flag, QByteArray buffer)
{
	Q_UNUSED(flag);
	Q_UNUSED(buffer);
}