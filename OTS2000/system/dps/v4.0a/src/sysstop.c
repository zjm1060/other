#include 	<signal.h>
#include	<gtk/gtk.h>

#include 	"../../../dms/v4.0a/inc/dms_com.h"
#include 	"../inc/dps_rec_def.h"
#include 	"../inc/dps_type_def.h"
#include 	"../inc/ipc_common.h"
#include 	"../inc/dps_functions_def.h"


#ifdef		SUN_OS
#include 	<sys/ipc.h>
#include 	<sys/sem.h>
#include 	<sys/shm.h>
#include 	<sys/socket.h>  /* for AF_INET        */
#include 	<netinet/in.h>  /* for struct in_addr */
#include 	<arpa/inet.h>   /* for inet_ntoa()    */
#include 	<sys/types.h>
#include	<fcntl.h>
#include 	<netdb.h>       /* for struct hostent */
#include	<errno.h>
#endif

#if defined(SUN) && defined(_AIX)
#include	<sys/wait.h>
#elif defined(SUN)
#include 	<sys/filio.h>
#include	<wait.h>
#endif


#ifdef	WINDOWS_OS
#define		SIGQUIT					SIGBREAK
#define 	bzero(addr, size)		memset(addr, 0, size)
#define		sleep(sec)				Sleep(sec*1000)
#define		kill(run_pid, SIGNAL)	TerminateProcess(run_pid, 0)
//#define		errno					WSAGetLastError()
#endif


#define	SYS_RUN_SEM				"sys_run_sem"

static	void ExitEmsSystem( int signo );
void	SaveSysStopTime(void);	


int		show_ask_window_flag;


typedef struct 
{
	GtkWidget	*window, *vbox, *hbox, *label, *sep, *button_ack, *button_deny;
	int			user_select;
	gint		timer;
} FRAME_WINDOWS;



char *_(char *c)
{
    return(g_locale_to_utf8(c,-1,0,0,0));
} 


int popup_exit_func(GtkWidget *w, gpointer data)
{
	gtk_main_quit();
	return TRUE;
}

int popup_button_func(GtkWidget *w, gpointer data)
{
	FRAME_WINDOWS	*frame_win= data;
	
	gtk_widget_destroy(frame_win->window);
	return TRUE;
}



void	PopupMessageBox(char *msg)
{
	GdkDisplay	*display, *display_new;
	GdkScreen	*screen;
	char		host[32];
	char		button_label[32];
	PangoFontDescription *desc;
	FRAME_WINDOWS	frame_win;
	
	if(!show_ask_window_flag)
	{
		SendTraceMsg(0, 0, msg);
		return;
	}
	
	gtk_init(NULL, NULL);
	display= gdk_display_get_default();
	strcpy(host, gdk_display_get_name(display));
	display_new= gdk_display_open(host);
	if(!(screen=gdk_display_get_default_screen(display_new)))    	   
	{
		printf("\nFail to open display: %s", host);
		return;	
	}
	
	frame_win.window= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_screen(GTK_WINDOW(frame_win.window), screen);
	gtk_window_set_policy(GTK_WINDOW(frame_win.window), FALSE, FALSE, FALSE);
	gtk_window_set_title(GTK_WINDOW(frame_win.window), "H9000 SYSTEM");
	gtk_container_border_width(GTK_CONTAINER(frame_win.window), 8);
	gtk_signal_connect(GTK_OBJECT(frame_win.window), "destroy", 
		GTK_SIGNAL_FUNC(popup_exit_func), &frame_win);
	
	frame_win.vbox= gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame_win.window), frame_win.vbox);
	
	frame_win.label= gtk_label_new(_(msg));
	gtk_misc_set_padding(GTK_MISC(frame_win.label), 10, 20);
	gtk_box_pack_start(GTK_BOX(frame_win.vbox), frame_win.label, FALSE, FALSE, 10);
	
	frame_win.sep= gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(frame_win.vbox), frame_win.sep, FALSE, FALSE, 10);
	
	frame_win.hbox= gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(frame_win.vbox), frame_win.hbox, FALSE, FALSE, 10);

	strcpy(button_label, _CS_("SYSMGR_MSG_CSTR_OK"));
	frame_win.button_ack= gtk_button_new_with_label(_(button_label));
	gtk_widget_set_usize((frame_win.button_ack), 80, 40);
	gtk_box_pack_end(GTK_BOX(frame_win.hbox), frame_win.button_ack, FALSE, FALSE, 10);
	gtk_signal_connect(GTK_OBJECT(frame_win.button_ack), "clicked", 
		GTK_SIGNAL_FUNC(popup_button_func), &frame_win);
	
	desc = pango_font_description_from_string(_CS_("SYSMGR_MSG_CSTR_FONTTYPE"));
	gtk_widget_modify_font(frame_win.label, desc);
	gtk_widget_modify_font(GTK_BIN(frame_win.button_ack)->child, desc);
	pango_font_description_free(desc);
	
	GTK_WIDGET_SET_FLAGS(frame_win.button_ack, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(frame_win.button_ack);
	gtk_window_set_position(GTK_WINDOW(frame_win.window), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_widget_show_all(frame_win.window);
	gtk_main();
}




int gtk_button_ack_func(GtkWidget *w, gpointer data)
{
	FRAME_WINDOWS	*frame_win= data;
	
	frame_win->user_select= 1;
	gtk_widget_destroy(frame_win->window);
	return TRUE;
}

int gtk_button_deny_func(GtkWidget *w, gpointer data)
{
	FRAME_WINDOWS	*frame_win= data;
	
	frame_win->user_select= 0;
	gtk_widget_destroy(frame_win->window);
	return TRUE;
}

int gtk_destroy_func(GtkWidget *w, gpointer data)
{
	FRAME_WINDOWS	*frame_win= data;
	
	if(frame_win->timer!=0)
	{
		gtk_timeout_remove(frame_win->timer);
		frame_win->timer= 0;
	}
	gtk_main_quit();
	return TRUE;
}

gint gtk_select_timer_func(gpointer data)
{
	FRAME_WINDOWS	*frame_win= data;
	
	gtk_widget_destroy(frame_win->window);
	return 0;
}

int GetUserSelect(char *info, int default_sel)
{
	GdkDisplay	*display, *display_new;
	GdkScreen	*screen;
	char		host[64];
	char		button_label[32];
	PangoFontDescription *desc;
	FRAME_WINDOWS	frame_win;
	
	gtk_init(NULL, NULL);
	display= gdk_display_get_default();
	strcpy(host, gdk_display_get_name(display));
	display_new= gdk_display_open(host);
	if(!(screen=gdk_display_get_default_screen(display_new)))    	   
	{
		printf("\nFail to open display: %s", host);
    	exit(-1);	
  	}

	frame_win.window= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_screen(GTK_WINDOW(frame_win.window), screen);  
	gtk_window_set_policy(GTK_WINDOW(frame_win.window), FALSE, FALSE, FALSE);
	gtk_window_set_title(GTK_WINDOW(frame_win.window), "H9000 SYSTEM");
	gtk_window_set_default_size(GTK_WINDOW(frame_win.window), 400, 175);
	gtk_container_border_width(GTK_CONTAINER(frame_win.window), 8);
	gtk_signal_connect(GTK_OBJECT(frame_win.window), "destroy", 
		GTK_SIGNAL_FUNC(gtk_destroy_func), &frame_win);

	frame_win.vbox= gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame_win.window), frame_win.vbox);
	
	frame_win.label= gtk_label_new(_(info));
	gtk_misc_set_padding(GTK_MISC(frame_win.label), 10, 20);
	gtk_box_pack_start(GTK_BOX(frame_win.vbox), frame_win.label, FALSE, FALSE, 10);

	frame_win.sep= gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(frame_win.vbox), frame_win.sep, FALSE, FALSE, 10);

	frame_win.hbox= gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(frame_win.vbox), frame_win.hbox, FALSE, FALSE, 10);

	strcpy(button_label, _CS_("SYSMGR_MSG_CSTR_YES"));
	frame_win.button_ack= gtk_button_new_with_label(_(button_label));
	gtk_widget_set_usize((frame_win.button_ack), 80, 40);
	gtk_box_pack_start(GTK_BOX(frame_win.hbox), frame_win.button_ack, TRUE, FALSE, 30);
	gtk_signal_connect(GTK_OBJECT(frame_win.button_ack), "clicked", 
		GTK_SIGNAL_FUNC(gtk_button_ack_func), &frame_win);

	strcpy(button_label, _CS_("SYSMGR_MSG_CSTR_NO"));
	frame_win.button_deny= gtk_button_new_with_label(_(button_label));
	gtk_widget_set_usize(GTK_WIDGET(frame_win.button_deny), 80, 40);
	gtk_box_pack_end(GTK_BOX(frame_win.hbox), frame_win.button_deny, TRUE, FALSE, 30);
	gtk_signal_connect(GTK_OBJECT(frame_win.button_deny), "clicked", 
		GTK_SIGNAL_FUNC(gtk_button_deny_func), &frame_win);

	desc = pango_font_description_from_string(_CS_("SYSMGR_MSG_CSTR_FONTTYPE"));
	gtk_widget_modify_font(frame_win.label, desc);
	gtk_widget_modify_font(GTK_BIN(frame_win.button_ack)->child, desc);
	gtk_widget_modify_font(GTK_BIN(frame_win.button_deny)->child, desc);
	pango_font_description_free(desc);

	GTK_WIDGET_SET_FLAGS(frame_win.button_ack, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(frame_win.button_ack);
	gtk_window_set_position(GTK_WINDOW(frame_win.window), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_widget_show_all(frame_win.window);

	frame_win.user_select= default_sel;
    frame_win.timer =gtk_timeout_add(10000, (GtkFunction)gtk_select_timer_func, &frame_win); 
	gtk_main();
	return frame_win.user_select;
}



#ifdef	WINDOWS_OS /*WINDOWS_OS*/
pid_t FindRunningSystem(void)
{
	HANDLE	hFile, hSem;
	char	filename[256];
	
	sprintf(filename, "%s%s", SYS_TMP_PATH, SYS_LOCK_FILE);
	hFile= CreateFile(filename, GENERIC_READ|GENERIC_WRITE, 
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(hFile!=INVALID_HANDLE_VALUE) /*task is not run*/
	{
		return OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SYS_RUN_SEM);
	}
	else /*task is running*/
	{
		hSem= OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SYS_RUN_SEM);
		if(hSem)
			return hSem;
		else
		{
			MessageBox(NULL, _CS_("SYSMGR_MSG_CSTR_RUNNING"), NULL, MB_OK);
			exit(0);
		}
	}	
}
#endif /*WINDOWS_OS*/

#ifdef	SUN_OS
pid_t FindRunningSystem(void)
{
	int		fd;
	struct	flock lock;
	char	filename[256];
	
	sprintf(filename, "%s%s", SYS_TMP_PATH, SYS_LOCK_FILE);
	if( (fd=open(filename, O_RDONLY, 0666)) == -1 )
		return 0;
	
	lock.l_type= F_WRLCK;
	lock.l_start= 0;
	lock.l_whence= SEEK_SET;
	lock.l_len= 0;
	if( fcntl(fd, F_GETLK, &lock) < 0 )
		return 0;
	
    if( lock.l_type==F_UNLCK )
		return 0;
	else
		return lock.l_pid;
}
#endif /*SUN_OS*/


#ifdef	WINDOWS_OS
BOOL IsTaskRunning(char *task)
{
	char	cmd_str[256], task_name[256], cmd_ret_str[1024];
	
	sprintf(cmd_str, "tasklist /nh /fi \"imagename eq %s.exe\"", task);
	if(-1==NoConsoleSystemCmd(cmd_str, cmd_ret_str, sizeof(cmd_ret_str)))
		return FALSE;
	
	sprintf(task_name, "%s.exe", task);
	if(strstr(cmd_ret_str, task_name))
		return TRUE;
	else
		return FALSE;
}

#else /*SUN_OS*/
BOOL IsTaskRunning(char *task)
{
	char	cmd_str[256], filename[256], task_name[256];	
	char	err_str[256];
	char	buff_new[256];
	FILE	*fp;
	BOOL	ret;
	
	sprintf(filename, "%ssysmgr_tmp", SYS_TMP_PATH);
	sprintf(cmd_str, "ps -ef > %s", filename);
	sprintf(task_name, "%s%s", task, TASK_SUFFIX_NAME);
	system(cmd_str);
	usleep(100*1000);
	if(NULL==(fp=fopen(filename, "r")))
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		return FALSE;
	}
	ret= FALSE;
	while(fgets(buff_new, sizeof(buff_new), fp)!=NULL)
	{
		if(strstr(buff_new, task_name))
		{
			ret= TRUE;
			break;
		}
	}
	fclose(fp);
	unlink(filename);
	return ret;
}

#endif


int	main(int argc, char *argv[])
{
#ifdef	WINDOWS_OS
	WSADATA	WSAData;
#else
	pthread_t	h_thread;
#endif

	if(-1==InitSysLog(H9000_MODULE_SYSSTOP, "SysStop"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	
	if(-1==InitStrTextDefine())
		exit(0);
	
	if(argc>1 && strstr(argv[1], "no")) /*不弹出询问窗口*/
		show_ask_window_flag= 0;
	else
		show_ask_window_flag= 1;
	
#ifdef	WINDOWS_OS
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		PopupMessageBox("Fail to initialize net lib");
		exit(0);
	}
#endif
	
	if(show_ask_window_flag && !GetUserSelect(_CS_("SYSMGR_MSG_CSTR_ASK_STOP"), 0))
		exit(0);

	SendTraceMsg(0, 0, "Now stop H9000 system");
	
	ExitEmsSystem(0);
}


static void ExitEmsSystem( int signo )
{
	int		i;
	char	local_hostname[MAX_HOST_NAME_SIZE];
	char	process[MAX_PROCESS_NAME_SIZE];
	char	cmd[256], *ptr;
	char	str[256];
//	POINTER	point;
	pid_t	ret_pid;
	int		proc_num;
	
	ret_pid= FindRunningSystem();
	if(ret_pid!=0) /*系统正在运行*/
	{
#ifdef	WINDOWS_OS
		ReleaseSemaphore(ret_pid, 1, NULL);
#else
		kill(ret_pid, SIGTERM);
#endif
		exit(0);
	}

	if(gethostname(local_hostname, sizeof(local_hostname)))	
	{
		PopupMessageBox("Fail to gethostname");
		exit(0);
	}
	
#ifdef	WINDOWS_OS
	strupr(local_hostname);
#endif
	
	if(-1==MountDB(NULL))
	{
		int error_id= GetH9000ErrorId();
		
		if(H9000_ERROR_DB==error_id)
			PopupMessageBox("装载实时数据库失败");
		else if(H9000_ERROR_CONFIGFILE==error_id)
			PopupMessageBox("装载系统配置文件失败");
		else if(H9000_ERROR_NO_LICENSE==error_id)
			PopupMessageBox("无H9000系统授权文件，禁止运行!\n请联系中水科技");
		else if(H9000_ERROR_ILLEGAL_LICENSE==error_id)
			PopupMessageBox("非法的H9000系统授权文件，禁止运行!\n请联系中水科技");
		PopupMessageBox("Fail to mount DB");
		exit(0);
	}	
	
	if(!lan_out_init())
	{
		PopupMessageBox("Fail to init lan module");
		ClearEmsEnv();
		exit(0);
	}
	
//	modified by wzg 20120330
// 	if(-1!=GetHostStatePtId(local_hostname, &point))
// 		BroadCastIndPtState(point, 0, 0);
	sprintf(str, "%s h9000 is stopping", local_hostname);
	BroadCastSms(str, TRUE);
	
	SaveSysStopTime();
	
	sleep(1);

	proc_num= GetRunProcessNum();
	SendTraceMsg(0, 0, "Total tasks num: %d", proc_num);
	if(-1==proc_num)
	{
		PopupMessageBox("Fail to get run task num");
		ClearEmsEnv();
		exit(0);
	}
	
	for(i=0; i<proc_num; i++)
	{
		if(-1==GetRunProcess(i, process))
		{
			SendTraceMsg(0, 0, "Fail to get task: %d!", i);
			continue;
		}
		if(ptr=strchr(process, ' '))
			*ptr= 0;

#ifdef WINDOWS_OS
		sprintf(cmd, "tskill /A %s", process);
#else
		sprintf(cmd, "pkill %s", process);
#endif
		system(cmd);

		SendTraceMsg(0, 0, "stop task %s", process);
	}
	
	if(IsTaskRunning("oix"))
#ifdef WINDOWS_OS
	system("tskill /A oix");
#else
	system("pkill oix");
#endif
	
	sleep(1);
	ClearEmsEnv();
	UnMountDB();
	exit(0);
}



void SaveSysStopTime(void)	
{ 
	char		filename[256];
	struct tm	t_tm;
	long		newtime; 
	FILE		*fp;
	
	newtime=time(NULL);
	t_tm= *localtime( &newtime );
	sprintf(filename, "%s%s", SYS_TMP_PATH, EMS_RUN_TIME_FILE);
	if((fp=fopen(filename, "w")) == NULL)
	{
		SendTraceMsg(0, 0, "Fail to open %s", filename);
		return;
	}
	
	fprintf(fp, "H9000_IS_STOPPING_NOW!!!\n%d (%04d-%02d-%02d %02d:%02d:%02d)\n",
		newtime, t_tm.tm_year+1900, t_tm.tm_mon+1, t_tm.tm_mday,
		t_tm.tm_hour, t_tm.tm_min, t_tm.tm_sec);
	fclose(fp);
}






