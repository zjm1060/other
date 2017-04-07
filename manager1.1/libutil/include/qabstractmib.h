#ifndef QABSTRACTMIB_H
#define QABSTRACTMIB_H

#include "global.h"

#include <QMutex>
#include <QStringList>
#include <QVariant>
#include <QDateTime>

#include "qsharedobject.h"

class UTIL_EXPORT QAbstractMib : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QAbstractMib(int msec);
	virtual ~QAbstractMib();

public:
	enum MibModel
	{
		AutoModel,		//自动模式，周期自动更换数据
		ManualModel,	//手动模式，需要自己设定值
		ModelEnd
	};

public:
	virtual int type()=0;

public:
	void exec();
	QVariant result();
	QByteArray writeStream();

	void setModel(int mode);
	int model() const;

	void setManualResult(const QVariant& var);

	virtual int commond(int method, const QStringList& arguments);

public:
	bool isTurn(const QDateTime& datetime);
	bool hasResult() const;

	void finished();

public:
	void setInterval(int msec);
	int interval();

	virtual void setResult(const QVariant& var)=0;
	virtual void setResult(const QByteArray& buffer);

signals:
	void changed();

protected:
	virtual QVariant getResult()=0;

	virtual void writeStream(QDataStream& stream);

private:
	QMutex	m_mutex;

	int m_nInterval;
	QDateTime m_datetime;

	bool m_bTurn;
	bool m_bInit;

	int m_nModel;
};

typedef QSharedObjectPtr<QAbstractMib> QAbstractMibPtr;

typedef void (*CreateMibInstanceFunc)(QVector<QAbstractMibPtr>& instances);		//mib类动态库必须提供的接口

#endif // QABSTRACTMIB_H
