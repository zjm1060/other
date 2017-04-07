
/*-----------------------------------------------------------------------

  Update from X11 to GTK ;
  Created by chi.hailong
  Date : 2005.9.17 
  WINDOWS HAVE SYN TO UNIX by chi 2007.11.23 ; 
  
---------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"
#include	"../inc/gtk_view_def.h"
extern int fault_redisplay_on[MAX_CRT_NUM] ; 
#define HDL_OFFSET 4



char    *font_name[]    = {
	"-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-b&h-lucidatypewriter-medium-r-normal-asian-14-140-75-75-m-70-iso8859-1 ",
		
		"-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-adobe-times-medium-r-normal--17-120-100-100-p-84-iso8859-1",
		
		"-dt-kai-medium-r-normal--18-160-75-75-m-160-gbk-0,-adobe-times-medium-r-normal--20-140-100-100-p-96-iso8859-1",
		
		"-dt-kai-medium-r-normal--22-200-75-75-m-200-gbk-0, -adobe-times-medium-r-normal--25-180-100-100-p-125-iso8859-1",
		
		"-dt-kai-medium-r-normal--26-240-75-75-m-240-gbk-0,-adobe-times-medium-r-normal--34-240-100-100-p-170-iso8859-1" ,
		
		"-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-b&h-lucidatypewriter-medium-r-normal-asian-14-140-75-75-m-70-iso8859-1 ",
		
		"-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-adobe-times-medium-r-normal--17-120-100-100-p-84-iso8859-1",
		
		"-dt-kai-medium-r-normal--18-160-75-75-m-160-gbk-0,-adobe-times-medium-r-normal--20-140-100-100-p-96-iso8859-1",
		
		"-dt-kai-medium-r-normal--22-200-75-75-m-200-gbk-0, -adobe-times-medium-r-normal--25-180-100-100-p-125-iso8859-1",
		
		"-dt-kai-medium-r-normal--26-240-75-75-m-240-gbk-0,-adobe-times-medium-r-normal--34-240-100-100-p-170-iso8859-1"
};




GBRCD	*GetActCtrlItemRcd ();
int	SearchMenuId();
void	ClearItemTag (int );

extern  int  operate_flag;
extern  int selfkill_timer_id  ;  /*kick off super user !!!*/
extern GtkWidget  *popmenu1; 
extern gint num_screen;
extern void	DrawDynamicDsp(int crt_n, GdkDrawable	*window );
void	CtrlActHandler (int crt_n, GBRCD *rcd);
void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	UsrLogout ( int crt_n );
extern void	ClearArea (int crt_n,  GdkDrawable	*drawable,int x1,int y1,int width,int height,int expose_mode );
extern void	DrawDspBackground (int crt_n,  GdkDrawable *drawable );
extern void	ZoomOut (int crt_n, GdkPoint  *event );
extern void	ZoomIn (int	crt_n, GdkPoint  *event );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern int	SXtoX (int crt_n,int sx);
extern int	SYtoY (int crt_n,int sy);
extern void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );
extern void FormCtrlLockPacketAndBrdcst(int crt_n,DMS_COMMON *int_data,int flag);
extern int OpenDspFile (int crt_n, char	*filename);
extern BOOL OpenSubDspFile ( crt_n, filename);
extern void	ProgHandler (int crt_n,GBRCD	*rcd );
extern gint CheckCtrlMenu(int crt_n, GBRCD 	*rec);
extern void PopupDynCtrlMenu (int crt_n,GtkWidget	*parent,  GBRCD 	*rec);
extern int	XtoSX (int crt_n,int x);
extern int	YtoSY (int crt_n,int y);
extern char* GetActCtrlItemDbname (int crt_n, int x1,int y1,int x2,int y2 );
extern int ColorId3to4(int id3);
extern void	CheckZoomOrigin (int crt_n );

extern gboolean isRealCurvePt(const POINTER* pt);	//判断画面点是否是实时曲线记录点

extern void    RedrawView (GtkWidget *w);
extern int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] );
void    Redraw (GtkWidget *w)
{
	int	crt_n;
	/*GdkGC *gc ;*/
	
	GetCrtNumberOfWgt ( w, &crt_n);  
	
	if( crt_n < 0 || crt_n >= MAX_CRT_NUM)
		return;
	
	/* printf(" in Redraw : wid is %d  height is %d @@\n" ,canvas_width, canvas_height);*/
	
	ClearArea (crt_n,  GDK_DRAWABLE(bakgrd_pixmap[crt_n]), 0, 0, Canvas_RECT[crt_n].width+200, Canvas_RECT[crt_n].height+400, k_no_expose);  
	DrawDspBackground (crt_n, bakgrd_pixmap[crt_n]);  	
	//gdk_draw_pixmap(pixmap[crt_n], info[crt_n].gc,  bakgrd_pixmap[crt_n],0,0, 0, 0, 1280, 1200  );
	gdk_draw_pixmap(pixmap[crt_n], info[crt_n].gc,  bakgrd_pixmap[crt_n],0,0, 0, 0, LOCAL_WIDTH, LOCAL_HEIGHT  );
	
	DrawDynamicDsp (crt_n, pixmap[crt_n]);    
	//gdk_draw_pixmap(canvas[crt_n]->window, info[crt_n].gc, pixmap[crt_n], 0,0, 0, 0, 1280, 1200 );
	gdk_draw_pixmap(canvas[crt_n]->window, info[crt_n].gc, pixmap[crt_n], 0,0, 0, 0, LOCAL_WIDTH, LOCAL_HEIGHT );
	
}

void zoom_scroll_event(GtkWidget*widget,   GdkEventScroll *event, gpointer  data)  
{   
	int crt_n  = (int) data; 
	GdkPoint  pt ;
	GdkEventButton *bevent = (GdkEventButton *) event;

	pt.x = (int)((float)(int)bevent->x/xscale[crt_n]) ;  

	pt.y = (int)((float)(int)bevent->y/yscale[crt_n]) ;	 

	if (event->direction == GDK_SCROLL_DOWN)
	{
		// down code
		ZoomOut (crt_n, &pt ) ; ; 
		//printf ("---------scroll++++++++++++++++++++1\n");
	}
	else if (event->direction == GDK_SCROLL_UP)
	{
		// up code
		ZoomIn (crt_n, &pt ) ; ; 
		//printf ("---------scroll---------------------1\n");
	}

}

void ZoomAreaMove (int crt_n,int x1,int y1,int x2,int y2)
{
	int	dx, dy;
	
	dx	= x2 - x1;
	dy	= y2 - y1;
	zoomdb[crt_n].sx_orig	= zoomdb[crt_n].sx_orig + dx;
	zoomdb[crt_n].sy_orig	= zoomdb[crt_n].sy_orig + dy;
	CheckZoomOrigin ( crt_n );
	Redraw (canvas[crt_n] );
	if (view_win[crt_n])
		RedrawView (viewdb[crt_n].canvas);

}
gint button_release( GtkWidget *widget,GdkEvent *event , gpointer data)
{
    int x1,y1,x2 ,y2 ; 
    int crt_n ;
    int	bell_volume	= 30;
	int     point_offset    =2;

//    char cur_file_mode[50];
	GdkEventButton *bevent = (GdkEventButton *) event;
	crt_n = (gint)  data ;   
	GetCrtNumberOfWgt ( widget, &crt_n);  
	
    if (crt_n <0  || crt_n >= MAX_CRT_NUM)
		return FALSE;
	
	 
	if (event->type ==  GDK_BUTTON_RELEASE	) 
	{
		
		x1=cur[crt_n].bck.rect.x1;
		y1=cur[crt_n].bck.rect.y1;
		x2=SXtoX ( crt_n, (int)((float)(int)bevent->x/xscale[crt_n]) );
		y2=SYtoY ( crt_n, (int)((float)(int)bevent->y/yscale[crt_n]) );

        if (( abs(x1-x2) >= point_offset ||abs(y1-y2) >= point_offset))
        {	
			ZoomAreaMove (crt_n,x1,y1,x2,y2);
		}

		return TRUE;

	}
	return FALSE;
}

POINTER GetPointByPressedPos(int crt_n)
{    
	POINTER ptr;
	
    DMS_COMMON	 dms_cmm;
    int xtmp, ytmp;
     int	width, height;
     char   db_name[60];

	 memset(&ptr, 0, sizeof(POINTER));
     memset(db_name,0, sizeof(db_name) ); 

  	if( crt_n < 0 )
  		return ptr; 
  
        cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)info[crt_n].pt.x/xscale[crt_n]));  /*info[crt_n].pt.x*/
        cur[crt_n].bck.rect.y1	= SYtoY ( crt_n, (int)((float)(int)info[crt_n].pt.y/yscale[crt_n]));   /*info[crt_n].pt.y*/
        cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
        cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
        
        CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
		if (GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height)) 
			strcpy(db_name,GetActCtrlItemDbname (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height));  
        else
		{
			return  ptr;
        }
        
		if(db_name[0])
        {
			if ( ReadEntryByNameStr (  db_name, &dms_cmm ) == -1 )
			{
				return  ptr;
			}

			return dms_cmm.point;                  
        }        
        
      return ptr;         
}

gint button_press( GtkWidget *widget,GdkEvent *event , gpointer data)
{
    int x ,y ; 
    int crt_n ;
    int	width, height;
    int	xtmp, ytmp;
    int	bell_volume	= 30;
    
	GtkItemFactory* pItemFactory;
	GtkWidget*	pMenuItem;
	POINTER ptr;
	gchar strPath[128];

    GdkPoint  pt ;
    DMS_COMMON dms_cmm;
    char cur_file_mode[50];
	crt_n = (gint)  data ;   
	GetCrtNumberOfWgt ( widget, &crt_n);  
	
    if (crt_n <0  || crt_n >= MAX_CRT_NUM)
		return FALSE;
	
    if (event->type == GDK_BUTTON_PRESS) {  /*  GDK_2BUTTON_PRESS ---double clicked !!!! */
        GdkEventButton *bevent = (GdkEventButton *) event; 
        info[crt_n].pt.x =  pt.x =(int) (bevent->x) ;
        info[crt_n].pt.y =  pt.y =(int) (bevent->y) ;
        if (bevent->button == 3)  
        {	
			GtkWidget  *popupmenu  = g_object_get_data(G_OBJECT(main_w[crt_n]),"PopMenu");
			printf("popupmenu=%x\n", popupmenu );
			if (popupmenu != NULL )
			{
				pItemFactory=gtk_item_factory_from_widget(popupmenu);
				if (pItemFactory != NULL)
				{
					sprintf(strPath, "%s%s\0", "<main>", _toUtf8EX("OIX_CSTR_MENU_REAL_CURVE"));
					pMenuItem=gtk_item_factory_get_widget(pItemFactory, strPath);
					if (pMenuItem != NULL)
					{
						ptr=GetPointByPressedPos(crt_n);
						gtk_widget_set_sensitive(pMenuItem, isRealCurvePt(&ptr));
					}
				}
				
				gtk_menu_popup (GTK_MENU (popupmenu), NULL, NULL, NULL, NULL,
				bevent->button, bevent->time);	
			}
        }
		else if(bevent->button == 2)
		{
			pt.x = (int)((float)(int)bevent->x/xscale[crt_n]) ; 
			
			pt.y = (int)((float)(int)bevent->y/yscale[crt_n]) ;	
			
			if (bevent->state & GDK_SHIFT_MASK)
				ZoomOut (crt_n, &pt ) ;
            else
				ZoomIn (crt_n, &pt ) ;			
		} 
        else if ((bevent->button == 1))   /* button 1 pressed   */
        {   
            if(strcmp(cur_dsp_name[crt_n],"")!=0)
            {
                strcpy( cur_file_mode, strrchr( cur_dsp_name[crt_n], '.' )+1 );            
                if(strcmp(cur_file_mode,"sfbin")==0)
                    PopupMessageBox(main_w[crt_n],_CS_(MSG_THIS_IS_SFTCP_DSP));
            }
            
            x =(int) (bevent->x) ; y =(int)(bevent->y) ;
			/*g_message("x is %d , y is %d ,x_root is %d ,y_root is %d ,button is %d ,  crt_n is :%d ",
			x ,y,(int)bevent->x_root,(int)bevent->y_root,(int)bevent->button,(gint)data ); */
			
            cur[crt_n].bck.rect.x1	= SXtoX ( crt_n, (int)((float)(int)bevent->x/xscale[crt_n]) );   /*(int)bevent->x*/
            cur[crt_n].bck.rect.y1	= SYtoY ( crt_n,(int)((float)(int)bevent->y/yscale[crt_n])) ;    /*(int)bevent->y)*/
            cur[crt_n].bck.rect.x2	= cur[crt_n].bck.rect.x1;
            cur[crt_n].bck.rect.y2	= cur[crt_n].bck.rect.y1;
            CheckRectCoords (cur[crt_n].bck.rect, &xtmp, &ytmp, &width, &height);
            ctrl_rcd[crt_n]	= GetActCtrlItemRcd (crt_n,  xtmp, ytmp, xtmp+width, ytmp+height, &gdbase[crt_n]);        	
			
			
			if ( tag_rcd[crt_n] != (GBRCD *)NULL)
			{
				switch( ptr_tab_rcd_tag[crt_n])
				{
				case	PTR_TAB_RCD_AVAILABLE:
					ptr_tab_rcd[crt_n]	= ctrl_rcd[crt_n];
					break;
					
				default:
					break;
				}
				
				if ( ReadEntryByNameStr ( ctrl_rcd[crt_n]->lnk.dbname,&dms_cmm ) != -1 )
				{       
					if(dms_cmm.fixed.iost.ctrlLock!=YES)
					{
						if(tag_rcd[crt_n]->bck.tag == ITEM_TAG_ON )   
						{
							printf("!!!!!in button_press\n");
							FormCtrlLockPacketAndBrdcst (crt_n,&dms_cmm,YES ); 
						}
					}
					else
					{
						PopupMessageBox(main_w[crt_n],MSG_POINT_LOCKED);
						return FALSE;
					}
				}
				
#ifdef	DEBUG
				printf(" tag_rcd.act.ctrl_type=%d, tag_rcd.act.ctrl_tag=%d\n", 
					ctrl_rcd[crt_n]->lnk.act.ctrl_type, ctrl_rcd[crt_n]->lnk.act.ctrl_tag);
#endif
				
				CtrlActHandler ( crt_n, ctrl_rcd[crt_n] );
			}
			else
			{
#ifdef	DEBUG
				printf("OIX: tag_rcd is empty!\n");
#endif
			}      
			
			return FALSE;
			
        }        
    }   
    return FALSE;
}



void  expose_events(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
{
	
	int crt_n = (gint)data ;      
	if( crt_n < 0  || crt_n > MAX_CRT_NUMBER )	return;                       
	/* 
		canvas_width		 = widget->allocation.width  ;
		canvas_height 		 = widget->allocation.height ;  				
	
	*/ 
  		
	
	gdk_gc_set_clip_rectangle(widget->style->fg_gc[widget->state], &event->area );  
 			
	gdk_draw_pixmap(widget->window, info[crt_n].gc  ,pixmap[crt_n]  ,event->area.x ,event->area.y , 
		event->area.x ,event->area.y ,event->area.width, event->area.height );
	
	gdk_gc_set_clip_rectangle(widget->style->fg_gc[widget->state],NULL );
#ifdef DEBUG
	printf ("expose event %d is occur!!\n", crt_n);
#endif  
}



void	CtrlActHandler (int crt_n, GBRCD *rcd)
{
	char	dsp_name[50];
	DMS_COMMON dms_cmm;
	int res ; 
        
#ifdef	DEBUG
		printf("\nNow in CtrlActHandler!\n");
#endif
	
printf("\nCCCC: %d", rcd->lnk.act.ctrl_type);	
    if (fault_redisplay_on[crt_n] == 1 )
	{
		PopupMessageBoxRet(main_w[crt_n], "Fault redisplay being on  ,Please turn off it!!!") ; 
		return ; 
	}


	switch	( rcd->lnk.act.ctrl_type )
	{
	case	CTRL_REQ_DSP:
		if( rcd->lnk.act.fmt_def.dsp.name != ( char* )NULL )
		{
			if ( strlen(rcd->lnk.act.fmt_def.dsp.name) > DSP_FILE_SHORTNAME_LEN ) 
			{
				printf("OIX: dsp filename is too long, please correct it: %s\n",
					rcd->lnk.act.fmt_def.dsp.name);
				rcd->lnk.act.fmt_def.dsp.name[DSP_FILE_SHORTNAME_LEN-1] = 0;
			}
			strcpy(dsp_name, rcd->lnk.act.fmt_def.dsp.name);
			if(strstr(dsp_name,".dbin")!= (char *)NULL ||strstr(dsp_name,".drbin")!=(char *)NULL||strstr(dsp_name,".mrbin")!=(char *)NULL)
			{
				printf("####dsp_name=%s\n",dsp_name);
				if( TRUE==OpenDspFile ( crt_n, rcd->lnk.act.fmt_def.dsp.name))
				{
					if ( cur_dsp_ptr[crt_n]+1 < MAX_DSP_CNT )
						cur_dsp_ptr[crt_n]++; 
					else
						cur_dsp_ptr[crt_n] = 0;
			 		strcpy (rem_dsp_name[crt_n][cur_dsp_ptr[crt_n]], rcd->lnk.act.fmt_def.dsp.name);
				}
			} 
			else if(strstr(dsp_name,".sbin")!=(char *)NULL)
			{
				/*printf("#### here is sub dsp,dsp_name=%s\n",dsp_name);*/
                                strcpy(sub_dsp_name,rcd->lnk.act.fmt_def.dsp.name);
				if(TRUE!=OpenSubDspFile ( crt_n, rcd->lnk.act.fmt_def.dsp.name))
				{
					char	str[256];
					sprintf(str, "%s\n%s", _CS_(MSG_SBIN_FILE_NOT_FOUND), rcd->lnk.act.fmt_def.dsp.name);
					PopupMessageBox( main_w[crt_n],  str);
				}
                 else
					{
						file_mode[crt_n]=SUB_DSP_FILE;                                                      
                    }
			}

			ClearItemTag ( crt_n );
		}  //end if 
		break;

	case	CTRL_REQ_SUB_DSP:
		if ( strlen(rcd->lnk.act.fmt_def.dsp.name) > DSP_FILE_SHORTNAME_LEN )  
                {
                        printf("OIX: sub_dsp filename is too long, please correct it: %s\n",
                                        rcd->lnk.act.fmt_def.dsp.name);
			rcd->lnk.act.fmt_def.dsp.name[DSP_FILE_SHORTNAME_LEN-1] = 0;
		}
		
		OpenDspFile ( crt_n, rcd->lnk.act.fmt_def.dsp.name);
		ClearItemTag ( crt_n );
		break;

	case	CTRL_REQ_PROG:
		tag_rcd[crt_n]->bck.tag	= ITEM_TAG_ON;
                
           /*     if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) != -1 )
    		{
                        FormCtrlLockPacketAndBrdcst (crt_n,&dms_cmm,YES );                          
                 }*/
               
		if ( strlen( rcd->lnk.act.fmt_def.prog.status) > PROG_NAME_LEN )  
                {
                        printf("OIX: prog status string is too  long, please correct it: %s\n",
				rcd->lnk.act.fmt_def.prog.status );
			rcd->lnk.act.fmt_def.prog.status[PROG_NAME_LEN-1] = 0;
		}
		ProgHandler ( crt_n, rcd ); 		/* gtk ... to be continued */
                
                if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) != -1 )
                {       
                    if(dms_cmm.fixed.iost.ctrlLock!=NO )   /* added by zyp on 2007.7.17*/ 
                        {
                            printf("!!!!in CTRL_REQ_PROG,dbname=%s,dms_cmm.fixed.iost.ctrlLock=%d\n",
                                    rcd->lnk.dbname,dms_cmm.fixed.iost.ctrlLock);
                        FormCtrlLockPacketAndBrdcst (crt_n,&dms_cmm,NO ); 
                    }
                    
                }
		ClearItemTag ( crt_n );
		break;

	case	CTRL_REQ_MENU:
		if ( ctrl_menu_popup == ITEM_TAG_OFF )
		{			
			ctrl_rcd_menu_p[crt_n]	= *rcd;	
			 res =   CheckCtrlMenu(  crt_n,   	rcd) ;
                        if ( res == 0)  /*success!!*/
                            {  /*
                                tag_rcd[crt_n]->bck.tag	= ITEM_TAG_ON;
                                if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) != -1 )
                                {                                    
                                    if(dms_cmm.fixed.iost.ctrlLock!=YES )
                                    {
                                        FormCtrlLockPacketAndBrdcst (crt_n,&dms_cmm,YES );   
                                        printf("\n\n\n");
                                    }
                                }
                                ctrl_menu_popup		= ITEM_TAG_ON;
								*/
							    PopupDynCtrlMenu ( crt_n, main_w[crt_n], rcd);	
                        }
                        else if (res == -1){ 
                           PopupMessageBox ( main_w[crt_n], MSG_CTRL_MENU_NOT_EXITS);   
                        }
                        else 
                            return ;
                }
		else
			gdk_beep();  
		break;

	default:
		ClearItemTag ( crt_n );
		break;
	}
}


void	ClearItemTag (int crt_n)
{
	if ( tag_rcd[crt_n] != NULL)
		tag_rcd[crt_n]->bck.tag	= ITEM_TAG_OFF;
	ctrl_menu_popup	= ITEM_TAG_OFF;
}


/********************************************************  
draw Frame of selected Rcd when point within it ; 
added by chi   
**********************************************************/


void	DrawFrame (int crt_n, GdkWindow	*drawable,GdkGC	*gc, BACKRCD		*rcd ,int isDYN,GtkWidget *widget)
{
	
	int	x, y, width, height;
	BACKRCD	tmp;
    GtkStyle      *blankstyle;
    GdkPoint points[6];
	
	tmp	=  * rcd;
	if (drawable == canvas[crt_n]->window || drawable == bakgrd_pixmap[crt_n]   || drawable == pixmap[crt_n] ) 
	{ 
		tmp.rect.x1	= XtoSX ( crt_n, rcd->rect.x1);
		tmp.rect.y1	= YtoSY ( crt_n, rcd->rect.y1);
		tmp.rect.x2	= XtoSX ( crt_n, rcd->rect.x2);
		tmp.rect.y2	= YtoSY ( crt_n, rcd->rect.y2);
	}
	tmp.rect.x1 =		  (int) (tmp.rect.x1*xscale[crt_n]) ;
	tmp.rect.y1 =		  (int) (tmp.rect.y1*yscale[crt_n]) ;
	tmp.rect.x2 =		  (int) (tmp.rect.x2 *xscale[crt_n]) ;
	tmp.rect.y2 =         (int) (tmp.rect.y2*yscale[crt_n]) ;
	
	if  (rcd->draw_type == DrwT_RECTANGLE )
	{
		CheckRectCoords (tmp.rect, &x, &y, &width, &height );
		points[0].x = x;
		points[0].y = y;
		points[1].x = x + width ;
		points[1].y = y;
		points[2].x = x + width;
		points[2].y = y +  height;
		points[3].x = x ;
		points[3].y = y +  height;
		points[4].x =  x;
		points[4].y =  y;
        
             
		blankstyle = gtk_style_new ();
		blankstyle->bg_gc[GTK_STATE_NORMAL] =
			widget->style->bg_gc[GTK_STATE_NORMAL];
		blankstyle->dark_gc[GTK_STATE_NORMAL] =
			widget->style->bg_gc[GTK_STATE_NORMAL];
		blankstyle->light_gc[GTK_STATE_NORMAL] =
			widget->style->bg_gc[GTK_STATE_NORMAL];
		blankstyle->black_gc =
			widget->style->bg_gc[GTK_STATE_NORMAL];
		blankstyle->depth = gdk_drawable_get_depth(drawable);
		gtk_paint_polygon (blankstyle,
			drawable,
			GTK_STATE_NORMAL,
			GTK_SHADOW_IN,
			NULL,
			widget,
			NULL,
			points, 5,
			FALSE);
		
		g_object_unref (blankstyle);            
		
	}
    
}





/********************************************************  
Capture Motions events of Points 
added by chi   
**********************************************************/

int  IsInSelRCD(int crt_n, int  X, int Y ) 
{
	int	i; 
	GBRCD	*rcd; 
	int	inRcdFlag;
	int	x1, y1, width, height;
	
    if (gdbase[crt_n].nextpos == 0) return -1 ;
	X =  (int)SXtoX(crt_n, (int)(X/xscale[crt_n]));
	Y =  (int)SYtoY(crt_n, (int)(Y/yscale[crt_n]));

	for (i=gdbase[crt_n].nextpos-1 ; i >= 0; i--)
	{
		inRcdFlag= 0;
		rcd	= &gdbase[crt_n].buffer[i];
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
			CheckRectCoords(rcd->bck.rect, &x1, &y1, &width, &height);
			if ( (X >= x1) && (X <= (x1+width)) &&
				(Y >= y1) && (Y <= (y1+height)))
				inRcdFlag= 1;
			break;
			
		default:					
			break;
			
		}   /*    end switch    */

		if(inRcdFlag==1)
		{
			if (/*rcd->lnk.dyn.dyn_tag ==  DYN_TAG_ON*/(rcd->lnk.act.ctrl_type != CTRL_REQ_NULL && 
				rcd->lnk.act.ctrl_tag == CTRL_TAG_ON) ||
				rcd->lnk.dyn.dyn_tag == DYN_TAG_ON ||
				rcd->lnk.cgrp.tag  == DYN_CGRP_TAG_ON ||
				rcd->lnk.cgrp.tag  == DYN_CGRP_TAG_ADD_ON )
				return i;
		}
	}  /*   end for    */
	return -1 ;  /*  default value   */
}


/********************************************************  
Capture Motions events of Points 
added by chi   
**********************************************************/
void   GetRCDbyIndex(int crt_n,int  i,XRECT *rc)
{
	rc->x1  = MIN(gdbase[crt_n].buffer[i].bck.rect.x1  ,gdbase[crt_n].buffer[i].bck.rect.x2) ;
	rc->y1  = MIN(gdbase[crt_n].buffer[i].bck.rect.y1 ,gdbase[crt_n].buffer[i].bck.rect.y2) ;
	rc->x2  = MAX(gdbase[crt_n].buffer[i].bck.rect.x1 ,gdbase[crt_n].buffer[i].bck.rect.x2) ;
	rc->y2  = MAX(gdbase[crt_n].buffer[i].bck.rect.y1 ,gdbase[crt_n].buffer[i].bck.rect.y2) ;
	
}

void  canvas_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data) 
{
	int crt_n = (gint) data ; 
	zoomdb[crt_n].canvas_w	= a->width;
	zoomdb[crt_n].canvas_h	= a->height;
	Canvas_RECT[crt_n].width =  a->width ;
	Canvas_RECT[crt_n].height =  a->height ;
	g_message("\n OIX:canvas_w[%d] %d , canvas_h %d \n",crt_n , a->width ,a->height);
}


/********************************************************  
Capture Motions events of Points 
added by chi   
**********************************************************/

static int   cur_prompt_rcd, last_prompt_rcd;
static GtkWidget *prompt_widget;

void DrawPromptFrame(int crt_n, int prompt_rcd)
{
	BACKRCD	rcd;
    XRECT	rc;
	
	GetRCDbyIndex( crt_n, prompt_rcd, &rc); 
	
	rcd.draw_type = DrwT_RECTANGLE ;
	rcd.fg = ColorId3to4(32) ; 					 
	rcd.fill.fill_type = UNFILLED ;
	rcd.line_width	= 2;
	rcd.rect.x1 = rc.x1  ; rcd.rect.y1 = rc.y1 ;
	rcd.rect.x2 = rc.x2  ; rcd.rect.y2 = rc.y2  ;
	DrawFrame (crt_n,  prompt_widget->window,info[crt_n].gc,&rcd,0,prompt_widget);	
}


void ErasePromptFrame(int crt_n, int prompt_rcd)
{
    XRECT	rc;
    GdkRectangle rect;
	
	GetRCDbyIndex( crt_n, prompt_rcd, &rc) ;
	
	rect.x =  rc.x1 - 2*HDL_OFFSET  ;
	rect.y =  rc.y1 - 2*HDL_OFFSET ;
	rect.width  =  rc.x2 - rc.x1 + 4*HDL_OFFSET;
	rect.height =  rc.y2 - rc.y1 + 4*HDL_OFFSET;
	
	rect.x	=		(int)(rect.x*xscale[crt_n]);
	rect.y	=		(int)(rect.y*yscale[crt_n]);
	rect.width =	(int)(rect.width*xscale[crt_n]);
	rect.height=	(int)(rect.height*yscale[crt_n]);	
	gdk_window_invalidate_rect( prompt_widget->window, &rect, FALSE);   
}


void UpdatePromptFrame(void)
{
	int	crt_n;
	
	if(cur_prompt_rcd<0)
		return;

	GetCrtNumberOfWgt(prompt_widget, &crt_n);
    if( crt_n < 0  || crt_n > MAX_CRT_NUMBER )	
		return; 
	
	DrawPromptFrame(crt_n, cur_prompt_rcd);
}



void  motion_notify_callback(GtkWidget *widget,	GdkEventMotion *event,gpointer data)
{
    int     xpos, ypos	,crt_n ;
	
    crt_n = (gint) data ;		
    if( crt_n < 0  || crt_n > MAX_CRT_NUMBER )
		return; 
	
	prompt_widget= widget;
	
    last_prompt_rcd = cur_prompt_rcd;
    xpos	= (int) (event->x);
    ypos	= (int) (event->y);
	
	cur_prompt_rcd = IsInSelRCD(crt_n,xpos,ypos);

	if (last_prompt_rcd>0 && last_prompt_rcd != cur_prompt_rcd)
		ErasePromptFrame(crt_n, last_prompt_rcd);	
		
	if (cur_prompt_rcd>0 && last_prompt_rcd != cur_prompt_rcd)
		DrawPromptFrame(crt_n, cur_prompt_rcd);
		
	if(selfkill_timer_id!=0) 
	{
		gtk_timeout_remove ( selfkill_timer_id  ); 
		selfkill_timer_id   =gtk_timeout_add (  KICKOFFTIME, (GtkFunction)UsrLogout,(gpointer)crt_n); 
	}
}


/*----------------------------------------------------------------------
!_	
!_	 for the given widget ID.  Update to GTK+
!-----------------------------------------------------------------------*/

void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n )
{	
	
#ifdef SUN_OS	
	GdkScreen   *scr ;
	if (GTK_IS_WIDGET(wgt))
	{
		scr= gtk_widget_get_screen(wgt);
		*crt_n = gdk_screen_get_number(scr);
	}
	else
	{       
		printf ( "in event_p.c:GetCrtNumberofWgt Is Fail!!! \n"   );
		*crt_n = -1 ;
	}  		  
	return;
#endif	
	
#ifdef WINDOWS_OS
	int j;	
	GtkWidget *toplevel;

	if(!GTK_IS_WIDGET(wgt))
	{
		*crt_n=-1;
		return;
	}

    toplevel = gtk_widget_get_toplevel (wgt);	
	
	while(GTK_IS_WIDGET(toplevel)) 
	{
		for(j=0;j<num_screen;j++)
		{
			if(main_w[j]==toplevel)
			{
				*crt_n=j;
				return;
			}
		}
		toplevel=GTK_WIDGET(gtk_window_get_transient_for(GTK_WINDOW(toplevel)));
	}   
	
	printf ( "in event_p.c:GetCrtNumberofWgt Is Fail!!! \n"   );
	*crt_n = -1 ;
	
#endif
	
}

int	SearchMenuId ( char	menu_name[MENU_NAME_LEN])
{
	int	i;
	
	for( i=0; i < menudb.nextpos; i++)
	{
		if( strcmp( menudb.menu[i].name , menu_name) == 0)
		{
			return(i);
			break;
		}
	} 
	return -1 ;
}


