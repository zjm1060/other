#include "extern.h"

void  CreateAlarmInstance(QVector<QAbstractAlarmPtr>& instances)
{
	instances=QVector<QAbstractAlarmPtr>(QRegisterInstances<QAbstractAlarmPtr>::instance()->interfaces());
}

