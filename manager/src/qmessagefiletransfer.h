#ifndef QMESSAGEFILETRANSFER_H
#define QMESSAGEFILETRANSFER_H

#include <QObject>
#include <QRunnable>
#include <QMap>
#include <QFile>
#include <QTimer>
#include <QVector>
#include <QFileInfoList>
#include <QDataStream>

#include "qimessage.h"
#include "extern.h"

/**
*	@Desc 传输格式
*	@Author zlb
*	@Time 2011-3-31
*
*	QueryRoot:			QueryRoot|用户ID
*	AnswerRoot:			AnswerRoot|用户ID|路径
*	QueryPath:			QueryPath|用户ID|路径|排序方式
*	AnswerPath:			AnswerPath|用户ID|路径|查询状态(|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间)
*	AnswerDir:			同AnswerPath
*	QueryFile:			QueryFile|用户ID|绝对路径文件名
*	AnswerFileInfo:		AnswerFileInfo|用户ID|查询状态(|文件大小)
*	QueryTransferFile	QueryTransferFile|用户ID|绝对路径文件名|传输格式
*	QueryRetransmitFile	QueryRetransmitFile|用户ID|绝对路径文件名|传输格式|起始位置
*	AnswerTransferFile	AnswerTransferFile|用户ID|传输状态(|文件内容长度|文件内容)
*	QueryDir			QueryDir|用户ID|目录路径
*	QuerySymLink		QuerySymLink|用户ID|绝对路径文件名
*	AnswerSymLink		AnswerSymLink|用户ID|查询状态(|链接信息)
*	QueryDrive			QueryDrive|用户ID
*	AnswerDrive			AnswerDrive|用户ID|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间
*	QueryUpload			QueryUpload|用户ID|文件类型|保存地址(|传输格式(仅对于文件)|链接信息(对于链接文件))
*	AnswerUpload		AnswerUpload|用户ID|请求状态
*	QueryUploadFile		QueryUploadFile|用户ID|传输状态|文件内容长度|文件内容
*	AnswerUploadFile	AnswerUploadFile|用户ID|保存状态
*	QueryRename			QueryRename|用户ID|目录|行号|原名|新名
*	AnswerRename		AnswerRename|用户ID|行号|原名|新名|重命名操作状态
*	QueryNewDirectory	QueryNewDirectory|用户ID|父目录
*	AnswerNewDirectory	AnswerNewDirectory|用户ID|新建操作状态(|目录信息)
*	QueryDelete			QueryDelete|用户ID|父目录|文件个数|文件名列表
*	AnswerDelete		AnswerDelete|用户ID|删除操作状态
**/

class QMessageFileTransfer : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageFileTransfer(QObject *parent=0);
	~QMessageFileTransfer();

	enum {Type=MSG_FILE_TRANSFER};

	enum MsgType
	{
		QueryRoot=0,			//请求根目录
		AnswerRoot,				//返回根目录
		QueryPath,				//请求路径
		AnswerPath,				//返回路径下所有子目录以及文件信息(或不存在该目录)
		QueryFile,				//请求传输文件，设置传输格式（文本，二进制）
		AnswerFileInfo,			//返回文件信息(大小)，或文件不存在
		QueryTransferFile,		//文件创建完成，可以传输文件
		QueryRetransmitFile,	//从指定长度开始重传文件
		AnswerTransferFile,		//传输文件内容
		QueryDir,				//请求传输文件夹
		AnswerDir,				//返回文件夹信息
		QuerySymLink,			//请求传输符号链接文件
		AnswerSymLink,
		QueryDrive,				//请求磁盘驱动器信息
		AnswerDrive,
		QueryUpload,			//请求上传文件或目录，包含保存位置(,文件传输格式)
		AnswerUpload,
		QueryUploadFile,		//上传文件内容
		AnswerUploadFile,
		QueryRename,			//请求更名
		AnswerRename,
		QueryNewDirectory,		//请求创建新目录
		AnswerNewDirectory,
		QueryDelete,			//请求删除文件或文件夹
		AnswerDelete
	};

	enum QueryResult
	{
		QueryUnknow=-1,
		QueryPathNoExist=0,		//请求文件路径不存在
		QueryPathValid,			//请求文件路径有效
		QueryFileNoExist,		//请求文件不存在
		QueryFileUnreadable,	//请求文件不可读
		QueryFileValid,			//请求文件有效
		QuerySymLinkNoLink,		//请求文件不是符号链接文件
		QueryMakePathSuccess,	//请求创建文件夹成功
		QueryMakePathFail,		//请求创建文件夹失败
		QueryCreateFileSuccess,	//请求上传文件，创建临时文件成功
		QueryCreateFileFail,	//请求上传文件，创建临时文件失败
		QueryUploadFileWriteLost,	//请求上传文件，写文件发生错误后，已经关闭
		QueryUploadFileWriteFail,	//请求上传文件，写入临时文件失败
		QueryUploadFileWriteSuccess,//请求上传文件，写入临时文件成功
		QueryUploadFileSuccess,	//请求上传文件成功，将临时文件更名为保存文件
		QueryUploadFileFail,	//请求上传文件失败
		QueryUploadSymLinkSuccess,	//请求上传链接文件创建成功
		QueryUploadSymLinkFail	//请求上传链接文件创建失败
	};

	enum SortFlags
	{
		SortName=0,				//按名称排序
		SortTime,				//按时间排序
		SortSize,				//按大小排序
		SortType,				//按类型排序
		SortReversed=0x10		//反序，与其他排序方式混合使用
	};

	enum FileType
	{
		Drive=1,
		Dir=(1<<1),
		HideDir=(1<<2),
		File=(1<<3),
		HideFile=(1<<4),
		SymLink=(1<<5),
		HideSymLink=(1<<6),
		CDDVDDrive=(1<<7),
		SymLinkDir=SymLink|Dir,
		SymLinkHideDir=SymLink|HideDir,
		SymLinkFile=SymLink|File,
		SymLinkHideFile=SymLink|HideFile
	};

	enum FileTransferFormat
	{
		TextFormat=0,		//文本格式
		BinaryFormat		//二进制格式
	};

	enum TransferStatus
	{
		TransferNormal=0,
		TransferReadError,
		TransferFinish
	};

	enum RenameStatus
	{
		RenameFail=0,
		RenameSuccess,
		RenamePathLost		//未发现原始路径
	};

	enum NewDirectoryStatus
	{
		NewFail=0,
		NewSuccess
	};

	enum DelStatus
	{
		DelFail=0,
		DelSuccess
	};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QConnection* sender, QByteArray buffer, QObject* view=0);

public:
	Q_INVOKABLE bool addTransferFile(QObject* client, quint32 usrID, QFile* file);
	Q_INVOKABLE void removeTransferFile(QObject* client, quint32 usrID);

	QFile* transferFile(QObject* client, quint32 usrID);

private slots:
	void finished(QConnection* sender, int msgType, quint32 usrID, QByteArray buffer=QByteArray(), int status=-1);

	void timeout();

private:
	typedef struct _fileHandle
	{
		QFile*	file;
		int tick;
	}FileHandle;

	QMap<QObject*, QMap<quint32, FileHandle> >	m_mapFiles;

	QTimer		m_timer;
};

class QMessageFileTransferRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageFileTransferRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, 
		QMessageFileTransfer* message, QObject* parent=0);
	virtual ~QMessageFileTransferRunnable();

signals:
	void finished(QConnection* sender, int msgType, quint32 usrID, QByteArray buffer=QByteArray(), int status=-1);

public:
	void setArg(QConnection* sender, const QByteArray& buffer, QObject* view, QMessageFileTransfer* message);
	virtual void run();

public:
	static int readContents(QFile* file, QByteArray& buffer);
	static void entryInfoList(QFileInfoList& list, QVector<FileInfo>& files);
	static bool deleteDir(const QString& path);

private:
	QConnection*	m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;

	QMessageFileTransfer*	m_pMessage;
};

QDataStream& operator<<(QDataStream& out, const FileInfo& info);
QDataStream& operator>>(QDataStream& in, FileInfo& info);

#endif // QMESSAGEFILETRANSFER_H
