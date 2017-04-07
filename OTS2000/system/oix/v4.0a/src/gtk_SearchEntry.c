/******
	Created by zyp for gtk 2005.12.30
	SYNC by hcl 2007.12.09
*******/

#include <stdio.h>
#include <gtk/gtk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern GtkWidget* PopupDetailInfoBox(GtkWidget *parent,int crt_n,POINTER pt);
extern char *_toUtf8EX(char *c);
 

extern GtkWidget*
create_search_window (GtkWidget *parent,int crt_n);
GtkWidget *searchdialog[MAX_CRT_NUMBER];

GtkWidget *EntryName[MAX_CRT_NUMBER];




void DestroyEntryDlg(GtkWidget *w)
{	
	char entry_name[POINT_NAME_SIZE];
	POINTER		point;
	int	crt_n=0;

	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return ;
        
	strcpy(entry_name,gtk_entry_get_text(GTK_ENTRY(EntryName[crt_n])));

	GetPtIdFromStr(entry_name, &point);
	if(!IsPointExist(&point))
	{
		PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);
		gdk_beep();
		gtk_widget_destroy(w);
		return;	
	}	
	
	PopupDetailInfoBox(main_w[crt_n],crt_n,point);
	gtk_widget_destroy(w);
}

void PopEntryInputDlg(GtkWidget *parent,int crt_n)
{
	GtkWidget *button,*lable,*dialog[MAX_CRT_NUMBER];
	gchar *pConvert=0;
	
	dialog[crt_n] = gtk_dialog_new();
        
	gtk_widget_set_usize(dialog[crt_n],214,117);
	gtk_window_set_policy(GTK_WINDOW(dialog[crt_n]),FALSE,FALSE,FALSE);
	gtk_window_set_title(GTK_WINDOW(dialog[crt_n]), pConvert=_toUtf8EX(OIX_STR_SEARCH));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(dialog[crt_n]), GTK_WINDOW(parent));
        

	lable=gtk_label_new(_toUtf8EX(OIX_STR_SRH_PTORNAME));
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->vbox),lable,TRUE,TRUE,0);	

	EntryName[crt_n] = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->vbox),EntryName[crt_n],TRUE,TRUE,0);	

		       
	button = gtk_button_new_with_label("ok");
	GTK_WIDGET_SET_FLAGS(button,GTK_CAN_DEFAULT);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->action_area),button,FALSE,FALSE,0);
	gtk_widget_grab_default(button);
	
	gtk_signal_connect_object(GTK_OBJECT(EntryName[crt_n]), "activate",
		       GTK_SIGNAL_FUNC(DestroyEntryDlg),GTK_OBJECT (dialog[crt_n]));
		       
	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			       GTK_SIGNAL_FUNC(DestroyEntryDlg),
			       GTK_OBJECT (dialog[crt_n]));
	gtk_widget_show_all(dialog[crt_n]);	
}



void PopSearchWin(GtkWidget *parent,int crt_n)
{	
	searchdialog[crt_n] = create_search_window (parent,crt_n);/*gtk_dialog_new();*/       
}

