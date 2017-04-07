#ifndef QLOGGINGEVENT_H
#define QLOGGINGEVENT_H

#include <QDateTime>
#include <QThread>

#include "global.h"
#include "qloglevelmanager.h"
#include "qndc.h"


#define LOGPLUS_EVENT_DEFAULT_TYPE 1

/**
* The internal representation of logging events. When an affirmative
* decision is made to log then a <code>QLoggingEvent</code> 
* instance is created. This instance is passed around to the 
* different logplus components.
*/
class LOGPLUS_EXPORT QLoggingEvent
{
public:
	/**
	* Instantiate a LoggingEvent from the supplied parameters.
	* 
	* @param logger		The logger of this event.
	* @param level      The LogLevel of this event.
	* @param message	The message of this event.
	* @param filename	Name of file where this event has occurred, can be NULL.
	* @param line		Line number in file specified by the filename parameter.
	*/
	QLoggingEvent(const QString& logger, LogLevel level, const QString& message, const QString& filename, int line);

	QLoggingEvent(const QString& logger, LogLevel level, const QString& ndc, const QString& message,
		const QString& thread, QDateTime time, const QString& filename, int line);

	QLoggingEvent(const QLoggingEvent& event);

	virtual ~QLoggingEvent();

public:
	enum {Type=LOGPLUS_EVENT_DEFAULT_TYPE};

public:
	virtual int type() const	{return Type;}

public:
	virtual const QString& message() const;

	virtual std::auto_ptr<QLoggingEvent> clone() const;

	const QString& logger() const { return m_strLogger; }

	LogLevel level() const { return m_level; }

	const QString& ndc() const;

	const QString& thread() const;

	const QDateTime& timestamp() const { return m_timestamp; }

	const QString& filename() const { return m_strFilename; }

	int line() const { return m_nLine; }

public:
	// public operators
	QLoggingEvent& operator=(const QLoggingEvent& rhs);

private:
	QString m_strLogger;	//Logger name
	LogLevel m_level;		//Log Level

	QString m_strMessage;

	QDateTime m_timestamp;	//time of the log event

	QString m_strFilename;	//filename of the log event

	int m_nLine;

	/** Indicates whether or not the Threadname has been retrieved. */
	mutable bool m_bThreadCached;
	mutable QString m_strThread;

	/** Indicates whether or not the NDC has been retrieved. */
	mutable bool m_bNdcCached;
	mutable QString m_strNdc;
};

#endif // QLOGGINGEVENT_H
