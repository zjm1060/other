/****************************************************************************

        PROGRAM: ipm.c

        PURPOSE: internative picture Maker Tool.

        FUNCTIONS:

        WinMain() - calls initialization function, processes message loop
        InitApplication() - initializes window data and registers window
        InitInstance() - saves instance handle and creates main window
        WndProc() - processes messages
        CenterWindow() - used to center the "About" box over application window

        COMMENTS:
				This is the main program for IPM .

****************************************************************************/
#define _WIN32_WINNT   0x0400   //added chi 
#include <windows.h>   // required for all Windows applications
#include <stdio.h>

#include	"resource.h"


#include	"../../../dms/v4.0a/inc/dms_com.h"

#include	"../inc/ipm.h"
#include	"../inc/color_def.h"
#include	"../inc/draw_def.h"
#include	"../inc/text_edit_def.h"
#include	"../inc/scaler.h"
#include	"../inc/zoom_def.h"
#include	"../inc/view_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/toolkit.h"
#include	"../inc/symbol_grp_def.h"
#include	"../inc/color_grp_def.h"
#include	"../inc/string_grp_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/draw_def.h" //ADDED CHI

extern VOID APIENTRY HandlePopupMenu (HWND, POINT  ) ;  //added chi
PRJDEF prj ;   //  Max  projects  15   //ADD CHICHI
HDC  hdcBck ; 
void    ButtonDoubleClick    (HWND hWnd, UINT message, WPARAM uParam, LPARAM lParam);

int    ReadPrjDef(PRJDEF *prj);
BOOL InitInstance(
				  HINSTANCE          hInstance,
				  int             nCmdShow ,
				  LPSTR lpCmdLine     );
				  
void FormWindowTitle(char *WndTitle);

#define REPAINT GetClientRect(hWnd, &rc);   InvalidateRect(hWnd, &rc, TRUE); //ADD chi

#define APPCLASS 	"ipm"    // The name of the main window class
char szTitle[34]   = 	"ipm  / 中国水利水电科学院自动化所"; // The title bar text
char	dspMSG[70] = "显示分辨率不为1280 * 1024这样显示\n   的画面不完整，是否继续?";
/****************************************************************************

        FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)

        PURPOSE: calls initialization function, processes message loop

        COMMENTS:

                Windows recognizes this function by name as the initial entry point
                for the program.  This function calls the application initialization
                routine, if no other instance of the program is running, and always
                calls the instance initialization routine.  It then executes a message
                retrieval and dispatch loop that is the top-level control structure
                for the remainder of execution.  The loop is terminated when a WM_QUIT
                message is received, at which time this function exits the application
                instance by returning the value passed by PostQuitMessage().

                If this function must abort before entering the message loop, it
                returns the conventional value NULL.

****************************************************************************/
int CALLBACK WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
        MSG msg;
        HANDLE hAccelTable;
	    char dbname[100],prjhome[100];
/////////////////////////////////v4.0a ///////////////////////////////////	

        
		memset(home, 0, sizeof(home));
		strcpy(home, "/home/ems/project");

//		GifInit ();
		if (!ReadPrjDef(&prj)) exit(0);
		if (strcmp(prj.project[prj.curprj].name,""))
		{    strcat(home,"/");
		     strcat(home , prj.project[prj.curprj].name);
		}
		strcpy(prjhome, home);
	//	strcat(home,"/ipm/v4.0a");
	 	LoadSecurityFile (); 
		LoadStringGrpLibrary ();
		
		LoadSymbolLibrary ("",&symdb_class_cnt,symdb);   //modified by chi     
		LoadSymbolGrpLibrary ();
		LoadColorGrpLibrary ();
		LoadMenuLibrary ();   
		ReadTmpltFile();
		CreateColorPalette();   
		CreateFillPalette ();  
		CreateLogFont ();  
      
  


    //  SetErrLogMode(1,0);


       sprintf (dbname,"%s/dms/dat/DATABASE.OUT",prjhome);
        
		if  ( MountDB (NULL)==-1)
		{
				MessageBox(ghWnd, "IPM: 错误： MountDB Error !", NULL, MB_OK | MB_ICONERROR);
			//	return (FALSE);
		}

        if (!hPrevInstance) 
        {        
			if (!InitApplication(hInstance)) 
			{  
				return (FALSE);      
			}
        }

        

        if (!InitInstance(hInstance, nCmdShow,lpCmdLine)) 
        {
			return (FALSE);
        }


        hAccelTable = LoadAccelerators (hInstance, MAKEINTRESOURCE(IDR_ACC));	   
		//hAccel = LoadAccelerators (hInstance, szAppName) ;
		ghWndToolKit = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_TOOLKIT), ghWnd, (DLGPROC)ToolKitProc);
		ShowWindow(ghWndToolKit, SW_SHOW);

        /* Acquire and dispatch messages until a WM_QUIT message is received. */
        while (GetMessage(&msg, NULL, 0, 0))
        {
			if ( (!IsWindow(ghWndToolKit) || !IsDialogMessage(ghWndToolKit, &msg)) &&
				 (!IsWindow(ghWndSymGrpEdit) || !IsDialogMessage(ghWndSymGrpEdit, &msg)) ) 
			{ 
				if (!TranslateAccelerator (ghWnd, hAccelTable, &msg)){    ///add chi 
				TranslateMessage(&msg);// Translates virtual key codes
				DispatchMessage(&msg); }// Dispatches message to window 
			}
		}






		/* unregisters all classes registered for the application. */
		UnregisterClass(APPCLASS,hInstance);

/*		UnregisterClass(PALETTECLASS,hInstance);
*/		return (msg.wParam); // Returns the value from PostQuitMessage

        lpCmdLine; // This will prevent 'unused formal parameter' warnings

}


/****************************************************************************

        FUNCTION: InitApplication(HINSTANCE)

        PURPOSE: Initializes window data and registers window class

        COMMENTS:

                This function is called at initialization time only if no other
                instances of the application are running.  This function performs
                initialization tasks that can be done once for any number of running
                instances.

                In this case, we initialize a window class by filling out a data
                structure of type WNDCLASS and calling the Windows RegisterClass()
                function.  Since all instances of this application use the same window
                class, we only need to do this when the first instance is initialized.

				This routine registers all classes necessary for the application.

****************************************************************************/

BOOL InitApplication(HINSTANCE hInstance)
{
        WNDCLASS  wc;


	/*
    ** Set the common wndClass information.  This is common to all windows
    ** of this application.
    */
    wc.cbClsExtra    = 0;                      		// No per-class extra data.
    wc.cbWndExtra    = sizeof(LONG);            	// per-window extra data.
    wc.hCursor       = NULL;              //LoadCursor(NULL, IDC_ARROW);	// Cursor
    wc.hInstance     = hInstance;               	// Owner of this class

    /*
    ** Register the main top-level window .
    */
    wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;	// Class style(s).
    wc.lpfnWndProc   = (WNDPROC)WndProc;       // Window Procedure
    wc.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_IPM)); // Icon name from .RC
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_IPM); // Menu from .RC
    wc.lpszClassName = APPCLASS;              // Name to register as

    if(!RegisterClass(&wc))
        return(FALSE);

	/*
    ** Register the view window.
    */
 
    wc.style         = CS_HREDRAW | CS_VREDRAW;	// Class style(s).
   	wc.lpfnWndProc   = ViewProc;
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,255)); // blue background
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = VIEWCLASS;

    if(!RegisterClass(&wc))
    {
        UnregisterClass(APPCLASS,hInstance);
        return(FALSE);
    }



	/*
    ** Register the log palette window.
    */
  /*  wc.lpfnWndProc   = PaletteProc;
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = PALETTECLASS;

    if(!RegisterClass(&wc))
    {
        UnregisterClass(APPCLASS,hInstance);
        UnregisterClass(VIEWCLASS,hInstance);
        return(FALSE);
    }
*/
	return(TRUE);
}


/****************************************************************************

        FUNCTION:  InitInstance(HINSTANCE, int)

        PURPOSE:  Saves instance handle and creates main window

        COMMENTS:

                This function is called at initialization time for every instance of
                this application.  This function performs initialization tasks that
                cannot be shared by multiple instances.

                In this case, we save the instance handle in a static variable and
                create and display the main program window.

****************************************************************************/

BOOL InitInstance(
				HINSTANCE          hInstance,
				int             nCmdShow ,
		     	LPSTR lpCmdLine     )
{
    char WndTitle[256];  //added by chi useed to display title of window!
	char FileName[256]; int iLen ;
	// Save the instance handle in static variable, which will be used in
	// many subsequence calls from this application to Windows.

	ghInst = hInstance; // Store instance handle in our global variable

	// Create a main window for this application instance.

	ghWnd = CreateWindow(
				APPCLASS,            // See RegisterClass() call.
  				szTitle,             // Text for window title bar.
   				WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL, // Window style WS_OVERLAPPEDWINDOW | WS_VSCROLL
   				0, 0, 				 // position
   				GetSystemMetrics(SM_CXSCREEN), //width
   				GetSystemMetrics(SM_CYSCREEN), //height 
   				NULL,                // Overlapped windows have no parent.
   				NULL,                // Use the window class menu.
				ghInst,           // instance handle
 				NULL                 // no window creation data
			);

	// If window could not be created, return "failure"
	if (!ghWnd)
	{
		return (FALSE);
	}

	IpmInit ();


//////////////////////////////////////chi  05.03.03/////////////////////////////////////////
iLen = strlen(lpCmdLine) ;
if (iLen!= 0)
{      
        strncpy(FileName,++lpCmdLine,iLen-2);
        FileName[iLen-2] ='\0' ;
	    //MessageBox(ghWnd, FileName, NULL, MB_OK | MB_ICONINFORMATION);  //test chi
		
	    strcpy(cur_dsp_file , FileName) ;
    	OpenDspFile (FileName);
        
        // enable the Save As and Print menu items
        EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                            MF_BYCOMMAND | MF_ENABLED );
        EnableMenuItem( GetMenu( ghWnd ), ID_Print,
                             MF_BYCOMMAND | MF_ENABLED );
        DrawMenuBar( ghWnd); 
                // reset the title in the title bar to reflect the new open file
	
		FormWindowTitle(WndTitle);
        SetWindowText( ghWnd, WndTitle );  //chi
        InvalidateRect( ghWnd, NULL, TRUE );
           
}

/////////////////////////////////////////////////////////////////////////////////////////////





	// Make the window visible; update its client area; and return "success"
	ShowWindow(ghWnd, nCmdShow); // Show the window
	UpdateWindow(ghWnd);         // Sends WM_PAINT message



// 	PostMessage(ghWnd, WM_COMMAND, IDM_ABOUT, 0);

	return (TRUE);              // We succeeded...

}

/****************************************************************************

        FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)

        PURPOSE:  Processes messages

        MESSAGES:

        WM_COMMAND    - application menu (About dialog box)
        WM_DESTROY    - destroy window

****************************************************************************/

LRESULT CALLBACK WndProc(
                HWND hWnd,         // window handle
                UINT message,      // type of message
                WPARAM uParam,     // additional information
                LPARAM lParam)     // additional information
{
//	FARPROC lpProcAbout;  // pointer to the "About" function
	PAINTSTRUCT	ps;
	RECT	rc;
	HMENU	hmenu;HDC hdc;
	HBRUSH	hbrBkGnd;    
    POINT pt;  //test
    int xpos;
	int         iHStep,iVStep,iVertPos, iHorzPos  ; //added chi
	int fwKeys ; short zDelta ;    // wheel rotation

	switch (message) 
	{
	case	WM_CREATE:
			/* get coordinate transfer scaler */
			GetClientRect(hWnd, &rc);

			xscale   = (float)rc.right /  STD_WIDTH;
		    yscale   = (float)rc.bottom /  STD_HEIGHT;
			canvas_width = rc.right;
			canvas_height = rc.bottom;

//			IpmInit ();
			hmenu = GetMenu(hWnd);

            ///////   Added by chi ////////////
			hdc =GetDC(hWnd) ;
          		 
         

			xpos =(GetDeviceCaps(hdc,HORZRES) );
            xpos =(GetDeviceCaps(hdc,VERTRES) );
        	if (!((GetDeviceCaps(hdc,HORZRES) == DSP_HOR) && (GetDeviceCaps(hdc,VERTRES) == DSP_VER)))
			if (MessageBox(hWnd,dspMSG,"IPM:提示",MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL)== IDNO)
					exit(0) ;
               // Set vertical scroll bar range and page size
			  si.cbSize = sizeof (si) ;
			  si.fMask  = SIF_RANGE | SIF_PAGE| SIF_POS ;
			  si.nMin   = 0 ;
			  si.nMax   = canvas_height -1;          //NUMLINES - 1 ;
			  si.nPage  = canvas_height ;
			  SetScrollInfo (hWnd, SB_VERT, &si, TRUE) ;

               // Set horizontal scroll bar range and page size

          si.cbSize = sizeof (si) ;
          si.fMask  = SIF_RANGE | SIF_PAGE ;
          si.nMin   = 0 ;
          si.nMax   = canvas_width - 1;          //NUMLINES - 1 ;
          si.nPage  = canvas_width ;
          SetScrollInfo (hWnd, SB_HORZ, &si, TRUE) ;  

//			CheckMenuItem(, IHORZRESDM_AUTO_DSP, MF_BYCOMMAND | MF_UNCHECKED);
 			EnableMenuItem( hmenu, ID_SAVE,	MF_BYCOMMAND | MF_GRAYED );
            EnableMenuItem( hmenu, ID_Print, MF_BYCOMMAND | MF_GRAYED );
	        
//			CreateCaret(hWnd, (HBITMAP)1, 0, 12);
			break;

	case	WM_SETFOCUS:
			/* Create the caret when the window receives the keyboard focus. */
			CreateCaret(hWnd, (HBITMAP)1, 1, 15);
			break;

	case 	WM_KILLFOCUS:
			/* Destroy the caret when the window loses the keyboard focus. */
			DestroyCaret();  
			break;
			 
/*	case 	WM_CHAR: 
        	if (cur.bck.draw_type != DrwT_TEXT && cur.bck.draw_type != DrwT_MODIFY_TEXT) break;
            KeyPressProcess(hWnd, message, uParam, lParam);
        	break;

	case 	WM_KEYDOWN: 
   //     	if (cur.bck.draw_type != DrwT_TEXT && cur.bck.draw_type != DrwT_MODIFY_TEXT) break;
            if (cur.bck.draw_type != DrwT_MODIFY_TEXT) break;
            KeyPressProcess(hWnd, message, uParam, lParam);
        	break;
*/      
	case 	WM_PAINT:
			ghDC = BeginPaint(hWnd, &ps);
		
			      // Get vertical scroll bar position added by chi

            si.cbSize = sizeof (si) ;
            si.fMask  = SIF_POS ;
            GetScrollInfo (hWnd, SB_VERT, &si) ;
            iVertPos = si.nPos ;

               // Get horizontal scroll bar position

            GetScrollInfo (hWnd, SB_HORZ, &si) ;
            iHorzPos = si.nPos ;

			if(ghPal) 
			{
				SelectPalette (ghDC, ghPal, FALSE);
    			RealizePalette (ghDC);
			}


 			Redraw(hWnd,ghDC );       /*ghDC*/
		
			EndPaint(hWnd, &ps);
			break;
				 
	case 	WM_ERASEBKGND:
			GetClientRect(hWnd, &rc);
			if(ghPal) 
			{
				SelectPalette ((HDC)uParam, ghPal, FALSE);
    			RealizePalette ((HDC)uParam);
			}
			hbrBkGnd = CreateSolidBrush(Colors(canvas_bg_color) );  //canvas_bg_color
			FillRect((HDC)uParam, &rc, hbrBkGnd);
			DeleteObject(hbrBkGnd);
			return(1L);

///////////////////////added by chi use scroll bar control to display clent area!/////////////////
	case WM_SIZE:
          cxClient = LOWORD (lParam) ;
          cyClient = HIWORD (lParam) ;
          xscale   = (float)cxClient /  STD_WIDTH;
	      yscale   = (float)cyClient /  STD_HEIGHT;
		  zoomdb.canvas_w	= canvas_width  = cxClient;
		  zoomdb.canvas_h	= canvas_height = cyClient ;     
		 // xscale =yscale =1  ;
 
          break ; 
		  
    case WM_VSCROLL:
               // Get all the vertial scroll bar information

          si.cbSize = sizeof (si) ;
          si.fMask  = SIF_ALL ;
          GetScrollInfo (hWnd, SB_VERT, &si) ;

               // Save the position for comparison later on
          iVStep = (int)(20 * zoomdb.factor) ;  //   si.nMax /24;
          iVertPos = si.nPos ;

          switch (LOWORD (uParam))
          {
          case SB_TOP:
               si.nPos = si.nMin ;
               break ;
               
          case SB_BOTTOM:
               si.nPos = si.nMax ;
               break ;
               
          case SB_LINEUP:
               si.nPos -= iVStep ;
               break ;
               
          case SB_LINEDOWN:
               si.nPos += iVStep ;
               break ;
               
          case SB_PAGEUP:
               si.nPos -= si.nPage ;
               break ;
               
          case SB_PAGEDOWN:
               si.nPos += si.nPage ;
               break ;
               
          case SB_THUMBTRACK:
               si.nPos = si.nTrackPos ;
               break ;
               
          default:
               break ;         
          }
               // Set the position and then retrieve it.  Due to adjustments
               //   by Windows it may not be the same as the value set.

          si.fMask = SIF_POS ;
          SetScrollInfo (hWnd, SB_VERT, &si, TRUE) ;
          GetScrollInfo (hWnd, SB_VERT, &si) ;

               // If the position has changed, scroll the window and update it

          if (si.nPos != iVertPos)
          {                    
              zoomdb.sy_orig +=  (iVertPos - si.nPos);    
		      CheckZoomOrigin ();
              InvalidateRect(hWnd, NULL, TRUE);
			 
			  if( IsWindow(view) )
			      InvalidateRect(view, NULL, TRUE);
          }
          return 0 ;
          
     case WM_HSCROLL:
               // Get all the vertial scroll bar information

          si.cbSize = sizeof (si) ;
          si.fMask  = SIF_ALL ;
          GetScrollInfo (hWnd, SB_HORZ, &si) ;
               // Save the position for comparison later on

         
		  iHStep = (int)(20 * zoomdb.factor) ;     //si.nMax /24 ;  
          iHorzPos = si.nPos ;

          switch (LOWORD (uParam))
          {
		  case SB_LEFT:
               si.nPos = si.nMin ; 
			   break;
          case SB_RIGHT:
               si.nPos = si.nMax ;
			   break;
          case SB_LINELEFT:
               si.nPos -= iHStep ;
               break ;
               
          case SB_LINERIGHT:
               si.nPos += iHStep ;
               break ;
               
          case SB_PAGELEFT:
               si.nPos -= si.nPage ;
               break ;
               
          case SB_PAGERIGHT:
               si.nPos += si.nPage ;
               break ;
               
          case SB_THUMBPOSITION:
               si.nPos = si.nTrackPos ;
               break ;
               
          default :
               break ;
          }
               // Set the position and then retrieve it.  Due to adjustments
               //   by Windows it may not be the same as the value set.

          si.fMask = SIF_POS ;
          SetScrollInfo (hWnd, SB_HORZ, &si, TRUE) ;
          GetScrollInfo (hWnd, SB_HORZ, &si) ;
          
               // If the position has changed, scroll the window 

          if (si.nPos != iHorzPos)
          {

              /*ScrollWindow (hWnd, cxChar * (iHorzPos - si.nPos), 0, 
                             NULL, NULL) ;  */
              zoomdb.sx_orig += (iHorzPos - si.nPos);    
		      CheckZoomOrigin ();
              InvalidateRect(hWnd, NULL, TRUE);
			  if( IsWindow(view) )
			      InvalidateRect(view, NULL, TRUE);
          }
          return 0 ;
		  
/////////////////////////////////////end scroll /////////////////////////////////////////////////////
		  
	case	WM_COMMAND:  // message: command from application menu  added chi
		switch (LOWORD (uParam))
        {
          		          
          		case 	IDM_EDIT_PASTE:
				 	    hmenu = GetMenu( ghWnd );
                        if (GetMenuState (hmenu,ID_Paste,MF_BYCOMMAND )== MFS_ENABLED  )
                        IpmMenuProc(  hWnd,  0, ID_Paste, 0) ;
					    break ;
          
          		case 	IDM_EDIT_COPY:
                        hmenu = GetMenu( ghWnd );
                        if (GetMenuState (hmenu,ID_Copy,MF_BYCOMMAND )== MFS_ENABLED  )
						IpmMenuProc(  hWnd,  0, ID_Copy, 0) ;
					    break ;

                case   	ID_EDIT_RESIZE:
					    hmenu = GetMenu( ghWnd );
                        if (GetMenuState (hmenu,ID_Resize,MF_BYCOMMAND )== MFS_ENABLED  )
                            cur.bck.draw_type   =DrwT_RESIZE;
			            break;
				case     ID_SAVE:
					    hmenu = GetMenu( ghWnd );
                        if (GetMenuState (hmenu,ID_SAVE,MF_BYCOMMAND )== MFS_ENABLED  )
				    	IpmMenuProc(  hWnd,  0, ID_SAVE, 0) ;
					    break ;
                case     ID_ALL_SELECT:
					    IpmMenuProc(  hWnd,  0, ID_ALL_SELECT, 0) ;
				     	break;

                default :	IpmMenuProc (hWnd, message, uParam, lParam);  break;
		}
        return 0 ;
//#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)  //added by chi 

	
	case    WM_MOUSEWHEEL:
		    if (zoomdb.factor <=1) break;
		    fwKeys = LOWORD(uParam);    // key flags   WHEEL_DELTA
            zDelta = (short) HIWORD(uParam);    // wheel rotation

            if (MK_SHIFT & fwKeys)
				if (zDelta >0 )
				    SendMessage(hWnd,WM_HSCROLL,SB_LINERIGHT,0);
				else
                    SendMessage(hWnd,WM_HSCROLL,SB_LINELEFT,0);
			else 
                if (zDelta >0 )
				    SendMessage(hWnd,WM_VSCROLL,SB_LINEUP,0);
				else
                    SendMessage(hWnd,WM_VSCROLL,SB_LINEDOWN,0);
           
	    	break;
//#endif       /* if (_WIN32_WINNT < 0x0400)  #define _WIN32_WINNT   0x0400   //chi  */	

	case	WM_LBUTTONDOWN:
	    	
			ButtonPressProcess (hWnd, message, uParam, lParam);
			break;

	case	WM_MOUSEMOVE:  
   ///////////////////////////////////////add chi //////////////////////////////  
		 
            UpParam = uParam ; LoParam = lParam; //Golbal variant; //chi
			ButtonMotionProcess (hWnd, message, uParam, lParam);
			break;

	case	WM_LBUTTONUP:
			ButtonReleaseProcess (hWnd, message, uParam, lParam);
            if (cur.bck.draw_type == DrwT_ZOOM)
			{  			
				if (uParam & MK_SHIFT) ZoomOut(hWnd, message, uParam, lParam);
				else ZoomIn(hWnd, message, uParam, lParam);
            } //END IF CUR

			break;

          /*  if (cur.bck.draw_type == DrwT_ZOOM){  
            cur.bck.rect.x2	= LOWORD(lParam);	
			cur.bck.rect.y2	= HIWORD(lParam);
			//if (( abs(cur.bck.rect.x1-cur.bck.rect.x2) <= POINT_OFFSET &&
				  //abs(cur.bck.rect.y1-cur.bck.rect.y2) <= POINT_OFFSET))
			{
				if (uParam & MK_SHIFT) ZoomOut(hWnd, message, uParam, lParam);
				else ZoomIn(hWnd, message, uParam, lParam);
			}
            } //END IF CUR   */

	case	WM_LBUTTONDBLCLK:
	  	
            ButtonDoubleClick( hWnd,  message,  uParam,  lParam);  

			if( uParam & MK_SHIFT )
			{
				if( drawing_mode = DRAWING_ON && cur.bck.draw_type == DrwT_PLINE)
					ShiftBtn1DoubleClick(hWnd, message, uParam, lParam);
			}
			if( uParam & MK_CONTROL )
				CtrlBtn1DoubleClick (hWnd, message, uParam, lParam);
			break;

	case	WM_RBUTTONDOWN:
		

			cur.bck.rect.x1	= LOWORD(lParam);	
			cur.bck.rect.y1	= HIWORD(lParam);
			break;

		  
	case	WM_MBUTTONUP:

	case	WM_RBUTTONUP:
		if (uParam & MK_CONTROL)
		{
			GetClientRect (hWnd, (LPRECT)&rc);
			LONG2POINT(lParam, pt);
			if (PtInRect ((LPRECT)&rc, pt))
				HandlePopupMenu (hWnd, pt);  
			break;
		}
		else
			SendMessage(  ghWndToolKit,  WM_COMMAND, IDB_SELECT,  0 );
		break;
		
	case	WM_CLOSE:
			if (MessageBox(hWnd, "是否退出?退出前请先进行文件保存!", "IPM:提示", MB_YESNOCANCEL|MB_ICONQUESTION) == IDYES)
			{	
				if(ghWndSymEdit) DestroyWindow (ghWndSymEdit);
				if(ghWndSymGrpEdit) DestroyWindow (ghWndSymGrpEdit);
				if(ghWndCGrpEdit) DestroyWindow (ghWndCGrpEdit);
				if(ghWndStrGrpEdit) DestroyWindow (ghWndStrGrpEdit); 
				DestroyWindow (ghWndToolKit);
				DestroyWindow ( hWnd );
			}
			else
				break;
 	
 	case	WM_DESTROY:  // message: window being destroyed
    //    	DestroyCaret();
			ExitProc();
			PostQuitMessage(0);
			break;


	case	WM_KEYDOWN:
    	switch (uParam)
		{
    	case	VK_DELETE:
    		DeleteGdbItems ();
			break;
        case VK_ESCAPE:
			if (cur.bck.draw_type != DrwT_SELECTION)
		    	SendMessage(  ghWndToolKit,  WM_COMMAND, IDB_SELECT,  0 );    //added chi
            break;
        case   VK_F5:
		   //UpdateWindow (hWnd) ;
           GetClientRect(hWnd, &rc); 
		   InvalidateRect(hWnd, &rc, TRUE); 
			//REPAINT 
			break;
        case   VK_F4:
            ToggleProc(ID_Grid_Visible) ;
			break;
        case VK_NEXT:
			ZoomOut(hWnd, message, UpParam, LoParam);
			break;
        case VK_PRIOR:
			ZoomIn(hWnd, message, UpParam, LoParam);
			break;
        case VK_UP:
            MoveItemsFinished(&v4_gdbase,MOD_ARROW,UP);
			DrawGdbuffItems (hWnd, k_drag_items, k_mv_items);
			GetClientRect(hWnd, &rc); 
			InvalidateRect(hWnd, &rc, TRUE);
		
			break;
		case VK_DOWN:
            MoveItemsFinished(&v4_gdbase,MOD_ARROW,DOWN);
			DrawGdbuffItems (hWnd, k_drag_items, k_mv_items);
			GetClientRect(hWnd, &rc); 
			InvalidateRect(hWnd, &rc, TRUE);
			break;
		case VK_RIGHT:
            MoveItemsFinished(&v4_gdbase,MOD_ARROW,RIGHT);
			DrawGdbuffItems (hWnd, k_drag_items, k_mv_items);
			GetClientRect(hWnd, &rc); 
			InvalidateRect(hWnd, &rc, TRUE);
			break;
		case VK_LEFT:
            MoveItemsFinished(&v4_gdbase,MOD_ARROW,LEFT);
			DrawGdbuffItems (hWnd, k_drag_items, k_mv_items);
			GetClientRect(hWnd, &rc); 
			InvalidateRect(hWnd, &rc, TRUE);
			break;
        default :  break ;
		}


	default:          // Passes it on if unproccessed
		return (DefWindowProc(hWnd, message, uParam, lParam));
	}
	return (0);
}


#include	"../inc/fill_bitmap.h"
/*
 *  Destroy brush, pen, bitmap etc resource, Free allocated memory. 
 */
void	ExitProc ()
{           
	int	i;

	/* free logical palette */
	if(ghPal) DeleteObject(ghPal);

	/* free fill pattern bitmap */
	for (i = 0; i < MAX_FILL_PATTERN; i++)
	{
		if(fill_pixmenu[i]) DeleteObject(fill_pixmenu[i]);
	}

	/* free font */
	for( i=0; i<MAX_FONTSET; i++)
	{
		if(font_set[i]) DeleteObject(font_set[i]);
		if(prt_font_set[i]) DeleteObject(prt_font_set[i]);
	}

	UnMountDB ();

}        


