/* 
*	updated by zyp for v4.0db  2006.3
*/

#include <gtk/gtk.h>

#include        "../inc/oix.h"

extern int getCrtNumFromPointer(int data);
extern int getDataFromPointer(int data);
extern void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir);
extern int OpenDspFile (int crt_n, char	*filename);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_toUtf8EX(char *c);
/*
void	SetRptBtn (int  crt_n)
{
XmString        xmstr;
char    *tag = XmFONTLIST_DEFAULT_TAG;

  
	char    cur_day[40] = OIX_STR_CUR_DAY;
	char    hist_day_mon[60] = OIX_STR_HIST_REPORT;
	
	  
		if(crt_n<0)      return;
		
		  if( rpt_tab_date[crt_n] == CUR_DAY_RPT_DATA )
		  xmstr   = XmStringLtoRCreate ( cur_day, tag);
		  
			else if ( rpt_tab_date[crt_n] == HIST_RPT_DATA )
			{
			if( select_rpt_hist_name[crt_n] != (char * ) NULL )
			{
			strcpy( hist_day_mon, "历史报表:");
			strcat( hist_day_mon, select_rpt_hist_name[crt_n]);
			}		 
			xmstr = XmStringLtoRCreate ( hist_day_mon, tag );
			}
			else return;
			
			  SetResourceValue (wgts[crt_n][WgtN_rpt_date_btn], XmNlabelString, xmstr);
			  XmStringFree( xmstr);
}*/

void	GetCurDateString(int crt_n)
{
	long	tloc, rounded_tloc;
	struct	tm *time_ptr;
	
	time_t t;
	int i;
	char    kind_name[MAX_CRT_NUMBER][10];//=  {NULL};
	
	char	*str_tmp= {""};
	
	for( i=0;i<MAX_CRT_NUMBER;i++)
        strcpy(kind_name[i],"");

	time (&tloc);
	rounded_tloc	=tloc / 2 * 2;	/* read time every 2 seconds. */
	t	= time (&rounded_tloc);
	time_ptr = localtime (&t);
	
	strcpy( select_rpt_hist_name[crt_n], "");
	
	if( strcmp(cur_dsp_name[crt_n], "")!=0)
		strcpy( kind_name[crt_n], strchr( cur_dsp_name[crt_n], '.'));	
	
	if ( strcmp( kind_name[crt_n], ".drbin") == 0)
	{
		
#ifdef ENGLISH_VER
        sprintf (select_rpt_hist_name[crt_n],"%sHD%02d-%s-%04d",RPT_LOG_PATH ,time_ptr->tm_mday,mthstr[time_ptr->tm_mon],time_ptr->tm_year+1900);
#else
        sprintf (select_rpt_hist_name[crt_n],"%sHD%04d%02d%02d",RPT_LOG_PATH,time_ptr->tm_year+1900,time_ptr->tm_mon+1,time_ptr->tm_mday);
#endif
		
		strcat( select_rpt_hist_name[crt_n] , ".DATA");
	}
	else if ( strcmp( kind_name[crt_n], ".mrbin") == 0)
	{
		
#ifdef ENGLISH_VER
        sprintf (select_rpt_hist_name[crt_n],"%sHM%s-%04d",RPT_LOG_PATH,mthstr[time_ptr->tm_mon],time_ptr->tm_year+1900);
#else
        sprintf (select_rpt_hist_name[crt_n],"%sHM%02d%02d",RPT_LOG_PATH,time_ptr->tm_year+1900,time_ptr->tm_mon+1);
#endif
		
		strcat( select_rpt_hist_name[crt_n] , ".DATA");
	}
	else
	{
#ifdef ENGLISH_VER
        sprintf (select_rpt_hist_name[crt_n],"%sHD%02d-%s-%04d",RPT_LOG_PATH,time_ptr->tm_mday,mthstr[time_ptr->tm_mon],time_ptr->tm_year+1900);
#else
        sprintf (select_rpt_hist_name[crt_n],"%sHD%04d%02d%02d",RPT_LOG_PATH,time_ptr->tm_year+1900,time_ptr->tm_mon+1,time_ptr->tm_mday);
#endif
		strcat( select_rpt_hist_name[crt_n] , ".DATA");
	}
	printf("in GetCurDateString:select_rpt_hist_name=%s\n",select_rpt_hist_name[crt_n]);
}

int	GetHistDateString(int crt_n)
{
	char    kind_name[MAX_CRT_NUMBER][10];//=  {NULL};
	
	char	*str_tmp= {""};
	char	data_type_string[10]=  {""};
	gchar *pConvert=0;
	int i;
	for( i=0;i<MAX_CRT_NUMBER;i++)
        strcpy(kind_name[i],"");
	
	if( strcmp(cur_dsp_name[crt_n], "")!=0)
		strcpy( kind_name[crt_n], strchr( cur_dsp_name[crt_n], '.'));	
	if( strrchr( select_rpt_hist_name[crt_n] ,PATH_PIPE) != (char * ) NULL ) 
		strncpy( data_type_string ,  (strrchr( select_rpt_hist_name[crt_n] ,PATH_PIPE)+1), 2 ) ; 
	printf("###in GetHistDateString: select_rpt_hist_name=%s,data_type_string=%s\n",select_rpt_hist_name[crt_n],data_type_string);
	if ( strcmp( kind_name[crt_n], ".drbin") == 0)
	{
		if( strcmp( data_type_string, "HM") == 0) 
		{
			PopupMessageBox (main_w[crt_n] ,pConvert=_toUtf8EX(OIX_MSG_DAY_RPT));	
			G_SAFE_FREE(pConvert);
			return(-1);
		}
		
	}
	else if ( strcmp( kind_name[crt_n], ".mrbin") == 0)
	{
		if( strcmp( data_type_string, "HD") == 0) 
		{
			PopupMessageBox ( main_w[crt_n], pConvert=_toUtf8EX(OIX_MSG_MONTH_RPT));
			G_SAFE_FREE(pConvert);
			return(-1);
		}
	}
	return 1;
}



/*void Menu_Rpt(GtkWidget *w,gpointer data)
{
int crt_n = 0;

  switch((int)data)
  {
		case CUR_DAY_RPT_DATA:
		rpt_tab_date[crt_n] = CUR_DAY_RPT_DATA;
		
		  strcpy ( select_rpt_hist_name[crt_n], "");
		  GetCurDateString( crt_n);
		  break;
		  case HIST_RPT_DATA:
		  rpt_tab_date[crt_n] = HIST_RPT_DATA;
		  
			file_mode[crt_n]       = RPT_HIST_FILE;
			PopFileSelectionDlg(crt_n,OIX_STR_RPT_FILE_SELECT_BOX,"/RPTLOG/*.DATA");
			break;
			case RPT_PRINT:
			PopupMessageBox ( crt_n,MSG_NO_RPT_PRINT);
			break;			
			}
			}
*/
void Menu_Rpt(GtkMenuItem *w,gpointer data)
{
	int crt_n ;
#ifdef SUN_OS
	char   tmp ='/';
#endif
#ifdef  WINDOWS_OS
	char tmp='\\';
#endif
	int i;
	char    kind_name[MAX_CRT_NUMBER][10];//=  {NULL};
	char    file_type[MAX_CRT_NUMBER][10];//=  {NULL};
	
	for( i=0;i<MAX_CRT_NUMBER;i++)
        strcpy(kind_name[i],"");

	for( i=0;i<MAX_CRT_NUMBER;i++)
        strcpy(file_type[i],"");
	//GtkWidget  *cur_day_rpt,*cur_mon_rpt,*hist_rpt;
	
	/* GetCrtNumberOfWgt(w,&crt_n);*/
	crt_n=getCrtNumFromPointer((int)data);
	data=(gpointer)getDataFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	printf("!!!!!!!!in Menu_Rpt::::: data=%d,crt_n=%d\n",data,crt_n);
	
	/* cur_day_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"cur_day_menu");
	cur_mon_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"cur_mon_menu");
	hist_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"hist_rpt_menu");
	*/
	
	if( strchr( cur_dsp_name[crt_n], '.') != (char *)NULL)	
	{
		strcpy( file_type[crt_n], strchr( cur_dsp_name[crt_n], '.'));
		printf("file_type=%s\n",file_type[crt_n]);
		if( ( strcmp( file_type[crt_n], ".mrbin") != 0) && ( strcmp( file_type[crt_n], ".drbin") != 0) )
		{
			PopupMessageBox(main_w[crt_n],CUR_DSP_NOT_RPT_FILE);
			return;
		}
	}
	switch((int)data)
	{
	case CUR_DAY_RPT_DATA:
		if (gtk_check_menu_item_get_active((GtkCheckMenuItem*)w)) 
		{
			rpt_tab_date[crt_n] = CUR_DAY_RPT_DATA;
			
			/*SetRptBtn( w,crt_n);*/
			
			strcpy ( select_rpt_hist_name[crt_n], "");
			printf("cur_dsp_name[%d]=%s\n",crt_n,cur_dsp_name[crt_n]);
			/*	printf("strrchr( cur_dsp_name[crt_n], '/')+1=%s\n",strrchr( cur_dsp_name[crt_n], '/')+1);
			*/
			if( strcmp(cur_dsp_name[crt_n], "")!=0)
				strcpy( kind_name[crt_n], strrchr( cur_dsp_name[crt_n], tmp)+1);			
			
			printf("@@@@@CUR_DAY_RPT_DATA::  kind_name=%s\n",kind_name[crt_n]);
			OpenDspFile (crt_n,kind_name[crt_n]);
			GetCurDateString( crt_n);
		}
		break;
		
	case CUR_MONTH_RPT_DATA:/*Added by zyp on 2007.7.10*/
		if (gtk_check_menu_item_get_active((GtkCheckMenuItem*)w)) 
		{
			rpt_tab_date[crt_n] = CUR_MONTH_RPT_DATA;
			
			/*SetRptBtn( w,crt_n);*/
			
			strcpy ( select_rpt_hist_name[crt_n], "");
			if( strcmp(cur_dsp_name[crt_n], "")!=0)
				strcpy( kind_name[crt_n], strrchr( cur_dsp_name[crt_n], tmp)+1);	
			printf("@@@@@CUR_MONTH_RPT_DATA::  kind_name=%s\n",kind_name[crt_n]);
			OpenDspFile (crt_n,kind_name[crt_n]);
			GetCurDateString( crt_n);
		}
		break;
		
	case HIST_RPT_DATA:
		if (gtk_check_menu_item_get_active((GtkCheckMenuItem*)w)) 
		{
			rpt_tab_date[crt_n] = HIST_RPT_DATA;                        
			
			file_mode[crt_n]       = RPT_HIST_FILE;        
			printf("@@@@@HIST_RPT_DATA:: \n");
			PopFileSelectionDlg(main_w[crt_n],crt_n,_CS_(OIX_STR_RPT_FILE_SELECT_BOX),RPT_LOG_PATH);
			/*SetRptBtn(w, crt_n);*/
		}
		break;
	case RPT_PRINT:
		PopupMessageBox ( main_w[crt_n], MSG_NO_RPT_PRINT);
		break;			
	}
}



