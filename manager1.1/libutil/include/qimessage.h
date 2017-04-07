#ifndef QIMESSAGE_H
#define QIMESSAGE_H

#include <QDataStream>

#include "global.h"
#include "qsharedobject.h"

enum MsgType
{
	MSG_LOCAL_SERVER,
	MSG_REMOTE_SERVER,
	MSG_TOPOLOGY,
	MSG_PROCESSOR,
	MSG_SYSTEM,
	MSG_COMPUTER_SYSTEM,
	MSG_NETWORK_ADAPTER,
	MSG_SYSTEM_PROCESS,
	MSG_DISK_DRIVE,
	MSG_SYSTEM_MEMORY,
	MSG_FILE_TRANSFER,
	MSG_CONTROLLED,
	MSG_STATUS_CHANGED,
	MSG_DDB,
	MSG_CSCS,			//H9000相关消息
	MSG_CONFIGURE
};

enum ConfigureAckType
{
	QuerySyncAssociationConfigure,		//请求同步关联设置
	AnswerSyncAssociationConfigure,		//应答同步关联设置
	QueryWriteSyncAssociateionConfig,	//请求更改关联设置
	AnswerWriteSyncAssociateionConfig,
	QueryExecSyncConfigureCommand,		//请求执行文件同步命令
	AnswerExecSyncConfigureCommand
};

enum ConfigureAckResultType
{
	QueryWriteConfigureFail,		//写配置文件失败
	QueryWriteConfigureSuccess,
	QueryCommandFoundProcessesFail,	//未读取mib中进程信息
	QueryCommandKillProcessFail,	//终止进程失败
	QueryCommandCreateProcessFail,	//创建进程失败
	QueryCommandFoundCommandFail,	//不能找到执行函数或条件不满足
	QueryCommandExecCommand,
	QueryCommandFinish
};

enum CscsAckType
{
	QueryCscsStruct,	//请求H9000厂站结构
	AnswerCscsStruct,
	QueryCscsStationEigenvalue,
	AnswerCscsStationEigenvalue,
	QueryCscsGroupEigenvalue,
	AnswerCscsGroupEigenvalue,
	QueryCscsDataTypeEigenvalue,
	AnswerCscsDataTypeEigenvalue
};

enum CscsAckResultType
{
	CscsDataSourceLost,			//H9000未启动无实时库数据源
	CscsDataSourceFound,		//成功获取数据源
	CscsDataSourceNoExisted,	//查找的数据不存在
	CscsCommandNoExisted,
	CscsCommandParamError,
	CscsCommandFailed,			//执行失败
	CscsCommandSuccess
};

enum CscsEigenvalueType
{
	CscsDbHead,
	CscsStationEntry,
	CscsGroupEntry,
	CscsRecordEntry
};

enum DdbAckType
{
	QueryInformDdbConnection,		//ddb连接，区别于其他连接
	AnswerInformDdbConnection,
	QueryUploadPrivilegeData,		//上传权限数据
	AnswerUploadPrivilegeData,
	QueryCheckPrivilege,			//检查用户登入信息
	AnswerCheckPrivilege,
	QueryPrivileges,				//请求所有用户组信息
	AnswerPrivileges,
	QueryCreateGroup,				//请求创建组
	AnswerCreateGroup,
	QueryDeleteGroup,				//请求删除组
	AnswerDeleteGroup,
	QueryCreateUser,				//请求创建用户
	AnswerCreateUser,
	QueryDeleteUser,				//请求删除用户
	AnswerDeleteUser,
	QueryModifyUserPrivilege,		//请求修改用户权限
	AnswerModifyUserPrivilege,
	QueryModifyUserPassword,		//请求修改用户密码
	AnswerModifyUserPassword,
	UpdateUserPrivilege				//更新用户权限
};

enum DdbAckResultType
{
	PrivilegeDataModified,
	PrivilegeDataUnmodified,
	PrivilegeUncheckable,		//权限测试条件不满足
	PrivilegeUserNoExisted,		//用户名不存在
	PrivilegePasswdError,		//密码错误
	PrivilegeCheckPass,			//测试通过

	CreateGroupSuccess,			//创建组成功
	CreateGroupExisted,			//组已存在

	CreateUserSuccess,			//创建用户成功
	CreateUserExisted,			//用户存在
	CreateUserGroupNonexistent,	//创建用户组不存在

	ModifyPrivilegeUserNonexistent,	//修改权限，用户不存在
	ModifyPrivilegeSuccess,		//修改权限成功

	ModifyPasswordUserNonexistent,	//修改密码，用户不存在
	ModifyPasswordSuccess,			//修改密码成功

	UserDeleted,				//用户被删除
	UserPasswordChanged,		//用户密码被修改
	UserPrivilegesChanged		//用户权限被修改
};

enum LocalServerMsgAckType
{
	QueryLocalServer,		//请求获取本地服务器信息
	AnswerLocalServer,
	InformStatusChanged
};

enum RemoteServerMsgAckType
{
	QueryRemoteServerList,	//请求获取远端服务器列表
	AnswerRemoteServerList
};

enum TopologyMsgAckType
{
	QueryTopology,
	AnswerTopology
};

enum SystemProcessMsgAckType
{
	QueryRefProcess,		//请求连续发送进程信息
	QueryUnrefProcess,		//不再需要进程信息
	QueryProcess,			//请求进程信息（单次）
	AnswerProcess,
	QueryCreateProcess,
	AnswerCreateProcess,
	QueryTerminateProcess,
	AnswerTerminateProcess
};

enum SystemMsgAckType
{
	QueryOperatingSystem,	//请求操作系统信息
	AnswerOperatingSystem
};

enum ComputerSystemMsgAckType
{
	QueryComputerSystem,
	AnswerComputerSystem	//请求系统信息
};

enum NetworkAdapterMsgAckType
{
	QueryNetworkAdapter,	//请求网络适配器信息
	AnswerNetworkAdapter
};

enum ProcessorMsgAckType
{
	QueryProcessor,			//请求处理器配置信息
	AnswerProcessor,
	QueryRefRuntimeProcessor,	//请求连续发送处理器使用信息
	QueryUnrefRuntimeProcessor,	//请求终止发送处理器使用信息
	QueryRuntimeProcessor,		//请求处理器使用信息（单次）
	AnswerRuntimeProcessor
};

enum MemoryMsgAckType
{
	QueryPhysicalMemory,		//请求物理内存配置信息
	AnswerPhysicalMemory,
	QueryRefSystemMemory,		//请求连续发送系统内存使用信息
	QueryUnrefSystemMemory,
	QuerySystemMemory,			//请求发送一次系统内存使用信息
	AnswerSystemMemory
};

enum DiskDriveMsgAckType
{
	QueryDiskDrive,
	AnswerDiskDrive,
	QueryCDROMDrive,
	AnswerCDROMDrive,
	QueryLogicalDisk,
	AnswerLogicalDisk
};

enum CommandType
{
	CommandCreateProcess,		//创建进程,参数为命令行，当前目录
	CommandTerminateProcess		//终止进程,参数为进程句柄
};

enum CommandResultType
{
	ExecMethodComInvalid=-2,	//COM初始化失败
	ExecMethodInvalid=-1,		//无效方法
	ExecMethodFailed=0,
	ExecMethodSucceed,
	ExecMethodAccessDenied,
	ExecMethodPathNotFound,
	ExecMethodInvalidParameter
};

enum FileTransferMsgAckType
{
	QueryRoot,				//请求根目录,根目录为当前程序的父目录
	AnswerRoot,				//返回根目录
	QueryPath,				//请求路径
	AnswerPath,				//返回路径下所有子目录以及文件信息(或不存在该目录)
	QueryDrive,				//请求磁盘驱动器信息
	AnswerDrive,
	QuerySymLink,			//请求连接文件目标地址
	AnswerSymLink,

	QueryDownloadDir,		//请求下载文件夹
	AnswerDownloadDir,		
	QueryFileInfo,			//请求文件信息
	AnswerFileInfo,			//返回文件信息
	QueryDownloadFile,		//请求下载文件
	AnswerDownloadFileInfo,	//下载初始化，返回文件信息
	QueryTransferFile,		//文件创建完成，开始传输文件
	AnswerTransferFile,		//传输文件内容
	QueryDownloadSymLink,	//请求下载符号链接文件
	AnswerDownloadSymLink,
	QueryResumeDownloadFile,	//请求续传文件
	AnswerResumeDownloadFile,
	QueryDownloadFlowControl,	//下载流量控制
	AnswerDownloadFlowControl,

	QueryUploadDir,			//请求上传文件夹
	AnswerUploadDir,
	QueryUploadFile,		//请求上传文件
	AnswerUploadFile,
	QueryUploadFileContent,	//请求上传文件内容
	AnswerUploadFileContent,
	QueryUploadSymlink,		//请求上传符号链接文件
	AnswerUploadSymlink,
	QueryResumeUploadFile,	//请求续传文件
	AnswerResumeUploadFile,
	QueryUploadFlowControl,	//上传流量控制
	AnswerUploadFlowControl,

	QueryUploadSlientDir,		//请求上传文件夹而不必返回任何信息
	QueryUploadSlientSymlink,	//请求上传符号链接文件而不必返回任何信息
	QueryUploadSlientFile,		//请求上传文件而不必返回任何信息
	QueryUploadSlientFileContent,	//请求上传文件内容，而不必返回任何信息
	AnswerUploadSlientFileContent,

	QueryRename,			//请求更名
	AnswerRename,
	QueryNewDirectory,		//请求创建新目录
	AnswerNewDirectory,
	QueryDelete,			//请求删除文件或文件夹
	AnswerDelete,

	QueryInterruptTransfer,	//请求中断传输
	AnswerInterruptTransfer,

	QueryBackup,			//请求备份文件，文件夹
	AnswerBackup
};

enum ControlledMsgAckType
{
	QueryConnectToTarget,		//请求建立与目标主机之间的连接
	AnswerConnectToTarget,
	QueryUploadDirToTarget,		//请求源主机上传目录到目标主机
	AnswerUploadDirToTarget,
	QueryUploadFileToTarget,	//请求原主机上传文件到目标主机
	AnswerUploadFileToTarget,
	QueryUploadSymlinkToTarget,	//请求上传符号链接文件到目标主机
	AnswerUploadSymlinkToTarget,
	QueryInterruptTransferToTarget,	//请求中断与目标主机之间的文件传输
	AnswerInterruptTransferToTarget,
	QueryUploadSlientDirToTarget,	//请求源主机上传目录到目标主机,目标主机不需要返回任何信息
	AnswerUploadSlientDirToTarget,
	QueryUploadSlientFileToTarget,	//请求原主机上传文件到目标主机,目标主机不需要返回任何信息
	AnswerUploadSlientFileToTarget,
	QueryUploadSlientSymlinkToTarget,	//请求上传符号链接文件到目标主机,目标主机不需要返回任何信息
	AnswerUploadSlientSymlinkToTarget,
	QueryExecSyncCommandToTarget,			//请求上传执行同步命令
	AnswerExecSyncCommandToTarget
};

enum MsgAckResultType
{
	QueryExecuted=-2,		//消息已执行，用于文件传输
	QueryUnknow=-1,
	QueryFailed=0,
	QuerySucceed,

	QueryPathNoExist,		//路径不存在
	QueryPathValid,			//路径有效

	QueryDirNoExist,		//文件夹路径不存在
	QueryDirValid,			//文件夹路径有效
	QueryDirMakePathFailed,	//创建文件夹路径失败

	QueryFileNoExist,		//文件不存在
	QueryFileUnreadable,	//文件不可读
	QueryFileUnwritable,	//文件不可写
	QueryFileModified,		//文件被修改
	QueryFileValid,			//文件有效

	QueryFileNoSymLink,		//不是符号链接文件

	QueryFileOpenFail,		//打开文件失败
	QueryFileOpenSuccess,	//打开文件成功

	QueryFileCreateFail,	//创建文件失败
	QueryFileCreateSuccess,	//创建文件成功

	QueryFileWriteLost,		//文件已经关闭，写入失败
	QueryFileWriteFail,		//写入文件失败
	QueryFileWriteSuccess,	//写入文件成功
	QueryFileWriteFinish,	//写入文件完成

	QueryFileReadLost,		//文件已经关闭，读入失败
	QueryFileReadFail,		//读取文件失败
	QueryFileReadSuccess,	//读取文件成功
	QueryFileReadFinish,	//读取文件完成

	QueryFileSeekFail,		//设置文件当前位置失败

	QueryFileRenameFail,	//重命名文件失败
	QueryFileChangePermissionFail,	//更改文件权限失败

	QueryFileTransferFinish,	//传输文件完成

	QueryFileRemoveFail,	//删除文件失败
	QueryFileRemoveSuccess,
	QueryFileRemoved,

	QueryFileSendFlowControl,	//发送流量控制包

	QuerySymLinkNoLink,		//请求文件不是符号链接文件
	QueryCreateSymLinkFail,	//创建符号链接文件失败
	QueryCreateSymLinkSuccess,

	QueryMakePathSuccess,	//请求创建文件夹成功
	QueryMakePathFail,		//请求创建文件夹失败

	QueryRenameFail,
	QueryRenameSuccess,
	QueryRenamePathLost,	//未发现原始路径

	QueryNewFail,
	QueryNewSuccess,

	QueryDelFail,
	QueryDelSuccess,

	QueryConnectToTargetFailed,	//源主机与目标主机建立连接失败
	QueryConnectToTargetSuccess,//建立连接成功
	TargetDisconnected,			//源主机与目标主机断开连接
	QueryUploadToTargetSourceDirLost,		//源主机文件夹不存在
	
	QuerySourceFileOpenFail,	//源文件打开失败
	QuerySourceFileReadLost,	//源主机未找到文件
	QuerySourceFileReadFail,	//源主机读文件失败
	QuerySourceTransferFile,	//源主机传输文件中
	QuerySourceTransferFileFinish,	//源主机完成传输文件
	QuerySourceSendFlowControl,		//源主机发送流量控制
	QueryTargetFileCreateFail,	//目标主机创建文件失败
	QueryTargetFileWriteFail,	//目标主机写文件失败
	QueryTargetFileWriteSuccess,	//目标主机写文件成功
	QueryTargetFileWriteLost,	//目标主机写文件发生错误后，已经关闭
	QueryTargetFileWriteFinish,	//目标主机写入文件完成
	QueryTargetFileChangePermissionFail,	//目标文件改变文件权限失败

	QueryTargetUploadFileFail,		//目标主机将临时文件更名为保存文件失败
	QueryTargetUploadSymLinkSuccess,	//请求上传链接文件创建成功
	QueryTargetUploadSymLinkFail,		//请求上传链接文件创建失败

	QueryBackingUp,					//正在备份文件
	QueryBackUpFinish				//备份完成
};

enum SortFlags
{
	SortName,				//按名称排序
	SortTime,				//按时间排序
	SortSize,				//按大小排序
	SortType,				//按类型排序
	SortReversed=0x10		//反序，与其他排序方式混合使用
};

enum FileType
{
	Drive=1,
	CDDVDDrive=(1<<1),
	Dir=(1<<2),
	HideDir=(1<<3),
	File=(1<<4),
	HideFile=(1<<5),
	SymLink=(1<<6),
	HideSymLink=(1<<7),
	SymLinkDir=SymLink|Dir,
	SymLinkHideDir=SymLink|HideDir,
	SymLinkFile=SymLink|File,
	SymLinkHideFile=SymLink|HideFile,
	FileTypeMask=0x3f
};

enum FileTransferFormat
{
	TextFormat=0,		//文本格式
	BinaryFormat,		//二进制格式
	AutoFormat,
	FormatEnd
};

#define UNIVERSAL_MAC_ADDRESS	"FF:FF:FF:FF:FF:FF"

class UTIL_EXPORT QIMessage : public QSharedObject
{
public:
	QIMessage(){};
	virtual ~QIMessage(){};

public:
	virtual int type() const=0;

	virtual QByteArray query(int ackType, quint32 usrID=-1, const QByteArray& contents="")=0;		//发起请求
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=NULL)=0;
};

typedef QSharedObjectPtr<QIMessage> QIMessagePtr;

typedef void (*CreateMessageInstanceFunc)(QVector<QIMessagePtr>&);		//消息类动态库必须提供的接口

#endif // QIMESSAGE_H
