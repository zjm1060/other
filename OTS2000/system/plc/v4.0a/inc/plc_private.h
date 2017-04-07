/**************************************************************************************************************
modified by yyp on 2005.11.1 for H9000 V4.0

 note when using MSVC/C++ 5.0 - create workspace as a win32 console app and
 change project/settings/link Object/library modules list to include wsock32.lib

Modifying History:
2005.11.1	start to modify the main code yyp
**************************************************************************************************************/
#ifndef	_PLC_PRIVATE
#define	_PLC_PRIVATE

int sieport[MAX_WS_HOST_NUM+1];

void *  tfun1(void *arg);
void *	ThreadScan(void *); 
void *	ThreadScanH(void *); 
void *	ThreadCmd(void *); 
int ChildProc(unsigned char stnid,unsigned char devid);
int	GetHead(MYSOCKET sockplc,LCUID plcid);
int	GetANA(MYSOCKET sockplc,LCUID plcid);
int	GetIMP(MYSOCKET sockplc,LCUID plcid);
int	GetSysMsg(MYSOCKET sockplc,LCUID plcid);
int	GetPOL(MYSOCKET sockplc,LCUID plcid);
int	GetINTAlm(MYSOCKET sockplc,LCUID plcid);
int	WriteData(MYSOCKET sockplc,LCUID plcid);
int	WriteCmd(MYSOCKET sockplc,LCUID plcid);	
int	WriteTimeCmd(MYSOCKET sockplc,LCUID plcid);
int	WriteRunFlag(MYSOCKET sockplc,LCUID plcid);
int BdScanQlt(LCUID plcid,int *p_count);

MYSOCKET ConnectPLC(LCUID plcid,unsigned char thrtype);
MYSOCKET ConnectPLCChild(LCUID plcid, unsigned char threadtype);
BOOL TestConnPLC(LCUID plcid,unsigned char hotcpu, unsigned char netid,unsigned char thrtype); 
MYSOCKET CreateSock(LCUID plcid, char *address, unsigned short port);
int GetPlcPort(LCUID plcid,unsigned char threadtype, unsigned short * p_port,int * p_cpunum, int * p_netnum);
int BindLocal(LCUID plcid,MYSOCKET sockplc,unsigned short port);
int	DisconnectPLC(MYSOCKET *,LCUID);
int ConnSession(MYSOCKET sockplc,LCUID plcid);
int ClosePort(int sockid);
int ReadPortCfg();

int RecvPLC(MYSOCKET ,char *,int);
int WritePLC(MYSOCKET , unsigned char , int , int , unsigned char *, LCUID);
int ReadPLC(MYSOCKET , unsigned char , int , int , unsigned char *, LCUID);

int	SendPLC_MB(MYSOCKET , char *, int);
int RecvPLC_MB(MYSOCKET ,char *,int,int);
int WritePLC_MB(MYSOCKET , unsigned char , int , int , unsigned char *);
int ReadPLC_MB(MYSOCKET , unsigned char , int , int , unsigned char *);
int ReadPLC_MULT_MB(MYSOCKET , unsigned char , int , int , unsigned char *);

int	SendPLC_GE(MYSOCKET , char *, int);
int RecvPLC_GE(MYSOCKET ,char *,int);
int WritePLC_GE(MYSOCKET , unsigned char , int , int , unsigned char *);
int ReadPLC_GE(MYSOCKET , unsigned char , int , int , unsigned char *);

int WritePLC_SIE(MYSOCKET , unsigned char , int , int , unsigned char *);
int ReadPLC_SIE(MYSOCKET , unsigned char , int , int , unsigned char *);

int	SendPLC_AB(MYSOCKET , char *, int);
int RecvPLC_AB(MYSOCKET ,char *,int);
int WritePLC_AB(MYSOCKET , unsigned char , int , int , unsigned char *);
int ReadPLC_AB(MYSOCKET , unsigned char , int , int , unsigned char *);
static int	GetAddrByName(char *hostname1 ,unsigned long *ipaddr) ;
int UnRegisterSession(LCUID plcid,MYSOCKET sockplc);
int RegisterSession(LCUID plcid,MYSOCKET *p_sockplc);
void InitCIPRequest();
void ByteArrayToDword(char src[8],unsigned int *dest1,unsigned int *dest2);
void DwordToByteArray(unsigned int src1,unsigned int src2,char dest1[8]);

#endif


