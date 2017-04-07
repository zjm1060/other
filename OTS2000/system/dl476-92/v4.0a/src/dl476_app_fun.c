/******************************************************************************
**	This program is designed for
**		(1)	format convert between dl476-92 and H9000                        
**		(2)	Data list	
**		(3) 	Host online/offline
**		(4) 	ReadRealDB
**		(5)	Package
**                                                                          
**	History																	 
**																			
**	Date		author		comments
**	2004.5		Yuan/Hong	First version
**  2010.04		V3.0a-->V4.0a  by  zwj
**
******************************************************************************/


#include	"../inc/dl476_std.h"
#include	"../inc/dl476_data.h" 

char		*def_path="/home/ems/h9000/dl476-92/";
unsigned char	cmd_src;

/******************************************************************

 		模拟量转换：dl476-92  ----->H9000 

********************************************************************/

/*QualityToAnaStatus(unsigned char quality)
 {
	unsigned short status;

	status	=0;
 
	if((quality & DATA_INVALID)!=0)
		status = status | CHAN_FAIL;

	if((quality & DATA_ALARM)!=0)
		status = status | LMT_ATAT;

 	if((quality & DATA_MANSET)!=0)
		status= status | MAN_SET;

 	return(status);
}*/

/*ANLG_DATA	FloatToAnaData(float fdata)
{
	ANLG_DATA	ana;
	unsigned short	sgn,pnt,cvt;


	if( fdata <0 )
	{
		fdata = -fdata;
		sgn = 0x80;
	}
	else
		sgn = 0;
	
	if(fdata < 65.535)
	{
		pnt   = 0x060;
		cvt   = 1000;
	}
	else if ( fdata < 655.35 )
	{
		pnt   = 0x040;
		cvt   = 100;
	}
	else if ( fdata < 6553.5 )
	{
		pnt   = 0x020;
		cvt   = 10;
	}
	else
	{
		pnt   = 0;
		cvt   = 1;
	}

	ana.status = sgn | pnt;
	ana.data = (unsigned short)(fdata*cvt);

	printf("fdata=%f data=%d status=%d\n",fdata,ana.data,ana.status);
	return(ana);
}*/

/******************************************************************

 		模拟量转换：H9000  -----> dl476-92

********************************************************************/

unsigned char	AnaStatusToQuality(ANLG_STATUS status)
{
	unsigned char quality;
	unsigned short status1;

	quality	=0;
	status1 =0;
	status1 =status1|(status.lmt_state<<2);  /* 4.0 "8	复下下限"会出错，因为3.0 最大表示到7*/
	status1 =status1|(status.chan_fail<<1);
/*	if((status & CHAN_FAIL)!=0)
		quality = status | DATA_INVALID;

	if((status & LMT_ATAT)!=0)
		quality = status | DATA_ALARM;

 	if((status & MAN_SET)!=0)
		quality= status | DATA_MANSET;*/

	if(status.chan_fail)
		quality = status1 | DATA_INVALID;

	if(status.lmt_state)
		quality = status1 | DATA_ALARM;

 	if(status.overflow)
		quality= status1 | DATA_MANSET;

 	return(quality);
}

/*float AnaDataToFloat(unsigned short data,unsigned short status)
{
	float			fdata;

	fdata=(float)(data);
	
	switch(status&0x060)			
	{
		case 0x20:
			fdata = (float)(fdata*0.1);
			break;
		case 0x40:
			fdata = (float)(fdata*0.01);
			break;
		case 0x60:
			fdata = (float)(fdata*0.001);
			break;
		default:
			fdata = (float)fdata;
			break;
	}

	if( (status & 0x80)!=0)					
		fdata = -fdata;

	return(fdata);
}*/

/******************************************************************

 		状态量转换：H9000 -----> dl476-92 格式

********************************************************************/

unsigned char H9000SWToGB(IND_DATA status)
{

	unsigned short	result;

 	result= status.rt_state; 			/* 开关状态*/

	if (status.chan_fail)			/* 数据质量*/
	
		result=result | DATA_INVALID;

	if (status.man_set)
	
		result=result | DATA_MANSET;

	/*printf("result=%d\n",result);*/

	return(result);

}

/******************************************************************

 		状态量转换：dl476-92 格式 ----->  H9000

********************************************************************/
unsigned short GBSWToH9000(unsigned char status)
{
	unsigned short	result;

	result=(short)(status & 0x03)<<2;

	if (!(status & DATA_INVALID))
	
		result=result | CHAN_FAIL;

	if (!(status & DATA_MANSET))
	
		result=result | MAN_SET;

 	return(result);
}

int GenTableConf()		
{
	FILE			*fp;

	unsigned short	j;
	
	int				no;

	char			tmp[100];


	strcpy(tmp,def_path);
	strcat(tmp,"table_conf.def");

 	if((fp=fopen(tmp,"r"))==NULL)
 	{
 		printf("can't open file %s\n",tmp);
		return(FALSE);          
	}

	rewind(fp);	
	
	for(j=0;j<MAX_TABLE_NUM;j++)
	{
		fscanf(fp,"%d",&no);
		table[j].no=(char)no;
		
		fscanf(fp,"%s",tmp);
		if(strcmp(tmp,"ANA")==0)
			table[j].type=ANA_TABLE;
		else if(strcmp(tmp,"SW")==0)
			table[j].type=SW_TABLE;
		else if(strcmp(tmp,"SWCTRL")==0)
			table[j].type=SWCTRL_TABLE;
		else
			table[j].type=0;

		fscanf(fp,"%s",table[j].file);
	}
	
	fclose(fp);

	return(TRUE); 

}

int GenDATAIndex()		
{
	FILE			*fp;
	DMS_COMMON		dbentry;
	char			filename[100],tmp[100];

	unsigned short	j1,j,k;

	j=0;

 	for(k=0;k<MAX_TABLE_NUM;k++)
	{
		
		if(table[k].type==SWCTRL_TABLE)	continue;

		if(strcmp(table[k].file,"")==0)	continue;
		
		j1=-1;
 
		strcpy(filename,def_path);
		strcat(filename,table[k].file);

		if(strcmp(filename,"")==0)	continue;

		if((fp=fopen(filename,"r"))==NULL)
 		{
 				printf("can't open file %s\n",filename);
				return(FALSE);  
		}
		else
		{
			printf(" open file %s ok \n",filename);	
		}
		
		table[k].baseaddr=j;	
		  
		rewind(fp);	
	
		while(!feof(fp))
		{
			if(j>=MAX_DATA_SIZE)	break;

			fscanf(fp,"%s %s",dataindex[j].name,tmp);
	
			if(strcmp(dataindex[j].name,"")!=0)
			{ 
				/*ReadEntryByName(index[j].name,&data_type,&dbentry);
				if(dbentry.err_code==-1)*/
				if(ReadEntryByNameStr(dataindex[j].name,&dbentry)==-1)
				{
					printf("dl476_app_fun:: %d pnt: %s No this point in the database file!!!\n",j,dataindex[j].name);

				}
				else
				{
				
					dataindex[j].stn_id=dbentry.point.stn_id;
					dataindex[j].dev_id=dbentry.point.dev_id;
					dataindex[j].data_type=dbentry.point.data_type;
					dataindex[j].point_id=dbentry.point.pt_id;
					dataindex[j].tableno=table[k].no;
					dataindex[j].tabletype=table[k].type;
					/*
                                        if((index[j].dev_id==4)&&(index[j].data_type==12)&&(index[j].point_id==4))
                                        {
                                        printf("\n************************************");
					printf("index[%d]:: name=%s dev=%d type=%d pt=%d tableno=%d\n",j,index[j].name,
					index[j].dev_id,index[j].data_type,index[j].point_id,index[j].tableno);
                                        printf("\n************************************\n");
                                        }
                                        */					
				}

				j1++;
				j++;
			}
			else
			{	  break;}
		}	
 
		table[k].ptnum=j1+1;
		
		printf("GenDATAIndex:: table %d point number=%d  baseaddr=%d\n",k,table[k].ptnum,table[k].baseaddr);
		fclose(fp);
	}

	index_num=j;

	return(TRUE);
} 

int GenSWCTRLIndex()		
{
	FILE			*fp;
	DMS_COMMON		dbentry;
	char			filename[100],tmp[100];


	unsigned short	j1,j,k;

	j=0;

 	for(k=0;k<MAX_TABLE_NUM;k++)
	{

		if(table[k].type!=SWCTRL_TABLE)	continue;
		if(strcmp(table[k].file,"")==0)	continue;

		j1=-1;

		strcpy(filename,def_path);
		strcat(filename,table[k].file);
		
		if(strcmp(filename,"")==0)	continue;

 		if((fp=fopen(filename,"r"))==NULL)
 		{
 				printf("can't open file %s\n",filename);
				return(FALSE); 
		}
		  
		 printf("now open table %d:	%s\n",k,filename);
		  
		rewind(fp);	
	
		while(!feof(fp))
		{
			fscanf(fp,"%s %s",swctrl[j].name,tmp);
 	
			if(strcmp(swctrl[j].name,"")!=0)
			{

				if(j >=MAX_SWCTRL_SIZE)	break;

				/*ReadEntryByName(swctrl[j].name,&data_type,&dbentry);
				if(dbentry.err_code==-1)*/
				if(ReadEntryByNameStr(swctrl[j].name,&dbentry)==-1)
				{
				//	printf("dl476_app_fun:: %d pnt: %s No this point in the database file!!!\n",j,swctrl[j].name);
				 
				}
				else
				{
					swctrl[j].stn_id=dbentry.point.stn_id;
					swctrl[j].dev_id=dbentry.point.dev_id;
					swctrl[j].data_type=dbentry.point.data_type;
					swctrl[j].point_id=dbentry.point.pt_id;

					swctrl[j].tableno=table[k].no;
					swctrl[j].tabletype=table[k].type;
					
//					printf("swctrl[%d]:: name=%s dev=%d type=%d pt=%d\n",j,swctrl[j].name,
//						swctrl[j].dev_id,swctrl[j].data_type,swctrl[j].point_id);
				}
				
				j1++;
				j++;
			}
			else
			{	break;}
		}	
 
		table[k].ptnum=j1+1;
		
		printf("GenSWCTRLIndex:: table %d point number = %d\n",k,table[k].ptnum);
		fclose(fp);
	}

	swctrl_num=j;

	return(TRUE);
} 



int GenNodeTable()
{
	FILE			*fp;
	int				j;
	char			hostname[120];

	/* Get remote definition */

	strcpy(hostname,def_path);
	strcat(hostname,"remote_node.def");
	if((fp=fopen(hostname,"r"))==NULL)
	{
		printf("can't open file %s\n",hostname);
		return(FALSE);          
	}
	
	rewind(fp);	
	 
	fscanf(fp,"%d",&j);
	rnode.number=j;
	  
	if(rnode.number >= MAX_REMOTE_NODE)
	{
		printf("dl476_app_fun:: above the MAX_REMOTE_NODE %d\n",MAX_REMOTE_NODE);
		rnode.number = MAX_REMOTE_NODE;
	} 

	for(j=0;j<rnode.number;j++)
	{
		fscanf(fp,"%s %s",rnode.name[j],rnode.ip[j]);			
	}
	
	printf("GenNodeTable: remote node number = %d\n",j);	
	
	fclose(fp);

	/* Get local definition */

	strcpy(hostname,def_path);
	strcat(hostname,"local_node.def");

	if((fp=fopen(hostname,"r"))==NULL)
	{
		printf("can't open file %s\n",hostname);
		return(FALSE);          
	}
	
	rewind(fp);	
	
	fscanf(fp,"%d",&j);
	lnode.number=j;
	if(lnode.number > MAX_LOCAL_NODE)
	{
		printf("dl476_app_fun:: above the MAX_LOCAL_NODE %d\n",MAX_LOCAL_NODE);
		lnode.number = MAX_LOCAL_NODE;
	} 
	gethostname(hostname,20);
	
	for(j=0;j<lnode.number;j++)
	{
		fscanf(fp,"%s",lnode.name[j]);
		printf("local=%s host=%s\n",lnode.name[j],hostname);
		if(strcmp(lnode.name[j],hostname)==0)
		break;		
	}
		
	printf("GenNodeTable: local node number = %d\n",j);
	fclose(fp);
	

	if(j>=lnode.number)
	{
		printf(" dl476_app_fun:: not local node\n");
		return(FALSE);
	}
	else				
		return(TRUE);
	
}


 
short   GetWanCtrlPrvg()
{
        char            *getenv();
        char            host_name[15];
        DMS_COMMON      dms_cmm;
        short           status;
        char            entry_name[60]={""} ;

        strcpy( entry_name, DL476_SYS_TAB_NAME );
		gethostname (host_name,20);

        if (( strcmp( host_name , lnode.name[0]) ==0) ||
                ( strcmp( host_name , lnode.name[1]) ==0))
        {
#ifdef SINGLE_HOST
		return(CUR_WS_IN_HOST);		
#endif
                strcat( host_name, "_STAT");

                strcat( entry_name, host_name);
				if(ReadEntryByNameStr(entry_name,&dms_cmm)==-1)
                /*ReadEntryByName ( entry_name , &data_type, &dms_cmm);
 				if ( dms_cmm.err_code == -1)*/

                {
                        printf ("OIX: in GetCurWSCtrlPrvg%s is not found in database.\n",
                                        entry_name);
                        return(-1);
                }
                status  = dms_cmm.var.ind.status.rt_state;
							
                return( status);
        }
        else
        {
                status = 1;
                return( status);
        }
}

void ReadOnlineDB()
{
	unsigned short	j;
	DMS_COMMON		dbentry;
	int				data_type;
	
	for(j=0;j<index_num;j++)
	{  
		if(j>=500 && j%1000==0)	usleep(10000);		/*for cpu*/
		
		if(strcmp(dataindex[j].name,"")!=0)
		{
			/*ReadEntryByName(index[j].name,&data_type,&dbentry);
			if(dbentry.err_code==-1)*/
			if(ReadEntryByNameStr(dataindex[j].name,&dbentry)==-1)
			{
			//	printf("dl476_app_fun:: %d pnt: %s No this point in the database file!!!\n",j,dataindex[j].name);
				continue;
			}
			
			dataindex[j].stn_id=dbentry.point.stn_id;	
			dataindex[j].dev_id=dbentry.point.dev_id;
			dataindex[j].data_type=dbentry.point.data_type;
			dataindex[j].point_id=dbentry.point.pt_id;

			switch(dataindex[j].data_type)
			{
				case	ANA_TYPE:
				case	PARAM_TYPE:
				
		        /*case	DPS_PARAM:*/
						dataindex[j].data.real.quality=AnaStatusToQuality(dbentry.var.anlg.value.status);
						dataindex[j].data.real.value=(float)(dbentry.var.anlg.value.data/1000.0);
						dataindex[j].data.real.band=dbentry.var.anlg.eng_para.D_BAND;
						/*
						if(j==500)
						{
						        printf("\n************************\n");
							printf("index[%d]:: name=%s dev=%d type=%d pt=%d tableno=%d\n",j,index[j].name,
					                        index[j].dev_id,index[j].data_type,index[j].point_id,index[j].tableno);
						printf("j=%d data=%d satus=%dqul=%d value=%f band=%f\n",j,dbentry.entry.anlg.variable.value.data,dbentry.entry.anlg.variable.value.status,
						index[j].data.real.quality,index[j].data.real.value,index[j].data.real.band);
						}
						*/
						/**/
						break;
				
				case	SOE_TYPE:
				case	POL_TYPE:
				case	SYSMSG_TYPE:
							
						dataindex[j].data.sw.value=H9000SWToGB(dbentry.var.ind.status);
						break;

				default:
						break;
			}
		}
	}
	
 	/* printf("ReadOnlineDB ok\n");*/ 
}

unsigned short ReadAlmBuf()
{
	unsigned short		j,ptr,pt;
	IND_REC			*pInd_alm;
	ANLG_REC		*pLmt_alm;
	CMD_REC			*pCmd_alm;

	char			tableno;

	char			dev_id,stn_id;
	char			data_type,type_id;
	short			point_id;
	struct tm		*timeptr;
	time_t          	t;
	
	tableno=-1;

	datablock.length=0;
	datapdu.head.length=0;
	
	ptr = almptr;

	while( ptr!=shm_rec_addr->head.header_ptr )
	{ 
		t=time(NULL);
		if(t==((time_t)-1)) 		return(ptr);
		timeptr=localtime(&t);
		
		if (old_day !=(*timeptr).tm_mday)
		{
			old_day=(*timeptr).tm_mday;
		/*	ptr	=0;*/
			continue;
		}
		
		/*printf("@@@@@@@@@@@@ptr=%d ,alm_ptr=%d \n",ptr,shm_rec_addr->head.header_ptr);*/
		
		if( (datablock.length+ONE_TIME_REAL) > MAX_BLOCK_SIZE )	
				break; 
		pInd_alm = &shm_rec_addr->rec[ptr].ind_rec;
		pLmt_alm = &shm_rec_addr->rec[ptr].ana_rec;
		pCmd_alm = &shm_rec_addr->rec[ptr].cmd_rec;

		type_id = pInd_alm->type_id;	
				
		switch(type_id)
		{
		case	DPS_IND_ALM:

				stn_id		= pInd_alm->stn_id;
				dev_id		= pInd_alm->dev_id;
				data_type	= pInd_alm->data_type;
				point_id	= pInd_alm->pt_id; 
				break;

		case	DPS_ANA_ALM:
		case	DPS_IMP_ALM:
				stn_id =  pLmt_alm->stn_id ;
				dev_id =  pLmt_alm->dev_id ;
				data_type =  pLmt_alm->data_type ;
				point_id =  pLmt_alm->pt_id ;
				break;
		case	C_SYS_MODE_SET:
				stn_id =  pCmd_alm->stn_id ;
				dev_id =  pCmd_alm->dev_id ;
				data_type =  pCmd_alm->data_type ;
				point_id =  pCmd_alm->pt_id ;
				break;
		default:
				break;
		}
	/*	if((data_type!=DPS_INT_IND)&&(data_type!=DPS_POL_IND)
			&&(data_type!=DPS_SYSMSG)&&(data_type!=DPS_ANA_ALM)
			&&(data_type!=C_MSG_SYS_MODE_SET)&&(data_type !=40)&&(data_type !=72)&&(data_type!=73)
			&&(data_type !=74)&&(data_type!=75)&&(data_type !=76)&&(data_type!=77)&&(data_type !=78)&&(data_type!=79)
			&&(data_type !=80)&&(data_type!=81)&&(data_type!=82))*/
		if((type_id != DPS_IND_ALM) && (type_id != DPS_MAN_STAT_SET) && (type_id != C_SYS_MODE_SET)
			&& (type_id != DPS_ANA_ALM))
		{	
			
			ptr++;
			if(ptr>= MAX_SHM_RECORD_NUM)
				ptr = 0;      
			continue;
		} 
		if(type_id==DPS_ANA_ALM)	type_id=DPS_ANALOG; 
		
		  
		for(j=0;j<index_num;j++)	
		{        
			if((data_type==dataindex[j].data_type)&&(dev_id==dataindex[j].dev_id)&&(point_id==dataindex[j].point_id))
					
					break;
		} 
		if(j==index_num)
		{
			printf(" %d %d %d not in index\n",dev_id,data_type,point_id);

			ptr++;
			if(ptr >= MAX_SHM_RECORD_NUM)
			ptr = 0;
			 
			continue;
		} 
		if((tableno!=-1) && (tableno!=dataindex[j].tableno))	break;

		tableno=dataindex[j].tableno;

		ptr++;
		if(ptr >= MAX_SHM_RECORD_NUM)
		ptr = 0;

		pt=j-table[tableno].baseaddr;
		
		/*printf("table=%d ,base=%d j=%d pt =%d\n",tableno,table[tableno].baseaddr,j,pt);     */
		
		if(type_id != DPS_ANALOG)
		{       //  dbentry.var.ind.status
			if(type_id != C_SYS_MODE_SET&&type_id != DPS_MAN_STAT_SET)
			{
			dataindex[j].data.sw.value=H9000SWToGB(pInd_alm->ind_status);
#ifdef dl476debug
			printf("%2d-%2d-%2d %2d:%2d:%2d::%4d  table%d pnt%d is %d\n",
				pInd_alm->year,pInd_alm->month,pInd_alm->day,
				pInd_alm->hour,pInd_alm->minute,pInd_alm->second,
				pInd_alm->ms,tableno,
				pt,dataindex[j].data.sw.value);
#endif
                        
			datablock.data[datablock.length]=pInd_alm->year;
			datablock.data[datablock.length+1]=pInd_alm->month;
			datablock.data[datablock.length+2]=pInd_alm->day;
			datablock.data[datablock.length+3]=pInd_alm->hour;
			datablock.data[datablock.length+4]=pInd_alm->minute;
			datablock.data[datablock.length+5]=pInd_alm->second;
			
			memcpy(datablock.data+datablock.length+6,&pInd_alm->ms,2);
			memcpy(datablock.data+datablock.length+8,&pt,2);
			memcpy(datablock.data+datablock.length+10,&dataindex[j].data.sw.value,1);
		
#ifdef SUN
			Swap2Byte(datablock.data+datablock.length+6);
			Swap2Byte(datablock.data+datablock.length+8);
#endif
			datablock.length=datablock.length+ONE_TIME_SW;
			}
			else
			{
				dataindex[j].data.sw.value=pCmd_alm->cmd_status.opr;
#ifdef dl476debug
/*			printf("%2d-%2d-%2d %2d:%2d:%2d::%4d  table%d pnt%d is %d\n",
				pCmd_alm->year,pCmd_alm->month,pCmd_alm->day,
				pCmd_alm->hour,pCmd_alm->minute,pCmd_alm->second,
				pCmd_alm->ms,tableno,
				pt,dataindex[j].data.sw.value);		*/	//yujji
			printf("%2d-%2d-%2d %2d:%2d:%2d  table%d pnt%d is %d\n",
				pCmd_alm->year,pCmd_alm->month,pCmd_alm->day,
				pCmd_alm->hour,pCmd_alm->minute,pCmd_alm->second,
				tableno,
				pt,dataindex[j].data.sw.value);
				
#endif
                        
			datablock.data[datablock.length]=pCmd_alm->year;
			datablock.data[datablock.length+1]=pCmd_alm->month;
			datablock.data[datablock.length+2]=pCmd_alm->day;
			datablock.data[datablock.length+3]=pCmd_alm->hour;
			datablock.data[datablock.length+4]=pCmd_alm->minute;
			datablock.data[datablock.length+5]=pCmd_alm->second;
			
			/*memcpy(datablock.data+datablock.length+6,&pCmd_alm->ms,2);*/
			memset(datablock.data+datablock.length+6,0,2);/*pCmd_alm  no ms*/
			memcpy(datablock.data+datablock.length+8,&pt,2);
			memcpy(datablock.data+datablock.length+10,&dataindex[j].data.sw.value,1);
		
#ifdef SUN
			Swap2Byte(datablock.data+datablock.length+6);
			Swap2Byte(datablock.data+datablock.length+8);
#endif
			datablock.length=datablock.length+ONE_TIME_SW;
			}
		}
		else
		{
			dataindex[j].data.real.quality=AnaStatusToQuality(pLmt_alm->status.ana_status);
			dataindex[j].data.real.value=(float)(pLmt_alm->data/1000.0);
#ifdef dl476debug
			printf("%2d-%2d-%2d %2d:%2d:%2d  table%d pnt%d is %f quality is %d\n",
				pLmt_alm->year,pLmt_alm->month,pLmt_alm->day,
				pLmt_alm->hour,pLmt_alm->minute,pLmt_alm->second,
				tableno,pt,dataindex[j].data.real.value,dataindex[j].data.real.quality);
#endif
			datablock.data[datablock.length]=pLmt_alm->year;
			datablock.data[datablock.length+1]=pLmt_alm->month;
			datablock.data[datablock.length+2]=pLmt_alm->day;
			datablock.data[datablock.length+3]=pLmt_alm->hour;
			datablock.data[datablock.length+4]=pLmt_alm->minute;
			datablock.data[datablock.length+5]=pLmt_alm->second;
			memcpy(datablock.data+datablock.length+6,&pt,2);
			memcpy(datablock.data+datablock.length+8,&dataindex[j].data.real.value,4);
			memcpy(datablock.data+datablock.length+12,&dataindex[j].data.real.quality,1);
#ifdef SUN
			Swap2Byte(datablock.data+datablock.length+6);
			Swap4Byte(datablock.data+datablock.length+8);
#endif
			datablock.length=datablock.length+ONE_TIME_REAL;
		}
		

	}/*end while*/
	
	if(datablock.length<ONE_TIME_SW)		return(ptr);

	if(dataindex[j].data_type==ANA_TYPE)
		datablock.bid=B_TIME_REAL;
	else
		datablock.bid=B_TIME_SW;

	datablock.tableno=tableno;
	datapdu.head.length=datablock.length+BLOCK_HEAD_SIZE;
#ifdef SUN
	Swap2Byte(&datablock.length);
#endif
	memcpy((char *)&datapdu.block,(char *)&datablock,datapdu.head.length);
	
	printf(" alm package len =%d \n",datapdu.head.length);
#ifdef SUN
	Swap2Byte(&datapdu.head.length); 
#endif

	return(ptr);

}

 
void	ServerCmdPackage()
{ 

	POINTER		ptr;
	
	float		fdata;

	unsigned short	end,buf_num;

	unsigned short	obj;
	unsigned char	pmode,cmdtype;
	CMD_REC	*pCmd_alm;

	//end=shm_recv_addr->head;
	end=shm_rec_addr->head.header_ptr;
		
	for(;;)
	{
#ifdef	MSWIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		cmdapdu.head.ctr=A_DATA;
						
		cmdapdu.head.priority=HIGH_PRIORITY;		
		
		/*printf("\nwaiting for sending command\n");*/
 			
		memset(cmdapdu.block,0,MAX_BLOCK_SIZE);
		cmdapdu.head.length=0; 
 	
	   	while(shm_rec_addr->head.header_ptr!=end)
		{
		   	buf_num=end;

			end++;

            if(end==MAX_SHM_RECORD_NUM)   end=0;
		
			/*if(shm_recv_addr->shm_recv_seg[buf_num].lan_head.dp_type!=COMM_RECORD)
			{
				continue;
			}*/
			pCmd_alm = &shm_rec_addr->rec[buf_num].cmd_rec;

			cmdtype = pCmd_alm->type_id;
			
			ptr.dev_id=pCmd_alm->dev_id;

			ptr.data_type=pCmd_alm->data_type;

			ptr.pt_id = pCmd_alm->pt_id;
			/*memcpy((char *)&ptr.point_id,(shm_recv_addr->shm_recv_seg[buf_num].data+6),2);*/

#ifdef SUN
			Swap2Byte(&ptr.pt_id);
#endif 		 
			obj=ptr.dev_id; 
			
			switch(cmdtype)
			{
					case	 C_APOWER: 	/*P 设定*/
		   				
						cmdapdu.head.length=7;		
						
						cmdapdu.block[0]=B_ADJ_RUN;
						
						cmdapdu.block[3]=R_APOWER;

					/*	memcpy((char *)&status,shm_recv_addr->shm_recv_seg[buf_num].data+4,2);

						memcpy((char *)&data,shm_recv_addr->shm_recv_seg[buf_num].data+6,2);

						fdata=AnaDataToFloat(data,status);*/

						fdata = (float)(pCmd_alm->data.iValue/1000.0);

						memcpy((cmdapdu.block+4),&fdata,4); 
#ifdef	SUN
						Swap4Byte(&cmdapdu.block+4);
#endif					 					
						break;
						
					case	C_RPOWER:	/* Q 设定*/
					
						cmdapdu.head.length=7;		
						
						cmdapdu.block[0]=B_ADJ_RUN;
						
						cmdapdu.block[3]=R_RPOWER;

					/*	memcpy((char *)&status,shm_recv_addr->shm_recv_seg[buf_num].data+4,2);

						memcpy((char *)&data,shm_recv_addr->shm_recv_seg[buf_num].data+6,2);

						fdata=AnaDataToFloat(data,status);*/

						fdata = (float)(pCmd_alm->data.iValue/1000.0);

						memcpy((cmdapdu.block+4),&fdata,4);
#ifdef	SUN
						Swap4Byte(&cmdapdu.block+4);
#endif
						break;
						
					case	C_BREAKER: 						/*断路器\开关操作*/
					case	C_SWITCH:
					case	C_DEVICE:
					
						for(obj=0;obj<swctrl_num;obj++)	
						{
							if((ptr.data_type==swctrl[obj].data_type)
								&&(ptr.dev_id==swctrl[obj].dev_id)
								&&(ptr.pt_id==swctrl[obj].point_id))
								break;
						}

						if(obj==swctrl_num)
						{
							printf(" %d %d %d not in index\n",ptr.dev_id,ptr.data_type,ptr.pt_id);

							cmdapdu.head.length=0;
							
							break;
						}
						
						cmdapdu.head.length=5;	
					   
						cmdapdu.block[0]=B_CTRL_RUN;
						
						if(cmdtype==C_BREAKER)	{	cmdapdu.block[3]=R_BREAKER; }

						if(cmdtype==C_SWITCH)	{	cmdapdu.block[3]=R_SWITCH; }
						
						/*memcpy((char *)&status,shm_recv_addr->shm_recv_seg[buf_num].data+4,2);

#ifdef SUN
						Swap2Byte(&status);
#endif								
 						pmode=status>>4;*/

						pmode=pCmd_alm->cmd_status.opr;
 							
						cmdapdu.block[4]=pmode;

						break;

				  	case	C_UNIT:		
				  		printf(" 开÷停机 操作\n");
						
						cmdapdu.head.length=5;		
							
						cmdapdu.block[0]=B_CTRL_RUN;

						cmdapdu.block[3]=R_UNIT; 
						
					/*	memcpy((char *)&status,shm_recv_addr->shm_recv_seg[buf_num].data+4,2);

						pmode=(char)(status & 0xf0)>>4;*/
						
						pmode=pCmd_alm->cmd_status.opr;

						cmdapdu.block[4]=pmode;

						break;
	
					default:	/*无定义*/
						break;
			}/*server 操作类型组包完毕*/
#ifdef SUN
			Swap2Byte(&cmdapdu.head.length);
#endif	
			memcpy(cmdapdu.block+1,(char *)&obj,2); /*控制对象*/
#ifdef SUN
			Swap2Byte(cmdapdu.block+1);
#endif			
			if(cmdapdu.head.length!=0)	return;
				
	   } /*end while*/
	
	} /*end for(;;)*/
		
}
 
int	ClientCmdConvert(SOCKET sock,DATA_APDU apdu)
{	
 	float			fdata;

	
	unsigned char	bid,pmode;

	DMS_COMMON 		dbentry;


	short			obj,cmd_len;

	CMD_BUF			cmd;

 	
	char			name[30];
 	char			sbo_str[60];

	char		cmdtype,cmd_stat;
 	
/*	cmd_len=apdu.head.length;
#ifdef SUN
	Swap2Byte(&cmd_len);
#endif	
	printf("recv cmd ,apdu.head.length=%d:\n",acmd_len);

	for(i=0;i<cmd_len;i++)
	{
		printf("=%d=",apdu.block[i]);

		if(i==apdu.head.length-1)	printf("\n");
	}
*/	
	bid=apdu.block[0];
												  
	memcpy((char *)&obj,apdu.block+1,2);
#ifdef SUN
	Swap2Byte(&obj);
#endif

	pmode=apdu.block[3];
 	
	printf ("bid=%d object=%d pmode=%d\n",bid,obj,pmode);
 	   								 
	cmd_len=10; 
	
	switch(bid)
	{
		case	B_ANASET:
			
			apdu.block[0]=B_ANASET_BACK;
			
			if(Do_Send(sock,apdu)==0)
			{
				Do_Disconnect(sock);
				return(0);
			}

			cmd_len=0;

			break;

		case	B_CONTROL:
			
			apdu.block[0]=B_CTRL_BACK;
			
			if(Do_Send(sock,apdu)==0)
			{
				Do_Disconnect(sock);
				return(0);
			}
			
			cmd_len=0;

			break;

		case	B_ADJUST:

			apdu.block[0]=B_ADJ_BACK;

			if(Do_Send(sock,apdu)==0)
			{
				Do_Disconnect(sock);
				return(0);
			}
			
			cmd_len=0;

			break;

		case	B_CURSE:			/*曲线*/
				
	/*		for(i=0;i<MAX_PLAN_POINT;i++)
			{
				//cmd.dev_id=SYS_RTU;			//apdu.block.data[1];
				GetStnSysLcuIdById( STN_ID, &SysLCUid );
				cmd.data_type=DPS_MAN_VALUE_SET;
				cmd.point_id=PLAN_POINT_BASE+i*2;

				memcpy(&fdata,(apdu.block+3+2*i),4);
#ifdef SUN
				Swap4Byte(&fdata);
#endif
				ana=FloatToAnaData(fdata);
				cmd.data=ana.data;
				cmd.status=ana.status | UP_UNIT;
#ifdef SUN
				Swap2Byte(&cmd.data);
				Swap2Byte(&cmd.status);
#endif
				memcpy((cmdbuf+i*10),(char *)&cmd,10);
			}
				
			cmd_len=MAX_PLAN_POINT*10; */

			break;

		case	B_ANASET_RUN:			/*模拟量设置*/
				
				memset(&cmd,0,sizeof (CMD_BUF));

				if(ReadEntryByNameStr(swctrl[obj].name,&dbentry)==-1)
				{
				//		printf (" dl476_app_fun:%s not in Database!\n",swctrl[obj].name);
							
						//cmd_len=0;
							
						break;
				}
				cmdtype = DPS_MAN_VALUE_SET;
				memcpy(&fdata,(apdu.block+3),4);
#ifdef SUN
				Swap4Byte(&fdata);
#endif
				SEComdSendToLCU(cmdtype, fdata, &dbentry);	

				break;
				
		case	B_CTRL_RUN:
		case	B_ADJ_RUN:
  
			switch(pmode)
			{
				case	R_APOWER: 
				case	R_RPOWER:			/*P Q设定*/
										
				/*	memset(&cmd,0,sizeof (CMD_BUF));

					strcpy (sbo_str, DEVNAME[obj]);*/
					GetStnNameById(STN_ID,sbo_str);  /*STN_ID 为厂站ID ，.h直接定义*/
					/*strcpy(sbo_str,GetDevNameStr(obj));*/
					GetGroupNameById( STN_ID, obj, name );
					strcpy(sbo_str,name);
					if ( pmode == R_APOWER )
					strcat(sbo_str, ".PAM.YG_GD");
					if ( pmode == R_RPOWER )
					strcat(sbo_str, ".PAM.WG_GD");

					if(ReadEntryByNameStr(sbo_str,&dbentry)==-1)
					{
				//		printf (" dl476_app_fun:%s not in Database!\n",sbo_str);
							
						//cmd_len=0;
							
						break;
					}
					

					if (pmode==R_APOWER)	cmdtype=C_APOWER;
					else			cmdtype=C_RPOWER;
						
					memcpy(&fdata,(apdu.block+4),4);
#ifdef SUN
					Swap4Byte(&fdata);
#endif
					
					strcat(sbo_str, ".Power");
					printf("sbo_str=%s\n",sbo_str);

					if(InterLock_Judge( sbo_str )!=1)
					{
						printf("Ctrl condition %s not fulfiled!\n",sbo_str);
						BroadCastSms("闭锁条件不满足",true);
						break;
					}		//yujji
					SEComdSendToLCU(cmdtype, fdata, &dbentry);
					break;

				case	R_SYS_MODE_SET:		
				case	R_BREAKER: 
				case	R_SWITCH:		/*断路器\开关操作*/
				case	R_DEVICE:
				case	R_GATE:		

					if(ReadEntryByNameStr(swctrl[obj].name,&dbentry)==-1)
					{
				//		printf (" dl476_app_fun:%s not in Database!\n",swctrl[obj].name);			
						break;
					}
					
					cmd_stat = apdu.block[4];
					strcpy (sbo_str, swctrl[obj].name);
 
					if(pmode !=R_SYS_MODE_SET)
					{
						if(pmode == R_BREAKER)
						{
							cmdtype=C_BREAKER;
							strcat(sbo_str, ".C_BREAKER");
							switch(apdu.block[4])
							{
						  
								case 0:		printf("试验分闸\n");
											strcat(sbo_str, ".Open");
											break; 
								case 1:		printf("分闸\n");
											strcat(sbo_str, ".Open");
											break;
								case 2:		printf("合闸\n");
											strcat(sbo_str, ".Close");
											break;
								case 3:		
											strcat(sbo_str, ".IM_Line");
											break;
								case 4:		 
											strcat(sbo_str, ".IIM_Line");
											break;
								case 5:		 
											strcat(sbo_str, ".Line_IM");
											break;
								case 6:		 
											strcat(sbo_str, ".Line_IIM");
											break;
								case 7:		
											strcat(sbo_str, ".Tran_IM");
											break;
								case 8:		 
											strcat(sbo_str, ".Tran_IIM");
											break;
								case 9:		 
											strcat(sbo_str, ".IM_Tran");
											break;
								case 10:		 
											strcat(sbo_str, ".IIM_Tran");
											break; 		
								case 11:	printf("试验合\n");
											strcat(sbo_str, ".TestClose");
											break;
								case 12:	printf("同期退出\n");
											strcat(sbo_str, ".CancelSyn");
											break;
								case 13:	printf("环网合\n");
											strcat(sbo_str, ".LoopClose");
											break;	
								case 14:	printf("充电合\n");
											strcat(sbo_str, ".ChgVltCls");
											break;
								case 15:	printf("无压合\n");
											strcat(sbo_str, ".ZroVltCls");
											break;
							}
						}
						if(pmode == R_SWITCH)
						{
							cmdtype=C_SWITCH;
							strcat(sbo_str, ".C_SWITCH");
							switch(apdu.block[4])
							{
								case 1:		printf("分闸\n");
											strcat(sbo_str, ".Open");
											break;
								case 2:		printf("合闸\n");
											strcat(sbo_str, ".Close");
											break;
							}
						}
						if(pmode == R_DEVICE)
						{
							cmdtype=C_DEVICE;
							strcat(sbo_str, ".C_DEVICE");
							switch(apdu.block[4])
							{
								case 1:		printf("退出\n");
											strcat(sbo_str, ".Off");
											break;
								case 2:		printf("投入\n");
											strcat(sbo_str, ".On");
											break;
							}
						}
						if(pmode == R_GATE)
						{
							cmdtype=C_GATE;
							strcat(sbo_str, ".C_GATE");
							switch(apdu.block[4])
							{
								case 1:		printf("开闸门\n");
											strcat(sbo_str, ".Open");
											break;
								case 2:		printf("关闸门\n");
											strcat(sbo_str, ".Close");
											break;
								case 3:		printf("停止操作\n");
											strcat(sbo_str, ".Stop");
											break;
							}
						}

						if(InterLock_Judge( sbo_str )!=1)
						{
							printf("Ctrl condition %s not fulfiled!\n",sbo_str);
							BroadCastSms("闭锁条件不满足",true);
							break;
						}		//yujji
					}

					
					else	cmdtype=C_SYS_MODE_SET;
					if (cmdtype==C_SYS_MODE_SET)	//yujji2010
						cmd_stat = cmd_stat-1;
						
					ComdSendToLCU(cmdtype,cmd_stat,&dbentry);

					break;
						
				  case	R_UNIT:		
				  	
				  	/* 1.开2.停机 3.发电转调相 4。调相转发电
					  5.并网 8.同期退出9 解列*/
					cmdtype=C_UNIT;		//yujji
					
					GetStnNameById(STN_ID,sbo_str);  /*STN_ID 为厂站ID ，.h直接定义*/
					/*strcpy(sbo_str,GetDevNameStr(obj));*/
					GetGroupNameById( STN_ID, obj, name );
					strcpy(sbo_str,name);

					strcat(sbo_str, ".POL.JZ_STAT");
					sprintf(name,"WJD.%s",sbo_str);
					strcpy(sbo_str,name);
					

					if(ReadEntryByNameStr(sbo_str,&dbentry)==-1)
					{
				//		printf (" dl476_app_fun:%s not in Database!\n",sbo_str);
												
						break;
					}

					strcat(sbo_str, ".C_UNIT");
					switch(apdu.block[4])
					{
								case 1:		printf("发电\n");
											cmd_stat=1;
											strcat(sbo_str, ".Start");
											break;
								case 2:		printf("停机\n");
											cmd_stat=2;
											strcat(sbo_str, ".Stop");
											break;
								case 3:		printf("机组调相令\n");
											cmd_stat=9;  /*4.0 cmd_stat=9 机组调相令*/
											strcat(sbo_str, ".CmpStart");
											break;
								case 4:		printf("调相转发电\n");
											/*4.0 没有相应的调相转发电格式，如果有需要，和下位机商量好用规约中备用命令*/
											break;
								case 5:		printf("开机至并网令\n");/*将3.0规约中并网令理解为4.0中开机至并网令*/
											cmd_stat=23;  /*4.0 cmd_stat=23 开机至并网令*/
											strcat(sbo_str, ".StartToOnLin");
											break;
								case 8:		printf("同期退出\n");
											/*4.0 没有相应的同期退出格式，如果有需要，和下位机商量好用规约中备用命令*/
											break;
								case 9:		printf("解列\n");
											cmd_stat=11;
											strcat(sbo_str, ".Open");
											break;
					}

						if(InterLock_Judge( sbo_str )!=1)
						{
							printf("Ctrl condition %s not fulfiled!\n",sbo_str);
							BroadCastSms("闭锁条件不满足",true);
							break;
						}		//yujji
					ComdSendToLCU(cmdtype,cmd_stat,&dbentry);
					break;
					
				case	R_UNIT_TST:		
				  	
				  	cmdtype=C_UNIT;		//yujji			
					GetStnNameById(STN_ID,sbo_str);  /*STN_ID 为厂站ID ，.h直接定义*/
					/*strcpy(sbo_str,GetDevNameStr(obj));*/
					GetGroupNameById( STN_ID, obj, name );
					strcpy(sbo_str,name);

					strcat(sbo_str, ".POL.JZ_STAT");
					sprintf(name,"WJD.%s",sbo_str);
					strcpy(sbo_str,name);
	
					if(ReadEntryByNameStr(sbo_str,&dbentry)==-1)
					{
				//		printf (" dl476_app_fun:%s not in Database!\n",sbo_str);
												
						break;
					}

					strcat(sbo_str, ".C_UNIT");
					switch(apdu.block[4])
					{
								case 1:		printf("开机至空转令\n");
											cmd_stat=21;
											strcat(sbo_str, ".StartToSpin");
											break;
								case 2:		printf("开机至空载令\n");
											cmd_stat=22;
											strcat(sbo_str, ".StartToNLoad");
											break;
								case 3:		printf("开机假并网命令\n");
											cmd_stat=13;
											strcat(sbo_str,".TransNoLoad");
											break;
								case 5:		printf("零起升压\n");
											cmd_stat=14;
											strcat(sbo_str,".ZeroVRun");
											break;
								case 6:		printf("主变空载转机组备用（带主变零起升压）\n");
											cmd_stat=15;
											strcat(sbo_str,".TranZVRun");
											break;
								case 7:		printf("主变零起升压转备用\n");
											cmd_stat=16;
											strcat(sbo_str,".ZeroVStop");
											break;
							/*	case 8:		printf("开机准备\n");
											cmd_stat=12;
											strcat(sbo_str,".ReadyStart");
											break;
								case 9:		printf("空转\n");
											cmd_stat=6;
											strcat(sbo_str,".NoVoltRun");
											break;
							*/
								case 8:		printf("空载到空转\n");
											cmd_stat=6;
											strcat(sbo_str,".NoVoltRun");
											break;
								case 9:		printf("空转到空载\n");
											cmd_stat=7;
											strcat(sbo_str,".NoLoadRun");
											break;
							
								case 10:	printf("空转建压令\n");/*4.0 没有相应的空转建压令格式，用空载令。根据现场需要作修改*/
											cmd_stat=7;
											strcat(sbo_str,".NoLoadRun");
											break;
								case 11:	printf("撤压至空转令\n");/*4.0 没有相应的撤压至空转令格式，用停机至空转令。根据现场需要作修改*/
											cmd_stat=25;
											strcat(sbo_str,".StopToSpin");
											break;		
								default:
											break;
					}

						if(InterLock_Judge( sbo_str )!=1)
						{
							printf("Ctrl condition %s not fulfiled!\n",sbo_str);
							BroadCastSms("闭锁条件不满足",true);
							break;
						}		//yujji
					ComdSendToLCU(cmdtype,cmd_stat,&dbentry);
					break;


				default:

					break;
						
			}/*pmode 操作类型解析完毕*/
	
			break;
	default:

			break;
	}/* end switch(bid)*/
	
	return(1);
	
}/* end CmdConvert()*/

 
void DataConvert(DATA_APDU apdu)
{
	return;
}

SOCKET  	TcpSend(SOCKET sock,DATA_APDU apdu)
{ 
	char			name[60];
	DMS_COMMON		dbentry;

	if(sock == INVALID_SOCKET)	
	{
		printf("@@@@@@@@in Tcpsend: sock == invalid_socket\n");
		Dsend_errnum=Dsend_errnum+1;	
		sock=Do_Connect();
	}
	if(sock != INVALID_SOCKET)
	{       
		Dsend_errnum=0; 	  
		if(Do_Send(sock,apdu)==0)	
		{        
			Do_Disconnect(sock); //by wayne for reuse socket
			sock= INVALID_SOCKET;
		}     
	}
        
	sprintf(name,"%sRC_LINK_STA",DL476_SYS_TAB_NAME);

	/*ReadEntryByName(name,&type,&dbentry);
	if ( dbentry.err_code == -1 )*/
	if(ReadEntryByNameStr(name,&dbentry)==-1)
	{
		printf("%s not in Database!\n",name);
		
	}	
	
	if(sock == INVALID_SOCKET)
	{				
		connect_errnum=connect_errnum+1;
	        printf("\n ****connect_errnum==%d****",connect_errnum);
	    if(connect_errnum>=20)
	    {	        
		/*if(dbentry.entry.ind.variable.value.status !=4)
		{
			printf(" 远方通讯故障\n");	
			Wdb(name,4);
			BdcastMsg(dbentry.entry.ind.variable.ptr.dev_id,dbentry.entry.ind.variable.ptr.data_type,
					dbentry.entry.ind.variable.ptr.point_id,4); 
		}*/
			UpdateChannelFlag(1);
	    }
	}
	else
	{
	        connect_errnum=0;			
	/*	if(dbentry.entry.ind.variable.value.status !=0)
		{printf(" 远方通讯正常\n");	
			Wdb(name,0);
			BdcastMsg(dbentry.entry.ind.variable.ptr.dev_id,dbentry.entry.ind.variable.ptr.data_type,
					dbentry.entry.ind.variable.ptr.point_id,0); 
		}*/
			UpdateChannelFlag(0);
	}
	
	return(sock);
}


/*void    BdSBOStat()
{
	int			type;
	DMS_COMMON		dbentry;
	
	char			name[60];
	
	sprintf(name,"%sRC_ERR",DL476_SYS_TAB_NAME);
	
	ReadEntryByName(name,&type,&dbentry);
	if ( dbentry.err_code == -1 )
	{
		printf("%s not in Database!\n",name);
		
	}	

	BdcastMsg(dbentry.entry.ind.variable.ptr.dev_id,dbentry.entry.ind.variable.ptr.data_type,
					dbentry.entry.ind.variable.ptr.point_id,4); 
	 
	return; 
	
}*/

void ComdSendToLCU(char type_id,char cmd_stat,DMS_COMMON *pdbcomm)
{
	struct	tm	tmpt;
	time_t		tim;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;

	cmd_buf.type_id = type_id;
	cmd_buf.stn_id = pdbcomm->point.stn_id;
	cmd_buf.dev_id = pdbcomm->point.dev_id;
	cmd_buf.data_type = pdbcomm->point.data_type;
	cmd_buf.point_id = pdbcomm->point.pt_id;

	cmd_buf.status.opr=cmd_stat;
	cmd_buf.status.state=1;
	cmd_buf.status.result=0;
	cmd_buf.status.src=cmd_src;
	cmd_buf.data.iValue=0;
	cmd_buf.ctrl_addr=pdbcomm->fixed.ctrl_addr;
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"dl476");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif
	if((type_id == C_SYS_MODE_SET)||(type_id == DPS_MAN_STAT_SET))
	{
		lanhead.dest_id = 0;
		lanhead.dest_stn = 0;
		lanhead.dp_type=COMM_RECORD_TIME;

		printf("Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		printf("Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
	}
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

void SEComdSendToLCU(char type_id, float value, DMS_COMMON *pdbcomm)
{
	struct	tm	tmpt;
	time_t		tim;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;

	cmd_buf.type_id = type_id;
	cmd_buf.stn_id = pdbcomm->point.stn_id;
	cmd_buf.dev_id = pdbcomm->point.dev_id;
	cmd_buf.data_type = pdbcomm->point.data_type;
	cmd_buf.point_id = pdbcomm->point.pt_id;

	cmd_buf.status.state=1;
	cmd_buf.status.result=0;
	cmd_buf.status.opr=0;
	cmd_buf.status.src=cmd_src;
	cmd_buf.data.iValue=(int)(value*1000);
	cmd_buf.ctrl_addr=pdbcomm->fixed.ctrl_addr;
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"dl476");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif
	if((type_id == C_OP_VALUE_SET)||(type_id == DPS_MAN_VALUE_SET))
	{
		lanhead.dest_id = 0;
		lanhead.dest_stn = 0;
		lanhead.dp_type=COMM_RECORD_TIME;

		printf("Set Point Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		printf("Set Point Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
	}
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

int UpdateChannelFlag(unsigned short status)
{
	char	name_buf[36];
	char	host_name[36];

	POINTER		point;
	DMS_COMMON	dbentry;
	POINTER		pnt;
	SYS_MSG		lanbcdata;
	LAN_HEAD	lanhd;

	WR_VAR_UNION	new_value;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	time_t		tim;
	struct tm	tmpt;
	DATE_TIME	update_time;

/*	strcpy(name_buf,SYS_TAB_NAME);
*/
	gethostname(host_name, 10);
	if(0!=GetHostStnId(host_name, &point.stn_id))
	{
		printf("GetHostStnId() %s No this hostname point in database file!\n",host_name);
		return -1;
	}

	if(0!=GetHostSysLcuId(host_name, &point.dev_id))
	{
		printf("GetHostSysLcuId() %s No this hostname point in database file!\n",host_name);
		return -1;
	}

	point.data_type= SYSMSG_TYPE;

	strcpy(name_buf,host_name);
	strcat(name_buf,"_");
	strcat(name_buf,"RC_LINK_STA");
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		printf("%s No this com channel status point in stn:%d dev:%d the database file!\n",
			name_buf,point.stn_id,point.dev_id);

		return -1;
	}

	if(ReadEntryById(&point,&dbentry)==-1)
	{
		printf("%s No this com channel status point in stn:%d dev:%d the database file!\n",
			name_buf,point.stn_id,point.dev_id);
		return(false);
	}
	else if(dbentry.var.ind.status.rt_state != status)
	{
		lanbcdata.type_id =DPS_IND_ALM;
		lanbcdata.stn_id =dbentry.point.stn_id;
		lanbcdata.dev_id =dbentry.point.dev_id;
		lanbcdata.data_type=dbentry.point.data_type;
		lanbcdata.number = 1;
		lanbcdata.point_id = dbentry.point.pt_id;

		lanbcdata.status=status+0x0100;	/*communication channel status: 0:normal; 1:Fail */
		lanbcdata.ms=0;
		GetLocalHostId(&lanbcdata.host_id);

		tim=time (NULL);
		tmpt=*localtime (&tim);
		lanbcdata.sec=tmpt.tm_sec;
		lanbcdata.min=tmpt.tm_min;
		lanbcdata.hour=tmpt.tm_hour;
		lanbcdata.day=tmpt.tm_mday;
		lanbcdata.month=tmpt.tm_mon+1;
		lanbcdata.year=tmpt.tm_year+1900;

		lanhd.length=sizeof(SYS_MSG);
#ifdef SUN
		Swap2Byte(&lanbcdata.number);
		Swap2Byte(&lanbcdata.point_id);
		Swap2Byte(&lanbcdata.status);
		Swap2Byte(&lanbcdata.ms);
		Swap2Byte(&lanbcdata.year);
		Swap2Byte(&lanhd.length);
#endif
		lanhd.dp_type=BJ_DB_TIME;

		lanhd.dest_id =0;
		lanhd.src_id =lanbcdata.host_id;
		lanhd.dest_stn =lanbcdata.stn_id;
		lanhd.packet_num =0;
		lan_out ((char *)&lanbcdata,lanhd,(unsigned char)0);

		pnt = dbentry.point;
		new_value.ind.rt_state = status;
		new_value.ind.fault_tag = status;
		update_time.ms		= 0;
		update_time.second	= tmpt.tm_sec;
		update_time.minute	= tmpt.tm_min;
		update_time.hour	= tmpt.tm_hour;
		update_time.day		= tmpt.tm_mday;
		update_time.month	= tmpt.tm_mon+1;
		update_time.year	= tmpt.tm_year+1900;
		rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;

		if(SetPtValueIostById(&pnt, new_value, iost, update_time, rw_flag) == -1)
			printf ( "\ndl476-92: %d-%d-%d-%d No this point in the database when Wdb!\n",
				pnt.stn_id, pnt.dev_id, pnt.data_type, pnt.pt_id);

	}
	return(true);
}
