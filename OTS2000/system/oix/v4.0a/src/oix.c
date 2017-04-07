
/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong 
	LastUpadate : 2008.01.15
    WINDOWS HAVE SYN TO UNIX by chi.hailong  2007.11.23 ;   

---------------------------------------------------------------------*/

#define DEBUG    
#include <locale.h>
#include <gtk/gtk.h>

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtklabel.h>


#include	"../inc/oix.h"
#include	"../inc/gtk_oix_cstr.h"
#include	"../inc/gtk_widgets.h"
#include    "../../../pas/v4.0a/inc/trend_data_rec.h"

#pragma	comment(lib, "user32.lib")
#pragma	comment(lib, "gdi32.lib")
GtkWidget       *drawing_area[MAX_CRT_NUMBER];
SHM_RECORD	*shm_rec_addr;
CURVE_SHM_DATA *shm_real_curve; 
FAULT_RECORD_TABLE *shm_fault_addr;
SYS_COMM_TABLE  *shm_syscomm_addr;
INFO_RECORD		*shm_info_rec_addr;   //added by chi 070911
TAG_DISP         tag_display[8];    //ÓÃÓÚÏÔÊ¾Ä¸ÏßµçÑ¹±êÇ©£»
int   tagNum = 0 ; 
int dsp_rec_ptr;
int print_delay_time;
BOOL isExLang =FALSE   ; 
int SelectLayer[MAX_CRT_NUMBER][LAYERNUM];
int  iSetDispEntry[MAX_CRT_NUMBER]={0,0,0} ;

BOOL bRealiseFinished[MAX_CRT_NUMBER][2];	//add zlb

char *demo_dsp_name[] ={DEMO_DSP_NAME,DEMO_DSP_NAME_EX};
extern DEMO_DSP_CLASS demo_dsp_class[10];
extern  char   OIX_FAULTTAB_NO_VISIBLE  ;    
extern  char   OIX_DSP_TAG ; 
extern  GtkWidget	*ctrl_menu_shell[MAX_CRT_NUMBER];
extern GtkWidget 	*alm_pop_shell[MAX_CRT_NUMBER];
extern void    OixReadConfig()  ; 
extern int ReadCommonDirXml(GtkMenu *menuCommonDir,int crt_n);
GdkScreen *second_screen[MAX_CRT_NUMBER] ;
extern DB_TAB_WINDOW_INFO db_tab_info_tmp[MAX_CRT_NUMBER];
/*  All message process  following !!!!  */
extern  void  motion_notify_callback(GtkWidget* ,GdkEventMotion*,   char*);
extern	void  expose_events(GtkWidget *,GdkEventExpose *,  gpointer );
extern void canvas_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data);
extern gint button_press( GtkWidget *widget,GdkEvent *event , gpointer data);
extern gint button_release( GtkWidget *widget,GdkEvent *event , gpointer data);
extern void GetSelectedPrinter(GtkWidget *w,gpointer data);
extern void  InitPrinterName();
extern  void Menu_Login(GtkWidget *w,gpointer data);
extern  void Menu_Logout(GtkWidget *w,gpointer data);
extern  void Menu_ChgPasswd(GtkWidget *w,gpointer data);
extern  void Menu_DspLogInfo(GtkWidget *w,gpointer data);
extern	void Menu_OpenFile(GtkWidget *w,gpointer data);
extern  void Menu_Rpt(GtkWidget *w,gpointer data);
extern  int CreateFaultAlarm(int crt_n ,GtkWidget *DrawArea,GdkGC  *gc  );
extern void  fault_alarm_expose(GtkWidget *w ,  GdkEventExpose *event, gpointer data);

extern  void ProgTrapLog ( int crt_n ); 
extern void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );
extern void GetObjDlg(int,int,char*);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern int OpenDspFile (int crt_n, char	*filename);
extern void	ZoomIn (int	crt_n, GdkPoint  *event );
extern void	ZoomOut (int crt_n, GdkPoint  *event );
extern void	ProgHardCopy(int crt_n );
extern int	SXtoX (int crt_n,int sx);
extern int	SYtoY (int crt_n,int sy);
extern char* GetActCtrlItemDbname (int crt_n, int x1,int y1,int x2,int y2 );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern GtkWidget* PopupDetailInfoBox(GtkWidget *parent,int crt_n,POINTER pt);
extern void 	CmdBufInit ( int crt_n );
extern void 	CmdBufInit ( int crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern BOOL OpenSubDspFile (int crt_n, char *filename);
extern void 	ProgAlarmTab (GtkWidget *parent,	int crt_n  ,int TabMode  );
extern void		ProgKeySignalTab(GtkWidget *parent,	int crt_n  ,int TabMode);
//extern void 	ProgAlarmTab_new (GtkWidget *parent,	int crt_n  ,int TabMode  );
extern void 	ProgUnresetAlarmTab (GtkWidget *parent,	int crt_n   );
extern void 	ProgDbTab (GtkWidget *parent, int crt_n );
extern void PopSearchWin(GtkWidget *parent,int crt_n);
extern GtkWidget *create_pixmap  (GtkWidget   *widget, const gchar *filename);
extern void add_pixmap_directory (const gchar     *directory);
extern void ReadDemoDspName ( char	*full_name );
extern void	CreateFontX();
extern void	LoadStringGrpLibrary ();
extern void	LoadSymbolLibrary ();
extern void	LoadSymbolGrpLibrary ();
extern void	LoadColorGrpLibrary ();
extern int    InitRealCCurvePt();
extern void	LoadMenuLibrary ();
extern void	ZoomInit (int crt_n );
extern void GifInit (int crt_n );
extern void	CreateFillPalette (int crt_n);
extern void	OixInit (int crt_n );
extern int	ReadColor(int crt_n );
extern int PauseDemoDsp(GtkWidget*w,int crt_n);
extern void OpenNextDemoDsp(GtkWidget*w,int crt_n);

extern void zoom_scroll_event(GtkWidget*widget,   GdkEventScroll *event, gpointer  data)  ;
extern void	ProgSoftCopy (int crt_n );
extern int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] ); 
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern gint GetGroupDef(void);
extern void DspRealtePtInfo();
extern int ptEventProg(GtkWidget *parent,int crt_n,POINTER pt);
extern GtkWidget *PopupPtCheckBox(GtkWidget *parent,int crt_n,POINTER pt);
extern int ProgDoTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm);
extern int ProgOnTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm);
extern int ProgOffTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm);
extern int readOpCharacterCurveData();		//»ú×é×´Ì¬ÇúÏß
extern void ProgPopMenuRealCurve (GtkWidget *parent, POINTER ptr ,int  crt_n);	//ÓÒ¼ü²Ëµ¥ÊµÊ±ÇúÏß
extern void InitRealCurvePt();
extern POINTER GetPointByPressedPos(int crt_n);

void gtk_main_win_quit(  );

#if defined(SUN_OS)
#define DEMO_DSP_NAME  "/home/ems/h9000/def/demo_dsp_name.def"
#endif
int SCREEN_WIDTH_MOD[MAX_DSP_MOD]={800 ,960,1024,1280,1360,1400,1440,1600,1680,1920};

GtkWidget *ana_curve_dlg[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];


char	local_hostname[MAX_HOST_NAME_SIZE];
char	cur_usr_name[MAX_USER_NAME_LEN];
BOOL SoftCopyFlag;/*true softcopy button useless,false softcopy button usefull*/
extern gint num_screen;
extern RelatePoint			*relatepoint;
short oix_login_status=FALSE;
gint auto_login_timer;

int   read_tag_ply_conf()
{
	FILE			*fp;
	
	unsigned short	j;
	
	char    note[128] ;
	
    char	buffer[256];
	
	
	
	if((fp=fopen(OIX_TAG_DEF_FILE,"r"))==NULL)
	{
		printf("can t open oix def file %s\n",OIX_TAG_DEF_FILE);
		return(0);          
	}
	
	rewind(fp);	
	j =0 ; 
	while( 1 )
	{ 
		if(fgets(buffer, sizeof(buffer), fp) == NULL)
			break;
		
		
        if(buffer[0]==0 || buffer[0]==0x0d || buffer[0]==0x0a) /*¿ÕÐÐ¡¢»»ÐÐ·û¡¢»Ø³µ·û*/
			continue ;
		
		if(buffer[0]=='#' || buffer[0]=='!') /*#ºÍ!±íÊ¾×¢ÊÍ*/
			continue; 
        sscanf(buffer , "%s   %s    " ,&tag_display[j].cName ,&tag_display[j].logName,&note );
		if (strcmp(tag_display[j].cName,"NULL") ==0 )
			strcpy(tag_display[j].cName,"");
		else 
			strcat(tag_display[j].cName,":");
        j++; 
		
		
	}
	
	
	fclose(fp);
	
	return j; 
	
}



/* Obligatory basic callback */
static void print_hello( GtkWidget *w,
                         gpointer   data )
{
printf ("widget is %x :\n" ,w);
printf ("data is %d :\n" ,(gint)data);
  g_message ("Hello, World!\n");
}

static void pic_proc(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{    
    int crt_n ; 
	//GetCrtNumberOfWgt (menu_item, &crt_n);  
	crt_n=(gint)callback_data;
  	if( crt_n < 0 )
  		return; 
 
   switch((gint)callback_action )
    {
                case 0:
			 
			 Menu_OpenFile(main_w[crt_n],GINT_TO_POINTER(crt_n));  

                    break;
		case 1:  /* usally pic */
			
			//OpenDspFile ( crt_n, "YC_HMSY.dbin");
 			OpenDspFile ( crt_n, _CS_(commonDirName));
                    break;
		case 2:  /* main  toplogy*/
			
			 //OpenDspFile ( crt_n, "GIS_ZJX.dbin");
			OpenDspFile ( crt_n, _CS_(zjxName));
                    break;  
                    
           
     
    }
}

/* For the check button of popup menu */

static void zoom_proc(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{    
    int crt_n ; 
	//GetCrtNumberOfWgt (menu_item, &crt_n);  
	crt_n=(gint)callback_data;
  	if( crt_n < 0 )
  		return; 
   g_message ("crt_n is  - %d\n", crt_n) ;
   if ((gint)callback_action == 0)
       {
       ZoomIn (crt_n , &info[crt_n].pt )  ;     
   }
   else
       {
       ZoomOut (crt_n , &info[crt_n].pt )  ;     
    }
}

static void Print_proc(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{    
        int crt_n ; 
     //GetCrtNumberOfWgt (menu_item, &crt_n);  
	 crt_n=(gint)callback_data;

  	if( crt_n < 0 )
  		return; 
   
	if(!CheckUserPrivilege(cur_usr_name, PRIV_PRINT)) //ï¿½Ð¶Ï´ï¿½Ó¡È¨ï¿½ï¿½
	{
		PopupMessageBox ( main_w[crt_n], MSG_RPT_PRINTING  ); 
		return  ;
	}

	/*ï¿½Ò¼ï¿½Ëµï¿½ï¿½ï¿½Ê§ï¿½ï¿½ï¿½Ó¡*/
	print_delay_time=gtk_timeout_add(800, (GtkFunction)ProgHardCopy,(gpointer)crt_n);
	//ProgHardCopy( crt_n );
    

}
gint timeout_callback( gpointer data )
{
	   return FALSE;
}
static void ptCheck_proc(gpointer   callback_data,
						 guint      callback_action,
	   GtkWidget *menu_item)
{
	DMS_COMMON	 dms_cmm;
	   int crt_n  ,	xtmp, ytmp;
	   int	width, height;
	   char   db_name[60];
	   //GetCrtNumberOfWgt (menu_item, &crt_n);  
	   crt_n=(gint)callback_data;
	   memset(db_name,0, sizeof(db_name) ); 
	   
	     	if( crt_n < 0 )
				return; 
			
			cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
			cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
			cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
			cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
			
			CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
			if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
				strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
			else
			{
				PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
				return ;
			}
			
			if(db_name[0])
			{
				if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
				{
					PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
					return  ;
				}
				//	PopupPtEventdlg(main_w[crt_n], crt_n,dms_cmm.point);                  
				PopupPtCheckBox(main_w[crt_n], crt_n,dms_cmm.point);                  
			}        
			
      return ;
}

static void ptSwitchScene_proc_evt(gpointer   callback_data,
						 guint      callback_action,
						 GtkWidget *menu_item)
{
	DMS_COMMON	 dms_cmm;
	   int crt_n  ,	xtmp, ytmp;
	   int	width, height;
	   char   db_name[60];
	   char msg[SMS_MESSAGE_SIZE];
	   //GetCrtNumberOfWgt (menu_item, &crt_n);  
	   crt_n=(gint)callback_data;
	   memset(db_name,0, sizeof(db_name) ); 
	   
	     	if( crt_n < 0 )
				return; 
			
			cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
			cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
			cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
			cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
			
			CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
			if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
				strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
			else
			{
				PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
				return ;
			}
			
			if(db_name[0])
			{
				if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
				{
					PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
					return  ;
				}
				//	PopupPtEventdlg(main_w[crt_n], crt_n,dms_cmm.point);  
				sprintf(msg,"switch:%d.%d.%d.%d",dms_cmm.point.stn_id,dms_cmm.point.dev_id,dms_cmm.point.data_type,dms_cmm.point.pt_id);
				if(strlen(msg)>SMS_MESSAGE_SIZE)
					msg[SMS_MESSAGE_SIZE-1]='\0';
				BdSms(msg);
			}        
			
			return ;
}
static void ptevent_proc(gpointer   callback_data,
	   guint      callback_action,
	   GtkWidget *menu_item)
{    
	   DMS_COMMON	 dms_cmm;
	   int crt_n  ,	xtmp, ytmp;
	   int	width, height;
	   char   db_name[60];
	   //GetCrtNumberOfWgt (menu_item, &crt_n);  
	   crt_n=(gint)callback_data;
	   memset(db_name,0, sizeof(db_name) ); 

	     	if( crt_n < 0 )
				return; 
			
			cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
			cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
			cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
			cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
			
			CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
			if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
				strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
			else
			{
				PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
				return ;
			}
			
			if(db_name[0])
			{
				if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
				{
					PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
					return  ;
				}
			//	PopupPtEventdlg(main_w[crt_n], crt_n,dms_cmm.point);                  
				 ptEventProg(main_w[crt_n], crt_n,dms_cmm.point);                  
			}        
			
      return ;
}
static void ptInfo_proc(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{    
    DMS_COMMON	 dms_cmm;
    int crt_n  ,	xtmp, ytmp;
     int	width, height;
     char   db_name[60];
     //GetCrtNumberOfWgt (menu_item, &crt_n);  
	 crt_n=(gint)callback_data;
     memset(db_name,0, sizeof(db_name) ); 
  	if( crt_n < 0 )
  		return; 
  
        cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
        cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
        cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
        cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
        
        CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
		if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
			strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
        else
		{
			PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
            return ;
        }
        
		if(db_name[0])
        {
			if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
			{
				PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
				return  ;
			}
			PopupDetailInfoBox(main_w[crt_n], crt_n,dms_cmm.point);                  
        }        
        
      return ;         
}

static void ptDoTag_proc(gpointer   callback_data,
						guint      callback_action,
						GtkWidget *menu_item)
{    
    DMS_COMMON	 dms_cmm;
    int crt_n  ,	xtmp, ytmp;
	int	width, height;
	char   db_name[60];
	//GetCrtNumberOfWgt (menu_item, &crt_n);  
	crt_n=(gint)callback_data;
	memset(db_name,0, sizeof(db_name) ); 
	if( crt_n < 0 )
		return; 
	
	cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
	cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
	cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
	cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
	
	CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
	if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
		strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
		return ;
	}
	
	if(db_name[0])
	{
		if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
		{
			PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
			return  ;
		}
		ProgDoTag(main_w[crt_n], crt_n,dms_cmm);  
		 
	}        
	
	return ;         
}
static void ptOnTag_proc(gpointer   callback_data,
						 guint      callback_action,
						 GtkWidget *menu_item)
{    
    DMS_COMMON	 dms_cmm;
    int crt_n  ,	xtmp, ytmp;
	int	width, height;
	char   db_name[60];
	//GetCrtNumberOfWgt (menu_item, &crt_n);  
	crt_n=(gint)callback_data;
	memset(db_name,0, sizeof(db_name) ); 
	if( crt_n < 0 )
		return; 
	
	cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
	cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
	cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
	cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
	
	CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
	if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
		strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
		return ;
	}
	
	if(db_name[0])
	{
		if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
		{
			PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
			return  ;
		}
		ProgOnTag(main_w[crt_n], crt_n,dms_cmm);  
		
	}        
	
	return ;         
}
static void ptOffTag_proc(gpointer   callback_data,
						 guint      callback_action,
						 GtkWidget *menu_item)
{    
    DMS_COMMON	 dms_cmm;
    int crt_n  ,	xtmp, ytmp;
	int	width, height;
	char   db_name[60];
	//GetCrtNumberOfWgt (menu_item, &crt_n);  
	crt_n=(gint)callback_data;
	memset(db_name,0, sizeof(db_name) ); 
	if( crt_n < 0 )
		return; 
	
	cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
	cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
	cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
	cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
	
	CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
	if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
		strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
		return ;
	}
	
	if(db_name[0])
	{
		if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
		{
			PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
			return  ;
		}
		ProgOffTag(main_w[crt_n], crt_n,dms_cmm);  
		
	}        
	
	return ;         
}

static void ptInfo_proc_evt(gpointer   callback_data,
						guint      callback_action,
						GtkWidget *menu_item)
{    
    int crt_n  ;
	char   db_name[60];
	//GetCrtNumberOfWgt (menu_item, &crt_n);  
	crt_n=(gint)callback_data;
	memset(db_name,0, sizeof(db_name) ); 
	if( crt_n < 0 )
		return; 
	PopupDetailInfoBox(alm_pop_shell[crt_n ], crt_n, db_tab_info_tmp[crt_n].select_pt);
    
	
	return ;         
}
static void ptPic_proc_evt(gpointer   callback_data,
							guint      callback_action,
							GtkWidget *menu_item)
{    
    DMS_COMMON	 dms_cmm;
    int crt_n  ;
//	int	width, height;
	char   db_name[60];
	char dspname[200];
	//GetCrtNumberOfWgt (menu_item, &crt_n);  
	crt_n=(gint)callback_data;
	memset(db_name,0, sizeof(db_name) ); 
	if( crt_n < 0 )
		return; 

	if ( ReadEntryById (  &db_tab_info_tmp[crt_n].select_pt, &dms_cmm ) == -1 )
	{
//		PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
		return  ;
			}


	sprintf(dspname,"%s",dms_cmm.fixed.dsp_filename);
	
    if (num_screen>1);
    {
		crt_n=num_screen-1;
		
    }
	OpenDspFile ( crt_n, dspname);
    
	
	return ;         
}

static void ptrealcurve_proc_evt(gpointer   callback_data,
							  guint      callback_action,
						GtkWidget *menu_item)
{
	int crt_n ;
	POINTER ptr;

	crt_n=(gint)callback_data;

	if( crt_n < 0 )
		return; 

	ptr=GetPointByPressedPos(crt_n);
	ProgPopMenuRealCurve(main_w[crt_n], ptr, crt_n);
}

static void ptrelate_proc_evt(gpointer   callback_data,
						guint      callback_action,
						GtkWidget *menu_item)
{    
    int crt_n ;
	char   db_name[60];
	int relate_idx=-1;
	crt_n=(gint)callback_data;
	memset(db_name,0, sizeof(db_name) ); 
	if( crt_n < 0 )
		return; 

	GetPtNameById( &db_tab_info_tmp[crt_n].select_pt, db_name,NULL );
	relate_idx=RelatePoint_Judge(db_name);

	if (relate_idx<0||relate_idx>RELATEPOINT_NUM)
	{
		printf("This Point have no relatepoint\n");
		return;
	}
	else
		DspRealtePtInfo(crt_n,relate_idx);
		
	return ;         
}

extern POINTER		pnt_tab_ptr[MAX_CRT_NUMBER];

static void ptAlarmAck(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{
        DMS_COMMON	 dms_cmm;
        int crt_n  ,	xtmp, ytmp;
        int	width, height;
        char   db_name[60];
        
 /*GetCrtNumberOfWgt (menu_item, &crt_n);  Modified by zyp on 2008.1.4*/
		crt_n =(int) callback_data;
        if( crt_n < 0 )
  		return; 
        
		if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
		{
			PopupMessageBox ( main_w[crt_n], MSG_ALARM_ACK );
			return;
		}        
        
        memset(db_name,0, sizeof(db_name) );  	
  
        cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));
        cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));
        cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
        cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
        
        CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
        if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
           strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
        else
        {
                PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
                return ;
        }        
         if(strcmp(db_name,"")!=0)
        {
                if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
		{
			PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
                        return  ;
		}
                CmdBufInit ( crt_n );
		cmd_buf[crt_n].stn_id		= dms_cmm.point.stn_id;
        	cmd_buf[crt_n].dev_id		= dms_cmm.point.dev_id;
		cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;
		cmd_buf[crt_n].data_type	= dms_cmm.point.data_type;
		cmd_buf[crt_n].type_id		= DPS_IOST_SET;  
		cmd_buf[crt_n].status.state = ACKN;
		cmd_buf[crt_n].status.opr  = IOST_SET_ALM_NOACK;

		ProgDoAckCelBrdcst ( crt_n );                
        }  
      return ;               
}

static void consoleOut
(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{   
#ifdef  SUN_OS
	char  sysName[128]; 
    int crt_n ; 
        GetCrtNumberOfWgt (menu_item, &crt_n);  
  	if( crt_n < 0 )
  		return; 
	if(CheckUserPrivilege(cur_usr_name, PRIV_SYS_MAINT))
		{
		sprintf (sysName,"/usr/dt/bin/dtterm -display  %s:0.%d&",local_hostname,crt_n) ;
		system( sysName);
	}
	else
		PopupMessageBox ( main_w[crt_n], MSG_SYS_MAINT );
#endif
}

static void  ptObjInfo_proc(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{    
    DMS_COMMON	 dms_cmm;
    int crt_n  ,i,	xtmp, ytmp;
     int	width, height;
     char   db_name[60];
     POINTER tmp_ptr[MAX_CRT_NUMBER];
     memset(db_name,0, sizeof
     (db_name) );
     //GetCrtNumberOfWgt (menu_item, &crt_n);  
	 crt_n=(gint)callback_data;
  	if( crt_n < 0 )
  		return; 
  
        cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));
        cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));
        cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
        cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
        
        CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
        if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
           strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
        else
            {PopupMessageBox(main_w[crt_n],MSG_DBNAME_IS_NULL);
            return ;
        }
                 
        if(strcmp(db_name,"")!=0)
        {
                if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
			{
				PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);   
                                return  ;
			}
                 if ( -1 !=GetPtIdByNameStr(db_name  ,&tmp_ptr[crt_n] ))
                     {  
                         if(tmp_ptr[crt_n].data_type!=DEVOBJ_TYPE)
                         { 
                             for(i=0;i<MAX_SUB_WIN_NUM;i++)
                             {
                                     if(ana_curve_dlg[i][crt_n]!=NULL) 
                                     {
                                         
                                        if(strcmp(db_name,gtk_window_get_title(GTK_WINDOW(ana_curve_dlg[i][crt_n])))==0)
                                        {
                                            gtk_window_present(GTK_WINDOW(ana_curve_dlg[i][crt_n])); 
                                            break;
                                        }
                                     }
                             }
                              if(i<MAX_SUB_WIN_NUM)/* sub_win existed */
                                    return;
                             for(i=0;i<MAX_SUB_WIN_NUM;i++)
                             {
                                 if(ana_curve_dlg[i][crt_n]==NULL)                                
                                    break;                                 
                             }
                             if(i<MAX_SUB_WIN_NUM)
                                GetObjDlg(crt_n,i,db_name); 
                             else
                                 PopupMessageBox(main_w[crt_n],MSG_TOO_MUCH_SUB_WIN);
                         }
                         else
                         {
                            pnt_tab_ptr[crt_n] = tmp_ptr[crt_n];
                            strcpy(sub_dsp_name,db_name);
                            if(True!=OpenSubDspFile ( crt_n, "io_obj_point_data.sbin"))
                                   return ;/* PopupMessageBox( main_w[crt_n], MSG_SBIN_FILE_NOT_FOUND );*/
                            else                                    
                                file_mode[crt_n]=SUB_DSP_FILE;  
                         }
                 }
        }    
		
		return ;
        
}



               
 
static void print_toggle(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{
   g_message ("Check button state - %d\n",
              GTK_CHECK_MENU_ITEM(menu_item)->active);
printf ("widget is %x :\n" ,menu_item);
}

/* For the radio buttons */
static void print_selected(gpointer   callback_data,
                           guint      callback_action,
                           GtkWidget *menu_item)
{
   if(GTK_CHECK_MENU_ITEM(menu_item)->active)
     g_message("Radio button %d selected\n", callback_action);
printf ("widget is %x :\n" ,menu_item);
}


 #include "../inc/popmenu.h" 
/*static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);
static gint nmenu_items_obj_disable = sizeof (menu_items_obj_disable) / sizeof (menu_items_obj_disable[0]);
 */
extern  void ChangeDspMode ();
extern	GtkWidget* create_main_menu (gint);
gint timer ;
gint num_screen;
char *OpenDspFiles[MAX_CRT_NUM]={"","",""} ;



GtkWidget *create_popup_menu(int crt_n)
{
   GtkItemFactory *item_factory;
   
   GtkWidget  *menu;
  static gint nmenu_items;/*Modified by zyp on 2007.8*/
  
	
   /* Same as before but don't bother with the accelerators */
   item_factory = gtk_item_factory_new (GTK_TYPE_MENU, "<main>",
                                        NULL);
   if(IsDevObjDisable()==True)
   {

	   nmenu_items =sizeof (menu_items_obj_disable) / sizeof (menu_items_obj_disable[0]);
	   gtk_item_factory_create_items (item_factory, nmenu_items, menu_items_obj_disable,(gpointer) crt_n);//hcl
  	
	
   }
   else
   {
	   nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);
	   
	   gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, (gpointer)crt_n); //hcl
  	
  }
   
   menu = gtk_item_factory_get_widget(item_factory, "<main>");
printf ("menu is %x \n ", menu);
  
    gtk_menu_set_screen(GTK_MENU(menu), second_screen[crt_n]) ;
    gtk_widget_show(menu ) ;
    return  menu ;


}
GtkWidget *create_evt_popup_menu(int crt_n)
{
   GtkItemFactory *item_factory;
   
   GtkWidget  *menu;
   static gint nmenu_items;/*Modified by zyp on 2007.8*/
   
   
   /* Same as before but don't bother with the accelerators */
   item_factory = gtk_item_factory_new (GTK_TYPE_MENU, "<main>",
	   NULL);
   
	   
 nmenu_items =sizeof (menu_items_evt) / sizeof (menu_items_evt[0]);
	   gtk_item_factory_create_items (item_factory, nmenu_items, menu_items_evt,(gpointer) crt_n);//hcl
	   
	   
   

   
   menu = gtk_item_factory_get_widget(item_factory, "<main>");
   printf ("menu is %x \n ", menu);
   
   gtk_menu_set_screen(GTK_MENU(menu), second_screen[crt_n]) ;
   gtk_widget_show(menu ) ;
   return  menu ;
   
   
}

int  get_tag_value (int i)
{
	if (GetAnaPtfValueByNameStr( tag_display[i].logName , &tag_display[i].value  )==-1)
		tag_display[i].value = 0 ; 
	if  (GetAnaPtUnitNameByNameStr( tag_display[i].logName, tag_display[i].unitName )==-1)
		strcpy(tag_display[i].unitName,"") ; 
	return 1 ;
	
}
gint ticktick(gpointer data)
{
    time_t  newtime;
    struct  tm      devtime; 
 
    int i ;  GtkWidget *lblTimer,*lblDisplay ;
	char timer[30],display[128];

	gchar *pConvert=0;

    newtime = time(NULL);
    devtime = *localtime( &newtime);
    
    sprintf (timer , "%4d-%02d-%02d %02d:%02d:%02d  " ,devtime.tm_year+1900,
                    devtime.tm_mon+1 ,devtime.tm_mday , devtime.tm_hour, devtime.tm_min ,devtime.tm_sec) ;
if (OIX_DSP_TAG)
{

	for (i = 0 ;i<4 ; i++)	
		get_tag_value (i);
	
	sprintf (display , "%s%5.1f%s  %s%5.2f%s  %s%5.1f%s  %s%5.2f%s" ,
		tag_display[0].cName , tag_display[0].value, tag_display[0].unitName  ,
		tag_display[1].cName , tag_display[1].value, tag_display[1].unitName  ,
		tag_display[2].cName , tag_display[2].value, tag_display[2].unitName  ,
		tag_display[3].cName , tag_display[3].value, tag_display[3].unitName) ;
}
    for (i = 0 ; i < num_screen ;i++ )
        {
         lblTimer = g_object_get_data(G_OBJECT(main_w[i]),"lblTimer");
         if (lblTimer != NULL)
                gtk_label_set_text(GTK_LABEL(lblTimer),timer);

		 lblDisplay = g_object_get_data(G_OBJECT(main_w[i]),"lblDisplay");
         if (lblDisplay != NULL&& OIX_DSP_TAG)
		 {
			 gtk_label_set_text(GTK_LABEL(lblDisplay), pConvert = _toUtf8(display));
			 G_SAFE_FREE(pConvert);
		 }
         if(ctrl_menu_shell[i])
            gtk_window_present(GTK_WINDOW(ctrl_menu_shell[i]));
// 		 if (GTK_IS_WINDOW(alm_pop_shell[i]))
// 			 gtk_window_present(GTK_WINDOW(alm_pop_shell[i]));

    }

return TRUE;    
} 

static void tool_button_cb(GtkWidget *widget, gpointer data)
{   	gint button_id, crt_n;
        button_id =  (gint) data ;
	printf("%d\n", button_id );
       GetCrtNumberOfWgt(widget,&crt_n);  
     if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	switch (button_id) 
	{
		case 0:
			 
			 ProgAlarmTab (main_w[crt_n],crt_n ,FALSE);

                    break;
		case 1:
			 
                        ProgUnresetAlarmTab( main_w[crt_n],   crt_n );
                    break;
		case 2:


	
			 ProgDbTab ( main_w[crt_n], crt_n  );
			
                    break;  
        case 3:
			 OpenDspFile ( crt_n,_CS_(commonDirName));   //"YC_HMSY.dbin"
			 	
                    break; 
        case 4:
/*#ifdef  SUN_OS*/
				   if(SoftCopyFlag==FALSE)
			 ProgSoftCopy ( crt_n );
				   else
					   PopupMessageBox(main_w[crt_n],MSG_SOFTCOPY_DIS);

					   
/*#endif*/
			 
/*#ifdef  WINDOWS_OS

			 PopSearchWin(main_w[crt_n],crt_n);
#endif*/
                    break; 


				case 5:
			ProgKeySignalTab( main_w[crt_n], crt_n,FALSE);
				break;

				case 6:
			 ProgAlarmTab ( main_w[crt_n], crt_n,FALSE  );
			
                    break;  

				case 7:
			 ProgUnresetAlarmTab( main_w[crt_n],   crt_n );
			
                    break;  
	}
 	
	
 	
}
gint ChgCommonDir(GtkWidget *widget,   gpointer data)
{
	char *curIndex;
	GtkWidget *child;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;

	if (GTK_BIN (widget)->child)
	{
		child = GTK_BIN (widget)->child;
		if (GTK_IS_LABEL (child))
		{
			gtk_label_get (GTK_LABEL (child), &curIndex);
			OpenDspFile ( crt_n,_CS_(curIndex));
		}
	}
}

static GtkWidget* new_pixmap(char *filename ,GdkWindow *window, GdkColor *background)
{
    
    GdkPixmap *pmap;
    GdkBitmap *mask;
    
    pmap = gdk_pixmap_create_from_xpm(window,&mask,background,filename);
    
    return(gtk_pixmap_new(pmap,mask)) ;

}

static GtkWidget* make_toolbar(GtkWidget *window, int crt_n)
{
	GtkWidget *toolbar;
	GtkWidget *event_box;
	gchar *pConvert=0, *pConvert1=0;
	GtkWidget *optionmenu,*menu;
	toolbar = gtk_toolbar_new();
	
	gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), pConvert=_toUtf8EX(OIX_CSTR_ALARM_TAB),
		pConvert1=_toUtf8EX(OIX_CSTR_ALARM_TAB) ,NULL, NULL, 		(GtkSignalFunc)tool_button_cb ,0);
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);

	gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), pConvert=_toUtf8EX(OIX_CSTR_KEYSIGNAL_TAB),
		pConvert1=_toUtf8EX(OIX_CSTR_KEYSIGNAL_TAB) ,NULL, NULL, (GtkSignalFunc)tool_button_cb ,GINT_TO_POINTER(5));
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);

	gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), pConvert=_toUtf8EX(OIX_CSTR_FAULT_TAB),
		pConvert1=_toUtf8EX(OIX_CSTR_FAULT_TAB) , NULL,NULL, (GtkSignalFunc)tool_button_cb,GINT_TO_POINTER(1))  ;
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);

	/************************************************************************/
	/*     ("ï¿½Ûºï¿½ï¿½ï¿½Ï¢ï¿½ï¿½")   ("ï¿½Ûºï¿½ï¿½ï¿½Ï¢ï¿½ï¿½") delete by hcl for zhangweijun                                                               */
	/************************************************************************/


	gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), pConvert=_toUtf8EX(OIX_CSTR_Title),
		pConvert1=_toUtf8EX(OIX_CSTR_Title) ,NULL,NULL,		(GtkSignalFunc)tool_button_cb, GINT_TO_POINTER(2));  
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);
		
	if (!xml_comdir_btn)
	{
	gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), pConvert=_toUtf8EX(OIX_CSTR_GEN_INDEX),
		pConvert1=_toUtf8EX(OIX_CSTR_GEN_INDEX) , NULL,NULL, (GtkSignalFunc)tool_button_cb,GINT_TO_POINTER(3))  ;
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);
	}else{
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	ReadCommonDirXml(GTK_MENU(menu),crt_n);
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_container_add (GTK_CONTAINER (toolbar), optionmenu);
	}
    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),  pConvert=_toUtf8EX(OIX_CSTR_DSP_SFTCOPY),
		 pConvert1=_toUtf8EX(OIX_CSTR_DSP_SFTCOPY) , NULL,NULL,	(GtkSignalFunc)tool_button_cb,GINT_TO_POINTER(4))  ;
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);

	

 	return toolbar ;  		
}




/*******************************************************/
void get_main_menu( GtkWidget  *window,
                    GtkWidget **menubar )
{
/*
    GtkItemFactory *item_factory;
    GtkAccelGroup *accel_group;*/
  

  
}

void 
draw_area_realize(GtkWidget *widget,gpointer data)
{
	
   gint crt_n ;
   crt_n = (gint)data;
 
    canvas[crt_n] = widget ;
    
    info[crt_n].gc =  gdk_gc_new(widget->window);
    
   gdk_window_set_background(widget->window,&pixels[crt_n][BLACK4]); 	

   bRealiseFinished[crt_n][0]=TRUE;		//add zlb
}
void 
draw_area2_realize(GtkWidget *widget,gpointer data)
{
	
 
 
 
   gint crt_n ;
   crt_n = (gint)data;
    
  canvas2[crt_n] = widget ;
    info[crt_n].gc2 =  gdk_gc_new(widget->window);  
  
   gdk_window_set_background(widget->window,&pixels[0][BLACK4]); 
   
   bRealiseFinished[crt_n][1]=TRUE;		//add zlb
}




static void menuitem_response(GtkWidget*w , gchar *string )
{
    printf ("%s\n", string);
}



GtkWidget* CreateMainWindow(gint crt_n,gint isDebug)
{
    GtkWidget *main_vbox,*lblDisplay;
    GtkWidget *menubar;
    GtkWidget *main_window;  
    GtkWidget  *vbox1,*hbox1,*hbox2;
    GtkImage  *image1 ;
	GtkWidget *vpaned1 ;
	GtkWidget *frame1  ;
	GtkWidget *drawingarea2 ;
	PangoFontDescription *font1;
	GdkColor  clr  ; 	
	GtkWidget  *handleBox,*hboxN ,*sep,*cur_usr_label,*cur_filelab,*fault_redisplay,*next_dsp_btn,*pause_demo_btn,*step_btn,*pause_btn,*lblTimer;
	GtkWidget *oix_toolbar;	
	gchar *pConvert=0;
	gint root_x,root_y;

	if (isDebug)
        main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);  
	else
		main_window = gtk_window_new (GTK_WINDOW_POPUP);  
	
	
	gtk_signal_connect (GTK_OBJECT (main_window), "destroy", 
		      GTK_SIGNAL_FUNC (gtk_main_win_quit), 
			  "WM destroy");
	gtk_window_set_title (GTK_WINDOW(main_window), pConvert=_toUtf8EX( OIX_STR_OIX_TITLE));
	G_SAFE_FREE(pConvert);
	
	gtk_window_set_default_size(GTK_WINDOW(main_window),  LOCAL_WIDTH  ,LOCAL_HEIGHT) ;
	
#ifdef WINDOWS_OS
	//hcl
	root_x=LOCAL_WIDTH*crt_n;
    root_y=0;
	gtk_window_move (GTK_WINDOW(main_window), root_x, root_y);
    gtk_window_get_position (GTK_WINDOW(main_window),&root_x,&root_y);
	
#endif
	
	
	
	
	/*   if you want move the windows to a certain place , you must first call funcion realize  */
	
	gtk_container_border_width(GTK_CONTAINER(main_window),0); 
	
	main_vbox = gtk_vbox_new (FALSE, 0);
	
	gtk_container_border_width (GTK_CONTAINER (main_vbox), 0);
	gtk_container_add (GTK_CONTAINER (main_window), main_vbox);
	
	
	
	gtk_widget_show (main_vbox);
	
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (main_vbox), hbox1,FALSE, TRUE, 0);
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox1,TRUE, TRUE, 0);
	
    hbox2  = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox2,FALSE, TRUE, 0);
	
	menubar =  create_main_menu(crt_n); 
	gtk_box_pack_start (GTK_BOX (hbox2), menubar,FALSE, FALSE, 0);
	gtk_widget_show (menubar); 
	/*************** TTTTTTTTT*****************************************/
	lblDisplay = gtk_label_new(	pConvert=_toUtf8("")	);
	G_SAFE_FREE(pConvert);
	gtk_widget_show (lblDisplay);
	gtk_label_set_justify (GTK_LABEL (lblDisplay), GTK_JUSTIFY_RIGHT);
	
	gtk_widget_set_usize(GTK_WIDGET(lblDisplay),-1, -1); 
	g_object_set_data(G_OBJECT(main_window),"lblDisplay",lblDisplay );
    gtk_box_pack_end (GTK_BOX (hbox2), lblDisplay,TRUE, TRUE, 0);
	
	/*************** Toolbar  code here !!! **************************/  
	
	
	handleBox = gtk_handle_box_new ();
	
	gtk_box_pack_start (GTK_BOX ( vbox1), handleBox,FALSE, FALSE, 0);
	gtk_widget_show (handleBox);  
	
	image1 = GTK_IMAGE(create_pixmap (main_window, "ciwa.png"));
	gtk_widget_show (GTK_WIDGET(image1));
	gtk_box_pack_end (GTK_BOX (hbox1),GTK_WIDGET(image1), FALSE, TRUE, 0);
	
	hboxN = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (handleBox), hboxN);
	gtk_container_border_width (GTK_CONTAINER (hboxN), 0);
	gtk_widget_show (hboxN);
	
	oix_toolbar = make_toolbar(main_window, crt_n); 
	/* gtk_widget_set_usize( oix_toolbar , -1 , 60);*/
	gtk_box_pack_start (GTK_BOX (hboxN), oix_toolbar,FALSE, FALSE, 0);
	
	
	if(IsHisRdbDisable()!=TRUE)
	{
		fault_redisplay = gtk_label_new(pConvert =_toUtf8EX( OIX_CSTR_FAULT_REDISPLAY_OFF));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hboxN), fault_redisplay,FALSE, FALSE, 0);
		gtk_widget_show (fault_redisplay);
		g_object_set_data(G_OBJECT(main_window),"label_fault_redisplay",fault_redisplay );
		
		step_btn = gtk_button_new();
		gtk_box_pack_start (GTK_BOX (hboxN), step_btn,FALSE, FALSE, 0);
		
		g_object_set_data(G_OBJECT(main_window),"btn_step",step_btn );
		
		pause_btn = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAUSE ));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hboxN), pause_btn,FALSE, FALSE, 0);		
		g_object_set_data(G_OBJECT(main_window),"pause_btn",pause_btn );
	}
  		cur_usr_label = gtk_label_new(pConvert=_toUtf8EX( OIX_STR_CUR_USR));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hboxN), cur_usr_label,FALSE, FALSE, 0);
		gtk_widget_show (cur_usr_label);
		g_object_set_data(G_OBJECT(main_window),"label_cur_usr",cur_usr_label );

		next_dsp_btn = gtk_button_new_with_label(pConvert=_toUtf8EX( OIX_CSTR_NEXT_DSP));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hboxN), next_dsp_btn,FALSE, FALSE, 0);
		gtk_signal_connect (GTK_OBJECT (next_dsp_btn),"clicked",GTK_SIGNAL_FUNC(OpenNextDemoDsp),(gpointer)crt_n) ;
		
		/*gtk_widget_show (next_dsp_btn);*/
		g_object_set_data(G_OBJECT(main_window),"next_dsp_btn",next_dsp_btn );
		
		pause_demo_btn = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_PAUSE));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hboxN), pause_demo_btn,FALSE, FALSE, 0);
		gtk_signal_connect (GTK_OBJECT (pause_demo_btn),"pressed",GTK_SIGNAL_FUNC(PauseDemoDsp),(gpointer)crt_n) ;
		
         g_object_set_data(G_OBJECT(main_window),"pause_demo_btn",pause_demo_btn );
		
		
		sep = gtk_label_new(pConvert=_toUtf8( "       "));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hboxN), sep,FALSE, FALSE, 0);
		gtk_widget_show (sep);
		
		cur_filelab = gtk_label_new(pConvert=_toUtf8EX( OIX_CSTR_CUR_DSP_NAME));
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hboxN), cur_filelab,FALSE, FALSE, 0);
		gtk_widget_show (cur_filelab);
		g_object_set_data(G_OBJECT(main_window),"label_cur_filename",cur_filelab );
		
		gtk_widget_show (oix_toolbar);
		lblTimer  = gtk_label_new("");
		font1 = pango_font_description_from_string("Arial Black"); 
		pango_font_description_set_size (font1, 13 * PANGO_SCALE); 
		gtk_widget_modify_font (lblTimer, font1);
		gdk_color_parse ("#000244", &clr);
		gtk_widget_modify_fg(lblTimer,GTK_STATE_NORMAL,&clr);
		
		gtk_widget_show (lblTimer);
		gtk_box_pack_end (GTK_BOX (hboxN),lblTimer,FALSE, FALSE, 0);
		
		g_object_set_data(G_OBJECT(main_window),"lblTimer",lblTimer );
		
		
		
        
		vpaned1 = gtk_vpaned_new ();
		gtk_widget_show (vpaned1);
		gtk_box_pack_start (GTK_BOX (main_vbox), vpaned1, TRUE, TRUE, 0);
        
		
		
		
		/*************** Draw Area  code here !!! **************************/  
		
		
		
		
		
		drawing_area[crt_n] = gtk_drawing_area_new();
		if (OIX_FAULTTAB_NO_VISIBLE )
			gtk_widget_set_usize(drawing_area[crt_n],-1,(int)(2200*yscale[crt_n]));  
		else 
			gtk_widget_set_usize(drawing_area[crt_n],-1,(int)((LOCAL_HEIGHT -120)/**yscale[crt_n]*/));
		
		gtk_widget_set_events(drawing_area[crt_n], GDK_EXPOSURE_MASK  
			| GDK_LEAVE_NOTIFY_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK
			| GDK_POINTER_MOTION_MASK );			 
		
		gtk_signal_connect (GTK_OBJECT (drawing_area[crt_n]),"expose_event",GTK_SIGNAL_FUNC(expose_events),GINT_TO_POINTER(crt_n)) ;
		
		
		
		gtk_signal_connect (GTK_OBJECT (drawing_area[crt_n]),"motion_notify_event",
			GTK_SIGNAL_FUNC(motion_notify_callback),GINT_TO_POINTER(crt_n)) ;   
		gtk_signal_connect (GTK_OBJECT (drawing_area[crt_n]), "realize", 
			GTK_SIGNAL_FUNC (draw_area_realize),   GINT_TO_POINTER(crt_n));
		
        gtk_signal_connect (GTK_OBJECT (drawing_area[crt_n]), "button_press_event",
			GTK_SIGNAL_FUNC (button_press),GINT_TO_POINTER(crt_n) ); 

		gtk_signal_connect (GTK_OBJECT (drawing_area[crt_n]), "button_release_event",
			GTK_SIGNAL_FUNC (button_release),GINT_TO_POINTER(crt_n) ); 
		
		
        gtk_signal_connect (GTK_OBJECT (drawing_area[crt_n]), "size_allocate", 
			GTK_SIGNAL_FUNC ( canvas_allocate), GINT_TO_POINTER(crt_n));
		/***hcl11_23****/
		gtk_signal_connect (GTK_OBJECT (drawing_area[crt_n]), "scroll_event",
			GTK_SIGNAL_FUNC (zoom_scroll_event),GINT_TO_POINTER(crt_n) ); 
		
		
		
        frame1 = gtk_frame_new  (NULL/*_toUtf8( OIX_CSTR_FALUT_TAB,strlen(OIX_CSTR_FALUT_TAB ),NULL,NULL,NULL) */);  
        gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
        
        gtk_widget_show (frame1); 
        gtk_container_border_width (GTK_CONTAINER (frame1), 4);
        
		drawingarea2 = gtk_drawing_area_new ();
		gtk_widget_set_size_request(drawingarea2,-1,(int)(100*yscale[crt_n])) ;  
		gtk_widget_set_events(drawingarea2, GDK_EXPOSURE_MASK  
			| GDK_LEAVE_NOTIFY_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_POINTER_MOTION_MASK );
		gtk_signal_connect (GTK_OBJECT (drawingarea2), "realize", 
			GTK_SIGNAL_FUNC (draw_area2_realize),   GINT_TO_POINTER(crt_n));
		
		gtk_signal_connect (GTK_OBJECT (drawingarea2),"expose_event",
			GTK_SIGNAL_FUNC(fault_alarm_expose),GINT_TO_POINTER(crt_n)) ;
		
		gtk_container_add( GTK_CONTAINER(frame1),drawingarea2 );
		
		gtk_widget_show (drawingarea2);
		gtk_paned_pack1 (GTK_PANED (vpaned1), drawing_area[crt_n], FALSE, FALSE);
		gtk_paned_pack2 (GTK_PANED (vpaned1), frame1, TRUE, TRUE);
		
		
        gtk_paned_set_position (GTK_PANED (vpaned1), (int)(500*yscale[crt_n]));//
		gtk_widget_realize(GTK_WIDGET( drawing_area[crt_n])) ;
		gtk_widget_show (drawing_area[crt_n]);
		
		return main_window;
}
 

/*********************************

	Popup menu code here !!!
 
***********************************/

GtkWidget* CreatePoppMenu(gint crt_n,GtkWidget *parent )
{
 
	char buf[128]="";
	GtkWidget *menu_items;
	GtkWidget *popmenu;
	int i ;
	popmenu = gtk_menu_new ();
	
	
	for ( i = 0; i < 3; i++)
	{           
		sprintf (buf, "Popup menu - %d", i);
		
		menu_items = gtk_menu_item_new_with_label (buf);
		
		gtk_menu_append (GTK_MENU (popmenu), menu_items);
		
		gtk_signal_connect (GTK_OBJECT (menu_items), "activate",
			GTK_SIGNAL_FUNC (menuitem_response), (gpointer) g_strdup (buf));
		
		gtk_widget_show (menu_items);
	}
	gtk_menu_set_screen(GTK_MENU(popmenu), second_screen[crt_n]) ;
	gtk_widget_show(popmenu ) ;
	return  popmenu ;
}

void GetStationInfo()
{
	int stn_num,rec_num,dev_num,group_num;
	char stn_lname[STATION_LONGNAME_SIZE],stn_cname[STATION_COMMENT_SIZE];
	char dev_name[GROUP_NAME_SIZE],dev_lname[GROUP_LONGNAME_SIZE],dev_cname[GROUP_COMMENT_SIZE];
//	char group_name[GROUP_NAME_SIZE],group_cname[GROUP_COMMENT_SIZE];
	int i,j,k,crt_n;
	IOGROUP_INFO	ioGroup_info;
	GetStationNum(&stn_num);
	printf("stn_num=%d\n",stn_num);
	for(i=1;i<=stn_num;i++) 
	{  	
		UCHAR	stnId;
		
		if(-1==GetStnIdByIndex(i, &stnId))
			continue;
		
		StnDef[i].Id = stnId;
		
		if( GetStnNameById(StnDef[i].Id,StnDef[i].Name)!=-1 )
		{
			if(GetGroupNumById( StnDef[i].Id, &dev_num )!=-1)
			{	
				StnDef[i].dev_num =dev_num;
				for(j=1;j<=dev_num;j++)
				{	
					StnDef[i].dev[j].Id = j;					
					if(GetGroupLongnameById( StnDef[i].Id, StnDef[i].dev[j].Id, dev_lname )!=-1)
						strcpy(StnDef[i].dev[j].LongName,dev_lname);
					if(GetGroupNameById( StnDef[i].Id,StnDef[i].dev[j].Id, dev_name )!=-1)
						strcpy(StnDef[i].dev[j].Name,dev_name);	
					if(GetGroupCommentById( StnDef[i].Id,StnDef[i].dev[j].Id, dev_cname )!=-1)
						strcpy(StnDef[i].dev[j].CName,dev_cname);
					if(GetRecordNumById(StnDef[i].Id, StnDef[i].dev[j].Id,&rec_num)!=-1)
						StnDef[i].dev[j].rec_num = rec_num;
					if (GetLcuIoGroupNumById(StnDef[i].Id,StnDef[i].dev[j].Id,&group_num)!=-1)
					{
						StnDef[i].dev[j].group_num = group_num;
						for(k=0;k<group_num;k++)
						{
							if (GetLcuIoGroupInfoByIndex(StnDef[i].Id,StnDef[i].dev[j].Id,k, &ioGroup_info)!=-1)
							{
								StnDef[i].dev[j].group[k+1].Id = ioGroup_info.ioGroup;
								strcpy(StnDef[i].dev[j].group[k+1].CName,ioGroup_info.comment);
								StnDef[i].dev[j].group[k+1].Status = ioGroup_info.ioGroupStatus;
								for(crt_n=0;crt_n<MAX_CRT_NUM;crt_n++)
									StnDef[i].dev[j].group[k+1].isFilter[crt_n] = FALSE;
							}
						}
					}
					/*  printf("StnDef[%d].dev[%d].LongName=%s,CName=%s,dev_num=%d\n",i,j,StnDef[i].dev[j].LongName,StnDef[i].dev[j].CName,dev_num);  */
				}
			}			
		}		
		if( GetStnLongnameById( StnDef[i].Id, stn_lname )!=-1)
			strcpy(StnDef[i].LongName,stn_lname);
		if(GetStnCommentById(StnDef[i].Id,stn_cname)!=-1)
		{
			strcpy(StnDef[i].CName,stn_cname);
			/*  printf("StnDef[%d].CName=%s\n",i,StnDef[i].CName);  */
		}
	}  
}

gint         windk_screen_get_width             (GdkScreen   *screen,int screenNUM)
{
#ifdef  SUN_OS
	return gdk_screen_get_width(screen);
#endif
	
#ifdef WINDOWS_OS
	return gdk_screen_get_width(screen)/screenNUM;
#endif
}

gint         windk_screen_get_height            (GdkScreen   *screen,int screenNUM)
{	
#ifdef  SUN_OS
	return gdk_screen_get_height(screen);
#endif

#ifdef WINDOWS_OS
	return gdk_screen_get_height(screen);
#endif
}

gint        windk_display_get_n_screens      (GdkDisplay  *display)
{
#ifdef  SUN_OS
	return gdk_display_get_n_screens (display); 
#endif

#ifdef WINDOWS_OS
	GdkScreen *screendisplay= gdk_display_get_default_screen (display);
	return gdk_screen_get_n_monitors(screendisplay);
		
/*
    int i,tempWidth;

	tempWidth  = gdk_screen_get_width(screendisplay);
	for(i=0;i<MAX_DSP_MOD;i++)
	{
		if(tempWidth%SCREEN_WIDTH_MOD[i]==0)
		{
			return tempWidth/SCREEN_WIDTH_MOD[i];
		}
	}

	Err_log("Screen size is illegal!\n");
	
	return -1;
*/
#endif
	
}

void get_XY_scale(float * x,float * y)
{
	
#ifdef WINDOWS_OS		
	*x = (float)LOCAL_WIDTH/STND_WIDTH  ;
	*y = (float)LOCAL_HEIGHT/STND_HEIGHT;
#endif	
	
#ifdef  SUN_OS
	*x = (float)LOCAL_WIDTH/STND_WIDTH ; 
	*y = (float)LOCAL_HEIGHT/STND_HEIGHT;
	
#endif
}

/*get CHN popmenu, change menu_items_path to utf8*/
void make_Popmenu_items_CHN()
{
	int menu_items_size;
	int i;

	menu_items_size = sizeof (menu_items) / sizeof (menu_items[0]);
	for (i = 0 ; i < menu_items_size ; i++ )
	{
		menu_items[i].path=_toUtf8EX( menu_items[i].path);
	}

	menu_items_size =sizeof (menu_items_obj_disable) / sizeof (menu_items_obj_disable[0]);
	
	for (i = 0 ; i < menu_items_size ; i++ )
	{
		menu_items_obj_disable[i].path=_toUtf8EX( menu_items_obj_disable[i].path);
	}
}

void make_Popmenu_evt_items_CHN()
{
	int menu_items_size;
	int i;
	
	menu_items_size = sizeof (menu_items_evt) / sizeof (menu_items_evt[0]);
	for (i = 0 ; i < menu_items_size ; i++ )
	{
		menu_items_evt[i].path=_toUtf8EX( menu_items_evt[i].path);
	}
	
// 	menu_items_size =sizeof (menu_items_obj_disable) / sizeof (menu_items_obj_disable[0]);
// 	
// 	for (i = 0 ; i < menu_items_size ; i++ )
// 	{
// 		menu_items_obj_disable[i].path=_toUtf8EX( menu_items_obj_disable[i].path);
// 	}
}
/***********************************************************************************/
gint auto_login(gpointer data)//added by dm for oix auto login 
{
	if (shm_ots_addr->login_status == TRUE && oix_login_status == FALSE)
	{
		GetPasswd_activate_simu("ems","H9000ems");
		oix_login_status = TRUE;
	}
	else if (shm_ots_addr->login_status == FALSE && oix_login_status == TRUE)
	{
		UsrLogout ( (int)data );
		LoadUserPrivilegeDatabase ( );
		oix_login_status = FALSE;
	}
	return TRUE;
}


void main(int	argc,char	*argv[]){ 
//char s[100] ;
	GtkWidget  *popmenu1;
	GtkWidget  *popmenu_evt;//GtkWidget*w;
	gint i ,iSetScreenNUm ,iSetDebug ,iSetRemote ;
	gchar screenname[20],screenname1[20] ,displayname[20],displayname1[20];
	GdkDisplay *second_display[MAX_CRT_NUM];
//	POINTER pt ;  //for test ; 
#ifdef	WINDOWS_OS
	     WSADATA	WSAData;
#endif

		if(-1==InitSysLog(H9000_MODULE_OIX, "OIX"))
		 {
			 Err_log("Fail to InitSysLog!\n");
			 exit(-1);
		 }
	
	memset(bRealiseFinished, 0, sizeof(BOOL)*MAX_CRT_NUMBER*2);
		 
    iSetDebug = iSetScreenNUm =iSetRemote =0 ;
    memset(displayname1,0,sizeof(displayname1));

	SoftCopyFlag=FALSE;
 
	if(  InitStrTextDefine() ==-1)
	{
		printf("OIX::: while ReadDefineFile Error!!!\n");
		exit(-1) ;
	}
         
	gtk_set_locale();

	setlocale(LC_ALL, "zh.GBK");
	
    gtk_rc_add_default_file(GTK_RC_FILE); 
    gtk_rc_parse(GTK_RC_FILE); 
   // argv[argc++]="--sync";//GCL
    gtk_init (&argc, &argv); 
#ifdef	WINDOWS_OS
    if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		Err_log("Fail to initialize net!\n");
		exit(-1);
	}
	iSetDebug = 1 ;
#endif
    gethostname(local_hostname,sizeof(local_hostname));
	printf("hostname=%s\n",local_hostname);
 
 
	isExLang  =  GetExLangState();
    if (argc >1)
        {
            for  (i = 0 ; i < argc ; i++)
                {
                    if (strcmp(argv[i], "--sreen_num") == 0 || strcmp(argv[i], "-n") == 0  )
                    {   
                        num_screen =  atoi(argv[i+1] );
                        if (num_screen> 0  && num_screen < MAX_CRT_NUM  )
                            iSetScreenNUm  = 1 ;
                        else 
                            iSetScreenNUm  = 0 ;
                            
                    }else
                    
                    if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0  )
                    {          
                            iSetDebug  = 1 ;            
                    }else
					if (strcmp(argv[i], "--remote") == 0 || strcmp(argv[i], "-r") == 0  )
                    {          
                            iSetRemote  = 1 ;            
                    } else
                      /*  {printf ("OIX Usage : --debug  or -d open as debug mode \n  \t --screen_num X or -n N to open N screens \n ") } */
                     
					if (strcmp(argv[i], "--disp") == 0 || strcmp(argv[i], "-p") == 0  )
					{          
						iSetDispEntry[0]=iSetDispEntry[1] =iSetDispEntry[2]  = 1 ;            
                    } 
                        
                printf ("!!!!!argv [%d] =%s \n sreen = %d \n",i ,argv[i] ,num_screen );
            }
    }
        g_display = gdk_display_get_default();
        strcpy(displayname,gdk_display_get_name(g_display));
printf ("OIX:::display name =  %s \n",displayname);   
#ifdef  SUN_OS
      strncpy( displayname1,displayname , strrchr( displayname, '.')- displayname );
	  if (isExLang)
		  setenv("LANG" ,"en_US.UTF-8",1) ;
	  setenv ("G_FILENAME_ENCODING","@locale",1);
#endif

	  if (!iSetScreenNUm)
		  num_screen = windk_display_get_n_screens (g_display); 
	 
	   if (num_screen<0 || num_screen>MAX_CRT_NUM)
	  {
		  printf("Screen size is illegal!!!\nPlease check the screen width!\n");
		  printf("Screen width shoule within reasonable Range!\n ");		  
		  PopupMessageBoxRet (NULL,"Screen size is illegal! \n\n" );
		  exit(1);
	  }
	  
	  printf ("display Num is %d  \n",num_screen );
 /*************the following is moved from old oix ****************/ 
	
	  if (MountDB(NULL)==-1 )
	  {
		  printf("\n OIX: MountDB( ) Error !!!! \n ");
		  exit(-1); 
	  }  
      OixReadConfig()  ; 
	  GetStationInfo();
// 	  if (GetGroupDef()==-1)//added by dm to read group define
// 	  {
// 		  Err_log(  "Fail to load GetGroupDef ");
// 		  printf("\n OIX: GetGroupDef( ) Error !!!! \n ");
// 	  }
	  if( lan_out_init ()== -1)
	  {
		  printf("Lan_init  failure!!!\n");
		  exit(1);	
	  }
	/*sys_init();  */
	
	get_hdb_shm(); /* Map HDB shm ::in lan_common.h*/

	if (get_ots_shm()!=TRUE)
	{
		printf ("get_ots_shm()  error !!"); 
		return ; 
	}

	tagNum = read_tag_ply_conf();
	add_pixmap_directory ("..//dsp//pic//");
	add_pixmap_directory ("/home/ems/h9000/dsp/pic/");
        	
	ReadDemoDspName ( demo_dsp_name[isExLang] );
     /* ReadRptDefFile();*/	

	CreateFontX ();
	printf("create font finished \n");

	/*******************end ************************/	
	for (i =0 ;i <num_screen ; i++) 
	{
#ifdef  WINDOWS_OS
	    strcpy (screenname ,  "\\") ;
		sprintf (screenname1 , "%s:0.%d\0",local_hostname,i) ;
		strcat(screenname,screenname1) ;
		printf ("screenname : %s \n" ,screenname);	
#endif 
#ifdef	SUN_OS
	    sprintf (screenname , "%s.%d\0",displayname1,i) ;		
#endif

		printf ("OIXXX:screenname : %s \n" ,screenname);		
		second_display[i] = gdk_display_open ( screenname)  ;
		if (second_display[i])
		{
 	 	  second_screen[i] = gdk_display_get_default_screen (second_display[i]);     	   
 		}   
		else 
 		{
			g_print ("Can't open display :\n\t%s\n\n",  screenname);
#ifdef  SUN_OS
    		exit (1);
#endif
  		}

		if(i==0)
		{
			LOCAL_WIDTH  = windk_screen_get_width(second_screen[i],num_screen);
			LOCAL_HEIGHT = windk_screen_get_height (second_screen[i],num_screen);
		}
		get_XY_scale(&xscale[i],&yscale[i]);

		printf("\n  OIX::SREEN resolve capability :%d  X %d \n",LOCAL_WIDTH ,LOCAL_HEIGHT ) ;             
		main_w[i]=  CreateMainWindow(i,iSetDebug) ;
		
	    /*xscale[i] = yscale[i]= 1 ; */
		gtk_window_set_screen (GTK_WINDOW(main_w[i]), second_screen[i]);  
		gtk_widget_show (main_w[i]); 
		
	}

	canvas_width =  LOCAL_WIDTH;
	canvas_height = LOCAL_HEIGHT;

	InitPrinterName();
	printf("$$$$ OIX:: Init printer Name ...!!\n");           
	LoadUserPrivilegeDatabase( );
	
	printf("$$$$  oixInit sucess!!!\n");   
	
	
	printf("@@@@  CreateFillPalette sucess!!!\n");   
	
	LoadStringGrpLibrary ();
	printf("#### LoadStringGrpLibrary sucess!!!\n"); 
	
	LoadSymbolLibrary ();
	printf("&&& LoadSymbolLibrary sucess!!!\n"); 
	
	LoadSymbolGrpLibrary ();
	printf("&&& LoadSymbolGrpLibrary sucess!!!\n");  
	
	LoadColorGrpLibrary ();
	printf("&&& LoadColorGrpLibrary sucess!!!\n");
	if(InterLockFileLoad()==-1)
	{
		Err_log(  "Fail to load InterLockFile ");
		printf("OIX: ALARM  load InterLockFile Error !!!!\n");
		return;
			
	}
	else
		printf( "OIX: InterLockFileLoad OK\n");

	if(RelatePointFileLoad()==-1)
	{
	//	Err_log(  "Fail to load RelatePointFile ");
		printf("\nOIX: ALARM  load RelatePointFile Error !!!!\n");
		
	}
	else
		printf( "OIX: RelatePointFile Load OK\n");
/*	@@@@@@@@@@@@@@@   */    
 	shm_rec_addr = (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		Err_log(  "Fail to link record shm");
                printf("OIX: ALARM   LinktoShareMem Error !!!!\n");
		return ;
	}


/*   @@@@@@@@@ ï¿½Ûºï¿½ï¿½ï¿½Ï¢ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½  @@@@@@@@@ */  
	shm_info_rec_addr = (INFO_RECORD*) LinktoShareMem( SHM_INFO_RECORD_KEY_CHAR );
	if( shm_info_rec_addr == (INFO_RECORD*)-1 )
	{
		Err_log(  "Fail to link shm_info_rec_addr shm");
                printf("OIX: ALARM   shm_info_rec_addr Error !!!!\n");
	  // chichi	return ;
	}


/*   @@@@@@@@@ 24Ð¡Ê±ï¿½ï¿½ï¿½ï¿½@@@@@@@@@@@@@@@   shm_real_curve->reloadflag */    
 	shm_real_curve = (CURVE_SHM_DATA*) LinktoShareMem( SHM_TREND_KEY_CHAR );
	if( shm_real_curve == (CURVE_SHM_DATA*)-1 )
	{
		Err_log(  "Fail to  Real curve shm");
                printf("OIX: RealCurve::  LinktoShareMem Error !!!!\n");
		return ;
	}
    if (InitRealCCurvePt()==FALSE)
	{
		Err_log(  "Fail to  InitRealCCurvePt");
        printf("OIX: RealCurve::  Init  Real Config CurvePt Error !!!!\n");

	}
        
    dsp_rec_ptr= shm_rec_addr->head.header_ptr;
	printf("&&& LinktoShareMem sucess!!!\n");
	shm_fault_addr= (FAULT_RECORD_TABLE*) LinktoShareMem( SHM_FAULT_TABLE_KEY_CHAR );
	if( shm_fault_addr == (FAULT_RECORD_TABLE*)-1 )
	{
		Err_log("Fail to link fault record shm");
		return ;
	}
    shm_syscomm_addr= (SYS_COMM_TABLE*) LinktoShareMem( SHM_SYSCOMM_TABLE_KEY_CHAR );
	if( shm_syscomm_addr == (SYS_COMM_TABLE*)-1 )
	{
		Err_log("Fail to link syscomm record shm");
		return ;
	}
 
 	LoadMenuLibrary ();
//	GetSelectedPrinter(main_w[0],0);  chichi 080307
	
    make_Popmenu_items_CHN(); 
	make_Popmenu_evt_items_CHN();
    for (i =0 ;i <num_screen ; i++) 
	{
		while (!bRealiseFinished[i][0] || !bRealiseFinished[i][1])
		{
			MySleep(20);
		}
             ZoomInit ( i );
             GifInit (i );
             CreateFillPalette (i);
             OixInit(i); 
             if( -1 == ReadColor (i) )
             {
				printf("color[%d] allocate fail" , i);
				exit(1);
             }
           /*  printf("OpenDspFiles[%d]=%s\n",i,demo_dsp_name_str[i]);*/
             OpenDspFile ( i,  _CS_(commonDirName));    //"YC_HMSY.dbin" 
		
             if ( cur_dsp_ptr[i]+1 < MAX_DSP_CNT )
				 cur_dsp_ptr[i]++; 
             else
				 cur_dsp_ptr[i] = 0;
			 strcpy (rem_dsp_name[i][cur_dsp_ptr[i]], OpenDspFiles[i]);  
			 CreateFaultAlarm(  i ,canvas2[i], info[i].gc2    );     
             
             popmenu1 = create_popup_menu(i );
             g_object_set_data(G_OBJECT(main_w[i]),"PopMenu",popmenu1);

			 popmenu_evt=create_evt_popup_menu(i);
			 g_object_set_data(G_OBJECT(main_w[i]),"PopMenuEvt",popmenu_evt);


/*added by wzg 20070517*/
			if(!wgts[i][WgtN_cur_user])
				wgts[i][WgtN_cur_user] = gtk_object_get_data(GTK_OBJECT(main_w[i]),"label_cur_usr");
			if(wgts[i][WgtN_cur_user])
				gtk_label_set_text(GTK_LABEL(wgts[i][WgtN_cur_user]),_toUtf8EX( OIX_STR_LOGOUT));		

        }    
    
	/*  OpenDspFile ( 1, "sx_fdbt.dbin");    */   

	if (IsCurveWndDisable(OperCharacterCurveDisable) == FALSE)
	{
		readOpCharacterCurveData();		//³õÊ¼»¯»ú×éÔËÐÐ×´Ì¬Êý¾Ý
	}

	InitRealCurvePt();		//³õÊ¼»¯ÊµÊ±ÇúÏß¼ÇÂ¼µã
      
	if (timer  != 0 )
	{
		gtk_timeout_remove(timer );
		timer = 0;
	}
	timer =gtk_timeout_add ( 1000,(GtkFunction)ticktick, 0 ); 
	ticktick(0);
	
	if (auto_login_timer  != 0 )
	{
		gtk_timeout_remove(auto_login_timer );
		auto_login_timer = 0;
	}
	auto_login_timer =gtk_timeout_add ( 1000,(GtkFunction)auto_login, 0 ); 

	/*  g_thread_init(NULL) ; */
	gtk_main ();
  
  /***************gtk end *******************/
        
        
}
 void gtk_main_win_quit(  )
{
	 if (timer  != 0 )
	 {
		 gtk_timeout_remove(timer );
		 timer = 0;
	 }
	 if (auto_login_timer  != 0 )
	 {
		 gtk_timeout_remove(auto_login_timer );
		 auto_login_timer = 0;
	}
    gtk_main_quit ();            //ÍË³ögtkÖ÷Ñ­»·
}
































































