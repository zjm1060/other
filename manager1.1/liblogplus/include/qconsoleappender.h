#ifndef QCONSOLEAPPENDER_H
#define QCONSOLEAPPENDER_H

#include "qappender.h"

class LOGPLUS_EXPORT QConsoleAppender : public QAppender
{
public:
	QConsoleAppender(bool logToStdErr = false, bool immediateFlush = true);
	QConsoleAppender(const QProperties& properties);
	virtual ~QConsoleAppender();

public:
	virtual void close();

protected:
	virtual void append(const QLoggingEvent& event);

protected:
	bool m_bLogToStdErr;
	bool m_bImmediateFlush;
};

#endif // QCONSOLEAPPENDER_H

