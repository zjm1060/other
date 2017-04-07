
#include	<stdio.h>

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/color_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/grid_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/sub_dsp_def.h"

void DrawGrid (HDC);
int	SXtoX (int);
int	SYtoY (int);

void GridInit ()
{
    grid_visible    =GRID_VISIBLE;
    grid_active     =GRID_ACTIVE;
    grid_width      =8;
    grid_height     =8;

	grid_fb		=GRID_BACKGROUND;

}


void GridFgBgSet (fb)
int	fb;
{
	if (fb == ID_Fgd_Grid)
	{ grid_fb = GRID_FOREGROUND; }
	else
	{ grid_fb = GRID_BACKGROUND; }

}

//Modify chi 

void ToggleProc(int menuid)  
{

	HMENU	hmenu;
    HMENU   hPopMenu;
	hmenu = GetMenu(ghWnd);
    hPopMenu = GetSubMenu(LoadMenu (ghInst, MAKEINTRESOURCE(POP_UP)),0);
	switch (menuid)
	{
	case ID_Grid_Visible:
		if ( grid_visible == GRID_UNVISIBLE )
		{
			grid_visible	= GRID_VISIBLE;
			CheckMenuItem(hmenu, ID_Grid_Visible, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hPopMenu, ID_Grid_Visible, MF_BYCOMMAND | MF_CHECKED); 
			InvalidateRect( ghWnd, NULL, TRUE );
//			DrawGrid (ghDC);
		}
		else
		{
			grid_visible	= GRID_UNVISIBLE;
			CheckMenuItem(hmenu, ID_Grid_Visible, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hPopMenu, ID_Grid_Visible, MF_BYCOMMAND | MF_UNCHECKED);
			InvalidateRect( ghWnd, NULL, TRUE );
		}
		break;

	case ID_Grid_Valid:
		if (grid_active == GRID_INACTIVE)
		{
			grid_active =GRID_ACTIVE;
			CheckMenuItem(hmenu, ID_Grid_Valid, MF_BYCOMMAND | MF_CHECKED);
		}
		else
		{
			grid_active =GRID_INACTIVE;
			CheckMenuItem(hmenu, ID_Grid_Valid, MF_BYCOMMAND | MF_UNCHECKED);
		}
		break;

	case ID_Layer_Clutter:
		if (info.clutter == CLUTTER_OFF)
		{
			info.clutter = CLUTTER_ON; 
			CheckMenuItem(hmenu, ID_Layer_Clutter, MF_BYCOMMAND | MF_CHECKED);
			InvalidateRect( ghWnd, NULL, TRUE );
		}
		else
		{
			info.clutter = CLUTTER_OFF;
			CheckMenuItem(hmenu, ID_Layer_Clutter, MF_BYCOMMAND | MF_UNCHECKED);
			InvalidateRect( ghWnd, NULL, TRUE );
		}
		break;

	default:	;

	}

}


void DrawLine (HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx( hdc, x1, y1, NULL); 
	LineTo(hdc, x2, y2);
}

extern	int	SED_canvas_width, SED_canvas_height;

void DrawGrid (HDC hdc)
{
	int	i,j, icx, icy;
	int	xtmp, ytmp;
	int	cvs_w, cvs_h;
	HPEN	hPen, hOldPen;
	HBRUSH	hBrush,	hOldBrush;
	RECT	rc;

	//hPen = CreatePen(PS_DOT, 0, Colors (WHITE));	  //PS_DASH
	//hOldPen = SelectObject(hdc, hPen);
	SetBkColor(hdc, Colors (BLACK));   //line_style: dot or dash line need BKColor to blank
	//SetBkMode(hdc, TRANSPARENT);	 //  line_style: dot or dash line need BKColor to blank

	GetClientRect(ghWnd, &rc);
	cvs_w = rc.right;
	cvs_h = rc.bottom;

	if (grid_visible == GRID_VISIBLE)
	{    
		//icx	= cvs_w/grid_width +1;
		/*
		for (i=0; i<icx; i++)
		{
			xtmp	=i*grid_width;
			DrawLine (hdc, xtmp, 0, xtmp, cvs_h);
		}

		icy	= cvs_h/grid_height +1;
		for (i=0; i<icy; i++)
		{
			ytmp	=i*grid_height;
			DrawLine (hdc, 0, ytmp, cvs_w, ytmp);
		}
		*/

      //modify by chi
		icy	= cvs_h/grid_height +1;
		icx	= cvs_w/grid_width +1;
		for (i=0; i<icy; i++)
		{

		    for (j=0; j<icx; j ++)
			{
		    	xtmp	=j*grid_width;
			   	SetPixel (hdc, xtmp, ytmp, RGB(100,255,100));
			}
	    	ytmp	=i*grid_height;
		}
	} 

	if ( edit_mode == SYMBOL_EDIT )
	{
	//	grid_width = grid_height = 6 ;  //chichi
		hPen = CreatePen(PS_DASH, 0, Colors (BLACK));	  //PS_DASH
		hOldPen = SelectObject(hdc, hPen);
		xtmp	= icx/2 * grid_width;
		DrawLine (hdc, xtmp, 0, xtmp, cvs_h);
		DrawLine (hdc, grid_width, 0, grid_width, cvs_h);
		xtmp	= (icx-2) * grid_width;
		DrawLine (hdc, xtmp, 0, xtmp, cvs_h);
		ytmp	= icy/2 * grid_height;
		DrawLine (hdc, 0, ytmp, cvs_w, ytmp);
		DrawLine (hdc, 0, grid_height, cvs_w, grid_height);
		ytmp	= (icy -2) * grid_height;
		DrawLine (hdc, 0, ytmp, cvs_w, ytmp);
	}
	else if ( edit_mode == SUB_DSP_EDIT )
	{
//		hPen = CreatePen(PS_DASH, 0, Colors (WHITE));	  //PS_DASH
//		hOldPen = SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(Colors(WHITE));
		hOldBrush = SelectObject(hdc, hBrush);
		rc.left = SUB_DSP_x;
		rc.top =  SUB_DSP_y;
		rc.right = SUB_DSP_x+SUB_DSP_canvas_width;
		rc.bottom =  SUB_DSP_y+SUB_DSP_canvas_height;
		FrameRect ( hdc, &rc, hBrush);
		SelectObject(hdc, hOldBrush);
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);

}


int	grid_width_tmp;
int	grid_height_tmp;
int grid_min;
int grid_max;

BOOL CALLBACK GridSetProc(HWND, UINT, WPARAM, LPARAM);

void	PopupGridSetBox()
{

	DialogBox( ghInst, MAKEINTRESOURCE(DLG_GRID_SET),
				 ghWnd, (DLGPROC)GridSetProc );

}

BOOL CALLBACK GridSetProc(hwndDlg, message, wParam, lParam)
HWND	hwndDlg;
UINT	message;
WPARAM	wParam;
LPARAM	lParam;
{
	char	string[10];
	int		inc;
	HWND	hwndScrollBar;

	switch (message)
	{
		case WM_INITDIALOG:
			grid_width_tmp	= grid_width;
			grid_height_tmp	= grid_height;
			grid_min = 4;
			grid_max = 96;

			sprintf ( string, "%d\0", grid_width_tmp);
			SetDlgItemText(hwndDlg, IDC_GRID_WIDTH_EDIT, string);

			sprintf ( string, "%d\0", grid_height_tmp);
			SetDlgItemText(hwndDlg, IDC_GRID_HEIGHT_EDIT, string);

			hwndScrollBar = GetDlgItem( hwndDlg, IDC_GRID_WIDTH_SBAR );
			SetScrollRange( hwndScrollBar, SB_CTL, (int)grid_min, (int)grid_max, TRUE);

			hwndScrollBar = GetDlgItem( hwndDlg, IDC_GRID_HEIGHT_SBAR );
			SetScrollRange( hwndScrollBar, SB_CTL, (int)grid_min, (int)grid_max, TRUE);
			CenterWindow(hwndDlg, ghWnd);
			return TRUE;

		case WM_COMMAND:
			switch ( LOWORD(wParam) ) 
			{
				case IDC_GRID_WIDTH_EDIT:
					switch (HIWORD(wParam))
					{
						case EN_CHANGE:
							memset(string, 0, sizeof(string));
							GetDlgItemText(hwndDlg, IDC_GRID_WIDTH_EDIT, string, sizeof(string));
							sscanf( string, "%d", &grid_width_tmp);
							return TRUE;
						default:
							return FALSE;
					}									
	
				case IDC_GRID_HEIGHT_EDIT:
					switch (HIWORD(wParam))
					{
						case EN_CHANGE:
							memset(string, 0, sizeof(string));
							GetDlgItemText(hwndDlg, IDC_GRID_HEIGHT_EDIT, string, sizeof(string));
							sscanf( string, "%d", &grid_height_tmp);
							return TRUE;
						default:
							return FALSE;
					}									
	
				case IDOK:
					if( grid_width_tmp > grid_max)
					{
						grid_width_tmp = grid_max;
						sprintf ( string, "%d\0", grid_width_tmp);
						SetDlgItemText(hwndDlg, IDC_GRID_WIDTH_EDIT, string);
						MessageBox ( hwndDlg, "网格值越上限！", "IPM", MB_OK);
						return TRUE;
					}
					if( grid_width_tmp < grid_min )
					{
						grid_width_tmp = grid_min;
						sprintf ( string, "%d\0", grid_width_tmp);
						SetDlgItemText(hwndDlg, IDC_GRID_WIDTH_EDIT, string);
						MessageBox( hwndDlg, "网格值越下限！", "IPM", MB_OK);
						return TRUE;
					}
					if( grid_height_tmp > grid_max)
					{
						grid_height_tmp = grid_max;
						sprintf ( string, "%d\0", grid_height_tmp);
						SetDlgItemText(hwndDlg, IDC_GRID_HEIGHT_EDIT, string);
						MessageBox ( hwndDlg, "网格值越上限！", "IPM", MB_OK);
						return TRUE;
					}
					if( grid_height_tmp < grid_min )
					{
						grid_height_tmp = grid_min;
						sprintf ( string, "%d\0", grid_height_tmp);
						SetDlgItemText(hwndDlg, IDC_GRID_HEIGHT_EDIT, string);
						MessageBox( hwndDlg, "网格值越下限！", "IPM", MB_OK);
						return TRUE;
					}
					grid_width	=grid_width_tmp;
					grid_height	=grid_height_tmp;
					InvalidateRect( ghWnd, NULL, TRUE );	
				case IDCANCEL:
					EndDialog(hwndDlg, wParam);
					return TRUE;
			}
			break;

		case WM_VSCROLL:
			switch( LOWORD(wParam) )
			{
				case SB_LINEUP:
					inc = -1;
					break;
				case SB_LINEDOWN:
					inc = 1;
					break;
				default:
					inc = 0;
			}
			if((HWND)lParam == GetDlgItem(hwndDlg, IDC_GRID_WIDTH_SBAR) )
			{
				if (inc == max( grid_min - grid_width_tmp, 
							min(inc, grid_max - grid_width_tmp)))  
				{
					grid_width_tmp += inc;
					sprintf ( string, "%d\0", grid_width_tmp);
					SetDlgItemText(hwndDlg, IDC_GRID_WIDTH_EDIT, string);
				}
			}
			if((HWND)lParam == GetDlgItem(hwndDlg, IDC_GRID_HEIGHT_SBAR) )
			{
				if (inc == max( grid_min - grid_height_tmp, 
							min(inc, grid_max - grid_height_tmp)))  
				{
					grid_height_tmp += inc;
					sprintf ( string, "%d\0", grid_height_tmp);
					SetDlgItemText(hwndDlg, IDC_GRID_HEIGHT_EDIT, string);
				}
			}
			return TRUE;

		default:
			return FALSE;
	}
	return FALSE;
}


void GridCheck (xsrc, ysrc, xobj, yobj)
int	*xsrc, *xobj;
int	*ysrc, *yobj;
{
	int	tmp;
 
	if (grid_active == GRID_ACTIVE)
	{
		tmp	=(int)( *xsrc + grid_width * .5 )/grid_width;
		tmp 	= tmp * grid_width;
	}
	else
	{	tmp= *xsrc;}
	*xobj	=SXtoX (tmp);

	if (grid_active == GRID_ACTIVE)
	{
		tmp	=(int)( *ysrc + grid_height * .5 )/grid_height;
		tmp 	= tmp * grid_height;
	}
	else
	{	tmp= *ysrc;}
	*yobj	=SYtoY (tmp);

}



