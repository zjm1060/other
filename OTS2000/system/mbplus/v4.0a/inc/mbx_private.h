/**************************************************************************************************************
Created by yyp on 2006.3.31 for H9000 V4.0

 note when using MSVC/C++ 5.0 - create workspace as a win32 console app and
 change project/settings/link Object/library modules list to include wsock32.lib

Modifying History:
2006.3.31	start to modify the main code yyp
**************************************************************************************************************/
#ifndef	_PRIVATE
#define	_PRIVATE

#define	MBX
#define	MBXDEBUG
/*#define	DEBUG*/
#define	MULTITHREAD

#define PLC_SCAN_LEN        	12
#define PLC_CMD_LEN				13
#define WR_PLC_ACK_LEN          12
#define MBX_SCAN_LEN			6
#define MBX_CMD_LEN				7
#define	MBX_TIMEOUT				2
#define	MBXRECVPDUHEAD			3

void * MBXScanH( void * arg );
void * MBXScan( void * arg );
int ChildProc(unsigned char stnid,unsigned char devid);
int MBXCmd(NCB * nd, LCUID plcid);
int WriteCmd(NCB * nd,LCUID plcid);
int	GetHead(NCB *nd, LCUID plcid);
int	GetANA(NCB *nd,LCUID plcid);
int	GetIMP(NCB *nd,LCUID plcid);
int	GetSysMsg(NCB *nd,LCUID plcid);
int	GetPOL(NCB *nd,LCUID plcid);
int	GetINTAlm(NCB *nd,LCUID plcid);
int	WriteData(NCB *nd,LCUID plcid);
int	WriteCmd(NCB *nd,LCUID plcid);
int WriteRunFlag(NCB *nd,LCUID plcid);
int WriteTimeCmd(NCB * nd,LCUID plcid);
int WritePLC(NCB *nd, unsigned char seg, int ref, int size, unsigned char *buf,LCUID plcid);
int ReadPLC(NCB *nd, unsigned char seg, int ref, int size, unsigned char *buf,LCUID plcid,unsigned char type);

void 	DisConnectPLC(NCB *,LCUID);
NCB	*ConnectPLC(LCUID);
void	GetStationInfo();
int		myprintf(char *buf);


#endif