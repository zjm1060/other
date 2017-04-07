#ifndef QMESSAGEPERFORMER_H
#define QMESSAGEPERFORMER_H

#include <QObject>
#include <QFile>

#include "qimessage.h"
#include "extern.h"
#include "qclientmanager.h"
#include "qabstractfiletransfer.h"

/**
*	@Desc 控制机指令信息传输格式
*	@Author zlb
*	@Time 2011-6-29
*
*	QueryConnectToTarget	QueryConnectToTarget|用户ID|主机信息
*	AnswerConnectToTarget	AnswerConnectToTarget|用户ID|连接建立状态
*	QueryUpload				QueryUpload|用户ID|文件类型|源主机路径地址|目标主机保存地址|传输格式(仅对于文件)
*	AnswerUpload			AnswerUpload|用户ID|请求状态|数据长度|数据((仅对于文件夹)文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间)
**/

class QTempPerformer;
class QPerformerFileTransfer;
class QMessagePerformer : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessagePerformer(QObject *parent=0);
	~QMessagePerformer();

	enum {Type=MSG_PERFORMER};

	enum MsgType
	{
		QueryConnectToTarget,		//请求建立与目标主机之间的连接
		AnswerConnectToTarget,
		QueryUpload,				//请求源主机上传文件或目录到目标主机
		AnswerUpload
	};

	enum QueryResult
	{
		QueryConnectToTargetFailed,	//源主机与目标主机建立连接失败
		QueryConnectToTargetSuccess,//建立连接成功
		TargetDisconnected,			//源主机与目标主机断开连接
		QueryUploadDirSuccess,		//请求上传文件夹成功
		QuerySourceDirLost,			//源主机文件夹不存在
		QueryTargetMakeDirFail,		//目标主机创建文件夹失败
		QuerySourceFileReadLost,	//源主机未找到文件
		QuerySourceFileReadFail,	//源主机读文件失败
		QueryTargetCreateFileFail,	//目标主机创建文件失败
		QuerySourceTransferFileBengin,	//源主机开始传输文件
		QuerySourceTransferFileFinish,	//源主机完成传输文件
		QuerySourceTransferFile,	//源主机传输文件中
		QueryTargetFileWriteLost,	//目标主机写文件发生错误后，已经关闭
		QueryTargetFileWriteFail,	//目标主机写入临时文件失败
		QueryTargetFileWriteSuccess,//目标主机写入临时文件成功
		QueryUploadFileSuccess,		//目标主机将临时文件更名为保存文件成功
		QueryUploadFileFail,		//目标主机将临时文件更名为保存文件失败
		QueryUploadSymLinkSuccess,	//请求上传链接文件创建成功
		QueryUploadSymLinkFail		//请求上传链接文件创建失败
	};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QConnection* sender, QByteArray buffer, QObject* view=0);

private:
	QMap<QConnection*, QTempPerformer*>		m_mapPerformer;
};

class QTempPerformer : public QObject, public QAbstractFileTransfer
{
	Q_OBJECT

public:
	QTempPerformer(QConnection* sender, QObject* view, QObject* parent=0);
	virtual ~QTempPerformer();

public:
	void parse(const QByteArray& buffer);

private slots:
	void connected(QClient* client);
	void disconnected(QClient* client);

	void transferStatus(int status, QByteArray buffer);

private:
	void sendMessageToController(quint32 userID, int msgType, int status, const QByteArray& buffer="");	//发送消息至控制端

private:
	typedef struct _clientInfo 
	{
		QClient*	client;		//源主机与目标主机连接socket
		QPerformerFileTransfer*	transferInterface;

		bool	queryConnect;	//是否请求源主机与目标主机进行连接
	}ClientInfo;

private:
	QConnection*	m_pSender;	//控制器与源主机连接socket
	QObject*		m_pView;

	QClientManager*		m_pClientManager;		//用于控制源主机与目标主机通讯

	QMap<quint32, ClientInfo>	m_mapClient;	//key为控制端UsrID
};

class QPerformerFileTransfer : public QObject, public QAbstractFileTransfer
{
	Q_OBJECT

public:
	QPerformerFileTransfer(QObject* parent=0);
	~QPerformerFileTransfer();

public:
	void setSourceDir(const QString& dir);
	void setSourceFile(QFile* file);
	quint32 user();

signals:
	void transferStatus(int status, QByteArray buffer="");

protected:
	virtual void recvUpload(int flag);
	virtual void recvUploadFile(int flag);

private:
	void transferFile();

private:
	quint32		m_nUsrID;		//源主机UsrID，用于与目标主机通讯

	QFile*		m_pFile;		//用于文件传输
	QString		m_strDir;		//源主机当前上传目录
};

#endif // QMESSAGEPERFORMER_H
