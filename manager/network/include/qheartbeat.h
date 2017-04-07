#ifndef QHEARTBEAT_H
#define QHEARTBEAT_H

#include <QByteArray>
#include <QList>
#include <QMultiHash>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

#include "global.h"

class NETWORK_EXPORT QHeartBeat : public QObject
{
	Q_OBJECT

public:
	QHeartBeat(quint16 beatPort);
	virtual ~QHeartBeat();

public:
	void setServerPort(quint16 nPort);

	void run();
	void pause();

signals:
	void localNetStatusChanged(bool status);

private slots:
	void sendBeatDatagram();
	void readBeatDatagram();

private:
	QList<QHostAddress> m_listBroadcastAddr;	//广播地址列表

	QUdpSocket	m_socketBeat;
	QTimer		m_timerBeat;

	quint16		m_nPort;		//心跳广播地址端口

	bool		m_bConnecting;	//本机是否连接到网络
};

#endif
