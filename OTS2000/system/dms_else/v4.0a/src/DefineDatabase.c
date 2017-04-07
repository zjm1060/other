
#include	"../inc/os_type_def.h"
#include	"../inc/dms_text_def.h"
#include	"../inc/dms_com.h"
#include	"../inc/dms_functions.h"
#include	"../inc/dms_util_def.h"






int	DefineDatabaseStation(void);
int	DefineDatabaseGroup(void);
int GetEntries( int cur_rec_pos, char *rec_filename );
int	PrintError(char *buf);



extern  DBS_GBL_COMMON  *db_ptr;


void	SetDatabaseTime(void)
{
	db_ptr->check_valid_flag1=db_ptr->check_valid_flag2=time(NULL);
	GetSysTime(&db_ptr->Head_Union.head.gen_date_time);
}



int	DefineDatabase (void)
{
	SetDatabaseTime();
	SwapDateTimeByte(&db_ptr->Head_Union.head.gen_date_time);
	if(-1 == DefineDatabaseStation())
		return -1;
	if(-1 == DefineDatabaseGroup())
		return -1;

	return 0;
}


int	DefineDatabaseStation(void)
{
	int		i, lineNo, stn_valid;
	int		stn_num;
	int		stn_id;
	int		cur_stn_pos;	
	char	buffer[256];
	char	err_buf[256];
	FILE	*fp;
	char	source_filename[256];
	char	strbuf[5][MAX_CHAR_NUM_IN_LINE];

	printf ( "\n\nNow define stations, waiting ... " );

	sprintf(source_filename, "../gen/%s", STATION_DEF_FILE );

	fp = fopen ( source_filename, "r" );
	if ( fp == ( FILE *) 0 )
	{
		printf ( "\nFailed to open %s!\n", source_filename );
		return -1;
	}

	lineNo= 0;
	stn_num= 0;
	stn_valid= 0;
	cur_stn_pos= -1;
	while ( 1 )
	{ 
		lineNo++;
		if( fgets ( buffer, sizeof(buffer), fp ) == NULL )
			break;

		if ( buffer[0] == 'C' )
			continue;

		if ( buffer[0] == '\t' )
			buffer[0] = ' ';

		if ( -1 == GetStringsFromStr(buffer, sizeof(strbuf)/sizeof(strbuf[0]), strbuf) )
		{
			sprintf(err_buf, "DBSTATION.DEF line %d: %s: Format error", lineNo, buffer);
			PrintError(err_buf);
		}

		if ( !strcmp(strbuf[0], "") )
			continue;
		
		if ( !strcmp(strbuf[0], "END") )
			break;

		if ( !strcmp(strbuf[0], "STATION") & !strcmp(strbuf[1], "NAME") )
		{
			if(stn_valid==1)
				printf("\n%-10s%-20s id= %d   %-20s",
					db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName,
					db_ptr->Station_Union.StationEntry[cur_stn_pos].stnLongname,
					db_ptr->Station_Union.StationEntry[cur_stn_pos].stnId,
					db_ptr->Station_Union.StationEntry[cur_stn_pos].stnComment);

			if(cur_stn_pos>=(STATION_ENTRY_NUMBER-1))
			{
				stn_valid= 0;
				sprintf(err_buf, "DBSTATION.DEF line %d: %s: Arrive max station number %d", 
					lineNo, buffer, STATION_ENTRY_NUMBER);
				PrintError(err_buf);
				break;
			}

			stn_valid= 1;
			cur_stn_pos++;

			if(strlen(strbuf[2])>=STATION_NAME_SIZE)
			{
				sprintf(err_buf, "DBSTATION.DEF line %d: %s: station name too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[2][STATION_NAME_SIZE-1]=0;
			}
			for(i=0; i<cur_stn_pos; i++)
			{
				if(!strcmp(strbuf[2], ""))
					break;
				if(!strcmp(db_ptr->Station_Union.StationEntry[i].stnName, strbuf[2]))
				{
					sprintf(err_buf, "DBSTATION.DEF line %d: %s: station name repeat", 
						lineNo, buffer);
					PrintError(err_buf);
					break;
				}
			}
			strcpy(db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName, strbuf[2]);
			db_ptr->Station_Union.StationEntry[cur_stn_pos].stnId= cur_stn_pos+1;
			continue;
		}

		if(stn_valid==0)
			continue;

		if ( !strcmp(strbuf[0], "STN_ID") )
		{
			stn_id= atoi(strbuf[1]);
			if(stn_id != db_ptr->Station_Union.StationEntry[cur_stn_pos].stnId)
			{
				sprintf(err_buf, "DBSTATION.DEF line %d: %s: stn_id error", 
					lineNo, buffer);
				PrintError(err_buf);
			}
			continue;
		}

		else if ( !strcmp(strbuf[0], "LONGNAME") )
		{
			if(strlen(strbuf[1])>=STATION_LONGNAME_SIZE)
			{
				sprintf(err_buf, "DBSTATION.DEF line %d: %s: station longname too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[1][STATION_LONGNAME_SIZE-1]=0;
			}
			strcpy(db_ptr->Station_Union.StationEntry[cur_stn_pos].stnLongname, strbuf[1]);
			continue;
		}

		else if ( !strcmp(strbuf[0], "COMMENT") )
		{
			if(strlen(strbuf[1])>=STATION_COMMENT_SIZE)
			{
				sprintf(err_buf, "DBSTATION.DEF line %d: %s: station comment too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[1][STATION_COMMENT_SIZE-1]=0;
			}
			strcpy(db_ptr->Station_Union.StationEntry[cur_stn_pos].stnComment, strbuf[1]);
			continue;
		}
	}

	if((cur_stn_pos >=0)&&(stn_valid==1))
		printf("\n%-10s%-20s id= %d   %-20s",
			db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName,
			db_ptr->Station_Union.StationEntry[cur_stn_pos].stnLongname,
			db_ptr->Station_Union.StationEntry[cur_stn_pos].stnId,
			db_ptr->Station_Union.StationEntry[cur_stn_pos].stnComment);

	stn_num = cur_stn_pos+1;
	printf("\nstation num = %d", stn_num);
	Swap4Byte(&stn_num);
	db_ptr->Head_Union.head.stn_num = stn_num;
	
	fclose ( fp );

	return 0;
}



int	DefineDatabaseGroup(void)
{
	int		i, kk, lineNo, stn_valid, grp_valid;
	CHAR	cur_grp_id;
	int		cur_stn_pos;	
	int		cur_grp_pos;
	int		grp_num_in_stn;
	UCHAR	sysLcuId;

	int		grp_id;
	char	buffer[256];
	char	err_buf[256];
	FILE	*fp;
	char	source_filename[256];
	char	strbuf[5][MAX_CHAR_NUM_IN_LINE];

	printf ( "\n\nNow define groups, waiting ... " );

	sprintf(source_filename, "../gen/%s", GROUP_DEF_FILE );

	fp = fopen ( source_filename, "r" );
	if ( fp == ( FILE *) 0 )
	{
		printf ( "\nFailed to open %s!\n", source_filename );
		return -1;
	}

	lineNo= 0;
	stn_valid= 0;
	grp_valid= 0;
	grp_num_in_stn= 0;
	cur_stn_pos= -1;
	cur_grp_pos= -1;
	cur_grp_id= 0;
	while ( 1 )
	{ 
		lineNo++;
		if( fgets ( buffer, sizeof(buffer), fp ) == NULL )
			break;

		if ( buffer[0] == 'C' )
			continue;

		if ( buffer[0] == '\t' )
			buffer[0] = ' ';

		if ( -1 == GetStringsFromStr(buffer, sizeof(strbuf)/sizeof(strbuf[0]), strbuf) )
		{
			sprintf(err_buf, "DATABASE.DEF line %d: %s: Format error", lineNo, buffer);
			PrintError(err_buf);
		}

		if ( !strcmp(strbuf[0], "") )
			continue;
		
		if ( !strcmp(strbuf[0], "END") )
			break;

		if ( !strcmp(strbuf[0], "STATION") & !strcmp(strbuf[1], "NAME") )
		{
			if(grp_valid==1)
			{
				printf("\n%-10s%-10s id=%02d  type=%-10s",
					db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpId,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpTypeName);

#ifdef DEBUG
				kk = db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_offset;
				Swap4Byte(&kk);
				printf("\n%-10sgrp_num= %02d, grp_offset=%03d\n",
					db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName,
					grp_num_in_stn,
					kk);
#endif

				kk= grp_num_in_stn;
				Swap4Byte(&kk);
				db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_num = kk;
				db_ptr->Station_Union.StationEntry[cur_stn_pos].sysLcuId = sysLcuId;
			}

			stn_valid= 0;
			grp_valid= 0;
			cur_grp_id= 0;
			grp_num_in_stn= 0;
			sysLcuId= 0;

			for ( i=0; i < STATION_ENTRY_NUMBER; i++ )
			{
				if(!strcmp(strbuf[2], db_ptr->Station_Union.StationEntry[i].stnName))
				{
					stn_valid= 1;
					cur_stn_pos= i;
					break;
				}
			}
			if(stn_valid==0)
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: can't find the station", 
					lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			kk= cur_grp_pos+1;
			Swap4Byte(&kk);
			db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_offset= kk;
			continue;
		}

		if(stn_valid==0)
			continue;

		if ( !strcmp(strbuf[0], "GROUP") & !strcmp(strbuf[1], "NAME") )
		{
			if(grp_valid==1)
				printf("\n%-10s%-10s id=%02d  type=%-10s %-20s",
					db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpId,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpTypeName,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpComment);

			grp_valid= 0;

			if(grp_num_in_stn>=GROUP_ENTRY_NUM_PER_STATION)
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: Arrive max group number in station %d", 
					lineNo, buffer, GROUP_ENTRY_NUM_PER_STATION);
				PrintError(err_buf);
				break;
			}

			if(strlen(strbuf[2])>=GROUP_NAME_SIZE)
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: group name too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[2][GROUP_NAME_SIZE-1]=0;
			}

			grp_valid= 1;

			if(cur_grp_id>1)
			{
				kk= db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_offset;
				Swap4Byte(&kk);
				for(i=kk; i<=cur_grp_pos; i++)
				{
					if(!strcmp(strbuf[2], ""))
						break;
					if(!strcmp(db_ptr->Group_Union.GroupEntry[i].grpName, strbuf[2]))
					{
						grp_valid= 0;
						sprintf(err_buf, "DATABASE.DEF line %d: %s: group name repeat", 
							lineNo, buffer);
						PrintError(err_buf);
						break;
					}
				}
			}

			if(grp_valid==0)
				continue;

			cur_grp_pos++;
			cur_grp_id++;
			grp_num_in_stn++;
			strcpy(db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName, strbuf[2]);
			GetNameStrHash(db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName, 
				&db_ptr->Group_Union.GroupEntry[cur_grp_pos].hash_key);
			Swap4Byte(&db_ptr->Group_Union.GroupEntry[cur_grp_pos].hash_key);
			db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpId= cur_grp_id;
			continue;
		}

		if(grp_valid==0)
			continue;

		if ( !strcmp(strbuf[0], "GROUP_ID") )
		{
			grp_id= atoi(strbuf[1]);
			if(grp_id != db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpId)
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: grp_id error", 
					lineNo, buffer);
				PrintError(err_buf);
			}
			continue;
		}

		else if ( !strcmp(strbuf[0], "LONGNAME") )
		{
			if(strlen(strbuf[1])>=GROUP_LONGNAME_SIZE)
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: group longname too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[1][GROUP_LONGNAME_SIZE-1]=0;
			}
			strcpy(db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpLongname, strbuf[1]);
			continue;
		}

		else if ( !strcmp(strbuf[0], "RTU_TYPE") )
		{
			if(strlen(strbuf[1])>=GROUP_TYPENAME_SIZE)
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: group typename too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[1][GROUP_TYPENAME_SIZE-1]=0;
			}
			strcpy(db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpTypeName, strbuf[1]);
			if(!strcmp(strbuf[1], "SYS"))
			{
				if(sysLcuId>0)
				{
					sprintf(err_buf, "DATABASE.DEF line %d: %s: sys lcu define repeat", 
						lineNo, buffer);
					PrintError(err_buf);
				}
				db_ptr->Group_Union.GroupEntry[cur_grp_pos].sysLcuFlag= 1;
				sysLcuId= cur_grp_id;
			}
			else
				db_ptr->Group_Union.GroupEntry[cur_grp_pos].sysLcuFlag= 0;
			if(strcmp(strbuf[1], "SYS") && strcmp(strbuf[1], "RTP") 
				&& strcmp(strbuf[1], "QUANTUM") && strcmp(strbuf[1], "PREMIUM") 
				&& strcmp(strbuf[1], "SIEMENS") && strcmp(strbuf[1], "GE9030")
				&& strcmp(strbuf[1], "GE9070") && strcmp(strbuf[1], "GE"))
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: unsupported lcu type define", 
					lineNo, buffer);
				PrintError(err_buf);
			}

			continue;
		}

		else if ( !strcmp(strbuf[0], "COMMENT") )
		{
			if(strlen(strbuf[1])>=GROUP_COMMENT_SIZE)
			{
				sprintf(err_buf, "DATABASE.DEF line %d: %s: group comment too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[1][GROUP_COMMENT_SIZE-1]=0;
			}
			strcpy(db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpComment, strbuf[1]);
			continue;
		}
	}

	if(grp_valid==1)
		printf("\n%-10s%-10s id=%02d  type=%-10s %-20s",
			db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName,
			db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName,
			db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpId,
			db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpTypeName,
			db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpComment);
	if(stn_valid==1)
	{
#ifdef DEBUG
		kk= db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_offset;
		Swap4Byte(&kk);
		printf("\n%-10sgrp_num= %02d, grp_offset=%03d\n",
			db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName,
			grp_num_in_stn,
			kk);
#endif

		kk= grp_num_in_stn;
		Swap4Byte(&kk);
		db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_num = kk;
		db_ptr->Station_Union.StationEntry[cur_stn_pos].sysLcuId = sysLcuId;
	}

#ifdef	DEBUG
	kk = cur_grp_pos+1;
	printf("\ntotal group num = %d", kk);
#endif

	Swap4Byte(&kk);
	db_ptr->Head_Union.head.grp_sum = kk;

	fclose ( fp );

	return 0;
}



int 	cur_imp_pos;
int 	cur_ana_pos;
int 	cur_status_pos;
int 	cur_chara_pos;
int 	cur_calc_pos;
int 	cur_curve_pos;
int 	cur_devobj_pos;
UCHAR	cur_stn_id;
UCHAR	cur_dev_id;

float	plc_hi_code;

void GetPlcHiCodeFromGroupType(char *grpTypeName)
{
	if(strcmp(grpTypeName, "QUANTUM")==0)
		plc_hi_code= 16000;
	else if(strcmp(grpTypeName, "PREMIUM")==0)
		plc_hi_code= 16000;
	else if(strcmp(grpTypeName, "GE9030")==0)
		plc_hi_code= 32000;
	else if(strcmp(grpTypeName, "GE9070")==0)
		plc_hi_code= 32000;
	else if(strcmp(grpTypeName, "GE")==0)
		plc_hi_code= 32000;
	else
		plc_hi_code= 1;
}


int	GenerateDatabase (void)
{
	int		i, kk, lineNo;
	int		cur_stn_pos;
	int		cur_grp_pos;
	int		cur_rec_pos;	
	int		rec_num_in_grp;
	int		stn_num, grp_num, grp_offset;

	char	buffer[256];
	char	err_buf[256];
	FILE	*fp;
	char	gen_filename[256];
	char	rec_filename[256];
	char	strbuf[5][MAX_CHAR_NUM_IN_LINE];

	printf ( "\n\nNow define points, waiting ... " );

	sprintf(gen_filename, "../gen/%s", RECORD_DEF_FILE );

	fp = fopen ( gen_filename, "r" );
	if ( fp == ( FILE *) 0 )
	{
		printf ( "\nOpen database gen file failed!\n");
		return -1;
	}

	stn_num= db_ptr->Head_Union.head.stn_num;
	Swap4Byte(&stn_num);


	cur_imp_pos=-1;
	cur_ana_pos=-1;
	cur_status_pos=-1;
	cur_chara_pos=-1;
	cur_calc_pos=-1;
	cur_curve_pos=-1;
	cur_devobj_pos=-1;

	
	lineNo= 0;
	cur_stn_pos=-1;
	cur_grp_pos=-1;
	cur_rec_pos=-1;	
	rec_num_in_grp=-1;
	while ( 1 )
	{ 
		lineNo++;
		if( fgets ( buffer, sizeof(buffer), fp ) == NULL )
			break;

		if ( buffer[0] == 'C' )
			continue;

		if ( buffer[0] == '\t' )
			buffer[0] = ' ';

		if ( -1 == GetStringsFromStr(buffer, sizeof(strbuf)/sizeof(strbuf[0]), strbuf) )
		{
			sprintf(err_buf, "DATABASE.GEN line %d: %s: Format error", lineNo, buffer);
			PrintError(err_buf);
		}

		if ( !strcmp(strbuf[0], "") )
			continue;
		
		if ( !strcmp(strbuf[0], "END") )
			break;

		if ( !strcmp(strbuf[0], "STATION") & !strcmp(strbuf[1], "NAME") )
		{
			if(rec_num_in_grp>=0)
			{
#ifdef	DEBUG
				kk= db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_offset;
				Swap4Byte(&kk);
				printf("\n%02d %-10s rec_num=%03d  rec_offset=%03d", cur_grp_pos,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName,
					rec_num_in_grp, kk);
#endif

				kk= rec_num_in_grp;
				Swap4Byte(&kk);
				db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_num = kk;
			}

			rec_num_in_grp= -1;
			cur_stn_pos= -1;
			cur_grp_pos= -1;

			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_buf, "DATABASE.GEN line %d: %s: No station name", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			for(i=0; i<stn_num; i++ )
			{
				if(!strcmp(strbuf[2], db_ptr->Station_Union.StationEntry[i].stnName))
				{
					cur_stn_pos= i;
					cur_stn_id= db_ptr->Station_Union.StationEntry[i].stnId;
					break;
				}
			}
			if(cur_stn_pos==-1)
			{
				sprintf(err_buf, "DATABASE.GEN line %d: %s: can't find the station", 
					lineNo, buffer);
				PrintError(err_buf);
				continue;
			}
			continue;
		}

		if(cur_stn_pos==-1)
			continue;

		if ( !strcmp(strbuf[0], "GROUP") & !strcmp(strbuf[1], "NAME") )
		{
			if(rec_num_in_grp>=0)
			{
#ifdef	DEBUG
				kk= db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_offset;
				Swap4Byte(&kk);
				printf("\n%02d %-10s rec_num=%03d  rec_offset=%03d", cur_grp_pos,
					db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName,
					rec_num_in_grp, kk);
#endif

				kk= rec_num_in_grp;
				Swap4Byte(&kk);
				db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_num = kk;
			}

			rec_num_in_grp= -1;
			cur_grp_pos= -1;

			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_buf, "DATABASE.GEN line %d: %s: No group name", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			grp_offset= db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_offset;
			Swap4Byte(&grp_offset);
			grp_num= db_ptr->Station_Union.StationEntry[cur_stn_pos].grp_num;
			Swap4Byte(&grp_num);
			for(i=grp_offset; i<(grp_offset+grp_num); i++)
			{
				if(!strcmp(db_ptr->Group_Union.GroupEntry[i].grpName, strbuf[2]))
				{
					cur_grp_pos= i;
					cur_dev_id= db_ptr->Group_Union.GroupEntry[i].grpId;
					break;
				}
			}
			if(cur_grp_pos==-1)
			{
				sprintf(err_buf, "DATABASE.GEN line %d: %s: can't find the group", 
					lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			rec_num_in_grp = 0;
			kk= cur_rec_pos+1;
			Swap4Byte(&kk);
			db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_offset = kk;
			GetPlcHiCodeFromGroupType(db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpTypeName);
#ifdef	DEBUG
			printf("\n\n%-10s %-10s", 
				db_ptr->Station_Union.StationEntry[cur_stn_pos].stnName, strbuf[2] );
#endif
			
			continue;
		}
		if ( !strcmp(strbuf[0], "INCLUDE") )
		{
			if(cur_grp_pos==-1)
				continue;

			if(cur_rec_pos>=(RECORD_ENTRY_NUMBER-1))
			{
				sprintf(err_buf, "DATABASE.GEN line %d: %s: Arrive max rec number %d", 
					lineNo, buffer, RECORD_ENTRY_NUMBER);
				PrintError(err_buf);
				break;
			}

			cur_rec_pos++;
			rec_num_in_grp++;
			strcpy ( rec_filename, strbuf[1] );

#ifdef	DEBUG
			printf("\n%03d  %s", cur_rec_pos, strbuf[1]);
#endif

			if(-1==GetEntries( cur_rec_pos, rec_filename ))
			{
				sprintf(err_buf, "DATABASE.GEN line %d: %s: error, exit", 
					lineNo, buffer);
				PrintError(err_buf);
				exit(-1);
			}
			
			for(kk=1;kk<rec_num_in_grp;kk++)
			{
				if(db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType
					==db_ptr->Record_Union.RecordEntry[cur_rec_pos-kk].recType)
				{
					sprintf(err_buf, "DATABASE.GEN line %d: %s: Record type repeat %d", 
						lineNo, buffer, db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType);
					PrintError(err_buf);
				}
			}
		}

	}

	if(rec_num_in_grp>=0)
	{
#ifdef	DEBUG
		kk= db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_offset;
		Swap4Byte(&kk);
		printf("\n%02d %-10s rec_num=%03d  rec_offset=%03d", cur_grp_pos,
			db_ptr->Group_Union.GroupEntry[cur_grp_pos].grpName,
			rec_num_in_grp, kk);
#endif

		kk= rec_num_in_grp;
		Swap4Byte(&kk);
		db_ptr->Group_Union.GroupEntry[cur_grp_pos].rec_num = kk;
	}

	kk = cur_rec_pos+1;
	printf("\ntotal rec num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.rec_sum = kk;
	kk = cur_status_pos+1;
	printf("\ntotal status num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.status_sum= kk;
	kk = cur_ana_pos+1;
	printf("\ntotal ana num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.anlg_sum= kk;
	kk = cur_imp_pos+1;
	printf("\ntotal imp num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.imp_sum= kk;
	kk = cur_calc_pos+1;
	printf("\ntotal calc num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.calc_sum= kk;
	kk = cur_chara_pos+1;
	printf("\ntotal chara num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.chara_sum= kk;
	kk = cur_curve_pos+1;
	printf("\ntotal curve num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.curve_sum= kk;
	kk = cur_devobj_pos+1;
	printf("\ntotal devobj num = %d", kk);
	Swap4Byte(&kk);
	db_ptr->Head_Union.head.devobj_sum= kk;

	fclose ( fp );

	return 0;
}




