#ifndef QMESSAGEEXTERN_H
#define QMESSAGEEXTERN_H

#include "global.h"

#include <QString>
#include <QVector>
#include <QThreadPool>

#include "qutildef.h"
#include "qimessage.h"
#include "qimulticastmsg.h"

#define REGISTER_MESSAGE_INSTANCE(name) static QRegisterInterface<name, QIMessagePtr> static##name##MessageInstance;

extern "C" MSG_EXPORT void  CreateMessageInstance(QVector<QIMessagePtr>& instances);

#define REGISTER_MULTICAST_MESSAGE_INSTANCE(name) static QRegisterInterface<name, QIMulticastMsgPtr> static##name##MulticastMsgInstance;

extern "C" MSG_EXPORT void  CreateMulticastMessageInstance(QVector<QIMulticastMsgPtr>& instances);

QThreadPool* globalThreadPool();

#endif // QMESSAGEEXTERN_H
