/****
updated by zyp on 2005 by gtk
OpenDspFileCallback: 
data(0,1,2,3) should be incording with oix_menu.rc 
SYN by hcl 2008.14.12
***/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

/* #define	DEBUG     */

extern gint UpdateDsp (int crt_n );
extern void	Redraw (GtkWidget *w);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern int OpenDspFile (int crt_n, char	*filename);
extern void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir);
extern void	BlankStripCopy (char	*dst, char	*src, int len);
extern void	GetCurDateString(int crt_n);
extern void	ReadGdbaseData (GDBUFF	*gdb, FILE *fp);
extern void AppendDspData(  int crt_n ,  char*dsp_name);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void	ReadSubGdbaseData (int crt_n, GCOND_BUFF	*gcon, FILE		*fp,int sub_win_no);
extern void PopupDynSubWin (int crt_n,int sub_win_n,GtkWidget	*parent,SHM_OTS *shm_ots);
extern void	ZoomInit (int crt_n );
extern int	GetHistDateString(int crt_n);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern void	ResetZoom (int crt_n );
extern void PopupAutocheckFile(int crt_n,char* pathfilename);
extern char *_fromUtf8(char *c);

FILE 	*OpenFileX();
FILE 	*OpenWriteFile();

extern POINTER pnt_tab_ptr[MAX_CRT_NUMBER];
char	Cur_Dsp_name[MAX_CRT_NUM][MAX_FULL_FILE_NAME_LEN+10];
short	change_flag[] = {TO_RBIN, TO_RBIN};	

void CheckGdbaseData ( GDBUFF * );
void	UpdateDspName ( int crt_n, char *str );

void OpenDspFileCallback(GtkWidget *w,gpointer data)
{
	char filename[MAX_FULL_FILE_NAME_LEN];
	int crt_n=0;
	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return; 
	
	printf("filename=%d\n",data);
	switch((int)data)
	{
	case 0:
		strcpy(filename,_CS_(commonDirName));
		break;
	case 1:
		//strcpy(filename,"zjx.dbin");
		strcpy(filename,_CS_(zjxName));
		break;
	case 2:
		strcpy(filename,"dsp_index.dbin");
		break;
	case 3:
		//strcpy(filename,"zjx.dbin");
		strcpy(filename,_CS_(zjxName));
		break;			
	}	
	OpenDspFile (crt_n, filename);
}

void Menu_OpenFile(GtkWidget * widget, gpointer data)
{
	char  dirname[150] ;
	int crt_n=(gint) data;
	file_mode[crt_n]       = DSP_FILE; 
	strcpy(dirname,OIX_DSP_PATH);
	strcat (dirname, "*.dbin");
	PopFileSelectionDlg(main_w[crt_n],crt_n,_CS_(OIX_STR_FILE_SELECT_BOX),dirname); 
	
}

void PopAutochkDlg(GtkWidget* widget, gpointer usr_data)
{
	GtkWidget *pDlg;
	GtkFileFilter	 *pTxtFilter;
	gchar *pConvert=0;
	gchar strFilePath[256];
	

	int crt_n=GPOINTER_TO_INT(usr_data);    
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
    
	file_mode[crt_n] = FAIL_FILE; 

	pDlg = gtk_file_chooser_dialog_new (pConvert=_toUtf8EX(OIX_STR_AUTOCHK_FILE_SELECT_BOX),
		GTK_WINDOW(widget),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	G_SAFE_FREE(pConvert);//dzy1017

	gtk_dialog_set_alternative_button_order (GTK_DIALOG (pDlg), GTK_RESPONSE_ACCEPT, GTK_RESPONSE_CANCEL, -1);

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(pDlg), AUTOCHECK_LOG_PATH);

	pTxtFilter=gtk_file_filter_new();
	gtk_file_filter_set_name(pTxtFilter,"TXT");
	gtk_file_filter_add_pattern(pTxtFilter, "*.txt");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pDlg), pTxtFilter);

// 	pXlsFilter=gtk_file_filter_new();
// 	gtk_file_filter_set_name(pXlsFilter, "XLS");
// 	gtk_file_filter_add_pattern(pXlsFilter, "*.xls");
// 	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pDlg), pXlsFilter);
// 	
	if (gtk_dialog_run (GTK_DIALOG (pDlg)) == GTK_RESPONSE_ACCEPT)
	{
		gchar* filename;
		
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (pDlg));
		sprintf(strFilePath, "%s",	pConvert=_fromUtf8(filename));
		G_SAFE_FREE(pConvert);//dzy1017
		PopupAutocheckFile(crt_n,filename);		
		g_free (filename);
	}
	
	g_object_unref(pTxtFilter);
	gtk_widget_destroy (pDlg);
}

void Menu_OpenCHKFile(GtkWidget * widget, gpointer data)
{
	int crt_n=(gint) data;
	PopAutochkDlg(main_w[crt_n],data);
	
}
int OpenDspFile (int crt_n, char	*filename)
{
	FILE	*fp;
	char	part_name[MAX_FULL_FILE_NAME_LEN+10]={""};
	char	kind_name[10];
	char    data_type_string[10]=  {""};
    GtkWidget  *cur_day_rpt,*cur_mon_rpt/*,*hist_rpt*/;
	
	if (!filename || !*filename)
	{
		
		return	FALSE;
	}
	strcpy(shm_ots_addr->data_point.curname,filename);
	/* if( strcmp(filename,"gen_index.dbin") == 0)
	FlashDspTimeOut(crt_n);	gtk */
	
	if( strchr( filename, '.') != (char *)NULL)		
		strcpy( kind_name, strchr( filename, '.'));
	if( ( strcmp( kind_name, ".mrbin") == 0) || ( strcmp( kind_name, ".drbin") == 0) )
		rpt_tag[crt_n] =	RPT_TAG_ON;
   	else
	{
		if ( rpt_tag[crt_n] == RPT_TAG_ON )
		{
			change_flag[crt_n] = TO_DBIN;		
			rpt_tag[crt_n]	= RPT_TAG_OFF;
		}
		if( ( strcmp( filename , "io_int_point_data.dbin" ) == 0) ||
			( strcmp( filename ,"io_ana_point_data.dbin") == 0 ) ||
			( strcmp( filename ,"io_pol_point_data.dbin") == 0 ) ||
			( strcmp( filename ,"io_num_point_data.dbin") == 0 ) ||
			( strcmp( filename ,"io_puls_point_data.dbin") == 0 ) ||
			( strcmp( filename ,"io_sysmsg_point_data.dbin") == 0 ) )
		{
			ptr_tab_rcd_tag[crt_n]	= PTR_TAB_RCD_PROHIBIT;
		} 
		else
			ptr_tab_rcd_tag[crt_n]	= PTR_TAB_RCD_AVAILABLE;
	}
	change_flag[crt_n] = TO_DBIN;
#ifdef WINDOWS_OS
	if(filename[0]!='\\')
#else
	if(filename[0]!='/')
#endif
	{
	strcpy (part_name, FILE_DSP_PATH);
	strcat (part_name, filename);
	}
	else
		strcpy(part_name, filename);

	if (!strcmp(Cur_Dsp_name[crt_n],part_name))
	{
		return FALSE;
	}
	fp=	OpenFileX( part_name);
	/*g_message("file name[%d] is %s\n" ,crt_n,part_name );*/
	if ( fp == NULL)
	{
		printf ("OIX: open filepart_name =%s error.\n", part_name); return FALSE;
	}
	strcpy(Cur_Dsp_name[crt_n],part_name);
	
	UpdateDspName ( crt_n, filename ); 			/*  Update windows title*/
	
	BlankStripCopy ( cur_dsp_name[crt_n], part_name, MAX_FULL_FILE_NAME_LEN+10); 
	
	if ( ( strcmp( kind_name, ".mrbin") == 0) || ( strcmp( kind_name, ".drbin") == 0) ) 
	{
		if(rpt_tab_date[crt_n]!=HIST_RPT_DATA)
		{
			if(strcmp( kind_name, ".mrbin") == 0)
			{
				cur_mon_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"cur_mon_menu");
				/* gtk_check_menu_item_set_active(cur_mon_rpt,True);*/
			}
			else if(strcmp( kind_name, ".drbin") == 0)
			{
				cur_day_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"cur_day_menu");
				/*gtk_check_menu_item_set_active(cur_day_rpt,True);*/
			}
		}
		if( rpt_tab_date[crt_n] == CUR_DAY_RPT_DATA )  
		{
			GetCurDateString( crt_n);  
			/*if(strcmp( kind_name, ".mrbin") == 0)
			{
			PopupMessageBox(main_w[crt_n],MSG_NOT_DAY_RPT_DATA);
			return False;
		}*/
			
		}       
		else if( rpt_tab_date[crt_n] == CUR_MONTH_RPT_DATA )
		{
			GetCurDateString( crt_n);  
			/*if(strcmp( kind_name, ".drbin") == 0)
			{
			PopupMessageBox(main_w[crt_n],MSG_NOT_MONTH_RPT_DATA);
			gtk_check_menu_item_set_active(cur_mon_rpt,False);
			return False;
		}*/
			
		}        
		else
		{
			GetHistDateString( crt_n);
			if( strrchr( select_rpt_hist_name[crt_n] ,'/') != (char * ) NULL )
			{
				strncpy( data_type_string ,  
					(strrchr( select_rpt_hist_name[crt_n] ,'/')+1), 2 ) ;
			}
			if(( ( strcmp( kind_name, ".mrbin") == 0)) && 
				( strcmp( data_type_string, "HD") ==0 ))
			{	
				PopupMessageBox(main_w[crt_n],MSG_NOT_MONTH_RPT_DATA);
				fclose (fp);
				return	False;
			}
			else if(( ( strcmp( kind_name, ".drbin") == 0))  &&
				( strcmp( data_type_string, "HM") ==0 ) )
			{	
				PopupMessageBox(main_w[crt_n],MSG_NOT_DAY_RPT_DATA);
				fclose (fp);
				return	False;
			}
			/*  hist_rpt=gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"hist_rpt_menu");
			gtk_check_menu_item_set_active(hist_rpt,True);*/
		}
	}	
	
	ReadGdbaseData ( &gdbase[crt_n], fp);
	fclose (fp);
// 	ZoomInit (  crt_n );
// 	zoomdb[crt_n].zoom_lock = FALSE; 
	ResetZoom (  crt_n )   ;  
	if( change_flag[crt_n] == TO_DBIN)  
	{
		
		Redraw ( canvas[crt_n]); 
		
		if (dsp_timer_id[crt_n] !=0 )
		{
			gtk_timeout_remove(dsp_timer_id[crt_n]); 
			dsp_timer_id[crt_n]= 0;
		}
		dsp_timer_id[crt_n]   =gtk_timeout_add (  1500, (GtkFunction)UpdateDsp,GINT_TO_POINTER (crt_n));
		/*g_message(" dsp_timer_id[%d] : %d    is added !!!" , crt_n ,dsp_timer_id[crt_n]);	 */
		
	}
	else
	{
		Redraw ( canvas[crt_n] ); 
	}
	AppendDspData(   crt_n , filename  ) ;
	
	return	TRUE;
}

BOOL OpenSubDspFile ( crt_n, filename,  shm_ots)
int     crt_n;
char    *filename;
SHM_OTS *shm_ots;
{
    int i,sub_win_no;
	FILE    *fp;
	char    part_name[MAX_FULL_FILE_NAME_LEN]={""};
	char    kind_name[10];
	memset(kind_name,0,10);
	
#ifdef    DEBUG
	printf("in OpenSubDspFile\n");
#endif
	if (!filename || !*filename)
	{
		
		return FALSE ;
	}
	strcpy (part_name, FILE_DSP_PATH);
	strcat (part_name, filename);
	
	fp=     OpenFileX( part_name);
	if ( fp == NULL)
	{
		PopupMessageBox( main_w[crt_n], MSG_SBIN_FILE_NOT_FOUND );
		printf(" sbin_file %s not open\n", part_name);
		return( FALSE);
	}
	else
	{   
		for(i=0;i<MAX_SUB_WIN_NUM;i++)
		{                   
			if(strcmp(sub_win_title[i][crt_n],filename)==0&&strcmp(filename,"io_obj_point_data.sbin")!=0)
			{
				gtk_window_present(GTK_WINDOW(sub_win_shell[i][crt_n]));
				break;                        
			}                       
		}
		if(i<MAX_SUB_WIN_NUM)/* sub_win existed */
			return(TRUE);
		for(i=0;i<MAX_SUB_WIN_NUM;i++)
		{
			if(sub_win_shell[i][crt_n]==NULL)                   
				break;
		}
		sub_win_no =i;
		
		if(sub_win_no>=MAX_SUB_WIN_NUM)
		{
			PopupMessageBox(main_w[crt_n],MSG_TOO_MUCH_SUB_WIN);
			return FALSE;
		}
		
		if(sub_win_shell[sub_win_no][crt_n]==NULL)
		{                    
			ReadSubGdbaseData (crt_n, &gcondb[sub_win_no][crt_n], fp,sub_win_no);
			printf("###gdb[%d][%d]->nextpos=%d\n",sub_win_no,crt_n,gcondb[sub_win_no][crt_n].nextpos);
			fclose (fp);
			obj_pnt_tab_ptr[sub_win_no][crt_n]= pnt_tab_ptr[crt_n];
			printf("obj[%d][%d].stn_id=%d,dev_id=%d,data_type=%d\n",sub_win_no,crt_n,obj_pnt_tab_ptr[sub_win_no][crt_n],obj_pnt_tab_ptr[sub_win_no][crt_n].stn_id,obj_pnt_tab_ptr[sub_win_no][crt_n].dev_id,obj_pnt_tab_ptr[sub_win_no][crt_n].data_type);
			PopupDynSubWin ( crt_n,sub_win_no,main_w[crt_n] ,shm_ots);                           
		}
		else                
			PopupMessageBox(main_w[crt_n],MSG_CLOSE_SUB_WIN);                
		
		return( TRUE);
	}
}




void	UpdateDspName ( int crt_n, char *str )
{
	char *buff[100]/*,curfilename[40]*/;
	GtkWidget* filename;
	gchar *pConvert=0;
	sprintf((char*)buff,"%s---", pConvert=_toUtf8EX( OIX_STR_OIX_TITLE) ) ;
	G_SAFE_FREE(pConvert);//dzy1017
	strcat((char *)buff,pConvert=_toUtf8(  str) );
	G_SAFE_FREE(pConvert);//dzy1017
	/*strcpy(curfilename,OIX_CSTR_CUR_DSP_NAME);
	strcat(curfilename,str);*/
	filename = g_object_get_data(G_OBJECT(main_w[crt_n]),"label_cur_filename");
	gtk_object_set(GTK_OBJECT(filename),"label",pConvert=_toUtf8(str),NULL);
	G_SAFE_FREE(pConvert);//dzy1017
	/*gtk_window_set_title (GTK_WINDOW(main_w[crt_n]), buff);*/
}




FILE	*OpenWriteFile (char	*full_name)
{
	struct	stat	statb;
	FILE	 *fp;
	
	/* make sure the file is a regular text file and open it */
	if (stat(full_name, &statb) == -1 ||
		(statb.st_mode & S_IFMT) != S_IFREG ||
		!(fp = fopen(full_name, "wb")))
	{
		
		printf ("in OpenFile: file opened.%s\n", full_name);
		if ((statb.st_mode & S_IFMT) == S_IFREG)
			perror (full_name);              /* send to stderr why we can't read it*/
		else
			printf ("OIX: in OpenFile: %s: not a regular file\n", full_name);
		
		return	NULL;
	}
	else
	{
		return (fp);
	}
}


void CheckGdbaseData ( GDBUFF  *gdb )
{
	int     i, j, nextpos;
	/*     int     point_offset    = 1;			*/
	
#ifdef    DEBUG
	printf("in CheckGdbaseData\n");
#endif
	nextpos = gdb->nextpos;
	printf(" OIX: CheckGdbaseData: Rcd total num is %d\n", gdb->nextpos );
	i = 0;
	while ( i < nextpos )
	{
		
		if ( gdb->buffer[i].bck.rect.x1 == gdb->buffer[i].bck.rect.x2 &&
			gdb->buffer[i].bck.rect.y1 == gdb->buffer[i].bck.rect.y2 )
		{
			printf(" OIX: CheckGdbaseData: Rcd is a point, delete it !\n");
			for ( j=i; j< nextpos-1; j++ )
			{
				gdb->buffer[j] = gdb->buffer[j+1];
			}
			nextpos = nextpos-1;
		}
		else i++;
	}
	gdb->nextpos = nextpos;
	printf(" OIX: CheckGdbaseData: After: Rcd total num is %d\n", gdb->nextpos );
}


