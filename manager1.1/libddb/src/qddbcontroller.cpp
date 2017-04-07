#include "qddbcontroller.h"

#include "qreceiverparser.h"

QDDBController::QDDBController(QObject *parent)
	: QAbstractController(parent)
{
}

QDDBController::~QDDBController()
{

}

void QDDBController::recvInformDdbConnection(quint32 usrID)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvInformDdbConnection();
	}
}

void QDDBController::recvPrivilegeData(quint32 usrID, int flag, const QMap<QString, PrivilegeGroup>& groups)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvPrivilegeData(flag, groups);
	}
}

