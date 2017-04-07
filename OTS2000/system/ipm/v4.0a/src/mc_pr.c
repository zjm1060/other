

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <winspool.h>
#include <commdlg.h>

#include "../inc/ipm.h"

#include "../inc/pr.h"
#include "../inc/scaler.h"
#include "../inc/text_edit_def.h"
#include "../inc/color_def.h"

BOOL	CreatePrtLogFont(float);

void Print ()
{
	int	 width, height, bgcolorsave;
	float	 xscalesave,yscalesave,xprtfontscale,yprtfontscale,fLogPelsX1,fLogPelsY1,fLogPelsX2,fLogPelsY2;
  	DOCINFO di;
	HDC		hdc;
	HANDLE	palsave;

  	SetMapMode (prtdc, MM_TEXT);

	bgcolorsave = canvas_bg_color;
  	xscalesave = xscale;
  	yscalesave = yscale;
	palsave = ghPal;
	ghPal = NULL;

	canvas_bg_color = WHITE;						//	白色
 	width     = GetDeviceCaps (prtdc, HORZRES);		//	检取有关给定设备的设备特定信息
  	height    = GetDeviceCaps (prtdc, VERTRES) - 200;
	xscale = (float)width/(float)OLDWIDTH;
	yscale = (float)height/(float)OLDHEIGHT;

	hdc = GetDC(ghWnd);						//	检取给定窗口显示设备描述表	
	fLogPelsX1 = (float)GetDeviceCaps (hdc, LOGPIXELSX);
	fLogPelsY1 = (float)GetDeviceCaps (hdc, LOGPIXELSY);
	ReleaseDC(ghWnd, hdc);
	fLogPelsX2 = (float)GetDeviceCaps (prtdc, LOGPIXELSX);
	fLogPelsY2 = (float)GetDeviceCaps (prtdc, LOGPIXELSY);
	xprtfontscale = fLogPelsX2/fLogPelsX1;
	yprtfontscale = fLogPelsY2/fLogPelsY1;
	if(CreatePrtLogFont(yprtfontscale) == FALSE)	//	创建具有指定特征的逻辑字体	
	{
		 
		MessageBox (ghWnd, "Can't Create Fitable Printer font", "IPM", MB_OK);
	}		

	if(ghPal) 
	{
		SelectPalette (prtdc, ghPal, FALSE);
    	RealizePalette (prtdc);
	}

  	di.cbSize      = sizeof(DOCINFO);
  	di.lpszDocName = "print dsp";
  	di.lpszOutput  = NULL;

	toprint = TOPRINTON;

  	StartDoc  (prtdc, &di);		//	启动打印作业
  	StartPage (prtdc);			//	打印机驱动程序准备接收数据
	Redraw(ghWnd, prtdc);

  	EndPage   (prtdc);			//	换页码
  	EndDoc    (prtdc);			//	终止打印作业
 // DeleteDC  (prtdc);
	toprint = TOPRINTOFF;
	canvas_bg_color = bgcolorsave;
	xscale = xscalesave;
	yscale = yscalesave;
	ghPal = palsave;
}


BOOL	CreatePrtLogFont(float yprtfontscale)
{
	int		font_id, height;
	
	for(font_id=0; font_id<MAX_FONTSET; font_id++)
	{
		height = (int)(font_size[font_id].height*yprtfontscale);
		prt_font_set[font_id] = CreateFont(height, 0,
										0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
										OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY,
										VARIABLE_PITCH | FF_DONTCARE, "RS_Song");	//RS_Song  RS_FangSong RS_Kai 宋体

		if( prt_font_set[font_id] == NULL) 
		{
			
			MessageBox (ghWnd, "Create Printer font set error", "IPM", MB_OK);
            return(FALSE);
		}
	}
	return(TRUE);
}


