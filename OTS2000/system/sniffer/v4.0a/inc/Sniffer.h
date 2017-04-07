/*共享内存格式(指针)
  WritePoint ReadPoint Count1(u_int)   Content1(Count1)
	写指针   读指针    报文1长度				报文1内容 
	 4         4           4					Count1
*/
#define SHM_SNIFFER_GCOM_KEY	'Y'  //GCOM ShareMemory
#define SHM_SNIFFER_VIP_KEY	'Z'		//VIP ShareMemory
#define GCOM  1        //GCOM Protocol ID
#define VIP   2        //VIP Protocol ID
#define DSP_INFO_SIZE 10240
#define SHM_SNIFFER_HEAD_Length 8
#define MaxPackageLength 2048
#define BufferRecordCount 5000
typedef struct  
{
	u_int writePoint;
	u_int readPoint;
	unsigned int  gcom_tn_hb_recv_flag, gcom_dsv_hb_recv_flag;
	unsigned int  gcom_tn_hb_check_flag, gcom_dsv_hb_check_flag;
}SHM_SNIFFER_HEAD;

typedef struct
{
	SHM_SNIFFER_HEAD header;
	//char	buf[DSP_INFO_SIZE];
	u_short packageLength[BufferRecordCount];
	char buf[BufferRecordCount][MaxPackageLength];
}SHM_SNIFFER;
