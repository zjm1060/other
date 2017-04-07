#include "qloglog.h"

#include "qstreams.h"

static char const PREFIX[] = "";
static char const WARN_PREFIX[] = "WARNING: ";
static char const ERR_PREFIX[] = "ERROR: ";

QMutex QLogLog::m_gMutex;
QSharedObjectPtr<QLogLog> QLogLog::m_pInstance;

QLogLog::QLogLog()
{
	memset(m_bStatus, 0, _countof(m_bStatus)*sizeof(bool));
}

QLogLog::~QLogLog()
{

}

QSharedObjectPtr<QLogLog> QLogLog::instance()
{
	if (m_pInstance.get() == 0)
	{
		m_gMutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=QSharedObjectPtr<QLogLog>(new QLogLog);
		}
		m_gMutex.unlock();
	}

	return m_pInstance;
}

void QLogLog::setStatus(int type, bool status)
{
	Q_ASSERT(type < StatusEnd);

	m_bStatus[type]=status;
}

void QLogLog::debug(const QString& msg)
{
	QMutexLocker locker(&m_mutex);

	if(m_bStatus[InternalDebugging] && !m_bStatus[QuiteMode])
	{
		QTextStream& out=QStreams::instance()->stream("stdout");
		out<<PREFIX<<msg<<"\n";

		out.flush();
	}
}

void QLogLog::warn(const QString& msg)
{
	QMutexLocker locker(&m_mutex);
	if(m_bStatus[QuiteMode]) return;

	QTextStream& out=QStreams::instance()->stream("stderr");

	out<<WARN_PREFIX<<msg<<"\n";
	out.flush();
}


void QLogLog::error(const QString& msg)
{
	QMutexLocker locker(&m_mutex);
	if(m_bStatus[QuiteMode]) return;

	QTextStream& out=QStreams::instance()->stream("stderr");

	out<<ERR_PREFIX<<msg<<"\n";
	out.flush();
}

void QLogLog::debug(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	QString s;
	s = s.vsprintf(format, ap);
	va_end(ap);
	
	debug(s);
}

void QLogLog::error(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	QString s;
	s = s.vsprintf(format, ap);
	va_end(ap);

	error(s);
}

void QLogLog::warn(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	QString s;
	s = s.vsprintf(format, ap);
	va_end(ap);

	warn(s);
}


typedef QSharedObjectPtr<QLogLog> QLogLogPtr;

QLogLogUser::QLogLogUser()
{
	loglogRef = new QSharedObjectPtr<QLogLog>(QLogLog::instance());
}

QLogLogUser::QLogLogUser(const QLogLogUser& rhs)
{
	loglogRef = new QSharedObjectPtr<QLogLog>(*static_cast<QLogLogPtr*>(rhs.loglogRef));
}


QLogLogUser::~QLogLogUser()
{
	delete static_cast<QLogLogPtr*>(loglogRef);
}

QLogLog& QLogLogUser::logLog() const
{
	QLogLogPtr* ptr = static_cast<QLogLogPtr*>(loglogRef);
	return **ptr;
}


QLogLogUser& QLogLogUser::operator=(const QLogLogUser& rhs)
{
	if(this == &rhs) {
		return *this;
	}

	delete static_cast<QLogLogPtr*>(loglogRef);
	loglogRef = new QSharedObjectPtr<QLogLog>(*static_cast<QLogLogPtr*>(rhs.loglogRef));

	return *this;
}
