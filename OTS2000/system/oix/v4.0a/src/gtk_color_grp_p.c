/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
    have been SYNC
	Date : 2007.11.27 
     

---------------------------------------------------------------------*/


#include <stdio.h>

#define		COLOR_GROUP_DEF  

/* #define		DEBUG    */
#include <gtk/gtk.h>
#include	"../inc/gtk_ipm_k_def.h"
#include            "../inc/oix.h"
 
	
void	LoadColorGrpLibrary ()
{
#ifdef SUN_OS
	int j;
#endif
 char version[6] ;

	int	len, i;
	FILE	*fp;

	char	clr_grp_part_name[100]  ;
    
	strcpy(clr_grp_part_name,FILE_COLOR_GROUP);

	fp = fopen(clr_grp_part_name, "rb");
	if (fp == NULL)
	{
		printf ("OIX: open COLOR_GRP_LIBRARY.DBIN error.\n");
		exit (-1);
	}

   
   fread(version,1,6,fp);
   if (strcmp("v4.0a",version)!=0)    /*4.0a *.dbin file data formate */
      rewind (fp);
   		
	len	=4;
	fread (&cgdb_cnt, len, 1, fp);
#ifdef SUN_OS
        Swap4Byte(&cgdb_cnt);        
printf("cgdb_cnt = %d\n",cgdb_cnt);
#endif
	
	len     =sizeof(cgdb[0]);
	for (i=0; i<cgdb_cnt; i++)
	{
		fread (&cgdb[i], len, 1, fp);
#ifdef SUN_OS 
                for(j=0;j<GRP_MAX_COLORS;j++)
                {
                        Swap4Byte(&cgdb[i].cgrp_cell[j].name);
                        Swap4Byte(&cgdb[i].cgrp_cell[j].c_normal);
			Swap4Byte(&cgdb[i].cgrp_cell[j].flash);    
                }
#endif
	}	

	fclose (fp);
#ifdef	DEBUG
	printf ("OIX: LoadColorGrpLibrary ok\n");
#endif

}



