#include "qviewcontroller.h"

#include "qimessage.h"
#include "qreceiverparser.h"

QViewController::QViewController(QObject *parent)
	: QAbstractController(parent)
{
}

QViewController::~QViewController()
{

}

void QViewController::recvLocalServer(quint32 usrID, int flag, ServerInfo info)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvLocalServer(flag, info);
	}
}

void QViewController::recvLocalServerQuit(quint32 usrID)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvLocalServerQuit();
	}
}

void QViewController::recvInformStatusChanged(quint32 usrID, quint64 sign)
{
	if (usrID != QReceiverParser::AllUser)
	{
		QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
		if (pInterface != NULL)
		{
			pInterface->recvInformStatusChanged(sign);
		}
	}
	else
	{
		QList<quint32> keys=QReceiverParser::instance()->instanceInterfaces();
		QAbstractReceiver* pInterface;
		foreach (quint32 key, keys)
		{
			pInterface=QReceiverParser::instance()->instanceInterface(key);
			if (pInterface != NULL)
			{
				pInterface->recvInformStatusChanged(sign);
			}
		}
	}
}

void QViewController::recvRemoteServerList(quint32 usrID, int flag, QList<ServerInfo> servers)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvRemoteServerList(flag, servers);
	}
}

void QViewController::recvTopology(quint32 usrID, int flag, GraphTopo graph)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvTopology(flag, graph);
	}
}

void QViewController::recvProcessor(quint32 usrID, int flag, QVector<ProcessorInfo> processors)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvProcessor(flag, processors);
	}
}

void QViewController::recvPerfProcessor(quint32 usrID, int flag, PerfProcessor processor)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvPerfProcessor(flag, processor);
	}
}

void QViewController::recvPhysicalMemroy(quint32 usrID, int flag, QVector<PhysicalMemory> memories)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvPhysicalMemroy(flag, memories);
	}
}

void QViewController::recvSystemMemory(quint32 usrID, int flag, SystemMemory memory)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvSystemMemory(flag, memory);
	}
}

void QViewController::recvDiskDrive(quint32 usrID, int flag, QVector<DiskDrive> drives)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDiskDrive(flag, drives);
	}
}

void QViewController::recvCDROMDrive(quint32 usrID, int flag, QVector<CDROMDrive> drives)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCDROMDrive(flag, drives);
	}
}

void QViewController::recvLogicalDisk(quint32 usrID, int flag, QVector<LogicalDisk> disc)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvLogicalDisk(flag, disc);
	}
}

void QViewController::recvOperatingSystem(quint32 usrID, int flag, OperatingSystem system)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvOperatingSystem(flag, system);
	}
}

void QViewController::recvComputerSystem(quint32 usrID, int flag, ComputerSystem system)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvComputerSystem(flag, system);
	}
}

void QViewController::recvNetworkAdapter(quint32 usrID, int flag, QVector<NetworkAdapter> adapters)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvNetworkAdapter(flag, adapters);
	}
}

void QViewController::recvSystemProcess(quint32 usrID, int flag, QVector<SystemProcess> processes, quint64 memory)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvSystemProcess(flag, processes, memory);
	}
}

void QViewController::recvCreateProcess(quint32 usrID, QStringList command, int result)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCreateProcess(command, result);
	}
}

void QViewController::recvTerminateProcess(quint32 usrID, quint32 pid, int result)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvTerminateProcess(pid, result);
	}
}

void QViewController::recvCheckPrivilege(quint32 usrID, int flag, const PrivilegeUser& user)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCheckPrivilege(flag, user);
	}
}

void QViewController::recvPrivileges(quint32 usrID, const PrivilegeGroups& groups)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvPrivileges(groups);
	}
}

void QViewController::recvCreateGroup(quint32 usrID, int flag, const QString& name, const PrivilegeGroups& groups)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCreateGroup(flag, name, groups);
	}
}

void QViewController::recvDeleteGroup(quint32 usrID, const QString& groupName, const PrivilegeGroups& groups)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDeleteGroup(groupName, groups);
	}
}

void QViewController::recvCreateUser(quint32 usrID, int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCreateUser(flag, groupName, userName, groups);
	}
}

void QViewController::recvDeleteUser(quint32 usrID, const QString& userName, const PrivilegeGroups& groups)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvDeleteUser(userName, groups);
	}
}

void QViewController::recvModifyUser(quint32 usrID, int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvModifyUser(flag, groupName, userName, groups);
	}
}

void QViewController::recvModifyPassword(quint32 usrID, int flag, const QString& userName, const PrivilegeUser& user)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvModifyPassword(flag, userName, user);
	}
}

void QViewController::recvCscsStruct(quint32 usrID, int flag, CscsDatabase db)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCscsStruct(flag, db);
	}
}

void QViewController::recvCscsStationEigenvalue(quint32 usrID, int stn, int flag, QByteArray buffer)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCscsStationEigenvalue(stn, flag, buffer);
	}
}

void QViewController::recvCscsGroupEigenvalue(quint32 usrID, int stn, int group, int flag, QByteArray buffer)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCscsGroupEigenvalue(stn, group, flag, buffer);
	}
}

void QViewController::recvCscsRecordEigenvalue(quint32 usrID, int stn, int group, int datatype, int flag, QByteArray buffer)
{
	QAbstractReceiver* pInterface=QReceiverParser::instance()->instanceInterface(usrID);
	if (pInterface != NULL)
	{
		pInterface->recvCscsRecordEigenvalue(stn, group, datatype, flag, buffer);
	}
}
