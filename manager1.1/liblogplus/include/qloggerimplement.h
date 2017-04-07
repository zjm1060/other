#ifndef QLOGGERIMPLEMENT_H
#define QLOGGERIMPLEMENT_H

#include "qappenderattachable.h"

class QHierarchy;
typedef QSharedObjectPtr<QHierarchy> QHierarchyPtr;

class QLoggerImplement;
typedef QSharedObjectPtr<QLoggerImplement> QLoggerImplementPtr;

class LOGPLUS_EXPORT QLoggerImplement : public QAppenderAttachableImplement
{
protected:
	QLoggerImplement(const QString& name, const QHierarchyPtr& hierarchy);

public:
	virtual ~QLoggerImplement();

public:
	virtual void callAppenders(const QLoggingEvent& event);

	virtual void closeNestedAppenders();

	virtual bool isEnabledFor(LogLevel ll);

	virtual void log(LogLevel logLevel, const QString& message, const char* file=NULL, int line=-1);

	virtual LogLevel chainedLogLevel();

	LogLevel logLevel() const { return this->m_logLevel; }
	void setLogLevel(LogLevel logLevel) { this->m_logLevel = logLevel; }


	virtual const QHierarchyPtr& hierarchy() const;

	QString name() const { return m_strName; }

	bool additivity() const;
	void setAdditivity(bool additive);


protected:
	virtual void forcedLog(LogLevel logLevel, const QString& message, const char* file=NULL, int line=-1);


protected:
	QString m_strName;

	LogLevel m_logLevel;

	QLoggerImplementPtr m_pParent;

	bool m_bAdditive;

private:
	QHierarchyPtr m_pHierarchy;

private:
	// Disallow copying of instances of this class
	QLoggerImplement(const QLoggerImplement&);
	QLoggerImplement& operator=(const QLoggerImplement&);

	// Friends
	friend class QLogger;
	friend class QDefaultLoggerFactory;
	friend class QHierarchy;
};

class LOGPLUS_EXPORT QRootLogger : public QLoggerImplement
{
public:
	QRootLogger(const QHierarchyPtr& hierarchy, LogLevel logLevel);
	virtual ~QRootLogger();

public:
	virtual LogLevel chainedLogLevel() const;

public:
	void setLogLevel(LogLevel ll);

};
#endif // QLOGGERIMPLEMENT_H
