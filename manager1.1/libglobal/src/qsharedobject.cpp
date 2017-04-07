#include "qsharedobject.h"

QSharedObject::~QSharedObject()
{
	Q_ASSERT(m_nCount == 0);
}

void QSharedObject::addRef()
{
	QMutexLocker locker(&m_mutex);
	Q_ASSERT(m_nCount >= 0);
	++m_nCount;
}


void QSharedObject::removeRef()
{
	bool bDestroy = false;
	
	m_mutex.lock();

	Q_ASSERT(m_nCount > 0);
	if (--m_nCount == 0)
		bDestroy = true;
	
	m_mutex.unlock();

	if (bDestroy)
		delete this;
}
