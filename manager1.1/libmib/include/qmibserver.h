#ifndef QMIBSERVER_H
#define QMIBSERVER_H

#include "qmibextern.h"

class MIB_EXPORT QMibServer : public QAbstractMib
{
public:
	QMibServer();
	~QMibServer();

public:
	enum {Type=UTIL_TYPE_LOCAL_SERVER};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual QVariant getResult() CONVERT_TO_VARIANT(m_server)

private:
	ServerInfo	m_server;
};

#endif // QMIBSERVER_H
