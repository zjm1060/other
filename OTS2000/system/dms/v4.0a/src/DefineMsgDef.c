
#include	"../inc/os_type_def.h"
#include	"../inc/dms_text_def.h"
#include	"../inc/dms_com.h"
#include	"../inc/dms_functions.h"
#include	"../inc/dms_util_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"


/*#define		DEBUG*/


int	PrintError(char *buf);

extern  DBS_GBL_COMMON  *db_ptr;

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


extern  PAIR_POINT	pair_info[];
extern  int		pair_pt_num;



int DoublePtDatabase(void)
{
	int			i;
	POINTER		self_pt_id, pair_pt_id, pair;
	char		err_buf[256];
	int			status_num;
	BOOL		doublePt, normalClose1, normalClose2;
	IOATTR		ioattr;

	for(i=0; i<pair_pt_num; i++)
	{
		self_pt_id= pair_info[i].self_pt;
		pair_pt_id.stn_id= pair_info[i].pair_pt.stn_id;
		pair_pt_id.dev_id= pair_info[i].pair_pt.dev_id;
		pair_pt_id.data_type= pair_info[i].pair_pt.data_type;
		if(-1==GetEntryIdByName(pair_info[i].pair_pt.entryName, &pair_pt_id))
		{
			sprintf(err_buf, "\nNo pair point %d.%d.%d-%s in double pt %d.%d.%d.%d", 
				pair_pt_id.stn_id, pair_pt_id.dev_id, 
				pair_pt_id.data_type, pair_info[i].pair_pt.entryName,
				self_pt_id.stn_id, self_pt_id.dev_id, 
				self_pt_id.data_type, self_pt_id.pt_id);
			PrintError(err_buf);
			continue;
		}

		if(self_pt_id.data_type==POL_TYPE || self_pt_id.data_type==SOE_TYPE
			|| self_pt_id.data_type==SYSMSG_TYPE)
		{
			if(-1==IsPtDoublePointById(&self_pt_id, &doublePt))
			{
				sprintf(err_buf, "\nNo point %d.%d.%d.%d in double pt define", 
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id);
				PrintError(err_buf);
				continue;
			}
			if(!doublePt)
			{
				sprintf(err_buf, "\n%d.%d.%d.%d isn't double point but defined pair point", 
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id);
				PrintError(err_buf);
				continue;
			}

			if(pair_pt_id.data_type!=POL_TYPE && pair_pt_id.data_type!=SOE_TYPE
				&& pair_pt_id.data_type!=SYSMSG_TYPE)
			{
				sprintf(err_buf, "\n%d.%d.%d.%d pair point datatype error: %d", 
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id, pair_pt_id.data_type);
				PrintError(err_buf);
				continue;
			}

			IsPtDoublePointById(&pair_pt_id, &doublePt);

			if(!doublePt)
			{
				sprintf(err_buf, "\nPair point %d.%d.%d-%s isn't double point in double pt %d.%d.%d.%d", 
					pair_pt_id.stn_id, pair_pt_id.dev_id, 
					pair_pt_id.data_type, pair_info[i].pair_pt.entryName,
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id);
				PrintError(err_buf);
				continue;
			}

			IsPtNormalCloseById(&self_pt_id, &normalClose1);
			IsPtNormalCloseById(&pair_pt_id, &normalClose2);
			if( normalClose1 && normalClose2)
			{
				sprintf(err_buf, "\nTwo pt is all normal close point: %d.%d.%d.%d \
					and %d.%d.%d.%d", 
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id, 
					pair_pt_id.stn_id, pair_pt_id.dev_id, 
					pair_pt_id.data_type, pair_pt_id.pt_id);
				PrintError(err_buf);
				continue;
			}
			if( !normalClose1 && !normalClose2)
			{
				sprintf(err_buf, "\nNo one pt is normal close point: %d.%d.%d.%d \
					and %d.%d.%d.%d", 
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id,
					pair_pt_id.stn_id, pair_pt_id.dev_id, 
					pair_pt_id.data_type, pair_pt_id.pt_id);
				PrintError(err_buf);
				continue;
			}
		}
		else if(self_pt_id.data_type==ANA_TYPE || self_pt_id.data_type==PARAM_TYPE)
		{
			if(pair_pt_id.data_type!=ANA_TYPE && pair_pt_id.data_type!=PARAM_TYPE)
			{
				sprintf(err_buf, "\n%d.%d.%d.%d pair point datatype error: %d", 
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id, pair_pt_id.data_type);
				PrintError(err_buf);
				continue;
			}
		}

		GetPairPtById(&self_pt_id, &pair);
		if(pair.stn_id)
		{
			if(pair.stn_id!=pair_pt_id.stn_id || pair.dev_id!=pair_pt_id.dev_id
				|| pair.data_type!=pair_pt_id.data_type || pair.pt_id!=pair_pt_id.pt_id)
			{
				sprintf(err_buf, "\nPair error in %d.%d.%d.%d and %d.%d.%d.%d", 
					self_pt_id.stn_id, self_pt_id.dev_id, 
					self_pt_id.data_type, self_pt_id.pt_id,
					pair_pt_id.stn_id, pair_pt_id.dev_id, 
					pair_pt_id.data_type, pair_pt_id.pt_id);
				PrintError(err_buf);
				continue;
			}
		}
		SetPairPtById(&self_pt_id, pair_pt_id);
		SetPairPtById(&pair_pt_id, self_pt_id);
	}
	
	status_num= db_ptr->Head_Union.head.status_sum;
	Swap4Byte(&status_num);
	for(i=0; i<status_num; i++)
	{
		ioattr= db_ptr->EntryFixed_Status_Union.EntryFixed[i].ioattr;
		Swap4Byte(&ioattr);
		if(!ioattr.doublePt)
			continue;

		if(!db_ptr->EntryFixed_Status_Union.EntryFixed[i].pair_pt.stn_id)
		{
			sprintf(err_buf, "\nThe point need pair point define: %s", 
				db_ptr->EntryFixed_Status_Union.EntryFixed[i].entryLongname);
			PrintError(err_buf);
			continue;
		}
	}

	return 0;
}



int GetProcTypeByName( char *name )
{
	if ( !strcmp ( name, "RELAY" ) )
		return 0;
	else if ( !strcmp ( name, "BREAKER" ) )  
		return 1;
	else if ( !strncmp ( name, "VALVE", 5 ) )   /*reserved for old format*/
		return 2;
	else if ( !strncmp ( name, "DEVICE", 6 ) )  /*reserved for old format*/
		return 3;
	else if ( !strncmp ( name, "SWITCH", 6 ) )  /*reserved for old format*/
		return 4;
	else if ( !strncmp ( name, "GENSTA", 6 ) )  /*reserved for old format*/
		return 7;
	else if ( !strcmp ( name, "DEVMSG" ) )
		return 8;
	else if ( !strcmp ( name, "LCUMSG" ) )		/*reserved for old format*/
		return 8;
	else if ( !strcmp ( name, "STREAM" ) )
		return 9;
	else if ( !strcmp ( name, "NETMSG" ) )
		return 10;
	else if ( !strncmp ( name, "HOSTMSG", 7 ) ) /*reserved for old format*/
		return 11;
	else if ( !strcmp ( name, "SYSMODE" ) )
		return 12;
	else if ( !strcmp ( name, "ONOFFMSG" ) )
		return 13;
	else if ( !strcmp ( name, "AGCCMD" ) )
		return 14;
	else if ( !strcmp ( name, "CTRLMODE" ) )
		return 15;
	else if ( !strcmp ( name, "AGCMODE" ) )
		return 16;
	else if ( !strcmp ( name, "AVCMODE" ) )
		return 17;
	else if ( !strcmp ( name, "UNITMODE" ) )
		return 18;
	else if ( !strcmp ( name, "LCUMODE" ) )
		return 19;
	else if ( !strcmp ( name, "DEVMODE" ) )
		return 20;
	else if ( !strcmp ( name, "ACTIVE" ) ) /*imp*/
		return 0;
	else if ( !strcmp ( name, "REACTIVE" ) ) /*imp*/
		return 1;
	else
		return -1;
}


int GetCmdTypeByName( char *name )
{
	if ( !strcmp ( name, "C_UNIT" ) )
		return C_UNIT;
	else if ( !strcmp ( name, "C_BREAKER" ) )  
		return C_BREAKER;
	else if ( !strcmp ( name, "C_SWITCH" ) )   
		return C_SWITCH;
	else if ( !strcmp ( name, "C_DEVICE" ) )   
		return C_DEVICE;
	else if ( !strcmp ( name, "C_APOWER" ) )  
		return C_APOWER;
	else if ( !strcmp ( name, "C_RPOWER" ) )  
		return C_RPOWER;
	else if ( !strcmp ( name, "C_GATE" ) )
		return C_GATE;
	else if ( !strcmp ( name, "C_TRANS" ) )
		return C_TRANS;
	else if ( !strcmp ( name, "C_VOLT_STEP" ) )
		return C_VOLT_STEP;
	else if ( !strcmp ( name, "C_SPEED_STEP" ) ) 
		return C_SPEED_STEP;
	else if ( !strcmp ( name, "C_ONOFF_FORCE" ) )
		return C_ONOFF_FORCE;
	else if ( !strcmp ( name, "C_ANA_FORCE" ) )
		return C_ANA_FORCE;
	else if ( !strcmp ( name, "C_LCU_ONOFF" ) )
		return C_LCU_ONOFF;
	else if ( !strcmp ( name, "C_SIGNAL_RESET" ) )
		return C_SIGNAL_RESET;
	else if ( !strcmp ( name, "C_DEV_MODE" ) )
		return C_DEV_MODE;
	else if ( !strcmp ( name, "TIME_SYNC" ) )
		return TIME_SYNC;
	else if ( !strcmp ( name, "C_HOST_SYS" ) )
		return C_HOST_SYS;
	else if ( !strcmp ( name, "C_NET_SWITCH" ) )
		return C_NET_SWITCH;
	else if ( !strcmp ( name, "C_PLC_SWITCH" ) )
		return C_PLC_SWITCH;
	else if ( !strcmp ( name, "C_OP_VALUE_SET" ) )
		return C_OP_VALUE_SET;
	else if ( !strcmp ( name, "C_SYS_MODE_SET" ) )
		return C_SYS_MODE_SET;
	else if ( !strcmp ( name, "C_DATA_POLLING" ) )
		return C_DATA_POLLING;
	else if ( !strcmp ( name, "ANA_PARAM_SET" ) )
		return DPS_ANA_PARAM_SET;
	else if ( !strcmp ( name, "IMP_PARAM_SET" ) )
		return DPS_IMP_PARAM_SET;
	else if ( !strcmp ( name, "C_ONOFF_SET" ) )
		return DPS_C_ONOFF_SET;
	else if ( !strcmp ( name, "IOST_SET" ) )
		return DPS_IOST_SET;
	else if ( !strcmp ( name, "IOATTR_SET" ) )
		return DPS_IOATTR_SET;
	else if ( !strcmp ( name, "IO_GROUP_SET" ) )
		return DPS_IO_GROUP_SET;
	else if ( !strcmp ( name, "MAN_VALUE_SET" ) )
		return DPS_MAN_VALUE_SET;
	else if ( !strcmp ( name, "MAN_STAT_SET" ) )
		return DPS_MAN_STAT_SET;
	else if ( !strcmp ( name, "MAN_CURVE_SET" ) )
		return DPS_MAN_CURVE_SET;
	else if ( !strcmp ( name, "C_LCU_PARAM" ) )
		return C_LCU_PARAM;
	else if ( !strcmp ( name, "C_IO_CTRL_BLOCK" ) )
		return C_IO_CTRL_BLOCK;
	else if ( !strcmp ( name, "MAN_TIME_SET" ) )
		return DPS_MAN_TIME_SET;
	else
		return -1;
}


int GenerateAlmMsgDef(void)
{
	int		i,j,k,lineNo;
	int		procType, groupNo, entryNo;

	char	buffer[256];
	char	err_buf[256];
	FILE	*fp;
	char	strbuf[6][MAX_CHAR_NUM_IN_LINE]; /*5*/
	char	filename[256];

	printf( "\n\nNow start alm msg define..." );

	for(i=0; i<MAX_PROCTYPE_NUM; i++)
		for(j=0; j<MAX_MSG_GROUP_NUM; j++)
			for(k=0; k<MAX_IND_STATE_NUM; k++)
			{
				sprintf(db_ptr->msg_def[i].group[j].def[k], "Err%d", k);
#ifdef EXTEND_LANGUAGE_SYSTEM
				if(GetExLangState())
					sprintf(db_ptr->msg_def[i].group[j].Exdef[k], "Err%d", k);
#endif
			}

	sprintf(filename, "../gen/%s", MSG_DEF_FILE);
	fp = fopen ( filename, "r" );
	if ( fp == ( FILE *) 0 )
	{
		printf ( "\nFailed to open %s!\n", filename );
		return -1;
	}

	procType= -1;
	groupNo= -1;
	lineNo= 0;
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
			sprintf(err_buf, "MSG_DEF.TXT line %d: %s: Format error", lineNo, buffer);
			PrintError(err_buf);
		}

		if ( !strcmp(strbuf[0], "") )
			continue;
		
		if ( !strcmp(strbuf[0], "END") )
			break;

		if ( !strcmp(strbuf[0], "PROCTYPE") & !strcmp(strbuf[1], "NAME") )
		{
			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_buf, "MSG_DEF.TXT line %d: %s: Need procType name", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			procType= GetProcTypeByName(strbuf[2]);
			if(-1 == procType)
			{
				sprintf(err_buf, "MSG_DEF.TXT line %d: %s: Unknown procType", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			continue;
		}

		if(procType==-1)
			continue;

		if ( !strcmp(strbuf[0], "GROUP") & !strcmp(strbuf[1], "NAME") )
		{
			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_buf, "MSG_DEF.TXT line %d: %s: Need group name", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			if(1!=sscanf(strbuf[2], "%d", &groupNo) || groupNo<0 || groupNo>=MAX_MSG_GROUP_NUM)
			{
				sprintf(err_buf, "MSG_DEF.TXT line %d: %s: groupNo is overrange(0~7)", 
					lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			continue;
		}

		if(groupNo==-1)
			continue;

		if ( !strcmp(strbuf[0], "STATE") )
		{
			if(1!=sscanf(strbuf[1], "%d", &entryNo) || entryNo<0 || entryNo>=MAX_IND_STATE_NUM)
			{
				sprintf(err_buf, "MSG_DEF.TXT line %d: %s: entryNo is overrange(0~15)", 
					lineNo, buffer);
				PrintError(err_buf);
				continue;
			}
			if(strlen(strbuf[2])>=MAX_ALM_MSG_SIZE)
			{
				sprintf(err_buf, "MSG_DEF.TXT line %d: %s: msg too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[2][MAX_ALM_MSG_SIZE-1]=0;
			}
			strcpy(db_ptr->msg_def[procType].group[groupNo].def[entryNo], strbuf[2]);
#ifdef	EXTEND_LANGUAGE_SYSTEM
			if(strlen(strbuf[3])>=MAX_ALM_MSG_SIZE)
			{
				sprintf(err_buf, "MSG_DEF.TXT line %d: %s: msg too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[3][MAX_ALM_MSG_SIZE-1]=0;
			}
			strcpy(db_ptr->msg_def[procType].group[groupNo].Exdef[entryNo], strbuf[3]);
#endif			
		}
	}

	fclose ( fp );

	return 0;
}



int GenerateCmdMsgDef(void)
{
	int		i,j,k,lineNo;
	int		cmdPos, cmdType, groupNo, entryNo;

	char	buffer[256];
	char	err_buf[256];
	FILE	*fp;
	char	strbuf[5][MAX_CHAR_NUM_IN_LINE];
	char	filename[256];

	printf( "\n\nNow start cmd msg define..." );

	for(i=0; i<MAX_CMDTYPE_NUM; i++)
	{
		db_ptr->cmd_msg[i].cmdType= -1;
		for(j=0; j<MAX_MSG_GROUP_NUM; j++)
			for(k=0; k<MAX_CMD_OPER_TYPE_NUM; k++)
			{
				sprintf(db_ptr->cmd_msg[i].group[j].def[k], "Err%d", k);
#ifdef EXTEND_LANGUAGE_SYSTEM
				if(GetExLangState())
					sprintf(db_ptr->cmd_msg[i].group[j].Exdef[k], "Err%d", k);
#endif
			}
	}

	sprintf(filename, "../gen/%s", CMD_MSG_FILE);
	fp = fopen ( filename, "r" );
	if ( fp == ( FILE *) 0 )
	{
		printf ( "\nFailed to open %s!\n", filename );
		return -1;
	}

	cmdPos= -1;
	cmdType= -1;
	groupNo= -1;
	lineNo= 0;
	while ( 1 )
	{ 
		lineNo++;
		if( fgets ( buffer, sizeof(buffer), fp ) == NULL )
			break;

		if ( buffer[0] == '\t' )
			buffer[0] = ' ';

		if ( -1 == GetStringsFromStr(buffer, sizeof(strbuf)/sizeof(strbuf[0]), strbuf) )
		{
			sprintf(err_buf, "CMD_MSG.TXT line %d: %s: Format error", lineNo, buffer);
			PrintError(err_buf);
		}

		if ( !strcmp(strbuf[0], "") )
			continue;
		
		if ( !strcmp(strbuf[0], "END") )
			break;

		if ( !strcmp(strbuf[0], "CMDTYPE") & !strcmp(strbuf[1], "NAME") )
		{
			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: Need cmdType name", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			cmdType= GetCmdTypeByName(strbuf[2]);
			if(-1 == cmdType)
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: Unknown cmdType", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			cmdPos= -1;
			for(i=0; i<MAX_CMDTYPE_NUM; i++)
			{
				if(db_ptr->cmd_msg[i].cmdType==cmdType)
				{
					cmdPos= i;
					sprintf(err_buf, "CMD_MSG.TXT line %d: %s: repeated cmdType", lineNo, buffer);
					PrintError(err_buf);
					break;
				}
				else if(db_ptr->cmd_msg[i].cmdType==255)
				{
					cmdPos= i;
					break;
				}
			}

			if(cmdPos!=-1)
				db_ptr->cmd_msg[cmdPos].cmdType= cmdType;
			else
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: too much cmdtype(max %d)", 
					lineNo, buffer, MAX_CMDTYPE_NUM);
				PrintError(err_buf);
				continue;
			}
			continue;
		}

		if(cmdPos==-1)
			continue;

		if ( !strcmp(strbuf[0], "GROUP") & !strcmp(strbuf[1], "NAME") )
		{
			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: Need group name", lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			if(1!=sscanf(strbuf[2], "%d", &groupNo) || groupNo<0 || groupNo>=MAX_MSG_GROUP_NUM)
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: groupNo is overrange(0~7)", 
					lineNo, buffer);
				PrintError(err_buf);
				continue;
			}

			continue;
		}

		if(groupNo==-1)
			continue;

		if ( !strcmp(strbuf[0], "STATE") )
		{
			if(1!=sscanf(strbuf[1], "%d", &entryNo) || entryNo<0 || entryNo>=MAX_CMD_OPER_TYPE_NUM)
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: entryNo is overrange(0~63)", 
					lineNo, buffer);
				PrintError(err_buf);
				continue;
			}
			if(strlen(strbuf[2])>=MAX_CMD_MSG_SIZE)
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: cmd msg too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[2][MAX_CMD_MSG_SIZE-1]=0;
			}
			strcpy(db_ptr->cmd_msg[cmdPos].group[groupNo].def[entryNo], strbuf[2]);
#ifdef	EXTEND_LANGUAGE_SYSTEM
			if(strlen(strbuf[3])>=MAX_CMD_MSG_SIZE)
			{
				sprintf(err_buf, "CMD_MSG.TXT line %d: %s: cmd msg too long", 
					lineNo, buffer);
				PrintError(err_buf);
				strbuf[3][MAX_CMD_MSG_SIZE-1]=0;
			}
			strcpy(db_ptr->cmd_msg[cmdPos].group[groupNo].Exdef[entryNo], strbuf[3]);
#endif
		}
	}

	fclose ( fp );

	return 0;
}


