/***********************************************
   SYN by hcl 2007.12.12
**********************************************/

#define	SYMBOL_PIX		64

#define SYMBOL_NAME_LEN		10	/*	Modified by zyp from 10 to 12     */
#define SYMBOL_CLASS_NAME_LEN	10	/* 10		*/
#define	MAX_SYMBOL_OBJECTS	128	/* max draws in one symbol old value --32 */
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
int		symdb_class_cnt;


