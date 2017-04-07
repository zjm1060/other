#ifndef QMIBREMOTESERVER_H
#define QMIBREMOTESERVER_H

#include <QMultiHash>

#include "qmibextern.h"
#include "qutildef.h"

class MIB_EXPORT QMibRemoteServer : public QAbstractMib
{
public:
	QMibRemoteServer();
	~QMibRemoteServer();

public:
	enum {Type=UTIL_TYPE_REMOTE_SERVER};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual QVariant getResult() CONVERT_TO_VARIANT(m_servers.values())
	
private:
	QMultiHash<QString, ServerInfo>	m_servers;
};

#endif // QMIBREMOTESERVER_H
