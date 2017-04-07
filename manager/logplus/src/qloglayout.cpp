#include "qloglayout.h"

#include <QTextStream>

#include "common.h"

void QSimpleLogLayout::formatAndAppend(QTextStream& output, const QLoggingEvent& event)
{
	output<<m_levelManager->toString(event.level())<<" - "<<event.message()<<"\n";
}

void QTTCCLogLayout::formatAndAppend(QTextStream& output, const QLoggingEvent& event)
{
	output<<event.timestamp().toString("yyyy-MM-dd hh:mm:ss.zzz");

	if (event.level() != DEBUG_LOG_LEVEL)
	{
		output<<" ["<<event.thread()<<"] "<<m_levelManager->toString(event.level()) 
			<<" "<<event.logger()<<" <"<<event.ndc()<<"> - "<<event.message()<<"\n";
	}
	else
	{
		output<<" ["<<event.thread()<<"] "<<m_levelManager->toString(event.level()) 
			<<" "<<event.logger()<<" <"<<event.ndc()<<"> "<<event.filename()<<":"<<event.line()<<" - "<<event.message()<<"\n";
	}
}
