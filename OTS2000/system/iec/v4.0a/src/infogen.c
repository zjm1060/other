
#include	"/home/ems/system/dms/v4.0a/inc/os_type_def.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_com.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_functions.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_util_def.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_structure_def.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_text_def.h"

void	PrintEntryFixed( DMS_COMMON *common_aa );
void	PrintDIVar( DMS_COMMON *common_aa );
void	PrintAIVar( DMS_COMMON *common_aa );
void	PrintImpVar( DMS_COMMON *common_aa );
void	PrintCalcVar( DMS_COMMON *common_aa );
void	PrintCharaVar( DMS_COMMON *common_aa );
void	PrintCurveVar( DMS_COMMON *common_aa );
void	PrintDevObjVar( DMS_COMMON *common_aa );
void	WriteIndPoint(POINTER *point);
void	WriteAnaPoint(POINTER *point);
void	WriteImpPoint(POINTER *point);
void	WriteCharaPoint(POINTER *point);
void	WriteCurvePoint(POINTER *point);
void	WriteDevObjPoint(POINTER *point);


#pragma	comment(lib, "/home/ems/system/dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "/home/ems/system/dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "/home/ems/system/lan/v4.0a/lib/lan.lib")
#ifdef WINDOWS_OS
	#pragma	comment(lib, "wsock32.lib")
#endif 

#define YC_TABLE  "/home/ems/system/iec/v4.0a/src/IEC104_ME_DATA.DEF"
#define YX_TABLE  "/home/ems/system/iec/v4.0a/src/IEC104_DP_DATA.DEF"

#define MAX_LCU_NUM 20
int STATION_ID=0;

void	main()	
{
	DMS_COMMON	common_aa;
	char		pt_name[POINT_NAME_SIZE],str[128],str1[128];
	POINTER		point;
	int lcuid,type,pt,YcNum,YxNum,tmp;
	FILE *fp;

	if(-1==MountDBFile(NULL, 1))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	if( (fp=fopen(YC_TABLE, "w"))==NULL )
	{	printf("can't open  %s  !!!\n",YC_TABLE); exit(-1);  }  


	printf("Please input station id:\n");
	scanf("%d",&STATION_ID);

	YcNum=0;
	
	for(lcuid=1;lcuid<=MAX_LCU_NUM;lcuid++)
	{
		for(type=1;type<=13;type++)
		{
			for(pt=0;pt<=3000;pt++)
			{
				if((type==ANA_TYPE)||(type==IMP_TYPE)||(type==PARAM_TYPE))
				{
					tmp=lcuid;
					
					sprintf(pt_name,"%d.%d.%d.%d",STATION_ID,tmp,type,pt);
					if(-1==GetPtIdFromStr(pt_name, &point))
						break;
					if(-1==ReadEntryById( &point, &common_aa ))
						break;
					GetPtNameById( &common_aa.point, str, NULL );
					GetPtLongnameById(&common_aa.point, str1);
					//if(common_aa.fixed.ioattr.cascade==1)
					{
						fprintf( fp, "%4d\t %30s\t %10.1f\t %10.1f\t  %4d\t  %10.1f\t %4d\t %4d\t %40s  \n", YcNum, str, 
							common_aa.var.anlg.eng_para.LO_RANGE,  common_aa.var.anlg.eng_para.HI_RANGE, 
							10,common_aa.var.anlg.eng_para.D_BAND, 3,2,str1);
						
						YcNum+=1;
					}
				}
			}
		}
	}
	fclose(fp);
	printf("Ana def finish!!!\n");

	if( (fp=fopen(YX_TABLE, "w"))==NULL )
	{	printf("can't open  %s  !!!\n",YX_TABLE); exit(-1);  }  

	YxNum=0;
	
	for(lcuid=1;lcuid<=MAX_LCU_NUM;lcuid++)
	{
		for(type=4;type<=6;type++)
		{    
			for(pt=0;pt<=4000;pt++)
			{
				tmp=lcuid;
				
				sprintf(pt_name,"%d.%d.%d.%d",STATION_ID,tmp,type,pt);
				if(-1==GetPtIdFromStr(pt_name, &point))
					break;
				if(-1==ReadEntryById( &point, &common_aa ))
					break;
				GetPtNameById( &common_aa.point, str, NULL );
				GetPtLongnameById(&common_aa.point, str1);
				//if(common_aa.fixed.ioattr.cascade==1)
				{
					fprintf( fp, "%4d\t  %30s\t  %10d\t %40s  \n", YxNum, str, 
						2, str1);
					YxNum+=1;
				}
			}
		}
	}
	fclose(fp);
	printf("Ind def finish!!!\n");

}


