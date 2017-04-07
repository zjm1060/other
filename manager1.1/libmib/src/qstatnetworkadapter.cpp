#include "qstatnetworkadapter.h"

#include "define.h"
#include "common.h"
#include "qlogger.h"

QStatNetworkAdapter::QStatNetworkAdapter(QObject *parent)
: QObject(parent)
{
}

QStatNetworkAdapter::~QStatNetworkAdapter()
{
}

void QStatNetworkAdapter::beferQuery() 
{
	m_vecAdapter.clear(); 
}

void QStatNetworkAdapter::query()
{
#if defined(Q_OS_SOLARIS) || defined(Q_OS_AIX)
	m_vecAdapter=findNetworkAdapter();
#endif
}

REGISTER_STAT_INSTANCE(QStatNetworkAdapter)
