
/*	simugen.c program

	Created by wdk, 1997.10.22.
	Used to simulate real time control process.

	This program compiles system description files writen with SIMULOG also
	created by wdk 2001.5.

*/

#include "/home/ems/system/ots/ots_hdb/inc/ots.h"

#define	SEQUENCE_DEF_MODULE

int	n_timer, n_unit, n_model, n_nlf, n_nlf_x, n_nlf_y, n_nlf_f, n_nlf_crv;

#ifdef	UNIX_VERSION

char	*mainfile	= "/OTSDATA/simulogMain.txt";
char	*genfile	= "/OTSDATA/simulog.dat";
char	*bakfile	= "/OTSDATA/simulogBak.bak";
char	*errfile	= "/OTSDATA/simulogErr.txt";
char	*listfile	= "/OTSDATA/simulogList.txt";

#else

char	*mainfile	= "/home/ems/h9000/ots/simulogMain.txt";
char	*genfile	= "/home/ems/h9000/ots/simulog.dat";
char	*bakfile	= "/home/ems/h9000/ots/simulogBak.bak";
char	*errfile	= "/home/ems/h9000/ots/simulogErr.txt";
char	*listfile	= "/home/ems/h9000/his/trap/simulogList.txt";

#endif

char	txt_fname[LINE_BUF_LENTH], txt_fullname[LINE_BUF_LENTH];
char	oper[DATANAME_LENTH];
char	buf[LINE_BUF_LENTH];
int	pos, txt_line_cnt, err_cnt,wrn_cnt;
static	DATE_DEF	dt;
static	TIME_DEF	tm;
DMS_COMMON	dms_cmm;

FILE	*fp_main, *fp_txt, *fp_gen, *fp_bak, *fp_err, *fp_list;


int		CopyStringFromBuf ();
void	DeleteSpace ();
void	ErrMsg ();
DATE_DEF	*GetDateFromDateString ( );
TIME_DEF	*GetTimeFromTimeString ( );
int		GetOneLineFromTxtFile ();
int		LoadSequenceTxtFile ();
int		LoadSceneTxtFile ();
void	PrintDataList ();



void	DeleteSpace ()
{
	while ( buf[pos] == ' ' || buf[pos] == TAB )
	{
		pos++;
	}
}

void LTrimSpace()
{
	int i;
	i=0;
	while ( buf[i] == ' ' || buf[i] == TAB )
	{
		i++;
	}
	memcpy(buf,buf+i,sizeof(buf)-i);	
}

void	ErrMsg ( msg )
char	*msg;
{
	err_cnt++;
	//fprintf ( fp_err, "\t%s Line %d err: %s: %s", txt_fname, txt_line_cnt, msg, buf );
	fprintf ( fp_err, "\tErr in line %d of %s: %s\n\t*\t%s\n", 
		txt_line_cnt, txt_fname, msg, buf );
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
			ErrMsg ( str1 );
			pnt_flag	= OFF;
			break;
		}
		else if ( str[i] == SPOINT )
		{
			if ( has_point == 1 )
			{
				ErrMsg ( str1 );
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


void	GetDataAndDBNameFromStr ( TFB_COEF_DEF *dd, int *coef_type )
{
	int	i, len, pnt_flag, asterix_pos, dtype;
	char	str[100], str1[100], data_str[100]="", name_str[100]="";

#define	DS_NUMERIC	1
#define	DS_ALPHABET	2
#define	DS_ERROR	3

	CopyStringFromBuf ( str, DATANAME_LENTH );
	strcpy ( str1, str );
	pnt_flag	= OFF;
	dtype	= 0;

	if ( ( str[0] == SADD ) || ( str[0] == SSUBTRACT ) || isdigit (str[0])
		|| ( str[0] == SPOINT ) )
	{
		dtype	= DS_NUMERIC;
		if ( str[0] == SPOINT )
		{
			pnt_flag	= ON;
		}
	}
	else if ( isalpha (str[0]))
	{
		dtype	= DS_ALPHABET;
	}
	else
	{
		strcat ( str1, ":: Non numeric/alphabet data beginning." );
		dtype	= DS_ERROR;
		ErrMsg ( str1 );
		return;
	}

	strcat ( str1, ":: Non numeric value." );
	if ( dtype == DS_ALPHABET )
	{
		strcpy ( data_str, "1." );
		strcpy ( name_str, str );
	}
	else if ( dtype == DS_NUMERIC )
	{
		len	= strlen ( str );
		asterix_pos	= 0;
		for ( i=0; i<len; i++ )
		{
			if ( str[i] == ASTERIX )
			{
				asterix_pos	= i;
				continue;
			}

			if ( asterix_pos == 0 )
			{
				data_str[i]	= str[i];
			}
			else
			{
				name_str[i-asterix_pos-1]	= str[i];
			}
		}
	}

	sscanf ( data_str, "%hf", &dd->data );
	sscanf ( name_str, "%hs", &dd->nlf_name );

	if ( strcmp ( name_str, "" ) )
	{
		dd->dtmp	= dd->data;
		*coef_type	= IS_NLF;
		dd->nlf_tag	= IS_NLF;
	}
	else
	{
		dd->dtmp	= dd->data;
	}
}


int	GetOneLineFromTxtFile ( FILE *fp )
{
	while ( 1 )
	{
		txt_line_cnt++;

		if ( fgets ( buf, LINE_BUF_LENTH, fp ) == NULL )
		{
			if ( feof ( fp ) )
			{
				return -1;
			}
			else
			{
				ErrMsg ( "This is a Null line!" );
				return -1;
			}
		}

		pos	= 0;
		LTrimSpace();
		return pos;
	}
}


int	CopyStringFromBuf ( str, max )
char	*str;
int	max;
{
	int	str_len;

	DeleteSpace ();

	str_len	= 0;
	while ( buf[pos] != CR && buf[pos] != LF && buf[pos] != 0 && 
		buf[pos] != TAB && buf[pos] != ' ' && str_len<max-1 )
	{
		str[str_len]	= buf[pos];
		str_len++; pos++;
	}
	str[str_len]	= 0;	/* set char string a end	*/

	return	str_len;

}


/*	count the number of a character in str	*/
int	CountCharInString ( char *str, char *letter )
{
	int	n_str, n;

	n_str	= n	= 0;
	while ( str[n] != CR && str[n] != LF && str[n] != 0 && str[n] != TAB && str[n] != ' ' )
	{
		if ( str[n] == letter[0] )
		{
			n_str++; n++;
		}
		else
			n++;
	}

	return	n_str;
}


void	ReadEntry ()
{
	char	data_name[DATANAME_LENTH];
	int	data_type, dspflag;

	if ( n_entry >= MAX_AND )
	{
		ErrMsg ( "ENTRY exceeds MAX_AND,overlapped!" );
		n_entry--;
		if ( groupdb[n_group].num )
			groupdb[n_group].num--;
	}

	CopyStringFromBuf ( entrydb[n_entry].name, DBNAME_SIZE );

	DeleteSpace ();

	switch ( buf[pos] )
	{
	case 	'?':
		entrydb[n_entry].opr	= BIN_LG;
		break;

	case	'<':
		pos++;
		if ( buf[pos] != '=' )
			entrydb[n_entry].opr	= LT;
		else
			entrydb[n_entry].opr	= LE;
		break;

	case	'>':
		pos++;
		if ( buf[pos] != '=' )
			entrydb[n_entry].opr	= GT;
		else
			entrydb[n_entry].opr	= GE;
		break;

	case	'=':
		pos++;
		if ( buf[pos] != '=' )
		{
			ErrMsg ( "Miss'='!" );
		}
		entrydb[n_entry].opr	= EQ;
		break;

	case	'!':
		pos++;
		if ( buf[pos] != '=' )
		{
			ErrMsg ( "Miss'='!" );
		}
		entrydb[n_entry].opr	= NE;
		break;

	default:
		pos++;
		ErrMsg ( "Error operator!" );
	 	entrydb[n_entry].opr	= BIN_LG;
	}
	//	wdk 2008.3.22
	if ( ReadEntryByNameStr ( entrydb[n_entry].name, &dms_cmm ) == -1 )
	{
		ErrMsg ( "Entry not found in DATABASE!" );
		if( strncmp(entrydb[n_entry].name,"PARAM",5) == 0)
		{
			data_type = -1;
		}
	}
	else
	{
		data_type = dms_cmm.point.data_type;	//added by wdk 2008.3.22
	}

	pos++;
	CopyStringFromBuf ( data_name, DATANAME_LENTH );


	if ( entrydb[n_entry].opr == BIN_LG )
	{
		if ( data_type != SOE_TYPE && data_type != POL_TYPE && data_type != OUTS_TYPE
			&& data_type != SYSMSG_TYPE && data_type != DEVOBJ_TYPE && data_type != -1 )
	//	if ( data_type != DPS_IND_ALM  
	//		&& data_type != DPS_SMS && data_type != -1 )	//???
		{ 
			ErrMsg ( "The data_type isn't an ON_OFF variable!" );
		}

		dspflag	= 0;
		if ( !strcmp ( data_name, "CLOSE" ) )
		{
			entrydb[n_entry].data.status	= CLOSE;
		}
		else 
		{
			if ( !strcmp ( data_name, "OPEN" ) )
			{
				entrydb[n_entry].data.status	= OPEN;
			}
			else
			{
				if ( sscanf ( data_name, "%hd", &entrydb[n_entry].data.status) == -1 )
				{
					ErrMsg ( "The value isn't an int!" );
				}

				dspflag	= 1;
				if ( entrydb[n_entry].data.status < 0 || entrydb[n_entry].data.status > 15 )
				{
					entrydb[n_entry].data.status	= OPEN;
					ErrMsg ( "The status is out of 0--15!" );
				}
			}
		}
	}
	else
	{
		if ( ( data_type != DPS_ANALOG &&  data_type != 12) &&  
			data_type != DPS_IMPULSE && data_type != -1 )
		{
			ErrMsg ( "The data_type is not ANALOG!" );
		}
		//modified by yyp 2012.11.16
		if(CheckStrIsNumeric( data_name ) == TRUE)
		{
			entrydb[n_entry].isnum = 1;
			sscanf ( data_name, "%hf", &entrydb[n_entry].data.value);
			strcpy(entrydb[n_entry].value_name,"");
		}
		else
		{
			entrydb[n_entry].isnum = 0;
			entrydb[n_entry].data.value = 0;
			strcpy(entrydb[n_entry].value_name,data_name);
		}

		
	}
	n_entry++;
	groupdb[n_group].num++;

}


void	GetInitEntryValue ( INIT_DEF *init, int data_type )
{
	char	data_name[DATANAME_LENTH];
	int	dspflag;

	CopyStringFromBuf ( data_name, DATANAME_LENTH );

		if ( data_type == SOE_TYPE || data_type == POL_TYPE 
			|| data_type == SYSMSG_TYPE || data_type == OUTS_TYPE)
	{
		dspflag	= 0;
		if ( !strcmp ( data_name, "CLOSE" ) )
		{
			init->data.status	= CLOSE;
		}
		else 
		{
			if ( !strcmp ( data_name, "OPEN" ) )
			{
				init->data.status	= OPEN;
			}
			else
			{
				if ( sscanf ( data_name, "%hd", &init->data.status) == -1 )
				{
					ErrMsg ( "The value isn't an int!" );
				}

				dspflag	= 1;
				if ( init->data.status < 0 || init->data.status > 15 )
				{
					init->data.status	= OPEN;
					ErrMsg ( "The status is out of 0--15!" );
				}
			}
		}
	}
	else if ( data_type == ANA_TYPE || data_type == PARAM_TYPE )
	{
		//yyp
		init->data.value	= ( float )atof ( data_name );
		if ( sscanf ( data_name, "%f", &init->data.value ) == -1 )
		{
			ErrMsg ( "The value is not float!" );
		}
	}
	else
	{
		ErrMsg ( "The data_type is -1!" );
	}

}



void	GetSceneSegEntryValue  ( SCENESEG_DEF *init, int data_type )
{
	char	data_name[DATANAME_LENTH];
	int	dspflag;

	CopyStringFromBuf ( data_name, DATANAME_LENTH );

		if ( data_type == SOE_TYPE || data_type == POL_TYPE 
			|| data_type == SYSMSG_TYPE || data_type == OUTS_TYPE)
	{
		dspflag	= 0;
		if ( !strcmp ( data_name, "CLOSE" ) )
		{
			init->data.status	= CLOSE;
		}
		else 
		{
			if ( !strcmp ( data_name, "OPEN" ) )
			{
				init->data.status	= OPEN;
			}
			else
			{
				if ( sscanf ( data_name, "%hd", &init->data.status) == -1 )
				{
					ErrMsg ( "The value isn't an int!" );
				}

				dspflag	= 1;
				if ( init->data.status < 0 || init->data.status > 15 )
				{
					init->data.status	= OPEN;
					ErrMsg ( "The status is out of 0--15!" );
				}
			}
		}
	}
	else if ( data_type == ANA_TYPE || data_type == PARAM_TYPE )
	{
		//yyp
		init->data.value	= ( float )atof ( data_name );
		if ( sscanf ( data_name, "%f", &init->data.value ) == -1 )
		{
			ErrMsg ( "The value is not float!" );
		}
	}
	else
	{
		ErrMsg ( "The data_type is -1!" );
	}

}

void ReadCalEntry ()
{
//wdk	int	data_type;

	if ( n_entry >= MAX_AND )
	{
		ErrMsg ( "ENTRY exceeds MAX_AND, overlapped!" );
		n_entry--;
		if ( groupdb[n_group].num )
			groupdb[n_group].num--;
	}

	CopyStringFromBuf ( entrydb[n_entry].name, DBNAME_SIZE );
	
	//yyp debug
	//printf("name=%s ,opr=%d\n",entrydb[n_entry].name,entrydb[n_entry].opr);

	if ( entrydb[n_entry].opr & RESET_C )
	{
		
		//modified by yyp 2012.11.16
		if(CheckStrIsNumeric( entrydb[n_entry].name ) == TRUE)
		{
			entrydb[n_entry].isnum = 1;
			sscanf ( entrydb[n_entry].name, "%hf", &entrydb[n_entry].data.value);
			//yyp debug
			//printf("name=%s,value =%f\n",entrydb[n_entry].name,entrydb[n_entry].data.value);
			strcpy(entrydb[n_entry].value_name,"");
		}
		else
		{
			entrydb[n_entry].isnum = 0;
			entrydb[n_entry].data.value = 0;
			strcpy(entrydb[n_entry].value_name,entrydb[n_entry].name);
		}
		
		//if ( sscanf ( entrydb[n_entry].name, "%lf", &entrydb[n_entry].data.value ) == -1 )
		//{
		//	ErrMsg ( "value s not a float!" );
		//}

#ifdef DEBUG
		printf ( "%3d:\t%s%9.3f\t\t", txt_line_cnt, oper, entrydb[n_entry].data.value );
#endif
	}
	else
	{
		if ( ReadEntryByNameStr ( entrydb[n_entry].name, &dms_cmm ) == -1 )
		{
			ErrMsg ( "Entry not found in DATABASE!" );
		}
#ifdef DEBUG
		printf ( "%3d:\t%s%-24s ", txt_line_cnt, oper, entrydb[n_entry].name );
#endif
	}
#ifdef DEBUG
	printf ( "\t\t\t\t<group:%d entry:%d gt:%d g_head:%d et:%d>\n",
		stepdb[n_step].num, groupdb[n_group].num, n_group, groupdb[n_group].head, n_entry );
#endif

	n_entry++;
	groupdb[n_group].num++;

}


void	CheckSequenceNameExistence ( char *sequence_name )
{
	int	i, err_tag	= OFF;

	for ( i=0; i<n_sequence; i++ )
	{
		if ( !strcmp ( sequencedb[i].seq_name, sequence_name ) )
		{
			err_tag	= ON;
//			break;
		}
	}

	if ( err_tag == OFF )
	{
		fprintf ( fp_err, "\tErr: Sequence name %s is not existing!\n", sequence_name );
		err_cnt++;
	}
}

void	CheckModelNameExistence ( char *model_name )
{
	int	i, err_tag	= OFF;

	for ( i=0; i<n_model; i++ )
	{
		if ( !strcmp ( mdb[i].name, model_name ) )
		{
			err_tag	= ON;
			break;
		}
	}

	if ( err_tag == OFF )
	{
		fprintf ( fp_err, "\tErr: Model name %s is not existing!\n", model_name );
		err_cnt++;
	}

}



/*	check names (sequence or model) referenced in steps whether exist.	*/
void	CheckNameExistenceInStep ()
{
	int	i, j, head, num;
	STEP_DEF	*step;

	for ( i=0; i<n_sequence; i++ )
	{
		head	= sequencedb[i].head;
		num		= sequencedb[i].num;
		for ( j=head; j<head+num; j++ )
		{
			step	= &stepdb[j];
			if ( step->mask == MASK_SEQUENCE ||
				step->mask == MASK_ELSE_SEQUENCE ||
				step->mask == MASK_TIMER_SEQUENCE ||
				step->mask == MASK_STOP_SEQUENCE )
			{
				CheckSequenceNameExistence ( step->o_name );
			}
			else if ( step->mask == MASK_MODEL ||
				step->mask == MASK_ELSE_MODEL ||
				step->mask == MASK_TIMER_MODEL ||
				step->mask == MASK_STOP_MODEL )
			{
				CheckModelNameExistence ( step->o_name );
			}
		}
	}

}


/*	check NLF names referenced in models whether exist.	*/
void	CheckNLF_NameExistence ()
{
	int	i, j, k, err_tag;
	TFB_DEF	*tfb;

	for ( i=0; i<n_model; i++ )
	{
		for ( j=mdb[i].tfb.head; j<mdb[i].tfb.end; j++ )
		{
			tfb	= &tfbdb[j];
			err_tag	= OFF;
			if ( tfb->linearity == PRE_NONLINEAR || tfb->linearity == POST_NONLINEAR )
			{
				for ( k=0; k<n_nlf; k++ )
				{
					if ( !strcmp ( tfb->nlf_name, nlfdb[k].name ) )
					{
						err_tag	= ON;
						break;
					}
				}

				if ( err_tag == OFF )
				{
					fprintf ( fp_err, "\tErr: NLF name %s does not exist!\n", tfb->nlf_name );
					err_cnt++;
				}
			}

			if ( tfb->coef_type == IS_NLF )
			{
				if ( strcmp ( tfb->a.nlf_name, "" ) )	// if name is not blank.
				{
					err_tag	= OFF;
					for ( k=0; k<n_nlf; k++ )
					{
						if ( !strcmp ( tfb->a.nlf_name, nlfdb[k].name ) )
						{
							err_tag	= ON;
							break;
						}
					}

					if ( err_tag == OFF )
					{
						//fprintf ( fp_err, "\tErr: NLF name %s does not exist!\n", tfb->a.nlf_name );
						//err_cnt++;
					}
				}

				if ( strcmp ( tfb->b.nlf_name, "" ) )
				{
					err_tag	= OFF;
					for ( k=0; k<n_nlf; k++ )
					{
						if ( !strcmp ( tfb->b.nlf_name, nlfdb[k].name ) )
						{
							err_tag	= ON;
							break;
						}
					}

					if ( err_tag == OFF )
					{
						//fprintf ( fp_err, "\tErr: NLF name %s does not exist!\n", tfb->b.nlf_name );
						//err_cnt++;
					}
				}
				if ( strcmp ( tfb->c.nlf_name, "" ) )
				{
					err_tag	= OFF;
					for ( k=0; k<n_nlf; k++ )
					{
						if ( !strcmp ( tfb->c.nlf_name, nlfdb[k].name ) )
						{
							err_tag	= ON;
							break;
						}
					}

					if ( err_tag == OFF )
					{
						//fprintf ( fp_err, "\tErr: NLF name %s does not exist!\n", tfb->c.nlf_name );
						//err_cnt++;
					}
				}
				if ( strcmp ( tfb->d.nlf_name, "" ) )
				{
					err_tag	= OFF;
					for ( k=0; k<n_nlf; k++ )
					{
						if ( !strcmp ( tfb->d.nlf_name, nlfdb[k].name ) )
						{
							err_tag	= ON;
							break;
						}
					}

					if ( err_tag == OFF )
					{
						//fprintf ( fp_err, "\tErr: NLF name %s does not exist!\n", tfb->d.nlf_name );
						//err_cnt++;
					}
				}
			}
		}
	}
}


/*	check NLF names not referenced by any models.	*/
void	CheckNLF_NameNotRefered ()
{
	int	i, j, k, err_tag;

	for ( i=0; i<n_nlf; i++ )
	{
		err_tag	= OFF;
		for ( j=0; j<n_model; j++ )
		{
			for ( k=mdb[j].tfb.head; k<mdb[j].tfb.end; k++ )
			{
				if ( !strcmp ( tfbdb[k].nlf_name, nlfdb[i].name ) )
				{
					err_tag	= ON;
					break;
				}
				else if ( !strcmp ( tfbdb[k].a.nlf_name, nlfdb[i].name ) )
				{
					err_tag	= ON;
					break;
				}
				else if ( !strcmp ( tfbdb[k].b.nlf_name, nlfdb[i].name ) )
				{
					err_tag	= ON;
					break;
				}
				else if ( !strcmp ( tfbdb[k].c.nlf_name, nlfdb[i].name ) )
				{
					err_tag	= ON;
					break;
				}
				else if ( !strcmp ( tfbdb[k].d.nlf_name, nlfdb[i].name ) )
				{
					err_tag	= ON;
					break;
				}
			}
		}
		if ( err_tag == OFF )
		{
			fprintf ( fp_err, "\tErr: NL function %s is not refered!\n", nlfdb[i].name );
			wrn_cnt++;
		}
	}


}


/*	check sequence names not referenced by any sequences.	*/
void	CheckSequenceNameNotRefered ( )
{
	int	i, j, k, err_tag;
	int	head, num;

	/* check whether sequence_name is refered in other sequences.	*/
	for ( i=0; i<n_sequence; i++ )
	{
		err_tag	= OFF;
		for ( j=0; j<n_sequence; j++ )
		{
			head	= sequencedb[j].head;
			num	= sequencedb[j].num;
			if ( i != j )
			{
				for ( k=head; k<head+num; k++ )
				{
					if ( ( stepdb[k].mask == MASK_SEQUENCE ||
						stepdb[k].mask == MASK_ELSE_SEQUENCE ||
						stepdb[k].mask == MASK_TIMER_SEQUENCE ) &&
						!strcmp ( sequencedb[i].seq_name, stepdb[k].o_name ) )
					{
						err_tag	= ON;
						break;
					}
				}
			}
		}

		if ( err_tag == OFF )
		{
			fprintf ( fp_err, "\tErr: sequence %s is not refered!\n", sequencedb[i].seq_name );
			wrn_cnt++;
		}

	}

	for ( i=0; i<n_sequence; i++ )
	{
		err_tag	= ON;
		head	= sequencedb[i].head;
		num	= sequencedb[i].num;
		for ( j=head; j<head+num; j++ )
		{
			if ( ( stepdb[j].mask == MASK_SEQUENCE ||
				stepdb[j].mask == MASK_ELSE_SEQUENCE ||
				stepdb[j].mask == MASK_TIMER_SEQUENCE ) &&
				!strcmp ( sequencedb[i].seq_name, stepdb[j].o_name ) )
			{
				err_tag	= OFF;
				break;
			}
		}

		if ( err_tag == OFF )
		{
			fprintf ( fp_err, "\tErr: sequence %s is self refered!\n", sequencedb[i].seq_name );
			err_cnt++;
		}
	}

}


/*	check model names not referenced by any sequences.	*/
void	CheckModelNameNotRefered ( )
{
	int	i, j, k, err_tag;
	int	head, num;

	for ( i=0; i<n_model; i++ )
	{
		err_tag	= OFF;
		for ( j=0; j<n_sequence; j++ )
		{
			head	= sequencedb[j].head;
			num	= sequencedb[j].num;
			for ( k=head; k<head+num; k++ )
			{
				if ( ( stepdb[k].mask == MASK_MODEL ||
					stepdb[k].mask == MASK_ELSE_MODEL ||
					stepdb[k].mask == MASK_TIMER_MODEL ) &&
					!strcmp ( mdb[i].name, stepdb[k].o_name ) )
				{
					err_tag	= ON;
					break;
				}
			}
		}

		if ( err_tag == OFF )
		{
			fprintf ( fp_err, "\tErr: model %s is not refered!\n", mdb[i].name );
			wrn_cnt++;
		}
	}

}


void	CheckSystemNames ()
{

	CheckNameExistenceInStep ( );
	CheckNLF_NameExistence ( );
	CheckNLF_NameNotRefered ( );
	CheckSequenceNameNotRefered ( );
	CheckModelNameNotRefered ( );

}


void	WriteSimulogDataFile ()
{
	char	temp;
	int	n_nlf_f_end;

	if ( ( fp_gen = fopen ( genfile, "rb" ) ) == NULL )
	{
		printf ( "No bak file generated ! can't open ( Rb) the old %s\n", genfile );
	}
	else
	{
		if ( ( fp_bak = fopen ( bakfile, "wb" ) ) == NULL )
		{
			printf ( "No bak file generated ! can't open (wb) the file %s!\n", bakfile );
		}
		else
		{
			temp = fgetc ( fp_gen );
			while	( !feof ( fp_gen ) )
			{
				fputc ( temp, fp_bak );
				temp	= fgetc ( fp_gen );
			}
			fclose ( fp_bak );
		}
		fclose ( fp_gen );
	}

	if ( ( fp_gen = fopen ( genfile, "wb" ) ) == NULL )
	{
		printf ( "No date file generated ! can't open (wb) the file %s!\n", genfile );
		return;
	}
printf("@@@@@@@n_step=%d@@@@@@@\n",n_step);//test
	n_step++;
	n_group++;
printf("!!!!!!n_step=%d n_group=%d!!!!\n",n_step,n_group);//test
	fwrite ( &n_sequence, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_scene, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_step, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_show, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_group, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_entry, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_init, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_sceneseg, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_model, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_tfb, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_param, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_param_seq, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_spare, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_nlf, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_nlf_crv, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_unit, sizeof ( int ), 1, fp_gen );
	fwrite ( &n_timer, sizeof ( int ), 1, fp_gen );
	if((signed)fwrite ( &sequencedb[0], sizeof ( SEQUENCE_DEF ), n_sequence, fp_gen )!=n_sequence)
	{
		err_write("fwrite sequencedb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &scenedb[0], sizeof ( SCENE_DEF ), n_scene, fp_gen )!=n_scene)
	{
		err_write("fwrite scenedb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &stepdb[0], sizeof ( STEP_DEF ), n_step, fp_gen )!=n_step)
	{
		err_write("fwrite stepdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &showdb[0], sizeof ( SHOW_DEF ), n_show, fp_gen )!=n_show)
	{
		err_write("fwrite showdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &groupdb[0], sizeof ( GROUP_DEF ), n_group, fp_gen )!= n_group)
	{
		err_write("fwrite groupdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &entrydb[0], sizeof ( ENTRY_DEF ), n_entry, fp_gen )!=n_entry)
	{
		err_write("fwrite entrydb error");
		MySleep(10000);
		return;
	}
	//yyp 
	if((signed)fwrite ( &initdb[0], sizeof ( INIT_DEF ), n_init, fp_gen )!=n_init)
	{
		err_write("fwrite initdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &scenesegdb[0], sizeof ( SCENESEG_DEF ), n_sceneseg, fp_gen )!=n_sceneseg)
	{
		err_write("fwrite scenesegdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &mdb[0], sizeof ( MODEL_DEF ), n_model, fp_gen )!=n_model)
	{
		err_write("fwrite mdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &tfbdb[0], sizeof ( TFB_DEF ), n_tfb, fp_gen )!=n_tfb)
	{
		err_write("fwrite tfbdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &paramdb[0], sizeof ( PARAM_DEF ), n_param, fp_gen )!=n_param)
	{
		err_write("fwrite paramdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &paramseqdb[0], sizeof ( PARAM_DEF ), n_param_seq, fp_gen )!=n_param_seq)
	{
		err_write("fwrite paramseqdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &sparedb[0], sizeof ( SPARE_DEF ), n_spare, fp_gen )!=n_spare)
	{
		err_write("fwrite sparedb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &nlfdb[0], sizeof ( NLF_DEF ), n_nlf, fp_gen )!=n_nlf)
	{
		err_write("fwrite nlfdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &nlf_ptr[0], sizeof ( HE_PTR_DEF ), n_nlf_crv, fp_gen )!=n_nlf_crv)
	{
		err_write("fwrite nlf_ptr error");
		MySleep(10000);
		return;
	}
	n_nlf_f_end	= nlf_ptr[nlfdb[n_nlf-1].f.end-1].end;

	if((signed)fwrite ( &nlfdata.x[0], sizeof ( double ), nlfdb[n_nlf-1].x.end, fp_gen )!=nlfdb[n_nlf-1].x.end)
	{
		err_write("fwrite nlfdata.x error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &nlfdata.y[0], sizeof ( double ), nlfdb[n_nlf-1].y.end, fp_gen )!=nlfdb[n_nlf-1].y.end)
	{
		err_write("fwrite nlfdata.y error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &nlfdata.f[0], sizeof ( double ), n_nlf_f_end, fp_gen )!=n_nlf_f_end)
	{
		err_write("fwrite nlfdata.f error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &unitdb[0], sizeof ( UNIT_DATA_DEF ), n_unit, fp_gen )!=n_unit)
	{
		err_write("fwrite unitdb error");
		MySleep(10000);
		return;
	}
	if((signed)fwrite ( &timerdb[0], sizeof ( TIMER_DEF ), n_timer, fp_gen )!=n_timer)
	{
		err_write("fwrite timerdb error");
		MySleep(10000);
		return;
	}
	PrintDataList ();
	fclose ( fp_gen );
	return;

}


void	PrintDataList ()
{
	int	i, j, k, i_tmp;
	SEQUENCE_DEF	*sq;
	STEP_DEF	*step;

	fprintf ( fp_list, "\n\t\tSEQUENCE_DB\n\n" );
	for ( i=0; i<n_sequence; i++)
	{
		sq	= &sequencedb[i];
		fprintf ( fp_list, "\tName: %s\thead:\t%d\tnum:\t%d\tihead:\t%d\tinum:\t%d\n", 
			sq->seq_name, sq->head, sq->num, sq->init.head, sq->init.num );
	}

	fprintf ( fp_list, "\n\t\tINITIALS\n\n" );
	for ( i=0; i<n_init; i++)
	{
		fprintf ( fp_list, "\tEntryName: %s\tvalue:\t%d\n", 
			initdb[i].name, initdb[i].data.status );
	}

	fprintf ( fp_list, "\n\t\tSTEP_DB\n\n" );
	for ( i=0; i<n_step; i++)
	{
		step	= &stepdb[i];
		fprintf ( fp_list, "\tName: %s\t\thead: %d\tnum: %d\ttype: %d\tresult: %d\tdelay: %.3f\tmask: %d\n", 
			step->o_name, step->head, step->num, step->type, step->result, step->delay_time, step->mask );
	}

	fprintf ( fp_list, "\n\t\tGROUP_DB\n\n" );
	for ( i=0; i<n_group; i++)
	{
		fprintf ( fp_list, "\thead:\t%d\tnum:\t%d\n", 
			groupdb[i].head, groupdb[i].num );
	}

	fprintf ( fp_list, "\n\t\tENTRY_DB\n\n" );
	for ( i=0; i<n_entry; i++)
	{
		fprintf ( fp_list, "\tEntryName: %s\toperator:\t%d\n", 
			entrydb[i].name, entrydb[i].opr );
	}

	fprintf ( fp_list, "\n\t\tNLF_DB\tn_nlf= %d\n\n", n_nlf );
	for ( i=0; i<n_nlf; i++)
	{
		fprintf ( fp_list, "\tNLF name: %s\ttype: %d\tY_name: %s\tX:h/e:\t%d\t%d\n", 
			nlfdb[i].name, nlfdb[i].nlf_type, nlfdb[i].yname, nlfdb[i].x.head, nlfdb[i].x.end );

		fprintf ( fp_list, "\tX:" );
		for ( j=nlfdb[i].x.head; j<nlfdb[i].x.end; j++ )
		{
			fprintf ( fp_list, "\t%.4f", nlfdata.x[j] );
		}
		fprintf ( fp_list, "\n" );
		if ( nlfdb[i].nlf_type == N1D )
		{
			i_tmp	= nlfdb[i].f.head;
			fprintf ( fp_list, "\tF:" );
			for ( j=nlf_ptr[i_tmp].head; j<nlf_ptr[i_tmp].end; j++ )
			{
				fprintf ( fp_list, "\t%.4f", nlfdata.f[j] );
			}
			fprintf ( fp_list, "\n\n" );
		}
		else if ( nlfdb[i].nlf_type == N2D )
		{
			fprintf ( fp_list, "\tY:" );
			for ( j=nlfdb[i].y.head; j<nlfdb[i].y.end; j++ )
			{
				fprintf ( fp_list, "\t%.4f", nlfdata.y[j] );
			}
			fprintf ( fp_list, "\n" );

			for ( j=nlfdb[i].f.head; j<nlfdb[i].f.end; j++ )
			{
				fprintf ( fp_list, "\tF:",nlf_ptr[j].head, nlf_ptr[j].end );
				for ( k=nlf_ptr[j].head; k<nlf_ptr[j].end; k++ )
				{
					fprintf ( fp_list, "\t%.4f", nlfdata.f[k] );
				}
				fprintf ( fp_list, "\n" );
			}
			fprintf ( fp_list, "\n" );
		}
	}

	fprintf ( fp_list, "\n\t\tGenUnit_DB\n\n" );
	for ( i=0; i<n_unit; i++)
	{
		fprintf ( fp_list, "\tGenUnit name: %s\tPn: %6.3f\tQn: %6.3f\tTa: %6.3f\n", 
			unitdb[i].name, unitdb[i].pn, unitdb[i].qn, unitdb[i].ta );
	}

}


void	GetStepDelay ( STEP_DEF *step )
{
	char	data_name[DATANAME_LENTH];

	pos	= strlen ( DELAY );

	if ( CopyStringFromBuf ( data_name, DATANAME_LENTH ) == 0 )
		step->delay_time	= DEFAULT_DELAY;

	if ( sscanf ( data_name, "%f", &step->delay_time ) == -1 )
	{
		step->delay_time	= DEFAULT_DELAY;
		ErrMsg ( "Delay time error!" );
	}

}

void	GetStepAlarmDelay ( STEP_DEF *step )
{
	char	data_name[DATANAME_LENTH];

	pos	= strlen ( ALARM_DELAY );

	if ( CopyStringFromBuf ( data_name, DATANAME_LENTH ) == 0 )
		step->alarm_delay	= DEFAULT_DELAY;

	if ( sscanf ( data_name, "%hf", &step->alarm_delay ) == -1 )
	{
		step->alarm_delay	= DEFAULT_DELAY;
		ErrMsg ( "Alarm delay time error!" );
	}

//	printf ( "str= %s;	Alarm delay= %f\n", data_name, step->alarm_delay );

}

void	GetTimerInterval ( TIMER_DEF *timer )
{

	pos	= strlen ( INTERVAL );

	if ( CopyStringFromBuf ( timer->interval, DATANAME_LENTH ) == 0 )
		strcpy ( timer->interval, DEFAULT_INTERVAL );

	printf ( "in GetTimerInterval: str=%s.\n", timer->interval );
	timer->dt.interval	= *GetTimeFromTimeString ( timer->interval );
		
}

void	GetStartTime ( TIMER_DEF *timer )
{

	pos	= strlen ( ASTART_TIME );

	if ( CopyStringFromBuf ( timer->start_date, DATANAME_LENTH ) == 0 )
	{
		strcpy ( timer->start_date, DEFAULT_INTERVAL );
	}

	/* if only time is specified, move date to time and set date null */
	DeleteSpace ();
	if ( buf[pos] == CR || buf[pos] == LF || buf[pos] == 0 )
	{
		strcpy ( timer->start_time, timer->start_date );
		strcpy ( timer->start_date, " " );
	}
	else
	{
		if ( CopyStringFromBuf ( timer->start_time, DATANAME_LENTH ) == 0 )
		{
			strcpy ( timer->start_time, DEFAULT_INTERVAL );
		}
	}

	timer->dt.date	= *GetDateFromDateString ( timer->start_date );
	timer->dt.time	= *GetTimeFromTimeString ( timer->start_time );

	printf ( "in GetStartTime: date=%s time=%s.\n", timer->start_date, timer->start_time );

}


TIME_DEF	*GetTimeFromTimeString ( char *time_str )
{
	int	n_str, n2;

	n_str	= CountCharInString ( time_str, ":" );
	n2	= CountCharInString ( time_str, "." );
	tm.hour	= tm.minute	= tm.second	= tm.ms	=0;
	if ( n2 == 0 )
	{
		if ( n_str == 2 )
		{
			if ( sscanf ( time_str, "%d:%d:%d", 
				&tm.hour, &tm.minute, &tm.second ) == -1 )
			{
				ErrMsg ( "Time specification error!" );
			}
		}
		else if ( n_str == 1 )
		{
			if ( sscanf ( time_str, "%d:%d", 
				&tm.minute, &tm.second ) == -1 )
			{
				ErrMsg ( "Time specification error!" );
			}
			tm.hour	= -1;	/* un-specified	*/
		}
		else if ( n_str == 0 )
		{
			if ( sscanf ( time_str, "%d", 
				&tm.second ) == -1 )
			{
				ErrMsg ( "Time specification error!" );
			}
			tm.hour	= -1;	/* un-specified	*/
			tm.minute	= -1;	/* un-specified	*/
		}
	}
	else if ( n2 == 1 )
	{
		if ( n_str == 2 )
		{
			if ( sscanf ( time_str, "%d:%d:%d.%d", 
				&tm.hour, &tm.minute, &tm.second, &tm.ms ) == -1 )
			{
				ErrMsg ( "Time specification error!" );
			}
		}
		else if ( n_str == 1 )
		{
			if ( sscanf ( time_str, "%d:%d.%d", 
				&tm.minute, &tm.second, &tm.ms ) == -1 )
			{
				ErrMsg ( "Time specification error!" );
			}
			tm.hour	= -1;	/* un-specified	*/
		}
		else if ( n_str == 0 )
		{
			if ( sscanf ( time_str, "%d.%d", 
				&tm.second, &tm.ms ) == -1 )
			{
				ErrMsg ( "Time specification error!" );
			}
			tm.hour	= -1;	/* un-specified	*/
			tm.minute	= -1;	/* un-specified	*/
		}
	}
	printf ( "Time=%d:%d:%d.%d\n", tm.hour, tm.minute, tm.second, tm.ms );
	if ( tm.hour >=24 || tm.minute >= 60 || tm.second > 60 || tm.ms > 1000 )
			ErrMsg ( "Time DATA error!" );

	return ( &tm );

}


DATE_DEF	*GetDateFromDateString ( char *date_str )
{
	int	n_str;

	n_str	= CountCharInString ( date_str, "-" );
	dt.year	= dt.month= dt.day	= 0;
//	printf ( "Date=%s. n_str= %d\n", date_str, n_str );
	if ( n_str == 2 )
	{
		if ( sscanf ( date_str, "%d-%d-%d", &dt.year, &dt.month, &dt.day ) == -1 )
		{
			ErrMsg ( "Date specification error!" );
		}
	}
	else if ( n_str == 1 )
	{
		if ( sscanf ( date_str, "%hd-%hd", &dt.month, &dt.day ) == -1 )
		{
			ErrMsg ( "Date specification error!" );
		}
		dt.year	= -1;	/* un-specified	*/
	}
	else if ( n_str == 0 )
	{
		if ( sscanf ( date_str, "%hd", &dt.day ) == -1 )
		{
			ErrMsg ( "Date specification error!" );
		}
		dt.year	= -1;	/* un-specified	*/
		dt.month	= -1;	/* un-specified	*/
	}
	printf ( "Date=%d-%d-%d.\n", dt.year, dt.month, dt.day );

	return ( &dt );
}


int	LoadSceneTxtFile ()
{
	int	data_type;
	char	str[DATANAME_LENTH];
	scenedb[n_scene].head = n_sceneseg;
	while	( n_sceneseg < MAX_SCENESEG )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			printf ( "The file now EOF without END_SCENE!\n" );
			return -1;
		}
		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{

		}
		else if ( !strncmp ( END_SCENE, buf, strlen ( END_SCENE )) )
		{
			//printf ( "Line %d EndInitialize: %s\n", txt_line_cnt, buf );
			return 0;
		}
		else if( !strncmp ( TYPE, buf, strlen ( TYPE )) )
		{
			pos = strlen ( TYPE );
			CopyStringFromBuf ( str, DATANAME_LENTH );
			//CheckStrIsNumeric ( str );
			if( strcmp(str,"DISTURB") == 0 )
			{
				scenedb[n_scene].type = TYPE_DISTURB;
			}
			else
			{
				scenedb[n_scene].type = TYPE_NORMAL;					
			}
			//sscanf ( str, "%s", &mdl->type);
		}
		else
		{
			DeleteSpace ();
			if ( buf[pos] == LF ) continue;

			if(scenedb[n_scene].type == TYPE_NORMAL)
			{
				CopyStringFromBuf ( scenesegdb[n_sceneseg].name, DBNAME_SIZE );
				if ( ReadEntryByNameStr ( scenesegdb[n_sceneseg].name, &dms_cmm ) == -1 )
				{
					ErrMsg ( "sceneseg not found in DATABASE!" );
				}
				data_type	= dms_cmm.point.data_type;	// added by wdk 2008.3.22
				DeleteSpace ();	
				switch	( buf[pos] )
				{
				case	'=':
					pos++;
					DeleteSpace ();
					GetSceneSegEntryValue  ( &scenesegdb[n_sceneseg], data_type );
					break;

				default:
					ErrMsg ( "scene condition '=' is missed." );
				}
			}
			else
			{
				sscanf(buf,"%s %d %f",&scenesegdb[n_sceneseg].name, &(scenesegdb[n_sceneseg].speed), &(scenesegdb[n_sceneseg].len) );
			}
			n_sceneseg++;
			scenedb[n_scene].num++;
		}
	}
	return 0;
}

int	GetSequenceInitialize ()
{
	int	data_type;

	while	( n_init < MAX_INIT )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			printf ( "The file now EOF without END_INITIALIZE!\n" );
			return -1;
		}

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
		}
		else if ( !strncmp ( END_INITIALIZE, buf, strlen ( END_INITIALIZE )) )
		{
			//printf ( "Line %d EndInitialize: %s\n", txt_line_cnt, buf );
			return 0;
		}
		else
		{
			DeleteSpace ();

			if ( buf[pos] == LF ) continue;

			CopyStringFromBuf ( initdb[n_init].name, DBNAME_SIZE );

			if ( ReadEntryByNameStr ( initdb[n_init].name, &dms_cmm ) == -1 )
			{
				ErrMsg ( "Init not found in DATABASE!" );
			}
			data_type	= dms_cmm.point.data_type;	// added by wdk 2008.3.22

			DeleteSpace ();

			switch	( buf[pos] )
			{
			case	'=':
				pos++;
				DeleteSpace ();
				GetInitEntryValue ( &initdb[n_init], data_type );
				break;

			default:
				ErrMsg ( "Init condition '=' is missed." );
			}

			n_init++;
			sequencedb[n_sequence].init.num++;
			
		}
	}

	return 0;

}

void	GetStepResult ( STEP_DEF *step )
{
	char	data_name[DATANAME_LENTH];

	pos	= strlen ( RESULT );

	/*for multi-state logic */
	if ( CopyStringFromBuf ( data_name, DATANAME_LENTH ) == 0 )
		step->result	= LOGIC;
	else
	{
		if ( sscanf ( data_name, "%d", &step->result ) == -1 )
		{
			ErrMsg ( "value is not a int!" );
		}
	}
}

void	GetStepSpeed ( STEP_DEF *step )
{
	char	data_name[DATANAME_LENTH];

	pos	= strlen ( SPEED );

	/*for multi-state logic */
	if ( CopyStringFromBuf ( data_name, DATANAME_LENTH ) == 0 )
		step->speed	= LOGIC;
	else
	{
		if ( sscanf ( data_name, "%d", &step->speed ) == -1 )
		{
			ErrMsg ( "value is not a int!" );
		}
	}
}

void	GetStepType ( STEP_DEF *step )
{

	pos	= strlen ( TYPE );
	DeleteSpace ();

	switch	( buf[pos] )
	{
	case	'C':
		step->type	= CALC;		/*for calculator type*/
		break;

	case	'L':
		step->type	= LOGIC;	/*for logic type*/
		break;

	default:
		ErrMsg ( "The step type is not defined!" );
	}
}


void	GetEquaType ( STEP_DEF *step )
{

	pos	= strlen ( EQUA_TYPE );
	DeleteSpace ();

	switch	( buf[pos] )
	{
	case	'N':
		step->equa_type	= EQUA_NORMAL;		/*for normal type*/
		break;

	case	'C':
		step->equa_type	= EQUA_CHANGE;	/*for change type*/
		break;

	case	'L':
		step->equa_type	= EQUA_LOGICAL;	/* zwj for logical calc type*/
		break;

	default:
		ErrMsg ( "The step EquaType is not defined!" );
	}
}


//added by yyp 2012.06.30
void	GetSeqRunMode ( SEQUENCE_DEF *seq )
{

	pos	= strlen ( RUNMODE );
	DeleteSpace ();

	switch	( buf[pos] )
	{
	case	'S':
		seq->runmode	= MODE_SERIAL;		
		break;
	case	'P':
		seq->runmode	= MODE_PARALLEL;	
		break;

	default:
		ErrMsg ( "The  SEQ runmode is not defined!" );
	}
}

//added by yyp 2012.06.30
void	GetSeqType ( SEQUENCE_DEF *seq )
{

	pos	= strlen ( SEQ_TYPE );
	DeleteSpace ();

	switch	( buf[pos] )
	{
	case	'N':
		seq->type	= SEQ_NORMAL;		
		break;
	case	'D':
		seq->type	= SEQ_DEAD_LOOP;	
		break;

	case	'L':
		seq->type	= SEQ_LOGICAL;	
		break;
	case	'A':
		seq->type	= SEQ_AUTO_LOGICAL;	
		break;

	default:
		ErrMsg ( "The  SEQ type is not defined!" );
	}
}


void	GetStepFirstLine ( mask, str )
char	*str;
int	mask;
{
	pos	= strlen ( str );
	printf("======n_step=%d======\n",n_step);//test
	n_step++;
	sequencedb[n_sequence].num++;
	if ( sequencedb[n_sequence].num == FIRST_STEP )
	{
		sequencedb[n_sequence].head	= n_step;
	}

	CopyStringFromBuf ( stepdb[n_step].o_name, DBNAME_SIZE );

	stepdb[n_step].num	= 0;
	stepdb[n_step].mask	= mask;
	//added by yyp 2009.10.13
	stepdb[n_step].result = -1;
	/* check dbname */
	if ( mask == MASK_STEP || mask == MASK_ELSE_STEP )
	{
		if ( ReadEntryByNameStr ( stepdb[n_step].o_name, &dms_cmm ) == -1 )
		{
			ErrMsg ( "Entry not found in DATABASE!" );
		}
		DeleteSpace ();
	}
	else if ( mask == MASK_ELSE_SEQUENCE )
	{
		if ( n_step <= sequencedb[n_sequence].head ||
			stepdb[n_step-1].mask != MASK_SEQUENCE && 
			stepdb[n_step-1].mask != MASK_ELSE_SEQUENCE )
		{
			ErrMsg ( "ELSE_SEQUENCE without precedent GO_SEQUENCE!" );
		}
	}
	else if ( mask == MASK_ELSE_MODEL )
	{
		if ( n_step <= sequencedb[n_sequence].head ||
			stepdb[n_step-1].mask != MASK_MODEL && 
			stepdb[n_step-1].mask != MASK_ELSE_MODEL )
		{
			ErrMsg ( "ELSE_MODEL without precedent GO_MODEL!" );
		}
	}

	if ( n_group >= MAX_OR-1 )
	{
		ErrMsg ( "The step group has reached the MAX!" );
	}
	else
	{
		n_group++;
	}
	stepdb[n_step].head	= n_group;
	groupdb[n_group].num	= 0;
	groupdb[n_group].head	= n_entry;

}


//yyp deal with the param of tfb
int	P_ParamSeqDataProc ( SEQUENCE_DEF *seq )
{
	sscanf ( buf, "%s	=	%s", paramseqdb[n_param_seq].param_name,paramseqdb[n_param_seq].pt_name);

	return 0;
}

//added by yyp 2009.4.13
int	LoadP_ParamSeqData ( SEQUENCE_DEF *seq )
{

	seq->param.head	= n_param_seq;

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}
//		fprintf ( fp_list, "line %d, %s\n", txt_line_cnt, buf );

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if ( !strncmp ( END_PARAM, buf, strlen ( END_PARAM ) ) )
		{
			seq->param.end	= n_param_seq;
			break;
		}
		else if ( ( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ) )
		{
//			fprintf ( fp_list, "in LoadP_MatrixData LF/CR\n" );
		}
		else
		{
			P_ParamSeqDataProc ( seq );
			n_param_seq++;
		}
	}
	//printf ( "n_param= %d\n", n_param );
	return 0;
}

int	LoadSequenceTxtFile ( )
{
	int	type_flag, flag;

	pos	= 0;

	//printf ( "in Begin LoadSequenceTxtFile: Line %d comment: %s\n", txt_line_cnt, buf );
	type_flag	= ON;
	while	( n_step < MAX_STEP )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			printf ( "The file now EOF without END_SEQUENCE!\n" );
			return -1;
		}

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			printf ( "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		//added by yyp 2012.06.30
		else if ( !strncmp ( RUNMODE, buf, strlen ( RUNMODE )) )
		{
			GetSeqRunMode ( &sequencedb[n_sequence] );
			continue;
		}
		else if ( !strncmp ( SEQ_TYPE, buf, strlen ( SEQ_TYPE )) )
		{
			GetSeqType ( &sequencedb[n_sequence] );	
			continue;			
		}
		else if ( !strncmp ( INITIALIZE, buf, strlen ( INITIALIZE )) )
		{
			sequencedb[n_sequence].init.head	= n_init;
			sequencedb[n_sequence].init.num	= 0;
			if ( GetSequenceInitialize () == -1 )
			{
				ErrMsg ( "GetSequenceInitialize return is error." );
			}
		}
		else if ( !strncmp ( PARAM, buf, strlen ( PARAM ) ) )
		{
			LoadP_ParamSeqData ( &sequencedb[n_sequence] )	;		
			//break;
		}
		else
		{
/*			a new control sequence STEP:			*/
			if ( !strncmp( STEP, buf, strlen ( STEP ) ) )
			{
				//if ( type_flag == OFF )
				//	ErrMsg ( "The step type before this step is not specified!" );

				type_flag	= OFF;
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "The sequence reached MAX, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_STEP, STEP );
				continue;
			}
			else if ( !strncmp( ELSE_STEP, buf, strlen ( ELSE_STEP ) ) )
			{
				//if ( type_flag == OFF )
				//	ErrMsg ( "The step type before this step is not specified!" );

				type_flag	= OFF;
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "The sequence reached MAX, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_ELSE_STEP, ELSE_STEP );
				continue;
			}
/*			wait for expression to be satisfied			*/
			else if ( !strncmp ( TIMER_SEQUENCE, buf, strlen ( TIMER_SEQUENCE ) ) )
			{
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}
				if ( n_timer >= MAX_TIMER-1 )
				{
					ErrMsg ( "Timer number overflow MAX_TIMER, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_TIMER_SEQUENCE, TIMER_SEQUENCE );
				stepdb[n_step].timer_id	= n_timer;
				strcpy ( timerdb[n_timer].name, stepdb[n_step].o_name );
				timerdb[n_timer].prc_type	= PRC_SEQUENCE;

				//printf ( "n_timer=%d	timed_prc_name:%s\n", n_timer, stepdb[n_step].o_name );

				n_timer++;

				continue;
			}
			else if ( !strncmp ( TIMER_MODEL, buf, strlen ( TIMER_MODEL ) ) )
			{
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Model overflow MAX_STEP, exit now!" );
					break;
				}
				if ( n_timer >= MAX_TIMER-1 )
				{
					ErrMsg ( "Timer number overflow MAX_TIMER, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_TIMER_MODEL, TIMER_MODEL );
				stepdb[n_step].timer_id	= n_timer;
				strcpy ( timerdb[n_timer].name, stepdb[n_step].o_name );
				timerdb[n_timer].prc_type	= PRC_MODEL;

				//printf ( "n_timer=%d	timed_prc_name:%s\n", n_timer, stepdb[n_step].o_name );

				n_timer++;

				continue;
			}
/*			wait for expression to be satisfied			*/
			else if ( !strncmp ( WAIT_FOR, buf, strlen ( WAIT_FOR ) ) )
			{
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_WAIT_FOR, WAIT_FOR );
				continue;
			}
//			do while experssion to be satisfied added by yyp 09.08.05
			else if ( !strncmp ( DO_WHILE, buf, strlen ( DO_WHILE ) ) )
			{
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_DO_WHILE, DO_WHILE );
				continue;
			}
/*			a new control sequence STEP:			*/
			else if ( !strncmp ( GO_SEQUENCE, buf, strlen ( GO_SEQUENCE ) ) )
			{
				if ( type_flag == OFF )
					ErrMsg ( "The step type before this go_sequence is not specified!" );

				type_flag	= OFF;
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_SEQUENCE, GO_SEQUENCE );
				continue;
			}
			else if ( !strncmp ( ELSE_SEQUENCE, buf, strlen ( ELSE_SEQUENCE )) )
			{
				if ( type_flag == OFF )
					ErrMsg ( "The step type before this go_sequence is not specified!" );

				type_flag	= OFF;
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_ELSE_SEQUENCE, ELSE_SEQUENCE );
				continue;
			}
			else if ( !strncmp ( STOP_SEQUENCE, buf, strlen ( STOP_SEQUENCE ) ) )
			{
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				//printf ( "in Stop Sequence\n" );
				GetStepFirstLine ( MASK_STOP_SEQUENCE, STOP_SEQUENCE );
				continue;
			}
			else if ( !strncmp ( GO_MODEL, buf, strlen ( GO_MODEL ) ) )
			{
				//if ( type_flag == OFF )
				//	ErrMsg ( "The step type before this go_model is not specified!" );

				type_flag	= OFF;
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_MODEL, GO_MODEL );
				continue;
			}
			//added by yyp 2009.6.29
			else if ( !strncmp ( GO_SCENE, buf, strlen ( GO_SCENE ) ) )
			{
				if ( type_flag == OFF )
					ErrMsg ( "The step type before this go_scene is not specified!" );

				type_flag	= OFF;
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_SCENE, GO_SCENE );
				continue;
			}
			else if ( !strncmp ( ELSE_MODEL, buf, strlen ( ELSE_MODEL ) ) )
			{
				//if ( type_flag == OFF )
				//	ErrMsg ( "The step type before this go_model is not specified!" );

				type_flag	= OFF;
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				GetStepFirstLine ( MASK_ELSE_MODEL, ELSE_MODEL );
				continue;
			}
			else if ( !strncmp ( STOP_MODEL, buf, strlen ( STOP_MODEL ) ) )
			{
				if ( n_step >= MAX_STEP-1 )
				{
					ErrMsg ( "Sequence overflow MAX_STEP, exit now!" );
					break;
				}

				//printf ( "in Stop Model\n" );
				GetStepFirstLine ( MASK_STOP_MODEL, STOP_MODEL );
				continue;
			}
			else if ( !strncmp ( DELAY, buf, strlen ( DELAY )) )
			{
				GetStepDelay ( &stepdb[n_step] );
				continue;
			}
			else if ( !strncmp ( ALARM_DELAY, buf, strlen ( ALARM_DELAY )) )
			{
				GetStepAlarmDelay ( &stepdb[n_step] );
				continue;
			}
			else if ( !strncmp ( RESULT, buf, strlen ( RESULT )) )
			{
				GetStepResult ( &stepdb[n_step] );
				continue;
			}
			else if ( !strncmp ( SPEED, buf, strlen ( SPEED )) )
			{
				GetStepSpeed ( &stepdb[n_step] );
				continue;
			}
			else if ( !strncmp ( TYPE, buf, strlen ( TYPE )) )
			{
				GetStepType ( &stepdb[n_step] );
				continue;
			}
			else if ( !strncmp ( EQUA_TYPE, buf, strlen ( EQUA_TYPE )) )
			{
				GetEquaType ( &stepdb[n_step] );
				continue;
			}
			else if ( !strncmp ( INTERVAL, buf, strlen ( INTERVAL )) )
			{
				GetTimerInterval ( &timerdb[n_timer-1] );
				continue;
			}
			else if ( !strncmp ( ASTART_TIME, buf, strlen ( ASTART_TIME )) )
			{
				timerdb[n_timer-1].timer_type	= TIME_ABSOLUTE;
				GetStartTime ( &timerdb[n_timer-1] );
				continue;
			}
			else if ( !strncmp ( RSTART_TIME, buf, strlen ( RSTART_TIME )) )
			{
				timerdb[n_timer-1].timer_type	= TIME_RELATIVE;
				GetStartTime ( &timerdb[n_timer-1] );
				continue;
			}
/*			end of a control SEQUENCE	*/
			else if ( !strncmp ( END_SEQUENCE, buf, strlen ( END_SEQUENCE )) )
			{
				//printf ( "\nEND_SEQUENCE reached,Total st=%d gt=%d et=%d\n", n_step, n_group, n_entry );
				return 0;
			}
			else if ( !strncmp ( EQUATION, buf, strlen ( EQUATION )) )
			{
				type_flag	= ON;
				flag	= FIRST_F_LINE;
				pos	= strlen ( EQUATION );
			}
			//added by yyp 2009.9.17
			type_flag = ON;

			DeleteSpace ();

			if ( stepdb[n_step].type != CALC )
			{
				switch	( buf[pos] )
				{
				case	'=':
					if ( flag == FIRST_F_LINE )
					{
	          			strcpy ( oper, " = " );
						flag	= OTHER_F_LINE;
					}
					else
					{
						strcpy (oper, " & " );
						ErrMsg ( "More than one '=' in this STEP!" );
	          		}
					break;

				case	'&': 
					if ( flag == OTHER_F_LINE )
	        		  		strcpy ( oper, " & " );
					else
					{
						strcpy ( oper, " = " );
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	'+': 
					if ( flag == OTHER_F_LINE )
					{
						if ( n_group >= MAX_OR -1 )
						{
							ErrMsg ( "The group has reached the Max, Overlapped!" );
						}
						else
						{
							//for +, next group
							n_group++;

			         		stepdb[n_step].num++;

							groupdb[n_group].num	= 0;
							groupdb[n_group].head	= n_entry;
						}
						strcpy ( oper, " + " );
					}
					else
					{
						strcpy ( oper, " = " );
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	LF:
				case	CR:
#ifdef DEBUG
//					printf ( "%3d:Space\n", txt_line_cnt );
#endif
					continue;

				case	';':
#ifdef DEBUG
					printf ( "%3d:Descript line: %s\n", txt_line_cnt, buf );
#endif
					continue;

				default :
					ErrMsg ( "bad operator!" );
	        			continue;
				}
				pos++; pos++;
				DeleteSpace ();
				ReadEntry ();
			}
			else if ( stepdb[n_step].type == CALC )
			{
				switch	( buf[pos] )
				{
				case	'=':
					pos++;
					if ( buf[pos] != 'C' )
					{
						strcpy ( oper, " = " );
						entrydb[n_entry].opr = Simu_RESET;
					}
					else
					{
						strcpy ( oper, " +C " );
						entrydb[n_entry].opr	= RESET_C;
					}

					if ( flag == FIRST_F_LINE )
						flag	= OTHER_F_LINE;
					else
					{
						ErrMsg ( "More than one '=' in this STEP!" );
					}
					break;

				case	'+':
					pos++;
					if ( buf[pos] != 'C' )
					{
						strcpy ( oper, " + " );
						entrydb[n_entry].opr = ADD;
					}
					else
					{
						strcpy ( oper, " +C " );
						entrydb[n_entry].opr = ADD_C;
					}

					if ( flag != OTHER_F_LINE )
					{
						flag = OTHER_F_LINE;
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	'-':
					pos++;
					if ( buf[pos] != 'C' )
					{
						strcpy ( oper, " - " );
					 	entrydb[n_entry].opr	= MIN;
					}
					else
					{
						strcpy ( oper, " -C " );
						entrydb[n_entry].opr	= MIN_C;
					}
					if ( flag != OTHER_F_LINE )
					{
						flag	= OTHER_F_LINE;
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	'*':
					pos++;
					if ( buf[pos] != 'C' )
					{
						strcpy ( oper, " * " );
					  	entrydb[n_entry].opr	= MUL;
					}
					else
					{
						strcpy ( oper, " *C " );
						entrydb[n_entry].opr	= MUL_C;
					}
					if ( flag != OTHER_F_LINE )
					{
						flag	= OTHER_F_LINE;
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	'/':
					pos++;
					if ( buf[pos] != 'C' )
					{
						strcpy ( oper, " / " );
						entrydb[n_entry].opr	= DIV;
					}
					else
					{
						strcpy ( oper, " /C " );
						entrydb[n_entry].opr	= DIV_C;
					}
					if ( flag != OTHER_F_LINE )
					{
						flag	= OTHER_F_LINE;
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	'@':		/*乘方运算*/
					pos++;
					if ( buf[pos] != 'C' )
					{
						strcpy ( oper, " @ " );
						entrydb[n_entry].opr	= EXP;
					}
					else
					{
						strcpy ( oper, " @C " );
						entrydb[n_entry].opr	= EXP_C;
					}
					if ( flag != OTHER_F_LINE )
					{
						flag	= OTHER_F_LINE;
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	'^':		/*开方运算*/
					pos++;
					if ( buf[pos] != 'C' )
					{
						strcpy ( oper, " ^ " );
						entrydb[n_entry].opr	= SQU;
					}
					else
					{
						strcpy ( oper, " ^C " );
						entrydb[n_entry].opr	= SQU_C;
					}
					if ( flag != OTHER_F_LINE )
					{
						flag	= OTHER_F_LINE;
						ErrMsg ( "No '=' in this STEP!" );
					}
					break;

				case	LF:
				case	CR:
#ifdef DEBUG
//					printf ( "%3d:Space\n", txt_line_cnt );
#endif
					continue;

				case	';':
#ifdef DEBUG
					//printf ( "%3d:Descript line: %s\n", txt_line_cnt, buf );
#endif
					continue;

				default: 
					ErrMsg ( "Bad operator!" );
					continue;
				}
				pos++;
				DeleteSpace ();
				ReadCalEntry ();
			}
		}

	}

	//printf ( "\nTotal st=%d gt=%d et=%d exit now!\n", n_step, n_group, n_entry );

	return 0;
}


void	CheckSequenceName ( )
{
	int	i, err_tag	= OFF;

	for ( i=0; i<n_sequence; i++)
	{
		if ( !strcmp ( sequencedb[i].seq_name, sequencedb[n_sequence].seq_name ) )
		{
			err_tag	= ON;
		}
	}

	if ( err_tag == ON )
	{
		fprintf ( fp_err, "\t%s Line %d err: Sequence name %s is already existing!\n", 
			txt_fname, txt_line_cnt, sequencedb[n_sequence].seq_name );
		err_cnt++;
	}

}


GetTFB_Coef ( double t_step, TFB_DEF *tfb )
{
	double	a0, a1, a2, a3, a4, a5, a6, a7;

	if ( tfb->order == FT_GENERAL )
	{
		if ( tfb->a.data == 0 )
		{
			if ( tfb->d.data == 0 )
			{
				tfb->type	= INTEGRATION;
				tfb->k	= tfb->c.data/tfb->b.data;
				tfb->a.data	= 0.;
				tfb->b.data	= 0.;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
			else
			{
				tfb->type	= PROPORTIONAL_INTEGRATION;
				tfb->k	= tfb->c.data/tfb->b.data;
				tfb->a.data	= 0.;
				tfb->b.data	= tfb->d.data/tfb->c.data;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
		}
		else if ( tfb->d.data == 0 )
		{
			if ( tfb->b.data == 0 )
			{
				tfb->type	= PROPORTIONAL;
				tfb->k	= tfb->c.data/tfb->a.data;
				tfb->a.data	= 0.;
				tfb->b.data	= 0.;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
			else
			{
				tfb->type	= INERTIE;
				tfb->k	= tfb->c.data/tfb->b.data;
				tfb->a.data	= tfb->a.data/tfb->b.data;
				tfb->b.data	= 0.;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
		}
		else if ( tfb->b.data == 0 )
		{
/*		printf ( "a= %f	b= %f	c= %f	d= %f	step= %f\n", 
			tfb->a.data, tfb->b.data, tfb->c.data, tfb->d.data, t_step );
*/			tfb->type	= DIFFERENTIAL;
			tfb->b.data	= tfb->d.data/tfb->a.data;
			tfb->a.data	= tfb->c.data/tfb->a.data;
			tfb->c.data	= 0.;
			tfb->d.data	= 0.;
		}
		else
		{
			tfb->type	= INERTIE_PROPORTIONAL;
			tfb->k	= tfb->d.data/tfb->b.data;
			tfb->a.data	= tfb->a.data/tfb->b.data;
			tfb->b.data	= tfb->c.data/tfb->d.data;
			tfb->c.data	= 0.;
			tfb->d.data	= 0.;
		}
	}
	else if ( tfb->order == FT_SECOND_ORDER )
	{
		tfb->type	= OSCILATION2;
		a0	= sqrt( tfb->b.data- tfb->a.data*tfb->a.data/4. );
		a1	= tfb->a.data/2.;
		a2	= tfb->a.data/(2.*a0);
		a3	= tfb->b.data/a0;
		a4	= 1/tfb->b.data;
		a5	= (tfb->b.data*t_step-tfb->a.data)/(tfb->b.data*tfb->b.data);
		a6	= exp( -a1*t_step)* sin(a0*t_step);
		a7	= exp( -a1*t_step)* cos(a0*t_step);
	}

	fprintf ( fp_list, "type=%d\tk=%5.3f\ta=%5.3f\tb=%5.3f\tc=%5.3f\td=%5.3f",
		tfb->type, tfb->k, tfb->a.data, tfb->b.data, tfb->c.data, tfb->d.data );
		
	tfb->x1.value	= 0.;
	tfb->x2.value	= 0.;
	tfb->prev_input	= 0.;

	switch	( tfb->type )
	{
	case	INTEGRATION:
	case	PROPORTIONAL_INTEGRATION:
		tfb->e	= 1.;
		tfb->f	= tfb->k*t_step;
		tfb->g	= tfb->k*t_step*t_step/2.;
		break;

	case	PROPORTIONAL:
		tfb->e	= 0.;
		tfb->f	= tfb->k;
		tfb->g	= tfb->k*t_step;
		break;

	case	DIFFERENTIAL:

/*		printf ( "a= %f	b= %f	step= %f\n", tfb->a.data, tfb->b.data, t_step );
*/		tfb->e	= 0.;
		tfb->f	= tfb->a.data;
		tfb->g	= tfb->a.data*t_step+tfb->b.data;
		break;

	case	OSCILATION2:
		tfb->e2[0][0]	= a7+a2*a6;
		tfb->e2[0][1]	= a6/a0;
		tfb->e2[1][0]	= -a3*a6;
		tfb->e2[1][1]	= a7-a2*a6;
		tfb->f2[0]	= a4*(1.-a2*a6-a7);
		tfb->f2[1]	= a4*(a0+(a2*a2/a0))*a6;
		tfb->g2[0]	= a5+a4*a4*((a1*a1-a0*a0)*a6+2.*a0*a1*a7)/a0;
		tfb->g2[1]	= a4*(1.-(a1*a1*a1/a0+a1*a0)*a4*a6-a7);
		break;

	default:
		tfb->e	= exp ( -tfb->a.data*t_step );
		tfb->f	= tfb->k*( 1.-tfb->e )/tfb->a.data;
		tfb->g	= tfb->k*t_step/tfb->a.data+tfb->k*( tfb->e-1.)/(tfb->a.data*tfb->a.data);
		break;
	}

/*	printf ( "\ttfbname= %s\ttype=%d\te=%f\tf=%f\tg=%f\n",
		tfb->name, tfb->type, tfb->e, tfb->f, tfb->g );
*/	fprintf ( fp_list, "\ttype=%d\te=%f\tf=%f\tg=%f\n",
		tfb->type, tfb->e, tfb->f, tfb->g );

}

void	GetTFB_ModelCoefs ( MODEL_DEF *mdl )	/* 380	*/
{
	int	i;
	TFB_DEF	*tfb;
	double	t_step = mdl->t_step;

	fprintf ( fp_list, "\ntfb K & Coeff\n" );
	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		tfb	= &tfbdb[i];
		if ( tfb->coef_type != IS_NLF )	// if not nlf, following value will 
										// be calculated in off-line.
		{
			GetTFB_Coef ( t_step, tfb );
		}
	}
}




int CheckTFB_Name ( MODEL_DEF *mdl )
{
	int i;
	char name[DBNAME_SIZE];

	if ( mdl->tfb.end-mdl->tfb.head == 0 ) return 0;

	strcpy ( name, tfbdb[mdl->tfb.head].name );

	for ( i=mdl->tfb.head+1; i<mdl->tfb.end; i++ )
	{
		if ( !strcmp ( name, tfbdb[i].name ) )
		{
			return -1;
		}
	}
	return 0;
}


int GetTFB_ID ( MODEL_DEF *mdl, char *str )
{
	int i;
	
	if ( !strcmp ( str, "tfb_0" ) )
		return n_tfb;

	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		if ( !strcmp ( str, tfbdb[i].name ) )
		{
			return i;
		}
	}
	ErrMsg ( "node name not exist." );
	return -1;
}


//yyp deal with the connection of tfb
int	P_MatrixDataProc ( MODEL_DEF *mdl )
{
	char	str[DATANAME_LENTH];
	double p_tmp;
	int i, j;

	i = j = 0;
	

	for ( ;; )
	{
		CopyStringFromBuf ( str, DATANAME_LENTH );
		if ( !strncmp ( FACTOR, str, strlen ( FACTOR ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%hf", &p_tmp );
		}
		else if ( !strncmp ( TO, str, strlen ( TO ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			i = GetTFB_ID ( mdl, str );
			sparedb[n_spare].i = i;
		}
		else if ( !strncmp ( FROM, str, strlen ( FROM ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			j = GetTFB_ID ( mdl, str );
			sparedb[n_spare].j = j;
		}
		else if ( buf[pos] == LF || buf[pos] == CR || buf[0] == 0 )
		{
			break;
		}
		else
		{
			ErrMsg ( "Unknown keyword." );
		}
	}
	sparedb[n_spare].value = p_tmp;
	return 0;
}

void PrintP_Matrix ( MODEL_DEF *mdl )
{
	int i;

	fprintf ( fp_list, "\nP_MATRIX DATA\tn_spare:h/e= %d/%d\n", mdl->spare.head, mdl->spare.end );
	for ( i=mdl->spare.head; i<mdl->spare.end; i++ )
	{
		fprintf ( fp_list, "\ti= %d\tj= %d\t%6.4f\n", 
			sparedb[i].i, sparedb[i].j, sparedb[i].value );
	}
	fprintf ( fp_list, "\n" );

}


int	LoadP_MatrixData ( MODEL_DEF *mdl )
{

	mdl->spare.head	= n_spare;

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}
//		fprintf ( fp_list, "line %d, %s\n", txt_line_cnt, buf );

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if ( !strncmp ( END_MODEL, buf, strlen ( END_MODEL ) ) )
		{
			mdl->spare.end	= n_spare;
			break;
		}
		else if (( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ))
		{
//			fprintf ( fp_list, "in LoadP_MatrixData LF/CR\n" );
		}
		else
		{
			P_MatrixDataProc ( mdl );
			n_spare++;
		}

	}
//	printf ( "n_spare= %d\n", n_spare );

	PrintP_Matrix ( mdl );

	return 0;

}

//yyp deal with the param of tfb
int	P_ParamDataProc ( MODEL_DEF *mdl )
{
	sscanf ( buf, "%s	=	%s", paramdb[n_param].param_name,paramdb[n_param].pt_name);

	return 0;
}


//added by yyp 2009.4.13
int	LoadP_ParamData ( MODEL_DEF *mdl )
{

	mdl->param.head	= n_param;

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}
//		fprintf ( fp_list, "line %d, %s\n", txt_line_cnt, buf );

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if ( !strncmp ( END_MODEL, buf, strlen ( END_MODEL ) ) )
		{
			mdl->param.end	= n_param;
			break;
		}
		else if ( ( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ) )
		{
//			fprintf ( fp_list, "in LoadP_MatrixData LF/CR\n" );
		}
		else
		{
			P_ParamDataProc ( mdl );
			n_param++;
		}
	}
	//printf ( "n_param= %d\n", n_param );

	return 0;

}




//added by yyp 2009.7.8
int	LoadShowTxtFile ( )
{
	char tmp[128];
	char type[32];
	showdb[n_show].type = 0;
	showdb[n_show].delay_time =0;
	showdb[n_show].x = 0;
	showdb[n_show].y = 0;
	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}
		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{

		}
		else if ( !strncmp ( END_SHOW, buf, strlen ( END_SHOW ) ) )
		{
			break;
		}
		else if ( ( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ) )
		{

		}
		else if ( !strncmp( FILE_NAME, buf, strlen ( FILE_NAME ) ) )
		{
			sscanf(buf, "%s %s",tmp, showdb[n_show].filename );
		}
		else if ( !strncmp( TYPE, buf, strlen ( TYPE ) ) )
		{
			sscanf(buf, "%s %s",	tmp, type );

			if ( strcmp( type, SBIN ) == 0 )
			{
				showdb[n_show].type = 0;
			}
		}
		else if ( !strncmp( POS, buf, strlen ( POS ) ) )
		{
			sscanf(buf,"%s %s %d %s %d", tmp, tmp, &(showdb[n_show].x), tmp, &(showdb[n_show].y) );
		}
		else if ( !strncmp( DELAY, buf, strlen ( DELAY ) ) )
		{
			sscanf(buf,"%s %f", tmp, &( showdb[n_show].delay_time) );
			
		}
	}
	return TRUE;

}


void	SimModelDataCalc ()
{
	int	i;

	for ( i=0; i<n_model; i++ )
	{
		GetTFB_ModelCoefs ( &mdb[i] );
	}

}

int	GetNLF_Type ( char *str )
{
	if ( !strcmp ( str, "N1D" ) )
		return N1D;
	else if ( !strcmp ( str, "N2D" ) )
		return N2D;
	else
	{
		ErrMsg ( "Unknown nlf type." );
		return -1;
	}

}


int GetTFB_Linearity ( char *str )
{

	if ( !strcmp ( str, "LINEAR" ) )
		return LINEAR;
	else if ( !strcmp ( str, "PRE_SATURATION" ) )
		return PRE_SATURATION;
	else if ( !strcmp ( str, "PRE_INSENSITIVE" ) )
		return PRE_INSENSITIVE;
	else if ( !strcmp ( str, "PRE_GEAR_GAP" ) )
		return PRE_GEAR_GAP;
	else if ( !strcmp ( str, "PRE_RELAY" ) )
		return PRE_RELAY;
	else if ( !strcmp ( str, "PRE_INSENSITIVE_RELAY" ) )
		return PRE_INSENSITIVE_RELAY;
	else if ( !strcmp ( str, "PRE_INERTIE_RELAY" ) )
		return PRE_INERTIE_RELAY;
	else if ( !strcmp ( str, "PRE_NONLINEAR" ) )
		return PRE_NONLINEAR;
	else if ( !strcmp ( str, "POST_SATURATION" ) )
		return POST_SATURATION;
	else if ( !strcmp ( str, "POST_INSENSITIVE" ) )
		return POST_INSENSITIVE;
	else if ( !strcmp ( str, "POST_GEAR_GAP" ) )
		return POST_GEAR_GAP;
	else if ( !strcmp ( str, "POST_RELAY" ) )
		return POST_RELAY;
	else if ( !strcmp ( str, "POST_INSENSITIVE_RELAY" ) )
		return POST_INSENSITIVE_RELAY;
	else if ( !strcmp ( str, "POST_INERTIE_RELAY" ) )
		return POST_INERTIE_RELAY;
	else if ( !strcmp ( str, "POST_NONLINEAR" ) )
		return POST_NONLINEAR;
	else
	{
		ErrMsg ( "Unknown linearity type." );
		return LINEAR;
	}

}

int OneLineTFB_DataProc ( MODEL_DEF *mdl )
{
	char	str[DATANAME_LENTH];
	TFB_DEF	*tfb;

	tfb	= &tfbdb[n_tfb];
	//added by yyp
	if(tfb->order <= 1)
	{
		tfb->order = FT_GENERAL;
	}

	for ( ;; )
	{
		CopyStringFromBuf ( str, DATANAME_LENTH );
		if ( !strncmp ( A, str, strlen ( A ) ) )
		{
			GetDataAndDBNameFromStr ( &tfb->a, &tfb->coef_type );
		}
		else if ( !strncmp ( B, str, strlen ( B ) ) )
		{
			GetDataAndDBNameFromStr ( &tfb->b, &tfb->coef_type );
		}
		else if ( !strncmp ( C, str, strlen ( C ) ) )
		{
			GetDataAndDBNameFromStr ( &tfb->c, &tfb->coef_type );
		}
		else if ( !strncmp ( D, str, strlen ( D ) ) )
		{
			GetDataAndDBNameFromStr ( &tfb->d, &tfb->coef_type );
		}
		else if ( !strncmp ( X0, str, strlen ( X0 ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%hf", &tfb->x.value );
		}
		else if ( !strncmp ( Y0, str, strlen ( Y0 ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%hf", &tfb->output.value );
		}
		else if ( !strncmp ( ORDER, str, strlen ( ORDER ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%d", &tfb->order );
		}
		else if ( !strncmp ( LINEARITY, str, strlen ( LINEARITY ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			tfb->linearity = GetTFB_Linearity ( str );
		}
		else if ( !strncmp ( NLF_NAME, str, strlen ( NLF_NAME ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			sscanf ( str, "%s", &tfb->nlf_name );
		}
		else if ( !strncmp ( C1, str, strlen ( C1 ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%hf", &tfb->c1 );
		}
		else if ( !strncmp ( E1, str, strlen ( E1 ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%hf", &tfb->e1 );
		}
		else if ( !strncmp ( OUTPUT_DBNAME, str, strlen ( OUTPUT_DBNAME ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			sscanf ( str, "%s", &tfb->output.dbname );
			{
				tfb->output.flag	= ON;
			}
		}
		else if ( !strncmp ( OUTPUT_LONGNAME, str, strlen ( OUTPUT_LONGNAME ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			sscanf ( str, "%s", &tfb->output.longname );
		}
		else if ( !strncmp ( INPUT_DBNAME, str, strlen ( INPUT_DBNAME ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			sscanf ( str, "%s", &tfb->vin.dbname );
			{
				tfb->vin.flag	= ON;
			}
		}
		else if ( !strncmp ( INPUT_LONGNAME, str, strlen ( INPUT_LONGNAME ) ) )
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			sscanf ( str, "%s", &tfb->vin.longname );
		}
		else if ( buf[pos] == LF || buf[pos] == CR || ( buf[0] == 0 ))
		{
			break;
		}
		else
		{
			ErrMsg ( "Unknown keyword." );
		}
	}
	return 0; 
}


int	LoadTFB_Data ( MODEL_DEF *mdl )
{
	TFB_DEF	*tfb;
	char	str[DATANAME_LENTH];

	pos	= strlen ( TFB );
	CopyStringFromBuf ( str, DATANAME_LENTH );
	sscanf ( str, "%s", &tfbdb[n_tfb].name );	//yyp ??

	if ( CheckTFB_Name ( mdl ) == -1 )
	{
		ErrMsg ( "tfb name already exist." );
	}

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}

		if ( !strncmp ( TFB, buf, strlen ( TFB ) ) )
		{
			tfb	= &tfbdb[n_tfb];
			fprintf ( fp_list, "tfbData: n= %d name: %s order= %d linear:%d abcd= %5.2f %5.2f %5.2f %5.2f xy0: %5.2f %5.2f\n",
				n_tfb, tfb->name, tfb->order, tfb->linearity, 
				tfb->a, tfb->b, tfb->c, tfb->d, tfb->x.value, tfb->output.value );

			n_tfb++;

			LoadTFB_Data ( mdl );
			break;
		}
		else if ( !strncmp ( TFB_CONNECTION, buf, strlen ( TFB_CONNECTION ) ) )
		{
			tfb	= &tfbdb[n_tfb];
			fprintf ( fp_list, "tfbData: n= %d name: %s order= %d linear:%d abcd= %5.2f %5.2f %5.2f %5.2f xy0: %5.2f %5.2f\n",
				n_tfb, tfb->name, tfb->order, tfb->linearity, 
				tfb->a, tfb->b, tfb->c, tfb->d, tfb->x.value, tfb->output.value );

			n_tfb++;
			mdl->tfb.end	= n_tfb;
			if ( LoadP_MatrixData ( mdl ) == -1 )
			{
				fprintf ( fp_list, "LoadP_MatrixData Err\n" );
				return -1;
			}
			else
			{
				printf ( "LoadP_MatrixData OK\n" );
				break;
			}
		}
		else if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if (( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ))
		{
//			fprintf ( fp_list, "in LoadTFB_Data: LF/CR\n" );
		}
		else
		{
			OneLineTFB_DataProc ( mdl );
		}
	}

	return 0;

}

void	CheckGenUnitName ( char *name )
{
	int	i, err_tag = ON;

	if ( n_unit < 1 ) return;

	for ( i=0; i<n_unit; i++ )
	{
		if ( !strcmp ( name, unitdb[i].name ) )
		{
			err_tag	= OFF;
			break;
		}
	}

	if ( err_tag == OFF )
	{
		ErrMsg ( "Gen unit name already exists!" );
	}

}

void	CheckNLF_Name ( char *name )
{
	int	i, err_tag = ON;

	if ( n_nlf < 1 ) return;

	for ( i=0; i<n_nlf; i++ )
	{
		if ( !strcmp ( name, nlfdb[i].name ) )
		{
			err_tag	= OFF;
			break;
		}
	}

	if ( err_tag == OFF )
	{
		ErrMsg ( "NLF name already exists!" );
	}

}

void	CheckNLF_XY_Uniformity ( int n, double x[] )
{
	int	i;

	fprintf ( fp_list, "\tin CheckNLF_XY_Uniformity" );
	for ( i=0; i<n-1; i++ )
	{
		fprintf ( fp_list, "\t%f", x[i] );
		if ( x[i] > x[i+1] )
		{
			fprintf ( fp_list, "\n" );
			ErrMsg ( "NLF X/Y dis-uniform." );
			break;
		}
	}

}

void	ReadNLF_DataLine ( int *n_nlf_f, int max, NLF_DEF *nlf, double x[] )
{
	char	str[DATANAME_LENTH];

//	printf ( "n_nlf_f=%d\n", *n_nlf_f );
	for ( ;; )
	{
		if ( *n_nlf_f >= max )
		{
			fprintf ( fp_list, "Line %d: NLF %s data buffer overflow.\n", 
				txt_line_cnt, nlf->name );
			break;
		}

		CopyStringFromBuf ( str, DATANAME_LENTH );
		if (( str[0]  == LF ) || ( str[0]  == CR ) || ( str[0] == 0 ))
		{
//			printf ( "\n" );
			break;
		}
		else
		{
			sscanf ( str, "%hf", &x[*n_nlf_f] );
//			printf ( "f=%f	", x[*n_nlf_f] );
		}

		*n_nlf_f	= *n_nlf_f + 1;
	}
//	printf ( "n_nlf_f=%d\n", *n_nlf_f );
}


int	LoadNLF_Data ( NLF_DEF *nlf )
{
	DMS_COMMON	dms_cmm;
	int	data_type;
	char	str[DATANAME_LENTH];

	fprintf ( fp_list, "in begin LoadNLF_Data\n" );

	pos	= strlen ( FUNCTION );
	CopyStringFromBuf ( str, DATANAME_LENTH );
	sscanf ( str, "%s", &nlf->name );

	CheckNLF_Name ( nlf->name );

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if (( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ))
		{
//			fprintf ( fp_list, "in LoadNLF_Data: LF/CR\n" );
		}
		else if ( !strncmp ( END_FUNCTION, buf, strlen ( END_FUNCTION ) ) )
		{
			nlf->x.end	= n_nlf_x;
			nlf->y.end	= n_nlf_y;
			nlf->f.end	= n_nlf_crv;
			nlf_ptr[n_nlf_crv].end	= n_nlf_f;
			if ( nlf->nlf_type == N1D )
			{
				if ( ( nlf->x.end - nlf->x.head ) != 
					( nlf_ptr[n_nlf_crv].end - nlf_ptr[n_nlf_crv].head ))
				{
					ErrMsg ( "X-F points number dismatch." );
				}
				CheckNLF_XY_Uniformity ( nlf->x.end-nlf->x.head, &nlfdata.x[nlf->x.head] );
			}
			else if ( nlf->nlf_type == N2D )
			{
				if ( ( nlf->x.end - nlf->x.head )*( nlf->y.end - nlf->y.head ) != 
					( nlf_ptr[nlf->f.end].end - nlf_ptr[nlf->f.head].head ))
				{
					ErrMsg ( "XY-F points number dismatch." );
				}
				CheckNLF_XY_Uniformity ( nlf->x.end-nlf->x.head, &nlfdata.x[nlf->x.head] );
				CheckNLF_XY_Uniformity ( nlf->y.end-nlf->y.head, &nlfdata.y[nlf->y.head] );
			}
			break;
		}
		else
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			if ( !strncmp ( NLF_TYPE, str, strlen ( NLF_TYPE ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				nlf->nlf_type = GetNLF_Type ( str );
			}
			else if ( !strncmp ( X_NAME, str, strlen ( X_NAME ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				sscanf ( str, "%s", &nlf->xname );

				if ( ReadEntryByNameStr ( nlf->xname, &dms_cmm ) == -1 )
				{
					ErrMsg ( "DB entry is not found in the database file!" );
				}

				data_type	= dms_cmm.point.data_type;	// added by wdk 2008.3.22

				if ( data_type != DPS_ANALOG )
				{
					ErrMsg ( "DB entry is not analog!" );
				}
			}
			else if ( !strncmp ( Y_NAME, str, strlen ( Y_NAME ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				sscanf ( str, "%s", &nlf->yname );

				if ( ReadEntryByNameStr ( nlf->yname, &dms_cmm ) == -1 )
				{
					ErrMsg ( "DB entry is not found in the database file!" );
				}
				if ( data_type != DPS_ANALOG )
				{
					ErrMsg ( "DB entry is not analog!" );
				}

				if ( nlf->nlf_type != N2D )
					ErrMsg ( "Y_NAME defined for N1D Function." );
			}
			else if ( !strncmp ( F_NAME, str, strlen ( F_NAME ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				sscanf ( str, "%s", &nlf->fname );
			}
			else if ( !strncmp ( X_DATA, str, strlen ( X_DATA ) ) )
			{
				nlf_rd_st	= RD_NLF_X;
				ReadNLF_DataLine ( &n_nlf_x, MAX_NLF_X, nlf, nlfdata.x );
			}
			else if ( !strncmp ( Y_DATA, str, strlen ( Y_DATA ) ) )
			{
				if ( n_nlf_y >= MAX_NLF_Y )
				{
					fprintf ( fp_list, "Line %d: NLF %s Y data buffer overflow.\n", 
						txt_line_cnt, nlf->name );
					break;
				}

				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &nlfdata.y[n_nlf_y] );

				n_nlf_y++;
			}
			else if ( !strncmp ( F_DATA, str, strlen ( F_DATA ) ) )
			{
				nlf_rd_st	= RD_NLF_F;
				nlf_ptr[n_nlf_crv].head	= n_nlf_f;
				ReadNLF_DataLine ( &n_nlf_f, MAX_NLF_F, nlf, nlfdata.f );
				nlf_ptr[n_nlf_crv].end	= n_nlf_f;
				n_nlf_crv++;
			}
			else if ( !strncmp ( NEXT_LINE, str, strlen ( NEXT_LINE ) ) )
			{
				if ( nlf_rd_st == RD_NLF_X )
				{
					ReadNLF_DataLine ( &n_nlf_x, MAX_NLF_X, nlf, nlfdata.x );
				}
				else if ( nlf_rd_st == RD_NLF_F )
				{
					n_nlf_crv--;
					ReadNLF_DataLine ( &n_nlf_f, MAX_NLF_F, nlf, nlfdata.f );
					nlf_ptr[n_nlf_crv].end	= n_nlf_f;
					n_nlf_crv++;
				}
			}
			else
			{
				ErrMsg ( "Unknown keyword." );
			}
		}
	}

	fprintf ( fp_list, "in end LoadNLF_Data\n\n" );
	return 0;

}


int	LoadGenUnitData ( UNIT_DATA_DEF *unit )
{
	char	str[DATANAME_LENTH];

	fprintf ( fp_list, "in begin LoadGenUnitData\n" );

	pos	= strlen ( GEN_UNIT );
	CopyStringFromBuf ( str, DATANAME_LENTH );
	sscanf ( str, "%s", &unit->name );

	CheckGenUnitName ( unit->name );

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if (( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ))
		{
//			fprintf ( fp_list, "in LoadGenUnitData: LF/CR\n" );
		}
		else if ( !strncmp ( END_GEN_UNIT, buf, strlen ( END_GEN_UNIT ) ) )
		{
			n_unit++;
			break;
		}
		else
		{
			CopyStringFromBuf ( str, DATANAME_LENTH );
			if ( !strncmp ( PN, str, strlen ( PN ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->pn );
			}
			else if ( !strncmp ( QN, str, strlen ( QN ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->qn );
			}
			else if ( !strncmp ( HN, str, strlen ( HN ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->hn );
			}
			else if ( !strncmp ( NN, str, strlen ( NN ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->nn );
			}
			else if ( !strncmp ( YMAX, str, strlen ( YMAX ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->ymax );
			}
			else if ( !strncmp ( DIAMETER, str, strlen ( DIAMETER ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->diameter );
			}
			else if ( !strncmp ( TA, str, strlen ( TA ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->ta );
			}
			else if ( !strncmp ( TW, str, strlen ( TW ) ) )
			{
				CopyStringFromBuf ( str, DATANAME_LENTH );
				CheckStrIsNumeric ( str );
				sscanf ( str, "%hf", &unit->tw );
			}
			else
			{
				ErrMsg ( "Unknown keyword." );
			}
		}
	}

	fprintf ( fp_list, "in end LoadGenUnitData\n\n" );
	return 0;

}


int	LoadModelData ( MODEL_DEF *mdl )
{
	char	str[DATANAME_LENTH];

	mdl->tfb.head	= n_tfb;
	fprintf ( fp_list, "in begin LoadModelData\n" );

	pos	= strlen ( MODEL );
	CopyStringFromBuf ( str, DATANAME_LENTH );
	sscanf ( str, "%s", &mdl->name );
//	printf ( "model %d name: %s\n", n_model, mdl->name );

	if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
	{
		fprintf ( fp_list, "The file is empty! Exit now!\n" );
		return -1;
	}

//	fprintf ( fp_list, "%3d:%s\n", txt_line_cnt, buf );

	for ( ;; )
	{
		if ( n_tfb >= MAX_TFB_NUM )
		{
			ErrMsg ( "tfbdb buffer overflow." );
			break;
		}

 		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if (( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ))
		{
//			fprintf ( fp_list, "in LoadModelData: LF/CR\n" );
		}
		else if ( !strncmp ( END_MODEL, buf, strlen ( END_MODEL ) ) )
		{
			break;
		}
		else if ( !strncmp ( TIME_STEP, buf, strlen ( TIME_STEP ) ) )
		{
			pos = strlen ( TIME_STEP );
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%hf", &mdl->t_step );
		}
		else if ( !strncmp ( SIM_STEPS, buf, strlen ( SIM_STEPS ) ) )
		{
			pos = strlen ( SIM_STEPS );
			CopyStringFromBuf ( str, DATANAME_LENTH );
			CheckStrIsNumeric ( str );
			sscanf ( str, "%hd", &mdl->iterations );
		}
		else if ( !strncmp ( TYPE, buf, strlen ( TYPE ) ) )
		{
			pos = strlen ( TYPE );
			CopyStringFromBuf ( str, DATANAME_LENTH );
			//CheckStrIsNumeric ( str );
			sscanf ( str, "%s", &mdl->type);
		}
		else if ( !strncmp ( TEMPLATE, buf, strlen ( TEMPLATE ) ) )
		{
			pos = strlen ( TEMPLATE );
			CopyStringFromBuf ( str, DATANAME_LENTH );
			//CheckStrIsNumeric ( str );
			sscanf ( str, "%s", &mdl->template_name );
		}
		else if ( !strncmp ( TFB, buf, strlen ( TFB ) ) )
		{
			LoadTFB_Data ( mdl );
			break;
		}
		else if ( !strncmp ( PARAM, buf, strlen ( PARAM ) ) )
		{
			LoadP_ParamData ( mdl )	;		
			break;
		}
		else
		{
			ErrMsg ( "Unknown keyword." );
		}
	}

	fprintf ( fp_list, "in end LoadModelData\n\n" );
	return 0;

}



int	LoadDescriptionFile ( char *f_name )
{
	txt_line_cnt	= pos	= 0;

	fprintf ( fp_list, "in begin LoadDescriptionFile\n\n" );
	fprintf ( fp_err, "\nbegin compile %s\n\n", f_name );

	if ( ( fp_txt = fopen ( f_name, "r" ) ) == NULL )
	{
		printf ( "Can't open (r) the %s\n", f_name );
		exit ( -1 );
	}

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_txt ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if (( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ))
		{
//			fprintf ( fp_list, "in LoadSystemDescriptionFile: LF/CR\n" );
		}
		else if ( !strncmp ( MODEL, buf, strlen ( MODEL ) ) )
		{
			if ( n_model >= MAX_MODEL_NUMBER )
			{
				ErrMsg ( "model number overflow." );
				break;
			}
			else
			{
				LoadModelData ( &mdb[n_model] );
				n_model++;
			}
		}
		else if ( !strncmp ( FUNCTION, buf, strlen ( FUNCTION ) ) )
		{
			if ( n_nlf >= MAX_NLF )
			{
				ErrMsg ( "function number overflow." );
				break;
			}
			else
			{
				nlfdb[n_nlf].x.head	= n_nlf_x;
				nlfdb[n_nlf].y.head	= n_nlf_y;
				nlfdb[n_nlf].f.head	= n_nlf_crv;
				nlf_ptr[n_nlf_crv].head	= n_nlf_f;
				LoadNLF_Data ( &nlfdb[n_nlf] );
				n_nlf++;
			}
		}
		else if ( !strncmp ( SEQUENCE, buf, strlen ( SEQUENCE ) ) )
		{
			char	ch[DBNAME_SIZE];

			if ( n_sequence >= MAX_SEQUENCE )
			{
				ErrMsg ( "sequence number overflow." );
				return -1;
			}
			CopyStringFromBuf ( ch, DBNAME_SIZE );
			sequencedb[n_sequence].num	= 0;
			CopyStringFromBuf ( sequencedb[n_sequence].seq_name, DBNAME_SIZE );
			sequencedb[n_sequence].parentid = -1;
			sequencedb[n_sequence].type  = 0;
			CheckSequenceName ( );

			if ( LoadSequenceTxtFile () == -1 )
			{
				printf ( "file:%s error! No file generated!\n", f_name );
				return -1;
			}

			n_sequence++;
		}
		else if ( !strncmp ( SCENE, buf, strlen ( SCENE ) ) )
		{
			char	ch[DBNAME_SIZE];

			if ( n_scene >= MAX_SCENE )
			{
				ErrMsg ( "scene number overflow." );
				return -1;
			}
			CopyStringFromBuf ( ch, DBNAME_SIZE );
			scenedb[n_scene].num	= 0;
			CopyStringFromBuf ( scenedb[n_scene].scene_name, DBNAME_SIZE );
			//CheckSequenceName ( );

			if ( LoadSceneTxtFile () == -1 )
			{
				printf ( "file:%s error! No file generated!\n", f_name );
				return -1;
			}
			n_scene++;
		}
		else if ( !strncmp ( SHOW, buf, strlen ( SHOW ) ) )
		{
			char	ch[SHOWNAME_SIZE];

			if ( n_show >= MAX_SHOW )
			{
				ErrMsg ( "show number overflow." );
				return -1;
			}
			CopyStringFromBuf ( ch, SHOWNAME_SIZE );
			CopyStringFromBuf ( showdb[n_show].name, SHOWNAME_SIZE );
			//CheckSequenceName ( );

			if ( LoadShowTxtFile () == -1 )
			{
				printf ( "file:%s error! No file generated!\n", f_name );
				return -1;
			}
			n_show++;
		}
		else if ( !strncmp ( GEN_UNIT, buf, strlen ( GEN_UNIT ) ) )
		{
			if ( n_unit >= MAX_UNIT_NUMBER )
			{
				ErrMsg ( "gen unit number overflow." );
				break;
			}
			else
			{
				LoadGenUnitData ( &unitdb[n_unit] );
			}
		}
		else
		{
			ErrMsg ( "Unknown keyword." );
		}
	}

	fclose ( fp_txt );
	fprintf ( fp_list, "in end LoadDescriptionFile\n" );
	return 0;

}

int	LoadSystemMainFile ( char *f_name )
{
	txt_line_cnt	= pos	= 0;
	n_model	= 0;
	n_timer	= 0;
	n_nlf	= 0;
	n_nlf_x	= 0;
	n_nlf_y	= 0;
	n_nlf_f	= 0;
	n_tfb	= 0;
	n_param = 0;
	n_spare	= 0;
	n_sequence	= 0;
	n_step	= -1;
	n_group	= -1;
	n_init	= 0;
	n_entry	= 0;
	n_unit	= 0;
	n_nlf_crv	= 0;

	fprintf ( fp_list, "in begin LoadSystemMainFile\n\n" );

	if ( GetOneLineFromTxtFile ( fp_main ) == -1 )
	{
		fprintf ( fp_list, "The file is empty! Exit now!\n" );
		return -1;
	}

	if ( strncmp ( FILE_NAME, &buf[pos], strlen ( FILE_NAME )) )
	{
		fprintf ( fp_list, "FILE_NAME is missed: The first line is %s. Add filename!\n", buf );
	}
//	fprintf ( fp_list, "%3d:%s\n", txt_line_cnt, buf );

	for ( ;; )
	{
		if ( GetOneLineFromTxtFile ( fp_main ) == -1 )
		{
			fprintf ( fp_list, "The file now EOF without END_FILE!\n" );
			return -1;
		}

		if ( !strncmp ( "!", buf, 1 ) || !strncmp ( "c", buf, 1 ) || 
			!strncmp ( "C", buf, 1 ) )
		{
//			fprintf ( fp_list, "Line %d comment: %s\n", txt_line_cnt, buf );
		}
		else if (( buf[0]  == LF ) || ( buf[0]  == CR ) || ( buf[0] == 0 ))
		{
//			fprintf ( fp_list, "in LoadSystemDescriptionFile: LF/CR\n" );
		}
		else if ( !strncmp ( INCLUDE, buf, strlen ( INCLUDE ) ) )
		{
			pos	= strlen ( INCLUDE );
			CopyStringFromBuf ( txt_fname, LINE_BUF_LENTH );
			strcpy ( txt_fullname, SIMULOG_DATA_HOME );
			strcat ( txt_fullname, txt_fname );
			printf ( "fname: %s\n", txt_fullname );
			LoadDescriptionFile ( txt_fullname );
		}
		else if ( !strncmp ( END_FILE, buf, strlen ( END_FILE ) ) )
		{
			fprintf ( fp_err, "\n" );
			SimModelDataCalc ();
			CheckSystemNames ();
			WriteSimulogDataFile ();
			printf ( "\nEND_FILE reached. Total sq= %d, mdl = %d, sce=%d, st= %d, shw=%d gt= %d et= %d exit now!\n",
				 n_sequence,n_model,n_scene, n_step,n_show, n_group, n_entry );
			return 0;
		}
		else
		{
			ErrMsg ( "Unknown keyword." );
		}
	}

	fprintf ( fp_list, "in end LoadSystemMainFile\n" );
	return 0;

}


void	main ()
{
	int	imd;

	imd = 0;
	//yyp 2008.3.25
	if(MountDB(DB_NAME) == -1)
	{
		printf("call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}

	if ( ( fp_main = fopen ( mainfile, "r" ) ) == NULL )
	{
		printf ( "Can't open (r) the %s\n", mainfile );
		exit ( -1 );
	}

	if ( ( fp_err = fopen ( errfile, "w" ) ) == NULL )
	{
		printf ( "simugen.c:can't open (w) the file %s!\n", errfile );
		exit ( -1 );
	}

	if ( ( fp_list = fopen ( listfile, "w" ) ) == NULL )
	{
		printf ( "simugen.c:can't open (w) the file %s!\n", listfile );
		exit ( -1 );
	}

	if ( LoadSystemMainFile ( mainfile ) == -1 )
	{
		fprintf ( fp_list, "file:%s error! No file generated!\n", mainfile );
	}

	fprintf ( fp_err, "\n\nsimugen compile %d err.\n\n", err_cnt );

	fprintf ( fp_err, "\n\nsimugen compile %d warn.\n\n", wrn_cnt );
	
	fclose ( fp_main );
	fclose ( fp_err );
	fclose ( fp_list );

	UnMountDB ();
	if ( err_cnt == 0  )
	{
		printf ( "\n\nsimugen end ok, warn =%d\n\n",wrn_cnt );
	}
	else
	{
		printf ( "\n\nsimugen compile %d err, %d warn refer to simulogErr.txt for details.\n\n", err_cnt,wrn_cnt );
	}
	
	MySleep(10000);

}

