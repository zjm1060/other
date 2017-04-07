
#include	"resource.h"

#define	COLOR_DEF_MODULE

#include	"../inc/ipm.h"
#include	"../inc/color_def.h"
#include	"../inc/draw_def.h"
#include	"../inc/toolkit.h"



extern	CHOOSECOLOR choose_color;
extern	COLORREF	color_define[16];


void ChangeItemsColor (int, int);


/* create a logical palette */
void	CreateColorPalette()
{
 	int	iLoop;
	HDC	hDCGlobal;		/* The Screen DC  */
	INT iRasterCaps, iNumColors;   	/* Raster capabilities   */
	NPLOGPALETTE    gpLogPal;       /* Pointer to program's logical palette        */

	hDCGlobal   = GetDC (NULL);
    iRasterCaps = GetDeviceCaps(hDCGlobal, RASTERCAPS);
    iRasterCaps = (iRasterCaps & RC_PALETTE) ? TRUE : FALSE;

     if (iRasterCaps)
     {
         iNumColors = GetDeviceCaps(hDCGlobal, SIZEPALETTE);
		 if (iNumColors < MAXCOLORS)
		 	MessageBox(NULL, "System Palette Entries cannot meet need.", "IPM", MB_OK | MB_ICONHAND);
     }
     else
     {
         iNumColors = GetDeviceCaps( hDCGlobal, NUMCOLORS);
//		 MessageBox(ghWnd, "Not Support System Palette.", "IPM", MB_OK | MB_ICONHAND);
//		 if (iNumColors < MAXCOLORS)
//		 	MessageBox(NULL, "Not Support System Palette and Colors cannot meet need.", "IPM", MB_OK | MB_ICONHAND);
     }
     ReleaseDC (NULL,hDCGlobal);

    if (iRasterCaps)
    {	
		gpLogPal = (NPLOGPALETTE) LocalAlloc (LMEM_FIXED,
                                            (sizeof (LOGPALETTE) +
                                            (sizeof (PALETTEENTRY) * (PALETTESIZE))));
        if(!gpLogPal){
               MessageBox(NULL, "Not enough memory for palette.", NULL, MB_OK | MB_ICONHAND);
			   ExitProc();
               PostQuitMessage (0) ;
               return;
        }

        gpLogPal->palVersion    = 0x300;
        gpLogPal->palNumEntries = PALETTESIZE;

        /* fill in intensities for all palette entry colors */
        for (iLoop = 0; iLoop < MAXCOLORS; iLoop++) {
              *((WORD *) (&gpLogPal->palPalEntry[iLoop].peRed)) = (WORD)rgb_value[iLoop].rred;
              *((WORD *) (&gpLogPal->palPalEntry[iLoop].peGreen))  = (WORD)rgb_value[iLoop].ggreen;
              *((WORD *) (&gpLogPal->palPalEntry[iLoop].peBlue))  = (WORD)rgb_value[iLoop].bblue;
			  gpLogPal->palPalEntry[iLoop].peFlags = 0;
        }

        /*  create a logical color palette according the information
         *  in the LOGPALETTE structure.
         */
        ghPal = CreatePalette ((LPLOGPALETTE) gpLogPal) ;
		if(gpLogPal) LocalFree(gpLogPal);
	}
	else
    	ghPal = NULL;

}            


/*modified by wzg 20060417
int Colors(int	i)
{
	if (ghPal) return( PALETTEINDEX(i) );
	else return( RGB(rgb_value[i].rred, rgb_value[i].ggreen, rgb_value[i].bblue) );
}
*/


int Colors(int id)
{
	return RGB( (id&0x1F)<<3, ((id>>5)&0x1F)<<3, ((id>>10)&0x1F)<<3 );
}

int GetColorId(COLORREF rgb)
{
	return  ((rgb&0xF8)>>3) | ((rgb&0xF800)>>6) | ((rgb&0xF80000)>>9);
}

int ColorId3to4(int id3)
{
    if (id3 > 159 || id3 < 0  ) return  0 ;
	if (ghPal) 
		return GetColorId( PALETTEINDEX(id3) );
	return GetColorId(RGB(rgb_value[id3].rred, rgb_value[id3].ggreen, rgb_value[id3].bblue));
}



void	SetColor ( fb_choice, color_num)
int	fb_choice, color_num;
{
	HWND	hbtn;
	RECT	rc;
	HBRUSH	hBrush, hbr;
	HDC		hdc;

	if (gdbuff.nextpos == 0) /* no item selected, change foreground and background color*/
	{
		switch (fb_choice)
		{
		case	k_bg:
			cur.bck.bg	=color_num;
        	hbtn = GetDlgItem(ghWndToolKit,IDB_BGBTN);
        	GetClientRect(hbtn, &rc);
			hdc = GetDC(hbtn);
    		if(ghPal)
    		{
    			SelectPalette (hdc, ghPal, FALSE);
    			RealizePalette (hdc);
			}
			hBrush = CreateSolidBrush ( Colors (color_num) );
			hbr = GetStockObject(BLACK_BRUSH);
			FillRect(hdc, &rc, hBrush);
        	FrameRect(hdc, (LPRECT)&rc, hbr);
			ReleaseDC(hbtn, hdc);
			DeleteObject(hBrush);
			DeleteObject(hbr);
			break;

		default:
			cur.bck.fg	=color_num;
        	hbtn = GetDlgItem(ghWndToolKit,IDB_FGBTN);
        	GetClientRect(hbtn, &rc);
			hdc = GetDC(hbtn);
    		if(ghPal)
    		{
    			SelectPalette (hdc, ghPal, FALSE);
    			RealizePalette (hdc);
			}
			hBrush = CreateSolidBrush ( Colors (color_num) );
			hbr = GetStockObject(BLACK_BRUSH);
			FillRect(hdc, &rc, hBrush);
        	FrameRect(hdc, (LPRECT)&rc, hbr);
			ReleaseDC(hbtn, hdc);
			DeleteObject(hBrush);
			DeleteObject(hbr);
			break;

		}
	}
	else		/* items selected, change item's color */
	{
		switch (fb_choice)
		{
		case	k_bg:
			ChangeItemsColor (k_bg, color_num);
			break;

		case	k_fg:
			ChangeItemsColor (k_fg, color_num);
			break;

		default:
			break;
		}

	}

}


BOOL ColorChoiceFlag;  //choice color in PaletteDlg or not
BOOL CALLBACK PaletteDlgProc(HWND, UINT, WPARAM, LONG);
void SetCanvasBgColor ()
{
	/*modified by wzg 20060417
	 
*/		
	choose_color.lStructSize= sizeof(choose_color);
	choose_color.hwndOwner= ghWnd;
	choose_color.hInstance= NULL;
	choose_color.rgbResult= canvas_bg_color;
	choose_color.lpCustColors= color_define;
	choose_color.Flags= CC_RGBINIT|CC_ANYCOLOR;
	choose_color.lCustData= 0;
	choose_color.lpfnHook= NULL;
	choose_color.lpTemplateName= 0;
	if(ChooseColor(&choose_color))
	{
		if(canvas_bg_color != GetColorId(choose_color.rgbResult))
		{
			canvas_bg_color	= GetColorId(choose_color.rgbResult);
			InvalidateRect(ghWnd, NULL, TRUE);
		}
	}
}

void ChangeItemsColor (fg, color)
int	fg, color;
{
	int	i;

	for (i=0; i<v4_gdbase.nextpos; i++)
	{
		if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
		{
			if (fg == k_fg)
			{ v4_gdbase.buffer[i].bck.fg = color; }
			else if (fg == k_bg)
			{ v4_gdbase.buffer[i].bck.bg = color; }
		}
	}
	InvalidateRect(ghWnd, NULL, TRUE);

}

