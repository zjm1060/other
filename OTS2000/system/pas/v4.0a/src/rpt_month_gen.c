#include	"../../../dms/v4.0a/inc/dms_com.h"
#include        "../../../dms/v4.0a/inc/os_type_def.h"
#include        "../../../pas/v4.0a/inc/rpt_def.h"
#include        "../../../pas/v4.0a/inc/pdc_def.h"

void GenMonthRptName();
int  CreateInitData();

RPT_DATA     buf1[CLE];

RPT_DATA     tmp[H_MAX];



static char HM_time[20],HM_name[20],HM_full_name[60];

int    Min,Hour,Day,Mon,Year;

short  Day_sav;



struct tm timeptr1;

time_t t1;
static char   month_data[60];
FILE   	*fp_month_def, *fp_month_data;
char   monthly_name_def1[H_MAX][RPT_NAME_SIZE],month_file_def[60],err_str[128];



void GenMonthRptName0()

{

	/*     if(timeptr1.tm_hour==0)*/

	if((timeptr1.tm_hour==0) && (timeptr1.tm_min ==0)) t1= t1 - 23 * 3600;                     /*  for get yesterday  daily  */


	timeptr1=*localtime(&t1);
	

	sprintf(HM_time,"%04d%02d",timeptr1.tm_year+1900,timeptr1.tm_mon+1);

	strcpy(HM_name,"HM");

	strcat ( HM_name,HM_time);

	strcat (HM_name,".DATA");

	strcpy(HM_full_name,RPTLOG_NAME_PATH);

	strcat( HM_full_name,HM_name);

	strcpy(month_data, HM_full_name);                

}



int CreateInitData()

{

	int  i,pnt_number,pnt_number0;


	strcpy(month_file_def,RPTDEF_NAME_PATH);
	strcat(month_file_def,"monthly_rpt_file.def");
	if( (fp_month_def = fopen(month_file_def,"r") ) == NULL)            /*added by hfz 050908  ,load def-->DATA*/

	{	

		sprintf ( err_str,"rpt_month_gen.c;EC:001; cannot open monthly_rpt_file.def !!!");                              

		Err_log(err_str);		     

		return(-1);

	}

	else

	{                         

		fscanf( fp_month_def,"%d",&month_rpt_pnt_number); 

		fscanf( fp_month_def,"%d",&lg_number);

		printf("month_rpt_pnt_number=%d,lg_number=%d\n",month_rpt_pnt_number,lg_number);



		for(i = 0; i<month_rpt_pnt_number ; i++)

		fscanf(fp_month_def,"%s",monthly_name_def[i]);

		

		fclose(fp_month_def);

	}



	fp_month_data = fopen(month_data,"wb+");	             

	pnt_number = month_rpt_pnt_number;

	pnt_number0 = lg_number;
	
	Swap4Byte(&pnt_number);

	Swap4Byte(&pnt_number0);             


	fwrite(&pnt_number,sizeof(int),1,fp_month_data);                                   

	fwrite(&pnt_number0,sizeof(int),1,fp_month_data);                 

	

	for(i = 0; i < month_rpt_pnt_number ; i++)

	fwrite(&monthly_name_def[i],sizeof(char),RPT_NAME_SIZE,fp_month_data);      /* hfz 050908, end load def   */

	

	buf1[0].value.float_data = 0;

	for(i = 0; i < month_rpt_pnt_number * TIM_month; i++)

	{

		if( fwrite(&buf1[0],sizeof(RPT_DATA),1,fp_month_data) != 1)

		{

			sprintf(err_str,"rpt_month_gen;EC:002; init MD_full_name file fwrite error");

			Err_log(err_str);	

			fclose(fp_month_data);

			return(-1);

		}

	}

	fclose(fp_month_data);

	return(0);

}

void LogWrite(char *log)
{
	FILE *fp;
	DATE_TIME date_time;
	char file_name[128];

	GetSysTime(&date_time);
	sprintf(file_name,"/home/ems/h9000/his/rpt/rtp_%04d-%02d-%02d.log",date_time.year,date_time.month, date_time.day);
	if(fp=fopen(file_name, "a+"))
	{
		fprintf(fp,"%04d-%02d-%02d %02d:%02d:%02d.%03d : %s\n", date_time.year,date_time.month, date_time.day, date_time.hour,date_time.minute, date_time.second, date_time.ms, log);
		fseek(fp,0L,2);
		fclose(fp);
	}

}

void *rptm(PDC_TASK *pTask)
{   

	int    	i,l,create_init_flag,run_flag;       

	DMS_COMMON    common_am;
	static  char  ch[L_MAX];
	char	log[256];   


	for(;;)
	{
		sleep(pTask->check_period);

		if((run_flag = PdcCheck(pTask))!=0) 
		continue;

		printf("\n------RPT_MONTH------%d\n", time(NULL));

		sleep(1);					/* gj 080504 ,run after calcu */

		t1 = time(NULL);                          
		timeptr1=*localtime(&t1);        
		Hour=timeptr1.tm_hour;
		Day=timeptr1.tm_mday;
		Min =timeptr1.tm_min;
		GenMonthRptName0();



		if(access(month_data,06)==-1)     /*   if  file not exist    */

		{

			create_init_flag = CreateInitData();

			if(create_init_flag == -1) continue;

		}             

		else 

		{   

			fp_month_data = fopen(month_data,"rb+");	           

			fread(&month_rpt_pnt_number,sizeof(int),1,fp_month_data);           

			fread(&lg_number,sizeof(int),1,fp_month_data);               


			Swap4Byte(&month_rpt_pnt_number);

			Swap4Byte(&lg_number);               

			

			for(i = 0; i < month_rpt_pnt_number ; i++)

			fread(&monthly_name_def[i],sizeof(char),RPT_NAME_SIZE,fp_month_data);  

			fclose(fp_month_data);                                             /* hfz 050908 ,end read def from DATA */

		}            


		for(i = 0; i<month_rpt_pnt_number - lg_number; i++)

		{

			l = strlen(monthly_name_def[i]);

			ch[i]=monthly_name_def[i][l-1];

			monthly_name_def[i][l-2] = '\0';

		}  


		/****   读脉冲、开合次数   ****/

		for(i = 0; i<month_rpt_pnt_number - lg_number; i++)             

		{	

			if ( -1 == ReadEntryByNameStr ( monthly_name_def[i], &common_am) )                        

			{

				sprintf ( err_str,"rpt_month_gen;EC:003; %s No this point in the database file !!!",monthly_name_def[i]);                              

				Err_log(err_str);

				tmp[i].value.float_data = 0;

			}

			else

			{

				switch(ch[i])

				{
					


				case    'H':


					if(common_am.var.imp.peak.day>24)//cxs 20120223
						common_am.var.imp.peak.day=24;//cxs 20120223
					tmp[i].value.float_data = common_am.var.imp.peak.day;


					break;



				case    'L':


					if(common_am.var.imp.valley.day>24)//cxs 20120223
						common_am.var.imp.valley.day=24;//cxs 20120223
					tmp[i].value.float_data = common_am.var.imp.valley.day;


					break;



				case    'S':

					

					tmp[i].value.float_data = common_am.var.imp.peak.month;

					break;



				case    'M':

					tmp[i].value.float_data = common_am.var.imp.valley.month;

					break;



				case    'T':



					tmp[i].value.float_data = common_am.var.imp.peak.year;

					break;



				case    'Y':



					tmp[i].value.float_data = common_am.var.imp.valley.year;

					break;



					

				case    'R':

					if(common_am.var.imp.normal.day>24)//cxs 20120223
						common_am.var.imp.normal.day=24;//cxs 20120223

					tmp[i].value.float_data = common_am.var.imp.normal.day;
					
					if(Hour==0 && Min<1 && Day==common_am.var.imp.entry.update_time.day)
					{
						if(common_am.var.imp.last.day>24)//cxs 20120223
						common_am.var.imp.last.day=24;//cxs 20120223

						tmp[i].value.float_data=common_am.var.imp.last.day;
						/*bzero(log,sizeof(log));
						sprintf(log,"point=%s data=%f,update_time=%4d-%02d-%02d %02d:%02d:%02d:%03d",monthly_name_def[i],tmp[i].value.float_data,
						common_am.var.imp.entry.update_time.year,common_am.var.imp.entry.update_time.month,common_am.var.imp.entry.update_time.day,
						common_am.var.imp.entry.update_time.hour,common_am.var.imp.entry.update_time.minute,common_am.var.imp.entry.update_time.second,
						common_am.var.imp.entry.update_time.ms);
						LogWrite(log);	*/
					}

					break;



				case    'U':



					tmp[i].value.float_data = common_am.var.imp.normal.month;

					break;



				case    'N':



					tmp[i].value.float_data = common_am.var.imp.normal.year;

					break;



				case    'J':


					if(common_am.var.imp.sum.day>24)//cxs 20120223
						common_am.var.imp.sum.day=24;//cxs 20120223

					tmp[i].value.float_data = common_am.var.imp.sum.day;	

					break;



				case    'K':



					tmp[i].value.float_data = common_am.var.imp.sum.month;

					break;



				case    'X':



					tmp[i].value.float_data = common_am.var.imp.sum.year;

					break;

					
				case    'I':


					if(common_am.var.imp.ppeak.day>24)//cxs 20120223
						common_am.var.imp.ppeak.day=24;//cxs 20120223
					tmp[i].value.float_data = common_am.var.imp.ppeak.day;

					break;

					

				case    'V':



					tmp[i].value.float_data = common_am.var.imp.ppeak.month;

					break;



				case    'P':



					tmp[i].value.float_data = common_am.var.imp.ppeak.year;

					break;
					
					/****   读开关动作次数  ****/               


				case    'Z':



					tmp[i].value.float_data = common_am.var.ind.stc.norm_open;

					break;



				case    'A':



					tmp[i].value.float_data = common_am.var.ind.stc.norm_close;

					break;

					

				case    'G':



					tmp[i].value.float_data = common_am.var.ind.stc.fault_open; 

					break;



				case    'B':



					tmp[i].value.float_data = common_am.var.ind.stc.fault_close;
					break;


				default:

					break;

				}

			}

		}



		/****   读计算量   ****/

		for(i = month_rpt_pnt_number - lg_number; i<month_rpt_pnt_number; i++)

		{

			if ( -1 == ReadEntryByNameStr ( monthly_name_def[i], &common_am) )                        

			{

				sprintf ( err_str,"rpt_month_gen;EC:004; %s No this calc point in the database file !!!",monthly_name_def[i]);                              

				Err_log(err_str);

				tmp[i].value.float_data = 0;

			}

			else		 

			tmp[i].value.float_data = common_am.var.calc.fValue;			 

		}



		/****   写数据   ****/

		if( ( fp_month_data = fopen(month_data,"rb+") ) == NULL )

		{

			sprintf(err_str,"rpt_month_gen;EC:005 cannot open %s",month_data);

			Err_log(err_str);

			continue;

		}

		fseek(fp_month_data,(long)month_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0); /*by hfz  ,skip   def  */

		

		if( Hour == 23 ) Day_sav = Day;

		Day = Day - 1;



		if(( Hour != 0 ) || ( Hour == 0 && Min >= 1 ))
		{/* add for 3minute */         

			fseek(fp_month_data,(long) Day * (long) month_rpt_pnt_number * sizeof(RPT_DATA),1);
			
		}

		else
		{

			fseek(fp_month_data,(long) ( Day_sav - 1 ) * (long) month_rpt_pnt_number * sizeof(RPT_DATA),1);   /*by hfz 0-->1*/   
		}

		for(i=0; i<month_rpt_pnt_number; i++)
		{
			Swap4Byte(&tmp[i].value.float_data);                    
		}
		



		if(fwrite(&tmp[0],sizeof(RPT_DATA),month_rpt_pnt_number,fp_month_data) != (size_t)month_rpt_pnt_number)

		{

			sprintf(err_str,"rpt_month_gen;EC:006; monthly_rpt_file.data write error");

			Err_log(err_str);

			fclose(fp_month_data);

			continue;
		}

		fclose(fp_month_data);



		if( ( fp_month_data = fopen(month_data,"rb+") ) == NULL )

		{

			sprintf(err_str,"rpt_month_gen;EC:006; cannot open %s",month_data);

			Err_log(err_str);              

			continue;

		}

		

		fseek(fp_month_data,(long)month_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);  /*by hfz ,skip   def  */

		fseek(fp_month_data,(long) 31 * (long) month_rpt_pnt_number * sizeof(RPT_DATA),1);   /*by hfz 0-->1*/

		

		if(fwrite(&tmp[0],sizeof(RPT_DATA),month_rpt_pnt_number,fp_month_data) != (size_t)month_rpt_pnt_number)

		{

			sprintf(err_str,"rpt_month_gen;EC:006; monthly_rpt_file.data write error");

			Err_log(err_str);

			fclose(fp_month_data);

			continue;

		}

		fclose(fp_month_data);
		
		/****** check  write  data ******/

#ifdef CHECK         /*调试时，检查DATA*/
		for(i = 0; i < month_rpt_pnt_number ; i++)

		{

			tmp[i].value.float_data  = 0;

		}



		month_rpt_pnt_number = 0;

		lg_number = 0;
		

		fp_month_data = fopen(month_data,"rb+");

		fread(&month_rpt_pnt_number,sizeof(int),1,fp_month_data);

		fread(&lg_number,sizeof(int),1,fp_month_data);


		Swap4Byte(&month_rpt_pnt_number);

		Swap4Byte(&lg_number);

		printf("@@@month_rpt_pnt_number=%d,lg_number=%d\n",month_rpt_pnt_number,lg_number);
		
		fclose(fp_month_data);

#endif
	}

}


