#include "qconsoleappender.h"

#include "qproperties.h"
#include "qloggingevent.h"
#include "qloglog.h"
#include "qstreams.h"

QConsoleAppender::QConsoleAppender(bool logToStdErr, bool immediateFlush)
: m_bLogToStdErr(logToStdErr),
  m_bImmediateFlush(immediateFlush)
{
}

QConsoleAppender::QConsoleAppender(const QProperties& properties)
: QAppender(properties),
  m_bLogToStdErr(false),
  m_bImmediateFlush(false)
{
    QString val = properties.property("LogToStdErr").toString().toLower();
    if(val == "true")
	{
        m_bLogToStdErr = true;
    }

    if(properties.exists("ImmediateFlush"))
	{
        QString tmp = properties.property("ImmediateFlush").toString().toLower();
		if (tmp == "true")
		{
			m_bImmediateFlush=true;
		}
		
	}
}

QConsoleAppender::~QConsoleAppender()
{
    destroy();
}

void QConsoleAppender::close()
{
    logLog().debug("Entering ConsoleAppender::close()..");
    m_bClosed= true;
}

void QConsoleAppender::append(const QLoggingEvent& event)
{
	QMutexLocker locker(&(QLogLog::m_gMutex));

	QTextStream& out=QStreams::instance()->stream(m_bLogToStdErr? "stderr":"stdout");
    m_layout->formatAndAppend(out, event);
    if(m_bImmediateFlush)
	{
        out.flush();
    }
}


