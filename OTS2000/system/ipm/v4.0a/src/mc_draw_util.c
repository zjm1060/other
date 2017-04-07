
#define _LeadByte 0x80   // added chi 05.02.25
#define _isleadbyte( _c)  ( (unsigned char)(_c) & _LeadByte)   //added chi 05.02.25
#define  G_PI    3.1415926535897  
//add 1

#include	<windows.h>
#include	<math.h>

#include	"resource.h"
#include	"../inc/ipm.h"
#include	"../inc/color_def.h"
#include	"../inc/draw_def.h"
#include	"../inc/dyn_form_def.h"
#include	"../inc/fill_bitmap.h"
#include	"../inc/layer_def.h"
#include	"../inc/text_edit_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/scaler.h"
#include	"../inc/pr.h"
#include	"../inc/symbol_def.h"


int  DrawArc (HDC, BACKRCD *, int);
int	XtoSX ( int);
int	YtoSY ( int);
void DynButtonInit ();
void EditButtonInit ();
void SetEditButton ();
void SetDynamicButton();
void CheckArcDirection( XRECT, XArc *);
void ClearPreviousSelect (HWND, NEW_GDBUFF * );
BOOL CheckPointsDistance (int, int, int, int, int);
void GridCheck (int *, int *, int *, int *);
void DrawGif(HWND ,HDC ,int,BACKRCD* );
void    DrawDialMeter ( HWND	hwnd, HDC	hdc, BACKRCD	*rcd ,NEW_LINKRCD *lnk);
void	DrawDynamicRcd (HWND hwnd ,HDC hdc , BACKRCD *rcd, NEW_LINKRCD *lnk  );

extern int _lstrlen(  LPCTSTR lpString  );  //added by chi 
//to test length of mixture of Double char and single char ;added by chi 
int _lstrlen(  LPCTSTR lpString  )
{
    int n =0;	
    while (*lpString)
	{   
		if( _isleadbyte(*lpString))
			lpString +=2 ;
        else
            lpString ++ ;
		n++ ;
	}
	return n ;
}


void DrawRcd ( hwnd, hdc, rcd)
HWND	hwnd;
HDC	hdc;
BACKRCD	*rcd;
{	   
	int		fill_type, iSize,hSize; //chi;
	HFONT hfont;
    int     x, y, width, height, xtmp, ytmp,linewidth;
	int    PenStyle ,fillbk ;  //   chi 
	HBRUSH	hOldBrush, hBrush;
	HPEN	hOldPen, hPen;
	HFONT	hOldFont;
	BACKRCD	tmp;
    POINT	plist[5];
	TEXTMETRIC  tm ;   int cxChar,cyChar  ,i   ;//chi
	char  *pChar ;
	char  sWBuff[2],sBuff[1] ; //chi
    
	if (sym_edit_mode	== EDIT_EXISTING_SYMBOL     ) 
		xscale= yscale = 1 ; 
	
	tmp	= *rcd;
	if ( hwnd == ghWnd )
	{
		tmp.rect.x1	= XtoSX ( rcd->rect.x1);
		tmp.rect.y1	= YtoSY ( rcd->rect.y1);
		tmp.rect.x2	= XtoSX ( rcd->rect.x2);
		tmp.rect.y2	= YtoSY ( rcd->rect.y2);
	}
    
	fill_type	= rcd->fill.fill_type;
	CheckRectCoords (tmp.rect, &xtmp, &ytmp, &width, &height);
	linewidth = _linewidth(rcd->line_width);  //chi
	
	PenStyle  = (_islinedash(rcd->line_width))?PS_DASH:PS_SOLID;  
	
	/* create brush, pen, set background, foreground */
	hBrush     = CreatePatternBrush (fill_pixmenu[rcd->fill.fill_type]);
	if (rcd->fill.fill_type >= MAX_FILL_PATTERN)
		printf ("rcd->fill.fill_type=%d  \n" , rcd->fill.fill_type ); 
	
	hOldBrush  = SelectObject(hdc, hBrush); 
	fillbk = (rcd->fill.fillbkmode ==0 )?TRANSPARENT: OPAQUE ; 
	
	/*	if (rcd->fill.fillbkmode ==0 )
	{
	fillbk =  TRANSPARENT ;
	SetROP2(hdc, R2_XORPEN);
	}else
	fillbk =  OPAQUE ;
	
	*/
	
	//modify by chi to speed up 
	if (rcd->draw_type == DrwT_SELECTION || _islinedash(rcd->line_width)  )
		SetBkMode(hdc,TRANSPARENT ); 
	else
	{
		
		SetBkMode(hdc, TRANSPARENT);
	}
	
	
	
	if (rcd->fill.fill_type == UNFILLED || rcd->draw_type == DrwT_LINE )
	{ 
		
		
		if (rcd->draw_type == DrwT_SELECTION)
            hPen = CreatePen(PenStyle,0 , Colors (ColorId3to4(WHITE)));//rcd->line_width   rcd->fg
		else 
			hPen = CreatePen(PenStyle,linewidth, Colors (rcd->fg)); // CHICHI
		hOldPen = SelectObject(hdc, hPen); 
	}
	else if (rcd->fill.fill_type == FILLED)
	{ 
		hPen = CreatePen(PenStyle, linewidth, Colors (rcd->fg));
		hOldPen = SelectObject(hdc, hPen);
		SetTextColor(hdc, Colors (rcd->bg));
		SetBkColor(hdc, Colors (rcd->fg));
	}
	else
	{
		hPen = CreatePen(PenStyle, linewidth, Colors (rcd->fg));
		hOldPen = SelectObject(hdc, hPen);
		SetTextColor(hdc, Colors (rcd->bg)); 
		SetBkColor(hdc, Colors (rcd->fg)); 
	}
	
	
	switch (rcd->draw_type)
	{
	case	DrwT_SELECTION:
		//modify by chi 
		
		CheckRectCoords (tmp.rect, &x, &y, &width, &height);//rcd->rect
		width = (int)( (x+width) * xscale);
		height = (int)( (y+height) * yscale);
		x = (int)(x * xscale);
		y = (int)(y * yscale);
		
		plist[0].x = x;     plist[0].y = y;
		plist[1].x = width; plist[1].y = y;
		plist[2].x = width; plist[2].y = height;
		plist[3].x = x;  	plist[3].y = height;
		plist[4].x = x;		plist[4].y = y;
		
		Polyline (hdc, plist, 5);
		
		
		break;
		
		
    case    DrwT_LINE_DASH:  //added by chi 
		SetBkMode(hdc,TRANSPARENT );
        hPen = CreatePen(PS_DASH, rcd->line_width, Colors (rcd->fg));
		hOldPen = SelectObject(hdc, hPen);
		SetTextColor(hdc, Colors (rcd->bg));
		SetBkColor(hdc, Colors (rcd->fg));
		
	case	DrwT_LINE:		
		MoveToEx( hdc, (int)(tmp.rect.x1 * xscale), (int)(tmp.rect.y1 * yscale), NULL); 
		LineTo(hdc, (int)(tmp.rect.x2 * xscale), (int)(tmp.rect.y2 * yscale));
		break;
		
	case	DrwT_ARC:
		if ( hwnd == ghWnd )
		{
			tmp.data.arc.x   = XtoSX ( tmp.data.arc.x);
			tmp.data.arc.y   = YtoSY ( tmp.data.arc.y);
			tmp.data.arc.width	= (int)(tmp.data.arc.width*zoomdb.factor);
			tmp.data.arc.height	= (int)(tmp.data.arc.height*zoomdb.factor);
        }
        DrawArc (hdc, &tmp, fill_type);
		break;
		
	case	DrwT_RECTANGLE:
		{
			CheckRectCoords (tmp.rect, &x, &y, &width, &height);
			width = (int)( (x+width) * xscale);
			height = (int)( (y+height) * yscale);
			x = (int)(x * xscale);
			y = (int)(y * yscale);
			plist[0].x = x;
			plist[0].y = y;
			plist[1].x = width;
			plist[1].y = y;
			plist[2].x = width;
			plist[2].y = height;
			plist[3].x = x;
			plist[3].y = height;
			plist[4].x = x;
			plist[4].y = y;
			if(fill_type == UNFILLED)
			{
				Polyline (hdc, plist, 5);
			}
			else
			{	
				
				// SetROP2(hdc,R2_XORPEN) ;
				Polygon (hdc, plist, 5); 
			}
		}
		break;
		
	case	DrwT_CIRCLE:
       	CheckRectCoords (tmp.rect, &x, &y, &width, &height);
		if ( fill_type == UNFILLED)
		{
			Arc (hdc, (int)(x * xscale), (int)(y * yscale), 
				(int)((x+width) * xscale), (int)((y+height) * yscale),
				(int)((x+width/2) * xscale), 
				(int)(y * yscale), (int)((x+width/2) * xscale),
				(int)(y * yscale) );
		}
		else
		{
			Ellipse (hdc, (int)(x * xscale), (int)(y * yscale), (int)((x+width) * xscale), (int)((y+height) * yscale));
		}
        break;
    case    DrwT_MODIFY_TEXT:
	case	DrwT_TEXT:  
		{
		/*			//增加了字体缩放功能 modified by chi
			*/          SetBkMode(hdc, fillbk );  // OPAQUE
			
			if ( toprint == TOPRINTON ) 
			{
				SetTextColor(hdc, RGB(0,0,0));
				SetBkColor(hdc, RGB(255,255,255));
				hOldFont = 	SelectObject(hdc, prt_font_set[rcd->ftset_id]); 	
			}
			else 
			{
				
				SetTextColor(hdc, Colors (rcd->fg));
				SetBkColor(hdc, Colors (rcd->bg));
				if (zoomdb.factor == 1.0  && xscale ==1 && yscale ==1) 
					hOldFont = 	SelectObject(hdc, font_set[rcd->ftset_id]);
				else
				{   
					iSize = (int)(font_size[rcd->ftset_id].height * zoomdb.factor* min(xscale,yscale));
					hSize = (int)(font_size[rcd->ftset_id].height * zoomdb.factor* yscale);
					hfont = CreateFont(iSize,0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						VARIABLE_PITCH | FF_DONTCARE, "RS_Song");
					hOldFont = SelectObject(hdc,hfont );
				}
			}
            if (rcd->data.text.DirOut == ITEM_TAG_VERTI	)   
			{
				// test 文字竖排 added chi
                GetTextMetrics (hdc, &tm) ;    
				cyChar = tm.tmHeight + tm.tmExternalLeading ;
                cxChar = tm.tmAveCharWidth ;
                pChar = rcd->data.text.text ;
				x = (int)(tmp.rect.x1 * xscale) ; 
				y = (int)(tmp.rect.y1 * yscale) ;   
				
				for ( i=0 ; i < _lstrlen(rcd->data.text.text); i++ )
				{
					
					if( _isleadbyte(  (*pChar)))
					{
						wcsncpy((WCHAR*)sWBuff ,(WCHAR*)pChar,sizeof(sWBuff));
                        TextOut(hdc, x, y,sWBuff ,sizeof(sWBuff));
						pChar +=2  ;
						
					}
					else 
					{
						strncpy(sBuff ,pChar,1);
                        TextOut(hdc, x, y,sBuff ,1);
						pChar ++  ;
						
					}
					y += cyChar ;
					
				}  //FOR 
			} //if 
			else   //横向打印；
				TextOut(hdc, (int)(tmp.rect.x1 * xscale), (int)(tmp.rect.y1 * yscale),
				rcd->data.text.text, strlen(rcd->data.text.text));
			
			////////////////////////////////////////////////////////////////////////////////////
			
			SelectObject(hdc, hOldFont);
			DeleteObject(hfont);
		}
		break;
    case    DrwT_GIF :
		
        DrawGif(hwnd,hdc,RE_DRAW,&tmp);
		break ;
	case	DrwT_PLINE:
		{
			
			int		i;
			POINT	plist[MAXPLCNT];
			
			if ( hwnd == ghWnd )
			{
				for (i=0; i< tmp.plcnt; i++)
				{
					tmp.data.pl[i].x = XtoSX ( tmp.data.pl[i].x);
					tmp.data.pl[i].y = YtoSY ( tmp.data.pl[i].y);
				}
			}
			
			for ( i = 0; i < tmp.plcnt; i++ ) {
				plist[i].x = (long)(tmp.data.pl[i].x * xscale);
				plist[i].y = (long)(tmp.data.pl[i].y * yscale);
			}
			
			if(fill_type == UNFILLED)
			{
				Polyline (hdc, plist, rcd->plcnt);
			}
			else
			{
				
				Polygon (hdc, plist, rcd->plcnt); 
			}
			
		}
		break;
		
	default:
		break;
		
	}
	
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	
}
//added by chi    hwnd, hdc, rcd


void DrawGif(HWND hwnd ,HDC hdc ,int DrawMode ,BACKRCD	*rcd)
{
	HBITMAP   		hBitmapImag ;
	//HINSTANCE 		hInstance ;
	int       	    cxBitmap, cyBitmap ;
	BITMAP          bitmap ;
	HDC             hdcMemImag;
	int             res;
	HFONT  hfont,hOldFont ;
	
	char GifFile[MAX_FILE_NAME_LEN] = ""  ;
	char part_name[MAX_FILE_NAME_LEN]= ""  ; 
	
	// Load the original image and get its size
	
	strcpy (part_name, "/home/ems/h9000/dsp/pic/");
	strcat (part_name, rcd->data.fname);
	//	FormFullName (home, GifFile, part_name);
    strcpy (GifFile,part_name);	
    hBitmapImag = LoadImage(ghInst,GifFile,IMAGE_BITMAP,0, 0 ,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION |LR_DEFAULTSIZE );
	if (hBitmapImag == NULL )
	{
		//printf ("Load Picture error! \n");
		rcd->draw_type = DrwT_RECTANGLE ;
		rcd->fill.fill_type = UNFILLED ;
		rcd->line_width = 1 ;
		rcd->bg = 0 ;
		rcd->fg = 1 ;
		DrawRcd(hwnd,hdc,rcd);
		SetBkMode(hdc, OPAQUE);
		SetTextColor(hdc, RGB(0,255,0));
		SetBkColor(hdc, RGB(0,0,0));
		hfont = CreateFont( (int)(15 * zoomdb.factor), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,VARIABLE_PITCH | FF_DONTCARE, "RS_Song");
		hOldFont = SelectObject(hdc,hfont );
		TextOut(hdc,min(rcd->rect.x2 + 10,rcd->rect.x1 +10) ,min(rcd->rect.y2 +10,rcd->rect.y1 +10),
			strcat(part_name,": \n Can not Found!!!\0"),strlen(strcat(part_name,": \n Can not Found!!!\0"))) ;
		
		SelectObject(hdc, hOldFont);
		DeleteObject(hfont);
		
		return ;
		
	}
	
    GetObject (hBitmapImag, sizeof (BITMAP), &bitmap) ;
    cxBitmap = bitmap.bmWidth ;
    cyBitmap = bitmap.bmHeight ;
	if (DrawMode == FIRST_DRAW)
	{
		rcd->rect.x2 = rcd->rect.x1 + cxBitmap;
		rcd->rect.y2 = rcd->rect.y1 + cyBitmap;
	}
	
	// Select the original image into a memory DC
    hdcMemImag  = CreateCompatibleDC (hdc) ;
    SelectObject (hdcMemImag, hBitmapImag) ;
	
    if (DrawMode == FIRST_DRAW)
		res = BitBlt (hdc, rcd->rect.x1  , rcd->rect.y1  , cxBitmap, cyBitmap, hdcMemImag, 0, 0, SRCCOPY) ;
    else
		res = StretchBlt (hdc, (int)(min(rcd->rect.x2,rcd->rect.x1)* xscale), (int)(min( rcd->rect.y2,rcd->rect.y1) * yscale),
		(int)(abs(rcd->rect.x2 - rcd->rect.x1 ) * xscale) , (int)(abs(rcd->rect.y2 - rcd->rect.y1)* yscale), 
		hdcMemImag, 0,  0,cxBitmap,cyBitmap, SRCCOPY) ;
	DeleteDC (hdcMemImag) ;
	DeleteObject (hBitmapImag) ;
	
}

int DrawArc (hdc, rcd, fill_type)
HDC		hdc;
BACKRCD	*rcd;
int	fill_type;
{
	int	x, y, x1, y1, x2, y2, width, height;
	XArc	*arc;
	
	arc = &(rcd->data.arc);
	CheckRectCoords (rcd->rect, &x, &y, &width, &height);
	if ( (arc->angel1 == ANGLE_0 && arc->angel2 == ANGLE_90) ||
		(arc->angel1 == ANGLE_90 && arc->angel2 == -ANGLE_90 ) )
	{
		x1 = arc->x+arc->width;
		y1 = arc->y+arc->height/2;
		x2 = arc->x+arc->width/2;
		y2 = arc->y;
	}
	else if ( (arc->angel1 == ANGLE_180 && arc->angel2 == -ANGLE_90) ||
		(arc->angel1 == ANGLE_90 && arc->angel2 == ANGLE_90 ) )	
	{
		x1 = arc->x+arc->width/2;
		y1 = arc->y;
		x2 = arc->x;
		y2 = arc->y+arc->height/2;
	}
	else if ( abs(arc->angel1) == ANGLE_180 )
	{
		x1 = arc->x;
		y1 = arc->y+arc->height/2;
		x2 = arc->x+arc->width/2;
		y2 = arc->y+arc->height;
	}
	else if ( (arc->angel1 == ANGLE_0 && arc->angel2 == -ANGLE_90) ||
		(arc->angel1 == ANGLE_270 && arc->angel2 == ANGLE_90) ) 
	{
		x1 = arc->x+arc->width/2;
		y1 = arc->y+arc->height;
		x2 = arc->x+arc->width;
		y2 = arc->y+arc->height/2;
	}
	else  return(1);  // error
	
	if ( fill_type == UNFILLED)
	{
		Arc (hdc, (int)(arc->x * xscale), (int)(arc->y * yscale), (int)((arc->x+arc->width) * xscale), 
			(int)((arc->y+arc->height) * yscale), (int)(x1 * xscale),
			(int)(y1 * yscale), (int)(x2 * xscale), (int)(y2 * yscale));
	}
	else
	{
		Pie (hdc, (int)(arc->x * xscale), (int)(arc->y * yscale), (int)((arc->x+arc->width) * xscale), 
			(int)((arc->y+arc->height) * yscale), (int)(x1 * xscale), (int)(y1 * yscale),
			(int)(x2 * xscale), (int)(y2 * yscale));
	}
	
	return(0);
}


void CheckRectCoords (rect, xleft, ytop, width, height)
XRECT	rect;
int	*xleft, *ytop, *width, *height;
{
	*xleft	= rect.x1;
	*width	= rect.x2 - rect.x1;
	if (*width < 0)
	{
		*width   = - *width;
		*xleft   = rect.x2;
	}
	
	*ytop   = rect.y1;
	*height	= rect.y2 - rect.y1;
	if ( *height < 0)
	{
		*height  = - *height;
		*ytop   = rect.y2;
	}
	
}


void DrawPage (HWND hwnd,HDC hdc, int x1, int  y1,int  width, int height)
{
	BACKRCD	rcdtmp;
	
	rcdtmp.fg	=  (  ColorId3to4( WHITE));     //canvas_bg_color;
	rcdtmp.rect.x1	= x1;
	rcdtmp.rect.y1	= y1;
	rcdtmp.rect.x2	= rcdtmp.rect.x1+width;
	rcdtmp.rect.y2	= rcdtmp.rect.y1+height;
	rcdtmp.fill.fill_type	= UNFILLED;
	rcdtmp.draw_type	= DrwT_RECTANGLE;
	rcdtmp.line_width = 1;
	
	DrawRcd (   hwnd, hdc, &rcdtmp );
}




void RubberDraw (hwnd, rcd, xpos, ypos)
HWND	hwnd;
BACKRCD		*rcd;
int		xpos, ypos;
{
	BACKRCD	rec;
	XArc	arc;
	HDC		hdc;
	
	hdc = GetDC(hwnd);
	
	if(ghPal) 
	{
		SelectPalette (hdc, ghPal, FALSE);
		RealizePalette (hdc);
	}
	
	SetROP2(hdc, R2_XORPEN);	 //   R2_NOT: Erases the previous object
	
	switch	(rcd->draw_type)
	{
	case	DrwT_SELECTION:
		DrawRcd ( hwnd, hdc, rcd);
		break;
		
	case	DrwT_ARC:
	case	DrwT_LINE:
	case    DrwT_LINE_DASH: 
	case	DrwT_RECTANGLE:
	case	DrwT_CIRCLE:
		DrawRcd ( hwnd, hdc, rcd);    //test 
		break;
		
	case	DrwT_SYMBOL:
		if (drawing_mode == DRAWING_ON)
		{
			DrawSymbol ( hwnd, hdc, rcd, symdb,0);
		}
		break;
		
	case	DrwT_PLINE:
		rec	= *rcd;
		rec.plcnt	=rcd->plcnt+1;
		DrawRcd ( hwnd, hdc, &rec);
		break;
		
	default:	;
		
	}
	
	if (rcd->draw_type != DrwT_PLINE)
	{
		GridCheck (&xpos, &ypos, &rcd->rect.x2, &rcd->rect.y2);
	}
	
	
	switch	(rcd->draw_type)
	{
	case	DrwT_SELECTION:
		DrawRcd ( hwnd, hdc, rcd);
		break;
		
	case	DrwT_ARC:
		CheckArcDirection (cur.bck.rect, &arc);
		cur.bck.data.arc = arc;
    case    DrwT_LINE_DASH:
	case	DrwT_LINE:
	case	DrwT_RECTANGLE:
	case	DrwT_CIRCLE:
		DrawRcd ( hwnd, hdc, rcd);
		break;
		
	case	DrwT_SYMBOL:    //chichi
		if (drawing_mode == DRAWING_ON)
		{
			DrawSymbol ( hwnd, hdc, rcd,symdb,0);
		}
		break;
		
	case	DrwT_PLINE:
		{	
			int	x, y;
			
			GridCheck (&xpos, &ypos, &x, &y);
			rcd->data.pl[rcd->plcnt].x	=x;
			rcd->data.pl[rcd->plcnt].y	=y;
			rec	= *rcd;
			rec.plcnt	=rcd->plcnt+1;
			DrawRcd ( hwnd, hdc, &rec);
		}
		break;
		
	default:	;
		
	}
	
	ReleaseDC(hwnd, hdc);
	
}




void ReadPlData ( rcd, x, y)
BACKRCD	*rcd;
int	x, y;
{
	rcd->data.pl[rcd->plcnt].x	=x;
	rcd->data.pl[rcd->plcnt].y	=y;
	rcd->plcnt++;
	
}
void CheckArcDirection( rect, arc_return)
XArc    *arc_return;
XRECT	rect;
{
    static int      major_axis;
    int             rx,ry;
	int     x1, y1, x2, y2;
	
	x1	=rect.x1;
	y1	=rect.y1;
	x2	=rect.x2;
	y2	=rect.y2;
	
	rx = abs(x1-x2);
	ry = abs(y1-y2);
	arc_return->width =  2* rx;
	arc_return->height = 2* ry;
	
	if (arc_mode == 0)   major_axis = 0;
	if ((!major_axis) && (rx || ry))
		major_axis = (rx < ry) ? HORZ_AXIS : VERT_AXIS;
	
	if (major_axis == HORZ_AXIS)
	{
		if (x1 < x2)
		{
			if(y1> y2)
			{
				/* in 2 region  */
				arc_return->angel2 = -90;
				arc_return->y = y2;
			}
			else
			{
				/* in 7 region  */
				arc_return->angel2 = 90;
				arc_return->y = y2-2*ry;
			}
			arc_return->angel1 = 180;
			arc_return->x = x1;
		}
		else
		{
			if(y1	> y2)
			{
				/* in 3  region  */
				arc_return->angel2 = 90;
				arc_return->y = y2;
			}
			else
			{
				/* in 6  region  */
				arc_return->angel2 = -90;
				arc_return->y = y2-2*ry;
			}
			arc_return->angel1 = 0;
			arc_return->x = x2-rx;
		}
		
	}
	else
	{
		if (x1 < x2)
		{
			if(y1	> y2)
			{
				/* in 1  region  */
				arc_return->angel1 = 270;
				arc_return->angel2 = 90;
				arc_return->y = y2-ry;
			}
			else if( y1	< y2)
			{
				/* in  8  region  */
				arc_return->angel1 = 90;
				arc_return->angel2 = -90;
				arc_return->y = y1;
			}
			arc_return->x = x1-rx;
		}
		else
		{
			if(y1> y2)
			{
				/* in  4  region  */
				arc_return->angel1 = -90;
				arc_return->angel2 = -90;
				arc_return->y = y2-ry;
			}
			
			else
			{
				/* in  5  region  */
				arc_return->angel1 = 90;
				arc_return->angel2 = 90;
				arc_return->y = y1;
			}
			arc_return->x = x2;
		}
		
	}
	arc_return->angel1=     arc_return->angel1*64;
	arc_return->angel2=     arc_return->angel2*64;
	
}
//added by chi

void SelectAllItems(NEW_GDBUFF	*gdb)
{
	NEW_GBRCD	*rcd; RECT rc ;
	int i ;
	gdbuff.nextpos = 0 ;
    for (i = 0;i <gdb->nextpos ;  i++)
	{
		rcd	= &gdb->buffer[i];
		if (rcd->bck.layer == cur.bck.layer)
		{
			gdbuff.buffer[gdbuff.nextpos] = *rcd;
			rcd->bck.tag =ITEM_TAG_ON;
			gdbuff.nextpos++;
			DrawHandle( ghWnd, &rcd->bck, 0);  //在元素的四个角标记选中符号chi
		}
	} //for 
	if (gdbuff.nextpos != 0)
		SetEditButton ();
	
	if (gdbuff.nextpos == 1)
	{
		EnableMenuItem( GetMenu( ghWnd ), ID_Resize, MF_BYCOMMAND | MF_ENABLED );
		EnableMenuItem( GetMenu( ghWnd ), ID_Reshape, MF_BYCOMMAND | MF_ENABLED );
		SetDynamicButton ();
	}
	GetClientRect(ghWnd, &rc); 
	InvalidateRect(ghWnd, &rc, TRUE);
} 
#define   MAX_SELECT_NUM    25 
void SearchItems ( hwnd, x1, y1, x2, y2, gdb,K_Control)  //modified by chi
HWND	hwnd;
int	x1, y1;
int	x2, y2;
NEW_GDBUFF	*gdb;
BOOL K_Control;
{
	int	i;
	int	point_offset	=2;
	
	int	xtmp, ytmp,xpos,ypos;
	int	width, height;
	NEW_GBRCD	*rcd;
	XRECT rc;
	
    if (((!K_Control)  ) || ((K_Control) && (gdbuff.nextpos >= MAX_SELECT_NUM))) //added chi  || (gdbuff.nextpos > 1)
	{ 
		gdbuff.nextpos	=0;
		ClearPreviousSelect( hwnd, gdb  );
	}
	
	
	if (( abs(x1-x2) <= point_offset || abs(y1-y2) <= point_offset))
	{
		for (i=gdb->nextpos-1 ; i >= 0; i--)
		{
			rcd	= &gdb->buffer[i];
			if (rcd->bck.layer == cur.bck.layer)
			{   
                
				rc.x1	=   ( rcd->bck.rect.x1); // XtoSX
				rc.y1	=   ( rcd->bck.rect.y1);
				rc.x2	=   ( rcd->bck.rect.x2);
				rc.y2	=   ( rcd->bck.rect.y2);
				CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height); //test ; chi 04.08.25
				switch (rcd->bck.draw_type)
				{
				case	DrwT_TEXT:
				case	DrwT_ARC:
				case	DrwT_RECTANGLE:
				case	DrwT_CIRCLE:
				case	DrwT_SYMBOL:
				case	DrwT_PLINE:
				case	DrwT_GIF:
					//CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height);
					if ( (x1 >= xtmp) && (x1 <= xtmp+width) &&
						(y1 >= ytmp) && (y1 <= ytmp+height))
					{
						gdbuff.buffer[gdbuff.nextpos] = *rcd;
						rcd->bck.tag =ITEM_TAG_ON;
						gdbuff.nextpos++;
						DrawHandle( hwnd, &rcd->bck, 0); //在元素的四个角标记选中符号chi
						i = -1 ;     // exit for loop;
						
					}
					break;
                case	DrwT_LINE:   //added by  chi 
                case	DrwT_LINE_DASH:
                    
					
					if ( (x1 >= xtmp-HDL_OFFSET  ) && (x1 <= xtmp+width+HDL_OFFSET  ) &&
						(y1 >= ytmp -HDL_OFFSET ) && (y1 <= ytmp+height+HDL_OFFSET ))
					{
						//Case 0: 一个点
                        if ((width <HDL_OFFSET) && (height <HDL_OFFSET ))
							goto IS_IN;
						
						//Case 1: 水平线
						if (height <HDL_OFFSET )
							goto IS_IN;
						
                        //Case 2: 垂直线
						if (width <HDL_OFFSET )
							goto IS_IN;
						
                        //Case else  : 斜线
                        if (width <= height)
						{
							xpos  = (y1-rc.y1 )*(rc.x2 -rc.x1 ) /(rc.y2 - rc.y1 ) + rc.x1 ;
							if ((x1 >= xpos - HDL_OFFSET*3  ) && (x1 <= xpos + HDL_OFFSET*3 ))
								goto IS_IN;
							else 
								goto IS_OUT;
						}
						else
						{
							ypos  = (x1-rc.x1 )*(rc.y2 -rc.y1 ) /(rc.x2 - rc.x1 ) + rc.y1 ;
							if ((y1 >= ypos - HDL_OFFSET*3  ) && (y1 <= ypos + HDL_OFFSET*3 ))
								goto IS_IN;
							else 
								goto IS_OUT;
						}   
						
						
IS_IN:  				{
							gdbuff.buffer[gdbuff.nextpos] = *rcd;
							rcd->bck.tag =ITEM_TAG_ON;
							gdbuff.nextpos++;
							DrawHandle( hwnd, &rcd->bck, 0); //在元素的四个角标记选中符号chi
						}
                        goto IS_OUT;
					}
					
IS_OUT:         	break;
				default:
					break;
					
				}  //switch   
			} //if 
			//added by chi
			if ((!K_Control) && (gdbuff.nextpos > 0)) 
				break;
            if ((K_Control) && (gdbuff.nextpos >= MAX_SELECT_NUM)) 
				break;
			
		}
	}
	else
		if (( abs(x1-x2) > point_offset || abs(y1-y2) > point_offset))
		{
			for (i=0; i < gdb->nextpos; i++)
			{
				rcd	= &gdb->buffer[i];
				if (rcd->bck.layer == cur.bck.layer)
				{
					switch (rcd->bck.draw_type)
					{
					case	DrwT_TEXT:
					case	DrwT_LINE:
					case	DrwT_LINE_DASH:
					case	DrwT_PLINE:
					case	DrwT_ARC:
					case	DrwT_SYMBOL:
					case	DrwT_RECTANGLE:
					case	DrwT_CIRCLE:
					case	DrwT_GIF:
						CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height);
						if ( (x1 <= xtmp) && (x2 >= xtmp+width) &&
							(y1 <= ytmp) && (y2 >= ytmp+height))
						{
							gdbuff.buffer[gdbuff.nextpos] = *rcd;
							rcd->bck.tag =ITEM_TAG_ON;
							gdbuff.nextpos++;
							if (gdbuff.nextpos == 1)
							{   
								DrawHandle( hwnd, &rcd->bck,1219);
								//rcd->bck.tag =ITEM_TAG_ON_P;					
							}
							else 
								DrawHandle( hwnd, &rcd->bck,1219);
						}
						
					default:
						break;
						
					}
				}
			}
		}
		
		if (gdbuff.nextpos != 0)
			SetEditButton ();
		
		if (gdbuff.nextpos == 1)
		{
			EnableMenuItem( GetMenu( ghWnd ), ID_Resize, MF_BYCOMMAND | MF_ENABLED );
			EnableMenuItem( GetMenu( ghWnd ), ID_Reshape, MF_BYCOMMAND | MF_ENABLED );
			SetDynamicButton ();
		}
		
}


void ClearPreviousSelect (hwnd, gdb     )
HWND	hwnd;
NEW_GDBUFF      *gdb;

{
	int	i;
	
	for (i=0; i< gdb->nextpos; i++)
	{
		if ( gdb->buffer[i].bck.tag == ITEM_TAG_ON)
		{ 	
			//	DrawHandle ( hwnd, &gdb->buffer[i].bck);
			gdb->buffer[i].bck.tag = ITEM_TAG_OFF;
		}
	}
    
	gdbuff.nextpos	=0;
	
	DynButtonInit ();
	EditButtonInit ();
	
}


void EditButtonInit ()
{
	int	i;
	
	for(i=0; i<10; i++)
		EnableMenuItem( GetMenu( ghWnd ), ID_Copy+i, MF_BYCOMMAND | MF_GRAYED );

//	CheckMenuItem(GetMenu( ghWnd ), ID_DISPLAY_DBNAME, MF_BYCOMMAND | MF_UNCHECKED);
	DrawMenuBar( ghWnd);
	
}

void SetPasteButton ()
{
	HMENU	hmenu;
	
	hmenu =  GetMenu( ghWnd );
	EnableMenuItem( hmenu, ID_Copy, MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Move, MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Cut,  MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Paste,MF_BYCOMMAND | MF_ENABLED );
	EnableMenuItem( hmenu, ID_Replace,MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Delete, MF_BYCOMMAND | MF_GRAYED );	
	EnableMenuItem( hmenu, ID_Resize, MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Reshape, MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Send_Fg, MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Send_Bg, MF_BYCOMMAND | MF_GRAYED );
	DrawMenuBar( ghWnd);
	
}

void SetEditButton ()
{
	HMENU	hmenu;
	
	hmenu =  GetMenu( ghWnd );
	EnableMenuItem( hmenu, ID_Copy, MF_BYCOMMAND | MF_ENABLED );
	EnableMenuItem( hmenu, ID_Move, MF_BYCOMMAND | MF_ENABLED );
	EnableMenuItem( hmenu, ID_Cut, MF_BYCOMMAND | MF_ENABLED );
	EnableMenuItem( hmenu, ID_Paste, MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( hmenu, ID_Replace, MF_BYCOMMAND | MF_ENABLED );
	EnableMenuItem( hmenu, ID_Delete, MF_BYCOMMAND | MF_ENABLED );	
	EnableMenuItem( hmenu, ID_Send_Fg, MF_BYCOMMAND | MF_ENABLED );
	EnableMenuItem( hmenu, ID_Send_Bg, MF_BYCOMMAND | MF_ENABLED );
	DrawMenuBar( ghWnd);
	
}

void CheckRectCoordsOfPline ( rcd, x, y, width, height)
BACKRCD         *rcd;
int     *x, *y, *width, *height;
{
	int	xmin, xmax, ymin, ymax;
	int	i, pt_cnt;
	
	xmin= rcd->data.pl[0].x;
	ymin= rcd->data.pl[0].y;
	xmax= rcd->data.pl[0].x;
	ymax= rcd->data.pl[0].y;
	
	pt_cnt= rcd->plcnt;
    for(i=1; i< pt_cnt; i++)
    {
		xmin= min( xmin, (int)rcd->data.pl[i].x);
		ymin= min( ymin, (int)rcd->data.pl[i].y);
		xmax= max( xmax, (int)rcd->data.pl[i].x);
		ymax= max( ymax, (int)rcd->data.pl[i].y);
    }
	*x 	= xmin;
	*y 	= ymin;
	*width 	=xmax - xmin;
	*height =ymax - ymin;
	rcd->rect.x1	=xmin;
	rcd->rect.y1	=ymin;
	rcd->rect.x2	=xmax;
	rcd->rect.y2	=ymax;
	
}


void ClearObjArea (hwnd, x1, y1, width, height)
HWND	hwnd;
int     x1, y1, width, height;
{
	RECT	rc;
	
	rc.left       =XtoSX (x1);
	rc.top        =YtoSY (y1);
	rc.right = rc.left + (int)(width*zoomdb.factor);
	rc.bottom = rc.top + (int)(height*zoomdb.factor);
	InvalidateRect(hwnd, &rc, TRUE); 
	
}

void ClearTagItems (hwnd, rcd)
HWND	hwnd;
BACKRCD		*rcd;
{
	int     xtmp, ytmp, width, height;
	int	margin;
	
	margin	=  8;
	if(rcd->draw_type ==  DrwT_PLINE)
	{
		CheckRectCoordsOfPline( rcd, &xtmp, &ytmp, &width, &height);
    }
    else
    {
		CheckRectCoords (rcd->rect, &xtmp, &ytmp, &width, &height);
	}
	
	/*clear the expose area */
	ClearObjArea (hwnd, (xtmp- margin), (ytmp- margin), (width+ margin*2), (height+ margin*2));
	
	/*clear the expose frame */
	ClearObjArea ( hwnd, xtmp, ytmp, margin, (height+1));
	ClearObjArea ( hwnd, (xtmp+width-margin), ytmp, (margin+1), (height+1));
	ClearObjArea ( hwnd, xtmp, ytmp, (width+1), margin);
	ClearObjArea ( hwnd, xtmp, (ytmp+height), (width+1), (margin+1));
	
}


void DrawHandleRect (hdc, x, y,mode)
HDC	hdc;
int	x, y,mode;
{   
	
	
	HBRUSH 	hBrush, hOldBrush;
	HPEN  hPen,hOldPen;
    int x1,x2,y1,y2 ;
	//SetPolyFillMode (hdc, WINDING) ;
    //modified by chi 
    if (mode == 1219)  
	{   
		hPen       = CreatePen(PS_SOLID,1, RGB(255,0,0) );
		hOldPen    = SelectObject(hdc, hPen);	    
	}else
	{
        hBrush       = CreateSolidBrush ( RGB(0,255,0) );
		hOldBrush = SelectObject(hdc, hBrush);  
	}
	
    x1 = (int)((x-HDL_OFFSET)*xscale) ;
	y1 = (int)((y-HDL_OFFSET)*yscale) ;
	x2 = (int)((x+HDL_OFFSET)*xscale) ;
	y2 = (int)((y+HDL_OFFSET)*yscale) ;
	//SetROP2(hdc,R2_COPYPEN);
	Rectangle ( hdc, (int)((x-HDL_OFFSET)*xscale), (int)((y-HDL_OFFSET)*yscale),
		(int)((x+HDL_OFFSET)*xscale), (int)((y+HDL_OFFSET)*yscale));
	//	FillRect(hdc, &rc, hbr);
    if (mode == 1219)  
	{    
		SelectObject(hdc, hOldPen);
		DeleteObject(hPen);
	}else
	{
        SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
	}
	
}

void DrawHandle ( HWND hwnd, BACKRCD* rcd, int Mode)
{
	int	x1, y1, x2, y2;
	int W,H ;
	XPoint  *pt_hdl;
	int	num_pt_hdl;
	XPoint	*pt_hdl_end;
	int	draw_type;
	XArc	arc;
    BOOL ltop_overlap;
    BOOL lbtm_overlap;
    BOOL rtop_overlap;
    BOOL rbtm_overlap;
	HDC	hdc;
	hdc = GetDC(hwnd);
	SetROP2(hdc,    R2_COPYPEN  );   //R2_XORPEN
	draw_type=	rcd->draw_type;
	
	x1      =XtoSX(rcd->rect.x1) ;
    y1      =YtoSY(rcd->rect.y1) ;
    x2      =XtoSX(rcd->rect.x2) ;
    y2      =YtoSY(rcd->rect.y2) ;
	W       =x2-x1;
	H       =y2-y1;
	
	if( draw_type == DrwT_LINE  || draw_type == DrwT_LINE_DASH) 
	{
		DrawHandleRect (hdc, x1, y1,Mode);
		DrawHandleRect (hdc, x2, y2,Mode );
	}
	else if( draw_type == DrwT_ARC)
    {
		arc.x   =XtoSX(rcd->data.arc.x);
        arc.y   =YtoSY(rcd->data.arc.y);
		
        ltop_overlap    = CheckPointsDistance(arc.x, x1, arc.y, y2, HDL_OFFSET);
        lbtm_overlap    = CheckPointsDistance(arc.x, x1, arc.y, y1, HDL_OFFSET);
        rtop_overlap    = CheckPointsDistance(arc.x, x2, arc.y, y2, HDL_OFFSET);
        rbtm_overlap    = CheckPointsDistance(arc.x, x2, arc.y, y1, HDL_OFFSET);
		
        if(lbtm_overlap || rtop_overlap)
        {
			DrawHandleRect (hdc, x1, y2,Mode );
            DrawHandleRect (hdc, x2, y1,Mode );
        }
        else 
        {
			DrawHandleRect (hdc, x1, y1,Mode);
            DrawHandleRect (hdc, x2, y2,Mode);
        }
    }
	else if( draw_type == DrwT_PLINE) 
	{
		pt_hdl		= rcd->data.pl;
        num_pt_hdl	= rcd->plcnt;
		pt_hdl_end	= pt_hdl+num_pt_hdl- 1;
		for( ; pt_hdl <= pt_hdl_end; pt_hdl++)
		{
			x1      =XtoSX(pt_hdl->x) ;
			y1      =YtoSY(pt_hdl->y) ;
			DrawHandleRect (hdc, x1, y1,Mode);
		}	
	}
	else    //modifyed chi
	{   
		
		DrawHandleRect (hdc, x1, y1,Mode);        //top left;
        DrawHandleRect (hdc, x1+(W/2), y1,Mode);  //top middle;
		DrawHandleRect (hdc, x2, y1,Mode);        //top right;
		DrawHandleRect (hdc, x1, y1+(H/2),Mode);  //horizon left;
        DrawHandleRect (hdc, x2, y1+(H/2),Mode);  //horizon right;
		DrawHandleRect (hdc, x1, y2,Mode);
		DrawHandleRect (hdc, x2, y2,Mode);
		DrawHandleRect (hdc, x1+(W/2), y1+H,Mode);  //bottom middle;
	}
	
	ReleaseDC(hwnd, hdc);
	
}

void ChangeRcdCoords (rcd, x1n, y1n, x2n, y2n)
BACKRCD	*rcd;
int	x1n, y1n, x2n, y2n;
{
	rcd->rect.x1	= x1n;
	rcd->rect.y1	= y1n;
	rcd->rect.x2	= x2n;
	rcd->rect.y2	= y2n;
	gdbuff.buffer[gdbuff.nextpos].bck	= *rcd;
}


void ChangeGdbuffValue (x1n, y1n, x2n, y2n, x, y, lastx, lasty)
int	*x, *y, *lastx, *lasty;
int	x1n, y1n, x2n, y2n;
{
	*x	=x1n	;
	*y	=y1n	;
	*lastx	=x2n	;
	*lasty	=y2n	;
	
}

/*
* Draw window background to solid black 
*/
void DrawBlackBackground(HWND hwnd, HDC hdc)
{
	HBRUSH hBrush, hOldBrush;
	RECT	rc;
	
	hBrush       = CreateSolidBrush (BLACK);
	hOldBrush = SelectObject(hdc, hBrush);
	GetClientRect (hwnd, &rc);
	Rectangle (hdc, rc.left, rc.top, rc.right, rc.bottom); // no right and bottom border
}

void ClearCanvas (HWND hwnd)
{
	RECT rc;
	
	v4_gdbase.nextpos  =0;
	gdbuff.nextpos  =0;
	zoomdb.factor   =1.0f;
	GetClientRect(hwnd, &rc);
	InvalidateRect( hwnd, &rc, TRUE);
}
///////////////////////////////////////////////////////////////////////
//            purpose : shape a rect use x,y as central added chi
//////////////////////////////////////////////////////////////////////////
void GetPosRect (rc, x, y)
XRECT	*rc;
int	x, y;
{
	
    rc->x1 = (int)((x-HDL_OFFSET) ) ;
	rc->y1 = (int)((y-HDL_OFFSET) ) ;
	rc->x2 = (int)((x+HDL_OFFSET) ) ;
	rc->y2 = (int)((y+HDL_OFFSET) ) ;
	
	
}


/////////////////////////////////////////////////////////////////////////////////
//            purpose : to judge current pos in selected RCD added by chi
/////////////////////////////////////////////////////////////////////////////////

IN_STATE IsInSelRCD(NEW_GDBUFF *gdb, int X,int Y, NEW_GBRCD	**rcd_ptr) 
//HWND	hwnd;
//GDBUFF      *gdb;
{
	int	i; 
    NEW_GBRCD	*rcd; 
	int	xtmp, ytmp,xpos,ypos;  //xpos 位置检验点
	int	width, height;
	XRECT rc ,rc_LU,rc_RU,rc_LB,rc_RB,rc_UM,rc_BM,rc_LM,rc_RM;
	POINT Pt;
	
	Pt.x =X ; Pt.y = Y ;
    if (gdb->nextpos == 0) 
		return IS_OUT ;
	
	for (i=gdb->nextpos-1 ; i >= 0; i--)
	{
		rcd	= &gdb->buffer[i];
		*rcd_ptr= &gdb->buffer[i];

		if (rcd->bck.layer == cur.bck.layer)
		{
			rc.x1	= XtoSX ( rcd->bck.rect.x1);
			rc.y1	= YtoSY ( rcd->bck.rect.y1);
			rc.x2	= XtoSX ( rcd->bck.rect.x2);
			rc.y2	= YtoSY ( rcd->bck.rect.y2);	
			CheckRectCoords (rc, &xtmp, &ytmp, &width, &height);// old rcd->bck.rect
			// FOUR ANGLE;  L---left; U--up;  R--rigth; etc.....
			GetPosRect( &rc_LU ,xtmp,ytmp);
			GetPosRect( &rc_RU ,xtmp+width,ytmp);
			GetPosRect( &rc_LB ,xtmp,ytmp+height);
			GetPosRect( &rc_RB ,xtmp+width,ytmp+height);
			
			GetPosRect( &rc_UM ,xtmp+width/2,ytmp);
			GetPosRect( &rc_LM ,xtmp,ytmp+height/2);
			GetPosRect( &rc_BM ,xtmp+width/2,ytmp+height);
			GetPosRect( &rc_RM ,xtmp+width,ytmp+height/2);
			
			switch (rcd->bck.draw_type)
			{
				
			case	DrwT_LINE:
			case	DrwT_LINE_DASH:
				
				if ( (X >= xtmp-HDL_OFFSET ) && (X <= xtmp+width+HDL_OFFSET ) &&
					(Y >= ytmp -HDL_OFFSET ) && (Y <= ytmp+height+HDL_OFFSET ))
				{
					
					//if (gdb->nextpos == 1) return IS_IN;  //TEST ; 
					//Case 0: 一个点
					if ((width <HDL_OFFSET) && (height <HDL_OFFSET ))
						return IS_IN;
					
					//Case 1: 水平线	
					if (height <HDL_OFFSET )
					{
						if (gdb->nextpos == 1) 
						{
							if ((X >= xtmp-HDL_OFFSET/2) && (X <= xtmp+HDL_OFFSET/2))
								return IS_LEFT;
							if ((X >= xtmp+width-HDL_OFFSET/2) && (X <= xtmp+width+HDL_OFFSET/2))
								return IS_RIGHT;
						}
						return IS_IN;
						
					}
					//Case 2: 垂直线
					if (width <HDL_OFFSET )
					{
						if (gdb->nextpos == 1) 
						{
							if ((Y >= ytmp-HDL_OFFSET/2) && (Y <= ytmp+HDL_OFFSET/2))
								return IS_UP;
							if ((Y >= ytmp+height-HDL_OFFSET/2) && (Y <= ytmp+height+HDL_OFFSET/2))
								return IS_BOTTOM;
						}
						return IS_IN;
						
					}
					//Case else  : 斜线
					if (width <= height)
					{
						xpos  = (Y-rc.y1 )*(rc.x2 -rc.x1 ) /(rc.y2 - rc.y1 ) + rc.x1 ;
						if ((X >= xpos - HDL_OFFSET) && (X <= xpos + HDL_OFFSET))
						{
							if (gdb->nextpos == 1) 
							{
								if ((X >= xtmp-HDL_OFFSET/2) && (X <= xtmp+HDL_OFFSET/2))
									return IS_UP;
								if ((X >= xtmp+width-HDL_OFFSET/2) && (X <= xtmp+width+HDL_OFFSET/2))
									return IS_BOTTOM;
							}
							return IS_IN;
						}
					}
					else
					{
						ypos  = (X-rc.x1 )*(rc.y2 -rc.y1 ) /(rc.x2 - rc.x1 ) + rc.y1 ;
						if ((Y >= ypos - HDL_OFFSET) && (Y <= ypos + HDL_OFFSET))
						{
							if (gdb->nextpos == 1)
							{
								if ((Y >= ytmp-HDL_OFFSET/2) && (Y <= ytmp+HDL_OFFSET/2))
									return IS_LEFT;
								if ((Y >= ytmp+height-HDL_OFFSET/2) && (Y <= ytmp+height+HDL_OFFSET/2))
									return IS_RIGHT;
							}							
							return IS_IN;
						}
						
					}
					
					return IS_OUT;
				} //endif 
				
				break;
				//  to be continue.......  chi
			case	DrwT_TEXT:
			case	DrwT_ARC:
			case	DrwT_RECTANGLE:
			case	DrwT_CIRCLE:
			case	DrwT_SYMBOL:
			case	DrwT_PLINE:
				// to be continue........  chi
			case	DrwT_GIF:
				
				
				if ( (X >= xtmp-HDL_OFFSET/2) && (X <= xtmp+width+HDL_OFFSET/2) &&
					(Y >= ytmp -HDL_OFFSET/2) && (Y <= ytmp+height+HDL_OFFSET/2))
				{
					if ((gdb->nextpos == 1) && (rcd->bck.draw_type !=DrwT_TEXT))
					{
						if (PtInRect((RECT*)&rc_LU,Pt))
							return IS_LU;
						if (PtInRect((RECT*)&rc_RU,Pt))
							return IS_RU;
						if (PtInRect((RECT*)&rc_LB,Pt))
							return IS_LB;
						if (PtInRect((RECT*)&rc_RB,Pt))
							return IS_RB;
						/////////////////////////////////////////////
						if (PtInRect((RECT*)&rc_UM,Pt))
							return IS_UP;
						if (PtInRect((RECT*)&rc_LM,Pt))
							return IS_LEFT;
						if (PtInRect((RECT*)&rc_BM,Pt))
							return IS_BOTTOM;
						if (PtInRect((RECT*)&rc_RM,Pt))
							return IS_RIGHT;
					}
					
					return IS_IN;  //default;
				}
				
				
				break;
				
			default:
				return IS_OUT ;	break;
				
				}  //end switch
			}
			//	if (gdb->nextpos > 0) break;
	}  //endfor 
	
	return IS_OUT ;  //default value 
}


BOOL IsInDynRCD(NEW_GDBUFF *gdb, int X,int Y, NEW_GBRCD	**rcd_ptr) 
{
	int	i; 
    NEW_GBRCD	*rcd; 
	int	xtmp, ytmp,xpos,ypos;  //xpos 位置检验点
	int	width, height;
	XRECT rc ,rc_LU,rc_RU,rc_LB,rc_RB,rc_UM,rc_BM,rc_LM,rc_RM;
	POINT Pt;
	
	Pt.x =X ; Pt.y = Y ;
    if (gdb->nextpos == 0) 
		return FALSE ;
	
	for (i=gdb->nextpos-1 ; i >= 0; i--)
	{
		rcd	= &gdb->buffer[i];
		*rcd_ptr= &gdb->buffer[i];

		if(!(rcd->lnk.dbname[0] || (rcd->lnk.act.ctrl_type==CTRL_REQ_DSP&&rcd->lnk.act.fmt_def.dsp.name[0])))
			continue;
		
		if (rcd->bck.layer == cur.bck.layer)
		{
			rc.x1	= XtoSX ( rcd->bck.rect.x1);
			rc.y1	= YtoSY ( rcd->bck.rect.y1);
			rc.x2	= XtoSX ( rcd->bck.rect.x2);
			rc.y2	= YtoSY ( rcd->bck.rect.y2);	
			CheckRectCoords (rc, &xtmp, &ytmp, &width, &height);// old rcd->bck.rect
			// FOUR ANGLE;  L---left; U--up;  R--rigth; etc.....
			GetPosRect( &rc_LU ,xtmp,ytmp);
			GetPosRect( &rc_RU ,xtmp+width,ytmp);
			GetPosRect( &rc_LB ,xtmp,ytmp+height);
			GetPosRect( &rc_RB ,xtmp+width,ytmp+height);
			
			GetPosRect( &rc_UM ,xtmp+width/2,ytmp);
			GetPosRect( &rc_LM ,xtmp,ytmp+height/2);
			GetPosRect( &rc_BM ,xtmp+width/2,ytmp+height);
			GetPosRect( &rc_RM ,xtmp+width,ytmp+height/2);
			
			switch (rcd->bck.draw_type)
			{
				
			case	DrwT_LINE:
			case	DrwT_LINE_DASH:
				
				if ( (X >= xtmp-HDL_OFFSET ) && (X <= xtmp+width+HDL_OFFSET ) &&
					(Y >= ytmp -HDL_OFFSET ) && (Y <= ytmp+height+HDL_OFFSET ))
				{
					
					//if (gdb->nextpos == 1) return IS_IN;  //TEST ; 
					//Case 0: 一个点
					if ((width <HDL_OFFSET) && (height <HDL_OFFSET ))
						return TRUE;
					
					//Case 1: 水平线	
					if (height <HDL_OFFSET )
					{
						if (gdb->nextpos == 1) 
						{
							if ((X >= xtmp-HDL_OFFSET/2) && (X <= xtmp+HDL_OFFSET/2))
								continue;
							if ((X >= xtmp+width-HDL_OFFSET/2) && (X <= xtmp+width+HDL_OFFSET/2))
								continue;
						}
						return TRUE;
						
					}
					//Case 2: 垂直线
					if (width <HDL_OFFSET )
					{
						if (gdb->nextpos == 1) 
						{
							if ((Y >= ytmp-HDL_OFFSET/2) && (Y <= ytmp+HDL_OFFSET/2))
								continue;
							if ((Y >= ytmp+height-HDL_OFFSET/2) && (Y <= ytmp+height+HDL_OFFSET/2))
								continue;
						}
						return TRUE;
						
					}
					//Case else  : 斜线
					if (width <= height)
					{
						xpos  = (Y-rc.y1 )*(rc.x2 -rc.x1 ) /(rc.y2 - rc.y1 ) + rc.x1 ;
						if ((X >= xpos - HDL_OFFSET) && (X <= xpos + HDL_OFFSET))
						{
							if (gdb->nextpos == 1) 
							{
								if ((X >= xtmp-HDL_OFFSET/2) && (X <= xtmp+HDL_OFFSET/2))
									continue;
								if ((X >= xtmp+width-HDL_OFFSET/2) && (X <= xtmp+width+HDL_OFFSET/2))
									continue;
							}
							return TRUE;
						}
					}
					else
					{
						ypos  = (X-rc.x1 )*(rc.y2 -rc.y1 ) /(rc.x2 - rc.x1 ) + rc.y1 ;
						if ((Y >= ypos - HDL_OFFSET) && (Y <= ypos + HDL_OFFSET))
						{
							if (gdb->nextpos == 1)
							{
								if ((Y >= ytmp-HDL_OFFSET/2) && (Y <= ytmp+HDL_OFFSET/2))
									continue;
								if ((Y >= ytmp+height-HDL_OFFSET/2) && (Y <= ytmp+height+HDL_OFFSET/2))
								continue;
							}							
							return TRUE;
						}
						
					}
					
					continue;
				} //endif 
				
				break;
				//  to be continue.......  chi
			case	DrwT_TEXT:
			case	DrwT_ARC:
			case	DrwT_RECTANGLE:
			case	DrwT_CIRCLE:
			case	DrwT_SYMBOL:
			case	DrwT_PLINE:
				// to be continue........  chi
			case	DrwT_GIF:
				
				
				if ( (X >= xtmp-HDL_OFFSET/2) && (X <= xtmp+width+HDL_OFFSET/2) &&
					(Y >= ytmp -HDL_OFFSET/2) && (Y <= ytmp+height+HDL_OFFSET/2))
				{
					if ((gdb->nextpos == 1) && (rcd->bck.draw_type !=DrwT_TEXT))
					{
						if (PtInRect((RECT*)&rc_LU,Pt))
							continue;
						if (PtInRect((RECT*)&rc_RU,Pt))
							continue;
						if (PtInRect((RECT*)&rc_LB,Pt))
							continue;
						if (PtInRect((RECT*)&rc_RB,Pt))
							continue;
						/////////////////////////////////////////////
						if (PtInRect((RECT*)&rc_UM,Pt))
							continue;
						if (PtInRect((RECT*)&rc_LM,Pt))
							continue;
						if (PtInRect((RECT*)&rc_BM,Pt))
							continue;
						if (PtInRect((RECT*)&rc_RM,Pt))
							continue;
					}
					
					return TRUE;  //default;
				}
				
				
				break;
				
			default:
				continue;
				break;
				
				}  //end switch
			}
			//	if (gdb->nextpos > 0) break;
	}  //endfor 
	
	return FALSE ;  //default value 
}


void	DrawDynamicRcd (HWND hwnd ,HDC hdc , BACKRCD *rcd, NEW_LINKRCD *lnk  )
{
	
	switch	( lnk->dyn.dyn_type )
	{
		
	case    DF_DIAL_METER:
		DrawDialMeter ( hwnd,hdc, rcd,lnk ); 
		break;   
		/*   case ... ... and so on ! chichi  */
	default:
		break;
	}
	
}




/* Dial meter  add by chichi */


int j ;
void    DrawDialMeter ( HWND	hwnd, HDC	hdc, BACKRCD	*rcd ,NEW_LINKRCD *lnk)
{
	
    int xc, yc ;
    DIAL_METER  dial ;
    XRectangle  rc ;
    double s,c;
    
    double theta, last, increment;
    int upper, lower;
    int tick_length;
    int i, inc;
    POINT points[6];
    HPEN  hPen,hOldPen ; 
	SetBkMode(hdc, OPAQUE);
	
	hPen = CreatePen(PS_SOLID,1, Colors (rcd->fg)); // CHICHI
	hOldPen = SelectObject(hdc, hPen); 
	
	SetBkColor(hdc, Colors (rcd->fg));
	
	
    rc.x = min(XtoSX(rcd->rect.x1),XtoSX( rcd->rect.x2));
    rc.y = min(YtoSY(rcd->rect.y1),YtoSY(rcd->rect.y2));
    rc.width  =  abs(XtoSX(rcd->rect.x2) - XtoSX(rcd->rect.x1));
    rc.height =  abs(YtoSY(rcd->rect.y2) - YtoSY(rcd->rect.y1));
    dial.radius = (int)(min (rc.width, rc.height) * 0.45);
    dial.pointer_width = dial.radius / 5;
    dial.angle   = (float)(G_PI/6 - j* G_PI / 60) ;
    j ++ ;
	
    if (dial.angle < - G_PI/6 ) dial.angle =  (float)(7* G_PI / 6); 
	
    upper = 100 ;
    lower = 0 ;
    xc = rc.x + rc.width / 2;
    yc = rc.y + rc.height / 2;
    s = sin (dial.angle);
    c = cos (dial.angle);
    
    points[0].x = (long)(xc + s*dial.pointer_width/2);
    points[0].y = (long)(yc + c*dial.pointer_width/2);
    points[1].x = (long)(xc + c*dial.radius);
    points[1].y = (long)(yc - s*dial.radius);
    points[2].x = (long)(xc - s*dial.pointer_width/2);
    points[2].y = (long)(yc - c*dial.pointer_width/2);
    points[3].x = (long)(xc - c*dial.radius/10);
    points[3].y = (long)(yc + s*dial.radius/10);
    points[4].x = points[0].x;
    points[4].y = points[0].y;
	
	Polygon (hdc, points, 5); 
    
    
    
    /* Draw Ticks */
    increment = (100*G_PI) / (dial.radius*dial.radius);
	
	inc = (upper - lower);
	
	while (inc < 100) inc *= 10;
	while (inc >= 1000) inc /= 10;
	last = -1;
	
	for (i = 0; i <= inc; i++)
    {
		theta = ((float)i*G_PI / (18*inc/24.) - G_PI/6.);
		
		if ((theta - last) < (increment))
			continue;     
		last = theta;
		
		s = sin (theta);
		c = cos (theta);
		
		tick_length = (i%(inc/10) == 0) ? dial.pointer_width : dial.pointer_width / 2;
		
		points[0].x = (long)(xc + c*(dial.radius - tick_length));
		points[0].y = (long)(yc - s*(dial.radius - tick_length));
		points[1].x =  (long)(xc + c*dial.radius);
		points[1].y =  (long)(yc - s*dial.radius);
		Polygon (hdc, points, 2); 
		
    }
    SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
    
}
