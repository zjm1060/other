#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include	"../../../../dms/v4.0a/inc/dms_com.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include	"../../inc/gtk_widgets.h"  
#include	"../../inc/mode_def.h"
#include	"/home/ems/system/oix/v4.0a/inc/oix.h"
#ifndef DEMO_DSP_ON 
#define DEMO_DSP_ON 1
#define DEMO_DSP_OFF 0
#endif

#ifdef WINDOWS_OS 
#define WM_OPEN_SIMU_MAIN		(WM_USER+0x001)
#define WM_OPEN_SETTASK			(WM_USER+0x002)
#define WM_OPEN_SETSCENE		(WM_USER+0x003)
#define WM_OPEN_SETFAULT		(WM_USER+0x004)
#define WM_OPEN_CURRENTTASK		(WM_USER+0x005)
#define WM_OPEN_REVERT			(WM_USER+0x006)
#define WM_OPEN_LOCAL_OPR		(WM_USER+0x007)
#define WM_OPEN_HISTORY			(WM_USER+0x008)
#define WM_OPEN_SENDMSG			(WM_USER+0x009)
#define WM_OPEN_EXAM			(WM_USER+0x010)
typedef void (WINAPI *PSWITCHTOTHISWINDOW) (HWND,BOOL);
PSWITCHTOTHISWINDOW SwitchToThisWindow;
#endif

extern int SelectLayer[MAX_CRT_NUMBER][LAYERNUM];
extern void  ProgDispEntryName (GtkWidget *parent,	int crt_n);
extern int ProgAutoCheck(GtkWidget *parent,	int crt_n);
extern gint num_screen;
extern void ManDemodsp(int crt_n);
extern GtkWidget* create_selected_class_window1 (GtkWidget *w,int crt_n);
extern  void Menu_Login(GtkWidget *w,gpointer data);
extern  void Menu_Logout(GtkWidget *w,gpointer data);
extern  void Menu_ChgPasswd(GtkWidget *w,gpointer data);
extern  void Menu_DspLogInfo(GtkWidget *w,gpointer data);
extern	void Menu_OpenFile(GtkWidget *w,gpointer data);

extern  void GetPrevDsp(GtkWidget *w,gpointer data);
extern  void GetNextDsp(GtkWidget *w,gpointer data);
extern  void ProgTrapLog (GtkWidget *w, int crt_n ); 
extern  void	AcknDspAlmFlag (GtkWidget *w,int  crt_n );
extern  void ProgSoftCopy ( int crt_n);
extern  void ProgHardCopy1(int crt_n );
extern void ProgRealCurve (GtkWidget *parent ,int  crt_n );
extern void ProgRealConfig (GtkWidget *parent ,int  crt_n );
extern void ProgOpCharacterCurve(GtkWidget *parent ,int crt_n );
extern void ProgGenPowerPlanCurve(GtkWidget *parent ,int crt_n);
extern  int doDSPAlarmAck(GtkWidget       *widget,   gpointer     data) ;
void  on_item53_activate (GtkMenuItem     *menuitem,    gpointer         user_data) ;
extern int GetTurnClass(GtkMenuItem     *menuitem,gpointer user_data);
extern void OpenDspFileCallback(GtkWidget *w,gpointer data);
extern void 	ProgDbTab (GtkWidget *parent, int crt_n );
extern void ChangeDspMode (int crt_n ,int mode );
extern void 	ProgUnresetAlarmTab (GtkWidget *parent,	int crt_n   );
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void PopSearchWin(GtkWidget *parent,int crt_n);
extern void PopEntryInputDlg(GtkWidget *parent,int crt_n);
extern GtkWidget*create_alm_stn_config_win(int crt_n);
extern void  AlmConfig_active(int crt_n);
extern GtkWidget* create_alm_info_config_window (int crt_n);
extern void	SetCanvasBgColor ( int crt_n  );
extern PopupPrinterSetDlg (GtkWidget *parent,int crt_n ,int prt_obj_type);
extern void 	ProgAlarmTab (GtkWidget *parent,	int crt_n  ,int TabMode  );
extern void 	ProgKeySignalTab (GtkWidget *parent,	int crt_n  ,int TabMode  );
extern void 	ProgAlarmTab_new (GtkWidget *parent,	int crt_n  ,int TabMode  );
extern int PauseDemoDsp(GtkWidget*w,int crt_n);
extern GtkWidget* create_selected_demo_screen_window (GtkWidget *w,int crt_n);
extern void	PopupLanguageSetDlg (GtkWidget *parent,int crt_n ,int prt_obj_type);
extern GtkWidget* create_turndsp_config_dlg (GtkWidget       *widget,   int     data);
extern  void  PopUpViewWindow(GtkWidget*parent ,  int crt_n)  ;  
extern void PopupFaultRedisplayBox (GtkWidget *parent ,int crt_n);
extern void EndFaultRedisplay(GtkWidget *parent ,int crt_n );
extern void ProgHisCurve (GtkWidget *parent ,int  crt_n );
extern void ProgCondCheckTab(GtkWidget *parent, int crt_n);
extern void	Redraw(GtkWidget*);
extern int SelectLayer[MAX_CRT_NUMBER][LAYERNUM];
extern gint UpdateDsp (int crt_n );
extern void gtk_main_win_quit(  );
extern int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] );
extern void Menu_OpenCHKFile(GtkWidget * widget, gpointer data);
extern void DelSgzyHistFile(int crt_n,int file_type);

void on_SearchEvent_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

#ifdef  SUN_OS
	system("/home/ems/h9000/bin/findstr.dxe&");
#endif
     printf ("\\\\tt on_SearchEvent_activate!!!!!\\\\n") ; 
}

void
on________a_1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	
	// ProgAlarmTab_new(main_w[(gint)user_data] ,(int) (user_data),FALSE);
}

#ifdef WINDOWS_OS
void
on_simu_main_activate
										(GtkMenuItem     *menuitem,
                                        gpointer         user_data)	
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd==NULL)
	{
		char* cmd = "/home/ems/h9000/bin/SimuTrainee.exe";
		WinExec(cmd,SW_SHOWNORMAL);
	} 
	else
	{
		//SendMessageTimeout(hWnd,WM_OPEN_SIMU_MAIN,0,0,SMTO_NORMAL,200,0);
		SendMessage(hWnd,WM_OPEN_SIMU_MAIN,0,0); 
		SwitchToThisWindow(hWnd,TRUE);
		//ShowWindow( hWnd,SW_SHOWNORMAL ); 
		//SetActiveWindow(hWnd);
		//gtk_widget_set_sensitive(item_clientmain_settask,TRUE);
	}
}

void 
on_simu_settask_activate
										(GtkMenuItem     *menuitem,
										gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_settask;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		hWnd_settask=FindWindow(NULL,"学员任务设置");
		if (hWnd_settask!=NULL)
		{
			SwitchToThisWindow(hWnd_settask,TRUE);
			//ShowWindow( hWnd,SW_SHOWNORMAL );
			//SetActiveWindow(hWnd_settask);
			ShowWindow( hWnd,SW_HIDE ); 
		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_SETTASK,0,0,SMTO_NORMAL,200,0);
			hWnd_settask=FindWindow(NULL,"学员任务设置");
			//SetActiveWindow(hWnd_settask);
			SwitchToThisWindow(hWnd_settask,TRUE);
			ShowWindow( hWnd,SW_HIDE ); 
		}
	}	
}

void 
on_simu_setscene_activate
										(GtkMenuItem     *menuitem,
										gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_setscene;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		hWnd_setscene=FindWindow(NULL,"设置工况");
 		if (hWnd_setscene!=NULL)
		{
			SwitchToThisWindow(hWnd_setscene,TRUE);
			//ShowWindow( hWnd,SW_SHOWNORMAL );
			//SetActiveWindow(hWnd_setscene);
 			ShowWindow( hWnd,SW_HIDE ); 
 		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_SETSCENE,0,0,SMTO_NORMAL,200,0);
			hWnd_setscene=FindWindow(NULL,"设置工况");
			SwitchToThisWindow(hWnd_setscene,TRUE);
			//SetActiveWindow(hWnd_setscene);
 			ShowWindow( hWnd,SW_HIDE ); 
		}
	}	
}

void 
on_simu_setfault_activate
											(GtkMenuItem     *menuitem,
											gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_setfault;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		hWnd_setfault=FindWindow(NULL,"设置故障");
		if (hWnd_setfault!=NULL)
		{
			SwitchToThisWindow(hWnd_setfault,TRUE);
			//ShowWindow( hWnd,SW_SHOWNORMAL );
			//SetActiveWindow(hWnd_setfault);
			ShowWindow( hWnd,SW_HIDE ); 
		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_SETFAULT,0,0,SMTO_NORMAL,200,0);
			hWnd_setfault=FindWindow(NULL,"设置故障");
			SwitchToThisWindow(hWnd_setfault,TRUE);
			//SetActiveWindow(hWnd_setfault);
			ShowWindow( hWnd,SW_HIDE ); 
		}
	}	
}

void 
on_simu_currenttask_activate
											(GtkMenuItem     *menuitem,
											gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_currenttask;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		SendMessageTimeout(hWnd,WM_OPEN_CURRENTTASK,0,0,SMTO_NORMAL,200,0);
		hWnd_currenttask=FindWindow(NULL,"任务列表");
		SwitchToThisWindow(hWnd_currenttask,TRUE);
		//ShowWindow( hWnd,SW_SHOWNORMAL );
		//SetActiveWindow(hWnd_currenttask);
		ShowWindow( hWnd,SW_HIDE ); 
	/*	if (hWnd_currenttask!=NULL)
		{
			ShowWindow( hWnd,SW_SHOWNORMAL );
			SetActiveWindow(hWnd_currenttask);
			ShowWindow( hWnd,SW_HIDE ); 
		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_CURRENTTASK,0,0,SMTO_NORMAL,200,0);
			hWnd_currenttask=FindWindow(NULL,"任务列表");
			SetActiveWindow(hWnd_currenttask);
			ShowWindow( hWnd,SW_HIDE ); 
		}*/
	}	
}

void 
on_simu_revert_activate
											(GtkMenuItem     *menuitem,
											gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_revert;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		SendMessageTimeout(hWnd,WM_OPEN_REVERT,0,0,SMTO_NORMAL,200,0);
		hWnd_revert=FindWindow(NULL,"再现工况");
		SwitchToThisWindow(hWnd_revert,TRUE);
		//ShowWindow( hWnd,SW_SHOWNORMAL );
		//SetActiveWindow(hWnd_revert);
		ShowWindow( hWnd,SW_HIDE ); 
	/*	if (hWnd_revert!=NULL)
		{
			ShowWindow( hWnd,SW_SHOWNORMAL );
			SetActiveWindow(hWnd_revert);
			ShowWindow( hWnd,SW_HIDE ); 
		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_REVERT,0,0,SMTO_NORMAL,200,0);
			hWnd_revert=FindWindow(NULL,"再现工况");
			SetActiveWindow(hWnd_revert);
			ShowWindow( hWnd,SW_HIDE ); 
		}*/
	}	
}

void 
on_simu_local_opr_activate
											(GtkMenuItem     *menuitem,
											gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_local_opr;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		hWnd_local_opr=FindWindow(NULL,"其它运行操作");
		if (hWnd_local_opr!=NULL)
		{
			SwitchToThisWindow(hWnd_local_opr,TRUE);
			//ShowWindow( hWnd,SW_SHOWNORMAL );
			//SetActiveWindow(hWnd_local_opr);
			ShowWindow( hWnd,SW_HIDE ); 
		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_LOCAL_OPR,0,0,SMTO_NORMAL,1000,0);
			hWnd_local_opr=FindWindow(NULL,"其它运行操作");
			SwitchToThisWindow(hWnd_local_opr,TRUE);
			//SetActiveWindow(hWnd_local_opr);
			ShowWindow( hWnd,SW_HIDE ); 
		}
	}	
}

void 
on_simu_history_activate
										(GtkMenuItem     *menuitem,
                                        gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_history;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		hWnd_history=FindWindow(NULL,"历史记录查询");
		if (hWnd_history!=NULL)
		{
			SwitchToThisWindow(hWnd_history,TRUE);
			//ShowWindow( hWnd,SW_SHOWNORMAL );
			//SetActiveWindow(hWnd_history);
			ShowWindow( hWnd,SW_HIDE ); 
		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_HISTORY,0,0,SMTO_NORMAL,200,0);
			hWnd_history=FindWindow(NULL,"历史记录查询");
			SwitchToThisWindow(hWnd_history,TRUE);
			//SetActiveWindow(hWnd_history);
			ShowWindow( hWnd,SW_HIDE ); 
		}
	}	
}
void 
on_simu_sendmsg_activate
											(GtkMenuItem     *menuitem,
											gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_sendmsg;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		SendMessageTimeout(hWnd,WM_OPEN_SENDMSG,0,0,SMTO_NORMAL,200,0);
		hWnd_sendmsg=FindWindow(NULL,"短消息");
		SwitchToThisWindow(hWnd_sendmsg,TRUE);
		ShowWindow( hWnd,SW_HIDE ); 
	}	
}
void 
on_simu_exam_activate
											(GtkMenuItem     *menuitem,
											 gpointer         user_data)	 
{
	HWND   hWnd=FindWindow(NULL,"OTS2000  学员站");
	HWND   hWnd_exam;
	HMODULE hUser32=GetModuleHandle(_T("user32"));
	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	if (hWnd!=NULL)
	{
		hWnd_exam=FindWindow(NULL,"专业考试系统");
		if (hWnd_exam!=NULL)
		{
			SwitchToThisWindow(hWnd_exam,TRUE);
			ShowWindow( hWnd,SW_HIDE ); 
		}
		else
		{
			SendMessageTimeout(hWnd,WM_OPEN_EXAM,0,0,SMTO_NORMAL,200,0);
			hWnd_exam=FindWindow(NULL,"专业考试系统");
			SwitchToThisWindow(hWnd_exam,TRUE);
			ShowWindow( hWnd,SW_HIDE ); 
		}
	}	
}

#endif

void
on_reg1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
         
	 Menu_Login(main_w[(gint)user_data] ,GINT_TO_POINTER (user_data)) ;
}


void 
on_exit_reg1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
        Menu_Logout(main_w[(gint)user_data] ,GINT_TO_POINTER (user_data));	 
}


void
on_changepwd_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	  Menu_ChgPasswd(main_w[(gint)user_data] ,GINT_TO_POINTER (user_data));	 
}


void
on_display_reg_info_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	 Menu_DspLogInfo(main_w[(gint)user_data] ,GINT_TO_POINTER (user_data)); 

}


void
on___________a_1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on________q_1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
        gtk_main_win_quit();  

}

void
on_delsgzy_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{	
	DelSgzyHistFile((gint)user_data,DEL_SGZY_FILE);	
}

void
on_delsftcp_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{	
	printf("del sgzy is pressed %d\n",(gint)user_data);
//#ifdef  SUN_OS
	DelSgzyHistFile((gint)user_data,DEL_SFTCP_FILE);
//#endif
}

void
on_openfile_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	  Menu_OpenFile(main_w[(gint)user_data],GINT_TO_POINTER (user_data));  
}


void
on_CYML_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	OpenDspFileCallback(main_w[(gint)user_data],0);
}


void
on_BMML_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	 OpenDspFileCallback(main_w[(gint)user_data],GINT_TO_POINTER (1)) ;
}


void
on_ZJX_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 	OpenDspFileCallback (main_w[(gint)user_data],GINT_TO_POINTER (3)) ;
}

void
on_LayerFilter_activate                      (GtkMenuItem     *menuitem,
										 gpointer         user_data)
{
	UseLayerFilter[(gint)user_data].UseFilter=(UseLayerFilter[(gint)user_data].UseFilter+1)%2;
	UseLayerFilter[(gint)user_data].changeflag=1;

}

void
on_Layer1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	SelectLayer[(gint)user_data][0]=(SelectLayer[(gint)user_data][0]+1)%2;
	Redraw (canvas[(gint)user_data] );
//	UpdateDsp((gint)user_data);
}


void
on_Layer2_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	SelectLayer[(gint)user_data][1]=(SelectLayer[(gint)user_data][1]+1)%2;
	Redraw (canvas[(gint)user_data] );
}


void
on_Layer3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	SelectLayer[(gint)user_data][2]=(SelectLayer[(gint)user_data][2]+1)%2;
	Redraw (canvas[(gint)user_data] );
}


void
on_Layer4_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	SelectLayer[(gint)user_data][3]=(SelectLayer[(gint)user_data][3]+1)%2;
	Redraw (canvas[(gint)user_data] );
}

void
on_Dbtab_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ProgDbTab(main_w[(gint)user_data],(gint)user_data);
}


void
on_DRBB_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_LSBB_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}





void
on_a_1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


 

void
on_previous_activate                   (GtkMenuItem     *menuitem,GdkEventButton *event,
                                        gpointer         user_data)
{
	GetPrevDsp( main_w[(gint)user_data] ,GINT_TO_POINTER (user_data));
}


void
on_next_activate                       (GtkMenuItem     *menuitem,GdkEventButton *event,
                                        gpointer         user_data)
{
	GetNextDsp( main_w[(gint)user_data] ,GINT_TO_POINTER (user_data));
}





void
on_quit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{	
	gtk_main_win_quit();
}


void
on_item41_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    int i;
     GtkWidget *pause_demo_btn[MAX_CRT_NUMBER],*next_dsp_btn[MAX_CRT_NUMBER];
     if (gtk_check_menu_item_get_active((GtkCheckMenuItem*)menuitem)) 
     {
         for(i=0;i<num_screen;i++)
         {
            pause_demo_btn[i] = gtk_object_get_data(GTK_OBJECT(main_w[(gint)    i]),"pause_demo_btn");
            if (pause_demo_btn[i])
				gtk_widget_show (pause_demo_btn[i]);
        
            next_dsp_btn[i] = gtk_object_get_data(GTK_OBJECT(main_w[(gint)    i]),"next_dsp_btn");        
            gtk_widget_hide (next_dsp_btn[i]);
            printf("item41_toggled \n");         
         
                ChangeDspMode ((gint)    i  ,DEMO_DSP_OFF );    
                ChangeDspMode ((gint)    i  ,DEMO_DSP_ON );         
         }
     }	
}


void
on_item43_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{     
    int i;
    GtkWidget *pause_demo_btn[MAX_CRT_NUMBER];
    if (gtk_check_menu_item_get_active((GtkCheckMenuItem*)menuitem)) 
    {         
        printf("item43_toggled \n");
        for(i=0;i<num_screen;i++)
        {
            pause_demo_btn[i] = gtk_object_get_data(GTK_OBJECT(main_w[(gint)    i]),"pause_demo_btn");
            gtk_widget_hide (pause_demo_btn[i]);
            ChangeDspMode ((gint)    i  ,DEMO_DSP_OFF );         
            ManDemodsp((gint)    i);
        }
    }  
}

void
on_item44_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    int i;
     GtkWidget *pause_demo_btn[MAX_CRT_NUMBER],*next_dsp_btn[MAX_CRT_NUMBER];   
    
    if (gtk_check_menu_item_get_active((GtkCheckMenuItem*)menuitem)) 
    {
        for(i=0;i<num_screen;i++)
        {
            ChangeDspMode ((gint)    i  ,DEMO_DSP_OFF ); 
            printf("item44_toggled \n");
            next_dsp_btn[i] = gtk_object_get_data(GTK_OBJECT(main_w[(gint)    i]),"next_dsp_btn");
            gtk_widget_hide (next_dsp_btn[i]);
            pause_demo_btn[i] = gtk_object_get_data(GTK_OBJECT(main_w[(gint)    i]),"pause_demo_btn");
            gtk_widget_hide (pause_demo_btn[i]);
        }
    }
  
}

void
on_item45_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{   
        create_selected_class_window1 (GTK_WIDGET(menuitem),(gint)user_data);  
  
}

void
on_item46_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{   
        create_selected_demo_screen_window (GTK_WIDGET(menuitem),(gint)user_data);  
  
}

void
on_item42_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     
      /* if (gtk_check_menu_item_get_active((GtkCheckMenuItem*)menuitem)) ;
     {
        ChangeDspMode ((gint)    user_data ,DEMO_DSP_ON ); 
        printf("tem42_activate\n");
     }*/
 printf("#####tem42_activate,user_data=%d\n",(gint) user_data);
    create_turndsp_config_dlg (main_w[(gint) user_data],   (gint) user_data);     
      
}


void
on_item51_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    ProgAlarmTab ( main_w[(gint) user_data],   (gint) user_data,FALSE);  
}
void
on_item511_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    ProgKeySignalTab ( main_w[(gint) user_data],   (gint) user_data,FALSE);  
}
void
on_item53_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    doDSPAlarmAck(main_w[(gint) user_data],   GINT_TO_POINTER( user_data) ) ;
    printf ("alarm  ack  !!!11\n");
}


void
on_UnresetfaultTab_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    printf("user_data=%d\n",(gint)user_data);
	ProgUnresetAlarmTab( main_w[(gint) user_data],   (gint) user_data );
}


void
on_TrabLog_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
#ifdef SUN_OS
	ProgTrapLog ( main_w[(gint) user_data],   (gint) user_data );
#endif

#ifdef WINDOWS_OS
	ProgTrapLog ( main_w[(gint) user_data],   (gint) user_data );
#endif
}


void
on_AckpageAlm_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	AcknDspAlmFlag ( main_w[(gint) user_data],(gint) user_data);
}

void
Fault_redisplay_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
//#ifdef SUN_OS
	 PopupFaultRedisplayBox ( main_w[(gint) user_data],   (gint) user_data ); 
//#endif
}

void
End_faultRedisplay_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
//#ifdef SUN_OS
	 EndFaultRedisplay ( main_w[(gint) user_data],   (gint) user_data );  
//#endif
}

void
on_item61_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    /*  History Curve */
 
     ProgHisCurve (main_w[(gint) user_data],   (gint) user_data);
  
}


void
on_sftcp_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

	ProgSoftCopy ((gint) user_data );

}

void
on_Hardcpy_activate(GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

    PopupPrinterSetDlg(main_w[(gint) user_data],   (gint) user_data,0/*HARD_COPY in ipm_k_def.h*/);
}
void 
on_view_activate(GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	int crt_n  ;
	crt_n  =  (gint) user_data  ;  

printf  ("on_view_activate!!!!\n")  ; 	
	PopUpViewWindow(main_w[ crt_n],    crt_n)  ;  
    
}

void
on_item63_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}
void  /*实时曲线设置 */
on_item66_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    ProgRealConfig (main_w[(gint) user_data],   (gint) user_data);    

}

void  /*语言设置 */
on_item67_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
   
	PopupLanguageSetDlg (main_w[(gint) user_data],   (gint) user_data,0);
}

void
on_item64_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	SetCanvasBgColor ( (gint) user_data  );
}

void
on_ConfigAlmStn_activate                     (GtkMenuItem     *menuitem,
											   gpointer         user_data)
{
	AlmConfig_active((int) (user_data));
}


void
on_ConfigAlmInfo_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
         create_alm_info_config_window ((gint) user_data);
}

 
void
on_real_curve_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{


    ProgRealCurve (main_w[(gint) user_data],(gint) user_data);
  
}

void
on_operating_characteristic_curve_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	ProgOpCharacterCurve(main_w[GPOINTER_TO_INT(user_data)],GPOINTER_TO_INT(user_data));
}

void
on_gen_power_plan_curve_activate (GtkMenuItem     *menuitem,
												   gpointer         user_data)
{
	ProgGenPowerPlanCurve(main_w[GPOINTER_TO_INT(user_data)],GPOINTER_TO_INT(user_data));
}


void
on_SearchPt_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	PopEntryInputDlg(main_w[(gint) user_data],(gint) user_data);
}

void
on_SearchWin_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	PopSearchWin(main_w[(gint) user_data],(gint) user_data);
}


void
on_turn1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{ 	
        int crt_n;
        GetCrtNumberOfWgt (GTK_WIDGET(menuitem), &crt_n );
        selected_class[crt_n]= (int)user_data;
}

/***************画面数据点名显示函数*********************/

void 
on_itemDispEntry_activate (GtkMenuItem     *menuitem,
						   gpointer         user_data)
{ 	
	ProgDispEntryName (main_w[(gint) user_data],   (gint) user_data);
}

void
on_itemcheck_a01_activate(GtkMenuItem     *menuitem,
						  gpointer         user_data)
{ 	
	
	if (PopupMessageBoxRet(main_w[(gint) user_data],_CS_(MSG_EXCUTE_AUTOCHECK))==GTK_RESPONSE_CANCEL)
			return ; 

	ProgAutoCheck (main_w[(gint) user_data],   (gint) user_data);
}
void
on_itemcheck_opencheck_activate(GtkMenuItem     *menuitem,
						  gpointer         user_data)
{ 	
	
	Menu_OpenCHKFile(main_w[(gint)user_data],GINT_TO_POINTER (user_data));  
}

void
on_itemcheck_b_activate(GtkMenuItem     *menuitem,
						  gpointer         user_data)
{ 	
	ProgCondCheckTab(main_w[(gint) user_data],   (gint) user_data);
}