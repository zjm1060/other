#ifndef QLOGLAYOUTFACTORY_H
#define QLOGLAYOUTFACTORY_H

#include "global.h"
#include "qfactory.h"
#include "qsharedobject.h"
#include "qloggingevent.h"
#include "qproperties.h"
#include "qloglevelmanager.h"

/**
* This class is used to layout strings sent to an {@link QAppender}.
*/
class LOGPLUS_EXPORT QLogLayout : public QSharedObject, protected QLogLogUser
{
public:
	QLogLayout();
	QLogLayout(const QProperties&);
	virtual ~QLogLayout();

public:
	virtual void formatAndAppend(QExclusiveTextStream& output, const QLoggingEvent& event) = 0;

private:
	// Disable copy
	QLogLayout(const QLogLayout&);
	QLogLayout& operator=(QLogLayout&);

protected:
	QLogLevelManagerPtr	m_levelManager;
};

typedef QSharedObjectPtr<QLogLayout> QLogLayoutPtr;

class LOGPLUS_EXPORT QLogLayoutFactoryBase : public QFactoryBase
{
public:
	typedef QLogLayout QProductType;
	typedef QLogLayoutPtr QProductPtr;

public:
	QLogLayoutFactoryBase();
	virtual ~QLogLayoutFactoryBase();

	/**
	* Create a "Layout" object.
	*/
	virtual QProductPtr createObject(const QProperties& properties) = 0;
};

typedef QSharedObjectPtr<QLogLayoutFactoryBase> QLogLayoutFactoryBasePtr;

typedef QTemplateFactoryRegistry<QLogLayoutFactoryBase> QLogLayoutFactoryRegistry;
typedef QFactoryRegister<QLogLayoutFactoryRegistry> QLogLayoutFactoryRegister;

#define Q_EXPORT_REGISTER_LOG_LAYOUT_FACTORY(name, factory) \
	static QLogLayoutFactoryRegister static##name##FactoryRegister(#name, factory::createInstance);

LOGPLUS_EXPORT QSharedObjectPtr<QLogLayoutFactoryRegistry > qLogLayoutFactoryRegistryInstance();	//外部调用此动态库中使用

#endif // QLOGLAYOUTFACTORY_H
