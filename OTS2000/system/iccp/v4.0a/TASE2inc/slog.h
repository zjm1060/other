/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1993 - 1997, All Rights Reserved.		        */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : slog.h							*/
/* PRODUCT(S)  : SLOG							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 03/11/02  JRB     19    SLOG_PAUSE, SLOG_LF, SLOG_CLRSCR do nothing.	*/
/* 04/26/01  DSF     18    Define SOCKET if not yet defined		*/
/* 04/16/01  DSF     17    Include windows.h instead of including	*/
/*			   winsock.h directly				*/
/* 03/23/01  MDE     16    Added _slogStr, SLOGALWAYSS 			*/
/* 11/27/00  JRB     15    Added read_log_cfg_file, logcfgx protos.	*/
/* 08/22/00  KCR     14    Added FIL_CTRL_NO_LOG_HDR			*/
/* 03/09/00  MDE     13    Added SD_CONST modifiers to LOGMEM_ITEM	*/
/* 09/13/99  MDE     12    Added SD_CONST modifiers			*/
/* 11/11/98  DSF     11    Minor changes to _slog_dyn_log_fun		*/
/* 10/16/98  DSF     10    Spelling					*/
/* 10/08/98  MDE     09    Migrated to updated SLOG interface		*/
/* 06/15/98  NAV     08    Conditionally include winsock.h or winsock2.h*/
/* 06/03/98  MDE     07    Replaced 'winsock.h' with 'windows.h'	*/
/* 12/12/97  MDE     06    Added DEBUG_SISCO versions of SLOGALWAYSx	*/
/* 11/05/97  DSF     05    Added SYSTIME_EN				*/
/* 09/12/97  DSF     04    Expose slogSetTimeText ()			*/
/* 06/11/97  MDE     03    Include time.h for all			*/
/* 05/27/97  DSF     02    Added IPC logging capability			*/
/* 05/27/97  DSF     01    Include time.h for sun			*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifndef SLOG_INCLUDED
#define SLOG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>	/* for time_t */

#if !defined (MMS_LITE)
#if defined (_WIN32)
#define LOG_IPC_SUPPORT
#include <windows.h>
#ifndef SOCKET
typedef unsigned int SOCKET;
#endif
#endif

#endif

/************************************************************************/
/* Memory logging string buffer size limit				*/
/* NOTE : Do not change this lightly! QMEM.C should be looked at	*/
/************************************************************************/

#define SLOG_MEM_BUF_SIZE 125

/* Memory logging item control						*/
/* A table of these is established at initialization time		*/

#define LMF_USED	0x0001
#define LMF_HEADER	0x0002
#define LMF_HEX		0x0004


typedef struct logmem_item
  {
  ST_UINT flags;		/* see LMF_xxx flags			*/
  ST_INT logType;		/* log type storage			*/
  SD_CONST ST_CHAR *logTypeStr;	/* log type string			*/
  ST_INT lineNum;		/* source file line number		*/
  SD_CONST ST_CHAR *sourceFile;	/* source file name, NULL for none	*/
  union
    {			/* timestamp					*/
    ST_FLOAT ms;	/* for differential time			*/
    time_t t;		/* for time/date				*/
    }u;
  ST_CHAR *string;		/* text						*/
  } LOGMEM_ITEM;


/************************************************************************/
/* File Logging State flags						*/
/************************************************************************/

#define FIL_STATE_OPEN 	       0x0001
#define FIL_STATE_NEED_WIPE    0x0002

/* File Logging Control flags						*/

#define FIL_CTRL_WIPE_EN       0x0001
#define FIL_CTRL_WRAP_EN       0x0002
#define FIL_CTRL_MSG_HDR_EN    0x0004	
#define FIL_CTRL_NO_LOG_HDR    0x0040	

/* File output mode flags						*/

#define FIL_CTRL_NO_APPEND     0x0008	
#define FIL_CTRL_HARD_FLUSH    0x0010	
#define FIL_CTRL_SETBUF_EN     0x0020	

typedef struct file_log_ctrl
  {
/* User sets these elements						*/
  ST_ULONG  maxSize;
  ST_CHAR  *fileName;		/* destination filename			*/
  ST_UINT ctrl;			/* see FIL_CTRL_xxx flags		*/

/* Internal use only							*/
  ST_UINT state;		/* see FIL_STATE_xxx flags		*/
  FILE  *fp;
  } FILE_LOG_CTRL;

/* Memory Logging State flags						*/

#define MEM_STATE_INIT        0x0001

/* Memory Logging Control flags						*/

#define MEM_CTRL_MSG_HDR_EN   0x0001
#define MEM_CTRL_AUTODUMP_EN  0x0002
#define MEM_CTRL_HEX_LOG      0x0004

typedef struct mem_log_ctrl
  {
/* User sets these elements						*/
  ST_INT maxItems;	 /* # items to allocate at powerup		*/
  ST_CHAR *dumpFileName; /* where memory dump goes			*/
  ST_UINT ctrl;	    	 /* see MEM_CTRL_xxx flags			*/

/* Internal use only							*/
  ST_UINT state;		/* see MEM_STATE_xxx flags			*/
  LOGMEM_ITEM *item; 	/* Item table					*/
  ST_INT nextPut; 	/* Current position (where last was put		*/
  } MEM_LOG_CTRL;


#ifdef LOG_IPC_SUPPORT
/* IPC Logging State flags						*/

#define IPC_STATE_INIT        0x0001

#define LOG_IP_ADDR	64
#define LOG_MAX_NAME	32

typedef struct ipc_log_ctrl
  {
/* User sets these elements						*/
#if defined (_WIN32)
  ST_INT loggerPort;
  ST_CHAR ipAddr[LOG_IP_ADDR + 1];
  ST_CHAR name[LOG_MAX_NAME + 1];
#endif  

/* Internal use only							*/
  ST_UINT state;		/* see IPC_STATE_xxx flags		*/
#if defined (_WIN32)
  SOCKET socket;
#endif  
  } IPC_LOG_CTRL;
#endif


/************************************************************************/
/* LOG CONTROL								*/
/* A structure of this type is used for each independent logging entity	*/
/************************************************************************/

/* Log control mask values						*/
#define LOG_MEM_EN	     0x0001L
#define LOG_FILE_EN	     0x0002L
#define LOG_TIME_EN	     0x0008L
#define LOG_TIMEDATE_EN	     0x0010L
#define LOG_DIFFTIME_EN	     0x0020L
#define LOG_TIME_INITIALIZED 0x0040L
#define LOG_IPC_EN	     0x0080L
#define LOG_SYSTIME_EN	     0x0100L

#define LOG_NO_HEADER_CR     0x0200L
#define LOG_FILENAME_SUPPRESS 0x0400L
#define LOG_LOGTYPE_SUPPRESS  0x0800L

#define SLOG_NORMAL 	 0
#define SLOG_CONT 	-1
/* Dynamic logging standard command definitions				*/
#define	SLOG_DYN_PAUSE  -10	/* OBSOLETE	*/
#define	SLOG_DYN_LF     -11	/* OBSOLETE	*/
#define	SLOG_DYN_CLRSCR -12	/* OBSOLETE	*/

typedef struct log_ctrl
  {
/* User sets these elements						*/
  ST_UINT32 logCtrl; 	/* Logging Control flags - see LOG_xxx defines	*/
  FILE_LOG_CTRL fc;	/* File logging control				*/
  MEM_LOG_CTRL  mc;	/* Memory logging control			*/
#ifdef LOG_IPC_SUPPORT
  IPC_LOG_CTRL  ipc;	/* IPC logging control				*/
#endif

/* Application specific information					*/
  ST_UINT32 logMask1;	/* These bit masked variables are used by the	*/
  ST_UINT32 logMask2;	/* user application to determine whether a item	*/
  ST_UINT32 logMask3;	/* is to be logged. 6 ST_INT32's == 192 bits	*/
  ST_UINT32 logMask4;
  ST_UINT32 logMask5;
  ST_UINT32 logMask6;
  } LOG_CTRL;


/* Max size of any single log statement (sprintf)			*/

#define MAX_LOG_SIZE 500

/* Set the following variable to a number larger than the default of    */
/* MAX_LOG_SIZE if you want to log more than 500 bytes in a single      */
/* message.                                                             */

extern ST_INT sl_max_msg_size;

/************************************************************************/
/* Main entry points into the SLOG library				*/
/************************************************************************/

ST_VOID slog (LOG_CTRL *lc, 
	      SD_CONST ST_INT logType, 
	      SD_CONST ST_CHAR *SD_CONST sourceFile, 
	      SD_CONST ST_INT lineNum, 
	      SD_CONST ST_CHAR *format, ...);

ST_VOID slogx (ST_UINT32 doit, LOG_CTRL *lc, 
	       SD_CONST  ST_INT logType, 
	       SD_CONST ST_CHAR *SD_CONST sourceFile, 
	       SD_CONST ST_INT lineNum, 
	       SD_CONST ST_CHAR *format, ...);

ST_VOID slogHex (LOG_CTRL *lc, 
		 SD_CONST ST_INT logType, 
		 SD_CONST ST_CHAR *SD_CONST fileName, 
		 SD_CONST ST_INT lineNum,
	  	 SD_CONST ST_INT numBytes, 
		 SD_CONST ST_VOID *hexData);

ST_VOID _slog (LOG_CTRL *lc, 
	       SD_CONST ST_CHAR *SD_CONST logTypeStr, 
	       SD_CONST ST_CHAR *SD_CONST sourceFile, 
	       SD_CONST ST_INT	lineNum, 
	       SD_CONST ST_CHAR *format, ...);
ST_VOID _slogc (LOG_CTRL *lc, SD_CONST ST_CHAR *format, ...);

ST_VOID _slogx (ST_UINT32 doit, LOG_CTRL *lc, 
		SD_CONST ST_CHAR *SD_CONST logTypeStr, 
		SD_CONST ST_CHAR *SD_CONST sourceFile, 
		SD_CONST ST_INT	lineNum, 
		SD_CONST ST_CHAR *format, ...);

ST_VOID _slogHex (LOG_CTRL *lc, 
		  ST_INT numBytes, 
		  SD_CONST ST_VOID *hexData);
ST_VOID _slogStr (LOG_CTRL *lc, ST_CHAR *strData);

/************************************************************************/
/* Main entry points into the SLOG library 				*/
/************************************************************************/

/* File Logging Utility Functions					*/

ST_VOID slogCloseFile (LOG_CTRL *lc);
ST_VOID slogCloneFile (LOG_CTRL *lc, SD_CONST ST_CHAR *newfile);
ST_VOID slogCloneFileEx (LOG_CTRL *lc, SD_CONST ST_CHAR *newFile);
ST_VOID slogDeleteFile (LOG_CTRL *lc);
ST_VOID slogCallStack (LOG_CTRL *lc, SD_CONST ST_CHAR *txt);

/* Memory Logging Utility Functions					*/

ST_VOID slogDumpMem (LOG_CTRL *lc);
ST_VOID slogResetMem (LOG_CTRL *lc);
ST_CHAR *slogGetMemMsg (LOG_CTRL *lc, SD_CONST ST_INT msgNum);
ST_INT slogGetMemCount (LOG_CTRL *lc);

/* Internal functions							*/

ST_VOID slogFile (LOG_CTRL *lc, SD_CONST ST_INT logType, 
		SD_CONST ST_CHAR *SD_CONST logTypeStr, 
		SD_CONST ST_CHAR *SD_CONST sourceFile, 
		SD_CONST ST_INT lineNum, 
		SD_CONST ST_INT bufLen, SD_CONST ST_CHAR *buf);

ST_VOID slogMem (LOG_CTRL *lc, 
		SD_CONST ST_INT logType, 
		SD_CONST ST_CHAR *SD_CONST logTypeStr, 
		SD_CONST ST_CHAR *SD_CONST sourceFile, 
		SD_CONST ST_INT lineNum, 
		ST_INT bufLen, SD_CONST ST_CHAR *buf);

#ifdef LOG_IPC_SUPPORT
ST_VOID slogIpc (LOG_CTRL *lc, ST_INT logType, ST_CHAR *logTypeStr, 
		 ST_CHAR *sourceFile, ST_INT lineNum, ST_INT bufLen, 
                 ST_CHAR *buf);
#endif

ST_VOID slogMemInit (LOG_CTRL *lc);
ST_RET read_log_cfg_file (ST_CHAR *cfg_filename);
ST_RET logcfgx (ST_CHAR *xml_filename);

/************************************************************************/
/* Global variables for the SLOG library.				*/
/************************************************************************/
extern LOG_CTRL *sLogCtrl;

/************************************************************************/
/* These elements are used for creating time strings			*/
/************************************************************************/

#define TIME_BUF_LEN 30
extern ST_CHAR slogTimeText[TIME_BUF_LEN];  
ST_VOID slogSetTimeText (LOG_CTRL *lc);

/************************************************************************/
/* Global variables used to get and keep time.				*/
/************************************************************************/

extern time_t slogTime;
extern ST_FLOAT slogMs;


#define SLOG_MAX_FNAME    20

ST_VOID slogTrimFileName (ST_CHAR *dest, SD_CONST ST_CHAR *fullName);

/************************************************************************/
/* dynamic loging function						*/
/************************************************************************/

extern ST_VOID (*slog_dyn_log_fun) (LOG_CTRL *lc, 
			SD_CONST ST_INT logType, 
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);
extern ST_VOID (*_slog_dyn_log_fun) (LOG_CTRL *lc, 
			SD_CONST ST_CHAR *timeStr, 
			SD_CONST ST_INT logType, 
			SD_CONST ST_CHAR *SD_CONST logTypeStr,
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);

/************************************************************************/
/* This function pointer is invoked during slow SLOG operations, such	*/
/* as 'clone' and 'find oldest message'					*/

extern ST_VOID (*slog_service_fun) (ST_VOID);

/************************************************************************/
/* Remote logging function						*/
/************************************************************************/

extern ST_VOID (*slog_remote_fun) (ST_UINT32 slog_remote_flags,
			LOG_CTRL *lc, 
			SD_CONST ST_INT logType, 
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);

extern ST_VOID (*_slog_remote_fun) (ST_UINT32 slog_remote_flags,
			LOG_CTRL *lc, 
			SD_CONST ST_INT logType, 
                        SD_CONST ST_CHAR *SD_CONST logTypeStr, 
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);

/* Parameter for use with remote logging function pointer		*/
/* SLOG will pass this global variable to the slog_remote_fun function	*/
extern ST_UINT32 slogRemoteFlags;

/************************************************************************/
/* Standard dynamic logging control definitions				*/
/************************************************************************/
/* These macros are OBSOLETE. They now do nothing. Previously they	*/
/* passed logType values of SLOG_DYN_PAUSE, SLOG_DYN_LF, SLOG_DYN_CLRSCR*/
/* respectively to slog.						*/

#define SLOG_PAUSE(lc,msg)
#define SLOG_LF(lc)
#define SLOG_CLRSCR(lc)

/************************************************************************/
/* Base macro types, useful for typical user logging, to reduce user	*/
/* macro verbosity							*/
/************************************************************************/

#define SLOGH1(lc,mask,id,x,y) {\
			   if (lc->logMask1 & mask)\
			     slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}


#define SLOGH2(lc,mask,id,x,y) {\
                           if (lc->logMask2 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH3(lc,mask,id,x,y) {\
                           if (lc->logMask3 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH4(lc,mask,id,x,y) {\
                           if (lc->logMask4 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH5(lc,mask,id,x,y) {\
                           if (lc->logMask5 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH6(lc,mask,id,x,y) {\
                           if (lc->logMask6 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}



/* ******************* LOG MASK 1 MACROS *************************	*/

#define SLOG1_0(lc,mask,id,a) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG1_1(lc,mask,id,a,b) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG1_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG1_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG1_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG1_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG1_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG1_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG1_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 2 MACROS *************************	*/
#define SLOG2_0(lc,mask,id,a) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG2_1(lc,mask,id,a,b) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG2_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG2_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG2_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG2_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG2_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG2_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG2_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 3 MACROS *************************	*/
#define SLOG3_0(lc,mask,id,a) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG3_1(lc,mask,id,a,b) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG3_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG3_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG3_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG3_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG3_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG3_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG3_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 4 MACROS ************************* 	*/
#define SLOG4_0(lc,mask,id,a) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG4_1(lc,mask,id,a,b) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG4_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG4_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG4_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG4_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG4_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG4_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG4_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }




/* ******************* LOG MASK 5 MACROS *************************	*/
#define SLOG5_0(lc,mask,id,a) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG5_1(lc,mask,id,a,b) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG5_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG5_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG5_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG5_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG5_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG5_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG5_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 6 MACROS ************************* 	*/
#define SLOG6_0(lc,mask,id,a) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG6_1(lc,mask,id,a,b) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG6_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG6_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG6_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG6_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG6_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG6_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG6_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/************************************************************************/
/************************************************************************/
#if defined(DEBUG_SISCO)
/************************************************************************/

extern ST_CHAR *_slogAlwaysLogTypeStr;

#define SLOGALWAYS0(a) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a);\
    }
#define SLOGALWAYS1(a,b) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b);\
    }
#define SLOGALWAYS2(a,b,c) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c);\
    }
#define SLOGALWAYS3(a,b,c,d) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d);\
    }
#define SLOGALWAYS4(a,b,c,d,e) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e);\
    }
#define SLOGALWAYS5(a,b,c,d,e,f) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f);\
    }
#define SLOGALWAYS6(a,b,c,d,e,f,g) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g);\
    }
#define SLOGALWAYS7(a,b,c,d,e,f,g,h) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
    }
#define SLOGALWAYS8(a,b,c,d,e,f,g,h,i) {\
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
    }

#define SLOGCALWAYS0(a) {\
    _slogc (sLogCtrl,a);\
    }
#define SLOGCALWAYS1(a,b) {\
    _slogc (sLogCtrl,a,b);\
    }
#define SLOGCALWAYS2(a,b,c) {\
    _slogc (sLogCtrl,a,b,c);\
    }
#define SLOGCALWAYS3(a,b,c,d) {\
    _slogc (sLogCtrl,a,b,c,d);\
    }
#define SLOGCALWAYS4(a,b,c,d,e) {\
    _slogc (sLogCtrl,a,b,c,d,e);\
    }
#define SLOGCALWAYS5(a,b,c,d,e,f) {\
    _slogc (sLogCtrl,a,b,c,d,e,f);\
    }
#define SLOGCALWAYS6(a,b,c,d,e,f,g) {\
    _slogc (sLogCtrl,a,b,c,d,e,f,g);\
    }
#define SLOGCALWAYS7(a,b,c,d,e,f,g,h) {\
    _slogc (sLogCtrl,a,b,c,d,e,f,g,h);\
    }
#define SLOGCALWAYS8(a,b,c,d,e,f,g,h,i) {\
    _slogc (sLogCtrl,a,b,c,d,e,f,g,h,i);\
    }

#define SLOGALWAYSH(numBytes,dataPtr) {\
    _slogHex (sLogCtrl, numBytes, dataPtr);\
    }

#define SLOGALWAYSS(dataPtr) {\
    _slogStr (sLogCtrl, dataPtr);\
    }

/************************************************************************/
#else	/* #if defined(DEBUG_SISCO) */
/************************************************************************/

#define SLOGALWAYS0(a)
#define SLOGALWAYS1(a,b)
#define SLOGALWAYS2(a,b,c)
#define SLOGALWAYS3(a,b,c,d)
#define SLOGALWAYS4(a,b,c,d,e)
#define SLOGALWAYS5(a,b,c,d,e,f)
#define SLOGALWAYS6(a,b,c,d,e,f,g)
#define SLOGALWAYS7(a,b,c,d,e,f,g,h)
#define SLOGALWAYS8(a,b,c,d,e,f,g,h,i)
#define SLOGCALWAYS0(a)
#define SLOGCALWAYS1(a,b)
#define SLOGCALWAYS2(a,b,c)
#define SLOGCALWAYS3(a,b,c,d)
#define SLOGCALWAYS4(a,b,c,d,e)
#define SLOGCALWAYS5(a,b,c,d,e,f)
#define SLOGCALWAYS6(a,b,c,d,e,f,g)
#define SLOGCALWAYS7(a,b,c,d,e,f,g,h)
#define SLOGCALWAYS8(a,b,c,d,e,f,g,h,i)
#define SLOGALWAYSH(numBytes,dataPtr)
#define SLOGALWAYSS(dataPtr)

#endif
/************************************************************************/



#ifdef __cplusplus
}
#endif

#endif  /* end of 'already included' 	*/

