#ifndef QFACTORY_H
#define QFACTORY_H

#include "qsharedobject.h"
#include "qproperties.h"

#include <QMutex>

/**
* This is the base class for all products.
*/
class GLOBAL_EXPORT QFactoryBase : public QSharedObject
{
public:
	QFactoryBase(){}
	virtual ~QFactoryBase(){}

	virtual QString name() = 0;
};

typedef QSharedObjectPtr<QFactoryBase>	QFactoryBasePtr;
typedef QFactoryBasePtr (*qFactoryInstanceFunction)(const QString&);

template<class T, typename QTemplateFactoryBase>
class GLOBAL_EXPORT QTemplateFactory : public QTemplateFactoryBase
{
public:
	typedef typename QTemplateFactoryBase::QProductPtr QProductPtr;

public:
	QTemplateFactory(const QString& name): m_strName(name){}
	virtual ~QTemplateFactory(){}

public:
	static QFactoryBasePtr createInstance(const QString& name)
	{
		return QFactoryBasePtr(new QTemplateFactory(name));
	}

public:
	virtual QProductPtr createObject(const QProperties& properties)
	{
		return QProductPtr(new T(properties));
	}

	virtual QString name()
	{
		return m_strName;
	}

private:
	QString m_strName;
};

/**
* This is the base class used to implement the functionality required
* by the QFactoryRegistry template class.
*/
class GLOBAL_EXPORT QFactoryRegistryBase : public QSharedObject
{
protected:
	QFactoryRegistryBase();
	virtual ~QFactoryRegistryBase();

public:
	bool exists(const QString& name);

	QList<QString> allProductNames();

protected:
	void registry(const QString& name, QFactoryBasePtr item);
	QFactoryBasePtr item(const QString& name);

	virtual void clear();

protected:
	QMap<QString, QFactoryBasePtr > m_data;

private:
	QMutex m_mutex;
};

template<class T>
class GLOBAL_EXPORT QTemplateFactoryRegistry : public QFactoryRegistryBase
{
public:
	QTemplateFactoryRegistry(){}
	virtual ~QTemplateFactoryRegistry()
	{
		clear();
	}

public:
	static QSharedObjectPtr<QTemplateFactoryRegistry > instance()
	{
		static QSharedObjectPtr<QTemplateFactoryRegistry > m_pInstance(new QTemplateFactoryRegistry);
		
		return m_pInstance;
	}

public:
	void put(QFactoryBasePtr object)
	{
		registry(object->name(), object);
	}

	QSharedObjectPtr<T> get(const QString& name)
	{
		return QSharedObjectPtr<T>(static_cast<T*>(item(name).get()));
	}
};

template<typename T>
class GLOBAL_EXPORT QFactoryRegister
{
public:
	QFactoryRegister(const QString& name, qFactoryInstanceFunction function)
	{
		T::instance()->put(function(name));
	}
};

#endif // QFACTORY_H
