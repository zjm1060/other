
/* 
 * File:   _STATION_DEF.h
 * Author: chi.hailong;
 *
 * Created on 2006年8月30日, 上午10:48
 *  SYN by hcl 2007.12.12
 */

#ifndef _STATION_DEF_H
#define	_STATION_DEF_H

#ifdef	__cplusplus
extern "C" {
#endif


#if defined(WINDOWS_OS)
#include "../../../dms/v4.0a/inc/dms_structure_def.h"
#endif

#ifdef	SUN_OS
#include "/DBINC/dms_structure_def.h"
#endif


typedef struct
{
	char	CName[GROUP_COMMENT_SIZE];
	UINT	Status;
	UCHAR	Id;
	BOOL	isFilter[MAX_CRT_NUMBER];
}GROUPDEF;

typedef struct	 		/* group structure */
{
	char		Name[GROUP_NAME_SIZE];	
	char		LongName[GROUP_LONGNAME_SIZE];
	char		CName[GROUP_COMMENT_SIZE];
	UCHAR		Id;
	GULONG		rec_num;			/* Number of records in the group	*/
	UINT		group_num;
	GROUPDEF	group[GROUP_ENTRY_NUMBER];
}DEVDEF;

typedef struct
{	char	Name[STATION_NAME_SIZE];
	char	LongName[STATION_LONGNAME_SIZE];
	char	CName[STATION_COMMENT_SIZE];
	UCHAR   Id;
	UCHAR	dev_num;
	DEVDEF	dev[GROUP_ENTRY_NUMBER];
}STNDEF;
STNDEF	StnDef[STATION_ENTRY_NUMBER];

#ifdef	__cplusplus
}
#endif

#endif	/*  DEF_H */