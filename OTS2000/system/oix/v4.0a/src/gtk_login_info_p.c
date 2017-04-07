/************************************************************
2007.04.13 : HAVE SYNC by chichi ;
*************************************************************/


#include	<gtk/gtk.h>
#include	"../inc/oix.h"
//#include	"../../../sms/v4.0a/inc/sms_cstr.h"
#include	"../../../sms/v4.0a/inc/user_privilege_def.h"
#include	"../inc/gtk_widgets.h"

STN_LCU_POS lcuPos[MAX_CRT_NUMBER][128];


GtkWidget *LogInDialog[MAX_CRT_NUMBER]={NULL,NULL,NULL};
GtkWidget *host_privilege_wgts[MAX_CRT_NUMBER][MAX_CTRL_LCU_NUM];
int		all_lcu_num[MAX_CRT_NUMBER];
LCU_ID	lcu_list[MAX_CRT_NUMBER][MAX_CTRL_LCU_NUM];
int		priv_lcu_select_set[MAX_CRT_NUMBER];	   



void	DspCurUsrInfo( int crt_n );
void	DspCurHostInfo( int crt_n );
static GtkWidget*  CreateLoginInfoBox (GtkWidget *parent, int crt_n );
extern void DestroyDlg(GtkWidget *w,gpointer arg);
int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] );
void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern int getCrtNumFromPointer(int data);
extern int getDataFromPointer(int data);
extern int getMenuDataPointer(int crtn,int data);


void UpdateCurHostPrivilege(int crt_n)
{
	int			j, cur_host_db_pos, modified_flag;
	HOST_DEF	host_tmp;

	for(cur_host_db_pos=0; cur_host_db_pos<privilege_db.host_num; cur_host_db_pos++)
	{
		if(!strcmp(privilege_db.host_list[cur_host_db_pos].hostname, local_hostname))
			break;
	}
	if(cur_host_db_pos>=privilege_db.host_num)
	{
		cur_host_db_pos= privilege_db.host_num;
		memset(&privilege_db.host_list[cur_host_db_pos], 0, sizeof(HOST_DEF));
		strcpy(privilege_db.host_list[cur_host_db_pos].hostname, local_hostname);
		privilege_db.host_num++;
	}

	memset(&host_tmp, 0, sizeof(host_tmp));
	host_tmp.lcu_num= 0;
	strcpy(host_tmp.hostname, local_hostname);
	for(j=0; j<all_lcu_num[crt_n]; j++)	
	{	
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(host_privilege_wgts[crt_n][j])))
			host_tmp.lcu[host_tmp.lcu_num++]= lcu_list[crt_n][j];
	}

	modified_flag= 0;
	if(host_tmp.lcu_num!=privilege_db.host_list[cur_host_db_pos].lcu_num)
		modified_flag= 1;
	else
	{
		for(j=0; j<host_tmp.lcu_num; j++)	
		{	
			if(privilege_db.host_list[cur_host_db_pos].lcu[j].stn_id!=host_tmp.lcu[j].stn_id
				|| privilege_db.host_list[cur_host_db_pos].lcu[j].dev_id!=host_tmp.lcu[j].dev_id)
			{
				modified_flag= 1;
				break;
			}
		}
	}

	if(modified_flag)
	{
        if(PopupMessageBoxRet(LogInDialog[crt_n],MSG_HOST_LCU_MODIFIED)== GTK_RESPONSE_OK ) 
			privilege_db.host_list[cur_host_db_pos]= host_tmp;
	}
	return;
}


int DestroyLoginDlg(GtkWidget *w,gpointer data)
{
    int  crt_n= (gint)data;

	if(priv_lcu_select_set[crt_n])
		UpdateCurHostPrivilege(crt_n);

    gtk_widget_destroy(LogInDialog[crt_n]);
    LogInDialog[crt_n]= NULL;    
	return TRUE;
}


int ExitLoginDlg(GtkWidget *w,gpointer data)
{
    int  crt_n= (gint)data;

    gtk_widget_destroy(LogInDialog[crt_n]);
	return TRUE;
}


void PopupLoginInfoBox (GtkWidget *parent, int crt_n )
{   
	if ( LogInDialog[crt_n]   == (GtkWidget *)NULL)
	{
	/*	if ( !cur_usr_name[0] )
		{
			PopupMessageBox ( main_w[crt_n], MSG_NO_USR_LOGIN );
			return;
		}  */
		 LogInDialog[crt_n]  = CreateLoginInfoBox ( parent ,crt_n);
		gtk_window_set_position(GTK_WINDOW(LogInDialog[crt_n]),GTK_WIN_POS_CENTER);
		gtk_widget_show( LogInDialog[crt_n] );
	}
	else
		gtk_window_present ( GTK_WINDOW(LogInDialog[crt_n]) );
}

gint dispLCUDev(GtkWidget *widget,   gpointer data)
{
	int   id    ;  
	int i ; 
    int crt_n  ;  
	crt_n  =   getCrtNumFromPointer((int)  data) ; 
    id  =      getDataFromPointer ( (int) data)  ; 
	
 
	printf  ("asdfasdfasdf data =  %d \n",data);
	for  (i = 0  ; i<all_lcu_num[crt_n] ; i++)  
	{
		
		if  (i>=lcuPos[crt_n][id].start_id && i<lcuPos[crt_n][id].end_id)
			gtk_widget_show(host_privilege_wgts[crt_n][i]);
		else
			gtk_widget_hide(host_privilege_wgts[crt_n][i]);
	}
	return  TRUE ; 
}


static GtkWidget*  CreateLoginInfoBox (GtkWidget *parent, int crt_n )
{
	int		i, dev_id, stn_num, dev_num,iData;
	UCHAR stn_id;

	GtkWidget *dialog,*optionmenu,*items,*menu;
	GtkWidget *dialog_vbox1;
	GtkWidget *table1;
	GtkWidget *cur_host_name_lab;
	GtkWidget *frame2;
	GtkWidget *alignment2;
	GtkWidget *host_privilege_frame_lab;
	GtkWidget *usr_frame;
	GtkWidget *alignment1;
	GtkWidget *usr_privilege_frame_lab;
	GtkWidget *dialog_action_area1;
	GtkWidget *okbutton1,*vbox1;
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	stn_name[128] ; 
	static	gchar	privilgstr[MAX_PRIV_TYPE_NUM][64];

	gchar *pConvert=0;
    
	strcpy(privilgstr[PRIV_ALM_ACK], _CS_(OIX_CSTR_ALM_ACK));
	strcpy(privilgstr[PRIV_PRINT], _CS_(OIX_CSTR_PRINT));
	strcpy(privilgstr[PRIV_LCU_CTRL], _CS_(OIX_CSTR_LCU_CTRL));
	strcpy(privilgstr[PRIV_POWER_ADJ], _CS_(OIX_CSTR_POWER_ADJ));
	strcpy(privilgstr[PRIV_SYS_RUN_SET], _CS_(OIX_CSTR_SYS_RUN_SET));
	strcpy(privilgstr[PRIV_CTRL_BLOCK_SET], _CS_(OIX_CSTR_CTRL_BLOCK_SET));
	strcpy(privilgstr[PRIV_IN_FORCE_SET], _CS_(OIX_CSTR_IN_FORCE_SET));
	strcpy(privilgstr[PRIV_OUT_FORCE_SET], _CS_(OIX_CSTR_OUT_FORCE_SET));
	strcpy(privilgstr[PRIV_SYS_CONF_MGR], _CS_(OIX_CSTR_SYS_CONF_MGR));
	strcpy(privilgstr[PRIV_TAG_OPR], _CS_(OIX_CSTR_TAG_OPR));
	strcpy(privilgstr[PRIV_SCAN_ALM_SET], _CS_(OIX_CSTR_SCAN_ALM_SET));
	strcpy(privilgstr[PRIV_IO_ATTR_SET], _CS_(OIX_CSTR_IO_ATTR_SET));
	strcpy(privilgstr[PRIV_SGZY_SET], _CS_(OIX_CSTR_SGZY_SET));
	strcpy(privilgstr[PRIV_RTVALUE_MAN_SET], _CS_(OIX_CSTR_RTVALUE_MAN_SET));
	strcpy(privilgstr[PRIV_IMP_MAN_SET], _CS_(OIX_CSTR_IMP_MAN_SET));
	strcpy(privilgstr[PRIV_LIMIT_PARA_SET], _CS_(OIX_CSTR_LIMIT_PARA_SET));
	strcpy(privilgstr[PRIV_SYS_MAINT], _CS_(OIX_CSTR_SYS_MAINT));
 	strcpy(privilgstr[PRIV_LCU_SELECT_SET], _CS_(OIX_CSTR_LCU_SELECT_SET));
 

	dialog = gtk_dialog_new ();
	gtk_signal_connect(GTK_OBJECT (dialog), "destroy",
					   GTK_SIGNAL_FUNC(DestroyLoginDlg), (gpointer)crt_n);
	gtk_window_set_title (GTK_WINDOW (dialog), pConvert=_toUtf8EX(OIX_CSTR_LOGIN_INFO));
	G_SAFE_FREE(pConvert);
	gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
	gtk_window_set_type_hint (GTK_WINDOW (dialog), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent)); 

	gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog),TRUE);
	dialog_vbox1 = GTK_DIALOG (dialog)->vbox;
	gtk_widget_show (dialog_vbox1);

	table1 = gtk_table_new (2, 2, FALSE);
	gtk_widget_show (table1);
	gtk_box_pack_start (GTK_BOX (dialog_vbox1), table1, TRUE, TRUE, 0);

	wgts[crt_n][WgtN_SMS_username] = gtk_label_new (pConvert=_toUtf8(cur_usr_name));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (wgts[crt_n][WgtN_SMS_username]);
	gtk_table_attach (GTK_TABLE (table1), wgts[crt_n][WgtN_SMS_username], 0, 1, 0, 1,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (wgts[crt_n][WgtN_SMS_username]), GTK_JUSTIFY_CENTER);
	gtk_misc_set_alignment (GTK_MISC (wgts[crt_n][WgtN_SMS_username]), 0, 0.5);

	cur_host_name_lab = gtk_label_new (pConvert=_toUtf8(local_hostname));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (cur_host_name_lab);
	gtk_table_attach (GTK_TABLE (table1), cur_host_name_lab, 1, 2, 0, 1,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (cur_host_name_lab), GTK_JUSTIFY_CENTER);
	gtk_misc_set_alignment (GTK_MISC (cur_host_name_lab), 0, 0.5);

	frame2 = gtk_frame_new (NULL);
	gtk_widget_show (frame2);
	gtk_table_attach (GTK_TABLE (table1), frame2, 1, 2, 1, 2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_container_set_border_width (GTK_CONTAINER (frame2), 5);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_IN);

	alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment2);
	gtk_container_add (GTK_CONTAINER (frame2), alignment2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (alignment2), vbox1);  
  

	if(CheckUserPrivilege(cur_usr_name, PRIV_LCU_SELECT_SET))
		priv_lcu_select_set[crt_n]= 1;
	else
		priv_lcu_select_set[crt_n]= 0;

	//////////////////////////////add station button !!!///////////////////////////////
	
	
	GetStationNum(&stn_num);	
	optionmenu = gtk_option_menu_new();
   	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (vbox1), optionmenu, FALSE, FALSE, 0);
	
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	
	for(i=1;i<=stn_num;i++) 
	{
		if(-1==GetStnIdByIndex(i, &stn_id))
			continue;
		
		if( GetStnCommentById(stn_id,stn_name)!=-1 )
		{					
			items = gtk_menu_item_new_with_label(pConvert=_toUtf8(stn_name));
			G_SAFE_FREE(pConvert);
			gtk_widget_show( items);
			gtk_container_add(GTK_CONTAINER(menu),items);
			iData=getMenuDataPointer(crt_n,stn_id);
			gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (dispLCUDev), GINT_TO_POINTER(iData));				
		}
	}
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);

//////////////////////////////////////////////////////////////////


	all_lcu_num[crt_n]= 0;
// 	if(-1==GetStationNum(&stn_num))
// 		return NULL;

	for(i=1; i<=stn_num; i++)
	{
		if(-1==GetStnIdByIndex(i, &stn_id))
			continue;
		
		if(-1==GetGroupNumById((UCHAR)stn_id, &dev_num))           
			continue;    
		lcuPos[crt_n][i].start_id  = 	 all_lcu_num[crt_n]  ; 
		for(dev_id=1; dev_id<=dev_num; dev_id++)
		{
			if(-1==GetGroupCommentById((UCHAR)stn_id, (UCHAR)dev_id, dev_comment))
				continue;

			host_privilege_wgts[crt_n][all_lcu_num[crt_n]]=gtk_check_button_new_with_mnemonic (pConvert=_toUtf8(dev_comment));
			G_SAFE_FREE(pConvert);
			gtk_widget_set_sensitive( host_privilege_wgts[crt_n][all_lcu_num[crt_n]],priv_lcu_select_set[crt_n]); 
			//gtk_widget_show (host_privilege_wgts[crt_n][all_lcu_num[crt_n]]);

			 if (i ==1) //only one station show
				gtk_widget_show(host_privilege_wgts[crt_n][all_lcu_num[crt_n]]);
			else
				gtk_widget_hide(host_privilege_wgts[crt_n][all_lcu_num[crt_n]]);

			gtk_box_pack_start (GTK_BOX (vbox1), host_privilege_wgts[crt_n][all_lcu_num[crt_n]], FALSE, FALSE, 0);  
			lcu_list[crt_n][all_lcu_num[crt_n]].stn_id= stn_id;
			lcu_list[crt_n][all_lcu_num[crt_n]].dev_id= dev_id;
			all_lcu_num[crt_n]++;
		}
		lcuPos[crt_n][stn_id].end_id  = 	 all_lcu_num[crt_n]  ; 
	}
  
	host_privilege_frame_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_PRIVILEGE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (host_privilege_frame_lab);
	gtk_frame_set_label_widget (GTK_FRAME (frame2), host_privilege_frame_lab);
	gtk_label_set_use_markup (GTK_LABEL (host_privilege_frame_lab), TRUE);

	usr_frame = gtk_frame_new (NULL);
	gtk_widget_show (usr_frame);
	gtk_table_attach (GTK_TABLE (table1), usr_frame, 0, 1, 1, 2,
					  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
					  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
	gtk_container_set_border_width (GTK_CONTAINER (usr_frame), 5);
	gtk_frame_set_shadow_type (GTK_FRAME (usr_frame), GTK_SHADOW_IN);


	alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment1);
	gtk_container_add (GTK_CONTAINER (usr_frame), alignment1);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (alignment1), vbox1);  

	for(i=0;i<MAX_PRIV_TYPE_NUM;i++)
	{      
		wgts[crt_n][WgtN_SMS_dsp_monit +i] = gtk_check_button_new_with_label(pConvert=_toUtf8(privilgstr[i]));
		G_SAFE_FREE(pConvert);
		gtk_widget_set_sensitive( wgts[crt_n][WgtN_SMS_dsp_monit +i],0);   
		gtk_widget_show (wgts[crt_n][WgtN_SMS_dsp_monit +i]);
		gtk_box_pack_start (GTK_BOX (vbox1), wgts[crt_n][WgtN_SMS_dsp_monit +i], FALSE, FALSE, 0);  
	}

	usr_privilege_frame_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_PRIVILEGE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (usr_privilege_frame_lab);
	gtk_frame_set_label_widget (GTK_FRAME (usr_frame), usr_privilege_frame_lab);
	gtk_label_set_use_markup (GTK_LABEL (usr_privilege_frame_lab), TRUE);

	dialog_action_area1 = GTK_DIALOG (dialog)->action_area;
	gtk_widget_show (dialog_action_area1);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

	okbutton1 =  gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);
		//gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog), okbutton1, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT (okbutton1), "clicked",
					   GTK_SIGNAL_FUNC(ExitLoginDlg),(gpointer)crt_n);
	DspCurUsrInfo(crt_n);
	DspCurHostInfo(crt_n);
	return dialog;
}


void DspCurUsrInfo( int crt_n )
{
	int	i;

	for( i=0; i<MAX_PRIV_TYPE_NUM; i++ )
	{
		if(CheckUserPrivilege(cur_usr_name, i))
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wgts[crt_n][WgtN_SMS_dsp_monit+i]),TRUE);	
		else
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wgts[crt_n][WgtN_SMS_dsp_monit+i]),FALSE);	
	}
}



void DspCurHostInfo( int crt_n )
{	
	int	i;		
        
	for(i=0; i<all_lcu_num[crt_n]; i++)
	{
		if(CheckHostPrivilege(local_hostname, lcu_list[crt_n][i]))
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(host_privilege_wgts[crt_n][i]),TRUE);
		else	
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(host_privilege_wgts[crt_n][i]),FALSE);
	}
}





