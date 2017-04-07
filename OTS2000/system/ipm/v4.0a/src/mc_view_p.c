
#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/view_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/color_def.h"
#include	"../inc/scaler.h"

int	viewbox_width, viewbox_height;

BOOL	View( ) 
{
	RECT		rc;

	if ( IsWindow(view) )
	{
		SetWindowPos(view,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		return(TRUE); 
	}

	GetClientRect(ghWnd, &rc);
	viewbox_width = rc.right/6;
	viewbox_height = rc.bottom/6;
	view = CreateWindow(
				VIEWCLASS,            
  				"view_box",      // Text for window title bar.
   				WS_CHILD | WS_CAPTION | WS_SYSMENU | 
   				WS_CLIPSIBLINGS | WS_VISIBLE | WS_THICKFRAME,// Window style.
   				rc.right/4, rc.bottom/4, viewbox_width, viewbox_height,	 // position and width height
   				ghWnd,               // parent window.
   				NULL,                // Use the window class menu.
				ghInst,           // instance handle
 				NULL                 // no window creation data
			);

	// If window could not be created, return "failure"
	if (!view)
	{
		MessageBox (ghWnd, "view window cannot create", "IPM", MB_OK);
		return (FALSE);
	}
	return(TRUE); 
	
}


LONG CALLBACK ViewProc(HWND hWnd, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int		x, y;
	RECT	rc; 
    PAINTSTRUCT ps;
	HDC		hdc;
	LPMINMAXINFO	lpmmi;

    switch(wMsg)
    {
		case WM_INITMENU:
			DeleteMenu( GetSystemMenu( hWnd, FALSE ), SC_SIZE, MF_BYCOMMAND);
			DeleteMenu( GetSystemMenu( hWnd, FALSE ), SC_MINIMIZE, MF_BYCOMMAND);
			DeleteMenu( GetSystemMenu( hWnd, FALSE ), SC_MAXIMIZE, MF_BYCOMMAND);
			DeleteMenu( GetSystemMenu( hWnd, FALSE ), SC_RESTORE, MF_BYCOMMAND);
			DeleteMenu( GetSystemMenu( hWnd, FALSE ), SC_TASKLIST, MF_BYCOMMAND);
			break;

        case WM_CREATE:
            GetClientRect(hWnd, &rc);
			viewdb.width = rc.right;
			viewdb.height = rc.bottom;
			viewdb.xfactor	=(float)viewdb.width/zoomdb.canvas_w;
			viewdb.yfactor	=(float)viewdb.height/zoomdb.canvas_h;
            break;

		case WM_GETMINMAXINFO:
			lpmmi = (LPMINMAXINFO)lParam;	 // no resize
			lpmmi->ptMinTrackSize.x = viewbox_width; 
			lpmmi->ptMinTrackSize.y = viewbox_height;
			lpmmi->ptMaxTrackSize.x = viewbox_width;
			lpmmi->ptMaxTrackSize.y = viewbox_height;
			return(0);
				
        case WM_COMMAND:
            break;

        case WM_PAINT:
    		hdc = BeginPaint(hWnd,&ps);
			if( ghPal)
			{
    			SelectPalette (hdc, ghPal, FALSE);
    			RealizePalette (hdc);
			}
			DrawViewRect(hWnd, hdc);
    		EndPaint(hWnd,&ps);
            break;

		case WM_LBUTTONDOWN:
			x	= LOWORD(lParam);
			y	= HIWORD(lParam);

			zoomdb.sx_orig =(int)( -x/viewdb.xfactor+canvas_width/2 );
			zoomdb.sy_orig =(int)( -y/viewdb.yfactor+canvas_height/2 );

			CheckZoomOrigin ();
		///////////////////////////////////chi///////////////////////////////////////

               // Set vertical scroll bar range and page size
			si.cbSize = sizeof (si) ;
            si.fMask  = SIF_POS ;
            GetScrollInfo (hWnd, SB_VERT, &si) ;
            si.nPos   = -zoomdb.sy_orig ;
			si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
            SetScrollInfo (ghWnd, SB_VERT, &si, TRUE) ;
               // Set horizontal scroll bar range and page size
            si.cbSize = sizeof (si) ;
            si.fMask  = SIF_POS ;
            GetScrollInfo (hWnd, SB_HORZ, &si) ;
            si.nPos   = -zoomdb.sx_orig ;
			si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
            SetScrollInfo (ghWnd, SB_HORZ, &si, TRUE) ;
           ///////////////////////////////END SCROLL SET INFO ///////////////////////////////////////////////
			InvalidateRect(hWnd, NULL, TRUE);
			InvalidateRect(ghWnd, NULL, TRUE);
			break;

		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;

        case WM_DESTROY:
            break;

        default:
            return(DefWindowProc(hWnd,wMsg,wParam,lParam));
    }
    return(0l);
}


void DrawViewRect (hwnd, hdc)
HWND	hwnd;
HDC		hdc;
{
	BACKRCD	rcd;

	memset( &rcd, 0, sizeof(BACKRCD) );
	if (hwnd != NULL)
	{
		rcd.fg	= WHITE;
		rcd.draw_type	=DrwT_RECTANGLE;
		viewdb.xfactor	=(float)viewdb.width/zoomdb.canvas_w;
		viewdb.yfactor	=(float)viewdb.height/zoomdb.canvas_h;
		rcd.rect.x1	= (int)( -zoomdb.sx_orig*viewdb.xfactor / xscale);
		rcd.rect.y1	= (int)( -zoomdb.sy_orig*viewdb.yfactor / yscale);
		rcd.rect.x2	= (int)( (canvas_width-zoomdb.sx_orig)*viewdb.xfactor / xscale);
		rcd.rect.y2	= (int)( (canvas_height-zoomdb.sy_orig)*viewdb.yfactor / yscale);
		rcd.fill.fill_type	= UNFILLED;
		rcd.line_width = 1;

		DrawRcd ( hwnd, hdc, &rcd);
	}
}



