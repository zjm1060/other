#ifndef QFACTORY_H
#define QFACTORY_H

#include "common.h"
#include "qsharedobject.h"
#include "qproperties.h"

#include <QMutex>

/**
* This is the base class for all products.
*/
class GLOBAL_EXPORT QFactoryBase
{
public:
	QFactoryBase(){}
	virtual ~QFactoryBase(){}

	virtual QString name() = 0;
};

template<class T, typename QTemplateFactoryBase>
class GLOBAL_EXPORT QTemplateFactory : public QTemplateFactoryBase
{
public:
	typedef typename QTemplateFactoryBase::QProductPtr QProductPtr;

public:
	QTemplateFactory(const QString& name)
		: m_strName(name){}
	virtual ~QTemplateFactory(){}

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
	QFactoryRegistryBase(){}
	virtual ~QFactoryRegistryBase(){}

public:
	bool exists(const QString& name);

	QList<QString> allProductNames();

protected:
	void registry(const QString& name, void* item);
	void* item(const QString& name);

	virtual void deleteObject(void *product) const = 0;

	virtual void clear();

protected:
	QMap<QString, void*> m_data;
};

template<class T>
class GLOBAL_EXPORT QTemplateFactoryRegistry : public QFactoryRegistryBase
{
public:
	virtual ~QTemplateFactoryRegistry()
	{
		clear();
	}

public:
	static QSharedObjectPtr<QTemplateFactoryRegistry> instance()
	{
		if (m_pInstance.get() == 0)
		{
			m_gMutex.lock();
			if (m_pInstance == 0)
			{
				m_pInstance=QSharedObjectPtr<QTemplateFactoryRegistry>(new QTemplateFactoryRegistry);
			}
			m_gMutex.unlock();
		}

		return m_pInstance;
	}

public:
	void put(std::auto_ptr<T> object)
	{
		registry(object->name(), object.get());
		object.release();
	}

	T* get(const QString& name)
	{
		return static_cast<T*>(item(name));
	}

protected:
	virtual void deleteObject(void *object) const
	{
		delete static_cast<T*>(object);
	}

private:
	static QMutex m_gMutex;
	static QSharedObjectPtr<QTemplateFactoryRegistry>	m_pInstance;
};

template<class T> QMutex QTemplateFactoryRegistry<T>::m_gMutex;
template<class T> QSharedObjectPtr<QTemplateFactoryRegistry<T> > QTemplateFactoryRegistry<T>::m_pInstance;

#define REGISTER_PRODUCT(registry, factorybase, factory, name) \
	registry->put(std::auto_ptr<factorybase> (new factory(#name)))

#endif // QFACTORY_H
