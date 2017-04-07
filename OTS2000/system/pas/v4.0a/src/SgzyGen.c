
#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../inc/sgzy.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"


#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "wsock32.lib")



int main (int argc, char *argv[])
{
	char   		full_name[150];
	FILE    	*fp;
	IND_DATA	rt_status;
	
	unsigned 	char	devid;
	short		type,id;
	GULONG		datanum,maxstn,maxdev;
	int			i,k,ret;
	BOOL		io_type;
	SGZY_DY 	data;
	char		ptname[200];
	
	POINTER		ptr;
	
	char		cmd[32];
	BOOL		bOverRide=FALSE;
	
	
	if(-1==MountDB(NULL))/*DATABASE_NAME))*/
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	printf("can you want to override? y/n(default)");
	memset(cmd, 0, sizeof(cmd));
	gets(cmd);

	if (strcmp(cmd, "y") == 0)
	{
		bOverRide=TRUE;
	}
	
	data.next_pos=0;
	
	GetStationNum(&maxstn);
	
	printf("maxstn=%d\n",maxstn);
	
	for(type=0;type<10;type++)
	{
		
		for(k=1;k<=maxstn;k++)
		{
			UCHAR stnId;
			GetStnIdByIndex(k, &stnId);
			
			ret=GetGroupNumById(stnId,&maxdev);
			
			if(ret==-1) continue;		
			
			for(devid=1;devid<=maxdev;devid++)
			{
				ptr.stn_id      =stnId;
				ptr.dev_id      =devid;
				ptr.data_type   =type+1;
				
				if(GetEntryNumById(ptr.stn_id,ptr.dev_id,ptr.data_type, &datanum )==-1)
					continue;
				
				
				for(id=0;id<datanum;id++)
				{
					ptr.pt_id    =id;
					
					if( IsPtFaultRecById(&ptr,&io_type)==-1)	continue;
					
					if (io_type == 1)
					{	
						GetPtNameById(&ptr,ptname,NULL);
						
						strcpy(full_name,"/home/ems/h9000/dat/");
						strcat ( full_name, ptname ); 
						strcat ( full_name, ".FAIL" ); 
						/*
						printf("\nname=%s ,id=%d type=%d,dev=%d stn=%d\n",full_name,ptr.pt_id,ptr.data_type,ptr.dev_id,ptr.stn_id);
						*/printf("\nname=%s\n",full_name);

						if (!bOverRide)
						{
							if (access(full_name, 0) != -1)
							{
								printf("Sgzy file %s existed!", full_name);
								continue;
							}
						}

						if ((fp = fopen(full_name, "rb")) == NULL) 
						{
							data.next_pos = 0;
							for (i=1; i<2; i++)
							{
								data.buffer[data.next_pos].stn_id	= ptr.stn_id;
								data.buffer[data.next_pos].dev_id	= ptr.dev_id;
								data.buffer[data.next_pos].data_type = 1;
								data.buffer[data.next_pos].pt_id = i;
								Swap2Byte(&data.buffer[data.next_pos].pt_id);
								data.next_pos++;
							}
							Swap2Byte(&data.next_pos);
							if ((fp = fopen(full_name, "wb")) == NULL) 
								continue;
							
							if (fwrite(&data,sizeof(SGZY_DY), 1, fp)!=1)
								printf("sgzy_file write error\n");
							fclose( fp );
						}
						else
							fclose( fp );
						
					}
					else
						continue;
				}
			}
		}
	}
}
