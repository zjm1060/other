
/*-----------------------------------------------------------------------	
Created by zyp
Date : 		2006.7.5   
2007.04.10: SYNC by chichi
LAST UPDATA by hcl 2007.12.09
2008.8.5 by zyp :POL curve wrong

---------------------------------------------------------------------*/


#include 	<stdio.h>
#include	<gtk/gtk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

extern gint button_press( GtkWidget *widget,GdkEvent *event , gpointer data);
/*#define	DEBUG   */
void DrawSubWinCanvas (GtkWidget	*w);
void	SubWinExposeCB (GtkWidget *widget, GdkEventExpose *event, gpointer data);
void 	sub_win_realize(GtkWidget*,gpointer );

extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	ClearItemTag (int crt_n);
extern void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );
extern void	CtrlActHandler (int crt_n, GBRCD *rcd);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void DrawSubDspBackground (int crt_n,GdkDrawable	*drawable,int sub_win_n );
extern void	DrawSubDynamicDsp (int crt_n,GdkDrawable	* drawable ,int sub_win_n);
extern void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n) ;
extern void my_draw_lines(GdkDrawable  *drawable, GdkGC  *gc,GdkPoint *points,	 int npoints ,int HY, int LY   );
extern char *_toUtf8(char *c);
extern char *_toUtf8EX(char *c);

static GtkWidget* CreateDynSubWin ( int crt_n,int sub_win_n,GtkWidget	*parent,SHM_OTS *shm_ots);

gint	sub_win_timeout_id[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];


GtkWidget *ana_curve_dlg[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
GtkWidget *Curve_drawingarea[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
GdkGC  *curve_gc[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
int ana_curve_timeout_id[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];

#define ANA_CURVE_POINT_NUM 180
#define CURVE_HEIGHT    120
#define CURVE_WIDTH     ANA_CURVE_POINT_NUM

typedef struct 
{
	int crt_n;
	int cur_no;	   /*curve_val:0~ANA_CURVE_POINT_NUM*/
	int sub_dlg_no;/*0~MAX_SUB_WIN_NUM*/
	float value[ANA_CURVE_POINT_NUM];
	char db_name[POINT_NAME_SIZE];
}EntryInfo;

void DestroySubWin (GtkWidget	*w,gpointer data)
{
	int	crt_n;
	
	int sub_win_n=(int)data;
	/*  printf("data=%d\n",data);*/
	
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 )
		return;   
	
	if (sub_win_shell[sub_win_n][crt_n] !=(GtkWidget*) NULL)
	{		
		gtk_widget_destroy(sub_win_shell[sub_win_n][crt_n]);
		sub_win_shell[sub_win_n][crt_n] = NULL ;
		gtk_widget_destroy(sub_win_canvas[sub_win_n][crt_n]);
		sub_win_canvas[sub_win_n][crt_n] = NULL ;
		strcpy(sub_win_title[sub_win_n][crt_n],"");
	}
	if(sub_win_timeout_id[sub_win_n][crt_n])
	{
		gtk_timeout_remove(sub_win_timeout_id[sub_win_n][crt_n]);
		sub_win_timeout_id[sub_win_n][crt_n]= 0;
	}
	/*printf("destroy::sub_win_timeout_id[%d][%d]=%d\n",sub_win_n,crt_n,sub_win_timeout_id[sub_win_n][crt_n]);*/
	ClearItemTag ( crt_n );	
}

void SubWinQuitByTimeCall (int  data)
{
	int crt_n ,i; 
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	DestroySubWin (main_w[crt_n], i) ; 	
}

GBRCD	*GetSubActCtrlItemRcd (int crt_n, int x1,int y1,int x2,int y2, int sub_win_n)
{
	int	i;
	int	point_offset	=2;
	int	ctrl_act_found;
	int	xtmp, ytmp;
	int	width, height;
	GBRCD	*rcd;
	
	ctrl_act_found	=ITEM_TAG_OFF;
	
	if (( abs(x1-x2) <= point_offset || abs(y1-y2) <= point_offset ))
	{
		printf("gdb[%d][%d]->nextpos=%d\n",sub_win_n,crt_n,gcondb[sub_win_n][crt_n].nextpos);
		for (i=gcondb[sub_win_n][crt_n].nextpos-1 ; i >= 0; i--)
		{
			rcd	=&gcondb[sub_win_n][crt_n].buffer[i];			
			
			if (rcd->lnk.act.ctrl_type != CTRL_REQ_NULL || 
				rcd->lnk.act.ctrl_tag == CTRL_TAG_ON)	
			{
				printf("ctrl_type=%d,ctrl_tag=%d\n",rcd->lnk.act.ctrl_type,rcd->lnk.act.ctrl_tag);
				switch (rcd->bck.draw_type )
				{
				case	DrwT_TEXT:
				case	DrwT_LINE:
				case	DrwT_ARC:
				case	DrwT_RECTANGLE:
				case	DrwT_CIRCLE:
				case	DrwT_SYMBOL:
				case	DrwT_PLINE:
					CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height );
					xtmp	= xtmp - sub_win[sub_win_n][crt_n].x1;
					ytmp	= ytmp - sub_win[sub_win_n][crt_n].y1;
					printf("xtmp=%d,x1=%d,ytmp=%d,y1=%d\n",xtmp,x1,ytmp,y1);
					if ( (x1 >= xtmp) && (x1 <= xtmp+width) &&
						(y1 >= ytmp) && (y1 <= ytmp+height ))
					{
						ctrl_act_found	=ITEM_TAG_ON;
					}
					break;
					
				default:
					printf ("OIX: GetActCtrlItemRcd: DrwType Error.\n" );
					break;
				}
			}
			
			if ( ctrl_act_found == ITEM_TAG_ON) break;
		}
	}
	
	if ( ctrl_act_found == ITEM_TAG_ON) 
	{
		ctrl_act_rcd_found[crt_n] = gcondb[sub_win_n][crt_n].buffer[i];
		tag_rcd[crt_n] = &gcondb[sub_win_n][crt_n].buffer[i];
		return &(ctrl_act_rcd_found[crt_n]);
	}
	else
	{
#ifdef	DEBUG
		printf ( "OIX:  rcd is not found \n" );
#endif
		memset( &ctrl_act_rcd_found[crt_n], 0, sizeof(GBRCD) );
		tag_rcd[crt_n] = (GBRCD *)NULL;
		return	(GBRCD *)NULL;
	}
}


gint 	subwin_button_press( GtkWidget *w ,GdkEvent *event , gpointer data) 
{
	int x ,y ; 
	
	int    crt_n,sub_win_n;
	XRECT   tmp_rect;
	GdkEventButton *bevent ;
	sub_win_n   = (gint)data;        
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 )
		return FALSE; 
	printf("in subwin_button_press\n");
	bevent = (GdkEventButton *) event; 
	if ((bevent->button == 1))   /* button 1 pressed   */
	{
		
		//x = (int)(bevent->x) ; y = (int)(bevent->y) ;        	
		x = (int)(bevent->x/xscale[crt_n]); y = (int)(bevent->y/yscale[crt_n]);	  //CHI.HAILONG 20110922 修改子菜单分辨率错位问题
		tmp_rect.x1=tmp_rect.x2	= x;
		tmp_rect.y1=tmp_rect.y2	= y;
		printf("here is subwin_button_press::x=%d,y=%d\n",x,y); 
		printf("gdb[%d][%d]->nextpos=%d\n",sub_win_n,crt_n,gcondb[sub_win_n][crt_n].nextpos);
		ctrl_rcd[crt_n]	= GetSubActCtrlItemRcd (crt_n,   
			tmp_rect.x1, tmp_rect.y1, tmp_rect.x2, tmp_rect.y2, 
			sub_win_n);			
		ptr_tab_rcd[crt_n]	= ctrl_rcd[crt_n];
		
		if( ctrl_rcd[crt_n] != (GBRCD *) NULL)
		{	
			printf("ctrl_rcd[crt_n] != (GBRCD *) NULL\n");
			if (ctrl_rcd[crt_n]->lnk.act.ctrl_type != CTRL_REQ_NULL || 
				ctrl_rcd[crt_n]->lnk.act.ctrl_tag == CTRL_TAG_ON)
			{
				printf("ctrl_type=%d,ctrl_tag=%d\n",ctrl_rcd[crt_n]->lnk.act.ctrl_type,ctrl_rcd[crt_n]->lnk.act.ctrl_tag);
				CtrlActHandler ( crt_n, ctrl_rcd[crt_n] );
			}
		}
		else		
			return FALSE;		
		
		return TRUE;
	}  	
	return TRUE;
}



gboolean
sub_win_quit(GtkWidget       *widget,    gpointer         user_data)
{
    int sub_win_n =(int) user_data;
    
    /*printf("sub_win_quit::user_data=%d\n",user_data);*/
	DestroySubWin (widget,GINT_TO_POINTER(sub_win_n));
	
	return FALSE;
}


void PopupDynSubWin (int crt_n,int sub_win_n,GtkWidget	*parent,SHM_OTS *shm_ots)
{
    if (sub_win_shell[sub_win_n][crt_n] == (GtkWidget *)NULL)
    {
        sub_win_shell[sub_win_n][crt_n] = CreateDynSubWin(crt_n, sub_win_n,parent,shm_ots);
        gtk_widget_show(sub_win_canvas[sub_win_n][crt_n]);   
    }
    else
    {
		PopupMessageBox(main_w[crt_n],MSG_CLOSE_SUB_WIN);        
		return;
    }
	/* printf("sub_win_shell[%d][%d]=%d\n",sub_win_n,crt_n,sub_win_shell[sub_win_n][crt_n]);*/
}

static GtkWidget* CreateDynSubWin ( int crt_n,int sub_win_n,GtkWidget	*parent,SHM_OTS *shm_ots)
{	
	int	delay_time	=1000;		/* 10000 milliseconds	= 10 seconds */	
	int data ; 
	GtkWidget  *subwin[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];	
	GtkWidget *vbox, *draw_area[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
	gchar *pConvert=0;
	
#ifdef	DEBUG
	printf ("&&&&in PopupDynSubWin\n");
#endif       
	/*gtk_set_locale();
	gtk_rc_add_default_file("/OIXSRC/gtkrc.zh");  */
	
	subwin[sub_win_n][crt_n]	=gtk_window_new(GTK_WINDOW_TOPLEVEL);        
	
	gtk_window_set_transient_for(GTK_WINDOW(subwin[sub_win_n][crt_n]),GTK_WINDOW(parent));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(subwin[sub_win_n][crt_n]),TRUE);      
	
	gtk_widget_realize(GTK_WIDGET(subwin[sub_win_n][crt_n])) ;  
	gtk_widget_set_usize(GTK_WIDGET(subwin[sub_win_n][crt_n]),(int)(sub_win_width[sub_win_n][crt_n]*xscale[crt_n]), (int)(sub_win_height[sub_win_n][crt_n]*yscale[crt_n]));
	gtk_window_set_policy(GTK_WINDOW(subwin[sub_win_n][crt_n]),FALSE,FALSE,FALSE);  	
	gtk_window_set_keep_above(GTK_WINDOW(subwin[sub_win_n][crt_n]),TRUE);	 
	gtk_window_set_title(GTK_WINDOW(subwin[sub_win_n][crt_n]), pConvert=_toUtf8(sub_dsp_name)  );
	G_SAFE_FREE(pConvert);
	strcpy(sub_win_title[sub_win_n][crt_n],gtk_window_get_title(GTK_WINDOW(subwin[sub_win_n][crt_n])));        
	
	gtk_window_set_modal(GTK_WINDOW(subwin[sub_win_n][crt_n]),FALSE);
	gtk_signal_connect(GTK_OBJECT(subwin[sub_win_n][crt_n]), "destroy",GTK_SIGNAL_FUNC(sub_win_quit),(gpointer)sub_win_n);   
	
// 	if (shm_ots)
// 		gtk_window_move(GTK_WINDOW(subwin[sub_win_n][crt_n]) , (shm_ots->child.pos.x)*xscale[crt_n]   , (shm_ots->child.pos.y)*yscale[crt_n] );
// 	else
		gtk_window_set_position(GTK_WINDOW(subwin[sub_win_n][crt_n]),GTK_WIN_POS_MOUSE);
	
	vbox	=	gtk_vbox_new(FALSE,0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER( subwin[sub_win_n][crt_n]), vbox);
	
	gtk_widget_show(vbox);
	
	/************************  canvas generate code start here!!!!*********************/
	
	draw_area[sub_win_n][crt_n] = gtk_drawing_area_new();
	
	gtk_widget_set_events(draw_area[sub_win_n][crt_n], GDK_EXPOSURE_MASK  
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_BUTTON_RELEASE_MASK
		| GDK_POINTER_MOTION_MASK );        
	
  	 gtk_signal_connect (GTK_OBJECT (draw_area[sub_win_n][crt_n]),"expose_event",GTK_SIGNAL_FUNC(SubWinExposeCB),(gpointer)sub_win_n) ; /*  0 --crt_n */ 
	 gtk_signal_connect (GTK_OBJECT (draw_area[sub_win_n][crt_n]), "button_press_event",
		 GTK_SIGNAL_FUNC (subwin_button_press),GINT_TO_POINTER(sub_win_n) ); 
	 gtk_signal_connect (GTK_OBJECT (draw_area[sub_win_n][crt_n]), "realize",    GTK_SIGNAL_FUNC (sub_win_realize),(gpointer)sub_win_n);
	 
   	 gtk_box_pack_start (GTK_BOX (vbox),draw_area[sub_win_n][crt_n], TRUE, TRUE,0);   	
	 
   	 gtk_widget_realize(GTK_WIDGET(draw_area[sub_win_n][crt_n])) ;  
	 gtk_widget_show (GTK_WIDGET(draw_area[sub_win_n][crt_n]));
	 
	 if(sub_win_timeout_id[sub_win_n][crt_n])
	 {
		 gtk_timeout_remove(sub_win_timeout_id[sub_win_n][crt_n]);
		 sub_win_timeout_id[sub_win_n][crt_n]= 0;
	 }      
	 sub_win_timeout_id[sub_win_n][crt_n]	=gtk_timeout_add (  delay_time, (GtkFunction)DrawSubWinCanvas, sub_win_canvas[sub_win_n][crt_n]);
// 	 if (shm_ots)
// 	 {
// 		 data=getMenuDataPointer(crt_n,sub_win_n);
// 		 delay_time   =  shm_ots->child.time_len *1000  * 8/(  shm_ots_addr->speed); 
// 		 gtk_timeout_add (  delay_time, (GtkFunction) SubWinQuitByTimeCall, data);
// 	 }
	 
	 gtk_widget_show (GTK_WIDGET(subwin[sub_win_n][crt_n]));
	 return (subwin[sub_win_n][crt_n]);       
}


void DrawSubWinCanvas (GtkWidget	*w)
{	
	int	i,crt_n;
	
	GetCrtNumberOfWgt ( w, &crt_n );	
	if( crt_n < 0 )
		return; 
	printf("in DrawSubWinCanvas\n");
	
	
	/* printf("DrawSubWinCanvas:: w=%d\n",w);*/
	if(w !=NULL)
	{                  
		for(i=0;i<MAX_SUB_WIN_NUM;i++)  
		{
			if(w==sub_win_canvas[i][crt_n])
			{
                gdk_window_copy_area(sub_pixmap[i][crt_n], info[crt_n].sub_gc[i] ,0 , 0 , bkgrd_sub_pixmap[i][crt_n] ,0 , 0 ,sub_win_width[i][crt_n] , sub_win_height[i][crt_n] );
				
                gdk_draw_pixmap(sub_pixmap[i][crt_n], info[crt_n].sub_gc[i], bkgrd_sub_pixmap[i][crt_n] ,0 , 0 ,0 , 0 ,sub_win_width[i][crt_n] , sub_win_height[i][crt_n] );
				
                DrawSubDspBackground (crt_n, sub_pixmap[i][crt_n],i);
                
                DrawSubDynamicDsp (crt_n, /*sub_win_canvas[i][crt_n]->window*/sub_pixmap[i][crt_n],i);         
                /*printf("in DrawSubWinCanvas::info[%d].sub_gc[%d]=%d\n",crt_n,i,info[crt_n].sub_gc[i]);*/
                gdk_draw_drawable(sub_win_canvas[i][crt_n]->window, info[crt_n].sub_gc[i] ,sub_pixmap[i][crt_n] ,0 , 0 ,  0 , 0 ,(gint)(sub_win_width[i][crt_n]*xscale[crt_n]) , (gint)(sub_win_height[i][crt_n]*yscale[crt_n])    );
                /*gtk_window_set_keep_above(GTK_WIDGET(sub_win_shell[i][crt_n]),TRUE);	 */
                break;
			}                
		}  
	}
}


void sub_win_realize(GtkWidget *widget,gpointer data)
{	
	int crt_n ;
	int sub_win_n  = (gint) data ; 
	GetCrtNumberOfWgt ( widget, &crt_n );	
	if( crt_n < 0 )
		return; 
	sub_win_canvas[sub_win_n][crt_n] = widget;
	/*printf("realize::sub_win_canvas[%d][%d]=%d\n",sub_win_n,crt_n,sub_win_canvas[sub_win_n][crt_n]);*/
	info[crt_n].sub_gc[sub_win_n] = gdk_gc_new(widget->window);
	sub_pixmap[sub_win_n][crt_n] = gdk_pixmap_new (widget->window  ,
		(gint)(sub_win_width[sub_win_n][crt_n]*xscale[crt_n]), (gint)(sub_win_height[sub_win_n][crt_n]*yscale[crt_n]), -1);
	bkgrd_sub_pixmap[sub_win_n][crt_n] = gdk_pixmap_new (widget->window  ,
		(gint)(sub_win_width[sub_win_n][crt_n]*xscale[crt_n]), (gint)(sub_win_height[sub_win_n][crt_n]*yscale[crt_n]), -1);
	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]); 
	if(sub_win_canvas[sub_win_n][crt_n] !=NULL)
	{
		DrawSubDspBackground (crt_n, sub_win_canvas[sub_win_n][crt_n]->window,sub_win_n); 
		/* DrawSubDspBackground (crt_n, bkgrd_sub_pixmap[sub_win_n][crt_n],sub_win_n);*/
		DrawSubWinCanvas (sub_win_canvas[sub_win_n][crt_n]); 
	}    
}
/****************************  expose       *******************************/

void  SubWinExposeCB(	GtkWidget *widget, 
					 GdkEventExpose *event,    
					 gpointer data)
{    
	int crt_n ;
	int sub_win_n=(int) data;
	
	GetCrtNumberOfWgt ( widget, &crt_n );	
	if( crt_n < 0 )
		return; 
#ifdef DEBUG	  
	g_message(":void  SubWinExposeCB(@@@@@@@:");
#endif           
	if(sub_win_canvas[sub_win_n][crt_n] !=NULL)
	{                   
		/*printf("Expose::sub_win_canvas[%d][%d]->window=%d\n",sub_win_n,crt_n,sub_win_canvas[sub_win_n][crt_n]->window);*/
		DrawSubDspBackground (crt_n, sub_win_canvas[sub_win_n][crt_n]->window,sub_win_n); 
        
		DrawSubWinCanvas (sub_win_canvas[sub_win_n][crt_n]);  
	}
	else
	{
		/*printf("expose::sub_win_timeout_id[%d][%d]=%d\n",sub_win_n,crt_n,sub_win_timeout_id[sub_win_n][crt_n]);*/
		return;
	}
	
}


/*************sub win of ana curve **********************/

void DestroyAnaCurveDlg(GtkWidget *w,EntryInfo	 *entryInfo) 
{   
    int crt_n,dlg_no;
    crt_n = entryInfo->crt_n;
    dlg_no = entryInfo->sub_dlg_no;   
    
    if(ana_curve_timeout_id[dlg_no][crt_n]!=0) 
    {
        gtk_timeout_remove(ana_curve_timeout_id[dlg_no][crt_n]);
        ana_curve_timeout_id[dlg_no][crt_n]= 0;
    }
    gtk_widget_destroy(ana_curve_dlg[dlg_no][crt_n]);
    ana_curve_dlg[dlg_no][crt_n]=NULL;  
}









void EntryCurveRedraw ( EntryInfo	 *entryInfo)
{
    GdkPoint	p[ANA_CURVE_POINT_NUM];
    int i,within_2min;        
	int cx,cy,y_top,width=CURVE_WIDTH,height=CURVE_HEIGHT,x_left=59;
	int crt_n,dlg_no,y_btm,y_lmt,y_warn_top,y_warn_btm,y_alm_top,y_alm_btm ;
	
	char up_lmt[10],dn_lmt[10];
	char  val[50],d_quality[3];
	float tmp_val[ANA_CURVE_POINT_NUM];
	
	float	fx, fy, VT, VB, ys, ye;
	DMS_COMMON dms_cmm;
	
	crt_n = entryInfo->crt_n;
	dlg_no = entryInfo->sub_dlg_no;
	
	y_top = 1;
	printf("### this entry:db_name=%s\n",entryInfo->db_name);	
    if(!ana_curve_dlg[dlg_no][crt_n])
        return;
    if ( ReadEntryByNameStr (  entryInfo->db_name, &dms_cmm ) == -1 )
    {
        /*PopupMessageBox(crt_n,OIX_STR_DB_NO_POINT); */
        printf("no this entry:db_name=%s\n",entryInfo->db_name);
        return;
    }
	gdk_window_clear(Curve_drawingarea[dlg_no][crt_n]->window);
	gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][GREEN4]);   
	strcpy(d_quality,"");
	if(dms_cmm.point.data_type==ANA_TYPE||dms_cmm.point.data_type == PARAM_TYPE  
		||dms_cmm.point.data_type == CHARA_TYPE ||dms_cmm.point.data_type ==OUTA_TYPE)
	{       
        
        ye= dms_cmm.var.anlg.eng_para.HI_RANGE;
        ys= dms_cmm.var.anlg.eng_para.LO_RANGE;    
		if(ye==ys)
		{
			PopupMessageBox( main_w[crt_n],"Please Set Hi_Range and Low_Range!");
			return;
		} 
        sprintf(up_lmt,"%.1f",ye);
        sprintf(dn_lmt,"%.1f",ys);
        
        sprintf(val,"%.2f",dms_cmm.var.anlg.fValue);
        if(dms_cmm.var.anlg.value.status.lmt_state==ANA_ALM_HI_ALARM||dms_cmm.var.anlg.value.status.lmt_state==ANA_ALM_LO_ALARM
			||dms_cmm.var.anlg.value.status.lmt_state==ANA_ALM_HI_WARN||dms_cmm.var.anlg.value.status.lmt_state==ANA_ALM_LO_WARN)                 
			gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][RED1]);        
        else
            gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][GREEN4]);  
		windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 1, y_top+10, val,strlen(val),crt_n);
		
		gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][BLACK]); 
		windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 50, y_top+10, 
			dms_cmm.var.anlg.unitName,strlen(dms_cmm.var.anlg.unitName),crt_n);       
		
		if(!dms_cmm.fixed.iost.scanEnable)
		{
            strcpy(d_quality,"M");
            gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][YLW1]);  
        }
        if(dms_cmm.var.anlg.value.status.chan_fail)
		{
            strcpy(d_quality,"F");
            gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][RED1]);  
        }
        if(dms_cmm.var.anlg.value.status.ctrl_block)
            strcpy(d_quality,"B"); 
		
		windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 100, y_top+10, d_quality,strlen(d_quality),crt_n);
		
        
	}
	else if(dms_cmm.point.data_type==SOE_TYPE||dms_cmm.point.data_type==POL_TYPE
		||dms_cmm.point.data_type == SYSMSG_TYPE||dms_cmm.point.data_type ==OUTS_TYPE)
	{
		GetIndPtRtStateStrById(&dms_cmm.point, val );	
		sprintf(val,"%s(%d)",val,dms_cmm.var.ind.status.rt_state);
		printf("val=%s\n",val);
		if(dms_cmm.var.ind.status.rt_state==1)
			gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][RED]); 
		windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 1, y_top+10, val,strlen(val),crt_n);
		
		if(dms_cmm.var.ind.status.man_set)
		{
			strcpy(d_quality,"M");
			gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][YLW1]); 
		}
		if(dms_cmm.var.ind.status.chan_fail)
		{
            strcpy(d_quality,"F"); 
            gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][RED1]);
		}
		windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 100, y_top+10, d_quality,strlen(d_quality),crt_n);
		
		ye= 1.0;
		ys= 0.0;    
		sprintf(up_lmt,"%.0f",ye);
		sprintf(dn_lmt,"%.0f",ys);         
    }  
    
	gdk_gc_set_foreground(curve_gc[dlg_no][crt_n], &pixels[crt_n][BLACK]);    
    
    if(entryInfo->cur_no>ANA_CURVE_POINT_NUM-1)    
    {
        entryInfo->cur_no = ANA_CURVE_POINT_NUM-1;         
        for(i=0;i<entryInfo->cur_no;i++)        
			entryInfo->value[i]=entryInfo->value[i+1]; 
        within_2min=0;
    }
    else
        within_2min =1;
    
	if(dms_cmm.point.data_type==ANA_TYPE||dms_cmm.point.data_type == PARAM_TYPE  
		||dms_cmm.point.data_type == CHARA_TYPE ||dms_cmm.point.data_type ==OUTA_TYPE)  
        entryInfo->value[entryInfo->cur_no]	= dms_cmm.var.anlg.fValue;   
    else if(dms_cmm.point.data_type==SOE_TYPE||dms_cmm.point.data_type==POL_TYPE
		||dms_cmm.point.data_type == SYSMSG_TYPE||dms_cmm.point.data_type ==OUTS_TYPE)
        entryInfo->value[entryInfo->cur_no]	=dms_cmm.var.ind.status.rt_state;
    
	printf("###crt_n=%d,db_name=%s,value[%d]=%.1f\n",crt_n,entryInfo->db_name,entryInfo->cur_no,entryInfo->value[entryInfo->cur_no]);
	y_top = y_top+20;
    
    cx	 = x_left ;			
    gdk_draw_line (  Curve_drawingarea[dlg_no][crt_n]->window,curve_gc[dlg_no][crt_n], cx, y_top+1, cx, y_top+height-1);
	
    cx	 = width+x_left ;			
    gdk_draw_line (  Curve_drawingarea[dlg_no][crt_n]->window,curve_gc[dlg_no][crt_n], cx, y_top+1, cx, y_top+height-1);
    
    cy	 = y_top+height-1 ;
    gdk_draw_line (  Curve_drawingarea[dlg_no][crt_n]->window,curve_gc[dlg_no][crt_n], x_left+1, cy, x_left+width-1, cy);
    windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], x_left, y_top+height+10, "t-3",strlen("t-3"),crt_n);    
    windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], x_left+width, y_top+height+10, "t",strlen("t"),crt_n);    
    y_alm_btm = cy;
    
    cy	 = y_top+1 ;
    gdk_draw_line (  Curve_drawingarea[dlg_no][crt_n]->window,curve_gc[dlg_no][crt_n], x_left+1, cy, x_left+width-1, cy);    
    windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 1, y_top+10, up_lmt,strlen(up_lmt),crt_n);      
    windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 1, y_top+height-5, dn_lmt,strlen(dn_lmt),crt_n);    
    y_alm_top = cy;
    
    
    
    y_btm	= y_top + height;
    fx 	= (float)width/ANA_CURVE_POINT_NUM; 		/*Modified by zyp   add (float) */
    fy 	= (float)height/(ye-ys);
    VT	 = (ys>ye) ? ys:ye;
    VB	 = (ys<ye) ? ys:ye;  
    
	printf("up_lmt=%s,dn_lmt=%s\n",up_lmt,dn_lmt);
	if(dms_cmm.point.data_type==ANA_TYPE||dms_cmm.point.data_type == PARAM_TYPE  
		||dms_cmm.point.data_type == CHARA_TYPE ||dms_cmm.point.data_type ==OUTA_TYPE)
	{
		y_warn_top =(int) (y_alm_btm -fy*(dms_cmm.var.anlg.lmt_para.HI_WARN-ys));   
		sprintf(up_lmt,"%.1f",dms_cmm.var.anlg.lmt_para.HI_ALARM);
		sprintf(dn_lmt,"%.1f",dms_cmm.var.anlg.lmt_para.LO_ALARM);
		y_lmt  = (int)(y_btm -fy*(dms_cmm.var.anlg.lmt_para.HI_ALARM-ys));
		printf("y_lmt=%d\n",y_lmt);
		gdk_draw_line (  Curve_drawingarea[dlg_no][crt_n]->window,curve_gc[dlg_no][crt_n], x_left-3, y_lmt, x_left, y_lmt);  
		windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 1, y_lmt-5, up_lmt,strlen(up_lmt),crt_n);      
		
		y_warn_btm = (int)(y_alm_btm -fy*(dms_cmm.var.anlg.lmt_para.LO_WARN-ys));
		
		y_lmt  = (int)(y_btm -fy*(dms_cmm.var.anlg.lmt_para.LO_ALARM-ys));
		gdk_draw_line (  Curve_drawingarea[dlg_no][crt_n]->window,curve_gc[dlg_no][crt_n], x_left-3, y_lmt, x_left, y_lmt); 
		windk_draw_text( Curve_drawingarea[dlg_no][crt_n]->window, font_set[SGZY_FONT], curve_gc[dlg_no][crt_n], 1, y_lmt+5, dn_lmt,strlen(dn_lmt),crt_n);    
		printf("###  y_top_alm=%d,y_top_warn=%d,y_btm_warn=%d,y_btm_alm=%d\n",y_alm_top,y_warn_top,y_warn_btm,y_alm_btm);  
	}
	else
	{
		y_warn_top = 1;
		y_warn_btm = 0;
	}
    if(entryInfo->cur_no<=ANA_CURVE_POINT_NUM)
	{
		for(i=0;i<=entryInfo->cur_no;i++)
			tmp_val[i]=entryInfo->value[entryInfo->cur_no-i];
    }
    
    for(i=0;i<=entryInfo->cur_no;i++)
    {    
        if(within_2min==0)
            p[i].x	= (int)(x_left+width-fx*i);
        else
            p[i].x	= (int)(x_left+fx*i);
		
		if ( tmp_val[i] < VB )
		{
            p[i].y = y_btm;
            printf("1:::tmp_val[%d]=%.1f,VB=5d\n",i,tmp_val[i],VB);
		}
		else
		{
			if ( entryInfo->value[i] > VT )
				p[i].y  = y_top;
			else
			{
				if(within_2min==0)
					p[i].y  = (int)(y_btm -fy*(tmp_val[i]-ys));
				else
					p[i].y  = (int)(y_btm -fy*(entryInfo->value[i]-ys));
			}
			printf("2:::tmp_val[%d]=%.1f,ys=%.1f,y_top=%d\n",i,tmp_val[i],ys,y_top);
		}
        printf("crt_n=%d,p[%d].x=%d,p[%d].y=%d,VB=%.0f,VT=%.0f,value[%d]=%.1f\n",crt_n,i,p[i].x,i,p[i].y,VB,VT,i,entryInfo->value[i]);
        
    }
	
    my_draw_lines ( Curve_drawingarea[dlg_no][crt_n]->window, curve_gc[dlg_no][crt_n],  p, entryInfo->cur_no ,y_warn_top,y_warn_btm ); 
    entryInfo->cur_no++;    
	
	/* printf("end::in EntryCurveRedraw\n");*/
}			

void 
Curve_draw_area_realize(GtkWidget *widget,EntryInfo	 *entryInfo)
{
	int crt_n,dlg_no;		
	
	crt_n = entryInfo->crt_n;
	dlg_no = entryInfo->sub_dlg_no;
	
	Curve_drawingarea[dlg_no][crt_n]= widget ;
    
	curve_gc[dlg_no][crt_n] =  gdk_gc_new(widget->window);         
}

void  Curve_expose_events(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
{   
    int crt_n,dlg_no;
    EntryInfo	 *tmp,*entryInfo[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
    
    GetCrtNumberOfWgt( widget, &crt_n );
    if( crt_n < 0 )
        return;
    tmp = (EntryInfo	 *)data;
    dlg_no = tmp->sub_dlg_no;
    
	entryInfo[dlg_no][crt_n]=(EntryInfo	 *)data;    
    printf("in      curve_expose_events:::name=%s\n",entryInfo[dlg_no][crt_n]->db_name);    
    EntryCurveRedraw (entryInfo[dlg_no][crt_n]);
}

void
GetObjDlg (int crt_n,int dlg_no,char *dbname)
{
    
	GtkWidget *longname_lab;	
	GtkWidget *vbox1;
	GtkWidget *hbox1;
	GtkWidget *exit_btn;
	static EntryInfo	 entryInfo[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
	DMS_COMMON    dms_cmm;
	char dlg_title[15];
	char  LongNamerStr1[128];
	gchar *pConvert=0;
	
    if ( ReadEntryByNameStr (  dbname, &dms_cmm ) == -1 )
    {
        PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT); 
        return;
    }
    if(dms_cmm.point.data_type!=ANA_TYPE &&dms_cmm.point.data_type!=PARAM_TYPE&&dms_cmm.point.data_type!=OUTA_TYPE
        &&dms_cmm.point.data_type!=POL_TYPE&&dms_cmm.point.data_type!=SOE_TYPE&&dms_cmm.point.data_type!=SYSMSG_TYPE)
    {
        PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
    }
	if(strstr(dbname,".")!=NULL)
		strcpy(dlg_title,strstr(dbname,".")+1);
	
	entryInfo[dlg_no][crt_n].cur_no=0;
	entryInfo[dlg_no][crt_n].crt_n = crt_n;
	entryInfo[dlg_no][crt_n].sub_dlg_no=dlg_no;
	strcpy(entryInfo[dlg_no][crt_n].db_name , dbname);
	
	ana_curve_dlg[dlg_no][crt_n] = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (ana_curve_dlg[dlg_no][crt_n]), pConvert=_toUtf8(dbname));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(ana_curve_dlg[dlg_no][crt_n]),GTK_WINDOW(main_w[crt_n]) );
	gtk_window_set_position(GTK_WINDOW(ana_curve_dlg[dlg_no][crt_n]),GTK_WIN_POS_MOUSE);
	gtk_widget_set_usize (ana_curve_dlg[dlg_no][crt_n], 250, 240); 
	gtk_window_set_policy(GTK_WINDOW(ana_curve_dlg[dlg_no][crt_n]),FALSE,FALSE,FALSE);
	
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (ana_curve_dlg[dlg_no][crt_n]), vbox1);
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 5);
	
//	GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
	strcpy(LongNamerStr1,dms_cmm.fixed.entryLongname);
	longname_lab = gtk_label_new (pConvert=_toUtf8(LongNamerStr1));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (longname_lab);
	gtk_box_pack_start (GTK_BOX (hbox1), longname_lab, FALSE, FALSE, 0); 
	
	Curve_drawingarea[dlg_no][crt_n] = gtk_drawing_area_new ();
	gtk_widget_set_usize (Curve_drawingarea[dlg_no][crt_n], 250, 160); 
	gtk_widget_show (Curve_drawingarea[dlg_no][crt_n]);
	gtk_box_pack_start (GTK_BOX (vbox1), Curve_drawingarea[dlg_no][crt_n], FALSE, FALSE, 5);   
	
	gtk_widget_set_events(Curve_drawingarea[dlg_no][crt_n], GDK_EXPOSURE_MASK
        | GDK_LEAVE_NOTIFY_MASK|GDK_POINTER_MOTION_HINT_MASK 
        | GDK_POINTER_MOTION_MASK );
    
    gtk_signal_connect (GTK_OBJECT (Curve_drawingarea[dlg_no][crt_n]), "realize", 
		      GTK_SIGNAL_FUNC (Curve_draw_area_realize), &entryInfo[dlg_no][crt_n]);
    
    gtk_widget_realize(GTK_WIDGET(Curve_drawingarea[dlg_no][crt_n])) ;      
    
    gtk_signal_connect (GTK_OBJECT (Curve_drawingarea[dlg_no][crt_n]),"expose_event",GTK_SIGNAL_FUNC(Curve_expose_events),&entryInfo[dlg_no][crt_n]) ; 
	
    
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 5);
	
	exit_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_EXIT));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (exit_btn);
	
	gtk_widget_set_usize (exit_btn, 50, 30); 
	gtk_box_pack_start (GTK_BOX (hbox1), exit_btn, FALSE, FALSE, 100);  
	
	gtk_signal_connect(GTK_OBJECT(ana_curve_dlg[dlg_no][crt_n]),
        "destroy",
        GTK_SIGNAL_FUNC(DestroyAnaCurveDlg),
        &entryInfo[dlg_no][crt_n]);
	gtk_signal_connect(GTK_OBJECT(exit_btn),
        "pressed",
        GTK_SIGNAL_FUNC(DestroyAnaCurveDlg),
        &entryInfo[dlg_no][crt_n]);
    if (ana_curve_timeout_id[dlg_no][crt_n] != 0 )
	{
		gtk_timeout_remove(ana_curve_timeout_id[dlg_no][crt_n]);	
		ana_curve_timeout_id[dlg_no][crt_n]= 0;
	}
	ana_curve_timeout_id[dlg_no][crt_n]= gtk_timeout_add (1000, (GtkFunction)EntryCurveRedraw, &entryInfo[dlg_no][crt_n]); 
	
    gtk_widget_show (ana_curve_dlg[dlg_no][crt_n]);
	
}




