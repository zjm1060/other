
/*
	cretet by chi.hailong
	date 2005.09.19;
	LAST UPDATA by hcl 2007.12.09
	SYNC by hcl 2007.12.09
*/


#include <stdio.h>
#include        "../inc/oix.h"
 
#define	DEBUG		

void	LoadSymbolGrpLibrary ()
{
#ifdef	SUN_OS
	int j,k;
#endif

	int	len, i;
	FILE	*fp;
	char	sym_grp_part_name[100] ; 
    strcpy(sym_grp_part_name,FILE_SYMBOL_GROUP);
	fp = fopen( sym_grp_part_name, "rb");

	if (fp == NULL)
	{
		printf ("OIX: open SYMBOL_GRP_LIBRARY.DBIN error.\n");
		printf ("ExitProc (); ") ;
	}

	len	=4;
	fread (&symgdb_cnt, len, 1, fp);
#ifdef SUN_OS
        Swap4Byte(&symgdb_cnt);
	printf("symgdb_cnt = %x\n",symgdb_cnt);
#endif
	len     =sizeof(symgdb[0]);
	for (i=0; i<symgdb_cnt; i++)
	{
		fread (&symgdb[i], len, 1, fp);
#ifdef SUN_OS
		for(j=0;j<GRP_MAX_SYMBOLS;j++)
		{
			for(k=0;k<MAX_ANIMATE_SYMBOLS;k++)
			{
                		Swap4Byte(&symgdb[i].symbol[j][k].class);
                		Swap4Byte(&symgdb[i].symbol[j][k].name);  
                	}
		}
#endif
	}	

	fclose (fp);
#ifdef	DEBUG
	printf ("OIX: LoadSymbolGrpLibrary ok\n");
#endif
}



