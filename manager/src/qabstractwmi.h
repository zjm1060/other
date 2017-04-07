#ifndef QABSTRACTWMI_H
#define QABSTRACTWMI_H

#include <qglobal.h>
#include <QMutex>
#include <QStringList>
#include <QVector>
#include <QVariant>

#ifdef Q_OS_WIN

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

#endif	//Q_OS_WIN

class QAbstractWmi
{
public:
	QAbstractWmi();
	virtual ~QAbstractWmi();

	enum ExecMethodResult
	{
		ExecMethodComInvalid=-2,	//COM初始化失败
		ExecMethodInvalid=-1,		//无效方法
		ExecMethodFailed=0,
		ExecMethodSucceed,
		ExecMethodAccessDenied,
		ExecMethodInsufficientPrivilege,
		ExecMethodPathNotFound,
		ExecMethodInvalidParameter
	};

public:
	virtual int type()=0;
	virtual QVariant result()=0;

public:
	virtual void clear();
	virtual QVariant exec(IWbemServices* service);
	virtual int execMethod(IWbemServices* service, int method, const QStringList& arguments);

protected:
	virtual void beferQuery();	//执行查询操作以前需要完成的工作
	virtual QStringList query()=0;		//请求查询信息列表
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index)=0;	//获得一条记录，记录该记录
	virtual void afterQuery();	//查询完毕后需要执行的操作

protected:
	quint16 findUInt16(IWbemClassObject* object, LPCWSTR wszName);
	quint32 findUInt32(IWbemClassObject* object, LPCWSTR wszName);
	quint64 findUInt64(IWbemClassObject* object, LPCWSTR wszName);
	QString findString(IWbemClassObject* object, LPCWSTR wszName);
	bool findBoolean(IWbemClassObject* object, LPCWSTR wszName);
	QVector<QString> findStringArray(IWbemClassObject* object, LPCWSTR wszName);
	QVector<quint16> findUInt16Array(IWbemClassObject* object, LPCWSTR wszName);
	qreal findReal(IWbemClassObject* object, LPCWSTR wszName);

private:
	virtual void indicate(IEnumWbemClassObject* enumerator, IWbemServices* service, int index);

protected:
	QMutex m_mutex;
};

#endif // QABSTRACTWMI_H
