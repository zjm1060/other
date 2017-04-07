#define lint 
#include "../inc/hdbhead_all.h"
int UpdateYcReal();
int UpdateYcPeriod();
int InsertAlReal();
int UpdateYcReal_New();
int UpdateSpecial();
int InsertAlReal_New();
int SetReadPtInfoFlag();
int GetClob();
static  Cda_Def	cda; 
static  Lda_Def lda; 
int CrtID =0 ;
int		i;
int		tmp;
int		mode;
char TableName[]="ems.speciality";
char		LogiName[55]; 
char		TimeTag[22];
float		NewValue;
int 		PointNum;
unsigned short	status;
time_t mclock;
struct tm *newtime; 
char LoginStr[50];

int main( void )
{
	int mode,i,type,id;
 	int count;
 	unsigned char curid=0;
 	int rscount=0;
 	char str[22];
	IND_REC rec;
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
  	ReadDefFile();
	sprintf(strPwd,"%s/%s@",strUsername,strPassword);
 	strcpy(LoginID,strPwd);
 	strcat(LoginID,SIDSTR);
	strcpy(LoginStr,LoginID);	
	if(MountDB(DB_NAME) == -1)
	{
		printf("\n==== curvescan.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("\n==== curvescan.c: call Lan_init_all() error\n");		
		exit(0);
	}
	get_hdb_shm();

	if( ConnectHDB(&lda,&cda,LoginID)==FALSE )
	{	printf("ConnectHDB() Orcal error !   ( %s )\n",SIDSTR);  exit(-1); }
	else
	{	printf("\n=== Connect '%s' Success \n",SIDSTR); }
#ifdef SUN
	signal(SIGINT,sig_termp);	
#endif
	i=0;
	for(;;)
	{
		printf("\n\n\n======== 请选择: ========\n"); 
		printf("\n   00.重新读取点表"); 
		printf("\n\n   01.查看曲线共享区"); 
		printf("\n   02.写曲线共享区");
		printf("\n   03.查看曲线详细信息");  
		printf("\n   04.查看历史报警共享区");
		printf("\n   05.写历史报警共享区");
		printf("\n   06.详细查看历史报警");
		printf("\n   07.查看事故回放共享区");
		printf("\n   08.写事故回放共享区");
		printf("\n   09.详细查看事故回放");
		printf("\n   10.查看场景共享区");
		printf("\n   11.写场景共享区");

		printf("\n\n   20.退出\n");

		printf("\n====请输入(0-20):");
		  
		scanf("%d",&mode);
		if(mode==1)
		{
			printf("the data in curve shared memory is:\n");
			printf("\nqueryflag=%d",shm_hdb_addr->data_curve[CrtID].queryflag);
			printf("\nquerytype=%d",shm_hdb_addr->data_curve[CrtID].querytype);
			printf("\nlockflag=%d",shm_hdb_addr->data_curve[CrtID].lockflag);
			printf("\ncurvenum=%d",shm_hdb_addr->data_curve[CrtID].num);
			printf("\ninterval=%d",shm_hdb_addr->data_curve[CrtID].interval);
			printf("\nstarttime=%s",shm_hdb_addr->data_curve[CrtID].starttime);
			printf("\nendtime=%s",shm_hdb_addr->data_curve[CrtID].endtime);
			printf("\nseg[0].logicalname=%s",shm_hdb_addr->data_curve[CrtID].seg[0].logicalname);	
			printf("\nseg[1].logicalname=%s",shm_hdb_addr->data_curve[CrtID].seg[1].logicalname);	
			printf("\nseg[2].logicalname=%s",shm_hdb_addr->data_curve[CrtID].seg[2].logicalname);	
			printf("\nseg[3].logicalname=%s",shm_hdb_addr->data_curve[CrtID].seg[3].logicalname);	
			printf("\nseg[4].logicalname=%s",shm_hdb_addr->data_curve[CrtID].seg[4].logicalname);	
			printf("\nseg[5].logicalname=%s",shm_hdb_addr->data_curve[CrtID].seg[5].logicalname);		
			printf("\nseg[0].rscount=%d",shm_hdb_addr->data_curve[CrtID].seg[0].rscount);	
			printf("\nseg[1].rscount=%d",shm_hdb_addr->data_curve[CrtID].seg[1].rscount);	
			printf("\nseg[2].rscount=%d",shm_hdb_addr->data_curve[CrtID].seg[2].rscount);	
			printf("\nseg[3].rscount=%d",shm_hdb_addr->data_curve[CrtID].seg[3].rscount);
			printf("\nseg[4].rscount=%d",shm_hdb_addr->data_curve[CrtID].seg[4].rscount);	
			printf("\nseg[5].rscount=%d",shm_hdb_addr->data_curve[CrtID].seg[5].rscount);	
		}
		else if(mode==2)
		{	
  			shm_hdb_addr->data_curve[CrtID].querytype=0;
			shm_hdb_addr->data_curve[CrtID].lockflag=LOCK_FLAG  ;    /* IDLE_FLAG */
			shm_hdb_addr->data_curve[CrtID].num=1;
			sprintf(shm_hdb_addr->data_curve[CrtID].starttime,"2008-9-18 09:00:00");
			sprintf(shm_hdb_addr->data_curve[CrtID].endtime,"2008-9-18 09:30:00");
			sprintf(shm_hdb_addr->data_curve[CrtID].seg[0].logicalname,"RB.LCU33R.ANA.PC2__L9UCA_AI04");	
			shm_hdb_addr->data_curve[CrtID].interval=ONE_SEC; 
			shm_hdb_addr->data_curve[CrtID].queryflag=1;
			shm_hdb_addr->data_curve[CrtID].lockflag= IDLE_FLAG;   
			 
			printf("write query request to shm successfully!\n");   
		}
		else if(mode==3)
		{	
			printf(" \n(^_^) HDB curve sharem memory:(^_^)\n");
			for(curid=0;curid<shm_hdb_addr->data_curve[CrtID].num;curid++)
			{
				printf(" \n(^_^) ( curid= %d ) rscount=%d lasttime=%s ! (^_^)\n",curid,shm_hdb_addr->data_curve[CrtID].seg[curid].rscount,shm_hdb_addr->data_curve[CrtID].seg[curid].lasttime );
				for(i=0;i<shm_hdb_addr->data_curve[CrtID].seg[curid].rscount;i++)
				{
					printf("i=%d, %8.3f ,  %s\n",i,shm_hdb_addr->data_curve[CrtID].seg[curid].data[i],shm_hdb_addr->data_curve[CrtID].seg[curid].timestr[i]);
				}
			}
		}
		else if(mode==4)
		{
			printf("the data in alarm shared memory is:\n");
			printf("\nconnflag=%d",shm_hdb_addr->connflag_al);
			printf("\nqueryflag=%d",shm_hdb_addr->data_al.queryflag);
			printf("\nquerytype=%d",shm_hdb_addr->data_al.querytype);
			printf("\nlockflag=%d",shm_hdb_addr->data_al.lockflag);
			printf("\nalnum=%d",shm_hdb_addr->data_al.alnum);
			printf("\nstarttime=%s",shm_hdb_addr->data_al.starttime);
			printf("\nendtime=%s",shm_hdb_addr->data_al.endtime);
			printf("\nfilter_stnid=%d",shm_hdb_addr->data_al.filter_stnid);	
			printf("\nfilter_devid=%d",shm_hdb_addr->data_al.filter_devid);	
			printf("\nfilter_typeid=%d",shm_hdb_addr->data_al.filter_typeid);					
			printf("\ncname=%s",shm_hdb_addr->data_al.cname);	
			printf("\ncname_type=%d",shm_hdb_addr->data_al.cname_type);					

		}
		else if(mode==5)
		{	
  			shm_hdb_addr->data_al.queryflag=1;
  			shm_hdb_addr->data_al.querytype=0;
			shm_hdb_addr->data_al.lockflag=LOCK_FLAG  ;    /* IDLE_FLAG */
			strcpy(shm_hdb_addr->data_al.starttime,"2008-5-6 10:00:00");
			strcpy(shm_hdb_addr->data_al.endtime,"2008-5-6 12:00:00");
			shm_hdb_addr->data_al.filter_stnid=1;
			shm_hdb_addr->data_al.filter_devid=0;
			shm_hdb_addr->data_al.filter_typeid=0;		
			shm_hdb_addr->data_al.cname_type = 1;
			//strcpy(shm_hdb_addr->data_al.cname,"空冷器冷却水流量低");		
			shm_hdb_addr->data_al.lockflag= IDLE_FLAG;   
			 
			printf("write query request to shm successfully!\n");   
		}
		else if(mode==6)
		{	
			printf("the data in alarm shared memory is:\n");
			printf("\nqueryflag=%d",shm_hdb_addr->data_al.queryflag);
			printf("\nquerytype=%d",shm_hdb_addr->data_al.querytype);
			printf("\nlockflag=%d",shm_hdb_addr->data_al.lockflag);
			printf("\nalnum=%d",shm_hdb_addr->data_al.alnum);
			printf("\nstarttime=%s",shm_hdb_addr->data_al.starttime);
			printf("\nendtime=%s",shm_hdb_addr->data_al.endtime);
			printf("\nfilter_stnid=%d",shm_hdb_addr->data_al.filter_stnid);	
			printf("\nfilter_devid=%d",shm_hdb_addr->data_al.filter_devid);	
			printf("\nfilter_typeid=%d",shm_hdb_addr->data_al.filter_typeid);
			
			printf("\n\nalarm data buffer:\n");
			for(i=0;i<shm_hdb_addr->data_al.alnum;i++)
			{
				printf("i=%02d: %15s, %22s, %8d,  %8.3f\n",i,shm_hdb_addr->data_al.rec_data[i].l_name,
					shm_hdb_addr->data_al.rec_data[i].TimeTag,shm_hdb_addr->data_al.rec_data[i].status,
					shm_hdb_addr->data_al.rec_data[i].fvalue);
			}

			printf("\n\n\nrecord data buffer:\n");
			printf("\n\nrec_header_ptr=%d\n",shm_hdb_addr->data_al.hdb_rec.head.header_ptr);
			for(i=0;i<shm_hdb_addr->data_al.hdb_rec.head.header_ptr;i++)
			{
				rec=shm_hdb_addr->data_al.hdb_rec.rec[i].ind_rec;
				printf("i=%02d: %d, %d, %d, %d, %d	%d-%d-%d %d:%d:%d\n",i,
					rec.type_id,rec.stn_id,rec.dev_id,rec.type_id,rec.pt_id, 
					rec.year,rec.month,rec.day,rec.hour,rec.minute,rec.second);
			}			


						
		}	
		else if(mode==7)
		{
			printf("the data in revert shared memory is:\n");
			printf("\nconnflag=%d",shm_hdb_addr->connflag_revert);
			printf("\nqueryflag=%d",shm_hdb_addr->data_revert.queryflag);
			printf("\nquerytype=%d",shm_hdb_addr->data_revert.querytype);
			printf("\nlockflag=%d",shm_hdb_addr->data_revert.lockflag);
			printf("\nycnum=%d",shm_hdb_addr->data_revert.ycnum);
			printf("\nyxnum=%d",shm_hdb_addr->data_revert.yxnum);
			printf("\nstarttime=%s",shm_hdb_addr->data_revert.starttime);
			printf("\nendtime=%s",shm_hdb_addr->data_revert.endtime);
			printf("\ncurptr=%d",shm_hdb_addr->data_revert.curptr );
			for(i=0;i<shm_hdb_addr->data_revert.ycnum;i++)
			{
				printf("\nyc_data[%d].logicalname=%s, rscount=%d",i,shm_hdb_addr->data_revert.yc_data[i].logicalname, 
				shm_hdb_addr->data_revert.yc_data[i].rscount);
			}
			for(i=0;i<shm_hdb_addr->data_revert.yxnum;i++)
			{
				printf("\nyx_data[%d].logicalname=%s, rscount=%d",i,shm_hdb_addr->data_revert.yx_data[i].logicalname, 
				shm_hdb_addr->data_revert.yx_data[i].rscount);
			}	

		}
		else if(mode==8)
		{	
  			shm_hdb_addr->data_revert.querytype=0;
			shm_hdb_addr->data_revert.lockflag=LOCK_FLAG  ;    /* IDLE_FLAG */
			shm_hdb_addr->data_revert.ycnum=2;
			shm_hdb_addr->data_revert.yxnum=0;
			strcpy(shm_hdb_addr->data_revert.starttime,"2009-3-12 17:29:00");
			strcpy(shm_hdb_addr->data_revert.endtime,"2009-3-12 17:33:00");
			strcpy(shm_hdb_addr->data_revert.yc_data[0].logicalname,"BS.BS_F1.ANA.ZS");	
			strcpy(shm_hdb_addr->data_revert.yc_data[1].logicalname,"BS.BS_F1.ANA.LC_V");
			for(i=0;i<shm_hdb_addr->data_revert.yxnum/2;i++)
			{
				strcpy(shm_hdb_addr->data_revert.yx_data[2*i].logicalname,"RB.LCU22R.POL.PC1__AC220_DI48"); 
				strcpy(shm_hdb_addr->data_revert.yx_data[2*i+1].logicalname,"RB.LCU22R.POL.PC1__DC220_DI48");				
			}	
			shm_hdb_addr->data_revert.queryflag=1;
			shm_hdb_addr->data_revert.lockflag= IDLE_FLAG;   
			printf("write query request to shm successfully!\n");
		}
		else if(mode==9)
		{	
			printf("\n====请选择(1: 遥测  2:遥信):");
			scanf("%d",&type);
			printf("\n====请输入id号:");
			scanf("%d",&id);
			if(type == 1)
			{
				if( id>=shm_hdb_addr->data_revert.ycnum )
				{
					printf("\ninput invalid \n");
					break;
				}
				printf(" \n ( curid= %d )  yc name=%s, rscount=%d  ! \n",id,shm_hdb_addr->data_revert.yc_data[id].logicalname,shm_hdb_addr->data_revert.yc_data[id].rscount);
				for(i=0;i<shm_hdb_addr->data_revert.yc_data[id].rscount;i++)
				{
					printf("i=%d, %8.3f ,  %s\n",i,shm_hdb_addr->data_revert.yc_data[id].value[i],shm_hdb_addr->data_revert.yc_data[id].timestr[i]);
				}
			}
			else
			{
				if( id>=shm_hdb_addr->data_revert.yxnum )
				{
					printf("\ninput invalid \n");
					break;
				}
				printf(" \n ( curid= %d )  yc name=%s, rscount=%d  ! \n",id,shm_hdb_addr->data_revert.yx_data[id].logicalname,shm_hdb_addr->data_revert.yx_data[id].rscount);
				for(i=0;i<shm_hdb_addr->data_revert.yx_data[id].rscount;i++)
				{
					printf("i=%d, %d ,  %s\n",i,shm_hdb_addr->data_revert.yx_data[id].status[i],shm_hdb_addr->data_revert.yx_data[id].timestr[i]);
				}
			}
		}
		else if(mode==10)
		{

			shm_hdb_addr->write_alm_flag = 1;		
		}	
		else if(mode==11)
		{	

		}
	
		else if(mode==0)
		{
			SetReadPtInfoFlag();
		}
		else
		{
			printf("input error!\n");
			return FALSE;
		}
	}/*end for*/
	return TRUE;   
 
}



int UpdateYcReal()
{
		printf("\n\n=== Please input record number to be updated :  ");
		scanf("%d",&PointNum);
		time( &mclock );  
	        newtime=  localtime(&mclock) ;
		printf("\n=== starttime is: %s \n",asctime(newtime));
		for(i=0;i<PointNum;i++)
		{
			strcpy(LogiName,"GBX.GBX_F5.ANA.Uca");
			strcpy(TimeTag,"2006-01-04 09:20:00");
			NewValue =i ;
			status =1;
			UpdateHdbByYc( cda, LogiName, TimeTag, &NewValue, status);
		}
		time( &mclock );  
	        newtime=  localtime(&mclock) ;
		printf("\n=== endtime is: %s \n",asctime(newtime));	

	return 0;
}


int InsertAlReal()
{
		int msecond;
		printf("\n\n=== Please input record number to be inserted :  ");
		scanf("%d",&PointNum);
		time( &mclock );  
	        newtime=  localtime(&mclock) ;
		printf("\n=== starttime is: %s \n",asctime(newtime));
		for(i=0;i<PointNum;i++)
		{
			strcpy(LogiName,"GBX.GBX_F5.ANA.Uca");
			strcpy(TimeTag,"2006-01-04 09:20:03");
			msecond =i ;
			status =1;
			
		}
		time( &mclock );  
	        newtime=  localtime(&mclock) ;
		printf("\n=== endtime is: %s \n",asctime(newtime));	

	return 0;
}

int InsertAlReal_New()
{
		int msecond;
		printf("\n\n=== Please input record number to be inserted :  ");
		scanf("%d",&PointNum);
		time( &mclock );  
	        newtime=  localtime(&mclock) ;
		printf("\n=== starttime is: %s \n",asctime(newtime));
		for(i=0;i<PointNum;i++)
		{
			strcpy(LogiName,"GBX.GBX_F5.ANA.Uca");
			strcpy(TimeTag,"2006-01-04 09:20:03");
			msecond =i+20000 ;
			status =1;
			
		}
		time( &mclock );  
	        newtime=  localtime(&mclock) ;
		printf("\n=== endtime is: %s \n",asctime(newtime));	

	return 0;
}

int UpdateYcPeriod()
{
	return 0;
}

int GetClob()
{
	struct  hdb_curve rs;
	int		curid =0;
	rs.seg[curid].lastrscount = rs.seg[curid].rscount;
	strcpy(rs.starttime,"2005-01-1 09:20:03");	
	strcpy(rs.endtime,"2006-06-15 09:20:03");
	strcpy(rs.seg[curid].logicalname,"LYX.LYX_F1.ANA.DZ_Uab_V");
	rs.seg[curid].stnid=1;
	rs.seg[curid].devid=1;
	rs.interval = ONE_SEC;	
	SelectCurveFromHdb(cda, curid,&rs);	
	return 0;
}


int UpdateSpecial()
{
/*
  char cmdstr[]= "update ems.speciality set datetime= '2006-06-02 14:12:52',data= 20.0, quality_code= 1 where logicalname= 'LYX.LYX_F1.ANA.DZ_A_V'";
  ExecuteSQL(cmdstr);
*/
  float val=35;
  UpdateHdbByYc( cda, "LYX.LYX_F1.ANA.DZ_A_V", "2006-06-05 09:00:00", &val, 1);
  
}

int SetReadPtInfoFlag()
{
	int mode;
	printf("\n是否确定需要重新读取点表?(0: 否  1:是):");
	scanf("%d",&mode);
	if(mode == 1)
	{
		shm_hdb_addr->readpt_real = 1;
		shm_hdb_addr->readpt_curve = 1;
		shm_hdb_addr->readpt_period = 1;
		printf("\n\n信号已发出, 写库程序2分钟将重读所有点信息!");
	}
	return TRUE;
}