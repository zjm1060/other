#ifndef QMUTILCASTRECEIVER_H
#define QMUTILCASTRECEIVER_H

#include "global.h"
#include "qabstractmulticast.h"

class NETWORK_EXPORT QMulticastReceiver : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QMulticastReceiver(const QString& uid, quint16 port, QObject* view, QObject* parent=0);
	~QMulticastReceiver();

public slots:
	void readDatagram();

private:
	QUdpSocket	m_socket;

	QByteArray	m_buffer;			//通信信息缓冲区
	QString		m_uid;				//唯一标识
	quint16		m_nPort;

	QObject*	m_pView;
};

#endif // QMUTILCASTRECEIVER_H
