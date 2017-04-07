#ifndef RPT_DEF_H
#define RPT_DEF_H

/*#define CHECK*/

#define   RPTDEF_NAME_PATH  "/home/ems/h9000/def/"
#define   RPTLOG_NAME_PATH  "/home/ems/h9000/his/rpt/"

#define RPT_START_TIME  1
#define CLE       	2
#define ZBNUM           10
#define H_MAX     	110000
#define L_MAX     	110000
#define MAX_hour    	28
#define MAX_minu    	29
#define MAX_pt		25
#define MIN_hour        26
#define MIN_minu        27
#define MIN_pt          24
#define START_bit 	2
#define BIT_num   	4
#define ANA_START_bit   5
#define ANA_BIT_num     2
#define TIM_hour        26
#define TIM_date        24
#define TIM_month       32
#define TIM_year        12
/*#define CONSTANT        32767*/
#define IMP_NUM		28
#define RPT_NAME_SIZE	75
#define RPT_FILE_HEAD_SIZE	8


char    hourly_name_def[H_MAX][75]; /* hourly_name_def[H_MAX][L_MAX]; by zm */
char    dately_name_def[H_MAX][75]; /* dately_name_def[H_MAX][L_MAX]; by zm */
char    monthly_name_def[H_MAX][75]; /* monthly_name_def[H_MAX][L_MAX]; by zm */
char    yearly_name_def[H_MAX][75]; /* yearly_name_def[H_MAX][L_MAX]; by zm */

char    hourly_name_data[H_MAX][75];
char    monthly_name_data[H_MAX][75];

int     hour_rpt_pnt_number;
int     hour_rpt_pnt_number0; 
int     hour_rpt_pnt_number_sav; 

int     hour_rpt_def_number;
int     hour_rpt_def_number0;
int     hour_rpt_data_number;
int     hour_rpt_data_number0;

int     month_rpt_def_number;
int     month_rpt_def_number0;
int     month_rpt_data_number;
int     month_rpt_data_number0;

int     date_rpt_pnt_number;
int     month_rpt_pnt_number;
int     month_rpt_pnt_number_sav;
int     year_rpt_pnt_number;
int	lg_number, lg_number1, lg_number2;

typedef struct
{
      union
      {
	float           float_data;
	ANLG_DATA       anlg_data;
      }value;
      DATE_TIME         date_time;
}RPT_DATA;

#endif   /*RPT_DEF_H*/


 

