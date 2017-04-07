#ifndef QABSTRACTCONTROLLER_H
#define QABSTRACTCONTROLLER_H

#include <QObject>

#include "qsharedobject.h"

class UTIL_EXPORT QAbstractController : public QObject, public QSharedObject
{
//	Q_OBJECT

public:
	QAbstractController(QObject *parent=0);
	~QAbstractController();
};

typedef QSharedObjectPtr<QAbstractController> QSharedControllerPtr;

#endif // QABSTRACTCONTROLLER_H
