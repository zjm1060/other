
typedef	struct
{
	int	width, height;
	float	xfactor, yfactor;
} VIEWDB;

VIEWDB	viewdb;

HWND	view;
#define VIEWCLASS "ViewClass"	// view sub window class

BOOL	View();
LONG CALLBACK ViewProc(HWND, UINT, WPARAM, LONG);
void DrawViewRect (HWND, HDC);
