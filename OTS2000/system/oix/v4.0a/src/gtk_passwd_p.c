#include <gtk/gtk.h>

/* passwd.c -- Prompt a passwd. Meaning, all input looks like a
 * a series of *'s. Store the actual data typed by the user in
 * an internal variable. Don't allow paste operations. Handle 
 * backspacing by deleting all text from insertion point to the end of text.
 * by zyp on 2005.11
 * updated on 2006.1.17 : added display current login user name on the toolbar 
 * UNIX HAVE SYN TO WINDOWS by chi 2007.05.20 
*/

#include	<sys/stat.h>
#include	<stdio.h>
#include        <time.h>
#include	<string.h>


#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"



int		priority_mode;
void	GetPasswd_activate_simu(char *username,char *passwd);
void	GetPasswd_activate ( GtkWidget *text_w, gpointer callback_data );
void	GetPasswd_changed ( GtkWidget *text_w, gpointer callback_data );
void	GetUserName_activate ( GtkWidget *text_w, gpointer callback_data );
void	GetNewPasswd_activate ( GtkWidget *text_w, gpointer callback_data );
void	GetNewPasswd_changed ( GtkWidget *text_w, gpointer callback_data );
void	GetNewPasswd1_activate ( GtkWidget *text_w, gpointer callback_data );
void	GetPasswd_next( GtkWidget *text_w, gpointer callback_data );
void 	CreatePasswdBox (int crt_n);
void	UsrLogout ( int crt_n );
void	BroadLogMsg(char *userName, short log_flag);

extern char OIX_KICK_SUSER ; 
extern void PopupLoginInfoBox (GtkWidget *parent, int crt_n );
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern int    GetHost_Cmd();
extern gint        windk_display_get_n_screens      (GdkDisplay  *display);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);


int selfkill_timer_id=0  ;

#define	LOGIN	1
#define	LOGOUT	0

#define	MODE_LOGIN		0
#define	MODE_CHANGE_PASSWD	1

#define  DEBUG

void Menu_Login(GtkWidget *w,gpointer data)
{
	priority_mode	= MODE_LOGIN;
	CreatePasswdBox ((int)data );
}

void Menu_Logout(GtkWidget *w,gpointer data)
{
	UsrLogout ( (int)data );
	LoadUserPrivilegeDatabase ( );
}

 void Menu_ChgPasswd(GtkWidget *w,gpointer data)
{
	priority_mode	= MODE_CHANGE_PASSWD;
	CreatePasswdBox ((int)data);
}

void Menu_DspLogInfo(GtkWidget *w,gpointer data)
{       
	int crt_n = (gint)data ;
	PopupLoginInfoBox ( main_w[crt_n],crt_n );
}

void DestroyPassDlg(GtkWidget *w)
{	
    int	crt_n;
	GetCrtNumberOfWgt (w, &crt_n);
  	if( crt_n < 0 )
  		return; 
	gtk_widget_destroy(w);
	wgts[crt_n][WgtN_passwd_box]=NULL;
}

void CreatePasswdBox( int crt_n )
{
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *button;  

	gchar *pConvert=0;

	wgts[crt_n][WgtN_passwd_box] = gtk_dialog_new();
	gtk_signal_connect(GTK_OBJECT (wgts[crt_n][WgtN_passwd_box]), "delete_event",
				   (GtkSignalFunc) DestroyPassDlg, NULL);
	gtk_window_set_transient_for(GTK_WINDOW(wgts[crt_n][WgtN_passwd_box]), GTK_WINDOW(main_w[crt_n]));

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(wgts[crt_n][WgtN_passwd_box])->vbox), hbox, TRUE, TRUE, 0);    
	label = gtk_label_new(pConvert=_toUtf8EX( OIX_CSTR_USER_NAME));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);  

	wgts[crt_n][WgtN_username] = gtk_entry_new_with_max_length (MAX_USER_NAME_LEN-1);
	gtk_entry_set_text(GTK_ENTRY(wgts[crt_n][WgtN_username]),"");
	gtk_entry_set_position(GTK_ENTRY(wgts[crt_n][WgtN_username]),0);
	gtk_signal_connect(GTK_OBJECT(wgts[crt_n][WgtN_username]), "activate",
		   GTK_SIGNAL_FUNC(GetUserName_activate),wgts[crt_n][WgtN_username]);
	gtk_box_pack_start (GTK_BOX (hbox), wgts[crt_n][WgtN_username], TRUE, TRUE, 0);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(wgts[crt_n][WgtN_passwd_box])->vbox), hbox, TRUE, TRUE, 0);     
	label = gtk_label_new(pConvert=_toUtf8( _CS_(OIX_CSTR_PASSWD)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);   

	wgts[crt_n][WgtN_passwd_text] = gtk_entry_new_with_max_length (MAX_USER_PASSWD_LEN-1);
	gtk_entry_set_text(GTK_ENTRY(wgts[crt_n][WgtN_passwd_text]),"");
	gtk_entry_set_position(GTK_ENTRY(wgts[crt_n][WgtN_passwd_text]),0);
	gtk_entry_set_visibility(GTK_ENTRY(wgts[crt_n][WgtN_passwd_text]),FALSE);	
	gtk_box_pack_start (GTK_BOX (hbox), wgts[crt_n][WgtN_passwd_text], TRUE, TRUE, 0);
	if( priority_mode == MODE_LOGIN )
	{
		gtk_widget_set_usize( GTK_WIDGET (wgts[crt_n][WgtN_passwd_box]), 300, 100);
		gtk_window_set_title(GTK_WINDOW (wgts[crt_n][WgtN_passwd_box]), pConvert=_toUtf8(_CS_(OIX_CSTR_LOGIN)));
		G_SAFE_FREE(pConvert);
		gtk_signal_connect(GTK_OBJECT(wgts[crt_n][WgtN_passwd_text]), "activate",
			GTK_SIGNAL_FUNC(GetPasswd_activate),wgts[crt_n][WgtN_passwd_text]);
	}
	else if( priority_mode == MODE_CHANGE_PASSWD ) 
	{
		gtk_widget_set_usize(GTK_WIDGET (wgts[crt_n][WgtN_passwd_box]), 300, 150);
		gtk_window_set_title(GTK_WINDOW (wgts[crt_n][WgtN_passwd_box]), pConvert=_toUtf8EX(OIX_CSTR_CHANGE_PASSWD));
		G_SAFE_FREE(pConvert);
		gtk_signal_connect(GTK_OBJECT(wgts[crt_n][WgtN_passwd_text]), "activate",
			GTK_SIGNAL_FUNC(GetPasswd_next),wgts[crt_n][WgtN_passwd_text]);

		hbox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG(wgts[crt_n][WgtN_passwd_box])->vbox), hbox, TRUE, TRUE, 0);
		label = gtk_label_new(pConvert=_toUtf8EX( OIX_CSTR_NEW_PASSWD));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);    		
    
		wgts[crt_n][WgtN_new_passwd_text] = gtk_entry_new_with_max_length (MAX_USER_PASSWD_LEN-1);
		gtk_signal_connect(GTK_OBJECT(wgts[crt_n][WgtN_new_passwd_text]), "activate",
			GTK_SIGNAL_FUNC(GetNewPasswd_activate),	wgts[crt_n][WgtN_new_passwd_text]);
 		gtk_entry_set_text(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_text]),"");
 		gtk_entry_set_position(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_text]),0);
		gtk_entry_set_visibility(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_text]),FALSE);	
		gtk_box_pack_start (GTK_BOX (hbox), wgts[crt_n][WgtN_new_passwd_text], TRUE, TRUE, 0); 
                
		hbox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG(wgts[crt_n][WgtN_passwd_box])->vbox), hbox, TRUE, TRUE, 0);
		label = gtk_label_new(pConvert=_toUtf8EX( OIX_CSTR_PASSWD_AGAIN));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);    		
    
		wgts[crt_n][WgtN_new_passwd_again] = gtk_entry_new_with_max_length (MAX_USER_PASSWD_LEN-1);
 		gtk_entry_set_text(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_again]),"");
 		gtk_entry_set_position(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_again]),0);
		gtk_signal_connect(GTK_OBJECT(wgts[crt_n][WgtN_new_passwd_again]), "activate",
			GTK_SIGNAL_FUNC(GetPasswd_activate), wgts[crt_n][WgtN_new_passwd_again]);
		gtk_entry_set_visibility(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_again]),FALSE);	
		gtk_box_pack_start (GTK_BOX (hbox), wgts[crt_n][WgtN_new_passwd_again], TRUE, TRUE, 0); 
	}
     			
	button = gtk_button_new_with_label (pConvert=_toUtf8EX( OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);
	gtk_widget_set_usize( GTK_WIDGET (button), 50, 30);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(wgts[crt_n][WgtN_passwd_box])->action_area), button, TRUE, TRUE, 0);
	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
		GTK_SIGNAL_FUNC(GetPasswd_activate), wgts[crt_n][WgtN_passwd_text]);

	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default (button);

	button = gtk_button_new_with_label (pConvert=_toUtf8EX( OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);
	gtk_widget_set_usize( GTK_WIDGET (button), 50, 30);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(wgts[crt_n][WgtN_passwd_box])->action_area), button, TRUE, TRUE, 0);
	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
		GTK_SIGNAL_FUNC(DestroyPassDlg), GTK_OBJECT (wgts[crt_n][WgtN_passwd_box]));
	gtk_window_set_position(GTK_WINDOW(wgts[crt_n][WgtN_passwd_box]),GTK_WIN_POS_CENTER) ; 
	gtk_widget_show_all(wgts[crt_n][WgtN_passwd_box]);
    
	LoadUserPrivilegeDatabase ( );
}



void	CheckLogin(int crt_n, char	*username, char	*passwd )
{
	int		i;	
	gint	num_screen;
	char 	label_usrname[50];
	gchar *pConvert=0;
	
	if(!CheckUserExist(username))
	{
		PopupMessageBox ( main_w[crt_n], MSG_UNAUTHORIZED_USER );
		return;
	}
	if(!CheckUserLogin(username, passwd))
	{
		PopupMessageBox ( main_w[crt_n], MSG_PASSWD_ERR );
		return;
	}
		
	strcpy(cur_usr_name, username);
	BroadLogMsg(cur_usr_name, LOGIN);				

	g_display = gdk_display_get_default ();
	num_screen = windk_display_get_n_screens (g_display); 


       
	for( i=0; i<num_screen; i++ )
	{
		if(!wgts[i][WgtN_cur_user])
			wgts[i][WgtN_cur_user] = gtk_object_get_data(GTK_OBJECT(main_w[i]),"label_cur_usr");
		if(wgts[i][WgtN_cur_user])
		{
			strcpy(label_usrname , pConvert=_toUtf8EX(OIX_STR_CUR_USR)) ;
			G_SAFE_FREE(pConvert);
			
			gtk_label_set_text(GTK_LABEL(wgts[i][WgtN_cur_user]),strcat(label_usrname , username));
		}
	}

	if( CheckUserPrivilege(cur_usr_name, PRIV_SYS_MAINT)   )
	{
		//PopupMessageBox ( main_w[crt_n], "kick super user!!" );
		//call self-kill programe
		//rm by chichi
		if (OIX_KICK_SUSER)
			selfkill_timer_id   =gtk_timeout_add (    KICKOFFTIME   , (GtkFunction)UsrLogout,(gpointer)crt_n); 
	
	}




}

void	CheckNewPasswd (int crt_n, char	*username, char	*passwd, char	*new_passwd )
{
	if(!CheckUserExist(username))
	{
		PopupMessageBox ( main_w[crt_n], MSG_UNAUTHORIZED_USER );
		return;
	}
	if(!CheckUserLogin(username, passwd))
	{
		PopupMessageBox ( main_w[crt_n], MSG_PASSWD_ERR );
		return;
	}

	if(strlen(new_passwd)>=MAX_USER_PASSWD_LEN)
	{
		PopupMessageBox ( main_w[crt_n], MSG_NEW_PASSWD_LENGTH_ERR );
		return;
	}
		
	if(-1==UpdateUserPasswd(username, passwd, new_passwd))
		PopupMessageBox ( main_w[crt_n], MSG_UPDATE_PASSWD_FAIL );
	else
		PopupMessageBox ( main_w[crt_n], MSG_UPDATE_PASSWD_SUCCESS );
}

void	UsrLogout (int crt_n )
{
	char	str[256];
	int		i;
	gint	cur_ws_crt_num;
	gchar *pConvert=0;

	if(cur_usr_name[0])
	{
		g_display = gdk_display_get_default ();
	//	cur_ws_crt_num = gdk_display_get_n_screens (g_display); 	
	cur_ws_crt_num =windk_display_get_n_screens(g_display); 
		sprintf ( str, "'%s'", cur_usr_name );

	//	PopupMessageBox ( main_w[crt_n], strcat ( str, _CS_(OIX_STR_CANCEL_LOG) ) );		
		BroadLogMsg(cur_usr_name, LOGOUT);
		cur_usr_name[0]= 0;				
		
		for( i=0; i<cur_ws_crt_num; i++ )
		{
			if(!wgts[i][WgtN_cur_user])
				wgts[i][WgtN_cur_user] = gtk_object_get_data(GTK_OBJECT(main_w[i]),"label_cur_usr");
			if(wgts[i][WgtN_cur_user])
			{
				gtk_label_set_text(GTK_LABEL(wgts[i][WgtN_cur_user]), pConvert=_toUtf8EX( OIX_STR_LOGOUT));	
				G_SAFE_FREE(pConvert);
			}
		}		
	}
	else
		PopupMessageBox ( main_w[crt_n], MSG_NO_USER_LOGIN );

	if(selfkill_timer_id!=0) 
	{
		gtk_timeout_remove ( selfkill_timer_id  ); 
		selfkill_timer_id =0 ;
		//PopupMessageBox ( main_w[crt_n], "超级用户被踢!!" );
	}
}

void	BroadLogMsg(char *userName, short log_flag)
{
	char 		lan_msg[32],msg[SMS_MESSAGE_SIZE];
	time_t		newtime;
	struct	tm	devtime;

	newtime = time(NULL);
	devtime = *localtime( &newtime);

	lan_msg[0]= DPS_SMS;
	lan_msg[1]= GetHost_Cmd();
#ifdef SUN_OS		
	Swap4Byte(&newtime);
#endif	
	memcpy(&lan_msg[2],&newtime,4);
	strcpy(msg,userName);
	if (log_flag !=LOGOUT) 
	    strcat(msg,_CS_(DPS_STR_USERS_LOGIN));
	else 
		strcat(msg,_CS_(DPS_STR_USERS_LOGOUT)); 
	if(strlen(msg)>SMS_MESSAGE_SIZE)
		msg[SMS_MESSAGE_SIZE-1]='0';
        
	strcpy(&lan_msg[6],msg);

	BdSms(msg);      
}

void	GetPasswd_activate_simu( char *username, char *passwd ) //added by dm for user login from SimuTrainee
{				
	int crt_n = 0;
	if( username[0] && passwd[0] )						
		CheckLogin( crt_n, username, passwd );			
	else
		PopupMessageBox( main_w[crt_n], MSG_USRNAME_OR_PASSWD_ERR );				
}

void	GetPasswd_activate( GtkWidget *text_w, gpointer callback_data )
{
	char	username[MAX_USER_NAME_LEN], passwd[MAX_USER_PASSWD_LEN];
	char	new_passwd[MAX_USER_PASSWD_LEN], new_passwd1[MAX_USER_PASSWD_LEN];
	int		crt_n;

	GetCrtNumberOfWgt(text_w, &crt_n);
  	if( crt_n < 0 )
  		return; 		
	
	if( priority_mode == MODE_LOGIN ) 
	{
		strcpy(username, gtk_entry_get_text(GTK_ENTRY(wgts[crt_n][WgtN_username])));
		strcpy(passwd, gtk_entry_get_text(GTK_ENTRY(text_w)));					
		gtk_entry_set_visibility(GTK_ENTRY(text_w), FALSE);
		DestroyPassDlg(wgts[crt_n][WgtN_passwd_box]);
			
		if( username[0] && passwd[0] )						
			CheckLogin( crt_n, username, passwd );			
		else
			PopupMessageBox( main_w[crt_n], MSG_USRNAME_OR_PASSWD_ERR );				
	}
	else if(priority_mode ==MODE_CHANGE_PASSWD)
	{
		strcpy(username, gtk_entry_get_text(GTK_ENTRY(wgts[crt_n][WgtN_username])) );
		strcpy(passwd, gtk_entry_get_text(GTK_ENTRY(wgts[crt_n][WgtN_passwd_text])));		
		strcpy(new_passwd, gtk_entry_get_text(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_text])));
		strcpy(new_passwd1, gtk_entry_get_text(GTK_ENTRY(wgts[crt_n][WgtN_new_passwd_again])));
		DestroyPassDlg(wgts[crt_n][WgtN_passwd_box]);
		if(strcmp(new_passwd, new_passwd1))
		{
			PopupMessageBox ( main_w[crt_n], MSG_NEW_PASSWD_ERR );
			return;
		}
		
		if(username[0] && passwd[0] && new_passwd[0])		
			CheckNewPasswd ( crt_n, username, passwd, new_passwd );		
		else		
			PopupMessageBox ( main_w[crt_n], MSG_USRNAME_OR_OLD_PASSWD_ERR );
	}
	else 			
		gtk_widget_grab_focus(wgts[crt_n][WgtN_new_passwd_text]);
}

void	GetPasswd_changed( GtkWidget *text_w, gpointer callback_data )
{				
	gtk_entry_set_visibility(GTK_ENTRY(text_w),FALSE);	
}


void	GetNewPasswd_activate( GtkWidget *text_w, gpointer callback_data )
{   
	int		crt_n;

	GetCrtNumberOfWgt(text_w, &crt_n);
  	if( crt_n < 0 )
  		return; 

	gtk_widget_grab_focus(wgts[crt_n][WgtN_new_passwd_again]);
}

void	GetNewPasswd_changed ( GtkWidget *text_w, gpointer callback_data )
{
	gtk_entry_set_visibility(GTK_ENTRY(text_w),FALSE);	
}


void	GetUserName_activate( GtkWidget *text_w, gpointer callback_data )
{
	int crt_n;
	
	GetCrtNumberOfWgt (text_w, &crt_n);
  	if( crt_n < 0 )
  		return;
  		
	gtk_widget_grab_focus(wgts[crt_n][WgtN_passwd_text]);	
}

void	GetPasswd_next( GtkWidget *text_w, gpointer callback_data )
{
	int crt_n;
	
	GetCrtNumberOfWgt (text_w, &crt_n);
  	if( crt_n < 0 )
  		return;
  		
	gtk_widget_grab_focus(wgts[crt_n][WgtN_new_passwd_text]);	
}


