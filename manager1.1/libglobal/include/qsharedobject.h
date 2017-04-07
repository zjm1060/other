#ifndef QSHAREDOBJECT_H
#define QSHAREDOBJECT_H

#include <QMutex>

#include "global.h"

class GLOBAL_EXPORT QSharedObject
{
protected:
	QSharedObject():m_nCount(0){}
	QSharedObject(const QSharedObject&):m_nCount(0){}
	virtual ~QSharedObject();

public:
	void addRef();
	void removeRef();

protected:
	QSharedObject& operator=(const QSharedObject&) { return *this; }

private:
	QMutex	m_mutex;

private:
	mutable int m_nCount;
};

template<class T>
class GLOBAL_EXPORT QSharedObjectPtr
{
public:
	explicit QSharedObjectPtr(T* realPtr = 0)
		: pointee(realPtr)
	{
		addref ();
	}

	QSharedObjectPtr(const QSharedObjectPtr& rhs)
		: pointee(rhs.pointee)
	{
		addref ();
	}

	~QSharedObjectPtr()
	{
		removeRef();
	}

public:
	bool operator==(const QSharedObjectPtr& rhs) const { return (pointee == rhs.pointee); }
	bool operator!=(const QSharedObjectPtr& rhs) const { return (pointee != rhs.pointee); }
	bool operator==(const T* rhs) const { return (pointee == rhs); }
	bool operator!=(const T* rhs) const { return (pointee != rhs); }
	T* operator->() const {Q_ASSERT_NO_NULL(pointee); return pointee; }
	T& operator*() const {Q_ASSERT_NO_NULL(pointee); return *pointee; }

	QSharedObjectPtr& operator=(const QSharedObjectPtr& rhs)
	{
		return this->operator = (rhs.pointee);
	}

	QSharedObjectPtr& operator=(T* rhs)
	{
		QSharedObjectPtr<T> (rhs).swap (*this);
		return *this;
	}

public:
	T* get() const { return pointee; }

	void swap (QSharedObjectPtr & other) throw ()
	{
		qSwap (pointee, other.pointee);
	}

	typedef T * (QSharedObjectPtr:: * unspec_bool_type) () const;
	operator unspec_bool_type () const
	{
		return pointee ? &QSharedObjectPtr::get : 0;
	}

	bool operator ! () const
	{
		return !pointee;
	}

private:
	void addref() const
	{
		if (pointee != NULL)
			pointee->addRef();
	}

	void removeRef() const
	{
		if (pointee != NULL)
			pointee->removeRef();
	}

private:
	T* pointee;
};

typedef QSharedObjectPtr<QSharedObject>	QObjectPtr;

#endif // QSHAREDOBJECT_H
