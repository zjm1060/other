
#include <stdio.h>

#define	SUB_DSP_DEF

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/mode_def.h"
#include	"../inc/sub_dsp_def.h"

extern char	cur_dsp_file[MAX_FILE_NAME_LEN];

void DrawGrid (HDC);
void ResizeWindow(HWND, int, int, int, int);

void SubDspEditHandler ()
{

	edit_mode	= SUB_DSP_EDIT;
	memset(cur_dsp_file, 0, MAX_FILE_NAME_LEN);
    EnableMenuItem( GetMenu( ghWnd ), ID_SAVE,
                       MF_BYCOMMAND | MF_GRAYED );
    EnableMenuItem( GetSubMenu( GetMenu(ghWnd), 0), 7,
                       MF_BYPOSITION | MF_GRAYED );
//	DrawGrid (ghDC);
	ResizeWindow(ghWnd, SUB_DSP_x, SUB_DSP_y, SUB_DSP_canvas_width, SUB_DSP_canvas_height );
}

void AbortSubDsp()
{
	edit_mode = DISPLAY_EDIT;
    EnableMenuItem( GetSubMenu( GetMenu(ghWnd), 0), 7,
                       MF_BYPOSITION | MF_ENABLED );
	MoveWindow(ghWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE); 
}


