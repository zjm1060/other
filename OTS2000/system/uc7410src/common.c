/****************************************************************************************
    File name:	main.c
	
    History: 主程序   
    Version	邓小刚 	Date	05.8	Comment 
****************************************************************************************/

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
#include		<semaphore.h>
#include		<linux/sem.h>
#include		<stdarg.h>


#include "main.h"


void abplc(void);

SHARE_COMM_TABLE *share_table;

int		err_output_mode;
static char	module_name[36]="NO_DEFINED";

void SetModuleName(char *name)
{
	strncpy(module_name, name, sizeof(module_name));
}

int GetTaskId(int argc, char *argv[])
{
	int i;

	for(i=1; i<argc; i++)
	{
		int value;
		if(sscanf(argv[i], "%d", &value)==1)
			return value;
	}

	return -1;	
}

int	SetErrLogMode(int argc, char *argv[])
{
	if(argc>=2)
	{
		int i;
		for(i=1; i<argc; i++)
		{
			if(!strcmp(argv[i], "-errlog"))
			{
				err_output_mode= 1;
				break;
			}
		}
	}
	return err_output_mode;
}


void	SendTraceMsg(const char* format, ...)
{
    va_list ap;
    char	msg[4096], *ptr= msg;
	
	if(err_output_mode==0)
		return;
	
    va_start(ap, format);
    vsprintf(ptr, format, ap);
	va_end(ap);
	
	printf("\n%s: %s", module_name, msg);

	fflush(stdout);
}



int InitShareCommTable(void)
{
	int shmid;
	
	shmid=shmget((key_t)1234, sizeof(SHARE_COMM_TABLE), 0666|IPC_CREAT);
	if(shmid==-1)
	{
		SendTraceMsg("shmget failed\n");
		return FALSE;
	}
	
	if((share_table=(SHARE_COMM_TABLE *)shmat(shmid,0,0))==(SHARE_COMM_TABLE *)-1)
	{
		SendTraceMsg("shmat failed\n");
		return FALSE;
	}
	return TRUE;
}



#define IS_QUOTE(c)	(((c)=='\'')||((c)=='\"'))

int	GetStringsFromStr(char *bufptr, int strbuf_num, char strbuf[][MAX_CHAR_NUM_IN_LINE])
{
	int		i, substr_ptr, num, quote_field_start, new_substr_start;
	char	buffer[MAX_CHAR_NUM_IN_LINE+1];
	
	for(i=0; i<strbuf_num; i++)
		strbuf[i][0]= 0;
	
	if(bufptr[0]==0 || bufptr[0]==0x0d || bufptr[0]==0x0a) /*空行、换行符、回车符*/
		return 0;
	
	if(bufptr[0]=='#' || bufptr[0]=='!') /*#和!表示注释*/
		return 0;
	
	if(strlen(bufptr) >= sizeof(buffer))
	{
		strncpy(buffer, bufptr, sizeof(buffer)-1);
		buffer[sizeof(buffer)-1]= 0;
	}
	else
		strcpy(buffer, bufptr);
	

	i= 0;
	while(buffer[i])
	{
		if ( ( buffer[i] == '=') || ( buffer[i] == ',') || ( buffer[i] == '\t'))
			buffer[i] = ' ';
		
		if ( (buffer[i] == 0x0d) || (buffer[i] == 0x0a) ) /*去掉行末的回车符和换行符*/
		{
			buffer[i] = 0;
			break;
		}
		i++;
	}
	
	i= 0;
	substr_ptr= 0;
	num= 0;
	quote_field_start= 0;
	new_substr_start= 0;
	while(buffer[i])
	{
		if(IS_QUOTE(buffer[i])) /*引号*/
		{
			if(quote_field_start) /*后引号*/
			{
				if(buffer[i+1]==' ') /*后引号后面有空格*/
				{
					quote_field_start= 0; /*引号域结束*/
					strbuf[num][substr_ptr]= 0; /*结束当前的子串*/
					new_substr_start= 0;
					num++;
					if (num>=strbuf_num) /*以前错误使用MAX_STR_NUM_IN_LINE*/
						return num; 
					substr_ptr= 0;
					i += 2;
					continue;
				}
				if(buffer[i+1]==0) /*后引号后面字符串结束*/
				{
					strbuf[num][substr_ptr]= 0; /*结束当前的子串*/
					return num+1; 
				}
			}
			else /*前引号*/
			{
				if(i==0 || buffer[i-1]==' ')/*前引号 前面必须有空格，除非字符串开始*/
				{
					quote_field_start= 1; /*引号域开始*/
					i++;
					continue;
				}
			}
		}
		
		if(quote_field_start) /*引号域内拷贝所有字符*/
		{
			strbuf[num][substr_ptr++] = buffer[i];
			i++;
			new_substr_start= 1;
			continue;
		}
		
		if(buffer[i] != ' ') /*拷贝非空格字符*/
		{
			strbuf[num][substr_ptr++] = buffer[i];
			new_substr_start= 1;
		}
		else if((i>0) && (buffer[i-1]!=' ') && (buffer[i]==' ')) /*空格前为非空格字符*/
		{   
			strbuf[num][substr_ptr]= 0; /*结束当前的子串*/
			new_substr_start= 0;
			num++;
			if (num>=strbuf_num) /*以前错误使用MAX_STR_NUM_IN_LINE*/
				return num; 
			substr_ptr= 0;
		}
		i++;
	} /*while*/

	if(new_substr_start)
		strbuf[num][substr_ptr]= 0;	/*结束当前的子串*/
	return num+new_substr_start;
}



int	LocateKeyword( FILE *fp, char *keyword )
{
	char	str[MAX_CHAR_NUM_IN_LINE];
	char	key[64];
	
	sprintf(key, "[%s]", keyword);
	rewind( fp );
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( strstr(str, key) )
			return 0;
	}
	
	return -1;
}


int GetProfString(char* szFileName, char* szSectionName, char* szKeyName, char* szString)
{
	FILE	*fp;
	char	str[256];
	char	strbuf[MAX_STR_NUM_IN_LINE][MAX_CHAR_NUM_IN_LINE];
	int		str_num;
	
	if((fp=fopen(szFileName, "r"))==NULL)
	{
		SendTraceMsg("Fail to open %s", szFileName);
		return FALSE;
	}
	
	if(-1==LocateKeyword(fp, szSectionName))
	{
		SendTraceMsg("Fail to find [%s] section", szSectionName);
		fclose(fp);
		return FALSE;
	}
	
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			SendTraceMsg("Format error in [%s]: %s", szSectionName, str);
			break;
		}
		
		if(str_num==0)
			continue;
		
		if(str_num!=2)
		{
			SendTraceMsg("Format error in [%s]: %s", szSectionName, str);
			break;
		}
		
		if(!strcmp(strbuf[0], szKeyName))
		{
			strcpy(szString, strbuf[1]);
			fclose(fp);
			return TRUE;
		}
	}
	
	fclose(fp);
	return FALSE;	
}


int GetProfInt(char* szFileName, char* szSectionName, char* szKeyName, int* iValue)
{
	char	buff[256];

	if(GetProfString(szFileName, szSectionName, szKeyName, buff))
	{
		if(1==sscanf(buff, "%d", iValue))
			return TRUE;
		else
			SendTraceMsg("Format error in [%s]: %s", szSectionName, szKeyName);
	}
	return FALSE;
}


int GetProfSection(char* szFileName, char* szSectionName, char* szString, int nSize)
{
	FILE	*fp;
	char	buff[256];
	int		readSize=0, len, copySize;
	
	if((fp=fopen(szFileName, "r"))==NULL)
	{
		SendTraceMsg("Fail to open %s", szFileName);
		return FALSE;
	}
	
	if(-1==LocateKeyword(fp, szSectionName))
	{
		SendTraceMsg("Fail to find [%s] section", szSectionName);
		fclose(fp);
		szString[0]=szString[1]= 0;
		return FALSE;
	}
	
	while( NULL != fgets(buff, sizeof(buff), fp) )
	{
		if( (strstr(buff, "[") && strstr(buff, "]")) || strstr(buff, "END") )	
			break;
		
		len= strlen(buff);
		copySize= (nSize-2-readSize)<len? (nSize-2-readSize):len;
		strncpy(&szString[readSize], buff, copySize);
		readSize += copySize;
	}
	
	fclose(fp);
	szString[readSize]=szString[readSize+1]= 0;
	return readSize;
}


int GetCommParamConf(COMM_PARAM_INFO *comm_param_info, COMM_TASK_INFO *pTaskInfo)
{
	char	*filename=CONFIG_DEFINE_FILE;
	char	string[256], sect_name[MAX_DEF_SECTION_NAME_SIZE];
	int		value;
	
	strcpy(sect_name, pTaskInfo->sect_name);
	if(!GetProfInt(filename, sect_name, "DATA_SWAP", &value))
		return FALSE;
	pTaskInfo->swap_flag= value;
	SendTraceMsg("comm data swap flag: %d", pTaskInfo->swap_flag);
	
	strcpy(sect_name, pTaskInfo->sect_name);
	if(!GetProfInt(filename, sect_name, "DATA_BYTES", &value))
		return FALSE;
	if(value!=2 && value!=4)
	{
		SendTraceMsg("comm data_bytes: %d must be 2 or 4", pTaskInfo->data_bytes);
		return FALSE;
	}
	pTaskInfo->data_bytes= value;
	SendTraceMsg("comm data_bytes: %d", pTaskInfo->data_bytes);
	
	if(!GetProfString(filename, sect_name, "PORT", string))
		return FALSE;
	if(strlen(string)>=MAX_COMM_PORT_NAME_SIZE)
	{
		SendTraceMsg("comm port name %s too long: max %d", string, MAX_COMM_PORT_NAME_SIZE);
		return FALSE;
	}
	strcpy(comm_param_info->port, string);
	SendTraceMsg("comm port: %s", comm_param_info->port);
	if(!sscanf(string+strlen(string)-1, "%d", &pTaskInfo->port_id))
	{
		SendTraceMsg("comm port name %s error", string);
		return FALSE;
	}
	if(pTaskInfo->port_id <0 || pTaskInfo->port_id >7)
	{
		SendTraceMsg("comm port name %s error", string);
		return FALSE;
	}
	
	if(!GetProfInt(filename, sect_name, "MODE", &value))
		return FALSE;
	comm_param_info->mode= value;
	SendTraceMsg("comm port mode: %d", comm_param_info->mode);
	
	if(!GetProfInt(filename, sect_name, "BAUD", &value))
		return FALSE;
	comm_param_info->baud= value;
	SendTraceMsg("comm port baud: %d", comm_param_info->baud);
	
	if(!GetProfInt(filename, sect_name, "DATA_BITS", &value))
		return FALSE;
	comm_param_info->data_bits= value;
	SendTraceMsg("comm port data_bits: %d", comm_param_info->data_bits);
	
	if(!GetProfInt(filename, sect_name, "PARITY", &value))
		return FALSE;
	comm_param_info->parity= value;
	SendTraceMsg("comm port parity: %d", comm_param_info->parity);
	
	if(!GetProfInt(filename, sect_name, "START_POS", &value))
		return FALSE;
	pTaskInfo->start_pos= value;
	SendTraceMsg("comm start_pos: %d", pTaskInfo->start_pos);
	
	if(!GetProfInt(filename, sect_name, "DATA_NUM", &value))
		return FALSE;
	pTaskInfo->data_num= value;
	SendTraceMsg("comm data num: %d", pTaskInfo->data_num);
	
	if(!GetProfInt(filename, sect_name, "UNIT_NUM", &value))
		return FALSE;
	comm_param_info->unit_num= value;
	SendTraceMsg("comm unit_num: %d", comm_param_info->unit_num);

	if(!GetProfInt(filename, sect_name, "FIRST_UNIT_ADDR", &value))
		return FALSE;
	comm_param_info->first_unit_addr= value;
	SendTraceMsg("comm first_unit_addr: %d", comm_param_info->first_unit_addr);

	
	return TRUE;
}


int ComInit(COMM_PARAM_INFO	*comm_param_info)
{
	struct termios  	term;
	int		port_fd, rlen;
	tcflag_t	flag=0;
	
	port_fd= open(comm_param_info->port, O_RDWR | O_NDELAY);
	if( port_fd < 0 ) 
	{
		SendTraceMsg("Fail to open %s: errno= %d", comm_param_info->port, errno);
		return -1;
	}
	SendTraceMsg("Success to open %s", comm_param_info->port);
	
	/* initialize the port*/
	rlen = comm_param_info->mode;
	ioctl(port_fd, MOXA_SET_OP_MODE, &rlen);

	if(comm_param_info->baud==1200)
		flag |= B1200;
	else if(comm_param_info->baud==1800)
		flag |= B1800;
	else if(comm_param_info->baud==2400)
		flag |= B2400;
	else if(comm_param_info->baud==4800)
		flag |= B4800;
	else if(comm_param_info->baud==9600)
		flag |= B9600;
	else if(comm_param_info->baud==19200)
		flag |= B19200;
	else
	{
		SendTraceMsg("Unsupported baud: %d", comm_param_info->port);
		close(port_fd);
		return -1;
	}
	
	if(comm_param_info->data_bits==5)
		flag |= CS5;
	else if(comm_param_info->data_bits==6)
		flag |= CS6;
	else if(comm_param_info->data_bits==7)
		flag |= CS7;
	else if(comm_param_info->data_bits==8)
		flag |= CS8;
	else
	{
		SendTraceMsg("Unsupported data bits: %d", comm_param_info->data_bits);
		close(port_fd);
		return -1;
	}
	
	if(comm_param_info->parity==1)
		flag |= PARENB;
	else if(comm_param_info->parity==2)
		flag |= PARODD;
	else if(comm_param_info->parity!=0)
	{
		SendTraceMsg("Unsupported parity: %d", comm_param_info->parity);
		close(port_fd);
		return -1;
	}
	
	term.c_cflag = flag|CREAD|CLOCAL;
	term.c_iflag = 0;
	term.c_oflag = 0;
	term.c_lflag = 0;
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 0;

	tcflush(port_fd, 2);
	tcsetattr(port_fd, TCSANOW, &term);
	
	return port_fd;
}


int ComInitEx(COMM_PARAM_INFO	*comm_param_info, int block)
{
	struct termios  	term;
	int		port_fd, rlen;
	tcflag_t	flag=0;
	
	if(block)
		port_fd= open(comm_param_info->port, O_RDWR);
	else
		port_fd= open(comm_param_info->port, O_RDWR | O_NDELAY);

	if( port_fd < 0 ) 
	{
		SendTraceMsg("Fail to open %s: errno= %d", comm_param_info->port, errno);
		return -1;
	}
	SendTraceMsg("Success to open %s", comm_param_info->port);
	
	/* initialize the port*/
	rlen = comm_param_info->mode;
	ioctl(port_fd, MOXA_SET_OP_MODE, &rlen);
	
	if(comm_param_info->baud==1200)
		flag |= B1200;
	else if(comm_param_info->baud==1800)
		flag |= B1800;
	else if(comm_param_info->baud==2400)
		flag |= B2400;
	else if(comm_param_info->baud==4800)
		flag |= B4800;
	else if(comm_param_info->baud==9600)
		flag |= B9600;
	else if(comm_param_info->baud==19200)
		flag |= B19200;
	else
	{
		SendTraceMsg("Unsupported baud: %d", comm_param_info->port);
		close(port_fd);
		return -1;
	}
	
	if(comm_param_info->data_bits==5)
		flag |= CS5;
	else if(comm_param_info->data_bits==6)
		flag |= CS6;
	else if(comm_param_info->data_bits==7)
		flag |= CS7;
	else if(comm_param_info->data_bits==8)
		flag |= CS8;
	else
	{
		SendTraceMsg("Unsupported data bits: %d", comm_param_info->data_bits);
		close(port_fd);
		return -1;
	}
	
	if(comm_param_info->parity==1)
		flag |= PARENB;
	else if(comm_param_info->parity==2)
		flag |= PARODD;
	else if(comm_param_info->parity!=0)
	{
		SendTraceMsg("Unsupported parity: %d", comm_param_info->parity);
		close(port_fd);
		return -1;
	}
	
	term.c_cflag = flag|CREAD|CLOCAL;
	term.c_iflag = 0;
	term.c_oflag = 0;
	term.c_lflag = 0;
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 0;
	
	tcflush(port_fd, 2);
	tcsetattr(port_fd, TCSANOW, &term);
	
	return port_fd;
}


WORD GetCrcCheck(int byteNum, BYTE *buff)
{
	WORD CrcCheck;
	WORD CrcTemp,i,j;
	
	CrcCheck = 0xFFFF;
	for(i=0;i<byteNum;i++)
	{
		CrcCheck = CrcCheck^(WORD)(buff[i]);
		for(j=0;j<8;j++)
		{ 
			CrcTemp = CrcCheck & 0x0001;
			CrcCheck = CrcCheck >> 1;
			if(CrcTemp == 0x0001)
			{ 
				CrcCheck = CrcCheck ^ 0xA001;
			}
		}
	}
	return(CrcCheck);
}

WORD GetSumCheck(int byteNum, BYTE *buff)
{
	WORD sum=0;
	WORD i;
	
	for(i=0;i<byteNum;i++)
	{
		sum += buff[i];
	}
	return sum;
}



void PrintfData(int size, char *buf, int recv_flag)
{
	int		i;
	char	string[4096], *ptr;
	
	ptr= string;
	for(i=0; i<size; i++)
	{
		if(!(i%10) && i/10)
		{
			SendTraceMsg("%s", string);
			ptr= string;
		}
		ptr +=sprintf(ptr, "%02x ", buf[i]);
	}
	SendTraceMsg("%s", string);
	if(recv_flag)
		SendTraceMsg("Recv: %d bytes", size);
	else
		SendTraceMsg("Send: %d bytes", size);
}


void Swap2Byte(void *pShort)
{
	char tmp; 
	
	tmp =  *((char *)pShort); 
	*((char *)pShort)=*((char*)pShort+1); 
	*((char*)pShort+1) = tmp; 
}


void Swap4Byte(void *pInt)
{
	char tmp;
	
	tmp =  *((char *)pInt); 
	*((char *)pInt)=*((char*)pInt+3); 
	*((char*)pInt+3) = tmp; 
	tmp =  *((char *)pInt+1); 
	*((char *)pInt+1)=*((char*)pInt+2); 
	*((char*)pInt+2) = tmp; 
}



int xnet_select(SOCKET s, int sec, int msec, short x)
{
	int st = errno;
	struct timeval to;
	fd_set fs;
	
	if( s == INVALID_SOCKET )
	{
		SendTraceMsg("xnet_select, sock = -1, error!\n"); 
		return -1;
	}
	to.tv_sec = sec;
	to.tv_usec = msec * 1000;
	FD_ZERO(&fs);
	FD_SET(s, &fs);
	switch(x)
	{
	case READ_STATUS:
		st = select(s+1, &fs, 0, 0, &to);
		break;
	case WRITE_STATUS:
		st = select(s+1, 0, &fs, 0, &to);
		break;
	case EXCPT_STATUS:
		st = select(s+1, 0, 0, &fs, &to);
		break;
	}
	return(st);
}






