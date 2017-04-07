

#include    <time.h>


#include	"../inc/os_type_def.h"
#include	"../inc/dms_structure_def.h"
#include	"../inc/dms_text_def.h"
#include	"../inc/dms_com.h"
#include	"../inc/dms_functions.h"
#include	"../inc/dms_util_def.h"
#include	"../inc/dms_data_def.h"



int	PrintError(char *buf);
int	GetCurEntryPos(UCHAR data_type);
void RTDB_AI_Initialize(void);
void RTDB_DI_Initialize(int soe_flag);
void RTDB_IMP_Initialize(void);
void RTDB_CALC_Initialize(void);
void RTDB_CHARA_Initialize(void);
void RTDB_CURVE_Initialize(void);
void RTDB_DEVOBJ_Initialize(void);
void RTDB_TIME_PT_Initialize(void);
int	RTDB_EntryNameCheck( int cur_rec_pos, int cur_entry_pos, ENTRY_FIXED fixed[] );
int RTDB_Set_AttributesFromString(void);
int GetProcTypeByName( char *name );
ENTRY_FIXED* GetStartEntryFixedPtr(UCHAR data_type);
int MakeEntryHashTable(ENTRY_FIXED *start_fixed, int entry_num, USHORT hash_pn);

extern DBS_GBL_COMMON  *db_ptr;

extern  int 	cur_imp_pos;
extern  int 	cur_ana_pos;
extern  int 	cur_status_pos;
extern  int 	cur_chara_pos;
extern  int 	cur_calc_pos;
extern  int 	cur_curve_pos;
extern  int 	cur_devobj_pos;
extern  int 	cur_time_pos;

extern  float	plc_hi_code;

static	char	rec_filename[128];
static	int		lineNo;
static	char	buffer[256];
static	char	err_buf[256];
static	ENTRY_FIXED *fixed_ptr;
static	RD_VAR_UNION *var_ptr;
static	UCHAR	data_type;

extern  UCHAR	cur_stn_id;
extern  UCHAR	cur_dev_id;
UCHAR			cur_data_type;
USHORT			cur_pt_id;

extern	BOOL	enable_hash_statistic_flag;

#define		MAX_PAIR_POINT_NUM	5000
typedef struct
{
	struct
	{
		UCHAR	stn_id;
		UCHAR	dev_id;
		UCHAR	data_type;
		char	entryName[ENTRY_NAME_SIZE];
	} pair_pt;
	POINTER		self_pt;
} PAIR_POINT;

PAIR_POINT	pair_info[MAX_PAIR_POINT_NUM];
int			pair_pt_num;
int			cur_pair_pt_ptr;



char	strbuf[5][MAX_CHAR_NUM_IN_LINE];




int	GetEntries ( int cur_rec_pos, char *filename )	
{
	int		kk, ret, soe_flag;
	int		entry_num_in_rec;
	int		rec_valid, entry_valid;
	ENTRY_FIXED *start_fixed_ptr;
	USHORT	hash_pn;

	FILE	*fp;
	sprintf(buffer, "../gen/%s", filename );
	fp = fopen ( buffer, "r" );
	if ( fp == ( FILE *) 0 )
	{
		printf ( "\nOpen database file %s failed!\n", filename );
		db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType= -1;
		db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_num= 0;
		db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_offset= 0;
		return -1;
	}

	strcpy(rec_filename, filename);

	lineNo= 0;
	rec_valid= 0;
	entry_valid= 0;
	entry_num_in_rec= 0;
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
			sprintf(err_buf, "%s line %d: %s: Format error", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}

		if ( !strcmp(strbuf[0], "") )
			continue;
		
		if ( !strcmp(strbuf[0], "END") )
			break;
		
		if ( !strcmp(strbuf[0], "RECORD") & !strcmp(strbuf[1], "NAME") )
		{
			ret= GetRecTypeByName(strbuf[2], &data_type);
			if(ret==-1)
			{
				sprintf(err_buf, "%s line %d: %s: No such a type", rec_filename, 
					lineNo, buffer);
				PrintError(err_buf);

				db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType= -1;
				db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_num= 0;
				db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_offset= 0;
				return -1;
			}

			db_ptr->Record_Union.RecordEntry[cur_rec_pos].recType= data_type;
			cur_data_type= data_type;
			kk= GetCurEntryPos(data_type)+1;
			Swap4Byte(&kk);
			db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_offset= kk; 
			start_fixed_ptr= GetStartEntryFixedPtr(data_type);
			rec_valid= 1;
			entry_valid= 0;
			entry_num_in_rec= 0;
			continue;
		}

		if(rec_valid==0)
			continue;

		if ( !strcmp ( strbuf[0], "ENTRY" ) & !strcmp ( strbuf[1], "NAME" ) )
		{
			entry_valid= 1;
			cur_pt_id= entry_num_in_rec;
			entry_num_in_rec++;
			cur_pair_pt_ptr= -1;

			switch( data_type )
			{
			case	SOE_TYPE:
			case	POL_TYPE:
			case	OUTS_TYPE:
			case	SYSMSG_TYPE:
				cur_status_pos++;
				if(cur_status_pos>=ENTRY_STATUS_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much status type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_STATUS_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_Status_Union.EntryFixed[cur_status_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_Status_Union.EntryVariable[cur_status_pos];

				RTDB_EntryNameCheck(cur_rec_pos, cur_status_pos, 
					&db_ptr->EntryFixed_Status_Union.EntryFixed[0]);
				if(data_type==SOE_TYPE)
					soe_flag= 1;
				else
					soe_flag= 0;
				RTDB_DI_Initialize(soe_flag);
				break;

			case	ANA_TYPE:
			case	OUTA_TYPE:
			case	PARAM_TYPE:
				cur_ana_pos++;
				if(cur_ana_pos>=ENTRY_ANLG_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much ana type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_ANLG_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_Anlg_Union.EntryFixed[cur_ana_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_Anlg_Union.EntryVariable[cur_ana_pos];
				RTDB_EntryNameCheck(cur_rec_pos, cur_ana_pos, 
					&db_ptr->EntryFixed_Anlg_Union.EntryFixed[0]);
				RTDB_AI_Initialize();
				break;

			case	IMP_TYPE:
				cur_imp_pos++;
				if(cur_imp_pos>=ENTRY_IMP_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much imp type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_IMP_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_Imp_Union.EntryFixed[cur_imp_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_Imp_Union.EntryVariable[cur_imp_pos];
				RTDB_EntryNameCheck(cur_rec_pos, cur_imp_pos, 
					&db_ptr->EntryFixed_Imp_Union.EntryFixed[0]);
				RTDB_IMP_Initialize();
				break;

			case	CALC_TYPE:
				cur_calc_pos++;
				if(cur_calc_pos>=ENTRY_CALC_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much calc type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_ANLG_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_Calc_Union.EntryFixed[cur_calc_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_Calc_Union.EntryVariable[cur_calc_pos];
				RTDB_EntryNameCheck(cur_rec_pos, cur_calc_pos, 
					&db_ptr->EntryFixed_Calc_Union.EntryFixed[0]);
				RTDB_CALC_Initialize();
				break;

			case	CHARA_TYPE:
				cur_chara_pos++;
				if(cur_chara_pos>=ENTRY_CHARA_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much chara type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_CHARA_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_Chara_Union.EntryFixed[cur_chara_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_Chara_Union.EntryVariable[cur_chara_pos];
				RTDB_EntryNameCheck(cur_rec_pos, cur_chara_pos, 
					&db_ptr->EntryFixed_Chara_Union.EntryFixed[0]);
				RTDB_CHARA_Initialize();
				break;

			case	CURVE_TYPE:
				cur_curve_pos++;
				if(cur_curve_pos>=ENTRY_CURVE_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much curve type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_CURVE_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_Curve_Union.EntryFixed[cur_curve_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_Curve_Union.EntryVariable[cur_curve_pos];
				RTDB_EntryNameCheck(cur_rec_pos, cur_curve_pos, 
					&db_ptr->EntryFixed_Curve_Union.EntryFixed[0]);
				RTDB_CURVE_Initialize();
				break;

			case	DEVOBJ_TYPE:
				cur_devobj_pos++;
				if(cur_devobj_pos>=ENTRY_DEVOBJ_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much devobj type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_DEVOBJ_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_DevObj_Union.EntryFixed[cur_devobj_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_DevObj_Union.EntryVariable[cur_devobj_pos];
				RTDB_EntryNameCheck(cur_rec_pos, cur_devobj_pos, 
					&db_ptr->EntryFixed_DevObj_Union.EntryFixed[0]);
				RTDB_DEVOBJ_Initialize();
				break;

			case	TIME_TYPE:
				cur_time_pos++;
				if(cur_time_pos>=ENTRY_TIME_NUMBER)
				{
					sprintf(err_buf, "%s line %d: %s: Too much time type points defined(Maximum %d)", rec_filename, 
						lineNo, buffer, ENTRY_TIME_NUMBER);
					PrintError(err_buf);
					return -1;
				}
				fixed_ptr= 
					&db_ptr->EntryFixed_Time_Union.EntryFixed[cur_time_pos];			
				var_ptr= 
					(RD_VAR_UNION*)&db_ptr->EntryVar_Time_Union.EntryVariable[cur_time_pos];
				RTDB_EntryNameCheck(cur_rec_pos, cur_time_pos, 
					&db_ptr->EntryFixed_Time_Union.EntryFixed[0]);
				RTDB_TIME_PT_Initialize();
				break;
				
			default:
				return -1;
			}
			continue;
		}

		if(entry_valid==0)
			continue;

		RTDB_Set_AttributesFromString();

	}

	fclose ( fp );

	kk= entry_num_in_rec;
	Swap4Byte(&kk);
	db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_num= kk; 

	kk= FindLastPrimeNumber(entry_num_in_rec);
	if(kk==-1)
		hash_pn= 0;
	else
		hash_pn= (USHORT)kk;
	if(-1==MakeEntryHashTable(start_fixed_ptr, entry_num_in_rec, hash_pn))
		return -1;

	Swap2Byte(&hash_pn);
	db_ptr->Record_Union.RecordEntry[cur_rec_pos].hash_pn= hash_pn; 

	kk= db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_offset;
	Swap4Byte(&kk);
#ifdef	DEBUG
	printf("  type= %d, entry_num= %d, entry_offset= %d", 
		data_type, entry_num_in_rec, kk);	
#endif	
	return 0;
}



int	GetCurEntryPos(UCHAR data_type)
{
	switch( data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		return cur_status_pos;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		return cur_ana_pos;

	case	IMP_TYPE:
		return cur_imp_pos;

	case	CALC_TYPE:
		return cur_calc_pos;

	case	CHARA_TYPE:
		return cur_chara_pos;

	case	CURVE_TYPE:
		return cur_curve_pos;

	case	DEVOBJ_TYPE:
		return cur_devobj_pos;

	case	TIME_TYPE:
		return cur_time_pos;
		
	default:
		return -1;
	}
}



ENTRY_FIXED* GetStartEntryFixedPtr(UCHAR data_type)
{
	switch( data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		return 	&db_ptr->EntryFixed_Status_Union.EntryFixed[cur_status_pos+1];

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		return 	&db_ptr->EntryFixed_Anlg_Union.EntryFixed[cur_ana_pos+1];

	case	IMP_TYPE:
		return 	&db_ptr->EntryFixed_Imp_Union.EntryFixed[cur_imp_pos+1];

	case	CALC_TYPE:
		return 	&db_ptr->EntryFixed_Calc_Union.EntryFixed[cur_calc_pos+1];

	case	CHARA_TYPE:
		return 	&db_ptr->EntryFixed_Chara_Union.EntryFixed[cur_chara_pos+1];

	case	CURVE_TYPE:
		return 	&db_ptr->EntryFixed_Curve_Union.EntryFixed[cur_curve_pos+1];

	case	DEVOBJ_TYPE:
		return 	&db_ptr->EntryFixed_DevObj_Union.EntryFixed[cur_devobj_pos+1];

	case	TIME_TYPE:
		return 	&db_ptr->EntryFixed_Time_Union.EntryFixed[cur_time_pos+1];
		
	default:
		return (ENTRY_FIXED*)-1;
	}
}



int RTDB_EntryNameCheck( int cur_rec_pos, int cur_entry_pos, ENTRY_FIXED fixed[] )
{
	int	i, kk;

	if(strlen(strbuf[2])==0)
	{
		sprintf(err_buf, "%s line %d: %s: Entry name is null", rec_filename, lineNo, buffer);
		PrintError(err_buf);
		return -1;
	}
	if(strlen(strbuf[2])>=ENTRY_NAME_SIZE)
	{
		strbuf[2][ENTRY_NAME_SIZE-1]= 0;
		sprintf(err_buf, "%s line %d: %s: Entry name is too long", rec_filename, lineNo, buffer);
		PrintError(err_buf);
		return -1;
	}

	kk= db_ptr->Record_Union.RecordEntry[cur_rec_pos].entry_offset;
	Swap4Byte(&kk);
	for( i=kk; i<cur_entry_pos; i++ )
	{
		if(!strcmp(strbuf[2], fixed[i].entryName))
		{
			sprintf(err_buf, "%s line %d: %s: Entry name is repeat", rec_filename, lineNo, buffer);
			PrintError(err_buf);
			return -1;
		}
	}
	return 0;
}

#define MAX_HASH_REPEAT_TIMES	24

int MakeEntryHashTable(ENTRY_FIXED *start_fixed, int entry_num, USHORT hash_pn)
{
	int		id;
	USHORT	hash_val, idle_pos;
	GULONG	key;
	int		repeat_times[MAX_HASH_REPEAT_TIMES];

	memset(repeat_times, 0 ,sizeof(repeat_times));

	if(hash_pn==0)
		return 0;

	for(id=0; id<entry_num; id++)
	{
		start_fixed[id].hash_index= 0xFFFF;
		start_fixed[id].hash_next= 0xFFFF;
	}

	for(id=0; id<entry_num; id++)
	{
		key= start_fixed[id].hash_key;
		hash_val= key%hash_pn;
		if(start_fixed[hash_val].hash_index!=0xFFFF)
			continue;
		start_fixed[hash_val].hash_index= id;
	}

	idle_pos=0;
	for(id=0; id<entry_num; id++)
	{
		int		times= 0;

		key= start_fixed[id].hash_key;
		hash_val= key%hash_pn;
		if(start_fixed[hash_val].hash_index==id)
		{
			if(enable_hash_statistic_flag)
			{
				repeat_times[times]++;	
			}
			continue;
		}
		for(; idle_pos<entry_num; idle_pos++)
		{
			if(start_fixed[idle_pos].hash_index==0xFFFF)
				break;
		}
		if(idle_pos>=entry_num)
		{
			PrintError("Make hash table error");
			exit(0);
		}
		start_fixed[idle_pos].hash_index= id;
		if(enable_hash_statistic_flag)
		{
			times++;
			if(times>=MAX_HASH_REPEAT_TIMES)
				times= MAX_HASH_REPEAT_TIMES-1;
		}
		while(start_fixed[hash_val].hash_next!=0xFFFF)
		{
			if(enable_hash_statistic_flag)
			{
				times++;
				if(times>=MAX_HASH_REPEAT_TIMES)
					times= MAX_HASH_REPEAT_TIMES-1;
			}
			hash_val= start_fixed[hash_val].hash_next;
		}
		start_fixed[hash_val].hash_next= idle_pos;
		if(enable_hash_statistic_flag)
		{
			repeat_times[times]++;
		}
	}

	if(enable_hash_statistic_flag)
	{
		FILE	*fp;
		if(fp=fopen("/home/ems/h9000/gen/hash_statistic.txt", "a"))
		{
			fprintf(fp, "\n\nentry_num=%d hash=%d\n", entry_num, hash_pn);
			for(id=0; id<MAX_HASH_REPEAT_TIMES; id++)
				fprintf(fp, "%03d ", repeat_times[id]);
			fclose(fp);
		}
	}


	for(id=0; id<entry_num; id++)
	{
		Swap2Byte(&start_fixed[id].hash_index);		
		Swap2Byte(&start_fixed[id].hash_next);		
		Swap4Byte(&start_fixed[id].hash_key);		
	}
	return 0;		
}



void RTDB_Time_Initialize( DATE_TIME *date_time )
{
	GetSysTime(date_time);
	SwapDateTimeByte(date_time);
}



void RTDB_PI_Initialize( IMP_DATA *imp )
{
	imp->day	= 0;
	imp->month	= 0;
	imp->year	= 0;
}


void RTDB_FIXED_Initialize(int soe_flag)
{
	IOST	iost;
	IOATTR	ioattr;

	strcpy(fixed_ptr->entryName, strbuf[2]);
	GetNameStrHash(fixed_ptr->entryName, &fixed_ptr->hash_key);
	fixed_ptr->entryLongname[0]= 0;
	fixed_ptr->dsp_filename[0]= 0;
	fixed_ptr->msg_sel.alm= 0;
	fixed_ptr->msg_sel.cmd= 0;
	fixed_ptr->update_src= 0;

	memset(&iost, 0, sizeof(iost));
	memset(&ioattr, 0, sizeof(ioattr));
	iost.noUpdate= 1;
	iost.viewPoint= 2;
	iost.almEnable= 1;
	iost.scanEnable= 1;
	ioattr.cascade= 0;
	ioattr.hdbRec= 0;

	/*debug*/
	ioattr.onAlm= 1;
	ioattr.offAlm= 1;

	if(soe_flag==1)
		ioattr.soe= 1;
	else
		ioattr.soe= 0;
	Swap2Byte(&iost);
	Swap4Byte(&ioattr);
	fixed_ptr->iost= iost;
	fixed_ptr->ioattr= ioattr;

	fixed_ptr->hard_addr.terminal_no[0]= 0;
	fixed_ptr->hard_addr.cabinet_no[0]= 0;
	fixed_ptr->hard_addr.unit_no[0]= 0;
	fixed_ptr->hard_addr.slot_no[0]= 0;
	fixed_ptr->hard_addr.channel_no[0]= 0;
	fixed_ptr->ctrl_addr= 0;
	fixed_ptr->update_src= 0;
	fixed_ptr->io_group= 0;
	fixed_ptr->pair_pt.stn_id= 0;
	fixed_ptr->pair_pt.dev_id= 0;
	fixed_ptr->pair_pt.data_type= 0;
	fixed_ptr->pair_pt.pt_id= 0;
}



void RTDB_ENG_PARA_Initialize( ANA_ENG_PARA *eng_para )
{
	float	zero = (float)0.0;
	float	one = (float)1.0;
	float	little = (float)0.01;
	float	hi_code= plc_hi_code;

	Swap4Byte(&zero);
	Swap4Byte(&one);
	Swap4Byte(&little);
	Swap4Byte(&hi_code);

	eng_para->D_BAND	=	little;
	eng_para->ENG_CVT	=	one;
	eng_para->HI_RANGE	=	one;
	eng_para->LO_RANGE	=	zero;
	eng_para->ZERO		=	zero;
	eng_para->COMPSATE	=	zero;
	eng_para->HI_CODE	=	hi_code;
	eng_para->LO_CODE	=	zero;
}


void RTDB_LMT_PARA_Initialize( LMT_PARA *lmt_para )
{
	float	hi_lmt = (float)9999.0;
	float	lo_lmt = (float)-9999.0;

	Swap4Byte(&hi_lmt);
	Swap4Byte(&lo_lmt);

	lmt_para->HI_ALARM	=	hi_lmt;
	lmt_para->HI_WARN	=	hi_lmt;
	lmt_para->HI_RECOVER=	0;
	lmt_para->LO_ALARM	=	lo_lmt;
	lmt_para->LO_WARN	=	lo_lmt;
	lmt_para->LO_RECOVER=	0;
	lmt_para->TR_WARN	=	hi_lmt;
	lmt_para->TR_RECOVER=	0;
}


void RTDB_AI_Initialize(void)
{
	RTDB_FIXED_Initialize(0);
	RTDB_Time_Initialize(&var_ptr->anlg.update_time);
	memset(&var_ptr->anlg.value.status, 0, sizeof(var_ptr->anlg.value.status));
	var_ptr->anlg.value.data= 0;
	var_ptr->anlg.fValue= 0;
	var_ptr->anlg.unitName[0]= 0;
	RTDB_ENG_PARA_Initialize( &var_ptr->anlg.eng_para );
	RTDB_LMT_PARA_Initialize( &var_ptr->anlg.lmt_para );
}



void RTDB_DI_Initialize(int soe_flag)
{
	RTDB_FIXED_Initialize(soe_flag);
	RTDB_Time_Initialize(&var_ptr->ind.update_time);
	memset(&var_ptr->ind.change_time, 0, sizeof(var_ptr->ind.change_time));
	memset(&var_ptr->ind.status, 0, sizeof(var_ptr->ind.status));
	var_ptr->ind.stc.norm_open	= 0;
	var_ptr->ind.stc.norm_close	= 0;
	var_ptr->ind.stc.fault_open	= 0;
	var_ptr->ind.stc.fault_close= 0;
}



void RTDB_IMP_Initialize(void)
{
	FLOAT	fone= 1.0;
	GULONG	max = (GULONG)-1;
	GULONG	rate= 1000000;

	RTDB_FIXED_Initialize(0);
	RTDB_Time_Initialize(&var_ptr->imp.entry.update_time);

	var_ptr->imp.entry.rt_value	=	0;
	memset(&var_ptr->imp.entry.status, 0, sizeof(var_ptr->imp.entry.status));
	var_ptr->imp.entry.base_value= 0;
	var_ptr->imp.entry.d_accum= 0.0;
	Swap4Byte(&fone);
	var_ptr->imp.entry.eng_para.ENG_CVT	= fone;
	var_ptr->imp.entry.eng_para.PT	= fone;
	var_ptr->imp.entry.eng_para.CT	= fone;
	Swap4Byte(&max);
	var_ptr->imp.entry.eng_para.CONSTANT	= max;
	Swap4Byte(&rate);
	var_ptr->imp.entry.eng_para.DAY_RATE	= rate;
	var_ptr->imp.entry.eng_para.YEAR_RATE	= rate;
	RTDB_PI_Initialize( &var_ptr->imp.entry.init );
	RTDB_PI_Initialize( &var_ptr->imp.entry.last );
	RTDB_PI_Initialize( &var_ptr->imp.entry.ppeak );
	RTDB_PI_Initialize( &var_ptr->imp.entry.peak );
	RTDB_PI_Initialize( &var_ptr->imp.entry.valley );
	RTDB_PI_Initialize( &var_ptr->imp.entry.normal );
	RTDB_PI_Initialize( &var_ptr->imp.entry.sum );
}


void RTDB_CALC_Initialize(void)
{
	RTDB_FIXED_Initialize(0);
	RTDB_Time_Initialize(&var_ptr->calc.update_time);
	memset(&var_ptr->calc.value.status, 0, sizeof(var_ptr->calc.value.status));
	var_ptr->calc.value.data= 0;
	var_ptr->calc.fValue= 0;
	var_ptr->calc.unitName[0]= 0;
	RTDB_ENG_PARA_Initialize( &var_ptr->calc.eng_para );
	RTDB_LMT_PARA_Initialize( &var_ptr->calc.lmt_para );
}



void RTDB_CHARA_Initialize(void)
{
	RTDB_FIXED_Initialize(0);
	RTDB_Time_Initialize(&var_ptr->chara.update_time);

	var_ptr->chara.chara_data.max_no	= 0;
	var_ptr->chara.chara_data.min_no	= 0;
	var_ptr->chara.chara_data.max_rate_no	= 0;
	var_ptr->chara.chara_data.min_rate_no	= 0;
	var_ptr->chara.chara_data.max_value = 0.0;
	var_ptr->chara.chara_data.min_value = 0.0;
	var_ptr->chara.chara_data.avg_value = 0.0;
	var_ptr->chara.chara_data.std_value = 0.0;
	var_ptr->chara.chara_data.max_rate = 0.0;
	var_ptr->chara.chara_data.min_rate = 0.0;
}



void RTDB_CURVE_Initialize(void)
{
	USHORT		num= MAX_CURVE_SEG_NUM;
	int			i;

	RTDB_FIXED_Initialize(0);
	RTDB_Time_Initialize(&var_ptr->curve.update_time);
	RTDB_ENG_PARA_Initialize( &var_ptr->curve.eng_para );
	Swap2Byte(&num);
	var_ptr->curve.seg_num= num;
	for(i=0; i<MAX_CURVE_SEG_NUM; i++)
		var_ptr->curve.data.value[i]= 0.0;
}


void RTDB_DEVOBJ_Initialize(void)
{
	RTDB_FIXED_Initialize(0);
	RTDB_Time_Initialize(&var_ptr->devobj.update_time);
	memset(&var_ptr->devobj.cmd_time, 0, sizeof(var_ptr->devobj.cmd_time));
	var_ptr->devobj.usr_name[0]= 0;
	var_ptr->devobj.host_id= 0;
	memset(&var_ptr->devobj.status, 0, sizeof(var_ptr->devobj.status));
	memset(&var_ptr->devobj.data, 0, sizeof(var_ptr->devobj.data));
	memset(&var_ptr->devobj.fb_status1, 0, sizeof(var_ptr->devobj.fb_status1));
	memset(&var_ptr->devobj.fb_status2, 0, sizeof(var_ptr->devobj.fb_status2));
	memset(&var_ptr->devobj.param, 0, sizeof(var_ptr->devobj.param));
}

void RTDB_TIME_PT_Initialize(void)
{
	RTDB_FIXED_Initialize(0);
	memset(&var_ptr->time, 0, sizeof(var_ptr->time));
}


int RTDB_Set_StrAttrFromString(void)
{
	if( !strcmp( strbuf[0], "LONGNAME" ) )
	{
		if( strlen( strbuf[1] ) >= ENTRY_LONGNAME_SIZE )
		{
			strbuf[1][ENTRY_LONGNAME_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: Longname is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->entryLongname, strbuf[1] );
		return 0;
	}
#ifdef	EXTEND_LANGUAGE_SYSTEM
	if( !strcmp( strbuf[0], "EXLONGNAME" ) )
	{
		if( strlen( strbuf[1] ) >= ENTRY_EXLONGNAME_SIZE )
		{
			strbuf[1][ENTRY_EXLONGNAME_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: ExLongname is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->entryExLongname, strbuf[1] );
		return 0;
	}
#endif

	else if( !strcmp( strbuf[0], "DSP_FILE" ) )
	{
		if( strlen( strbuf[1] ) >= DSP_FILENAME_SIZE )
		{
			strbuf[1][DSP_FILENAME_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: Dsp_file name is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->dsp_filename, strbuf[1] );
		return 0;
	}
	else if( !strcmp( strbuf[0], "TERMINAL_NO" ) )
	{
		if( strlen( strbuf[1] ) >= TERMINAL_NO_SIZE )
		{
			strbuf[1][TERMINAL_NO_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: Terminal_no name is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->hard_addr.terminal_no, strbuf[1] );
		return 0;
	}
	else if( !strcmp( strbuf[0], "CABINET_NO" ) )
	{
		if( strlen( strbuf[1] ) >= CABINET_NO_SIZE )
		{
			strbuf[1][CABINET_NO_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: Cabinet_no name is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->hard_addr.cabinet_no, strbuf[1] );
		return 0;
	}
	else if( !strcmp( strbuf[0], "UNIT_NO" ) )
	{
		if( strlen( strbuf[1] ) >= UNIT_NO_SIZE )
		{
			strbuf[1][UNIT_NO_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: Unit_no name is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->hard_addr.unit_no, strbuf[1] );
		return 0;
	}
	else if( !strcmp( strbuf[0], "SLOT_NO" ) )
	{
		if( strlen( strbuf[1] ) >= SLOT_NO_SIZE )
		{
			strbuf[1][SLOT_NO_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: Slot_no name is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->hard_addr.slot_no, strbuf[1] );
		return 0;
	}
	else if( !strcmp( strbuf[0], "CHANNEL_NO" ) )
	{
		if( strlen( strbuf[1] ) >= CHANNEL_NO_SIZE )
		{
			strbuf[1][CHANNEL_NO_SIZE-1] =0;  
			sprintf(err_buf, "%s line %d: %s: Channel_no name is too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		strcpy( fixed_ptr->hard_addr.channel_no, strbuf[1] );
		return 0;
	}
	return -1;
}



int RTDB_Set_ProctypeFromString(void)
{
	IOATTR	ioattr;
	GULONG	procType;
	
	ioattr= fixed_ptr->ioattr;
	Swap4Byte(&ioattr);

	procType= GetProcTypeByName( strbuf[1] );
	if(procType==-1)
	{
		sprintf(err_buf, "%s line %d: %s: Unknown proType", rec_filename, lineNo, buffer);
		PrintError(err_buf);
		return -1;
	}

	ioattr.procType= procType;
	Swap4Byte(&ioattr);
	fixed_ptr->ioattr= ioattr;
	return 0;
}


int RTDB_Set_BOOL_AttributesFromString(BOOL value)
{
	IOATTR	ioattr;
	
	ioattr= fixed_ptr->ioattr;
	Swap4Byte(&ioattr);

	if( !strcmp( strbuf[0], "STATISTIC" ) )
		ioattr.statistic	= value;
	else if( !strcmp( strbuf[0], "FAULT_PNT" ) )
		ioattr.fault = value;
	else if( !strcmp( strbuf[0], "FAIL_PNT" ) )
		ioattr.fail = value;
	else if( !strcmp( strbuf[0], "SBO" ) ) /*reserved for old format*/
		ioattr.ctrl = value;
	else if( !strcmp( strbuf[0], "CONTROL" ) )
		ioattr.ctrl = value;
	else if( !strcmp( strbuf[0], "SOE" ) )
		ioattr.soe = value;
	else if( !strcmp( strbuf[0], "TREND_ALM" ) )
		ioattr.trendAlm = value;
	else if( !strcmp( strbuf[0], "KEY_SIGNAL" ) )
		ioattr.keySignal = value;
	else if( !strcmp( strbuf[0], "EN_FAIL_REC" ) )
		ioattr.faultRec = value;
	else if( !strcmp( strbuf[0], "AUDIO_ALM" ) )
		ioattr.audAlm = value;
	else if( !strcmp( strbuf[0], "REVERSE_PNT" ) )
		ioattr.normalClose = value;
	else if( !strcmp( strbuf[0], "NORMAL_CLOSE" ) )
		ioattr.normalClose = value;
	else if( !strcmp( strbuf[0], "DOUBLE_PNT" ) )
		ioattr.doublePt = value;
	else if( !strcmp( strbuf[0], "VIRTUAL_PNT" ) )
		ioattr.sysVirtual = value;
	else if( !strcmp( strbuf[0], "LCU_VIRTUAL" ) )
		ioattr.lcuVirtual = value;
	else if( !strcmp( strbuf[0], "CASCADE_PNT" ) )
		ioattr.cascade = value;
	else if( !strcmp( strbuf[0], "HDB_REC" ) )
		ioattr.hdbRec = value;
	else if( !strcmp( strbuf[0], "INTO_HDB" ) )	 /*reserved for old format*/
		ioattr.hdbRec = value;
	else if( !strcmp( strbuf[0], "HDB_CURVE" ) )
		ioattr.hdbCurve = value;
	else if( !strcmp( strbuf[0], "ALM_CBA" ) )
		ioattr.almCba = value;
	else if( !strcmp( strbuf[0], "ON_ALM" ) )
		ioattr.onAlm = value;
	else if( !strcmp( strbuf[0], "OFF_ALM" ) )
		ioattr.offAlm = value;
	else if( !strcmp( strbuf[0], "NORM_STAT" ) )
		ioattr.normalState = value;

	Swap4Byte(&ioattr);
	fixed_ptr->ioattr= ioattr;
	return 0;
}


int RTDB_Set_Fixed_AttributesFromString(void)
{
	int		ret;
	int		value;
	IOATTR	ioattr;
	IOST	iost;
	USHORT	io_group;
	
	ioattr= fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	iost= fixed_ptr->iost;
	Swap2Byte(&iost);

	ret= RTDB_Set_StrAttrFromString();
	if(ret==0)
		return 0;

	if( !strcmp( strbuf[1], "NO" ) || !strcmp( strbuf[1], "OFF" ) )
	{	
		RTDB_Set_BOOL_AttributesFromString(0);
		return 0;
	}

	else if( !strcmp( strbuf[1], "YES" ) || !strcmp( strbuf[1], "ON" ) )
	{	
		RTDB_Set_BOOL_AttributesFromString(1);
		return 0;
	}
	else if( !strcmp( strbuf[0], "PROCTYPE" ) )
	{
		RTDB_Set_ProctypeFromString();
		return 0;
	}
	else if( !strcmp( strbuf[0], "HDB_PERIOD" ) )
	{
		sscanf(strbuf[1], "%d", &value);
		if(value<0 || value>5)
		{
			sprintf(err_buf, "%s line %d: %s: over range(0-5)", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		else
		{
			ioattr.hdbPeriod= value;
			Swap4Byte(&ioattr);
			fixed_ptr->ioattr= ioattr;
		}
		return 0;
	}
	else if( !strcmp( strbuf[0], "IO_GROUP" ) )
	{
		sscanf(strbuf[1], "%d", &value);
		if(value<0 || value>65535)
		{
			sprintf(err_buf, "%s line %d: %s: over range(0-65535)", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		else
		{
			io_group= (USHORT)value;
			Swap2Byte(&io_group);
			fixed_ptr->io_group= io_group;
		}
		return 0;
	}
	else if( !strcmp( strbuf[0], "DEC_POINT" ) )
	{
		sscanf(strbuf[1], "%d", &value);
		if(value<0 || value>3)
		{
			sprintf(err_buf, "%s line %d: %s: over range(0-3)", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		else
		{
			iost.viewPoint= (USHORT)value;
			Swap2Byte(&iost);
			fixed_ptr->iost= iost;
		}
		return 0;
	}
	else if( !strcmp( strbuf[0], "CMD_MSG" ) )
	{
		sscanf(strbuf[1], "%d", &value);
		if(value<0 || value>=MAX_MSG_GROUP_NUM)
		{
			sprintf(err_buf, "%s line %d: %s: over range(0-%d)", rec_filename, lineNo, buffer, MAX_MSG_GROUP_NUM-1);
			PrintError(err_buf);
		}
		else
			fixed_ptr->msg_sel.cmd= (UCHAR)value;
		return 0;
	}
	else if( !strcmp( strbuf[0], "MSG_DEF" ) )
	{
		sscanf(strbuf[1], "%d", &value);
		if(value<0 || value>=MAX_MSG_GROUP_NUM)
		{
			sprintf(err_buf, "%s line %d: %s: over range(0-%d)", rec_filename, lineNo, buffer, MAX_MSG_GROUP_NUM-1);
			PrintError(err_buf);
		}
		else
			fixed_ptr->msg_sel.alm= (UCHAR)value;
		return 0;
	}
	else if( !strcmp( strbuf[0], "CTRL_ADDR" ) )
	{
		sscanf(strbuf[1], "%d", &value);
		if(value<1)
		{
			sprintf(err_buf, "%s line %d: %s: over range(1-65535 )", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}
		else
		{
			fixed_ptr->ctrl_addr= (USHORT)value;
			Swap2Byte(&fixed_ptr->ctrl_addr);
		}
		return 0;
	}
	else if( !strcmp( strbuf[0], "PAIR_POINT" ) )
	{
		if( pair_pt_num>=MAX_PAIR_POINT_NUM)
		{
			sprintf(err_buf, "%s line %d: %s: Too much double point define", rec_filename, lineNo, buffer);
			PrintError(err_buf);
			return -1;
		}
		if( strlen(strbuf[1])>=ENTRY_NAME_SIZE)
		{
			sprintf(err_buf, "%s line %d: %s: Entry name too long", rec_filename, lineNo, buffer);
			PrintError(err_buf);
			return -1;
		}

		if(cur_pair_pt_ptr==-1)
			cur_pair_pt_ptr= pair_pt_num++;

		strcpy(pair_info[cur_pair_pt_ptr].pair_pt.entryName, strbuf[1]);
		pair_info[cur_pair_pt_ptr].pair_pt.stn_id= cur_stn_id;
		pair_info[cur_pair_pt_ptr].pair_pt.dev_id= cur_dev_id;
		pair_info[cur_pair_pt_ptr].self_pt.stn_id= cur_stn_id;
		pair_info[cur_pair_pt_ptr].self_pt.dev_id= cur_dev_id;
		pair_info[cur_pair_pt_ptr].self_pt.data_type= cur_data_type;
		pair_info[cur_pair_pt_ptr].self_pt.pt_id= cur_pt_id;
		return 0;
	}
	else if( !strcmp( strbuf[0], "PAIR_TYPE" ) )
	{
		if(cur_pair_pt_ptr==-1)
			cur_pair_pt_ptr= pair_pt_num++;

		sscanf(strbuf[1], "%d", &value);
		pair_info[cur_pair_pt_ptr].pair_pt.data_type= (UCHAR)value;
		return 0;
	}

	else if( !strcmp( strbuf[0], "ID" ) )
	{
#define MAX_HDB_ID	655360000
		static int hdb_id[MAX_HDB_ID/32];
		sscanf(strbuf[1], "%d", &value);
		if(value<0 || value>=MAX_HDB_ID)
		{
			sprintf(err_buf, "%s line %d: %s: ID over range(0-%d)", rec_filename, lineNo, buffer, MAX_HDB_ID-1);
			PrintError(err_buf);
			return -1;
		}

		if((hdb_id[value/32] & (1<<(value%32))) ^ 1)
			hdb_id[value/32] |= (1<<(value%32));
		else
		{
			sprintf(err_buf, "%s line %d: %s: ID repeated", rec_filename, lineNo, buffer);
			PrintError(err_buf);
		}

		return 0;
	}

	return -1;
}



int RTDB_Set_AnlgPara_AttributesFromString(void)
{
	float tmp;
	int	i;
	
	for(i=0; i<4; i=i+2)
	{
		if(strcmp(strbuf[i+0],"")==0)
			return 0;

		sscanf(strbuf[i+1], "%f", &tmp);
		Swap4Byte(&tmp);

		if( !strcmp( strbuf[i+0], "HH_LMT" ) )
			var_ptr->anlg.lmt_para.HI_ALARM= tmp;
		else if( !strcmp( strbuf[i+0], "H_LMT" ) )
			var_ptr->anlg.lmt_para.HI_WARN= tmp;
		else if( !strcmp( strbuf[i+0], "TR_LIM" ) )
			var_ptr->anlg.lmt_para.TR_WARN= tmp;
		else if( !strcmp( strbuf[i+0], "HR_LMT" ) )
			var_ptr->anlg.lmt_para.HI_RECOVER= tmp;
		else if( !strcmp( strbuf[i+0], "L_LMT" ) )
			var_ptr->anlg.lmt_para.LO_WARN= tmp;
		else if( !strcmp( strbuf[i+0], "LL_LMT" ) )
			var_ptr->anlg.lmt_para.LO_ALARM= tmp;
		else if( !strcmp( strbuf[i+0], "RT_LIM" ) )
			var_ptr->anlg.lmt_para.TR_RECOVER= tmp;
		else if( !strcmp( strbuf[i+0], "LR_LMT" ) )
			var_ptr->anlg.lmt_para.LO_RECOVER= tmp;
		else if( !strcmp( strbuf[i+0], "D_BAND" ) )
			var_ptr->anlg.eng_para.D_BAND= tmp;
		else if( !strcmp( strbuf[i+0], "ENG_CVT" ) )
			var_ptr->anlg.eng_para.ENG_CVT= tmp;
		else if( !strcmp( strbuf[i+0], "HI_RANGE" ) )
			var_ptr->anlg.eng_para.HI_RANGE= tmp;
		else if( !strcmp( strbuf[i+0], "LO_RANGE" ) )
			var_ptr->anlg.eng_para.LO_RANGE= tmp;
		else if( !strcmp( strbuf[i+0], "ZERO" ) )
			var_ptr->anlg.eng_para.ZERO= tmp;
		else if( !strcmp( strbuf[i+0], "COMPENSATE" ) )
			var_ptr->anlg.eng_para.COMPSATE= tmp;
		else if( !strcmp( strbuf[i+0], "HI_CODE" ) )
			var_ptr->anlg.eng_para.HI_CODE= tmp;
		else if( !strcmp( strbuf[i+0], "LO_CODE" ) )
			var_ptr->anlg.eng_para.LO_CODE= tmp;
		else
			return -1;
	}
	return 0;
}



int RTDB_Set_CurvePara_AttributesFromString(void)
{
	float tmp;
	int	i;
	
	for(i=0; i<4; i=i+2)
	{
		if(strcmp(strbuf[i+0],"")==0)
			return 0;

		sscanf(strbuf[i+1], "%f", &tmp);
		Swap4Byte(&tmp);

		if( !strcmp( strbuf[i+0], "HI_RANGE" ) )
			var_ptr->curve.eng_para.HI_RANGE= tmp;
		else if( !strcmp( strbuf[i+0], "LO_RANGE" ) )
			var_ptr->curve.eng_para.LO_RANGE= tmp;
		else if( !strcmp( strbuf[i+0], "ENG_CVT" ) )
			var_ptr->curve.eng_para.ENG_CVT= tmp;
		else
			return -1;
	}
	return 0;
}



int RTDB_Set_AttributesFromString(void)
{
	GULONG	iValue;
	FLOAT	fValue;

	if(0 == RTDB_Set_Fixed_AttributesFromString())
		return 0;

	switch( data_type )
	{
	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
	case	CALC_TYPE:
		if( !strcmp( strbuf[0], "UNIT_NAME" ) )
		{
			if( strlen( strbuf[1] ) >= ANLG_UNIT_NAME_SIZE )
			{
				strbuf[1][ANLG_UNIT_NAME_SIZE-1] =0;  
				sprintf(err_buf, "%s line %d: %s: unit name is too long", rec_filename, lineNo, buffer);
				PrintError(err_buf);
			}
			strcpy( var_ptr->anlg.unitName, strbuf[1] );
			return 0;
		}
		else if(0 == RTDB_Set_AnlgPara_AttributesFromString())
			return 0;
		break;

	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		break;

	case	IMP_TYPE:
		if( !strcmp ( strbuf[0], "CVT_F" ) )
		{
			sscanf(strbuf[1], "%f", &fValue);
			var_ptr->imp.entry.eng_para.ENG_CVT= fValue;
			Swap4Byte(&var_ptr->imp.entry.eng_para.ENG_CVT);
			return 0;
		}
		else if( !strcmp ( strbuf[0], "CONSTANT" ) )
		{
			sscanf(strbuf[1], "%d", &iValue);
			var_ptr->imp.entry.eng_para.CONSTANT= iValue;
			Swap4Byte(&var_ptr->imp.entry.eng_para.CONSTANT);
			return 0;
		}
		else if( !strcmp ( strbuf[0], "DAY_RATE" ) )
		{
			sscanf(strbuf[1], "%d", &iValue);
			if(iValue!=1 && iValue!=1000 && iValue!=1000000 && iValue!=1000000,000)
			{
				sprintf(err_buf, "%s line %d: %s: must be 1/1000/1000,000/1000,000,000", 
					rec_filename, lineNo, buffer);
				PrintError(err_buf);
				return -1;
			}
			var_ptr->imp.entry.eng_para.DAY_RATE= iValue;
			Swap4Byte(&var_ptr->imp.entry.eng_para.DAY_RATE);
			return 0;
		}
		else if( !strcmp ( strbuf[0], "YEAR_RATE" ) )
		{
			sscanf(strbuf[1], "%d", &iValue);
			if(iValue!=1 && iValue!=1000 && iValue!=1000000 && iValue!=1000000,000)
			{
				sprintf(err_buf, "%s line %d: %s: must be 1/1000/1000,000/1000,000,000", 
					rec_filename, lineNo, buffer);
				PrintError(err_buf);
				return -1;
			}
			var_ptr->imp.entry.eng_para.YEAR_RATE= iValue;
			Swap4Byte(&var_ptr->imp.entry.eng_para.YEAR_RATE);
			return 0;
		}
		else if( !strcmp ( strbuf[0], "CT" ) )
		{
			sscanf(strbuf[1], "%f", &fValue);
			var_ptr->imp.entry.eng_para.CT= fValue;
			Swap4Byte(&var_ptr->imp.entry.eng_para.CT);
			return 0;
		}
		else if( !strcmp ( strbuf[0], "PT" ) )
		{
			sscanf(strbuf[1], "%f", &fValue);
			var_ptr->imp.entry.eng_para.PT= fValue;
			Swap4Byte(&var_ptr->imp.entry.eng_para.PT);
			return 0;
		}
		break;

	case	CHARA_TYPE:
		if( !strcmp ( strbuf[0], "START_PT_NAME" ) )
		{
			if( strlen( strbuf[1] ) >= POINT_NAME_SIZE )
			{
				strbuf[1][POINT_NAME_SIZE-1] =0;  
				sprintf(err_buf, "%s line %d: %s: point name is too long", rec_filename, lineNo, buffer);
				PrintError(err_buf);
			}
			strcpy( var_ptr->chara.start_pt_name, strbuf[1] );
			return 0;
		}
		else if( !strcmp ( strbuf[0], "PT_NUM" ) )
		{
			sscanf(strbuf[1], "%d", &iValue);
			if(iValue<1 || iValue>255)
			{
				sprintf(err_buf, "%s line %d: %s: pt_num overrange(1~255)", rec_filename, lineNo, buffer);
				PrintError(err_buf);
				return -1;
			}
			var_ptr->chara.pt_num = (UCHAR)iValue;
			return 0;
		}
		break;

	case	CURVE_TYPE:
		if( !strcmp ( strbuf[0], "NUM" ) )
		{
			sscanf(strbuf[1], "%d", &iValue);
			if(iValue>MAX_CURVE_SEG_NUM)
			{
				sprintf(err_buf, "%s line %d: %s: pt_num overrange(0~%d)", 
					rec_filename, lineNo, buffer, MAX_CURVE_SEG_NUM);
				PrintError(err_buf);
				return -1;
			}
			var_ptr->curve.seg_num = iValue;
			Swap2Byte(&var_ptr->curve.seg_num);
			return 0;
		}
		else if(0 == RTDB_Set_CurvePara_AttributesFromString())
			return 0;
		break;

	case	TIME_TYPE:
		if( !strcmp ( strbuf[0], "X" ) )
		{
			sscanf(strbuf[1], "%f", &fValue);
			var_ptr->time.coords.x = fValue;
			Swap4Byte(&var_ptr->time.coords.x);
			return 0;
		}
		else if( !strcmp ( strbuf[0], "Y" ) )
		{
			sscanf(strbuf[1], "%f", &fValue);
			var_ptr->time.coords.y = fValue;
			Swap4Byte(&var_ptr->time.coords.y);
			return 0;
		}
		else if( !strcmp ( strbuf[0], "Z" ) )
		{
			sscanf(strbuf[1], "%f", &fValue);
			var_ptr->time.coords.z = fValue;
			Swap4Byte(&var_ptr->time.coords.z);
			return 0;
		}
		break;
		
	default:
		break;
	}
/*	sprintf(err_buf, "%s line %d: %s: unsupported defination of this type", rec_filename, lineNo, buffer);
	PrintError(err_buf);
*/	return -1;
}


