#include	"../../../dms/v4.0a/inc/dms_com.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"
#include    "../../../pas/v4.0a/inc/rpt_def.h"
#include    "../../../pas/v4.0a/inc/pdc_def.h"


void GenHourRptFileName();
void ReadData();
int  InitRptFile();

/* #define DEBUG */

RPT_DATA	    bufh1[CLE];
RPT_DATA     	tmph[H_MAX];
RPT_DATA     	max[H_MAX];
RPT_DATA     	min[H_MAX];

static char      HD_time[20],HD_name[20],HD_full_name[60];

struct tm   timeptr2;
time_t 	    t2;

int    hour, hour1, day;
static char   	hour_data1[60];
char   err_str[128]; 

/*****debug  tmph val  *****/

char    hourly_name_def1[H_MAX][RPT_NAME_SIZE];
float   tmp1[H_MAX];

void GenHourRptFileName ()
{
	timeptr2=*localtime (&t2);
#ifdef ENGLISH_VER
	sprintf (HD_time,"%02d-%s-%04d" ,timeptr2.tm_mday,mthstr[timeptr2.tm_mon],timeptr2.tm_year+1900);
#else
	sprintf (HD_time,"%04d%02d%02d",timeptr2.tm_year+1900,timeptr2.tm_mon+1,timeptr2.tm_mday);
#endif
	strcpy(HD_name,"HD");
	strcat ( HD_name,HD_time);
	strcat (HD_name,".DATA");
	strcpy(HD_full_name,RPTLOG_NAME_PATH);
	strcat( HD_full_name,HD_name);
	strcpy(hour_data1, HD_full_name);
}


void ReadData()
{
	int i;
	DMS_COMMON common_ah;

	for (i = 0; i<hour_rpt_pnt_number; i++)
	{
		if (i<hour_rpt_pnt_number - hour_rpt_pnt_number0)      /*  ANA    */
		{
			if ( -1 == ReadEntryByNameStr ( hourly_name_def[i], &common_ah) )               
			{
				sprintf ( err_str,"rpt_hour_gen;EC:001; %s No this point in the database file !!!",hourly_name_def[i]);                              
				Err_log(err_str);		         
				tmph[i].value.float_data = 0;
			}
			else                      	
			tmph[i].value.float_data = common_ah.var.anlg.fValue;	 
		}

		else
		{
			if ( (i >= hour_rpt_pnt_number - hour_rpt_pnt_number0) && (i<hour_rpt_pnt_number) ) //CALC
			{
				if ( -1 == ReadEntryByNameStr ( hourly_name_def[i], &common_ah) )
				{
					sprintf ( err_str,"rpt_hour_gen;EC:002; %s No this point in the database file !!!",hourly_name_def[i]);
					Err_log(err_str);			         
					tmph[i].value.float_data = 0;
				}
				else                            	
				tmph[i].value.float_data  = common_ah.var.calc.fValue;
			}
		}
	}
}


int InitRptFile()
{
	int  i,pnt_number,pnt_number0;
	FILE *fp_hour_def,*fp_hour_data1;
	char hour_file_def[150];

	strcpy(hour_file_def,RPTDEF_NAME_PATH);
	strcat(hour_file_def,"hourly_rpt_file.def");
	if ( ( fp_hour_def = fopen(hour_file_def,"r") ) == NULL)              /*added by hfz 050908 ,load def-->DATA*/
	{
		sprintf (err_str,"rpt_hour_gen;EC:003; cannot open hourly_rpt_file.def");
		Err_log(err_str);
		printf("cannot open hourly_rpt_file.def\n");
		return(-1);
	}
	else
	{
		fscanf ( fp_hour_def,"%d",&hour_rpt_pnt_number);
		fscanf ( fp_hour_def,"%d",&hour_rpt_pnt_number0);

		for (i = 0; i<hour_rpt_pnt_number; i++)         
		fscanf ( fp_hour_def,"%s",hourly_name_def[i]);          
		fclose ( fp_hour_def );
	}  

	if ( ( fp_hour_data1 = fopen(hour_data1,"wb+") )== NULL)
	{ 
		printf("cannot open hour_data1\n");
		return(-1);
	}

	pnt_number = hour_rpt_pnt_number;
	pnt_number0 = hour_rpt_pnt_number0;

	Swap4Byte(&pnt_number);
	Swap4Byte(&pnt_number0);

	if ( fwrite(&pnt_number,sizeof(int),1,fp_hour_data1) != 1)  /* write data num1 */   
	{ printf("write data num1 err\n");  return(-1);  }
	if ( fwrite(&pnt_number0,sizeof(int),1,fp_hour_data1) != 1) /* write data num2 */
	{  printf("write data num2 err\n");  return(-1); }          

	for(i = 0; i < hour_rpt_pnt_number ; i++)     
	{
		if ( fwrite(&hourly_name_def[i],sizeof(char),RPT_NAME_SIZE,fp_hour_data1) != RPT_NAME_SIZE)    /* write string name */	                                                                
		{ printf("write string name err\n");  return(-1);  }
	}

	bufh1[0].value.float_data = 0;   /*   清零   */  

	for (i = 0; i < hour_rpt_pnt_number * TIM_hour; i++)
	{                                        
		if ( fwrite (&bufh1[0],sizeof (RPT_DATA),1,fp_hour_data1) != 1)
		{
			sprintf (err_str,"rpt_hour_gen;EC:004; HD_full_name file fwrite error");
			Err_log(err_str);
			printf("HD_full_name file fwrite error\n");
			return(-1);
		}  
	}   

	ReadData();

	/*  init  max  and  min    */
	for (i = 0; i<hour_rpt_pnt_number; i++)
	min[i] = max[i] = tmph[i];        

	/*    write MIN      */
	fseek ( fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);          /* added by hfz ,skip def */
	fseek ( fp_hour_data1,(long) MIN_pt * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1); 

	for (i = 0; i<hour_rpt_pnt_number; i++)          /******   add min 2003.4.1  ********/
	Swap4Byte(&min[i].value.float_data);

	if ( fwrite (&min[0],sizeof (RPT_DATA),hour_rpt_pnt_number,fp_hour_data1) != (unsigned int)hour_rpt_pnt_number)
	{
		sprintf (err_str,"rpt_hour_gen;EC:005; MIN_pt_file write error");
		Err_log(err_str);
		printf("MIN_pt_file write error\n");
		fclose ( fp_hour_data1);
		return(-1);
	}



	/*    write MAX      */

	fseek ( fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);             /* added by hfz ,skip def */
	fseek ( fp_hour_data1,(long) MAX_pt * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1);  

	for (i = 0; i<hour_rpt_pnt_number; i++) 
	Swap4Byte(&max[i].value.float_data);                               

	if ( fwrite (&max[0],sizeof (RPT_DATA),hour_rpt_pnt_number,fp_hour_data1) != (unsigned int)hour_rpt_pnt_number)
	{
		sprintf (err_str,"rpt_hour_gen;EC:006; MAX_pt_file write error");
		Err_log(err_str);
		printf("MAX_pt_file write error\n");
		fclose ( fp_hour_data1);
		return(-1);        
	}  

	fclose ( fp_hour_data1);
	return(0);
}       


void *rpth(PDC_TASK *pTask)
{
	int    i,create_init_flag,run_flag;
	FILE   *fp_hour_data1;
	float  max_val[H_MAX],min_val[H_MAX];

	for(;;)
	{
		sleep(pTask->check_period);
		if((run_flag = PdcCheck(pTask))!=0) 
		continue;         

		
		printf("\n------RPT_HOUR ------%d\n", time(NULL)); 
		
		t2 = time(NULL);				           /*    get  second   from  1970  to  present    */
		timeptr2=*localtime (&t2);			       /*    get  local time    */
		hour=timeptr2.tm_hour;	 
		if (timeptr2.tm_hour >= RPT_START_TIME )   /*    RPT_START_TIME 在头文件中定义   */
		GenHourRptFileName ();                 /*    创建数据文件名   */
		else{
			t2 = t2 - RPT_START_TIME*3600;
			GenHourRptFileName ();
		}         

		if (access(hour_data1,06)==-1)		           /*    if  file not exist    */
		{
			create_init_flag = InitRptFile();
			if ( create_init_flag == -1) continue;
		}
		else
		{
			if ( ( fp_hour_data1 = fopen(hour_data1,"rb") )== NULL)
			printf("cannot open hour_data1\n");
			if ( fread(&hour_rpt_pnt_number,sizeof(int),1,fp_hour_data1) !=1)   /* read data num1 */
			printf("read data num1 err\n");
			if ( fread(&hour_rpt_pnt_number0,sizeof(int),1,fp_hour_data1) !=1)  /* read data num2 */
			printf("read data num2 err\n ");

			Swap4Byte(&hour_rpt_pnt_number);
			Swap4Byte(&hour_rpt_pnt_number0);
			
			for(i = 0; i < hour_rpt_pnt_number ; i++)		 
			{
				if ( fread(&hourly_name_def[i],sizeof(char),RPT_NAME_SIZE,fp_hour_data1) !=RPT_NAME_SIZE) /* read  string name */
				printf("read  string name err\n");
			}		 

			fclose(fp_hour_data1);
			ReadData();
		}	     

		if ( ( fp_hour_data1 = fopen(hour_data1,"rb+") ) == NULL)
		{
			sprintf (err_str,"rpt_hour_gen;EC:007; need creat HD_full_name");
			Err_log(err_str);
			printf(" need creat HD_full_name\n");
			continue;
		}

		if   (hour == 0) hour1 = 23;
		else  hour1 = hour - 1;

		fseek ( fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);           /* added by hfz ,skip def */
		fseek ( fp_hour_data1,(long) hour1 * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1);    /* by hfz ,0-->1 */

		for (i = 0; i<hour_rpt_pnt_number; i++)  
		Swap4Byte(&tmph[i].value.float_data);        
		
		if ( fwrite (&tmph[0],sizeof (RPT_DATA),hour_rpt_pnt_number,fp_hour_data1) != (unsigned int)hour_rpt_pnt_number)
		{
			sprintf (err_str,"rpt_hour_gen;EC:008; HD_full_name write error");
			Err_log(err_str); 
			printf("HD_full_name write error\n");
			fclose ( fp_hour_data1);
			continue;
		}
		fclose ( fp_hour_data1);


		for (i = 0; i<hour_rpt_pnt_number; i++)
		{

			Swap4Byte(&tmph[i].value.float_data);
			max_val[i] = min_val[i] =tmph[i].value.float_data;
		}

		/* calculate min value */ 	

		if ( ( fp_hour_data1 = fopen( hour_data1,"rb+") )== NULL)		 
		printf("not open hour_data1\n");

		fseek(fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);    /* skip def */
		fseek ( fp_hour_data1,(long) MIN_pt * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1);

		if ( fread(&min[0],sizeof (RPT_DATA),hour_rpt_pnt_number,fp_hour_data1) !=(unsigned int) hour_rpt_pnt_number)
		{
			sprintf ( err_str,"rpt_hour_gen.c;EC:009; MIN_pt_file read error");
			Err_log(err_str);
			printf("MIN_pt_file read error\n");
			fclose ( fp_hour_data1);
			continue;
		}

		fclose ( fp_hour_data1);

		for (i = 0; i<hour_rpt_pnt_number; i++)
		Swap4Byte(&min[i].value.float_data);

		for (i = 0; i<hour_rpt_pnt_number; i++)
		{
			if ( min_val[i] <= min[i].value.float_data )
			min[i].value.float_data = min_val[i];
		}

		if ( ( fp_hour_data1 = fopen( hour_data1,"rb+") )== NULL)		 
		printf("not open hour_data1\n");      
		
		fseek(fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);    /* skip def */
		fseek ( fp_hour_data1,(long) MIN_pt * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1);

		for (i = 0; i<hour_rpt_pnt_number; i++)
		Swap4Byte(&min[i].value.float_data);

		if ( fwrite (&min[0],sizeof (RPT_DATA),hour_rpt_pnt_number ,fp_hour_data1) != (unsigned int)hour_rpt_pnt_number )
		{
			sprintf ( err_str,"rpt_hour_gen;EC:010; hourly_rpt_file.data write min error");
			Err_log(err_str);
			printf("hourly_rpt_file.data write min error\n");
			fclose ( fp_hour_data1);
			continue;
		}

		fclose ( fp_hour_data1);

		



		/* calculate max value */

		if ( ( fp_hour_data1 = fopen( hour_data1,"rb+") )== NULL)		  
		printf("not open hour_data1\n");

		fseek(fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);    /* skip def */
		fseek ( fp_hour_data1,(long) MAX_pt * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1);

		if ( fread(&max[0],sizeof (RPT_DATA),hour_rpt_pnt_number,fp_hour_data1) != (unsigned int)hour_rpt_pnt_number)
		{
			sprintf ( err_str,"rpt_hour_gen.c;EC:011; MAX_pt_file read error");
			Err_log(err_str);
			printf("MAX_pt_file read error\n");
			fclose ( fp_hour_data1);
			continue;
		}

		for (i = 0; i<hour_rpt_pnt_number; i++)  
		Swap4Byte(&max[i].value.float_data);

		for (i = 0; i<hour_rpt_pnt_number; i++) 
		{
			if ( max_val[i] >= max[i].value.float_data )
			max[i].value.float_data = max_val[i];
		}
		
		for (i = 0; i<hour_rpt_pnt_number; i++)  
		Swap4Byte(&max[i].value.float_data);


		fseek(fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);    /* skip def */
		fseek ( fp_hour_data1,(long) MAX_pt * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1);

		if ( fwrite (&max[0],sizeof (RPT_DATA),hour_rpt_pnt_number,fp_hour_data1) != (unsigned int)hour_rpt_pnt_number)
		{
			sprintf ( err_str,"rpt_hour_gen.c;EC:012; HD_full_name write error");
			Err_log(err_str);
			printf("HD_full_name write error\n");
			fclose ( fp_hour_data1);
			continue;
		}

		fclose ( fp_hour_data1);

		
		/****** check  write  data ******/



#ifdef CHECK         /* 调试时，检查DATA */


		for(i = 0; i < hour_rpt_pnt_number ; i++)         /* init */   
		tmph[i].value.float_data = 0;

		hour_rpt_pnt_number = 0;
		hour_rpt_pnt_number0 = 0;

		if ( ( fp_hour_data1 = fopen( hour_data1,"rb+") )== NULL)   	  
		printf("not open hour_data1\n");
		
		fread(&hour_rpt_pnt_number,sizeof(int),1,fp_hour_data1);  /* read data num1 */
		fread(&hour_rpt_pnt_number0,sizeof(int),1,fp_hour_data1); /* read data num2 */

		Swap4Byte(&hour_rpt_pnt_number);
		Swap4Byte(&hour_rpt_pnt_number0);

		
		/**** read  string name  *****/    

		for(i = 0; i < hour_rpt_pnt_number ; i++)
		{
			fread(&hourly_name_def1[i],sizeof(char),RPT_NAME_SIZE,fp_hour_data1);  
			/*printf("hourly_name_def1[%d]=%s\n",i,hourly_name_def1[i]);*/
		}  
		
		/***** HOUR DATA  START*****/         		 

		fseek(fp_hour_data1,(long) hour_rpt_pnt_number * RPT_NAME_SIZE+RPT_FILE_HEAD_SIZE,0);
		fseek ( fp_hour_data1,(long) hour1 * (long) hour_rpt_pnt_number * sizeof (RPT_DATA),1);
		if(fread(&tmph[0],sizeof(RPT_DATA),hour_rpt_pnt_number,fp_hour_data1) != (unsigned int)hour_rpt_pnt_number)
		{
			printf("\nrpt_hour_gen.c:: hourly_rpt_file.data read error\n");
			fclose(fp_hour_data1);
			continue;
		}
		
		printf("**********HOUR=%d*********\n",hour1);
		
		for(i = 0; i < hour_rpt_pnt_number ; i++)
		{
			Swap4Byte(&tmph[i].value.float_data);		   
			printf("D%d=%f   ",i,tmph[i].value.float_data);
		}
		
		printf("\n");

		fclose(fp_hour_data1);



#endif
	}

}
