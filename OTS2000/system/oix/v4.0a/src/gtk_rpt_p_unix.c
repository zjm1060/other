/* 
*	updated by zyp for v4.0db  2006.3
*/

#include <gtk/gtk.h>

#include        "../inc/oix.h"
#include        "/home/ems/system/pas/v4.0a/inc/rpt_def.h"
 

void	SetRptBtn (GtkWidget *w,int  crt_n)
{
	/*XmString        xmstr;
	char    *tag = XmFONTLIST_DEFAULT_TAG;*/
	char	menu_label[50];

	char    cur_day[40] = OIX_STR_CUR_DAY;
	char    hist_day_mon[60] = OIX_STR_HIST_REPORT;	

	if(crt_n<0)      return;

        if( rpt_tab_date[crt_n] == CUR_DAY_RPT_DATA )
	/*	xmstr   = XmStringLtoRCreate ( cur_day, tag);*/
			strcpy(menu_label,cur_day);

	else if ( rpt_tab_date[crt_n] == HIST_RPT_DATA )
	{
		if( select_rpt_hist_name[crt_n] != (char * ) NULL )
		{
			strcpy( hist_day_mon, "历史报表:");
			strcat( hist_day_mon, select_rpt_hist_name[crt_n]);
		}		 
		/*	xmstr = XmStringLtoRCreate ( hist_day_mon, tag );*/
		strcpy(menu_label,hist_day_mon);
	}
	else 
            {
               strcpy(menu_label,"test");
	//gtk_object_set(GTK_OBJECT(w),"GtkButton::label",_toUtf8(menu_label),NULL);
	   
            return;
        }
printf("#####menu_label=%s\n",menu_label);
/*	SetResourceValue (wgts[crt_n][WgtN_rpt_date_btn], XmNlabelString, xmstr);*/
      /*  strcpy(menu_label,"test");*/
	//gtk_object_set(GTK_OBJECT(w),"GtkButton::label",_toUtf8(menu_label),NULL);
	/*XmStringFree( xmstr);*/
}

void	GetCurDateString(int crt_n)
{
	long	tloc, rounded_tloc;
	struct	tm *time_ptr;

	time_t t;
	char    kind_name[MAX_CRT_NUMBER][10]=  {NULL};

	char	*str_tmp= {""};
	

	time (&tloc);
	rounded_tloc	=tloc / 2 * 2;	/* read time every 2 seconds. */
	t	= time (&rounded_tloc);
	time_ptr = localtime (&t);
	
	strcpy( select_rpt_hist_name[crt_n], "");

	if( cur_dsp_name[crt_n] != (char) NULL)
		strcpy( kind_name[crt_n], strchr( cur_dsp_name[crt_n], '.'));	

	if ( strcmp( kind_name[crt_n], ".drbin") == 0)
	{

#ifdef ENGLISH_VER
        sprintf (select_rpt_hist_name[crt_n],"%sHD%02d-%s-%04d" ,RPTLOG_NAME_PATH,time_ptr->tm_mday,mthstr[time_ptr->tm_mon],time_ptr->tm_year+1900);
#else
        sprintf (select_rpt_hist_name[crt_n],"%sHD%04d%02d%02d",RPTLOG_NAME_PATH,time_ptr->tm_year+1900,time_ptr->tm_mon+1,time_ptr->tm_mday);
#endif		
	}
	else if ( strcmp( kind_name[crt_n], ".mrbin") == 0)
	{

#ifdef ENGLISH_VER
        sprintf (select_rpt_hist_name[crt_n],"%sHM%s-%04d",RPTLOG_NAME_PATH ,mthstr[time_ptr->tm_mon],time_ptr->tm_year+1900);
#else
        sprintf (select_rpt_hist_name[crt_n],"%sHM%02d%02d",RPTLOG_NAME_PATH,time_ptr->tm_year+1900,time_ptr->tm_mon+1);
#endif		
	}
	else
	{
#ifdef ENGLISH_VER
        sprintf (select_rpt_hist_name[crt_n],"%sHD%02d-%s-%04d" ,RPTLOG_NAME_PATH,time_ptr->tm_mday,mthstr[time_ptr->tm_mon],time_ptr->tm_year+1900);
#else
        sprintf (select_rpt_hist_name[crt_n],"%sHD%04d%02d%02d",RPTLOG_NAME_PATH,time_ptr->tm_year+1900,time_ptr->tm_mon+1,time_ptr->tm_mday);
#endif                
	}
        strcat( select_rpt_hist_name[crt_n] , ".DATA");
        printf("#####in GetCurDateString:: select_rpt_hist_name=%s\n",select_rpt_hist_name[crt_n]);
}

int	GetHistDateString(int crt_n)
{
	char    kind_name[MAX_CRT_NUMBER][10]=  {NULL};

	char	*str_tmp= {""};
	char	data_type_string[10]=  {NULL};

	if( cur_dsp_name[crt_n] != (char) NULL)
		strcpy( kind_name[crt_n], strchr( cur_dsp_name[crt_n], '.'));	
	if( strrchr( select_rpt_hist_name[crt_n] ,RPTLOG_NAME_PATH) != (char * ) NULL ) 
		strncpy( data_type_string ,  (strrchr( select_rpt_hist_name[crt_n] ,RPTLOG_NAME_PATH)+1), 2 ) ; 

	if ( strcmp( kind_name[crt_n], ".drbin") == 0)
	{
		if( strcmp( data_type_string, "HM") == 0) 
		{
			PopupMessageBox ( crt_n, OIX_MSG_DAY_RPT);	
			return(-1);
		}			
	}
	else if ( strcmp( kind_name[crt_n], ".mrbin") == 0)
	{
		if( strcmp( data_type_string, "HD") == 0) 
		{
			PopupMessageBox ( crt_n, OIX_MSG_MONTH_RPT);	
			return(-1);
		}
	}
}



void Menu_Rpt(GtkMenuItem *w,gpointer data)
{
	int crt_n = 0;
        char    kind_name[MAX_CRT_NUMBER][10]=  {NULL};
        char    file_type[MAX_CRT_NUMBER][10]=  {NULL};
        
	GtkWidget  *cur_day_rpt,*cur_mon_rpt,*hist_rpt;
        
        GetCrtNumberOfWgt(w,&crt_n);
        
        printf("!!!!!!!!in Menu_Rpt::::: data=%d,crt_n=%d\n",data,crt_n);
        
       /* cur_day_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"cur_day_menu");
        cur_mon_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"cur_mon_menu");
        hist_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"hist_rpt_menu");
        */
        
        if( strchr( cur_dsp_name[crt_n], '.') != (char *)NULL)	
            {
		strcpy( file_type, strchr( cur_dsp_name[crt_n], '.'));
                printf("file_type=%s\n",file_type);
                if( ( strcmp( file_type, ".mrbin") != 0) && ( strcmp( file_type, ".drbin") != 0) )
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
                        if( cur_dsp_name[crt_n] != (char) NULL)
                            strcpy( kind_name[crt_n], strrchr( cur_dsp_name[crt_n], '/')+1);	
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
                        if( cur_dsp_name[crt_n] != (char) NULL)
                            strcpy( kind_name[crt_n], strrchr( cur_dsp_name[crt_n], '/')+1);	
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
			PopFileSelectionDlg(main_w[crt_n],crt_n,OIX_STR_RPT_FILE_SELECT_BOX,RPTLOG_NAME_PATH);
                        /*SetRptBtn(w, crt_n);*/
                    }
			break;
		case RPT_PRINT:
			PopupMessageBox ( crt_n,MSG_NO_RPT_PRINT);
			break;			
	}
}

