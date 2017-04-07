#include "extern.h"

void  CreateDEAInstance(QVector<QIDEAPtr>& instances)
{
	instances=QVector<QIDEAPtr>(QRegisterInstances<QIDEAPtr>::instance()->interfaces());
}

