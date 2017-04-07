#ifndef QLOGGERFACTORY_H
#define QLOGGERFACTORY_H

#include "global.h"
#include "qsharedobject.h"

class QLogger;
class QHierarchy;

class LOGPLUS_EXPORT QLoggerFactoryInterface : public QSharedObject
{
public:
	virtual QSharedObjectPtr<QLogger> makeNewLoggerInstance(const QString& name, const QSharedObjectPtr<QHierarchy>& hierarchy) = 0; 
};

typedef QSharedObjectPtr<QLoggerFactoryInterface>	QLoggerFactoryInterfacePtr;

class LOGPLUS_EXPORT QDefaultLoggerFactory : public QLoggerFactoryInterface
{
public:
	QDefaultLoggerFactory();
	~QDefaultLoggerFactory();

public:
	virtual QSharedObjectPtr<QLogger> makeNewLoggerInstance(const QString& name, const QSharedObjectPtr<QHierarchy>& hierarchy);
};

#endif // QLOGGERFACTORY_H
