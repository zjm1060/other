
#include	"../inc/os_type_def.h"
#include	"../inc/dms_structure_def.h"
#include	"../inc/dms_text_def.h"
#include	"../inc/dms_com.h"
#include	"../inc/dms_functions.h"
#include	"../inc/dms_util_def.h"
#include	"../inc/dms_data_def.h"


extern  DBS_GBL_COMMON  *db_ptr;
extern  char	cur_dbfile[128];
extern  char	new_dbfile[128];


int	InitializeDatabaseFile ( char *new_file );
int	GetCommonPtrById( POINTER *point, DMS_COMMON_PTR *common_ptr);
int	GetCommonPtrByNameStr( char *ptNameStr, DMS_COMMON_PTR *common_ptr);



void	ResetDatabaseRtValue( DBS_GBL_COMMON  *cur_db_ptr )
{
	DMS_COMMON_PTR  common_ptr;
	ENTRY_FIXED		*fixed_ptr;
	RD_VAR_UNION	*var_ptr;
	int		stn_num, grp_num, rec_num, entry_num;
	int		grp_offset, rec_offset, entry_offset;
	int		cur_stn_pos, cur_grp_pos, cur_rec_pos, cur_entry_pos;
	char	stn_name[STATION_NAME_SIZE];
	char	grp_name[GROUP_NAME_SIZE];
	char	rec_name[RECORD_NAME_SIZE];
	char	pt_name[256];
	int		ret;
	extern BOOL	clear_overflow_flag;

	stn_num= cur_db_ptr->Head_Union.head.stn_num;
	Swap4Byte(&stn_num);
	for(cur_stn_pos=0; cur_stn_pos<stn_num; cur_stn_pos++)
	{
		strcpy(stn_name, cur_db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName);

		grp_num= cur_db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_num;
		grp_offset= cur_db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_offset;
		Swap4Byte(&grp_num);
		Swap4Byte(&grp_offset);

		for(cur_grp_pos=grp_offset; cur_grp_pos<(grp_offset+grp_num); cur_grp_pos++)
		{
			strcpy(grp_name, cur_db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName);
			
			rec_num= cur_db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_num;
			rec_offset= cur_db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_offset;
			Swap4Byte(&rec_num);
			Swap4Byte(&rec_offset);

			for(cur_rec_pos=rec_offset; cur_rec_pos<(rec_offset+rec_num); cur_rec_pos++)
			{
				GetRecNameByType(cur_db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType, rec_name);
				entry_num= cur_db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_num;
				entry_offset= cur_db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_offset;
				Swap4Byte(&entry_num);
				Swap4Byte(&entry_offset);

				for(cur_entry_pos=entry_offset; cur_entry_pos<(entry_offset+entry_num); cur_entry_pos++)
				{
					ret= -1;
					
					switch(cur_db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType)
					{
					case	ANA_TYPE:
					case	OUTA_TYPE:
					case	PARAM_TYPE:
						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_Anlg_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_Anlg_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_Anlg_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->anlg.update_time= var_ptr->anlg.update_time;
						common_ptr.var_ptr->anlg.value= var_ptr->anlg.value;
						common_ptr.var_ptr->anlg.fValue= var_ptr->anlg.fValue;
						if(clear_overflow_flag)
							common_ptr.var_ptr->anlg.value.status.overflow= 0;
						break;

					case	SOE_TYPE:
					case	POL_TYPE:
					case	OUTS_TYPE:
					case	SYSMSG_TYPE:
						if(!strcmp(cur_db_ptr->EntryFixed_Status_Union.EntryFixed[cur_entry_pos].entryName,""))
							break;

						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_Status_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_Status_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_Status_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->ind.update_time= var_ptr->ind.update_time;
						common_ptr.var_ptr->ind.change_time= var_ptr->ind.change_time;
						common_ptr.var_ptr->ind.status= var_ptr->ind.status;
						common_ptr.var_ptr->ind.stc= var_ptr->ind.stc;
						break;

					case	IMP_TYPE:
						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_Imp_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_Imp_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_Imp_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->imp.entry.update_time= var_ptr->imp.entry.update_time;
						common_ptr.var_ptr->imp.entry.rt_value= var_ptr->imp.entry.rt_value;
						common_ptr.var_ptr->imp.entry.status= var_ptr->imp.entry.status;
						common_ptr.var_ptr->imp.entry.base_value= var_ptr->imp.entry.base_value;
						common_ptr.var_ptr->imp.entry.d_accum= var_ptr->imp.entry.d_accum;

						common_ptr.var_ptr->imp.entry.init= var_ptr->imp.entry.init;
						common_ptr.var_ptr->imp.entry.last= var_ptr->imp.entry.last;
						common_ptr.var_ptr->imp.entry.peak= var_ptr->imp.entry.peak;
						common_ptr.var_ptr->imp.entry.valley= var_ptr->imp.entry.valley;
						common_ptr.var_ptr->imp.entry.normal= var_ptr->imp.entry.normal;
						common_ptr.var_ptr->imp.entry.sum= var_ptr->imp.entry.sum;
						common_ptr.var_ptr->imp.entry.ppeak= var_ptr->imp.entry.ppeak;
						break;

					case	CALC_TYPE:
						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_Calc_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_Calc_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_Calc_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->calc.update_time= var_ptr->calc.update_time;
						common_ptr.var_ptr->calc.value= var_ptr->calc.value;
						common_ptr.var_ptr->calc.fValue= var_ptr->calc.fValue;
						break;
		
					case	CHARA_TYPE:
						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_Chara_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_Chara_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_Chara_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->chara.update_time= var_ptr->chara.update_time;
						common_ptr.var_ptr->chara.chara_data= var_ptr->chara.chara_data;
						break;

					case	CURVE_TYPE:
						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_Curve_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_Curve_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_Curve_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->curve.update_time= var_ptr->curve.update_time;
						common_ptr.var_ptr->curve.data= var_ptr->curve.data;
						break;

					case	DEVOBJ_TYPE:
						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_DevObj_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_DevObj_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_DevObj_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->devobj= var_ptr->devobj;
						break;

					case	TIME_TYPE:
						sprintf(pt_name, "%s.%s.%s.%s", stn_name, grp_name, rec_name,
							cur_db_ptr->EntryFixed_Time_Union.EntryFixed[cur_entry_pos].entryName);
						ret= GetCommonPtrByNameStr( pt_name, &common_ptr);
						if(-1==ret)
						{
							printf("\nNo %s in the new database", pt_name);
							break;
						}
						
						fixed_ptr= &cur_db_ptr->EntryFixed_Time_Union.EntryFixed[cur_entry_pos];
						var_ptr  = (RD_VAR_UNION*)&cur_db_ptr->EntryVar_Time_Union.EntryVariable[cur_entry_pos];
						
						common_ptr.var_ptr->time.update_time= var_ptr->time.update_time;
						common_ptr.var_ptr->time.value_sec= var_ptr->time.value_sec;
						common_ptr.var_ptr->time.value_ms= var_ptr->time.value_ms;
						common_ptr.var_ptr->time.status= var_ptr->time.status;
						common_ptr.var_ptr->time.host_id= var_ptr->time.host_id;
						strncpy(common_ptr.var_ptr->time.usr_name, var_ptr->time.usr_name, sizeof(var_ptr->time.usr_name));
						break;
						
					default:
						printf("\nUnknown data type %d", cur_db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType);
						break;
					}

					if(-1!=ret)
					{
						common_ptr.fixed_ptr->iost= fixed_ptr->iost;
					}	
				}
			}
		}
	}
}



int	UpdateDatabase(BOOL isReset)
{
	DBS_GBL_COMMON  *cur_db_ptr;
	DBS_GBL_COMMON  *new_db_ptr;
	struct stat		statbuf;

	if(-1==(stat(cur_dbfile, &statbuf)))
	{
		if(isReset)
		{
			printf("\nFail to find %s", cur_dbfile);
			printf("\nCancel to restore database");
		}
		isReset= 0;
		if(-1==InitializeDatabaseFile(cur_dbfile))
			return -1;
	}
	else
	{
		if(statbuf.st_size != sizeof(DBS_GBL_COMMON))
		{
			printf("\nFile %s length error: %d", cur_dbfile, statbuf.st_size);
			if(isReset)
				printf("\nCancel to restore database");
			isReset= 0;
			if(-1==InitializeDatabaseFile(cur_dbfile))
				return -1;
		}
	}
	
	if( -1==MountDBFile( cur_dbfile, 0 ) )
	{
		printf ( "\nMount %s failed!\n", cur_dbfile );
		return -1;
	}
	cur_db_ptr= db_ptr;

	db_ptr= (DBS_GBL_COMMON *)-1;
	if( -1==MountDBFile( new_dbfile, 0 ) )
	{
		printf ( "\nMount %s failed!\n", new_dbfile );
		db_ptr= cur_db_ptr;
		UnMountDB();
		return -1;
	}
	new_db_ptr= db_ptr;

	if(isReset)
	{
		SendTraceMsg(0,0,"Start to restore new database...");
		ResetDatabaseRtValue(cur_db_ptr);
		SendTraceMsg(0,0,"Success to restore database");
	}
	
	SendTraceMsg(0,0,"Start to update database...");
	*cur_db_ptr= *new_db_ptr;
	SendTraceMsg(0,0,"Success to update database");

	db_ptr= new_db_ptr;
	UnMountDB();
	db_ptr= cur_db_ptr;
	UnMountDB();
	
	utime(cur_dbfile, NULL);
	remove(new_dbfile);
	return 0;
}


