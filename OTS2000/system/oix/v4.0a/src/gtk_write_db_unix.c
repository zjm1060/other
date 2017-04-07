
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

extern int fault_redisplay_on[MAX_CRT_NUM],step_id ;
extern int fault_redisplay_crt_num   ; 
extern struct tm fault_starttime,fault_endtime;
HDB_REVERT *shm_realdb_addr,realdb_data;

extern char *_toUtf8(char *c);
extern char *_toUtf8EX(char *c);

#define CONNECT_HDB_FAIL    1
#define CONNECT_HDB_SUCESS  0
int connect_id=1;
/*#define DEBUG*/
int  iPlayStep  = 100   ;   //Unit MilliSecond     
void*	WriteDbFromHDB(void *arg)	
{	
	POINTER		point;
	WR_VAR_UNION	new_value;
	ENTRY_DEVOBJ_VAL obj_value;
	IOST		iost;
	DATE_TIME	updatetime;
	int		i,j=0,k=0,m,no_val=0,crt_n=(int)arg;	
	char		label[256],tmp[256];
	GtkWidget  *lab_fault_redisplay;
	struct tm fault_time;
	time_t tTim;      
//	char new_start_time[128],new_end_time[128];
	struct tm new_start_t;
	time_t new_tTim_start,old_tTim_start;

	gchar *pConvert=0;
	
	time_t tTim_start,tTim_end;
	m = 0 ; 
	tTim_start = mktime(&fault_starttime);
	tTim_end   = mktime(&fault_endtime  );
	shm_realdb_addr = &realdb_data; 
	
	for(;;)
	{
#ifdef DEBUG
		printf("11::queryflag=%d\n",shm_hdb_addr->data_revert.queryflag);
#endif
		lab_fault_redisplay = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"label_fault_redisplay");
		if (lab_fault_redisplay != NULL)		
			strcpy(label,_CS_(OIX_CSTR_FAULT_REDISPLAY_ON));
		else 
			printf("!!!!lab_fault_redisplay is NULL\n");
		if(j==0)
		{                        
			strcpy(label,_CS_(MSG_CONNECTING_HDB));
			gtk_label_set_text(GTK_LABEL(lab_fault_redisplay), pConvert=_toUtf8(label));
			G_SAFE_FREE(pConvert);
			/*printf("start sleep\n");*/
			connect_id =1;
			if (shm_hdb_addr->data_revert.queryflag)
			{   
				m++ ; 
				if (m<12)  //one minute 
				{
					MySleep(5000); 
					continue ;  //goto next loop !!!
				}else  //超时退出
				{
					connect_id=CONNECT_HDB_FAIL ;
					strcpy(label,_CS_("connect DB error!"));
					gtk_label_set_text(GTK_LABEL(lab_fault_redisplay), pConvert=_toUtf8(label));
					G_SAFE_FREE(pConvert);
					goto ExitThr;
				}
				
			}
			
			connect_id =0;
			/*printf("end sleep\n");*/
			memcpy(shm_realdb_addr,&(shm_hdb_addr->data_revert),sizeof( HDB_REVERT));
		}
		if(fault_redisplay_on[fault_redisplay_crt_num]==1)
		{
			if(shm_hdb_addr->data_revert.stopflag==1)
			{
				/*sleep(1);*/
				/*printf("stopflag=%d\n",shm_hdb_addr->data_revert.stopflag);*/
				continue;
			}
			if(shm_hdb_addr->data_revert.querytype ==STEP_BY_STEP)
			{                            
				if(step_id==1)
				{                                    
					step_id=0;
					shm_hdb_addr->data_revert.stopflag=1;
					printf("querytype=STEP_BY_STEP&&step_id==1\n");                                    
				}
				else
				{
					step_id=1;
					shm_hdb_addr->data_revert.stopflag=0;
					printf("querytype=STEP_BY_STEP&&step_id==0\n");                                    
					continue;
				}
			}
			
			printf("ycnum=%d,yxnum=%d\n",shm_hdb_addr->data_revert.ycnum,shm_hdb_addr->data_revert.yxnum);
			
			
			for(i=0; i<(int)shm_hdb_addr->data_revert.ycnum; i++)
			{				
				point = shm_hdb_addr->data_revert.yc_data[i].point;
				
				new_value.anlg.status.chan_fail		=0;
				new_value.anlg.status.ctrl_block	=0;
				new_value.anlg.status.alm_type		=0;
				new_value.anlg.status.trend_state	=0;
				new_value.anlg.status.lmt_state		=0;
				if(shm_hdb_addr->data_revert.yc_data[i].rscount==0||j>=shm_hdb_addr->data_revert.yc_data[i].rscount)
				{
					printf("yci=%d,yc[%d].logname=%s,j=%d,rscount=%d\n",i,i,shm_hdb_addr->data_revert.yc_data[i].logicalname,
						j,shm_hdb_addr->data_revert.yc_data[i].rscount);
					continue;
				}
				else
					printf("sucessful::yci=%d,yc[%d].logname=%s,j=%d,rscount=%d\n",i,i,shm_hdb_addr->data_revert.yc_data[i].logicalname,
					j,shm_hdb_addr->data_revert.yc_data[i].rscount);
				
				strncpy(tmp,shm_hdb_addr->data_revert.yc_data[i].timestr[j],4);
				updatetime.year = atoi(tmp);
				strncpy(tmp,&shm_hdb_addr->data_revert.yc_data[i].timestr[j][5],2);
				tmp[2]='\0';
				updatetime.month = atoi(tmp);
				strncpy(tmp,shm_hdb_addr->data_revert.yc_data[i].timestr[j]+8,2);
				tmp[2]='\0';
				updatetime.day = atoi(tmp);
				strncpy(tmp,shm_hdb_addr->data_revert.yc_data[i].timestr[j]+11,2);
				tmp[2]='\0';
				updatetime.hour = atoi(tmp);                               
				strncpy(tmp,shm_hdb_addr->data_revert.yc_data[i].timestr[j]+14,2);
				tmp[2]='\0';
				updatetime.minute = atoi(tmp);
				strncpy(tmp,shm_hdb_addr->data_revert.yc_data[i].timestr[j]+17,2);
				tmp[2]='\0';
				updatetime.second = atoi(tmp);
				
				memset(&iost, 0, sizeof(iost));
				switch	( point.data_type )
				{
				case	ANA_TYPE:
				case	OUTA_TYPE:
				case	PARAM_TYPE:
					new_value.anlg.data =   (GLONG)(1000  * shm_hdb_addr->data_revert.yc_data[i].value[j])   ; //  1000*shm_realdb_addr->yc_data[i].value[j];
					
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );
					break;
					
				case	CALC_TYPE:
					new_value.calc.data= (GLONG)shm_realdb_addr->yc_data[i].value[j];
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );
					break;
					
				case	IMP_TYPE:
					new_value.imp.status.option= 0;
					new_value.imp.iValue= (GULONG)shm_hdb_addr->data_revert.yc_data[i].value[j] ;  //shm_realdb_addr->yc_data[i].value[j];
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME);
					break;
					
				default:
					break;
				}
				shm_hdb_addr->data_revert.curptr  = j; 
				
				
			}
			
			for(i=0; i<(int)shm_hdb_addr->data_revert.yxnum; i++)
			{				
				point = shm_hdb_addr->data_revert.yx_data[i].point;				
				
				if(shm_hdb_addr->data_revert.yx_data[i].rscount==0 || j>=shm_hdb_addr->data_revert.yx_data[i].rscount)
				{				
					continue;
				}
				else
					printf("yxi=%d,j=%d,rscount=%d\n",i,j,shm_hdb_addr->data_revert.yx_data[i].rscount);
				
				printf("timestr=%s\n",shm_hdb_addr->data_revert.yx_data[i].timestr[j]);
				
				strncpy(tmp,shm_hdb_addr->data_revert.yx_data[i].timestr[j],4);
				updatetime.year = atoi(tmp);
				strncpy(tmp,&shm_hdb_addr->data_revert.yx_data[i].timestr[j][5],2);
				tmp[2]='\0';
				updatetime.month = atoi(tmp);
				strncpy(tmp,shm_hdb_addr->data_revert.yx_data[i].timestr[j]+8,2);
				tmp[2]='\0';
				updatetime.day = atoi(tmp);
				strncpy(tmp,shm_hdb_addr->data_revert.yx_data[i].timestr[j]+11,2);
				tmp[2]='\0';
				updatetime.hour = atoi(tmp);                               
				strncpy(tmp,shm_hdb_addr->data_revert.yx_data[i].timestr[j]+14,2);
				tmp[2]='\0';
				updatetime.minute = atoi(tmp);
				strncpy(tmp,shm_hdb_addr->data_revert.yx_data[i].timestr[j]+17,2);
				tmp[2]='\0';
				updatetime.second = atoi(tmp);
				/* if(updatetime.second!=0)*/
				printf("poltime: second=%d,min=%d,hour=%d,day=%d,month=%d,year=%d\n",updatetime.second,updatetime.minute,updatetime.hour,updatetime.day,updatetime.month,updatetime.year);	
				switch	( point.data_type )
				{
				case	SOE_TYPE:
				case	POL_TYPE:
				case	OUTS_TYPE:
				case	SYSMSG_TYPE:					
					new_value.ind.rt_state= shm_hdb_addr->data_revert.yx_data[i].status[j] ;  //   shm_realdb_addr->yx_data[i].status[j];
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );											
					break;
				case     DEVOBJ_TYPE:/* added by zyp on 2007.7.31*/
					memcpy ( &obj_value.fb_status1 ,  &shm_hdb_addr->data_revert.yx_data[i].status[j],sizeof(FB_STATUS1)) ;   //shm_realdb_addr->yx_data[i].status[j];
					SetDevObjPtVarById( &point, obj_value );
					break;                                    
				}	
				shm_hdb_addr->data_revert.curptr  = j; 
			}
			
			
			fault_time = fault_starttime;
			printf("shm_hdb_addr->connflag_revert=%d\n",shm_hdb_addr->connflag_revert);
			if(shm_hdb_addr->connflag_revert!=1)
			{
				j++;                           
				strcpy(label,_CS_(MSG_CONNECTING_HDB_FAIL));
				gtk_label_set_text(GTK_LABEL(lab_fault_redisplay), pConvert=_toUtf8( label));
				G_SAFE_FREE(pConvert);
				MySleep(iPlayStep) ;  //  sleep(1000); 
				printf("tm_year<=0,updatetime.year=%d\n",updatetime.year);
				connect_id=CONNECT_HDB_FAIL ;
				goto ExitThr;
				/* continue;*/
			}
			tTim = mktime(&fault_time);
			if(tTim!=-1)
			{
		/*
				tTim +=j/ *+k*MAX_REVERT_REC* // *-no_val* /;
						shm_hdb_addr->data_revert.curptr  = j;  //返回指针;;
						fault_time=*localtime(&tTim);
						
						sprintf(tmp,"%04d-%02d-%02d %02d:%02d:%02d",fault_time.tm_year+1900,fault_time.tm_mon+1,fault_time.tm_mday,
							fault_time.tm_hour,fault_time.tm_min,fault_time.tm_sec);
						strcpy(label,_CS_(OIX_CSTR_FAULT_REDISPLAY_ON));
						strcat(label,tmp);
						printf("time label=%s\n",label);
						gtk_label_set_text(GTK_LABEL(lab_fault_redisplay),_toUtf8EX(label));*/
		


			}
			else
			{
				printf("mktime error!,sec=%d,min=%d,hour=%d,day=%d,mon=%d,year=%d\n",fault_time.tm_sec,
					fault_time.tm_min,fault_time.tm_hour,fault_time.tm_mday,fault_time.tm_mon,fault_time.tm_year);
				sprintf(label,"time error!");
				gtk_label_set_text(GTK_LABEL(lab_fault_redisplay), pConvert=_toUtf8EX(label));
				G_SAFE_FREE(pConvert);
			}
			
			if(j==MAX_REVERT_REC-1)
			{
				j=0;
				k++;                                                      				
				old_tTim_start= mktime(&fault_starttime);
				new_tTim_start = old_tTim_start+k*MAX_REVERT_REC;
				new_start_t =*localtime(&new_tTim_start);
				sprintf(shm_hdb_addr->data_revert.starttime,"%04d-%02d-%02d %02d:%02d:%02d",new_start_t.tm_year+1900,new_start_t.tm_mon+1,new_start_t.tm_mday,
					new_start_t.tm_hour,new_start_t.tm_min,new_start_t.tm_sec);
				printf("new_starttime=%s\n",shm_hdb_addr->data_revert.starttime);
				printf("before memcpy::shm_hdb_addr->data_revert.starttime=%s\n",shm_hdb_addr->data_revert.starttime);
				shm_hdb_addr->data_revert.queryflag = 1; 
				MySleep(iPlayStep);  //1000
				memcpy(shm_realdb_addr,shm_hdb_addr,sizeof(struct shm_hdb));
			}
			else
			{
				j++;
				if(j+k*MAX_REVERT_REC>(int)(tTim_end-tTim_start)/*arg*/)
					goto ExitThr;                       
				
				MySleep(iPlayStep);  //1000
			}
			
#ifdef DEBUG	
			printf("in WriteDb:: arg=%d,j=%d\n",arg,j);
#endif
		}         
		else/*(  fault_redisplay_on == 0 )  */ 
		{ 
#ifdef DEBUG
			printf("in else::queryflag=%d\n",shm_hdb_addr->data_revert.queryflag);     		
#endif                 
			goto ExitThr;         		        
		}  
	}/* for loop end*/    
ExitThr:
#ifdef SUN_OS
    pthread_exit(0);
#endif
#ifdef WINDOWS_OS
	ExitThread(0) ; 
#endif
#ifdef DEBUG   
	printf("\nthread %d return \n", pthread_self());
#endif	 
	
	return (NULL);	
}

void *PeriodRequestData(void *arg)
{
	int i=0,j=0;
//	char new_start_time[30],new_end_time[30];
	struct tm new_start_t;
	time_t new_tTim_start,old_tTim_start;
	for(;;)
	{
		if(fault_redisplay_on[fault_redisplay_crt_num] ==1)
		{
			if(shm_hdb_addr->data_revert.stopflag==1)
			{
				MySleep(1);
                /* printf("stopflag=%d\n",shm_hdb_addr->data_revert.stopflag);*/
				continue;
            }
            if(shm_hdb_addr->data_revert.querytype ==STEP_BY_STEP&&step_id==1)
            {
				MySleep(1);
				/* printf("querytype=STEP_BY_STEP&&step_id==1\n");*/
                continue;
            }
			if(i<MAX_REVERT_REC)
			{
				printf("i=%d\n",i);
				i++;	
				MySleep(1);                        
			}
			else
			{
				j++;
				i=0;
				
				if(j>(int)arg/MAX_REVERT_REC)
				{
					printf("j=%d,arg=%d,MAX_REVERT_REC=%d\n",j,(int)arg,MAX_REVERT_REC);
					goto endThr;
				}
				printf("####### ***** PeriodRequestData start!\n");
				printf("pre_starttime=%s\n",shm_hdb_addr->data_revert.starttime);
				
				shm_hdb_addr->data_revert.queryflag = 1;                        
				
				old_tTim_start= mktime(&fault_starttime);
				new_tTim_start = old_tTim_start+j*MAX_REVERT_REC;
				new_start_t =*localtime(&new_tTim_start);
				sprintf(shm_hdb_addr->data_revert.starttime,"%d-%d-%d %d:%d:%d",new_start_t.tm_year+1900,new_start_t.tm_mon+1,new_start_t.tm_mday,
					new_start_t.tm_hour,new_start_t.tm_min,new_start_t.tm_sec);
				printf("new_starttime=%s\n",shm_hdb_addr->data_revert.starttime);
				/*
				sprintf(shm_hdb_addr->data_revert.endtime,"%d-%d-%d %d:%d:%d",end_time.year,end_time.month,end_time.day,
				end_time.hour,end_time.minute,end_time.second);
				strcpy(shm_hdb_addr->data_revert.starttime,tmp);*/
				MySleep(1);
			}
		}
		else
            goto endThr;  
	}
	
endThr:
#ifdef SUN_OS
    pthread_exit(0);
#endif
#ifdef WINDOWS_OS
	ExitThread(0) ; 
#endif
	return (NULL);
}
