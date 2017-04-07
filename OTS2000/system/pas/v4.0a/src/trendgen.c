#define UNIX_OS

#ifdef UNIX_OS

#include <stdio.h>
/*
#include "/DBINC/dmsstd.h"
#include "/DBINC/dmserr.h"
#include "/DBINC/dmsdef.h"
#include "/DBINC/dmscom.h"
*/
#include	"/DBINC/dms_com.h"
/*#include	"/PRJINC/project_def.h"*/
#include	"/PASINC/trend_data_rec.h"
#else

#include        <stdio.h>

#include        "/users/ems/system/dms/v4.0a/inc/dms_com.h"
#include	"/users/ems/project/cmm/v4.0a/inc/project_def.h"
#include	"/users/ems/system/pas/v4.0a/inc/trend_data_rec.h"

#endif

void main()
{
	char   		full_name[150];
	FILE    	*fp;

	unsigned 	char	devid;
	short		id;
	GULONG		maxstn,maxdev;
	int		k,ret;
	CURVE_DEF 	data;

	POINTER		ptr;
	char		curvefilename[256];
	
	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
	
	

	if(-1==MountDB(NULL))/*DATABASE_NAME))*/
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
	
	data.curve_num=0;
	
	strcpy(full_name,curvefilename);
	if ((fp = fopen(full_name, "rb")) == NULL) 
	{
		GetStationNum(&maxstn);
		
		printf("maxstn=%d\n",maxstn);
		
		for(k=1;k<=maxstn;k++)
		{
			UCHAR stnId;
			GetStnIdByIndex(k, &stnId);
		
			ret=GetGroupNumById(stnId,&maxdev);
		
			if(ret==-1) continue;		

			data.curve_num = 0;
			for(devid=1;devid<=maxdev;devid++)
			{
				ptr.stn_id      =stnId;
				ptr.dev_id      =devid;
				ptr.data_type   =ANA_TYPE;

        			for (id=1; id<2; id++)
        			{
        				data.buffer[data.curve_num].stn_id	= ptr.stn_id;
        				data.buffer[data.curve_num].dev_id	= ptr.dev_id;
        				data.buffer[data.curve_num].data_type = 1;
        				data.buffer[data.curve_num].pt_id = id;

						Swap2Byte(&data.buffer[data.curve_num].pt_id);

						data.curve_num++;
        			}
        		}
        	}

		Swap2Byte(&data.curve_num);

		if ((fp = fopen(full_name, "wb")) == NULL)
        {
			printf("open trend_file %s error\n",full_name);
			exit;
		}
					
   		if (fwrite(&data,sizeof(CURVE_DEF), 1, fp)!=1)
		printf("trend_file write error\n");
		fclose( fp );
	}
	else
	{
		printf("trend_file %s exist already! no file created!\n",full_name);
		fclose( fp );
	}
}
