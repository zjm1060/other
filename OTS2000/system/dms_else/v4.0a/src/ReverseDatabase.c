#include	"../inc/os_type_def.h"
#include	"../inc/dms_text_def.h"
#include	"../inc/dms_com.h"
#include	"../inc/dms_functions.h"

extern  DBS_GBL_COMMON  *db_ptr;


void ReverseDatabaseDefineFiles(char *output_path)
{
	GULONG		i, stn_num;
	FILE	*fp;
	char	output_file[256];

	if(-1==MountDB(NULL))
	{
		printf("\nFail to mount database");
		return;
	}

	sprintf(output_file, "%s%s", output_path, "DBSTATION.DEF");
	if((fp=fopen(output_file, "w"))==NULL)
	{
		printf("\n\nFail to create %s", output_file);
		return;
	}

	GetStationNum(&stn_num);
	for(i=0; i<stn_num; i++)
	{
		fprintf(fp, "\nSTATION	NAME	=	%s", db_ptr->Station_Union.StationEntry[i].stnName);

	}

	fclose(fp);



	UnMountDB();
}
