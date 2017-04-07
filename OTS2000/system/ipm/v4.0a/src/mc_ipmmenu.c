#include <windows.h>
#include <stdio.h>

#include	"../src/resource.h"

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/view_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/menu_def.h"
#include	"../inc/toolkit.h"
#include	"../inc/symbol_grp_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/color_grp_def.h"
#include	"../inc/string_grp_def.h"
#include	"../inc/pr.h"
int isDisplayDbName  = 0  ; 
extern FILE  *gCurDspFilefp ; 
extern  char   gCurDspFileName[256]  ; 
extern char	cur_dsp_file[MAX_FILE_NAME_LEN];
extern char szTitle[34];
extern int	MenuStateSave;
//0 --- up  ; 1 -- down ; 2 --left  ; 3 --right  ; 4 -- middle ;
typedef  enum {up=0, down, left ,  right , middle} ALIGN_MODE  ;

void EditButtonInit ();
void DynButtonInit ();
void SetPasteButton ();
void DeleteGdbItems ();
void ClearCanvas ();
void SendItemsToForeground (NEW_GDBUFF *);
void SendItemsToBackground (NEW_GDBUFF *);
void PasteItems (NEW_GDBUFF *);
BOOL SaveToFile( HWND );
BOOL SaveAs( HWND );
void DynamicsHandler (HWND, int);
void NoDynamicsHandler (HWND, int);
void DynamicsTest ( HWND, int);
void PopupFormatDialog ();
void GridFgBgSet (int);
void SetCanvasBgColor ();
void PopupGridSetBox();
void PopupLayerSetBox();
void PopupHideLockBox();
void SetCurrentLayerBtn (int);
void PopupGetSaveAsNameBox();
void AbortSubDsp();
void SubDspEditHandler ();
void AbortMenu ();
BOOL CALLBACK MenuEditHandler(HWND, UINT, WPARAM, LONG);
void SaveMenu (char *);
void SaveSymbol (char *);
void AbortSymbol ();
BOOL CALLBACK SymbolEditHandler(HWND, UINT, WPARAM, LONG);
BOOL CALLBACK ReplaceDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL APIENTRY Align( HWND , UINT ,UINT , LONG ) ; //chi
BOOL APIENTRY About( HWND , UINT ,UINT , LONG ) ; //chi
BOOL CALLBACK FindDlgProc(HWND  , UINT  , WPARAM  , LONG  )  ;  //chi
int PopupAttrDialog(HWND hParent);
void ZoomSelectedItem(double ratio);

void FormWindowTitle(char *WndTitle)
{
	strcpy(WndTitle, szTitle);
	strcat(WndTitle, "  --  ");
	strcat(WndTitle, cur_dsp_file);
	strcat(WndTitle, "          ");
}


/****************************************************************************

  FUNCTION: MenuProc(HWND, UINT, WPARAM, LPARAM)
  
	PURPOSE:  Processes application menu
	
****************************************************************************/

LRESULT CALLBACK IpmMenuProc( HWND hWnd, UINT message, WPARAM uParam, LPARAM lParam)
{
	//	FARPROC lpProcAbout;  // pointer to the "About" function
	int wmId, wmEvent;
	int		maxX=1, maxY=1;
	int MS; //test
	RECT	rc;
	FILE  *fp ; 
	char WndTitle[256];
	HMENU	hmenu;
    PRINTDLG  pd;
	wmId    = LOWORD(uParam);
	wmEvent = HIWORD(uParam);
	
	switch (wmId) 
	{
	case ID_NEWFILE:
		IpmInit() ;
		if (gdbuff.nextpos > 0 )
			SetPasteButton ();
		GetClientRect(ghWnd, &rc); 
		InvalidateRect(ghWnd, &rc, TRUE);
		break;
	case	ID_Open_File:
		if ( OpenNewFile( hWnd,cur_dsp_file, DSP_FILE ) == TRUE )
		{
			// enable the Save As and Print menu items
			EnableMenuItem( GetMenu( hWnd ), ID_SAVE,
				MF_BYCOMMAND | MF_ENABLED );
			EnableMenuItem( GetMenu( hWnd ), ID_Print,
				MF_BYCOMMAND | MF_ENABLED );
			DrawMenuBar( hWnd);
			// reset the title in the title bar to reflect the new open file
			FormWindowTitle(WndTitle);
			SetWindowText( hWnd, WndTitle );  //chi
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;
		
	case	ID_SAVE:
		if (edit_mode == DISPLAY_EDIT || edit_mode == SUB_DSP_EDIT)
		{
			SaveToFile( hWnd );
		}
		else if (edit_mode == SYMBOL_EDIT)  
		{
			SaveSymbol (symdb[info.sym.class].symbol[info.sym.name].name);
		}
		else if (edit_mode == MENU_EDIT)
		{
			SaveMenu (v4_menudb.menu[info.menu].name);
			//				AbortMenu ();
		}
		break;
		
	case	ID_SaveAs:
		if (edit_mode == DISPLAY_EDIT || edit_mode == SUB_DSP_EDIT)
		{
			if ( SaveAs( hWnd ) == TRUE )
			{
				EnableMenuItem( GetMenu( hWnd ), ID_SAVE,
					MF_BYCOMMAND | MF_ENABLED );
				DrawMenuBar( hWnd );
				FormWindowTitle(WndTitle);
				SetWindowText( hWnd, WndTitle );
			}
		}
		else if (edit_mode == MENU_EDIT)
		{
			PopupGetSaveAsNameBox();
			//				AbortMenu ();
		}
		else if (edit_mode == SYMBOL_EDIT)
		{
			PopupGetSaveAsNameBox();
		}
		break;
		
	case	ID_ABORT:
		if (edit_mode == DISPLAY_EDIT)
		{
		}  
		else if (edit_mode == SUB_DSP_EDIT)
		{
			AbortSubDsp ();
		}
		else if (edit_mode == SYMBOL_EDIT)  
		{
			AbortSymbol ();
		}
		else if (edit_mode == MENU_EDIT)  
		{
			AbortMenu ();
		}
		break;
	case ID_ALL_SELECT:
		SelectAllItems(&v4_gdbase);
		break;
		
	case ID_Print:
		{
			if ( prtdc == NULL ) 
			{
				
				memset ((void *) &pd, 0, sizeof(PRINTDLG));
				pd.lStructSize = sizeof(PRINTDLG);
				pd.hwndOwner   = hWnd;
				pd.hDevMode = (HANDLE)NULL;
				pd.hDevNames = (HANDLE)NULL;
				pd.Flags = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION | PD_PRINTSETUP;
				pd.nFromPage = 0;
				pd.nToPage = 0;
				pd.nMinPage = 0;
				pd.nMaxPage = 0;
				pd.nCopies = 1;   
				pd.hInstance   = NULL;
				
				if (PrintDlg(&pd) == TRUE)
				{
					prtdc = pd.hDC;
					Print(); 		
				}
				else
					ProcessCDError(CommDlgExtendedError(), hWnd );
				
			}
			
			DeleteDC(prtdc);
			prtdc = NULL;
			break;
		}
		
	case IDM_PRINTSETUP:
		{
			PRINTDLG  pd;
			
			memset ((void *) &pd, 0, sizeof(PRINTDLG));
			
			pd.lStructSize = sizeof(PRINTDLG);
			pd.hwndOwner   = hWnd;
			pd.hDevMode = (HANDLE)NULL;
			pd.hDevNames = (HANDLE)NULL;
			pd.Flags = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION | PD_PRINTSETUP  ;
			pd.nFromPage = 0;
			pd.nToPage = 0;
			pd.nMinPage = 0;
			pd.nMaxPage = 0;
			pd.nCopies = 0;
			pd.hInstance   = NULL;
			
			
			
			if (PrintDlg(&pd) == TRUE)
			{
				prtdc = pd.hDC;
				//Print();
				//DeleteDC(prtdc);
			}
			else
				ProcessCDError(CommDlgExtendedError(), hWnd );
			
			if (pd.hDevMode) GlobalFree (pd.hDevMode);
			if (pd.hDevNames) GlobalFree (pd.hDevNames);
			
			break;
		}
		/*
		case 	IDM_PALETTE:
		TestColor();
		break;
		
		*/
		
	case	ID_Symbol:
		ghWndSymEdit = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_SYMBOL_EDITOR),
			ghWndToolKit, (DLGPROC)SymbolEditHandler);
		if ( ghWndSymEdit )
		{
			edit_mode	=SYMBOL_EDIT;
			hmenu = GetMenu( hWnd );
			EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_GRAYED );
			MenuStateSave = EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_GRAYED );
			//EnableMenuItem( hmenu, ID_Grid_Set, MF_BYCOMMAND | MF_GRAYED );  chichi
			DrawMenuBar( hWnd);
			ShowWindow(ghWndSymEdit, SW_SHOW);
		}
		break;
		
	case	ID_Sym_Grps:
		ghWndSymGrpEdit = CreateDialog(ghInst,
			MAKEINTRESOURCE(DLG_SYMBOL_GRP_EDIT), ghWnd, (DLGPROC)SymbolGroupEditHandler);
		if ( ghWndSymGrpEdit )
		{
			edit_mode	=SYMBOL_GRP_EDIT;
			hmenu = GetMenu( hWnd );
			EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_GRAYED );
			MenuStateSave = EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( hmenu, ID_SaveAs, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_GRAYED );
			EnableMenuItem( hmenu, ID_Grid_Set, MF_BYCOMMAND | MF_GRAYED );
			DrawMenuBar( hWnd);
			ShowWindow(ghWndSymGrpEdit, SW_SHOW);
		}
		break;
		
	case	ID_Color_Grps:
		ghWndCGrpEdit = CreateDialog(ghInst,
			MAKEINTRESOURCE(DLG_CGRP_EDITOR), ghWnd, (DLGPROC)ColorGroupEditHandler);
		if ( ghWndCGrpEdit )
		{
			edit_mode	=COLOR_GRP_EDIT;
			hmenu = GetMenu( hWnd );
			EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_GRAYED );
			MenuStateSave = EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( hmenu, ID_SaveAs, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_GRAYED );
			EnableMenuItem( hmenu, ID_Grid_Set, MF_BYCOMMAND | MF_GRAYED );
			DrawMenuBar( hWnd);
			ShowWindow(ghWndSymGrpEdit, SW_SHOW);
		}
		break;
		
	case	ID_Str_Grps:
		if ( ghWndStrGrpEdit = CreateDialog( ghInst, MAKEINTRESOURCE(DLG_STRING_GRP_EDITOR),
			ghWnd, (DLGPROC)StringGroupEditHandler ) )
		{
			edit_mode	=STRING_GRP_EDIT;
			hmenu = GetMenu( hWnd );
			EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_GRAYED );
			MenuStateSave = EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( hmenu, ID_SaveAs, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_GRAYED );
			EnableMenuItem( hmenu, ID_Grid_Set, MF_BYCOMMAND | MF_GRAYED );
			DrawMenuBar( hWnd);
			ShowWindow(ghWndSymGrpEdit, SW_SHOW);
		}
		break;
		
	case	ID_Ctrl_Menus:
		edit_mode	=MENU_EDIT;
		hmenu = GetMenu( hWnd );
		EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_GRAYED );
		MenuStateSave = EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MF_GRAYED );
		EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_GRAYED );
		DialogBox( ghInst, MAKEINTRESOURCE(DLG_MENU_EDITOR),
			ghWnd, (DLGPROC)MenuEditHandler );
		break;
		
	case	ID_Sub_Display:
		SubDspEditHandler ();
		break;
		
	case	ID_Copy:
		cur.bck.draw_type   =DrwT_COPY;
		//added chi
		PasteRec.Pasting = TRUE ;
		if (gdbuff.nextpos>0 )
		{
			PasteRec.rect.x1 = XtoSX(gdbuff.buffer[0].bck.rect.x1) ;   
			PasteRec.rect.y1 = YtoSY(gdbuff.buffer[0].bck.rect.y1) ;  
			SetPasteButton ();  //test
		}
		break;

	case	ID_Undo:

		fp  =	FileOpen( gCurDspFileName); 
		if ( fp == NULL)
		{
			MessageBox(ghWnd, "Open File Error", NULL, MB_OK | MB_ICONINFORMATION);
			return;
		}	
		ReadGdbaseData (fp);
		ReadGdbaseData (fp);	
		InvalidateRect(ghWnd, NULL, TRUE);
		fclose (fp); 
		break; 
	case	ID_Move:
		cur.bck.draw_type   =DrwT_MOVE;
		break;
		
	case	ID_Cut:
		cur.bck.draw_type	=DrwT_CUT;
		//added chi
		PasteRec.Pasting = TRUE ;
		if (gdbuff.nextpos>0 )
		{
			PasteRec.rect.x1 = XtoSX(gdbuff.buffer[0].bck.rect.x1) ;   
			PasteRec.rect.y1 = YtoSY(gdbuff.buffer[0].bck.rect.y1) ;   
		}
		SetPasteButton ();
		
		break;
		
	case	ID_Paste:
		
		cur.bck.draw_type	=DrwT_CUT;
		PasteItems (&v4_gdbase);
		//added chi				
		DrawGdbuffItems (hWnd, k_paste_items, k_mv_items);
		MoveItemsFinished (&v4_gdbase,MOD_PASTE,UP);
		InvalidateRect(hWnd, NULL, TRUE); 
		PasteRec.Pasting = FALSE ; 
		hmenu = GetMenu( hWnd );
		if (gdbuff.nextpos > 0)
		{
			EnableMenuItem( hmenu, ID_Copy, MF_BYCOMMAND | MF_ENABLED );
			EnableMenuItem( hmenu, ID_Move, MF_BYCOMMAND | MF_ENABLED );
		}
		EnableMenuItem( hmenu, ID_Paste, MF_BYCOMMAND | MF_GRAYED );
		DrawMenuBar( ghWnd);
		
		break;
		
	case	ID_Replace:
		DialogBox( ghInst, MAKEINTRESOURCE(DLG_REPLACE),
			hWnd, (DLGPROC)ReplaceDlgProc );
		break;
	case	ID_FIND_IN_FILE:
		DialogBox( ghInst, MAKEINTRESOURCE(DLG_FIND_IN_FILE),
			hWnd, (DLGPROC)FindDlgProc );
		break ; 
		
	case	ID_Delete:
		DeleteGdbItems ();
		break;
		
	case 	ID_Clear_Scrn:
		ClearCanvas (hWnd);
		EditButtonInit ();
		DynButtonInit ();
		break;
		
	case 	ID_REDISPLAY:
		GetClientRect(hWnd, &rc); 
		InvalidateRect(hWnd, &rc, TRUE);
		break;
		
	case	ID_Resize:
		cur.bck.draw_type   =DrwT_RESIZE;
		break;
		
	case	ID_Reshape:
		cur.bck.draw_type   =DrwT_RESHAPE;
		break;
		
		/*	case	ID_in_Line:
		break;    */ 
		
	case	ID_Send_Fg:
		SendItemsToForeground (&v4_gdbase);
		GetClientRect(hWnd, &rc); 
		InvalidateRect(hWnd, &rc, TRUE);
		
		break;
		
	case	ID_Send_Bg:
		SendItemsToBackground (&v4_gdbase);
		GetClientRect(hWnd, &rc); 
		InvalidateRect(hWnd, &rc, TRUE);
		break;
		
	case	ID_Canvas_Bgd_Color:
		SetCanvasBgColor ();
		break;
		
	case	ID_Grid_Visible:
	case	ID_Grid_Valid:
	case	ID_Layer_Clutter:
		ToggleProc(wmId);
		break;
		
	case	ID_Fgd_Grid:
	case	ID_Bgd_Grid:
		GridFgBgSet (wmId);
		break;
		
	case	ID_Grid_Set:
		PopupGridSetBox();
		break;
		
	case	ID_View:
		View( );
		break;
		
	case	ID_Layer_LockHide:
		PopupHideLockBox();
		break;
		
	case	ID_Layer_Set:
		PopupLayerSetBox();
		break;
	case    ID_AUTO_SIZE:    //added chi
		hmenu = GetMenu(ghWnd) ;
		MS =GetMenuState ( hmenu,ID_AUTO_SIZE,MF_BYCOMMAND) ;
		if (GetMenuState ( hmenu,ID_AUTO_SIZE,MF_BYCOMMAND ) == MF_CHECKED)
			CheckMenuItem(hmenu, ID_AUTO_SIZE, MF_BYCOMMAND | MF_UNCHECKED);
		else
			CheckMenuItem(hmenu, ID_AUTO_SIZE, MF_BYCOMMAND | MF_CHECKED);
		break;
	case	ID_Layer_01:
	case	ID_Layer_02:
	case	ID_Layer_03:
	case	ID_Layer_04:
	case	ID_Layer_05:
	case	ID_Layer_06:
	case	ID_Layer_07:
	case	ID_Layer_08:
	case	ID_Layer_09:
	case	ID_Layer_10:
	case	ID_Layer_11:
	case	ID_Layer_12:
	case	ID_Layer_13:
	case	ID_Layer_14:
	case	ID_Layer_15:
		SetCurrentLayerBtn ( wmId - ID_Layer_01 );
		break;
	case    ID_NORMAL_PAGE:
	case	ID_SIMU_PAGE:
		SetPageRage ( wmId - ID_NORMAL_PAGE );
		break;
		
	case	ID_Add_Dynamic:
		PopupFormatDialog ();
		break;
		
	case	ID_Add_Display:
	case	ID_Add_Prog:
	case	ID_Add_CtrlMenu:
	case	ID_Add_Dyn_Color:
		DynamicsHandler(hWnd, wmId);
		break;
		
	case	ID_Del_Dynamics:
	case	ID_Del_Req:
	case	ID_Del_Dyn_Color:
		NoDynamicsHandler ( hWnd, wmId );
		break;
		
	case	ID_Datab_Test:
		DynamicsTest ( hWnd, wmId);
		break;
		
	case	ID_FILE_SEARCH:
		PopFileSearchDlg (wmId);
		break;
	case ID_DISPLAY_DBNAME:
		hmenu = GetMenu(ghWnd) ;
		MS =GetMenuState ( hmenu,ID_DISPLAY_DBNAME,MF_BYCOMMAND) ;
		if (GetMenuState ( hmenu,ID_DISPLAY_DBNAME,MF_BYCOMMAND ) == MF_CHECKED)
			CheckMenuItem(hmenu, ID_DISPLAY_DBNAME, MF_BYCOMMAND | MF_UNCHECKED);
		else
			CheckMenuItem(hmenu, ID_DISPLAY_DBNAME, MF_BYCOMMAND | MF_CHECKED);

		
		InvalidateRect(ghWnd, NULL, TRUE);
		break ; 
		
	case	ID_EXIT:
		if (MessageBox(hWnd, "是否退出?退出前请先进行文件保存!", "IPM:提示", MB_YESNOCANCEL|MB_ICONQUESTION) == IDYES)
		{
			if(ghWndSymEdit) DestroyWindow (ghWndSymEdit);
			if(ghWndSymGrpEdit) DestroyWindow (ghWndSymGrpEdit);
			if(ghWndCGrpEdit) DestroyWindow (ghWndCGrpEdit);
			if(ghWndStrGrpEdit) DestroyWindow (ghWndStrGrpEdit); 
			DestroyWindow (ghWndToolKit);
			DestroyWindow (hWnd);
		}
		else
			break;
	case ID_ALIGN:
		DialogBox (ghInst,
			MAKEINTRESOURCE(DLG_ALIGN),
			hWnd,
			Align);
		break;
	case IDM_ABOUT:
		DialogBox (ghInst,
			MAKEINTRESOURCE(DLG_ABOUT),
			hWnd,
			About);
		break;
		
	case ID_ATTRIBUTE:
		if(gdbuff.nextpos!=1)
			MessageBox(hWnd, "必须而且只能选择一个图元", NULL, MB_OK|MB_ICONERROR);
		else
			PopupAttrDialog(hWnd);
		break;
		
	case ID_SELECTED_shrink:
		ZoomSelectedItem(0.95);
		InvalidateRect( ghWnd, NULL, TRUE );
		break;
		
	case ID_SELECTED_extend:
		ZoomSelectedItem(1.0/0.95);
		InvalidateRect( ghWnd, NULL, TRUE );
		break;
		
	default:
		return (DefWindowProc(hWnd, message, uParam, lParam));
	}
	
	return (0);
}


/****************************************************************************
*                                                                          *
*  FUNCTION   : Align (hwnd, point);             added by chi 08.01.29     *
*                                                                          *
*  PURPOSE    :  Align the component of windows						    *                                                           *
*																			*
****************************************************************************/
int  AlignGraphCell() ; 

BOOL APIENTRY Align(HWND hDlg,UINT message, UINT wParam, LONG lParam)
{ 
    static ALIGN_MODE AlignMode ;  //0 --- up  ; 1 -- down ; 2 --left  ; 3 --right  ; 4 -- middle ;
	HWND hControll ;
    switch (message){
	case WM_INITDIALOG:
		hControll = GetDlgItem(hDlg, IDC_RADIO1);
		SendMessage (hControll, BM_SETCHECK, 1, 0) ;
		AlignMode = 0 ;
		return(TRUE);  
	case WM_COMMAND:
		// LOWORD added for portability
		switch (wParam)
		{
			
		case IDCANCEL:
			EndDialog(hDlg,0);
			return(TRUE);
		case IDC_RADIO1:
			AlignMode = up;
			break;
		case IDC_RADIO2:
			AlignMode = down;
			break;
		case IDC_RADIO3:
			AlignMode = left;
			break;
		case IDC_RADIO4:
			AlignMode = right;
			break;
		case IDC_RADIO5:
			AlignMode = middle;
			break;
			
		case IDOK :
			AlignGraphCell(AlignMode,&v4_gdbase) ; 
			InvalidateRect( ghWnd, NULL, TRUE );
			EndDialog(hDlg,0);			    
		default :
			break;
		}
		break;
    }
    return(FALSE);
	UNREFERENCED_PARAMETER(lParam);
	
}

/*移动某个图元若干偏移量*/
int MoveRCD(NEW_GBRCD *rcd ,int dx ,int dy ) 
{   
	int	pl_cnt;
	int 	numpt, xtmp, ytmp, width, height;
	AddValue ( &rcd->bck.rect.x1, &dx);
	AddValue ( &rcd->bck.rect.y1, &dy);
	AddValue ( &rcd->bck.rect.x2, &dx);
	AddValue ( &rcd->bck.rect.y2, &dy);
	
	if(rcd->bck.draw_type ==	DrwT_PLINE)
	{			
		pl_cnt= rcd->bck.plcnt;
		for(numpt=0; numpt< pl_cnt; numpt++)
		{
			AddValue_2 (&rcd->bck.data.pl[numpt].x, &dx);
			AddValue_2 (&rcd->bck.data.pl[numpt].y, &dy);
		}
		CheckRectCoordsOfPline (rcd->bck, &xtmp, &ytmp, &width, &height);
	}
	else if(rcd->bck.draw_type ==  DrwT_ARC)
	{
		AddValue_2 (&rcd->bck.data.arc.x, &dx);
		AddValue_2 (&rcd->bck.data.arc.y, &dy);
	}
	
}

//0 --- up  ; 1 -- down ; 2 --left  ; 3 --right  ; 4 -- middle   ;
int  AlignGraphCell(ALIGN_MODE  alignMode ,NEW_GDBUFF *gdb)
{
	int i ,dx, dy ,x,y;
	
	
	
	switch  (alignMode)
	{
	case   up:   //minY
		y = min(gdbuff.buffer[0].bck.rect.y1,gdbuff.buffer[0].bck.rect.y2 ); 
		
		for (i = 1  ; i <gdbuff.nextpos ; i++) 
		{
			y = min(min(gdbuff.buffer[i].bck.rect.y1,gdbuff.buffer[i].bck.rect.y2),y  ) ;     				 
		}
		break;
		
	case down: 
		y = max(gdbuff.buffer[0].bck.rect.y1,gdbuff.buffer[0].bck.rect.y2 );
		for (i = 1  ; i <gdbuff.nextpos ; i++) 
		{
			y = max(max(gdbuff.buffer[i].bck.rect.y1,gdbuff.buffer[i].bck.rect.y2),y  ) ;     				 
		}
		break;
		
	case   left:
		x = min(gdbuff.buffer[0].bck.rect.x1,gdbuff.buffer[0].bck.rect.x2 );
		for (i = 1  ; i <gdbuff.nextpos ; i++) 
		{
			x = min(min(gdbuff.buffer[i].bck.rect.x1,gdbuff.buffer[i].bck.rect.x2),x  ) ;     				 
		}
		break;
		
	case   right:
		x = max(gdbuff.buffer[0].bck.rect.x1,gdbuff.buffer[0].bck.rect.x2 );
		for (i = 1  ; i <gdbuff.nextpos ; i++) 
		{
			x = max(max(gdbuff.buffer[i].bck.rect.x1,gdbuff.buffer[i].bck.rect.x2),x  ) ;     				 
		}
		break;
		
	case   middle:
		//////////////////////
		break;
		
		
		//printf ("i");
	}
	
	
	switch  (alignMode)
	{
	case   up:   //minY
		for (i=0; i<gdb->nextpos; i++)
		{
			if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
			{	
				dx =   0   ;  
				dy =   y -min(gdb->buffer[i].bck.rect.y1  , gdb->buffer[i].bck.rect.y2 ) ; 
				MoveRCD(&gdb->buffer[i], dx,dy)  ;
    
			}
		}
		
		break;
		
	case down: 
		for (i=0; i<gdb->nextpos; i++)
		{
			if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
			{	
				dx =   0   ;  
				dy =   y -max(gdb->buffer[i].bck.rect.y1  , gdb->buffer[i].bck.rect.y2 ) ; 
				MoveRCD(&gdb->buffer[i], dx,dy)  ;
				
			}
		}
		
		break;
		
	case   left:
		for (i=0; i<gdb->nextpos; i++)
		{
			if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
			{	
				dy =   0   ;  
				dx =   x -min(gdb->buffer[i].bck.rect.x1  , gdb->buffer[i].bck.rect.x2 ) ; 
				MoveRCD(&gdb->buffer[i], dx,dy)  ;
				
			}
		}
		
		break;
		
	case   right:
		for (i=0; i<gdb->nextpos; i++)
		{
			if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
			{	
				dy =   0   ;  
				dx =   x -max(gdb->buffer[i].bck.rect.x1  , gdb->buffer[i].bck.rect.x2 ) ; 
				MoveRCD(&gdb->buffer[i], dx,dy)  ;
				
			}
		}
		
		break;
		
	case   middle:
		//////////////////////
		break;
		
		
		//printf ("i");
	}
	
	for (i=0; i<gdb->nextpos; i++)
	{
		if (gdb->buffer[i].bck.tag == ITEM_TAG_ON)
		{			
			/*	update gdbuff data.  
			gdbuff.buffer[gdbuff.nextpos]	= gdb->buffer[i];
			gdbuff.nextpos	++;    */
		}
	}
	
}

BOOL APIENTRY About(HWND hDlg,UINT message, UINT wParam, LONG lParam)
{
	
    switch (message){
	case WM_INITDIALOG:
		return(TRUE);
		
	case WM_COMMAND:
		// LOWORD added for portability
		if (LOWORD(wParam) == ID_DOK){
			EndDialog(hDlg,0);
			return(TRUE);
		}
		break;
    }
    return(FALSE);
	UNREFERENCED_PARAMETER(lParam);
}



/****************************************************************************
*                                                                          *
*  FUNCTION   : HandlePopupMenu (hwnd, point)   added by chi               *
*                                                                          *
*  PURPOSE    : Handles the display of the "floating" popup that appears   *                                                           *
*               on a mouse click in the app's client area.                 *
*                                                                          *
****************************************************************************/
VOID APIENTRY HandlePopupMenu (
							   HWND   hwnd,
							   POINT point)
							   
{   
    HMENU hMenu,hWndMenu ; 
	int i ;
	UINT ID ,STAT;
    HMENU hMenuTrackPopup;
	
    hMenu = LoadMenu (ghInst, MAKEINTRESOURCE(POP_UP));
	if (!hMenu)
        return;
    hWndMenu = GetMenu(hwnd); 
    hMenuTrackPopup = GetSubMenu (hMenu, 0);
	for (i=0; i< GetMenuItemCount(hMenuTrackPopup);i++)
    { 
		ID =GetMenuItemID(hMenuTrackPopup,i);
		if (ID == 0xFFFFFFFF) continue ;
		STAT = GetMenuState (hWndMenu,ID,MF_BYCOMMAND ) ;
        if (GetMenuState (hWndMenu,ID,MF_BYCOMMAND ) == MF_GRAYED)
			EnableMenuItem( hMenuTrackPopup, ID, MF_BYCOMMAND | MF_GRAYED );	
		if (GetMenuState (hWndMenu,ID,MF_BYCOMMAND ) == MF_CHECKED)
			CheckMenuItem(hMenuTrackPopup, ID, MF_BYCOMMAND | MF_CHECKED);
        if (GetMenuState (hWndMenu,ID,MF_BYCOMMAND ) == MF_UNCHECKED)
			CheckMenuItem(hMenuTrackPopup, ID, MF_BYCOMMAND | MF_UNCHECKED);
		
	}
	
    ClientToScreen (hwnd, (LPPOINT)&point);
	
    /* Draw and track the "floating" popup */
    TrackPopupMenu (hMenuTrackPopup, 0, point.x, point.y, 0, hwnd, NULL);
    DestroyMenu (hMenu);
}



void ZoomSelectedItem(double ratio)
{
	int	i;
	NEW_GBRCD	*rcd;

	for (i=v4_gdbase.nextpos-1 ; i >= 0; i--)
	{
		rcd	= &v4_gdbase.buffer[i];
		if (rcd->bck.layer != cur.bck.layer || rcd->bck.tag !=ITEM_TAG_ON)
			continue;
		
		switch (rcd->bck.draw_type)
		{
		case	DrwT_LINE:
		case	DrwT_LINE_DASH:
		case	DrwT_TEXT:
		case	DrwT_MODIFY_TEXT:
		case	DrwT_RECTANGLE:
		case	DrwT_CIRCLE:
		case	DrwT_SYMBOL:
		case	DrwT_GIF:
			rcd->bck.rect.x1 *= ratio;
			rcd->bck.rect.x2 *= ratio;
			rcd->bck.rect.y1 *= ratio;
			rcd->bck.rect.y2 *= ratio;
			break;	

		case	DrwT_ARC:
			rcd->bck.data.arc.x		*= ratio;
			rcd->bck.data.arc.y		*= ratio;
			rcd->bck.data.arc.width	*= ratio;
			rcd->bck.data.arc.height	*= ratio;
			break;
			
		case	DrwT_PLINE:
			{
				int	k;
				
				for (k=0; k < rcd->bck.plcnt; k++)
				{
					rcd->bck.data.pl[k].x 	*= ratio;
					rcd->bck.data.pl[k].y	*= ratio;
				}
			}

		}
	}
}



















