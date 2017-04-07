#include "qconsoleappender.h"

#include "qproperties.h"
#include "qloggingevent.h"
#include "qloglog.h"
#include "qstreams.h"

QConsoleAppender::QConsoleAppender(bool logToStdErr, bool immediateFlush)
:m_bLogToStdErr(logToStdErr),
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
    loglog()->debug("Entering %s named [%s]!", __FUNCTION__, name().toLocal8Bit().data());
    m_bClosed= true;
}

void QConsoleAppender::append(const QLoggingEvent& event)
{
	if (m_pLayout == NULL)
	{
		return;
	}
	
	QExclusiveTextStream& out=QStreams::instance()->stream(m_bLogToStdErr? STD_ERR_STREAM:STD_OUT_STREAM);
    m_pLayout->formatAndAppend(out, event);
    if(m_bImmediateFlush)
	{
        out.flush();
    }
}


