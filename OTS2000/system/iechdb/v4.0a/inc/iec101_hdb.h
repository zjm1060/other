#define DEBUG
#include "\home\ems\system\lan\v4.0a\inc\lan.h"
#include <ocidfn.h>
#include <ocikpr.h>
#include <ctype.h>
#include <ocidem.h>

#define		MAX_BHAL_NUM			2000


//点信息数据结构
typedef struct 
{ 
  short			id;
  char			devname[38];					
  char			infoname[20];				
  IND_DATA		status;
  unsigned char	bhdevid;
  unsigned char infoid;
  struct tm update_time;
} BHDSW;


//报警记录数据结构
typedef struct 
{ 
  short			id;						//点号
  unsigned short		status;
  char			update_time[40];		//报警时间
  int			msecond;				//毫秒
} BHAL;

typedef	struct
{
  USHORT	number;
  float	lorange;
  float	hirange;
  char	name[60];
  unsigned char	bhdevid;
  unsigned char infoid;
  struct tm update_time;
} BHANADEF;



void	Write_Hdb();
int AddOneAl(BHDSW dsw);
int tmtostring(struct tm dtime, char *str);
