
#define	TOOLKIT_BITMAP_NUM	11
#define	LINE_WIDTH_BM_NUM	6
HWND	ghWndToolKit;
HBITMAP	ghWndToolBtn[TOOLKIT_BITMAP_NUM];
HBITMAP	ghToolKitbmUp[TOOLKIT_BITMAP_NUM];
HBITMAP	ghToolKitbmDn[TOOLKIT_BITMAP_NUM];
HBITMAP	ghLnWdthbm[LINE_WIDTH_BM_NUM];

int	cb_width, cb_height;  // palette color block size
int	color_num_choice;

BOOL CALLBACK ToolKitProc(HWND, UINT, WPARAM, LPARAM);
