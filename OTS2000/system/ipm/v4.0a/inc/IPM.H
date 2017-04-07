

#include <windows.h>   // required for all Windows applications
//#include "resource.h"  // Windows resource IDs


#include	"..\inc\ipm_k_def.h"
#include	"..\inc\symbol_def.h"
#define   MAX_PRJ_NUM   30
typedef	struct _iobuf FILE;

/*
** Porting macros...
*/
#if defined(_WIN32) || defined(WIN32)
#define GETINSTANCE(hWnd)   (HANDLE)GetWindowLong(hWnd,GWL_HINSTANCE)
#define GETCLASSBRUSH(hWnd) (HBRUSH)GetClassLong(hWnd,GCL_HBRBACKGROUND)
#else
#define GETINSTANCE(hWnd)   (HANDLE)GetWindowWord(hWnd,GWW_HINSTANCE)
#define GETCLASSBRUSH(hWnd) (HBRUSH)GetClassWord(hWnd,GCW_HBRBACKGROUND)
#endif

// added by chi 2004.08.18

#ifdef WIN32
#define LONG2POINT(l, pt)  ((pt).x = (SHORT)LOWORD(l), (pt).y = (SHORT)HIWORD(l))
#else
#define LONG2POINT(l, pt)  ((pt).x = (INT)LOWORD(l), (pt).y = (INT)HIWORD(l))
#endif

#define DSP_VER  1024   //display vertres resolve  chi
#define DSP_HOR  1280   //display horizon resolve  chi


HWND            ghWnd; // Main window handle.
HDC				ghDC;  // Main window DC
HANDLE          ghPal;          /* Handle to the application's logical palette */
HINSTANCE 		ghInst;          // current instance


BOOL InitApplication(HANDLE);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void 	ProcessCDError(DWORD dwErrorCode, HWND hWnd);

void	FormFullName( char	*, char	*, char	*);
int 	Colors(int	i);

void	LoadSecurityFile ();
void	LoadStringGrpLibrary ();
void	LoadSymbolLibrary ();
void	LoadSymbolGrpLibrary ();
void	LoadColorGrpLibrary ();
void	LoadMenuLibrary ();
void	CreateFillPalette ();
void	CreateColorPalette();
void	CreateLogFont();

void	IpmInit ();
void	ExitProc ();
BOOL	TestColor();

void	ReadTmpltFile(); 

void	fwriteRcd (FILE *, NEW_GBRCD *);
void	fwriteBckRcd (FILE *, BACKRCD *);
void	fwriteLnkRcd (FILE *, NEW_LINKRCD *);
void	freadRcd (FILE *, GBRCD *);
void	freadBckRcd (FILE *, BACKRCD *);
void	freadLnkRcd (FILE *, LINKRCD *);
void	ReadGdbaseData (FILE *);
FILE	*FileOpen (char	*);
FILE	*OpenWriteFile (char *);
void DrawGif(HWND hwnd ,HDC hdc ,int DrawMode ,BACKRCD	*rcd);

void	OpenDspFile (char *);
BOOL 	OpenNewFile( HWND, char*, int);
int _lstrlen(  LPCTSTR lpString  );

void	Redraw(HWND, HDC);
void 	DrawRcd ( HWND, HDC, BACKRCD *);
void 	DrawSymbol ( HWND, HDC, BACKRCD *, SYMBOL_CLASS *, int);
void 	CheckRectCoords (XRECT, int *, int *, int *, int *);

LRESULT CALLBACK	IpmMenuProc (HWND, UINT, WPARAM, LPARAM);
void	ButtonPressProcess (HWND, UINT, WPARAM, LPARAM);
void	ButtonMotionProcess (HWND, UINT, WPARAM, LPARAM);
void	ButtonReleaseProcess (HWND, UINT, WPARAM, LPARAM);
void	ShiftBtn1DoubleClick (HWND, UINT, WPARAM, LPARAM);
void	CtrlBtn1DoubleClick (HWND, UINT, WPARAM, LPARAM);
//BOOL IsInSelRCD(int  ,int  );
int ColorId3to4(int id3);

void     SetFillBk(int isTrans);
void DeleteGdbItems (void);

int	XtoSX (int);
int	YtoSY (int);
int	SXtoX (int);
int	SYtoY (int);

void GetFilePartName(const char *FullName, char *PartName);
BOOL CenterWindow (HWND, HWND);
int	SearchMenuId ( char	menu_name[MENU_NAME_LEN]);
void SelectAllItems(NEW_GDBUFF	*gdb);
void DrawGdbuffItems (HWND, int, int);
void MoveItemsFinished (NEW_GDBUFF *,MOVE_MODE,DIR) ; //modified chi 04.08.19
void ToggleProc(int);
void DrawHandle(HWND, BACKRCD *, int);

typedef struct
{
	 char Longname[30];
	 char name[30] ;
}   PRJ ;

typedef struct
{
	 int  nextprj;
	 int  curprj ;
	 PRJ project[MAX_PRJ_NUM] ;   /*Max project is 30 */
}   PRJDEF ;



