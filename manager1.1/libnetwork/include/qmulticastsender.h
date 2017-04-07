#ifndef QMUTILCASTSENDER_H
#define QMUTILCASTSENDER_H

#include <QUdpSocket>
#include <QByteArray>

#include "global.h"
#include "qabstractmulticast.h"

class NETWORK_EXPORT QMulticastSender : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QMulticastSender(const QString& uid, quint16 port, QObject* parent=0);
	~QMulticastSender();

public:
	const QString& uid() const
	{
		return m_uid;
	}

	void sendDatagram(int msgType, const QString& uid="", const QByteArray& datagram="");	//uid="" 发送至组所有成员

private:
	QString		m_uid;		//本机唯一标识
	quint16		m_nPort;	//端口号

	QUdpSocket	m_socket;
};

typedef QSharedObjectPtr<QMulticastSender>	QMulticastSenderPtr;

#endif // QMUTILCASTSENDER_H
