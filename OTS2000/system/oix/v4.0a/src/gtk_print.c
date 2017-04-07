#include <windows.h>
#include <stdio.h>

#include <string.h>
#include <winspool.h>
#include <commdlg.h>
#pragma	comment(lib, "user32.lib")
#pragma	comment(lib, "gdi32.lib")

#include	"../inc/gtk_ipm_k_def.h"
#include	"../inc/gtk_widgets.h"
#include	"../inc/oix.h"



/**************win print pic******/
//#define BUFSIZE 256
#define TOPRINTON 1
#define TOPRINTOFF 0
typedef	struct
{
	int	width;
	int	height;
}	FontSize;
FontSize	font_size[MAX_FONTSET] = {
	{8, 15},
	{13, 15},
	{14, 18},
	{19, 24},
	{26, 24}
};  	 //	宋体  1024*768 win95
int	print_bg_color	=0;// BLACK
HANDLE          ghPal;          /* Handle to the application's logical palette */

//extern HDC    prtdc;/*winprint pic*/
int 	toprint;	// flag 
HFONT	prt_font_set[MAX_FONTSET];	 //used by printer
/*********************************/
BOOL	CreatePrtLogFont(float);

char  bmp_filename[100] ;
HANDLE hbmp = NULL;
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
char	temp_str_time_eg[MAX_TIME_LEN];
char	part_sft_cp_fn[MAX_FILE_NAME_LEN*2]	= {""};
/*time 20071220_173020*/
char* get_cur_time()
{
	struct tm *sftcp_time;
	time_t t;
	long	tloc;
	//char str_time[30];
	
	//char	temp_str_time_eg[MAX_TIME_LEN];
	char	temp_str_time[MAX_TIME_LEN];
	
	t	 = time (&tloc);
	sftcp_time = localtime (&t);
	
	sprintf(temp_str_time_eg, "%04d%02d%02d",sftcp_time->tm_year + 1900,sftcp_time->tm_mon+1,sftcp_time->tm_mday);
	sprintf(temp_str_time,"_%02d%02d%02d_",sftcp_time->tm_hour,sftcp_time->tm_min,sftcp_time->tm_sec);
	strcat(temp_str_time_eg,temp_str_time);
	
	printf("temp_str_time_eg=%s\n",temp_str_time_eg);
	return temp_str_time_eg;
}

char* get_temp_file_name(int crt_n)
{
	char path[100];
	char	sft_cp_fn[MAX_FILE_NAME_LEN*2]= {""}		;
	char	sft_cp_fn_tmp[MAX_FILE_NAME_LEN*2]= {""}	;
	char    tmp[MAX_FILE_NAME_LEN*2]	= {""};
	int	len, len_1, len_2;
  //  char	part_sft_cp_fn[MAX_FILE_NAME_LEN*2]	= {""};
	char	delt_part[10]				= {""};
	
	
	strcpy(path,PATH_PRINT_TEMP_PIC);
	strcpy (sft_cp_fn_tmp, strrchr(cur_dsp_name[crt_n], PATH_PIPE));
	len_1 	= strlen(sft_cp_fn_tmp);
	
	strcpy (delt_part, strrchr(sft_cp_fn_tmp, '.'));	
	len_2 	= strlen(delt_part);
	len 	= len_1 - len_2;
	strncpy (sft_cp_fn, sft_cp_fn_tmp+1, len-1); 		
	
	strcpy( str_time_eg,get_cur_time());
	strcpy(tmp,str_time_eg);
	strcat(tmp,sft_cp_fn);
	
	
	strcpy (part_sft_cp_fn, path);
	
	strcat(part_sft_cp_fn, tmp);
	strcat (part_sft_cp_fn, ".jpg");
	return part_sft_cp_fn;
	
}

int get_line_height(HDC hdc)
{   //return text line height
    //return value < 0 if map mode is not MM_TEXT, so that you can always use
    //y+= line_height when drawing to device.
    int map_mode = GetMapMode(hdc);
	int h;
	TEXTMETRIC tm;
	GetTextMetrics(hdc,&tm);
	
	h = tm.tmHeight + tm.tmExternalLeading;
	if(MM_TEXT != map_mode)
		h = 0 - h;
	return h;
}
double get_screen_pixel_width(int map_mode)
{   //purpose: a pixel displayed in MM_TEXT mode should be about the same size
    //when displayed in other map mode.
	HDC hdc = GetDC(NULL);
	double hroz_size = GetDeviceCaps(hdc,HORZSIZE);//Width, in millimeters, of the physical screen.
	double horz_res = GetDeviceCaps(hdc, HORZRES);//Width, in pixels, of the screen.
	double pixel_width = hroz_size / horz_res; // Width, in millimeters
	// 1 inch = 25.4 mm
	const double INCH_TO_MM = 25.4;
	//const double INCH_TO_TWIP = 1280;
	const double INCH_TO_TWIP = STND_WIDTH;
	switch(map_mode)
	{
	case MM_LOMETRIC:
		pixel_width *= 10;   
		break;
	case MM_HIMETRIC:
		pixel_width *= 100;
		break;
	case MM_TEXT:
		break;
	case MM_LOENGLISH: //Each logical unit is mapped to 0.01 inch
		pixel_width = pixel_width / INCH_TO_MM * 100;
		break;
	case MM_HIENGLISH: //Each logical unit is mapped to 0.001 inch
		pixel_width = pixel_width / INCH_TO_MM * 1000;
		break;
	case MM_TWIPS: //Each logical unit is mapped to one twentieth of a printer´s point (1/1440 inch, also called a twip).
		pixel_width = pixel_width / INCH_TO_MM * INCH_TO_TWIP;
		break;
	default:
		break;
	}
	return pixel_width;//Width, in logical units according to the map_mode
}
void draw(HDC hdc,char	*bmp_filename)
{
	int map_mode;
	   int x = 0;
	   int y = 0;
	   int line_h;
	   char map_name[100];
	   long width;
	   long height;
	   double pixel_size;
	   int bmp_draw_width;
	   int bmp_draw_height;
	   HDC memdc;
	   HGDIOBJ oldbmp;
	   strcpy(map_name,"");
	   hbmp=NULL;
	   if(hbmp == NULL)
	   {
		   hbmp = (HANDLE)LoadImage( NULL,bmp_filename ,
			   IMAGE_BITMAP, 0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);
	   }   
	   
	   map_mode= MM_TWIPS;
	   
	   SetMapMode(hdc,map_mode); //Each logical unit is mapped to 0.1 millimeter.
	   
	   line_h = get_line_height(hdc);
	   SelectObject(hdc,GetStockObject(BLACK_PEN)); 
	   SetBkMode(hdc, TRANSPARENT);
	   TextOut(hdc,x, y,map_name,strlen(map_name));
	   y += line_h;
	   if( NULL != hbmp)
	   {         
		   BITMAP bm;
		   GetObject( hbmp, sizeof(BITMAP), &bm );
		   
		   width=bm.bmWidth;
		   
		   height=bm.bmHeight; 
		   
		   memdc = CreateCompatibleDC(hdc);
		   oldbmp = SelectObject(memdc,hbmp);
		   pixel_size = get_screen_pixel_width(map_mode);
		   /*************************************************************/
		   /*有些显示器打印的时候 会出现打印范围超出画面*/
		   /*bmp_draw_width = (int)(width * pixel_size/LOCAL_WIDTH*1024) ;
		   bmp_draw_height = (int)(height * pixel_size/LOCAL_HEIGHT*768 );*/
		   /*************************************************************/

		   /*************************************************************/
		   /*正常情况应采用此方法*/
		   bmp_draw_width = (int)(width * pixel_size) ;
		   bmp_draw_height = (int)(height * pixel_size );

		   StretchBlt(hdc,x,y, bmp_draw_width ,-bmp_draw_height, memdc, 0,0,width,height,SRCCOPY);
		   SelectObject(memdc,oldbmp);
		   DeleteDC(memdc);
	   } 
	   else
	   {
		   LPCTSTR error_msg = "failed to load bitmap from file";
		   TextOut(hdc,x,y,error_msg,strlen(error_msg));
	   }     
}

void PrintWinPic (HDC prtdc,int crt_n,char * bmpfilename)
{
	int	 width, height, bgcolorsave;
	float	 xscalesave,yscalesave,xprtfontscale,yprtfontscale,fLogPelsX1,fLogPelsY1,fLogPelsX2,fLogPelsY2;
	DOCINFO di;
	HDC		hdc;
	HANDLE	palsave;
	HWND ghWnd;
	int 	toprint;
	//char tempName[200];
	
	SetMapMode (prtdc, MM_TEXT);
	
	bgcolorsave = print_bg_color;
	xscalesave = xscale[crt_n];
	yscalesave = yscale[crt_n];
	palsave = ghPal;
	ghPal = NULL;
	
	print_bg_color = WHITE;						//	白色
	width     = GetDeviceCaps (prtdc, HORZRES);		//	检取有关给定设备的设备特定信息
	height    = GetDeviceCaps (prtdc, VERTRES) - 200;
	/*xscale[crt_n] = (float)width/(float)OLDWIDTH;
	yscale[crt_n] = (float)height/(float)OLDHEIGHT;*/
	
	ghWnd= GetActiveWindow();
	
	hdc = GetDC(ghWnd);						//	检取给定窗口显示设备描述表	
	fLogPelsX1 = (float)GetDeviceCaps (hdc, LOGPIXELSX);
	fLogPelsY1 = (float)GetDeviceCaps (hdc, LOGPIXELSY);
	ReleaseDC(ghWnd, hdc);
	fLogPelsX2 = (float)GetDeviceCaps (prtdc, LOGPIXELSX);
	fLogPelsY2 = (float)GetDeviceCaps (prtdc, LOGPIXELSY);
	xprtfontscale = fLogPelsX2/fLogPelsX1;
	yprtfontscale = fLogPelsY2/fLogPelsY1;
	
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
	
    //strcpy( tempName,get_temp_file_name( crt_n));
	
	//	WinSaveJPGFile(HWND_DESKTOP, tempName);
	draw(prtdc,bmpfilename);
	
	EndPage   (prtdc);			//	换页码
	EndDoc    (prtdc);			//	终止打印作业
	// DeleteDC  (prtdc);
	toprint = TOPRINTOFF;
	print_bg_color = bgcolorsave;
	//xscale[crt_n] = xscalesave;
	//yscale[crt_n] = yscalesave;
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
            return(FALSE);
		}
	}
	return(TRUE);
}

int   WinSaveJPGFile(HWND hWnd, char* filename,int print_mode)   
{   
	HDC			hDC, hMemDC;   
	HBITMAP		hBitmap, hBmpOld;   
	LPVOID		lpBits;   
	RGBQUAD		RGBQuad;   
	DWORD		ImgSize, plSize;   
	int			CRes, Height, Width;   
	FILE		*fp;
	BITMAPFILEHEADER	bmFH;   
	LPBITMAPINFO		pBmInfo;   
	
	
	int CYCAPTION,CYMENU,CYKANJIWINDOW,CXBORDER,CYSCREEN,CXSIZEFRAME,CYSIZEFRAME;
	
	
	
	if(NULL==(fp=fopen(filename, "wb+")))
		return 1;   
	
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	
	if(hWnd==HWND_DESKTOP)   
	{   
		hWnd= GetActiveWindow();
		
		Width= GetSystemMetrics(SM_CXSCREEN);   
		Height= GetSystemMetrics(SM_CYSCREEN);   
		
		//Width= GetSystemMetrics(SM_CXMAXIMIZED);   
		//Height= GetSystemMetrics(SM_CYMAXIMIZED);
		
		CXBORDER=GetSystemMetrics(SM_CXBORDER); //边框
		CYSCREEN=GetSystemMetrics(SM_CYBORDER);
		
		CXSIZEFRAME=GetSystemMetrics(SM_CXSIZEFRAME);
		CYSIZEFRAME=GetSystemMetrics(SM_CYSIZEFRAME);
        
		
		CYCAPTION=GetSystemMetrics(SM_CYCAPTION);
		CYMENU=GetSystemMetrics(SM_CYMENU);
		CYKANJIWINDOW= GetSystemMetrics(SM_CYKANJIWINDOW);
        
		Height=Height-	CYCAPTION;	
        
	}   
	
	else   
	{   
		RECT   rc;   
		GetClientRect(hWnd,&rc);   
		Width= rc.right-rc.left;   
		Height= rc.bottom-rc.top;   
	}  
	
	
	hDC= GetDC(hWnd);   
	hMemDC= CreateCompatibleDC(hDC);   
	hBitmap= CreateCompatibleBitmap(hDC, Width, Height);   
	hBmpOld= SelectObject(hMemDC, hBitmap);   
	//BitBlt(hMemDC,0,0,Width,Height,hDC,0,0,SRCCOPY);   
	switch (print_mode)
	{
	case RTP_MODE:
	case SGZY_MODE:	
		BitBlt(hMemDC,0,0,Width,Height,hDC,0,0,NOTSRCERASE);   //NOTSRCCOPY  ok
		break;
	case SOFTCPY_MODE:
	case NORMAL_MODE:
		BitBlt(hMemDC,0,0,Width,Height,hDC,0,0,SRCCOPY);   //NOTSRCCOPY  ok
		break;
	default:
		BitBlt(hMemDC,0,0,Width,Height,hDC,0,0,SRCCOPY);
		break;
	}
		
	SelectObject(hMemDC, hBmpOld);  
	
	pBmInfo= (LPBITMAPINFO)malloc(sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));   
	memset(pBmInfo, 0, sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));   
	pBmInfo->bmiHeader.biSize= (DWORD)sizeof(BITMAPINFOHEADER);   
	pBmInfo->bmiHeader.biWidth= Width;   
	pBmInfo->bmiHeader.biHeight= Height;   
	pBmInfo->bmiHeader.biPlanes= 1;   
	pBmInfo->bmiHeader.biBitCount= (WORD)GetDeviceCaps(hDC, BITSPIXEL);   
	pBmInfo->bmiHeader.biCompression= BI_RGB;   
	GetDIBits(hDC, hBitmap, 0, Height, NULL, pBmInfo, DIB_RGB_COLORS);   
	if(!pBmInfo->bmiHeader.biSizeImage)   
		pBmInfo->bmiHeader.biSizeImage= ((((pBmInfo->bmiHeader.biWidth
		*pBmInfo->bmiHeader.biBitCount)+31)&~31)/8)*pBmInfo->bmiHeader.biHeight;   
	CRes= GetDeviceCaps(hDC, SIZEPALETTE);   
	plSize= CRes*sizeof(RGBQUAD);   
	ImgSize= pBmInfo->bmiHeader.biSizeImage;   
	
	bmFH.bfType= 0x4d42; //"BM"   
	bmFH.bfOffBits= plSize+sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);   
	bmFH.bfSize= ImgSize+bmFH.bfOffBits;   
	bmFH.bfReserved1= 0;   
	bmFH.bfReserved2= 0;   
	
	fwrite(&bmFH, 1, sizeof(BITMAPFILEHEADER), fp);   
	fwrite(&(pBmInfo->bmiHeader), 1, sizeof(BITMAPINFOHEADER), fp);   
	
	if(CRes)   
	{   
		LPLOGPALETTE	lp;
		int		i;
		lp=(LPLOGPALETTE)malloc(sizeof(LOGPALETTE)+(CRes*sizeof(PALETTEENTRY)));
		memset(lp, 0, sizeof(LOGPALETTE)+(CRes*sizeof(PALETTEENTRY))); 
		lp->palNumEntries=(WORD)CRes;   
		lp->palVersion=0x0300;   
		GetSystemPaletteEntries(hDC, 0, CRes, lp->palPalEntry);   
		RGBQuad.rgbReserved=0;   
		for(i=0; i<CRes; i++)   
		{   
			RGBQuad.rgbRed= lp->palPalEntry[i].peRed;   
			RGBQuad.rgbGreen= lp->palPalEntry[i].peGreen;   
			RGBQuad.rgbBlue= lp->palPalEntry[i].peBlue;   
			fwrite(&RGBQuad, 1, sizeof(RGBQUAD), fp);   
		}   
		free(lp);   
	}   
	
	lpBits= malloc(pBmInfo->bmiHeader.biSizeImage);
	memset(lpBits, 0, pBmInfo->bmiHeader.biSizeImage);
	GetDIBits(hDC, hBitmap, 0, Height, lpBits, pBmInfo, DIB_RGB_COLORS);   
	fwrite(lpBits, 1, ImgSize, fp);   
	free(lpBits);
	fclose(fp);
	
	free(pBmInfo);   
	DeleteObject(hBitmap);   
	DeleteDC(hMemDC);   
	ReleaseDC(hWnd,hDC);   
	SetCursor(LoadCursor(NULL,IDC_ARROW));   
	
	return   0;     
}   

