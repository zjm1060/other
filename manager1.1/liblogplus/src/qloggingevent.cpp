#include "qloggingevent.h"

QLoggingEvent::QLoggingEvent(const QString& logger, LogLevel level, const QString& message, const QString& filename, int line)
: m_strMessage(message),
m_strLogger(logger),
m_level(level),
m_timestamp(QDateTime::currentDateTime()),
m_strFilename(filename),
m_nLine(line),
m_bThreadCached(false),
m_bNdcCached(false)
{
}

QLoggingEvent::QLoggingEvent(const QString& logger, LogLevel level, const QString& ndc, const QString& message,
							 const QString& thread, QDateTime time, const QString& filename, int line)
: m_strMessage(message),
m_strLogger(logger),
m_level(level),
m_timestamp(time),
m_strFilename(filename),
m_nLine(line),
m_bThreadCached(true),
m_strThread(thread),
m_bNdcCached(true),
m_strNdc(ndc)
{
}

QLoggingEvent::QLoggingEvent(const QLoggingEvent& event)
: m_strMessage(event.message()),
m_strLogger(event.logger()),
m_level(event.level()),
m_timestamp(event.timestamp()),
m_strFilename(event.filename()),
m_nLine(event.line()),
m_bThreadCached(true),
m_strThread(event.thread()),
m_bNdcCached(true),
m_strNdc(event.ndc())
{
}

QLoggingEvent::~QLoggingEvent()
{

}

const QString& QLoggingEvent::message() const
{
	return m_strMessage;
}

std::auto_ptr<QLoggingEvent> QLoggingEvent::clone() const
{
	std::auto_ptr<QLoggingEvent> tmp(new QLoggingEvent(*this));
	return tmp;
}

const QString& QLoggingEvent::ndc() const
{ 
	if(!m_bNdcCached)
	{
		m_strNdc= QNdc::instance()->get();
		m_bNdcCached = true;
	}

	return m_strNdc; 
}

const QString& QLoggingEvent::thread() const
{
	if(!m_bThreadCached)
	{
		m_strThread= QThread::currentThread()->objectName();
		if (m_strThread.isEmpty())
		{
			m_strThread=QString::number((quint32)QThread::currentThreadId());
		}

		m_bThreadCached = true;
	}
	return m_strThread; 
}

QLoggingEvent& QLoggingEvent::operator=(const QLoggingEvent& event)
{
	if(this == &event) return *this;

	m_strMessage = event.m_strMessage;
	m_strLogger = event.m_strLogger;
	m_level = event.m_level;
	m_strNdc= event.ndc();
	m_strThread= event.thread();
	m_timestamp = event.m_timestamp;
	m_strFilename= event.m_strFilename;
	m_nLine= event.m_nLine;
	m_bThreadCached = true;
	m_bNdcCached = true;

	return *this;
}
