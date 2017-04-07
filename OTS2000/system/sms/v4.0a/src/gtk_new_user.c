/* passwd.c -- Prompt a passwd. Meaning, all input looks like a
 * a series of *'s. Store the actual data typed by the user in
 * an internal variable. Don't allow paste operations. Handle 
 * backspacing by deleting all text from insertion point to the end of text.
 * by zyp on 2005.11
* updated on 2006.1.17 : added display current login user name on the toolbar 
 */
/*updated by wzg 20070511*/


#include	<gtk/gtk.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../inc/user_privilege_def.h"

 
void	DspCurUserInfo(void);
void	DspCurHostInfo(void);
void	GetPasswd_activate( GtkWidget *text_w, gpointer callback_data );
void	GetPasswd_changed( GtkWidget *text_w, gpointer callback_data );
void	GetUserName_activate( GtkWidget *text_w, gpointer callback_data );
void	GetNewPasswd_activate( GtkWidget *text_w, gpointer callback_data );
void	GetNewPasswd_changed( GtkWidget *text_w, gpointer callback_data );
void 	CreatePasswdBox(void);
void	PopupMessageBox(char *msg);
void	DestroyDlg(GtkWidget *w);
void	ExitProc(void);
void	UsrMgrListCB(GtkWidget *w);
void	HostMgrListCB(GtkWidget *w); 
void	SaveData(void);
void	SyncPrivtoHosts(void);
void	DeleteUser(void);




gchar		user_privilege_titles[MAX_PRIV_TYPE_NUM][64];

GtkWidget *passwd_box,*request_box,*usrname_entry,*passwd_entry,*new_passwd_entry;
GtkWidget	*privilege_mgr_box;
GtkWidget	*user_list_window, *host_list_window;
GtkWidget	*user_privilege_wgts[MAX_PRIV_TYPE_NUM],*host_privilege_wgts[MAX_CTRL_LCU_NUM];
GtkWidget	*user_name_window, *host_name_window;

const gchar *list_item_data_key="list_item_row_no";

extern int		all_lcu_num, all_host_num, modified_flag;
extern char		*cur_host_name;
extern int		cur_user_pos;
extern PRIVILEGE_DB		privilege_db;
extern LCU_COMMENT		lcu_list[MAX_CTRL_LCU_NUM];
extern char			host_list[MAX_HOST_NUM][MAX_HOST_NAME_SIZE];
extern int			cur_user_pos, cur_host_db_pos;
extern  STN_LCU_POS lcuPos[128];


static 	GtkWidget	*vbox1; 
/*#define	DEBUG*/


char *_toUtf8(char *c)
{
  return (g_locale_to_utf8(c,-1,0,0,0));  
}

char *_fromUtf8(char *c)
{
  return (g_locale_from_utf8(c,-1,0,0,0));  
}






void	WindowTitlesInit(void)
{
	strcpy(user_privilege_titles[PRIV_ALM_ACK], _CS_("SMS_CSTR_ALM_ACK"));
	strcpy(user_privilege_titles[PRIV_PRINT], _CS_("SMS_CSTR_PRINT"));
	strcpy(user_privilege_titles[PRIV_LCU_CTRL], _CS_("SMS_CSTR_LCU_CTRL"));
	strcpy(user_privilege_titles[PRIV_POWER_ADJ], _CS_("SMS_CSTR_POWER_ADJ"));
	strcpy(user_privilege_titles[PRIV_SYS_RUN_SET], _CS_("SMS_CSTR_SYS_RUN_SET"));
	strcpy(user_privilege_titles[PRIV_CTRL_BLOCK_SET], _CS_("SMS_CSTR_CTRL_BLOCK_SET"));
	strcpy(user_privilege_titles[PRIV_IN_FORCE_SET], _CS_("SMS_CSTR_IN_FORCE_SET"));
	strcpy(user_privilege_titles[PRIV_OUT_FORCE_SET], _CS_("SMS_CSTR_OUT_FORCE_SET"));
	strcpy(user_privilege_titles[PRIV_SYS_CONF_MGR], _CS_("SMS_CSTR_SYS_CONF_MGR"));
	strcpy(user_privilege_titles[PRIV_TAG_OPR], _CS_("SMS_CSTR_TAG_OPR"));
	strcpy(user_privilege_titles[PRIV_SCAN_ALM_SET], _CS_("SMS_CSTR_SCAN_ALM_SET"));
	strcpy(user_privilege_titles[PRIV_IO_ATTR_SET], _CS_("SMS_CSTR_IO_ATTR_SET"));
	strcpy(user_privilege_titles[PRIV_SGZY_SET], _CS_("SMS_CSTR_SGZY_SET"));
	strcpy(user_privilege_titles[PRIV_RTVALUE_MAN_SET], _CS_("SMS_CSTR_RTVALUE_MAN_SET"));
	strcpy(user_privilege_titles[PRIV_IMP_MAN_SET], _CS_("SMS_CSTR_IMP_MAN_SET"));
	strcpy(user_privilege_titles[PRIV_LIMIT_PARA_SET], _CS_("SMS_CSTR_LIMIT_PARA_SET"));
	strcpy(user_privilege_titles[PRIV_SYS_MAINT], _CS_("SMS_CSTR_SYS_MAINT"));
	strcpy(user_privilege_titles[PRIV_LCU_SELECT_SET], _CS_("SMS_CSTR_LCU_SELECT_SET"));
}


void AddUser(void)
{
    if(privilege_db.user_num< MAX_USER_NUM)
		CreatePasswdBox();
    else
		PopupMessageBox(_CS_("SMS_CSTR_USER_NUM_OVER"));
}


void CheckUserInfo(char	*username, char	*passwd)
{
	int			i;
	GtkWidget	*list_item;

	for(i=0; i<privilege_db.user_num; i++)
	{
		if(!strcmp(username, privilege_db.user_list[i].username)) /* typed username found in <usrdb>	*/
		{
			PopupMessageBox( _CS_("SMS_CSTR_USER_NAME_REPEAT") );
			return;
		}
	}

	/* enter the username into <usrdb>	*/
	strcpy(privilege_db.user_list[i].username, username);
	strcpy(privilege_db.user_list[i].passwd, passwd);
	memset(&privilege_db.user_list[i].privilege, 0, sizeof(privilege_db.user_list[i].privilege));
        
	privilege_db.user_num++;       

	list_item= gtk_list_item_new_with_label(_toUtf8(username));	
	gtk_container_add(GTK_CONTAINER(user_list_window), list_item);
	gtk_widget_show(list_item);
	gtk_object_set_data(GTK_OBJECT(list_item), list_item_data_key, (gpointer)i);
	gtk_list_select_item(GTK_LIST(user_list_window),i);

    modified_flag= 1;
	DestroyDlg(passwd_box);
	return;
}


void CreatePasswdBox(void)
{
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *button;  

	passwd_box= gtk_dialog_new();
	gtk_widget_set_usize(GTK_WIDGET(passwd_box), 300, 150);
	gtk_window_set_title(GTK_WINDOW(passwd_box),_toUtf8(_CS_("SMS_CSTR_USER_WIN_TITLE")));
	gtk_window_set_transient_for(GTK_WINDOW(passwd_box), GTK_WINDOW(privilege_mgr_box));

	hbox= gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(passwd_box)->vbox), hbox, TRUE, TRUE, 0);    
	label= gtk_label_new(_toUtf8(_CS_("SMS_CSTR_USER_NAME")));
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);  
	usrname_entry= gtk_entry_new_with_max_length(MAX_USER_NAME_LEN-1);
	gtk_signal_connect(GTK_OBJECT(usrname_entry), "activate",
		GTK_SIGNAL_FUNC(GetUserName_activate), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), usrname_entry, TRUE, TRUE, 0);
	gtk_entry_set_text(GTK_ENTRY(usrname_entry),"");
	gtk_entry_set_position(GTK_ENTRY(usrname_entry),0);

	hbox= gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX (GTK_DIALOG(passwd_box)->vbox), hbox, TRUE, TRUE, 0);     
	label= gtk_label_new(_toUtf8( _CS_("SMS_CSTR_USER_PASSWD")));
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);   
	passwd_entry= gtk_entry_new_with_max_length(MAX_USER_PASSWD_LEN-1);
	gtk_signal_connect(GTK_OBJECT(passwd_entry), "activate",
		GTK_SIGNAL_FUNC(GetPasswd_activate), NULL);
   	gtk_signal_connect(GTK_OBJECT(passwd_entry), "changed",
		GTK_SIGNAL_FUNC(GetPasswd_changed), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), passwd_entry, TRUE, TRUE, 0);
	gtk_entry_set_text(GTK_ENTRY(passwd_entry),"");
	gtk_entry_set_position(GTK_ENTRY(passwd_entry),0);	
	
    hbox= gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(passwd_box)->vbox), hbox, TRUE, TRUE, 0);
    label= gtk_label_new(_toUtf8(_CS_("SMS_CSTR_USER_PASSWD_ACK")));
    gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);    		
    new_passwd_entry= gtk_entry_new_with_max_length(MAX_USER_PASSWD_LEN-1);
    gtk_signal_connect(GTK_OBJECT(new_passwd_entry), "activate",
		GTK_SIGNAL_FUNC(GetNewPasswd_activate),	NULL);
	gtk_signal_connect(GTK_OBJECT(new_passwd_entry), "changed",
		GTK_SIGNAL_FUNC(GetPasswd_changed), NULL);
    gtk_box_pack_start(GTK_BOX (hbox), new_passwd_entry, TRUE, TRUE, 0); 
    gtk_entry_set_text(GTK_ENTRY(new_passwd_entry),"");
    gtk_entry_set_position(GTK_ENTRY(new_passwd_entry),0);    	
 			
    button = gtk_button_new_with_label(_toUtf8(_CS_("SMS_CSTR_OK")));
	gtk_widget_set_usize(GTK_WIDGET(button), 50, 30);
	gtk_box_pack_start(GTK_BOX (GTK_DIALOG(passwd_box)->action_area), button, TRUE, TRUE, 0);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
		GTK_SIGNAL_FUNC(GetNewPasswd_activate), NULL);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(button);
    
	button = gtk_button_new_with_label(_toUtf8(_CS_("SMS_CSTR_CANCEL")));
	gtk_widget_set_usize( GTK_WIDGET(button), 50, 30);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(passwd_box)->action_area), button, TRUE, TRUE, 0);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
		GTK_SIGNAL_FUNC(DestroyDlg), passwd_box);

	gtk_widget_show_all(passwd_box);   	
}

void GetUserName_activate(GtkWidget *text_w, gpointer callback_data)
{	 		
	gtk_widget_grab_focus(passwd_entry);	
}

void GetPasswd_activate( GtkWidget *text_w, gpointer callback_data )
{			
	gtk_widget_grab_focus(new_passwd_entry);   		
}

void GetPasswd_changed( GtkWidget *text_w, gpointer callback_data )
{				
	gtk_entry_set_visibility(GTK_ENTRY(text_w),FALSE);	
}


void GetNewPasswd_activate( GtkWidget *text_w, gpointer callback_data )
{   
	char	username_str[MAX_USER_NAME_LEN];
	char	passwd_str[MAX_USER_PASSWD_LEN];
	char	new_passwd_str[MAX_USER_PASSWD_LEN];
		
    strcpy(username_str, gtk_entry_get_text(GTK_ENTRY(usrname_entry)));
    strcpy(passwd_str, gtk_entry_get_text(GTK_ENTRY(passwd_entry)));		
    strcpy(new_passwd_str, gtk_entry_get_text(GTK_ENTRY(new_passwd_entry)));

#ifdef	DEBUG
	printf("username= %s, passwd= %s, new_passwd= %s\n", username_str, passwd_str, new_passwd_str );
#endif 		

    if(strcmp(new_passwd_str,passwd_str))
    {
        PopupMessageBox( _CS_("SMS_CSTR_PASSWD_ERR") );
        return;
    }
    if(!strcmp(username_str,"") || !strcmp(passwd_str,"") || !strcmp(new_passwd_str,""))		
    {
        PopupMessageBox( _CS_("SMS_CSTR_STR_EMPTY") );
        return;
    }

	CheckUserInfo( username_str, passwd_str );
}

void PopupMessageBox(char *msg)
{
	GtkWidget *msg_dialog;

	msg_dialog = gtk_message_dialog_new(GTK_WINDOW(privilege_mgr_box),GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,  
						GTK_MESSAGE_INFO,GTK_BUTTONS_OK, _toUtf8(msg));
	gdk_beep();
	gtk_dialog_run(GTK_DIALOG(msg_dialog));
	gtk_widget_destroy(msg_dialog);	
}


void DestroyDlg(GtkWidget *w)
{	
	gtk_widget_destroy(passwd_box);
}

void GetSaveRespond(GtkWidget* w, gpointer data)
{
	extern int		need_exit_flag;

	if((int)data)
	{
		SaveUserPrivilegeDatabase();
		modified_flag= 0;
	}
	gtk_widget_destroy(request_box);
	if(need_exit_flag)
		exit(0);
}

void PopupRequestBox(void)
{
	GtkWidget *label;
	GtkWidget *button;  

	request_box = gtk_dialog_new();
	gtk_widget_set_usize( GTK_WIDGET (request_box), 300, 150);
	gtk_window_set_title(GTK_WINDOW (request_box),_toUtf8(_CS_("SMS_CSTR_REQUEST_WIN_TITLE")));
	gtk_window_set_transient_for(GTK_WINDOW(request_box), GTK_WINDOW(privilege_mgr_box));

	label = gtk_label_new(_toUtf8(_CS_("SMS_CSTR_REQUEST_SAVE")));
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(request_box)->vbox), label, TRUE, TRUE, 0);  

    button = gtk_button_new_with_label(_toUtf8( _CS_("SMS_CSTR_SAVE_YES")));
	gtk_widget_set_usize(GTK_WIDGET (button), 50, 30);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(request_box)->action_area), button, TRUE, TRUE, 0);
	gtk_signal_connect(GTK_OBJECT (button), "clicked",
		GTK_SIGNAL_FUNC(GetSaveRespond), (gpointer)1);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default (button);
    
	button = gtk_button_new_with_label(_toUtf8( _CS_("SMS_CSTR_SAVE_NO")));
	gtk_widget_set_usize(GTK_WIDGET (button), 50, 30);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(request_box)->action_area), button, TRUE, TRUE, 0);
	gtk_signal_connect(GTK_OBJECT (button), "clicked",
		GTK_SIGNAL_FUNC(GetSaveRespond), (gpointer)0);

	gtk_widget_show_all(request_box);   	
}


gint dispLCUDev(GtkWidget *widget,   gpointer data)
{
	int   id  = data ;  
	int i ; 
    
	for  (i = 0  ; i<all_lcu_num ; i++)  
	{
		
		if  (i>=lcuPos[id].start_id && i<lcuPos[id].end_id)
			gtk_widget_show(host_privilege_wgts[i]);
		else
			gtk_widget_hide(host_privilege_wgts[i]);
	}
	return  TRUE ; 
}


GtkWidget* create_main_window(void)
{
	GtkWidget	*main_window;
	GtkWidget	*table1;
	GtkWidget	*user_frame;
	GtkWidget	*alignment1;
	GtkWidget	*hbox1;
	GtkWidget	*user_name_lab;

	GtkWidget	*user_privileg_frame;
	GtkWidget	*alignment2;
  
	GtkWidget	*privilege_frame_lab;
	GtkWidget	*host_list_lab;
	GtkWidget	*scrolledmain_window;  
	GtkWidget	*scrolledwindow2;

	GtkWidget	*frame3;
	GtkWidget	*alignment3;
	GtkWidget	*hbox2;
	GtkWidget	*host_name_lab; 

	GtkWidget	*ctrl_obj_frame;
	GtkWidget	*alignment4;
	GtkWidget	*label11;
	GtkWidget	*Userlist;
	GtkWidget	*vbuttonbox1;
	GtkWidget	*add_user_btn;
	GtkWidget	*del_user_btn;
	GtkWidget	*save_btn, *sync_btn;
	GtkWidget	*exit_btn ,  *optionmenu, *menu,*items;
	GtkWidget	*list_item;
	int			i;
	int  station_num   ;   
	char  stn_name[128] ; 
		

	main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (main_window), _toUtf8(_CS_("SMS_CSTR_MAIN_WIN_TITLE")));
	gtk_signal_connect(GTK_OBJECT(main_window), "delete_event",
				   (GtkSignalFunc)ExitProc, NULL);


	table1 = gtk_table_new (2, 5, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (main_window), table1);
	gtk_table_set_row_spacings (GTK_TABLE (table1), 2);
	gtk_table_set_col_spacings (GTK_TABLE (table1), 5);

	user_frame = gtk_frame_new (NULL);
	gtk_widget_show (user_frame);
	gtk_table_attach (GTK_TABLE (table1), user_frame, 1, 2, 0, 1,
					  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
					  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (user_frame), GTK_SHADOW_IN);

	alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment1);
	gtk_container_add (GTK_CONTAINER (user_frame), alignment1);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);

	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_container_add (GTK_CONTAINER (alignment1), hbox1);

	user_name_lab = gtk_label_new (_toUtf8(_CS_("SMS_CSTR_USR_NAME")));
	gtk_widget_show (user_name_lab);
	gtk_box_pack_start (GTK_BOX (hbox1), user_name_lab, FALSE, FALSE, 0);


	user_name_window = gtk_label_new ("");
	gtk_widget_show (user_name_window);
	gtk_box_pack_start (GTK_BOX (hbox1), user_name_window, FALSE, FALSE, 0);  

	user_privileg_frame = gtk_frame_new (NULL);
	gtk_widget_show (user_privileg_frame);
	gtk_table_attach (GTK_TABLE (table1), user_privileg_frame, 1, 2, 1, 2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (user_privileg_frame), GTK_SHADOW_IN);

	alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment2);
	gtk_container_add (GTK_CONTAINER (user_privileg_frame), alignment2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);  
	gtk_widget_set_usize (alignment2, 180, -1);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (alignment2), vbox1);

	for(i=0;i<MAX_PRIV_TYPE_NUM;i++)
	{
		user_privilege_wgts[i]=gtk_check_button_new_with_mnemonic (_toUtf8(user_privilege_titles[i]));
		gtk_widget_show (user_privilege_wgts[i]);
		gtk_box_pack_start (GTK_BOX (vbox1), user_privilege_wgts[i], FALSE, FALSE, 0);
	}

	privilege_frame_lab = gtk_label_new (_toUtf8(_CS_("SMS_CSTR_PRIVILEGE")));
	gtk_widget_show (privilege_frame_lab);
	gtk_frame_set_label_widget (GTK_FRAME (user_privileg_frame), privilege_frame_lab);
	gtk_label_set_use_markup (GTK_LABEL (privilege_frame_lab), TRUE);

	host_list_lab = gtk_label_new (_toUtf8(_CS_("SMS_CSTR_HOST_NAME_LIST")));
	gtk_widget_show (host_list_lab);
	gtk_table_attach (GTK_TABLE (table1), host_list_lab, 2, 3, 0, 1,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (host_list_lab), 0, 0.5);

	scrolledmain_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledmain_window);
	gtk_table_attach (GTK_TABLE (table1), scrolledmain_window, 0, 1, 1, 2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_widget_set_usize (scrolledmain_window, 100, -1);


	user_list_window = gtk_list_new();  

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledmain_window),user_list_window );  
	gtk_widget_show (user_list_window); 
  
	gtk_signal_connect(GTK_OBJECT(user_list_window), "selection_changed",
				   GTK_SIGNAL_FUNC(UsrMgrListCB),NULL); 

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledmain_window),
									  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS); 
	gtk_list_set_selection_mode(GTK_LIST(user_list_window),GTK_SELECTION_SINGLE);
	for(i=0;i<privilege_db.user_num;i++)
	{          
		list_item = gtk_list_item_new_with_label(_toUtf8(privilege_db.user_list[i].username));	
		gtk_container_add(GTK_CONTAINER(user_list_window),list_item );
		gtk_widget_show(list_item);
		gtk_object_set_data(GTK_OBJECT(list_item),list_item_data_key,(gpointer)i);
	}

	cur_user_pos= -1;    
	gtk_list_select_item(GTK_LIST(user_list_window),0);

	scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow2);
	gtk_table_attach (GTK_TABLE (table1), scrolledwindow2, 2, 3, 1, 2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_widget_set_usize (scrolledwindow2, 140, -1);
    

	frame3 = gtk_frame_new (NULL);
	gtk_widget_show (frame3);
	gtk_table_attach (GTK_TABLE (table1), frame3, 3, 4, 0, 1,
					  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
					  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_IN);
	gtk_widget_set_usize (frame3, 180, -1);

	alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment3);
	gtk_container_add (GTK_CONTAINER (frame3), alignment3);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 12, 0);


	hbox2 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox2);
	gtk_container_add (GTK_CONTAINER (alignment3), hbox2);

	host_name_lab = gtk_label_new (_toUtf8(_CS_("SMS_CSTR_HOST_NAME")));
	gtk_widget_show (host_name_lab);
	gtk_box_pack_start (GTK_BOX (hbox2), host_name_lab, FALSE, FALSE, 0);

	host_name_window = gtk_label_new ("");
	gtk_widget_show (host_name_window);
	gtk_box_pack_start (GTK_BOX (hbox2), host_name_window, FALSE, FALSE, 0);  

	ctrl_obj_frame = gtk_frame_new (NULL);
	gtk_widget_show (ctrl_obj_frame);
	gtk_table_attach (GTK_TABLE (table1), ctrl_obj_frame, 3, 4, 1, 2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (ctrl_obj_frame), GTK_SHADOW_IN);

	alignment4 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment4);
	gtk_container_add (GTK_CONTAINER (ctrl_obj_frame), alignment4);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment4), 0, 0, 12, 0);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (alignment4), vbox1);  
//////////////////////////////add station button !!!///////////////////////////////


	GetStationNum(&station_num);	
	optionmenu = gtk_option_menu_new();
   	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (vbox1), optionmenu, FALSE, FALSE, 0);

	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);

	for(i=1;i<=station_num;i++) 
	{
		UCHAR stnId;
		GetStnIdByIndex(i, &stnId);

		if( GetStnCommentById(stnId,stn_name)!=-1 )
		{					
			items = gtk_menu_item_new_with_label(_toUtf8(stn_name));	 
			gtk_widget_show( items);
			gtk_container_add(GTK_CONTAINER(menu),items);
			gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (dispLCUDev), GINT_TO_POINTER(i));				
		}
	 }
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
   
	//for(i=lcuPos[1].start_id ;i<lcuPos[1].end_id;i++)
	for  (i = 0  ; i<all_lcu_num ; i++)  
	{
		host_privilege_wgts[i]=gtk_check_button_new_with_mnemonic(_toUtf8(lcu_list[i].comment));
		
		if  (i>=lcuPos[1].start_id && i<lcuPos[1].end_id)
		    gtk_widget_show(host_privilege_wgts[i]);
		else
			gtk_widget_hide(host_privilege_wgts[i]);
		gtk_box_pack_start (GTK_BOX (vbox1), host_privilege_wgts[i], FALSE, FALSE, 0);    
	}

	host_list_window = gtk_list_new();  

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow2), host_list_window);  
	gtk_widget_show (host_list_window); 
  
	gtk_signal_connect(GTK_OBJECT(host_list_window), "selection_changed",
				   GTK_SIGNAL_FUNC(HostMgrListCB),NULL); 

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2),
									  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_list_set_selection_mode(GTK_LIST(host_list_window), GTK_SELECTION_SINGLE);  

	for(i=0;i<all_host_num;i++)
	{
		list_item = gtk_list_item_new_with_label(_toUtf8(host_list[i]));	
		gtk_container_add(GTK_CONTAINER(host_list_window), list_item);
		gtk_widget_show(list_item);
		gtk_object_set_data(GTK_OBJECT(list_item), list_item_data_key,(gpointer)i);
	}

	cur_host_db_pos= -1;
	gtk_list_select_item(GTK_LIST(host_list_window), 0);

	label11 = gtk_label_new (_toUtf8(_CS_("SMS_CSTR_PRIVILEGE")));
	gtk_widget_show (label11);
	gtk_frame_set_label_widget (GTK_FRAME (ctrl_obj_frame), label11);

	Userlist = gtk_label_new (_toUtf8(_CS_("SMS_CSTR_USR_NAME_LIST")));
	gtk_widget_show (Userlist);
	gtk_table_attach (GTK_TABLE (table1), Userlist, 0, 1, 0, 1,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(Userlist), (gfloat)0.05, (gfloat)0.5);

	vbuttonbox1 = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox1);
	gtk_table_attach (GTK_TABLE (table1), vbuttonbox1, 4, 5, 1, 2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbuttonbox1), 10);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox1), GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox1), 10);

	add_user_btn = gtk_button_new_with_mnemonic (_toUtf8(_CS_("SMS_CSTR_ADD_USER")));
	gtk_widget_show (add_user_btn);
	gtk_container_add (GTK_CONTAINER (vbuttonbox1), add_user_btn);
	GTK_WIDGET_SET_FLAGS (add_user_btn, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT(add_user_btn), "pressed", GTK_SIGNAL_FUNC(AddUser), NULL); 

	del_user_btn = gtk_button_new_with_mnemonic (_toUtf8(_CS_("SMS_CSTR_DEL_USER")));
	gtk_widget_show (del_user_btn);
	gtk_container_add (GTK_CONTAINER (vbuttonbox1), del_user_btn);
	GTK_WIDGET_SET_FLAGS (del_user_btn, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT(del_user_btn), "pressed", GTK_SIGNAL_FUNC(DeleteUser), NULL); 

	sync_btn = gtk_button_new_with_mnemonic (_toUtf8(_CS_("SMS_CSTR_SYNC_PRIV")));
	gtk_widget_show (sync_btn);
	gtk_container_add (GTK_CONTAINER (vbuttonbox1), sync_btn);
	GTK_WIDGET_SET_FLAGS (sync_btn, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT(sync_btn), "pressed", GTK_SIGNAL_FUNC(SyncPrivtoHosts), NULL);
	gtk_widget_set_sensitive(sync_btn, FALSE);

	save_btn = gtk_button_new_with_mnemonic (_toUtf8(_CS_("SMS_CSTR_SAVE")));
	gtk_widget_show (save_btn);
	gtk_container_add (GTK_CONTAINER (vbuttonbox1), save_btn);
	GTK_WIDGET_SET_FLAGS (save_btn, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT(save_btn), "pressed", GTK_SIGNAL_FUNC(SaveData), NULL);

	exit_btn = gtk_button_new_with_mnemonic (_toUtf8(_CS_("SMS_CSTR_QUIT")));
	gtk_widget_show (exit_btn);
	gtk_container_add (GTK_CONTAINER (vbuttonbox1), exit_btn);
	GTK_WIDGET_SET_FLAGS (exit_btn, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT(exit_btn), "clicked", GTK_SIGNAL_FUNC(ExitProc), NULL);   

	return main_window;
}

