#include "qloglayout.h"

#include <QTextStream>

#include "common.h"

void QSimpleLogLayout::formatAndAppend(QExclusiveTextStream& output, const QLoggingEvent& event)
{
	output.lock();

	output<<m_levelManager->toString(event.level())<<" - "<<event.message()<<"\n";

	output.unlock();
}

Q_EXPORT_REGISTER_LOG_LAYOUT_FACTORY(QSimpleLogLayout, QSimpleLogLayoutFactory)

void QTtccLogLayout::formatAndAppend(QExclusiveTextStream& output, const QLoggingEvent& event)
{
	output.lock();

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

	output.unlock();
}

Q_EXPORT_REGISTER_LOG_LAYOUT_FACTORY(QTtccLogLayout, QTtccLogLayoutFactory)
