#ifndef QFRAMETRANSFERFILE_H
#define QFRAMETRANSFERFILE_H

#include <QFile>
#include <QTimer>
#include <QList>

#include "qclient.h"
#include "qabstractfiletransfer.h"
#include "qreceivertransferfile.h"

class QFrameTransferFile : public QAbstractFileTransfer
{
	Q_OBJECT

public:
	QFrameTransferFile(QClient* client, const QString& subPath, QWidget *parent=0);
	~QFrameTransferFile();

public:
	void queryPath(const QString& path);

	virtual void queryRename(const QString& location, const QString& old, const QString& now);
	virtual void queryNewDirectory(const QString& location);
	virtual void queryDelete(const QString& location, const QVector<QString>& files);

public:
	void remoteDeepRefresh();

protected:
	virtual void transfer(int row, const QReceiverTransferFile::TransferQueueItem& task);

	void transferFile(int row, const QReceiverTransferFile::TransferQueueItem& task);

	virtual void cancel();
	virtual void finished();

	virtual void gotoRemoteHome();
	virtual void gotoRemoteLocation(const QString& location);

private slots:
	void timeout();
	void sendMessageFailed();

	void recvDrives(const QVector<FileInfo> drives);
	void recvRoot(const QString root);

	void folderCatalog(const QString path, const QVector<FileInfo> files);
	void folderCatalogFailed(const QString path);

	void rename(const QString dir, const QString old, const QString now);
	void newDirSuccess(const FileInfo file);

	void deleteFiles();

	void downloadDirSuccess(const QString path, const QString savePath, const QVector<FileInfo> files, int direction, int format);
	void downloadDirFailed();

	void downloadFileRenamed(const QString file);

	void downloadFileCreateSuccess(quint64 size);
	void downloadFileFailed();
	void downloadFileSuccess();

	void downloadFileWriteSuccess(int size);

	void uploadFailed();
	void uploadSuccess();
	void uploadDirSuccess(QReceiverTransferFile::TransferQueueItem task, const QVector<FileInfo> files);

	void uploadFileRenamed(const QString file);
	void uploadFileReadSuccess(quint64 size);

	void interruptTransfer();

private:
	void fetchTask();

protected:
	QClient*	m_pClient;

	QString		m_strRoot;		//程序运行父目录
	QString		m_strSubPath;	//相对于父目录的路径

	QReceiverTransferFile*	m_pReceiver;	//文件传输

private:
	bool		m_bQueryComputer;	//请求获取计算机驱动器信息
	bool		m_bQueryHomePath;	//请求主目录

	int			m_nRow;			//当前同步文件位于同步列表行号
	bool		m_bStatusTransfer;	//传输状态， true传输完成，false传输失败
};

#endif // QFRAMETRANSFERFILE_H
