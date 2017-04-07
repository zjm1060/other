
#include <windows.h>

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/toolkit.h"
#include	"../inc/text_edit_def.h"
#include	"../inc/fill_bitmap.h"
#include	"../inc/draw_def.h"
#include	"../inc/mode_def.h"


void	SetColor ( int, int);
void	SetFont (int);
void    SetLineWidth (int);
void	SetFillPattern(int);
void 	ClearPreviousSelect (HWND, NEW_GDBUFF *);
void	SetDrawTool (int);
void	GetSymbolSelectBox (int);
int		GetColorId(COLORREF rgb);


BOOL CALLBACK PaletteDlgProc(HWND, UINT, WPARAM, LONG);


CHOOSECOLOR choose_color;
COLORREF	color_define[16];



int	gIdCurTool;

BOOL CALLBACK ToolKitProc(hDlg, msg, wParam, lParam)
HWND	hDlg;
UINT	msg;
WPARAM	wParam;
LPARAM	lParam;
{
	int	i, clr;
	int	fill_ptn_choice , fillbk;   
	int	line_width_choice;
	int	ftset_id_choice;
	HDC	hdcMem;
	LPDRAWITEMSTRUCT	lpdis;
	HWND	hlnwdth, hfillptn, hfont ,hfilltype;
	HBRUSH	hBrush, hbr;
	RECT	rc;
	int		IdOldTool;
		
	switch (msg)
	{
		case WM_INITDIALOG:
			for( i=0; i< TOOLKIT_BITMAP_NUM; i++ )
			{
                ghWndToolBtn[i]  = (HBITMAP)GetDlgItem(hDlg, IDB_LINE+i);
				ghToolKitbmUp[i] = LoadBitmap((HANDLE)ghInst, MAKEINTRESOURCE(BMP_LINEU +i));
				ghToolKitbmDn[i] = LoadBitmap((HANDLE)ghInst, MAKEINTRESOURCE(BMP_LINED +i));
			}


			for( i=0; i<LINE_WIDTH_BM_NUM; i++ )
				ghLnWdthbm[i] = LoadBitmap((HANDLE)ghInst, MAKEINTRESOURCE(BMP_LINEWIDTH1+i));

			hfillptn = GetDlgItem(hDlg, IDC_FILLPTN);
			for( i=0; i<MAX_FILL_PATTERN; i++ )
				SendMessage(hfillptn, CB_ADDSTRING, 0, (LPARAM)fill_pixmenu[i]);
			SendMessage(hfillptn, CB_SETCURSEL, cur.bck.fill.fill_type, 0);


			hfilltype =  GetDlgItem(hDlg, IDC_FILLPTN);

			hlnwdth = GetDlgItem(hDlg, IDC_LINEWIDTH);
			for( i=0; i<LINE_WIDTH_BM_NUM; i++ )
				SendMessage(hlnwdth, CB_ADDSTRING, 0, (LPARAM)ghLnWdthbm[i]);
			SendMessage(hlnwdth, CB_SETCURSEL, cur.bck.line_width, 0);

			hfont = GetDlgItem(hDlg, IDC_FONT);
			SendMessage(hfont,CB_RESETCONTENT,0,0 ); 
			for( i=0; i<MAX_FONTSET; i++ )
				SendMessage(hfont, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)font_string[i]);
			SendMessage(hfont, CB_SETCURSEL, cur.bck.ftset_id, 0);
            
			gIdCurTool = IDB_LINE;     //modify from IDB_LINE ; chi
			return TRUE;

		case WM_DRAWITEM:
			lpdis = (LPDRAWITEMSTRUCT)lParam;
			hdcMem = CreateCompatibleDC(lpdis->hDC);
			if ( lpdis->CtlID == IDB_FGBTN || lpdis->CtlID == IDB_BGBTN	)
			{
				if ( lpdis->CtlID == IDB_FGBTN ) clr = cur.bck.fg;
				else  clr = cur.bck.bg;
        		CopyRect ((LPRECT)&rc, (LPRECT)&lpdis->rcItem);
    			if(ghPal)
    			{
    				SelectPalette (lpdis->hDC, ghPal, FALSE);
    				RealizePalette (lpdis->hDC);
				}
				hBrush = CreateSolidBrush ( Colors (clr) );
				hbr = GetStockObject(BLACK_BRUSH);
				FillRect(lpdis->hDC, &rc, hBrush);
        		FrameRect(lpdis->hDC, (LPRECT)&rc, hbr);
				DeleteObject(hBrush);
				DeleteObject(hbr);
				DeleteDC(hdcMem);
				SetWindowPos(GetDlgItem(hDlg,IDB_FGBTN),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE); 
				return TRUE;
			}
			else if ( lpdis->CtlID >= IDB_LINE  && lpdis->CtlID <= IDB_ZOOM)
			{

				if ( lpdis->CtlID == (unsigned)gIdCurTool )
					SelectObject(hdcMem, ghToolKitbmDn[lpdis->CtlID - IDB_LINE]);
				else
					SelectObject(hdcMem, ghToolKitbmUp[lpdis->CtlID - IDB_LINE]);
			}
			else if ( lpdis->CtlID == IDC_LINEWIDTH )
			{
				SelectObject(hdcMem, ghLnWdthbm[lpdis->itemID]);
			}
			else if ( lpdis->CtlID == IDC_FILLPTN )
			{
        		CopyRect ((LPRECT)&rc, (LPRECT)&lpdis->rcItem);
        		InflateRect ((LPRECT)&rc, -2, -2);
				hBrush = CreatePatternBrush (fill_pixmenu[lpdis->itemID]);
				hbr = GetStockObject(BLACK_BRUSH);
				FillRect(lpdis->hDC, &rc, hBrush);
        		FrameRect(lpdis->hDC, (LPRECT)&rc, hbr);
				DeleteObject(hBrush);
				DeleteObject(hbr);
				DeleteDC(hdcMem);
				return TRUE;
			}
			else if (   lpdis->CtlID <= IDB_LINE_DASH )
			{

				if ( IsLineDash == TRUE  )
					SelectObject(hdcMem, ghToolKitbmDn[lpdis->CtlID - IDB_LINE]);
				else
					SelectObject(hdcMem, ghToolKitbmUp[lpdis->CtlID - IDB_LINE]);
				 
			}
			else
			{
				DeleteDC(hdcMem);
				return FALSE;
			}	
/*			StretchBlt(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top,
						lpdis->rcItem.right - lpdis->rcItem.left,
						lpdis->rcItem.bottom - lpdis->rcItem.top,
						hdcMem, 0, 0, 32, 32, SRCCOPY);
*/
			BitBlt(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top,
						lpdis->rcItem.right - lpdis->rcItem.left,
						lpdis->rcItem.bottom - lpdis->rcItem.top,
						hdcMem, 0, 0, SRCCOPY);				
			DeleteDC(hdcMem);
			return TRUE;
				
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDB_FGBTN:
	/*modified by wzg 20060417
					DialogBox( ghInst, MAKEINTRESOURCE(DLG_PALETTE),
				 				hDlg, (DLGPROC)PaletteDlgProc );*/
					choose_color.lStructSize= sizeof(choose_color);
					choose_color.hwndOwner= hDlg;
					choose_color.hInstance= NULL;
					choose_color.rgbResult= cur.bck.fg;
					choose_color.lpCustColors= color_define;
					choose_color.Flags= CC_RGBINIT|CC_ANYCOLOR;
					choose_color.lCustData= 0;
					choose_color.lpfnHook= NULL;
					choose_color.lpTemplateName= 0;
					if(ChooseColor(&choose_color))
						SetColor ( k_fg, GetColorId(choose_color.rgbResult));
					break;

				case IDB_BGBTN:
		/*modified by wzg 20060417
				DialogBox( ghInst, MAKEINTRESOURCE(DLG_PALETTE),
				 				hDlg, (DLGPROC)PaletteDlgProc );*/
					choose_color.lStructSize= sizeof(choose_color);
					choose_color.hwndOwner= hDlg;
					choose_color.hInstance= NULL;
					choose_color.rgbResult= cur.bck.bg;
					choose_color.lpCustColors= color_define;
					choose_color.Flags= CC_RGBINIT|CC_ANYCOLOR;
					choose_color.lCustData= 0;
					choose_color.lpfnHook= NULL;
					choose_color.lpTemplateName= 0;
					if(ChooseColor(&choose_color))
						SetColor ( k_bg, GetColorId(choose_color.rgbResult));
					break;

				case IDC_FILLPTN:
					switch( HIWORD(wParam) )
					{
						case CBN_SELCHANGE:
							fill_ptn_choice = SendMessage(GetDlgItem(hDlg, IDC_FILLPTN),
												CB_GETCURSEL,0,0L);

								SetFillPattern(fill_ptn_choice);	
							break;
						default:	
							break;
					}
					break;
               case IDC_FILLTYPE:
					switch( HIWORD(wParam) )
					{
						case BN_CLICKED:
							fillbk = SendMessage(GetDlgItem(hDlg, IDC_FILLTYPE),
												BM_GETCHECK,0,0L);

								SetFillBk(fillbk);	
							break;
						default:	
							break;
					}
					break;

				case IDC_LINEWIDTH:
					switch( HIWORD(wParam) )
					{
						case CBN_SELCHANGE:
							line_width_choice = SendMessage(GetDlgItem(hDlg, IDC_LINEWIDTH),
												CB_GETCURSEL,0,0L);
								SetLineWidth(line_width_choice+1);	
							break;
						default:	
							break;
					}
					break;

				case IDC_FONT:
					switch( HIWORD(wParam) )
					{
						case CBN_SELCHANGE:
							ftset_id_choice = SendMessage(GetDlgItem(hDlg, IDC_FONT),
												CB_GETCURSEL,0,0L);
								SetFont (ftset_id_choice);	
							break;
						default:	
							break;
					}
					break;
				case IDB_LINE_DASH:

					if (IsLineDash == TRUE )
						IsLineDash  = FALSE ;
					else 
						IsLineDash  = TRUE;

                    InvalidateRect((HWND)ghWndToolBtn[10], NULL, FALSE);  //gIdCurTool - IDB_LINE

					break ;
				case IDB_LINE:
				case IDB_ARC:
				case IDB_CIRCLE:
				case IDB_POLYLINE:
				case IDB_RECT:
				case IDB_SELECT:
				case IDB_GIF:
				case IDB_TEXT:
				case IDB_ZOOM:
 
            		IdOldTool = gIdCurTool;
					gIdCurTool = LOWORD(wParam);
                    InvalidateRect((HWND)ghWndToolBtn[IdOldTool - IDB_LINE], NULL, FALSE);
                    InvalidateRect((HWND)ghWndToolBtn[gIdCurTool - IDB_LINE], NULL, FALSE);
					SetDrawTool(LOWORD(wParam));
					break;

				case IDB_SYMBOL:
            		IdOldTool = gIdCurTool;
					gIdCurTool = LOWORD(wParam);
                    InvalidateRect((HWND)ghWndToolBtn[IdOldTool - IDB_LINE], NULL, FALSE);
                    InvalidateRect((HWND)ghWndToolBtn[gIdCurTool - IDB_LINE], NULL, FALSE);
					GetSymbolSelectBox(SYMBOL_DRAW);
					break;

			}
			break;

		case WM_DESTROY:
			for( i=0; i<TOOLKIT_BITMAP_NUM; i++ )
			{
				DeleteObject(ghToolKitbmUp[i]);
				DeleteObject(ghToolKitbmDn[i]);
			}
			for( i=0; i<LINE_WIDTH_BM_NUM; i++ )
				DeleteObject(ghLnWdthbm[i]);

			break;
	}
	return FALSE;
}
	
#define	PALETTE_WID_NUM	16
#define	PALETTE_HGT_NUM	10

extern BOOL ColorChoiceFlag;
BOOL CALLBACK PaletteDlgProc(HWND hWnd, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	xpos, ypos, row, col, i, margin;
    HDC         hDC;
    PAINTSTRUCT ps;
	RECT    clientrc, cbrc;
 	HBRUSH	hBrush;

    switch(wMsg)
    {
        case WM_PAINT:
			margin = 2;
		    if(hDC = BeginPaint(hWnd,&ps))
    		{
				GetClientRect (hWnd, &clientrc);
				cb_width = clientrc.right/PALETTE_WID_NUM;
				cb_height = clientrc.bottom/PALETTE_HGT_NUM;
   			 	if(ghPal)
    			{
    				SelectPalette (hDC, ghPal, FALSE);
    				RealizePalette (hDC);
				}
				i = 0;
				for(row = 0; row < PALETTE_HGT_NUM; row++)
				{
					cbrc.left = clientrc.left + margin;
					cbrc.right = clientrc.left + cb_width - margin;
					cbrc.top = clientrc.top + margin + row*cb_height;
					cbrc.bottom = cbrc.top + cb_height - 2*margin;
	  				for(col= 0; col < PALETTE_WID_NUM; col++)
	  				{
						hBrush = CreateSolidBrush(Colors (i));
						FillRect(hDC, &cbrc, hBrush);
						OffsetRect(&cbrc, cb_width, 0);
						DeleteObject(hBrush);
						i++;
	  				}
				}
    			EndPaint(hWnd,&ps);
			}
            break;
		
		case WM_LBUTTONUP:
			xpos = LOWORD(lParam);	
			ypos = HIWORD(lParam);
			row = ypos/cb_height;
			col = xpos/cb_width;
			color_num_choice = row * PALETTE_WID_NUM + col;
            ColorChoiceFlag = TRUE;
            EndDialog(hWnd, wParam);
            return TRUE;

		case WM_CLOSE:
			ColorChoiceFlag = FALSE;
			EndDialog(hWnd, wParam);
			return FALSE;
    }
    return FALSE;
}




void PopupFileSelectionBox(char* filename)
{
	if ( OpenNewFile( ghWnd,filename, GIF_FILE ) == TRUE )
    {
        // enable the Save As and Print menu items
        //EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,  MF_BYCOMMAND | MF_ENABLED );
        //EnableMenuItem( GetMenu( ghWnd ), ID_Print, MF_BYCOMMAND | MF_ENABLED );
        //DrawMenuBar( ghWnd);
        // reset the title in the title bar to reflect the new open file
		//FormWindowTitle(WndTitle);
        //SetWindowText( ghWnd, WndTitle );  //chi
		//InvalidateRect( ghWnd, NULL, TRUE );
    }
           
}

void	SetDrawTool (drawtypeid)
{
	char  filename[MAX_FILE_NAME_LEN] ="";
	ClearPreviousSelect (ghWnd, &v4_gdbase  );
	switch(drawtypeid)
	{
	    case IDB_LINE_DASH:
           cur.bck.draw_type = DrwT_LINE_DASH;
			break;    
		case IDB_LINE:
			cur.bck.draw_type = DrwT_LINE;
			break;
		case IDB_ARC:
			cur.bck.draw_type = DrwT_ARC;
			break;
		case IDB_CIRCLE:
			cur.bck.draw_type = DrwT_CIRCLE;
			break;
		case IDB_POLYLINE:
			cur.bck.draw_type = DrwT_PLINE;
			cur.bck.plcnt	= 0;
			break;
		case IDB_RECT:
			cur.bck.draw_type = DrwT_RECTANGLE;
			break;
		case IDB_SELECT:
			cur.bck.draw_type = DrwT_SELECTION;
			break;
		case IDB_GIF:
			cur.bck.draw_type = DrwT_GIF;
			cur.bck.rect.x1 = 100 ;
			cur.bck.rect.y1 = 100 ;
			drawing_mode	=DRAWING_ON;
			PopupFileSelectionBox (filename);
			GetFilePartName (filename ,cur.bck.data.fname ); 
			break;
		case IDB_TEXT:
			cur.bck.draw_type = DrwT_TEXT;
			break;
        case IDB_ZOOM:
			cur.bck.draw_type = DrwT_ZOOM;
			break;
		default:
			cur.bck.draw_type = DrwT_LINE;
			break;			
	}
}
