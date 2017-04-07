
#include "\home\ems\system\lan\v4.0a\inc\lan.h"
#include "../../../plc/v4.0a/inc/plc_com.h"
#include "mbc_com.h"

int		ReadMbmStnDef(char * pStnDefFile);
BOOL	GenAnaSendTable(int);	
BOOL	GenAnaRecvTable(int);	
BOOL	GenPolSendTable(int);	
BOOL	GenPolRecvTable(int);	
void *	ThreadCommM( void * arg );
void	mbm_comm();
BOOL	GetRemoteData(SOCKET sock, int id);
BOOL	SendDataToRemote(SOCKET sock, int id);
BOOL	GetSendData(int);
BOOL	BdRecvData(int);

int		FillMbArea(int);

SOCKET CreateSock(int id, char *address, unsigned short port);