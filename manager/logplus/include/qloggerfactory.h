#ifndef QLOGGERFACTORY_H
#define QLOGGERFACTORY_H

#include "global.h"

class QLogger;
class QHierarchy;
class LOGPLUS_EXPORT QLoggerFactoryInterface
{
public:
	virtual QLogger makeNewLoggerInstance(const QString& name, QHierarchy& hierarchy) = 0; 
};

class LOGPLUS_EXPORT QDefaultLoggerFactory : public QLoggerFactoryInterface
{
public:
	QDefaultLoggerFactory();
	~QDefaultLoggerFactory();

public:
	virtual QLogger makeNewLoggerInstance(const QString& name, QHierarchy& hierarchy);
};

#endif // QLOGGERFACTORY_H
