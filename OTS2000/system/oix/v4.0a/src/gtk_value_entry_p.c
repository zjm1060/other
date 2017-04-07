/*********************************************************************************

  Windows版本已经与Unix 版本SYNC！！！！   2007.04.04 by CHI.HAI.LONG
	 Last Update : 2207.06.08				by CHI.HAI.LONG ;
	 
**********************************************************************************/

#include <stdio.h>
#include <gtk/gtk.h>


#include	"../inc/oix.h"
#include "menusrc/support.h" //add by hcl

extern void DestroyDlg(GtkWidget *w);
extern void	FormCEnterValueAdjItem (int crt_n,float ent_value,LIMIT_VAL *lmtVal, int useLmt);
extern	POINTER		pnt_tab_ptr[MAX_CRT_NUMBER];
extern char            cmd_str[100];

int	InitializeValueEntryBox	(int crt_n,LIMIT_VAL  *lmtVal,POINTER *pt,int type);
static	GtkWidget* CreateValueEntryBox (GtkWidget *parent ,int crt_n ,POINTER *pt,int type,int entry_data_class);
void	GetAnalogLimitValue (int lmt_type,DMS_COMMON dms_cmm, /*float	*rt_value*/LIMIT_VAL  *lmtVal );
void	PopupOperCondWnd (int crt_n );
void PopManSetTimeInputDlg(GtkWidget *parent ,int crt_n ,GBRCD  *rcd);
gint DestoryAdjust (GtkWidget *widget , gpointer data );
GtkWidget *CreateManSetTimeInputDlg(GtkWidget *parent, int crt_n ,int inputParaflag,GBRCD *rcd,POINTER* point);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	ProgDoAckCel ( int crt_n );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void 	CmdBufInit ( int crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] );

GtkWidget *value_entry_box[MAX_CRT_NUMBER];
GtkWidget *value_box_no_scale[MAX_CRT_NUMBER];
GtkWidget *TimeEntry[MAX_CRT_NUMBER];
GtkWidget *curve_input_box[MAX_CRT_NUMBER];
GtkWidget *man_set_time_dlg[MAX_CRT_NUMBER];
POINTER *value_entry_pt[MAX_CRT_NUMBER];
enum {
	RANGE=0, 
		WARN,
		ALARM
		};

void DestroyTimeDlg(GtkWidget *w)
{
	int	crt_n=0;
	char    time_str[10];
	
	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return ;
	
	strcpy(time_str,gtk_entry_get_text(GTK_ENTRY(TimeEntry[crt_n])));        
	cmd_buf[crt_n].data.iValue	= atoi(time_str);
	ProgDoAckCel(crt_n);
	
	gtk_widget_destroy(w);
}


void PopTimeInputDlg(GtkWidget *parent,int crt_n)
{
	GtkWidget *ok_button,*cancel_button,*dialog[MAX_CRT_NUMBER];
	gchar *pConvert=0;
	
	dialog[crt_n] = gtk_dialog_new();
	
	gtk_widget_set_usize(dialog[crt_n],214,117);
	gtk_window_set_policy(GTK_WINDOW(dialog[crt_n]),FALSE,FALSE,FALSE);
	gtk_window_set_title(GTK_WINDOW(dialog[crt_n]), pConvert=_toUtf8EX(OIX_STR_TIME));
	G_SAFE_FREE(pConvert);

	gtk_window_set_transient_for(GTK_WINDOW(dialog[crt_n]), GTK_WINDOW(parent));
	gtk_window_set_modal(GTK_WINDOW(dialog[crt_n]),TRUE);
	
	TimeEntry[crt_n] = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->vbox),TimeEntry[crt_n],TRUE,TRUE,0);	
	gtk_entry_set_text(GTK_ENTRY(TimeEntry[crt_n]),"0");
	
	ok_button = gtk_button_new_with_label("ok");
	GTK_WIDGET_SET_FLAGS(ok_button,GTK_CAN_DEFAULT);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->action_area),ok_button,FALSE,FALSE,0);
	gtk_widget_grab_default(ok_button);
	
	gtk_signal_connect_object(GTK_OBJECT(TimeEntry[crt_n]), "activate",
		GTK_SIGNAL_FUNC(DestroyTimeDlg),GTK_OBJECT (dialog[crt_n]));
	
	gtk_signal_connect_object (GTK_OBJECT (ok_button), "clicked",
		GTK_SIGNAL_FUNC(DestroyTimeDlg),
		GTK_OBJECT (dialog[crt_n]));
	
	cancel_button = gtk_button_new_with_label("cancel");	
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->action_area),cancel_button,FALSE,FALSE,0);	
	gtk_signal_connect_object (GTK_OBJECT (cancel_button), "clicked",
		GTK_SIGNAL_FUNC(DestroyDlg),
		GTK_OBJECT (dialog[crt_n]));
	gtk_widget_show_all(dialog[crt_n]);	
}


gint  on_hscale1_value_changed(GtkAdjustment      *adjust,   gpointer    entrys)
{
	char buffer[16];
	
	sprintf(buffer,"%.2f",     adjust->value);
	gtk_entry_set_text (GTK_ENTRY (entrys), buffer);
	return TRUE;
}

gint GetScaleVal(GtkWidget *w,gpointer entry)
{
	char buffer[16],cmd_str[512];
	char TipMsg[256] ,cmdStr[256],stnName[128],devName[128],LongNamerStr1[128]; 
	float value; 
	LIMIT_VAL* plmit ;
	POINTER  pt ; 
	int crt_n=0;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ; 
	pt.stn_id  = cmd_buf[crt_n].stn_id ;
	pt.dev_id  = cmd_buf[crt_n].dev_id ;
	pt.data_type  = cmd_buf[crt_n].data_type ;
	pt.pt_id  = cmd_buf[crt_n].point_id ;
	strcpy(buffer,gtk_entry_get_text (GTK_ENTRY (entry))); 
    plmit = g_object_get_data(G_OBJECT(entry),"limtVal");
    

printf("buffer=%s\n",buffer);
	/*gtk_widget_destroy(value_entry_box[crt_n]);  */
	sscanf(buffer,"%f",&value);
printf("***value=%f\n",value);
//StnDef[i].CName
	DestoryAdjust ( NULL ,  GINT_TO_POINTER(crt_n) ) ;
    
	if(-1==GetPtCmdMsgStrById(&pt, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, cmdStr))               
		strcpy(cmdStr,"CmdMsgError!") ; 
	if (-1== GetStnLongnameById( pt.stn_id,  stnName ))
		strcpy(stnName,"CmdMsgError!") ; 
    if (-1==GetGroupLongnameById( pt.stn_id, pt.dev_id, devName ))  
		strcpy(devName,"DevNameError!") ;
	if (-1==GetEntryLongnameById (&pt,LongNamerStr1 ))
		strcpy(LongNamerStr1,"LongNameError!") ;
	sprintf(cmd_str,"%s:\n%s:\n%s:\n%s\0",StnDef[pt.stn_id].CName,devName,LongNamerStr1  ,cmdStr );
	sprintf (TipMsg,"%s\n 目标数值 %.2f\n将要下达!\0",cmd_str,value) ;
	
	if (PopupMessageBoxRet(main_w[crt_n],TipMsg)==GTK_RESPONSE_CANCEL)
		return TRUE ; 

	FormCEnterValueAdjItem (crt_n,  value,plmit,TRUE);
//	DestoryAdjust ( NULL ,  GINT_TO_POINTER(crt_n) ) ;
	return TRUE;
}



gint DestoryAdjust (GtkWidget *widget , gpointer data )
{
    int crt_n = (gint) data ;
	gtk_widget_destroy(GTK_WIDGET(value_entry_box[crt_n ]));
	
	value_entry_box[crt_n ] = NULL ;
	
	return TRUE;
	
}


gint WindowExit(GtkWidget *widget)
{
    int crt_n;
	
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 )
		return FALSE; 
	gtk_widget_destroy(GTK_WIDGET(value_entry_box[crt_n ]));
	value_entry_box[crt_n] = NULL ;
	return TRUE;
}




void PopupValueEntryBox (GtkWidget *parent ,int crt_n ,POINTER *pt,int entry_data_class)
{
	int type;

    if (value_entry_box[crt_n ] ) 
			gtk_widget_destroy(GTK_WIDGET(value_entry_box[crt_n ]));        
	

	if (strcmp( _CS_(OIX_P_Q_RANGE_OR_ALARM),"0")==0||strcmp( _CS_(OIX_P_Q_RANGE_OR_ALARM),"OIX_P_Q_RANGE_OR_ALARM")==0)
	{
		type=RANGE;
	}else if(strcmp( _CS_(OIX_P_Q_RANGE_OR_ALARM),"1")==0)
	   {
			type=WARN;
	   }else if(strcmp( _CS_(OIX_P_Q_RANGE_OR_ALARM),"2")==0)
	   {
		   type=ALARM;
	   }
	value_entry_box[crt_n ] =  CreateValueEntryBox(main_w[crt_n], crt_n, pt,type,entry_data_class);
	
	if (!value_entry_pt[crt_n])
	{
		value_entry_pt[crt_n]=malloc(sizeof(POINTER));
	}
	value_entry_pt[crt_n]->data_type=pt->data_type;
	value_entry_pt[crt_n]->dev_id=pt->dev_id;
	value_entry_pt[crt_n]->pt_id=pt->pt_id;
	value_entry_pt[crt_n]->stn_id=pt->stn_id;
	value_entry_pt[crt_n]->update_src=pt->update_src;
}

void
on_radiobutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer entry)
{

	POINTER  *pt ; 
	int crt_n=0;
	if (GTK_TOGGLE_BUTTON (togglebutton)->active) 
    {
	GetCrtNumberOfWgt (GTK_WIDGET(togglebutton), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 

	pt =value_entry_pt[crt_n];
    
	DestoryAdjust ( NULL ,  GINT_TO_POINTER(crt_n) ) ;

	value_entry_box[crt_n ] =  CreateValueEntryBox(main_w[crt_n], crt_n, pt,RANGE,-1);
	}	
}

void
on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer entry)
{

	POINTER  *pt ; 
	int crt_n=0;
	if (GTK_TOGGLE_BUTTON (togglebutton)->active) 
    {
        /* If control reaches here, the toggle button is depressed. */
    
	GetCrtNumberOfWgt (GTK_WIDGET(togglebutton), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ; 
	
	pt = value_entry_pt[crt_n];
    
	DestoryAdjust ( NULL ,  GINT_TO_POINTER(crt_n) ) ;
	
	value_entry_box[crt_n ] =  CreateValueEntryBox(main_w[crt_n], crt_n, pt,WARN,-1);
	}
}
void
on_radiobutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer entry)
{

	POINTER  *pt ; 
	int crt_n=0;
	if (GTK_TOGGLE_BUTTON (togglebutton)->active) 
    {
        /* If control reaches here, the toggle button is depressed. */
   
	GetCrtNumberOfWgt (GTK_WIDGET(togglebutton), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	
	pt = value_entry_pt[crt_n];
    
	DestoryAdjust ( NULL ,  GINT_TO_POINTER(crt_n) ) ;
	
	value_entry_box[crt_n ] =  CreateValueEntryBox(main_w[crt_n], crt_n, pt,ALARM,-1);
	}
}


static GtkWidget*
CreateValueEntryBox (GtkWidget *parent ,int crt_n ,POINTER *pt,int type,int entry_data_class)
//type:value range type(RANGE,WARN,ALARM)
//entry_data_class:input value or para,entry_data_class=-1 change value as before,entry_data_class>1 change para,H_LMT must < RANGE
{
	
	GtkWidget *vbox1;
	GtkWidget *label2,*label3,*label4,*hseparator1;
	GtkWidget *entry1;
	GtkWidget *hscale1;
	GtkWidget *adjust1;
	GtkWidget *hbox1,*hbox2,*hbox3,*hbox4;
	GtkWidget *btn_ok;
	GtkWidget *btn_cancel;
	GtkWidget *windows;
	GtkWidget *label1;
	GtkWidget *radiobutton1;
	GSList *radiobutton1_group = NULL;
	GtkWidget *radiobutton2;
  GtkWidget *radiobutton3;

  gchar *pConvert=0;

	//DMS_COMMON	db_cmm;
	char	val_str[64],up_str[64],dn_str[64];
	static LIMIT_VAL  lmtVal  ;
	char PtCname[256] ,winTitle[256] ,groupName[256];
	PangoFontDescription *font1 ; 
	font1 = pango_font_description_from_string("Arial Black"); 
	pango_font_description_set_size (font1, 11 * PANGO_SCALE); 

		
	windows  = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	/*gtk_window_set_default_size(GTK_WINDOW(windows),300,200) ;*/
	gtk_widget_set_usize(windows,300,200) ;
	gtk_window_set_position(GTK_WINDOW(windows),GTK_WIN_POS_CENTER);// 无此语句，调功窗口副屏出现到主屏上
	gtk_window_set_title(GTK_WINDOW(windows), pConvert=_toUtf8(cmd_str));/* added by zyp on 2007.7.31*/
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows), GTK_WINDOW(main_w[crt_n])); 
	
	gtk_signal_connect(GTK_OBJECT(windows), "delete_event",
		GTK_SIGNAL_FUNC(WindowExit), NULL);
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows),TRUE);
	
	/*add name by chichi 07.11.18*/
    if (GetGroupLongnameById( pt->stn_id, pt->dev_id, groupName )!=-1)
	{
		if ( GetEntryLongnameById (pt, PtCname ) != -1)
		{
			if(strcmp(cmd_str,"")==0)	/* added by zyp on 2007.7.31*/
				sprintf(winTitle,"%s:%s",groupName,PtCname) ;
			else
			sprintf(winTitle,"%s:%s--%s",groupName, PtCname,cmd_str);
			
		}
	}
	else
		strcpy(winTitle,OIX_CSTR_DATA_INPUT);			
	
	gtk_window_set_title (GTK_WINDOW (windows), pConvert=_toUtf8(winTitle));
	G_SAFE_FREE(pConvert);
	gtk_window_set_policy(GTK_WINDOW (windows),FALSE,FALSE,FALSE);
	
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (windows), vbox1);
	
	hbox2 = gtk_hbox_new (FALSE, 0);
	gtk_widget_set_usize(hbox2,300,60) ;
	gtk_widget_show (hbox2);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox2, FALSE, FALSE, 0);
	
	
	
	label2 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_CURRENT_VALUE));
	G_SAFE_FREE(pConvert);

	gtk_widget_show (label2);
	gtk_box_pack_start (GTK_BOX (hbox2), label2, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);
	
	entry1 = gtk_entry_new ();
	gtk_widget_modify_font (entry1, font1);
	gtk_widget_show (entry1);

	gtk_box_pack_start (GTK_BOX (hbox2), entry1, TRUE, TRUE, 0);
	hbox3 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox3);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox3, TRUE, TRUE, 0);
	
	label3 = gtk_label_new (("left:0"));
	gtk_widget_show (label3);
	gtk_box_pack_start (GTK_BOX (hbox3), label3, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);
	
	label4 = gtk_label_new (("right:200"));
	gtk_widget_show (label4);
	gtk_box_pack_end (GTK_BOX (hbox3), label4, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_RIGHT);
	
	hseparator1 = gtk_hseparator_new ();
	gtk_widget_show (hseparator1);
	gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, TRUE, TRUE, 0);
	
	
	
	InitializeValueEntryBox	( crt_n,  &lmtVal,pt ,type);
	sprintf(up_str,"%.2f",  lmtVal.up_lmt );
	sprintf(dn_str,"%.2f",lmtVal.dn_lmt    );
	gtk_label_set_text(GTK_LABEL(label3),dn_str);
	gtk_label_set_text(GTK_LABEL(label4),up_str);
	
	sprintf(val_str,"%.2f",lmtVal.cur_val);
	gtk_entry_set_text(GTK_ENTRY(entry1), val_str );	
	
	/*  value lower ,upper ,step_increase , page_increment ,pagesize   */
	
	adjust1 = GTK_WIDGET(gtk_adjustment_new (lmtVal.cur_val, lmtVal.dn_lmt , (lmtVal.up_lmt+.1), .1, .1, .1));
    gtk_widget_modify_font (adjust1, font1);	

	hscale1 = gtk_hscale_new (GTK_ADJUSTMENT (adjust1));
	gtk_widget_modify_font (hscale1, font1);
	gtk_widget_set_usize(hbox2,300,60) ;
	gtk_widget_show (hscale1);
	gtk_box_pack_start (GTK_BOX (vbox1), hscale1, TRUE, TRUE, 0);
	
	hbox4 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox4);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox4, TRUE, TRUE, 0);
	
	label1 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_RANGE_TYPE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label1);
	gtk_box_pack_start (GTK_BOX (hbox4), label1, FALSE, FALSE, 0);
	
	radiobutton1 = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(OIX_CSTR_USE_RANGE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (radiobutton1);
	gtk_box_pack_start (GTK_BOX (hbox4), radiobutton1, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton1), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton1));
	
	radiobutton2 = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(OIX_CSTR_USE_WARN));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (radiobutton2);
	gtk_box_pack_start (GTK_BOX (hbox4), radiobutton2, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton2), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton2));
	
	radiobutton3 = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(OIX_CSTR_USE_ALARM));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (radiobutton3);
	gtk_box_pack_start (GTK_BOX (hbox4), radiobutton3, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton3), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton3));
	if (entry_data_class!=-1)
	{
		type=RANGE;
		gtk_widget_set_sensitive(radiobutton2,FALSE);
		gtk_widget_set_sensitive(radiobutton3,FALSE);
	}

	switch (type)
	{
	case RANGE:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton1),TRUE) ;
		break;
	case WARN:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton2),TRUE) ;
		break;
	case ALARM:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton3),TRUE) ;
		break;
	default:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton1),TRUE) ;

	}

	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);
	
	btn_ok = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_APPLY));//gtk_button_new_from_stock ("gtk-apply");
	G_SAFE_FREE(pConvert);
	gtk_widget_show (btn_ok);
	gtk_box_pack_end (GTK_BOX (hbox1), btn_ok, FALSE, FALSE, 0);
	
	btn_cancel = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));//gtk_button_new_from_stock ("gtk-cancel");
	G_SAFE_FREE(pConvert);
	gtk_widget_show (btn_cancel);
	gtk_box_pack_end (GTK_BOX (hbox1), btn_cancel, FALSE, FALSE, 0);
	
	
	g_signal_connect (G_OBJECT (adjust1), "value_changed",
		G_CALLBACK (on_hscale1_value_changed), (gpointer) entry1);
	
	gtk_signal_connect (GTK_OBJECT (btn_cancel), "clicked",
		GTK_SIGNAL_FUNC(DestoryAdjust),  (gpointer) crt_n);
	
	gtk_signal_connect(GTK_OBJECT(btn_ok), "clicked",
		GTK_SIGNAL_FUNC(GetScaleVal),
			     (gpointer) entry1);
g_signal_connect ((gpointer) radiobutton1, "toggled",
		G_CALLBACK (on_radiobutton1_toggled),
                    (gpointer) entry1);
 	g_signal_connect ((gpointer) radiobutton2, "toggled",
 		G_CALLBACK (on_radiobutton2_toggled),
                     (gpointer) entry1);
	g_signal_connect ((gpointer) radiobutton3, "toggled",
		G_CALLBACK (on_radiobutton3_toggled),
                    (gpointer) entry1);
	g_object_set_data(G_OBJECT(entry1),"limtVal",&lmtVal) ;
	g_object_set_data(G_OBJECT(entry1),"pointer",(gpointer)pt) ;
	
	gtk_widget_show(windows);
	return  windows ;  
}

int InitializeValueEntryBox	(int crt_n,LIMIT_VAL  *lmtVal,POINTER *pt,int type)
{
	DMS_COMMON	dms_cmm;
	int pt_id ; 
	
	
	if(ReadEntryById ( pt, &dms_cmm )==-1)
	{	printf("no this entry \n");
	return FALSE;
	}
	if(cmd_buf[crt_n].type_id==C_UNIT)
	{
		lmtVal->up_lmt = 50;
		lmtVal->dn_lmt =0;
		lmtVal->cur_val = 0;
		return TRUE;
	}
	else
	{
switch(type)
	{
	case RANGE: 

		lmtVal->up_lmt = dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt = dms_cmm.var.anlg.eng_para.LO_RANGE;
		if(dms_cmm.point.data_type==CURVE_TYPE)
		{
			lmtVal->up_lmt = dms_cmm.var.curve.eng_para.HI_RANGE;
			lmtVal->dn_lmt = dms_cmm.var.curve.eng_para.LO_RANGE;                    
		}
		break;
	case WARN:
		lmtVal->up_lmt = dms_cmm.var.anlg.lmt_para.HI_WARN;
		lmtVal->dn_lmt = dms_cmm.var.anlg.lmt_para.LO_WARN;
		if(dms_cmm.point.data_type==CURVE_TYPE)
		{
			lmtVal->up_lmt = dms_cmm.var.curve.eng_para.HI_RANGE;
			lmtVal->dn_lmt = dms_cmm.var.curve.eng_para.LO_RANGE;                    
		}
		break;
	case ALARM:
		lmtVal->up_lmt = dms_cmm.var.anlg.lmt_para.HI_ALARM;
		lmtVal->dn_lmt = dms_cmm.var.anlg.lmt_para.LO_ALARM;
		if(dms_cmm.point.data_type==CURVE_TYPE)
		{
			lmtVal->up_lmt = dms_cmm.var.curve.eng_para.HI_RANGE;
			lmtVal->dn_lmt = dms_cmm.var.curve.eng_para.LO_RANGE;                    
		}
		break;
	default:
		lmtVal->up_lmt = dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt = dms_cmm.var.anlg.eng_para.LO_RANGE;
		if(dms_cmm.point.data_type==CURVE_TYPE)
		{
			lmtVal->up_lmt = dms_cmm.var.curve.eng_para.HI_RANGE;
			lmtVal->dn_lmt = dms_cmm.var.curve.eng_para.LO_RANGE;                    
		}

	}	
	}
	
	switch ( cmd_buf[crt_n].type_id )
	{
		
	case C_APOWER:	
	case C_RPOWER:	
	case C_OP_VALUE_SET:
	case C_ANA_FORCE:
	case C_TRANS:
	case C_GATE:
	case DPS_MAN_VALUE_SET:			
		lmtVal->cur_val = dms_cmm.var.anlg.fValue;
		printf("up=%f,dn=%f,cur_val=%.0f\n",lmtVal->up_lmt,lmtVal->dn_lmt,dms_cmm.var.anlg.fValue);
		break;
	case DPS_ANA_PARAM_SET:
		GetAnalogLimitValue ( cmd_buf[crt_n].status.opr, dms_cmm, lmtVal/*->cur_val */ );
		printf("opr=%d\n",cmd_buf[crt_n].status.opr);
		break;
    case DPS_MAN_CURVE_SET:
		pt_id = cmd_buf[crt_n].ctrl_addr;
		lmtVal->cur_val  =  dms_cmm.var.curve.data.value[pt_id] ;
		break ;
	}
	
	return TRUE;
}


void	GetAnalogLimitValue (int lmt_type,DMS_COMMON dms_cmm, LIMIT_VAL  *lmtVal/*float	*rt_value */)
{
	switch	( lmt_type )
	{
	case	ANA_PARAM_SET_D_BAND:
		lmtVal->cur_val   = dms_cmm.var.anlg.eng_para.D_BAND ;
		lmtVal->up_lmt	= 100.0;
		lmtVal->dn_lmt	= 0.0;
		printf("ANA_PARAM_SET_D_BAND: *rt_value=%f\n",lmtVal->cur_val);
		break;
		
	case	ANA_PARAM_SET_HI_WARN:
		lmtVal->cur_val   = dms_cmm.var.anlg.lmt_para.HI_WARN ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_HI_ALARM:
		lmtVal->cur_val	= dms_cmm.var.anlg.lmt_para.HI_ALARM ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_LO_WARN:
		lmtVal->cur_val	= dms_cmm.var.anlg.lmt_para.LO_WARN ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_LO_ALARM:
		lmtVal->cur_val	= dms_cmm.var.anlg.lmt_para.LO_ALARM ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_HI_RECOVER:
		lmtVal->cur_val   	= dms_cmm.var.anlg.lmt_para.HI_RECOVER ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_LO_RECOVER:
		lmtVal->cur_val	= dms_cmm.var.anlg.lmt_para.LO_RECOVER ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_TR_WARN:
		lmtVal->cur_val	= dms_cmm.var.anlg.lmt_para.TR_WARN ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_TR_RECOVER:
		lmtVal->cur_val	= dms_cmm.var.anlg.lmt_para.TR_RECOVER ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	case	ANA_PARAM_SET_ENG_CVT:
		lmtVal->cur_val= dms_cmm.var.anlg.eng_para.ENG_CVT ;
		lmtVal->up_lmt	= 1000.0;
		lmtVal->dn_lmt	= 0.0;
		break;
		
	case	ANA_PARAM_SET_HI_RANGE:
		lmtVal->cur_val= dms_cmm.var.anlg.eng_para.HI_RANGE ;
		lmtVal->up_lmt	= 32767.0;
		lmtVal->dn_lmt	= -32767.0;
		break;
		
	case	ANA_PARAM_SET_LO_RANGE:
		lmtVal->cur_val	= dms_cmm.var.anlg.eng_para.LO_RANGE ;
		lmtVal->up_lmt	= 32767.0;
		lmtVal->dn_lmt	= -32767.0;
		break;
		
	case	ANA_PARAM_SET_ZERO:
		lmtVal->cur_val= dms_cmm.var.anlg.eng_para.ZERO ;
		lmtVal->up_lmt	= 32767.0;
		lmtVal->dn_lmt	= -32767.0;
		break;
		
	case	ANA_PARAM_SET_COMPSATE:
		lmtVal->cur_val= dms_cmm.var.anlg.eng_para.COMPSATE ;
		lmtVal->up_lmt	= dms_cmm.var.anlg.eng_para.HI_RANGE;
		lmtVal->dn_lmt	= dms_cmm.var.anlg.eng_para.LO_RANGE;
		break;
		
	default:
		break;
	}
}



/******** CURVE INPUT BOX  *******/

void DestroyCurveInputBox(GtkWidget *w,int crt_n) 
{      
	gtk_widget_destroy(curve_input_box[crt_n]);
	curve_input_box[crt_n]=NULL;  
}

int GetHourMin(char	input_time[10],int *hour,int *min,int crt_n ) 
{
    char *token,time_str[3][3];
    
    int i=0;    
	
    if ((token = strtok(input_time, ":")) != NULL) 
    {
        strcpy(time_str[i], token);
        *hour = atoi(time_str[i]);
        i++;
        while ((token = strtok(NULL, ":")) != NULL) 
        {           
            strcpy(time_str[i], token);      
            *min = atoi(time_str[i]);
            i++;
        }
    }
	else
	{
		PopupMessageBox(main_w[crt_n],OIX_CSTR_TIME_FORMAT_ERROR);
		return FALSE;
	}
	if(*hour>24||*hour<0)
	{
		PopupMessageBox(main_w[crt_n],OIX_CSTR_HOUR_OVER_LMT);
		return FALSE;
	}
	if(*min>60||*min<0)
	{
		PopupMessageBox(main_w[crt_n],OIX_CSTR_MIN_OVER_LMT);
		return FALSE;
	}
	return TRUE;
}

BOOL CheckTimeFormat(char input_time[16],int crt_n)
{
	char hour[10],min[10];
	int  len,pos;
	char *str;
	if(strchr(input_time,':')==NULL)
	{
		PopupMessageBox(main_w[crt_n],OIX_CSTR_TIME_FORMAT_ERROR);
		return FALSE;
	}
	else
	{
		str = strchr(input_time,':');
		pos = input_time - str;
		strncpy(hour,input_time,pos);
		if(atoi(hour)>24||atoi(hour)<0)
		{
			PopupMessageBox(main_w[crt_n],OIX_CSTR_HOUR_OVER_LMT);
			return FALSE;
		}
		
		len=strlen(input_time)-pos-1;
		strncpy(min,input_time,len);
		if(atoi(min)>60||atoi(min)<0)
		{
			PopupMessageBox(main_w[crt_n],OIX_CSTR_MIN_OVER_LMT);
			return FALSE;
		}
		return TRUE;
	}
}

void GetCurveVal(GtkWidget *w,POINTER *pt)
{
    GtkWidget *start,*end,*data;
    char    start_time[16],end_time[16],curve_data[16];
    int start_hour,start_min,end_hour,end_min,start_seg_no,end_seg_no,seg_num;
    float val;  
    int crt_n;
    short total_seg_num,time_interval;
//	BOOL time_format;
    
    GetCrtNumberOfWgt( w, &crt_n );
    if( crt_n < 0 )
        return;    
    
    start = g_object_get_data(G_OBJECT(curve_input_box[crt_n]),"start_time");
    end = g_object_get_data(G_OBJECT(curve_input_box[crt_n]),"end_time");
    data = g_object_get_data(G_OBJECT(curve_input_box[crt_n]),"curve_data");
    if(gtk_entry_get_text(GTK_ENTRY(start)))    
        strcpy(start_time,gtk_entry_get_text(GTK_ENTRY(start)));

/*	time_format=CheckTimeFormat(start_time,crt_n);	
	if(time_format==FALSE)
		return;
	
    

	time_format=CheckTimeFormat(end_time,crt_n);
	if(time_format==FALSE)
		return;*/
	if(gtk_entry_get_text(GTK_ENTRY(end)))    
        strcpy(end_time,gtk_entry_get_text(GTK_ENTRY(end))); 

    if(gtk_entry_get_text(GTK_ENTRY(data)))    
        strcpy(curve_data,gtk_entry_get_text(GTK_ENTRY(data)));
    
    val = (float)(atof(curve_data));
    DestroyCurveInputBox(w,crt_n);
    
    GetHourMin(	start_time,&start_hour,&start_min ,crt_n) ;
    GetHourMin(	end_time,&end_hour,&end_min,crt_n ) ; 
    GetCurvePtSegNumById( pt, &total_seg_num );
    
    printf("###total_seg_num=%d,\n",total_seg_num );
    time_interval=60*24/total_seg_num;
    
    start_seg_no = start_hour*total_seg_num/24+start_min/time_interval -1;  //0-based
    end_seg_no = end_hour*total_seg_num/24+end_min/time_interval  -1;       //0-based   
    printf("start_seg_no=%d,end_seg_no=%d,end_min/time_interval=%d,time_interval=%d\n",
		start_seg_no,end_seg_no,end_min/time_interval,time_interval);
	
    for(seg_num = start_seg_no;seg_num<=end_seg_no;seg_num++)
    {   
        CmdBufInit ( crt_n );
        cmd_buf[crt_n].stn_id		= pt->stn_id;
        cmd_buf[crt_n].dev_id		= pt->dev_id;
        cmd_buf[crt_n].point_id		= pt->pt_id;
        cmd_buf[crt_n].data_type        = pt->data_type;
        cmd_buf[crt_n].type_id		= C_OP_VALUE_SET; 
        cmd_buf[crt_n].status.opr	= CRUVE_REAL_VALUE;
        cmd_buf[crt_n].data.iValue      =( gint) (val *1000);        
        cmd_buf[crt_n].ctrl_addr	= seg_num;        
        ProgDoAckCelBrdcst ( crt_n );
    }
    
    printf("start_hour=%d,start_min=%d,end_hour=%d,end_min=%d,curve_data=%f\n",
        start_hour,start_min,end_hour,end_min,val);
}
GtkWidget * CreateCurveInputBox(GtkWidget *parent, int crt_n ,POINTER pt) ; 
void PopupCurveInputBox(GtkWidget *parent, int crt_n ,POINTER pt)
{
	if (curve_input_box[crt_n])
		gtk_widget_destroy(curve_input_box[crt_n]);
	curve_input_box[crt_n] = CreateCurveInputBox( parent,   crt_n ,  pt)  ; 
	gtk_widget_show(curve_input_box[crt_n]) ; 

}



GtkWidget * CreateCurveInputBox(GtkWidget *parent, int crt_n ,POINTER pt)
{
	GtkWidget *table1,*window;
	GtkWidget *start_time_lab;
	GtkWidget *end_time_lab;
	GtkWidget *curve_data_lab;
	GtkWidget *start_time_entry;
	GtkWidget *end_time_entry;
	GtkWidget *curve_data_entry;
	GtkWidget *cancel_btn;
	GtkWidget *ok_btn;
	static POINTER curve_pt;
	char tmp_str[12];
	gchar *pConvert=0;
	
	curve_pt = pt;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), pConvert=_toUtf8EX(OIX_CSTR_DATA_INPUT));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(window),GTK_WINDOW(parent));
	gtk_widget_set_usize(GTK_WIDGET(window),250,170);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_widget_show(window);
    
	table1 = gtk_table_new (3, 2, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (window), table1);
	gtk_container_set_border_width (GTK_CONTAINER (table1), 20);
	gtk_table_set_row_spacings (GTK_TABLE (table1), 10);
	gtk_table_set_col_spacings (GTK_TABLE (table1), 5);
	
	start_time_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_START_TIME ));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (start_time_lab);
	gtk_table_attach (GTK_TABLE (table1), start_time_lab, 0, 1, 0, 1,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (start_time_lab), 0, 0.5);
	
	end_time_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_END_TIME));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (end_time_lab);
	gtk_table_attach (GTK_TABLE (table1), end_time_lab, 0, 1, 1, 2,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (end_time_lab), 0, 0.5);
	
	pConvert=_CS_(OIX_CSTR_DATA_INPUT);
	sprintf(tmp_str,"%s：",pConvert);
	G_SAFE_FREE(pConvert);

	curve_data_lab = gtk_label_new (pConvert=_toUtf8(tmp_str));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (curve_data_lab);
	gtk_table_attach (GTK_TABLE (table1), curve_data_lab, 0, 1, 2, 3,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (curve_data_lab), 0, 0.5);
	
	start_time_entry = gtk_entry_new ();
	gtk_widget_show (start_time_entry);
	gtk_table_attach (GTK_TABLE (table1), start_time_entry, 1, 2, 0, 1,
		(GtkAttachOptions) (GTK_EXPAND),
		(GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_max_length (GTK_ENTRY (start_time_entry), 5);
	gtk_entry_set_width_chars (GTK_ENTRY (start_time_entry), 10);
	
	
	end_time_entry = gtk_entry_new ();
	gtk_widget_show (end_time_entry);
	gtk_table_attach (GTK_TABLE (table1), end_time_entry, 1, 2, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND),
		(GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_max_length (GTK_ENTRY (end_time_entry), 5);
	gtk_entry_set_width_chars (GTK_ENTRY (end_time_entry), 10);
	
	
	curve_data_entry = gtk_entry_new ();
	gtk_widget_show (curve_data_entry);
	gtk_table_attach (GTK_TABLE (table1), curve_data_entry, 1, 2, 2, 3,
		(GtkAttachOptions) (GTK_EXPAND),
		(GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_width_chars (GTK_ENTRY (curve_data_entry), 10);
	
	
	cancel_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_CANCEL ));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (cancel_btn);
	gtk_table_attach (GTK_TABLE (table1), cancel_btn, 1, 2, 3, 4,
		(GtkAttachOptions) (0),
		(GtkAttachOptions) (0), 0, 0);
	
	ok_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (ok_btn);
	gtk_table_attach (GTK_TABLE (table1), ok_btn, 0, 1, 3, 4,
		(GtkAttachOptions) (0),
		(GtkAttachOptions) (0), 0, 0);
	
	gtk_signal_connect(GTK_OBJECT(window),
        "destroy",
        GTK_SIGNAL_FUNC(DestroyCurveInputBox),
        GINT_TO_POINTER(crt_n));
	gtk_signal_connect(GTK_OBJECT(cancel_btn),
        "pressed",
        GTK_SIGNAL_FUNC(DestroyCurveInputBox),
        GINT_TO_POINTER(crt_n));
	gtk_signal_connect(GTK_OBJECT(ok_btn),
        "pressed",
        GTK_SIGNAL_FUNC(GetCurveVal),
        &curve_pt);
	g_object_set_data(G_OBJECT(window),"start_time",start_time_entry);
	g_object_set_data(G_OBJECT(window),"curve_data",curve_data_entry);
	g_object_set_data(G_OBJECT(window),"end_time",end_time_entry);
	return window ; 
}

gint DestroyValueBoxNoScale (GtkWidget *widget , gpointer data )
{
    int crt_n = (gint) data ;
    if(value_box_no_scale[crt_n ])
		gtk_widget_destroy(GTK_WIDGET(value_box_no_scale[crt_n ]));
	
	value_box_no_scale[crt_n ]  = NULL ;
	
	return TRUE;
	
}
gint GetValSeting(GtkWidget *w,gpointer entry)
{	
	char buffer[16];
	float fvalue; 
	int crt_n=0;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ; 
	
	
	
	strcpy(buffer,gtk_entry_get_text (GTK_ENTRY (entry))); 
	
	
	sscanf(buffer,"%f",&fvalue);
	printf("***value=%f\n",fvalue);
	
	FormCEnterValueAdjItem (crt_n,  fvalue,NULL,FALSE);
	DestroyValueBoxNoScale ( NULL , GINT_TO_POINTER (crt_n) ) ;
	return TRUE;
}



GtkWidget *CreateValueBoxNoScale(GtkWidget *parent, int crt_n ,POINTER *pt)
{
	GtkWidget *windows,*table1;
	GtkWidget *labeltmp; 
	GtkWidget *value_entry;
	GtkWidget *cancel_btn;
	GtkWidget *ok_btn;
	DMS_COMMON	dms_cmm;
	char tmp_str[64];
	float fvalue ;
	gchar *pConvert=0;
	
	
	
	windows = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (windows), pConvert=_toUtf8(OIX_CSTR_DATA_INPUT));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
	gtk_widget_set_usize(GTK_WIDGET(windows),250,170);
	gtk_window_set_position(GTK_WINDOW(windows),GTK_WIN_POS_CENTER);
	gtk_widget_show(windows);
    
	table1 = gtk_table_new (2, 2, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (windows), table1);
	gtk_container_set_border_width (GTK_CONTAINER (table1), 20);
	gtk_table_set_row_spacings (GTK_TABLE (table1), 10);
	gtk_table_set_col_spacings (GTK_TABLE (table1), 5);
	
	labeltmp = gtk_label_new (pConvert=_toUtf8EX( 	OIX_CSTR_CHANGE_TO));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (labeltmp);
	gtk_table_attach (GTK_TABLE (table1), labeltmp, 0, 1, 0, 1,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (labeltmp), 0, 0.5);
	
	
	
	
	value_entry = gtk_entry_new ();
	gtk_widget_show (value_entry);
	gtk_table_attach (GTK_TABLE (table1), value_entry, 1, 2, 0, 1,
		(GtkAttachOptions) (GTK_EXPAND),
		(GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_max_length (GTK_ENTRY (value_entry), 5);
	gtk_entry_set_width_chars (GTK_ENTRY (value_entry), 10);
	
	
	
	
	cancel_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (cancel_btn);
	gtk_table_attach (GTK_TABLE (table1), cancel_btn, 1, 2,1, 2,
		(GtkAttachOptions) (0),
		(GtkAttachOptions) (0), 0, 0);
	
	ok_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (ok_btn);
	gtk_table_attach (GTK_TABLE (table1), ok_btn, 0, 1, 1, 2,
		(GtkAttachOptions) (0),
		(GtkAttachOptions) (0), 0, 0);
	
	gtk_signal_connect(GTK_OBJECT(windows),
        "destroy",
        GTK_SIGNAL_FUNC(DestroyValueBoxNoScale),
        GINT_TO_POINTER(crt_n));
	gtk_signal_connect(GTK_OBJECT(cancel_btn),
        "pressed",
        GTK_SIGNAL_FUNC(DestroyValueBoxNoScale),
        GINT_TO_POINTER(crt_n));
	gtk_signal_connect(GTK_OBJECT(ok_btn),
        "pressed",
        GTK_SIGNAL_FUNC(GetValSeting),
        value_entry);
	g_object_set_data(G_OBJECT(windows),"valueBox",value_entry);
	
	
	if ( ReadEntryById ( pt, &dms_cmm ) == -1)
	{
		printf ( "OIX: This point is not Found in the database file !!!\n" );
		PopupMessageBox(windows , OIX_STR_DB_NO_POINT ) ;
		return (GtkWidget *)NULL;
	}
	fvalue  =(float)(dms_cmm.var.imp.entry.rt_value);
	sprintf (tmp_str,"%.f\0",fvalue);
	gtk_entry_set_text(GTK_ENTRY(value_entry), tmp_str);
	return windows ;
	
}

void  PopupValueEntryBoxNoScale (GtkWidget *parent ,int crt_n ,POINTER *pt)
{
    if (value_entry_box[crt_n ]== NULL) 
    {
        value_box_no_scale[crt_n ] =  CreateValueBoxNoScale(parent, crt_n, pt); 
        gtk_widget_show( value_box_no_scale[crt_n ]);
    }else
	{
        gtk_window_present(GTK_WINDOW(value_box_no_scale[crt_n ]));
    }
    
}

//add by hcl man_set_time MAN_SET_TIME
gint DestroyManSetTimedlg(gpointer  user_data)
{
	int crt_n = (gint) user_data ;
    if(man_set_time_dlg[crt_n ])
		gtk_widget_destroy(GTK_WIDGET(man_set_time_dlg[crt_n ]));
	
	man_set_time_dlg[crt_n ]  = NULL ;
	
	return TRUE;
}

void on_manSetTime_cancelbtn_clicked0 (GtkButton  *button,  GBRCD *rcd)
{
	GtkWidget *calendar;
//	time_t  tvalue ;
	struct tm	t_tm;
	DMS_COMMON      dms_cmm;
	int crt_n;
	char db_name[100];
	
	GetCrtNumberOfWgt (GTK_WIDGET(button), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	
	strcpy(db_name,rcd->lnk.dbname);
	
	if ( ReadEntryByNameStr ( db_name,  &dms_cmm )== -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,db_name );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
	}
	
	t_tm= *localtime( (time_t*)&dms_cmm.var.time.value_sec );
	
	calendar=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]), "calendar");     
	gtk_calendar_select_month  (GTK_CALENDAR(calendar), (guint)t_tm.tm_mon ,  (guint)t_tm.tm_year+1900);
	gtk_calendar_select_day   (GTK_CALENDAR(calendar), (guint)t_tm.tm_mday);
	
	//	DestroyManSetTimedlg(crt_n);
}

void on_manSetTime_cancelbtn_clicked1 (GtkButton  *button, POINTER* point)
{
	GtkWidget *calendar;
//	time_t  tvalue ;
	struct tm	t_tm;
	DMS_COMMON      dms_cmm;
	int crt_n;
	char db_name[100];
	
	GetCrtNumberOfWgt (GTK_WIDGET(button), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return; 
	
	ReadEntryById( point , &dms_cmm);
	sprintf(db_name,"%d.%d.%d.%d",dms_cmm.point.stn_id,dms_cmm.point.dev_id,dms_cmm.point.data_type,dms_cmm.point.pt_id);	
	GetPtNameById(&dms_cmm.point,db_name,NULL);
	
	t_tm= *localtime( (time_t*)&dms_cmm.var.time.value_sec );
	
	calendar=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]), "calendar");     
	gtk_calendar_select_month  (GTK_CALENDAR(calendar), (guint)t_tm.tm_mon ,  (guint)t_tm.tm_year+1900);
	gtk_calendar_select_day   (GTK_CALENDAR(calendar), (guint)t_tm.tm_mday);
	
	//	DestroyManSetTimedlg(crt_n);
}


void on_dlg_input_time_destroy(GtkObject       *object, gpointer  user_data)
{
	DestroyManSetTimedlg(user_data);
}


void on_manSetTime_okbtn_clicked0  (GtkButton *button,  GBRCD *rcd)
{
	int yy,mm,dd,hh,mi,ss;
	GtkWidget *calendar;
	GtkWidget *spinbtn_hour;
	GtkWidget *spinbtn_min;
	GtkWidget *spinbtn_sec;
	int crt_n=0;
	char db_name[100];
	
	time_t  tvalue ;
	struct tm twhen ;
	
	DMS_COMMON      dms_cmm;
//	char  LongNamerStr1[128];
	
	GetCrtNumberOfWgt (GTK_WIDGET(button), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	
	calendar=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]),"calendar");     
	gtk_calendar_get_date(GTK_CALENDAR(calendar), (guint*)&yy, (guint*)&mm, (guint*)&dd); 
	
	spinbtn_hour=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]),"spinbtn_hour"); 
	hh = gtk_spin_button_get_value_as_int  (GTK_SPIN_BUTTON(spinbtn_hour)  );
	
	spinbtn_min=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]), "spinbtn_min"); 
	mi = gtk_spin_button_get_value_as_int  (GTK_SPIN_BUTTON(spinbtn_min)  );
	
	spinbtn_sec=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]),"spinbtn_sec"); 
	ss = gtk_spin_button_get_value_as_int  (GTK_SPIN_BUTTON(spinbtn_sec)  );
	
	printf("%d-%d-%d %d:%d:%d\n",yy,mm+1,dd,hh,mi,ss);
	
	
	twhen.tm_year = yy-1900 ; 
	twhen.tm_mon  = mm  ;
	twhen.tm_mday =dd      ;
	twhen.tm_hour  = hh   ;
	twhen.tm_min  = mi   ;
	twhen.tm_sec  = ss ; 
	tvalue = mktime(&twhen);
	
	strcpy(db_name,rcd->lnk.dbname);
	
	if ( ReadEntryByNameStr ( db_name,  &dms_cmm )== -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,db_name );
		PopupMessageBox ( main_w[crt_n], info);
		DestroyManSetTimedlg(GINT_TO_POINTER(crt_n));
		return;              
	}
	
	if (dms_cmm.var.time.value_sec!=tvalue)//时间修改才广播
	{
	
		CmdBufInit ( crt_n );
		cmd_buf[crt_n].stn_id	= dms_cmm.point.stn_id;
		cmd_buf[crt_n].dev_id	= dms_cmm.point.dev_id;
		cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;  
		cmd_buf[crt_n].data_type= dms_cmm.point.data_type; 
		cmd_buf[crt_n].type_id	= DPS_MAN_TIME_SET;	
		cmd_buf[crt_n].data.iValue = tvalue;   //1970年1月1日以来的累计秒数 ; 
		cmd_buf[crt_n].ctrl_addr	=0;		
		
		ProgDoAckCelBrdcst ( crt_n );
	}
	
	DestroyManSetTimedlg(GINT_TO_POINTER(crt_n));
	
}
void on_manSetTime_okbtn_clicked1  (GtkButton *button,  POINTER* point)
{
	int yy,mm,dd,hh,mi,ss;
	GtkWidget *calendar;
	GtkWidget *spinbtn_hour;
	GtkWidget *spinbtn_min;
	GtkWidget *spinbtn_sec;
	int crt_n=0;
//	char db_name[100];
	
	time_t  tvalue ;
	struct tm twhen ;
	
	DMS_COMMON      dms_cmm;
//	char  LongNamerStr1[128];
	
	GetCrtNumberOfWgt (GTK_WIDGET(button), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return; 
	
	calendar=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]), "calendar");     
	gtk_calendar_get_date(GTK_CALENDAR(calendar), (guint*)&yy, (guint*)&mm, (guint*)&dd); 
	
	spinbtn_hour=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]), "spinbtn_hour"); 
	hh = gtk_spin_button_get_value_as_int  (GTK_SPIN_BUTTON(spinbtn_hour)  );
	
	spinbtn_min=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]), "spinbtn_min"); 
	mi = gtk_spin_button_get_value_as_int  (GTK_SPIN_BUTTON(spinbtn_min)  );
	
	spinbtn_sec=gtk_object_get_data(GTK_OBJECT(man_set_time_dlg[crt_n]), "spinbtn_sec"); 
	ss = gtk_spin_button_get_value_as_int  (GTK_SPIN_BUTTON(spinbtn_sec)  );
	
	printf("%d-%d-%d %d:%d:%d\n",yy,mm+1,dd,hh,mi,ss);
	
	twhen.tm_year = yy-1900 ; 
	twhen.tm_mon  = mm  ;
	twhen.tm_mday =dd      ;
	twhen.tm_hour  = hh   ;
	twhen.tm_min  = mi   ;
	twhen.tm_sec  = ss ; 
	tvalue = mktime(&twhen);
	
	ReadEntryById( point , &dms_cmm);
	
	if (dms_cmm.var.time.value_sec!=tvalue)//时间修改才广播
	{
		
		CmdBufInit ( crt_n );
		cmd_buf[crt_n].stn_id	= dms_cmm.point.stn_id;
		cmd_buf[crt_n].dev_id	= dms_cmm.point.dev_id;
		cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;  
		cmd_buf[crt_n].data_type= dms_cmm.point.data_type; 
		cmd_buf[crt_n].type_id	= DPS_MAN_TIME_SET;	
		cmd_buf[crt_n].data.iValue = tvalue;   //1970年1月1日以来的累计秒数 ; 
		cmd_buf[crt_n].ctrl_addr	=0;		
		
		ProgDoAckCelBrdcst ( crt_n );
	}
	DestroyManSetTimedlg(GINT_TO_POINTER(crt_n));
	
}
//inputParaflag=0 input GBRCD,
//inputParaflag=1 input POINTER,
GtkWidget *CreateManSetTimeInputDlg(GtkWidget *parent, int crt_n ,int inputParaflag,GBRCD *rcd,POINTER* point)
{
	GtkWidget *dlg_input_time;
	GtkWidget *dialog_vbox1;
	GtkWidget *vbox1;
	GtkWidget *calendar1;
	GtkWidget *hbox1;
	GtkWidget *label3;
	GtkObject *spinbtn_hour_adj;
	GtkWidget *spinbtn_hour;
	GtkWidget *label4;
	GtkObject *spinbtn_min_adj;
	GtkWidget *spinbtn_min;
	GtkWidget *label5;
	GtkObject *spinbtn_sec_adj;
	GtkWidget *spinbtn_sec;
	GtkWidget *dialog_action_area1;
	GtkWidget *cancelbutton1;
	GtkWidget *okbutton1;
	GtkTooltips *tooltips;
	DMS_COMMON      dms_cmm;
	time_t		systime; 
	struct tm	t_tm;
//	char tmp_str[100];
	char dbname[100];
	//	DMS_COMMON      db_cmm;
	
	if (inputParaflag==0)
	{
		strcpy(dbname,rcd->lnk.dbname);
		
		if ( ReadEntryByNameStr ( dbname,  &dms_cmm )== -1)
		{
			char		info[50];
			strcpy(info,OIX_STR_DB_NO_POINT);
			strcat(info,dbname );
			PopupMessageBox ( main_w[crt_n], info);
			return NULL;              
		}
	}else
	{
		ReadEntryById( point , &dms_cmm);
		sprintf(dbname,"%d.%d.%d.%d",dms_cmm.point.stn_id,dms_cmm.point.dev_id,dms_cmm.point.data_type,dms_cmm.point.pt_id);	
		GetPtNameById(&dms_cmm.point,dbname,NULL);
		
	}
	
	
	if (dms_cmm.var.time.value_sec==0) //时间点为0值，显示系统时间
	{
		systime=time(NULL);
		t_tm= *localtime( &systime );
	}
	else{	//时间点不为0值，显示数据库中点时间
		
		t_tm= *localtime( (time_t*)&dms_cmm.var.time.value_sec );
		
	}
	
	
	tooltips = gtk_tooltips_new ();
	
	dlg_input_time = gtk_dialog_new ();
	gtk_window_set_transient_for(GTK_WINDOW(dlg_input_time),GTK_WINDOW(parent));
	gtk_window_set_title (GTK_WINDOW (dlg_input_time), _("MAN_SET_TIME"));
	gtk_window_set_type_hint (GTK_WINDOW (dlg_input_time), GDK_WINDOW_TYPE_HINT_DIALOG);
	
	dialog_vbox1 = GTK_DIALOG (dlg_input_time)->vbox;
	gtk_widget_show (dialog_vbox1);
	
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);
	
	calendar1 = gtk_calendar_new ();
	g_object_set_data(G_OBJECT(dlg_input_time),"calendar",calendar1) ;
	gtk_container_add (GTK_CONTAINER (dlg_input_time), calendar1);
	gtk_widget_show (calendar1);
	gtk_box_pack_start (GTK_BOX (vbox1), calendar1, TRUE, TRUE, 0);
	gtk_calendar_display_options (GTK_CALENDAR (calendar1),
		GTK_CALENDAR_SHOW_HEADING
		| GTK_CALENDAR_SHOW_DAY_NAMES);
	
	gtk_calendar_select_month  (GTK_CALENDAR(calendar1) ,t_tm.tm_mon ,  t_tm.tm_year+1900);
	gtk_calendar_select_day   (GTK_CALENDAR(calendar1), t_tm.tm_mday);
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);
	
	label3 = gtk_label_new (_("\346\227\266\351\227\264\357\274\232"));
	gtk_widget_show (label3);
	gtk_box_pack_start (GTK_BOX (hbox1), label3, FALSE, FALSE, 0);
	
	spinbtn_hour_adj = gtk_adjustment_new (t_tm.tm_hour, 0, 23, 1, 10, 10);
	spinbtn_hour = gtk_spin_button_new (GTK_ADJUSTMENT (spinbtn_hour_adj), 1, 0);
	g_object_set_data(G_OBJECT(dlg_input_time),"spinbtn_hour",spinbtn_hour) ;
	gtk_widget_show (spinbtn_hour);
	gtk_box_pack_start (GTK_BOX (hbox1), spinbtn_hour, TRUE, TRUE, 0);
	gtk_tooltips_set_tip (tooltips, spinbtn_hour, _("\346\227\266"), NULL);
	gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbtn_hour), TRUE);
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinbtn_hour), TRUE);
	
	label4 = gtk_label_new (_("\357\274\232"));
	gtk_widget_show (label4);
	gtk_box_pack_start (GTK_BOX (hbox1), label4, FALSE, FALSE, 0);
	
	spinbtn_min_adj = gtk_adjustment_new (t_tm.tm_min, 0, 59, 1, 1, 1);
	spinbtn_min = gtk_spin_button_new (GTK_ADJUSTMENT (spinbtn_min_adj), 1, 0);
	g_object_set_data(G_OBJECT(dlg_input_time),"spinbtn_min",spinbtn_min) ;
	gtk_widget_show (spinbtn_min);
	gtk_box_pack_start (GTK_BOX (hbox1), spinbtn_min, TRUE, TRUE, 0);
	gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbtn_min), TRUE);
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinbtn_min), TRUE);
	
	label5 = gtk_label_new (_("\357\274\232"));
	gtk_widget_show (label5);
	gtk_box_pack_start (GTK_BOX (hbox1), label5, FALSE, FALSE, 0);
	
	spinbtn_sec_adj = gtk_adjustment_new (t_tm.tm_sec, 0, 59, 1, 1, 1);
	spinbtn_sec = gtk_spin_button_new (GTK_ADJUSTMENT (spinbtn_sec_adj), 1, 0);
	g_object_set_data(G_OBJECT(dlg_input_time),"spinbtn_sec",spinbtn_sec) ;
	gtk_widget_show (spinbtn_sec);
	gtk_box_pack_start (GTK_BOX (hbox1), spinbtn_sec, TRUE, TRUE, 0);
	gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbtn_sec), TRUE);
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinbtn_sec), TRUE);
	
	
	dialog_action_area1 = GTK_DIALOG (dlg_input_time)->action_area;
	gtk_widget_show (dialog_action_area1);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);
	
	cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (dlg_input_time), cancelbutton1, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);
	
	okbutton1 = gtk_button_new_from_stock ("gtk-ok");
	gtk_container_add (GTK_CONTAINER (dlg_input_time), okbutton1);
	gtk_widget_show (okbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (dlg_input_time), okbutton1, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
	
	g_signal_connect ((gpointer) dlg_input_time, "destroy",
		G_CALLBACK (on_dlg_input_time_destroy),
		GINT_TO_POINTER(crt_n));
	
	
	if (inputParaflag==0)
	{	
		g_signal_connect ((gpointer) cancelbutton1, "clicked",
			G_CALLBACK (on_manSetTime_cancelbtn_clicked0),
			rcd);
	}
	else
	{
		g_signal_connect ((gpointer) cancelbutton1, "clicked",
			G_CALLBACK (on_manSetTime_cancelbtn_clicked1),
			point);
		
	}
	
	if (inputParaflag==0)
	{	
		g_signal_connect ((gpointer) okbutton1, "clicked",
			G_CALLBACK (on_manSetTime_okbtn_clicked0),
			rcd);
	}
	else
	{
		g_signal_connect ((gpointer) okbutton1, "clicked",
			G_CALLBACK (on_manSetTime_okbtn_clicked1),
			point);
		
	}
	return dlg_input_time;
	
	//gtk_widget_show_all(dlg_input_time)
	
}
void PopManSetTimeInputDlg(GtkWidget *parent ,int crt_n ,GBRCD  *rcd)
{
	
	
	if (man_set_time_dlg[crt_n ]== NULL) 
    {
        man_set_time_dlg[crt_n]=CreateManSetTimeInputDlg(parent, crt_n,0,rcd,NULL);
        gtk_widget_show( man_set_time_dlg[crt_n ]);
    }else
	{
        gtk_window_present(GTK_WINDOW(man_set_time_dlg[crt_n ]));
    }
}

