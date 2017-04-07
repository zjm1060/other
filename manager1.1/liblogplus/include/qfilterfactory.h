#ifndef QFILTERFACTORY_H
#define QFILTERFACTORY_H

#include "global.h"
#include "qfactory.h"
#include "qsharedobject.h"
#include "qloggingevent.h"
#include "qproperties.h"

class QFilter;
typedef QSharedObjectPtr<QFilter> QFilterPtr;

class LOGPLUS_EXPORT QFilter : public QSharedObject
{
public:
	QFilter();
	virtual ~QFilter();

public:
	enum FilterResult
	{ 
		DENY,
		NEUTRAL,
		ACCEPT
	};

public:
	static FilterResult checkFilter(const QFilter* filter, const QLoggingEvent& event);

public:
	void appendFilter(QFilterPtr filter);

public:
	virtual FilterResult decide(const QLoggingEvent& event) const = 0;
	virtual void setProperties(const QProperties& properties);

private:
	QFilterPtr next;
};

class LOGPLUS_EXPORT QFilterFactoryBase : public QFactoryBase
{
public:
	typedef QFilter QProductType;
	typedef QFilterPtr QProductPtr;

public:
	QFilterFactoryBase();
	virtual ~QFilterFactoryBase();

	virtual QProductPtr createObject(const QProperties& properties) = 0;
};

typedef QSharedObjectPtr<QFilterFactoryBase> QFilterFactoryBasePtr;

typedef QTemplateFactoryRegistry<QFilterFactoryBase> QFilterFactoryRegistry;
typedef QFactoryRegister<QFilterFactoryRegistry> QFilterFactoryRegister;

#define Q_EXPORT_REGISTER_FILTER_FACTORY(name, factory) \
	static QFilterFactoryRegister static##name##FactoryRegister(#name, factory::createInstance);

LOGPLUS_EXPORT QSharedObjectPtr<QFilterFactoryRegistry > qFilterFactoryRegistryInstance();	//外部调用此动态库中使用

#endif // QFILTERFACTORY_H
