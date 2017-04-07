// TransferConfig.h: interface for the CTransferConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFERCONFIG_H__056C79E4_E0D9_4C2F_9F51_7B62320E49F7__INCLUDED_)
#define AFX_TRANSFERCONFIG_H__056C79E4_E0D9_4C2F_9F51_7B62320E49F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//常量
const int MaxFileType=8;			//文件类型数
const int MaxDstNum=20;				//最大目标计算机数	
const int MAXFILE=20000;				//最大所选文件数
#endif // !defined(AFX_TRANSFERCONFIG_H__056C79E4_E0D9_4C2F_9F51_7B62320E49F7__INCLUDED_)
//自定义类型：目标计算机属性
struct DstComputer
{
	CString DstIP;					//目标计算机IP地址
	CString Username;				//用户名
	CString Passwd;					//密码
	int PortNum;					//端口号
	CString DstDriver;				//目标盘符
	CString DstFileDir[MaxFileType];		//每一种待传文件的路径，若为Unix机则为虚拟路径
	CString DstUnixDir[MaxFileType];		//专门针对Unix机，存储Unix机的绝对路径
	int DstType;					//目标计算机类型，0为PC，1为Unix
	int Status;						//是否启用，0为禁用，1为启用
};


//传输配置类，存储从配置文件读取的所有配置信息
class CTransferConfig
{
public:
	CTransferConfig();						//构造函数
	virtual ~CTransferConfig();				//析构函数
public:
	bool GetConfigFromFile();				//从配置文件读取所有配置信息
	bool WriteConfigToFile();				//将用户在界面中所选择的所有配置信息写入文件
	bool WriteDstConfigToFile();			//将用户在界面中所选择的目标计算机配置信息写入文件
	bool SetConfigDir();	
	bool DelDst(int nDst);					//删除指定计算机
public:
	CString strConfigFileName;				//配置文件完整路径
	CString m_prjName;						//项目名称
	CString m_srcDriver;					//源盘符
	CString m_srcVersion;					//源版本
	int m_srcFileSel[MaxFileType];			//所选待传输的文件
	int m_srcFileNum[MaxFileType];			//每种待传文件的个数，通配符也算一个	
	CString m_srcFileDir[MaxFileType];		//每一种待传文件的相对路径
	CString m_srcFileList[MaxFileType][MAXFILE];		//每一种待传文件类型文件列表数组
	int m_fileType[MaxFileType];			//文件类型，0为文本，1为二进制
	int m_dstCount;							//目标计算机的数目
	DstComputer m_dstComputer[20];			//目标计算机数组
	int m_ftpTransMode;						//文件传输方式，0为bat，1为ftp类方式
	int m_ShowMode;							//如果为bat运行，0为不显示窗口，1为显示窗口
};