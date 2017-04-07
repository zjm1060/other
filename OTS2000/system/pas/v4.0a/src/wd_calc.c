#include <math.h>

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include    "../../../pas/v4.0a/inc/wd_calc.h"
#include    "../../../pas/v4.0a/inc/rpt_def.h"
#include    "../../../pas/v4.0a/inc/pdc_def.h"

/*#define  DEBUG*/

#define	MAX_WD_PT_IN_CHARA	300		/* maximum points in a group of data to be calculated	*/

#define	MAX_CHARA_NUM        1000//300 wzg20110426			/* maximum group of data to be calculated	*/

#define CHARA_RATE_CONSTANT	100
typedef	struct
{
	USHORT	max_no, min_no;
	float	maximum, minimum;
}	MAX_MIN_VALUE;

typedef	struct
{
	char	output_name[POINT_NAME_SIZE];
	char	input_name[MAX_WD_PT_IN_CHARA][POINT_NAME_SIZE];	
	int		n_points;		/* n_points must <= MAX_WD_PT_IN_CHARA	*/
	float	value[MAX_WD_PT_IN_CHARA];
	float	pos[MAX_WD_PT_IN_CHARA];
}	ANA_GRP_DATA;


static int	first_calcu_flag=1;  	/* calculation flag: (0/1): first/other calculation	*/
static int 			chara_cnt;
static ANA_GRP_DATA	data_dscr[MAX_CHARA_NUM];

static	CHARA_DATA	*chara_temp_data;       /*    98.7.2    modified    */
static  CHARA_DATA      eigen;                  
static  MAX_MIN_VALUE   tmp_buf;              

int	LoadEigenDataFile(void);
MAX_MIN_VALUE	*GetMaxMinValue (float x[ ], int norm_flag[], int n);
CHARA_DATA	*GetAnaEigenValue (ANA_GRP_DATA *dscr );

MAX_MIN_VALUE	*GetMaxMinValue ( float x[], int norm_flag[], int n)
{ 
	int	j;

	tmp_buf.maximum = -99999.;
	tmp_buf.minimum = 99999.; 
	tmp_buf.max_no = 0; 
	tmp_buf.min_no = 0;
  	for ( j = 0; j < n; j++)                                                  
	{ 
		if(!norm_flag[j])
			continue;
		if (x[j] > tmp_buf.maximum) 
		{ 
			tmp_buf.maximum = x[j]; 
			tmp_buf.max_no = j;
		}
    	if ( x[j] < tmp_buf.minimum) 
		{ 
			tmp_buf.minimum = x[j]; 
			tmp_buf.min_no = j;
		}
	}

#ifdef DEBUG
	printf ("in GetMaxMinValue: n= %d, maximum = %f, index= %d, minimum = %f, index = %d\n", 
		n, tmp_buf.maximum, tmp_buf.max_no, tmp_buf.minimum, tmp_buf.min_no);
#endif
	return ( &tmp_buf );
}


CHARA_DATA	*GetAnaEigenValue ( ANA_GRP_DATA	*dscr )
{
	int 		i;
	MAX_MIN_VALUE	*tmp;	
	DMS_COMMON	common_aa;
	float		value[MAX_WD_PT_IN_CHARA], xc[MAX_WD_PT_IN_CHARA];
	double      sum, sum1 ;
	int			norm_pt_num;
	int			norm_flag[MAX_WD_PT_IN_CHARA];
	char		err_str[256];

	if ( dscr->n_points <= 0 ) return ((CHARA_DATA *) NULL);

	sum	= 0.0;
	sum1    = 0.0;
	norm_pt_num= 0;
	for(i = 0; i < dscr->n_points; i++)
  	{
		if ( -1 == ReadEntryByNameStr ( dscr->input_name[i], &common_aa) )                        
		{
			sprintf ( err_str,"wd_calc;EC:001; %s No this point in the database file !!!",dscr->input_name[i]);                              
			Err_log(err_str);
			norm_flag[i]= 0;
			continue;
		}
		if(common_aa.var.anlg.value.status.chan_fail || !common_aa.fixed.iost.scanEnable)
		{
			norm_flag[i]= 0;
			continue;
		}
		norm_flag[i]= 1;
		norm_pt_num++;

		value[i]	= common_aa.var.anlg.fValue;
	
		if ( !first_calcu_flag  )
		{
			xc[i]	= dscr->value[i] - value[i];
			if ( xc[i] < 0.0 ) xc[i] = -xc[i];	    
		}

		dscr->value[i] = value[i];
		sum  += value[i];
		sum1 += (double)value[i] * (double)value[i];                               	  
	} 

	/*	calculate the average, maximum, minimum value of array value.	*/
	tmp	= (MAX_MIN_VALUE *) GetMaxMinValue ( value, norm_flag, dscr->n_points );	 
	eigen.max_value   = tmp->maximum;
	eigen.min_value   = tmp->minimum;
	eigen.max_no	  = (USHORT)(dscr->pos[tmp->max_no]*CHARA_RATE_CONSTANT);
	eigen.min_no	  = (USHORT)(dscr->pos[tmp->min_no]*CHARA_RATE_CONSTANT);
	if(norm_pt_num>0)
		eigen.avg_value= (float)(sum/(double)norm_pt_num);
	else
		eigen.avg_value= 9999.;

    /*  calculate the standard value of array value.    */
	if(norm_pt_num>0)
		eigen.std_value= (float)sqrt((double)sum1/(double)norm_pt_num - (double)eigen.avg_value*(double)eigen.avg_value);   
	else
		eigen.std_value= 0;	
	
	/*	calculate the maximumimal changing rate of value dscr->value.	*/
	if ( !first_calcu_flag  )
	{      
		tmp	= (MAX_MIN_VALUE *) GetMaxMinValue ( xc, norm_flag, dscr->n_points ); 
		eigen.max_rate	    = tmp->maximum;
		eigen.min_rate	    = tmp->minimum;
		eigen.max_rate_no   = (USHORT)(dscr->pos[tmp->max_no]*CHARA_RATE_CONSTANT);
		eigen.min_rate_no   = (USHORT)(dscr->pos[tmp->min_no]*CHARA_RATE_CONSTANT);
	}
	else
	{
		eigen.max_rate	   = 0; 	eigen.min_rate	   = 0;
		eigen.max_rate_no  = 0; 	eigen.min_rate_no  = 0;
	}

#ifdef	DEBUG	
	printf("GetAnaEigenValue:: maxvalue=%f,minvalue=%f,maxno=%d,minno=%d,avgvalue=%f,stdvalue=%f,maxrate=%f,minrate=%f,maxrate_no=%d,minrate_no=%d\n",
	                           eigen.max_value,eigen.min_value,eigen.max_no,eigen.min_no,eigen.avg_value,
	                           eigen.std_value,eigen.max_rate,eigen.min_rate,eigen.max_rate_no,eigen.min_rate_no);
#endif

	return ( &eigen );
}


int	LoadEigenDataFile(void)
{
    char   fname[256];
    int	   temp_num;
	char   linebuf[256], err_str[256];
	FILE   *fp;
	POINTER	pt;
	int		new_pt_flag, pt_no;
	float	pos;
	char	input_name[256];
        
	chara_cnt = 0;

	sprintf(fname, "%s%s", SYS_DEFINE_PATH, TEMP_ENG_DEFINE_FILE);
	if( (fp=fopen( fname,"r"))==NULL )
	{
		sprintf( err_str,"wd_calc;EC:002; Can't open %s !!!",fname); 
		Err_log(err_str);
		return( -1 );
	}	

	new_pt_flag= 0;
	while(fgets(linebuf, 256, fp))
	{
		char str[256], str1[256];

		if(2!=sscanf ( linebuf, "%s %s", str, str1))
			continue;

		if( strcmp(str,"POINT:") ==0 )
		{
			if(-1!=GetPtIdByNameStr(str1, &pt))
			{
				if(chara_cnt>=MAX_CHARA_NUM)
				{
					sprintf(err_str, "%s chara num too much, max num: %d", str1, MAX_CHARA_NUM);
					Err_log(err_str);
					break;
				}

				pt_no= chara_cnt;
				chara_cnt++;
				new_pt_flag= 1;
				temp_num= 0;
				strcpy(data_dscr[pt_no].output_name, str1);
			}
			continue;
		}
		
		if(temp_num>=MAX_WD_PT_IN_CHARA)
		{
			new_pt_flag= 0;
			sprintf(err_str, "pt num too much in chara %s, max num: %d", data_dscr[pt_no].output_name, MAX_WD_PT_IN_CHARA);
			Err_log(err_str);
			continue;
		}

		if(!new_pt_flag)
			continue;

		if(2!=sscanf ( linebuf, "%f %s", &pos, input_name))
		{
			sprintf(err_str, "Format error: %s", linebuf);
			Err_log(err_str);
			continue;
		}
		if(-1!=GetPtIdByNameStr(input_name, &pt))
			input_name[POINT_NAME_SIZE-1]= 0;
			
		strcpy(data_dscr[pt_no].input_name[temp_num], input_name);
		data_dscr[pt_no].pos[temp_num]= pos;
		temp_num++;
		data_dscr[pt_no].n_points= temp_num;
	}

	fclose(fp);
	return 0;
}


void* tempcha(PDC_TASK *pTask)
{
	int 	j,run_flag; 
	DATE_TIME  write_time;
	char	err_str[256];

	first_calcu_flag= 1;

	for(;;)
	{
		sleep(pTask->check_period);

		if( (run_flag = PdcCheck(pTask))!=0) 
			continue;		


		printf("\n------TEMP_CHARA------%d\n", time(NULL));

		if ( LoadEigenDataFile () == -1 ) 
		{ 
			sprintf(err_str,"wd_calc;EC:003; LoadEigenDataFile ERR");
			Err_log(err_str);
			continue; 
		}	

		for ( j=0; j<chara_cnt; j++)
		{ 
			chara_temp_data	=   (CHARA_DATA *) GetAnaEigenValue ( &data_dscr[j] );
			if ( chara_temp_data != (CHARA_DATA*) NULL)
			{
				GetSysTime( &write_time);
				SetCharaPtDataByNameStr( data_dscr[j].output_name, *chara_temp_data, write_time);
			}
		}
		
		first_calcu_flag= 0;
	}
} 
