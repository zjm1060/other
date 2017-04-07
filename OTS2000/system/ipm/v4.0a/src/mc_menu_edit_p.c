
#include	<stdio.h>

#include	"../src/resource.h"

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/menu_def.h"
#include	"../inc/scaler.h"
#include	"../inc/mode_def.h"

void MenuInit ();
void EditMenu ();
void DeleteCtrlMenu (int);
void ClearCanvas (HWND);

extern int	MenuStateSave;
extern void v4_freadRcd  (FILE*, NEW_GBRCD*);
extern void fwriteRcd ();

//add by chi   for 4.0a  file data!!!
void TransV3toV4menu(MDBASE *v3gmendb  , NEW_MDBASE *v4mendb )
{   
	int  i ,j ; 


	i = j = 0 ;
  
    v4mendb->nextpos =  v3gmendb->nextpos ;
    v4mendb->tag =  v3gmendb->tag ;
	 
	for( i = 0 ; i< v3gmendb->nextpos ; i++ )
	{
         v4mendb->menu[i].tag =  v3gmendb->menu[i].tag ; 
		 v4mendb->menu[i].nextpos  =  v3gmendb->menu[i].nextpos ;
		 strcpy(v4mendb->menu[i].name ,  v3gmendb->menu[i].name ) ;

		 v4mendb->menu[i].x1  =  v3gmendb->menu[i].x1  ;
		 v4mendb->menu[i].y1  =  v3gmendb->menu[i].y1  ;
		 v4mendb->menu[i].x2  =  v3gmendb->menu[i].x2  ;
		 v4mendb->menu[i].y2  =  v3gmendb->menu[i].y2  ;
         for (j = 0 ; j<  v3gmendb->menu[i].nextpos; j++)
		 {
				memcpy(&v4mendb->menu[i].rcd[j].bck ,   &v3gmendb->menu[i].rcd[j].bck,sizeof(BACKRCD))  ;
                memcpy(&v4mendb->menu[i].rcd[j].lnk.act  ,   &v3gmendb->menu[i].rcd[j].lnk.act  ,sizeof(ACTION))  ;
				memcpy(&v4mendb->menu[i].rcd[j].lnk.cgrp   ,   &v3gmendb->menu[i].rcd[j].lnk.cgrp   ,sizeof(DYN_COLOR_GRP))  ;
				memcpy(&v4mendb->menu[i].rcd[j].lnk.dyn  ,   &v3gmendb->menu[i].rcd[j].lnk.dyn    ,sizeof(DYNAMIC))  ;
				strcpy(v4mendb->menu[i].rcd[j].lnk.dbname ,v3gmendb->menu[i].rcd[j].lnk.dbname );
				v4mendb->menu[i].rcd[j].bck.fg= ColorId3to4(v4mendb->menu[i].rcd[j].bck.fg);
				v4mendb->menu[i].rcd[j].bck.bg= ColorId3to4(v4mendb->menu[i].rcd[j].bck.bg);
		 }
	  
	}
 
}

void SaveMenuLibrary ();


void	LoadMenuLibrary ()
{
	int	len, i, j;
	FILE	*fp;
	char	menu_full_name[MAX_FILE_NAME_LEN];
	char	*menu_part_name = {"/DSP/CTRL_MENU_LIBRARY.DBIN"};
    char version[6] ;
	extern int version4_flag;

	FormFullName ( home, menu_full_name, menu_part_name );
	fp = fopen( menu_full_name, "rb");

	if (fp == NULL)
	{
		MessageBox(NULL, " open CTRL_MENU_LIBRARY.DBIN file error.", "IPM", MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}
 

   fread(version,1,6,fp);  //read
   if (strcmp("v4.0a",version)==0)    //4.0a *.meun file data formate
   {    
		version4_flag= 1;
	    
		len	=4;
		fread (&v4_menudb.nextpos, len, 1, fp);
		for (i=0; i<v4_menudb.nextpos; i++)
		{
			len	=4;
			fread (&v4_menudb.menu[i].nextpos, len, 1, fp);
			fread (&v4_menudb.menu[i].x1, len, 1, fp);
			fread (&v4_menudb.menu[i].y1, len, 1, fp);
			fread (&v4_menudb.menu[i].x2, len, 1, fp);
			fread (&v4_menudb.menu[i].y2, len, 1, fp);
			fread (v4_menudb.menu[i].name, sizeof(v4_menudb.menu[i].name), 1, fp);
			len     =sizeof (v4_menudb.menu[i].rcd[0]);
			for (j=0; j<v4_menudb.menu[i].nextpos; j++)
			{   
				v4_freadRcd (fp, &v4_menudb.menu[i].rcd[j]);
			}
		} //END FOR 

		fclose (fp);
   }
   else 
   {
		version4_flag= 0;
		MessageBox(ghWnd,"控制菜单文件的数据结构为v3.0a版，需要转换!\n注意!!!则这个文件将转存成新的格式",
			                  "IPM:提示信息",MB_OK|MB_ICONINFORMATION) ;
		rewind(fp);
	
		len	=4;
		fread (&menudb.nextpos, len, 1, fp);
		for (i=0; i<menudb.nextpos; i++)
		{
			len	=4;
			fread (&menudb.menu[i].nextpos, len, 1, fp);
			fread (&menudb.menu[i].x1, len, 1, fp);
			fread (&menudb.menu[i].y1, len, 1, fp);
			fread (&menudb.menu[i].x2, len, 1, fp);
			fread (&menudb.menu[i].y2, len, 1, fp);
			fread (menudb.menu[i].name, sizeof(menudb.menu[i].name), 1, fp);
			len     =sizeof (menudb.menu[i].rcd[0]);
			for (j=0; j<menudb.menu[i].nextpos; j++)
			{
				freadRcd (fp, &menudb.menu[i].rcd[j]);
			}
		}    //END FOR 

		TransV3toV4menu( &menudb  ,  &v4_menudb ); //from v3 to v4  chi		
		fclose (fp);
	//	SaveMenuLibrary () ;

	}

}

void SaveMenuLibrary ()
{
	int	len, i, j;
	FILE	*fp;
	char	menu_full_name[MAX_FILE_NAME_LEN];
	char	*menu_part_name = {"/DSP/CTRL_MENU_LIBRARY.DBIN"};
    char    version[] = "v4.0a" ;
	FormFullName ( home, menu_full_name, menu_part_name );
	fp = fopen( menu_full_name, "wb");

	if (fp == NULL)
	{
		MessageBox(NULL, " open CTRL_MENU_LIBRARY.DBIN file error.", "IPM", MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}
    fwrite(version,1,6,fp);  //version new ; 
	len	=4;
	fwrite (&v4_menudb.nextpos, len, 1, fp);
	for (i=0; i<v4_menudb.nextpos; i++)
	{
		len	=4;
		fwrite (&v4_menudb.menu[i].nextpos, len, 1, fp);
		fwrite (&v4_menudb.menu[i].x1, len, 1, fp);
		fwrite (&v4_menudb.menu[i].y1, len, 1, fp);
		fwrite (&v4_menudb.menu[i].x2, len, 1, fp);
		fwrite (&v4_menudb.menu[i].y2, len, 1, fp);
		fwrite (v4_menudb.menu[i].name, sizeof(v4_menudb.menu[i].name), 1, fp);
		len	=sizeof (v4_menudb.menu[i].rcd[0]);
		for (j=0; j<v4_menudb.menu[i].nextpos; j++)
		{
			fwriteRcd (fp, &v4_menudb.menu[i].rcd[j]);
		}
	}
	fclose (fp);
}


BOOL CALLBACK MenuEditHandler(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	i, menuid;
	HWND	hMenuList;
    HMENU	hmenu;

    switch(wMsg)
    {
		case WM_INITDIALOG:
			hMenuList = GetDlgItem(hDlg, IDC_MED_LIST);
			for( i=0; i<v4_menudb.nextpos; i++ )
				SendMessage(hMenuList, LB_ADDSTRING, 0, (LPARAM)v4_menudb.menu[i].name);
			SendMessage(hMenuList, LB_SETCURSEL, info.menu, 0);

            MenuInit ();
			CenterWindow(hDlg, ghWnd);
			return TRUE;

		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_MED_LIST:
				switch( HIWORD(wParam) )
				{
				case LBN_SELCHANGE:
					hMenuList = GetDlgItem(hDlg, IDC_MED_LIST);
					info.menu = SendMessage(hMenuList, LB_GETCURSEL, 0, 0);
					return TRUE;
				default:	
					break;
				}
				break;
			case IDC_MED_NEW:
				EndDialog(hDlg, wParam);
            	EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_GRAYED );
              	DrawMenuBar( ghWnd );
				menu_edit_mode	=ADD_NEW_MENU;
				info.menu	= v4_menudb.nextpos;
				ClearCanvas (ghWnd);
				return TRUE;
			case IDC_MED_EDIT:
				EndDialog(hDlg, wParam);
            	EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_ENABLED );
              	DrawMenuBar( ghWnd );
				menu_edit_mode	=EDIT_EXISTING_MENU;
                EditMenu ();
				return TRUE;
			case IDC_MED_DEL:
				menu_edit_mode	=DELETE_MENU;
                menuid = info.menu;
                DeleteCtrlMenu (info.menu);
				hMenuList = GetDlgItem(hDlg, IDC_MED_LIST);            	
				SendMessage(hMenuList, LB_DELETESTRING, menuid, 0);
				SendMessage(hMenuList, LB_SETCURSEL, info.menu, 0);
				break;
			case IDC_MED_SAVE_FILE:
                SaveMenuLibrary ();
				break;
			case IDCANCEL:
				EndDialog(hDlg, wParam);
				edit_mode	= DISPLAY_EDIT;
				hmenu = GetMenu( ghWnd );
           		EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_ENABLED );
            	EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MenuStateSave );
    			EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_ENABLED );
            	DrawMenuBar( ghWnd);
				return TRUE;
			}
			break;
    }
    return FALSE;
}

void	MenuInit ()
{
}

void LoadMenuToGdbase (menu)
NEW_MENU	menu;
{
	int     i, nextpos;

	nextpos	=menu.nextpos;
	v4_gdbase.nextpos	=nextpos;

	for (i=0; i < nextpos; i++)
	{
	  	v4_gdbase.buffer[i]	=menu.rcd[i];   //gtk .chi
	}	

}

void EditMenu ()
{
	ClearCanvas (ghWnd);
	LoadMenuToGdbase (v4_menudb.menu[info.menu]);
	Redraw (ghWnd, ghDC);
}


void DeleteCtrlMenu (int menuid)
{
	int	i;
		
	for (i=menuid; i < v4_menudb.nextpos-1; i++)
	{
		v4_menudb.menu[i]	= v4_menudb.menu[i+1];
	}
	v4_menudb.nextpos--;
	v4_menudb.nextpos = max(v4_menudb.nextpos, 0);  
	info.menu = min(info.menu, v4_menudb.nextpos-1 );  
}





void AbortMenu ()
{
	ClearCanvas (ghWnd);
	DialogBox( ghInst, MAKEINTRESOURCE(DLG_MENU_EDITOR),
				 ghWnd, (DLGPROC)MenuEditHandler );
}

void SaveMenu (menu_name)
char	*menu_name;
{
	NEW_MENU	menu;
	int	i, cnt, nextpos;

	strcpy (menu.name, menu_name);

	nextpos	=v4_gdbase.nextpos;
	cnt	=0;


	menu.x1	= GetSystemMetrics(SM_CXSCREEN);
	menu.y1	= GetSystemMetrics(SM_CYSCREEN);
	menu.x2	= 0;
	menu.y2	= 0;
	for (i=0; i < nextpos; i++)
	{
	  	menu.rcd[cnt]	=v4_gdbase.buffer[i];         //   gtk  chi ....
		switch	(menu.rcd[cnt].bck.draw_type)
		{

 
		case    DrwT_CIRCLE:
		case    DrwT_PLINE:
		case    DrwT_LINE:
		case	DrwT_TEXT:
		case	DrwT_RECTANGLE:
		case	DrwT_SYMBOL:
    	default: ;
			menu.x1	=min (menu.x1, min(menu.rcd[cnt].bck.rect.x1,menu.rcd[cnt].bck.rect.x2));
			menu.y1	=min (menu.y1, min(menu.rcd[cnt].bck.rect.y1,menu.rcd[cnt].bck.rect.y2));
			menu.x2	=max (menu.x2, max(menu.rcd[cnt].bck.rect.x2 ,menu.rcd[cnt].bck.rect.x1));
			menu.y2	=max (menu.y2, max(menu.rcd[cnt].bck.rect.y2, menu.rcd[cnt].bck.rect.y1));
			cnt++;
			break;

	

		}

	}
	menu.nextpos	=cnt;

	/* save menu 'menu' into 'menudb' database. */
	if (menu_edit_mode == EDIT_EXISTING_MENU)
	{
		v4_menudb.menu[info.menu] = menu;
	}
	else if (menu_edit_mode == ADD_NEW_MENU )
	{
		if (v4_menudb.nextpos < MAX_MENUS)
		{ 
			v4_menudb.menu[info.menu]      =menu;
			v4_menudb.nextpos++; 
            menu_edit_mode = EDIT_EXISTING_MENU;
            EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_ENABLED );
            DrawMenuBar( ghWnd );
		}
		else
		{ MessageBox (ghWnd, "Menu Buffer Over Flow", "IPM", MB_OK); }
	}
	else if (menu_edit_mode == COPY_EXISTING_MENU)
	{
		if (v4_menudb.nextpos < MAX_MENUS)
		{ 
			v4_menudb.menu[v4_menudb.nextpos]	=menu;
			info.menu	=v4_menudb.nextpos;
			v4_menudb.nextpos++; 
		}
		else
		{ MessageBox (ghWnd, "Menu Buffer Over Flow", "IPM", MB_OK); }
		menu_edit_mode	=EDIT_EXISTING_MENU;
	}

//	AbortMenu ();

}














