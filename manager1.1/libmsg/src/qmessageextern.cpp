#include "qmessageextern.h"
#include "qglobalinfo.h"

void  CreateMessageInstance(QVector<QIMessagePtr>& instances)
{
	instances=QVector<QIMessagePtr>(QRegisterInstances<QIMessagePtr>::instance()->interfaces());
}

void  CreateMulticastMessageInstance(QVector<QIMulticastMsgPtr>& instances)
{
	instances=QVector<QIMulticastMsgPtr>(QRegisterInstances<QIMulticastMsgPtr>::instance()->interfaces());
}

//Q_GLOBAL_STATIC(QThreadPool, theInstance)

QThreadPool* globalThreadPool()
{
	return QGlobalInfo::globalThreadPool();
}
