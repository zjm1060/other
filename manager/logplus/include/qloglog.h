#ifndef QLOGLOG_H
#define QLOGLOG_H

/**
* This class used to output log statements from within the logplus package.
*
* logplus components cannot make logplus logging calls. However, it is
* sometimes useful for the user to learn about what logplus is doing.
*/

#include <QIODevice>
#include <QTextStream>

#include "qsharedobject.h"

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
	static QSharedObjectPtr<QLogLog> instance();

public:
	void setStatus(int type, bool status);

public:
	void debug(const QString& msg);
	void error(const QString& msg);
	void warn(const QString& msg);

	void debug(const char* format, ...);
	void error(const char* format, ...);
	void warn(const char* format, ...);

public:
	static QMutex m_gMutex;

private:
	static QSharedObjectPtr<QLogLog> m_pInstance;

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
	QLogLog& logLog() const;

public:
	QLogLogUser& operator=(const QLogLogUser& rhs);

private:
	void* loglogRef;
};

#endif // QLOGLOG_H
