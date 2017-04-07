
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#define	SYMBOL_DEF
#define    GRID     14

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/color_def.h"
#include	"../inc/draw_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/toolkit.h"
#include	"../inc/scaler.h"
#include	"../inc/mode_def.h"
#include	"../inc/grid_def.h"

int	XtoSX ( int);
int	YtoSY ( int);
void ResetZoom ();
BOOL PtInSubWin(HWND, HWND, POINT *, POINT *);
BOOL CALLBACK SymSelectDlgProc(HWND, UINT , WPARAM , LONG );
BOOL CALLBACK SymImportDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam);
void SymbolDlgInit (HWND);
void DrawSymbolCanvas (HWND, HDC,SYMBOL_CLASS *);
void DrawSymbolGrid (HWND hwnd, HDC hdc);
void SED_DrawSymbol ( HWND, HDC, int, int,SYMBOL_CLASS *);
void ReExposeCanvasAreaInWin(HWND, HWND);

int	grid_width_save, grid_height_save;
char sym_file[MAX_FILE_NAME_LEN]; //added chi
SYMBOL_CLASS *pSymdb ;  //added by chi .
short symdb_class;
SYMBOL sym ;


void DrawSymbol ( hwnd, hdc, rec,pSymdb,mode)
HWND	hwnd;
HDC		hdc;
BACKRCD	*rec;
SYMBOL_CLASS *pSymdb;
int mode ; //added by chi 
{
	int	i, xtmp, ytmp, width, height;
	SYMBOL	sym;
	BACKRCD	rcd;
    if (mode == 1 )  //when mode =1 present draw symbol libary plane;
	{
		rcd = *rec;	  // must set rcd all fields!
		rcd.fg	= canvas_bg_color;
		rcd.bg	= canvas_bg_color;
		rcd.draw_type	= DrwT_RECTANGLE;
		rcd.fill.fill_type	=  FILLED;
		DrawRcd ( hwnd, hdc, &rcd);    
	}

	CheckRectCoords (rec->rect, &xtmp, &ytmp, &width, &height);
	if (rec->data.sym.class >	MAX_SYMBOL_CLASS) return ; 	
    if (rec->data.sym.name >	MAX_SYMBOLS ) return ; 
	sym	= (pSymdb + rec->data.sym.class)->symbol[rec->data.sym.name]; //added by chi

	for (i=0; i<sym.nextpos; i++)
	{
		rcd	= sym.objects[i];
		switch	(rcd.draw_type)
		{
		case	DrwT_ARC:
			rcd.data.arc.x		=rcd.data.arc.x*width/SYMBOL_PIX+xtmp;
			rcd.data.arc.y		=rcd.data.arc.y*height/SYMBOL_PIX+ytmp;
			rcd.data.arc.width	=rcd.data.arc.width*width/SYMBOL_PIX;
			rcd.data.arc.height	=rcd.data.arc.height*height/SYMBOL_PIX;
			break;

		case	DrwT_PLINE:
			{
				int	k;

				for (k=0; k < rcd.plcnt; k++)
				{
					rcd.data.pl[k].x 	=rcd.data.pl[k].x*width/SYMBOL_PIX+xtmp;
					rcd.data.pl[k].y	=rcd.data.pl[k].y*height/SYMBOL_PIX+ytmp;
				}
			}
			break;

		case	DrwT_LINE:
		case	DrwT_LINE_DASH:
		case	DrwT_CIRCLE:
		case	DrwT_RECTANGLE:
			rcd.rect.x1	=rcd.rect.x1*width/SYMBOL_PIX+xtmp;
			rcd.rect.y1	=rcd.rect.y1*height/SYMBOL_PIX+ytmp;
			rcd.rect.x2	=rcd.rect.x2*width/SYMBOL_PIX+xtmp;
			rcd.rect.y2	=rcd.rect.y2*height/SYMBOL_PIX+ytmp;
			
			break;

		default: ;

		}

		DrawRcd ( hwnd, hdc, &rcd);

	}  //end for;	
  


}

//modified to open a certain symbol file; //chi

void	LoadSymbolLibrary (char *sym_lib_name,int *class_cnt,SYMBOL_CLASS *psymdb)
{
	int	len;
	int	id_cl, id_sym, id_obj;
	FILE	*fp;
	char	symbol_full_name[MAX_FILE_NAME_LEN];
	char	*symbol_part_name = {"/DSP/SYMBOL_LIBRARY.DBIN"};
    char version[6] ;
	extern int version4_flag;

	//symbol_part_name   symbo_full_name;   //added by chi


    if (sym_lib_name[0] == 0)
		FormFullName ( home, symbol_full_name, symbol_part_name);
	else 
        strcpy(symbol_full_name,sym_lib_name) ;



	fp = fopen(symbol_full_name, "rb");

	if (fp == NULL)
	{
		MessageBox(ghWnd, "IPM:open SYMBOL_LIBRARY.DBIN file error.", NULL, MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}

	/*added by wzg 20060417*/
	fread(version,1,6,fp);
	if (strcmp("v4.0a",version)==0)    //4.0a *.dbin file data formate
		version4_flag= 1;
	else
	{
		MessageBox(ghWnd,"当前符号库文件为v3.0a版，需要转换!\n如果编辑并保存符号库，则将转存成新的格式",
			                  "IPM:提示信息",MB_OK|MB_ICONINFORMATION) ;
		version4_flag= 0;
		rewind( fp );
	}

	len	=4;
	fread (class_cnt, len, 1, fp);
	*class_cnt = min(*class_cnt, MAX_SYMBOL_CLASS);
	for (id_cl=0; id_cl<*class_cnt; id_cl++)
	{
		len	=4;  
		fread (&(psymdb + id_cl)->nextpos, len, 1, fp);  
		len	=sizeof((psymdb + id_cl)->name);     
		fread (&(psymdb + id_cl)->name, len, 1, fp);
		for (id_sym=0; id_sym<(psymdb + id_cl)->nextpos; id_sym++)  
		{
			len	=4;      
			fread (&(psymdb+id_cl)->symbol[id_sym].nextpos, len, 1, fp);   
			for (id_obj=0; id_obj<(psymdb+id_cl)->symbol[id_sym].nextpos; id_obj++)    
			{
				freadBckRcd (fp, &(psymdb+id_cl)->symbol[id_sym].objects[id_obj]);    
			}
		}
	}

	fclose (fp);

}

void SaveSymbolLibrary ()
{
	int	len;
	int	id_cl, id_sym, id_obj;
	FILE	*fp;
	char	symbol_full_name[MAX_FILE_NAME_LEN];
	char	*symbol_part_name = {"/DSP/SYMBOL_LIBRARY.DBIN"};
    char version[] = "v4.0a" ;


	FormFullName ( home, symbol_full_name, symbol_part_name);
	fp = fopen(symbol_full_name, "wb");

	if (fp == NULL)
	{
		MessageBox(ghWnd, "IPM:open SYMBOL_LIBRARY.DBIN file error.", NULL, MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}

	fwrite(version,sizeof(char),6,fp) ;  //add a version tag before file;

	len	=4;
	fwrite (&symdb_class_cnt, len, 1, fp);
	for (id_cl=0; id_cl<symdb_class_cnt; id_cl++)
	{
		len	=4;
		fwrite (&symdb[id_cl].nextpos, len, 1, fp);
		len	=sizeof(symdb[id_cl].name);
		fwrite (symdb[id_cl].name, len, 1, fp);
		for (id_sym=0; id_sym<symdb[id_cl].nextpos; id_sym++)
		{
			len	=4;
			fwrite (&symdb[id_cl].symbol[id_sym].nextpos, len, 1, fp);
			len	=sizeof (symdb[id_cl].symbol[id_sym].objects[id_obj]);
			for (id_obj=0; id_obj<symdb[id_cl].symbol[id_sym].nextpos; id_obj++)
			{
				fwriteBckRcd (fp, &symdb[id_cl].symbol[id_sym].objects[id_obj]);
			}
		}
	}
	fclose (fp);

}
//modified by chi 

void	GetSymbolSelectBox (int Edit_mode)
{
 	if( Edit_mode == SYMBOL_IMPORT )
	{
		DialogBox( ghInst, MAKEINTRESOURCE(DLG_SYMBOL_IMPORT),
				 	ghWndSymEdit, (DLGPROC)SymImportDlgProc );	
	}
    else if (Edit_mode == SYMBOL_DRAW)
	{
		cur.bck.draw_type	= DrwT_SYMBOL;
		DialogBox( ghInst, MAKEINTRESOURCE(DLG_SYMBOL_SELECT),
				 	ghWndToolKit, (DLGPROC)SymSelectDlgProc );
	}

}

//added chi   used to import external symbol from a certain file;
void DrawRect(HDC hDC,int row, int col,BOOL type)
{
	  POINT ptlist[5];
	  HPEN	hOldPen, hPen;

      SetBkMode(hDC,TRANSPARENT );
      hPen = CreatePen(PS_SOLID, 2, RGB(200,100,50));
	  hOldPen = SelectObject(hDC, hPen);
	  //InvalidateRect(hSymCanvas,NULL,FALSE);
	  DrawSymbolGrid(0,hDC);
	  ptlist[0].x = col * sym_width ;       ptlist[0].y = row * sym_height ;
	  ptlist[1].x = (col+1) * sym_width ;   ptlist[1].y = row * sym_height ;
	  ptlist[2].x = (col+1) * sym_width ;   ptlist[2].y = (row + 1) * sym_height ;
	  ptlist[3].x = (col) * sym_width ;     ptlist[3].y = (row+1) * sym_height ;
	  ptlist[4].x = col * sym_width ;       ptlist[4].y = row * sym_height ;
	  Polyline (hDC, ptlist, 5);	
      SelectObject(hDC, hOldPen);
	  DeleteObject(hDC);
}

BOOL CALLBACK SymImportDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	row, col, i;
	int class,name;
    HDC		hDC, hCanvasDC;
    PAINTSTRUCT ps, symps;
	HWND	hsymclass, hSymCanvas;
	POINT	pt, ptsub,ptlist[5];
	float	xscalesave, yscalesave;   
    HPEN	hOldPen, hPen;
    char dsp[80];
	static INFO_BUFF	infoIn;

	class = name = 0 ;
	
    switch(wMsg)
    {
		case WM_INITDIALOG:

            infoIn = info ;
			hsymclass = GetDlgItem(hDlg, IDC_SYMBOL_CLASS_LIST);
			for( i=0; i<symdb_class; i++ )
				SendMessage(hsymclass, LB_ADDSTRING, 0, (LPARAM)(pSymdb+i)->name);
			SendMessage(hsymclass, LB_SETCURSEL, info.sym.class=0, 0);

			hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
    		SymbolDlgInit (hSymCanvas);
            CenterWindow(hDlg, ghWnd);
			strcpy(dsp,"符号导入编辑: ");  
			strcat(dsp,sym_file)  ;
			SetWindowText(hDlg,dsp);
			
			return TRUE;
			//sym = (pSymdb+class)->symbol[name];

        case WM_PAINT:
		    if(hDC = BeginPaint(hDlg,&ps))
    		{
				xscalesave = xscale;
				yscalesave = yscale;
				xscale = yscale = 1.0f;
				hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
				hCanvasDC = BeginPaint(hSymCanvas, &symps);
   			 	if(ghPal)
    			{
    				SelectPalette (hCanvasDC, ghPal, FALSE);
    				RealizePalette (hCanvasDC);
				}
				DrawSymbolCanvas (hSymCanvas, hCanvasDC,pSymdb);
				EndPaint(hSymCanvas, &symps);
    			EndPaint(hDlg,&ps);
				xscale = xscalesave;
				yscale = yscalesave;
			}
            //return TRUE;
			break;
		
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_SYMBOL_CLASS_LIST:
				switch( HIWORD(wParam) )
				{
				case LBN_SELCHANGE:
					hsymclass = GetDlgItem(hDlg, IDC_SYMBOL_CLASS_LIST);
					info.sym.class = SendMessage(hsymclass,
										LB_GETCURSEL,0,0);
					hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
					ReExposeCanvasAreaInWin(hDlg, hSymCanvas);
					return TRUE;
				default:	
					break;
				}
				break;	
			case IDOK:   
			    if (symdb[infoIn.sym.class].nextpos < MAX_SYMBOLS)
				{ 
			        symdb[infoIn.sym.class ].symbol[symdb[infoIn.sym.class].nextpos]      =sym;
		        	symdb[infoIn.sym.class ].nextpos++; 
                    //sym_edit_mode = EDIT_EXISTING_SYMBOL;
                    //EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                    //MF_BYCOMMAND | MF_ENABLED );
                    DrawMenuBar( ghWnd );
				}
	        	else
				{ MessageBox (ghWnd, "Symbol Buffer Over Flow", "IPM", MB_OK); }
				info = infoIn;
                EndDialog(hDlg, wParam);

				break;
			case IDCANCEL:
					info = infoIn;
                EndDialog(hDlg, wParam);
				break;

			}
			break;

		case WM_LBUTTONUP:
			pt.x = LOWORD(lParam);	
			pt.y = HIWORD(lParam);
			hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
			if ( PtInSubWin(hDlg, hSymCanvas, &pt, &ptsub) ) 
			{
            	col  = ptsub.x/sym_width;
            	row  = ptsub.y/sym_height;
            	info.sym.name   =col + row * sym_xn;
                sym = (pSymdb +info.sym.class)->symbol[info.sym.name];
                hDC =GetDC(hSymCanvas);
                
				DrawSymbolGrid(0,hDC); 
                SetBkMode(hDC,TRANSPARENT );
                hPen = CreatePen(PS_SOLID, 0, RGB(20,255,20));
                hOldPen = SelectObject(hDC, hPen); 
			  ////////////////////////draw a rect if object be selected ! chi ////////////
                ptlist[0].x = col * sym_width ;       ptlist[0].y = row * sym_height ;
				ptlist[1].x = (col+1) * sym_width ;   ptlist[1].y = row * sym_height ;
				ptlist[2].x = (col+1) * sym_width ;   ptlist[2].y = (row + 1) * sym_height ;
				ptlist[3].x = (col) * sym_width ;     ptlist[3].y = (row+1) * sym_height ;
				ptlist[4].x = col * sym_width ;       ptlist[4].y = row * sym_height ;
				Polyline (hDC, ptlist, 5);	
                SendMessage(hDlg,WM_PAINT,0,0);
			    SelectObject(hDC, hOldPen);
	            DeleteObject(hDC);

				//EndDialog(hDlg, wParam);
            	return TRUE;
			}
			break;

		case WM_CLOSE:
			info = infoIn;
			EndDialog(hDlg, wParam);
            return TRUE;
    }
    return FALSE;
}





extern BOOL CALLBACK SymSelectDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	row, col, i;
    HDC		hDC, hCanvasDC;
    PAINTSTRUCT ps, symps;
	HWND	hsymclass, hSymCanvas;
	POINT	pt, ptsub;
	float	xscalesave, yscalesave;

    switch(wMsg)
    {
		case WM_INITDIALOG:

			hsymclass = GetDlgItem(hDlg, IDC_SYMBOL_CLASS_LIST);
			for( i=0; i<symdb_class_cnt; i++ )
				SendMessage(hsymclass, LB_ADDSTRING, 0, (LPARAM)symdb[i].name);
			SendMessage(hsymclass, LB_SETCURSEL, info.sym.class, 0);

			hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
    		SymbolDlgInit (hSymCanvas);

			return TRUE;

        case WM_PAINT:
		    if(hDC = BeginPaint(hDlg,&ps))
    		{
				xscalesave = xscale;
				yscalesave = yscale;
				xscale = yscale = 1.0f;
				hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
				hCanvasDC = BeginPaint(hSymCanvas, &symps);
   			 	if(ghPal)
    			{
    				SelectPalette (hCanvasDC, ghPal, FALSE);
    				RealizePalette (hCanvasDC);
				}
				DrawSymbolCanvas (hSymCanvas, hCanvasDC,symdb);
				EndPaint(hSymCanvas, &symps);
    			EndPaint(hDlg,&ps);
				xscale = xscalesave;
				yscale = yscalesave;
			}
            return TRUE;
		
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_SYMBOL_CLASS_LIST:
				switch( HIWORD(wParam) )
				{
				case LBN_SELCHANGE:
					hsymclass = GetDlgItem(hDlg, IDC_SYMBOL_CLASS_LIST);
					info.sym.class = SendMessage(hsymclass,
										LB_GETCURSEL,0,0);
					hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
					ReExposeCanvasAreaInWin(hDlg, hSymCanvas);
					return TRUE;
				default:	
					break;
				}
				break;
			}
			break;

		case WM_LBUTTONUP:
			pt.x = LOWORD(lParam);	
			pt.y = HIWORD(lParam);
			hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
			if ( PtInSubWin(hDlg, hSymCanvas, &pt, &ptsub) ) 
			{
            	col  = ptsub.x/sym_width;
            	row  = ptsub.y/sym_height;
            	info.sym.name   =col + row*sym_xn;
        		cur.bck.data.sym	= info.sym;
				EndDialog(hDlg, wParam);
            	return TRUE;
			}
			break;

		case WM_CLOSE:
			EndDialog(hDlg, wParam);
            return TRUE;
    }
    return FALSE;
}

void	SymbolDlgInit (HWND hwnd)
{
	RECT    clientrc;

	sym_xn	=8;
	sym_yn	=8;

	GetClientRect (hwnd, &clientrc);
	sym_canvas_width = clientrc.right;
	sym_canvas_height = clientrc.bottom;

	sym_width	=sym_canvas_width/sym_xn;
	sym_height	=sym_canvas_height/sym_yn;
	sym_canvas_width	=sym_width*sym_xn;
	sym_canvas_height	=sym_height*sym_yn;

}

void	DrawSymbolCanvas (HWND hwnd, HDC hdc,SYMBOL_CLASS *pSymdb)
{
	int	i, nextpos;

	DrawSymbolGrid (hwnd, hdc);
	nextpos = (pSymdb + info.sym.class)->nextpos ; 
	for (i=0; i < nextpos; i++)
	{ SED_DrawSymbol (hwnd, hdc, info.sym.class, i,pSymdb); }

}

void DrawSymbolGrid (HWND hwnd, HDC hdc)
{
	int     i;
	int     xtmp, ytmp;
	HPEN	hPen, hOldPen;

	hPen = CreatePen(PS_DOT, 0, Colors (BLACK));	  
	hOldPen = SelectObject(hdc, hPen);

    for (i=0; i<sym_xn+1; i++)
    {
		xtmp    =i*sym_width;
		MoveToEx( hdc, xtmp, 0, NULL); 
		LineTo(hdc, xtmp, sym_canvas_height);
    }

    for (i=0; i<sym_yn+1; i++)
    {
		ytmp    =i*sym_height;
		MoveToEx( hdc, 0, ytmp, NULL); 
		LineTo(hdc, sym_canvas_width, ytmp);
    }
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);

}


void SED_DrawSymbol ( hwnd, hdc, class, sym_pos,pSymdb)
HWND hwnd;
HDC hdc;
SYMBOL_CLASS *pSymdb;
int	class, sym_pos;
{
	int	ix, iy;
	BACKRCD	rcd;

	iy	=sym_pos/sym_xn;
	ix	=sym_pos-iy*sym_xn;

	memset(&rcd, 0, sizeof(BACKRCD));  // must set initial
	rcd.rect.x1	= ix*sym_width;
	rcd.rect.y1	= iy*sym_height;
	rcd.rect.x2	= (ix+1)*sym_width;
	rcd.rect.y2	= (iy+1)*sym_height;
	rcd.data.sym.class	= class;
	rcd.data.sym.name	= sym_pos;

	DrawSymbol (hwnd, hdc, &rcd,pSymdb,1);

}

void ReExposeCanvasAreaInWin(HWND hwndParent, HWND hwndCanvas)
{
	POINT	pt;
	RECT	rc;

	GetClientRect (hwndCanvas, &rc);
	pt.x = rc.left;
	pt.y = rc.top;
	ClientToScreen(hwndCanvas, &pt);
	ScreenToClient(hwndParent, &pt);
	rc.left = pt.x;
	rc.top = pt.y;
	pt.x = rc.right;
	pt.y = rc.bottom;
	ClientToScreen(hwndCanvas, &pt);
	ScreenToClient(hwndParent, &pt);
	rc.right = pt.x;
	rc.bottom = pt.y;
	InvalidateRect(hwndParent, &rc, TRUE);
}

void SED_CanvasConfig ();
void CanvasConfig ();
void AbortSymbol ();
void EditSymbol ();
void LoadSymbolToGdbase (SYMBOL);
void DeleteSymbol ();
void AddSymbolClass (char *);
void DeleteSymbolClass ();
void SaveSymbol (char *);
void ClearCanvas (HWND);
void	PopupGetSaveAsNameBox();
void SaveSymbolLibrary ();
void ResizeWindow(HWND, int, int, int, int);

BOOL CALLBACK SymbolEditHandler(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	row, col, i;
    HDC		hDC, hCanvasDC;
    PAINTSTRUCT ps, symps;
	HWND	hsymclass, hSymCanvas;
	POINT	pt, ptsub;
	float	xscalesave, yscalesave;
	HMENU	hmenu;
    //SYMBOL_CLASS *pSymdb ;  //added by chi .
    switch(wMsg)
    {
		case WM_INITDIALOG:

			hsymclass = GetDlgItem(hDlg, IDC_SYMBOL_CLASS_LIST);
			for( i=0; i<symdb_class_cnt; i++ )
				SendMessage(hsymclass, LB_ADDSTRING, 0, (LPARAM)symdb[i].name);
			SendMessage(hsymclass, LB_SETCURSEL, info.sym.class, 0);

			hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
    		SymbolDlgInit (hSymCanvas);

			//CenterWindow(hDlg, ghWnd);
			CenterWindow(hDlg, ghWnd);
			return TRUE;

        case WM_PAINT:
		    if(hDC = BeginPaint(hDlg,&ps))
    		{
				xscalesave = xscale;
				yscalesave = yscale;
				xscale = yscale = 1.0f;
				hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
				hCanvasDC = BeginPaint(hSymCanvas, &symps);
   			 	if(ghPal)
    			{
    				SelectPalette (hCanvasDC, ghPal, FALSE);
    				RealizePalette (hCanvasDC);
				}
				DrawSymbolCanvas (hSymCanvas, hCanvasDC,symdb);
				EndPaint(hSymCanvas, &symps);
    			EndPaint(hDlg,&ps);
				xscale = xscalesave;
				yscale = yscalesave;
			}
            return TRUE;
		
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_SYMBOL_CLASS_LIST:
				switch( HIWORD(wParam) )
				{
				case LBN_SELCHANGE:
					hsymclass = GetDlgItem(hDlg, IDC_SYMBOL_CLASS_LIST);
					info.sym.class = SendMessage(hsymclass,
										LB_GETCURSEL,0,0);
					hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
					ReExposeCanvasAreaInWin(hDlg, hSymCanvas);
					return TRUE;
				default:	
					break;
				}
				break;
			case IDC_SED_NEW:
//				EndDialog(hDlg, wParam);
				ShowWindow(ghWndSymEdit, SW_HIDE);
            	EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_GRAYED );
              	DrawMenuBar( ghWnd );
				sym_edit_mode	= ADD_NEW_SYMBOL;
				ResetZoom ();
				SED_CanvasConfig();
				ClearCanvas (ghWnd);
				Redraw (ghWnd, ghDC);
				info.sym.name	= symdb[info.sym.class].nextpos;
				return TRUE;
			case IDC_SED_IMPORT:   //added by chi 04.09.03
				EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_ENABLED );
				if ( OpenNewFile( ghWnd, sym_file,SYM_FILE ) == FALSE )  
					return FALSE;
               //DialogBox( ghInst, MAKEINTRESOURCE(DLG_SYMBOL_SELECT),
			  	// 	ghWndToolKit, (DLGPROC)SymSelectDlgProc );
				sym_edit_mode	= ADD_NEW_SYMBOL;								
		        pSymdb =  malloc( sizeof(SYMBOL_CLASS)*MAX_SYMBOL_CLASS);
                if( pSymdb == NULL )
				{
                    MessageBox(ghWnd, "Insufficient memory available!","IPM提示:",MB_OK|MB_ICONERROR );
				    return FALSE;
				}
	            LoadSymbolLibrary (sym_file,&symdb_class, (SYMBOL_CLASS*)pSymdb);                
				GetSymbolSelectBox(SYMBOL_IMPORT);   //edit_mode	= SYMBOL_IMPORT;
                  

                 //cur_dsp_file
				 // to be continue........ chi
				free(pSymdb);
				edit_mode	= DISPLAY_EDIT;
				return TRUE;
			case IDC_SED_EDIT:
//				EndDialog(hDlg, wParam);
				ShowWindow(ghWndSymEdit, SW_HIDE);
            	EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_ENABLED );
              	DrawMenuBar( ghWnd );
				sym_edit_mode	= EDIT_EXISTING_SYMBOL;
                EditSymbol ();
				return TRUE;
			case IDC_SED_DELETE:
				sym_edit_mode	= DELETE_SYMBOL;
                DeleteSymbol ();
				hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
				ReExposeCanvasAreaInWin(hDlg, hSymCanvas);
				break;
			case IDC_SED_ADD_CLASS:
//				EndDialog(hDlg, wParam);
//            	ShowWindow(ghWndSymEdit, SW_HIDE);
            	EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_GRAYED );
              	DrawMenuBar( ghWnd );
				sym_edit_mode	= ADD_SYMBOL_CLASS;
				PopupGetSaveAsNameBox();
				return TRUE;
			case IDC_SED_DEL_CLASS:
				sym_edit_mode	= DELETE_SYMBOL_CLASS;
                DeleteSymbolClass ();
				break;
			case IDC_SED_SAVE:
                SaveSymbolLibrary ();
				break;
			case IDCANCEL:
//				EndDialog(hDlg, wParam);
				DestroyWindow (hDlg);
				ghWndSymEdit = NULL;
				edit_mode	= DISPLAY_EDIT;
				hmenu = GetMenu( ghWnd );
           		EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_ENABLED );
    			EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_ENABLED );
            	EnableMenuItem( hmenu, ID_Grid_Set, MF_BYCOMMAND | MF_ENABLED );
            	DrawMenuBar( ghWnd);
				return TRUE;
			}
			break;

		case WM_LBUTTONUP:
			pt.x = LOWORD(lParam);	
			pt.y = HIWORD(lParam);
			hSymCanvas = GetDlgItem(hDlg, IDC_SYMBOL_CANVAS);
			if ( PtInSubWin(hDlg, hSymCanvas, &pt, &ptsub) ) 
			{
            	col  = ptsub.x/sym_width;
            	row  = ptsub.y/sym_height;
            	info.sym.name   =col + row*sym_xn;
        		cur.bck.data.sym	= info.sym;
            	return TRUE;
			}
			break;

    }
    return FALSE;
}

void SED_CanvasConfig ()
{
	grid_width_save		= grid_width;
	grid_height_save	= grid_height;
    grid_width      = SED_canvas_width/SYMBOL_PIX;
    grid_height     = SED_canvas_height/SYMBOL_PIX;
	ResizeWindow(ghWnd, 0, 0, SED_canvas_width, SED_canvas_height );
}

void CanvasConfig ()
{
	grid_width	= grid_width_save;
	grid_height	= grid_height_save;
	MoveWindow(ghWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE); 
}

void AbortSymbol ()
{
	CanvasConfig ();
	ClearCanvas (ghWnd);
	ShowWindow(ghWndSymEdit, SW_SHOW);
    sym_edit_mode	= EDIT_SYMBOL_CANCLE ; 	
}

void EditSymbol ()
{

	ResetZoom ();
	SED_CanvasConfig ();

	ClearCanvas (ghWnd);
	LoadSymbolToGdbase (symdb[info.sym.class].symbol[info.sym.name]);
	Redraw (ghWnd, ghDC);

}


void LoadSymbolToGdbase (sym)
SYMBOL	sym;
{
	int     i, nextpos;
	BACKRCD	rcd;

	nextpos	= sym.nextpos;
	v4_gdbase.nextpos	= nextpos;

	for (i=0; i < nextpos; i++)
	{
		rcd	= sym.objects[i];
		rcd.rect.x1	= rcd.rect.x1*grid_width;
		rcd.rect.y1	= rcd.rect.y1*grid_height;
		rcd.rect.x2	= rcd.rect.x2*grid_width;
		rcd.rect.y2	= rcd.rect.y2*grid_height;

		switch	(rcd.draw_type)
		{
		case	DrwT_ARC:
			rcd.data.arc.x		=rcd.data.arc.x*grid_width;
			rcd.data.arc.y		=rcd.data.arc.y*grid_height;
			rcd.data.arc.width	=rcd.data.arc.width*grid_width;
			rcd.data.arc.height	=rcd.data.arc.height*grid_height;	
			break;

		case	DrwT_PLINE:
		{
			int	k;
			for (k=0; k < rcd.plcnt; k++)
			{
				rcd.data.pl[k].x	=rcd.data.pl[k].x*grid_width;
				rcd.data.pl[k].y	=rcd.data.pl[k].y*grid_height;
			}
		}
			break;

		case	DrwT_CIRCLE:
		case	DrwT_RECTANGLE:
		case	DrwT_LINE:
		case	DrwT_LINE_DASH:
			break;

		default:	;

		}

		v4_gdbase.buffer[i].bck	= rcd;

	}	

}

void DeleteSymbol ()
{
	int	i, symid, classid;

	classid = info.sym.class;
	symid = info.sym.name;
	for (i=symid; i < symdb[classid].nextpos-1; i++)
	{
		symdb[classid].symbol[i]	= symdb[classid].symbol[i+1];
	}
	symdb[classid].nextpos--;
	symdb[classid].nextpos = max(symdb[classid].nextpos, 0);  
	info.sym.name = min(info.sym.name, symdb[classid].nextpos-1 );  
}

void AddSymbolClass (sym_class_name)
char	*sym_class_name;
{
	HWND	hsymclass, hSymCanvas;

	if (symdb_class_cnt >= MAX_SYMBOL_CLASS)
	{ MessageBox (ghWnd, "Symbol Class Count Overflow!", "IPM", MB_OK); }
	else
	{
		strcpy (symdb[symdb_class_cnt].name, sym_class_name);
		info.sym.class = symdb_class_cnt;
		symdb_class_cnt++;

		hsymclass = GetDlgItem(ghWndSymEdit, IDC_SYMBOL_CLASS_LIST);
		SendMessage(hsymclass, LB_ADDSTRING, 0, (LPARAM)sym_class_name);
		SendMessage(hsymclass, LB_SETCURSEL, info.sym.class, 0);
		hSymCanvas = GetDlgItem(ghWndSymEdit, IDC_SYMBOL_CANVAS);
		ReExposeCanvasAreaInWin(ghWndSymEdit, hSymCanvas);
	}

}


void DeleteSymbolClass ()
{
	int	i, classid;
	HWND hsymclass, hSymCanvas;

	classid = info.sym.class;
	for (i=classid; i < symdb_class_cnt-1; i++)
	{
		symdb[i]	= symdb[i+1];
	}
	symdb_class_cnt--;
	symdb_class_cnt = max(symdb_class_cnt, 0);  
	info.sym.class = min(info.sym.class, symdb_class_cnt-1 );  

	hsymclass = GetDlgItem(ghWndSymEdit, IDC_SYMBOL_CLASS_LIST);
	SendMessage(hsymclass, LB_DELETESTRING, classid, 0);
	SendMessage(hsymclass, LB_SETCURSEL, info.sym.class, 0);
	hSymCanvas = GetDlgItem(ghWndSymEdit, IDC_SYMBOL_CANVAS);
	ReExposeCanvasAreaInWin(ghWndSymEdit, hSymCanvas);
}

void SaveSymbol (sym_name)
char	*sym_name;
{
	SYMBOL	sym;
	int	i, cnt, nextpos, class;

//#define	   grid_height  14
	
    // grid_width  =  grid_height = 14 ;
	cnt	=0;
	nextpos	= v4_gdbase.nextpos;
	for (i=0; i < nextpos; i++)
	{
		sym.objects[cnt]	= v4_gdbase.buffer[i].bck;
		sym.objects[cnt].rect.x1	= sym.objects[cnt].rect.x1/GRID;
		sym.objects[cnt].rect.y1	= sym.objects[cnt].rect.y1/GRID;
		sym.objects[cnt].rect.x2	= sym.objects[cnt].rect.x2/GRID;
		sym.objects[cnt].rect.y2	= sym.objects[cnt].rect.y2/GRID;

		switch	(sym.objects[i].draw_type)
		{
		case	DrwT_ARC:
			sym.objects[i].data.arc.x	=sym.objects[i].data.arc.x/GRID;
			sym.objects[i].data.arc.y	=sym.objects[i].data.arc.y/GRID;
			sym.objects[i].data.arc.width	=sym.objects[i].data.arc.width/GRID;
			sym.objects[i].data.arc.height	=sym.objects[i].data.arc.height/GRID;
			cnt++;
			break;

		case	DrwT_PLINE:
			{
				int	k;
				for (k=0; k < sym.objects[i].plcnt; k++)
				{
					sym.objects[i].data.pl[k].x	=sym.objects[i].data.pl[k].x/GRID;
					sym.objects[i].data.pl[k].y	=sym.objects[i].data.pl[k].y/GRID;
				}
			}
			cnt++;
			break;

		case	DrwT_CIRCLE:
		case	DrwT_RECTANGLE:
		case	DrwT_LINE:
		case	DrwT_LINE_DASH:
			cnt++;
			break;

		default: ;	  // text, sym

		}

	}
	sym.nextpos	= cnt;
	strcpy (sym.name, sym_name);

	/* save symbol 'sym' into 'symdb' database. */
	class	=info.sym.class;
	if (sym_edit_mode == EDIT_EXISTING_SYMBOL)
	{
		symdb[class].symbol[info.sym.name]      =sym;
	}
	else if (sym_edit_mode == ADD_NEW_SYMBOL )
	{
		if (symdb[class].nextpos < MAX_SYMBOLS)
		{ 
			symdb[class].symbol[info.sym.name]      =sym;
			symdb[class].nextpos++; 
            sym_edit_mode = EDIT_EXISTING_SYMBOL;
            EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_ENABLED );
            DrawMenuBar( ghWnd );
		}
		else
		{ MessageBox (ghWnd, "Symbol Buffer Over Flow", "IPM", MB_OK); }
	}
	else if (sym_edit_mode == COPY_EXISTING_SYMBOL)
	{
		if (symdb[class].nextpos < MAX_SYMBOLS)
		{ 
			symdb[class].symbol[symdb[class].nextpos]	=sym;
			info.sym.name	=symdb[info.sym.class].nextpos;
			symdb[class].nextpos++; 
		}
		else
		{ MessageBox (ghWnd, "Symbol Buffer Over Flow", "IPM", MB_OK); }
		sym_edit_mode	=EDIT_EXISTING_SYMBOL;
	}

//	AbortSymbol ();
}


