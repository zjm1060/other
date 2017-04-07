#ifndef QABSTRACTMUTILCAST_H
#define QABSTRACTMUTILCAST_H

#include <QUdpSocket>
#include <QByteArray>

#include "global.h"
#include "qsharedobject.h"

class NETWORK_EXPORT QAbstractMulticast : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QAbstractMulticast(QObject* parent=0);
	~QAbstractMulticast();

public:
	virtual void adapterChanged(QUdpSocket* s, const QString & address=QString());	//Õ¯¬Á…Ë÷√–ﬁ∏ƒ

public:
	void setPort(quint16 port);
	quint16 defaultPort();

protected:
	virtual bool bindAdapter(int fd);

protected:
	QHostAddress	m_address;
	quint16			m_nDefaultPort;
	quint16			m_nPort;
};

typedef QSharedObjectPtr<QAbstractMulticast>	QAbstractMulticastPtr;

#endif // QABSTRACTMUTILCAST_H
