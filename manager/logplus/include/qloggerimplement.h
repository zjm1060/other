#ifndef QLOGGERIMPLEMENT_H
#define QLOGGERIMPLEMENT_H

#include "qappenderattachable.h"
#include "qhierarchy.h"

class QHierarchy;
class LOGPLUS_EXPORT QLoggerImplement : public virtual QSharedObject, public QAppenderAttachableImplement
{
protected:
	QLoggerImplement(const QString& name, QHierarchy& hierarchy);

public:
	virtual ~QLoggerImplement();

public:
	typedef QSharedObjectPtr<QLoggerImplement> QSharedLoggerImplementPtr;

public:
	virtual void callAppenders(const QLoggingEvent& event);

	virtual void closeNestedAppenders();

	virtual bool isEnabledFor(LogLevel ll) const;

	virtual void log(LogLevel logLevel, const QString& message, const char* file=NULL, int line=-1);

	virtual LogLevel chainedLogLevel() const;

	LogLevel logLevel() const { return this->m_logLevel; }
	void setLogLevel(LogLevel logLevel) { this->m_logLevel = logLevel; }


	virtual QHierarchy& hierarchy() const;

	QString name() const { return m_strName; }

	bool additivity() const;
	void setAdditivity(bool additive);


protected:
	virtual void forcedLog(LogLevel logLevel, const QString& message, const char* file=NULL, int line=-1);


protected:
	QString m_strName;

	LogLevel m_logLevel;

	QSharedLoggerImplementPtr m_parent;

	bool m_bAdditive;

private:
	QHierarchy& m_hierarchy;

private:
	// Disallow copying of instances of this class
	QLoggerImplement(const QLoggerImplement&);
	QLoggerImplement& operator=(const QLoggerImplement&);

	// Friends
	friend class QLogger;
	friend class QDefaultLoggerFactory;
	friend class QHierarchy;
};

typedef QLoggerImplement::QSharedLoggerImplementPtr QSharedLoggerImplementPtr;


class LOGPLUS_EXPORT QRootLogger : public QLoggerImplement
{
public:
	QRootLogger(QHierarchy& hierarchy, LogLevel logLevel);
	virtual ~QRootLogger();

public:
	virtual LogLevel chainedLogLevel() const;

public:
	void setLogLevel(LogLevel ll);

};
#endif // QLOGGERIMPLEMENT_H
