#ifndef QLOGLOG_H
#define QLOGLOG_H

/**
* This class used to output log statements from within the logplus package.
*
* logplus components cannot make logplus logging calls. However, it is
* sometimes useful for the user to learn about what logplus is doing.
*/

#include "qstreams.h"

class QLogLog;
typedef QSharedObjectPtr<QLogLog> QLogLogPtr;

class LOGPLUS_EXPORT QLogLog : public QSharedObject
{
public:
	virtual ~QLogLog();

private:
	QLogLog();
	QLogLog(QLogLog&);

public:
	enum Status
	{
		InternalDebugging,
		QuiteMode,
		StatusEnd
	};

public:
	static QLogLogPtr instance();

public:
	void setStatus(int type, bool status);

public:
	void debug(const QString& msg);
	void error(const QString& msg);
	void warning(const QString& msg);

	void debug(const char* format, ...);
	void error(const char* format, ...);
	void warning(const char* format, ...);

private:
	static QMutex m_mutex;
	static QLogLogPtr m_pInstance;

private:
	bool m_bStatus[StatusEnd];
};

/**
* This class used to simplify the use of the QLogLog class.  Any class
* that uses the QLogLog class should extend this class and retrieve
* their reference to QLogLog using the method provided.
*/
class LOGPLUS_EXPORT QLogLogUser
{
public:
	QLogLogUser();
	QLogLogUser(const QLogLogUser&);
	virtual ~QLogLogUser();

public:
	QLogLogPtr& loglog();

public:
	QLogLogUser& operator=(const QLogLogUser& rhs);

private:
	QLogLogPtr	m_loglog;
};

#endif // QLOGLOG_H
