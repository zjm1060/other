#ifndef QMIBEXTERN_H
#define QMIBEXTERN_H

#include "global.h"

#include <QVector>

#include "qutildef.h"

#ifdef Q_OS_WIN
#include "qabstractwmi.h"
#else
#include "qabstractstat.h"
#endif

#include "qabstractmib.h"

#ifdef Q_OS_WIN
typedef QRegisterInstances<QAbstractWmiPtr>	QWmiRegisterInstances;

template<class T>
class QWmiRegisterInterface
{
public:
	QWmiRegisterInterface()
	{
		QWmiRegisterInstances::instance()->addInterface(QAbstractWmiPtr(new T));
	}
};

#define REGISTER_WMI_INSTANCE(name) static QWmiRegisterInterface<name> static##name##WmiInstance;

extern "C" MIB_EXPORT void CreateWmiInstance(QVector<QAbstractWmiPtr>& instances);
#else
typedef QRegisterInstances<QAbstractStatPtr>	QStatRegisterInstances;

template<class T>
class QStatRegisterInterface
{
public:
	QStatRegisterInterface()
	{
		QStatRegisterInstances::instance()->addInterface(QAbstractStatPtr(new T));
	}
};

#define REGISTER_STAT_INSTANCE(name) static QStatRegisterInterface<name> static##name##StatInstance;

extern "C" MIB_EXPORT void CreateStatInstance(QVector<QAbstractStatPtr>& instances);
#endif

typedef QRegisterInstances<QAbstractMibPtr>	QMibRegisterInstances;

template<class T>
class QMibRegisterInterface
{
public:
	QMibRegisterInterface()
	{
		QMibRegisterInstances::instance()->addInterface(QAbstractMibPtr(new T));
	}
};

#define REGISTER_MIB_INSTANCE(name) static QMibRegisterInterface<name> static##name##MibInstance;

extern "C" MIB_EXPORT void CreateMibInstance(QVector<QAbstractMibPtr>& instances);

#endif // QMIBEXTERN_H
