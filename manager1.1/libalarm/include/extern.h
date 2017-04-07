#ifndef EXTERN_H
#define EXTERN_H

#include "global.h"
#include "qutildef.h"
#include "qabstractalarm.h"

#define HMAN_ALARM_CONFIGURE_FILE	"ini/alarm.xml"

#define REGISTER_ALARM_INSTANCE(name) static QRegisterInterface<name, QAbstractAlarmPtr> static##name##AlarmInstance;

extern "C" ALARM_EXPORT void  CreateAlarmInstance(QVector<QAbstractAlarmPtr>& instances);


#endif // EXTERN_H
