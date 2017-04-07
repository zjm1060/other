#ifndef QRECEIVERINSPECT_H
#define QRECEIVERINSPECT_H

#include <QObject>

#include "qabstractreceiver.h"
#include "qclient.h"

class QReceiverInspect : public QObject, public QAbstractReceiver
{
	Q_OBJECT

public:
	QReceiverInspect(QObject *parent=0);
	~QReceiverInspect();

public:
	bool queryDatabase(QClient* client);
	bool queryDataSource(QClient* client, int msg, int ack, const QByteArray& buffer);

signals:
	void cscsDataSourceLost();
	void cscsDataSourceNoExisted();
	void recvCscsDatabase(CscsDatabase db);
	void recvCscsStationEigenvalue(int stn, QByteArray buffer);
	void recvCscsGroupEigenvalue(int stn, int group, QByteArray buffer);
	void recvCscsRecordEigenvalue(int stn, int group, int datatype, QByteArray buffer);

protected:
	virtual void recvCscsStruct(int flag, const CscsDatabase& db);
	virtual void recvCscsStationEigenvalue(int stn, int flag, QByteArray buffer);
	virtual void recvCscsGroupEigenvalue(int stn, int group, int flag, QByteArray buffer);
	virtual void recvCscsRecordEigenvalue(int stn, int group, int datatype, int flag, QByteArray buffer);

protected:
	bool sendMessage(QClient* client, const QByteArray& buffer, int msgType);

private:
	quint32		m_nUsrID;		//用户ID，用于查询目录信息
};

#endif // QRECEIVERINSPECT_H
