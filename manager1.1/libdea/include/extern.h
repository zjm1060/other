#ifndef EXTERN_H
#define EXTERN_H

#include "global.h"
#include "qutildef.h"
#include "qidea.h"

#define REGISTER_DEA_INSTANCE(name) static QRegisterInterface<name, QIDEAPtr> static##name##DEAInstance;

extern "C" DEA_EXPORT void  CreateDEAInstance(QVector<QIDEAPtr>& instances);


#endif // EXTERN_H
