#ifndef QMESSAGCONTROLED_H
#define QMESSAGCONTROLED_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc 控制机指令信息传输格式
*	@Author zlb
*	@Time 2011-6-29
*
*	QueryConnectToTarget		QueryConnectToTarget|用户ID|主机信息
*	AnswerConnectToTarget		AnswerConnectToTarget|用户ID|连接建立状态
*	QueryUploadDirToTarget		QueryUploadDirToTarget|用户ID|文件类型|源主机目录绝对路径|目标主机绝对保存地址
*	AnswerUploadDirToTarget		AnswerUploadDirToTarget|用户ID|文件夹创建状态(|源主机目录路径|目标主机保存地址|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间)
*	QueryUploadFileToTarget		QueryUploadFileToTarget|用户ID|文件类型|文件绝对路径|保存绝对路径|传输格式
*	AnswerUploadFileToTarget	AnswerUploadFileToTarget|用户ID|上传状态(|上传文件内容长度)
*	QueryUploadSymlinkToTarget	QueryUploadSymlinkToTarget|用户ID|链接文件类型|源主机目录绝对路径|目标主机绝对保存地址
*	AnswerUploadSymlinkToTarget	AnswerUploadSymlinkToTarget|用户ID|链接文件创建状态
*	QueryInterruptTransferToTarget	QueryInterruptTransferToTarget|用户ID
*	AnswerInterruptTransferToTarget	AnswerInterruptTransferToTarget|用户ID|中断状态
*	QueryUploadSlientDirToTarget`	QueryUploadSlientDirToTarget|用户ID|文件类型|源主机目录绝对路径|目标主机绝对保存地址
*	AnswerUploadSlientDirToTarget	AnswerUploadSlientDirToTarget|用户ID|文件夹创建状态(|源主机目录路径|目标主机保存地址|文件个数|文件类型|名称|字节大小|可读|可写|可执行|最后修改时间)
*	QueryUploadSlientSymlinkToTarget	QueryUploadSlientSymlinkToTarget|用户ID|链接文件类型|源主机目录绝对路径|目标主机绝对保存地址
*	AnswerUploadSlientSymlinkToTarget	AnswerUploadSlientSymlinkToTarget|用户ID|链接文件创建状态
*	QueryUploadSlientFileToTarget	QueryUploadSlientFileToTarget|用户ID||文件类型|文件绝对路径|保存绝对路径|传输格式
*	AnswerUploadSlientFileToTarget	AnswerUploadSlientFileToTarget|用户ID|上传状态(|上传文件内容长度)
*	QueryExecSyncCommandToTarget	QueryExecSyncCommandToTarget|用户ID|记录类别|工作目录|文件集
*	AnswerExecSyncCommandToTarget	AnswerExecSyncCommandToTarget|用户ID|执行状态|结果参数
**/

class MSG_EXPORT QMessageControlled : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageControlled(QObject *parent=0);
	~QMessageControlled();

	enum {Type=MSG_CONTROLLED};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);
};

class MSG_EXPORT QMessageControlledRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageControlledRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageControlledRunnable();

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGCONTROLED_H
