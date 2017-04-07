#ifndef QRECEIVERTRANSFERFILE_H
#define QRECEIVERTRANSFERFILE_H

#include <QObject>
#include <QTimer>

#include "qabstractreceiver.h"
#include "qclient.h"

class QReceiverTransferFile : public QObject, public QAbstractReceiver
{
	Q_OBJECT

public:
	QReceiverTransferFile(QClient* client, QObject *parent=0);
	~QReceiverTransferFile();

public:
	enum TipType
	{
		Information=0,
		Warning,
		Error,
		Download,
		Upload,
		Success,
		Unknown,
		Backup,
		TypeEnd
	};

	typedef struct _transferQueueItem
	{
		int direction;

		QString source;
		QString name;
		bool isTop;

		QString target;

		int format;
		quint64 size;

		int type;
		QString typeDesc;
	}TransferQueueItem;

public:
	void transfer(const TransferQueueItem& task);

	void finished();

public:
	void queryRoot();
	void queryDrives();
	void queryPath(const QString& path, int sort);
	void queryFileInfo(const QString& file);
	void queryDownloadDir(const QString& path, const QString& savePath);
	void queryDownloadFile(const QString& file, int format, const QString& savePath);
	void queryDownloadSymLink(const QString& symLink, const QString& savePath);
	void queryUploadDir(int type, const QString& savePath);
	void queryUploadSymlink(int type, const QString& file, const QString& savePath);
	void queryUploadFile(const QString& file, int format, const QString&savePath);
	void queryInterruptTransfer();

	void queryRename(const QString& location, const QString& old, const QString& now);
	void queryNewDirectory(const QString& location);
	void queryDelete(const QString& location, const QVector<QString>& files);

public:
	static QString appendPathName(const QString& path, const QString& name, bool isTop=false);
	static QString findSavePath(const QString& path, const QString& name, bool isTop);
	static int findFileFormat(const QString& name, int format);

signals:
	void transferTimeout();		//传输超时
	void sendTip(int type, const QString tips);
	void sendMessageFailed();		//发送失败，取消传输

	void receiveRoot(const QString root);
	void receiveDrives(const QVector<FileInfo> drives);

	void folderCatalog(const QString path, const QVector<FileInfo> files);
	void folderCatalogFailed(const QString path);

	void renameFailed(const QString dir, const QString old, const QString now);
	void renamePathLost(const QString dir, const QString old, const QString now);
	void renameSuccess(const QString dir, const QString old, const QString now);

	void newDirFailed();
	void newDirSuccess(const FileInfo file);
	void newDirPathNoExist();

	void deleteFailed();
	void deleteSuccess();

	void downloadDirSuccess(const QString path, const QString savePath, const QVector<FileInfo> files, int direction, int format);
	void downloadDirFailed();

	void downloadFileRenamed(const QString file);

	void downloadFileCreateSuccess(quint64 size);
	void downloadFileCreateFailed();
	void downloadFileNoExist();
	void downloadFileUnreadable();
	void downloadFileOpenFail();

	void downloadFileWriteSuccess(int size);
	void downloadFileWriteFailed();
	void downloadFileWriteLost();
	void downloadFileRenamedFailed();
	void downloadFileChangePermissionFailed();
	void downloadFileTransferFinished();
	void downloadFileReadLost();
	void downloadFileReadFailed();

	void downloadFileSymLinkNoLink();

	void uploadDirFailed();
	void uploadDirSuccess(QReceiverTransferFile::TransferQueueItem task, const QVector<FileInfo> files);
	void uploadSymLinkFailed();
	void uploadSymLinkSuccess();

	void uploadSymlinkNoSymLink();

	void uploadFileRenamed(const QString file);
	void uploadFileCanceled();

	void uploadFileReadFailed();
	void uploadFileCreateFailed();
	void uploadFileReadLost();
	void uploadFileReadSuccess(quint64 size);
	void uploadFileWriteFailed();
	void uploadFileRenameFailed();
	void uploadFileWriteLost();
	void uploadFileFinished();
	void uploadFileChangePermissionFailed();

	void interruptTransferFailed();
	void interruptTransferSuccess();

protected:
	virtual void recvRoot(const QString& root);
	virtual void recvDrives(const QVector<FileInfo>& drives);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvDownloadFileInfo(const QString& file, int queryFlag, const QString& savePath, int format, quint64 size, int flag);
	virtual void recvTransferFileInfo(int queryFlag, int flag, int size);
	virtual void recvDownloadDir(const QString& path, int queryFlag, const QString& savePath, int flag, const QVector<FileInfo>& files);
	virtual void recvDownloadSymLink(int queryFlag, int flag);
	virtual void recvUploadDirAndSymlink(int flag);
	virtual void recvUploadFile(int queryFlag, int flag, quint64 size);
	virtual void recvFileInfo(const QString& file, int flag, const FileInfo& info, const QString& savePath);
	virtual void recvRename(int flag, const QString& dir, const QString& old, const QString& now);
	virtual void recvNewDirectory(int flag, const FileInfo& file);
	virtual void recvDelete(int flag, const QVector<QString>& files);
	virtual void recvInterruptTransfer(int flag);

private:
	void downloads();
	void uploads();

	void sendMessage(const QByteArray& buffer, int msgType);

private slots:
	void timeout();

private:
	quint32		m_nUsrID;		//用户ID，用于查询目录信息
	quint32		m_nDynamicID;	//用户ID，用于文件传输

	int m_nOverWriteFlag;		//覆盖标志

	TransferQueueItem	m_task;		//任务

private:
	QTimer		m_timer;		//掉线传输超时
	int			m_nTick;

	QClient*	m_pClient;
};

Q_DECLARE_METATYPE(QReceiverTransferFile::TransferQueueItem)

#endif // QRECEIVERTRANSFERFILE_H
