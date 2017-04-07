#ifndef     _UTIL_DEF_H
#define     _UTIL_DEF_H

#include "./os_type_def.h"
#include "./dms_structure_def.h"


#define	MAX_MODULE_NAME_SIZE	36
enum	ERROR_LOG_MODE  {
							ERROR_LOG_NO,
							ERROR_LOG_OUTPUT, 
							ERROR_LOG_FILE,
							ERROR_LOG_SHM,
							ERROR_LOG_ALL
						};


#ifdef __cplusplus
extern "C"{
#endif
	
float	AnlgDatatoFloat ( ANLG_DATA *anlg_data );
void    FloattoAnlgData( float value, ANLG_DATA *anlg_data );

void	CrossCopy( void *strDest, void *strSrc, int nNum);



#ifndef	SUN_OS
#define Swap2Byte(p)			; 
#define Swap4Byte(p)			; 
#define SwapImpDataByte(p)		;
#define SwapLmtParaByte(p)		;
#define SwapCharaDataByte(p)	;
#define SwapImpEngParaByte(p)	;
#define SwapAnaEngParaByte(p)	;
#define SwapLmtParaByte(p)		;
#define SwapStcDataByte(p)		;
#define SwapDateTimeByte(p)		;
#define SwapCurveDataByte(p)	;
#else
void Swap2Byte(void *pShort);
void Swap4Byte(void *pInt);
void SwapImpDataByte( IMP_DATA *pImp);
void SwapImpEngParaByte( IMP_ENG_PARA *eng);
void SwapAnaEngParaByte( ANA_ENG_PARA *eng);
void SwapLmtParaByte( LMT_PARA *lmt);
void SwapCharaDataByte( CHARA_DATA *chara);
void SwapCurveDataByte( CURVE_DATA *data);
void SwapStcDataByte( STC_DATA *stc);
void SwapDateTimeByte( DATE_TIME *date_time);
#endif

BOOL	IsIndTypePoint(POINTER *pt);
BOOL	IsAnaTypePoint(POINTER *pt);

void	SetModuleName( char *module );
int		SetErrLogMode(int argc, char *argv[]);
void	Err_log( char *info_buf );
void	SendTraceMsg(int nErr, int nFilter, const char* format, ...);

void	GetSysTime( DATE_TIME *curr_time);
void 	GetSysTimeStr( char *timeStr);
void	PrintfWithCurrTime(char *str);

#ifdef __cplusplus
}
#endif



#endif  /*_UTIL_DEF_H*/


