#include        <stdio.h>
#include        <stdlib.h>
#include        <fcntl.h>
#include        <errno.h>
#include        <termios.h>
#include        <time.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <sys/ioctl.h>
#include        <linux/if.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include		<moxadevice.h>
#include	    <stddef.h>
#include	    <unistd.h>
#include        <pthread.h>
#include        <linux/sem.h>

#include "plcstd.h"

#define BIG_ENDIAN_


#define  WORD   unsigned short
#define  BYTE	unsigned char
#define  UINT	unsigned int


#define         TRUE 			1
#define         FALSE  			0


#define MAX_STR_NUM_IN_LINE		2
#define MAX_CHAR_NUM_IN_LINE	256


#define MAX_COMM_TASK_NUM		8
#define MAX_COMM_DATA_NUM		1024
#define MAX_DEF_SECTION_NAME_SIZE	24
#define MAX_COMM_TASK_NAME_SIZE	24
#define MAX_PLC_TYPE_NAME_SIZE	24
#define MAX_COMM_PORT_NAME_SIZE	24

typedef struct 
{
	int		port_id;
	int		swap_flag;
	int		data_bytes;
	int		start_pos;
	int		data_num;
	WORD	update_flag, update_flag_old;
	char	sect_name[MAX_DEF_SECTION_NAME_SIZE];
	WORD	data_buf[MAX_COMM_DATA_NUM];
} COMM_TASK_INFO;


typedef struct  
{
	int				dev_num;
	COMM_TASK_INFO	dev[MAX_COMM_TASK_NUM];
	WORD			read_data_buf[MAX_COMM_DATA_NUM];
}	SHARE_COMM_TABLE;


typedef struct 
{
	int		task_num;
	char	task[MAX_COMM_TASK_NUM][MAX_COMM_TASK_NAME_SIZE];
}	TASK_CONF_INFO;


typedef struct 
{
	char	port[MAX_COMM_PORT_NAME_SIZE];
	int		mode;
	int		baud;
	int		data_bits;
	int		parity; //0:no	1:even	2:odd
	int		unit_num;
	int     first_unit_addr;
}	COMM_PARAM_INFO;





#define CONFIG_DEFINE_FILE	"/root/comm_config.def"


extern SHARE_COMM_TABLE *share_table;
extern	int	err_output_mode;

void	SetModuleName(char *name);
void	SendTraceMsg(const char* format, ...);
int		SetErrLogMode(int argc, char *argv[]);
int		GetTaskId(int argc, char *argv[]);

int		InitShareCommTable(void);
int		GetProfInt(char* szFileName, char* szSectionName, char* szKeyName, int* iValue);
int		GetProfString(char* szFileName, char* szSectionName, char* szKeyName, char* szString);
int		GetProfSection(char* szFileName, char* szSectionName, char* szString, int nSize);

int		GetCommParamConf(COMM_PARAM_INFO *comm_param_info, COMM_TASK_INFO *pTaskInfo);
int		ComInit(COMM_PARAM_INFO	*comm_param_info);
int		ComInitEx(COMM_PARAM_INFO *comm_param_info, int block);
WORD	GetCrcCheck(int byteNum, BYTE *buff);
WORD	GetSumCheck(int byteNum, BYTE *buff);
void	PrintfData(int size, char *buf, int recv_flag);

void Swap2Byte(void *pShort);
void Swap4Byte(void *pInt);
int xnet_select(SOCKET s, int sec, int msec, short x);
