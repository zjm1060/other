#include "qloglog.h"

#define DEBUG_PREFIX	QObject::tr("DEBUG")+": "
#define WARN_PREFIX		QObject::tr("WARNING")+": "
#define ERROR_PREFIX	QObject::tr("ERROR")+": "

QMutex QLogLog::m_mutex;
QLogLogPtr QLogLog::m_pInstance;

QLogLog::QLogLog()
{
	memset(m_bStatus, 0, _countof(m_bStatus)*sizeof(bool));
}

QLogLog::~QLogLog()
{

}

QLogLogPtr QLogLog::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QLogLog;
		}
		m_mutex.unlock();
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
	if(m_bStatus[InternalDebugging] && !m_bStatus[QuiteMode])
	{
		QExclusiveTextStream& out=QStreams::instance()->stream(STD_OUT_STREAM);

		out.lock();
		out<<DEBUG_PREFIX<<msg<<"\n";
		out.unlock();

		out.flush();
	}
}

void QLogLog::warning(const QString& msg)
{
	if(m_bStatus[QuiteMode]) return;

	QExclusiveTextStream& out=QStreams::instance()->stream(STD_OUT_STREAM);

	out.lock();
	out<<WARN_PREFIX<<msg<<"\n";
	out.unlock();

	out.flush();
}


void QLogLog::error(const QString& msg)
{
	if(m_bStatus[QuiteMode]) return;

	QExclusiveTextStream& out=QStreams::instance()->stream(STD_ERR_STREAM);

	out.lock();
	out<<ERROR_PREFIX<<msg<<"\n";
	out.unlock();

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

void QLogLog::warning(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	QString s;
	s = s.vsprintf(format, ap);
	va_end(ap);

	warning(s);
}

QLogLogUser::QLogLogUser()
{
	m_loglog = QLogLogPtr(QLogLog::instance());
}

QLogLogUser::QLogLogUser(const QLogLogUser& rhs)
{
	m_loglog = rhs.m_loglog;
}


QLogLogUser::~QLogLogUser()
{
}

QLogLogPtr& QLogLogUser::loglog()
{
	return m_loglog;
}


QLogLogUser& QLogLogUser::operator=(const QLogLogUser& rhs)
{
	if(this == &rhs) 
	{
		return *this;
	}

	m_loglog=rhs.m_loglog;

	return *this;
}
