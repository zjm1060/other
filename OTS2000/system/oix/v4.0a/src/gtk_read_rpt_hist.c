/******************************************
SYNC by hcl 2007.12.09
*******************************************/
#include <stdio.h>

#include	"../inc/oix.h"
 
#include "/home/ems/system/pas/v4.0a/inc/rpt_def.h"

#define	FILE_NAME_LEN	40

/*#define DEBUG*/

#define	REQ_HOURLY	0
#define	REQ_DAYLY	1
#define	REQ_MONTHLY	2
#define	REQ_YEARLY	3
#define FILE_NUM        5

#define	NO_PEAK_TYPE	0	
#define	PEAK		1
#define	PPEAK1		2                        /*    modified for v3.0  01/11/26 */
#define VALLY1           3			
#define OPN_NUM         4
#define CLS_NUM         5
#define SG_OPN_NUM      6
#define SG_CLS_NUM      7
#define TST_OPN_NUM     8
#define TST_CLS_NUM     9
#define DAY_MAX         10
#define NO_PKVLY	11
#define SUM_PKVLY_ALL	12

#define FRET            0.0

char   *hour_file_def  = {"/home/ems/h9000/def/hourly_rpt_file.def"};
char   *month_file_def = {"/home/ems/h9000/def/monthly_rpt_file.def"};

RPT_DATA	*err_openfile()
{
	RPT_DATA	* return_data;

	printf("cannot open file\n");
	return_data=(RPT_DATA *)-1;
	return(return_data);
}


/*****   rpt_hist_name:HD*.DATA  HM*.DATA    ******/
int ReadRptEntryByName ( name, req_type, time_id, pk_vly_type,  rpt_hist_name, rpt_data_type, rpt_data)
char	name[FILE_NAME_LEN];
short	req_type;
short	time_id;
short	pk_vly_type;	
char	rpt_hist_name[80];
int	*rpt_data_type;
RPT_DATA	*rpt_data;
{
	int 		i,j;
	FILE		*fp_hour_data, /**fp_date_data,*/  *fp_month_data/*, *fp_month_save_data*//*,*fp_year_data*/;

	char            day_peak[3]      = "_H\0";
        char            day_vally[3]     = "_L\0";
	char            month_peak[3]    = "_S\0";
	char            month_vally[3]   = "_M\0";
	char            year_peak[3]     = "_T\0";
	char            year_vally[3]    = "_Y\0";
	char            day_aver[3]      = "_R\0";
	char		day_total[3]	 = "_J\0";
	char		month_aver[3]    = "_U\0";
	char		month_total[3]   = "_K\0";
	char            year_aver[3]     = "_N\0";
	char            year_total[3]    = "_X\0";		
	char		day_sum[3]       = "_D\0";
	char		month_sum[3]     = "_M\0";
	char		year_sum[3]      = "_Y\0";
	char            month_opn[3]     = "_Z\0";
	char            month_cls[3]     = "_A\0";
	char            month_sg_opn[3]  = "_G\0";
	char            month_sg_cls[3]  = "_B\0";
	char            month_tst_opn[3] = "_C\0";
	char            month_tst_cls[3] = "_E\0";
        char            month_max[3]     = "_W\0";
	char            day_ppeak[3]     = "_I\0";		/*      modified for v3.0  01/11/26 */
	char            month_ppeak[3]   = "_V\0";		/*      modified for v3.0  01/11/26 */
	char            year_ppeak[3]    = "_P\0";		/*      modified for v3.0  01/11/26 */
	

	char		name_tmp[POINT_NAME_SIZE]={""};
//	int		data_type;
//	float   	rt_value;


/******* added  by hfz20050905   rpt def from DATA *******/
    switch (req_type)
	   {
	     case   REQ_HOURLY:
		    *rpt_data_type = 1;			
		    break;

	     case   REQ_DAYLY:
	     case   REQ_MONTHLY:
	     case   REQ_YEARLY:
		    *rpt_data_type = 2;			
		    break;

	     default:
		    break;
	    }

    switch(*rpt_data_type)
	  {
	    case 1:
	                  
			if(( fp_hour_data = fopen( rpt_hist_name,"rb+")) == NULL)            			
			  {       
		            printf("RPT:: read_rpt_hist.c cannot open rpt_hist_name=%s\n",rpt_hist_name);
		            		      
			    return( -1);
			   }
		
	                else
			  {   
		            fread(&hour_rpt_pnt_number,sizeof(int),1,fp_hour_data);  
                            fread(&hour_rpt_pnt_number0,sizeof(int),1,fp_hour_data);
                            
                            #ifdef SUN_OS
                            Swap4Byte(&hour_rpt_pnt_number);
                            Swap4Byte(&hour_rpt_pnt_number0);
                            #endif 
                            
		            for( i=0; i<hour_rpt_pnt_number; i++)
			       {			     
				 fread(&hourly_name_def[i],sizeof(char),75,fp_hour_data);				 				 
			        }
		            fclose(fp_hour_data);			
			   }
			break;

	     case  2:

                        if((fp_month_data=fopen(rpt_hist_name, "rb+"))==NULL)
			  {	
		            printf("RPT:: read_rpt_hist.c cannot open rpt_hist_name=%s\n",rpt_hist_name);
		            /*return ( (RPT_DATA *)-1);*/
		            return( -1);
			   }
        
			else
			  {
		            fread(&month_rpt_pnt_number,sizeof(int),1,fp_month_data);
                            fread(&lg_number,sizeof(int),1,fp_month_data);
                            
                            #ifdef SUN_OS
                            Swap4Byte(&month_rpt_pnt_number);
                            Swap4Byte(&lg_number);
                            #endif 
                            
	       	            for(i=0;i<month_rpt_pnt_number;i++)
			       {
       			         fread(&monthly_name_def[i],sizeof(char),75,fp_month_data);
			        }
      	 	            fclose(fp_month_data);
			   }
			break;

	      default:
		        break;

           }

/******* end  hfz20050905*******/





	strcpy( name_tmp, name);

#ifdef DEBUG
	printf(" ReadRptEntrybyName: name=%s, type=%d, time_id=%d\n", name, data_type, time_id);
#endif  

	switch (req_type)
	{
	case	REQ_HOURLY: 

		*rpt_data_type = 1;

		for( i=0; i<hour_rpt_pnt_number; i++)
       		{
			if( i < hour_rpt_pnt_number - hour_rpt_pnt_number0) 
				*rpt_data_type = 1;		/*     for    ANA      */
			else
				*rpt_data_type  = 2;		/*     for    CALC     */		

                       	if(strcmp(name_tmp, hourly_name_def[i])==0)
                       	{
                                j=i;
               	                break;
                       	}
		}
		if(i >= hour_rpt_pnt_number) return(-1); 

		if(( fp_hour_data = fopen( rpt_hist_name,"rb")) == NULL)
		{
			printf ("in RPT:: rpt_hist_name not open %s\n",rpt_hist_name);
	                return(-1);
		}
                fseek(fp_hour_data,(long) hour_rpt_pnt_number * 75+8,0);      /*     added by hfz 050908  */
              /*  printf("j=%d,time_id=%d,hour_rpt_pnt_number=%d\n",j,time_id,hour_rpt_pnt_number);*/
                fseek(fp_hour_data,(long)( j + time_id * hour_rpt_pnt_number) * sizeof(RPT_DATA),1);   /* 0->1 mod by hfz 050908   */
               	if( !fread(rpt_data, sizeof(RPT_DATA), 1, fp_hour_data) )
		{
			fclose(fp_hour_data);
			printf ("HIST_RPT:%s is not found in rpt table.\n", name);
                       	return( -1);
		}

/*if (j<hour_rpt_pnt_number - hour_rpt_pnt_number0)
     {
#ifdef SUN_OS
Swap2Byte(&rpt_data->value.anlg_data.status);
Swap2Byte(&rpt_data->value.anlg_data.data);
#endif
if(0 != rpt_data->value.anlg_data.data)
	printf("\nanlg_data= %d\n", rpt_data->value.anlg_data.data);
      }
else
  if ( (j >= hour_rpt_pnt_number - hour_rpt_pnt_number0) && (j<hour_rpt_pnt_number) )*/
     {
#ifdef SUN_OS
Swap4Byte(&rpt_data->value.float_data);
#endif
/*if(0 != rpt_data->value.float_data)
	printf("\n name=%s,float_data= %f\n", name,rpt_data->value.float_data);*/
      }

	/*printf("in REQ_HOURLY:rpt_data->value.float_data=%.1f\n",rpt_data->value.float_data);*/
		fclose(fp_hour_data);
               	return(1);
	       
		break;
	

	case	REQ_DAYLY:
		*rpt_data_type = 2;

		if( pk_vly_type == PEAK)
			strcat(name_tmp, day_peak);
		else if( pk_vly_type == PPEAK1)
			strcat(name_tmp, day_ppeak);          /* modified for v3.0 01.11.23 */ 
		else if( pk_vly_type == VALLY1)
			strcat(name_tmp, day_vally);
		else if( pk_vly_type == NO_PKVLY)
			strcat(name_tmp,day_aver);		/*     add     */
                else if( pk_vly_type == SUM_PKVLY_ALL)
                        strcat(name_tmp,day_total);             /*     add     */
		else if( pk_vly_type == NO_PEAK_TYPE)
			strcat(name_tmp, day_sum);
		else if( pk_vly_type == OPN_NUM)
                        strcat(name_tmp,month_opn);
                else if( pk_vly_type == CLS_NUM)
                        strcat(name_tmp,month_cls);
                else if( pk_vly_type == SG_OPN_NUM)
                        strcat(name_tmp,month_sg_opn);
                else if( pk_vly_type == SG_CLS_NUM)
                        strcat(name_tmp,month_sg_cls);
                else if( pk_vly_type == TST_OPN_NUM)
                        strcat(name_tmp,month_tst_opn);
                else if( pk_vly_type == TST_CLS_NUM)
                        strcat(name_tmp,month_tst_cls);

                else if( pk_vly_type == DAY_MAX)
                        strcat(name_tmp,month_max);
		break;


	case	REQ_MONTHLY:

		*rpt_data_type = 2;
                     if( pk_vly_type == PEAK)
			strcat(name_tmp, month_peak);
		else if( pk_vly_type == PPEAK1)
			strcat(name_tmp, month_ppeak);		/*      modified for v3.0  01/11/23 */
		else if( pk_vly_type == VALLY1)
			strcat(name_tmp,month_vally);
	 	else if( pk_vly_type == NO_PKVLY)			
			strcat(name_tmp,month_aver);		/*     add     */
		else if( pk_vly_type == SUM_PKVLY_ALL)		
			strcat(name_tmp,month_total);		/*     add     */
		else if( pk_vly_type == NO_PEAK_TYPE)
			strcat(name_tmp, month_sum);
		else if( pk_vly_type == OPN_NUM)
			strcat(name_tmp,month_opn);
		else if( pk_vly_type == CLS_NUM)
			strcat(name_tmp,month_cls);
		else if( pk_vly_type == SG_OPN_NUM)
			strcat(name_tmp,month_sg_opn);
		else if( pk_vly_type == SG_CLS_NUM)
			strcat(name_tmp,month_sg_cls);
		else if( pk_vly_type == TST_OPN_NUM)
			strcat(name_tmp,month_tst_opn);
		else if( pk_vly_type == TST_CLS_NUM)
			strcat(name_tmp,month_tst_cls);
                else if( pk_vly_type == DAY_MAX)
                        strcat(name_tmp,month_max);
		break;


	case	REQ_YEARLY:
		*rpt_data_type = 2;
	             if ( pk_vly_type == PEAK)
			strcat(name_tmp, year_peak);
		else if( pk_vly_type == PPEAK1)
			strcat(name_tmp,year_ppeak);		/*	modified for v3.0  01/11/23 */
		else if( pk_vly_type == VALLY1)
			strcat(name_tmp,year_vally);
		else if( pk_vly_type == NO_PKVLY)                  
			strcat(name_tmp,year_aver);		/*     add     */
                else if( pk_vly_type == SUM_PKVLY_ALL)
                        strcat(name_tmp,year_total);            /*     add     */
		else if( pk_vly_type == NO_PEAK_TYPE)
			strcat(name_tmp, year_sum);
		break;


	default:
		break;
	}
	
	if( *rpt_data_type == 2)		/* this means the rpt_tab is not dayly report table */    
	{
        	for(i=0;i<month_rpt_pnt_number;i++)
       		{
	       		if(strcmp(name_tmp,monthly_name_def[i])==0) 
			{
 		       		if((fp_month_data=fopen(rpt_hist_name, "rb"))==NULL) 
				return(-1);
                                fseek(fp_month_data,(long) month_rpt_pnt_number * 75+8,0);      /* added by hfz 050908  */
         			fseek(fp_month_data,(long)( i + 
	/*				(time_id-1) * month_rpt_pnt_number) * sizeof(RPT_DATA),0); */
					(time_id) * month_rpt_pnt_number) * sizeof(RPT_DATA),1);   /*  0->1  mod by hfz 050908  */
 	        		if( !fread(rpt_data,sizeof(RPT_DATA),1,fp_month_data))
				{
					fclose(fp_month_data);
					printf ("HIST_RPT:%s is not found in rpt table.\n", name);
					return(-1);
				}


#ifdef SUN_OS                       /******** add rpt_data  2003.4.1 *********/
Swap4Byte(&rpt_data->value.float_data);
#endif
 
	      		   	fclose(fp_month_data);
        			return(1);
       			}	
		}
                printf("#####error1 in read_rpt_hist\n");
	   return -1;       /* added by hfz 050916   */
	}
   printf("#####error2 in read_rpt_hist\n");     
  return -1;               /* added by hfz 050916   */
}

