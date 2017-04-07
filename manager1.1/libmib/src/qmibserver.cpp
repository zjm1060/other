#include "qmibserver.h"

QMibServer::QMibServer()
:QAbstractMib(-1)
{
	setModel(ManualModel);
}

QMibServer::~QMibServer()
{

}

void QMibServer::setResult(const QVariant& var)
{
	if (var.isValid())
	{
		ServerInfo info=var.value<ServerInfo>();
		if (info != m_server)
		{
			m_server=info;

			emit changed();
		}
	}
}

REGISTER_MIB_INSTANCE(QMibServer)
