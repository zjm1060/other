#ifndef DL476DATA
#define DL476DATA

//#define PRTLOG

#define	MAX_PLAN_POINT		96
#define	PLAN_POINT_BASE		68

int		GenTableConf();
int		GenDATAIndex();
int		GenSWCTRLIndex();

int		GenNodeTable();

void		ReadOnlineDB();
unsigned short	ReadAlmBuf();
void		StoreRealValue();
void		StoreSWValue();

void		DatapduIntial();

void AllPackage(short tableno);
void		ChangedRealPackage(short k);
void		ServerCmdPackage();

SOCKET		Do_Connect(void);
void		Do_Disconnect(SOCKET sock);
unsigned short	Do_Send(SOCKET sockfd,DATA_APDU apdu);
unsigned short	Do_Recv(SOCKET sockfd,char * buff,short maxlen);
SOCKET		TcpSend(SOCKET sock,DATA_APDU apdu);

int		CmdRecv();
int		ClientCmdConvert();

int		DataRecv();
void		DataConvert();

int		ClientRecv();
int		ClientSend(void);
int		ServerRecv();
int		ServerSend();
int		UpdateChannelFlag(unsigned short status);
void	ComdSendToLCU(char type_id,char cmd_stat,DMS_COMMON *pdbcomm); 
void	SEComdSendToLCU(char type_id, float value, DMS_COMMON *pdbcomm);
/*void		LanCommand1(char *,short);
void		LanCommand(char *,short);
void 	 	LanBdCommand(char *,short);
void 		BdSBOStat();*/
extern int     	InterLockFileLoad();
char               strTemp1[100];
void	WriteMainInfo(char *buf);
/*****************************************************
 *	H9000系统数据格式
 ******************************************************/

#define		NORM_SCAN	0
#define		MAN_SET	  	0x01
#define		CHAN_FAIL	0x02
#define		CALC_DATA	0x03
#define		LMT_ATAT	0x16
#define		NET_UNIT	0x0200
#define		UP_UNIT		0x0000

#define		WAN_IN_VICE	1
#define		WAN_IN_HOST	0

typedef	struct
{
	char	dev_id;
	char	data_type;
	unsigned short	point_id;
	unsigned short	status;
	unsigned short	data;
	unsigned short	data_2;
}	CMD_BUF;

extern short		almptr;
extern SOCKET		clientsock,serversock;
extern char		old_day;

#define	SYS_RTU		12
#define	STN_ID		1   /*厂站ID*/
#define	true	(int)1
#define	false	(int)0
#define DL476_SYS_TAB_NAME           	"WJD.WJD_SYS.SYS."
#define WJD_SYS_TAB_NAME           	"WJHF_SYS.SYSMSG."  /*  added by zwj for WJD 20060707*/
int     Dsend_errnum;
int     connect_errnum;
char dev_idtmpforcom;
char data_typeforcom;
short point_idforcom;
short statusforcom;
#endif
