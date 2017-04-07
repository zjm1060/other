#include "qreceiverinspect.h"

#include "qlogger.h"
#include "qreceiverparser.h"
#include "qmessageparser.h"

QReceiverInspect::QReceiverInspect(QObject *parent)
	: QObject(parent)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);
}

QReceiverInspect::~QReceiverInspect()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
	m_nUsrID=QReceiverParser::AllUser;
}

bool QReceiverInspect::queryDatabase(QClient* client)
{
	if (client == NULL)
	{
		return false;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CSCS);
	if (pMsg == NULL)
	{
		logplusError(MANAGER_LOGGER_STR, QString("Failed to parse the message type=%1").arg(MSG_CSCS));
		return false;
	}

	QByteArray buffer=pMsg->query(QueryCscsStruct, m_nUsrID);

	return sendMessage(client, buffer, MSG_CSCS);
}

bool QReceiverInspect::queryDataSource(QClient* client, int msg, int ack, const QByteArray& buffer)
{
	if (client == NULL)
	{
		return false;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(msg);
	if (pMsg == NULL)
	{
		logplusError(MANAGER_LOGGER_STR, QString("Failed to parse the message type=%1").arg(msg));
		return false;
	}

	QByteArray buf=pMsg->query(ack, m_nUsrID, buffer);

	return sendMessage(client, buf, msg);
}

bool QReceiverInspect::sendMessage(QClient* client, const QByteArray& buffer, int msgType)
{
	Q_ASSERT_NO_NULL(client);

	return client->sendMessage(buffer, msgType);
}

void QReceiverInspect::recvCscsStruct(int flag, const CscsDatabase& db)
{
	if (flag == CscsDataSourceLost)
	{
		emit cscsDataSourceLost();
	}
	else if (flag == CscsDataSourceFound)
	{
		emit recvCscsDatabase(db);
	}
}

void QReceiverInspect::recvCscsStationEigenvalue(int stn, int flag, QByteArray buffer)
{
	if (flag == CscsDataSourceFound)
	{
		emit recvCscsStationEigenvalue(stn, buffer);
	}
	else if (flag == CscsDataSourceLost)
	{
		emit cscsDataSourceLost();
	}
	else if (flag == CscsDataSourceNoExisted)
	{
		emit cscsDataSourceNoExisted();
	}
}

void QReceiverInspect::recvCscsGroupEigenvalue(int stn, int group, int flag, QByteArray buffer)
{
	if (flag == CscsDataSourceFound)
	{
		emit recvCscsGroupEigenvalue(stn, group, buffer);
	}
	else if (flag == CscsDataSourceLost)
	{
		emit cscsDataSourceLost();
	}
	else if (flag == CscsDataSourceNoExisted)
	{
		emit cscsDataSourceNoExisted();
	}
}

void QReceiverInspect::recvCscsRecordEigenvalue(int stn, int group, int datatype, int flag, QByteArray buffer)
{
	if (flag == CscsDataSourceFound)
	{
		emit recvCscsRecordEigenvalue(stn, group, datatype, buffer);
	}
	else if (flag == CscsDataSourceLost)
	{
		emit cscsDataSourceLost();
	}
	else if (flag == CscsDataSourceNoExisted)
	{
		emit cscsDataSourceNoExisted();
	}
}
