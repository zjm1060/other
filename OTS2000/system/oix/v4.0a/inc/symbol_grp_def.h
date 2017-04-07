
/*
	97.12.14. modified by by wdk to add dynamic animation.
*/
/***********************************************
   SYN by hcl 2007.12.12
**********************************************/

#define MAX_SYMBOL_GRPS		256	/* old value 128 max symbol groups */
#define GRP_MAX_SYMBOLS		16	/* max symbols in a symbol group */
#define SYMBOL_GRP_NAME_LEN	20
#define	SYMBOL_GRP_WIDTH	32
#define	SYMBOL_GRP_HEIGHT	32

#define	MAX_ANIMATE_SYMBOLS	4	/* wdk 97.12.14.        */
					/* numb of symbols for picture animation        */

typedef struct
{
	char		name[SYMBOL_GRP_NAME_LEN];
	SYMB_ID		symbol[GRP_MAX_SYMBOLS][MAX_ANIMATE_SYMBOLS];
	int		xx;
}	SYMBOL_GRP;

SYMBOL_GRP	symgdb[MAX_SYMBOL_GRPS];
int		symgdb_cnt;


