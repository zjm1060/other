/***********************************************
   SYN by hcl 2007.12.12
**********************************************/
#define MAX_STR_GRPS		128	/* max string groups */
#define GRP_MAX_STRS		16	/* max strings in a string group */
#define STR_GRP_NAME_LEN	20
#define STR_LEN			16

typedef struct
{
	char		name[STR_GRP_NAME_LEN];
	char		str[STR_LEN][GRP_MAX_STRS];
	int		nextpos;
}	STR_GRP;

STR_GRP		strgdb[MAX_STR_GRPS];
int		strg_cnt;



