#ifndef QFILECONTROLLER_H
#define QFILECONTROLLER_H

#include <QVector>
#include <QByteArray>

#include "qutildef.h"
#include "qviewcontroller.h"

class UTIL_EXPORT QFileController : public QViewController
{
	Q_OBJECT

public:
	QFileController(QObject *parent=0);
	~QFileController();

public:
	//文件传输消息
	Q_INVOKABLE void recvRoot(quint32 usrID, QString root);
	Q_INVOKABLE void recvPath(quint32 usrID, QString path, int flag, QVector<FileInfo> fileInfo);
	Q_INVOKABLE void recvDrives(quint32 usrID, QVector<FileInfo> fileInfo);
	Q_INVOKABLE void recvSymLink(quint32 usrID, int queryFlag, FileInfo info, QString target);
	Q_INVOKABLE void recvFileInfo(quint32 usrID, QString file, int flag, FileInfo info, QString savePath);
	Q_INVOKABLE void recvDownloadFileInfo(quint32 usrID, QString file, int queryFlag, QString savePath, int format, quint64 size, int flag);
	Q_INVOKABLE void recvTransferFileInfo(quint32 usrID, int queryFlag, int flag, int size);
	Q_INVOKABLE void recvDownloadDir(quint32 usrID, QString path, int queryFlag, QString savePath, int flag, QVector<FileInfo> files);
	Q_INVOKABLE void recvDownloadSymLink(quint32 usrID, int queryFlag, int flag);
	Q_INVOKABLE void recvUploadDirAndSymlink(quint32 usrID, int flag);
	Q_INVOKABLE void recvUploadFile(quint32 usrID, int queryFlag, int flag, quint64 size=0);
	Q_INVOKABLE void recvRename(quint32 usrID, int flag, QString dir, QString old, QString now);
	Q_INVOKABLE void recvNewDirectory(quint32 usrID, int flag, FileInfo file);
	Q_INVOKABLE void recvDelete(quint32 usrID, int flag, QVector<QString> files);
	Q_INVOKABLE void recvInterruptTransfer(quint32 usrID, int flag);

	Q_INVOKABLE void recvConnectToTarget(quint32 usrID, int flag);
	Q_INVOKABLE void recvUploadDirToTarget(quint32 usrID, int flag, QString sourcePath, QString targetPath, QVector<FileInfo> files);
	Q_INVOKABLE void recvUploadSlientDirToTarget(quint32 usrID, int flag, QString sourcePath, QString targetPath, QVector<FileInfo> files);
	Q_INVOKABLE void recvUploadSymlinkToTarget(quint32 usrID, int flag);
	Q_INVOKABLE void recvUploadFileToTarget(quint32 usrID, int flag, quint64 size);
	Q_INVOKABLE void recvInterruptTransferToTarget(quint32 usrID, int flag);

	Q_INVOKABLE void recvUploadSlientFile(quint32 usrID, int flag, quint64 size=0);

	Q_INVOKABLE void recvBackup(quint32 usrID, int flag, int finished, int success);

	Q_INVOKABLE void recvSyncAssociationConfigure(quint32 usrID, SyncCommands commands);
	Q_INVOKABLE void recvWriteSyncAssociationConfigure(quint32 usrID, int flag);

	Q_INVOKABLE void recvCommandKillProcessFail(quint32 usrID, QString processName, quint32 processId, QString path);
	Q_INVOKABLE void recvCommandCreateProcessFail(quint32 usrID, QString processName);
	Q_INVOKABLE void recvCommandFoundProcessesFail(quint32 usrID);
	Q_INVOKABLE void recvCommandFoundCommandFail(quint32 usrID);
	Q_INVOKABLE void recvCommandExecCommand(quint32 usrID, int flag, QString command);
	Q_INVOKABLE void recvCommandFinish(quint32 usrID);
};

#endif // QFILECONTROLLER_H
