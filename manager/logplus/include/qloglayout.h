#ifndef QLOGLAYOUT_H
#define QLOGLAYOUT_H

#include "qloglog.h"
#include "qloggingevent.h"
#include "qproperties.h"
#include "qloglevelmanager.h"

/**
* This class is used to layout strings sent to an {@link QAppender}.
*/
class LOGPLUS_EXPORT QLogLayout : protected QLogLogUser
{
public:
	QLogLayout():m_levelManager(QLogLevelManager::instance()){}
	QLogLayout(const QProperties&):m_levelManager(QLogLevelManager::instance()){}
	virtual ~QLogLayout(){}

public:
	virtual void formatAndAppend(QTextStream& output, const QLoggingEvent& event) = 0;

private:
	// Disable copy
	QLogLayout(const QLogLayout&);
	QLogLayout& operator=(QLogLayout&);

protected:
	QSharedObjectPtr<QLogLevelManager>	m_levelManager;
};

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

	virtual void formatAndAppend(QTextStream& output, const QLoggingEvent& event);

private: 
	// Disallow copying of instances of this class
	QSimpleLogLayout(const QSimpleLogLayout&);
	QSimpleLogLayout& operator=(const QSimpleLogLayout&);
};

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
class LOGPLUS_EXPORT QTTCCLogLayout : public QLogLayout
{
public:
	QTTCCLogLayout(){};
	QTTCCLogLayout(const QProperties& properties) : QLogLayout(properties) {}
	virtual ~QTTCCLogLayout(){};

	virtual void formatAndAppend(QTextStream& output, const QLoggingEvent& event);

private:
	// Disallow copying of instances of this class
	QTTCCLogLayout(const QTTCCLogLayout&);
	QTTCCLogLayout& operator=(const QTTCCLogLayout&);
};

#endif // QLOGLAYOUT_H

