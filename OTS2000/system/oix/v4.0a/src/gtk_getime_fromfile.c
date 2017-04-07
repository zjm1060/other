/***********************************************************
  2006.2	updated by zyp for v4.0
  Windows Have  SYN to  UNIX  by chi 07.11.30 ;
************************************************/


#include	"../inc/oix.h"

void	GetRptTimeFromSelectFile (short crt_n, char select_rpt_hist_data[MAX_FULL_FILE_NAME_LEN], char	*char_date )
{
	char    char_year[100] = {""};
	char    char_mon[100] = {""};
	char    char_day[100] = {""};	
    
	char    file_tmp[40]   = {""};	
	int     len, full_len;
	
	if ( select_rpt_hist_data != (char*)NULL )
	{
		full_len = strlen ( select_rpt_hist_data);
		if ( strchr ( select_rpt_hist_data, '.') != NULL )
		{
			len= strlen (strchr ( select_rpt_hist_data, '.') );
			strncpy ( file_tmp, select_rpt_hist_data, (full_len-len));
		}
		
#ifdef ENGLISH_VER
		
		if ( strchr ( file_tmp, 'M') != NULL )
		{
			strcpy ( char_tmp, ( strchr (file_tmp, 'M')+1) );
			strncpy ( char_date, char_tmp, 8);
		}
		else if ( strchr ( select_rpt_hist_data, 'D') != NULL )
		{
			strcpy ( char_tmp, (strchr ( file_tmp, 'D')+1) );
			strncpy ( char_date, char_tmp, 11);
		}
#else
		if ( strchr ( file_tmp, 'M') != NULL )
			strcpy ( char_date, ( strchr (file_tmp, 'M')+1) );
		else if ( strchr ( select_rpt_hist_data, 'D') != NULL )
			strcpy ( char_date, (strchr ( file_tmp, 'D')+1) );
		
		strncpy ( char_year, char_date, 4);
		strcat ( char_year, _CS_(OIX_CSTR_YEAR));
		
		strncpy ( char_mon, char_date+4, 2);
		strcat ( char_mon, _CS_(OIX_CSTR_MONTH));
		
		if ( strlen ( char_date) > 6)
		{
			strncpy ( char_day, char_date+6, 2);
			strcat ( char_day, _CS_(OIX_CSTR_DAY));
		}
		
		strcpy ( char_date, "");
		strcpy ( char_date, char_year);
		strcat ( char_date, char_mon );
		strcat ( char_date, char_day );
#endif
		return;
	}
}
