
#include <locale.h>
#include <windows.h>

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"  
#include	"../inc/text_edit_def.h"
#include	"../inc/color_def.h"

BOOL CALLBACK TextProc(HWND, UINT, WPARAM, LPARAM);

void TextFinished(int * );
void StoreObject ();
void GetRcdCoordFromBaseline(  HWND, BACKRCD *, int, int);
void GetRcdVerticHW(  HWND, BACKRCD * , int, int );


void	PopupTextInputDlg()
{

	DialogBox( ghInst, MAKEINTRESOURCE(DLG_TEXT_INPUT),
				 ghWnd, (DLGPROC)TextProc );

}

BOOL CALLBACK TextProc(hDlg, message, wParam, lParam)
HWND	hDlg;
UINT	message;
WPARAM	wParam;
LPARAM	lParam;
{
	HDC	hDC;
    HWND hText,hRadio;
	static int AlignMode ; //0 --- parallel  1--- vertic
	switch (message)
	{
		case WM_INITDIALOG:
			if(cur.bck.draw_type == DrwT_MODIFY_TEXT)
				SetDlgItemText(hDlg, IDC_TEXT, gdbuff.buffer[gdbuff.nextpos-1].bck.data.text.text);

			hText   = GetDlgItem(hDlg, IDC_TEXT);
            hRadio = GetDlgItem(hDlg, IDC_PAREL);  //chi 
			SendMessage (hRadio, BM_SETCHECK, 1, 0) ;  //chi
			AlignMode =0 ;
            SetFocus(hText);
			CenterWindow(hDlg, ghWnd);
			return TRUE;
				
		case WM_COMMAND:

			switch( LOWORD(wParam) )
			{
				case IDOK:
					memset(text_buff, 0, sizeof(text_buff));
					GetDlgItemText(hDlg, IDC_TEXT, text_buff, sizeof(text_buff));
					if ( cur.bck.draw_type == DrwT_TEXT)
					{
						strcpy(cur.bck.data.text.text, text_buff);
						if (AlignMode == 0)  //parallel
						    GetRcdCoordFromBaseline(ghWnd, &cur.bck, start_x, start_y );
						else  //verticall 
						{
							// to do here chi!!
							cur.bck.data.text.DirOut = ITEM_TAG_VERTI ;
                            GetRcdVerticHW(ghWnd,&cur.bck,start_x,start_y);

						}						    
							
						hDC  = GetDC(ghWnd);
						if(ghPal) 
						{
							SelectPalette (hDC, ghPal, FALSE);
    						RealizePalette (hDC);
						}
						DrawRcd (ghWnd, hDC, &cur.bck);
						ReleaseDC(ghWnd, hDC);
					}
					else if ( cur.bck.draw_type == DrwT_MODIFY_TEXT)
					{
						strcpy(gdbuff.buffer[gdbuff.nextpos-1].bck.data.text.text, text_buff);
                       /////////////////////////////////////////////////
						start_x = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.x1;
                        start_y = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.y1;

						if (AlignMode == 0)  //parallel
						{   
							gdbuff.buffer[gdbuff.nextpos-1].bck.data.text.DirOut = 0 ;
						    GetRcdCoordFromBaseline(ghWnd, &gdbuff.buffer[gdbuff.nextpos-1].bck, start_x, start_y );
						}
						else  //verticall 
						{
							// to do here chi!!
							gdbuff.buffer[gdbuff.nextpos-1].bck.data.text.DirOut = ITEM_TAG_VERTI ;
                            GetRcdVerticHW(ghWnd,&gdbuff.buffer[gdbuff.nextpos-1].bck,start_x,start_y);

						}
						////////////////////////////////////
						start_x = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.x1;
						start_y = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.y1;

						hDC  = GetDC(ghWnd);
						DrawRcd (ghWnd, hDC, &gdbuff.buffer[gdbuff.nextpos-1].bck);
						ReleaseDC(ghWnd, hDC);
					}
					TextFinished((int *)&cur.bck.draw_type);
					EndDialog(hDlg, wParam);
					return TRUE;
				case IDCANCEL:
					if( cur.bck.draw_type == DrwT_MODIFY_TEXT )	cur.bck.draw_type = DrwT_SELECTION;
					gdbuff.nextpos = 0;
					EndDialog(hDlg, wParam);
					return TRUE;
		    	case IDC_PAREL:
                    AlignMode =0 ; //parallel
				  	return TRUE;
		    	case IDC_VERTI:
                    AlignMode =1 ;  //vertical
					return TRUE;				
			}
			break;

		default:
			break;
	}
	return FALSE;
}



int		CheckCursorPt();


void	KeyPressProcess (HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam)
{
//        char    theKeyBuffer[MAX_TEXT_LEN+5];
//        char    string[MAX_TEXT_LEN+5];
	int	i;
	TCHAR	ch;
	HFONT	hOldFont;
	HDC	hDC;

    if (( drawing_mode == DRAWING_ON) && (cur.bck.draw_type == DrwT_TEXT)
		|| ( cur.bck.draw_type == DrwT_MODIFY_TEXT))
    {
		switch (message) 
		{
		case 	WM_CHAR:
			switch(uParam)
			{
				case 0x08: 	/* backspace */
				case 0x0A:	/* linefeed */
				case 0x1B: 	/* escape */
					MessageBeep(0xFFFFFFFF);
					return;

				case 0x09:	/* tab */
					/* Convert tabs to four consecutive spaces. */
					for (i = 0; i < 4; i++)
						SendMessage(hwnd, WM_CHAR, 0x20, 0);
					return;

				case 0x0d: 	/* carriage return */
					break;

				default:	/* displayable character */
					ch = (TCHAR) uParam;
					HideCaret(hwnd);

					hDC = GetDC(hwnd);
					if(ghPal) 
					{
						SelectPalette (ghDC, ghPal, FALSE);
    					RealizePalette (ghDC);
					}
					/* Retrieve the character's width and output the character. */
					hOldFont = 	SelectObject(ghDC, font_set[cur.bck.ftset_id]);
//            		SetTextAlign(hDC, TA_BASELINE | TA_LEFT | TA_UPDATECP);
					SetTextColor(ghDC, Colors (cur.bck.fg));
					SetBkColor(ghDC, Colors (cur.bck.bg));
            		TextOut(ghDC, start_x, start_y, &ch, 1);
            		//
            		// Updating current position
        		    //
       			    {
					int	nCharWidth;

					GetCharWidth(ghDC, (UINT)uParam, (UINT)uParam, &nCharWidth);
					start_x += nCharWidth;
                	SetCaretPos(start_x, start_y);
            		}
            		ShowCaret(hwnd);
					SelectObject(ghDC, hOldFont);			 
					ReleaseDC(hwnd, hDC);
            		break;
			} 
        	break;

		case 	WM_KEYDOWN: 
        	break;
		}
    }
	else
		return;


}


void TextFinished(draw_type )
int	*draw_type;
{

	if ( *draw_type == DrwT_TEXT)
	{
//		DrawCharCursor ( dpy, window, &cur.bck, cur.bck.rect.x2);
		drawing_mode    =DRAWING_OFF;
		strcpy (cur.bck.data.text.text, text_buff);
 		StoreObject();
		cur.bck.data.text.text[0]        ='\0';
		text_buff[0]            ='\0';
	}
	else if( *draw_type == DrwT_MODIFY_TEXT && (drawing_mode == DRAWING_ON))
	{
		int     i;
		RECT	rc;

		rc.left = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.x1;
		rc.top =  gdbuff.buffer[gdbuff.nextpos-1].bck.rect.y1;
		rc.right = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.x2;
		rc.bottom = gdbuff.buffer[gdbuff.nextpos-1].bck.rect.y2;

	//	GetRcdCoordFromBaseline(ghWnd, &gdbuff.buffer[gdbuff.nextpos-1].bck, start_x, start_y );

		for ( i=0; i < v4_gdbase.nextpos; i++)
		{
			if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
			{
				v4_gdbase.buffer[i] = gdbuff.buffer[gdbuff.nextpos-1];
//				DrawCharCursor ( dpy, window, &v4_gdbase.buffer[i].bck, modify_x);
//				ClearTagItems ( &v4_gdbase.buffer[i].bck );
				v4_gdbase.buffer[i].bck.tag = ITEM_TAG_OFF;
				text_buff[0]='\0';
				gdbuff.buffer[gdbuff.nextpos-1].bck.data.text.text[0]='\0';
				gdbuff.nextpos = 0;
				drawing_mode    =DRAWING_OFF;
			}
		}

		InvalidateRect(ghWnd, &rc, TRUE);	// rect is client coordinate
		*draw_type= DrwT_SELECTION;
	}
	
}

/*
DrawTextItems( w,  string, count,   key_input_mode)
Widget  w;
char    *string;
int     count;
int	key_input_mode;
{
	int		mb_len;
	Display         *dpy=XtDisplay(w);
	Window          window  =XtWindow(w);

	if ( key_input_mode == INSERT)
	{
		DrawCharCursor ( dpy, window, &cur.bck, cur.bck.rect.x2  );
		strcat(text_buff, string);
		strcpy( cur.bck.data.text, text_buff);
		GetRcdCoordFromBaseline(  &cur.bck, start_x, start_y );
		DrawRcd ( dpy, window, info.gc, &cur.bck ); 
		DrawCharCursor ( dpy, window, &cur.bck, cur.bck.rect.x2 );
	}
	else if ( key_input_mode == DELETE )
	{
		DrawCharCursor ( dpy, window, &cur.bck, cur.bck.rect.x2 );
		DrawRcd ( dpy, window, gctxt, &cur.bck );
		text_buff_len = strlen(text_buff);
		if ( text_buff_len > 0)
		{
			CheckPrevCharNum ( text_buff, text_buff_len, &mb_len );
			if ( mb_len == 1)
				text_buff_len--;
			else
			{	
				text_buff_len= text_buff_len-2;
			}
			text_buff[text_buff_len]='\0';
			strcpy ( cur.bck.data.text, text_buff);
			GetRcdCoordFromBaseline (  &cur.bck, start_x, start_y );
			DrawRcd ( dpy, window, info.gc, &cur.bck );
			DrawCharCursor ( dpy, window, &cur.bck, cur.bck.rect.x2 );
		}		
	}
}		

ModifyGdbuffItems( w,  string, count,  k_modify_mode )
Widget  w;
char    *string;
int     count;
int	k_modify_mode;
{
	Display         *dpy	= XtDisplay(w);
	Window          window  = XtWindow(w);
	BACKRCD		rcd;
	int		i;
	int		mb_len;
	int		char_width;
	char		prev_buff[MAX_TEXT_LEN];
	int		prev_buff_len;
	int		modify_xn;
	int		x1, width_new;
	int		bsl_x, bsl_y;

	rcd     = gdbuff.buffer[gdbuff.nextpos-1].bck;
	strcpy ( text_buff, rcd.data.text);	
	text_buff_len = strlen(text_buff);
	x1	= rcd.rect.x1;
	DrawHandle ( w, &rcd);
	GetBaselineCoordFromRcd ( &rcd, &bsl_x, &bsl_y);	
	DrawRcd ( dpy, window, gctxt, &rcd);
	DrawCharCursor ( dpy, window, &rcd, modify_x);

	if( k_modify_mode == INSERT)
	{
		for(i = text_buff_len-1; i >= modify_loop; i--)
		{
			text_buff[i+count] = text_buff[i];
		}
		for (i=0; i<count; i++)
		{
			text_buff[modify_loop + i] = string[i];
		}
		text_buff_len += count;
		modify_loop += count;
		strcpy( prev_buff, text_buff);
		prev_buff[modify_loop] ='\0';
	}
	else if( k_modify_mode == DELETE)
	{
		strcpy( prev_buff, text_buff);
		prev_buff[modify_loop]='\0';
		for( i=modify_loop; i< text_buff_len; i++)
		{
			if((modify_loop != 1) && (text_buff_len > 0))
			{
				CheckPrevCharNum( prev_buff, strlen(prev_buff), &mb_len);
				if(mb_len == 1)
				{
					text_buff[i-1] = text_buff[i];
				}
				else
				{
					text_buff[i-2] = text_buff[i];
				}
			}	
			else
			{
				printf("no character cah be delete\n");
				
			}
		}
		
		if(mb_len == 1)
		{
			text_buff_len--;
			modify_loop--;
		}
		else
		{
			modify_loop = modify_loop-2;
			text_buff_len = text_buff_len -2;
		}
		prev_buff[modify_loop] ='\0';
	}
	text_buff[text_buff_len] ='\0';
	strcpy ( rcd.data.text, text_buff);
	CheckPrevCharWidth ( font_set[rcd.ftset_id], prev_buff, modify_loop, &width_new);
	modify_x 	= x1+width_new;
	GetRcdCoordFromBaseline ( &rcd, bsl_x, bsl_y );
	DrawRcd ( dpy, window, info.gc, &rcd);
	DrawCharCursor ( dpy, window, &rcd, modify_x);	
	DrawHandle ( w, &rcd);
	gdbuff.buffer[gdbuff.nextpos-1].bck	= rcd;
}

CheckPrevCharNum ( buff, buff_len, mb_len)
char	buff[MAX_TEXT_LEN+5];
int	buff_len;
int	*mb_len;
{
	*mb_len= mblen( &(buff[buff_len-1]), MAX_BYTES_I18N_CHAR);
}

CheckPrevCharWidth ( font_set, string, count, width)
XFontSet	font_set;
char		*string;
int		count;
int		*width;
{
	XRectangle      ovl_ink, ovl_lgc;
	int		npx;

	npx = XmbTextExtents( font_set, string, count, &ovl_ink,  &ovl_lgc);
	*width = ovl_lgc.width;
}

GetDefaultFontSize(width, height)
int	*width;
int	*height;
{
	XRectangle	ovl_ink, ovl_lgc;
	int		npx;
	char	*string = {"e"};

	npx 	= XmbTextExtents ( DEFAULT_FONT, string, 1, &ovl_ink,  &ovl_lgc);
	*width	= ovl_lgc.width;
	*height	= ovl_lgc.height;
}



TextModify ( dpy, window, rcd)
BACKRCD	*rcd;
Display *dpy;
Window  window;
{
	int	x1, lenth, prev_width, begin_x, cursor_x;
	XFontSet	fontset;

	x1	= rcd->rect.x1;
	strcpy( text_buff , rcd->data.text);
	lenth	= strlen(text_buff);
	fontset	= font_set[rcd->ftset_id];		
	begin_x = XtoSX( x1);
	cursor_x = XtoSX( cur.bck.rect.x1);
	modify_loop 	= CheckCursorPt( text_buff,  lenth, begin_x, &fontset, &cursor_x, &prev_width );
	modify_x = x1+prev_width; 
	DrawCharCursor ( dpy, window, rcd, modify_x);	

}

int CheckCursorPt ( buff, length, start_x, fontset, curs_x, prev_width)
char            *buff;
int		length, start_x, *curs_x, *prev_width;
XFontSet        *fontset;
{
	int	loop, temp, prev_x, nb, total_nb, prev_nb;
	XRectangle      ovl_ink, ovl_lgc;

	total_nb = prev_nb = 0;
	*prev_width = prev_x = start_x;
	if (length == 0)
	{
		*curs_x = start_x;
		loop = 0;
	}
	 else
	{
		for (loop = 0; loop < length; )
		{
			nb = mblen(&(buff[loop]), MAX_BYTES_I18N_CHAR);
			if (nb < 0)
			{
				printf("find_curspos_wsipe: invlid character %hx encountered !\n", buff[loop]);
				buff[loop] = '?';
				nb = 1;
			}
			total_nb += nb;
			temp = XmbTextExtents(*fontset, buff, total_nb, &ovl_ink, &ovl_lgc);

			if (loop ==0)
			{
				temp = ovl_lgc.width;
			}
			else
			{
				temp = ovl_lgc.width - *prev_width;
			}
			if (*curs_x < (start_x + ovl_lgc.width))
			{
				if ((*curs_x - prev_x) < (temp/2))
				{
					*curs_x = prev_x;
				}
				else
				{
					*curs_x = start_x + ovl_lgc.width;
					loop += nb;
				}
				break;
			}
		loop += nb;
		prev_nb = nb;
		*prev_width = ovl_lgc.width;
		prev_x = start_x + ovl_lgc.width;
		}
	}
	return (loop);
}
*/

void GetBaselineCoordFromRcd ( hwnd, rcd, bsl_x, bsl_y)
HWND	hwnd;
BACKRCD		*rcd; 
int		*bsl_x, *bsl_y;
{
	SIZE	size;
	char	buffer[MAX_TEXT_LEN +5];
	HDC	hdc;
	HFONT	hOldFont;

	strcpy( buffer, rcd->data.text.text);
	hdc = GetDC(hwnd);
	hOldFont = 	SelectObject(hdc, font_set[rcd->ftset_id]);
	GetTextExtentPoint( hdc, buffer, strlen( buffer),  &size);
	SelectObject(hdc, hOldFont);
	*bsl_x = rcd->rect.x1;
	*bsl_y = rcd->rect.y1;
}

//get heigth and width of parallel text string ;; 

void GetRcdCoordFromBaseline(  hwnd, rcd, bsl_x, bsl_y)
HWND	hwnd;
int		bsl_x, bsl_y; 
BACKRCD		*rcd;
{ 
	SIZE	size;
	char            buffer[MAX_TEXT_LEN+5];
	HDC	hdc;
	HFONT	hOldFont;
		
	strcpy( buffer, rcd->data.text.text);
	hdc = GetDC(hwnd);
	hOldFont = 	SelectObject(hdc, font_set[rcd->ftset_id]);
	GetTextExtentPoint( hdc, buffer, strlen( buffer),  &size);
	SelectObject(hdc, hOldFont);
	rcd->rect.x1 = bsl_x;
	rcd->rect.x2 = rcd->rect.x1+size.cx;
	rcd->rect.y1 = bsl_y;
	rcd->rect.y2 = rcd->rect.y1+size.cy; 
}

//  get heigth and width of parallel text string ;;  added by chi 

void GetRcdVerticHW(  hwnd, rcd, bsl_x, bsl_y)
HWND	hwnd;
int		bsl_x, bsl_y; 
BACKRCD		*rcd;
{ 
	char            buffer[MAX_TEXT_LEN+5];
	HDC	hdc;
	HFONT	hOldFont;
	int cyChar,cxChar,iLen ;
	TEXTMETRIC tm;

		
	strcpy( buffer, rcd->data.text.text);
	hdc = GetDC(hwnd);
	hOldFont = 	SelectObject(hdc, font_set[rcd->ftset_id]);



	GetTextMetrics (hdc, &tm) ;    
	cyChar = tm.tmHeight + tm.tmExternalLeading ;
    cxChar = tm.tmHeight ;


    iLen = _lstrlen(  buffer ); 
	//GetTextExtentPoint( hdc, buffer, strlen( buffer),  &size);
	SelectObject(hdc, hOldFont);
	rcd->rect.x1 = bsl_x;
    rcd->rect.y1 = bsl_y;
	rcd->rect.x2 = rcd->rect.x1+cxChar;	
	rcd->rect.y2 = rcd->rect.y1+iLen*cyChar; 
}


/*
DrawCharCursor ( dpy, window, rcd, cursor_x)
Display		*dpy;
Window		window;
BACKRCD		*rcd;
int		cursor_x;
{
	int	bsl_x, bsl_y, sx, sy,width;

	GetBaselineCoordFromRcd ( rcd, &bsl_x, &bsl_y);
	width	= cursor_x-bsl_x;
	sx 	= XtoSX( bsl_x )+ width;
	sy 	= YtoSY( bsl_y );
	XDrawLine ( dpy, window, info.xorgc, sx, sy, sx, sy-20); 	
}



void    CreateXim()
{
        XIMStyles       *xim_styles;
        XIMStyle        best_style      =XIMPreeditCallbacks;
        char            *locale 	= NULL;
        XrmDatabase     rdb 		= NULL;
        XIMStyle        app_supported_styles;
        char            *lmods;
	int		i, n;
	int		font_num;
        
        xim_mode =0;
	// Set the locale for the appication 
        printf( "locale = %s\n", setlocale( LC_CTYPE, "" ));

        if( !XSupportsLocale() )  printf("X does not support this locale\n");

        lmods = XSetLocaleModifiers( "" );
        if ( lmods == NULL )  printf("XSetLocaleModifiers failed\N");
        printf("Locale Modifiers = %s\n", lmods );

        if (locale == NULL) locale = DEFAULT_LOCALE;

        if ( setlocale(LC_CTYPE, locale) == NULL)
        {
                printf( "Error: setlocale() !\n");
                exit(0);
        }

        if (!XSupportsLocale())
        {
                printf( "X does not support this locale");
                exit(1);

        }
	printf("in CreateXim\n");
	for(font_num=0; font_num<MAX_FONTSET; font_num++)
	{
		printf ("CreateFontSet: %d\n", font_num);
		SetFont(font_num);
	}
	SetFont( DEFAULT_FONT);
	if ((xim = XOpenIM(dpy, rdb, NULL, NULL)) == NULL)
        { printf("Error : XOpenIM() !\n"); exit(0); }

        // set flags for the styles our application can support 
        app_supported_styles = XIMPreeditNone | XIMPreeditNothing | XIMPreeditCallbacks;
        app_supported_styles |= XIMStatusNone | XIMStatusNothing;
        XGetIMValues(xim, XNQueryInputStyle, &xim_styles, NULL);
        n       =1;
        if (xim_styles != (XIMStyles *)NULL)
        {
                for (i=0; i < xim_styles->count_styles; i++)
                {
                        xim_mode = xim_styles->supported_styles[i];
                        if ((xim_mode & app_supported_styles) == xim_mode)
                        {
                                  n = 0;
                                if (xim_mode & best_style)      // pick user selected style 
                                break;
                        }
                }
        }

        if (n) { printf("warning : Unsupport InputStyle. or No IMserver.\n");  exit (0); }

}



CreateXic (root, client,  dpy)
Widget  root, client;
Display *dpy;
{
        unsigned long   mask;
        Window  app_window, view_w;

        if ( xim != NULL )
        {
                app_window      = XtWindow(root);
                view_w          = XtWindow(client);

                // Create the XIM input context 

                xic     =XCreateIC( xim,
                        XNInputStyle, xim_mode,
                        XNClientWindow,app_window,
                        XNFocusWindow, view_w,
                        NULL );

                if ( xic == NULL )
                { printf("XOpenIC failed \n"); }
                else
                {
                        XGetICValues( xic, XNFilterEvents, &im_ev_mask, NULL );
                        mask = ExposureMask |  FocusChangeMask | im_ev_mask | //KeyPressMask |
                                ButtonPressMask | ButtonReleaseMask | Button1MotionMask
                                | Button3MotionMask
                                | KeymapStateMask | KeyReleaseMask ;
                        XSelectInput( dpy, view_w, mask);
                        XSetICFocus (xic);
                }
        }

}

*/


