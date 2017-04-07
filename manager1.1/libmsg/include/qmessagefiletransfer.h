#ifndef QMESSAGEFILETRANSFER_H
#define QMESSAGEFILETRANSFER_H

#include <QObject>
#include <QRunnable>
#include <QMap>
#include <QTimer>
#include <QVector>
#include <QFileInfoList>
#include <QDataStream>
#include <QMutex>

#include "qmessageextern.h"
#include "qfileex.h"

/**
*	@Desc 传输格式
*	@Author zlb
*	@Time 2011-3-31
*	@Modify	2012-2-28
*
*	QueryRoot:			QueryRoot|用户ID
*	AnswerRoot:			AnswerRoot|用户ID|路径
*	QueryPath:			QueryPath|用户ID|路径|排序方式|nameFilter|Filter
*	AnswerPath:			AnswerPath|用户ID|路径|查询状态(|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间)
*	QueryDrive			QueryDrive|用户ID
*	AnswerDrive			AnswerDrive|用户ID|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间
*	QuerySymLink		QuerySymLink|用户ID|文件绝对路径
*	AnswerSymLink		AnswerSymLink|用户ID|查询状态(|链接地址)
*	QueryDownloadDir:	QueryDownloadDir|用户ID|目录路径|目录保存路径
*	AnswerDownloadDir:	AnswerPath|用户ID|目录路径|查询状态(|目录保存路径|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间)
*	QueryFileInfo		QueryFileInfo|用户ID|绝对路径文件名
*	AnswerFileInfo		AnswerFileInfo|用户ID|绝对路径文件名|查询状态(|(若存在)文件信息|绝对路径路径+文件名[编号]）)
*	QueryDownloadFile:	QueryDownloadFile|用户ID|绝对路径文件名|传输格式|文件保存绝对路径文件名
*	AnswerDownloadFileInfo:		AnswerDownloadFileInfo|用户ID|绝对路径文件名|查询状态(|文件保存绝对路径文件名|保存格式|文件大小)
*	QueryTransferFile	QueryTransferFile|用户ID|传输状态
*	AnswerTransferFile	AnswerTransferFile|用户ID|传输状态((文件块号|文件内容长度|文件内容)|(文件权限信息))
*	QueryResumeDownloadFile	QueryResumeDownloadFile|用户ID|绝对路径文件名|传输格式|文件起始偏移量|最后修改日期|文件保存绝对路径文件名
*	AnswerResumeDownloadFile	AnswerResumeDownloadFile|用户ID|绝对路径文件名|查询状态（|文件保存绝对路径文件名|保存格式|文件大小|文件起始偏移量）
*	QueryDownloadFlowControl	QueryDownloadFlowControl|用户ID
*	AnswerDownloadFlowControl	AnswerDownloadFlowControl|用户ID
*	QueryDownloadSymLink		QueryDownloadSymLink|用户ID|文件绝对路径|文件保存绝对路径
*	AnswerDownloadSymLink		AnswerDownloadSymLink|用户ID|查询状态(文件保存绝对路径|链接信息|文件权限)
*	QueryUploadDir		QueryUploadDir|用户ID|文件夹类型|文件夹创建绝对路径
*	AnswerUploadDir		AnswerUploadDir|用户ID|文件夹创建状态
*	QueryUploadFile		QueryUploadFile|用户ID|保存绝对路径|文件绝对路径|传输格式
*	AnswerUploadFile	AnswerUploadFile|用户ID|文件创建状态|文件绝对路径|传输格式
*	QueryUploadFileContent	QueryUploadFileContent|用户ID|状态(文件块号|(文件长度|文件内容)|(文件权限信息))
*	AnswerUploadFileContent	AnswerUploadFileContent|用户ID|保存状态
*	QueryUploadSymlink	QueryUploadSymlink|用户ID|链接文件类型|文件创建绝对路径|文件链接地址
*	AnswerUploadSymlink	AnswerUploadSymlink|用户ID|链接文件创建状态
*	QueryUploadFlowControl	QueryUploadFlowControl|用户ID
*	AnswerUploadFlowControl	AnswerUploadFlowControl|用户ID
*	QueryResumeUploadFile	QueryResumeUploadFile|用户ID|保存绝对路径|文件绝对路径|传输格式|起始位置
*	AnswerResumeUploadFile	AnswerResumeUploadFile|用户ID|保存文件创建状态(|文件绝对路径|传输格式|起始位置)
*	QueryUploadSlientDir	QueryUploadSlientDir|用户ID|文件夹类型|文件夹创建绝对路径
*	QueryUploadSlientSymlink	QueryUploadSlientSymlink|用户ID|链接文件类型|文件创建绝对路径|文件链接地址
*	QueryUploadSlientFile	QueryUploadSlientFile|用户ID|保存绝对路径|文件绝对路径|传输格式
*	QueryUploadSlientFileContent	QueryUploadSlientFileContent|用户ID|保存绝对路径|传输格式
*	AnswerUploadSlientFileContent	AnswerUploadSlientFileContent|用户ID|保存绝对路径|传输格式|状态(文件块号|(文件长度|文件内容)|(文件权限信息))
*	QueryRename			QueryRename|用户ID|目录|原名|新名
*	AnswerRename		AnswerRename|用户ID|重命名操作状态|目录|原名|新名
*	QueryNewDirectory	QueryNewDirectory|用户ID|父目录
*	AnswerNewDirectory	AnswerNewDirectory|用户ID|新建操作状态(|目录信息)
*	QueryDelete			QueryDelete|用户ID|父目录|文件个数|文件名列表
*	AnswerDelete		AnswerDelete|用户ID|删除操作状态(父目录|文件个数，文件名列表)
*	QueryInterruptTransfer	QueryInterruptTransfer|用户ID
*	AnswerInterruptTransfer	AnswerInterruptTransfer|用户ID|中断状态
*	QueryBackup			QueryBackup|用户ID|备份源目录|备份目标目录|文件个数|文件信息
*	AnswerBackup		AnswerBackup|用户ID|备份状态|已备份文件总数|备份成功文件个数
**/

class MSG_EXPORT QMessageFileTransfer : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageFileTransfer(QObject *parent=0);
	~QMessageFileTransfer();

	enum {Type=MSG_FILE_TRANSFER};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int ackType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer=QByteArray());
	void download(QObject* sender, quint32 usrID, QObject* view);
	void upload(QObject* sender, quint32 usrID, QObject* view);
	void slientUpload(QObject* sender, quint32 usrID, QString save, int format, QObject* view);
};

class MSG_EXPORT QMessageFileTransferRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageFileTransferRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageFileTransferRunnable();

signals:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer=QByteArray());
	void download(QObject* sender, quint32 usrID, QObject* view);
	void upload(QObject* sender, quint32 usrID, QObject* view);
	void slientUpload(QObject* sender, quint32 usrID, QString save, int format, QObject* view);

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

class QSharedFile;
typedef QSharedObjectPtr<QSharedFile>	QSharedFilePtr;

#define SHARED_FILE_FIRST_BLOCK		1
class MSG_EXPORT QSharedFile : public QFileEx, public QSharedObject
{
public:
	QSharedFile(const QString & name);
	QSharedFile(QObject* parent);
	QSharedFile(const QString& name, QObject* parent);
	~QSharedFile();

public:
	static QSharedFilePtr createFile(const QString& filename, int format, QObject* client, quint32 usrID);

public:
	qint64 write(int number, const QByteArray& byteArray);

	void setPermissions(int number, Permissions permissionSpec);
	int setPermissions();
	int permissionsSerial();

	int nextBlock();
	bool isCurrentBlock(int number) const;

private:
	void init();

private:
	static QMutex	g_mutex;

private:
	QMutex	m_mutex;

	int		m_nNextBlock;

private:
	QMap<int, QByteArray>	m_contents;
	QPair<int, QFile::Permissions>	m_permissions;
};

class MSG_EXPORT QMessageFileTransferManager : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QMessageFileTransferManager(QObject* parent=0);
	~QMessageFileTransferManager();

public:
	static QMessageFileTransferManager* instance();

public:
	bool addTransferFile(QObject* client, quint32 usrID, const QSharedFilePtr& file);
	void removeTransferFile(QObject* client, quint32 usrID);

	QSharedFilePtr transferFile(QObject* client, quint32 usrID);
	int flowControl(QObject* client, quint32 usrID);

public:
	void check();

private slots:
	void timeout();

private:
	typedef struct _fileHandle
	{
		QSharedFilePtr	file;
		int tick;
		int flowCtl;	//传输控制
	}FileHandle;

	QMap<QObject*, QMap<quint32, FileHandle> >	m_mapFiles;

	QTimer		m_timer;

private:
	static QMutex		g_mutex;
};

#endif // QMESSAGEFILETRANSFER_H
