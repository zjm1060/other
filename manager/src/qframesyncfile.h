#ifndef QFRAMESYNCFILE_H
#define QFRAMESYNCFILE_H

#include <QFile>
#include <QTimer>
#include <QList>

#include "qabstractsync.h"
#include "extern.h"
#include "qclient.h"
#include "qabstractfiletransfer.h"

class QFrameSyncFile : public QAbstractSync, public QAbstractFileTransfer
{
	Q_OBJECT

public:
	QFrameSyncFile(QClient* client, const QString& subPath, QWidget *parent=0);
	~QFrameSyncFile();

public:
	void queryRoot();
	void queryDrives();
	void queryPath(const QString& path);
	void queryFile(const QString& file);
	void queryDir(const QString& path);
	void querySymLink(const QString& symLink);
	void queryTransferFile(const QString& file, int format);
	void queryUpload();
	void queryUploadFile();

	virtual void queryRename(const QString& location, int row, const QString& old, const QString& now);
	virtual void queryNewDirectory(const QString& location);
	virtual void queryDelete(const QString& location, const QStringList& files);

public:
	void remoteDeepRefresh();

protected:
	virtual void recvRoot(const QString& root);
	virtual void recvDrive(const QVector<FileInfo>& drives);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvFileInfo(int flag, quint64 size);
	virtual void recvFileData(int flag, const QByteArray& buffer);
	virtual void recvDir(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvSymLink(int flag, FileInfo info, const QString& symLink);
	virtual void recvUpload(int flag);
	virtual void recvUploadFile(int flag);
	virtual void recvRename(int row, QString old, QString now, int flag);
	virtual void recvNewDirectory(int flag, const FileInfo& file);
	virtual void recvDelete(int flag);

protected:
	virtual void transfer(int row, const QTreeViewTransferQueue::TransferQueueItem& task);

	void transferFile(int row, const QTreeViewTransferQueue::TransferQueueItem& task);
	void downloads();
	void uploads();

	virtual void cancel();
	virtual void finished();

	virtual void gotoRemoteHome();
	virtual void gotoRemoteLocation(const QString& location);

private slots:
	void timeout();

private:
	void fetchTask();

protected:
	QClient*	m_pClient;
	quint32		m_nUsrID;		//用户ID，用于查询目录信息
	quint32		m_nDynamicID;	//用户ID，用于文件传输

	int			m_nIndex;		//当前同步文件序号索引
	QList<FileInfo>	m_listFileInfo;

	QFile*		m_pFile;		//当前需要保存文件
	int			m_nFormat;		//当前同步文件传输格式
	quint64		m_nSize;		//当前同步文件大小

	QString		m_strRoot;		//程序运行父目录
	QString		m_strSubPath;	//相对于父目录的路径

private:
	bool		m_bQueryComputer;	//请求获取计算机驱动器信息

	int			m_nRow;			//当前同步文件位于同步列表行号
	QTreeViewTransferQueue::TransferQueueItem	m_task;		//任务
	bool		m_bStatus;		//文件传输状态,true为完成，false为失败

private:
	QTimer		m_timer;		//掉线传输超时
	int			m_nTick;
};

#endif // QFRAMESYNCFILE_H
