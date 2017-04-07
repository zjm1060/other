
#include	<stdio.h>

#include	"resource.h"
#include	"../inc/ipm.h"

#define		COLOR_GROUP_DEF

#include	"../inc/color_def.h"
#include	"../inc/color_grp_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/draw_def.h"
	
extern int MenuStateSave;
 
BOOL PtInSubWin(HWND, HWND, POINT *, POINT *);
void ReExposeCanvasAreaInWin(HWND, HWND);	
void PopupGetSaveAsNameBox();
void DrawBlackBackground(HWND, HDC);
void ReadPlData ( BACKRCD *, int, int);
void	ColorGroupInit (HWND, HWND);
void DrawSolidColorPalette (HWND, HDC);
void DrawColorPairPalette (HWND, HDC);
void DrawColorPair (HWND, HDC, int, int, int, int, int, COLOR_PAIR );
void SetCGRect( int, int, int, int, int, XRECT *);
void	DrawColorGroupCanvas (HWND hwnd, HDC hdc);
void CGED_DrawColor (HWND hwnd, HDC hdc, CGRP_CELL );
void DrawColorGroupPutCanvas (HWND hwnd, HDC hdc);
void AddNewColorGroup (HWND);
void SaveColorGroup (char *);
void DeleteColorGroup(HWND, int);


void	LoadColorGrpLibrary ()
{
	int	len, i;
	FILE	*fp;
	char	clr_grp_full_name[MAX_FILE_NAME_LEN];
	char	*clr_grp_part_name      = {"/DSP/COLOR_GRP_LIBRARY.DBIN"};
    char version[6] ;
	extern int version4_flag;
	int		j;

	FormFullName ( home, clr_grp_full_name, clr_grp_part_name);
	fp = fopen(clr_grp_full_name, "rb");

	if (fp == NULL)
	{
		MessageBox(NULL, "IPM: open COLOR_GRP_LIBRARY.DBIN error.", NULL, MB_OK | MB_ICONHAND);
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
		MessageBox(ghWnd,"当前颜色组库文件为v3.0a版，需要转换!\n如果编辑并保存颜色组库，则将转存成新的格式",
			                  "IPM:提示信息",MB_OK|MB_ICONINFORMATION) ;
		version4_flag= 0;
		rewind( fp );
	}

	len	=4;
	fread (&cgdb_cnt, len, 1, fp);
	cgdb_cnt = min(cgdb_cnt, MAX_COLOR_GRPS);
	len     =sizeof(cgdb[0]);
	for (i=0; i<cgdb_cnt; i++)
	{
		fread (&cgdb[i], len, 1, fp);
		if(version4_flag==0)
		{
			for(j=0; j<GRP_MAX_COLORS; j++)
				cgdb[i].cgrp_cell[j].c_normal= ColorId3to4(cgdb[i].cgrp_cell[j].c_normal);
		}
				
	}	

	fclose (fp);

	cg_buff	=cgdb[info.cgrp];

}


void SaveColorGrpLibrary ()
{
	int	len, i;
	FILE	*fp;
	char	clr_grp_full_name[MAX_FILE_NAME_LEN];
	char	*clr_grp_part_name = {"/DSP/COLOR_GRP_LIBRARY.DBIN"};
    char version[] = "v4.0a" ;

	FormFullName ( home, clr_grp_full_name, clr_grp_part_name);
	fp = fopen( clr_grp_full_name, "wb");
	if (fp == NULL)
	{
		printf ("open COLOR_GRP_LIBRARY.DBIN error.\n");
		ExitProc ();
	}


	fwrite(version,sizeof(char),6,fp) ;  //add a version tag before file;

	len     =4;
	fwrite (&cgdb_cnt, len, 1, fp);
	len	=sizeof(cgdb[0]);
	for (i=0; i<cgdb_cnt; i++)
	{
		fwrite (&cgdb[i], len, 1, fp);
	}
	fclose (fp);
}

int	offset=2;
int     cell_width, cell_height;
int	xn=8, yn=8;

BOOL CALLBACK ColorGroupEditHandler(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	xi, yi, i, index;
    HDC		hDC, hCanvasDC, hSolidDC, hFlashDC;
    PAINTSTRUCT ps, CGrpPS, SolidPS, FlashPS;
	HWND	hCGrpList, hCGrpCanvas, hSolidPal, hFlashPal;
	POINT	pt, ptsub;
    HMENU	hmenu;

    switch(wMsg)
    {
		case WM_INITDIALOG:
			cg_buff	=cgdb[info.cgrp];

			hCGrpList = GetDlgItem(hDlg, IDC_CGED_LIST);
			for( i=0; i<cgdb_cnt; i++ )
				SendMessage(hCGrpList, LB_ADDSTRING, 0, (LPARAM)cgdb[i].name);
			SendMessage(hCGrpList, LB_SETCURSEL, info.cgrp, 0);

			hCGrpCanvas = GetDlgItem(hDlg, IDC_CGED_CANVAS);
			hSolidPal = GetDlgItem(hDlg, IDC_CGED_SOLID);
            ColorGroupInit (hCGrpCanvas, hSolidPal);
			CenterWindow(hDlg, ghWnd);
			return TRUE;

        case WM_PAINT:
		    if(hDC = BeginPaint(hDlg,&ps))
    		{
				hCGrpCanvas = GetDlgItem(hDlg, IDC_CGED_CANVAS);
				hCanvasDC = BeginPaint(hCGrpCanvas, &CGrpPS);
   			 	if(ghPal)
    			{
    				SelectPalette (hCanvasDC, ghPal, FALSE);
    				RealizePalette (hCanvasDC);
				}
				DrawColorGroupCanvas(hCGrpCanvas, hCanvasDC);
				EndPaint(hCGrpCanvas, &CGrpPS);

				hSolidPal = GetDlgItem(hDlg, IDC_CGED_SOLID);
				hSolidDC = BeginPaint(hSolidPal, &SolidPS);
   			 	if(ghPal)
    			{
    				SelectPalette (hSolidDC, ghPal, FALSE);
    				RealizePalette (hSolidDC);
				}
				DrawSolidColorPalette(hSolidPal, hSolidDC);
				EndPaint(hSolidPal, &SolidPS);

				hFlashPal = GetDlgItem(hDlg, IDC_CGED_FLASH);
				hFlashDC = BeginPaint(hFlashPal, &FlashPS);
   			 	if(ghPal)
    			{
    				SelectPalette (hFlashDC, ghPal, FALSE);
    				RealizePalette (hFlashDC);
				}
				DrawColorPairPalette(hFlashPal, hFlashDC);
				EndPaint(hFlashPal, &FlashPS);
    			EndPaint(hDlg,&ps);
			}
            return TRUE;
		
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_CGED_LIST:
				switch( HIWORD(wParam) )
				{
				case LBN_SELCHANGE:
					hCGrpList = GetDlgItem(hDlg, IDC_CGED_LIST);
					info.cgrp = SendMessage(hCGrpList,LB_GETCURSEL,0,0);
					cg_buff	=cgdb[info.cgrp];
					cgrp_edit_mode	=EDIT_EXISTING_COLOR_GRP;
					EnableWindow(GetDlgItem(hDlg, IDC_CGED_SAVE), TRUE);
					hCGrpCanvas = GetDlgItem(hDlg, IDC_CGED_CANVAS);
					ReExposeCanvasAreaInWin(hDlg, hCGrpCanvas);
					return TRUE;
				default:	
					break;
				}
				break;
			case IDC_CGED_NEW:
				cgrp_edit_mode	=ADD_NEW_COLOR_GRP;
				EnableWindow(GetDlgItem(hDlg, IDC_CGED_SAVE), FALSE);
				AddNewColorGroup (hDlg);
				break;
			case IDC_CGED_SAVE:
				if (cgrp_edit_mode == ADD_NEW_COLOR_GRP)
				{ 
					PopupGetSaveAsNameBox (); 
				}
				else
				{ 
					SaveColorGroup (cg_buff.name); 
				}
				break;
			case IDC_CGED_SAVEAS:
				PopupGetSaveAsNameBox ();
               	break;
			case IDC_CGED_DEL:
				DeleteColorGroup (hDlg, info.cgrp);
				break;
			case IDC_CGED_SFILE:
				SaveColorGrpLibrary ();
				break;
			case IDCANCEL:
					DestroyWindow (hDlg);
					ghWndCGrpEdit = NULL;
					edit_mode	=DISPLAY_EDIT;
					hmenu = GetMenu( ghWnd );
           		 	EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_ENABLED );
          		  	EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MenuStateSave );
            		EnableMenuItem( hmenu, ID_SaveAs, MF_BYCOMMAND | MF_ENABLED );
    				EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_ENABLED );
            		EnableMenuItem( hmenu, ID_Grid_Set, MF_BYCOMMAND | MF_ENABLED );
            		DrawMenuBar( ghWnd);
					return TRUE;
			}
			break;

		case WM_LBUTTONUP:
			pt.x = LOWORD(lParam);	
			pt.y = HIWORD(lParam);
			hCGrpCanvas = GetDlgItem(hDlg, IDC_CGED_CANVAS);
			hSolidPal = GetDlgItem(hDlg, IDC_CGED_SOLID);
			hFlashPal = GetDlgItem(hDlg, IDC_CGED_FLASH);
			if ( PtInSubWin(hDlg, hCGrpCanvas, &pt, &ptsub) ) 
            {
				xi	= ptsub.x/CGED_clr_width;
				yi	= ptsub.y/CGED_clr_height;
				index	= yi; 
				cg_buff.cgrp_cell[index].name = index+1;
				if ( xi ==1)
				{
					cg_buff.cgrp_cell[index].flash	= cgrp_cell.flash;
				}
				else if ( xi == 0)
				{
					cg_buff.cgrp_cell[index].c_normal      = cgrp_cell.c_normal;
				}
				ReExposeCanvasAreaInWin(hDlg, hCGrpCanvas);
			}
			else if ( PtInSubWin(hDlg, hSolidPal, &pt, &ptsub) ) 
            {
				xi      = ptsub.x/cell_width;
				yi      = ptsub.y/cell_height;
				cgrp_cell.c_normal 	= ColorId3to4(xi + yi*xn);	
			}
			else if ( PtInSubWin(hDlg, hFlashPal, &pt, &ptsub) ) 
            {
				xi      = ptsub.x/cell_width;
				yi      = ptsub.y/cell_height;
				cgrp_cell.flash	= xi + yi*xn;
			}
			break;

    }
    return FALSE;
}


void	ColorGroupInit (HWND hCGrpDrawArea, HWND hSolidPal)
{
	RECT    rc;

	CGED_xn	= 4;
	CGED_yn	= 16;

	GetClientRect (hCGrpDrawArea, &rc);
	CGED_canvas_width = rc.right;
	CGED_canvas_height = rc.bottom;

	CGED_clr_width	=CGED_canvas_width/CGED_xn;
	CGED_clr_height	=CGED_canvas_height/CGED_yn;
	CGED_canvas_width	=CGED_clr_width*CGED_xn;
	CGED_canvas_height	=CGED_clr_height*CGED_yn;
	c_solid_type = C_DYN_SOLID_ON;
	c_flash_type = C_DYN_FLASH_ON;

	GetClientRect (hSolidPal, &rc);

	cell_width	= rc.right/xn;
	cell_height	= rc.bottom/yn;
	cgrp_cell.c_normal 	= ColorId3to4(0);
	cgrp_cell.flash 	= 0;
}

void DrawSolidColorPalette (HWND hSolidPal, HDC hSolidDC)
{
	int	ix, iy;
	BACKRCD	rcd;

	DrawBlackBackground(hSolidPal, hSolidDC);
	
	memset(&rcd, 0, sizeof(BACKRCD));
	rcd.bg= ColorId3to4(BLACK1);

	for ( iy=0 ; iy < yn; iy++)
	{
		for ( ix=0; ix < xn; ix++)
		{
			SetCGRect ( ix, iy, cell_width, cell_height, offset, &rcd.rect);
			rcd.fg	= ColorId3to4(xn*iy+ ix);
			rcd.line_width	= 1; 
			rcd.draw_type	= DrwT_RECTANGLE;
			rcd.fill.fill_type	= 4;
			DrawRcd ( hSolidPal, hSolidDC, &rcd);
		}
	}
}

void DrawColorPairPalette (HWND hFlashPal, HDC hFlashDC)
{
	int     ix, iy;
	
	DrawBlackBackground(hFlashPal, hFlashDC);

	for( iy=0 ; iy < yn; iy++)
	{
		for( ix=0; ix < xn; ix++)
		{
			DrawColorPair (hFlashPal, hFlashDC, ix, iy, cell_width, cell_height, offset, clr_pair[iy*xn+ix]);
		}
	}
}


void DrawColorPair (hwnd, hdc, ix, iy, width, height, offset, color_pair)
HWND hwnd;
HDC hdc;
int	ix, iy;
int	width, height, offset;
COLOR_PAIR	color_pair;
{
	BACKRCD	rcd;

	memset(&rcd, 0, sizeof(BACKRCD));
	rcd.bg	= ColorId3to4(BLACK1);
	rcd.line_width	= 1;
	rcd.draw_type	= DrwT_PLINE;
	rcd.fill.fill_type	= 4;
	SetCGRect( ix, iy, width, height, offset, &rcd.rect);

	rcd.plcnt       =0;
	ReadPlData ( &rcd, ix*width+offset, iy*height+offset);
	ReadPlData ( &rcd, (ix+1)*width-offset, iy*height+offset);
	ReadPlData ( &rcd, ix*width+offset, (iy+1)*height-offset);
	ReadPlData ( &rcd, ix*width+offset, iy*height+offset);
	rcd.fg	= ColorId3to4(color_pair.clr0);
	DrawRcd ( hwnd, hdc, &rcd);

	rcd.plcnt       =0;
	ReadPlData ( &rcd, (ix+1)*width-offset, iy*height+offset);
	ReadPlData ( &rcd, (ix+1)*width-offset, (iy+1)*height-offset);
	ReadPlData ( &rcd, ix*width+offset, (iy+1)*height-offset);
	ReadPlData ( &rcd, (ix+1)*width-offset, iy*height+offset);
	rcd.fg	= ColorId3to4(color_pair.clr1);
	DrawRcd ( hwnd, hdc, &rcd);

}


void SetCGRect( ix, iy, rect_width, rect_height, off_set, rect)
int	ix;
int	iy;
int	rect_width, rect_height;
int	off_set;
XRECT	*rect;
{
	rect->x1 =ix*rect_width+off_set;
	rect->y1 =iy*rect_height+off_set;
	rect->x2 =(ix+1)*rect_width-off_set; 
	rect->y2 =(iy+1)*rect_height-off_set;
}
	
void	DrawColorGroupCanvas (HWND hwnd, HDC hdc)
{
	int	i;

	DrawBlackBackground(hwnd, hdc);

	DrawColorGroupPutCanvas (hwnd, hdc);

	for (i=0; i < GRP_MAX_COLORS; i++)
	{
		if (cg_buff.cgrp_cell[i].name != 0)
		{
			CGED_DrawColor (hwnd, hdc, cg_buff.cgrp_cell[i]);
		}
	}
}


void CGED_DrawColor (HWND hwnd, HDC hdc, CGRP_CELL color_cell)
{
	int	ix, iy;
	int	index;
    BACKRCD 	rcd;

	memset(&rcd, 0, sizeof(BACKRCD));

	index 	= color_cell.name-1;

	iy      = index;
	rcd.draw_type	= DrwT_RECTANGLE;
	rcd.fill.fill_type	= 4;
	rcd.bg	=ColorId3to4(BLACK1);

	ix	=0;
 	SetCGRect ( ix, iy, CGED_clr_width,  CGED_clr_height,  4*offset, &rcd.rect);
	rcd.fg =	color_cell.c_normal;
	DrawRcd ( hwnd, hdc, &rcd);

	ix=1;
	DrawColorPair (hwnd, hdc, ix, iy, CGED_clr_width, CGED_clr_height, 4*offset, clr_pair[color_cell.flash]);

}
	
	
void DrawColorGroupPutCanvas (HWND hwnd, HDC hdc)
{
	int	ix, iy;
	BACKRCD 	rcd;

	memset(&rcd, 0, sizeof(BACKRCD));

	rcd.line_width 	= 1;
	rcd.draw_type 	= DrwT_RECTANGLE;
	rcd.fill.fill_type 	= 0;
	rcd.fg 		= ColorId3to4(GRAY1);
	rcd.bg 		= ColorId3to4(BLACK1);
	for ( iy=0 ; iy < CGED_yn; iy++)
	{
		for ( ix=0; ix < CGED_xn; ix++)
		{
			SetCGRect ( ix, iy, CGED_clr_width,  CGED_clr_height,  2*offset, &rcd.rect);
			DrawRcd ( hwnd, hdc, &rcd);
		}
	}				

}



void AddNewColorGroup (hDlg)
HWND	hDlg;
{
	int	i;
	HWND	hCGrpCanvas;

	cgrp_edit_mode	=ADD_NEW_COLOR_GRP;
	for (i=0; i < GRP_MAX_COLORS; i++)
	{
		cg_buff.cgrp_cell[i].name	= 0;
		cg_buff.cgrp_cell[i].c_normal	= 0;
		cg_buff.cgrp_cell[i].flash	= 0;
	}
	info.cgrp	=cgdb_cnt;
	hCGrpCanvas = GetDlgItem(hDlg, IDC_CGED_CANVAS);
	ReExposeCanvasAreaInWin(hDlg, hCGrpCanvas);

}


void SaveColorGroup (clr_grp_name)
char	*clr_grp_name;
{
	HWND	hCGrpList;

	if (cgrp_edit_mode == EDIT_EXISTING_COLOR_GRP)
	{
		cgdb[info.cgrp]	=cg_buff;
	}
	else if (cgrp_edit_mode == ADD_NEW_COLOR_GRP)
	{
		if (cgdb_cnt < MAX_COLOR_GRPS)
		{ 
			strcpy (cg_buff.name, clr_grp_name);
			cgdb[info.cgrp]	=cg_buff;
			cgdb_cnt++; 
			
			cgrp_edit_mode = EDIT_EXISTING_COLOR_GRP;
			hCGrpList = GetDlgItem(ghWndCGrpEdit, IDC_CGED_LIST);
			SendMessage(hCGrpList, LB_ADDSTRING, 0, (LPARAM)clr_grp_name);
			SendMessage(hCGrpList, LB_SETCURSEL, info.cgrp, 0);
			EnableWindow(GetDlgItem(ghWndCGrpEdit, IDC_CGED_SAVE), TRUE);
		}
		else
		{ 
			MessageBox (ghWndCGrpEdit, "Color Group Buffer Over Flow", "IPM", MB_OK);
		}
	}
	else if (cgrp_edit_mode == COPY_EXISTING_COLOR_GRP)
	{
		if (cgdb_cnt < MAX_COLOR_GRPS)
		{ 
			strcpy (cg_buff.name, clr_grp_name);
			info.cgrp	=cgdb_cnt;
			cgdb[info.cgrp]	=cg_buff;
			cgdb_cnt++; 

			hCGrpList = GetDlgItem(ghWndCGrpEdit, IDC_CGED_LIST);
			SendMessage(hCGrpList, LB_ADDSTRING, 0, (LPARAM)clr_grp_name);
			SendMessage(hCGrpList, LB_SETCURSEL, info.cgrp, 0);
			EnableWindow(GetDlgItem(ghWndCGrpEdit, IDC_CGED_SAVE), TRUE);
		}
		else
		{ 
			MessageBox (ghWndCGrpEdit, "Color Group Buffer Over Flow", "IPM", MB_OK);
		}
		cgrp_edit_mode	=EDIT_EXISTING_COLOR_GRP;
	}

}

void DeleteColorGroup(HWND hDlg, int cgrp_id)
{
	int	i;
	HWND hCGrpList, hCGrpCanvas;
		
	for (i=cgrp_id; i < cgdb_cnt-1; i++)
	{
		cgdb[i]	=cgdb[i+1];
	}
	cgdb_cnt--;
	cgdb_cnt = max(cgdb_cnt, 0);  
	info.cgrp = min(info.cgrp, cgdb_cnt-1 );  
	cg_buff = cgdb[info.cgrp];


	hCGrpList = GetDlgItem(hDlg, IDC_CGED_LIST);
	SendMessage(hCGrpList, LB_DELETESTRING, cgrp_id, 0);
	SendMessage(hCGrpList, LB_SETCURSEL, info.cgrp, 0);
	hCGrpCanvas = GetDlgItem(hDlg, IDC_CGED_CANVAS);
	ReExposeCanvasAreaInWin(hDlg, hCGrpCanvas);
}

