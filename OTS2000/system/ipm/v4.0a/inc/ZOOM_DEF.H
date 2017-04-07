
typedef struct
{
	float	factor;
	int	factor_max;
	int	factor_min;
	float	step;
	int	sx_orig, sy_orig;
	int	canvas_w, canvas_h;
}       ZOOMDB;

ZOOMDB	zoomdb;

void  	ZoomInit ();
void	ZoomIn (HWND, UINT, WPARAM, LPARAM);
void	ZoomOut (HWND, UINT, WPARAM, LPARAM);
void	ZoomAreaMove (HWND, UINT, WPARAM, LPARAM);
void 	CheckZoomOrigin ();
