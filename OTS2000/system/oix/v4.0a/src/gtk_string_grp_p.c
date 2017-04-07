/************************************
have been SYNC  by hcl;
LAST UPATE : 2007.12.09 by hcl ;
*************************************/
#include <stdio.h>

#include        "../inc/oix.h"
 
/*	#define	DEBUG	*/
extern BOOL isExLang   ; 
void	LoadStringGrpLibrary ()
{
	int	i;
	FILE	*fp;

	char	*str_grp_part_name[2] = {FILE_STRING_GRP ,  FILE_STRING_GRP_EXLANG};

	fp = fopen( str_grp_part_name[isExLang] , "rb");
	if (fp == NULL)
        {
                printf ("OIX: open STRING_GRP_LIBRARY.DBIN error., str_grp_part_name=%s\n", str_grp_part_name[isExLang]);
                exit (-1 );
        }

	fread (&strg_cnt, sizeof(strg_cnt), 1, fp);	
#ifdef SUN_OS
	Swap4Byte(&strg_cnt);
printf("strg_cnt = %d\n",strg_cnt);
#endif

	for (i=0; i<strg_cnt; i++)
	{
/*		fread(&strgdb[i].name, sizeof(char), STR_GRP_NAME_LEN, fp);
		fread(&strgdb[i].str, sizeof(char), STR_LEN*GRP_MAX_STRS, fp);
by zyp on 2004.7
*/
		fread(strgdb[i].name, sizeof(char), STR_GRP_NAME_LEN, fp);
                fread(strgdb[i].str, sizeof(char), STR_LEN*GRP_MAX_STRS, fp);

		fread (&strgdb[i].nextpos, sizeof(strgdb[i].nextpos), 1, fp);	
#ifdef SUN_OS
		Swap4Byte( &strgdb[i].nextpos );
#endif
		
	}	

	fclose (fp);
#ifdef DEBUG
	printf ("OIX: LoadStringGrpLibrary ok\n");
#endif

}






