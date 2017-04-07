#ifndef _SYMBOL_DEF_H
#define _SYMBOL_DEF_H

int	sym_canvas_width, sym_canvas_height;
int	sym_xn, sym_yn;
int	sym_width, sym_height;

int	sym_edit_mode;

#define	SED_new		0
#define	SED_edit	1
#define	SED_delete	2
#define	SED_add_class	3
#define	SED_del_class	4
#define	SED_save	5
#define	SED_quit	6

#define	ADD_NEW_SYMBOL		0
#define	EDIT_EXISTING_SYMBOL	1
#define	DELETE_SYMBOL		2
#define	ADD_SYMBOL_CLASS	3
#define	DELETE_SYMBOL_CLASS	4
#define	COPY_EXISTING_SYMBOL	5
#define	EDIT_SYMBOL_CANCLE	6

#define	SYMBOL_PIX		64

#define SYMBOL_NAME_LEN		10
#define SYMBOL_CLASS_NAME_LEN	10
#define	MAX_SYMBOL_OBJECTS	128    // old 32   	/* max draws in one symbol */
#define	MAX_SYMBOLS		64	/* max symbols in one symbol class */
#define	MAX_SYMBOL_CLASS	32	/* max class in IPM */

typedef struct
{
	char		name[SYMBOL_NAME_LEN];
	BACKRCD		objects[MAX_SYMBOL_OBJECTS];
	int		tag;
	int		nextpos;
}	SYMBOL;

typedef struct
{
	char		name[SYMBOL_CLASS_NAME_LEN];
	SYMBOL		symbol[MAX_SYMBOLS];
	int		tag;
	int		nextpos;
}	SYMBOL_CLASS;

SYMBOL_CLASS	symdb[MAX_SYMBOL_CLASS];
//SYMBOL_CLASS    symTemdb[MAX_SYMBOL_CLASS]; //added to import symbol chi;

int		symdb_class_cnt;

//XmStringTable	sym_name_list;

#ifdef	SYMBOL_DEF

int	SED_canvas_width	= 896;
int	SED_canvas_height	= 896;

#else

extern	int	SED_canvas_width;
extern	int	SED_canvas_height;

#endif

HWND ghWndSymEdit;


#endif	/*_SYMBOL_DEF_H*/
