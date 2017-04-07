/************************************************************************************************************************
FILENAME:	ots_hdb.cpp

FUNCTION:	
	
	some functions of ots hdb library,include:

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2010.4.21	create the file for qt hdb	on oracle	yyp

*************************************************************************************************************************/

//#include "/home/ems/system/lan/v4.0a/inc/lan.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"


DATA_USERINFO	data_userinfo[MAX_ROW_NUM],g_userinfo[MAX_ROW_NUM];
DATA_TRAIN_INFO	data_train_info[MAX_ROW_NUM];
DATA_LOGIN_INFO	data_login_info[MAX_ROW_NUM_LARGE];
DATA_RECORD		data_record[MAX_ROW_NUM_LARGE];
DATA_SCENE		data_scene[MAX_ROW_NUM];
DATA_SCENE_DATA	data_scene_data[MAX_ROW_NUM_HUGE];
DATA_REVERT		data_revert[MAX_ROW_NUM];
DATA_FAULT		data_fault[MAX_ROW_NUM];
DATA_FAULT_DATA	data_fault_data[MAX_ROW_NUM];
DATA_TASK_GRP	data_task_grp[MAX_ROW_NUM];
DATA_TASK		data_task[MAX_ROW_NUM];
DATA_SEND_TASK	data_send_task[MAX_ROW_NUM];
DATA_HOST		data_host[MAX_ROW_NUM],g_host[MAX_OTS_HOST_NUM];
DATA_OPR_TYPE	data_opr_type[MAX_ROW_NUM];
DATA_LOCAL_OPR	data_local_opr[MAX_ROW_NUM];
DATA_RUN_MODEL	data_run_model[MAX_ROW_NUM];
DATA_RUN_SEQUENCE	data_run_sequence[MAX_ROW_NUM];
DATA_MODEL		data_model[MAX_ROW_NUM];
DATA_SEQUENCE	data_sequence[MAX_ROW_NUM];
DATA_VAR_RELATION	data_var_relation[MAX_ROW_NUM];
DATA_SUBJECT	data_subject[MAX_ROW_NUM];
DATA_SCORE		data_score[MAX_ROW_NUM];
DATA_GROUPS		data_groups[MAX_ROW_NUM];
DATA_GROUPS_RECORD	data_groups_record[MAX_ROW_NUM];
DB_FMT			db_data;
char table_name[MAX_TABLE_NUM][TABLE_NAME_SIZE];
char field_name[MAX_FIELD_NUM][FIELD_NAME_SIZE];
char field_value[MAX_FIELD_NUM][VALUE_STR_SIZE];
char field_table[MAX_FIELD_NUM][TABLE_NAME_SIZE];
unsigned char field_type[MAX_FIELD_NUM];
char union_table[MAX_FIELD_NUM][TABLE_NAME_SIZE];
short field_id[MAX_FIELD_NUM];
char cond_name[MAX_COND_NUM][FIELD_NAME_SIZE];
char cond_value[MAX_COND_NUM][VALUE_STR_SIZE];
char cond_opr[MAX_COND_NUM][OPR_SYMBOL_SIZE];
char filter_name[MAX_FILTER_NUM][FIELD_NAME_SIZE];

DATA_STATION	data_station[MAX_STN_NUM+1];
DATA_DEV	data_dev[MAX_STN_NUM+1][MAX_LCU_NUM+1];
DATA_POINT data_point[MAX_PT_NUM];

char	hdb_type[10]="ORACLE";
char	server_ip[20]="127.0.0.1";
char	server_sid[10]="HISTA";
char	username[]="ots";
char	password[]="ots";
int		server_port=1521;
QSqlDatabase db_conn;
int field_num=0,cond_num=0,row_id=0,table_num=1;
int	g_host_num=0,g_user_num=0,g_host_id=0,g_user_id=0,g_groups_id=0,g_groups_role=0;
char g_host_name[20];
char g_rec_str[MAX_RECORD_SIZE];

int	SelectRecs(DB_FMT *p_data,QSqlDatabase db)
{
	int ret;
	QString q_str;
	QDateTime dtime;
	int i=0,j=0;
	char sql_str[4000]="";
	char field_str[2000]="";
	char cond_str[500]="";
	char table_str[200]="";
	char tmp_str[2000]="";
	field_num=0;cond_num=0;row_id=0;table_num=1;
	InitTableData(p_data->table_id);
	p_data->opr_mode = DB_SELECT;
	QSqlQuery db_opr("",db);	
	memset(&p_data->flag,1,sizeof(p_data->flag));
	CvtFieldData(p_data,0);

	for(i=0;i<table_num;i++)
	{
		if(strlen(union_table[i]) < 1 )
		{
			strcpy(union_table[i],table_name[p_data->table_id]);
		}
		sprintf(tmp_str,"%s",union_table[i]);
		strcat(table_str,tmp_str);
		if(i != table_num-1)
		{
			strcat(table_str,",");
		}
	}
		
	for(i=0;i<field_num;i++)
	{
		if( table_num == 1 )
		{
			sprintf(tmp_str,"%s",field_name[i]);
		}
		else
		{
			if(strlen(field_table[i]) < 1 )
			{
				strcpy(field_table[i],table_name[p_data->table_id]);
			}			
			sprintf(tmp_str,"%s.%s",field_table[i],field_name[i]);			
		}
		strcat(field_str,tmp_str);
		if(i != field_num-1)
		{
			strcat(field_str,",");
		}
	}

	for(i=0;i<cond_num;i++)
	{
		if(i==0)
		{
			strcpy(cond_str," where ");
		}
		/*if(table_num == 1)
		{
			sprintf(tmp_str,"%s = %s",cond_name[i],cond_value[i]);
		}
		else
		{
			if(strlen(cond_opr[i]) < 1 )
			{
				strcpy(cond_opr[i],"=");
			}			

			sprintf(tmp_str,"%s.%s %s %s",table_name[p_data->table_id],cond_name[i],cond_opr[i],cond_value[i]);
		}*/
		if(strlen(cond_opr[i]) < 1 )
		{
			strcpy(cond_opr[i],"=");
		}			

		sprintf(tmp_str,"%s.%s %s %s",table_name[p_data->table_id],cond_name[i],cond_opr[i],cond_value[i]);
		strcat(cond_str,tmp_str);
		if(i != cond_num-1)
		{
			strcat(cond_str," and ");
		}
	}
	sprintf(sql_str,"select %s from %s %s %s",field_str,table_str,cond_str,p_data->addon_str);
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	strcpy(p_data->sql_error,db_opr.lastError().text().toLocal8Bit().data());
	if(ret != TRUE)
	{
		return ret;
	}
	row_id=0;
	p_data->rec_num = db_opr.size();
	while(db_opr.next())
	{
		for(j=0;j<field_num;j++)
		{
			if(field_type[j]!= TYPE_TIME)
			{
				q_str = db_opr.value(j).toString();
				strcpy(field_value[j],q_str.toLocal8Bit().data());
				CvtArrayToStr(field_value[j],strlen(field_value[j]));
			}
			else
			{
				if(db_opr.value(j).isNull()!=TRUE)
				{
					dtime = db_opr.value(j).toDateTime();
					CvtQDateTimeToStr(field_value[j],dtime);
				}
				else
				{
					strcpy(field_value[j],"");
				}
			}
#ifdef DEBUG
			printf("%s	",field_value[j]);
			if(j==field_num-1)
			{
				printf("\n");
			}
#endif
		}//end for

		CvtFieldData(p_data,1);
		row_id++;
		
		if( p_data->table_id == TABLE_RECORD || p_data->table_id == TABLE_LOGIN_INFO )
		{
			if(row_id>=MAX_ROW_NUM_LARGE)
			{
				break;
			}
		}
		else if( p_data->table_id == TABLE_SCENE_DATA )
		{
			if(row_id>=MAX_ROW_NUM_HUGE)
			{
				break;
			}
		}
		else
		{
			if(row_id>=MAX_ROW_NUM)
			{
				break;
			}
		}
		
	}//end while
	strcpy(p_data->sql_str,sql_str);
	p_data->rownum = row_id;
	return ret;
	
}

int		InsertOneRec(DB_FMT *p_data,QSqlDatabase db)
{
	int ret;
	int i=0;
	char sql_str[2000]="";
	char field_str[500]="";
	char value_str[1000]="";
	char cond_str[200]="";
	char tmp_str[200]="";
	QSqlQuery db_opr("",db);	
	p_data->opr_mode = DB_INSERT;	
	memset(&p_data->cond_flag,0,sizeof(p_data->cond_flag));	
	CvtFieldData(p_data,0);

	for(i=0;i<field_num;i++)
	{
		strcat(field_str,field_name[i]);
		strcat(value_str,field_value[i]);
		if(i != field_num-1)
		{
			strcat(field_str,",");
			strcat(value_str,",");
		}
	}
	sprintf(sql_str,"insert into ots.%s( %s ) values( %s )",table_name[p_data->table_id],field_str,value_str);
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif
	strcpy(p_data->sql_str,sql_str);
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	strcpy(p_data->sql_error,db_opr.lastError().text().toLocal8Bit().data());
	return ret;
}

int		UpdateOneRec(DB_FMT *p_data,QSqlDatabase db)
{
	int ret;
	int i=0,j=0;
	char sql_str[2000]="";
	char field_str[1000]="";
	char cond_str[200]="";
	char tmp_str[600]="";
	QSqlQuery db_opr("",db);
	p_data->opr_mode = DB_UPDATE;		
	CvtFieldData(p_data,0);	
	
	for(i=0;i<field_num;i++)
	{
		sprintf(tmp_str,"%s = %s",field_name[i],field_value[i]);
		strcat(field_str,tmp_str);
		if(i != field_num-1)
		{
			strcat(field_str,",");
		}
	}
	for(i=0;i<cond_num;i++)
	{
		if(i==0)
		{
			strcpy(cond_str," where ");
		}
		sprintf(tmp_str,"%s = %s",cond_name[i],cond_value[i]);
		strcat(cond_str,tmp_str);
		if(i != cond_num-1)
		{
			strcat(cond_str," and ");
		}
	}
	sprintf(sql_str,"select * from ots.%s %s ",table_name[p_data->table_id],cond_str);	

	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	strcpy(p_data->sql_error,db_opr.lastError().text().toLocal8Bit().data());
	if(ret != TRUE)
	{
		return ret;
	}
	row_id=0;
	while( db_opr.next() )
	{
		row_id++;	
	}
	p_data->ret_code = 0;
	if( row_id <= 0 )
	{
		p_data->ret_code = 1;
		return FALSE;
	}
	sprintf(sql_str,"update ots.%s set %s %s ",table_name[p_data->table_id],field_str,cond_str);
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	strcpy(p_data->sql_error,db_opr.lastError().text().toLocal8Bit().data());
	strcpy(p_data->sql_str,sql_str);
	return ret;
}

int	DeleteOneRec(DB_FMT *p_data,QSqlDatabase db)
{
	int ret;
	int i=0,j=0;
	char sql_str[2000]="";
	char field_str[500]="";
	char cond_str[200]="";
	char tmp_str[200]="";
	QSqlQuery db_opr("",db);	
	p_data->opr_mode = DB_DELETE;		
	memset(&p_data->flag,0,sizeof(p_data->flag));		
	CvtFieldData(p_data,0);	

	for(i=0;i<cond_num;i++)
	{
		if(i==0)
		{
			strcpy(cond_str," where ");
		}
		if(strlen(cond_opr[i]) < 1 )
		{
			strcpy(cond_opr[i],"=");
		}
		sprintf(tmp_str,"%s %s %s",cond_name[i],cond_opr[i],cond_value[i]);
		strcat(cond_str,tmp_str);
		if(i != cond_num-1)
		{
			strcat(cond_str," and ");
		}
	}
	sprintf(sql_str,"delete from ots.%s %s ",table_name[p_data->table_id],cond_str);
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	strcpy(p_data->sql_error,db_opr.lastError().text().toLocal8Bit().data());
	strcpy(p_data->sql_str,sql_str);
	return ret;
}

int		SelectPtHdb(int mode)
{
	int ret;
	QString q_str;
	int i=0,j=0;
	char sql_str[2000]="";
	int stn_id,dev_id,stn_id_old,dev_id_old;
	int total_dev=0,total_pt=0;
	char timestr[22];
	DATA_POINT pt_data;
	QSqlQuery db_opr("",db_conn);

	g_stn_num=0;
	memset(g_dev_num,0,(MAX_STN_NUM+1) * sizeof(int) );	
	memset(data_station,0,(MAX_STN_NUM+1) * sizeof(DATA_STATION) );
	memset(data_dev,0,(MAX_STN_NUM+1) * (MAX_LCU_NUM+1) * sizeof(DATA_DEV) );
	memset(data_point,0,(MAX_PT_NUM) * sizeof(DATA_POINT) );

	strcpy(sql_str,"select station_id,station_name,station_comment from station order by station_id");
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	if(ret != TRUE)
	{
		return ret;
	}
	row_id=0;
	while( db_opr.next() )
	{
		stn_id=db_opr.value(0).toInt();
		data_station[stn_id].station_id = db_opr.value(0).toInt();
		q_str = db_opr.value(1).toString();
		strcpy(data_station[stn_id].station_name,q_str.toLocal8Bit().data());
		CvtArrayToStr(data_station[stn_id].station_name,strlen(data_station[stn_id].station_name));
		q_str = db_opr.value(2).toString();
		strcpy(data_station[stn_id].station_longname,q_str.toLocal8Bit().data());
		CvtArrayToStr(data_station[stn_id].station_longname,strlen(data_station[stn_id].station_longname));		
		row_id++;
	}
	g_stn_num=row_id;

	strcpy(sql_str,"select station_id,dev_id,dev_type,dev_name,dev_comment from dev order by station_id,dev_id");
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	if(ret != TRUE)
	{
		return ret;
	}
	row_id=0;
	while( db_opr.next() )
	{
		stn_id=db_opr.value(0).toInt();
		dev_id=db_opr.value(1).toInt();
		data_dev[stn_id][dev_id].station_id = db_opr.value(0).toInt();
		data_dev[stn_id][dev_id].dev_id = db_opr.value(1).toInt();
		q_str = db_opr.value(2).toString();
		strcpy(data_dev[stn_id][dev_id].dev_type,q_str.toLocal8Bit().data());
		CvtArrayToStr(data_dev[stn_id][dev_id].dev_type,strlen(data_dev[stn_id][dev_id].dev_type));

		q_str = db_opr.value(3).toString();
		strcpy(data_dev[stn_id][dev_id].dev_name,q_str.toLocal8Bit().data());
		CvtArrayToStr(data_dev[stn_id][dev_id].dev_name,strlen(data_dev[stn_id][dev_id].dev_name));

		q_str = db_opr.value(4).toString();
		strcpy(data_dev[stn_id][dev_id].dev_longname,q_str.toLocal8Bit().data());
		CvtArrayToStr(data_dev[stn_id][dev_id].dev_longname,strlen(data_dev[stn_id][dev_id].dev_longname));
		data_station[stn_id].dev_num ++ ;
		g_dev_num[stn_id] ++;
		row_id++;
	}
	total_dev = row_id;
	if(mode == 1)
	{
		GetCurTimeStr(timestr);
		printf("\n\nStart to Get hdb pt info, time=%s \n\n",timestr);
		strcpy(sql_str,"select station_id,dev_id,type_id,point_id,id,logicalname,longname,data,quality_code from speciality order by id");
//		strcpy(sql_str,"select station_id,dev_id,type_id,point_id,id,logicalname,longname,data,quality_code from speciality where dev_id=1 order by id");
#ifdef DEBUG
		printf("sql = %s\n",sql_str );
#endif	
		ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
		if(ret != TRUE)
		{
			return ret;
		}
		row_id=0;
		dev_id_old=1;
		stn_id_old = 1;
		data_dev[1][1].pt_start = 0;
		while( db_opr.next() )
		{
			stn_id=db_opr.value(0).toInt();
			dev_id=db_opr.value(1).toInt();
			pt_data.station_id = db_opr.value(0).toInt();
			pt_data.dev_id = db_opr.value(1).toInt();
			pt_data.type_id = db_opr.value(2).toInt();
			pt_data.point_id = db_opr.value(3).toInt();
			pt_data.id  = db_opr.value(4).toInt();
			
			/*
			q_str = db_opr.value(5).toString();
			strcpy(pt_data.logicalname,q_str.toLocal8Bit().data());
			CvtArrayToStr(pt_data.logicalname,strlen(pt_data.logicalname));

			q_str = db_opr.value(6).toString();
			strcpy(pt_data.longname,q_str.toLocal8Bit().data());
			CvtArrayToStr(pt_data.longname,strlen(pt_data.longname));
			*/
			pt_data.data  = db_opr.value(7).toDouble();
			pt_data.quality_code  = db_opr.value(8).toInt();
			data_point[row_id] = pt_data;
			if(dev_id != dev_id_old)
			{
				data_dev[stn_id][dev_id_old].pt_end = row_id-1;
				data_dev[stn_id][dev_id].pt_start = row_id;
				dev_id_old = dev_id;
			}
			row_id++;
		}
		if(row_id >= 1)
		{
			data_dev[stn_id][dev_id].pt_end = row_id-1;
		}
		total_pt=row_id;
		GetCurTimeStr(timestr);
		printf("\n\nEnd Get hdb pt info, time=%s \n\n",timestr);
	}
	printf(" SelectPtHdb() success. stn_num=%d, dev_num=%d, pt_num=%d \n",g_stn_num,total_dev,total_pt);
	return TRUE;	
}


void InitDbData(DB_FMT *p_data,unsigned char table_id)
{
	memset(p_data,0,sizeof(DB_FMT));
// 	switch(table_id)
// 	{
// 		case	TABLE_USERINFO:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_userinfo[i] = new DATA_USERINFO;
// 			break;
// 		case	TABLE_TRAIN_INFO:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_train_info[i] = new DATA_TRAIN_INFO;
// 			break;
// 		case	TABLE_LOGIN_INFO:
// 			for (int i=0;i<MAX_ROW_NUM_LARGE;i++)
// 				p_data->rec.p_login_info[i] = new DATA_LOGIN_INFO;
// 			break;
// 		case	TABLE_RECORD:
// 			for (int i=0;i<MAX_ROW_NUM_LARGE;i++)
// 				p_data->rec.p_record[i] = new DATA_RECORD;
// 			break;
// 		case	TABLE_SCENE:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_scene[i] = new DATA_SCENE;
// 			break;
// 		case	TABLE_SCENE_DATA:
// 			for (int i=0;i<MAX_ROW_NUM_HUGE;i++)
// 				p_data->rec.p_scene_data[i] = new DATA_SCENE_DATA;
// 			break;
// 		case	TABLE_REVERT:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_revert[i] = new DATA_REVERT;
// 			break;
// 		case	TABLE_FAULT:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_fault[i] = new DATA_FAULT;
// 			break;
// 		case	TABLE_FAULT_DATA:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_fault_data[i] = new DATA_FAULT_DATA;
// 			break;
// 		case	TABLE_TASK_GRP:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_task_grp[i] = new DATA_TASK_GRP;
// 			break;
// 		case	TABLE_TASK:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_task[i] = new DATA_TASK;
// 			break;
// 		case	TABLE_SEND_TASK:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_send_task[i] = new DATA_SEND_TASK;
// 			break;
// 		case	TABLE_HOST:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_host[i] = new DATA_HOST;
// 			break;
// 		case	TABLE_OPR_TYPE:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_opr_type[i] = new DATA_OPR_TYPE;
// 			break;
// 		case	TABLE_LOCAL_OPR:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_local_opr[i] = new DATA_LOCAL_OPR;
// 			break;
// 		case	TABLE_RUN_MODEL:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_run_model[i] = new DATA_RUN_MODEL;
// 			break;
// 		case	TABLE_RUN_SEQUENCE:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_run_sequence[i] = new DATA_RUN_SEQUENCE;
// 			break;
// 		case	TABLE_MODEL:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_model[i] = new DATA_MODEL;
// 			break;
// 		case	TABLE_SEQUENCE:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_sequence[i] = new DATA_SEQUENCE;
// 			break;
// 		case	TABLE_VAR_RELATION:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_var_relation[i] = new DATA_VAR_RELATION;
// 			break;
// 		case	TABLE_SUBJECT:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_subject[i] = new DATA_SUBJECT;
// 			break;
// 		case	TABLE_SCORE:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_score[i] = new DATA_SCORE;
// 			break;
// 		case	TABLE_GROUPS:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_groups[i] = new DATA_GROUPS;
// 			break;
// 		case	TABLE_GROUPS_RECORD:
// 			for (int i=0;i<MAX_ROW_NUM;i++)
// 				p_data->rec.p_groups_record[i] = new DATA_GROUPS_RECORD;
// 			break;
// 	}
	strcpy(p_data->addon_str,"");
	p_data->table_id=table_id;
}
void FreeDbData(DB_FMT *p_data,unsigned char table_id)
{
// 	switch(table_id)
// 	{
// 	case	TABLE_USERINFO:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_userinfo[i];
// 			p_data->rec.p_userinfo[i] = NULL;	
// 		}
// 		break;
// 	case	TABLE_TRAIN_INFO:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_train_info[row_id];
// 			p_data->rec.p_train_info[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_LOGIN_INFO:
// 		for (int i=0;i<MAX_ROW_NUM_LARGE;i++)
// 		{
// 			delete p_data->rec.p_login_info[row_id];
// 			p_data->rec.p_login_info[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_RECORD:
// 		for (int i=0;i<MAX_ROW_NUM_LARGE;i++)
// 		{
// 			delete p_data->rec.p_record[row_id];
// 			p_data->rec.p_record[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_SCENE:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_scene[row_id];
// 			p_data->rec.p_scene[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_SCENE_DATA:
// 		for (int i=0;i<MAX_ROW_NUM_HUGE;i++)
// 		{
// 			delete p_data->rec.p_scene_data[row_id];
// 			p_data->rec.p_scene_data[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_REVERT:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_revert[row_id];
// 			p_data->rec.p_revert[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_FAULT:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_fault[row_id];
// 			p_data->rec.p_fault[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_FAULT_DATA:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_fault_data[row_id];
// 			p_data->rec.p_fault_data[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_TASK_GRP:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_task_grp[row_id];
// 			p_data->rec.p_task_grp[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_TASK:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_task[row_id];
// 			p_data->rec.p_task[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_SEND_TASK:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_send_task[row_id];
// 			p_data->rec.p_send_task[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_HOST:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_host[row_id];
// 			p_data->rec.p_host[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_OPR_TYPE:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_opr_type[row_id];
// 			p_data->rec.p_opr_type[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_LOCAL_OPR:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_local_opr[row_id];
// 			p_data->rec.p_local_opr[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_RUN_MODEL:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_run_model[row_id];
// 			p_data->rec.p_run_model[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_RUN_SEQUENCE:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_run_sequence[row_id];
// 			p_data->rec.p_run_sequence[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_MODEL:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_model[row_id];
// 			p_data->rec.p_model[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_SEQUENCE:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_sequence[row_id];
// 			p_data->rec.p_sequence[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_VAR_RELATION:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_var_relation[row_id];
// 			p_data->rec.p_var_relation[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_SUBJECT:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_subject[row_id];
// 			p_data->rec.p_subject[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_SCORE:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_score[row_id];
// 			p_data->rec.p_score[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_GROUPS:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_groups[row_id];
// 			p_data->rec.p_groups[row_id] = NULL;
// 		}
// 		break;
// 	case	TABLE_GROUPS_RECORD:
// 		for (int i=0;i<MAX_ROW_NUM;i++)
// 		{
// 			delete p_data->rec.p_groups_record[row_id];
// 			p_data->rec.p_groups_record[row_id] = NULL;
// 		}
// 		break;
// 	}
}

void InitTableName()
{
	strcpy(table_name[TABLE_USERINFO], "userinfo");
	strcpy(table_name[TABLE_TRAIN_INFO], "train_info");
	strcpy(table_name[TABLE_LOGIN_INFO], "login_info");
	strcpy(table_name[TABLE_RECORD], "record");
	strcpy(table_name[TABLE_SCENE], "scene");
	strcpy(table_name[TABLE_SCENE_DATA], "scene_data");
	strcpy(table_name[TABLE_REVERT], "revert");
	strcpy(table_name[TABLE_FAULT], "fault");
	strcpy(table_name[TABLE_FAULT_DATA], "fault_data");
	strcpy(table_name[TABLE_TASK_GRP], "task_grp");
	strcpy(table_name[TABLE_TASK], "task");
	strcpy(table_name[TABLE_SEND_TASK], "send_task");
	strcpy(table_name[TABLE_HOST], "host");
	strcpy(table_name[TABLE_OPR_TYPE], "opr_type");
	strcpy(table_name[TABLE_LOCAL_OPR], "local_opr");
	strcpy(table_name[TABLE_RUN_MODEL], "run_model");
	strcpy(table_name[TABLE_RUN_SEQUENCE], "run_sequence");
	strcpy(table_name[TABLE_MODEL], "model");
	strcpy(table_name[TABLE_SEQUENCE], "sequence");
	strcpy(table_name[TABLE_VAR_RELATION], "var_relation");
	strcpy(table_name[TABLE_SUBJECT], "subject");
	strcpy(table_name[TABLE_SCORE],	"score");
	strcpy(table_name[TABLE_GROUPS],	"groups");
	strcpy(table_name[TABLE_GROUPS_RECORD],	"groups_record");
}	

void InitTableData(char table_id)
{
	int i;
	for(i=0;i<MAX_FIELD_NUM;i++)
	{
		strcpy(field_name[i],"");
		strcpy(field_value[i],"");
		strcpy(field_table[i],"");
		strcpy(union_table[i],"");
		strcpy(field_name[i],"");
		field_type[i]=0;
	}
	for(i=0;i<MAX_COND_NUM;i++)
	{
		strcpy(cond_name[i],"");
		strcpy(cond_value[i],"");
		strcpy(cond_value[i],"");
		strcpy(cond_opr[i],"");
	}
	switch(table_id)
	{
	case	TABLE_USERINFO:
		memset(data_userinfo,0, sizeof(DATA_USERINFO) * MAX_ROW_NUM );
		break;
	case	TABLE_TRAIN_INFO:
		memset(data_train_info,0, sizeof(DATA_TRAIN_INFO) * MAX_ROW_NUM );
		break;
	case	TABLE_LOGIN_INFO:
		memset(data_login_info,0, sizeof(DATA_LOGIN_INFO) * MAX_ROW_NUM_LARGE );
		break;
	case	TABLE_RECORD:
		memset(data_record,0, sizeof(DATA_RECORD) * MAX_ROW_NUM_LARGE );
		break;
	case	TABLE_SCENE:
		memset(data_scene,0, sizeof(DATA_SCENE) * MAX_ROW_NUM );
		break;
	case	TABLE_SCENE_DATA:
		memset(data_scene_data,0, sizeof(DATA_SCENE_DATA) * MAX_ROW_NUM_HUGE );
		break;
	case	TABLE_REVERT:
		memset(data_revert,0, sizeof(DATA_REVERT) * MAX_ROW_NUM );
		break;
	case	TABLE_FAULT:
		memset(data_fault,0, sizeof(DATA_FAULT) * MAX_ROW_NUM );
		break;
	case	TABLE_FAULT_DATA:
		memset(data_fault_data,0, sizeof(DATA_FAULT_DATA) * MAX_ROW_NUM );
		break;
	case	TABLE_TASK_GRP:
		memset(data_task_grp,0, sizeof(DATA_TASK_GRP) * MAX_ROW_NUM );
		break;
	case	TABLE_TASK:
		memset(data_task,0, sizeof(DATA_TASK) * MAX_ROW_NUM );
		break;
	case	TABLE_SEND_TASK:
		memset(data_send_task,0, sizeof(DATA_SEND_TASK) * MAX_ROW_NUM );
		break;
	case	TABLE_HOST:
		memset(data_host,0, sizeof(DATA_HOST) * MAX_ROW_NUM );
		break;
	case	TABLE_OPR_TYPE:
		memset(data_opr_type,0, sizeof(DATA_OPR_TYPE) * MAX_ROW_NUM );
		break;
	case	TABLE_LOCAL_OPR:
		memset(data_local_opr,0, sizeof(DATA_LOCAL_OPR) * MAX_ROW_NUM );
		break;
	case	TABLE_RUN_MODEL:
		memset(data_run_model,0, sizeof(DATA_RUN_MODEL) * MAX_ROW_NUM );
		break;
	case	TABLE_RUN_SEQUENCE:
		memset(data_run_sequence,0, sizeof(DATA_RUN_SEQUENCE) * MAX_ROW_NUM );
		break;
	case	TABLE_MODEL:
		memset(data_model,0, sizeof(DATA_MODEL) * MAX_ROW_NUM );
		break;
	case	TABLE_SEQUENCE:
		memset(data_sequence,0, sizeof(DATA_SEQUENCE) * MAX_ROW_NUM );
		break;
	case	TABLE_VAR_RELATION:
		memset(data_var_relation,0, sizeof(DATA_VAR_RELATION) * MAX_ROW_NUM );
		break;
	case	TABLE_SUBJECT:
		memset(data_subject,0, sizeof(DATA_SUBJECT) * MAX_ROW_NUM );
		break;
	case	TABLE_SCORE:
		memset(data_score,0, sizeof(DATA_SCORE) * MAX_ROW_NUM );
		break;
	case	TABLE_GROUPS:
		memset(data_groups,0, sizeof(DATA_GROUPS) * MAX_ROW_NUM );
		break;
	case	TABLE_GROUPS_RECORD:
		memset(data_groups_record,0, sizeof(DATA_GROUPS_RECORD) * MAX_ROW_NUM );
		break;
	}
}	

void	GetCondStr(char *str)
{
	char tmp_str[200]="";
	int i;
	strcpy(str," ");
	for(i=0;i<cond_num;i++)
	{
		if(i==0)
		{
			strcpy(str," where ");
		}
		sprintf(tmp_str,"%s = %s",cond_name[i],cond_value[i]);
		strcat(str,tmp_str);
		if(i != cond_num-1)
		{
			strcat(str,",");
		}
	}
}


int		ExecSql(char *sql_str)
{
	int ret;
	QSqlQuery db_opr("",db_conn);
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	return ret;
}

int		ExecSqlQStr(const QString &sql_str)
{
	int ret;
	QSqlQuery db_opr("",db_conn);
	ret = db_opr.exec(sql_str);
	return ret;
}

int ConnectHDB(QSqlDatabase db,const QString &connectionName)
{
	QSqlError err;
	QApplication::addLibraryPath("./plugins");
	if (strlen(g_ots_hdb_type)>1)
	{
		strcpy(hdb_type,g_ots_hdb_type);
	}
	if (strcmp(hdb_type,"ORACLE")==0)
	{
		server_port = 1521;
		if (connectionName == "defaultConnection")
			db = QSqlDatabase::addDatabase("QOCI"); 
		else
			db = QSqlDatabase::addDatabase("QOCI",connectionName); 
	}
	else if (strcmp(hdb_type,"MYSQL")==0)
	{
		server_port = 3306;
		if (connectionName == "defaultConnection")
			db = QSqlDatabase::addDatabase("QMYSQL"); 
		else
			db = QSqlDatabase::addDatabase("QMYSQL",connectionName); 
	}
	  
	if( strlen( g_ots_hdb_ip ) >1 )
	{
		strcpy(server_ip,g_ots_hdb_ip);
	}
	if( strlen( g_ots_hdb_sid ) >1 )
	{
		strcpy(server_sid,g_ots_hdb_sid);
	}
    db.setHostName(server_ip);   
    db.setDatabaseName(server_sid);   
    db.setUserName(username);   
    db.setPassword(password);   
    db.setPort(server_port);   
    if (db.open())   
 	{
		if (strcmp(hdb_type,"MYSQL")==0)
			db.exec(QString::fromLocal8Bit("SET NAMES 'utf8'")); 
		printf("打开成功\n");  
		return TRUE;
	}
    else
	{
		err=db.lastError();
		printf("打开失败  错误：%s \n",err.text().toLocal8Bit().data());  	
		return FALSE;
	}	
}

int CloseHDB(QSqlDatabase db)
{
	if (db.isOpen())
	{
		db.close();
	}
	return TRUE;
}

int	CvtFieldData(DB_FMT *p_data, int mode)
{
	int i=0,j=0;
	strcpy(field_table[0],table_name[p_data->table_id]);		
	switch(p_data->table_id)
	{
	case	TABLE_USERINFO:
		cvt_userinfo(p_data,mode);
		cvt_userinfo_cond(p_data,mode);
		break;
	case	TABLE_TRAIN_INFO:
		cvt_train_info(p_data,mode);
		cvt_train_info_cond(p_data,mode);

		break;
	case	TABLE_LOGIN_INFO:
		cvt_login_info(p_data,mode);
		cvt_login_info_cond(p_data,mode);
		break;
	case	TABLE_RECORD:
		cvt_record(p_data,mode);
		cvt_record_cond(p_data,mode);
		break;
	case	TABLE_SCENE:
		cvt_scene(p_data,mode);
		cvt_scene_cond(p_data,mode);
		break;
	case	TABLE_SCENE_DATA:
		cvt_scene_data(p_data,mode);
		cvt_scene_data_cond(p_data,mode);
		break;

	case	TABLE_REVERT:
		cvt_revert(p_data,mode);
		cvt_revert_cond(p_data,mode);
		break;
	case	TABLE_FAULT:
		cvt_fault(p_data,mode);
		cvt_fault_cond(p_data,mode);		
		break;
	case	TABLE_FAULT_DATA:
		cvt_fault_data(p_data,mode);
		cvt_fault_data_cond(p_data,mode);
		break;
	case	TABLE_TASK_GRP:
		cvt_task_grp(p_data,mode);
		cvt_task_grp_cond(p_data,mode);		
		break;
	case	TABLE_TASK:
		cvt_task(p_data,mode);
		cvt_task_cond(p_data,mode);	

		break;
	case	TABLE_SEND_TASK:
		cvt_send_task(p_data,mode);
		cvt_send_task_cond(p_data,mode);		
		break;
	case	TABLE_HOST:
		cvt_host(p_data,mode);
		cvt_host_cond(p_data,mode);

		break;
	case	TABLE_OPR_TYPE:
		cvt_opr_type(p_data,mode);
		cvt_opr_type_cond(p_data,mode);

		break;
	case	TABLE_LOCAL_OPR:
		cvt_local_opr(p_data,mode);
		cvt_local_opr_cond(p_data,mode);

		break;
	case	TABLE_RUN_MODEL:
		cvt_run_model(p_data,mode);
		cvt_run_model_cond(p_data,mode);

		break;
	case	TABLE_RUN_SEQUENCE:
		cvt_run_sequence(p_data,mode);
		cvt_run_sequence_cond(p_data,mode);

		break;
	case	TABLE_MODEL:
		cvt_model(p_data,mode);
		cvt_model_cond(p_data,mode);


		break;
	case	TABLE_SEQUENCE:
		cvt_sequence(p_data,mode);
		cvt_sequence_cond(p_data,mode);

		break;
	case	TABLE_VAR_RELATION:
		cvt_var_relation(p_data,mode);
		cvt_var_relation_cond(p_data,mode);
		break;
	case	TABLE_SUBJECT:
		cvt_subject(p_data,mode);
		cvt_subject_cond(p_data,mode);
		break;
	case	TABLE_SCORE:
		cvt_score(p_data,mode);
		cvt_score_cond(p_data,mode);
		break;
	case	TABLE_GROUPS:
		cvt_groups(p_data,mode);
		cvt_groups_cond(p_data,mode);
		break;
	case	TABLE_GROUPS_RECORD:
		cvt_groups_record(p_data,mode);
		cvt_groups_record_cond(p_data,mode);
		break;
	}

	return TRUE;
}




int GetUserInfoTable()
{
	DB_FMT data;
	int i;
	data.table_id=TABLE_USERINFO;
	InitDbData(&data,data.table_id);
	
	if( SelectRecs(&data) != TRUE)
	{
		printf("查询数据失败\n");	
		FreeDbData(&data,TABLE_USERINFO);
		return FALSE;
	}
	else
	{
		printf("查询数据成功\n");
		printf("返回查询记录 %d 条\n",data.rownum);		
	}
	g_user_num = data.rownum;
	for(i=0;i<g_user_num;i++)
	{
		g_userinfo[i]=data_userinfo[i];
	}
	FreeDbData(&data,data.table_id);
	return TRUE;
}

int GetHostTable()
{
	DB_FMT data;
	int i;
	data.table_id=TABLE_HOST;
	InitDbData(&data,data.table_id);
	
	if( SelectRecs(&data) != TRUE)
	{
		printf("查询数据失败\n");	
		FreeDbData(&data,TABLE_HOST);
		return FALSE;
	}
	else
	{
		printf("查询数据成功\n");
		printf("返回查询记录 %d 条\n",data.rownum);		
	}
	g_host_num = data.rownum;
	for(i=0;i<g_host_num;i++)
	{
		g_host[i]=data_host[i];
	}
	FreeDbData(&data,data.table_id);
	return TRUE;
}

int InitOTSHdb()
{
	gethostname(g_host_name,20); 
	InitTableName();
	if( GetUserInfoTable() != TRUE )
	{
		return FALSE;
	}
	if( GetHostTable() != TRUE )
	{
		return FALSE;
	}
	g_host_id=GetHostIdByName(g_host_name);
	g_user_id=-1;
	g_groups_id=-1;
	g_groups_role=-1;
	if( g_host_id==-1 )
	{
		printf("GetHostIdByName(%s) 失败\n",g_host_name);
		return FALSE;
	}
	if( SelectPtHdb(0) != TRUE )
	{
		printf("SelectPtHdb error\n");
		return FALSE;
	}

	return TRUE;
}

int GetHostIdByName(char *hostname)
{
	int i;
	int host_id=-1;
	for(i=0;i<g_host_num;i++)
	{
		if( strcmp(hostname,g_host[i].name)==0 )
		{
			host_id=g_host[i].host_id;
			break;
		}
	}
	return host_id;
}

int GetHostnNameById(int host_id, char *hostname)
{
	int i;
	for(i=0;i<g_host_num;i++)
	{
		if( host_id == g_host[i].host_id)
		{
			strcpy(hostname,g_host[i].name);
			return TRUE;
		}
	}
	return FALSE;
}

int	GetInexistenceIdInHdb(unsigned char table_id)
{
	char FieldStr[20];
	int ret;
	QString q_str;
	char sql_str[200]="";
	int id=0;
	QSqlQuery db_opr("",db_conn);
	switch(table_id)
	{
	case	TABLE_USERINFO:
		strcpy(FieldStr,"user_id");
		break;

	case	TABLE_SCENE:
		strcpy(FieldStr,"scene_id");
		break;

	case	TABLE_REVERT:
		strcpy(FieldStr,"revert_id");
		break;
	case	TABLE_FAULT:
		strcpy(FieldStr,"fault_id");
		break;

	case	TABLE_TASK_GRP:
		strcpy(FieldStr,"task_grp_id");
		break;
	case	TABLE_TASK:
		strcpy(FieldStr,"task_id");
		break;
	case	TABLE_SEND_TASK:
		strcpy(FieldStr,"send_task_id");
		break;
	case	TABLE_HOST:
		strcpy(FieldStr,"host_id");
		break;
	case	TABLE_OPR_TYPE:
		strcpy(FieldStr,"opr_type");
		break;

	case	TABLE_LOCAL_OPR:
		strcpy(FieldStr,"opr_id");
		break;

	case	TABLE_MODEL:
		strcpy(FieldStr,"model_id");
		break;

	case	TABLE_SEQUENCE:
		strcpy(FieldStr,"seq_id");
		break;

	case	TABLE_SUBJECT:
		strcpy(FieldStr,"subject_id");
		break;

	case	TABLE_GROUPS:
		strcpy(FieldStr,"groups_id");
		break;
	default:
		return FALSE;

	}
	sprintf(sql_str,"select min(%s+1) from %s a where not exists(select 1 from %s b where a.%s+1=b.%s)",FieldStr,table_name[table_id],table_name[table_id],FieldStr,FieldStr);
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	if(ret != TRUE)
	{
		return ret;
	}

	while( db_opr.next() )
	{
		id=db_opr.value(0).toInt();
		break;
	}
	return id;
}
int GetMaxIdInHdb(unsigned char table_id)
{
	char FieldStr[20];
	int ret;
	QString q_str;
	char sql_str[200]="";
	int max_id=0;
	QSqlQuery db_opr("",db_conn);
	switch(table_id)
	{
	case	TABLE_USERINFO:
		strcpy(FieldStr,"user_id");
		break;

	case	TABLE_SCENE:
		strcpy(FieldStr,"scene_id");
		break;

	case	TABLE_REVERT:
		strcpy(FieldStr,"revert_id");
		break;
	case	TABLE_FAULT:
		strcpy(FieldStr,"fault_id");
		break;

	case	TABLE_TASK_GRP:
		strcpy(FieldStr,"task_grp_id");
		break;
	case	TABLE_TASK:
		strcpy(FieldStr,"task_id");
		break;
	case	TABLE_SEND_TASK:
		strcpy(FieldStr,"send_task_id");
		break;
	case	TABLE_HOST:
		strcpy(FieldStr,"host_id");
		break;
	case	TABLE_OPR_TYPE:
		strcpy(FieldStr,"opr_type");
		break;

	case	TABLE_LOCAL_OPR:
		strcpy(FieldStr,"opr_id");
		break;

	case	TABLE_MODEL:
		strcpy(FieldStr,"model_id");
		break;

	case	TABLE_SEQUENCE:
		strcpy(FieldStr,"seq_id");
		break;

	case	TABLE_SUBJECT:
		strcpy(FieldStr,"subject_id");
		break;

	case	TABLE_GROUPS:
		strcpy(FieldStr,"groups_id");
		break;
	default:
		return FALSE;
		
	}
	sprintf(sql_str,"select max(%s) from %s",FieldStr,table_name[table_id]);
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	if(ret != TRUE)
	{
		return ret;
	}

	while( db_opr.next() )
	{
		max_id=db_opr.value(0).toInt();
		break;
	}
	return max_id;
}

int CvtQDateTimeToStr(char *str,QDateTime dtime)
{
	sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",dtime.date().year(),dtime.date().month(),	dtime.date().day(),
		dtime.time().hour(),dtime.time().minute(),	dtime.time().second());
	return TRUE;
}

int HdbLogWrite(char *sql,char *sql_error)
{
	FILE *fp;
	char log_name[256];
	QString appName;
	QString appPath = qApp->applicationFilePath();
	QString date = QDate::currentDate().toString(QString("yyyyMMdd"));
	QString time = QTime::currentTime().toString(QString("hh:mm:ss"));
	appName = appPath.section('/', -1);
	appName = appName.section('.',0,0);
	sprintf(log_name,"%s%s%s_err_log.txt",ERROR_LOG_PATH,appName.toLocal8Bit().data(),date.toLocal8Bit().data());
	/*if(-1==access(ERROR_LOG_PATH, 0))
	{
#ifdef	WINDOWS_OS
		if(-1==mkdir(ERROR_LOG_PATH))
#else
		if(-1==mkdir(ERROR_LOG_PATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#endif
		{
			qDebug("Fail to create path %s", ERROR_LOG_PATH);
			return -1;
		}
	}*/
	if( (fp=fopen(log_name, "a"))==NULL )
	{
		qDebug("Fail to open %s", log_name);
		return -1;
	}
	fprintf(fp,"%s\t%s\t%s\n",time.toLocal8Bit().data(),sql,sql_error);
	fclose(fp);
	return 0;
}

int	SelectRecsFuzzy(DB_FMT *p_data,QSqlDatabase db)
{
	int ret;
	QString q_str;
	QDateTime dtime;
	int i=0,j=0;
	char sql_str[4000]="";
	char field_str[2000]="";
	char cond_str[500]="";
	char table_str[200]="";
	char tmp_str[2000]="";
	field_num=0;cond_num=0;row_id=0;table_num=1;
	InitTableData(p_data->table_id);
	p_data->opr_mode = DB_FUZZY_SELECT;
	QSqlQuery db_opr("",db);	
	memset(&p_data->flag,1,sizeof(p_data->flag));
	CvtFieldData(p_data,0);

	for(i=0;i<table_num;i++)
	{
		if(strlen(union_table[i]) < 1 )
		{
			strcpy(union_table[i],table_name[p_data->table_id]);
		}
		sprintf(tmp_str,"%s",union_table[i]);
		strcat(table_str,tmp_str);
		if(i != table_num-1)
		{
			strcat(table_str,",");
		}
	}

	for(i=0;i<field_num;i++)
	{
		if( table_num == 1 )
		{
			sprintf(tmp_str,"%s",field_name[i]);
		}
		else
		{
			if(strlen(field_table[i]) < 1 )
			{
				strcpy(field_table[i],table_name[p_data->table_id]);
			}			
			sprintf(tmp_str,"%s.%s",field_table[i],field_name[i]);			
		}
		strcat(field_str,tmp_str);
		if(i != field_num-1)
		{
			strcat(field_str,",");
		}
	}

	for(i=0;i<cond_num;i++)
	{
		if(i==0)
		{
			strcpy(cond_str," where ");
		}
		if(table_num == 1)
		{
			sprintf(tmp_str,"%s like '%%%s%%'",cond_name[i],cond_value[i]);
		}
		else
		{
			if(strlen(cond_opr[i]) < 1 )
			{
				strcpy(cond_opr[i],"like");
			}			

			sprintf(tmp_str,"%s.%s %s '%%%s%%'",table_name[p_data->table_id],cond_name[i],cond_opr[i],cond_value[i]);
		}
		strcat(cond_str,tmp_str);
		if(i != cond_num-1)
		{
			strcat(cond_str," and ");
		}
	}
	sprintf(sql_str,"select %s from %s %s %s",field_str,table_str,cond_str,p_data->addon_str);
#ifdef DEBUG
	printf("sql = %s\n",sql_str );
#endif	
	ret = db_opr.exec(QString::fromLocal8Bit(sql_str));
	strcpy(p_data->sql_error,db_opr.lastError().text().toLocal8Bit().data());
	if(ret != TRUE)
	{
		return ret;
	}
	row_id=0;
	while( db_opr.next() )
	{
		for(j=0;j<field_num;j++)
		{
			if(field_type[j]!= TYPE_TIME)
			{
				q_str = db_opr.value(j).toString();
				strcpy(field_value[j],q_str.toLocal8Bit().data());
				CvtArrayToStr(field_value[j],strlen(field_value[j]));
			}
			else
			{
				if(db_opr.value(j).isNull()!=TRUE)
				{
					dtime = db_opr.value(j).toDateTime();
					CvtQDateTimeToStr(field_value[j],dtime);
				}
				else
				{
					strcpy(field_value[j],"");
				}
			}
#ifdef DEBUG
			printf("%s	",field_value[j]);
			if(j==field_num-1)
			{
				printf("\n");
			}
#endif
		}//end for

		CvtFieldData(p_data,1);
		row_id++;

		if( p_data->table_id == TABLE_RECORD || p_data->table_id == TABLE_LOGIN_INFO)
		{
			if(row_id>=MAX_ROW_NUM_LARGE)
			{
				break;
			}
		}
		if( p_data->table_id == TABLE_SCENE_DATA )
		{
			if(row_id>=MAX_ROW_NUM_HUGE)
			{
				break;
			}
		}
		else
		{
			if(row_id>=MAX_ROW_NUM)
			{
				break;
			}
		}

	}//end while
	strcpy(p_data->sql_str,sql_str);
	p_data->rownum = row_id;
	return ret;

}