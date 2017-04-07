#include "qabstractmib.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

QAbstractMib::QAbstractMib(int msec)
:m_bTurn(true)
,m_bInit(false)
{
	setModel(AutoModel);
	setInterval(msec);
}

QAbstractMib::~QAbstractMib()
{

}

void QAbstractMib::setModel(int mode)
{
	Q_ASSERT(mode < ModelEnd);

	m_nModel=mode;
}

int QAbstractMib::model() const
{
	return m_nModel;
}

void QAbstractMib::setInterval(int msec)
{
	QMutexLocker locker(&m_mutex);

	m_nInterval=msec;

	if (m_nInterval >= 0)
	{
		m_datetime=QDateTime::currentDateTime().addMSecs(m_nInterval);
	}
}

int QAbstractMib::interval()
{
	QMutexLocker locker(&m_mutex);
	return m_nInterval;
}

bool QAbstractMib::isTurn(const QDateTime& datetime)
{
	QMutexLocker locker(&m_mutex);
	if (m_nInterval >= 0 && m_bTurn && datetime > m_datetime)
	{
		m_bTurn=false;

		return true;
	}

	return false;
}

int QAbstractMib::commond(int method, const QStringList& arguments)
{
#ifdef Q_OS_WIN
		return QWmiFactory::instance()->execMethod(type(), method, arguments);
#else
		return QStatFactory::instance()->execMethod(type(), method, arguments);
#endif
}

void QAbstractMib::exec()
{
	QMutexLocker locker(&m_mutex);

	QVariant var;
#ifdef Q_OS_WIN
	var=QWmiFactory::instance()->exec(type());
#else
	var=QStatFactory::instance()->exec(type());
#endif

	if (var.isValid())
	{
		setResult(var);
	}
}

void QAbstractMib::setManualResult(const QVariant& var)
{
	QMutexLocker locker(&m_mutex);

	m_bInit=true;
	if (var.isValid())
	{
		setResult(var);
	}
}

QVariant QAbstractMib::result()
{
	QMutexLocker locker(&m_mutex);

	if (hasResult())
	{
		return getResult();
	}

	return QVariant();
}

QByteArray QAbstractMib::writeStream()
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

	if (hasResult())
	{
		writeStream(stream);
	}

	return buffer;
}

void QAbstractMib::writeStream(QDataStream& stream)
{
	Q_UNUSED(stream);
}

void QAbstractMib::finished()
{
	QMutexLocker locker(&m_mutex);

	m_bTurn=true;

	if (!m_bInit)
	{
		m_bInit=true;
	}

	if (m_nInterval >= 0)
	{
		m_datetime=QDateTime::currentDateTime().addMSecs(m_nInterval);
	}
}

bool QAbstractMib::hasResult() const
{
	return m_bInit;
}

void QAbstractMib::setResult(const QByteArray& buffer)
{
	Q_UNUSED(buffer);
}
