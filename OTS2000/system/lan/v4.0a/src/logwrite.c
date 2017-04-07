/************************************************************************************************************************
FILENAME:	logwrite.c

FUNCTION:	
	
	source code of  logwrite module,include:
	(1) read data from log shared memory continually 
	(2) write every data record to log file

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

*************************************************************************************************************************/

#include "../inc/log_all.h"

int CreateLogShm();
int MapLogShm();
int GetLogFileName(struct tm datetime,int filenum,char *filename);
int WriteLogFile(FILE *fp1,char *logstr);
void main(int argc, char *argv[])
{   
 	int count,loopcount;
 	int endptr;
 	char logstr[LOG_INFO_SIZE+100];			/*one log record string*/
 	FILE *fp;							
	char    filename[100];  
	int 	filenum;
	struct  tm datetime,newdatetime;
	time_t now;
	wsa_startup();
	SetLogLevel(argc, argv);
   	time( &now );
 	//CreateLockFile("logwrite.dxe"); 
	datetime = *localtime( &now );
	count=0;
	loopcount=0;
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== call Lan_init_all() error\n");		
		exit(0);
	}
	
	init_prog_name();
	
	/*
	shm_log_addr->flag=IDLE_FLAG;
	shm_log_addr->head=0;
	*/
	filenum=1;
	
	GetLogFileName(datetime,filenum,filename);

	if (( fp = fopen ( filename,"w+"))==NULL)
    {
        printf ("(^_^) -- can't open (w) the file %s!  -- (^_^)\n",filename );
		return;
    }
    else
    {
        printf ( "\n(^_^) -- open (w) %s  success! -- (^_^)\n",filename );
    }
    printf("\n(^_^) --- waitting for new log ...... \n\n");	
	endptr =shm_log_addr->head;
	for(;;)
	{
		/* printf("endptr=%d; head=%d\n",endptr,shm_log_addr->head); */
		while(endptr!=shm_log_addr->head)
		{
			MySleep(10);
          		if(endptr==MAX_LOG_NUM)  
          		{	
          			endptr=0;			
          			fclose(fp);
				MySleep(2000);
				filenum++;
				GetLogFileName(datetime,filenum,filename);
				if (( fp = fopen ( filename,"w+"))==NULL)
        			{
        				printf ("(^_^) -- can't open (w) the file %s!  -- (^_^)\n",filename );
					return;
        			}
        			else
        			{
        				printf ( "\n(^_^) -- open (w) %s  success! -- (^_^)\n",filename );
       				}
        			loopcount=0;
        			count = 0;
        			break;
          		}
 				/*
				if(shm_log_addr->shm_log_seg[endptr].errtype==ERR) 
				{
				*/
          			LogRecToStr(shm_log_addr->shm_log_seg[endptr],logstr);
					count++;
	          		WriteLogFile(fp,logstr);
    			endptr++;
		}
		
		/* if current day differ from old day,then open the next file */
		time( &now );
		newdatetime = *localtime( &now );
		if(newdatetime.tm_mday!=datetime.tm_mday)
		{
          	datetime=newdatetime;
          	endptr=0;
			shm_log_addr->head=0;  
          	count=0;
          	loopcount=0;
          	fflush(fp);
          	fclose(fp);
			MySleep(2000);
			filenum=1;
			GetLogFileName(datetime,filenum,filename);
			if (( fp = fopen ( filename,"w+"))==NULL)
        		{
        			printf ("(^_^) -- can't open (w) the file %s!  -- (^_^)\n",filename );
				return;
        		}
        		else
        		{
        			printf ( "\n(^_^) -- open (w) %s  success! -- (^_^)\n",filename );
       			}        					
		}
		if(count>=1)
		{
			fflush(fp);
			count=0;
		}
		loopcount++;
		/*if loopcount reach MAX,then reopen the file*/
		if(loopcount>=1000)
		{
			fclose(fp);
			MySleep(2000);
			if (( fp = fopen ( filename,"a+"))==NULL)
        		{
        			printf ("(^_^) -- can't open (w) the file %s!  -- (^_^)\n",filename );
				return;
        		}
        		loopcount=0;
		}	

		MySleep(2000);
		/*return;*/
	}
	fclose(fp);
}	

int GetLogFileName(struct tm datetime,int filenum,char *filename)
{
	char 	tmpstr[20]; 
	strcpy(filename,"/home/ems/h9000/his/trap/");
	sprintf(tmpstr,"H9000LOG%04d-%02d-%02d",datetime.tm_year+1900,
		datetime.tm_mon+1,datetime.tm_mday);	
	strcat(filename,tmpstr);
	strcpy(tmpstr,"");
	sprintf(tmpstr,"%03d.LOG",filenum);
	strcat(filename,tmpstr);
	return TRUE;
}


int WriteLogFile(FILE *fp1,char *logstr)
{
	unsigned int i;
	for(i=0;i<strlen(logstr);i++)
	{
		if(logstr[i] == '\n')
			logstr[i] ='\t';
	}
	fprintf(fp1,"%s\n",logstr);
	return TRUE;
}
