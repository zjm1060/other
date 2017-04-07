#ifndef _IEC_CMM_DEF_H
#define _IEC_CMM_DEF_H

#include	"../../../dms/v4.0a/inc/dms_com.h"


enum OUTPUT_FILTER {OUTPUT_INFO, OUTPUT_WARN, OUTPUT_ERROR};
void OutputDspInfo(
				   int nFilter,	// how to handle and where to report
				   // 0: default - normal flag
				   // else flags
				   const char* format,// from here the function uses printf syntax
				   ...
				   );

int GetCtrlPrivChannel(char *host, unsigned short *ctrlPrivChannel);
int UpdateCtrlPrivChan(char *host, unsigned short ctrlPrivChannel);

int UpdateHostCommState(char *host, char *comm_name, int chan_no, unsigned short status);
int GetHostCommState(char *host, char *comm_name, int chan_no, unsigned short *status);
void InitOutputFilter(char *comm_task, int stn_no);
void SetOutputFilter(int filter);


#define	DSP_INFO_SIZE		102400

typedef struct
{
	int	header_ptr;
	char	buf[DSP_INFO_SIZE];
}IEC_COMM_DSP_BUF;

void CreateCommDspBuf(void);
void* LinktoCommDspBuf(void);

#endif //_IEC_CMM_DEF_H





















