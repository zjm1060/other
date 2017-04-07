
/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2005.9.17 
	trasplant from Solaris to windows by chi.hailog 
	Date : 2006.9.07
	SYNC by hcl 2007.12.09
     

---------------------------------------------------------------------*/


#include <gtk/gtk.h>

#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"
#include "../inc/gtk_view_def.h"
 
extern void	Redraw(GtkWidget*);
void	CheckZoomOrigin (int crt_n );


float	alf_x, alf_y;

int	XtoSX (int crt_n,int x)
{
        	 
	return (int)(x*zoomdb[crt_n].factor	+zoomdb[crt_n].sx_orig);

}


int	YtoSY (int crt_n,int y)
{
     
	return (int)(y*zoomdb[crt_n].factor	+zoomdb[crt_n].sy_orig);
}

int	SXtoX (int crt_n,int sx)
{
	 
	return (int)((sx-zoomdb[crt_n].sx_orig)/zoomdb[crt_n].factor);

}


int	SYtoY (int crt_n,int sy)
{
	 
    return (int)((sy-zoomdb[crt_n].sy_orig)/zoomdb[crt_n].factor); 

}


void	ZoomInit (int crt_n )
{
        zoomdb[crt_n].factor     = 1;
        zoomdb[crt_n].factor_max = 4;   //old value --2
        zoomdb[crt_n].factor_min = 1;
        zoomdb[crt_n].step       = (float)1.2;

    	zoomdb[crt_n].sx_orig  	= 0;
    	zoomdb[crt_n].sy_orig	= 0;
/*
	  
*/
}

void	ResetZoom (int crt_n )
{
    zoomdb[crt_n].canvas_w =(int)(zoomdb[crt_n].canvas_w/zoomdb[crt_n].factor);
	zoomdb[crt_n].canvas_h =(int)(zoomdb[crt_n].canvas_h/zoomdb[crt_n].factor);
	zoomdb[crt_n].factor	= 1;
	zoomdb[crt_n].sx_orig	= 0;
	zoomdb[crt_n].sy_orig	= 0;
    zoomdb[crt_n].zoom_lock  = 0 ; 	 
	Redraw ( canvas[crt_n] );
	if (view_win[crt_n])
		RedrawView (viewdb[crt_n].canvas);

}


void	ZoomIn (int	crt_n, GdkPoint  *event )
{

    if (zoomdb[crt_n].zoom_lock) return ;
	if (zoomdb[crt_n].factor*zoomdb[crt_n].step < zoomdb[crt_n].factor_max)
	{	zoomdb[crt_n].factor	=zoomdb[crt_n].factor*zoomdb[crt_n].step; }
	else
	{ zoomdb[crt_n].factor	=zoomdb[crt_n].factor; return;	}

	alf_x	=(float)( event->x  - zoomdb[crt_n].sx_orig)/zoomdb[crt_n].canvas_w;
	alf_y	=(float)( event->y  - zoomdb[crt_n].sy_orig)/zoomdb[crt_n].canvas_h;
	zoomdb[crt_n].sx_orig	=(int)(zoomdb[crt_n].sx_orig	-alf_x*(zoomdb[crt_n].step - 1)*zoomdb[crt_n].canvas_w);
	zoomdb[crt_n].sy_orig	=(int)(zoomdb[crt_n].sy_orig	-alf_y*(zoomdb[crt_n].step - 1)*zoomdb[crt_n].canvas_h);

	CheckZoomOrigin ( crt_n );

	zoomdb[crt_n].canvas_w	=(int)(zoomdb[crt_n].canvas_w*zoomdb[crt_n].step);
	zoomdb[crt_n].canvas_h	=(int)(zoomdb[crt_n].canvas_h*zoomdb[crt_n].step);

	Redraw (canvas[crt_n] );
	if (view_win[crt_n])
		RedrawView (viewdb[crt_n].canvas);

}


void	ZoomOut (int crt_n, GdkPoint  *event )
{

    if (zoomdb[crt_n].zoom_lock) return ;
	if ((zoomdb[crt_n].factor-zoomdb[crt_n].factor_min) > (zoomdb[crt_n].step-1)*0.5)
	{	zoomdb[crt_n].factor	=zoomdb[crt_n].factor/zoomdb[crt_n].step;
	}
	else
	{	zoomdb[crt_n].sx_orig	=0; zoomdb[crt_n].sy_orig =0; return;	}

	alf_x	=(float)(event->x - zoomdb[crt_n].sx_orig)/zoomdb[crt_n].canvas_w;
	alf_y	=(float)(event->y - zoomdb[crt_n].sy_orig)/zoomdb[crt_n].canvas_h;

	zoomdb[crt_n].canvas_w	=(int)(zoomdb[crt_n].canvas_w/zoomdb[crt_n].step);
	zoomdb[crt_n].canvas_h	=(int)(zoomdb[crt_n].canvas_h/zoomdb[crt_n].step);

	zoomdb[crt_n].sx_orig	=(int)(zoomdb[crt_n].sx_orig	+alf_x*(zoomdb[crt_n].step - 1)*zoomdb[crt_n].canvas_w);
	zoomdb[crt_n].sy_orig	=(int)(zoomdb[crt_n].sy_orig	+alf_y*(zoomdb[crt_n].step - 1)*zoomdb[crt_n].canvas_h);

	CheckZoomOrigin ( crt_n );

	Redraw (canvas[crt_n] );
   	if (view_win[crt_n] )
		RedrawView (viewdb[crt_n].canvas) ; 
}



void	CheckZoomOrigin (int crt_n )
{
	int	sx_orig_max, sy_orig_max;

	sx_orig_max	=(int) (canvas_width*(1 -zoomdb[crt_n].factor));
	sy_orig_max	=(int) (canvas_height*(1 -zoomdb[crt_n].factor));

	zoomdb[crt_n].sx_orig	= MIN (zoomdb[crt_n].sx_orig, 0);
	zoomdb[crt_n].sx_orig	= MAX (zoomdb[crt_n].sx_orig, sx_orig_max);

	zoomdb[crt_n].sy_orig	= MIN (zoomdb[crt_n].sy_orig, 0);
	zoomdb[crt_n].sy_orig	= MAX (zoomdb[crt_n].sy_orig, sy_orig_max);
	 
}

