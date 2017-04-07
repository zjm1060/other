#include <gtk/gtk.h>

#include <gdk/gdk.h>
#include	"../inc/oix.h"
#include "../inc/gtk_view_def.h"
int view_button_press( GtkWidget *widget,GdkEvent *event , gpointer data) ; 
void DrawViewRect (int crt_n,GdkDrawable *drawable,GdkGC  *gc )  ; 
char*	_toUtf8EX(char *c);


void  view_canvas_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data) 
{

	int crt_n = (gint) data ; 
	viewdb[crt_n].width		= a->width ;
	viewdb[crt_n].height	= a->height ;
	viewdb[crt_n].xfactor	=(float)viewdb[crt_n].width/zoomdb[crt_n].canvas_w;
	viewdb[crt_n].yfactor	=(float)viewdb[crt_n].height  /zoomdb[crt_n].canvas_h;
	g_message("\n OIX:view_canvas_w[%d] %d , view_canvas_h %d \n",crt_n , a->width ,a->height);
}

gint view_win_quit(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE; 	
	g_object_unref (view_win[crt_n]); 	
	
	gtk_widget_destroy(GTK_WIDGET(view_win[crt_n])); 
	view_win[crt_n] = (GtkWidget *)NULL ;
    viewdb[crt_n].canvas= (void *)NULL ; 
	return TRUE;
}

void    RedrawView (GtkWidget *w)
{
	int	crt_n;
	/*GdkGC *gc ;*/
	float xscale1,yscale1,factor ,sx_orig,sy_orig ; 	
	GetCrtNumberOfWgt ( w, &crt_n);  
	if( crt_n < 0 || crt_n >= MAX_CRT_NUM)
		return;	

	ClearArea (crt_n,  view_pixmap[crt_n], 0, 0, viewdb[crt_n].width, viewdb[crt_n].height, k_no_expose);  
	xscale1  =xscale[crt_n] ; 
	yscale1  =yscale[crt_n] ;   
	xscale[crt_n] =  (float) w->allocation.width            / (float)STND_WIDTH  ; 
	yscale[crt_n] =	 (float) (w->allocation.height * 1.1)   / (float)STND_HEIGHT  ; 
	factor  = zoomdb[crt_n].factor ;
	sx_orig  = (float)zoomdb[crt_n].sx_orig  ;
	sy_orig  = (float)zoomdb[crt_n].sy_orig  ; 
	zoomdb[crt_n].sx_orig =0  ;
	zoomdb[crt_n].sy_orig =0  ;
	zoomdb[crt_n].factor =1  ; 
	DrawDspBackground (crt_n, view_pixmap[crt_n]); 
	xscale[crt_n]  =xscale1 ; 
	yscale[crt_n]  =yscale1 ;
	zoomdb[crt_n].factor  = factor ;
	zoomdb[crt_n].sx_orig =(int)sx_orig  ;
	zoomdb[crt_n].sy_orig =(int)sy_orig  ;
//	gdk_window_invalidate_rect(drawable,NULL,0) ; 
	DrawViewRect (  crt_n,  view_pixmap[crt_n],   info[crt_n].view_gc )  ; 
	gdk_draw_pixmap(w->window, info[crt_n].view_gc, view_pixmap[crt_n], 0,0, 0, 0, viewdb[crt_n].width, viewdb[crt_n].height  );

printf  ("\n^_^ :::RedrawView (GtkWidget *w)   ^^^^&&******\n")  ; 	
}

int view_button_press( GtkWidget *widget,GdkEvent *event , gpointer data)
{
    int x ,y ; 
    int crt_n ;
//    int	width, height;
//    int	xtmp, ytmp;
    int	bell_volume	= 30;
    GdkRectangle rc ; 
//    GdkPoint  pt ;
//    DMS_COMMON dms_cmm;
//    char cur_file_mode[50];
	crt_n = (gint)  data ;   
	GetCrtNumberOfWgt ( widget, &crt_n);  
	
    if (crt_n <0  || crt_n >= MAX_CRT_NUM)
		return FALSE;
	
    if (event->type == GDK_BUTTON_PRESS) { 	/*  GDK_2BUTTON_PRESS ---double clicked !!!! */
	 GdkEventButton *bevent = (GdkEventButton *) event; 
     x =(int) (bevent->x) ;
	 y =(int) (bevent->y) ;
     if (bevent->button == 1)  
     {	
		zoomdb[crt_n].sx_orig =(int)( ( -x/viewdb[crt_n].xfactor+Canvas_RECT[crt_n].width /2 ) );
		zoomdb[crt_n].sy_orig =(int)( ( -y/viewdb[crt_n].yfactor+Canvas_RECT[crt_n].height /2)  );
		CheckZoomOrigin (crt_n);
		Redraw (canvas[crt_n] );
		rc.x=rc.y =  0  ; 
		rc.width =  viewdb[crt_n].width  ;  
		rc.height=  viewdb[crt_n].height ;
 		gdk_window_invalidate_rect(viewdb[crt_n].canvas->window ,&rc,0);
     } 
	 
    }   
    return FALSE;
}

void  view_expose_events(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
{
	
	int crt_n = (gint)data ;      
	if( crt_n < 0  || crt_n > MAX_CRT_NUMBER )	return;   
	RedrawView(widget); 
//  	gdk_gc_set_clip_rectangle(widget->style->fg_gc[widget->state], &event->area );  		
//   	gdk_draw_pixmap(widget->window, info[crt_n].view_gc  ,view_pixmap[crt_n]  ,event->area.x ,event->area.y , 
//  		event->area.x ,event->area.y ,event->area.width, event->area.height );	
//   	gdk_gc_set_clip_rectangle(widget->style->fg_gc[widget->state],NULL );	
	printf ("view_expose_events event %d is occur!!\n", crt_n);
#ifdef DEBUG
	printf ("expose event %d is occur!!\n", crt_n);

#endif  
}


void DrawViewRect (int crt_n,GdkDrawable *drawable,GdkGC  *gc )
{
	BACKRCD	rcd;
 
	memset( &rcd, 0, sizeof(BACKRCD) );
	if (drawable != NULL)
	{   
		rcd.fg	= ColorId3to4(YELLOW);
		rcd.draw_type	=DrwT_RECTANGLE;
		viewdb[crt_n].xfactor	=(float)viewdb[crt_n].width/zoomdb[crt_n].canvas_w;
		viewdb[crt_n].yfactor	=(float)viewdb[crt_n].height/zoomdb[crt_n].canvas_h;
		rcd.rect.x1	= (int)( -zoomdb[crt_n].sx_orig *viewdb[crt_n].xfactor / xscale[crt_n]);
		rcd.rect.y1	= (int)( -zoomdb[crt_n].sy_orig*viewdb[crt_n].yfactor / yscale[crt_n]);
		rcd.rect.x2	= (int)( (Canvas_RECT[crt_n].width   - zoomdb[crt_n].sx_orig)*viewdb[crt_n].xfactor / xscale[crt_n]);
		rcd.rect.y2	= (int)( (Canvas_RECT[crt_n].height  - zoomdb[crt_n].sy_orig)*viewdb[crt_n].yfactor / yscale[crt_n]);
		rcd.fill.fill_type	= UNFILLED;
		rcd.line_width = 3;
		
		DrawRcd ( crt_n, drawable, gc, &rcd,0);
	}
}


void 
view_area_realize(GtkWidget *widget,gpointer data)
{
	
	gint crt_n ;
	crt_n = (gint)data;
    info[crt_n].view_gc  =  gdk_gc_new(widget->window);
	viewdb[crt_n].canvas  = widget ; 
   	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK4]); 
printf  ("\n view_area_realize!!!!! \n") ;
}




GtkWidget* CreateViewWindow(GtkWidget *parent , gint crt_n)
{
    GtkWidget *main_vbox;
    GtkWidget /**menubar,   */*view_area;
    GtkWidget *view_window;  
//   GtkWidget  *vbox1,*hbox1;
//    GtkImage  *image1 ;
//	GtkWidget *vpaned1 ;
    /*
    GtkWidget  *handleBox,*hboxN ,*sep,*cur_usr_label,*cur_filelab,*fault_redisplay,/ **consistent_btn,* / *step_btn,*pause_btn,*lblTimer;
    */
    
//	GtkWidget  *handleBox,*hboxN ,*sep,*cur_usr_label,*cur_filelab,*fault_redisplay,*next_dsp_btn,*pause_demo_btn,*step_btn,*pause_btn,*lblTimer;
	
	gint root_x,root_y;
	gchar *pConvert=0;
	
    view_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);   
	gtk_window_set_transient_for(GTK_WINDOW(view_window),GTK_WINDOW(parent)); 
	
		gtk_signal_connect (GTK_OBJECT (view_window), "destroy", 
 		      GTK_SIGNAL_FUNC (view_win_quit), (gpointer)crt_n);
	gtk_window_set_title (GTK_WINDOW(view_window), pConvert=_toUtf8EX( "OIX_CSTR_VIEW_WINDOW"));
	G_SAFE_FREE(pConvert);
	
	gtk_window_set_default_size(GTK_WINDOW(view_window), main_w[crt_n]->allocation.width/6  ,main_w[crt_n]->allocation.height/6/*VIEW_HEIGHT*/) ;
	
#ifdef WINDOWS_OS
	//hcl
	root_x=LOCAL_WIDTH*crt_n;
    root_y=0;
	gtk_window_move (GTK_WINDOW(view_window), root_x, root_y);
    gtk_window_get_position (GTK_WINDOW(view_window),&root_x,&root_y);
	
#endif
	
	
	
	
	/*   if you want move the windows to a certain place , you must first call funcion realize  */
	
	gtk_container_border_width(GTK_CONTAINER(view_window),0); 
	
	main_vbox = gtk_vbox_new (FALSE, 0);
	
	gtk_container_border_width (GTK_CONTAINER (main_vbox), 0);
	gtk_container_add (GTK_CONTAINER (view_window), main_vbox);	
	gtk_widget_show (main_vbox);
	
	/*************** Draw Area  code here !!! **************************/  
		
	view_area  = gtk_drawing_area_new();
    gtk_box_pack_start (GTK_BOX (main_vbox), view_area , TRUE, TRUE,0);
 
	
	gtk_widget_set_events(view_area , GDK_EXPOSURE_MASK  
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_BUTTON_RELEASE_MASK
		| GDK_POINTER_MOTION_MASK );			 
	
	gtk_signal_connect (GTK_OBJECT (view_area ),"expose_event",GTK_SIGNAL_FUNC(view_expose_events),GINT_TO_POINTER(crt_n)) ;
	
// 	gtk_signal_connect (GTK_OBJECT (view_area ),"motion_notify_event",
// 		GTK_SIGNAL_FUNC(motion_notify_callback),GINT_TO_POINTER(crt_n)) ;   
	gtk_signal_connect (GTK_OBJECT (view_area ), "realize", 
		GTK_SIGNAL_FUNC (view_area_realize),   GINT_TO_POINTER(crt_n));
	gtk_signal_connect (GTK_OBJECT (view_area), "size_allocate", 
			GTK_SIGNAL_FUNC ( view_canvas_allocate), GINT_TO_POINTER(crt_n)); 
	gtk_signal_connect (GTK_OBJECT (view_area ), "button_press_event",
		GTK_SIGNAL_FUNC (view_button_press),GINT_TO_POINTER(crt_n) ); 
 
	
 	gtk_widget_realize(GTK_WIDGET( view_area )) ;
	gtk_widget_show (view_area );
	
	return view_window;
}

void  PopUpViewWindow(GtkWidget*parent ,  int crt_n)
{
	if (view_win[crt_n])
		gtk_window_present((GtkWindow *)view_win[crt_n]);
    else
	{
		view_win[crt_n] =  CreateViewWindow(main_w[crt_n],   crt_n)  ;  
		gtk_widget_show(view_win[crt_n]) ; 
	}

}
