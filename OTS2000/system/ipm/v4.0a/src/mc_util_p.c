
#include <windows.h>
#include <stdio.h>


int	Max(x,y)
int	x,y;
{
return (x>y? x:y);
}

int	Min(x,y)
int	x,y;
{
return (x<y? x:y);
}

void AddValue_2 (x, dx)
short	*x;
int	*dx;
{
	*x	=	*x + *dx;
}


void AddValue (x, dx)
int	*x, *dx;
{
	*x	=	*x + *dx;
}



void BlankStripCopy (dst, src, len)
char	*dst;
char	*src;
int	len;
{
	while ( (--len >= 0) && (src[len] == ' ') );

	while ( *src != '\0')
	{
		*dst++ = *src++;
	}
	*dst = '\0';
}


void BlankFillCopy (dst,src,len)
char    *dst;
char    *src;
int     len;
{
	while ( *src && len-- > 0 )
		*(dst++) = *(src++);

	while (len-- > 0)
		*(dst++) = ' ';
	*dst = '\0';
}


float power(float *base, int exp)
{
	int i;
	float pow;

	pow = 1.0f;
	for ( i=0; i<exp; i++ )
	{
	 	pow = pow * (*base);
	}
	return(pow);
}

BOOL	CheckPointsDistance (x, x0, y, y0, offset)
int	x, y, x0, y0, offset;
{

	return( (abs(x-x0) <= offset) && (abs(y-y0) <= offset) );
}

BOOL	CheckPointInVerticalLine (x, y, x1, y1, y2, offset)
int	x, y, x1, y1, y2, offset;
{

	BOOL	points_in_line;

	points_in_line=(abs(x-x1) <= offset) &&
		y <=max((y2-offset*2),(y1+offset*2)) &&
		y >=min((y1+offset*2),(y2-offset*2));
               
	return	(points_in_line);
}


BOOL	CheckPointInHorizontalLine (x, y, y1, x1, x2, offset)
int	x, y, y1, x1, x2, offset;
{

	BOOL	points_in_line;

	points_in_line=(abs(y-y1) <= offset) &&
		x <=Max((x2-offset*2),(x1+offset*2)) &&
		x >=Min((x1+offset*2),(x2-offset*2));

	return	(points_in_line);
}


/****************************************************************************

        FUNCTION: CenterWindow (HWND, HWND)

        PURPOSE:  Center one window over another

        COMMENTS:

        Dialog boxes take on the screen position that they were designed at,
        which is not always appropriate. Centering the dialog over a particular
        window usually results in a better position.

****************************************************************************/

void GetCenterPos (HWND hwndChild, HWND hwndParent, int *xPos, int *yPos)
{
	RECT    rChild, rParent;
	int     wChild, hChild, wParent, hParent;
	int     wScreen, hScreen;
	HDC		hdc;

	// Get the Height and Width of the child window
	GetWindowRect (hwndChild, &rChild);
	wChild = rChild.right - rChild.left;
	hChild = rChild.bottom - rChild.top;

	// Get the Height and Width of the parent window
	GetWindowRect (hwndParent, &rParent);
	wParent = rParent.right - rParent.left;
	hParent = rParent.bottom - rParent.top;

	// Get the display limits
	hdc = GetDC (hwndChild);
	wScreen = GetDeviceCaps (hdc, HORZRES);
	hScreen = GetDeviceCaps (hdc, VERTRES);
	ReleaseDC (hwndChild, hdc);

	// Calculate new X position, then adjust for screen
	*xPos = rParent.left + ((wParent - wChild) /2);
	if (*xPos < 0) 
	{
		*xPos = 0;
	} 
	else if ((*xPos+wChild) > wScreen) 
	{
		*xPos = wScreen - wChild;
	}

	// Calculate new Y position, then adjust for screen
	*yPos = rParent.top  + ((hParent - hChild) /2);
	if (*yPos < 0) 
	{
		*yPos = 0;
	} 
	else if ((*yPos+hChild) > hScreen) 
	{
		*yPos = hScreen - hChild;
	}

	return;
}

BOOL CenterWindow (HWND hwndChild, HWND hwndParent)
{
	int     xNew, yNew;

	// Calculate new X, Y position, then adjust for screen
	GetCenterPos(hwndChild, hwndParent, &xNew, &yNew);

	// Set it, and return
	return SetWindowPos (hwndChild, NULL,
				xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}


BOOL CenterAndResizeWindow (HWND hwndChild, HWND hwndParent, int width, int height)
{
	int	xNew, yNew;

	// Calculate new X, Y position, then adjust for screen, and 
	GetCenterPos(hwndChild, hwndParent, &xNew, &yNew);

	// Set it, and return
	return MoveWindow(hwndChild, xNew, yNew, width, height, FALSE);

}


/****************************************************************************

        FUNCTION: ResizeWindow ()

        PURPOSE:  Resize window rect by new client rect 

        Parameter:
			xpos: (i) client left xpos 
			ypos: (i) client top ypos 
			width: (i) client width 
			height: (i) client height 
****************************************************************************/
/*
void ResizeWindow(HWND hwnd, int xpos, int ypos, int width, int height )
{
	POINT	pt, ltpt;
	RECT	winrc, clirc;

	GetWindowRect(hwnd, &winrc);
	GetClientRect(hwnd, &clirc);
	ltpt.x = clirc.left;
	ltpt.y = clirc.top;
	ClientToScreen(hwnd, &ltpt);
	pt.x = xpos;
	pt.y = ypos;
	ClientToScreen(hwnd, &pt);
	pt.x = pt.x - (ltpt.x - winrc.left);
	pt.y = pt.y - (ltpt.y - winrc.top);
	width = winrc.right - winrc.left - clirc.right + width;
	height = winrc.bottom - winrc.top - clirc.bottom + height;
	MoveWindow(hwnd, pt.x, pt.y, width, height, TRUE); 
}
*/

/*****************************************************************************

        FUNCTION: ResizeWindow ()

        PURPOSE:  Resize window rect by new client rect 

        Parameter:
			xpos: (i/o) client(i)/window(o) left xpos 
			ypos: (i/o) client(i)/window(o) top ypos 
			width: (i/o) client(i)/window(o) width 
			height: (i/o) client(i)/window(o) height 
****************************************************************************/
void ResizeWindow(HWND hwnd, int xpos, int ypos, int width, int height )
{
	RECT	winrc, clirc;

	GetWindowRect(hwnd, &winrc);
	GetClientRect(hwnd, &clirc);
	xpos = xpos - clirc.left + winrc.left;
	ypos = ypos - clirc.top + winrc.top;
	width = winrc.right - winrc.left - clirc.right + width;
	height = winrc.bottom - winrc.top - clirc.bottom + height;
	MoveWindow(hwnd, xpos, ypos, width, height, TRUE); 
}

/*****************************************************************************
 * Judgt whether Point in Parent window	is in SubWindow, and convert point coordinate
 * pt:  (i) point client coordinate in parent window
 * ptsub: (o) point client coordinate in sub window
 * return: 	TRUE = yes
 *			FALSE = no
****************************************************************************/
BOOL PtInSubWin(HWND hParentWnd, HWND hSubWin, POINT *pt, POINT *ptsub)
{
	RECT	rc;
	
	*ptsub = *pt;
	ClientToScreen(hParentWnd, ptsub);
	ScreenToClient(hSubWin, ptsub);
	GetClientRect(hSubWin, &rc);
	return ( PtInRect(&rc, *ptsub) ); 
} 

