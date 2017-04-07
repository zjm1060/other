#include "qmibextern.h"

#ifdef Q_OS_WIN
void CreateWmiInstance(QVector<QAbstractWmiPtr>& instances)
{
	instances=QVector<QAbstractWmiPtr>(QWmiRegisterInstances::instance()->interfaces());
}
#else

void CreateStatInstance(QVector<QAbstractStatPtr>& instances)
{
	instances=QVector<QAbstractStatPtr>(QStatRegisterInstances::instance()->interfaces());
}
#endif

void CreateMibInstance(QVector<QAbstractMibPtr>& instances)
{
	instances=QVector<QAbstractMibPtr>(QMibRegisterInstances::instance()->interfaces());
}
