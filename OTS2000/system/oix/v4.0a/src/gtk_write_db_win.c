#include <gtk/gtk.h>
 
#include	"oix.h"
/*int sleepflag=0;*/
extern int fault_redisplay_on,step_id ;
extern struct tm fault_starttime,fault_endtime;
struct shm_hdb *shm_realdb_addr;

void	*WriteDbFromHDB(void *arg)	
{	
	POINTER		point;
	WR_VAR_UNION	new_value;
	IOST		iost;
	DATE_TIME	updatetime;
	int		i,j=0,k=0,no_val=0,crt_n=0;	
	char		label[50],tmp[30];
	GtkWidget  *lab_fault_redisplay;
	struct tm fault_time;
	time_t tTim;      
        char new_start_time[30],new_end_time[30];
        struct tm new_start_t;
        time_t new_tTim_start,old_tTim_start;

		gchar *pConvert=0;
	
	for(;;)
	{
#ifdef DEBUG
printf("11::queryflag=%d\n",shm_hdb_addr->data_revert.queryflag);
#endif
		if(j==0)
                    memcpy(shm_realdb_addr,shm_hdb_addr,sizeof(struct shm_hdb));
		if(fault_redisplay_on ==1)
		{
			if(shm_hdb_addr->data_revert.stopflag==1)
			{
				/*MySleep(1);*/
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
			lab_fault_redisplay = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"label_fault_redisplay");
     			if (lab_fault_redisplay != NULL)
			{
				strcpy(label,OIX_CSTR_FAULT_REDISPLAY_ON);					
			}
                        printf("***ycnum=%d\n",shm_hdb_addr->data_revert.ycnum);
			for(i=0; i<=shm_hdb_addr->data_revert.ycnum; i++)
			{				
				point = shm_hdb_addr->data_revert.yc_data[i].point;
                                
				new_value.anlg.status.chan_fail =0;
				new_value.anlg.status.ctrl_block =0;
				new_value.anlg.status.alm_type =0;
				new_value.anlg.status.trend_state =0;
				new_value.anlg.status.lmt_state =0;
				if(shm_hdb_addr->data_revert.yc_data[i].rscount==0||j>=shm_hdb_addr->data_revert.yc_data[i].rscount)
				{
					printf("yci=%d,j=%d,rscount=%d\n",i,j,shm_hdb_addr->data_revert.yc_data[i].rscount);
					continue;
				}
                                updatetime.year =  shm_hdb_addr->data_revert.yc_data[i].basetime.tm_year+1900;
				updatetime.month = shm_hdb_addr->data_revert.yc_data[i].basetime.tm_mon+1;
				updatetime.day = shm_hdb_addr->data_revert.yc_data[i].basetime.tm_mday;
				updatetime.hour = shm_hdb_addr->data_revert.yc_data[i].basetime.tm_hour;
				updatetime.minute= shm_hdb_addr->data_revert.yc_data[i].basetime.tm_min;
				updatetime.second= shm_hdb_addr->data_revert.yc_data[i].basetime.tm_sec;
				switch	( point.data_type )
				{
					case	ANA_TYPE:
					case	OUTA_TYPE:
					case	PARAM_TYPE:
						new_value.anlg.data= 1000*shm_realdb_addr->data_revert.yc_data[i].value[j];
/*#ifdef DEBUG*/
/*if(i<5) 
    {
printf("logical_name=%s,data=%d,value[%d][%d]=%.3f\n",shm_hdb_addr->data_revert.yc_data[i].logicalname,new_value.anlg.data,i,j,shm_realdb_addr->data_revert.yc_data[i].value[j]);
printf("time: second=%d,min=%d,hour=%d,day=%d,month=%d,year=%d\n",updatetime.second,updatetime.minute,updatetime.hour,updatetime.day,updatetime.month,updatetime.year);
}*/
/*#endif*/
						SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );
						break;

					case	CALC_TYPE:
						new_value.calc.data= shm_realdb_addr->data_revert.yc_data[i].value[j];
						SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );
						break;

					case	IMP_TYPE:
						new_value.imp.status.option= 0;
						new_value.imp.iValue= shm_realdb_addr->data_revert.yc_data[i].value[j];
						SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME);
						break;
							
					default:
						break;
				}			
			}
			/*for(i=0; i<=shm_hdb_addr->data_revert.yxnum; i++)
			{				
				point = shm_hdb_addr->data_revert.yx_data[i].point;				
                               
				if(shm_hdb_addr->data_revert.yx_data[i].rscount==0||j>=shm_hdb_addr->data_revert.yx_data[i].rscount)
				{				
                                    continue;
				}
                                else
                                    printf("yxi=%d,j=%d,rscount=%d\n",i,j,shm_hdb_addr->data_revert.yx_data[i].rscount);
                                updatetime.year =  shm_hdb_addr->data_revert.yx_data[i].basetime.tm_year+1900;
				updatetime.month = shm_hdb_addr->data_revert.yx_data[i].basetime.tm_mon+1;
				updatetime.day = shm_hdb_addr->data_revert.yx_data[i].basetime.tm_mday;
				updatetime.hour = shm_hdb_addr->data_revert.yx_data[i].basetime.tm_hour;
				updatetime.minute= shm_hdb_addr->data_revert.yx_data[i].basetime.tm_min;
				updatetime.second= shm_hdb_addr->data_revert.yx_data[i].basetime.tm_sec;
                                printf("poltime: second=%d,min=%d,hour=%d,day=%d,month=%d,year=%d\n",updatetime.second,updatetime.minute,updatetime.hour,updatetime.day,updatetime.month,updatetime.year);	
				switch	( point.data_type )
				{
					case	SOE_TYPE:
					case	POL_TYPE:
					case	OUTS_TYPE:
					case	SYSMSG_TYPE:					
						new_value.ind.rt_state= shm_realdb_addr->data_revert.yx_data[i].status[j];
						SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );											
						break;
				}				
			}*/
printf("111 time: second=%d,min=%d,hour=%d,day=%d,month=%d,year=%d\n",updatetime.second,updatetime.minute,updatetime.hour,updatetime.day,updatetime.month,updatetime.year);
                        fault_time.tm_sec = updatetime.second;
			fault_time.tm_min = updatetime.minute;
			fault_time.tm_hour= updatetime.hour;
			fault_time.tm_mday=updatetime.day;
			fault_time.tm_mon =updatetime.month-1;
			fault_time.tm_year=updatetime.year-1900;
			if(fault_time.tm_year<=0)
			{
                            j++;                           
                            sprintf(label,"%s","connectingHdb");
                            gtk_label_set_text(GTK_LABEL(lab_fault_redisplay), pConvert=_toUtf8( label));
							G_SAFE_FREE(pConvert);
                            Sleep(1); 
                            printf("tm_year<=0,updatetime.year=%d\n",updatetime.year);
                            continue;
                         }
			tTim = mktime(&fault_time);
			if(tTim!=-1)
			{
				tTim = tTim+j/*+k*MAX_REVERT_REC*//*-no_val*/;
                                
				fault_time=*localtime(&tTim);
				if(j==0)
                                printf("k=%d,tm_min=%d\n",k,fault_time.tm_min);
				sprintf(tmp,"%d-%d-%d %d:%d:%d",fault_time.tm_year+1900,fault_time.tm_mon+1,fault_time.tm_mday,
					fault_time.tm_hour,fault_time.tm_min,fault_time.tm_sec);
				strcat(label,tmp);

         			gtk_label_set_text(GTK_LABEL(lab_fault_redisplay), pConvert=_toUtf8( label));
					G_SAFE_FREE(pConvert);
         		}
         		else
         		{
         			printf("mktime error!,sec=%d,min=%d,hour=%d,day=%d,mon=%d,year=%d\n",fault_time.tm_sec,
         			fault_time.tm_min,fault_time.tm_hour,fault_time.tm_mday,fault_time.tm_mon,fault_time.tm_year);
         			sprintf(label,"time error!");
         			gtk_label_set_text(GTK_LABEL(lab_fault_redisplay), pConvert=_toUtf8( label));
					G_SAFE_FREE(pConvert);
         		}

			if(j==MAX_REVERT_REC-1)
			{
				j=0;
                                k++;                                                      
                       
                                old_tTim_start= mktime(&fault_starttime);
                                new_tTim_start = old_tTim_start+k*MAX_REVERT_REC;
                                new_start_t =*localtime(&new_tTim_start);
                                sprintf(shm_hdb_addr->data_revert.starttime,"%d-%d-%d %d:%d:%d",new_start_t.tm_year+1900,new_start_t.tm_mon+1,new_start_t.tm_mday,
                                    new_start_t.tm_hour,new_start_t.tm_min,new_start_t.tm_sec);
                                printf("new_starttime=%s\n",shm_hdb_addr->data_revert.starttime);
                                printf("before memcpy::shm_hdb_addr->data_revert.starttime=%s\n",shm_hdb_addr->data_revert.starttime);
				shm_hdb_addr->data_revert.queryflag = 1; 
                                Sleep(1);
                                memcpy(shm_realdb_addr,shm_hdb_addr,sizeof(struct shm_hdb));
			}
			else
                        {
				j++;
                                if(j+k*MAX_REVERT_REC>(int)arg)
                                    goto ExitThr;                       
                            
                                Sleep(1); 
                        }
                        
/*#ifdef DEBUG	*/
printf("in WriteDb:: arg=%d,j=%d\n",arg,j);
/*#endif*/
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
#ifdef DEBUG   
	 printf("\nthread %d return \n", pthread_self());
#endif	 
	pthread_exit(0);
     return (NULL);	
}

void *PeriodRequestData(void *arg)
{
	int i=0,j=0;
	char new_start_time[30],new_end_time[30];
        struct tm new_start_t;
        time_t new_tTim_start,old_tTim_start;
	for(;;)
	{
            if(fault_redisplay_on ==1)
            {
                if(shm_hdb_addr->data_revert.stopflag==1)
                {
                    Sleep(1);
                /* printf("stopflag=%d\n",shm_hdb_addr->data_revert.stopflag);*/
                    continue;
            }
            if(shm_hdb_addr->data_revert.querytype ==STEP_BY_STEP&&step_id==1)
            {
		Sleep(1);
              /* printf("querytype=STEP_BY_STEP&&step_id==1\n");*/
                continue;
            }
		if(i<MAX_REVERT_REC)
		{
                    printf("i=%d\n",i);
			i++;	
			Sleep(1);                        
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
			Sleep(1);
		}
            }
            else
            goto endThr;  
        }
          
endThr:
	pthread_exit(0);
     return (NULL);
}
