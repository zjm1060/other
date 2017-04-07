/*-----------------------------------------------------------------------

  Upadate from X11 to GTK ;
  Created by chi.hailong
  Date : 		2005.9.23
  Last Update :	2005.9.28  
  
	updated by zyp on 2006.3.26 for v4.0db
	SaveMenuLibrary () is added by zyp to transtrate the old dbin or DBIN to new 
				because the DBLEN is changed from 36 to 48(add station_name)
				LastUpadate : 2006.3.23  by chi.hl
				UNIX HAVE SYN TO WINDOWS by chi.hl  2007.06.15
				last SYN by hcl 2007.11.28
---------------------------------------------------------------------*/

#include 	<stdio.h>
#include	<gtk/gtk.h>
#include	"../inc/gtk_ipm_k_def.h"
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

GTimer		*timer ;
BOOL		can_pressed = FALSE ;
GtkWidget	*ctrl_menu_shell[MAX_CRT_NUMBER]={NULL,NULL,NULL} ;
GtkWidget	*ctrl_menu_canvas[MAX_CRT_NUMBER] ={NULL,NULL,NULL} ; 
typedef unsigned int    COLORREF_X;
/*#define	DEBUG*/   
extern  POINTER          pnt_tab_ptr[MAX_CRT_NUMBER];  

void	CtrlMenuExposeCB(	GtkWidget *widget, GdkEventExpose *event, gpointer data);
void 	ctrl_menu_realize(GtkWidget*,gpointer );
gint 	ctrl_menu_button_press	( GtkWidget*,GdkEvent*, gpointer) ;
gint 	ctrl_menu_button_release( GtkWidget*,GdkEvent*, gpointer) ;
GBRCD *GetCtrlActFromCtrlMenu (int	 crt_n, GtkWidget  *w, int x1, int y1, int x2, int y2, MENU	*menu);
void  ctrl_menu_motion_notify_callback(GtkWidget *widget,GdkEventMotion *event,gpointer data);

static GtkWidget* CreateDynCtrlMenu ( GtkWidget	*parent,int crt_n, GBRCD 	*rec);

extern void	fwriteBckRcd (FILE	*fp, BACKRCD 	*rcd);
extern void	freadBckRcd (FILE	*fp, BACKRCD 	*rcd);
extern void	freadLnkRcd (FILE	*fp, LINKRCD 	*rcd);
extern int	SearchMenuId ( char	menu_name[MENU_NAME_LEN]);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void FormCtrlLockPacketAndBrdcst(int crt_n,DMS_COMMON *int_data,int flag);
extern void	ClearItemTag (int crt_n);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void	CtrlActHandler (int crt_n, GBRCD *rcd);
extern void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );

extern void	DrawSymbol (int crt_n,  GdkDrawable	*drawable,  GdkGC *gc, BACKRCD	*rec,int fb,BOOL isDYN, BOOL disable_flag );
extern void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype);

extern void	DrawDynamicRcd (int crt_n,GBRCD	*rcd, GdkGC	*gc, GdkDrawable	*drawable );
extern int GetColorId(COLORREF_X rgb);
extern int ColorId3to4(int id3);
char *_toUtf8(char *c);
extern BOOL isExLang   ; 
XRECT	menu_box_rect;
XRECT	btn_rect;
int	menu_box_width, menu_box_height;
gint	menu_timeout_id[MAX_CRT_NUMBER]={0,0,0};
char	menu_title_str[50];

static int   ctrl_menu_cur_prompt_rcd= -1, ctrl_menu_last_prompt_rcd= -1;
static GtkWidget *ctrl_menu_prompt_widget;

void CtrlMenuDrawPromptFrame(int crt_n, GBRCD *rec);


void	LoadMenuLibrary ()
{
	int	len, i, j;
	FILE	*fp;
 

	char	*menu_part_name[2] = {FILE_CTRL_MENU,FILE_CTRL_MENU_EXLANG};

	char	version[6];
	
	fp = fopen( menu_part_name[isExLang], "rb");
	
	if (fp == NULL)
	{
		printf ("OIX: open %sfile error.\n" , menu_part_name[isExLang] );
	 
		exit(-1) ;
		/* ExitProc ();  */
	}
	fread(version,1,6,fp);
	
	if(strcmp(version,"v4.0a")==0)
		printf("this is new ctrl_menu\n");
	else
	{
		printf("this is old ctrl_menu\n");
		return;
	}
	
	len	=4;
	fread (&menudb.nextpos, len, 1, fp);
#ifdef SUN_OS
	Swap4Byte(&menudb.nextpos);
#endif
	
	for (i=0; i<menudb.nextpos; i++)
	{
		len	=4;
		fread (&menudb.menu[i].nextpos, len, 1, fp);
		fread (&menudb.menu[i].x1, len, 1, fp);
		fread (&menudb.menu[i].y1, len, 1, fp);
		fread (&menudb.menu[i].x2, len, 1, fp);
		fread (&menudb.menu[i].y2, len, 1, fp);
#ifdef SUN_OS
		
		Swap4Byte(&menudb.menu[i].nextpos);
		Swap4Byte(&menudb.menu[i].x1);
		Swap4Byte(&menudb.menu[i].y1);
		Swap4Byte(&menudb.menu[i].x2);
		Swap4Byte(&menudb.menu[i].y2);
		/*printf("11111::::menu[%d].x1=%d,y1=%d,x2=%d,y2=%d\n",i,menudb.menu[i].x1,menudb.menu[i].y1,menudb.menu[i].x2,menudb.menu[i].y2);*/
#endif
		fread (menudb.menu[i].name, sizeof(menudb.menu[i].name), 1, fp);
		
		/*printf("@@@@:::menu[%d].name=%s\n",i,	menudb.menu[i].name);*/
		
		if ( strlen(menudb.menu[i].name) > MENU_NAME_LEN )     
		{
			printf("Load menu lib: menu name is too long: %s\n", menudb.menu[i].name );
			menudb.menu[i].name[MENU_NAME_LEN-1] = 0;
		}
		/*printf("@@@@::::menudb.menu[%d].nextpos=%d\n",i,menudb.menu[i].nextpos);*/
		for (j=0; j<menudb.menu[i].nextpos; j++)
		{
			freadBckRcd (fp, &menudb.menu[i].rcd[j].bck);
			freadLnkRcd (fp, &menudb.menu[i].rcd[j].lnk);
		}
	}
	
	fclose (fp);
	printf ("OIX: LoadMenuLibrary ok !!!\n");	
}


XRECT	GetMenuBoxRect (GBRCD	*rec)
{
	XRECT	rect;
	/*	GBRCD	*rcd;*/
	MENU	menu;
	int	 menu_id, menu_nextpos;
	
	menu_id = rec->lnk.act.fmt_def.menu.id;
	menu_id = SearchMenuId (rec->lnk.act.fmt_def.menu.name);
	rec->lnk.act.fmt_def.menu.id= menu_id;
	
	menu = menudb.menu[menu_id];
	menu_nextpos	=menudb.menu[menu_id].nextpos;
	
	if (menu_nextpos != 0)
	{	
		rect.x1 = menu.x1;
		rect.y1 = menu.y1;
		rect.x2	= menu.x2;
		rect.y2 = menu.y2;
	}
	else
	{
		rect.x1	=0;
		rect.y1	=0;
		rect.x2	=0;
		rect.y2	=0;
	}
	
#ifdef	DEBUG
	printf ("#####OIX: GetMenuBoxRect: rect x1= %d, y1= %d, x2= %d, y2= %d\n", 
		rect.x1, rect.y1, rect.x2, rect.y2);
#endif
	return	rect;
	
}


void DestroyMenuBox (GtkWidget	*w)
{
	int	crt_n;
	DMS_COMMON dms_cmm;
	
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 )
		return;   
	printf ("@@@@@@@@@@@@destroy menubox!!!!\n")   ;	 
	
	ctrl_menu_shell[crt_n]= 0;
	if(menu_timeout_id[crt_n])
	{
		gtk_timeout_remove(menu_timeout_id[crt_n]);
		menu_timeout_id[crt_n]= 0;
	}
	
	if( ctrl_rcd[crt_n]!=0 && ReadEntryByNameStr ( ctrl_rcd[crt_n]->lnk.dbname,&dms_cmm ) != -1 )       
		FormCtrlLockPacketAndBrdcst (crt_n,&dms_cmm,NO );           
	ClearItemTag ( crt_n );
}


gboolean
ctrl_menu_quit(int  crt_n)
{
    printf("##### ctrl_menu_quit\n");
	gtk_widget_destroy(ctrl_menu_shell[crt_n]);
	ctrl_menu_shell[crt_n]= 0;
	return FALSE;
}

gint CheckCtrlMenu(int crt_n, GBRCD 	*rec)
{
    int menu_id ;
    menu_id = rec->lnk.act.fmt_def.menu.id;
    
    if (strcmp(rec->lnk.act.fmt_def.menu.name,"NULL")==0) return -2;
    menu_id = SearchMenuId (rec->lnk.act.fmt_def.menu.name);
	
    if (menu_id  >=0 )
	{
		rec->lnk.act.fmt_def.menu.id= menu_id;
		return 0 ;
    }
    return -1 ;
}


void PopupDynCtrlMenu (int crt_n,GtkWidget	*parent,  GBRCD 	*rec)
{
	LCU_ID  lcu_id;
	POINTER pt;
    if (ctrl_menu_shell[crt_n] == (GtkWidget *)NULL)
	{
		GetPtIdByNameStr((char *)&rec->lnk.dbname,&pt);
		lcu_id.stn_id= pt.stn_id;
		lcu_id.dev_id= pt.dev_id;
		if(CheckHostPrivilege(local_hostname, lcu_id))
		{
			ctrl_menu_shell[crt_n] = CreateDynCtrlMenu( parent ,crt_n, rec);
			gtk_widget_show(ctrl_menu_shell[crt_n]); 
		}					
		else
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
	}
	else
		gtk_window_present (GTK_WINDOW(ctrl_menu_shell[crt_n]));
	
}


static GtkWidget* CreateDynCtrlMenu ( GtkWidget	*parent,int crt_n, GBRCD 	*rec)
{
	long	delay_time	=10000;		/* 10000 milliseconds	= 10 seconds */
    GtkWidget  *winAlarm;

	DMS_COMMON  dms_cmm ;  
	GtkWidget *vbox,  *draw_area;
	
#ifdef	DEBUG
	printf ("&&&&in PopupDynCtrlMenu\n");
#endif
	
	gchar *pConvert=0;
	ctrl_menu_cur_prompt_rcd= -1;

	menu_box_rect	=GetMenuBoxRect (rec); 
	menu_box_width	= (int)((float)(menu_box_rect.x2 - menu_box_rect.x1)*xscale[crt_n]);
	menu_box_height	=(int)((float)(menu_box_rect.y2 - menu_box_rect.y1)*yscale[crt_n]); 
/*	gtk_set_locale();*/
	
    winAlarm	=	gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	
    gtk_window_set_transient_for(GTK_WINDOW(winAlarm),GTK_WINDOW(parent));
    gtk_window_set_destroy_with_parent(GTK_WINDOW(winAlarm),TRUE);   
    
    
	gtk_window_set_modal(GTK_WINDOW(winAlarm),TRUE);   
	
    gtk_widget_realize(GTK_WIDGET(winAlarm)) ;  
	gtk_widget_set_usize(GTK_WIDGET(winAlarm),menu_box_width, menu_box_height);
   	gtk_window_set_policy(GTK_WINDOW(winAlarm),FALSE,FALSE,FALSE); 
	
	
	gtk_window_set_title(GTK_WINDOW(winAlarm), 
		pConvert=_toUtf8(rec->lnk.dbname));
	G_SAFE_FREE(pConvert);//dzy1017
	
   	gtk_signal_connect(GTK_OBJECT(winAlarm),
		"destroy",
		GTK_SIGNAL_FUNC(DestroyMenuBox),   
		     GINT_TO_POINTER  (crt_n));
	
	
	gtk_window_set_position(GTK_WINDOW(winAlarm),GTK_WIN_POS_CENTER);
	
    vbox	=	gtk_vbox_new(FALSE,0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER( winAlarm), vbox); 	
    gtk_widget_show(vbox);
	
	/************************  canvas generate code start here!!!!*********************/
	
	draw_area = gtk_drawing_area_new();
	
	gtk_widget_set_events(draw_area, GDK_EXPOSURE_MASK  
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_BUTTON_RELEASE_MASK
		| GDK_POINTER_MOTION_MASK );
			 
	
	
  	 gtk_signal_connect (GTK_OBJECT (draw_area),"expose_event",GTK_SIGNAL_FUNC(CtrlMenuExposeCB),rec) ; /*  0 --crt_n */ 
 
	 gtk_signal_connect (GTK_OBJECT (draw_area), "realize",    GTK_SIGNAL_FUNC (ctrl_menu_realize),GINT_TO_POINTER (crt_n));
	 
	 gtk_signal_connect (GTK_OBJECT (draw_area),"motion_notify_event",
		 GTK_SIGNAL_FUNC(ctrl_menu_motion_notify_callback),GINT_TO_POINTER(rec)) ;   
   	 gtk_box_pack_start (GTK_BOX (vbox),draw_area, TRUE, TRUE,0); 
	 
	 
	 
   	 gtk_widget_realize(GTK_WIDGET(draw_area)) ;  
	 gtk_widget_show (GTK_WIDGET(draw_area));
	 gtk_signal_connect(GTK_OBJECT (draw_area), "button_press_event",
		 GTK_SIGNAL_FUNC (ctrl_menu_button_press),GINT_TO_POINTER (crt_n));
	 gtk_signal_connect  (GTK_OBJECT (draw_area), "button_release_event",
		 GTK_SIGNAL_FUNC (ctrl_menu_button_release),GINT_TO_POINTER (crt_n));
	 if(menu_timeout_id[crt_n])
	 {
		 gtk_timeout_remove(menu_timeout_id[crt_n]);
		 menu_timeout_id[crt_n]= 0;
	 }
	 
	 menu_timeout_id[crt_n]	=gtk_timeout_add (  delay_time, (GtkFunction)ctrl_menu_quit, (gpointer)crt_n);
	 tag_rcd[crt_n]->bck.tag	= ITEM_TAG_ON;
	 
	 /****************************************************************/
	 if ( ReadEntryByNameStr ( rec->lnk.dbname,&dms_cmm ) != -1 )
	 {                                    
		 if(dms_cmm.fixed.iost.ctrlLock!=YES )
		 {
			 FormCtrlLockPacketAndBrdcst (crt_n,&dms_cmm,YES );   
		 }
	 }
	 ctrl_menu_popup		= ITEM_TAG_ON;
	 gtk_widget_show (GTK_WIDGET(winAlarm));
	 return (winAlarm);
	 
}

void FormCmdWarning(int crt_n,  char    *point_id,int cmd_type)
{
	char    message[40];
	
	
	switch( cmd_type)
	{
	case C_UNIT:
		strcpy( message , " StarUnit");
		break;
	default:
		strcpy( message , " ");		
		break;
	}
	
}

int GetInterlockName(char* dbname, GBRCD *rcd, char* interlockname)
{
	char	*ptr= interlockname;

	ptr +=sprintf(ptr, "%s.", dbname); 
	switch(rcd->lnk.act.fmt_def.prog.id)
	{
	case	PrgN_C_UNIT:
		ptr +=sprintf(ptr, "%s.", "C_UNIT");
		break;
	case	PrgN_C_BREAKER:
		ptr +=sprintf(ptr, "%s.", "C_BREAKER");
		break;
	case	PrgN_C_SWITCH:
		ptr +=sprintf(ptr, "%s.", "C_SWITCH");
		break;
	case	PrgN_C_APOWER:
		ptr +=sprintf(ptr, "%s.", "C_APOWER");
		break;
	case	PrgN_C_RPOWER:
		ptr +=sprintf(ptr, "%s.", "C_RPOWER");
		break;
	case	PrgN_C_DEVICE:
		ptr +=sprintf(ptr, "%s.", "C_DEVICE");
		break;
	case	PrgN_C_GATE:
		ptr +=sprintf(ptr, "%s.", "C_GATE");
		break;
	case	PrgN_C_TRANS:
		ptr +=sprintf(ptr, "%s.", "C_TRANS");
		break;
	case	PrgN_C_VOLT_STEP:
		ptr +=sprintf(ptr, "%s.", "C_VOLT_STEP");
		break;
	case	PrgN_C_SPEED_STEP:
		ptr +=sprintf(ptr, "%s.", "C_SPEED_STEP");
		break;
	case	PrgN_C_ONOFF_FORCE:
		ptr +=sprintf(ptr, "%s.", "C_ONOFF_FORCE");
		break;
	case	PrgN_C_ANA_FORCE:
		ptr +=sprintf(ptr, "%s.", "C_ANA_FORCE");
		break;
	case	PrgN_OP_VALUE_SET:
		ptr +=sprintf(ptr, "%s.", "C_OP_VALUE_SET");
		break;
	case	PrgN_C_SYS_MODE_SET:
		ptr +=sprintf(ptr, "%s.", "C_SYS_MODE_SET");
		break;
	default:
		return -1;		
	}
	ptr +=sprintf(ptr, "%s", rcd->lnk.act.fmt_def.prog.status);
	return 0;
}

static int menu_entry_disable[MAX_MENU_OBJECTS];
void DrawCtrlMenuCanvas (GtkWidget	*w, GBRCD 		*rec )
{
	GBRCD	rcd;
	
	int	i;
	int	menu_id, menu_nextpos;
	char	*point_id;
	int	cmd_type ; 
	int	crt_n;
	int	disable_flag;
	
	GetCrtNumberOfWgt ( w, &crt_n );  
	
	if( crt_n < 0  || crt_n >= MAX_CRT_NUM)
		return; 
	
	cmd_type = rec->lnk.dyn.dio_type;
	point_id = rec->lnk.dbname;
	GetPtIdByNameStr((char*)&rec->lnk.dbname,&pnt_tab_ptr[crt_n]);
	
	if( cmd_type <= PROJ_CMD_TYPE_MAX && cmd_type >= PROJ_CMD_TYPE_MIN)
	{
		FormCmdWarning( crt_n, point_id, cmd_type);
	}
	menu_id	= rec->lnk.act.fmt_def.menu.id;
	
	menu_nextpos	= menudb.menu[menu_id].nextpos;	
	for (i=0; i<menu_nextpos; i++)
	{
		disable_flag= 0;
		menu_entry_disable[i]= 0;
		rcd	= menudb.menu[menu_id].rcd[i];

		rcd.bck.rect.x1	= rcd.bck.rect.x1 - menu_box_rect.x1;
		rcd.bck.rect.y1	= rcd.bck.rect.y1 - menu_box_rect.y1;
		rcd.bck.rect.x2	= rcd.bck.rect.x2 - menu_box_rect.x1;
		rcd.bck.rect.y2	= rcd.bck.rect.y2 - menu_box_rect.y1;
		
		if(rcd.lnk.act.ctrl_type==CTRL_REQ_PROG)
		{
			char	interlock_name[256];

			if(0==GetInterlockName(rec->lnk.dbname, &rcd, interlock_name))
			{
				if(0==InterLock_Judge(interlock_name))
				{
					disable_flag= 1;
					menu_entry_disable[i]= 1;
					rcd.bck.fg= GetColorId(0x606060);
				}
			}
		}
		if (rcd.bck.draw_type != DrwT_SYMBOL)
		{ 
			if ( rcd.lnk.dyn.dyn_tag == DYN_TAG_ON  
				||( rcd.lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd.lnk.cgrp.cgrp_id !=0) ) 
			{
				
				DrawDynamicRcd ( crt_n,&rcd, info[crt_n].ctrl_gc, ctrl_menu_canvas[crt_n]->window );     
				
			}else
				DrawRcd ( crt_n, ctrl_menu_canvas[crt_n]->window, info[crt_n].ctrl_gc, &rcd.bck ,FALSE); 
		}else
		{ 
			DrawSymbol ( crt_n, ctrl_menu_canvas[crt_n]->window, info[crt_n].ctrl_gc, &rcd.bck, BACKGROUND ,FALSE, disable_flag); 
		}
		
// 		if(disable_flag)
// 		{
// 			int		tmp;
// 
// 			rcd.bck.draw_type= DrwT_LINE;
// 			rcd.bck.line_width= 4;
// 			rcd.bck.fg= GetColorId(0x000080);
// 			rcd.bck.rect.x1 += 10;
// 			rcd.bck.rect.x2 -= 10;
// 			rcd.bck.rect.y2 -= 5;
// 			rcd.bck.rect.y1 += 5;
// 			DrawRcd ( crt_n, ctrl_menu_canvas[crt_n]->window, info[crt_n].ctrl_gc, &rcd.bck ,FALSE);
// 			tmp= rcd.bck.rect.y1;
// 			rcd.bck.rect.y1= rcd.bck.rect.y2;
// 			rcd.bck.rect.y2= tmp;
// 			DrawRcd ( crt_n, ctrl_menu_canvas[crt_n]->window, info[crt_n].ctrl_gc, &rcd.bck ,FALSE);
// 		}
	}

	if (ctrl_menu_cur_prompt_rcd>=0)
		CtrlMenuDrawPromptFrame(crt_n, &menudb.menu[rec->lnk.act.fmt_def.menu.id].rcd[ctrl_menu_cur_prompt_rcd]);

}

GBRCD *GetCtrlActFromCtrlMenu (int	 crt_n, GtkWidget  *w, int x1, int y1, int x2, int y2, MENU	*menu)
{
	int     i;
	int     point_offset    =2;
	int     ctrl_act_found;
	int     xtmp, ytmp;
	int     width, height;
	GBRCD 	*rcd;
	
	ctrl_act_found  =ITEM_TAG_OFF;
	
	if (( abs(x1-x2) <= point_offset || abs(y1-y2) <= point_offset))  
	{
		x2 = x1;
		y2 = y1;
		for (i=menu->nextpos-1 ; i >= 0; i--)
		{
			rcd     = &menu->rcd[i];
			
			if (rcd->lnk.act.ctrl_type != CTRL_REQ_NULL && 
				rcd->lnk.act.ctrl_tag == CTRL_TAG_ON)
			{
				switch ( rcd->bck.draw_type )
				{
				case    DrwT_TEXT:
				case    DrwT_LINE:
				case    DrwT_ARC:
				case    DrwT_RECTANGLE:
				case    DrwT_CIRCLE:
				case    DrwT_SYMBOL:
				case    DrwT_PLINE:
					CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height);
					xtmp	= xtmp - menu_box_rect.x1;
					ytmp	= ytmp - menu_box_rect.y1;
					x1 = (int)(x2/xscale[crt_n]);
					y1 = (int)(y2/yscale[crt_n]);
					if ( (x1 >= xtmp) && (x1 <= xtmp+width) &&
						(y1 >= ytmp) && (y1 <= ytmp+height))
					{
						ctrl_act_found  =ITEM_TAG_ON;
#ifdef	DEBUG
						printf ("in GetCtrlActFromCtrlMenu: i= %d CtrlType= %d\n", i, rcd->lnk.act.ctrl_type );
#endif
					}
					break;
					
				default:
					printf (" OIX: GetCtrlActFromCtrlMenu: DrwType Error.\n");
					break;
				}
			}
			if (ctrl_act_found == ITEM_TAG_ON) break;
		}
	}
	
#ifdef	DEBUG
	printf ("in GetCtrlActFromCtrlMenu: end\n");
#endif
	if (ctrl_act_found == ITEM_TAG_ON /*&& menu_entry_disable[i]!=1*/) 
	{
		ctrl_act_rcd_found[crt_n] = menu->rcd[i];	/* zqh,00,4,2   */
		return ( &(ctrl_act_rcd_found[crt_n]) );
		/*		return  rcd;	*/
	}
	else
	{
		/*deleted by wzg 20090226*/
//		memset( &ctrl_act_rcd_found[crt_n], 0, sizeof(GBRCD) );
		return (GBRCD *)NULL;
	}
}

void ctrl_menu_realize(GtkWidget *widget,gpointer data)
{
	int crt_n ;
	
	crt_n  = (gint) data ; 
    ctrl_menu_canvas[crt_n] = widget;
	info[crt_n].ctrl_gc = gdk_gc_new(widget->window);
	
	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]);	
}



int  IsInSelCtrlMenuRCD(int crt_n, int  X, int Y, GBRCD  *rec ) 
{
	int	i; 
	GBRCD	*rcd; 
	int	inRcdFlag, menu_nextpos;
	int	x1, y1, width, height, xtmp, ytmp;
	
	menu_nextpos= menudb.menu[rec->lnk.act.fmt_def.menu.id].nextpos;	
	for (i=0; i<menu_nextpos; i++)
	{
		rcd	= &menudb.menu[rec->lnk.act.fmt_def.menu.id].rcd[i];
		inRcdFlag= 0;
		switch (rcd->bck.draw_type)
		{
		case	DrwT_TEXT:
		case	DrwT_ARC:
		case	DrwT_RECTANGLE:
		case	DrwT_CIRCLE:
		case	DrwT_SYMBOL:
		case	DrwT_PLINE:
		case    DrwT_LINE:
		case	DrwT_GIF:
			CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height);
			xtmp	= xtmp - menu_box_rect.x1;
			ytmp	= ytmp - menu_box_rect.y1;
			x1 = (int)(X/xscale[crt_n]);
			y1 = (int)(Y/yscale[crt_n]);
			if ( (x1 >= xtmp) && (x1 <= xtmp+width) &&
				(y1 >= ytmp) && (y1 <= ytmp+height))
				inRcdFlag= 1;
			break;
			
		default:					
			break;
			
		}   /*    end switch    */

		if(inRcdFlag==1 && rcd->lnk.act.ctrl_type != CTRL_REQ_NULL 
			&& rcd->lnk.act.ctrl_tag == CTRL_TAG_ON /*&& menu_entry_disable[i]!=1*/)
			return i;
	}  /*   end for    */
	return -1 ;  /*  default value   */
}


void   CtrlMenuGetRCDbyIndex(int crt_n,GBRCD *rec, XRECT *rc)
{
	rc->x1  = MIN(rec->bck.rect.x1, rec->bck.rect.x2) ;
	rc->y1  = MIN(rec->bck.rect.y1, rec->bck.rect.y2) ;
	rc->x2  = MAX(rec->bck.rect.x1, rec->bck.rect.x2) ;
	rc->y2  = MAX(rec->bck.rect.y1, rec->bck.rect.y2) ;
	
}


void CtrlMenuDrawPromptFrame(int crt_n, GBRCD *rec)
{
	BACKRCD	rcd;
    XRECT	rc;

	CtrlMenuGetRCDbyIndex( crt_n, rec, &rc); 
	
	rcd.draw_type = DrwT_RECTANGLE ;
	rcd.fg = ColorId3to4(WHITE) ; 					 
	rcd.fill.fill_type  = UNFILLED ;
	rcd.line_width	= 3;
	rcd.rect.x1 = rc.x1  ; rcd.rect.y1 = rc.y1 ;
	rcd.rect.x2 = rc.x2  ; rcd.rect.y2 = rc.y2  ;

	rcd.rect.x1	= rcd.rect.x1 - menu_box_rect.x1;
	rcd.rect.y1	= rcd.rect.y1 - menu_box_rect.y1;
	rcd.rect.x2	= rcd.rect.x2 - menu_box_rect.x1;
	rcd.rect.y2	= rcd.rect.y2 - menu_box_rect.y1;
	
	DrawRcd ( crt_n, ctrl_menu_canvas[crt_n]->window, info[crt_n].ctrl_gc, &rcd ,FALSE);
}

#define HDL_OFFSET 4

void CtrlMenuErasePromptFrame(int crt_n, GBRCD *rec)
{
    XRECT	rc;
    GdkRectangle rect;

	CtrlMenuGetRCDbyIndex( crt_n, rec, &rc) ;
	
	rect.x =  rc.x1 - 2*HDL_OFFSET - menu_box_rect.x1 ;//
	rect.y =  rc.y1 - 2*HDL_OFFSET - menu_box_rect.y1;
	rect.width  =  rc.x2 - rc.x1 + 4*HDL_OFFSET;
	rect.height =  rc.y2 - rc.y1 + 4*HDL_OFFSET;
	
	rect.x	=		(int)(rect.x*xscale[crt_n]);
	rect.y	=		(int)(rect.y*yscale[crt_n]);
	rect.width =	(int)(rect.width*xscale[crt_n]);
	rect.height=	(int)(rect.height*yscale[crt_n]);	
	gdk_window_invalidate_rect( ctrl_menu_prompt_widget->window, &rect, FALSE);   
}


void  ctrl_menu_motion_notify_callback(GtkWidget *widget,GdkEventMotion *event,gpointer data)
{
    int     xpos, ypos	,crt_n ;
	GBRCD	*rec= ( GBRCD* )data ;
	
	GetCrtNumberOfWgt ( widget, &crt_n );  
	if( crt_n < 0  || crt_n >= MAX_CRT_NUM)
		return; 
	
	ctrl_menu_prompt_widget= widget;
	
    ctrl_menu_last_prompt_rcd = ctrl_menu_cur_prompt_rcd;
    xpos	= (int) (event->x);
    ypos	= (int) (event->y);
	
	ctrl_menu_cur_prompt_rcd = IsInSelCtrlMenuRCD(crt_n,xpos,ypos,rec);
	
	if (ctrl_menu_last_prompt_rcd>=0 && ctrl_menu_last_prompt_rcd != ctrl_menu_cur_prompt_rcd)
		CtrlMenuErasePromptFrame(crt_n, &menudb.menu[rec->lnk.act.fmt_def.menu.id].rcd[ctrl_menu_last_prompt_rcd]);	

	if (ctrl_menu_cur_prompt_rcd>=0 && ctrl_menu_last_prompt_rcd != ctrl_menu_cur_prompt_rcd)
		CtrlMenuDrawPromptFrame(crt_n, &menudb.menu[rec->lnk.act.fmt_def.menu.id].rcd[ctrl_menu_cur_prompt_rcd]);
}



/****************************  expose       *******************************/

void  CtrlMenuExposeCB(	GtkWidget *widget, 
					   GdkEventExpose *event,    
					   gpointer data)
{
	
	GBRCD *rec;
	
#ifdef DEBUG	
	
	g_message(":void  CtrlMenuExposeCB(@@@@@@@:");
#endif
	rec = ( GBRCD* )data ;
	DrawCtrlMenuCanvas (widget, rec);    
	
}



gint ctrl_menu_button_press( GtkWidget *w ,GdkEvent *event , gpointer data ) 
{
    int x ,y ; 
    int crt_n = 0 ;
/*
        int	width, height;
        int	xtmp, ytmp;*/
    
    int	bell_volume	= 30;
	
    
	
    if (event->type == GDK_BUTTON_PRESS) {  /*  GDK_2BUTTON_PRESS ---double clicked !!!! */
        GdkEventButton *bevent = (GdkEventButton *) event; 
        if ((bevent->button == 1))   /* button 1 pressed   */
        {
			x =(int) (bevent->x) ; y =(int) (bevent->y) ;
			g_message("Button press and x is %d , y is %d   ",
				x ,y );
			btn_rect.x1	=x;
			btn_rect.y1	=y;
			can_pressed = TRUE; 
			
			return TRUE;
			
        }
        
    }
	
	
    return FALSE;
}

gint 	ctrl_menu_button_release( GtkWidget *w ,GdkEvent *event , gpointer data) 
{
	int x ,y ;  	  
	int    crt_n;
	GBRCD	*tmp_rcd;

	crt_n   = (gint)data;

	if (event->type == GDK_BUTTON_RELEASE) 
	{  /*  GDK_2BUTTON_PRESS ---double clicked !!!! */
        GdkEventButton *bevent = (GdkEventButton *) event; 
        if ((bevent->button == 1))   /* button 1 pressed   */
        {
			x = (int)(bevent->x) ; 
			y = (int)(bevent->y) ;
			
			btn_rect.x2	= x;
			btn_rect.y2	= y;
			
			if( ctrl_rcd[crt_n] != (GBRCD *) NULL)
			{
				if( ctrl_rcd[crt_n]->lnk.act.ctrl_type == CTRL_REQ_PROG)
				{
					strcpy(ctrl_rcd_menu_p[crt_n].lnk.dbname, ctrl_rcd_parent[crt_n].lnk.dbname);//wzg20090310
					ctrl_rcd_menu_p[crt_n].lnk.dyn.data_class= ctrl_rcd_parent[crt_n].lnk.dyn.data_class;//wzg20090310

					tmp_rcd	= GetCtrlActFromCtrlMenu (crt_n, w,   
						btn_rect.x1, btn_rect.y1, btn_rect.x2, btn_rect.y2, 
						&menudb.menu[ctrl_rcd_parent[crt_n].lnk.act.fmt_def.menu.id]);
				}
				else
				{
					tmp_rcd	= GetCtrlActFromCtrlMenu (crt_n, w, 
						btn_rect.x1, btn_rect.y1, btn_rect.x2, btn_rect.y2, 
						&menudb.menu[ctrl_rcd[crt_n]->lnk.act.fmt_def.menu.id]);
				}
			}
			else
			{
				return FALSE;
			}
			/* after searched new ctrl_rcd   */			
			if( tmp_rcd != (GBRCD *) NULL)
			{
				strcpy( tmp_rcd->lnk.dbname, ctrl_rcd_menu_p[crt_n].lnk.dbname);
				tmp_rcd->lnk.dyn.data_class = ctrl_rcd_menu_p[crt_n].lnk.dyn.data_class; /* this is for parmter modify */	
				
				/********   UnManageWidget *******/
				if (can_pressed)
				{	
					if (ctrl_menu_shell[crt_n] !=(GtkWidget*) NULL)
						ctrl_menu_quit(crt_n);
					can_pressed = FALSE;
				}

				if (tmp_rcd->lnk.act.ctrl_type != CTRL_REQ_NULL || 
					tmp_rcd->lnk.act.ctrl_tag == CTRL_TAG_ON)
				{
					CtrlActHandler ( crt_n, tmp_rcd);
				}
			}
			else
			{
				return	FALSE; /*hcl 070918*/
			}
			
			return TRUE;
        }
	}       		
	
	return TRUE;
}
