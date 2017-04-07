/**************************************************************************************************************
added by yyp on 2008.5.26 for H9000 V4.0

	data structrue define for CIP		

Modifying History:

**************************************************************************************************************/
#ifndef	_AB_PLC
#define	_AB_PLC

#ifdef __cplusplus
extern "C" {
#endif


#include "plcstd.h"

#define MAX_PLC_IPADDR_NUM	4
#define TAGLEN	30
#define MAX_TAGS 1
	
	
typedef struct _AB_PLC_DEF
{
	unsigned short port;
	int		ipaddr_num;
	char	ipaddr[MAX_PLC_IPADDR_NUM][40];
	char	tagname[TAGLEN];
} AB_PLC_DEF;


typedef struct _MYSOCKET
{
	SOCKET sock;
	int session;
} MYSOCKET;
	
	
#define		MAX_QUERY_SIZE_AB	200		/* max query word num of AB PLC */

#define		READ_STATUS	0
#define		WRITE_STATUS	1
#define		EXCPT_STATUS	2
#define		RECV_FLAGS      (0)
#define		ICMP_TIMEOUT_S	2		/* icmp sock recv/send timeout : sec */
#define		ICMP_TIMEOUT_MS	0		/* icmp sock recv/send timeout : msec */
#define		SOCK_TIMEOUT_S	2		/* sock recv/send timeout : sec */
#define		SOCK_TIMEOUT_MS	500		/* sock recv/send timeout : msec. note: must be less than 1000 */
#define		SOCK_TIMEOUT_MS_0X	900		/* sock recv/send timeout 0X : msec. note: must be less than 1000 */
#define		NODELAY		0	
#define		CONN_TIMEOUT_S	0		/* sock conn timeout : sec */
#define		CONN_TIMEOUT_MS	500		/* sock conn timeout : msec */
#define		TIMEOUT_ALARM	2		/* sock alarm timeout : sec */




#define HEADTAG	1
#define	SEND_SIZE_AB 1024
#define SEGBYTE 2			//yyp
#define BYTE_ID 0XC2
#define SHORT_ID 0XC3
#define INT_ID   0XC4
#define FLOAT_ID 0XCC

//存放标签名称和元素的个数的结构体
typedef struct _TagAndElements
{
	char strName[TAGLEN];  //名称
	unsigned short int usintElements;  //元素数
}TagAndElements;

//存放要写的标签名称、元素个数、值类型、值
typedef struct _TagsWrite
{
	char strName[TAGLEN];
    char bDataType;
	char bReserver;
	unsigned short int usintElements; 
	unsigned short int uintAddtionalLength;
}TagsWrite;
    
//IOS字串
typedef struct _IOS
{
	char bExtendedSymbolSegment; //字串类型,0x91为标签名称
	char      bDataSize;   //标签名称的字节数，
	char      *bData;      //标签名称
	char      *pad;        //标签名称的字符数为奇数时的填充字节
}IOS;//RequirePath;

//用于接收
typedef struct _ServiceReply
{
	char bService; //服务类型,0xcc为读响应,0xcd为写响应
	char bReserved;
	char bGeneralStatus;  //标准状态字节
	char bAdditonalStatusSize; //扩展状态字的长度
	char cReplyData[MAX_QUERY_SIZE_AB]; //返回的数据值
}ServiceReply;


typedef struct _ValueToWrite
{
	char bDataType;
	char bReserve;
	unsigned short int usintElements;
	char data[SEND_SIZE_AB];
}ValueToWrite;



//封装协议头,24字节
typedef struct _EncapsulationCIPHead
{
	short int sintCommand;   
	//命令代码,0x65:RegisterSession,0x66: UnRegisterSession,0x04:ListServices, 
	//0x63:ListIdentity, 0x64:ListInterface, 0x6f:SendRRData,0x70:SendUnitData
	short int sintLength;  //CIP协议字节数，不包括封装协议
	int       intSessionHandle;  //会话句柄,由目标方传给原始方
	int       intStatus;         //状态码,成功为0，原始方应设置此值为0
	char      bSenderContext[8];  //发送方上下文，根据此值判断响应是由所对应的发送
	int       intOption;          //可选项，应为0
}EncapsulationCIPHead;


//地址和数据项
typedef struct _AddressAndDataItem
{
	short int sintTypeID;  //地址或数据项类型,对应UnConnected Message, Address应设为0x00,
	//Data应设为0xb2;
	short int sintLength;  //地址和数据的数据部分长度
}AddressAndDataItem;

//各种命令的专有数据
typedef struct _CommandSpecificData
{
    int intInterfaceHandle;  //接口句柄,CIP是应为0；
	short int sintTimeout;   //操作超时，指封装协议
	short int sintItemCount; //地址和数据的项数，一般为2
	short int sintAddressTypeID; //地址,0x0000
	short int sintAddressItemLength; //0x0000
	short int sintDataTypeID;    //数据,0x00b2
	short int sintDataItemLength;  //此字段后的数据的字节数
}CommandSpecificData;

//CIP协议
//CommonIndustrialProtocol	6->8 ARM
#define CIP_HEAD_SIZE	6
typedef struct _CommondIndustrialProtocol
{
	char bService;  //服务类型, 0x52为非连接发送服务请求
	char bRequestPath;  //请求路径16bit数(为2)
	char bLogicalClassSegment;  //逻辑类，20:指8位逻辑类
	char bClass;                //类，06:指连接管理器类，
	char bLogicalInstanceSegment; //逻辑实例,24:指8位逻辑实例
	char bInstance;             //实例,01:实例01
}CommonIndustrialProtocol;

//上面服务所对应的数据 
//ServiceRequestData	10->12 ARM
#define SVR_REQ_DATA_SIZE	10
typedef struct _ServiceRequestData
{
	char bPriorityAndTime_tick;  //优先权和超时因子
	char bTime_out_ticks;  //超时时间常数
	unsigned short sintMessageRequestSize; //请求报文的字节数
	char bService;  //服务类型,0x0a:多个服务包
	char bRequestPathSize;    //请求服务路径的字节数,此字段后不含RoutePath的数据的字节数
	char bRequestPathClassSegment; //请求服务路径的类所属的段
	char bClass;                   //段类型
	char bRequestPathInstacneSegment; //请求服务路径的实例所属的段
	char bInstance;                //实例
	//要加
}ServiceRequestData;

//用于接收
typedef struct _ServiceRequestResponseData
{
	char bService; //服务类型，响应0x8a;多个服务包
	char bReserved; 
	char bGeneralStatus; //标准状态字节
	char bAddtionalStatusSize; //扩展状态的字数
}ServiceRequestReponseData;

//请求报文
typedef struct _MessageRequest
{
	char bService;  //服务类型,0x0a:多个服务包
	char bRequestPathSize;    //
	char bRequestPathClassSegment;
	char bClass;
	char bRequestPathInstacneSegment;
	char bInstance;
	//要加
}MessageRequest;

//多个服务
//ServicesAggregate		2->4 ARM
#define SVR_AGGR_SIZE	2
typedef struct _ServicesAggregate
{
	unsigned short usintNumberOfServices; //服务数
}ServicesAggregate;

typedef struct _RoutePath
{
	char bRoutePathSizeWord;  //16bit数：0x01
	char BResever;            //保留   :0
	char bExtendLinkAddress;  //是否扩展链接地址:01
	char bLinkAddress;        //地址:0
}RoutePath;

typedef struct _recCIP
{
	unsigned char bService;  //服务类型,0x8a:多个服务包
	unsigned char bReserve;
	unsigned char bGeneralStatus;   //错误代码
	unsigned char bAddtionalStatusSizeWords;  //扩展错误代码字大小
}recCIP;

typedef struct _recServices
{
	char bService;  //服务代码
	char bReserve;  //保留
}recServices;

typedef struct _RecordTask
{
	short dwContext;
	int blFinished;
}RecordTask;

//服务包
typedef struct _ServicePackRead
{
	char bService;				//服务类型， 0x4c为读, 0x4d为写
	char bRequirePathWords;		//IOS字串的16bit数
	char bExtendedSymbolSegment; //字串类型,0x91为标签名称
	char bDataSize;				//标签名称的字节数，
	char TagName[TAGLEN];
	short IOIType;				//0x29 for two byte	
	short StartPos;				//所要请求的数组开始位置
	short ElementNum;			//读的元素数
}ServicePackRead;


//服务包
typedef struct _ServicePackWrite
{
	char bService;				//服务类型， 0x4c为读, 0x4d为写
	char bRequirePathWords;		//IOS字串的16bit数
	char bExtendedSymbolSegment; //字串类型,0x91为标签名称
	char bDataSize;				//标签名称的字节数，
	char TagName[TAGLEN];
	short IOIType;				//0x29 for two byte	
	short StartPos;				//所要请求的数组开始位置
	char bDataType;
	char bReserve;
	short ElementNum;			//读的元素数
	char data[SEND_SIZE_AB];
}ServicePackWrite;


typedef struct _CIPRequest
{
	EncapsulationCIPHead eciphead;		//封装头部
	CommandSpecificData cpfhead;		//CPF头部
	CommonIndustrialProtocol ciphead;	//CIP 数据区
	ServiceRequestData sendhead;		//请求头
	ServicesAggregate tagnum;			//请求个数
	short offset[MAX_TAGS];				//偏移地址
	ServicePackRead	srvdata[MAX_TAGS];		//请求包
	RoutePath path; 
}CIPRequest;



typedef struct _CIPWrite
{
	EncapsulationCIPHead eciphead;		//封装头部
	CommandSpecificData cpfhead;		//CPF头部
	CommonIndustrialProtocol ciphead;	//CIP 数据区
	ServiceRequestData sendhead;		//请求头
	ServicesAggregate tagnum;			//请求个数
	short offset[MAX_TAGS];				//偏移地址
	ServicePackWrite srvdata[MAX_TAGS];		//服务包,注意,同一个tag的srvdata和wrdata连续存储
	RoutePath path; 
}CIPWrite;

typedef struct _CIPResponse
{
	EncapsulationCIPHead eciphead;		//封装头部
	CommandSpecificData cpfhead;		//CPF头部
	recCIP ciphead;						//CIP 数据区
	ServicesAggregate tagnum;			//请求个数
	short offset[MAX_TAGS];				//偏移地址
	ServiceReply srvdata[MAX_TAGS];		//请求包
}CIPResponse;

//#pragma   pack()

int CvtReadToBuf(char *buffer,int *p_bytesend, CIPRequest ciprq);
int CvtWriteToBuf(char *buffer,int *p_bytesend, CIPWrite cipwt);



#ifdef __cplusplus
}
#endif

#endif


