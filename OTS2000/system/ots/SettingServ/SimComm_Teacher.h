
#ifndef SIMMCOMM_H
#define SIMMCOMM_H
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//巡视系统与仿真系统的通讯接口
//
#define XHStatOpened		0
#define XHStatClosed		1
#define FUHE_Opened			7	//带负荷分刀闸
#define FUHE_Closed			8	//带负荷合刀闸

//用于教师机向学员机发送数据
typedef struct{
	int		rank;			//电压等级
	BYTE	student;		//学员号
	BYTE	unit;			//单元号
	BYTE	phase;			//ABC三相指示 0-A,1-B,2-C,3-全部
	BYTE	device_type;	//设备类型
	BYTE	device_index;	//该设备在该单元的同种设备中的序列号
	BYTE	weather;		//天气
	BYTE	wind;			//风速
	float	scale;		//设备缩放
	float	pos[4];		//位置x,y,z和朝向
	char	model[64];	//模型名称
}MODEL_MESSAGE;

#define OPERATE_DATA_LENGTH	112

#define OPERATE_COMM_NORMAL	100		//巡视操作记录，正常操作
#define OPERATE_COMM_ERR	101		//巡视操作记录，一般错误操作
#define OPERATE_COMM_FAULT	102		//巡视操作记录，严重错误操作
	
#define SIMCOMM_ECHO		255		//响应教师机

//学员机向教师机发送的变位数据，为了统一起见，学员机和教师机间都用OPERATE_DATA_LENGTH
#define TTS_ID_LENGTH				32

//学员机向教师机发送的变位数据
typedef struct{
	int		student;		//学员号
	char	id[TTS_ID_LENGTH];
	int		state;
	float	value;
	char	opr_msg[OPERATE_DATA_LENGTH];		//学员机向教师机发送的操作记录数据
}COMM_MSG;

int		ProcessTTSData(unsigned char *Buffer);
int		ProcessTTSControl(char *code, int abc, int toStat, int Rank);
int		ProcessTTSBiao(char *code, int Bdevicetype, float fValue, int Rank);
int		ProcessTTSEVT(int nDeviceType, char *code, int abc, int toStat, int nSubType);
int		FindDevice(char *code, int type, int phase,  int *Rank);
int		ProcessStudentData( COMM_MSG *pMsg, int nWorkerIndex );
int		ProcessTeacherData( MODEL_MESSAGE *pMsg );
int		ProcessTTSGuide( char *code, int nVaule );
int		ProcessTTSReset();

void loadconfig();
void InsertErrLog(char *err,WORD nType);
UINT TimerThread(LPVOID lParam);

#endif