#ifndef QDDBCONTROLLER_H
#define QDDBCONTROLLER_H

#include <QMap>

#include "qutildef.h"
#include "qabstractcontroller.h"

class DDB_EXPORT QDDBController : public QAbstractController
{
	Q_OBJECT

public:
	QDDBController(QObject *parent=0);
	~QDDBController();

public:
	Q_INVOKABLE void recvInformDdbConnection(quint32 usrID);
	Q_INVOKABLE void recvPrivilegeData(quint32 usrID, int flag, const QMap<QString, PrivilegeGroup>& groups);
};

#endif // QDDBCONTROLLER_H
