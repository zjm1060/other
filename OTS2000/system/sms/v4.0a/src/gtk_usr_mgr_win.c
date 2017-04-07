/*updated by wzg 20070511*/

#include	<gdk/gdkkeysyms.h>
#include	<gtk/gtk.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../inc/user_privilege_def.h"




PRIVILEGE_DB		privilege_db;

extern const gchar *list_item_data_key;
extern GtkWidget	*privilege_mgr_box;
extern GtkWidget	*user_privilege_wgts[MAX_PRIV_TYPE_NUM],*host_privilege_wgts[MAX_CTRL_LCU_NUM];
extern GtkWidget	*user_name_window, *host_name_window;
extern GtkWidget	*user_list_window, *host_list_window;




LCU_COMMENT		lcu_list[MAX_CTRL_LCU_NUM];
char			host_list[MAX_HOST_NUM][MAX_HOST_NAME_SIZE];

int			all_lcu_num, all_host_num, modified_flag;
char		*cur_host_name;
int			cur_user_pos, cur_host_db_pos;

int		isPrivSync;
int		need_exit_flag;

STN_LCU_POS lcuPos[128];



GtkWidget	*create_main_window(void);
void	DspCurUserInfo(void);
void	DspCurHostInfo(void);
void	PopupMessageBox(char msg[]);
void	HostMgrListCB(GtkWidget*w);          
void 	CreatePasswdBox(void);
void	UpdateCurUserPrivilege(void);
void	UpdateCurHostPrivilege(void);
void	PopupRequestBox(void);
void	WindowTitlesInit(void);
char	*_toUtf8(char *c);


#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")


/*#define DEBUG*/


int InitLcuList(void)
{
	int		i, dev_id, stn_num, dev_num;

	all_lcu_num= 0;
	if(-1==GetStationNum(&stn_num))
		return -1;

	for(i=1; i<=stn_num; i++)
	{
		UCHAR stn_id;
		GetStnIdByIndex(i, &stn_id);
		if(-1==GetGroupNumById((UCHAR)stn_id, &dev_num))           
			continue;    
	    lcuPos[i].start_id  = 	 all_lcu_num  ; 
		for(dev_id=1; dev_id<=dev_num; dev_id++)
		{
			if(-1==GetGroupCommentById((UCHAR)stn_id, (UCHAR)dev_id, lcu_list[all_lcu_num].comment))
				continue;
			if(lcu_list[all_lcu_num].comment[0]==0)
				continue;
			lcu_list[all_lcu_num].lcu_id.stn_id= stn_id;
			lcu_list[all_lcu_num].lcu_id.dev_id= dev_id;

		//	host_privilege_wgts[all_lcu_num]=gtk_check_button_new_with_mnemonic(_toUtf8(lcu_list[all_lcu_num].comment));
			all_lcu_num++;
		}
		lcuPos[i].end_id  = 	 all_lcu_num  ; 
	}
	return 0;
}


int InitHostList(void)
{
	int		i;
 
	all_host_num= GetSysHostNum();
	for(i=0; i<all_host_num; i++)
		GetHostNameById(i+1, host_list[i]);
	return 0;
}

int	main(int argc, char *argv[])
{
	char	rc_filename[256];

    if(MountDB(NULL)==-1)
    {
		printf("\nSMS: MountDB() Error");
		exit(0); 
    }

    if(-1==InitStrTextDefine())
    {
		printf("\nSMS: InitStrTextDefine Error");
		exit(0); 
    }

    sprintf(rc_filename, "%sgtkrc.zh", SYS_DEFINE_PATH); 
    gtk_set_locale();
    gtk_rc_add_default_file(rc_filename); 
    gtk_rc_parse(rc_filename); 
    gtk_init(&argc, &argv); 

	WindowTitlesInit();

    LoadUserPrivilegeDatabase();

	if(-1==InitLcuList())
	{
		printf("\nSMS: Fail to init lcu list");
		exit(0);
	}
	if(-1==InitHostList())
	{
		printf("\nSMS: Fail to init host list");
		exit(0);
	}

    modified_flag= 0;

    privilege_mgr_box= create_main_window();
    gtk_widget_show(privilege_mgr_box); 
    gtk_main();
	return 0;
}



void UpdateCurUserPrivilege(void)
{
	unsigned short	i;
	USER_DEF		*cur_user;
	USER_DEF		user_tmp;

	if(cur_user_pos==-1)
		return;

	cur_user= &privilege_db.user_list[cur_user_pos];
	memcpy(user_tmp.privilege, cur_user->privilege, sizeof(user_tmp.privilege));
	memset(&cur_user->privilege, 0, sizeof(cur_user->privilege));
	for(i=0; i<MAX_PRIV_TYPE_NUM; i++)	
	{	
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(user_privilege_wgts[i])))
			cur_user->privilege[i/8] |= (1<<(i%8));
		else
			cur_user->privilege[i/8] &= (~(1<<(i%8)));
	}
	for(i=0; i<sizeof(user_tmp.privilege); i++)
	{
		if(user_tmp.privilege[i]!=cur_user->privilege[i])
		{
			modified_flag= 1;
			break;
		}
	}
	
#ifdef DEBUG
	printf("UpdateCurUserPrivilege: user_name=%s, privilege=%02x%02x\n", 
		cur_user->username, cur_user->privilege[1], cur_user->privilege[0]);
#endif	
}


void UpdateCurHostPrivilege(void)
{
	int			j;
	HOST_DEF	host_tmp;

	if(cur_host_db_pos==-1)
		return;

	host_tmp= privilege_db.host_list[cur_host_db_pos];
	privilege_db.host_list[cur_host_db_pos].lcu_num= 0;
	for(j=0; j<all_lcu_num; j++)	
	{	
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(host_privilege_wgts[j])))
			privilege_db.host_list[cur_host_db_pos].lcu[privilege_db.host_list[cur_host_db_pos].lcu_num++]= lcu_list[j].lcu_id;
	}

#ifdef DEBUG
	printf("UpdateCurHostPrivilege: host_name=%s\n", cur_host_name );
#endif

	if(host_tmp.lcu_num!=privilege_db.host_list[cur_host_db_pos].lcu_num)
	{
		modified_flag= 1;
		return;
	}
	for(j=0; j<host_tmp.lcu_num; j++)	
	{	
		if(privilege_db.host_list[cur_host_db_pos].lcu[j].stn_id!=host_tmp.lcu[j].stn_id
			|| privilege_db.host_list[cur_host_db_pos].lcu[j].dev_id!=host_tmp.lcu[j].dev_id)
		{
			modified_flag= 1;
			return;
		}
	}
}



void UsrMgrListCB(GtkWidget *w)          /* when format type selected */
{
	GList		*dlist;
	GtkObject	*list_item;

	dlist= GTK_LIST(w)->selection;
	if(!dlist)		
		return;

	if(cur_user_pos!=-1)
		UpdateCurUserPrivilege();

	list_item= GTK_OBJECT(dlist->data);	
	cur_user_pos= (int)gtk_object_get_data(list_item,list_item_data_key);	
	DspCurUserInfo();
}

void HostMgrListCB(GtkWidget *w)          /* when format type selected */
{
	GList		*dlist;
	GtkObject	*list_item;
	int			i, select_row;	
	
	dlist = GTK_LIST(w)->selection;
	if(!dlist)		
		return;

	if(cur_host_db_pos!=-1)
		UpdateCurHostPrivilege();

	list_item= GTK_OBJECT(dlist->data);	
	select_row= (int)gtk_object_get_data(list_item, list_item_data_key);	
	cur_host_name= host_list[select_row];

	for(i=0; i<privilege_db.host_num; i++)
	{
		if(!strcmp(cur_host_name, privilege_db.host_list[i].hostname))
			break;
	}
	if(i>=privilege_db.host_num)
	{
		strcpy(privilege_db.host_list[i].hostname, cur_host_name);
		privilege_db.host_list[i].lcu_num= 0;
		privilege_db.host_num++;
	}
	cur_host_db_pos= i;
	DspCurHostInfo();	
}


void DspCurUserInfo(void)
{	
	int		i;	

	gtk_label_set_text(GTK_LABEL(user_name_window),
		_toUtf8(privilege_db.user_list[cur_user_pos].username));

	for(i=0; i<MAX_PRIV_TYPE_NUM; i++ )
	{
		if((privilege_db.user_list[cur_user_pos].privilege[i/8]>>(i%8)) & 1)
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(user_privilege_wgts[i]), TRUE);
		else
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(user_privilege_wgts[i]), FALSE);
	}
}


void DspCurHostInfo(void)
{	
	int		j, k, state;	
	
	gtk_label_set_text(GTK_LABEL(host_name_window), _toUtf8(cur_host_name));

	for(j=0; j<all_lcu_num; j++)	
	{	
		state= 0;
		for(k=0; k<privilege_db.host_list[cur_host_db_pos].lcu_num; k++)
		{
			if(privilege_db.host_list[cur_host_db_pos].lcu[k].stn_id==lcu_list[j].lcu_id.stn_id
				&& privilege_db.host_list[cur_host_db_pos].lcu[k].dev_id==lcu_list[j].lcu_id.dev_id)
			{
				state= 1;
				break;
			}
		}
		if(state)
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(host_privilege_wgts[j]), TRUE);
		else
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(host_privilege_wgts[j]), FALSE);
	}
}



void DeleteUser(void)
{
	int			i;
	GList		*dlist;
	GtkObject	*list_item;

	if(privilege_db.user_num==0)
	{
		PopupMessageBox(_CS_("SMS_CSTR_USER_NUM_ZERO"));
		return;
	}

	for(i=cur_user_pos; i<(privilege_db.user_num-1); i++)
		privilege_db.user_list[i]= privilege_db.user_list[i+1];
	privilege_db.user_num--;    
    modified_flag= 1;

	gtk_list_clear_items(GTK_LIST(user_list_window), cur_user_pos, cur_user_pos+1);
	dlist = GTK_LIST(user_list_window)->children;
	i= 0;
	while(dlist)
	{
		list_item=GTK_OBJECT(dlist->data);	
		gtk_object_set_data(list_item, list_item_data_key, (gpointer)i);
		dlist= dlist->next;
		i++;
	}
	cur_user_pos= -1;
	gtk_list_select_item(GTK_LIST(user_list_window), 0);
}


void SaveData(void)
{
	if(cur_user_pos!=-1)
		UpdateCurUserPrivilege();
	if(cur_host_db_pos!=-1)
		UpdateCurHostPrivilege();
    SaveUserPrivilegeDatabase();
    modified_flag= 0;
}
 
void SyncPrivtoHosts(void)
{
	if(cur_user_pos!=-1)
		UpdateCurUserPrivilege();
	if(cur_host_db_pos!=-1)
		UpdateCurHostPrivilege();
	if(modified_flag)
		PopupRequestBox();
	printf("\nNo realized");
}


void ExitProc(void)
{
	if(cur_user_pos!=-1)
		UpdateCurUserPrivilege();
	if(cur_host_db_pos!=-1)
		UpdateCurHostPrivilege();
	need_exit_flag= 1;
	if(modified_flag)
		PopupRequestBox();
	else 
		exit(0);
}


