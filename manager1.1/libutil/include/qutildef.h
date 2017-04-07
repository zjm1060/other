#ifndef QUTILDEF_H
#define QUTILDEF_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QFileInfo>
#include <QIcon>
#include <QMetaType>
#include <QMap>
#include <QPair>

#include "global.h"
#include "qsharedobject.h"

template<class T>
class QRegisterInstances
{
public:
	QRegisterInstances(){};
	~QRegisterInstances(){};

public:
	static QRegisterInstances* instance()
	{
		static QRegisterInstances* g_pInstances=new QRegisterInstances;

		return g_pInstances;
	}

public:
	void addInterface(const T& ptr)
	{
		if (ptr != NULL)
		{
			m_instances.append(ptr);
		}
	}

	const QVector<T>& interfaces() const
	{
		return m_instances;
	}

private:
	QVector<T> m_instances;
};

template<class T, typename PT>
class QRegisterInterface
{
public:
	QRegisterInterface()
	{
		QRegisterInstances<PT>::instance()->addInterface(PT(new T));
	}
}; 

template<typename T, class P>
class QRegisterClassProducer
{
public:
	QRegisterClassProducer(){};
	~QRegisterClassProducer(){};

public:
	static QRegisterClassProducer* instance()
	{
		static QRegisterClassProducer* g_pInstances=new QRegisterClassProducer;

		return g_pInstances;
	}

public:
	void registerProducer(const T& type, const P& ptr)
	{
		if (ptr != NULL)
		{
			m_instances.insert(type, ptr);
		}
	}

	const P producer(const T& type)
	{
		return m_instances[type];
	}

private:
	QMap<T, P> m_instances;
};

template<class T>
class QAbstractClassProducer : public QSharedObject
{
public:
	QAbstractClassProducer(){};
	~QAbstractClassProducer(){};

public:
	virtual T* produce()=0;
};

template<class T, typename QTemplateClassProducer>
class QClassProducer : public QTemplateClassProducer
{
public:
	QClassProducer(){};
	~QClassProducer(){};

public:
	virtual T* produce()
	{
		return new T;
	}
};

enum UtilType
{
	UTIL_TYPE_LOCAL_SERVER,
	UTIL_TYPE_TOPOLOGY,
	UTIL_TYPE_REMOTE_SERVER,
	UTIL_TYPE_PROCESSOR,
	UTIL_TYPE_PERF_PROCESSOR,
	UTIL_TYPE_SYSTEM,
	UTIL_TYPE_COMPUTER_SYSTEM,
	UTIL_TYPE_NETWORK_ADAPTER,
	UTIL_TYPE_PROCESS,
	UTIL_TYPE_PERF_PROC_PROCESS,
	UTIL_TYPE_DISK_DRIVE,
	UTIL_TYPE_CDROM_DRIVE,
	UTIL_TYPE_LOGICAL_DISK,
	UTIL_TYPE_PHYSICAL_MEMORY,
	UTIL_TYPE_SYSTEM_MEMORY,
	UTIL_TYPE_END
};

enum OSType
{
	OS_WINDOWS,
	OS_SOLARIS,
	OS_AIX
};

#define SERVER_HOST_IP_SEPARATOR	"@"
#define FILE_MODIFIED_DATETIME_FORMAT	"yyyy-MM-dd hh:mm:ss"
#define STANDARD_DATETIME_FORMAT	"yyyy-MM-dd hh:mm:ss"

typedef struct _server 
{
	QString		addr;			//服务器ip地址
	quint32		netmask;		//子网掩码
	quint16		port;			//服务器端口号,使用同一端口
}Server;

typedef struct _serverInfo
{
	QString		hostName;
	QString		usrName;

	QString		home;			//程序主目录

	int	count;
	QMap<QString, QVector<Server> > servers;	//以物理MAC地址作为key
}ServerInfo;

UTIL_EXPORT bool operator==(const Server& source, const Server& target);
UTIL_EXPORT bool operator==(const ServerInfo& source, const ServerInfo& target);
UTIL_EXPORT bool operator!=(const ServerInfo& source, const ServerInfo& target);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const ServerInfo& info);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, ServerInfo& info);

typedef struct _mibRemoteServer
{
	ServerInfo	info;

	bool coming;
}MibRemoteServer;

typedef struct _processorInfo			//处理器信息
{
	int os;

	//windows
	quint32 configManager;		//配置管理状态

	//aix
	QString status;

	//all
	int core;					//核心数
	quint16	addressWidth;		//地址字长
	quint16	dataWidth;			//处理器数据宽度

	QString description;
	QString deviceID;			//处理器的唯一标识
	quint32 externalClock;		//外部时钟频率MHz
	quint32 L2CacheSize;
	quint32 L2CacheSpeed;		//MHz
	QString manufacturer;
	quint32 maxClockSpeed;		//MHz
	QString name;				//处理器名称
}ProcessorInfo ;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const ProcessorInfo& processor);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, ProcessorInfo& processor);

typedef struct _perfProcessorCore 
{
	QString name;
	double percentProcessorTime;
}PerfProcessorCore;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const PerfProcessorCore& core);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, PerfProcessorCore& core);

typedef struct _perfProcessor
{
	quint32 currentClockSpeed;	//MHz
	qreal	currentVoltage;		//0~6位数值/10表示电压
	QString status;				//运行状态

	QVector<PerfProcessorCore> core;
}PerfProcessor;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const PerfProcessor& perf);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, PerfProcessor& perf);

typedef struct _operatingSystem
{
	QString caption;		//系统名称
	QString version;		//系统版本
	QString buildNumber;	//创建编号
	QString CSDVersion;		//service pack信息
	QString OSArchitecture;	//系统类型
}OperatingSystem;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const OperatingSystem& system);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, OperatingSystem& system);

typedef struct _computerSystem
{
	quint32 numberOfProcessors;
	quint64 totalPhysicalMemory;

	QString name;			//机器名称
	QString workgroup;		//工作组
	QString domain;			//域

	QString processor;		//处理器
}ComputerSystem;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const ComputerSystem& system);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, ComputerSystem& system);

typedef struct _networkAdapter
{
	QString description;
	QVector<QString> defaultIPGateway;
	QVector<QString> DNSServer;
	QVector<quint16> gatewayCostMetric;
	QVector<QString> IPAddress;
	QVector<QString> IPSubnet;
	QString MACAddress;
}NetworkAdapter;

UTIL_EXPORT bool operator!=(const NetworkAdapter& source, const NetworkAdapter& target);
UTIL_EXPORT bool operator==(const NetworkAdapter& source, const NetworkAdapter& target);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const NetworkAdapter& adapter);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, NetworkAdapter& adapter);

#if defined(Q_OS_AIX) || defined(Q_OS_SOLARIS)
UTIL_EXPORT QVector<NetworkAdapter> findNetworkAdapter();
#endif

typedef struct _systemProcess
{
	QString usrName;
	QString	executablePath;
	QString name;
	quint32 processId;
	quint32 sessionId;
	quint64 workingSetSize;
	quint64 processorTime;	//占用cpu时间
	qreal	percentProcessorTime;
}SystemProcess;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const SystemProcess& process);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, SystemProcess& process);

typedef struct _fileInfo 
{
	int type;
	QString name;
	quint64 size;
	bool isReadable;
	bool isWritable;
	bool isExecutable;
	QDateTime lastModified;
}FileInfo;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const FileInfo& info);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, FileInfo& info);
UTIL_EXPORT bool operator==(const FileInfo& source, const FileInfo& target);
UTIL_EXPORT bool operator!=(const FileInfo& source, const FileInfo& target);

typedef struct _diskDrive
{
	int os;					//系统类型

	//windows
	quint32 configManager;		//运行状态
	quint32 partitions;			//分区数
	quint32 numberOfMediaSupported;
	quint32 SCSIBus;
	quint16 SCSILogicalUnit;
	quint16 SCSITargetId;
	quint16 SCSIPort;
	QString mediaType;

	//aix
	QString status;
	QString serialNumber;
	QString position;

	//all
	QString description;
	QString deviceID;
	QString interfaceType;
	QString manufacturer;

	quint64 size;				//字节
	QString model;
}DiskDrive;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const DiskDrive& disk);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, DiskDrive& disk);

typedef struct _CDROMDrive 
{
	int os;

	//windows
	quint32 configManager;		//运行状态
	QString drive;				//盘符
	quint32 numberOfMediaSupported;
	quint32 SCSIBus;
	quint16 SCSILogicalUnit;
	quint16 SCSITargetId;
	quint16 SCSIPort;
	quint64 size;
	QString mediaType;

	//aix
	QString status;
	QString position;

	//all
	QString description;
	QString deviceID;
	QString manufacturer;

	qreal	transferRate;		//单位KB/s
	QString volumeName;
	QString volumeSerialNumber;
	QString name;
}CDROMDrive;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const CDROMDrive& disk);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, CDROMDrive& disk);

typedef struct _logicalDisk
{
	QString description;
	QString deviceID;
	quint32 driveType;
	QString driveTypeDesc;
	QString fileSystem;
	quint64 freeSpace;
	quint64 size;
}LogicalDisk;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const LogicalDisk& disk);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, LogicalDisk& disk);

typedef struct _physicalMemory
{
	quint64 capacity;
	quint16 dataWidth;
	QString description;
	QString manufacturer;
	QString memoryType;
	QString name;
	QString serialNumber;
	quint32 speed;
	QString tag;
	QString version;
}PhysicalMemory;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const PhysicalMemory& memory);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, PhysicalMemory& memory);

typedef struct _systemMemory 
{
	QDateTime datetime;		//获取内存信息时间

	//单位KB
	quint64 freePhysicalMemory;
	quint64 freeVirtualMemory;

	quint64 totalVisibleMemorySize;
	quint64 totalVirtualMemorySize;

	quint64 freeSpaceInPagingFiles;
	quint64 sizeStoredInPagingFiles;
	quint64 sizeUsageInPagingFiles;
	quint64 sizeUsageInPagingFilesPeak;

	quint64 systemCache;

	quint64 poolNonpaged;
	quint64 poolPaged;
}SystemMemory;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const SystemMemory& memory);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, SystemMemory& memory);

enum NodeType
{
	HoldNode=1,	//占位符
	RouterNode=(1<<1),
	SwitchL3Node=(1<<2),
	SwitchNode=(1<<3),
	FirewallNode=(1<<4),
	HubNode=(1<<5),
	HostNode=(1<<6),
	LeafNode=(FirewallNode|HostNode),
	LogicalNode=(HubNode),
	AllNode=0xff
};

typedef struct _graphNodeIpEntAddr 
{
	QString address;	//IP地址
	int ifIndex;		//接口索引号
	QString netmask;	//子网掩码
}GraphNodeIpEntAddr;

typedef struct _graphNodeIpRoute 
{
	QString dest;
	int ifIndex;
	QString nextHop;
	int routeType;		//3=direct 直连子网,4=indirect 连接另一个路由器
	QString routeMask;
}GraphNodeIpRoute;

typedef struct _graphNodeIpNetToMedia 
{
	int ifIndex;
	QString physAddress;
	QString netAddress;
}GraphNodeIpNetToMedia;

typedef struct _graphNodeInterface
{
	int ifIndex;	//接口索引
	QString	desc;	//接口描述信息
	int type;		//接口类型，6=ethernetCsmacd，53=propVirtual，1=other
	quint32	speed;	//接口速度
	QString physAddress;	//接口MAC地址
	int operStatus;		//1=up, 2=down

	QVector<QString>	addresses;	//接口绑定的IP地址
}GraphNodeInterface;

typedef struct _graphNodeBridgeBasePort 
{
	int port;			//端口号
	int ifIndex;	//端口号对应的接口索引
}GraphNodeBridgeBasePort;

typedef struct _graphNodeBridgeBase 
{
	QString address;	//MAC地址，交换机最小MAC地址
	int numPorts;		//交换机端口总数
	int type;			//

	QMap<int, GraphNodeBridgeBasePort>	portTable;
}GraphNodeBridgeBase;

typedef struct _graphNodeFdb 
{
	QString address;	//MAC地址
	int port;			//端口号
	int status;		//1=other,2=invalid,3=learned,4=self,5=mgmt
}GraphNodeFdb;

#define GRAPH_TOPO_INVALID_NODE	(quint64)-1		//无效节点标识
#define GRAPH_TOPO_INVALID_SEGMENT	(quint32)-1	//无效网段
#define GRAPH_TOPO_INVALID_IP_ADDRESS	(quint32)-1
#define GRAPH_TOPO_ROOT_HOLD_NODE	0			//拓扑图根节点

typedef struct _graphTopoNode GraphTopoNode;
struct _graphTopoNode
{
	quint64 uid;	//唯一标识,若有mac地址，则为其最小MAC地址，否则低48位为其连接设备的最小MAC地址，高16位为其连接的设备个数

	int type;		//节点类型
	QString	name;	//设备名称
	QString oid;	//设备对象ID
	QString desc;	//设备描述
	quint32 services;	//设备提供的七层服务标识

	quint32	ifNumber;	//设备接口总数
	quint32 ipForwarding;	//1:forwarding 具有向前转发数据包的功能（判断是路由器或三层交换机的必要条件）	2:notForwarding

	QMap<QString, GraphNodeIpEntAddr>	ipEntAddrs;		//IP地址对应表，可获取子网信息
	QMap<int, GraphNodeIpRoute>		ipRoutes;		//路由信息,key:ifIndex,value:路由信息
	QMap<QString, GraphNodeIpNetToMedia>	ipMedia;	//arp信息

	QMap<int, GraphNodeInterface>	interfaces;		//交换机或路由器的接口信息,key:ifIndex,vale:接口信息

	GraphNodeBridgeBase	bridgeBase;	//交换机端口与接口对应信息
	QMap<int, GraphNodeFdb>	fdb;	//交换机AFT表信息，key:端口号，value:邻接信息表

	QMap<int, QPair<quint64, int> >	peers;	//key=接口号,value=<唯一标识,接口号>
};

typedef struct _graphTopo
{
	GraphTopoNode root;		//网络拓扑初始占位节点，从此节点遍历整个拓扑图
	QMap<quint64, QPair<GraphTopoNode, int> >	nodes;	//网络拓扑图为无向图结构,quint64=uid, int=ticks心跳计数，<>0时表示节点离线
}GraphTopo;

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const GraphNodeInterface& face);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, GraphNodeInterface& face);

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const GraphTopoNode& node);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, GraphTopoNode& node);

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const GraphTopo& graph);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, GraphTopo& graph);

UTIL_EXPORT FileInfo entryInfo(const QFileInfo& info, bool isDrive);
UTIL_EXPORT int entryInfoFileType(const QFileInfo& info);
UTIL_EXPORT QString fileType(const QFileInfo& info);

UTIL_EXPORT void entryInfoList(const QFileInfoList& list, QVector<FileInfo>& files, bool isDrive);
UTIL_EXPORT bool deleteDir(const QString& path);
UTIL_EXPORT int readContents(QFile* file, QByteArray& buffer, bool highRate=false);

typedef struct _transferFileItem 
{
	QString name;
	quint64 size;
	int type;
	QString typeDesc;
}TransferFileItem;

UTIL_EXPORT bool operator==(const TransferFileItem& source, const TransferFileItem& target);
UTIL_EXPORT bool operator!=(const TransferFileItem& source, const TransferFileItem& target);

#ifdef Q_WS_WIN
#ifndef Q_OS_WINCE
UTIL_EXPORT QPixmap convertHIconToPixmap( const HICON icon);
#else
UTIL_EXPORT QPixmap convertHIconToPixmap( const HICON icon, bool large = false);
#endif
#endif

UTIL_EXPORT QIcon fileIcon(const FileInfo& info);
UTIL_EXPORT QIcon fileIcon(int type);
UTIL_EXPORT QIcon fileIcon(const QString& filename, int type);
UTIL_EXPORT quint64 fileSizeForNumber(const FileInfo& info);
UTIL_EXPORT QString fileSize(const FileInfo& info);
UTIL_EXPORT QString fileSize(quint64 size);
UTIL_EXPORT QString fileType(const FileInfo& info);
UTIL_EXPORT bool isFile(int type);
UTIL_EXPORT bool isDir(int type);

UTIL_EXPORT QString configManagerErrorCode(quint32 configManager);

#define HMAN_PRIVILEGE_FILE	"/ini/authority"
#define HMAN_PRIVILEGE_FILE_HEADER	"PRIVILEGE"
#define HMAN_PRIVALEGE_DEFAULT_GROUP_NAME	"Administrator"
#define HMAN_PRIVALEGE_DEFAULT_USER_NAME	"root"
#define HMAN_PRIVALEGE_DEFAULT_USER_PASSWD	"root"

//用户权限
typedef struct _privilegeUser
{
	QString	datetime;	//时间
	QString name;		//用户名
	QString passwd;		//密码

	QMap<int, quint32> privileges;	//权限
}PrivilegeUser;

enum EAuthorityType
{
	PrivilegeRole,
	ProcessPrivilege,
	FileTransferPrivilege,
	FileSynchronizationPrivilege
};

enum EAuthorityFlag
{
	AdministratorRole=1,
	GuestRole=1<<1,

	CreateProcessPrivilege=1,
	TerminateProcessPrivilege=1<<1,

	UploadFileAndFolderPrivilege=1,
	DownloadFileAndFolderPrivilege=1<<1,

	CommitFilePrivilege=1,
	UpdateFilePrivilege=1<<1
};

UTIL_EXPORT bool checkPrivileges(const QMap<int, quint32>& privileges, int type, int flag);

typedef struct _privilegeGroup
{
	QString	datetime;	//时间
	QString name;		//组名

	QMap<QString, PrivilegeUser>	usrs;	//属于组的所有用户,key=用户名
}PrivilegeGroup;

enum EPrivilegeOperation
{
	PrivilegeDeleteGroup,
	PrivilegeDeleteUser
};

typedef struct _privilegeOperation 
{
	QString uid;		//分层使用,/分隔符
	QString datetime;	//操作时间

	int operation;		//操作类型
}PrivilegeOperation;

typedef struct _privilegeGroups 
{
	QMap<QString, PrivilegeGroup>		groups;		//所有组,key=组名
	QList<PrivilegeOperation>			operations;	//操作
}PrivilegeGroups;

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, PrivilegeUser& user);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const PrivilegeUser& user);

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, PrivilegeGroup& group);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const PrivilegeGroup& group);

UTIL_EXPORT bool operator==(const PrivilegeGroup& source, const PrivilegeGroup& target);
UTIL_EXPORT bool operator!=(const PrivilegeGroup& source, const PrivilegeGroup& target);

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, PrivilegeOperation& operation);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const PrivilegeOperation& operation);

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, PrivilegeGroups& groups);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const PrivilegeGroups& groups);

//与H9000巡检有关
typedef struct _cscsDataType 
{
	uchar dataType;
	QString longname;
}CscsDataType;

typedef struct _cscsGroup
{
	uchar grpId;
	QString longname;

	QVector<CscsDataType> types;
}CscsGroup;

typedef struct _cscsStation 
{
	uchar stnId;
	QString longname;

	QVector<CscsGroup> groups;
}CscsStation;

typedef struct _cscsDatabase
{
	QVector<CscsStation> stations;
}CscsDatabase;

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, CscsDatabase& data);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const CscsDatabase& data);

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, CscsStation& data);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const CscsStation& data);

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, CscsGroup& data);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const CscsGroup& data);

UTIL_EXPORT QDataStream& operator>>(QDataStream& in, CscsDataType& data);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const CscsDataType& data);

typedef struct _syncMonitorFile
{
	QString path;
	FileInfo file;

	bool enabled;
}SyncMonitorFile;

typedef struct _syncProgram
{
	QString	path;
	FileInfo program;
	bool enabled;
	bool running;
	bool terminal;

	QVector<SyncMonitorFile> files;
}SyncProgram;

typedef struct _syncCommand 
{
	QString command;
	QVector<QString> params;
	bool enabled;

	QVector<SyncMonitorFile> files;
}SyncCommand;

typedef struct _syncCommands
{
	QMap<int, SyncProgram> programs;
	QMap<int, SyncCommand> commands;
}SyncCommands;

#define SYNC_COMMANDS_PROGRAM_KIND	"program"
#define SYNC_COMMANDS_COMMAND_KIND	"command"

UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const SyncMonitorFile& info);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, SyncMonitorFile& info);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const SyncProgram& info);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, SyncProgram& info);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const SyncCommand& info);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, SyncCommand& info);
UTIL_EXPORT QDataStream& operator<<(QDataStream& out, const SyncCommands& info);
UTIL_EXPORT QDataStream& operator>>(QDataStream& in, SyncCommands& info);

Q_DECLARE_METATYPE(SyncMonitorFile)
Q_DECLARE_METATYPE(QVector<SyncMonitorFile>)
Q_DECLARE_METATYPE(SyncProgram)
Q_DECLARE_METATYPE(SyncCommand)

Q_DECLARE_METATYPE(Server)
Q_DECLARE_METATYPE(ServerInfo)
Q_DECLARE_METATYPE(MibRemoteServer)
Q_DECLARE_METATYPE(QList<ServerInfo>)
Q_DECLARE_METATYPE(QVector<ProcessorInfo>)
Q_DECLARE_METATYPE(PerfProcessor)
Q_DECLARE_METATYPE(QVector<OperatingSystem>)
Q_DECLARE_METATYPE(OperatingSystem)
Q_DECLARE_METATYPE(ComputerSystem)
Q_DECLARE_METATYPE(QVector<NetworkAdapter>)
Q_DECLARE_METATYPE(QVector<SystemProcess>)
Q_DECLARE_METATYPE(QVector<FileInfo>)
Q_DECLARE_METATYPE(FileInfo)
Q_DECLARE_METATYPE(QVector<DiskDrive>)
Q_DECLARE_METATYPE(QVector<CDROMDrive>)
Q_DECLARE_METATYPE(QVector<LogicalDisk>)
Q_DECLARE_METATYPE(LogicalDisk)
Q_DECLARE_METATYPE(QVector<PhysicalMemory>)
Q_DECLARE_METATYPE(SystemMemory)
Q_DECLARE_METATYPE(GraphTopo)
Q_DECLARE_METATYPE(PrivilegeGroups)
Q_DECLARE_METATYPE(PrivilegeUser)
Q_DECLARE_METATYPE(CscsDatabase)

#endif // QUTILDEF_H
