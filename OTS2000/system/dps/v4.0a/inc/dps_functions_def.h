#ifndef		_DPS_FUNCTIONS_DEF_H
#define		_DPS_FUNCTIONS_DEF_H

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_event_str.h" 
#include	"../../../oix/v4.0a/inc/gtk_color_def.h" 


#ifdef __cplusplus
extern "C"{
#endif 

int	GetPointFromRec(REC_ENTRY_UNION *rec, POINTER *pt);
long GetTimefromRec(REC_ENTRY_UNION *rec);
void SwapRecEntryByte(REC_ENTRY_UNION *rec_entry);
int	GetCmdSrcStr(USHORT	src_id, char *cmd_src);
int	GetPtLogicnameById(POINTER	*point, char *logicname);
long GetAbsTimeFromDateTime(DATE_TIME *date_time);



int	GetAlmStrings(REC_ENTRY_UNION *rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);

int	GetAnaAlmStrings(ANLG_REC *ana_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);
int	GetImpAlmStrings(ANLG_REC *ana_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);
int	GetIndAlmStrings(IND_REC *ind_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);
int	GetParamSetAlmStrings(PARAM_SET_REC *param_set_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);
int	GetLmsAlmStrings(LMS_REC *lms_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);
int	GetSmsAlmStrings(SMS_REC *sms_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);
int	GetCmdRecStrings(CMD_REC *cmd_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color);


int FillTimeMsg(UCHAR *time_buf);

void FillIndAlmInfo(POINTER point, IND_DATA status, DATE_TIME *date_time, SP_IND_ALM_PKG *info);
void FillIndAllSendInfo(POINTER point, IND_DATA status, DATE_TIME *date_time, SP_IND_ALLSEND_PKG *info);
int BroadCastIndPtState(POINTER point, UCHAR net, USHORT state);
int BroadCastIndPt(POINTER point, UCHAR net, IND_DATA status, DATE_TIME *date_time);

void FillAnaAlmInfo(POINTER point, float value, ANLG_STATUS status, DATE_TIME *date_time, SP_ANA_INFO_PKG *info);
void FillAnaAllSendInfo(POINTER point, float value, ANLG_STATUS status, DATE_TIME *date_time, SP_ANA_INFO_PKG *info);
int BroadCastAnaPtValue(POINTER point, UCHAR net, float value);
int BroadCastAnaPt(POINTER point, UCHAR net, float value, ANLG_STATUS status, DATE_TIME *date_time);

int BroadCastDataInfo(UCHAR dp_type, char info[], int length, UCHAR net);

int BroadCastSms(char* message, BOOL IsAlm);
int BroadCastLms(short lms_id);

int BroadCastHostSwitchCmd(char *hostname, BOOL main_state, char *usr_name);

void FilterAudStr(char *audStr);
BOOL IsAlmRecord(REC_ENTRY_UNION *rec);
int	GetEventTxtString(REC_ENTRY_UNION *rec, char *text_str);


#ifdef __cplusplus
}
#endif




#endif /*_DPS_FUNCTIONS_DEF_H*/


