#include "qmibremoteserver.h"

QMibRemoteServer::QMibRemoteServer()
:QAbstractMib(-1)
{
}

QMibRemoteServer::~QMibRemoteServer()
{

}

void QMibRemoteServer::setResult(const QVariant& var)
{
	if (var.isValid())
	{
		bool bChanged;

		MibRemoteServer info=var.value<MibRemoteServer>();
		if (info.coming)
		{
			bChanged=true;
			if (m_servers.contains(info.info.hostName))
			{
				bool bFind=false;
				QMultiHash<QString, ServerInfo>::iterator it=m_servers.find(info.info.hostName);
				while (it != m_servers.end() && it.key() == info.info.hostName)
				{
					if (info.info == it.value())
					{
						bFind=true;

						it.value()=info.info;

						bChanged=false;
						break;
					}

					it++;
				}

				if (!bFind)
				{
					m_servers.insert(info.info.hostName, info.info);
				}
			}
			else
			{
				m_servers.insert(info.info.hostName, info.info);
			}
		}
		else
		{
			bChanged=false;
			QMultiHash<QString, ServerInfo>::iterator it=m_servers.find(info.info.hostName);
			while (it != m_servers.end() && it.key() == info.info.hostName)
			{
				if (info.info == it.value())
				{
					it=m_servers.erase(it);

					bChanged=true;
					break;
				}

				it++;
			}
		}
		
		if (bChanged)
		{
			emit changed();
		}
	}
}

REGISTER_MIB_INSTANCE(QMibRemoteServer)
