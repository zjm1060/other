
/*
        2006.6.25.      by zyp:
		2007.12.09      SYN by hcl 
*/

#include <gtk/gtk.h>


#define		SUB_DSP_DEF
/*
#define		DEBUG		
*/


#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

 


extern	int	errno;

GdkPixmap *OperCondOffDrawable[MAX_CRT_NUMBER] ;

gint	oper_cond_timeout_id[MAX_CRT_NUMBER]={0,0,0}	 ;

extern	POINTER		oper_cond_ptr[MAX_CRT_NUMBER];

void	UnmanageOperCondBox (GtkWidget* w,int crt_n );
void  OperCondExposeProc(	GtkWidget *widget,GdkEventExpose *event, gpointer data);
extern void	DrawOperCondDsp (int crt_n, GdkWindow *drawable );
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );

void	UpdateOperCondCanvas (int	crt_n )
{
	DMS_COMMON	dms_cmm;	
	
	
	if ( sub_dsp_box[crt_n]==(GtkWidget*) NULL ) 
		return;
			
	if ( ReadEntryById ( &oper_cond_ptr[crt_n], &dms_cmm ) == -1)
	{
		printf ("\n OIX No this point in the database file !!!");
		return;
	}
	/*sbo_result[crt_n] = InterLock_Judge ( sbo_pnt_name[crt_n] );*/	
		
	gdk_gc_set_background ( info[crt_n].interlock_gc, &pixels[crt_n][BLACK]);
	gdk_gc_set_foreground ( info[crt_n].interlock_gc, &pixels[crt_n][BLACK]);
	gdk_draw_rectangle (OperCondOffDrawable[crt_n],info[crt_n].interlock_gc,TRUE, 0,0,sub_dsp_box_width, sub_dsp_box_height); 	
	DrawOperCondDsp ( crt_n,  OperCondOffDrawable[crt_n] );
	
	gdk_window_copy_area(sub_dsp_canvas[crt_n]->window, info[crt_n].interlock_gc ,0 , 0 , OperCondOffDrawable[crt_n] ,
		0 , 0 ,sub_dsp_box_width, sub_dsp_box_height    );	
/*printf("###in UpdateOperCondCanvas,sub_dsp_canvas[%d]=%d,sub_dsp_box=%d\n",crt_n,sub_dsp_canvas[crt_n],sub_dsp_box[crt_n]);*/
	
		
}

void 
OperCond_draw_area_realize(GtkWidget *widget,gpointer data)
{
   gint crt_n ;
   crt_n = (gint)data;
  
    sub_dsp_canvas[crt_n] = widget ;    
    info[crt_n].interlock_gc =  gdk_gc_new(widget->window);
    OperCondOffDrawable[crt_n] = gdk_pixmap_new (widget->window  ,
					sub_dsp_box_width, sub_dsp_box_height, -1);
   gdk_window_set_background(widget->window,&pixels[crt_n][BLACK4]); 	
   UpdateOperCondCanvas(crt_n);
}

static  GtkWidget* CreateOperCondWindow(GtkWidget *parent,	int crt_n)
{
	GtkWidget	*scrolled_window,*window;
	GtkWidget	*vbox;	
        int		next_minute=1000;

    	/*	gtk_set_locale();
    gtk_rc_add_default_file("./gtkrc.zh");  */
					
    	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));
	gtk_window_set_default_size(GTK_WINDOW(window), sub_dsp_box_width, sub_dsp_box_height);
	/*gtk_window_set_title(GTK_WINDOW(window), _toUtf8(""));*/
	printf("in CreateOperCondWindow::crt_n=%d\n",crt_n);
	gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(UnmanageOperCondBox), (gpointer)crt_n);
    

	vbox=gtk_vbox_new(FALSE, 5);
    	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    	gtk_container_add(GTK_CONTAINER(window), vbox);
    
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
   
	sub_dsp_canvas[crt_n]= gtk_drawing_area_new();
	gtk_widget_set_usize(sub_dsp_canvas[crt_n],sub_dsp_box_width, sub_dsp_box_height);
       
	gtk_widget_set_events(sub_dsp_canvas[crt_n], GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK );
	gtk_signal_connect (GTK_OBJECT (sub_dsp_canvas[crt_n]),"expose_event",GTK_SIGNAL_FUNC(OperCondExposeProc),(gpointer)crt_n) ;
	gtk_signal_connect (GTK_OBJECT (sub_dsp_canvas[crt_n]), "realize",GTK_SIGNAL_FUNC (OperCond_draw_area_realize),(gpointer)crt_n);
	
    	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window),sub_dsp_canvas[crt_n] );
	gtk_widget_realize(GTK_WIDGET(sub_dsp_canvas[crt_n])) ;
	if (next_minute > 0)
            {
                /*printf("next_minute=%d\n",next_minute);*/
		oper_cond_timeout_id[crt_n] = gtk_timeout_add(next_minute,(GtkFunction)UpdateOperCondCanvas,(gpointer)crt_n );
        }
	return  window ;  
}



void	PopupOperCondWnd (int crt_n )
{
/*printf("sub_dsp_box=%d\n",sub_dsp_box[crt_n]);*/	
	if ( sub_dsp_box[crt_n] == (GtkWidget*)NULL)
	{		
		sub_dsp_box[crt_n]=CreateOperCondWindow( main_w[crt_n], crt_n ) ;	
		gtk_window_set_position(GTK_WINDOW(sub_dsp_box[crt_n]),GTK_WIN_POS_CENTER);
		gtk_widget_show_all(sub_dsp_box[crt_n]);	
	}
	else
	{		
		gtk_window_present(GTK_WINDOW(sub_dsp_box[crt_n]));
		return;	
	}	
}


void  OperCondExposeProc(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
{
	int	crt_n;

    	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return ;
	gdk_window_copy_area (widget->window,info[crt_n].interlock_gc,0,0,
		OperCondOffDrawable[crt_n],0,0,sub_dsp_box_width, sub_dsp_box_height);	
	UpdateOperCondCanvas(crt_n);
}

void	UnmanageOperCondBox (GtkWidget* w,int crt_n )
{
        if(sub_dsp_box[crt_n]!=(GtkWidget*) NULL)
        {
			g_object_unref (OperCondOffDrawable[crt_n]); //added by zyp on 2008.12.25
            gtk_widget_destroy ( sub_dsp_box[crt_n]);
            sub_dsp_box[crt_n]= NULL;
            gtk_widget_destroy ( sub_dsp_canvas[crt_n]);
            sub_dsp_canvas[crt_n]= NULL;    
            printf("*******in UnmanageOperCondBox::w=%d,sub_dsp_box[crt_n]=%d\n\n\n\n",w,sub_dsp_box[crt_n]);
        }
        if ( oper_cond_timeout_id[crt_n] != 0 )
        {
            gtk_timeout_remove(oper_cond_timeout_id[crt_n]);
            oper_cond_timeout_id[crt_n] = 0;
        }
         
}


void	SubRedraw (GtkWidget* w )
{
	int	crt_n;

	GetCrtNumberOfWgt ( w, &crt_n);
  	if( crt_n < 0 )
  		return; 

	DrawOperCondDsp ( crt_n, OperCondOffDrawable[crt_n] );	
	
	gdk_window_copy_area (w->window,info[crt_n].interlock_gc,0,0,
		OperCondOffDrawable[crt_n],0,0,sub_dsp_box_width, sub_dsp_box_height);		
}


