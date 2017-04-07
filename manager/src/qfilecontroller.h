#ifndef QFILECONTROLLER_H
#define QFILECONTROLLER_H

#include <QObject>
#include <QVector>
#include <QByteArray>

#include "extern.h"
#include "qresourcemanager.h"
#include "qmessagefiletransfer.h"

class QFileController : public QObject
{
	Q_OBJECT

public:
	QFileController(QObject *parent=0);
	~QFileController();

public:
	//文件传输消息
	Q_INVOKABLE void recvRoot(quint32 usrID, QString root);
	Q_INVOKABLE void recvPath(quint32 usrID, QString path, int flag, QVector<FileInfo> fileInfo);
	Q_INVOKABLE void recvDir(quint32 usrID, QString path, int flag, QVector<FileInfo> fileInfo);
	Q_INVOKABLE void recvDrive(quint32 usrID, QVector<FileInfo> fileInfo);
	Q_INVOKABLE void recvFileInfo(quint32 usrID, int flag, qint64 fileSize);
	Q_INVOKABLE void recvFileData(quint32 usrID, int flag, QByteArray buffer);
	Q_INVOKABLE void recvSymLinkInfo(quint32 usrID, int flag, FileInfo info, QString symLink);
	Q_INVOKABLE void recvUpload(quint32 usrID, int flag);
	Q_INVOKABLE void recvUploadFile(quint32 usrID, int flag);
	Q_INVOKABLE void recvRename(quint32 usrID, int row, QString old, QString now, int flag);
	Q_INVOKABLE void recvNewDirectory(quint32 usrID, int flag, FileInfo file);
	Q_INVOKABLE void recvDelete(quint32 usrID, int flag);

	Q_INVOKABLE void recvConnectToHost(quint32 usrID, int flag);
	Q_INVOKABLE void recvSourceUpload(quint32 usrID, int flag, QByteArray buffer);
};

#endif // QFILECONTROLLER_H
