#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "../inc/gtk_widgets.h" 
#include "../inc/gtk_msg_text_def.h"
#include "../inc/gtk_oix_cstr.h"
#include "../../../dms/v4.0a/inc/dms_com.h"
#include	"../inc/oix.h"

static DMS_COMMON check_dms_cmm[MAX_CRT_NUMBER];
GtkWidget *PtCheckDlg[MAX_CRT_NUMBER]= {NULL,NULL,NULL};
POINTER		check_pt[MAX_CRT_NUMBER];
GtkWidget *entry_value[MAX_CRT_NUMBER][10]= {NULL,NULL,NULL};
int pt_check_timeout_id[MAX_CRT_NUMBER];

extern char *_toUtf8EX(char *c);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void	GetDbDetailFixedInfo(DMS_COMMON *db_cmm, int *item_num, char entry_str[][64], char value_str[][POINT_LONGNAME_SIZE] );
extern void GetAnaRtVal(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE]);

gint ptCheckDlgQuit(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	/*  GetCrtNumberOfWgt(widget,&crt_n);  */
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE; 
	
	if(pt_check_timeout_id[crt_n]!=0)
	{
		gtk_timeout_remove(pt_check_timeout_id[crt_n]);
		pt_check_timeout_id[crt_n]= 0;
	}
	if(PtCheckDlg[crt_n])
	{
		gtk_widget_destroy(PtCheckDlg[crt_n]);	
		PtCheckDlg[crt_n] =NULL;
	}
	
	return TRUE;
	
}
int GetPtCheckValue(DMS_COMMON *db_cmm,char entry_str[][64],char value_str[][POINT_LONGNAME_SIZE])
{
	int item = 0;
	char tmp_state[50];
	strcpy(entry_str[item], _CS_(OIX_STR_IO_ADDRESS));
	sprintf(value_str[item],"%d.%d.%d.%d",db_cmm->point.stn_id,db_cmm->point.dev_id,db_cmm->point.data_type,db_cmm->point.pt_id);	
	item++;
	
	strcpy(entry_str[item], _CS_(OIX_CSTR_NAME));
	GetPtNameById(&db_cmm->point,value_str[item],NULL);	
	item++;
	
	strcpy(entry_str[item], _CS_(OIX_CSTR_LONGNAME));
	GetEntryLongnameById(&db_cmm->point,value_str[item]);	
	item++;
	
	switch(db_cmm->point.data_type)
	{
	case	ANA_TYPE:
	case	PARAM_TYPE:
	case	OUTA_TYPE: 
	case	CALC_TYPE:
		strcpy(entry_str[item], _CS_(OIX_CSTR_RT_VALUE));
		sprintf(value_str[item], "%.3f", db_cmm->var.anlg.fValue); 
		item++;
		break;
	case	POL_TYPE:
	case	SOE_TYPE:
	case	SYSMSG_TYPE:
	case    OUTS_TYPE:
		strcpy(entry_str[item], _CS_(OIX_CSTR_RT_STATE));
		GetIndPtRtStateStrById(&db_cmm->point, value_str[item] );
        sprintf(tmp_state,"(%d)",db_cmm->var.ind.status.rt_state);
        strcat(value_str[item],tmp_state);
		item++;
		break;
	case    IMP_TYPE:
		strcpy(entry_str[item], _CS_(OIX_STR_REAL_ENERGY));
		sprintf(value_str[item], "%u", db_cmm->var.imp.entry.rt_value); 
		item++;
		break;
	case	CHARA_TYPE:
		strcpy(entry_str[item], _CS_(OIX_STR_POINT_NUM));
		sprintf(value_str[item], "%d", db_cmm->var.chara.pt_num); 
		item++;	
		break;
	case	CURVE_TYPE:
		strcpy(entry_str[item], "seg0");
		sprintf(value_str[item],"%.0f ", db_cmm->var.curve.data.value[0]);
		break;
	case    DEVOBJ_TYPE:
		strcpy(entry_str[item],_CS_(OIX_CSTR_OPERATE_STATE));
		sprintf(value_str[item],"%d",db_cmm->var.devobj.status.opr);
		item++;	
		break;
	case TIME_TYPE:
		strcpy(entry_str[item],_CS_(OIX_CSTR_TIMER));
		sprintf(value_str[item],"%.4f",db_cmm->var.time.coords.x);
		item++;
		break;
	default:
		break;
	}
	return item;
}
gint ProgPtCheck(GtkWidget *widget , gpointer data)
{
	int crt_n;
	int type;
	char str[9],msg[SMS_MESSAGE_SIZE];
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return FALSE;

	type = GPOINTER_TO_INT(data);
	if (type == 1)
	{
		sprintf(str,"%s","Õý³£");
	}
	else
	{
		sprintf(str,"%s","Òì³£");
	}
	sprintf(msg,"pt_id:%s %s",gtk_entry_get_text(entry_value[crt_n][0]),str);
	if(strlen(msg)>SMS_MESSAGE_SIZE)
		msg[SMS_MESSAGE_SIZE-1]='\0';
	BdSms(msg);
	ptCheckDlgQuit(widget,GINT_TO_POINTER(crt_n));
}
int UpdateDspPtCheckDlg(GtkWidget *parent)
{
	int		crt_n;    
	char	entry_str[10][64];
	char 	value_str[10][POINT_LONGNAME_SIZE];
	int		item_num,item;
	POINTER pt;
	gchar *pConvert=0;
	GetCrtNumberOfWgt(parent, &crt_n);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return FALSE;
	pt = check_pt[crt_n];
	if ( ReadEntryById ( &pt, &check_dms_cmm[crt_n] ) == -1 )
	{		
		PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
	item_num = GetPtCheckValue(&check_dms_cmm[crt_n],entry_str,value_str);
	for (item=0;item<item_num;item++)
	{
		gtk_entry_set_text(GTK_ENTRY(entry_value[crt_n][item]),pConvert= _toUtf8EX(value_str[item]));
		G_SAFE_FREE(pConvert);//dzy1017
	}
	return TRUE;
}

void DspPtCheckDlg(int crt_n,GtkWidget *window)
{
	GtkWidget *label[10],*hbox[10];
	GtkWidget *vbox,*hbox_button,*button_normal,*button_anomaly;
	int item;
	char	entry_str[10][64];
	char 	value_str[10][POINT_LONGNAME_SIZE];
	int		item_num;
	gchar *pConvert=0;
	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	item_num = GetPtCheckValue(&check_dms_cmm[crt_n],entry_str,value_str);
	for (item=0;item<item_num;item++)
	{
		hbox[item]=gtk_hbox_new(FALSE, 2);
		label[item] =gtk_label_new(pConvert =_toUtf8EX(entry_str[item]));
		G_SAFE_FREE(pConvert);//dzy1017
		entry_value[crt_n][item] =gtk_entry_new();
		gtk_widget_set_usize(GTK_WIDGET(entry_value[crt_n][item]), 250, 20);
		gtk_entry_set_text(GTK_ENTRY(entry_value[crt_n][item]),pConvert= _toUtf8EX(value_str[item]));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_entry_set_editable(GTK_ENTRY(entry_value[crt_n][item]),FALSE);
		gtk_widget_set_sensitive(entry_value[crt_n][item],FALSE);
		gtk_box_pack_start(GTK_BOX(hbox[item]),label[item],FALSE,FALSE,40);
		gtk_box_pack_end(GTK_BOX(hbox[item]),entry_value[crt_n][item],FALSE,FALSE,40);
		gtk_box_pack_start(GTK_BOX(vbox),hbox[item],FALSE,FALSE,5);
	}
	button_normal = gtk_button_new_with_label(_toUtf8EX(OIX_CSTR_CHECK_NORMAL));
	button_anomaly = gtk_button_new_with_label(_toUtf8EX(OIX_CSTR_CHECK_ANOMALY));
	gtk_widget_set_usize(GTK_WIDGET(button_normal), 120, 50);
	gtk_widget_set_usize(GTK_WIDGET(button_anomaly), 120, 50);
	hbox_button = gtk_hbox_new(FALSE, 70);
	gtk_box_pack_start(GTK_BOX(hbox_button),button_normal,FALSE,FALSE,30);
	gtk_box_pack_end(GTK_BOX(hbox_button),button_anomaly,FALSE,FALSE,30);
	gtk_box_pack_start(GTK_BOX(vbox),hbox_button,FALSE,FALSE,40);
	gtk_signal_connect(GTK_OBJECT(button_normal), "clicked", GTK_SIGNAL_FUNC(ProgPtCheck),GINT_TO_POINTER(1));
	gtk_signal_connect(GTK_OBJECT(button_anomaly), "clicked", GTK_SIGNAL_FUNC(ProgPtCheck),GINT_TO_POINTER(0));
}


GtkWidget* PopupPtCheckBox(GtkWidget *parent,int crt_n,POINTER pt)
{
	char  EntryName[100] ;
	char  PtInfoTitle[150] ;
	char LongName[100];
	GtkWidget *window;
	char *text ;
	
	if(PtCheckDlg[crt_n])
		ptCheckDlgQuit(PtCheckDlg[crt_n],GINT_TO_POINTER(crt_n));
	
	if ( ReadEntryById ( &pt, &check_dms_cmm[crt_n] ) == -1 )
	{		
		PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
	
	if ( GetPtNameById(&pt,EntryName,NULL)  == -1 )
	{		
		PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
	if ( GetPtLongnameById ( &pt, LongName ) == -1 )
	{		
        PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
	check_pt[crt_n]= pt;
	sprintf (PtInfoTitle, "%s-%s",LongName , EntryName);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(window),GTK_WINDOW(parent));  
	gtk_widget_set_usize(GTK_WIDGET(window), 400, 250);
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window),TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), text =_toUtf8(PtInfoTitle));  
	G_SAFE_FREE(text);//dzy1017
    gtk_window_set_policy(GTK_WINDOW(window),FALSE,FALSE,TRUE);
    
	DspPtCheckDlg(crt_n,window);
    gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(ptCheckDlgQuit), (gpointer)crt_n);	
	pt_check_timeout_id[crt_n]= gtk_timeout_add (1000, (GtkFunction)UpdateDspPtCheckDlg, (gpointer)parent);

    gtk_widget_show_all(window);
	PtCheckDlg[crt_n]= window;
    return  window ;
}

