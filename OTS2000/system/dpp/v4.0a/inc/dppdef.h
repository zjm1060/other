
#ifndef dppdefh

#define dppdefh

/* #define DEBUG */

#define INDPRO
#define ANAPRO

#ifdef WINDOWS_OS
#include <signal.h>
#include <windows.h>       
#include <winbase.h>
#include <stdlib.h>
#include <stdio.h>         
#include <time.h>
#include <string.h>        
#include <memory.h>
#include <process.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <errno.h>
#endif

#if defined(SUN) && defined(_AIX)
#include	<sys/wait.h>
#elif defined(SUN)
#include	<wait.h>
#endif

#ifdef SUN_OS
#include 	<sys/types.h>
#include 	<sys/socket.h>
#include 	<sys/errno.h>
#include 	<netinet/in.h>
#include 	<netdb.h>
#include 	<string.h>                      
#include 	<stdio.h>
#include 	<signal.h>
#include	<stdlib.h>
#include 	<fcntl.h>
#include	<unistd.h>
#include 	<sys/shm.h>
#include 	<sys/sem.h>
#include	<time.h>
#include	<errno.h>			
#include	<arpa/inet.h>		
#include 	<pthread.h>
#include 	<math.h>
#include	<sys/stat.h>
#include	<errno.h>

#define 	_access 	access
#define 	_open 		open
#define 	_close 		close
#define		_fstat		fstat
#define		_O_RDONLY	O_RDONLY
#define		_stat		stat
#endif

#define ANA_ALM_MAX	177
#define IND_ALM_MAX	354
#define OBJ_ALM_MAX	177

#define	ANA_ALL_MAX	236
#define	IND_ALL_MAX	708
#define	ALM_BUF_SIZE	200

#define PACKETLEN_MAX 	1432

#define SOE_PACKET_MAX	70
#define OBJ_PACKET_MAX	50

#define ANA_ALLSEND_PERIOD	240 	/* 4min */
#define IMP_ALLSEND_PERIOD	240 	/* 4min */
#define DII_ALLSEND_PERIOD	120 	/* 2min */
#define IND_ALLSEND_PERIOD	120 	/* 2min */
#define DATA_DOWNSEND_PERIOD	1 	/* 1s */


#define FACTOR1		0.1
#define FACTOR2		0.2
#define FACTOR3		0.3
#define FACTOR4		0.4

#define SAMPLE_NUM	5


#define ANA_HI_ALARM_DEFAULT	9999
#define ANA_HI_WARN_DEFAULT	9999
#define ANA_LO_ALARM_DEFAULT	-9999
#define ANA_LO_WARN_DEFAULT	-9999

#define RECORD_FILE_SIZE  1024000 /*debug file size<=1024k bytes*/

/*********** data structure ********/
typedef struct 
{
	unsigned short      point_id;
	ANLG_STATUS	    status;
        GLONG		    data;       /* int is 4 bytes,short is 2 bytes */
}AISTRUCT;

typedef struct 
{
	unsigned short      point_id;
	IMP_STATUS	    status;
        GLONG		    data;       /* int is 4 bytes,short is 2 bytes */
}PISTRUCT;

typedef struct 
{
 unsigned short       point_id;    
 IND_DATA      	      status;     
}DI_STRUCT;

typedef struct
{
 	unsigned short	point_id;    
 	FB_STATUS1	fb_status1;
	FB_STATUS2	fb_status2;
	USHORT		param;
}OBJ_STRUCT;


/*********** data structure in realtime database ********/
typedef struct
{
	IOST			iost;	
	IOATTR      		ioattr;	
	ANLG_STATUS		status;
	GLONG			data;
	float			fValue;
	float			d_band;
	float			hh_lim;
	float			h_lim;
	float			hr_lmt;
	float			ll_lim;
	float			l_lim;
	float			lr_lmt;
	float			hi_trlmt;
	float			hr_trlmt;
	float			eng_cvt;
	float			hi_range;
	float			lo_range;
	float			hi_code;
	float			lo_code;
	float			zero;
	float			compensate;
	UCHAR			hh_flag;
	UCHAR			h_flag;
	UCHAR			l_flag;
	UCHAR			ll_flag;
}ANA_STRUCT;

typedef struct 
{
	IOST		iost;
	IOATTR      	ioattr;
	IND_DATA	status;
	
}IND_STRUCT;

typedef struct 
{
	IOST				iost;
	IOATTR				ioattr;
	IMP_STATUS			status;
	unsigned long			imp_accm;
	float				d_accum;
	float				day_peak;
	float				month_peak;
	float				year_peak;
	float				day_vally;
	float				month_vally;
	float				year_vally;
	float				day_ave;
	float				month_ave;
	float				year_ave;
	float				day_sum;
	float				month_sum;
	float				year_sum;
	float				day_ppeak;
	float				month_ppeak;
	float				year_ppeak;
}IMP_STRUCT;

typedef struct
{
	IOST		iost;	
	IOATTR      	ioattr;	
	FB_STATUS1	fb_status1;
	FB_STATUS2	fb_status2;
	USHORT		param;
}DEVOBJ_STRUCT;


/*********** all-send data structure  ********/
typedef struct 
{
	ANLG_STATUS		status;
        GLONG                    data;
}AIALLSTRUCT;

typedef struct 
{
	IMP_STATUS		status;
        GLONG                    data;
}PIALLSTRUCT;

typedef struct 
{
	FB_STATUS1	fb_status1;
	FB_STATUS2	fb_status2;
	USHORT		param;
}OBJALLSTRUCT;

/*********** alarm data structure  ********/
typedef struct 
{
   unsigned char		type_id;
   unsigned char		stn_id;
   unsigned char		dev_id;
   unsigned char		data_type;
   unsigned short		number;
   AISTRUCT	        	ai_object[ANA_ALM_MAX];
   unsigned char		host_id;
   unsigned char        second;
   unsigned char        minute;
   unsigned char        hour;
   unsigned char        day;
   unsigned char        month;
   unsigned short       year;
}AIALMBUFSTRUCT;

typedef struct 
{
   unsigned char		type_id;
   unsigned char		stn_id;
   unsigned char		dev_id;
   unsigned char		data_type;
   unsigned short		number;
   PISTRUCT	        	ai_object[ANA_ALM_MAX];
   unsigned char		host_id;
   unsigned char        second;
   unsigned char        minute;
   unsigned char        hour;
   unsigned char        day;
   unsigned char        month;
   unsigned short       year;
}PIALMBUFSTRUCT;

typedef struct 
{
   unsigned char		type_id;
   unsigned char		stn_id;
   unsigned char		dev_id;
   unsigned char		data_type;
   unsigned short		number;
   DI_STRUCT	        	di_object[IND_ALM_MAX];
   unsigned short		ms;
   unsigned char		host_id;
   unsigned char        second;
   unsigned char        minute;
   unsigned char        hour;
   unsigned char        day;
   unsigned char        month;
   unsigned short       year;
}DIALMBUFSTRUCT;

typedef struct 
{
   unsigned char		type_id;
   unsigned char		stn_id;
   unsigned char		dev_id;
   unsigned char		data_type;
   unsigned short		number;
   OBJ_STRUCT	        	obj_object[OBJ_ALM_MAX];
   unsigned short		ms;
   unsigned char		host_id;
   unsigned char        second;
   unsigned char        minute;
   unsigned char        hour;
   unsigned char        day;
   unsigned char        month;
   unsigned short       year;
}OBJBUFSTRUCT;

/*********** other data structure  ********/
typedef struct 
{	
	time_t			ai_tm[SAMPLE_NUM];
	unsigned short  ms[SAMPLE_NUM];
	
        float		value[SAMPLE_NUM];
        float		t_value;
        float		t_value_old;
        unsigned short  t_flag;
}TREND_STRUCT;


typedef struct
{	
//	POINTER		pt;
	float		scale; //added by wzg 20110409			
	CHAR		l_name[POINT_NAME_SIZE];	/* Entry's name	*/
	CHAR		c_name[POINT_LONGNAME_SIZE];	/* Entry's chinese longname */
}DOWN_DATA;

/*modified by wzg 20110409
下传模拟量分成两部分：37个归一化模拟量 37个标度化模拟量 SEND_ANA_SIZE=74*/
typedef struct
{	
	unsigned short		anum, dnum, anum_scale;	
	//DOWN_DATA 		ana[SEND_ANA_SIZE];
	DOWN_DATA 		ana[SEND_ANA_SIZE/2];
	DOWN_DATA 		ana_scale[SEND_ANA_SIZE/2];
	DOWN_DATA		ind[SEND_POL_SIZE];
	time_t			otime,ntime;
}SENDATA;

/*********** dpp data structure  ********/

typedef struct
{
		unsigned short stnid;
		unsigned short devid;
	
        AIALMBUFSTRUCT	 aibuf[ALM_BUF_SIZE],aobuf[ALM_BUF_SIZE];
		AIALMBUFSTRUCT   aialm[ALM_BUF_SIZE],aoalm[ALM_BUF_SIZE];
		PIALMBUFSTRUCT	 pibuf[ALM_BUF_SIZE],pialm[ALM_BUF_SIZE];
		
		OBJBUFSTRUCT	 objbuf[ALM_BUF_SIZE];

		DIALMBUFSTRUCT   soebuf[ALM_BUF_SIZE],dinbuf[ALM_BUF_SIZE],diibuf[ALM_BUF_SIZE],msgbuf[ALM_BUF_SIZE],doutbuf[ALM_BUF_SIZE];
		
		DI_STRUCT	 dii_object[DIN_SIZE];
		unsigned short   dii_num;
		
		ANA_STRUCT	ana_db[AIALL_SIZE];
		IND_STRUCT	ind_db[DIN_SIZE];
		IMP_STRUCT	imp_db[PI_SIZE];
		DEVOBJ_STRUCT	obj_db[OBJ_SIZE];
		
		unsigned short          dii_new[DII_SIZE/16+1]; 		
		unsigned short          dii_fault[DII_SIZE/16+1];
		unsigned short          dii_force[DII_SIZE/16+1];
		unsigned short			dii_old[DII_SIZE/16+1];
		unsigned short          dii_fault_old[DII_SIZE/16+1];
		unsigned short          dii_force_old[DII_SIZE/16+1];
		
		unsigned short          pol_new[DIN_SIZE/16+1]; 		
		unsigned short          pol_fault[DIN_SIZE/16+1];
		unsigned short          pol_force[DIN_SIZE/16+1];
		unsigned short          pol_old[DIN_SIZE/16+1];
		unsigned short          pol_fault_old[DIN_SIZE/16+1];
		unsigned short          pol_force_old[DIN_SIZE/16+1];

		unsigned short          dout_new[DOUT_SIZE/16+1]; 		
		unsigned short          dout_fault[DOUT_SIZE/16+1];
		unsigned short          dout_force[DOUT_SIZE/16+1];
		unsigned short          dout_old[DOUT_SIZE/16+1];
		unsigned short          dout_fault_old[DOUT_SIZE/16+1];
		unsigned short          dout_force_old[DOUT_SIZE/16+1];
		
		unsigned short          msg_new[MSG_SIZE/16+1]; 
		unsigned short          msg_fault[MSG_SIZE/16+1];
		unsigned short          msg_force[MSG_SIZE/16+1];
		unsigned short          msg_old[MSG_SIZE/16+1];
		unsigned short          msg_fault_old[MSG_SIZE/16+1];
		unsigned short          msg_force_old[MSG_SIZE/16+1];

		unsigned short			trendAlm_old[AIALL_SIZE];
		unsigned short      	trend_id[AIALL_SIZE];
		TREND_STRUCT			trend[AIALL_SIZE];
		float                   value0[AIALL_SIZE];
		float                   value1[AIALL_SIZE];
		float                   value2[AIALL_SIZE];
		float                   value3[AIALL_SIZE];
		float                   value4[AIALL_SIZE];
		time_t			tm0[AIALL_SIZE];
		time_t			tm1[AIALL_SIZE];
		time_t			tm2[AIALL_SIZE];
		time_t			tm3[AIALL_SIZE];
		time_t			tm4[AIALL_SIZE];
		
		unsigned short		ms0[AIALL_SIZE];
		unsigned short		ms1[AIALL_SIZE];
		unsigned short		ms2[AIALL_SIZE];
		unsigned short		ms3[AIALL_SIZE];
		unsigned short		ms4[AIALL_SIZE];
		
		unsigned short	       	ai_new[AIALL_SIZE];
		float                   ai_value[AIALL_SIZE];      
		float                   ai_value_old[AIALL_SIZE];			
		unsigned short          ai_fault[AIALL_SIZE/16+1];
		unsigned short          ai_force[AIALL_SIZE/16+1];
		unsigned short          ai_fault_old[AIALL_SIZE/16+1];
		unsigned short          ai_force_old[AIALL_SIZE/16+1];
		
		unsigned short	       	ao_new[AO_SIZE];
		float                   ao_value[AO_SIZE];      
		float                   ao_value_old[AO_SIZE];
		unsigned short          ao_fault[AO_SIZE/16+1];
		unsigned short          ao_force[AO_SIZE/16+1];
		unsigned short          ao_fault_old[AO_SIZE/16+1];
		unsigned short          ao_force_old[AO_SIZE/16+1];
	
		unsigned long           pi_new[PI_SIZE];
		unsigned long           pi_old[PI_SIZE];
		unsigned short          pi_force[PI_SIZE/16+1];
		unsigned short          pi_fault[PI_SIZE/16+1];
 		unsigned short          pi_fault_old[PI_SIZE/16+1];
		unsigned short          pi_force_old[PI_SIZE/16+1];
        unsigned short		pi_status[PI_SIZE];
           	
        OBJALLSTRUCT		obj_new[OBJ_SIZE];
		
		unsigned short		ai_ptr1,ai_ptr2;  
		unsigned short		ao_ptr1,ao_ptr2;  
		unsigned short		pi_ptr1,pi_ptr2;
		
		unsigned short		aialm_ptr1,aialm_ptr2; 
		unsigned short		aoalm_ptr1,aoalm_ptr2;
		unsigned short		pialm_ptr1,pialm_ptr2;

		unsigned short		dii_ptr1,dii_ptr2;
						
		unsigned short		din_ptr1,din_ptr2;
		unsigned short	      	msg_ptr1,msg_ptr2;
		unsigned short		dout_ptr1,dout_ptr2;		
		unsigned short		soe_ptr1,soe_ptr2;
		unsigned short		obj_ptr1,obj_ptr2;		
		
 		unsigned short		soeptr,objptr;
 		unsigned short		polptr;
		
		IND_DATA		dii_all[DII_SIZE];
		IND_DATA          	din_all[DIN_SIZE]; 
		IND_DATA		msg_all[DOUT_SIZE]; /*modified ycx 070517*/
		IND_DATA		dout_all[DOUT_SIZE];
		
		AIALLSTRUCT	ai_all[AIALL_SIZE];
		AIALLSTRUCT	ao_all[AO_SIZE];
		PIALLSTRUCT	pi_all[PI_SIZE];
		AIALLSTRUCT	param_all[PARAM_SIZE];
		OBJALLSTRUCT	obj_all[OBJ_SIZE];

		float                   param_value[PARAM_SIZE];      
		unsigned short	       	param_new[PARAM_SIZE];


                unsigned short          flag_aialm[AIALL_SIZE];     /* Analog alarm flag */
                unsigned short          flag_overflow[AIALL_SIZE];
                
                char           isdataflag;
                char           isdataflag_old;
                
        unsigned char		soeallsend_flag;
		unsigned char		ai_allsend_flag;
        unsigned char       rtd_allsend_flag;
        unsigned char       ao_allsend_flag;
        unsigned char		com_ai_allsend_flag;
                
		unsigned char		first_scan; /* dpp first scan date of PLC  mark */

		DATE_TIME		ai_time,di_time;
		
        time_t		    rtd_ntime,ai_ntime,ao_ntime,imp_ntime,dii_ntime,pol_ntime,dout_ntime,msg_ntime,obj_ntime;
		time_t			rtd_otime,ai_otime,ao_otime,imp_otime,dii_otime,pol_otime,dout_otime,msg_otime,obj_otime;
		time_t		   	com_ai_ntime,com_imp_ntime,com_pol_ntime,com_ai_otime,com_imp_otime,com_pol_otime;
		
		unsigned short 	        ana_update_flag;		
		unsigned short 	        imp_update_flag;
		unsigned short	        rtd_update_flag;
		unsigned short	        param_update_flag;
		unsigned short 	        sysmsg_update_flag;
		unsigned short		obj_update_flag;
			
		unsigned short		fault_count;
		unsigned short 		plc_firstrun,plc_firstrun_old;
		
		//moved to here for each lcu by wzg 20120312
		SENDATA    	dpp_sendata;
		USHORT		dpp_sendata_flag;

 
}DPP_DATA;


	
//extern DPP_DATA 	dpp_data[MAX_PLCDATA_SEG_NUM];

extern unsigned short   dpp_data_id[MAX_STN_NUM][MAX_LCU_NUM];

//deleted by wzg 20120312
//extern SENDATA    dpp_sendata;
//extern USHORT		dpp_sendata_flag;

//cxs 20110607 start
USHORT	QUANTUM_SOE_PT_NUM_IN_MODULE;
USHORT	SIEMENS_SOE_PT_NUM_IN_MODULE;
USHORT	GE9030_SOE_PT_NUM_IN_MODULE;
USHORT	GE9070_SOE_PT_NUM_IN_MODULE;
USHORT	ABCONTROL_SOE_PT_NUM_IN_MODULE;
//cxs 20110607 end


#endif
