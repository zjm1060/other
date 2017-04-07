/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2006.4.8 
  SYN by hcl 2007.12.12  

---------------------------------------------------------------------*/



#ifndef   _GTK_FAULTALARM_TAB_H 
#define   _GTK_FAULTALARM_TAB_H  

PAGE_INFO		unreset_page[MAX_CRT_NUMBER];

extern FAULT_RECORD_TABLE *shm_fault_addr;

gint 	unresetalm_timeout_id[MAX_CRT_NUMBER] ={0,0,0} ;


static	char *UnresetAlm_faulttype[]={OIX_CSTR_ALM_ALL_TYPEID,OIX_CSTR_ALL_FAULT,OIX_CSTR_ALL_FAIL};
static	char *UnresetAlm_typeid[]={OIX_CSTR_ALL_TYPE,OIX_CSTR_ANA_ALARM,OIX_CSTR_SOE_ALARM};
#define  	UNRESETALM_DATATYPE_NUM 3
#if defined(SUN_OS)
#define	UnresetPrintFile   "/home/ems/h9000/tmp/UnresetPrintFile.tmp"
#endif

/*add by hcl for print*/
#if defined(WINDOWS_OS)
#define	UnresetPrintFile   "\\home\\ems\\h9000\\tmp\\UnresetPrintFile.tmp" 
#endif 


typedef struct
{
	 UINT 	index_pt[MAX_FAULT_RECORD_NUM] ;
	 USHORT	count   ;
	 USHORT header  ;
         USHORT buff_size;
} UNRESET_INDEX;   



#endif

