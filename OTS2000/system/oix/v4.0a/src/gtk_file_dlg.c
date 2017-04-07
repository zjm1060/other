

/*************************************************

  have been SYNC
  Date : 2007.11.29
  by chi.hailong 
**************************************************/


#include <gtk/gtk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

/*  #define DEBUG  */

extern GtkWidget *alarm_window;
extern GtkWidget 	*alm_pop_shell[MAX_CRT_NUMBER];
extern	short	change_flag[];
extern  int    ReadHistRecFile (int crt_n, char *filename);

extern	GtkWidget 	*alm_pop_shell[MAX_CRT_NUMBER];

extern char Cur_Dsp_name[MAX_CRT_NUM][MAX_FULL_FILE_NAME_LEN+10];


extern BOOL SoftCopyFlag;/*true softcopy button useless,false softcopy button usefull*/

/*extern log_shell[MAX_CRT_NUMBER];   */
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern int OpenDspFile (int crt_n, char	*filename);
extern FILE    *OpenFileX (char    *full_name);
extern void    Redraw (GtkWidget *w);
extern int ReadHistRecFile (int crt_n, char *filename);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_fromUtf8(char *c);
extern void	ReadSftCpData (int crt_n, FILE	*fp);
extern void	UpdateDspName ( int crt_n, char *str );
extern void	ReadSgzyFile (int crt_n, char	*filename);
extern void ReadTrendFile (int crt_n, char	*filename);
//extern void	ReadFailFile (int crt_n, char	*filename);
extern void ReadHistLogFile(int crt_n, char filename[MAX_FULL_FILE_NAME_LEN]);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);

void FileSelOk(GtkWidget *w,gpointer arg)
{
	//struct	stat	statb;
	FILE	*fp;
	int	crt_n=0;
//	char	kind_name[10];
	char	file_part_name[MAX_CRT_NUMBER][MAX_FULL_FILE_NAME_LEN];//={""};
	char    sftcp_dsp_name[50],curfile[50];
	char 	tmp_title[100], *pChar;
	char	cmd_str[50],filenameOnly[200];
	int i  ;
	gchar *pConvert=0;
	GtkFileChooser *fileSel = GTK_FILE_CHOOSER(arg);
	char filename[256] ;/*= gtk_file_selection_get_filename(fileSel);*/
	for (i=0;i<MAX_CRT_NUMBER;i++)
		strcpy(file_part_name[i],"");
	
#ifdef WINDOWS_OS
	if(file_mode[crt_n] ==SGZY_HIST_FILE)
	{
		strcpy(filename,pConvert=_fromUtf8(gtk_file_chooser_get_filename(fileSel)));
		G_SAFE_FREE(pConvert);//dzy1017
	}
	else
#endif
		strcpy(filename,/*_fromUtf8*/(gtk_file_chooser_get_filename(fileSel)));
	printf("in FileSelOk::::file=%s\n",filename);
	strcpy(curfile,filename);
	

	strcpy(sftcp_dsp_name,"OIX_CSTR_MENU_SOFT_COPY ");    //"OIX_CSTR_MENU_SOFT_COPY" 画面软拷贝显示：
	
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;	
	
	i = 0;
	pChar = strrchr(filename,PATH_PIPE);
	
	pChar ++ ;
	while (*pChar !=0)
	{
		tmp_title[i] = *pChar  ;
		i++ ; pChar++ ;
		
	}
	tmp_title[i++] = 0 ;
	
	if (!filename || !*filename)
	{
		
		return;
	}
	printf("file_mode[crt_n]=%d\n",file_mode[crt_n]);
	switch	( file_mode[crt_n] )
	{
	case	DSP_FILE:
		OpenDspFile (  crt_n, tmp_title); 
		
		if(fileSel!=NULL)
			gtk_widget_hide(GTK_WIDGET(fileSel));
		
		break;
		
	case	SFTCP_FILE:
		fp	= OpenFileX ( filename);
		if ( fp == NULL)			
			return;			
		ReadSftCpData ( crt_n, fp);
		fclose( fp);
		strcpy( file_part_name[crt_n], strrchr( filename, PATH_PIPE )+1 );
		
		strcat ( sftcp_dsp_name, file_part_name[crt_n]);			
		Redraw (canvas[crt_n]);
		UpdateDspName (  crt_n, sftcp_dsp_name );
		strcpy(cur_dsp_name[crt_n],filename);
		strcpy(Cur_Dsp_name[crt_n], filename);
		break;
		
	case	SGZY_HIST_FILE:                    
		ReadSgzyFile (crt_n, filename);
		break;
		
	case    TREND_HIST_FILE:
		ReadTrendFile (crt_n, filename);
		break;
		
	case    FAIL_FILE:
		printf("in FAIL_FILE: filename=%s\n",filename);
//		ReadFailFile (crt_n, filename);
		break;
		
	case    RPT_HIST_FILE:
		strcpy( select_rpt_hist_name[crt_n], filename);
		if( select_rpt_hist_name[crt_n] != (char * ) NULL )
		{
			/*SetRptBtn( crt_n);*/
			if( strcmp(cur_dsp_name[crt_n],"") != 0)
			{
				if ( strrchr( cur_dsp_name[crt_n],  PATH_PIPE ) != NULL )
				{
					strcpy( file_part_name[crt_n], strrchr( cur_dsp_name[crt_n],  PATH_PIPE )+1 );
					OpenDspFile (crt_n,file_part_name[crt_n]);
					printf("in RPT_HIST_FILE: file_part_name=%s\n",file_part_name[crt_n]);
				}
			}
			
		}
		break;
		
	case	ALM_HIST_FILE:			
		strcpy( select_hist_alm_file[crt_n], filename);
		strcpy( filenameOnly , strrchr( filename, PATH_PIPE )+1 );
		printf("select_hist_alm_file=%s\n",select_hist_alm_file[crt_n]);		
		if (ReadHistRecFile (crt_n, filename)==FALSE) 
		{
			if(file_select_dialog[crt_n]!=NULL)
			{			
				gtk_widget_destroy(file_select_dialog[crt_n]);
				file_select_dialog[crt_n]= NULL;			
			}
			return ;
		}
		//strcpy(tmp_title,"事件文件显示窗口------");			
		//strcat(tmp_title,filenameOnly);
		if (alm_pop_shell[crt_n])
		{
			printf ("alm his file name = %s\n",tmp_title);
			strcpy(tmp_title,pConvert=_toUtf8EX(OIX_CSTR_FILE_SELECT_WIN));
			G_SAFE_FREE(pConvert);//dzy1017
			gtk_window_set_title(GTK_WINDOW(alm_pop_shell[crt_n]), strcat(tmp_title,filenameOnly));
		}
		break;
	case	LOG_HIST_FILE:	                             
		printf("select_hist_log_file=%s\n",filename);
		if(strstr(filename,".txt")!=NULL)
		{
			PopupMessageBox ( main_w[crt_n],MSG_FILE_TYPE_ERROR);
			break;
		}
		ReadHistLogFile (crt_n, filename);
		strcpy(tmp_title,OIX_HIS_FAULT_WIN);			
		strcat(tmp_title,filename);
		log_rec_mode[crt_n]	= LOG_REC_HISTRY;
		if (log_shell[crt_n])
		{
			gtk_window_set_title(GTK_WINDOW(log_shell[crt_n]), pConvert=_toUtf8(tmp_title));
			G_SAFE_FREE(pConvert);//dzy1017
		}
		break;	
		
	case	DEL_SGZY_FILE:
		if(strstr(filename,"sgzy")== (char *) NULL)
		{
			PopupMessageBox ( main_w[crt_n],MSG_FILE_MODE_ERROR);
			if(file_select_dialog[crt_n]!=NULL)
			{			
				gtk_widget_destroy(file_select_dialog[crt_n]);
				file_select_dialog[crt_n]= NULL;			
			}
			return;
		}
#ifdef WINDOWS_OS
		strcpy(cmd_str,"del ");		
#endif
#ifdef  SUN_OS
		strcpy(cmd_str,"rm ");		
#endif
		strcat(cmd_str,filename);
		printf("cmd_str=%s\n",cmd_str);					
		system(cmd_str);
		break;
		
	case	DEL_SFTCP_FILE:
		if(strstr(filename,"sfbin")==NULL)
		{
			PopupMessageBox ( main_w[crt_n],MSG_FILE_MODE_ERROR);
			if(file_select_dialog[crt_n]!=NULL)
			{			
				gtk_widget_destroy(file_select_dialog[crt_n]);
				file_select_dialog[crt_n]= NULL;			
			}
			return;
		}
		
#ifdef WINDOWS_OS
		strcpy(cmd_str,"del ");		
#endif
#ifdef  SUN_OS
			strcpy(cmd_str,"rm ");		
#endif
		strcat(cmd_str,filename);
		printf("cmd_str=%s\n",cmd_str);					
		system(cmd_str);
		break;	      
	default:
		
		break;
		}                
		if(file_select_dialog[crt_n]!=NULL)
		{			
			gtk_widget_destroy(file_select_dialog[crt_n]);
			file_select_dialog[crt_n]= NULL;			
		}			
}

void FileSelCancel(GtkWidget *w,gpointer arg)
{
	int crt_n = (gint) arg ;
    /*   GetCrtNumberOfWgt (w, &crt_n );  */
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	
	
	gtk_widget_destroy(file_select_dialog[crt_n]);
	file_select_dialog[crt_n] = NULL;
}

void FileSelEntry(GtkWidget *w,gpointer arg)
{
	int crt_n = 0 ;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	if (file_select_dialog[crt_n])
	{
		gtk_widget_destroy(file_select_dialog[crt_n]);
		file_select_dialog[crt_n] = NULL;
	}
	
}


void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir)
{
	GtkFileFilter	*pFilter;
	
	gchar strPath[256];
	gchar strFilter[64];
	gchar suffix[32];

	int nLen;

	gchar *pConvert=0;
	gchar *ptr;

	if (file_select_dialog[crt_n] == (GtkWidget*)NULL)
	{     //zlb 原文件打开对话框不支持中文
//         file_select_dialog[crt_n]=gtk_file_selection_new((title));
//         gtk_signal_connect(GTK_OBJECT(file_select_dialog[crt_n]),"destroy", GTK_SIGNAL_FUNC(FileSelCancel),
// 			(gpointer)crt_n);
// 		
//         gtk_file_selection_set_filename(GTK_FILE_SELECTION(file_select_dialog[crt_n]),dir);
//         gtk_file_selection_complete(GTK_FILE_SELECTION(file_select_dialog[crt_n]),dir);
// 		gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(file_select_dialog[crt_n]));
// 		gtk_window_set_transient_for (GTK_WINDOW(file_select_dialog[crt_n]),GTK_WINDOW(parent));
// 		gtk_window_set_destroy_with_parent(GTK_WINDOW(file_select_dialog[crt_n]),TRUE);
// 		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(file_select_dialog[crt_n])->ok_button),"clicked",
// 			(GtkSignalFunc)FileSelOk,file_select_dialog[crt_n]);
// 		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(file_select_dialog[crt_n])->cancel_button),"clicked",
// 			(GtkSignalFunc)FileSelCancel,(gpointer) crt_n );
// 		
//         gtk_widget_show (file_select_dialog[crt_n]);  
//         
		nLen=strlen(dir);
		if (dir[nLen-1] == '/' || dir[nLen-1] == '\\')
		{
			strcpy(strPath, dir);
			memset(strFilter, 0, sizeof(strFilter));
		}
		else
		{
#ifdef WINDOWS_OS
			ptr=strrchr(dir, '\\');
#elif defined(SUN_OS)
			ptr=strrchr(dir, '/');
#endif
			if (ptr != 0)
			{
				memcpy(strPath, dir, ptr-dir+1);
				strPath[ptr-dir+1]=0;
				strcpy(strFilter, ptr+1);
			}
			else
			{
				memset(strPath, 0, sizeof(strPath));
				strcpy(strFilter, ptr);
			}
		}

		file_select_dialog[crt_n] = gtk_file_chooser_dialog_new (pConvert=_toUtf8(title),
			GTK_WINDOW(parent),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			NULL);

		G_SAFE_FREE(pConvert);

		gtk_dialog_set_alternative_button_order (GTK_DIALOG (file_select_dialog[crt_n]), GTK_RESPONSE_ACCEPT, GTK_RESPONSE_CANCEL, -1);
		
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_select_dialog[crt_n]), strPath);
		
		pFilter=gtk_file_filter_new();
		if (strlen(strFilter) != 0)
		{
			ptr=strrchr(strFilter, '.');
			if (ptr != NULL)
			{
				sprintf(suffix, "%s Files(%s)", ptr+1, strFilter);
			}
			else
			{
				sprintf(suffix, "%s Files(%s)", strFilter, strFilter);
			}

			gtk_file_filter_set_name(pFilter, suffix);
			gtk_file_filter_add_pattern(pFilter, strFilter);
		}
		else
		{
			gtk_file_filter_set_name(pFilter, "All Files(*.*)");
			gtk_file_filter_add_pattern(pFilter, "*.*");
		}
		
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_select_dialog[crt_n]), pFilter);
		
		if (gtk_dialog_run (GTK_DIALOG (file_select_dialog[crt_n])) == GTK_RESPONSE_ACCEPT)
		{
// 			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_select_dialog[crt_n]));
// 			
// 			if (filename != NULL)
// 			{
// #ifdef SUN_OS
// 				sprintf(strFilePath, "%s", filename);
// #else
// 				sprintf(strFilePath, "%s", pConvert=_fromUtf8(filename));
// 				G_SAFE_FREE(pConvert);
// #endif
// 				
// 				g_free (filename);
// 
// 				
// 			}

			FileSelOk(file_select_dialog[crt_n], file_select_dialog[crt_n]);
		}
		else
		{
			gtk_widget_destroy (file_select_dialog[crt_n]);
			file_select_dialog[crt_n]=NULL;
		}
	}
	else
		gtk_window_present ( GTK_WINDOW(file_select_dialog[crt_n]));
}			
