#include	<math.h>
#include	"../inc/os_type_def.h"
#include	"../inc/dms_com.h"




int	GetPtId(char *pt_name, POINTER *point);
void	PrintEntryFixed( DMS_COMMON *common_aa );
void	PrintDIVar( DMS_COMMON *common_aa );
void	PrintAIVar( DMS_COMMON *common_aa );
void	PrintImpVar( DMS_COMMON *common_aa );
void	PrintCalcVar( DMS_COMMON *common_aa );
void	PrintCharaVar( DMS_COMMON *common_aa );
void	WriteIndPoint(POINTER *point);
void	WriteAnaPoint(POINTER *point);
void	WriteImpPoint(POINTER *point);
void	WriteCharaPoint(POINTER *point);






int	main(int argc, char *argv[])	
{
	DMS_COMMON	common_aa;
	POINTER		point;
	WR_VAR_UNION	new_value={0};
	IOST		iost={0};
	DATE_TIME	time={0};
	unsigned int	i, j, k, p;
	char		timeStr[256];
	char		namestr[POINT_NAME_SIZE];
	POINT_NAME	pt_name;		
	unsigned int	stn_num, grp_num, rec_num, pt_num;
	int		times;

	printf("\nsize= %d", sizeof(ENTRY_FIXED));

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
	
	SetModuleName("DBTEST");
	SetErrLogMode(argc, argv);

	if(-1==GetStationNum(&stn_num))
		exit(0);
		
	point.stn_id= 1;
	point.dev_id= 1;
	point.data_type= 5;
	point.pt_id= 130;

	GetSysTimeStr( timeStr );
	printf("\n%s", timeStr);
		GetPtNameById(&point, namestr, NULL);
	for(times=0; times<10000000; times++)
	{
		point.pt_id= 10;
		GetPtNameById(&point, namestr, NULL);
		if(-1==ReadEntryByNameStr( namestr, &common_aa ))
		{
			printf("\nNo this point in database");
			break;
		}
		/*if(-1==ReadEntryById( &point, &common_aa ))
		{
			printf("\nNo this point in database");
			break;
		}*/
	}
	GetSysTimeStr( timeStr );
	printf("\n%s", timeStr);
	
	exit(0);


		GetSysTimeStr( timeStr );
		printf("\n%s", timeStr);
	for(times=0; times<1; times++)
	{
		GetSysTime(&time);
		
		for(i=1; i<=stn_num; i++)
		{	
			if(-1==GetGroupNumById((UCHAR)i, &grp_num))
				continue;
				
			for(j=1; j<=grp_num; j++)
			{
				if(-1==GetRecordNumById((UCHAR)i, (UCHAR)j, &rec_num))
					continue;
				
				for(k=ANA_TYPE; k<MAX_DATATYPE_NUM; k++)
				{
					if(-1== GetEntryNumById((UCHAR)i, (UCHAR)j, (UCHAR)k, &pt_num))
						continue;

					for(p=0; p<pt_num; p++)
					{
						point.stn_id= i;
						point.dev_id= j;
						point.data_type= k;
						point.pt_id= p;
	
						GetPtNameById(&point, namestr, &pt_name);
						if(-1==ReadEntryByNameStr( namestr, &common_aa ))
						{
							printf("\nNo this point in database %d.%d.%d.%d",
								point.stn_id, point.dev_id, point.data_type, point.pt_id);
							break;
						}
						/*if(-1==ReadEntryById( &point, &common_aa ))
						{
							printf("\nNo this point in database");
							break;
						}*/
					
						/*switch	( point.data_type )
						{
						case	SOE_TYPE:
						case	POL_TYPE:
						case	OUTS_TYPE:
						case	SYSMSG_TYPE:
							new_value.ind.rt_state= p;
							SetPtValueIostById( &point, new_value, iost, time, RW_RT_VALUE );
							if(-1!=IsPtDoublePointById( &point, &doublePt ))
								if(doublePt)
									p++;
							break;

						case	ANA_TYPE:
						case	OUTA_TYPE:
						case	PARAM_TYPE:
							new_value.anlg.data= p*1000;
							SetPtValueIostById( &point, new_value, iost, time, RW_RT_VALUE );
							break;

						case	CALC_TYPE:
							new_value.calc.data= p*1000;
							SetPtValueIostById( &point, new_value, iost, time, RW_RT_VALUE );
							break;

						case	IMP_TYPE:
							new_value.imp.status.option= 0;
							new_value.imp.iValue= p*12;
							SetPtValueIostById( &point, new_value, iost, time, RW_RT_VALUE );
							break;
							
						default:
							break;
						}*/
					}
				}
			}
		}
	}
		GetSysTimeStr( timeStr );
		printf("\n%s", timeStr);
	UnMountDB();
}


