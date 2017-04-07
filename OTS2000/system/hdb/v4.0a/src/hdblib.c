/************************************************************************************************************************
FILENAME:	hdblib.c

FUNCTION:	
	
	some functions of hdb library,include:
	(1) int SelectWmsRecFromHdb( char *SelectCmdStr,WMS_RECSTRU *rec)
	(2) int SelectYcRecFromHdb( unsigned char curid,struct  hdb_curve *p_rs)
	(3) int InsertHdbByYcEvent( char *DateTime, char *LogicalName, int id, float *fvalue )
	(4) int UpdateHdbByYc( char *LogicalName, char *DateTime, float *fvalue, int istatus  )
	(5) int InsertHdbByYxEvent( char *DateTime, char *LogicalName, int id, int ivalue  )
	(6) int UpdateHdbByYx( char *LogicalName, char *DateTime, int ivalue, int istatus )
	(7) int ConnectHDB( char *LoginID )
	(8) int DisconnectHDB()
	(12)int ExecuteSQL( char *cmdstr )

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

*************************************************************************************************************************/

#include "../inc/hdbhead_all.h"

#define MAX_APPEND_SIZE 4000
#define DEBUGSTR "%s(line:%d): "
#define DEBUGARGS __FILE__,__LINE__
char	LoginID[50];
char	SIDSTR[100];
int	Interval;
char	strPwd[100];
char	strUsername[]="telcom";
char	strPassword[]="telcom";
char	Yc_Update_Mode=0;
int	HdbConn=FALSE;
struct lcu_pt_conf ptconf[MAX_STN_NUM+1][MAX_LCU_NUM+1];
char	PkgStr[MAXPKGBUF];
struct hdb_id hdb_pt_id[MAX_DEV_NUM_HDB][MAX_PT_NUM_DEV];
LCUID dev_id[MAX_DEV_NUM_HDB];
int dev_pos[MAX_STN_NUM+1][MAX_LCU_NUM+1];
int pt_pos[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_DATATYPE_NUM+1];
int hdb_dev_num;
int StatNum;
int	hdb_id_num;
int	hdb_all_num;
char log_str[500];
//struct array_period_data a_data[MAX_YC_NUM];

//char log_str[256];

int SelectIdFromHdb(Cda_Def cda)
{
	char	sqlstr[300]="select id,hdb_band, trim(logicalname),station_id,dev_id,type_id,point_id,NVL(hdb_condition,0) from ems.speciality "; 
	char	cmd[300];
	int	i=0,j,n;
	int nDev,nPt;
	unsigned int	tmpid,station_id,dev_id,type_id,point_id,hdb_condition;
	sb2 indp = -1;
	float	offset;
	time_t t1,t2;
	char	LogicalName[LOGICALNAME_LEN+1];
#ifndef MULTIDEV
	strcat(sqlstr," where dev_id=1 ");
#endif
	strcat(sqlstr," order by id ");
	for(i=1;i<MAX_STN_NUM;i++)
	{
		for(j=1;j<=MAX_LCU_NUM;j++)
		{
			for(n=0;n<MAX_PT_NUM_DEV;n++)
			{
				hdb_pt_id[ dev_pos[i][j] ][ n ].iscond = TRUE;	
				hdb_pt_id[ dev_pos[i][j] ][ n ].id = -1;
				hdb_pt_id[ dev_pos[i][j] ][ n ].isvalid = FALSE;
			}	
		}
	}
	strcpy(cmd,sqlstr);		

//	log_write(INF,3,cmd);
	log_write(INF,1,"Getting inner ID from hdb, please wait...");	

	if( oparse(&cda, (unsigned char *)cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) &tmpid, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) 	&indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 2, (ub1 *) &offset, (sword) sizeof(float),SQLT_FLT,
               -1, (sb2 *) &indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
// 	     	err_write("oci error: odefin(hdb_band)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
	}
	
	if (odefin(&cda, 3, (ub1 *) LogicalName, LOGICALNAME_LEN+1,SQLT_STR,
               -1, (sb2 *) &indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (odefin(&cda, 4, (ub1 *) &station_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) 	&indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}
 
	if (odefin(&cda, 5, (ub1 *) &dev_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) 	&indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}
  	
	if (odefin(&cda, 6, (ub1 *) &type_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) 	&indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}  	

	if (odefin(&cda, 7, (ub1 *) &point_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) 	&indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}  	  		

	if (odefin(&cda, 8, (ub1 *) &hdb_condition, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) 	&indp, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}  	
	if (oexec(&cda))
	{
      		OracleError(&cda);
      		return(FALSE);
	}
		
	i=0;
	time(&t1);	
	while( ofetch(&cda)==0 )
 	{     
 		if( i>=MAX_PT_NUM )
 		{	
 			cda.rc = NO_DATA_FOUND;
 			break; 
 		}
 		else
 		{
			if(dev_id<=MAX_LCU_NUM && type_id<=MAX_TYPE_NUM && point_id<MAX_PT_NUM_DEV)
			{
				nDev = dev_pos[station_id][dev_id];
				nPt = pt_pos[station_id][dev_id][type_id] + point_id;
				hdb_pt_id[nDev][nPt].point.stn_id = (unsigned char)station_id;
				hdb_pt_id[nDev][nPt].point.dev_id = (unsigned char)dev_id;
				if(type_id ==11) type_id =14;
				hdb_pt_id[nDev][nPt].point.data_type = (unsigned char)type_id;
				hdb_pt_id[nDev][nPt].point.pt_id = (unsigned short)point_id;
				strcpy(hdb_pt_id[nDev][nPt].l_name,LogicalName);
				hdb_pt_id[nDev][nPt].id = tmpid;
				hdb_pt_id[nDev][nPt].offset = offset;
				hdb_pt_id[nDev][nPt].iscond = hdb_condition;
				hdb_pt_id[nDev][nPt].isvalid=TRUE;
			}
			
 		}
		offset = 0;
		hdb_condition = 0;
		tmpid = 0;
 		i++;
	}
	hdb_all_num = i;

	if (cda.rc != NO_DATA_FOUND)
	{
		return FALSE;
	}
	else
	{	

	}

	time(&t2);
	sprintf(log_str,"Getting inner ID from hdb successfully! cost %d sec,num=%d",t2-t1,hdb_all_num);
	log_write(INF,1,log_str);
	return TRUE;
}

int SelectDevCondFromHdb(Cda_Def cda )
{
	char	sqlstr[]="select ems.dev.station_id,ems.dev.dev_id,ems.dev.dev_condition_id , trim(ems.speciality.logicalname) from ems.dev,ems.speciality where ems.dev.dev_condition_id = ems.speciality.id  order by ems.dev.station_id,ems.dev.dev_id"; 
	char	cmd[300];
	int	i=0,j,total;
	int station_id,dev_id;
	int	tmpid;
	time_t t1,t2;
	char	LogicalName[LOGICALNAME_LEN+1];
	for(i=1;i<MAX_STN_NUM+1;i++)
	{
		for(j=1;j<MAX_LCU_NUM+1;j++)
		{
			cond[i][j].id = -1;			
		}
	}
	strcpy(cmd,sqlstr);		

	/* log_write(INF,3,cmd); */
//	log_write(INF,3,"Getting Dev Condition ID from hdb, please wait...");	

	if( oparse(&cda, (unsigned char *)cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) &station_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}


	if (odefin(&cda, 2, (ub1 *) &dev_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}


	if (odefin(&cda, 3, (ub1 *) &tmpid, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 4, (ub1 *) LogicalName, LOGICALNAME_LEN+1,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (oexec(&cda))
	{
      	OracleError(&cda);
      	return(FALSE);
	}
		
	i=0;
	time(&t1);	
	while( ofetch(&cda)==0 )
 	{     
		if((station_id<=MAX_STN_NUM) && (dev_id<=MAX_LCU_NUM))
		{
			cond[station_id][dev_id].id = tmpid;
			strcpy(cond[station_id][dev_id].logicalname,LogicalName);
		}
	}
	total = i;
	if (cda.rc != NO_DATA_FOUND)
	{
		return FALSE;
	}
	else
	{	

	}
	time(&t2);
//	sprintf(log_str,"Getting Dev Condition ID from hdb successfully! cost %d sec",t2-t1);
//	log_write(INF,3,log_str);
	return TRUE;	
}

int SelectStatPtFromHdb(Cda_Def cda )
{
	char	sqlstr[]="select trim(STAT_NAME),ID,STATUS_ID, STAT_YES,STAT_TOTAL, NVL(STAT_TRUE,0), NVL(STAT_DATETIME,'2000-1-1') from ems.statistic order by ID "; 	
	int	i=0,j;
	int id,oldid = -1;
	int status_id;
	int isstat,istrue;
	float total_time;
	char TimeStr[22];
	char	name[20];
	for(i=0;i<MAX_STAT_NUM;i++)
	{
		yxstat[i].id = -1;
		yxstat[i].ischg  = 0;
		for(j=0;j<16;j++)
		{
			yxstat[i].isstat[j] = 0; 
		}
	}
	/* log_write(INF,3,sqlstr); */

	if( oparse(&cda, (unsigned char *)sqlstr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) name, 20,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (odefin(&cda, 2, (ub1 *) &id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 3, (ub1 *) &status_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}


	if (odefin(&cda, 4, (ub1 *) &isstat, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 5, (ub1 *) &total_time, (sword) sizeof(float), SQLT_FLT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}


	if (odefin(&cda, 6, (ub1 *) &istrue, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}


	if (odefin(&cda, 7, (ub1 *) TimeStr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (oexec(&cda))
	{
      	OracleError(&cda);
      	return(FALSE);
	}
		
	i=-1;
	while( ofetch(&cda)==0 )
 	{     
		if(status_id>=16) 
		{
			MySleep(10);
			continue;
		}
		if( oldid != id)
		{
			oldid = id;
			i++;
		}
		yxstat[i].id = id;
		yxstat[i].isstat[status_id] = isstat;
		yxstat[i].istrue[status_id] = istrue;
		strcpy(yxstat[i].name[status_id],name);
		if(istrue == 1)
		{
			strcpy(yxstat[i].lasttime,TimeStr);
		}
		yxstat[i].totaltime[status_id] = total_time;
	}
	StatNum = i+1;
	if (cda.rc != NO_DATA_FOUND)
	{
      	OracleError(&cda);
		return FALSE;
	}
	else
	{	
		
	}
	return TRUE;	
}

int SelectStatTrueFromHdb(Cda_Def cda, struct stat_seg *p_stat)
{

	char sqlstr[]="select STATUS_ID, STAT_TRUE,STAT_TOTAL, NVL(STAT_DATETIME,'2000-1-1') from ems.statistic where  ID=%d"; 
	char cmd[300];
	int	i=0;
	int oldid = -1;
	int status_id;
	int istrue;
	float total_time;
	char TimeStr[22];
	
	sprintf(cmd,sqlstr,p_stat->id);	
//	log_write(INF,3,cmd); 

	if( oparse(&cda, (unsigned char *)cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}


	if (odefin(&cda, 1, (ub1 *) &status_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}


	if (odefin(&cda, 2, (ub1 *) &istrue, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 3, (ub1 *) &total_time, (sword) sizeof(float),SQLT_FLT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(hdb_band)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 4, (ub1 *) TimeStr, 22, SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (oexec(&cda))
	{
      	OracleError(&cda);
      	return(FALSE);
	}
		
	while( ofetch(&cda)==0 )
 	{     
		if(status_id>=16) continue;
		p_stat->istrue[status_id] = istrue;
		//strcpy(p_stat->lasttime[status_id],TimeStr);
		p_stat->totaltime[status_id] = total_time;
	}
	if (cda.rc != NO_DATA_FOUND)
	{
		return FALSE;
	}
	else
	{	
		
	}
	return TRUE;	
}


int SelectOneIdFromHdb(Cda_Def cda, struct hdb_id *p_id )
{
	char	sqlstr[]="select id, trim(logicalname) from ems.speciality where logicalname='%s'"; 
	char	cmd[300];
	int	tmpid;
	char	LogicalName[LOGICALNAME_LEN+1];

	p_id->isvalid = FALSE; 
	p_id->id = -1;
	sprintf(cmd,sqlstr,p_id->l_name);		
	
//	log_write(INF,3,cmd);

	if( oparse(&cda, (unsigned char *)cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) &tmpid, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}
	
	if (odefin(&cda, 2, (ub1 *) LogicalName, LOGICALNAME_LEN+1,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}
	if (oexec(&cda))
	{
      		OracleError(&cda);
      		return(FALSE);
	}
		
	while( ofetch(&cda)==0 )
 	{     
	
		p_id->id = tmpid;
		p_id->isvalid = TRUE;
	}
	return TRUE;
}

int SelectLogicalNameFromHdb(Cda_Def cda, struct hdb_id *p_id )
{
	char	sqlstr[]="select logicalname from ems.speciality where ID='%d'"; 
	char	cmd[300];
	char	LogicalName[LOGICALNAME_LEN+1];

	p_id->isvalid = FALSE; 
	sprintf(cmd,sqlstr,p_id->id);		
	
//	log_write(INF,3,cmd);

	if( oparse(&cda, (unsigned char *)cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}
	if (odefin(&cda, 1, (ub1 *) LogicalName, LOGICALNAME_LEN+1,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}
	if (oexec(&cda))
	{
      		OracleError(&cda);
      		return(FALSE);
	}
		
	while( ofetch(&cda)==0 )
 	{     
	
		strcpy(p_id->l_name,LogicalName);
		p_id->isvalid = TRUE;
	}
	return TRUE;
}

int InsertOneYcSceneFromHDB(Cda_Def cda,char *logicalname, char *time_str, int scene_id)
{
	char	curve[1500];
	char	cmd[1500];
	char	TimeStr[30];
	char	tmpstr[10];
	int		sec;
	float	data0,datai;
	float	fvalue;
	short	status=0;
	int		i,id,n;	
	struct tm datetime;	
	StrToTime(time_str, &datetime,ONE_MIN,0);
	sec = datetime.tm_sec; 
	StrToTime(time_str, &datetime,ONE_MIN,1);
	tmtostring(datetime,TimeStr);
	
	strcpy(curve,"select ");
	strcat(curve,"NVL(data0,");
	strcat(curve,CONST_NEG_STR);
	strcat(curve,"),");
	strcat(curve,"NVL(data");
	sprintf(tmpstr,"%d",sec);
	strcat(curve,tmpstr);
	strcat(curve,",");
	strcat(curve,CONST_NEG_STR);
	strcat(curve,"),");

	strcat(curve," sec.id as id ");
	i= strlen(curve);
	strcat(curve,"  from ems.speciality spe, ems.second sec where spe.id=sec.id");
	strcat(curve,"  and spe.logicalname='%s' and sec.datetime= '%s' ");

	sprintf(cmd, curve, logicalname, TimeStr);
//	log_write(INF,3,cmd);
	if( oparse(&cda,(unsigned char *) cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);	
	}


	if (odefin(&cda, 1, (ub1 *) &data0, (sword) sizeof(float),SQLT_FLT,
			   -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
//		err_write("oci error: odefin(data)\n");
		OracleError(&cda);
		return(FALSE);   
  	}
	if (odefin(&cda, 2, (ub1 *) &datai, (sword) sizeof(float),SQLT_FLT,
			   -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
//		err_write("oci error: odefin(data)\n");
		OracleError(&cda);
		return(FALSE);   
  	}

	if (odefin(&cda, 3, (ub1 *) &id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
//		err_write("oci error: odefin(id)\n");
		OracleError(&cda);
		return(FALSE);   
  	}


	if (oexec(&cda))
	{
		OracleError(&cda);
		return(FALSE);
	}
	n=0;	
	while( ofetch(&cda)==0 )
 	{     
		if(datai == CONST_NEG)
		{
			fvalue= data0;
		}
		else
		{
			fvalue= datai;
		}
 		n++;  	 	                         	 	
	}
 	if ( n<=0 )	
	{
//		sprintf(log_str,"err: %s  %s  second data not found ",logicalname, TimeStr);
//		err_write(log_str);
		return TRUE;
	}

	if(InsertHdbBySceneData( cda, scene_id, id, fvalue, status ) !=TRUE)
	{
		return FALSE;
	}

	return TRUE;	
}
 
int InsertOneYxSceneFromHDB(Cda_Def cda,char *logicalname, char *time_str, int scene_id)
{
	float	fvalue;
	int		status=0;
	int		id,n;	
	char	alstr[]="select spe.id as id, NVL(alm.STATUS_ID,0), NVL(alm.data,0)  from ems.almcmd alm, ems.speciality spe where spe.logicalname ='%s' and spe.id=alm.id and  alm.datetime<= '%s' and rownum<=1 order by alm.datetime desc"; 
	char	SelectStr[300];

	sprintf(SelectStr, alstr, logicalname, time_str);
//	log_write(INF,3,SelectStr);
	if( oparse(&cda, (unsigned char *)SelectStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
 //       	err_write("oci error: oparse()\n");
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) &id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
// 	     	err_write("oci error: odefin(id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 2, (ub1 *) &status, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(status_id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 3, (ub1 *) &fvalue, (sword) sizeof(float),SQLT_FLT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(fvalue)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (oexec(&cda))
	{
//      		err_write("oci error: oexec()\n");
      		OracleError(&cda);
      		return(FALSE);
	}
	n=0;
	while( ofetch(&cda)==0 )
 	{     
		n++;
 		break;
	}
 	if ( n<=0 )	
	{
//		sprintf(log_str,"err: %s  %s  second data not found ",logicalname, time_str);
//		err_write(log_str);
		return TRUE;
	}
	
	if(InsertHdbBySceneData( cda, scene_id, id, fvalue, (short)status ) !=TRUE)
	{
		return FALSE;
	}

	return TRUE;	
}
 
/* removed by yyp 2011.5.23
int SelectSceneListFromHdb(Cda_Def cda , unsigned int *p_num, struct hdb_scene_seg *p_scene)
{
	int scene_id;
	char timestr[30];
	char inserttime[30];
	int type;
	char remark[500];
	int		n;	
	char	SelectStr[]="select scene_id, NVL(type,0), datetime, NVL(inserttime,'1900-1-1'), NVL(remark,' ')  from ems.scene order by scene_id desc"; 

	log_write(INF,3,SelectStr);
	if( oparse(&cda, (unsigned char *)SelectStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	err_write("oci error: oparse()\n");
        	OracleError(&cda);
        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) &scene_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	err_write("oci error: odefin(id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 2, (ub1 *) &type, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	err_write("oci error: odefin(type)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 3, (ub1 *) timestr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}

	if (odefin(&cda, 4, (ub1 *) inserttime, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}

	if (odefin(&cda, 5, (ub1 *) remark, 400,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}

	if (oexec(&cda))
	{
      		err_write("oci error: oexec()\n");
      		OracleError(&cda);
      		return(FALSE);
	}
	n=0;
	while( ofetch(&cda)==0 )
 	{     
		if( n>=MAX_HDB_SCENE )
 		{	
			cda.rc = NO_DATA_FOUND;
 			break; 
 		}
 		else
 		{
			(p_scene+n)->scene_id = scene_id;
			(p_scene+n)->type = type;
			strcpy((p_scene+n)->timestr,timestr);
			strcpy((p_scene+n)->inserttime, inserttime);
			strcpy((p_scene+n)->remark, remark);				
		}
 		n++;  	 	                         	 	
	}
 
	if (cda.rc != NO_DATA_FOUND)
	{
		OracleError(&cda);
		//p_rs->yc_data[cur_id].rscount=0;
	}
	else
	{	
		*p_num = n;
	}

	return TRUE;
}

int SelectSceneDataFromHdb(Cda_Def cda , int scene_id, unsigned int *p_num, struct scene_data *p_data)
{
	int status;
	float fvalue;
	char logicalname[55];
	int	n;	
	char SqlStr[]="select trim(spe.logicalname), sc.status, sc.data from ems.speciality spe, ems.scenedata sc  \
		where spe.id=sc.pt_id and sc.scene_id =%d order by sc.pt_id "; 
	char SelectStr[300];
	sprintf(SelectStr,SqlStr,scene_id);
	log_write(INF,3,SelectStr);
	if( oparse(&cda, (unsigned char *)SelectStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	err_write("oci error: oparse()\n");
        	OracleError(&cda);
        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) logicalname, 55,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}


	if (odefin(&cda, 2, (ub1 *) &status, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	err_write("oci error: odefin(status)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 3, (ub1 *) &fvalue, (sword) sizeof(float),SQLT_FLT,
				   -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	err_write("oci error: odefin(fvalue)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}


	if (oexec(&cda))
	{
      		err_write("oci error: oexec()\n");
      		OracleError(&cda);
      		return(FALSE);
	}
	n=0;
	while( ofetch(&cda)==0 )
 	{     
		if( n>=MAX_PT_NUM )
 		{	
			cda.rc = NO_DATA_FOUND;
 			break; 
 		}
 		else
 		{
			(p_data+n)->status = (unsigned short)status;
			(p_data+n)->value = fvalue;
			strcpy( (p_data+n)->LogiName, logicalname);
		}
 		n++;
	}
 
	if (cda.rc != NO_DATA_FOUND)
	{
		OracleError(&cda);
		*p_num=0;
	}
	else
	{	
		*p_num = n;
	}
	return TRUE;
}

*/
int SelectRevertFromHdb( Cda_Def cda,struct  hdb_revert *p_rs, struct point_value *p_data)
{
	char	curve[1500]; 
	char	cmd[1500];
	char	TimeStr[30];
	char	tmpstr[10];
	int		year,mon,day,hour,min,sec;
	int		n;
	int		rowid;
	int 	secnum=1;						
	char	 LogicalName[38], StartTime[30], EndTime[30];	
	float	data[60];
	int		i;
 	struct tm tmtime;	
	unsigned char stnid,devid;
	StrToTime(p_rs->starttime,&tmtime,ONE_MIN,1);
	tmtostring(tmtime,StartTime);
	StrToTime(p_rs->endtime,&tmtime,ONE_MIN,1);
	tmtostring(tmtime,EndTime);
	strcpy(curve,"select  to_char(datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, ");
	for(i=0;i<60;i++)
	{
		strcat(curve,"NVL(data");
		sprintf(tmpstr,"%d",i);
		strcat(curve,tmpstr);
		strcat(curve,",");
		strcat(curve,CONST_NEG_STR);
		strcat(curve,")");
		if(i!=59)
		{
			strcat(curve,",");
		}
	}
	i= strlen(curve);
	strcat(curve,"  from ems.second where ");
	strcat(curve," id = %d and datetime>= '%s' and datetime<= '%s' order by datetime");
	p_data->rscount=0;

    strcpy(LogicalName,p_data->logicalname);

	stnid = p_data->point.stn_id;
	devid = p_data->point.dev_id;
	sprintf(cmd, curve, p_data->id, StartTime, EndTime);

//	log_write(INF,3,cmd);

	if( oparse(&cda,(unsigned char *) cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) TimeStr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}
	for(i=0;i<60;i++)
	{
		if (odefin(&cda, i+2, (ub1 *) &(data[i]), (sword) sizeof(float),SQLT_FLT,
				   -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  		{
//			err_write("oci error: odefin(data)\n");
			OracleError(&cda);
			return(FALSE);   
  		}
	}
	if (oexec(&cda))
	{
 //     	err_write("oci error: oexec()\n");
		printf("SqlStr=%s\n",cmd);
		OracleError(&cda);
		return(FALSE);
	}
		
	n=0;
	rowid = 0;
	while( ofetch(&cda)==0 )
 	{     
		if( rowid>=MAX_SELECT_REC )
 		{	
			cda.rc = NO_DATA_FOUND;
 			break; 
 		}
 		else
 		{
			for(i=0;i<60;i++)
			{
				if(data[i] != CONST_NEG)
				{
					p_data->value[rowid] = data[i] ; 
					sscanf(TimeStr,"%d-%d-%d %d:%d:%d",&year,&mon,&day,
	 					&hour,&min,&sec);
					sprintf(TimeStr,"%d-%d-%d %d:%d:%d",year,mon,day,
	 					hour,min,i);	 												
					strcpy(p_data->timestr[rowid],TimeStr);
					rowid ++;
 					if( rowid>=MAX_REVERT_REC )
 					{	
 						break; 
 					}
				}
			}
 			if( rowid>=MAX_REVERT_REC )
 			{	
 				break; 
 			}
 		}
 		n++;  	 	                         	 	
	}
 
	if (cda.rc != NO_DATA_FOUND)
	{
		OracleError(&cda);
		//p_data->rscount=0;
	}
	else
	{	
		p_data->rscount = rowid;
	}
	p_data->rscount = rowid;
	return(TRUE);
}

int SelectYxRevertFromHdb_A(cocictx *ctxptr, struct point_value *p_data, struct  yx_revert *p_yx, int rownum)
{	
	int n=0;
	int a=0,b=0,c=0;
	char TimeStr[MAX_REVERT_REC][22];
	int status_id[MAX_REVERT_REC] = {0};
	ub4 valsk1 = (ub4) sizeof(struct yx_revert); // value skip 
	ub4 valsk2 = (ub4) sizeof(struct point_value); // value skip 
	ub4 indsk = 0; //  indicator skips 
	ub4 rlsk = 0; //  return length skips 
	ub4 rcsk = 0; //  return code skips 
//	text *ins_stmt1 = (text *)"select id, to_char(datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, MILLISECOND,NVL(STATUS_ID,0),NVL(data,0), NVL(QUALITY_CODE,0),NVL(TYPEID_ID,0)  from ems.almcmd where id= :1 and  datetime>= (select max(datetime) from ems.almcmd where datetime<= :2 and id = :3 ) and datetime<= :4  and typeid_id<47 order by datetime";
	//text *ins_stmt1 = (text *)"select to_char(datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, NVL(STATUS_ID,0) from ems.almcmd where id= :1 and  datetime>= (select max(datetime) from ems.almcmd where datetime<= :2 and id = :3 ) and datetime<= :4  and typeid_id<47 order by datetime";
	text *ins_stmt1 = (text *)"select NVL(STATUS_ID,0) from ems.indrec where id= :1";
	OCIBind *bndp[4];
    OCIDefine *defnp[2];
	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
		ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
		(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[1], 
		ctxptr->errhp, (ub4) 1, (dvoid *)&p_yx->id, 
		(sb4) sizeof(p_yx->id), SQLT_INT,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
// 	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[2], 
// 		ctxptr->errhp, (ub4) 2, (dvoid *)(p_yx->starttime), 
// 		22, SQLT_STR,
// 		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
// 		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
// 	
// 	
// 	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[3], 
// 		ctxptr->errhp, (ub4) 3, (dvoid *) &(p_yx->id), 
// 		(sb4) sizeof(p_yx->id), SQLT_INT,
// 		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
// 		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
// 	
// 	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[4], 
// 		ctxptr->errhp, (ub4) 4, (dvoid *) (p_yx->endtime), 
// 		22, SQLT_STR,
// 		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
// 		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	if (OCIBindArrayOfStruct(bndp[1],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
// 		|| OCIBindArrayOfStruct(bndp[2],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
// 		|| OCIBindArrayOfStruct(bndp[3],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
// 		|| OCIBindArrayOfStruct(bndp[4],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		)
	{
		printf("FAILED: OCIBindArrayOfStruct()\n");
		return FALSE;
	}    
	
// 		checkerr(ctxptr->errhp, OCIDefineByPos(ctxptr->stmthp, defnp[1], 
// 			ctxptr->errhp, (ub4) 1, (dvoid *) (TimeStr), 
// 			22, SQLT_STR, (dvoid *) 0, 
// 			(ub2 *)0, (ub2 *)0, (ub4) OCI_DEFAULT));
	
		a = sizeof(status_id);
		b = sizeof(status_id[0]);
		checkerr(ctxptr->errhp, OCIDefineByPos(ctxptr->stmthp, &defnp[2], 
			ctxptr->errhp, (ub4) 1, (dvoid *) &(status_id[0]), 
			(sb4) sizeof(status_id[0]), SQLT_INT, (dvoid *) 0, 
			(ub2 *)0, (ub2 *)0, (ub4) OCI_DEFAULT));
	
		if (/*OCIDefineArrayOfStruct(defnp1,ctxptr->errhp,22, sizeof(sb2), sizeof(ub2), sizeof(ub2))*/
			/*||*/ OCIDefineArrayOfStruct(defnp[2],ctxptr->errhp,(ub4) sizeof(status_id[0]), sizeof(sb2), sizeof(ub2), sizeof(ub2))
			)
		{
			printf("FAILED: OCIDefineArrayOfStruct()\n");
			return FALSE;
		}   
	 
	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) 0, (ub4)0,
						  (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT))==FALSE)
	{
		HdbConn=FALSE; 
	}
	while (OCIStmtFetch(ctxptr->stmthp, ctxptr->errhp,1,OCI_FETCH_NEXT,OCI_DEFAULT) != OCI_NO_DATA) 
	{ 
		p_data->status[n] = status_id[n] ; 
		strcpy(p_data->timestr[n],TimeStr[n]);
		printf("\n a = %d\n",status_id[n]);
		n++;
	} 
	return(TRUE);	
}
int SelectYxRevertFromHdb(Cda_Def cda,struct  hdb_revert *p_rs,struct point_value *p_data)
{
	//char	alstr[]="select id, to_char(datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, MILLISECOND,NVL(STATUS_ID,0),NVL(data,0), NVL(QUALITY_CODE,0),NVL(TYPEID_ID,0)  from ems.almcmd where id=%d and  datetime> '%s' and datetime<= '%s'  and typeid_id<47 order by datetime "; 
	char	alstr[]="select id, to_char(datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, MILLISECOND,NVL(STATUS_ID,0),NVL(data,0), NVL(QUALITY_CODE,0),NVL(TYPEID_ID,0)  from ems.indrec where id=%d and  datetime>= (select max(datetime) from ems.indrec where datetime<= '%s' and id = %d ) and datetime<= '%s'  and typeid_id<47 order by datetime "; 
	char	SelectStr[800];
	float	fvalue;
	char	TimeStr[30], LogicalName[LOGICALNAME_LEN+1];
	int	ms;
	int	type_id,quality_code,status_id;	
	int	n;
	int id;
	int 	secnum=1;			
	char	 StartTime[30], EndTime[30];	
	//SelectYxRevertFromHdb_First(cda,  p_rs , p_data);	
	n=0;
	strcpy(LogicalName, p_data->logicalname);
	strcpy(StartTime,p_rs->starttime);
	//strcpy(StartTime,p_data->timestr[0]);
	strcpy(EndTime,p_rs->endtime);
	
	//sprintf(SelectStr, alstr, p_data->id, StartTime, EndTime);
	sprintf(SelectStr, alstr, p_data->id, StartTime,p_data->id, EndTime);
	log_write(INF,3,SelectStr);
	if( oparse(&cda, (unsigned char *)SelectStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
//        	err_write("oci error: oparse()\n");
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) &id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 2, (ub1 *) TimeStr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
 //     		err_write("oci error: odefin(TimeStr)\n");
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (odefin(&cda, 3, (ub1 *) &ms, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(ms)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 4, (ub1 *) &status_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(status_id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 5, (ub1 *) &fvalue, (sword) sizeof(float),SQLT_FLT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
// 	     	err_write("oci error: odefin(fvalue)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 6, (ub1 *) &quality_code, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(quality_code)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}
  	
	if (odefin(&cda, 7, (ub1 *) &type_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(type_id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}  	

	if (oexec(&cda))
	{
//      		err_write("oci error: oexec()\n");
			printf("SqlStr=%s\n",SelectStr);
      		OracleError(&cda);
      		return(FALSE);
	}

	while( ofetch(&cda)==0 )
 	{     
 		if( n>=MAX_REVERT_REC )
 		{	
 			cda.rc = NO_DATA_FOUND;			
 			break; 
 		}
 		else
 		{	
			if(type_id >= TIME_SYNC)
			{
				continue;
			}
			p_data->status[n] = status_id ; 
			strcpy(p_data->timestr[n],TimeStr);
 			n++;
 		}
	}
 
	if (cda.rc != NO_DATA_FOUND)
	{
	
		OracleError(&cda);

		/* p_data->rscount=0; */
	}
	else
	{	
		p_data->rscount=n;
	}

	return TRUE;
}

int SelectYxRevertFromHdb_First(Cda_Def cda, unsigned short curid, struct  hdb_revert *p_rs)
{
	char	alstr[]="select spe.logicalname, to_char(alm.datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, alm.MILLISECOND,NVL(alm.STATUS_ID,0),NVL(alm.data,0), NVL(alm.QUALITY_CODE,0),NVL(alm.TYPEID_ID,0)  from ems.indrec alm,ems.speciality spe where spe.logicalname ='%s' and spe.id=alm.id and  alm.datetime<= '%s' and rownum<=1 order by alm.datetime desc"; 
	char	SelectStr[300];
	float	fvalue;
	char	TimeStr[30], LogicalName[LOGICALNAME_LEN+1];
	int		ms;
	int		type_id,quality_code,status_id;	
	int		n;
	int 	secnum=1;
	char	StartTime[30];

	strcpy(LogicalName, p_rs->yx_data[curid].logicalname);
	strcpy(StartTime,p_rs->starttime);
	sprintf(SelectStr, alstr, LogicalName, StartTime);
//	log_write(INF,3,SelectStr);
	if( oparse(&cda, (unsigned char *)SelectStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
//		err_write("oci error: oparse()\n");
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);	
	}
	if (odefin(&cda, 1, (ub1 *) LogicalName, LOGICALNAME_LEN+1,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
//      		err_write("oci error: odefin(LogicalName)\n");
      		OracleError(&cda);
      		return(FALSE);
	}	
	if (odefin(&cda, 2, (ub1 *) TimeStr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
//      		err_write("oci error: odefin(TimeStr)\n");
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (odefin(&cda, 3, (ub1 *) &ms, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(ms)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 4, (ub1 *) &status_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(status_id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 5, (ub1 *) &fvalue, (sword) sizeof(float),SQLT_FLT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(fvalue)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 6, (ub1 *) &quality_code, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(quality_code)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}
  	
	if (odefin(&cda, 7, (ub1 *) &type_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(type_id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}  	
	if (oexec(&cda))
	{
 //     		err_write("oci error: oexec()\n");
      		OracleError(&cda);
      		return(FALSE);
	}
	n=0;
	while( ofetch(&cda)==0 )
 	{     
		p_rs->yx_data[curid].status[n] = status_id ; 
		strcpy(p_rs->yx_data[curid].timestr[n],StartTime);
		n++;
 		break;
	}
	
	p_rs->yx_data[curid].rscount=n;
	return TRUE;
}

int SelectGenPowerPlanCurveFromHdb(Cda_Def cda, unsigned char cur_id,struct  hdb_curve *p_rs)
{
	char	alstr[]="select time,value from ems.dispatchscheduledata where scheduleid ='%d' and  time>= '%s' and time < '%s' order by time"; 
	char	selectStr[300];
	int		n;
	char	startTime[22], endTime[22];

	char	timeStr[22];
	float	fvalue;

	int		id=p_rs->seg[cur_id].stnid;
	
	strcpy(startTime, p_rs->starttime);
	strcpy(endTime, p_rs->endtime);
	sprintf(selectStr, alstr, id, startTime, endTime);
//	log_write(INF, 3, selectStr);

	if( oparse(&cda, (unsigned char *)selectStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
//		err_write("oci error: oparse()\n");
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);	
	}
	
	if (odefin(&cda, 1, (ub1 *)timeStr, 22,SQLT_STR,
		-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
//      		err_write("oci error: odefin(TimeStr)\n");
			OracleError(&cda);
			return(FALSE);   
	}
	
	if (odefin(&cda, 2, (ub1 *) &fvalue, (sword) sizeof(float),SQLT_FLT,
		-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
 //	     	err_write("oci error: odefin(ms)\n");
			OracleError(&cda);
			return(FALSE);   
	}
	
	if (oexec(&cda))
	{
 //     		err_write("oci error: oexec()\n");
			OracleError(&cda);
			return(FALSE);
	}

	n=0;
	while( ofetch(&cda)==0 )
	{
		strcpy(p_rs->seg[cur_id].timestr[n], timeStr);
		p_rs->seg[cur_id].data[n] = fvalue ; 
		
		n++;

		if (n >= MAX_SELECT_REC)
		{
			break;
		}
	}
	
	p_rs->seg[cur_id].rscount=n;
	return TRUE;
}

int SelectCurveFromHdb(Cda_Def cda, unsigned char cur_id,struct  hdb_curve *p_rs)
{
	char	curve[1500]; 
	char	cmd[1500];
	char	TimeStr[30];
	char	tmpstr[10];
	int		year,mon,day,hour,min,sec;
	int		n;
	int		rowid;
	int 	secnum=1;						
	char	 LogicalName[38], StartTime[30], EndTime[30];	
	float	data[60];
	int		i;
	unsigned char stnid,devid;
	strcpy(curve,"select  to_char(sec.datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, ");
	for(i=0;i<60;i++)
	{
		strcat(curve,"NVL(data");
		sprintf(tmpstr,"%d",i);
		strcat(curve,tmpstr);
		strcat(curve,",");
		strcat(curve,CONST_NEG_STR);
		strcat(curve,")");
		if(i!=59)
		{
			strcat(curve,",");
		}

	}
	i= strlen(curve);
	strcat(curve,"  from ems.speciality spe, ems.second sec where spe.id=sec.id");
	strcat(curve,"  and spe.logicalname='%s' and sec.datetime>= '%s' and sec.datetime<= '%s' order by sec.datetime");
	p_rs->seg[cur_id].rscount = 0;

    strcpy(LogicalName,p_rs->seg[cur_id].logicalname);
	strcpy(StartTime,p_rs->starttime);		
	strcpy(EndTime,p_rs->endtime);
	stnid = p_rs->seg[cur_id].stnid;
	devid = p_rs->seg[cur_id].devid;
	sprintf(cmd, curve, LogicalName, StartTime, EndTime);
//	printf("%s\n", cmd);
//	log_write(INF,3,cmd);
	if( oparse(&cda,(unsigned char *) cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) TimeStr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}
	for(i=0;i<60;i++)
	{
		if (odefin(&cda, i+2, (ub1 *) &(data[i]), (sword) sizeof(float),SQLT_FLT,
				   -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  		{
 //	     		err_write("oci error: odefin(data)\n");
 	     		OracleError(&cda);
      			return(FALSE);   
  		}
	}
	if (oexec(&cda))
	{
      		OracleError(&cda);
      		return(FALSE);
	}
		
	n=0;
	rowid = 0;
	while( ofetch(&cda)==0 )
 	{     
		if( rowid>=MAX_SELECT_REC )
 		{	
			cda.rc = NO_DATA_FOUND;
 			break; 
 		}
 		else
 		{
			for(i=0;i<60;i++)
			{
				if(data[i] != CONST_NEG)
				{
					p_rs->seg[cur_id].data[rowid] = data[i] ; 
					sscanf(TimeStr,"%d-%d-%d %d:%d:%d",&year,&mon,&day,
	 					&hour,&min,&sec);
					sprintf(TimeStr,"%d-%d-%d %d:%d:%d",year,mon,day,
	 					hour,min,i);	 												
					strcpy(p_rs->seg[cur_id].timestr[rowid],TimeStr);
					rowid ++;
 					if( rowid>=MAX_SELECT_REC )
 					{	
 						break; 
 					}					
				}
			}
 			if( rowid>=MAX_SELECT_REC )
 			{	
 				break; 
 			}	
 		}
 		for(i=0;i<60;i++)
		{
			data[i] = CONST_NEG;
		}
 		n++;  	 	                         	 	
	}
 
	if (cda.rc != NO_DATA_FOUND)
	{
		OracleError(&cda);
		p_rs->seg[cur_id].rscount=0;
	}
	else
	{	
		p_rs->seg[cur_id].rscount = rowid;
	}
	return(TRUE);
}

int InsertHdbByCurve(cocictx *ctxptr,int ptnum, SECDATA *data_all )
{
	return(TRUE);
}


int SelectAlRecFromHdb(Cda_Def cda, struct  hdb_al *p_rs)
{
//	char	alstr[800]="select trim(ems.speciality.logicalname), to_char(ems.almcmd.datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, ems.almcmd.MILLISECOND,NVL(ems.almcmd.STATUS_ID,0),NVL(ems.almcmd.data,0), NVL(ems.almcmd.QUALITY_CODE,0),NVL(ems.almcmd.TYPEID_ID,0)  from ems.almcmd,ems.speciality where (not (ems.almcmd.typeid_id=4 and ems.almcmd.data=-1)) and (not (ems.almcmd.typeid_id=11 and ems.almcmd.data=-1)) and ems.almcmd.id =ems.speciality.id and ems.almcmd.datetime>= '%s' and ems.almcmd.datetime<= '%s' "; 
	char	alstr[800]="select trim(spe.logicalname), to_char(alm.datetime,'yyyy-mm-dd hh24:mi:ss') as dtime, alm.MILLISECOND,NVL(alm.STATUS_ID,0),NVL(alm.data,0), NVL(alm.QUALITY_CODE,0),NVL(alm.TYPEID_ID,0)  from ems.almcmd alm,ems.speciality spe where (not (alm.typeid_id=11 and alm.data=-1)) and alm.id =spe.id and alm.datetime>= '%s' and alm.datetime<= '%s' "; 
	char	SelectStr[800],TmpStr[100];
	float	fvalue;
	char	TimeStr[30], LogicalName[LOGICALNAME_LEN+1];
	int	ms;
	int	type_id,quality_code,status_id;	
	int	n;
	int 	secnum=1;						/*sleep seconds number*/
	char	 StartTime[30], EndTime[30];	
	strcpy(StartTime,p_rs->starttime);	
	strcpy(EndTime,p_rs->endtime);
	sprintf(SelectStr, alstr, StartTime, EndTime);
	if(p_rs->cname_type==1)				/* longname */
	{
		sprintf(TmpStr," and spe.longname like '%s%s%s' ","%",p_rs->cname,"%");
		strcat(SelectStr,TmpStr);		
	}
	else if(p_rs->cname_type==2)			/* logicalname */
	{
		sprintf(TmpStr," and spe.logicalname like '%s%s%s' ","%",p_rs->cname,"%");
		strcat(SelectStr,TmpStr);	
	}
	if(p_rs->filter_typeid >0 )
	{
		sprintf(TmpStr," and  alm.type_id=%d ",p_rs->filter_typeid);
		strcat(SelectStr,TmpStr);
	}
	strcat(SelectStr," order by alm.datetime");	
//	log_write(INF,2,SelectStr);	

	if( oparse(&cda, (unsigned char *)SelectStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
//        	err_write("oci error: oparse()\n");
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) LogicalName, LOGICALNAME_LEN+1,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
//      		err_write("oci error: odefin(LogicalName)\n");
      		OracleError(&cda);
      		return(FALSE);   
	}	

	
	if (odefin(&cda, 2, (ub1 *) TimeStr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
 //     		err_write("oci error: odefin(TimeStr)\n");
      		OracleError(&cda);
      		return(FALSE);   
	}

	if (odefin(&cda, 3, (ub1 *) &ms, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(ms)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 4, (ub1 *) &status_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
// 	     	err_write("oci error: odefin(status_id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 5, (ub1 *) &fvalue, (sword) sizeof(float),SQLT_FLT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(fvalue)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}

	if (odefin(&cda, 6, (ub1 *) &quality_code, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 //	     	err_write("oci error: odefin(quality_code)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}
  	
	if (odefin(&cda, 7, (ub1 *) &type_id, (sword) sizeof(int),SQLT_INT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
// 	     	err_write("oci error: odefin(type_id)\n");
 	     	OracleError(&cda);
      		return(FALSE);   
  	}  	

	if (oexec(&cda))
	{
 //     		err_write("oci error: oexec()\n");
      		OracleError(&cda);
      		return(FALSE);
	}
		
	n=0;
	while( ofetch(&cda)==0 )
 	{     
 		if( n>=MAX_AL_NUM )
 		{	
			cda.rc = NO_DATA_FOUND;
 			break; 
 		}
 		else
 		{	
			p_rs->rec_data[n].fvalue=fvalue;
			p_rs->rec_data[n].type_id=(unsigned char)type_id;
			p_rs->rec_data[n].status=(short)status_id;
			p_rs->rec_data[n].msecond=ms;
 			strcpy(p_rs->rec_data[n].l_name,LogicalName);	
 			strcpy(p_rs->rec_data[n].TimeTag,TimeStr);
 			n++;
 		}   	 	                         	 	
	}
 
	if (cda.rc != NO_DATA_FOUND)
	{
	    OracleError(&cda);
	    p_rs->alnum=0;
	}
	else
	{	
		p_rs->alnum = n;
	}
	return(TRUE);
}

int SelectYcRecFromHdb(Cda_Def cda, unsigned char curid,struct  hdb_curve *p_rs)
{
	char	minute[]="select min_data, to_char(min_datetime,'yyyy-mm-dd hh24:mi:ss') as dtime from ems.minute min, ems.speciality spe  where spe.logicalname= '%s' and min.id =spe.id and min_datetime>= '%s' and min_datetime<= '%s' order by min_datetime"; 
	char	hour[]="select hour_data, to_char(hour_datetime,'yyyy-mm-dd hh24:mi:ss') as dtime from ems.hour hour, ems.speciality spe where spe.logicalname= '%s' and hour.id=spe.id and hour_datetime>= '%s' and hour_datetime<= '%s'order by hour_datetime "; 
	char	SelectStr[300],	cmd[300];
	float	fvalue;
	char	TimeStr[30];
	int		n;
	int 	secnum=1;
	char	LogicalName[30], StartTime[30], EndTime[30];	
	p_rs->seg[curid].lastrscount = p_rs->seg[curid].rscount;
	strcpy(LogicalName,p_rs->seg[curid].logicalname);
	strcpy(StartTime,p_rs->seg[curid].lasttime);	
	strcpy(EndTime,p_rs->endtime);
	switch (p_rs->interval)
	{
	case ONE_MIN:
		secnum=1*60;
		strcpy(SelectStr,minute);
		break;
	case FIVE_MIN:
		return(FALSE) ;		
		/*
		secnum=5*60;
		strcpy(TableName,"ems.five");
		strcpy(SelectStr,five);
		*/
		 break;
	case FIFTEEN_MIN:
		return(FALSE) ;
		/*
		secnum=15*60;
		strcpy(TableName,"ems.fifteen");
		strcpy(SelectStr,fifteen);
		*/
		break;
	case ONE_HOUR:
		return(FALSE) ;
		/*		
		secnum=60*60;
		strcpy(TableName,"ems.hour");
		strcpy(SelectStr,hour);
		*/
		break;
	default:
		return(FALSE) ;
		break;
	}
	
	sprintf(cmd, SelectStr, LogicalName, StartTime, EndTime);
//	log_write(INF,2,cmd);
	if( oparse(&cda, (unsigned char *)cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if (odefin(&cda, 1, (ub1 *) &fvalue, (sword) sizeof(float),SQLT_FLT,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
  	{
 	     	OracleError(&cda);
      		return(FALSE);   
  	}
	
	if (odefin(&cda, 2, (ub1 *) TimeStr, 22,SQLT_STR,
               -1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
      		OracleError(&cda);
      		return(FALSE);   
	}
	if (oexec(&cda))
	{
      		OracleError(&cda);
      		return(FALSE);
	}
		
	n=0;
	while( ofetch(&cda)==0 )
 	{     
 		if( n>=MAX_SELECT_REC )
 		{	
			/* cda.rc = NO_DATA_FOUND; */
 			break; 
 		}
 		else
 		{	
			p_rs->seg[curid].data[n]=fvalue;
 			strcpy(p_rs->seg[curid].timestr[n],TimeStr);
 			n++;
 		}   	 	                         	 	
	}
 
	if (cda.rc != NO_DATA_FOUND)
	{
	    OracleError(&cda);
	    p_rs->seg[curid].rscount=0;
	}
	else
	{	
		p_rs->seg[curid].rscount = n;
	}
	return(TRUE);
}

int InsertHdbAlarm(Cda_Def cda, char *DateTime, int id, int status_id, int ivalue,float fvalue, unsigned char typeid  )
{
	char	InsertStr[]="insert into ems.almcmd( id, datetime, millisecond, status_id, data, typeid_id)  values(:ID, :TIME, :IVALUE, :STATUS_ID,:FVALUE ,:TYPEID)";	
	int 	type;
//	char tmpstr[500];
	type = typeid;
//	sprintf(log_str,"insert into ems.almcmd( id, datetime, millisecond, status_id, data, typeid_id) values(%d, '%s' ,%d ,%d,%5.2f,%d) ",
//		id, DateTime, ivalue, status_id,fvalue,typeid);
//	strcpy(tmpstr,log_str);
//	log_write(INF,3,log_str);	
	
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)InsertStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
    {
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}
	if(  obndrv(&cda, (text *) ":ID",-1, (ub1 *) &id,
		(sword) sizeof (ub4), SQLT_INT, -1,
		(sb2 *) 0, (text *) 0, -1, -1))
	{
        	OracleError(&cda); 
//        	sprintf(log_str,"Obndrv Orcal error ! (%d)\n",id); 
//			err_write(log_str);
			return(FALSE);    	
	}
	
	if(  obndrv(&cda, (text *) ":TIME",-1, (ub1 *) DateTime,
		TIMETAG_LEN+1, SQLT_STR, -1,
		(sb2 *) 0, (text *) 0, -1, -1))
    {
        	OracleError(&cda); 
//       	sprintf(log_str,"Obndrv Orcal error ! (%s)\n",DateTime); 
//			err_write(log_str);
			return(FALSE);       	
	}
	if(  obndrv(&cda, (text *) ":IVALUE",-1, (ub1 *) &ivalue,
		(sword) sizeof (ub4), SQLT_INT, -1,
		(sb2 *) 0, (text *) 0, -1, -1))
    {
        	OracleError(&cda); 
//       	sprintf(log_str,"Obndrv Orcal error ! (%d)\n",ivalue); 
//			err_write(log_str);
			return(FALSE);      	
	}
	if(  obndrv(&cda, (text *) ":STATUS_ID",-1, (ub1 *) &status_id,
		(sword) sizeof (ub4), SQLT_INT, -1,
		(sb2 *) 0, (text *) 0, -1, -1))
    {
        	OracleError(&cda); 
//        	sprintf(log_str,"Obndrv Orcal error ! (%d)\n",id); 
//			err_write(log_str);
			return(FALSE);      	
	}

	if(  obndrv(&cda, (text *) ":FVALUE",-1, (ub1 *) &fvalue,
		(sword) sizeof (ub4), SQLT_FLT, -1,
		(sb2 *) 0, (text *) 0, -1, -1))
    {
        	OracleError(&cda); 
//        	sprintf(log_str,"Obndrv Orcal error ! (%5.2f)\n",fvalue); 
//			err_write(log_str);
			return(FALSE);      	
	}
	
	if(  obndrv(&cda, (text *) ":TYPEID",-1, (ub1 *) &type,
		(sword) sizeof (ub4), SQLT_INT, -1,
		(sb2 *) 0, (text *) 0, -1, -1))
    {
        	OracleError(&cda); 
//        	sprintf(log_str,"Obndrv Orcal error ! (%d)\n",typeid); 
//			err_write(log_str);
			return(FALSE);      	
	}
   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      	OracleError(&cda);
//     	err_write("Oexec Orcal error !\n"); 
//		log_write(ERR,1,tmpstr);
		/* return(FALSE) ;  */
	}

	return(TRUE);
}

/**************insert yx in almcmd to hdb  add by dm 2010.10.28***************************/ 
int InsertHdbAlarm_A(cocictx *ctxptr, struct yx_seg *p_yx, int rownum)
{
	ub4 valsk1 = (ub4) sizeof(struct yx_seg); /* value skip */
	ub4 indsk = 0; /*  indicator skips */
	ub4 rlsk = 0; /*  return length skips */
	ub4 rcsk = 0; /*  return code skips */
	text *ins_stmt1 = (text *)"insert into ems.almcmd( id, datetime, millisecond, status_id, data, typeid_id)  values( :1, :2, :3, :4, -1, 4)";
	OCIBind *bndp[4];
	int i;
	for(i=0;i<4;i++)
	{
		bndp[i] = (OCIBind *) NULL;
	}
	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
		ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
		(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));
	
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[0], 
		ctxptr->errhp, (ub4) 1, (dvoid *)&(p_yx->id), 
		(sb4) sizeof(p_yx->id), SQLT_INT,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[1], 
		ctxptr->errhp, (ub4) 2, (dvoid *)p_yx->ReadTime, 
		22, SQLT_STR,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[2], 
		ctxptr->errhp, (ub4) 3, (dvoid *) &(p_yx->ms), 
		(sb4) sizeof(p_yx->ms), SQLT_INT,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[3], 
		ctxptr->errhp, (ub4) 4, (dvoid *) &(p_yx->status_id), 
		(sb4) sizeof(p_yx->status_id), SQLT_INT,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	if (OCIBindArrayOfStruct(bndp[0],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		|| OCIBindArrayOfStruct(bndp[1],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		|| OCIBindArrayOfStruct(bndp[2],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		|| OCIBindArrayOfStruct(bndp[3],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		)
	{
		printf("FAILED: OCIBindArrayOfStruct()\n");
		return FALSE;
	}    
	
	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) rownum, (ub4)0,
						  (OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT)) == FALSE)
	{
		HdbConn=FALSE; 
		return FALSE;
	}
	
	checkerr(ctxptr->errhp, OCITransCommit(ctxptr->svchp, ctxptr->errhp, (ub4) 0));
	
	return(TRUE);	
}
int InsertHdbSmsAlarm(Cda_Def cda, char *DateTime, char *message, unsigned char typeid  )
{
	char	FormatStr[200]="insert into ems.message(  datetime, message, typeid_id) values('%s','%s',%d)";
	char	InsertStr[300];
	sprintf(InsertStr,FormatStr, DateTime, message, typeid );
//	log_write(INF,3,InsertStr);
	
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)InsertStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      		OracleError(&cda);
//      		err_write("Oexec Orcal error !\n"); 
			/* return(FALSE);   */
	}
	return(TRUE);
}

int	InsertHdbRecord( Cda_Def cda, int user_id,int host_id,int groups_id, char *datetime,short ms, 
	unsigned char type, char *content, unsigned char station_id, unsigned char dev_id,int task_id, int send_task_id,
	char *logicalname, unsigned char type_id, short status,float fvalue)
{
	DATE_TIME time_cur;
	char	FormatStr[300]="insert into ots.record(user_id, host_id, groups_id, datetime,ms,type,content,station_id,dev_id,task_id,send_task_id,logicalname,type_id,status,fvalue) values(%d,%d,%d,'%s',%d,%d,'%s',%d,%d,%d,%d,'%s',%d,%d,%f)";
	char	InsertStr[500];
	char	cstatus[2];
	GetSysTime(&time_cur);	
	if(ms==0)
	{
		ms=time_cur.ms;
	}
	memcpy(&cstatus,&status,2);
	sprintf(InsertStr,FormatStr,(short)user_id, (short)host_id, (short)groups_id, datetime,ms,type,content,station_id,dev_id,task_id,send_task_id,logicalname,type_id,(short)(cstatus[0]),fvalue );
	//log_write(INF,3,InsertStr);
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)InsertStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\nInsertHdbByYx_A"); 
			return(FALSE);	
	}
   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      		OracleError(&cda);
//     		err_write("Oexec Orcal error !\n");
//			log_write(ERR,1,InsertStr);
			/* return(FALSE);   */
	}
	return(TRUE);
}
int InsertHdbByYcStatus(Cda_Def cda,int id,  char *DateTime, int istatus  )
{
	char	FormatStr[200]="insert into ems.quality_change(id,datetime, quality_code) values(%d,'%s',%d)";
	char	InsertStr[300];
	sprintf(InsertStr,FormatStr,id, DateTime, istatus );
//	log_write(INF,3,InsertStr);
	
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)InsertStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      		OracleError(&cda);
//      		err_write("Oexec Orcal error !\n"); 
			/* return(FALSE);   */
	}
	return(TRUE);
}


int InsertHdbByScene(Cda_Def cda, int scene_id, char *timestr, int type, char *inserttime )
{
	char	FormatStr[200]="insert into ems.scene(scene_id,datetime, type,inserttime) values(%d,'%s',%d, '%s')";
	char	InsertStr[300];
	sprintf(InsertStr,FormatStr,scene_id, timestr, type, inserttime );
//	log_write(INF,3,InsertStr);
	
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)InsertStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      		OracleError(&cda);
 //     		err_write("Oexec Orcal error !\n"); 
			/* return(FALSE);   */
	}
	return(TRUE);
}



int InsertHdbBySceneData( Cda_Def cda, int scene_id, int ptid, float fvalue, short status)
{	
	char	FormatStr[200]="insert into ems.scenedata(scene_id,pt_id, data, status) values(%d,%d,%f,%d)";
	char	InsertStr[300];
	sprintf(InsertStr,FormatStr,scene_id,ptid,fvalue, status );
//	log_write(INF,3,InsertStr);
	
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)InsertStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
        	OracleError(&cda);
 //       	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      		OracleError(&cda);
//      		err_write("Oexec Orcal error !\n"); 
			/* return(FALSE);   */
	}
	return(TRUE);
}

int DelSceneFromHDB( Cda_Def cda, int scene_id)
{	
	char	FormatStr[200]="delete from ems.scenedata where scene_id=%d";
	char	FormatStr2[200]="delete from ems.scene where scene_id=%d";
	char	DelStr[300];
	sprintf(DelStr,FormatStr,scene_id);
//	log_write(INF,3,DelStr);
	
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)DelStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);	
	}
	
   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
		OracleError(&cda);
//		err_write("Oexec Orcal error !\n"); 
		/* return(FALSE);   */
	}
	
	sprintf(DelStr,FormatStr2,scene_id);
//	log_write(INF,3,DelStr);
	/* Parse the PL/SQL block */
	if( oparse(&cda, (unsigned char *)DelStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);	
	}
	
   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
		OracleError(&cda);
//		err_write("Oexec Orcal error !\n"); 
		/* return(FALSE);   */
	}
	
	return(TRUE);
}

int UpdateHdbByYc(Cda_Def cda, int id, char *DateTime, float *fvalue, int istatus  )
{	
	char	FormatStr[200]="update ems.speciality set datetime= '%s',data= %.3f, quality_code= %d where id= %d";
	char	UpdateStr[300];
	sprintf(UpdateStr,FormatStr, DateTime, *fvalue, (char)istatus, id );
//	log_write(INF,3,UpdateStr);

	if( oparse(&cda, (unsigned char *)UpdateStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);
	}
	
	if( oexec(&cda) )
	{
		OracleError(&cda);
//		err_write("Oexec Orcal error !\n"); 
//		err_write(UpdateStr);
		return(FALSE);  
	}

	return(TRUE);
}


int UpdateHdbByYc_A(cocictx *ctxptr, struct yc_seg *p_yc, int rownum)
{
	ub4 valsk1 = (ub4) sizeof(struct yc_seg); /* value skip */
	ub4 indsk = 0; /*  indicator skips */
	ub4 rlsk = 0; /*  return length skips */
	ub4 rcsk = 0; /*  return code skips */
	text *ins_stmt1 = (text *)"update ems.speciality set datetime= :1,data= :2, quality_code= :3 where id= :4";
	OCIBind *bndp1 = (OCIBind *) NULL;
	OCIBind *bndp2 = (OCIBind *) NULL;
	OCIBind *bndp3 = (OCIBind *) NULL;
	OCIBind *bndp4 = (OCIBind *) NULL;

	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
                          ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
                          (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp1, 
				  ctxptr->errhp, (ub4) 1, (dvoid *)p_yc->TimeTag, 
				  (sb4) 22, SQLT_STR,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp2, 
				  ctxptr->errhp, (ub4) 2, (dvoid *)&(p_yc->value), 
				  (sb4) sizeof(p_yc->value), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));


	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp3, 
				  ctxptr->errhp, (ub4) 3, (dvoid *) &(p_yc->status), 
				  (sb4) sizeof(p_yc->status), SQLT_INT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp4, 
				  ctxptr->errhp, (ub4) 4, (dvoid *) &(p_yc->id), 
				  (sb4) sizeof(p_yc->id), SQLT_INT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	if (OCIBindArrayOfStruct(bndp1,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp2,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp3,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp4,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   )
	  {
		printf("FAILED: OCIBindeArrayOfStruct()\n");
		return FALSE;
	  }    

	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) rownum, (ub4)0,
						  (OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT)) == FALSE)
	{
		HdbConn=FALSE; 
		return FALSE;
	}
	
	checkerr(ctxptr->errhp, OCITransCommit(ctxptr->svchp, ctxptr->errhp, (ub4) 0));

	return(TRUE);	
}



int UpdateHdbByChara(Cda_Def cda, int id, char *DateTime, float max, float min, float avg,
					 float max_change, float min_change,float standard,unsigned short max_no,
					 unsigned short min_no,unsigned short max_change_no,unsigned short min_change_no)
{	
	time_t now;
	struct tm *datetime;
	char TimeStr[30];

	char	UpdateStr[300]="update ems.charavalue set datetime= '%s',MAXIMUM= %.3f,  MINIMUM= %.3f, AVERAGE=%.3f, STANDARD=%.3f, MAX_CHANGE=%.3f,MIN_CHANGE=%.3f,MAX_NO=%d,MIN_NO=%d,MAX_CHANGE_NO=%d,MIN_CHANGE_NO=%d where id= %d";
	char	InsertStr[300]="insert into ems.charahist(datetime,MAXIMUM,MINIMUM,AVERAGE,   \
		STANDARD,MAX_CHANGE,MIN_CHANGE,MAX_NO,MIN_NO,MAX_CHANGE_NO,MIN_CHANGE_NO,id) values('%s',%.3f,  \
		%.3f, %.3f, %.3f, %.3f,%.3f, %d, %d, %d, %d, %d)";
	char	SqlStr[300];
	time(&now);
	datetime=localtime(&now);
	tmtostring(*datetime,TimeStr);

	sprintf(SqlStr,UpdateStr, DateTime, max, min,avg,standard,max_change,
		min_change,max_no,min_no,max_change_no,min_change_no,id );
	/* log_write(INF,3,SqlStr); */

	if( oparse(&cda, (unsigned char *)SqlStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);
	}
	
	if( oexec(&cda) )
	{
		OracleError(&cda);
//		err_write("Oexec Orcal error !\n"); 
//		err_write(SqlStr);
		return(FALSE);  
	}

	sprintf(SqlStr,InsertStr, TimeStr, max, min,avg,standard,max_change,
		min_change,max_no,min_no,max_change_no,min_change_no,id );
	/* log_write(INF,3,SqlStrr); */

	if( oparse(&cda, (unsigned char *)SqlStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);
	}
	
	if( oexec(&cda) )
	{
		OracleError(&cda);
//		err_write("Oexec Orcal error !\n"); 
//		err_write(SqlStr);
		return(FALSE);  
	}
	
	return(TRUE);
}



int UpdateHdbByStat(Cda_Def cda, struct stat_seg *p_stat)
{	
	char	UpdateStr1[300]="update ems.statistic set STAT_DATETIME= '%s',  \
		STAT_TRUE= 1 where STAT_NAME= '%s'";
	char	UpdateStr2[300]="update ems.statistic set STAT_TRUE= 0,  \
		STAT_TOTAL= %.3f where STAT_NAME= '%s'";
	char	InsertStr[300]="insert into ems.stat(STAT_NAME,STAT_START,STAT_END,STAT_HOURS)	\
		values('%s','%s','%s',%.3f)";
	char	SqlStr[500];
	float total;
	int i;
	struct tm time1,time2;
	time_t itime1,itime2;

	if( SelectStatTrueFromHdb(cda, p_stat) != TRUE)
	{
//		err_write("SelectStatTrueFromHdb error !\n"); 
		return(FALSE);
	}
	for(i=0;i<16;i++)
	{
		if(p_stat->isstat[i] != 1 )
		{
			continue;
		}
		if( i==p_stat->curstatus )
		{
			sprintf(SqlStr,UpdateStr1, p_stat->starttime,   p_stat->name[p_stat->curstatus] );
			/* log_write(INF,3,SqlStr); */
			
			if( oparse(&cda, (unsigned char *)SqlStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
			{
				OracleError(&cda);
//				err_write("Oparse Orcal error !\n"); 
				return(FALSE);
			}
			
			if( oexec(&cda) )
			{
				OracleError(&cda);
//				err_write("Oexec Orcal error !\n"); 
//				err_write(SqlStr);
				return(FALSE);  
			}
		}
		else
		{
			if( p_stat->istrue[i] ==1 )
			{
				/* needed to be added how to get totaltime */
				StrToTime(p_stat->lasttime,&time1,ONE_SEC,0);
				StrToTime(p_stat->starttime,&time2,ONE_SEC,0);
				itime1 = mktime(&time1);
				itime2 = mktime(&time2);
				total =(float) (itime2 - itime1) /(60 *60);
				p_stat->totaltime[i] += total;
				sprintf(SqlStr,UpdateStr2, p_stat->totaltime[i],   p_stat->name[i] );
				/* log_write(INF,3,SqlStr); */

				if( oparse(&cda, (unsigned char *)SqlStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
				{
					OracleError(&cda);
//					err_write("Oparse Orcal error !\n"); 
					return(FALSE);
				}
				
				if( oexec(&cda) )
				{
					OracleError(&cda);
//					err_write("Oexec Orcal error !\n"); 
//					err_write(SqlStr);
					return(FALSE);  
				}				
			}
		}
	}

	sprintf(SqlStr,InsertStr, p_stat->name[p_stat->oldstatus],p_stat->lasttime, 
		p_stat->starttime, p_stat->totaltime[p_stat->oldstatus] );
	/* log_write(INF,3,SqlStr); */

	if( oparse(&cda, (unsigned char *)SqlStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);
	}
	
	if( oexec(&cda) )
	{
		OracleError(&cda);
//		err_write("Oexec Orcal error !\n"); 
//		err_write(SqlStr);
		return(FALSE);  
	}


	return(TRUE);
}

#ifdef YYP
int UpdateHdbByPeriod(cocictx *ctxptr, struct hdb_yc_period *p_data)
{	
	char	FormatStr[200]="insert into ems.minute(id,min_datetime,min_data,min_max,min_min,min_avg) values(%d, '%s',%5.2f, %5.2f, %5.2f, %5.2f)";
	int		totalsec;
	time_t t;
	struct tm datetime;
	char	UpdateStr[300];
	char	timestr[30];
	int		i,rownum,iscond;
	ub4 valsk1 = (ub4) sizeof(struct array_period_data); /* value skip */
	ub4 indsk = 0; /*  indicator skips */
	ub4 rlsk = 0; /*  return length skips */
	ub4 rcsk = 0; /*  return code skips */
	text *ins_stmt1 = (text *)"INSERT INTO ems.minute(id,min_datetime,min_data,min_max,min_min,min_avg) VALUES(:1, :2,:3,:4,:5,:6)";
	OCIBind *bndp1 = (OCIBind *) NULL;
	OCIBind *bndp2 = (OCIBind *) NULL;
	OCIBind *bndp3 = (OCIBind *) NULL;
	OCIBind *bndp4 = (OCIBind *) NULL;
	OCIBind *bndp5 = (OCIBind *) NULL;
	OCIBind *bndp6 = (OCIBind *) NULL;

	totalsec = 60;
 	t=time(NULL); 
	datetime=*localtime((long *)&t);
	DateMinus(&datetime,ONE_MIN);
	datetime.tm_sec = 0;
	tmtostring(datetime, timestr);
	rownum = 0;

	for(i=0;i<p_data->num;i++)
	{
		iscond = TRUE;
		GetHdbCondByName((unsigned int *)&(iscond),	p_data->seg[i].logicalname);
		if(cond[p_data->seg[i].point.stn_id][p_data->seg[i].point.dev_id].status == 1 
			&& iscond == TRUE && datetime.tm_min !=0  && g_wrcond == TRUE)  
			/* notice:  if olny dev cond == 1 then not write */
		{
			continue;
		}
		if( p_data->seg[i].id <=0  )
		{
			continue;
		}
		a_data[rownum].id = p_data->seg[i].id;
		strcpy(a_data[rownum].timestr,timestr);
		a_data[rownum].startdata  = p_data->seg[i].data_last.startdata;
		a_data[rownum].max  = p_data->seg[i].data_last.max;
		a_data[rownum].min  = p_data->seg[i].data_last.min;
		a_data[rownum].avg   = p_data->seg[i].data_last.sum/totalsec ;
		rownum++;
	}

	sprintf(UpdateStr, FormatStr, a_data[0].id,a_data[0].timestr,a_data[0].startdata, 
		a_data[0].max , a_data[0].min , a_data[0].avg);
//	log_write(INF,3,UpdateStr);
	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
                          ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
                          (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp1, 
				  ctxptr->errhp, (ub4) 1, (dvoid *) &(a_data[0].id), 
				  (sb4) sizeof(a_data[0].id), SQLT_INT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp2, 
				  ctxptr->errhp, (ub4) 2, (dvoid *)a_data[0].timestr, 
				  (sb4) 22, SQLT_STR,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp3, 
				  ctxptr->errhp, (ub4) 3, (dvoid *)&(a_data[0].startdata), 
				  (sb4) sizeof(a_data[0].startdata), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));


	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp4, 
				  ctxptr->errhp, (ub4) 4, (dvoid *)&(a_data[0].max), 
				  (sb4) sizeof(a_data[0].max), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp5, 
				  ctxptr->errhp, (ub4) 5, (dvoid *)&(a_data[0].min), 
				  (sb4) sizeof(a_data[0].min), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp6, 
				  ctxptr->errhp, (ub4) 6, (dvoid *)&(a_data[0].avg), 
				  (sb4) sizeof(a_data[0].avg), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	if (OCIBindArrayOfStruct(bndp1,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp2,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp3,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp4,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp5,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp6,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   )
	  {
		printf("FAILED: OCIBindeArrayOfStruct()\n");
		return FALSE;
	  }    

	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) rownum, (ub4)0,
						  (OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT)) == FALSE)
	{
		HdbConn=FALSE; 
	}
	
	checkerr(ctxptr->errhp, OCITransCommit(ctxptr->svchp, ctxptr->errhp, (ub4) 0));

	return(TRUE);
}

#endif

int InsertHdbByJh( Cda_Def cda, int curvetype, int num)
{
	time_t now;
	struct tm curtime;
	int	i,	j=0;
	char	TimeStr[30];
	char	FormatStr[200]="select * from ems.schedule where sche_time ='%s' and source_num =1 and object_num =1 and timetable_num=1";
	char	FormatStr2[200]="insert into ems.schedule(source_num,object_num,sche_power, sche_back,timetable_num,sche_time) values(1,1,0,0,1, '%s')";
	char	UpdateStr[300];
	time(&now);
	curtime=*localtime(&now);
	curtime.tm_mday = curtime.tm_mday +curvetype;
	curtime.tm_hour =0;
	curtime.tm_min =0;
	curtime.tm_sec =0;
	curtime.tm_isdst =-1;
	mktime( &curtime );

	tmtostring(curtime,TimeStr);
	sprintf(UpdateStr,FormatStr, TimeStr);
//	log_write(INF,3,UpdateStr);
	if( oparse(&cda, (unsigned char *)UpdateStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
//		err_write("Oparse Orcal error !\n"); 
		return(FALSE);	
	}
	if (oexec(&cda))
	{
      		OracleError(&cda);
      		return(FALSE);
	}

	while( ofetch(&cda)==0 )
 	{     
		return TRUE;
	}
	
	for(i=0;i<96;i++)
	{
		tmtostring(curtime,TimeStr);
		sprintf(UpdateStr,FormatStr2, TimeStr );
//		log_write(INF,3,UpdateStr);
		/* Parse the PL/SQL block */
		if( oparse(&cda, (unsigned char *)UpdateStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
		{
			OracleError(&cda);
//			err_write("Oparse Orcal error !\n"); 
			return(FALSE);
		}

   		/* Execute the PL/SQL block.*/
		if( oexec(&cda) )
		{
			OracleError(&cda);
//			err_write("Oexec Orcal error !\n"); 
			return(FALSE);  
		}
		DateAdd(&curtime,FIFTEEN_MIN);		
	}
	return TRUE;
}

int UpdateHdbByJh(Cda_Def cda, int curveid, int timeid, float value0, float value1,float value2  )
{	
	char	FormatStr[200]="update ems.schedule set sche_power= %.3f, sche_back= %.3f where source_num =1 and object_num =1 and timetable_num =1 and sche_time= '%s'";
	char	UpdateStr[300];
	int 	i=0;
	time_t now;
	struct tm curtime;
	char	TimeStr[30];
	time(&now);
	curtime=*localtime(&now);
	curtime.tm_mday = curtime.tm_mday +curveid;
	curtime.tm_hour =0;
	curtime.tm_min =0;
	curtime.tm_sec =0;
	curtime.tm_isdst =-1;
	mktime( &curtime );
	for(i=0;i<timeid;i++)
	{
		DateAdd(&curtime,FIFTEEN_MIN);
	}
	tmtostring(curtime,TimeStr);			
	sprintf(UpdateStr,FormatStr, value0, value1, TimeStr );

//	log_write(INF,3,UpdateStr);

	if( oparse(&cda, (unsigned char *)UpdateStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
    	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}
	
	if( oexec(&cda) )
	{
      		OracleError(&cda);
//     		err_write("Oexec Orcal error !\n"); 
      		return(FALSE);  
	}

	return(TRUE);
}


int UpdateHdbByYx(Cda_Def cda, int id, char *DateTime, int ivalue, int istatus )
{
	char	FormatStr[200]="update ems.speciality set datetime= '%s',data= %d, quality_code= %d where id= %d";
	char	UpdateStr[300];
	sprintf(UpdateStr,FormatStr, DateTime, ivalue, (char)istatus, id );
//	log_write(INF,3,UpdateStr);
	
	if( oparse(&cda, (unsigned char *)UpdateStr, (sb4) -1, (sword) TRUE, (ub4) 2) )
    	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}

	if( oexec(&cda) )
	{
      	OracleError(&cda);
//      	err_write("Oexn Orcal error !\n"); return(FALSE); 
//		err_write(UpdateStr);			
	}		
	return(TRUE);
}

int UpdateHdbByYx_A(cocictx *ctxptr, struct yx_seg *p_yx, int rownum)
{

	ub4 valsk1 = (ub4) sizeof(struct yx_seg); /* value skip */
	ub4 indsk = 0; /*  indicator skips */
	ub4 rlsk = 0; /*  return length skips */
	ub4 rcsk = 0; /*  return code skips */
	text *ins_stmt1 = (text *)"update ems.speciality set datetime= :1,data= :2, quality_code= :3 where id= :4";
	OCIBind *bndp1 = (OCIBind *) NULL;
	OCIBind *bndp2 = (OCIBind *) NULL;
	OCIBind *bndp3 = (OCIBind *) NULL;
	OCIBind *bndp4 = (OCIBind *) NULL;
	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
                          ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
                          (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp1, 
				  ctxptr->errhp, (ub4) 1, (dvoid *)p_yx->TimeTag, 
				  (sb4) 22, SQLT_STR,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp2, 
				  ctxptr->errhp, (ub4) 2, (dvoid *)&(p_yx->fvalue), 
				  (sb4) sizeof(p_yx->fvalue), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));


	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp3, 
				  ctxptr->errhp, (ub4) 3, (dvoid *) &(p_yx->quality), 
				  (sb4) sizeof(p_yx->quality), SQLT_INT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp4, 
				  ctxptr->errhp, (ub4) 4, (dvoid *) &(p_yx->id), 
				  (sb4) sizeof(p_yx->id), SQLT_INT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	if (OCIBindArrayOfStruct(bndp1,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp2,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp3,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp4,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   )
	  {
		printf("FAILED: OCIBindeArrayOfStruct()\n");
		return FALSE;
	  }    

	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) rownum, (ub4)0,
						  (OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT)) == FALSE)
	{
		HdbConn=FALSE; 
		return FALSE;
	}
	
	checkerr(ctxptr->errhp, OCITransCommit(ctxptr->svchp, ctxptr->errhp, (ub4) 0));

	return(TRUE);	
}
/**************insert yx in indrec to hdb  abb by dm 2010.10.27***************************/ 
int InsertHdbByYx_A(cocictx *ctxptr, struct yx_seg *p_yx, int rownum)
{

	ub4 valsk1 = (ub4) sizeof(struct yx_seg); /* value skip */
	ub4 indsk = 0; /*  indicator skips */
	ub4 rlsk = 0; /*  return length skips */
	ub4 rcsk = 0; /*  return code skips */
	text *ins_stmt1 = (text *)"insert into ems.indrec( id, datetime, millisecond, status_id, quality_code, data, typeid_id)  values(:1, :2, :3, :4, 0, -1, 4)";
	OCIBind *bndp[4];
	int i;
	for(i=0;i<4;i++)
	{
		  bndp[i] = (OCIBind *) NULL;
	}
	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
		ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
		(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[0], 
		ctxptr->errhp, (ub4) 1, (dvoid *)&(p_yx->id), 
		(sb4) sizeof(p_yx->id), SQLT_INT,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[1], 
		ctxptr->errhp, (ub4) 2, (dvoid *)p_yx->TimeTag, 
		22, SQLT_STR,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[2], 
		ctxptr->errhp, (ub4) 3, (dvoid *) &(p_yx->ms), 
		(sb4) sizeof(p_yx->ms), SQLT_INT,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[3], 
		ctxptr->errhp, (ub4) 4, (dvoid *) &(p_yx->status_id), 
		(sb4) sizeof(p_yx->status_id), SQLT_INT,
		(dvoid *) 0, (ub2 *)0, (ub2 *)0,
		(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	
	if (OCIBindArrayOfStruct(bndp[0],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		|| OCIBindArrayOfStruct(bndp[1],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		|| OCIBindArrayOfStruct(bndp[2],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		|| OCIBindArrayOfStruct(bndp[3],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
		)
	{
		printf("FAILED: OCIBindArrayOfStruct()\n");
		return FALSE;
	}    
	
	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) rownum, (ub4)0,
						  (OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT)) == FALSE)
	{
		HdbConn=FALSE; 
		return FALSE;
	}
	
	checkerr(ctxptr->errhp, OCITransCommit(ctxptr->svchp, ctxptr->errhp, (ub4) 0));
	
	return(TRUE);	
}

void OracleError(Lda_Def * lda)
{  	
	char msgbuf[512];
  	int n=oerhms(lda, lda->rc, (unsigned char *)msgbuf, (int) sizeof(msgbuf) );
  	sprintf(log_str,"%.*s\n",n,msgbuf);
	err_write(log_str);
}

int ConnectHDB(Lda_Def * lda_c,Cda_Def *cda, char *MyStr )
{	
	static 	ub4 hstb[512]; 	
   	char LoginString[100];
	strcpy(LoginString,MyStr);
	/* Connect to Oracle */   	
	
	if (olog(lda_c, (ub1 *)hstb, (unsigned char *)LoginString, -1, (unsigned char *)0, -1,(unsigned char *) 0, -1, (ub4)OCI_LM_DEF))
	{	
//		sprintf(log_str,"Unable to connect as %s\n", MyStr); 
//		err_write(log_str);
		return(FALSE);	
	}
 	/* Open a cursor for executing the PL/SQL block. */
  	if (oopen(cda, lda_c, (text *) 0, -1, 0, (unsigned char *) 0, -1))
  	{	OracleError(cda); return(FALSE);	}
	
//	sprintf(log_str," Connect hdb as %s sucessfully!\n", MyStr); 
//	log_write(INF,2,log_str);
  	if (ocon(lda_c))
  	{	OracleError(lda_c); return(FALSE);	 } 
  	return(TRUE);
}

int DisconnectHDB(Lda_Def * lda,Cda_Def *cda)
{
	/*Close the cursor on which the PL/SQL block executed.*/
  	if (oclose(cda))
  	{	
		OracleError(cda); 
		return(FALSE);	
	} 
     	/* Log off. */
	if (ologof(lda))
  	{	
		OracleError(cda); 
		return(FALSE);    	
	}
    MySleep(10000);
 			
  	return(TRUE);
}




int ConnectHDB_B(cocictx *ctxptr,char *srvname,char *username,char *password)
{
	if (OCIEnvCreate((OCIEnv **) &ctxptr->envhp,
				   (ub4)OCI_THREADED|OCI_OBJECT, (dvoid *)0,
					(dvoid * (*)(dvoid *, size_t)) 0,
				   (dvoid * (*)(dvoid *, dvoid *, size_t))0,
				   (void (*)(dvoid *, dvoid *)) 0,
				   (size_t) 0, (dvoid **) 0 ))
	printf("FAILED: OCIEnvCreate()\n");


	printf("\nConnect to server\n");

	if (OCIHandleAlloc((dvoid *) ctxptr->envhp,
					 (dvoid **) &ctxptr->errhp,
					 (ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0))
	printf("FAILED: OCIHandleAlloc() on ctxptr->errhp\n");

	if (OCIHandleAlloc((dvoid *) ctxptr->envhp,
					 (dvoid **) &ctxptr->srvhp,
					 (ub4) OCI_HTYPE_SERVER, (size_t) 0, (dvoid **) 0))
	printf("FAILED: OCIHandleAlloc() on ctxptr->srvhp\n");

	if (OCIHandleAlloc((dvoid *) ctxptr->envhp,
					 (dvoid **) &ctxptr->svchp,
					 (ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0))
	printf("FAILED: OCIHandleAlloc() on ctxptr->svchp\n");

	if (OCIHandleAlloc((dvoid *) ctxptr->envhp,
					 (dvoid **) &ctxptr->authp,
					 (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0))
	printf("FAILED: OCIHandleAlloc() on ctxptr->authp\n");

	if (OCIServerAttach(ctxptr->srvhp, ctxptr->errhp,
					  (text *) srvname, (sb4) strlen(srvname),
					  (ub4) OCI_DEFAULT))
	printf("FAILED: OCIServerAttach()\n");

	if (OCIAttrSet((dvoid *) ctxptr->svchp, (ub4) OCI_HTYPE_SVCCTX,
				 (dvoid *) ctxptr->srvhp, (ub4) 0,
				 (ub4) OCI_ATTR_SERVER, ctxptr->errhp))
	printf("FAILED: OCIAttrSet() server attribute\n");

	/*begin log_on part */
	if (OCIAttrSet((dvoid *) ctxptr->authp, (ub4) OCI_HTYPE_SESSION,
				 (dvoid *) username, (ub4) strlen((char *) username),
				 (ub4) OCI_ATTR_USERNAME, ctxptr->errhp))
	printf("FAILED: OCIAttrSet() userid\n");

	if (OCIAttrSet((dvoid *) ctxptr->authp, (ub4) OCI_HTYPE_SESSION,
				 (dvoid *) password, (ub4) strlen((char *) password),
				 (ub4) OCI_ATTR_PASSWORD, ctxptr->errhp))
	printf("FAILED: OCIAttrSet() passwd\n");

	printf("Logging on as %s  ....\n", username);

	if(checkerr(ctxptr->errhp, OCISessionBegin((dvoid *)ctxptr->svchp,
						ctxptr->errhp, ctxptr->authp,
					   (ub4) OCI_CRED_RDBMS,(ub4) OCI_DEFAULT )) == FALSE)
	{
		  printf("%s logged failed.\n", username);
		  return FALSE;
	} 
	printf("%s logged on.\n", username);
             
	if (OCIAttrSet((dvoid *) ctxptr->svchp, (ub4) OCI_HTYPE_SVCCTX,
			 (dvoid *) ctxptr->authp, (ub4) 0, (ub4) OCI_ATTR_SESSION, 
			 ctxptr->errhp))
	printf("FAILED: OCIAttrSet() session\n");
	/* end log_on part */

	/* alocate stmt handle for sql queries */

	if (OCIHandleAlloc((dvoid *)ctxptr->envhp, (dvoid **) &ctxptr->stmthp,
				   (ub4)OCI_HTYPE_STMT, (CONST size_t) 0, (dvoid **) 0))
	printf("FAILED: alloc statement handle\n");

	return TRUE;
} /* end initialize() */


/*clean up envionment*/
int DisconnectHDB_B(cocictx *ctxptr)
{
	printf("\n ########## clean up ############ \n");

	if (OCISessionEnd(ctxptr->svchp, ctxptr->errhp, 
					  ctxptr->authp, (ub4) 0))
	printf("FAILED: OCISessionEnd()\n");

	printf("Logged off.\n");

	if (OCIServerDetach(ctxptr->srvhp, ctxptr->errhp,
				   (ub4) OCI_DEFAULT))
	printf("FAILED: OCIServerDetach()\n");

	printf("Detached from server.\n");

	printf("Freeing handles ...\n");
	if (ctxptr->stmthp)
	OCIHandleFree((dvoid *) ctxptr->stmthp, (ub4) OCI_HTYPE_STMT);
	if (ctxptr->errhp)
	OCIHandleFree((dvoid *) ctxptr->errhp, (ub4) OCI_HTYPE_ERROR);   
	if (ctxptr->srvhp)
	OCIHandleFree((dvoid *) ctxptr->srvhp, (ub4) OCI_HTYPE_SERVER);
	if (ctxptr->svchp)
	OCIHandleFree((dvoid *) ctxptr->svchp, (ub4) OCI_HTYPE_SVCCTX);
	if (ctxptr->authp)
	OCIHandleFree((dvoid *) ctxptr->authp, (ub4) OCI_HTYPE_SESSION);
	if (ctxptr->envhp)
	OCIHandleFree((dvoid *) ctxptr->envhp, (ub4) OCI_HTYPE_ENV);
	
	return TRUE;
} /* end cleanup() */

/*check status and print error information*/
int checkerr(OCIError *errhp,sword status)
{
	text errbuf[512];
	sb4 errcode = 0;

	switch (status)
	{
	case OCI_SUCCESS:
	break;
	case OCI_SUCCESS_WITH_INFO:
//	(void) err_write("Error - OCI_SUCCESS_WITH_INFO\n");
	break;
	case OCI_NEED_DATA:
//	(void) err_write("Error - OCI_NEED_DATA\n");
	break;
	case OCI_NO_DATA:
//	(void) err_write("Error - OCI_NODATA\n");
	break;
	case OCI_ERROR:
	(void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
						errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
//		sprintf(log_str,"Error - %.*s\n", 512, errbuf);
//		err_write(log_str);
	return FALSE;
	break;
	case OCI_INVALID_HANDLE:
//		sprintf(log_str,"Error - OCI_INVALID_HANDLE\n");
//		err_write(log_str);
		return FALSE;
	break;
	case OCI_STILL_EXECUTING:
//	(void) err_write("Error - OCI_STILL_EXECUTE\n");
	break;
	case OCI_CONTINUE:
//	(void) err_write("Error - OCI_CONTINUE\n");
	break;
	default:
	break;
	}
	return TRUE;
} /* end checkerr() */


/*perform array insert*/
int curve_array_insert(cocictx *ctxptr,int hdb_pt_num)
{ 
  char timestr[40];
  text *ins_stmt1 = (text *)("INSERT INTO ems.second VALUES (:1, :2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12,:13,:14,:15,:16, :17,:18,:19,:20,:21,:22,:23,:24,:25,:26,:27,:28,:29,:30,:31, :32,:33,:34,:35,:36,:37,:38,:39,:40,:41,:42,:43,:44,:45,:46, :47,:48,:49,:50,:51,:52,:53,:54,:55,:56,:57,:58,:59,:60,:61,:62)");
  time_t  tmptime ;
  struct tm   *lcday;

  ub4 valsk1 = (ub4) sizeof(SECDATA); /* value skip */
  ub4 indsk = 0; /*  indicator skips */
  ub4 rlsk = 0; /*  return length skips */
  ub4 rcsk = 0; /*  return code skips */
  OCIBind *bndp[60];
  int i;
  for(i=0;i<62;i++)
  {
	bndp[i] = (OCIBind *) NULL;
  }

  tmptime=time(NULL);
  lcday=localtime(&tmptime);

  sprintf(timestr,"%d%d%d%d%d%d%d\n",(lcday->tm_year +1900) /100 +100,
	                                 (lcday->tm_year+1900) %100 +100,
									 lcday->tm_mon+1,
									 lcday->tm_mday,
									 lcday->tm_hour+1,
									 lcday->tm_min+1,
									 lcday->tm_sec+1);
  checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
                          ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
                          (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

/*
　hBind = NULL; 
　　swResult = OCIBindByPos(stmtp &hBind, errhp,1,tstd[0].tname, 
　　sizeof(tstd[0].tname), SQLT_CHR, &tstdInd.sb2_usernmae[0], 
　　&tstdLen.ub2_username[0],&tstdRet.ub2_username[0],100, &rec_num, OCI_DEFAULT); 
　　swResult = OCIBindArrayOfStruct(hBind, errhp,sizeof(tstd [0]), sizeof(sb2), sizeof(ub2), sizeof(ub2)); 

  OCIStmt       *stmtp, //语句句柄 
                OCIBind       **bindpp,//结合句柄,=NULL 
                OCIError      *errhp, 
                CONST text    *placeholder,//占位符名称 
                sb4           placeh_len, //占位符长度 
                dvoid         *valuep, //绑定的变量名 
                sb4           value_sz, //绑定的变量名长度 
                ub2           dty,  //绑定的类型 
                dvoid         *indp, //指示符变量指针(sb2类型),单条绑定时为NULL, 
                ub2           *alenp, //说明执行前后被结合的数组变量中各元素数据实际的长度，单条绑定时为NULL 
                ub2           *rcodep,//列级返回码数据指针，单条绑定时为NULL 
                ub4           maxarr_len, //最多的记录数,如果是单条绑定，则为0 
                ub4           *curelep, //实际的记录数,单条绑定则为NULL 
                ub4           mode //=OCI_DEFAULT 

  */
    checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &(bndp[0]), 
                  ctxptr->errhp, (ub4) 1, (dvoid *) &data_all[0].id, 
                  (sb4) sizeof(data_all[0].id), SQLT_INT,
                  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
                  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	/*
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[1], 
                  ctxptr->errhp, (ub4) 2, (dvoid *)data_all[0].datetime, 
                  (sb4) sizeof(data_all[0].datetime), SQLT_DAT,
                  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
                  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	*/
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[1], 
				  ctxptr->errhp, (ub4) 2, (dvoid *)data_all[0].datetime, 
				  (sb4) 22, SQLT_STR,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	for(i=0;i<60;i++)
	{	
		 checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp[i+2], 
						  ctxptr->errhp, (ub4) i+3, (dvoid *)&(data_all[0].data[i]), 
						  (sb4) sizeof(data_all[0].data[0]), SQLT_FLT,
						  (dvoid *) sec_ischg[i], (ub2 *)0, (ub2 *)0,
						  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	}

	for(i=0;i<62;i++)
	{
		if (OCIBindArrayOfStruct(bndp[i],ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2)) != 0)
		{
			    printf("FAILED: OCIBindeArrayOfStruct()\n");
				return FALSE;
		}
	}

	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) hdb_pt_num, (ub4)0,
						  (OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT)) == FALSE)
	{
	  HdbConn=FALSE; 
	}
  
	checkerr(ctxptr->errhp, OCITransCommit(ctxptr->svchp, ctxptr->errhp, (ub4) 0));

	return TRUE;
} 

int ExecuteSQL(Cda_Def cda, char *cmdstr )
{	
//	log_write(INF,3,cmdstr);
	if( oparse(&cda, (unsigned char *)cmdstr, (sb4) -1, (sword) TRUE, (ub4) 2) )
    	{
        	OracleError(&cda);
//        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}
   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      	OracleError(&cda);
//      	err_write("Oexec Orcal error !\n"); 
		return(FALSE);  
	}
	return(TRUE);
}

