#include "qabstractreceiver.h"

QAbstractReceiver::QAbstractReceiver()
{

}

QAbstractReceiver::~QAbstractReceiver()
{
}

void QAbstractReceiver::recvLocalServer(int flag, const ServerInfo& info)
{
	Q_UNUSED(flag);
	Q_UNUSED(info);
}

void QAbstractReceiver::recvLocalServerQuit()
{

}

void QAbstractReceiver::recvInformStatusChanged(quint64 sign)
{
	Q_UNUSED(sign);
}

void QAbstractReceiver::recvRemoteServerList(int flag, const QList<ServerInfo>& servers)
{
	Q_UNUSED(flag);
	Q_UNUSED(servers);
}

void QAbstractReceiver::recvTopology(int flag, const GraphTopo& graph)
{
	Q_UNUSED(flag);
	Q_UNUSED(graph);
}

void QAbstractReceiver::recvProcessor(int flag, const QVector<ProcessorInfo>& processors)
{
	Q_UNUSED(flag);
	Q_UNUSED(processors);
}

void QAbstractReceiver::recvPerfProcessor(int flag, const PerfProcessor& processor)
{
	Q_UNUSED(flag);
	Q_UNUSED(processor);
}

void QAbstractReceiver::recvPhysicalMemroy(int flag, const QVector<PhysicalMemory>& memories)
{
	Q_UNUSED(flag);
	Q_UNUSED(memories);
}

void QAbstractReceiver::recvSystemMemory(int flag, const SystemMemory& memory)
{
	Q_UNUSED(flag);
	Q_UNUSED(memory);
}

void QAbstractReceiver::recvDiskDrive(int flag, const QVector<DiskDrive>& drives)
{
	Q_UNUSED(flag);
	Q_UNUSED(drives);
}

void QAbstractReceiver::recvCDROMDrive(int flag, const QVector<CDROMDrive>& drives)
{
	Q_UNUSED(flag);
	Q_UNUSED(drives);
}

void QAbstractReceiver::recvLogicalDisk(int flag, const QVector<LogicalDisk>& disc)
{
	Q_UNUSED(flag);
	Q_UNUSED(disc);
}

void QAbstractReceiver::recvOperatingSystem(int flag, const OperatingSystem& system)
{
	Q_UNUSED(flag);
	Q_UNUSED(system);
}

void QAbstractReceiver::recvComputerSystem(int flag, const ComputerSystem& system)
{
	Q_UNUSED(flag);
	Q_UNUSED(system);
}

void QAbstractReceiver::recvNetworkAdapter(int flag, const QVector<NetworkAdapter>& adapters)
{
	Q_UNUSED(flag);
	Q_UNUSED(adapters);
}

void QAbstractReceiver::recvSystemProcess(int flag, const QVector<SystemProcess>& processes, quint64 memory)
{
	Q_UNUSED(flag);
	Q_UNUSED(processes);
	Q_UNUSED(memory);
}

void QAbstractReceiver::recvCreateProcess(const QStringList& command, int result)
{
	Q_UNUSED(command);
	Q_UNUSED(result);
}

void QAbstractReceiver::recvTerminateProcess(quint32 pid, int result)
{
	Q_UNUSED(pid);
	Q_UNUSED(result);
}

void QAbstractReceiver::recvRoot(const QString& root)
{
	Q_UNUSED(root);
}

void QAbstractReceiver::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	Q_UNUSED(path);
	Q_UNUSED(flag);
	Q_UNUSED(files);
}

void QAbstractReceiver::recvDrives(const QVector<FileInfo>& drives)
{
	Q_UNUSED(drives);
}

void QAbstractReceiver::recvSymLink(int queryFlag, const FileInfo& info, const QString& target)
{
	Q_UNUSED(queryFlag);
	Q_UNUSED(info);
	Q_UNUSED(target);
}

void QAbstractReceiver::recvFileInfo(const QString& file, int flag, const FileInfo& info, const QString& savePath)
{
	Q_UNUSED(file);
	Q_UNUSED(flag);
	Q_UNUSED(info);
	Q_UNUSED(savePath);
}

void QAbstractReceiver::recvDownloadFileInfo(const QString& file, int queryFlag, const QString& savePath, int format, quint64 size, int flag)
{
	Q_UNUSED(file);
	Q_UNUSED(queryFlag);
	Q_UNUSED(savePath);
	Q_UNUSED(format);
	Q_UNUSED(size);
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvTransferFileInfo(int queryFlag, int flag, int size)
{
	Q_UNUSED(queryFlag);
	Q_UNUSED(flag);
	Q_UNUSED(size);
}

void QAbstractReceiver::recvDownloadDir(const QString& path, int queryFlag, const QString& savePath, int flag, const QVector<FileInfo>& files)
{
	Q_UNUSED(path);
	Q_UNUSED(queryFlag);
	Q_UNUSED(savePath);
	Q_UNUSED(flag);
	Q_UNUSED(files);
}

void QAbstractReceiver::recvDownloadSymLink(int queryFlag, int flag)
{
	Q_UNUSED(queryFlag);
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvUploadDirAndSymlink(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvUploadFile(int queryFlag, int flag, quint64 size)
{
	Q_UNUSED(queryFlag);
	Q_UNUSED(flag);
	Q_UNUSED(size);
}

void QAbstractReceiver::recvRename(int flag, const QString& dir, const QString& old, const QString& now)
{
	Q_UNUSED(dir);
	Q_UNUSED(old);
	Q_UNUSED(now);
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvNewDirectory(int flag, const FileInfo& file)
{
	Q_UNUSED(flag);
	Q_UNUSED(file);
}

void QAbstractReceiver::recvDelete(int flag, const QVector<QString>& files)
{
	Q_UNUSED(flag);
	Q_UNUSED(files);
}

void QAbstractReceiver::recvInterruptTransfer(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvConnectToTarget(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvUploadDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files)
{
	Q_UNUSED(flag);
	Q_UNUSED(sourcePath);
	Q_UNUSED(targetPath);
	Q_UNUSED(files);
}

void QAbstractReceiver::recvUploadSlientDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files)
{
	Q_UNUSED(flag);
	Q_UNUSED(sourcePath);
	Q_UNUSED(targetPath);
	Q_UNUSED(files);
}

void QAbstractReceiver::recvUploadSymlinkToTarget(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvUploadFileToTarget(int flag, quint64 size)
{
	Q_UNUSED(flag);
	Q_UNUSED(size);
}

void QAbstractReceiver::recvInterruptTransferToTarget(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvUploadSlientFile(int flag, quint64 size)
{
	Q_UNUSED(flag);
	Q_UNUSED(size);
}

void QAbstractReceiver::recvInformDdbConnection()
{

}

void QAbstractReceiver::recvPrivilegeData(int flag, const QMap<QString, PrivilegeGroup>& groups)
{
	Q_UNUSED(flag);
	Q_UNUSED(groups);
}

void QAbstractReceiver::recvCheckPrivilege(int flag, const PrivilegeUser& user)
{
	Q_UNUSED(flag);
	Q_UNUSED(user);
}

void QAbstractReceiver::recvPrivileges(const PrivilegeGroups& groups)
{
	Q_UNUSED(groups);
}

void QAbstractReceiver::recvCreateGroup(int flag, const QString& name, const PrivilegeGroups& groups)
{
	Q_UNUSED(flag);
	Q_UNUSED(name);
	Q_UNUSED(groups);
}

void QAbstractReceiver::recvDeleteGroup(const QString& groupName, const PrivilegeGroups& groups)
{
	Q_UNUSED(groupName)
	Q_UNUSED(groups);
}

void QAbstractReceiver::recvCreateUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups)
{
	Q_UNUSED(flag);
	Q_UNUSED(groupName);
	Q_UNUSED(userName);
	Q_UNUSED(groups);
}

void QAbstractReceiver::recvDeleteUser(const QString& userName, const PrivilegeGroups& groups)
{
	Q_UNUSED(userName);
	Q_UNUSED(groups);
}

void QAbstractReceiver::recvModifyUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups)
{
	Q_UNUSED(flag);
	Q_UNUSED(groupName);
	Q_UNUSED(userName);
	Q_UNUSED(groups);
}

void QAbstractReceiver::recvModifyPassword(int flag, const QString& userName, const PrivilegeUser& user)
{
	Q_UNUSED(flag);
	Q_UNUSED(userName);
	Q_UNUSED(user);
}

void QAbstractReceiver::recvCscsStruct(int flag, const CscsDatabase& db)
{
	Q_UNUSED(flag);
	Q_UNUSED(db);
}

void QAbstractReceiver::recvCscsStationEigenvalue(int stn, int flag, QByteArray buffer)
{
	Q_UNUSED(stn);
	Q_UNUSED(flag);
	Q_UNUSED(buffer);
}

void QAbstractReceiver::recvCscsGroupEigenvalue(int stn, int group, int flag, QByteArray buffer)
{
	Q_UNUSED(stn);
	Q_UNUSED(group);
	Q_UNUSED(flag);
	Q_UNUSED(buffer);
}

void QAbstractReceiver::recvCscsRecordEigenvalue(int stn, int group, int datatype, int flag, QByteArray buffer)
{
	Q_UNUSED(stn);
	Q_UNUSED(group);
	Q_UNUSED(datatype);
	Q_UNUSED(flag);
	Q_UNUSED(buffer);
}

void QAbstractReceiver::recvBackup(int flag, int finished, int success)
{
	Q_UNUSED(flag);
	Q_UNUSED(finished);
	Q_UNUSED(success);
}

void QAbstractReceiver::recvSyncAssociationConfigure(SyncCommands commands)
{
	Q_UNUSED(commands);
}

void QAbstractReceiver::recvWriteSyncAssociationConfigure(int flag)
{
	Q_UNUSED(flag);
}

void QAbstractReceiver::recvCommandKillProcessFail(QString processName, quint32 processId, QString path)
{
	Q_UNUSED(processName);
	Q_UNUSED(processId);
	Q_UNUSED(path);
}

void QAbstractReceiver::recvCommandCreateProcessFail(QString processName)
{
	Q_UNUSED(processName);
}

void QAbstractReceiver::recvCommandFoundProcessesFail()
{
	
}

void QAbstractReceiver::recvCommandFoundCommandFail()
{

}

void QAbstractReceiver::recvCommandExecCommand(int flag, QString command)
{
	Q_UNUSED(flag);
	Q_UNUSED(command);
}

void QAbstractReceiver::recvCommandFinish()
{

}
