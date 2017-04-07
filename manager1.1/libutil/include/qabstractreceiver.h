#ifndef QABSTRACTRECEIVER_H
#define QABSTRACTRECEIVER_H

#include "qutildef.h"

class UTIL_EXPORT QAbstractReceiver
{
public:
	QAbstractReceiver();
	virtual ~QAbstractReceiver();

public:
	virtual void recvLocalServer(int flag, const ServerInfo& info);
	virtual void recvLocalServerQuit();
	virtual void recvInformStatusChanged(quint64 sign);

	virtual void recvRemoteServerList(int flag, const QList<ServerInfo>& servers);

	virtual void recvTopology(int flag, const GraphTopo& graph);

	virtual void recvProcessor(int flag, const QVector<ProcessorInfo>& processors);
	virtual void recvPerfProcessor(int flag, const PerfProcessor& processor);

	virtual void recvPhysicalMemroy(int flag, const QVector<PhysicalMemory>& memories);
	virtual void recvSystemMemory(int flag, const SystemMemory& memory);

	virtual void recvDiskDrive(int flag, const QVector<DiskDrive>& drives);
	virtual void recvCDROMDrive(int flag, const QVector<CDROMDrive>& drives);
	virtual void recvLogicalDisk(int flag, const QVector<LogicalDisk>& disc);

	virtual void recvOperatingSystem(int flag, const OperatingSystem& system);
	virtual void recvComputerSystem(int flag, const ComputerSystem& system);

	virtual void recvNetworkAdapter(int flag, const QVector<NetworkAdapter>& adapters);
	virtual void recvSystemProcess(int flag, const QVector<SystemProcess>& processes, quint64 memory);
	virtual void recvCreateProcess(const QStringList& command, int result);
	virtual void recvTerminateProcess(quint32 pid, int result);

	virtual void recvRoot(const QString& root);
	virtual void recvDrives(const QVector<FileInfo>& drives);
	virtual void recvSymLink(int queryFlag, const FileInfo& info, const QString& target);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvFileInfo(const QString& file, int flag, const FileInfo& info, const QString& savePath);
	virtual void recvDownloadFileInfo(const QString& file, int queryFlag, const QString& savePath, int format, quint64 size, int flag);
	virtual void recvTransferFileInfo(int queryFlag, int flag, int size);
	virtual void recvDownloadDir(const QString& path, int queryFlag, const QString& savePath, int flag, const QVector<FileInfo>& files);
	virtual void recvDownloadSymLink(int queryFlag, int flag);
	virtual void recvUploadDirAndSymlink(int flag);
	virtual void recvUploadFile(int queryFlag, int flag, quint64 size);
	virtual void recvRename(int flag, const QString& dir, const QString& old, const QString& now);
	virtual void recvNewDirectory(int flag, const FileInfo& file);
	virtual void recvDelete(int flag, const QVector<QString>& files);
	virtual void recvInterruptTransfer(int flag);

	virtual void recvConnectToTarget(int flag);
	virtual void recvUploadDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files);
	virtual void recvUploadSlientDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files);
	virtual void recvUploadSymlinkToTarget(int flag);
	virtual void recvUploadFileToTarget(int flag, quint64 size);
	virtual void recvInterruptTransferToTarget(int flag);

	virtual void recvUploadSlientFile(int flag, quint64 size);

	virtual void recvBackup(int flag, int finished, int success);

	virtual void recvSyncAssociationConfigure(SyncCommands commands);
	virtual void recvWriteSyncAssociationConfigure(int flag);

	virtual void recvCommandKillProcessFail(QString processName, quint32 processId, QString path);
	virtual void recvCommandCreateProcessFail(QString processName);
	virtual void recvCommandFoundProcessesFail();
	virtual void recvCommandFoundCommandFail();
	virtual void recvCommandExecCommand(int flag, QString command);
	virtual void recvCommandFinish();

	//分布式数据库消息
	virtual void recvInformDdbConnection();
	virtual void recvPrivilegeData(int flag, const QMap<QString, PrivilegeGroup>& groups);
	virtual void recvCheckPrivilege(int flag, const PrivilegeUser& user);
	virtual void recvPrivileges(const PrivilegeGroups& groups);
	virtual void recvCreateGroup(int flag, const QString& name, const PrivilegeGroups& groups);
	virtual void recvDeleteGroup(const QString& groupName, const PrivilegeGroups& groups);
	virtual void recvCreateUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups);
	virtual void recvDeleteUser(const QString& userName, const PrivilegeGroups& groups);
	virtual void recvModifyUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups);
	virtual void recvModifyPassword(int flag, const QString& userName, const PrivilegeUser& user);

	//巡检
	virtual void recvCscsStruct(int flag, const CscsDatabase& db);
	virtual void recvCscsStationEigenvalue(int stn, int flag, QByteArray buffer);
	virtual void recvCscsGroupEigenvalue(int stn, int group, int flag, QByteArray buffer);
	virtual void recvCscsRecordEigenvalue(int stn, int group, int datatype, int flag, QByteArray buffer);
};

#endif // QABSTRACTRECEIVER_H
