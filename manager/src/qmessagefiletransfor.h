#ifndef QMESSAGEFILETRANSFOR_H
#define QMESSAGEFILETRANSFOR_H

#include <QObject>
#include <QRunnable>
#include <QMap>
#include <QFile>
#include <QTimer>
#include <QVector>
#include <QFileInfoList>

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
*	AnswerPath:			AnswerPath|用户ID|查询状态(|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间)
*	QueryFile:			QueryFile|用户ID|绝对路径文件名
*	AnswerFile:			AnswerFile|用户ID|查询状态(|文件大小)
*	QueryTransforFile	QueryTransforFile|用户ID|传输格式
*	QueryRetransforFile	QueryRetransforFile|用户ID|传输格式|起始位置
*	AnswerTransforFile	AnswerTransforFile|用户ID|文件内容长度|文件内容
*	QueryDir			QueryDir|用户ID|目录路径
*	QuerySymLink		QuerySymLink|用户ID|绝对路径文件名
*	AnswerSymLink		AnswerSymLink|用户ID||查询状态(|链接信息)
*	QueryDrive			QueryDrive|用户ID
*	AnswerDrive			AnswerDrive|用户ID|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间
**/

class QMessageFileTransfor : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageFileTransfor(QObject *parent=0);
	~QMessageFileTransfor();

	enum {Type=MSG_FILE_TRANSFER};

	enum MsgType
	{
		QueryRoot=0,			//请求根目录
		AnswerRoot,				//返回根目录
		QueryPath,				//请求路径
		AnswerPath,				//返回路径下所有子目录以及文件信息(或不存在该目录)
		QueryFile,				//请求传输文件，设置传输格式（文本，二进制）
		AnswerFileInfo,			//返回文件信息(大小)，或文件不存在
		QueryTransforFile,		//文件创建完成，可以传输文件
		QueryRetransforFile,	//从指定长度开始重传文件
		AnswerTransforFile,		//传输文件内容
		QueryDir,				//请求传输文件夹
		AnswerDir,				//返回文件夹信息
		QuerySymLink,			//请求传输符号链接文件
		AnswerSymLink,
		QueryDrive,				//请求磁盘驱动器
		AnswerDrive
	};

	enum QueryResult
	{
		QueryPathNoExist=0,		//请求文件路径不存在
		QueryPathValid,			//请求文件路径有效
		QueryFileNoExist,		//请求文件不存在
		QueryFileUnreadable,	//请求文件不可读
		QueryFileValid,			//请求文件有效
		QuerySymLinkNoLink		//请求文件不是符号链接文件
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
		Dir=0,
		File,
		SymLink,
		Drive,
		HideDir,
		HideFile,
		HideSymLink
	};

	enum FileTransforFormat
	{
		TextFormat=0,		//文本格式
		DataFormat			//二进制格式
	};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QConnection* sender, QByteArray buffer, QObject* view=0);

public:
	bool addTransforFile(int usrID, QFile* file);
	void removeTransforFile(int usrID);

	QFile* transforFile(int usrID);

private slots:
	void finished(QConnection* sender, int msgType, quint32 usrID, QByteArray buffer);

	void timeout();

private:
	typedef struct _fileHandle
	{
		QFile*	file;
		int tick;
	}FileHandle;

	QMap<int, FileHandle>	m_mapFiles;

	QTimer		m_timer;
};

class QMessageFileTransforRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageFileTransforRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageFileTransforRunnable();

signals:
	void finished(QConnection* sender, int msgType, quint32 usrID, QByteArray buffer);

public:
	void setArg(QConnection* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

private:
	void entryInfoList(QFileInfoList& list, QVector<FileInfo>& files);
	void readContents(QFile* file, int format, QByteArray& buffer);

private:
	QConnection*	m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGEFILETRANSFOR_H
