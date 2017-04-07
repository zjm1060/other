#ifndef QLOGLAYOUT_H
#define QLOGLAYOUT_H

#include "qloglog.h"
#include "qloglayoutfactory.h"

/**
* SimpleLayout consists of the LogLevel of the log statement,
* followed by " - " and then the log message itself.
*/
class LOGPLUS_EXPORT QSimpleLogLayout : public QLogLayout
{
public:
	QSimpleLogLayout() {}
	QSimpleLogLayout(const QProperties& properties) : QLogLayout(properties) {}
	virtual ~QSimpleLogLayout(){};

	virtual void formatAndAppend(QExclusiveTextStream& output, const QLoggingEvent& event);

private: 
	// Disallow copying of instances of this class
	QSimpleLogLayout(const QSimpleLogLayout&);
	QSimpleLogLayout& operator=(const QSimpleLogLayout&);
};

typedef QTemplateFactory<QSimpleLogLayout, QLogLayoutFactoryBase>			QSimpleLogLayoutFactory;

/**
* TTCC layout format consists of time, thread, Logger and nested diagnostic context information, hence the name.
*
*  The first field is the current local time. The second field is the thread outputting the
*  log statement. The third field is the LogLevel, the fourth field is
*  the logger to which the statement belongs.
* 
*  The fifth field (just before the '-') is the nested diagnostic
*  context.  Note the nested diagnostic context may be empty as in the
*  first two statements. The text after the '-' is the message of the
*  statement.
*/
class LOGPLUS_EXPORT QTtccLogLayout : public QLogLayout
{
public:
	QTtccLogLayout(){};
	QTtccLogLayout(const QProperties& properties) : QLogLayout(properties) {}
	virtual ~QTtccLogLayout(){};

	virtual void formatAndAppend(QExclusiveTextStream& output, const QLoggingEvent& event);

private:
	// Disallow copying of instances of this class
	QTtccLogLayout(const QTtccLogLayout&);
	QTtccLogLayout& operator=(const QTtccLogLayout&);
};

typedef QTemplateFactory<QTtccLogLayout, QLogLayoutFactoryBase>			QTtccLogLayoutFactory;

#endif // QLOGLAYOUT_H

