
#include	<windows.h>

#include	"../inc/ipm_k_def.h"
#include	"../inc/draw_def.h"
#include	"../inc/view_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/scaler.h"


int	XtoSX (x)
int	x;
{
	int	sx;
	sx	= (int)(x*zoomdb.factor) + zoomdb.sx_orig;
	return	sx;	

}


int	YtoSY (y)
int	y;
{
	int	sy;
	sy	= (int)(y*zoomdb.factor)	+ zoomdb.sy_orig;
	return	sy;	

}

int	SXtoX (sx)
int	sx;
{
	int	x;
	
	x	= (int)( (sx-zoomdb.sx_orig)/zoomdb.factor );
	
	return	x;	

}


int	SYtoY (sy)
int	sy;
{
	int	y;
	y	= (int)( (sy-zoomdb.sy_orig)/zoomdb.factor );
	
	return	y;	

}


void	ZoomInit ()
{
	zoomdb.factor     = 1.0f;
    zoomdb.factor_max = 16;
    zoomdb.factor_min = 1;
    zoomdb.step       = 1.2f;

	zoomdb.sx_orig  	= 0;
	zoomdb.sy_orig	= 0;

	zoomdb.canvas_w	= canvas_width;
	zoomdb.canvas_h	= canvas_height;

}

void ResetZoom ()
{

	zoomdb.factor	= 1.0f;
	zoomdb.sx_orig	= 0;
	zoomdb.sy_orig	= 0;

}


void	ZoomIn (hwnd, uMsg, wParam, lParam)
HWND	hwnd;
UINT	uMsg;
WPARAM	wParam;
LPARAM	lParam;
{
	float	alf_x, alf_y ;
	int xpos,ypos ;
    XRECT rc ;
	xpos	= LOWORD(lParam);	
	ypos	= HIWORD(lParam);
	xpos  = (int)(xpos /xscale ) ;
	ypos  = (int)(ypos / yscale ) ;
	if (edit_mode == DISPLAY_EDIT || edit_mode == SUB_DSP_EDIT ||  edit_mode == SYMBOL_DRAW  )
	{	
		if (zoomdb.factor*zoomdb.step < zoomdb.factor_max)
		{	zoomdb.factor	=zoomdb.factor*zoomdb.step; }
		else
		{ zoomdb.factor	=zoomdb.factor; return;	}

		alf_x	=(float)(xpos - zoomdb.sx_orig)/zoomdb.canvas_w;
		alf_y	=(float)(ypos - zoomdb.sy_orig)/zoomdb.canvas_h;
		zoomdb.sx_orig = zoomdb.sx_orig -
						(int)( alf_x*(zoomdb.step - 1)*zoomdb.canvas_w );
		zoomdb.sy_orig = zoomdb.sy_orig -
						(int)( alf_y*(zoomdb.step - 1)*zoomdb.canvas_h );

		CheckZoomOrigin ( );
		zoomdb.canvas_w =(int)(zoomdb.canvas_w*zoomdb.step);
		zoomdb.canvas_h =(int)(zoomdb.canvas_h*zoomdb.step);
///////////////////////////////////chi///////////////////////////////////////

         // Set vertical scroll bar range and page size

		 GetClientRect(hwnd,(LPRECT)&rc);
         si.cbSize = sizeof (si) ;
         si.fMask  = SIF_RANGE | SIF_PAGE |SIF_POS ;
         si.nMin   = 0 ;
         si.nMax   = zoomdb.canvas_h + 2  ;         
         si.nPage  = rc.y2- rc.y1   ;                  //canvas_height ;
		 si.nPos   = -zoomdb.sy_orig;        
         SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;

         // Set horizontal scroll bar range and page size

         si.cbSize = sizeof (si) ;
         si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS  ;
         si.nMin   = 0 ;
         si.nMax   = zoomdb.canvas_w +2  ; 
         si.nPage  = rc.x2- rc.x1  ;       //cxClient;     //canvas_width;      
		 si.nPos   = -zoomdb.sx_orig ;
         SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
 


//////////////////////////////////////////////////////////////////////////////
		if( IsWindow(view) )
			InvalidateRect(view, NULL, TRUE);	
		InvalidateRect(hwnd, NULL, TRUE);
	}
}


void	ZoomOut (hwnd, uMsg, wParam, lParam)
HWND	hwnd;
UINT	uMsg;
WPARAM	wParam;
LPARAM	lParam;
{
	int xpos ,ypos ;

	float	alf_x, alf_y;
    XRECT rc ;
	xpos	= LOWORD(lParam);	
	ypos	= HIWORD(lParam);
	xpos  = (int)(xpos /xscale ) ;
	ypos  = (int)(ypos / yscale ) ;

	if (edit_mode == DISPLAY_EDIT || edit_mode == SUB_DSP_EDIT ||  edit_mode == SYMBOL_DRAW )
	{	
		if ((zoomdb.factor-zoomdb.factor_min) > (zoomdb.step-1)*0.5)
		{	zoomdb.factor	=zoomdb.factor/zoomdb.step;
		}
		else
		{	zoomdb.sx_orig	=0; zoomdb.sy_orig =0; return;	}

		alf_x	=(float)(xpos - zoomdb.sx_orig)/zoomdb.canvas_w;
		alf_y	=(float)(ypos - zoomdb.sy_orig)/zoomdb.canvas_h;

		zoomdb.canvas_w	=(int)( (float)zoomdb.canvas_w/zoomdb.step );
		zoomdb.canvas_h	=(int)( (float)zoomdb.canvas_h/zoomdb.step );

		zoomdb.sx_orig = zoomdb.sx_orig +
							(int)( alf_x*(zoomdb.step - 1)*zoomdb.canvas_w );
		zoomdb.sy_orig = zoomdb.sy_orig +
							(int)( alf_y*(zoomdb.step - 1)*zoomdb.canvas_h );

		CheckZoomOrigin ( );
///////////////////////////////////chi///////////////////////////////////////

               // Set vertical scroll bar range and page size
         GetClientRect(hwnd,(LPRECT)&rc); 
         si.cbSize = sizeof (si) ;
         si.fMask  = SIF_RANGE | SIF_PAGE| SIF_POS ;
         si.nMin   = 0 ;
         si.nMax   = zoomdb.canvas_h + 2  ;         
         si.nPage  = rc.y2- rc.y1 ;   
		 si.nPos   = -zoomdb.sy_orig ; 
         SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;

               // Set horizontal scroll bar range and page size

         si.cbSize = sizeof (si) ;
         si.fMask  = SIF_RANGE | SIF_PAGE| SIF_POS ;
         si.nMin   = 0 ;
         si.nMax   = zoomdb.canvas_w + 2   ;  
         si.nPage  = rc.x2 -rc.x1  ;     
		 si.nPos   = -zoomdb.sx_orig ;
         SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
 


//////////////////////////////////////////////////////////////////////////////
		if( IsWindow(view))
			InvalidateRect(view, NULL, TRUE);	
		InvalidateRect(hwnd, NULL, TRUE);
	}
}



void CheckZoomOrigin ()
{
	int	sx_orig_max, sy_orig_max;

	sx_orig_max	= (int)( canvas_width*(1 - zoomdb.factor) );
	sy_orig_max	= (int)( canvas_height*(1 - zoomdb.factor) );

	zoomdb.sx_orig	= min (zoomdb.sx_orig, 0);
	zoomdb.sx_orig	= max (zoomdb.sx_orig, sx_orig_max);

	zoomdb.sy_orig	= min (zoomdb.sy_orig, 0);
	zoomdb.sy_orig	= max (zoomdb.sy_orig, sy_orig_max);

}


void	ZoomAreaMove (hwnd, uMsg, wParam, lParam)
HWND	hwnd;
UINT	uMsg;
WPARAM	wParam;
LPARAM	lParam;
{
	int	dx, dy;

	if (edit_mode == DISPLAY_EDIT || edit_mode == SUB_DSP_EDIT)
	{
		dx	= cur.bck.rect.x2 - cur.bck.rect.x1;
		dy	= cur.bck.rect.y2 - cur.bck.rect.y1;
		zoomdb.sx_orig	= zoomdb.sx_orig + dx;
		zoomdb.sy_orig	= zoomdb.sy_orig + dy;
		
		CheckZoomOrigin ( );

		if( IsWindow(view))
			InvalidateRect(view, NULL, TRUE);	
		InvalidateRect(hwnd, NULL, TRUE);
	}
}


