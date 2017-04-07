#ifndef _H9000_DEF_H
#define _H9000_DEF_H

#ifdef __cplusplus
extern "C" {
#endif


#include "../../../../dms/v4.0a/inc/dms_com.h"
#include "../uca_srvr/uca_obj.h"



typedef struct  
{
	struct 
	{
		ST_CHAR		name[128];
		ST_CHAR		h9000_name[128];
	} ld_array[MVL_NUM_DYN_DOMS];
	int ld_num;
} LD_DEFINE;

extern LD_DEFINE ld_define;

#define MAX_DOI_DESC_NUM 30000

enum DOI_RECORD_TYPE {SIMPLE_DOI_TYPE, DOI_SDI_TYPE, SIMPLE_SDI_TYPE};
typedef struct  
{
	struct 
	{
		int		type; //DOI_RECORD_TYPE
		ST_CHAR	ld_name[MAX_IDENT_LEN+1];
		ST_CHAR	ln_name[MAX_IDENT_LEN+1];
		ST_CHAR doi_name[MAX_IDENT_LEN+1];
		ST_CHAR sdi_name[MAX_IDENT_LEN+1];
		ST_CHAR desc[MAX_IDENT_LEN*2+1];
	} entry[MAX_DOI_DESC_NUM];
	int num;
} DOI_DESC_INFO;

extern DOI_DESC_INFO doi_desc_info;


#define MAX_MAN_MAP_NUM 10000
typedef struct  
{
	struct 
	{
		ST_CHAR	ld_name[MAX_IDENT_LEN+1];
		ST_CHAR	ln_name[MAX_IDENT_LEN+1];
		ST_CHAR doi_name[MAX_IDENT_LEN+1];
		ST_CHAR sdi_name[MAX_IDENT_LEN+1];
		ST_CHAR db_name[POINT_NAME_SIZE+1];
	} entry[MAX_MAN_MAP_NUM];
	int num;
} MAN_MAP_LIST;

extern MAN_MAP_LIST man_map_list;


ST_RET AddLdH9000Name(ST_CHAR *ld_name, ST_CHAR *h9000_name);
ST_RET GetLdH9000Name(ST_CHAR *ld_name, ST_CHAR *h9000_name);
ST_VOID GetCurrUTCtime(MMS_UTC_TIME *utc_time);
ST_RET DateTime2UTCtime(DATE_TIME *date_time, MMS_UTC_TIME *utc_time);
ST_RET GetDoiLeafDesc(ST_CHAR *ld_name, ST_CHAR *leaf_name, ST_CHAR *desc);
ST_RET FindH9000PointByDoiDesc(ST_CHAR *ld_name, ST_CHAR *leaf_name, ST_CHAR *desc, ST_CHAR *h9000_name);
ST_RET GetLeafPartialName(ST_CHAR *leaf_name, ST_CHAR *ln_name, ST_CHAR *doi_name, ST_CHAR *sdi_name);
ST_RET WriteManMapOutFile(ST_CHAR *ld_name, ST_CHAR *leaf, ST_CHAR *desc);
ST_RET man_datamap_cfg_read(char *in_filename);
ST_RET FindH9000PointFromManDataMapList(ST_CHAR *ld_name, ST_CHAR *leaf_name, ST_CHAR *pt_name);


#ifdef __cplusplus
}
#endif

#endif //_H9000_DEF_H