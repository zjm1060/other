#ifndef QVIEWCONTROLLER_H
#define QVIEWCONTROLLER_H

#include <QVector>
#include <QByteArray>

#include "qabstractcontroller.h"
#include "qutildef.h"

class UTIL_EXPORT QViewController : public QAbstractController
{
	Q_OBJECT

public:
	QViewController(QObject *parent=0);
	~QViewController();

public:
	Q_INVOKABLE void recvLocalServer(quint32 usrID, int flag, ServerInfo info);
	Q_INVOKABLE void recvLocalServerQuit(quint32 usrID);
	Q_INVOKABLE void recvInformStatusChanged(quint32 usrID, quint64 sign);

	Q_INVOKABLE void recvRemoteServerList(quint32 usrID, int flag, QList<ServerInfo> servers);

	Q_INVOKABLE void recvTopology(quint32 usrID, int flag, GraphTopo graph);

	Q_INVOKABLE void recvProcessor(quint32 usrID, int flag, QVector<ProcessorInfo> processors);
	Q_INVOKABLE void recvPerfProcessor(quint32 usrID, int flag, PerfProcessor processor);

	Q_INVOKABLE void recvPhysicalMemroy(quint32 usrID, int flag, QVector<PhysicalMemory> memories);
	Q_INVOKABLE void recvSystemMemory(quint32 usrID, int flag, SystemMemory memory);

	Q_INVOKABLE void recvDiskDrive(quint32 usrID, int flag, QVector<DiskDrive> drives);
	Q_INVOKABLE void recvCDROMDrive(quint32 usrID, int flag, QVector<CDROMDrive> drives);
	Q_INVOKABLE void recvLogicalDisk(quint32 usrID, int flag, QVector<LogicalDisk> disc);

	Q_INVOKABLE void recvOperatingSystem(quint32 usrID, int flag, OperatingSystem system);
	Q_INVOKABLE void recvComputerSystem(quint32 usrID, int flag, ComputerSystem system);

	Q_INVOKABLE void recvNetworkAdapter(quint32 usrID, int flag, QVector<NetworkAdapter> adapters);
	Q_INVOKABLE void recvSystemProcess(quint32 usrID, int flag, QVector<SystemProcess> processes, quint64 memory);

	Q_INVOKABLE void recvCreateProcess(quint32 usrID, QStringList command, int result);
	Q_INVOKABLE void recvTerminateProcess(quint32 usrID, quint32 pid, int result);

	Q_INVOKABLE void recvCheckPrivilege(quint32 usrID, int flag, const PrivilegeUser& user);
	Q_INVOKABLE void recvPrivileges(quint32 usrID, const PrivilegeGroups& groups);
	Q_INVOKABLE	void recvCreateGroup(quint32 usrID, int flag, const QString& name, const PrivilegeGroups& groups);
	Q_INVOKABLE void recvDeleteGroup(quint32 usrID, const QString& groupName, const PrivilegeGroups& groups);
	Q_INVOKABLE	void recvCreateUser(quint32 usrID, int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups);
	Q_INVOKABLE void recvDeleteUser(quint32 usrID, const QString& userName, const PrivilegeGroups& groups);
	Q_INVOKABLE void recvModifyUser(quint32 usrID, int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups);
	Q_INVOKABLE void recvModifyPassword(quint32 usrID, int flag, const QString& userName, const PrivilegeUser& user);

	Q_INVOKABLE void recvCscsStruct(quint32 usrID, int flag, CscsDatabase db);
	Q_INVOKABLE void recvCscsStationEigenvalue(quint32 usrID, int stn, int flag, QByteArray buffer);
	Q_INVOKABLE void recvCscsGroupEigenvalue(quint32 usrID, int stn, int group, int flag, QByteArray buffer);
	Q_INVOKABLE void recvCscsRecordEigenvalue(quint32 usrID, int stn, int group, int datatype, int flag, QByteArray buffer);
};

#endif // QVIEWCONTROLLER_H
