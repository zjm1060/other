
#include	<windows.h>
#include	<stdio.h>

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/layer_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/grid_def.h"
#include	"../inc/scaler.h"
#include	"../inc/text_edit_def.h"
#include	"resource.h"

#define   OFFSET 1   //added chi Offset Value when Arrow button press down!

extern int isDisplayDbName  ;

void ClearCanvas (HWND hwnd);
void RubberDraw (HWND, BACKRCD *, int, int);
void StoreObject ();
void MakeRectDirDown ( BACKRCD * );
void CheckArcDirection( XRECT, XArc *);
BOOL	CheckPointsDistance (int, int, int, int, int);
void SearchItems ( HWND, int, int, int, int, NEW_GDBUFF *,BOOL);
void AddValue_2 (short *, int *);
void AddValue (int *, int *);
int CopyItemsFinished (NEW_GDBUFF *,DWORD);
void CheckRectCoordsOfPline ( BACKRCD *, int *, int *, int *, int *);
BOOL	CheckPointInVerticalLine (int, int, int, int, int, int);
BOOL	CheckPointInHorizontalLine (int, int, int, int, int, int);
void CheckReshapeHandle ( int, int, int *);
void ChangeGdbuffValue (int, int, int, int, int *, int *, int *, int *);
void ResizeItemId ( int *, int *, int *, int *, int *);
void ResizeModeId ( int, int, int, int, int, int, int, int, int *);
void ResizeGdbuffItems (HWND, int, int);
void ReshapeGdbuffItems (HWND, int, int);
void ChangeRcdCoords (BACKRCD *, int, int, int, int);
void ReshapeItemsFinished (int,  NEW_GDBUFF *);
void ResizeItemsFinished (int, NEW_GDBUFF *);
void DrawGrid (HDC);
void GridCheck (int *, int *, int *, int *);
void	PopupTextInputDlg();
void DrawPage (HWND hwnd,HDC hdc, int x1, int  y1,int  width, int height);
int	SXtoX (int);
int	SYtoY (int);
extern  IN_STATE IsInSelRCD(NEW_GDBUFF *gdb, int X, int Y, NEW_GBRCD **rcd);
extern void RedrawAllItems(NEW_GDBUFF	*gdb);
extern void    DrawDialMeter ( HWND	hwnd, HDC	hdc, BACKRCD	*rcd ,NEW_LINKRCD *lnk);
extern void	DrawDynamicRcd (HWND hwnd ,HDC hdc , BACKRCD *rcd, NEW_LINKRCD *lnk  );
static BOOL en_Move ,en_Resize;  // added chi;
HMENU	hmenu; //addded chi
int s ; //test
int	reshp_hdl;
BOOL PushDown;
static short oldState;

void DrawInit ()
{      
	    
        cur.bck.draw_type	=DrwT_LINE ; // TEST  FROM DrwT_LINE CHI
        drawing_mode    =DRAWING_OFF;		 
   
}


void ShiftBtn1DoubleClick (HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam)
{
	BOOL	pl_points_overlap;
	int	x, y, xpos, ypos;
	HDC	hdc;

	xpos	= LOWORD(lParam);	
	ypos	= HIWORD(lParam);
	xpos    = (int)(xpos / xscale) ;
	ypos    = (int)(ypos / yscale) ;

	hdc = GetDC(hwnd);
	if(ghPal) 
	{
		SelectPalette (hdc, ghPal, FALSE);
    	RealizePalette (hdc);
	}

	GridCheck (&xpos, &ypos, &x, &y);
	cur.bck.rect.x1	=min (cur.bck.rect.x1, x);
	cur.bck.rect.y1	=min (cur.bck.rect.y1, y);
	cur.bck.rect.x2	=max (cur.bck.rect.x2, x);
	cur.bck.rect.y2	=max (cur.bck.rect.y2, y);

	pl_points_overlap=	CheckPointsDistance(
		x, cur.bck.data.pl[cur.bck.plcnt-1].x,
		y, cur.bck.data.pl[cur.bck.plcnt-1].y, click_pl_offset);

	if (pl_points_overlap == FALSE)	 
	{
		cur.bck.data.pl[cur.bck.plcnt].x= x;
		cur.bck.data.pl[cur.bck.plcnt].y= y;
		cur.bck.plcnt++;
		DrawRcd (hwnd, hdc, &cur.bck);
	}

	pl_points_overlap=	CheckPointsDistance (x, cur.bck.data.pl[0].x, 
		y, cur.bck.data.pl[0].y,  click_pl_offset );

	if(pl_points_overlap)
	{
		cur.bck.data.pl[cur.bck.plcnt].x= cur.bck.data.pl[0].x;
		cur.bck.data.pl[cur.bck.plcnt].y= cur.bck.data.pl[0].y;
		cur.bck.plcnt++;
		DrawRcd (hwnd, hdc, &cur.bck);
	}

	StoreObject ();
	drawing_mode	= DRAWING_OFF;
	cur.bck.plcnt	= 0;

	ReleaseDC(hwnd, hdc);
}


void	ButtonPressProcess (HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam)
{
	int	num_resize	= 0;
	int	x1, y1, x2, y2, xpos, ypos;
	int	draw_type;
	XArc    arc_data;
	BACKRCD	rcd;
	HDC		hdc;

	xpos	= LOWORD(lParam);	
	ypos	= HIWORD(lParam);
    PushDown = TRUE;  //ADDED BY chi ;
	hdc = GetDC(hwnd);
//    SetCapture(hwnd);
 xpos  = (int)(xpos /xscale ) ;
 ypos  = (int)(ypos / yscale ) ;
	switch (cur.bck.draw_type)
	{
	case	DrwT_SELECTION:
        if(gdbuff.nextpos > 0)
        {
        	rcd = gdbuff.buffer[gdbuff.nextpos-1].bck;  
        	draw_type = rcd.draw_type;
		}     
		GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
		cur.bck.rect.x2	= cur.bck.rect.x1;
		cur.bck.rect.y2	= cur.bck.rect.y1;
//	    DrawRcd (hwnd, hdc, &cur.bck);
        ButtonMotionProcess (  hwnd,   message,   uParam,   lParam) ; //8.24
		hmenu = GetMenu( ghWnd );
		if (GetMenuState ( hmenu,ID_AUTO_SIZE,MF_BYCOMMAND ) == MF_CHECKED)
		{
			if ((uParam == MK_LBUTTON) && (GetMenuState (hmenu,ID_Move,MF_BYCOMMAND )== MFS_ENABLED  ))//
			{ 
				if ((cur.bck.draw_type != DrwT_RESIZE) && en_Move==TRUE)
				{     
					oldState = cur.bck.draw_type ;
					cur.bck.draw_type   =DrwT_MOVE; 
				}
			}

			if ((uParam == MK_LBUTTON) && (GetMenuState (hmenu,ID_Resize,MF_BYCOMMAND )== MFS_ENABLED  ))//
			{       
				if ((cur.bck.draw_type != DrwT_MOVE) && en_Resize==TRUE)
				{         
					oldState = cur.bck.draw_type ;
					cur.bck.draw_type   =DrwT_RESIZE;
					goto Resize;     // should avoid use goto as this case  ; chi

				}	
			}
		} //END IF CHECKED 

		DrawGdbuffItems (hwnd, k_drag_items, k_mv_items);
        SetCapture (hwnd) ;       //to avoid a  bug  by chi
		drawing_mode    = DRAWING_ON;

		break;

	

	case	DrwT_RESIZE:
Resize: if ( gdbuff.nextpos == 1)
		{
			ResizeItemId (&x1, &y1, &x2, &y2, &draw_type);
			GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
			cur.bck.rect.x2	=cur.bck.rect.x1;
			cur.bck.rect.y2	=cur.bck.rect.y1;
			ResizeModeId ( cur.bck.rect.x2, cur.bck.rect.y2, x1, y1, x2, y2, 
				draw_type, k_drag_items, &resize_mode);	
//			ResizeGdbuffItems (w, k_drag_items, resize_mode);
		}
		drawing_mode    = DRAWING_ON;
		break;

	case	DrwT_COPY:
	case	DrwT_MOVE:
		GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
		cur.bck.rect.x2	= cur.bck.rect.x1;
		cur.bck.rect.y2	= cur.bck.rect.y1;
		DrawGdbuffItems (hwnd, k_drag_items, k_mv_items);
		drawing_mode    = DRAWING_ON;   
		break;
	
	case	DrwT_RESHAPE:
		if ( gdbuff.nextpos == 1)
		{
			GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
			cur.bck.rect.x2	=cur.bck.rect.x1;
			cur.bck.rect.y2	=cur.bck.rect.y1;
			CheckReshapeHandle (cur.bck.rect.x2, cur.bck.rect.y2, &reshp_hdl);
			ReshapeGdbuffItems (hwnd, k_drag_items, reshp_hdl);
		}
		drawing_mode    = DRAWING_ON;
		break;

	case	DrwT_TEXT:
		if (( drawing_mode == DRAWING_ON) && (strlen(text_buff) !=0))
        {
 
        	strcpy (cur.bck.data.text.text, text_buff);
        	StoreObject ();
        }
              
		drawing_mode	= DRAWING_ON;
		GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
	    start_x         = cur.bck.rect.x1;	   //nCaretPosX
        start_y         = cur.bck.rect.y1;
		cur.bck.rect.x2		= cur.bck.rect.x1;
		cur.bck.rect.y2		= cur.bck.rect.y1;
		cur.bck.data.text.text[0]	= '\0';
		text_buff[0]		= '\0';
 
		SetCaretPos(cur.bck.rect.x1, cur.bck.rect.y1);
		ShowCaret(hwnd);
		PopupTextInputDlg(); 
		break;  

	case	DrwT_ARC:
		drawing_mode    = DRAWING_ON;
		GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
        cur.bck.rect.x2	= cur.bck.rect.x1;
        cur.bck.rect.y2	= cur.bck.rect.y1;
        arc_mode	= 0;
        CheckArcDirection (cur.bck.rect, &arc_data);
        cur.bck.data.arc	= arc_data;
		if (IsLineDash)    //adde by chi 
				_setlinedash(cur.bck.line_width) ;   
			else
                _rsetlinedash(cur.bck.line_width) ;   
 
        break;
 
	case	DrwT_LINE:
    case    DrwT_LINE_DASH:
	case	DrwT_RECTANGLE:
	case	DrwT_CIRCLE:
		drawing_mode	= DRAWING_ON;
		GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
		cur.bck.rect.x2	= cur.bck.rect.x1;
		cur.bck.rect.y2	= cur.bck.rect.y1;
		if (IsLineDash)    //added by chi 
				_setlinedash(cur.bck.line_width) ;   
		else
                _rsetlinedash(cur.bck.line_width) ;  
 
		break;

	case	DrwT_PLINE:
		if (drawing_mode == DRAWING_OFF)
		{
			drawing_mode	= DRAWING_ON;
		}
		if (IsLineDash)
				_setlinedash(cur.bck.line_width) ;   
		else
                _rsetlinedash(cur.bck.line_width) ;  //added chi
		break;

	case	DrwT_GIF:
		//drawing_mode	=DRAWING_ON;
		GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
		//cur.bck.rect.x2	= cur.bck.rect.x1;
		//cur.bck.rect.y2	= cur.bck.rect.y1;

    	break;

	case	DrwT_SYMBOL:
		drawing_mode	=DRAWING_ON;
		GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
		cur.bck.rect.x2	=cur.bck.rect.x1;
		cur.bck.rect.y2	=cur.bck.rect.y1;

		break;

	default:
		break;
	}

	ReleaseDC(hwnd, hdc);
}
///////////////////////////////////////////////////////////////////////////
//purpose: to get absolute value  x y position    added chi ;
///////////////////////////////////////////////////////////////////////////

void GetPositionXY(HWND hwnd ,int * xpos, int*ypos, LPARAM lParam)
{ 
	XRECT rc;
	*xpos	=     (LOWORD(lParam));	
	*ypos	=     (HIWORD(lParam));

	//    *xpos &= 0x7FFFF ;

	GetClientRect(hwnd,(LPRECT)&rc);
	if ((*xpos >=rc.x2) && (*xpos <32767))
          *xpos=rc.x2-1  ;
	if ((*ypos >=rc.y2) && (*ypos <32767))
          *ypos=rc.y2 -1;
	if (*xpos >=32767)  // value is minus
          *xpos=1  ;
	if (*ypos >=32767)
          *ypos=1;
}


// modifyed chi
void	ButtonMotionProcess (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int     xpos, ypos, xmov, ymov;
    XArc    arc_data;
    POINT pt;
	HMENU hmenu;

    GetPositionXY( hwnd ,&xpos, &ypos,  lParam);
	xpos  = (int)(xpos / xscale ) ;
	ypos  = (int)(ypos / yscale ) ;
	pt.x =xpos; pt.y =ypos;


	if (PasteRec.Pasting)     //added chi 04.09.10
	{ 
		PasteRec.rect.x2 = (xpos);  //XtoSX
    	PasteRec.rect.y2 = (ypos);  //YtoSY
	}

/////////////////added chi /////////////////////////////////////

    if ((GetMenuState ( GetMenu(hwnd),ID_AUTO_SIZE,MF_BYCOMMAND ) == MF_CHECKED) && !PushDown  )
	{
		NEW_GBRCD	*rcd_tmp; 
	    switch (IsInSelRCD(&gdbuff, xpos,ypos,&rcd_tmp))
		{
    	case IS_LEFT:
    	case IS_RIGHT:
	    	 hCurOld =SetCursor(LoadCursor( ghInst ,  MAKEINTRESOURCE(IDC_SPLITH) ));
		     en_Resize = TRUE;
		     en_Move = FALSE ;
		     break;
    	case IS_UP:
    	case IS_BOTTOM:
             hCurOld =SetCursor(LoadCursor( ghInst ,  MAKEINTRESOURCE(IDC_SPLITV)));
    		 en_Resize = TRUE;
    		 en_Move = FALSE ;
    		 break;
    	case IS_LU:
    	case IS_RB:
             hCurOld =SetCursor(LoadCursor( ghInst ,  MAKEINTRESOURCE(IDC_SE)));
	    	 en_Resize = TRUE;
	    	 en_Move = FALSE ;
	    	 break;
    	case IS_RU:
	    case IS_LB:
             hCurOld =SetCursor(LoadCursor( ghInst ,  MAKEINTRESOURCE(IDC_SW)));
		     en_Resize = TRUE;
		     en_Move = FALSE ;
	    	 break;
        case IS_IN:
             hCurOld =SetCursor(LoadCursor( ghInst ,  MAKEINTRESOURCE(IDC_MOVE)));
	    	 en_Move = TRUE; 
	    	 en_Resize = FALSE ;
	    	 break;
        case IS_OUT:
             hCurOld =SetCursor(LoadCursor( NULL ,  MAKEINTRESOURCE(IDC_ARROW )));
		     en_Resize = FALSE;
		     en_Move = FALSE ;
		     break;
	    default:
		 
             SetCursor(LoadCursor( NULL ,  MAKEINTRESOURCE(IDC_ARROW )));
		     en_Resize = FALSE;
		     en_Move = FALSE ;
		     break;
		} //switch
	}// ENDIF CHECKED;
	else	
    SetCursor(hCurOld);
    //SetCursor(LoadCursor( ghInst ,  IDC_PEN )) ;

	if ( drawing_mode == DRAWING_ON && cur.bck.draw_type == DrwT_PLINE)
		RubberDraw (hwnd, &cur.bck, xpos, ypos); 
//	if ( drawing_mode = DRAWING_ON )
	if ( wParam & MK_LBUTTON ) 
	{
		switch (cur.bck.draw_type)
		{
		case	DrwT_SELECTION:
			RubberDraw (hwnd, &cur.bck, xpos, ypos);   
			break;

		case	DrwT_COPY:
		case	DrwT_MOVE:
		
			DrawGdbuffItems (hwnd, k_drag_items, k_mv_items);
			GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
			DrawGdbuffItems (hwnd, k_drag_items, k_mv_items);
			break;

		case	DrwT_RESIZE:
			if ( gdbuff.nextpos == 1 )
			{
				ResizeGdbuffItems (hwnd, k_drag_items, resize_mode);
				GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
				ResizeGdbuffItems (hwnd, k_drag_items, resize_mode);
			}
			break;

		case	DrwT_RESHAPE:
			if ( gdbuff.nextpos == 1 )
			{
				ReshapeGdbuffItems (hwnd, k_drag_items, reshp_hdl);
				GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
				ReshapeGdbuffItems (hwnd, k_drag_items, reshp_hdl);
			}
			break;

		case	DrwT_ARC:
                xmov	= abs(cur.bck.rect.x2-cur.bck.rect.x1);
                ymov	= abs(cur.bck.rect.y2-cur.bck.rect.y1);
                if ( (xmov <20)  && (ymov <20))
                        arc_mode	= 0;
                else
                {
                        arc_mode	= 1;
                        CheckArcDirection (cur.bck.rect, &arc_data);
                }
                cur.bck.data.arc = arc_data;
        case    DrwT_LINE_DASH:
		case	DrwT_LINE:
		case	DrwT_RECTANGLE:
		case	DrwT_CIRCLE:
			RubberDraw (hwnd, &cur.bck, xpos, ypos);
			break;

		case	DrwT_PLINE:
			if (drawing_mode == DRAWING_ON)
			{ RubberDraw (hwnd, &cur.bck, xpos, ypos); }
			break;

		case	DrwT_GIF:
            RubberDraw (hwnd, &cur.bck, xpos, ypos);
			break;

		case	DrwT_SYMBOL:
			RubberDraw (hwnd, &cur.bck, xpos, ypos);
			break;

		default:	;
		}
	}

	hmenu = GetMenu( hwnd );
	if(GetMenuState ( hmenu,ID_DISPLAY_DBNAME,MF_BYCOMMAND ) == MF_CHECKED)
	{
		HFONT hOldFont;
		HDC	hdc = GetDC(hwnd);
		static RECT	tip_rect;
		static NEW_GBRCD *tip_rcd, *tmp_rcd; 

		if(IsInDynRCD(&v4_gdbase,xpos,ypos,&tmp_rcd))
		{
			RECT	rc;
			SIZE	str_size;
			char	longname[256],string[256];
			
			if(tip_rect.right==0 || tip_rcd!=tmp_rcd)
			{
				tip_rcd= tmp_rcd;

				InvalidateRect(hwnd,&tip_rect,TRUE);
				SendMessage(hwnd,WM_PAINT,(WPARAM)hdc,0);
				
				GetClientRect(hwnd, &rc);
				SetTextColor(hdc, Colors (cur.bck.fg));
				SetBkColor(hdc, Colors (cur.bck.bg));
				hOldFont = 	SelectObject(hdc, font_set[cur.bck.ftset_id]);

				if(tmp_rcd->lnk.dbname[0])
				{
					if(-1==GetEntryLongnameByNameStr(tip_rcd->lnk.dbname, longname))
						sprintf(string, "%s %s", tip_rcd->lnk.dbname, "No such Pt");
					else
						sprintf(string, "%s %s", tip_rcd->lnk.dbname, longname);
				}
				else
					sprintf(string, "%s", tip_rcd->lnk.act.fmt_def.dsp.name);
					

				GetTextExtentPoint(hdc,string, strlen(string), &str_size);
				if(xpos>(rc.right-str_size.cx))
					xpos= rc.right-str_size.cx;
				tip_rect.left=xpos;
				tip_rect.right=tip_rect.left+str_size.cx;
				tip_rect.top= ypos-12;
				tip_rect.bottom= tip_rect.top+str_size.cy+4;
				
				InvalidateRect(hwnd,&tip_rect,TRUE);
				SendMessage(hwnd,WM_PAINT,(WPARAM)hdc,0);
				
				TextOut(hdc, xpos, ypos-10, string, strlen(string));
				
				SelectObject(hdc, hOldFont);
				DeleteObject(hOldFont);
				ReleaseDC(hwnd, hdc);
			}
		}
		else
		{
			InvalidateRect(hwnd,&tip_rect,TRUE);
			tip_rect.bottom=0;
			tip_rect.left=0;
			tip_rect.right=0;
			tip_rect.top=0;				
		}
	}
}


/////////////////////////////////////////////////////////////////////////
//  Purpose:  Capture message of doubleclick   added chi
////////////////////////////////////////////////////////////////////////


void    ButtonDoubleClick    (HWND hWnd, UINT message, WPARAM uParam, LPARAM lParam)
{           int xpos,ypos;
	        xpos = LOWORD(lParam);
			ypos = HIWORD(lParam); 
			xpos    = (int)(xpos / xscale) ;
			ypos    = (int)(ypos / yscale) ;
	        if ((gdbuff.buffer[gdbuff.nextpos-1].bck.draw_type == DrwT_TEXT) && ( gdbuff.nextpos ==1))  //mod chi
			     cur.bck.draw_type 	= DrwT_MODIFY_TEXT;
           	if ((cur.bck.draw_type == DrwT_MODIFY_TEXT) && ((uParam & MK_CONTROL)== 0))
			{
		        if (( drawing_mode == DRAWING_ON) && (strlen(text_buff) !=0))
				{
//          	DrawCharCursor ( XtDisplay(w), XtWindow(w), &cur.bck, cur.bck.rect.x2);
            	strcpy (cur.bck.data.text.text, text_buff);
            	StoreObject ();
				}
              
	        	drawing_mode	= DRAWING_ON;
	        	GridCheck (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
	            start_x         = cur.bck.rect.x1;	   //nCaretPosX
                start_y         = cur.bck.rect.y1;
	        	cur.bck.rect.x2		= cur.bck.rect.x1;
		        cur.bck.rect.y2		= cur.bck.rect.y1;
		        cur.bck.data.text.text[0]	= '\0';
		        text_buff[0]		= '\0';

		        SetCaretPos(cur.bck.rect.x1, cur.bck.rect.y1);
		        ShowCaret(hWnd);
	        	PopupTextInputDlg();
				
			}  //end if 
}


void	ButtonReleaseProcess (HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam)
{
	int	width, height;
	int	xtmp, ytmp, xpos, ypos;
	XArc    arc_data;
	HDC		hdc; RECT rc ;
    PushDown = FALSE;  //ADDED BY chi ;
	if (drawing_mode != DRAWING_ON)
	{
		return;
	}

//  drawing_mode	= DRAWING_OFF;
    GetPositionXY( hwnd ,&xpos, &ypos,  lParam);
	xpos  = (int)(xpos / xscale ) ;
	ypos  = (int)(ypos / yscale ) ;
	hdc = GetDC(hwnd);
	if(ghPal) 
	{
		SelectPalette (hdc, ghPal, FALSE);
    	RealizePalette (hdc);
	}

	// Draws the new object
    SetROP2(hdc, R2_COPYPEN);

	if (drawing_mode == DRAWING_ON)
	{
		switch (cur.bck.draw_type)
		{
		case	DrwT_SELECTION:
		 	GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
			CheckRectCoords (cur.bck.rect, &xtmp, &ytmp, &width, &height);
			SearchItems (hwnd,xtmp, ytmp, xtmp+width, ytmp+height, &v4_gdbase ,(uParam & MK_SHIFT));
			SetROP2(hdc, R2_XORPEN);
			DrawRcd (hwnd, hdc, &cur.bck);	
			GetClientRect(hwnd, &rc); 
			InvalidateRect(hwnd, &rc , TRUE);
            //RedrawAllItems( &v4_gdbase) ;  
			ReleaseCapture () ;  

			drawing_mode    = DRAWING_OFF;
			if (en_Resize  || en_Move)
			{
			    en_Resize = FALSE;	 en_Move = FALSE ;
			    cur.bck.draw_type = oldState ;
			}
			break;

		case	DrwT_MOVE:
		case	DrwT_COPY:
			DrawGdbuffItems (hwnd, k_drag_items, k_mv_items);
			GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
			if ( cur.bck.draw_type == DrwT_MOVE)
			{	
				DrawGdbuffItems (hwnd, k_paste_items, k_mv_items);
				MoveItemsFinished (&v4_gdbase,MOD_CUR,UP); //MODIFY chi
                GetClientRect(hwnd, &rc); 
			    InvalidateRect(hwnd, &rc, TRUE); //ADD chi
			}
			else 
			{
				DrawGdbuffItems (hwnd, k_paste_items, k_cp_items);
				CopyItemsFinished (&v4_gdbase,lParam);
				InvalidateRect(hwnd, NULL, TRUE);  //add chi
			}
			drawing_mode    = DRAWING_OFF;
			ReleaseCapture () ;  

			if(cur.bck.draw_type == DrwT_MOVE) InvalidateRect(hwnd, NULL, TRUE); 
			break;

		case	DrwT_RESIZE:
			if ( gdbuff.nextpos == 1)
			{
//				ResizeGdbuffItems (hwnd, k_drag_items, resize_mode);
				GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
				ResizeGdbuffItems (hwnd, k_paste_items, resize_mode);
				ResizeItemsFinished (resize_mode, &v4_gdbase);
			}
			drawing_mode    = DRAWING_OFF;
			InvalidateRect(hwnd, NULL, TRUE);
			en_Resize = FALSE;  //test
			ReleaseCapture () ;  //test

			break;

		case	DrwT_RESHAPE:
			if ( gdbuff.nextpos == 1)
			{
				ReshapeGdbuffItems (hwnd, k_drag_items, reshp_hdl);
				GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
				ReshapeGdbuffItems (hwnd, k_paste_items, reshp_hdl);
				ReshapeItemsFinished (reshp_hdl, &v4_gdbase);
			}
			drawing_mode    = DRAWING_OFF;
			ReleaseCapture () ;  //test
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case	DrwT_ARC:
        	GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
            CheckArcDirection (cur.bck.rect, &arc_data);
			
            cur.bck.data.arc	= arc_data;
            DrawRcd (hwnd, hdc, &cur.bck);
            drawing_mode    = DRAWING_OFF;
			ReleaseCapture () ;  //test
            StoreObject ();
            break;

		case	DrwT_LINE:
		case    DrwT_LINE_DASH:
		case	DrwT_RECTANGLE:
		case	DrwT_CIRCLE:
        	GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
		/*	if (IsLineDash)
				_setlinedash(cur.bck.line_width) ;   //added chi
			else
                _rsetlinedash(cur.bck.line_width) ;  //added chi   */
			DrawRcd (hwnd, hdc, &cur.bck);
			drawing_mode	= DRAWING_OFF;
			ReleaseCapture ( ) ;  //test
			StoreObject ( );
			break;

		case	DrwT_PLINE:
			if (drawing_mode == DRAWING_ON)
			{
				int	x, y;
				GridCheck (&xpos, &ypos, &x, &y);
				if(cur.bck.plcnt == 0)
				{
					cur.bck.rect.x1	= x;
					cur.bck.rect.y1	= y;
					cur.bck.rect.x2	= x;
					cur.bck.rect.y2	= y;
				}	
				cur.bck.data.pl[cur.bck.plcnt].x	= x;		
				cur.bck.data.pl[cur.bck.plcnt].y	= y;
				cur.bck.data.pl[cur.bck.plcnt+1].x	= x;		
				cur.bck.data.pl[cur.bck.plcnt+1].y	= y;
				cur.bck.rect.x1	=min (cur.bck.rect.x1, x);
				cur.bck.rect.y1	=min (cur.bck.rect.y1, y);
				cur.bck.rect.x2	=max (cur.bck.rect.x2, x);
				cur.bck.rect.y2	=max (cur.bck.rect.y2, y);
				if (cur.bck.plcnt ==27)
					MessageBox(ghWnd,">27?","",MB_OK);
				if (cur.bck.plcnt < MAXPLCNT-3 )
				{ cur.bck.plcnt++; }
				else { MessageBox (hwnd, "Polygone Buffer Over Flow", "IPM", MB_OK); }
				DrawRcd (hwnd, hdc, &cur.bck);
			}
			break;

		case	DrwT_SYMBOL:
        	GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
//			CheckRectCoords (cur.bck.rect, &xtmp, &ytmp, &width, &height);
			DrawSymbol (hwnd, hdc, &cur.bck,symdb,0);
			drawing_mode    = DRAWING_OFF;
			ReleaseCapture () ;  //test
			StoreObject ();
			break;

		case	DrwT_GIF:
        	//GridCheck (&xpos, &ypos, &cur.bck.rect.x2, &cur.bck.rect.y2);
			if (drawing_mode != DRAWING_ON) break;
			//CheckRectCoords (cur.bck.rect, &xtmp, &ytmp, &width, &height);
			//if ( width*height > 400 )
			{
				DrawGif (hwnd, hdc,FIRST_DRAW, &cur.bck);
				StoreObject ();
			}
			drawing_mode    = DRAWING_OFF;
			break;


		default:
			break;
		}
	}

	ReleaseDC(hwnd, hdc);
}


void StoreObject ()
{
	int	nextpos, dx, dy;
	int k_min_offset = 2;

	dx	= cur.bck.rect.x2	-cur.bck.rect.x1;
	dy	= cur.bck.rect.y2	-cur.bck.rect.y1;

	if (abs(dx) > k_min_offset || abs(dy) > k_min_offset)
	{
		nextpos	=v4_gdbase.nextpos;

        /* check for v4_gdbase space. */
		if (edit_mode == DISPLAY_EDIT)
		{
	    	if (nextpos >= MAXOBJECTS)
			{
			 	MessageBox (ghWnd, "Display Buffer Overflow!", "IPM: Display Edit", MB_OK);
				return; 
			}
		}
		else if (edit_mode == SYMBOL_EDIT)
		{
			if (nextpos >= MAX_SYMBOL_OBJECTS)
			{ 
				MessageBox (ghWnd, "Symbol Buffer Overflow!", "IPM: Symbol Edit", MB_OK);
				return; 
			}
		}
	
		MakeRectDirDown ( &cur.bck ); 

		v4_gdbase.buffer[nextpos]	= cur;

		v4_gdbase.nextpos++;
	}

}

void MakeRectDirDown ( rcd )
BACKRCD	*rcd;
{
	int     tmp;

	if ( rcd->draw_type != DrwT_LINE )
	{
		if ( rcd->rect.x1 > rcd->rect.x2 )
		{
			tmp	= rcd->rect.x1;
			rcd->rect.x1 	= rcd->rect.x2;
			rcd->rect.x2 	= tmp;
		}

		if ( rcd->rect.y1 > rcd->rect.y2 )
		{
			tmp	= rcd->rect.y1;
			rcd->rect.y1 	= rcd->rect.y2;
			rcd->rect.y2 	= tmp;
		}
	}

}


void SendItemsToForeground (gdb)
NEW_GDBUFF	*gdb;
{
	int	i, j, cnt;
	NEW_GBRCD	rcd;

	cnt	=0;
	for (i=gdb->nextpos-1; i>=0; i--)
	{
		if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
		{
			rcd	=gdb->buffer[i];
			for (j=i+1; j<gdb->nextpos-cnt; j++)
			{
				gdb->buffer[j-1]	=gdb->buffer[j];
			}
			gdb->buffer[gdb->nextpos-cnt-1]	=rcd;
			cnt++;
		}
    }

//	InvalidateRect(ghWnd, NULL, TRUE);

}


void SendItemsToBackground (gdb)
NEW_GDBUFF	*gdb;
{
	int	i, j, cnt;
	NEW_GBRCD	rcd;

	cnt	=0;
	for (i=0; i<gdb->nextpos; i++)
	{
		if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
		{
			rcd	=gdb->buffer[i];
			for (j=i-1; j>=cnt; j--)
			{
				gdb->buffer[j+1]	=gdb->buffer[j];
			}
			gdb->buffer[cnt]	=rcd;
			cnt++;
		}
	}
   //MoveItemsFinished (&v4_gdbase);
   //InvalidateRect(hwnd, NULL, TRUE);  //add chi


}


void PasteItems (gdb)
NEW_GDBUFF	*gdb;
{
	int	nextpos, i;
	// added chi : Clear all old lable ;
    for (i=0 ; i<gdb->nextpos; i++)
	{
         if (gdb->buffer[i].bck.tag == ITEM_TAG_ON )
			 gdb->buffer[i].bck.tag	= ITEM_TAG_OFF;
	}

	for (i=0; i<gdbuff.nextpos; i++)
	{
		nextpos	=gdb->nextpos;
       	if (nextpos >= MAXOBJECTS)
        {
			MessageBox (ghWnd, "Display Buffer is Full.", "IPM", MB_OK);
			return;
		}
		gdb->buffer[nextpos]   = gdbuff.buffer[i];
		gdb->buffer[nextpos].bck.layer	= cur.bck.layer;
        gdb->buffer[nextpos].bck.tag	= ITEM_TAG_ON;
		gdb->nextpos++;
	}
	InvalidateRect(ghWnd, NULL, TRUE);
}
extern  int  iPageMode ; 


void DrawPrimaryRcd ( HWND, HDC, BACKRCD *  ,NEW_LINKRCD *);

void	Redraw(hwnd, hdc)
HWND	hwnd;
HDC		hdc;
{
	int	i;
	BACKRCD	*rcd;
	NEW_LINKRCD *lnk;

	if (! iPageMode ) 
		DrawPage( hwnd, hdc , -10 ,-10 ,1380, 900 ) ;
	else
		DrawPage( hwnd, hdc , -10 ,-10 ,1380, 780 ) ;   /*·ÂÕæÒ³Ãæ!*/

	if (grid_fb == GRID_BACKGROUND) DrawGrid (hdc);

	for ( i=0; i < v4_gdbase.nextpos; i++)
	{
		rcd	= &v4_gdbase.buffer[i].bck;
		lnk = &v4_gdbase.buffer[i].lnk ;
		if ( info.clutter == CLUTTER_ON )
		{
			DrawPrimaryRcd ( hwnd, hdc, rcd ,lnk);
		}
		else
		{
			if ( layerdb[rcd->layer].hide_tag == VISIBLE || cur.bck.layer == rcd->layer )
			{
				DrawPrimaryRcd ( hwnd, hdc, rcd,lnk );
			}
			else
			{
				if ( zoomdb.factor >= layerdb[rcd->layer].min_visible &&
					zoomdb.factor  <= layerdb[rcd->layer].max_visible)
				{
					DrawPrimaryRcd ( hwnd, hdc, rcd,lnk );
				}
			}
		}  
	}

	if ( grid_fb == GRID_FOREGROUND ) DrawGrid (hdc);

}


/// chichi 

void DrawPrimaryRcd ( hwnd, hdc, rcd,lnk )
HWND	hwnd;
HDC		hdc;
BACKRCD	*rcd;
NEW_LINKRCD *lnk ;

{
	BACKRCD tmp;	

	tmp = *rcd ;

	switch	( rcd->draw_type )
	{
	case	DrwT_SYMBOL:
		DrawSymbol (hwnd, hdc, rcd,symdb,0);
		break;

	case	DrwT_GIF:
		tmp.rect.x1	= XtoSX ( rcd->rect.x1);
		tmp.rect.y1	= YtoSY ( rcd->rect.y1);
		tmp.rect.x2	= XtoSX ( rcd->rect.x2);
		tmp.rect.y2	= YtoSY ( rcd->rect.y2);
		DrawGif (hwnd, hdc, RE_DRAW,&tmp);
		break;

	default:
		DrawRcd (hwnd, hdc, rcd);
	}


	if (lnk )
    if ( lnk->dyn.dyn_tag == DYN_TAG_ON 
			|| ( lnk->cgrp.tag == DYN_CGRP_TAG_ON &&  lnk->cgrp.cgrp_id !=0) 
			|| ( lnk->cgrp.tag == DYN_CGRP_TAG_ADD_ON && lnk->cgrp.cgrp_id !=0)) 
	{
          DrawDynamicRcd ( hwnd,hdc , rcd ,lnk  );
	}

	if ( rcd->tag == ITEM_TAG_ON)
	{ 
		DrawHandle (hwnd, rcd, 0); 
	}
	else if ( rcd->tag == ITEM_TAG_ON)
	{
		DrawHandle (hwnd, rcd, 0);	
	}   // 1219 continue .... chi

}

//added chi  
    
void DrawRCDFrame(HWND	hwnd,HDC hdc, BACKRCD *rcd)
{

	        HPEN	hOldPen, hPen; //added c chi
            POINT	plist[5];
	        int x,y,width,height ;
			XRECT rc ;

	         
		    SetBkMode(hdc, TRANSPARENT);  // OPAQUE
            hPen = CreatePen(PS_DOT, 2, RGB(255,255,255)); //Colors (rcd->fg) ,rectmp.line_width
	    	hOldPen = SelectObject(hdc, hPen);
		    

			rc.x1	= XtoSX ( rcd->rect.x1);  // used for zoom scale chi
		    rc.y1	= YtoSY ( rcd->rect.y1);
	    	rc.x2	= XtoSX ( rcd->rect.x2);
		    rc.y2	= YtoSY ( rcd->rect.y2);

       		CheckRectCoords (rc , &x, &y, &width, &height);   //rcd->rect
			width = (int)( (x+width) * xscale);
			height = (int)( (y+height) * yscale);
			x = (int)(x * xscale);
			y = (int)(y * yscale);
  			plist[0].x = x;      plist[0].y = y;
			plist[1].x = width;	 plist[1].y = y;
			plist[2].x = width;  plist[2].y = height;
			plist[3].x = x;      plist[3].y = height;
			plist[4].x = x;      plist[4].y = y;
		
			Polyline (hdc, plist, 5);
		   	SelectObject(hdc, hOldPen);
	       	DeleteObject(hPen);                 
		
}
 
void DrawGdbuffItems (hwnd, k_mode, k_hdl)
HWND	hwnd;
int	k_mode;
int	k_hdl;
{
	int	i, dx, dy;
	HDC	hdc;
	BACKRCD	rectmp;
	int	draw_type;
    
	hdc = GetDC(hwnd);
	if(ghPal) 
	{
		SelectPalette (hdc, ghPal, FALSE);
    	RealizePalette (hdc);
	}

	dx	=cur.bck.rect.x2	-cur.bck.rect.x1;
	dy	=cur.bck.rect.y2	-cur.bck.rect.y1;

	if (k_mode == k_paste_items)
	{ SetROP2(hdc, R2_COPYPEN); }
	else if (k_mode == k_drag_items)
	{ SetROP2(hdc, R2_XORPEN); }

    for (i=0; i < gdbuff.nextpos; i++)
	{
		rectmp	=gdbuff.buffer[i].bck;
		draw_type	=rectmp.draw_type;

		if (draw_type == DrwT_PLINE)
		{
			short	x1,y1;
        	int     numpt;
			int	pl_cnt;

			pl_cnt=rectmp.plcnt;
			for(numpt=0; numpt< pl_cnt; numpt++)
			{	
				x1 = rectmp.data.pl[numpt].x;
				y1 = rectmp.data.pl[numpt].y;
				rectmp.data.pl[numpt].x=(x1+dx) ;
				rectmp.data.pl[numpt].y=(y1+dy) ;
			}

  		}
		else if(draw_type == DrwT_ARC)
        {
        	rectmp.data.arc.x =(gdbuff.buffer[i].bck.data.arc.x+dx);
            rectmp.data.arc.y =(gdbuff.buffer[i].bck.data.arc.y+dy);
        }

		rectmp.rect.x1	=(gdbuff.buffer[i].bck.rect.x1+dx) ;
		rectmp.rect.y1	=(gdbuff.buffer[i].bck.rect.y1+dy) ;
		rectmp.rect.x2	=(gdbuff.buffer[i].bck.rect.x2+dx) ;
		rectmp.rect.y2	=(gdbuff.buffer[i].bck.rect.y2+dy) ;
		//added chi DrwT_MOVE

        if  ((draw_type == DrwT_TEXT) || (draw_type == DrwT_SYMBOL )|| (draw_type == DrwT_RECTANGLE )||   //((draw_type == DrwT_TEXT) ||
			    (draw_type == DrwT_MOVE ) || (draw_type == DrwT_GIF )) 
			DrawRCDFrame( 	hwnd,  hdc, &rectmp) ;
		
        else 
	      	DrawPrimaryRcd ( hwnd, hdc, &rectmp,NULL );

//chichi		DrawHandle( hwnd, &rectmp, 0);  
		if(k_hdl== k_cp_items) {DrawHandle( hwnd, &rectmp, 0);  }
	
	}  //for
	ReleaseDC(hwnd, hdc);
}


int CopyItemsFinished (gdb,LoParam)
NEW_GDBUFF	*gdb;
DWORD   LoParam;
{
	int	dx, dy;
	int xpos,ypos;
	int	nextpos;
	int	i;
	int	k_min_copy_offset	= grid_width;
    xpos = LOWORD(LoParam);
	ypos = HIWORD(LoParam);

 
 
	dx	= cur.bck.rect.x2	-cur.bck.rect.x1;  //old code chi
	dy	= cur.bck.rect.y2	-cur.bck.rect.y1;


	if (abs(dx) > k_min_copy_offset || abs(dy) > k_min_copy_offset)
	{
		for (i=0; i<gdbuff.nextpos; i++)
		{
			nextpos	=gdb->nextpos;
	        if (nextpos >= MAXOBJECTS)
	        {
				MessageBox (ghWnd, "Display Buffer is Full.", "IPM", MB_OK);
				return 0;
			}
			gdb->buffer[nextpos]   =gdbuff.buffer[i];
			gdb->buffer[nextpos].bck.layer	=cur.bck.layer;
            gdb->buffer[nextpos].bck.tag	=ITEM_TAG_OFF;
			AddValue (&gdb->buffer[nextpos].bck.rect.x1, &dx);
			AddValue (&gdb->buffer[nextpos].bck.rect.y1, &dy);
			AddValue (&gdb->buffer[nextpos].bck.rect.x2, &dx);
			AddValue (&gdb->buffer[nextpos].bck.rect.y2, &dy);

			if(gdb->buffer[nextpos].bck.draw_type == DrwT_PLINE)
			{
				int	numpt, pl_cnt;

				pl_cnt= gdb->buffer[nextpos].bck.plcnt;
				for(numpt=0; numpt< pl_cnt; numpt++)
				{
					AddValue_2 ( &gdb->buffer[nextpos].bck.data.pl[numpt].x, &dx);
					AddValue_2 ( &gdb->buffer[nextpos].bck.data.pl[numpt].y, &dy);
               	}
			}
			else if(gdb->buffer[nextpos].bck.draw_type ==  DrwT_ARC)
            {
                AddValue_2 (&gdb->buffer[nextpos].bck.data.arc.x, &dx);
                AddValue_2 (&gdb->buffer[nextpos].bck.data.arc.y, &dy);
			}
			gdb->nextpos++;
		}

		return	1;
	}
	else
		return	0;

}


void MoveItemsFinished (gdb,m_mode,dir)  //modify chi
NEW_GDBUFF	*gdb;
MOVE_MODE m_mode ;
DIR dir ; //ADDED BY chi

{
	int	dx, dy;
	int	i;
    
	switch (m_mode)
    {
	case MOD_PASTE:
        if (PasteRec.Pasting )
		{    
		    dx	= PasteRec.rect.x2	-PasteRec.rect.x1;
		    dy	= PasteRec.rect.y2	-PasteRec.rect.y1; 
			dx  = (int)(dx / zoomdb.factor) ;
			dy  = (int)(dy /zoomdb.factor)  ;
		}
        break;
    case MOD_CUR:
	    dx	=cur.bck.rect.x2	-cur.bck.rect.x1;
    	dy	=cur.bck.rect.y2	-cur.bck.rect.y1;  
        break;
    case MOD_ARROW:   //chi
		switch (dir)
		{
		case UP:
			dx=0 ;dy=-OFFSET;
			break;
		case DOWN:
			dx=0 ;dy=OFFSET;
			break;
        case RIGHT:
			dx=OFFSET ;dy=0;
			break;
		case LEFT:
			dx=-OFFSET ;dy=0;
			break;
		}
        break;
    } 
    cur.bck.draw_type = DrwT_SELECTION ;   //add chi
     
	gdbuff.nextpos	=0;
	for (i=0; i<gdb->nextpos; i++)
	{
		if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
		{
//			ClearTagItems (&gdb->buffer[i].bck );	
			AddValue (&gdb->buffer[i].bck.rect.x1, &dx);
			AddValue (&gdb->buffer[i].bck.rect.y1, &dy);
			AddValue (&gdb->buffer[i].bck.rect.x2, &dx);
			AddValue (&gdb->buffer[i].bck.rect.y2, &dy);

			if(gdb->buffer[i].bck.draw_type ==	DrwT_PLINE)
			{	
				int	pl_cnt;
				int 	numpt, xtmp, ytmp, width, height;

				pl_cnt= gdb->buffer[i].bck.plcnt;
				for(numpt=0; numpt< pl_cnt; numpt++)
				{
					AddValue_2 (&gdb->buffer[i].bck.data.pl[numpt].x, &dx);
					AddValue_2 (&gdb->buffer[i].bck.data.pl[numpt].y, &dy);
				}
				CheckRectCoordsOfPline (&gdb->buffer[i].bck, &xtmp, &ytmp, &width, &height);
            }
      		else if(gdb->buffer[i].bck.draw_type ==  DrwT_ARC)
            {
            	AddValue_2 (&gdb->buffer[i].bck.data.arc.x, &dx);
                AddValue_2 (&gdb->buffer[i].bck.data.arc.y, &dy);
			}

			/*	update gdbuff data. */
			gdbuff.buffer[gdbuff.nextpos]	=gdb->buffer[i];
			gdbuff.nextpos	++;
		}
	}
  //InvalidateRect(hWnd, &rc, TRUE);
}



void ResizeGdbuffItems (hwnd, k_mode, k_resize_mode)
HWND	hwnd;
int	k_mode;
int	k_resize_mode;
{
	int	dx, dy;
	int	x, y, lastx, lasty;
	HDC	hdc;
//	char	text[MAX_TEXT_LEN+5];
	int	draw_type;
	BACKRCD	rcd, rcdtmp;
	int	fill;
	XArc	arc_data;

	hdc = GetDC(hwnd);

	if(ghPal) 
	{
		SelectPalette (hdc, ghPal, FALSE);
    	RealizePalette (hdc);
	}

	dx	=cur.bck.rect.x2	-cur.bck.rect.x1;
	dy	=cur.bck.rect.y2	-cur.bck.rect.y1;

	if (k_mode == k_paste_items)
	{ SetROP2(hdc, R2_COPYPEN); }
	else if (k_mode == k_drag_items)
	{ SetROP2(hdc, R2_XORPEN); }

	rcd	= gdbuff.buffer[gdbuff.nextpos-1].bck;
	rcdtmp	= rcd;
	draw_type	= rcd.draw_type;
	fill		= rcd.fill.fill_type;

	if (draw_type == DrwT_LINE  || draw_type == DrwT_LINE_DASH  )
	{
		switch( k_resize_mode)
		{
		case	UL_HANDLE:
			ChangeGdbuffValue (rcd.rect.x2, rcd.rect.y2, (rcd.rect.x1+dx), (rcd.rect.y1+dy), 
				&x, &y, &lastx, &lasty); 
			break;

		case	UR_HANDLE:
			ChangeGdbuffValue (rcd.rect.x1, rcd.rect.y1, (rcd.rect.x2+dx), (rcd.rect.y2+dy),
				 &x, &y, &lastx, &lasty); 
			break;

		case	WRONG_HANDLE:
			ChangeGdbuffValue (rcd.rect.x1, rcd.rect.y1, rcd.rect.x2, rcd.rect.y2,
				&x, &y, &lastx, &lasty); 
			break;

		default:	
			return;	

		}
	}	
	else
	{
		switch( k_resize_mode)
		{
		case	UL_HANDLE:
			ChangeGdbuffValue (rcd.rect.x2, rcd.rect.y2, (rcd.rect.x1+dx), (rcd.rect.y1+dy),
				&x, &y, &lastx, &lasty); 
			break;

		case	UR_HANDLE:
			ChangeGdbuffValue (rcd.rect.x1, rcd.rect.y2, (rcd.rect.x2+dx), (rcd.rect.y1+dy),
				&x, &y, &lastx, &lasty); 
			break;
	
		case	BL_HANDLE:
			ChangeGdbuffValue (rcd.rect.x2, rcd.rect.y1, (rcd.rect.x1+dx), (rcd.rect.y2+dy),
				&x, &y, &lastx, &lasty); 
			break;
		
		case	BR_HANDLE:
			ChangeGdbuffValue (rcd.rect.x1, rcd.rect.y1, (rcd.rect.x2+dx), (rcd.rect.y2+dy),
				&x, &y, &lastx, &lasty); 
			break;
	
		case	L_LINE_HANDLE:
			ChangeGdbuffValue (rcd.rect.x2, rcd.rect.y1, (rcd.rect.x1+dx), rcd.rect.y2,
				&x, &y, &lastx, &lasty); 
			break;
	
		case	T_LINE_HANDLE:
			ChangeGdbuffValue (rcd.rect.x2, rcd.rect.y2, rcd.rect.x1, (rcd.rect.y1+dy),
				&x, &y, &lastx, &lasty); 
			break;
	
		case	R_LINE_HANDLE:
			ChangeGdbuffValue (rcd.rect.x1, rcd.rect.y2, (rcd.rect.x2+dx), rcd.rect.y1, 
				&x, &y, &lastx, &lasty); 
			break;

		case	B_LINE_HANDLE:
			ChangeGdbuffValue (rcd.rect.x1, rcd.rect.y1, rcd.rect.x2, (rcd.rect.y2+dy),
				&x, &y, &lastx, &lasty); 
			break;
	
		case	WRONG_HANDLE:
			ChangeGdbuffValue (rcd.rect.x1, rcd.rect.y1, rcd.rect.x2, rcd.rect.y2,
				&x, &y, &lastx, &lasty); 
			break;
		default:	
			return;	
		}
	}
/*	if (k_mode == k_paste_items)
	{
		strcpy (text, rcd.data.text);
		SetRcdAttributes (XtDisplay(w), gctmp, &rcd);
	}
*/
	rcdtmp.rect.x1	= x;
	rcdtmp.rect.y1	= y;
	rcdtmp.rect.x2	= lastx;
	rcdtmp.rect.y2	= lasty;

	if (draw_type == DrwT_ARC)
	{
//		arc_mode	= 1;
		CheckArcDirection (rcdtmp.rect, &arc_data);
        rcdtmp.data.arc	= arc_data;
	}

	
	
	//DrawPrimaryRcd ( hwnd, hdc, &rcdtmp); //modify chi

	if ((draw_type == DrwT_TEXT) || (draw_type == DrwT_SYMBOL )
		    || (draw_type == DrwT_RECTANGLE ) || (draw_type == DrwT_GIF ))
			DrawRCDFrame( 	hwnd,  hdc, &rcdtmp) ;
		
        else 
	      	DrawPrimaryRcd ( hwnd, hdc, &rcdtmp , NULL);

//	DrawHandle (hwnd, &rcdtmp, 0);

	ReleaseDC(hwnd, hdc);
}


void ReshapeGdbuffItems (hwnd, k_mode, rshp_hdl)
HWND	hwnd;
int     k_mode;
int     rshp_hdl;
{
	int     x1, y1, x2, y2, dx, dy;
    HDC		hdc;
    XPoint  *reshp_pt;
    int     num_reshpt;
    int     fill    =UNFILLED;
	BACKRCD	rectmp;

	hdc = GetDC(hwnd);

	if(ghPal) 
	{
		SelectPalette (hdc, ghPal, FALSE);
    	RealizePalette (hdc);
	}

	dx	=cur.bck.rect.x2	-cur.bck.rect.x1;
	dy	=cur.bck.rect.y2	-cur.bck.rect.y1;

	if (k_mode == k_paste_items)
	{ SetROP2(hdc, R2_COPYPEN); }
	else if (k_mode == k_drag_items)
	{ SetROP2(hdc, R2_XORPEN); }


    reshp_pt	= gdbuff.buffer[gdbuff.nextpos-1].bck.data.pl;
    num_reshpt	= gdbuff.buffer[gdbuff.nextpos-1].bck.plcnt;
	rectmp = gdbuff.buffer[gdbuff.nextpos-1].bck;
	rectmp.draw_type= DrwT_LINE;
    if ( rshp_hdl == 0 )	// drag pline first point
    {
        x1	= (reshp_pt+ (reshp_hdl+1) ) -> x;
        y1	= (reshp_pt+ (reshp_hdl+1) ) -> y;
		rectmp.rect.x1	= x1;
		rectmp.rect.y1	= y1;
		rectmp.rect.x2	= cur.bck.rect.x2;
		rectmp.rect.y2	= cur.bck.rect.y2;
		DrawRcd (hwnd, hdc, &rectmp);
		if ( gdbuff.buffer[gdbuff.nextpos-1].bck.fill.fill_type != UNFILLED)
//		if ( (reshp_pt+ num_reshpt-1 )->x == reshp_pt->x && 
//			 (reshp_pt+ num_reshpt-1 )->y == reshp_pt->y )
		{	
	        x1	= (reshp_pt+ num_reshpt-1 ) -> x;
        	y1	= (reshp_pt+ num_reshpt-1 ) -> y;
			rectmp.rect.x1	= x1;
			rectmp.rect.y1	= y1;
			DrawRcd (hwnd, hdc, &rectmp);
		}
	}
    else if ( reshp_hdl == num_reshpt-1 ) // drag last point of pline
    {
        x1	= (reshp_pt+ (reshp_hdl-1) ) -> x;
        y1	= (reshp_pt+ (reshp_hdl-1) ) -> y;
		rectmp.rect.x1	= x1;
		rectmp.rect.y1	= y1;
		rectmp.rect.x2	= cur.bck.rect.x2;
		rectmp.rect.y2	= cur.bck.rect.y2;
		DrawRcd (hwnd, hdc, &rectmp);
		if ( gdbuff.buffer[gdbuff.nextpos-1].bck.fill.fill_type != UNFILLED)
//		if ( (reshp_pt+ num_reshpt-1 )->x == reshp_pt->x && 
//			 (reshp_pt+ num_reshpt-1 )->y == reshp_pt->y )
		{
	        x1= reshp_pt -> x;
        	y1= reshp_pt -> y;
			rectmp.rect.x1 = x1;
			rectmp.rect.y1 = y1;
			DrawRcd (hwnd, hdc, &rectmp);
		}
    }
    else
    {
    	x1	= (reshp_pt+ (reshp_hdl-1) ) -> x;
        x2	= (reshp_pt+ (reshp_hdl+1) ) -> x;
        y1	= (reshp_pt+ (reshp_hdl-1) ) -> y;
        y2	= (reshp_pt+ (reshp_hdl+1) ) -> y;
		rectmp.rect.x1	= x1;
		rectmp.rect.y1	= y1;
		rectmp.rect.x2	= cur.bck.rect.x2;
		rectmp.rect.y2	= cur.bck.rect.y2;
		DrawRcd (hwnd, hdc, &rectmp);
		rectmp.rect.x1	= x2;
		rectmp.rect.y1	= y2;
		DrawRcd (hwnd, hdc, &rectmp);
    }
	ReleaseDC(hwnd, hdc);

}
 

void ResizeItemId ( x1, y1, x2, y2, draw_type)
int	*x1, *y1;
int	*x2, *y2;
int	*draw_type;
{
	*x1 = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.x1;
    *y1 = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.y1;
    *x2 = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.x2;
    *y2 = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.y2;
	*draw_type	= gdbuff.buffer[gdbuff.nextpos-1].bck.draw_type;
}


void ResizeModeId ( x, y, x1, y1, x2, y2, draw_type, k_draw_mode, resize_mode_id )
int	x, y, x1, y1, x2, y2;
int	draw_type, k_draw_mode;
int	*resize_mode_id;
{
	BOOL	in_lup_hdl, in_rup_hdl, in_lbot_hdl, in_rbot_hdl;
	BOOL	in_left_line, in_right_line, in_top_line,in_bottom_line;

/*	lup_hdl:	left_up point of a rectangle.
	rup_hdl:	right_up point of a rectangle.
	lbot_hdl:	left_below point of a rectangle.
	rbot_hdl:	right_below point of a rectangle.
*/
	switch	(draw_type)
	{
	case    DrwT_ARC:
	case    DrwT_RECTANGLE:
	case    DrwT_CIRCLE:
	case    DrwT_SYMBOL:
	case    DrwT_GIF:
		in_lup_hdl 	= CheckPointsDistance (x, x1, y, y1, HDL_OFFSET);
		in_rup_hdl 	= CheckPointsDistance (x, x2, y, y1, HDL_OFFSET);
		in_lbot_hdl 	= CheckPointsDistance (x, x1, y, y2, HDL_OFFSET);
		in_rbot_hdl 	= CheckPointsDistance (x, x2, y, y2, HDL_OFFSET);

		in_left_line 	= CheckPointInVerticalLine (x, y, x1, y1, y2, HDL_OFFSET);
		in_right_line 	= CheckPointInVerticalLine (x, y, x2, y1, y2, HDL_OFFSET);
		in_top_line 	= CheckPointInHorizontalLine (x, y, y1, x1, x2, HDL_OFFSET);
		in_bottom_line 	= CheckPointInHorizontalLine (x, y, y2, x1, x2, HDL_OFFSET);

		if (in_lup_hdl)			*resize_mode_id	= UL_HANDLE;
		else if (in_rup_hdl)		*resize_mode_id	= UR_HANDLE;
		else if (in_lbot_hdl)		*resize_mode_id	= BL_HANDLE;
		else if (in_rbot_hdl)		*resize_mode_id	= BR_HANDLE;
		else if (in_left_line)		*resize_mode_id	= L_LINE_HANDLE;
		else if (in_top_line)		*resize_mode_id	= T_LINE_HANDLE;
		else if (in_right_line)		*resize_mode_id	= R_LINE_HANDLE;
		else if (in_bottom_line)	*resize_mode_id	= B_LINE_HANDLE;
		else 				{*resize_mode_id = WRONG_HANDLE;}
		break;

    case    DrwT_LINE:
		in_lup_hdl = CheckPointsDistance (x, x1, y, y1, HDL_OFFSET);
		in_rup_hdl = CheckPointsDistance (x, x2, y, y2, HDL_OFFSET);

		if (in_lup_hdl)		*resize_mode_id = UL_HANDLE;
		else if (in_rup_hdl)	*resize_mode_id = UR_HANDLE;
		else 			{ *resize_mode_id = WRONG_HANDLE;}
		break;

	default:	break;
	}      


}

void CheckReshapeHandle ( x, y, rsp_hdl)
int	x, y;
int	*rsp_hdl;
{
	int	i, num_reshpt;
	XPoint	*reshp_pt, *reshp_pt_end;
	BOOL	in_hdl;

	reshp_pt	= gdbuff.buffer[gdbuff.nextpos-1].bck.data.pl;
	num_reshpt	= gdbuff.buffer[gdbuff.nextpos-1].bck.plcnt;
	reshp_pt_end	= reshp_pt+num_reshpt-1;
	
	i = 0;
	for( ;reshp_pt <= reshp_pt_end; reshp_pt++)
    {
		in_hdl = CheckPointsDistance (x, reshp_pt->x, y, reshp_pt->y, 
					2*HDL_OFFSET);	
		if (in_hdl) { *rsp_hdl=	i; break;}
		i++;
	}

}


void ResizeItemsFinished (k_resize_mode, gdb)
NEW_GDBUFF	*gdb;
int	k_resize_mode;
{
	int	dx, dy;
	int	i;
	int	x1,x2;
	int	y1,y2;
	int	draw_type;
	XArc	arc_data;

	dx	=cur.bck.rect.x2	-cur.bck.rect.x1;
	dy	=cur.bck.rect.y2	-cur.bck.rect.y1;
	gdbuff.nextpos	=0;
	for (i=0; i<gdb->nextpos; i++)
	{
		if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
		{
//			ClearTagItems (&gdb->buffer[i].bck );	
			x1= gdb->buffer[i].bck.rect.x1;
			x2= gdb->buffer[i].bck.rect.x2;
			y1= gdb->buffer[i].bck.rect.y1;
			y2= gdb->buffer[i].bck.rect.y2;
			draw_type= gdb->buffer[i].bck.draw_type;

			if (draw_type == DrwT_LINE)
			{
				switch( k_resize_mode)
        		{
	            case    UL_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x2, y2, (x1+dx),(y1+dy));
					gdbuff.nextpos	++;
					break;

		        case    UR_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x1, y1, (x2+dx),(y2+dy));
					gdbuff.nextpos	++;
					break;

		  	    case    WRONG_HANDLE:
					gdbuff.buffer[gdbuff.nextpos] = gdb->buffer[i];
					gdbuff.nextpos	++;
        	        break;
		        default:
                	return;
				}
			
			}
			else
			{	
				switch( k_resize_mode)
		        {
	        	case    UL_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x2, y2, (x1+dx),(y1+dy));
					gdbuff.nextpos	++;
					break;
	
		        case    UR_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x1, y2, (x2+dx),(y1+dy));
					gdbuff.nextpos	++;
					break;
	
				case    BL_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x2, y1, (x1+dx),(y2+dy));
					gdbuff.nextpos	++;
					break;

	        	case    BR_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x1, y1, (x2+dx),(y2+dy));
					gdbuff.nextpos	++;
					break;

		        case    L_LINE_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x2, y1, (x1+dx),y2);
					gdbuff.nextpos	++;
					break;
	
	        	case    T_LINE_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x2, y2, x1,(y1+dy));
					gdbuff.nextpos	++;
					break;

	        	case    R_LINE_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x1, y2, (x2+dx),y1);
					gdbuff.nextpos	++;
					break;

		        case    B_LINE_HANDLE:
					ChangeRcdCoords (&gdb->buffer[i].bck, x1, y1, x2,(y2+dy));
					gdbuff.nextpos	++;
					break;

	        	case    WRONG_HANDLE:
					gdbuff.buffer[gdbuff.nextpos]	= gdb->buffer[i];
					gdbuff.nextpos	++;
		                        break;

	        	default:
	                return;
	
				}
			}
			
			if (draw_type == DrwT_ARC)
			{
				CheckArcDirection (gdb->buffer[i].bck.rect, &arc_data);
        		gdb->buffer[i].bck.data.arc	= arc_data;
			}

		}
	}
	cur.bck.draw_type = DrwT_SELECTION ; //added chi

}


void ReshapeItemsFinished (resp_hdl,  gdb)
NEW_GDBUFF	*gdb;
int	resp_hdl;
{
	int	i;
	XPoint	*reshp_pt; 

	for (i=0; i< gdb->nextpos; i++)
    {
        if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
		{
			int	xtmp, ytmp, width, height;

//			ClearTagItems (&gdb->buffer[i].bck );	
			reshp_pt= gdb->buffer[i].bck.data.pl;

			gdb->buffer[i].bck.data.pl[resp_hdl].x = cur.bck.rect.x2;
			gdb->buffer[i].bck.data.pl[resp_hdl].y = cur.bck.rect.y2;
			gdbuff.buffer[gdbuff.nextpos-1].bck.data.pl[resp_hdl].x = cur.bck.rect.x2;
			gdbuff.buffer[gdbuff.nextpos-1].bck.data.pl[resp_hdl].y = cur.bck.rect.y2;
			CheckRectCoordsOfPline (&gdb->buffer[i].bck, &xtmp, &ytmp, &width, &height);
			break;
		}
  
	}

}
//add new chi 
void RedrawAllItems(NEW_GDBUFF	*gdb)
{
	NEW_GBRCD	*rcd;RECT rc ;
	int i ;
    for (i = 0;i <gdb->nextpos-1 ;  i++)
	{
     	rcd	= &gdb->buffer[i];
			if (rcd->bck.layer == cur.bck.layer)
			{
			    rc.left     =(int)((rcd->bck.rect.x1 -HDL_OFFSET)*xscale);
                rc.top      =(int)((rcd->bck.rect.y1 -HDL_OFFSET)*xscale);
			    rc.right    =(int)((rcd->bck.rect.x2 +HDL_OFFSET)*xscale);
                rc.bottom   =(int)((rcd->bck.rect.y2 +HDL_OFFSET)*xscale);
            	InvalidateRect(ghWnd, &rc , TRUE);

			}
	} //for 

		
} 

