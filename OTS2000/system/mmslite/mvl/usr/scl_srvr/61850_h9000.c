#include "glbtypes.h"
#include "sysincs.h"
#include "sx_log.h"
#include "uca_obj.h"
#include "sx_log.h"
#include "str_util.h"	/* for strn..._safe	protos	*/

#include "uca_srvr.h"	/* User Specified */

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__; 
#endif


#include "61850_h9000.h"

DOI_DESC_INFO doi_desc_info;
MAN_MAP_LIST man_map_list;

#define LINE_BUF_LEN	1024	/* max len of line read from file.	*/

#define SDI_NAME_QUOTE	"NO_SDI"

ST_RET FindH9000PointFromManDataMapList(ST_CHAR *ld_name, ST_CHAR *leaf_name, ST_CHAR *pt_name)
{
	int i;
	ST_CHAR ln_name[256], doi_name[256], sdi_name[256]; 	

	if(SD_FAILURE==GetLeafPartialName(leaf_name, ln_name, doi_name, sdi_name))
	{
		SXLOG_ERR2("Fail to get partial name: %s %s", ld_name, leaf_name);
		return SD_FAILURE;
	}

	if(!sdi_name[0])
		strcpy(sdi_name, "NO_SDI");

	for(i=0; i<man_map_list.num; i++)
	{
		if(strcmp(man_map_list.entry[i].ld_name, ld_name))
			continue;
		if(strcmp(man_map_list.entry[i].ln_name, ln_name))
			continue;
		if(strcmp(man_map_list.entry[i].doi_name, doi_name))
			continue;
		if(strcmp(man_map_list.entry[i].sdi_name, sdi_name))
			continue;
		//find same entry in list
		strcpy(pt_name, man_map_list.entry[i].db_name);
		return SD_SUCCESS; 
	}
	return SD_FAILURE;
}

ST_RET man_datamap_cfg_read(char *in_filename)
{
	FILE *fp;
	ST_CHAR in_buf[LINE_BUF_LEN];	/* buffer for one line read from file	*/
	ST_CHAR *curptr;		/* ptr to current position in token_buf	*/
	char seps[] = ",\t\n";
	int	line_num= 0;
	ST_CHAR	*ld_name, *ln_name, *doi_name, *sdi_name, *db_name;
	ST_RET retcode = SD_SUCCESS;  

	memset(&man_map_list, 0, sizeof(man_map_list));

	if(NULL==(fp= fopen(in_filename, "r")))
	{
		SXLOG_ERR1("Fail to open %s", in_filename);
		return SD_SUCCESS;
	}

	/* Read one line at a time from the input file	*/
	while (fgets (in_buf, sizeof(in_buf) - 1, fp) != NULL)
	{
		line_num++;

		/* If string is too close to max len, we probably did not read	*/
		/* complete line. DO NOT allow that.				*/
		if (strlen (in_buf) > (sizeof (in_buf) - 3))
		{
			/* If this happens, increase value of LINE_BUF_LEN.		*/
			SXLOG_ERR2 ("Line exceeds maximum length '%d' in '%s'. Cannot parse.",
				LINE_BUF_LEN, in_filename);

			man_map_list.num= 0;
			retcode = SD_FAILURE;
			break;
		}
		/* NOTE: get_next_string modifies the input buffer.			*/
		/*       Here we're just counting lines, so that is OK.		*/
		curptr = in_buf;	/* init "curptr"	*/
		ld_name = get_next_string(&curptr, seps);	/* "curptr" adjusted by this funct	*/

		/* If NULL, this is empty line. If first char is '#', this is comment line.*/
		if (ld_name == NULL || ld_name[0] == '#' || ld_name[0] == '!')
			continue;		/* Ignore empty lines & comment lines	*/
		if (ld_name[0] == '\0')
		{
			/* First token is empty. This is probably empty line.	*/
			/* Ignore this line, but log error if more tokens found. 	*/
			if ((ln_name = get_next_string (&curptr, seps)) != NULL)
				SXLOG_ERR3 ("Input ignored because first token is empty at line %d in '%s'. Second token='%s'",
				line_num, in_filename, ln_name);
			continue;
		}

		/* NOTE: if one get_next_string returns NULL, subsequent calls should also.*/
		ln_name   = get_next_string (&curptr, seps);
		doi_name = get_next_string (&curptr, seps);
		sdi_name = get_next_string (&curptr, seps);
		db_name = get_next_string (&curptr, seps);
		/* NOTE: If more columns of input are needed, add a call to	*/
		/*       "get_next_string" for each additional column.	*/

		if (ln_name && doi_name && sdi_name && db_name)
		{
			if (strlen(ld_name) < sizeof(man_map_list.entry[man_map_list.num].ld_name))
				strcpy (man_map_list.entry[man_map_list.num].ld_name, ld_name);
			else 
			{
				SXLOG_ERR3 ("LD name '%s' too long at line %d in '%s'.",
					ld_name, line_num, in_filename);
				retcode = SD_FAILURE;
				break;
			}

			if (strlen(ln_name) < sizeof(man_map_list.entry[man_map_list.num].ln_name))
				strcpy (man_map_list.entry[man_map_list.num].ln_name, ln_name);
			else 
			{
				SXLOG_ERR3 ("LN name '%s' too long at line %d in '%s'.",
					ln_name, line_num, in_filename);
				retcode = SD_FAILURE;
				break;
			}

			if (strlen(doi_name) < sizeof(man_map_list.entry[man_map_list.num].doi_name))
				strcpy (man_map_list.entry[man_map_list.num].doi_name, doi_name);
			else 
			{
				SXLOG_ERR3 ("DOI name '%s' too long at line %d in '%s'.",
					doi_name, line_num, in_filename);
				retcode = SD_FAILURE;
				break;
			}

			if (strlen(sdi_name) < sizeof(man_map_list.entry[man_map_list.num].sdi_name))
				strcpy (man_map_list.entry[man_map_list.num].sdi_name, sdi_name);
			else 
			{
				SXLOG_ERR3 ("SDI name '%s' too long at line %d in '%s'.",
					sdi_name, line_num, in_filename);
				retcode = SD_FAILURE;
				break;
			}

			if (strlen(db_name) < sizeof(man_map_list.entry[man_map_list.num].db_name))
				strcpy (man_map_list.entry[man_map_list.num].db_name, db_name);
			else 
			{
				SXLOG_ERR3 ("DB name '%s' too long at line %d in '%s'.",
					db_name, line_num, in_filename);
				retcode = SD_FAILURE;
				break;
			}

			man_map_list.num++;
		}
		else
		{
			SXLOG_ERR2 ("Invalid input at line %d in '%s'. Must contain 5 parameters.", line_num, in_filename);
			SXLOG_CERR1 ("%s", in_buf);
			retcode = SD_FAILURE;
			break;
		}

	}	/* end "while (fgets..)"	*/

	fclose(fp);
	return retcode;
}

ST_RET WriteManMapOutFile(ST_CHAR *ld_name, ST_CHAR *leaf_name, ST_CHAR *desc)
{
	static MAN_MAP_LIST temp_map_list;
	int		i;
	FILE	*fp_man_map_out;
	ST_CHAR ln_name[256], doi_name[256], sdi_name[256]; 	

	if(SD_FAILURE==GetLeafPartialName(leaf_name, ln_name, doi_name, sdi_name))
	{
		SXLOG_ERR2("Fail to get partial name: %s %s", ld_name, leaf_name);
		return SD_FAILURE;
	}

	for(i=0; i<temp_map_list.num; i++)
	{
		if(strcmp(temp_map_list.entry[i].ld_name, ld_name))
			continue;
		if(strcmp(temp_map_list.entry[i].ln_name, ln_name))
			continue;
		if(strcmp(temp_map_list.entry[i].doi_name, doi_name))
			continue;
		if(strcmp(temp_map_list.entry[i].sdi_name, sdi_name))
			continue;

		return SD_SUCCESS; //same entry in list
	}

	if(fp_man_map_out= fopen("man_datamapout.cfg", "a"))
	{
		if(sdi_name[0])
			fprintf(fp_man_map_out, "%s\t%s\t%s\t%s\t\"NULL\"\t\"%s\"\n", ld_name, ln_name, doi_name, sdi_name, desc);
		else
			fprintf(fp_man_map_out, "%s\t%s\t%s\t%s\t\"NULL\"\t\"%s\"\n", ld_name, ln_name, doi_name, SDI_NAME_QUOTE, desc);

		fclose(fp_man_map_out);
	}

	if(temp_map_list.num<MAX_MAN_MAP_NUM)
	{
		strcpy(temp_map_list.entry[temp_map_list.num].ld_name, ld_name);
		strcpy(temp_map_list.entry[temp_map_list.num].ln_name, ln_name);
		strcpy(temp_map_list.entry[temp_map_list.num].doi_name, doi_name);
		strcpy(temp_map_list.entry[temp_map_list.num].sdi_name, sdi_name);
		temp_map_list.num++;
		return SD_SUCCESS;
	}
	else
	{
		SXLOG_ERR3("Fail to add %s %s man_map record: arrive max num %d", ld_name, leaf_name, MAX_MAN_MAP_NUM);
		return SD_FAILURE;
	}
}



ST_RET AddLdH9000Name(ST_CHAR *ld_name, ST_CHAR *h9000_name)
{
	if(ld_define.ld_num<MVL_NUM_DYN_DOMS)
	{
		strcpy(ld_define.ld_array[ld_define.ld_num].name, ld_name);
		strcpy(ld_define.ld_array[ld_define.ld_num].h9000_name, h9000_name);
		ld_define.ld_num++;
		return SD_SUCCESS;
	}
	else
	{
		SXLOG_ERR2("Fail to add ld %s h9000 name: arrive max ld num %d", ld_name, MVL_NUM_DYN_DOMS);
		return SD_FAILURE;
	}
}


ST_RET GetLdH9000Name(ST_CHAR *ld_name, ST_CHAR *h9000_name)
{
	int ldx;

	for(ldx=0; ldx<ld_define.ld_num; ldx++)
	{
		if(!strcmp(ld_define.ld_array[ldx].name, ld_name))
		{
			strcpy(h9000_name, ld_define.ld_array[ldx].h9000_name);
			return SD_SUCCESS;
		}
	}
	SXLOG_ERR1("Fail to find ld %s define", ld_name);
	return SD_FAILURE;
}

static time_t	DateTime2AbsTime(DATE_TIME *date_time)
{
	struct	tm	tm_time;

	tm_time.tm_isdst= 0;
	tm_time.tm_year= date_time->year-1900;
	tm_time.tm_mon= date_time->month-1;
	tm_time.tm_mday= date_time->day;
	tm_time.tm_hour= date_time->hour;
	tm_time.tm_min= date_time->minute;
	tm_time.tm_sec= date_time->second;

	return mktime(&tm_time);
}

ST_VOID GetCurrUTCtime(MMS_UTC_TIME *utc_time)
{
	DATE_TIME	date_time;
	time_t time_s;

	GetSysTimeEx(&date_time, &time_s);
	utc_time->secs = (ST_UINT32) time_s;	/* Update seconds in UTC time	*/
	utc_time->fraction = (ST_UINT32)((ST_DOUBLE)date_time.ms / 1000.0 * (ST_DOUBLE)0x01000000); /* milliseconds*/
	utc_time->qflags =  0x03;	/* 3 bits time accuracy	*/
}

ST_RET DateTime2UTCtime(DATE_TIME *date_time, MMS_UTC_TIME *utc_time)
{
	time_t time_s;
	
	time_s= DateTime2AbsTime(date_time);
	utc_time->secs = (ST_UINT32) time_s;	/* Update seconds in UTC time	*/
	utc_time->fraction = (ST_UINT32)((ST_DOUBLE)date_time->ms / 1000.0 * (ST_DOUBLE)0x01000000); /* milliseconds*/
	utc_time->qflags =  0x03;	/* 3 bits time accuracy	*/
	return SD_SUCCESS;
}


ST_RET GetLeafPartialName(ST_CHAR *leaf_name, ST_CHAR *ln_name, ST_CHAR *doi_name, ST_CHAR *sdi_name)
{
	ST_CHAR nameBuf[MAX_IDENT_LEN+1];
	ST_CHAR *nameToFind;
	ST_CHAR *compEnd;

	strcpy(nameBuf, leaf_name);
	nameToFind = nameBuf;

	compEnd= strchr(nameToFind, '$');
	if(!compEnd)
		return SD_FAILURE;

	*compEnd= 0;
	strcpy(ln_name, nameToFind);

	nameToFind = compEnd+1;
	compEnd= strchr(nameToFind, '$');
	if(!compEnd)
		return SD_FAILURE;

	nameToFind = compEnd+1;
	compEnd= strchr(nameToFind, '$');
	if(!compEnd)
		return SD_FAILURE;

	*compEnd= 0;
	strcpy(doi_name, nameToFind);

	nameToFind = compEnd+1;
	compEnd= strchr(nameToFind, '$');
	if(compEnd)
	{
		*compEnd= 0;
		strcpy(sdi_name, nameToFind);
	}
	else
		sdi_name[0]= 0;

	return SD_SUCCESS;
}

ST_RET GetDoiLeafDesc(ST_CHAR *ld_name, ST_CHAR *leaf_name, ST_CHAR *desc)
{
	int idx;
	ST_CHAR ln_name[MAX_IDENT_LEN+1];
	ST_CHAR doi_name[MAX_IDENT_LEN+1];
	ST_CHAR sdi_name[MAX_IDENT_LEN+1];

	if(GetLeafPartialName(leaf_name, ln_name, doi_name, sdi_name))
		return SD_FAILURE;
	
	for(idx=0; idx<doi_desc_info.num; idx++)
	{
		if(strcmp(ld_name, doi_desc_info.entry[idx].ld_name))
			continue;

		if(strcmp(ln_name, doi_desc_info.entry[idx].ln_name))
			continue;

		if(strcmp(doi_name, doi_desc_info.entry[idx].doi_name))
			continue;

		if(SIMPLE_DOI_TYPE == doi_desc_info.entry[idx].type)
		{
			strcpy(desc, doi_desc_info.entry[idx].desc);
			return SD_SUCCESS;
		}
		else if(SIMPLE_SDI_TYPE == doi_desc_info.entry[idx].type)
		{
			if(!strcmp(sdi_name, doi_desc_info.entry[idx].sdi_name))
			{
				strcpy(desc, doi_desc_info.entry[idx].desc);
				return SD_SUCCESS;
			}
		}
	}
	return SD_FAILURE;
}

ST_RET GetFcName(ST_CHAR *leaf_name, ST_CHAR *fcName)
{
	ST_CHAR nameBuf[MAX_IDENT_LEN+1];
	ST_CHAR *nameToFind;
	ST_CHAR *compEnd;

	strcpy(nameBuf, leaf_name);
	nameToFind = nameBuf;

	compEnd= strchr(nameToFind, '$');
	if(!compEnd)
		return SD_FAILURE;

	nameToFind = compEnd+1;
	compEnd= strchr(nameToFind, '$');
	if(!compEnd)
		return SD_FAILURE;

	*compEnd= 0;
	strcpy(fcName, nameToFind);
	return SD_SUCCESS;
}

ST_RET GetStnDevIdbyName(ST_CHAR *nameStr, POINTER *pt)
{
	ST_CHAR nameBuf[MAX_IDENT_LEN+1];
	ST_CHAR *nameToFind;
	ST_CHAR *compEnd;

	strcpy(nameBuf, nameStr);
	nameToFind = nameBuf;

	compEnd= strchr(nameToFind, '.');
	if(!compEnd)
		return SD_FAILURE;

	*compEnd= 0;
	compEnd++;

	if(-1==GetStnIdByName(nameBuf, &pt->stn_id))
		return SD_FAILURE;
	if(-1==GetGroupIdByName(pt->stn_id, compEnd, &pt->dev_id))
		return SD_FAILURE;
	return SD_SUCCESS;
}

ST_RET FindH9000PointByDoiDesc3(ST_CHAR *desc, POINTER *pt)
{
	GULONG pt_num;
	GULONG idx;
	int	gz_flag;
	ST_CHAR longname[ENTRY_LONGNAME_SIZE];
	ST_CHAR str[256];

	if(!strcmp(&desc[strlen(desc)-4], "¹ÊÕÏ"))
	{
		gz_flag= 1;
		strcpy(str, desc);
		str[strlen(desc)-4]= 0;
	}
	else
		gz_flag= 0;

	if(-1==GetEntryNumById(pt->stn_id, pt->dev_id, pt->data_type, &pt_num))
		return SD_FAILURE;

	for(idx=0; idx<pt_num; idx++)
	{
		pt->pt_id= (USHORT)idx;
		if(-1==GetEntryLongnameById(pt, longname))
			return SD_FAILURE;
		
		if(!strcmp(longname, desc))
		{
			pt->pt_id= idx;
			return SD_SUCCESS;
		}
		
		if(gz_flag)
		{
			if(!strcmp(longname, str))
			{
				pt->pt_id= idx;
				return SD_SUCCESS;
			}
		}
	}
	return SD_FAILURE;
}

ST_RET FindH9000PointByDoiDesc2(ST_CHAR *h9000_name, ST_CHAR *fcName, ST_CHAR *desc, POINTER *pt)
{
	ST_RET ret;
	
	ret= GetStnDevIdbyName(h9000_name, pt);
	if(ret)
		return ret;

	if(!strcmp(fcName, "ST"))
	{
		pt->data_type= SOE_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;

		pt->data_type= POL_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;

		pt->data_type= SYSMSG_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;
	}
	else if(!strcmp(fcName, "CO"))
	{
		pt->data_type= SOE_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;

		pt->data_type= POL_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;

		pt->data_type= SYSMSG_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;
	}
	else if(!strcmp(fcName, "DC") || !strcmp(fcName, "SV") || !strcmp(fcName, "EX") || !strcmp(fcName, "CF"))
	{
		pt->data_type= ANA_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;

		pt->data_type= SOE_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;

		pt->data_type= POL_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;

		pt->data_type= SYSMSG_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;
	}
	else if(!strcmp(fcName, "MX"))
	{
		pt->data_type= ANA_TYPE;
		if(!FindH9000PointByDoiDesc3(desc, pt))
			return SD_SUCCESS;
	}
	return SD_FAILURE;
}
ST_RET FindH9000PointByDoiDesc(ST_CHAR *ld_name, ST_CHAR *leaf_name, ST_CHAR *desc, ST_CHAR *h9000_name)
{
	ST_RET ret;
	ST_CHAR fcName[MAX_IDENT_LEN+1];
	ST_CHAR dev_name[MAX_IDENT_LEN+1];
	POINTER	pt;

	ret= GetFcName(leaf_name, fcName);

	if (ret == SD_SUCCESS)
		ret= GetLdH9000Name(ld_name, dev_name);

	if (ret == SD_SUCCESS)
		ret= FindH9000PointByDoiDesc2(dev_name, fcName, desc, &pt);

	if (ret == SD_SUCCESS)
		GetPtNameById(&pt, h9000_name, NULL);

	return ret;
}