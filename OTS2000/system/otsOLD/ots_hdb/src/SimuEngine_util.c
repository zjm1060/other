
#include "/home/ems/system/ots/ots_hdb/inc/ots.h"

extern	FILE	*fp_gen, *fp_list;


ANLG_DATA	GetFormatAnaFromRealValue ( double value )
{
	ANLG_DATA anlg;
	int		j, k;
	short status;
	if ( value < 0 )
	{
		value 	= -value;
		k	= 0x80;
	}
	else	k	= 0;

	if ( value < 10 )
	{
		j	= 0x060;
		value	*= 1000;
	}
	else if ( value < 100 )
	{
		j	= 0x40;
		value	*= 100;
	}
	else if ( value < 1000 )
	{
		j	= 0x20;
		value	*= 10;
	}
	else if ( value < 10000 )
	{
		j	= 0;
	}
	else if ( value >= 10000 )
	{
		value	= 9999;
		j	= 0;
	}
	status = j | k;
	//yyp 2008.03.25
	memcpy(&anlg.status,&status,2);
	anlg.data 	= (int)value;

	return ( anlg );

}




void	GetCurrentTimeTable ( RTU_TIME *rec )
{
	struct	tm ltime;
	time_t t;

	time ( &t );
	ltime = *localtime ( &t );

	rec->minute	= ltime.tm_min;
	rec->second	= ltime.tm_sec;
}


void	GetCurrentTimeMS_Table ( RTU_TIME_MS *rec )
{
	struct	tm ltime;
	time_t t;

	time ( &t );
	ltime = *localtime ( &t );

	rec->minute	= ltime.tm_min;
	rec->second	= ltime.tm_sec;

}



void	CalcuDiff ( SEQUENCE_DEF *sq, int numb )
{
	DMS_COMMON	dms_rec;
	double	step_time;
	int		j, k;
	float value;
	double	p, wh;
	char	name_buf[DBNAME_SIZE];

	step_time	= sq->time.initial + sq->time.t;
	if ( time_cnt < step_time ) return;	/* step action time is not yet arrived*/

	sq->time.t	+= stepdb[numb].delay_time;
	sq->cur_step++;

	j	= stepdb[numb].head;
	k	= groupdb[j].head;

	if ( ReadEntryByNameStr ( entrydb[k+2].name, &dms_rec ) == -1)
	{
		return;
	}
	p	= dms_rec.var.anlg.fValue;

	if ( ReadEntryByNameStr ( entrydb[k+3].name, &dms_rec ) == -1)
	{
		return;
	}

	wh	= dms_rec.var.anlg.fValue;

	strcpy ( name_buf, entrydb[k+1].name );
/*yyp 2008.3.25 what is y1,y2?
	if ( y1 == y2 )
		value	= v1;
	else
		value	= v1+(v2-v1)*(wh-y1)/(y2-y1);
*/

	value =0;	//yyp 2008.3.25 needed to be del 
	BdANAByName ( stepdb[numb].o_name, value );
	printf ( "in DoDataCalculation:\n" );

}

int BdOTSStat(short state)
{
	char		tmp[100],buf[100];
	POINTER		pt;
	unsigned char stnid,devid;
	unsigned short	status;
	//added by yyp 2012.04.10
	return TRUE;
 	stnid = 1;
	devid = g_sys_lcu[stnid];	
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.FZ_STAT");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	
	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	return(TRUE);	
}	



void BdIostSetCmd(char *logicalname, unsigned short state)
{
	DMS_COMMON	dms_cmm;
	IOST tmp_iost;
	LAN_HEAD        head;
	COMMAND_MSG	msg;
	int		length;
	char		buf[COMMAND_SIZE];
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		pt;
	if ( ReadEntryByNameStr ( logicalname, &dms_cmm) == -1)
	{
		printf ( " %s is not found in database.", logicalname);
		return ;
	}	
	tmp_iost = dms_cmm.fixed.iost;
	if((unsigned short)(tmp_iost.almEnable) == state && (unsigned short)(tmp_iost.scanEnable) == state)
	{
		return;
	}
	if(GetPtIdByNameStr(logicalname,&pt)==-1) return;

	msg.type_id = DPS_IOST_SET;
	msg.stn_id = pt.stn_id;
	msg.dev_id = pt.dev_id;
	msg.data_type = pt.data_type;
	msg.point_id = pt.pt_id;	

	msg.status.result=0;
	msg.status.src = 0;
	msg.ctrl_addr =0;
	msg.data.iValue = 0;
	GetSrcId(&msg.host_id);
	msg.usr_id=0;
	strcpy(msg.usr_login_name,"ots");

	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.second  =devtime->tm_sec;
	msg.minute  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
	msg.day  =devtime->tm_mday;
	msg.month  =devtime->tm_mon+1;
	msg.year  =devtime->tm_year+1900; 
	head.dest_id=0;
	head.src_id = msg.host_id;
	head.dp_type=COMM_RECORD_TIME;
	head.length = COMMAND_SIZE;
	head.dest_stn = msg.stn_id;
	length = head.length;
	msg.status.opr = IOST_SET_SCAN_STATE;
	msg.status.state = state;
	Swap2Byte(&head.length);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
	memcpy(buf,&msg,COMMAND_SIZE);
	lan_out((char *)&msg,head,0);
	MySleep(5);

	msg.status.opr = IOST_SET_ALM_STATE;
	msg.status.state = state;
	Swap2Byte(&head.length);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
	memcpy(buf,&msg,COMMAND_SIZE);
	lan_out((char *)&msg,head,0);
	return;	
}

int GetFZStyle()
{
	char		tmp[100],buf[100];
	POINTER		pt;
	unsigned char stnid,devid;
	unsigned short	status;
 	//added by yyp 2012.04.10
	return TRUE;
	stnid = 1;
	devid = g_sys_lcu[stnid];	
 	if(GetStnNameById(stnid,tmp) == -1) return 0;
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return 0;
	strcat(buf,tmp);
	strcat(buf,".SYS.FZ_STYLE");
	if(GetPtIdByNameStr(buf,&pt)==-1) return 0;
	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return 0;
	
	return (int)status;
}	


double	GetUnitValue ( double x, double x_base )
{
	double	f;

	if ( fabs (x_base) > 0.00001 )
		f	= x/x_base;

	return	f;
}

double	GetRealValue ( double xu, double x_base )
{
	double	f;

	f	= xu*x_base;

	return	f;
}


int	CheckStrIsNumeric ( char str[] )
{
	int	i, len, pnt_flag;
	char	str1[100];
	int has_point = 0;
	strcpy ( str1, str );
	strcat ( str1, ":: Non numeric value." );
	pnt_flag	= ON;
	if ( !( str[0] == SADD ) && !( str[0] == SSUBTRACT ) && (!isdigit (str[0])) )
	{
		if ( !( str[0] == SPOINT ) )
		{
			//ErrMsg ( str1 );
			pnt_flag	= OFF;
			return FALSE;
		}
		else
		{
			has_point = 1;
		}
	}

	len	= strlen ( str );
	for ( i=1; i<len; i++ )
	{
		if ( !( str[i] == SPOINT ) && !isdigit (str[i]) )
		{
			//ErrMsg ( str1 );
			pnt_flag	= OFF;
			break;
		}
		else if ( str[i] == SPOINT )
		{
			if ( has_point == 1 )
			{
				//ErrMsg ( str1 );
				pnt_flag	= OFF;
				break;
			}
		}
	}
	if(pnt_flag == ON)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
int BdYGParam(unsigned char stnid,unsigned char devid,float dstvalue)
{
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	POINTER		point;
	float		orig;
	char		namebuf[200];

	if(-1==GetStnNameById(stnid, stn_name))
	{
		return FALSE;
	}
	if(-1==GetGroupNameById(stnid, devid, dev_name))
	{
		return FALSE;
	}
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "ANA.YG_MW");
	GetAnaValueFromName(namebuf,&orig);	

	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "ANA.YG_ORG");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return FALSE;
	}
	BdANAByName(namebuf,orig);

	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "ANA.YG_DST");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return FALSE;
	}
	BdANAByName(namebuf,dstvalue);
	return TRUE;
}

int BdWGParam(unsigned char stnid,unsigned char devid,float dstvalue)
{
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	POINTER		point;
	float		orig;
	char		namebuf[200];

	if(-1==GetStnNameById(stnid, stn_name))
	{
		return FALSE;
	}
	if(-1==GetGroupNameById(stnid, devid, dev_name))
	{
		return FALSE;
	}
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "ANA.WG_MVAR");
	GetAnaValueFromName(namebuf,&orig);	
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "ANA.WG_ORG");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return FALSE;
	}
	BdANAByName(namebuf,orig);

	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "ANA.WG_DST");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return FALSE;
	}
	BdANAByName(namebuf,dstvalue);
	return TRUE;
}


void 	BdANADisturb( SCENESEG_DEF seg)
{
	time_t t;
	float band;
	float fvalue,chgvalue;
	ANA_ENG_PARA eng;
	LMT_PARA lmt;
	time(&t);
	if(GetAnaPtEngLmtParaByNameStr(seg.name,&eng,&lmt ) == -1)
	{
		printf("GetAnaPtEngLmtParaByNameStr %s error",seg.name);
		return;
	}
	band=eng.D_BAND;
	
	if(t % 2 == 0)
	{
		chgvalue= (-1) * seg.len * band * (t % 5) /5;
	}
	else
	{
		chgvalue= 1 * seg.len * band * (t % 5) /5;
	}
	//printf("t=%d, chgvalue=%f\n",t,chgvalue);	
	GetAnaValueFromName(seg.name,&fvalue);
	if(abs((int)fvalue) <= 1)
	{
		return;
	}
	fvalue = fvalue + chgvalue;
	BdANAByName(seg.name ,fvalue);
	return;
}

int GetAnaValueFromName(char *name, float *fvalue)
{
	int	data_type;
	DMS_COMMON	dms_cmm;
	float value;
	value = -1;
	if ( ReadEntryByNameStr ( name, &dms_cmm ) == -1 )
	{
		//yyp 2008.09.01
		printf (  "This point %s is not Found in the database file !\n",name );
		return -1;
	}
	data_type	= dms_cmm.point.data_type;	//wdk 2008.3.22
	if ( data_type == ANA_TYPE ||   data_type == PARAM_TYPE )
	{
		value 	= dms_cmm.var.anlg.fValue;	
	}
	else
	{
		return -1;
	}
	*fvalue = value;
	return TRUE;
}

int	GetSequenceIdByName ( char *sq_name )
{
	int	i, sq_id = -1;

	for ( i=0; i<n_sequence; i++)
	{
		if ( !strcmp ( sequencedb[i].seq_name, sq_name ) )
		{
			sq_id	= i;
		//	fprintf ( fp_list, "in GetSequenceIdByName: %s is found when i= %d\n", sequencedb[i].seq_name, i );
			break;
		}
	}

	return	sq_id;

}

