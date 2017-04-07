#ifndef QMESSAGEDISK_H
#define QMESSAGEDISK_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc 计算机存储设备驱动器信息传输格式
*	@Author zlb
*	@Time 2011-4-24
*
*	QueryDiskDrive:			QueryDiskDrive|用户ID
*	AnswerDiskDrive:		AnswerDiskDrive|用户ID|请求状态(|存储设备驱动器信息)
*	QueryCDROMDrive:		QueryCDROMDrive|用户ID
*	AnswerCDROMDrive:		AnswerCDROMDrive|用户ID|请求状态(|光驱驱动器信息)
*	QueryLogicalDisk:		QueryLogicalDisk|用户ID
*	AnswerLogicalDisk:		AnswerLogicalDisk|用户ID|请求状态(|光驱分区信息)
**/

class MSG_EXPORT QMessageDisk : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageDisk(QObject *parent=0);
	~QMessageDisk();

	enum {Type=MSG_DISK_DRIVE};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageDiskRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageDiskRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageDiskRunnable();

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

signals:
	void finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer);

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGEDISK_H
