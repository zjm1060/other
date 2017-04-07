#ifndef QABSTRACTSTAT_H
#define QABSTRACTSTAT_H

#include "global.h"

#include <QMutex>
#include <QStringList>
#include <QVariant>

#include "qsharedobject.h"

class UTIL_EXPORT QAbstractStat : public QSharedObject
{
public:
	QAbstractStat();
	virtual ~QAbstractStat();

	enum ExecMethodResult
	{
		ExecMethodInvalid=-1,		//无效方法
		ExecMethodFailed=0,
		ExecMethodSucceed,
		ExecMethodAccessDenied,
		ExecMethodInsufficientPrivilege,
		ExecMethodPathNotFound,
		ExecMethodInvalidParameter,
		ExecMethodUnfinished
	};

public:
	virtual int type()=0;
	virtual QVariant result()=0;

public:
	virtual void init();
	virtual void clear();

	virtual QVariant exec();
	virtual int execMethod(int method, const QStringList& arguments);

protected:
	virtual void beferQuery();	//执行查询操作以前需要完成的工作
	virtual void afterQuery();	//查询完毕后需要执行的操作

	virtual void query()=0;

protected:
	QMutex	m_mutex;
};

typedef QSharedObjectPtr<QAbstractStat> QAbstractStatPtr;

typedef void (*CreateStatInstanceFunc)(QVector<QAbstractStatPtr>& instances);		//windows mib类动态库必须提供的接口

#endif // QABSTRACTSTAT_H
