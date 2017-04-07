#define MAX_SYMBOL_GRPS		256	/* old value 128 max symbol groups */
#define GRP_MAX_SYMBOLS		16	/* max symbols in a symbol group */
#define SYMBOL_GRP_NAME_LEN	20
#define	SYMBOL_GRP_WIDTH	32
#define	SYMBOL_GRP_HEIGHT	32


#define	SGED_new	0
#define	SGED_edit	1
#define	SGED_save	2
#define	SGED_save_as	3
#define	SGED_save_file	4
#define	SGED_quit	5

#define	MAX_ANIMATE_SYMBOLS		4	/*Added by zyp on 2001.5.16 */

typedef struct
{
	char		name[SYMBOL_GRP_NAME_LEN];
	SYMB_ID		symbol[GRP_MAX_SYMBOLS][MAX_ANIMATE_SYMBOLS];/* By zyp */
//	SYMB_ID		symbol[GRP_MAX_SYMBOLS];
	int		nextpos;
}	SYMBOL_GRP;

SYMBOL_GRP	symgdb[MAX_SYMBOL_GRPS];
SYMBOL_GRP	symg_buff;
int		symgdb_cnt;


int	SGED_canvas_width, SGED_canvas_height;
int	SGED_xn, SGED_yn;
int	SGED_sym_width, SGED_sym_height;


int	sym_grp_edit_mode;


#define	ADD_NEW_SYMBOL_GRP		0
#define	EDIT_EXISTING_SYMBOL_GRP	1
#define	DELETE_SYMBOL_GRP		2
#define COPY_EXISTING_SYMBOL_GRP	3

HWND	ghWndSymGrpEdit;
BOOL CALLBACK SymbolGroupEditHandler(HWND, UINT, WPARAM, LPARAM);

