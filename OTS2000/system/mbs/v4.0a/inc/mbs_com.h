
#include "\home\ems\system\lan\v4.0a\inc\lan.h"
#include "../../../plc/v4.0a/inc/plc_com.h"
#include "mbc_com.h"

int ReadMbsStnDef(char * pStnDefFile);
BOOL	GenAnaSendTable(int);	
BOOL	GenAnaRecvTable(int);	
BOOL	GenPolSendTable(int);	
BOOL	GenPolRecvTable(int);	
BOOL	GenImpSendTable(int);	
void * ThreadCommS( void * arg );
void	mbs_comm();
BOOL	MbFunc16(char *,int);
BOOL	MbFunc04(char *,int);
BOOL	GetSendData(int);
BOOL	BdRecvData(int type, int ptnum,int start,char * buf, int id);


int FillMbArea(int);
int WaitForConnect(int *p_sock);
int CreatListenSock(int *p_sock);


