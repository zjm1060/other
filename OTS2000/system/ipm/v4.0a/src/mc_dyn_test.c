
#include <windows.h>

#include	"resource.h"

#include	"../inc/draw_def.h"
#include	"../inc/ipm_k_def.h"

#include	"../../../dms/v4.0a/inc/dms_com.h"

void DrawHandle ( HWND, BACKRCD *);

void	DynamicsTest ( hwnd, test_type)
HWND	hwnd;
int		test_type;
{
	int		i;
	DMS_COMMON	dms_cmm;
	NEW_GBRCD		rcd;
	char	msg[30] = "个数据库点名错误，请修正!";
	char	err_num[40]="";	
	char 	str[256];
	FILE	*fp;

	int	dberr_cnt =0;
	
	fp= fopen("/home/ems/h9000/dsp/all_dbname.txt", "w");

	switch( test_type)
	{
	case	ID_Datab_Test:
		for (i=0; i < v4_gdbase.nextpos; i++)
		{		
			rcd	= v4_gdbase.buffer[i];

			if( strcmp ( rcd.lnk.dbname ,"") !=0)
			{				
				if (ReadEntryByNameStr (rcd.lnk.dbname,&dms_cmm) == -1)
				{
					dberr_cnt++;
					sprintf(str, "IPM: %s 在数据库里不存在!\n", rcd.lnk.dbname);  /*modified  chi 04.08.24*/
					if (MessageBox (hwnd, str, "IPM", MB_OKCANCEL)==IDCANCEL)
						break;
					v4_gdbase.buffer[i].bck.tag = ITEM_TAG_ON;
					DrawHandle (hwnd, &v4_gdbase.buffer[i].bck );
				}
				if(fp)
					fprintf(fp, "%03d %s\n", i, rcd.lnk.dbname);
			}
		}
		if( dberr_cnt !=0)
		{
			sprintf( err_num, "%d\0", dberr_cnt);	
			strcat( err_num, msg);
			MessageBox ( hwnd, err_num, "IPM", MB_OK);
		}  
		else
			MessageBox (hwnd, " 数据库点名全部正确! ", "IPM", MB_OK); 
		break;

	default:
		break;
	}
	
	if(fp)
		fclose(fp);
}

