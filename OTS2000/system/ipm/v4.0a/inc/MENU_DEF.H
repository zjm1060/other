
int	menu_edit_mode;

#define	ADD_NEW_MENU		0
#define	EDIT_EXISTING_MENU	1
#define	DELETE_MENU		2
#define	COPY_EXISTING_MENU	5


#define	MAX_MENU_OBJECTS	48	/* max draws in one control menu */
#define	MAX_MENUS		256	/* max control menus from 128->256*/


#define	k_MED_new		0
#define	k_MED_edit		1
#define	k_MED_delete		2
#define	k_MED_save_file		3
#define	k_MED_quit		4
#define	k_MED_upload		5

typedef struct
{
	char		name[MENU_NAME_LEN];
	NEW_GBRCD		rcd[MAX_MENU_OBJECTS];
	int		x1, y1, x2, y2;
	int		tag;
	int		nextpos;
}	NEW_MENU;  //4.0


typedef struct
{
	char		name[MENU_NAME_LEN];
	GBRCD		rcd[MAX_MENU_OBJECTS];
	int		x1, y1, x2, y2;
	int		tag;
	int		nextpos;
}	MENU;    //3.0




typedef struct
{
	MENU		menu[MAX_MENUS];
	int		tag;
	int		nextpos;
}	MDBASE;

typedef struct
{
	NEW_MENU		menu[MAX_MENUS];
	int		tag;
	int		nextpos;
}	NEW_MDBASE;

MDBASE			menudb;
NEW_MDBASE		v4_menudb;

//XmStringTable	menu_name_list;



