
/*************************************
	2006.2 Modified by zyp for v4.0 ;
	Windows Have  SYN to  UNIX  by chi 07.11.30 ;

**************************************/

#include <gtk/gtk.h>
#include	"../inc/gtk_ipm_k_def.h"
#include 	"../inc/gtk_widgets.h"
#include	"../inc/gtk_msg_text_def.h"
#include 	"../inc/mode_def.h"
#include	"../inc/oix.h"
extern void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir);
void DelSgzyHistFile(int crt_n,int file_type)
{
	if(file_type==DEL_SGZY_FILE)
	{
		PopFileSelectionDlg(main_w[crt_n],crt_n,_CS_(OIX_STR_FILE_SELECT_BOX),SGZY_FILE_PATH/*"/h9000his/sgzy/*.sgzy"*/);
		file_mode[crt_n] = DEL_SGZY_FILE;
	}
	else if(file_type==DEL_SFTCP_FILE)
	{
		PopFileSelectionDlg(main_w[crt_n],crt_n,_CS_(OIX_STR_FILE_SELECT_BOX),FILE_SOFTCP_PIC/*"/h9000his/sftcp/*.sfbin"*/);
		file_mode[crt_n] = DEL_SFTCP_FILE;
	}
}