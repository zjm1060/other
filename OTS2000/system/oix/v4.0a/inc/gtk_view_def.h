
/*******************************
created by zyp on 2005.11 for v4.0a gtk 
SYN by hcl 2007.12.12 
***********************************/
#include "gtk_widgets.h"

#ifndef _VIEW_DEF_H_
#define _VIEW_DEF_H_
typedef	struct
{
	int	width, height;
	float	xfactor, yfactor;
	GtkWidget  *canvas;
} VIEWDB;

VIEWDB	viewdb[MAX_CRT_NUMBER];

 
GtkWidget *view_win[MAX_CRT_NUMBER];
#define VIEWCLASS  "ViewClass"	// ctrl menu window class
#define  VIEW_WIDTH    200
#define  VIEW_HEIGHT	150


//int	View( int );
//LONG CALLBACK ViewProc(HWND, UINT, WPARAM, LONG);
//void DrawViewRect (int, HWND, HDC);

void    RedrawView (GtkWidget *w);
int		PopupMessageBoxRet (GtkWidget *parent,char msg[] );
void	GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
void	ClearArea (int crt_n,  GdkDrawable	*drawable,int x1,int y1,int width,int height,int expose_mode );
void	DrawDspBackground (int crt_n,  GdkDrawable *drawable );
void	CheckZoomOrigin (int crt_n );
void    Redraw (GtkWidget *w);
int		ColorId3to4(int id3);
void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype);

#endif