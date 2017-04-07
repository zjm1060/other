#include "webscan_def.h"
//
#define DEBUG

#define  SERV_PORT 	9001
#define  SERV_IP   	"127.0.0.1"

#define  BUFSIZE	65535
#define  MAXDATASIZE	256
#define  SEND_TIMEOUT_VALUE	1000
#define  RECV_TIMEOUT_VALUE	1000

#define  bzero(target,size)	memset(target,0,size);

#ifdef WINDOWS_OS
#define sleep(sec)				Sleep(sec*1000)
#endif

typedef struct RECORD
{	char			name[POINT_NAME_SIZE];
	char			type;
	int				data;
} RECORD;

typedef struct RECORDLIST
{
	int				count;
	RECORD			record[100];
} RECORDLIST;

/*typedef union RECORDVALUE
{
	char	pt_id[POINT_NAME_SIZE];
	char	pt_logicalname[POINT_NAME_SIZE];
	char	pt_longname[POINT_LONGNAME_SIZE];
	float		ana_fvalue;
}RECORDVALUE;*/

typedef struct CLIENT {
    int fd;
    struct sockaddr_in addr;    
}CLIENT;

void	InitProc(),EndProc(),RcvDataProc(int num,char buf[],int *retNum,char ret[]);

