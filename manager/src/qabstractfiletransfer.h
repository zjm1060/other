#ifndef QABSTRACTFILETRANSFER_H
#define QABSTRACTFILETRANSFER_H

#include "extern.h"

class QAbstractFileTransfer
{
public:
	QAbstractFileTransfer();
	virtual ~QAbstractFileTransfer();

public:
	virtual void recvRoot(const QString& root);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvDir(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvDrive(const QVector<FileInfo>& drives);
	virtual void recvFileInfo(int flag, quint64 size);
	virtual void recvFileData(int flag, const QByteArray& buffer);
	virtual void recvSymLink(int flag, FileInfo info, const QString& symLink);
	virtual void recvUpload(int flag);
	virtual void recvUploadFile(int flag);
	virtual void recvRename(int row, QString old, QString now, int flag);
	virtual void recvNewDirectory(int flag, const FileInfo& file);
	virtual void recvDelete(int flag);

	virtual void recvConnectToHost(int flag);
	virtual void recvSourceUpload(int flag, QByteArray buffer);
};

#endif // QABSTRACTFILETRANSFER_H
