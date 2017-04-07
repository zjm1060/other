

#include	<windows.h>
#include	<stdio.h>
#include	"../../../dms/v4.0a/inc/dms_com.h"

#include	"../inc/jtaud.h"


/*
函数一览: 

	jTTSPreInit			初始化之前设置系统将读取的音库
	jTTSInit			初始化jTTS核心，读取音库
	jTTSEnd				释放jTTS核心占用内存
	jTTSSetLogFile		设置系统的记录日志文件

	jTTSSetPlay			确定放音设备和放音过程中的回调机制
						(标准版本下只支持缺省放音设备)
	jTTSPlay			利用放音设备阅读指定的文本
	jTTSStop			中止当前的阅读
	jTTSPause			暂停当前的阅读
	jTTSResume			恢复当前的阅读
	jTTSSetParam		设置参数
	jTTSGetParam		得到参数
	jTTSSet2			通过结构的方式一次性设置语音合成的所有参数
	jTTSGet2			通过结构的方式一次性获得语音合成的所有参数
	jTTSPlayToFileEx	对文本进行语音合成，将结果写入语音文件，并且使用传入的合成参数，
	                    使用新的回调函数，可以回传用户指定的数据

	(标准版本不支持以下函数)
	jTTSSessionStart	开始直接得到语音流的语音合成过程
	jTTSSessionConfig2	设置语音合成过程的参数，使用新的结构
	jTTSSessionGetDataEx	进行语音合成，直接得到语音流，使用内部缓冲区
	jTTSSessionStop			中止直接得到语音流的语音合成过程

-----------------------
下述函数仅为和以前版本兼容而保留:
1.0
	jTTSSet				通过结构的方式一次性设置语音合成的所有参数(为1.0版兼容保留，请使用jTTSSet2)
	jTTSGet				通过结构的方式一次性获得语音合成的所有参数(为1.0版兼容保留，请使用jTTSGet2)
	jTTSPlayToFileConfig		对文本进行语音合成，将结果写入语音文件，并且使用传入的合成参数
								(为1.0版兼容保留，请使用jTTSPlayToFileConfig2)
	jTTSSessionConfig	设置语音合成过程的参数
						(为1.0版兼容保留，请使用jTTSSessionConfig2)

2.0
	jTTSSessionGetData	进行语音合成，直接得到语音流
						(为2.0版兼容保留，请使用jTTSSessionConfig2)

	jTTSSetPlayToFile	确定放音到文件过程中的回调机制
	jTTSPlayToFile		对文本进行语音合成，将结果写入语音文件
						(标准版本下只支持WAV文件)
	jTTSPlayToFileConfig2		对文本进行语音合成，将结果写入语音文件，并且使用传入的合成参数

2.1
	jTTSSetCodePage		设置语音合成文本的代码集
	jTTSGetCodePage		得到当前语音合成文本的代码集
	jTTSSetVoiceID		设置语音合成所用的音库
	jTTSGetVoiceID		得到当前语音合成所用的音库
	jTTSSetPitch		设置语音合成的基频
	jTTSGetPitch		得到语音合成的基频
	jTTSSetVolume		设置语音合成的音量
	jTTSGetVolume		得到语音合成的音量
	jTTSSetSpeed		设置语音合成的语速
	jTTSGetSpeed		得到语音合成的语速
	jTTSSetPuncMode		设置语音合成的标点符号阅读方式
	jTTSGetPuncMode		得到语音合成的标点符号阅读方式
	jTTSSetDigitMode	设置语音合成的数字串阅读方式
	jTTSGetDigitMode	得到语音合成的数字串阅读方式
	jTTSSetEngMode		设置语音合成的英文串阅读方式
	jTTSGetEngMode		得到语音合成的英文串阅读方式
	jTTSSetRemoteTryTime		设置远程合成时连接的重试次数
	jTTSGetRemoteTryTime		得到远程合成时连接的重试次数
	jTTSSetRemoteLoadBalance	设置远程合成时是否使用负载平衡机制
	jTTSGetRemoteLoadBalance	得到远程合成时是否使用负载平衡机制
*/




#define ERR_NONE			0
#define	ERR_ALREADYINIT	1
#define	ERR_NOTINIT			2 
#define	ERR_MEMORY			3
#define	ERR_INVALIDHWND		4
#define	ERR_INVALIDFUNC		5
#define	ERR_OPENLIB			6
#define	ERR_READLIB			7
#define	ERR_PLAYING		8
#define	ERR_DONOTHING			9
#define ERR_INVALIDTEXT		10
#define	ERR_CREATEFILE			11
#define	ERR_WRITEFILE		12
#define	ERR_FORMAT				13
#define	ERR_INVALIDSESSION		14
#define	ERR_TOOMANYSESSION		15
#define	ERR_MORETEXT			16
#define	ERR_CONFIG				17
#define	ERR_OPENDEVICE			18
#define	ERR_RESETDEVICE		19
#define	ERR_PAUSEDEVICE		20
#define	ERR_RESTARTDEVICE		21
#define	ERR_STARTTHREAD		22
#define	ERR_BEGINOLE		23
#define	ERR_NOTSUPPORT			24
#define	ERR_SECURITY		25
#define	ERR_CONVERT			26
#define	ERR_PARAM				27
#define	ERR_INPROGRESS		  28
#define	ERR_INITSOCK			29
#define	ERR_CREATESOCK			30
#define	ERR_CONNECTSOCK		31
#define	ERR_TOOMANYCON			32
#define	ERR_CONREFUSED			33
#define	ERR_SEND				34
#define	ERR_RECEIVE			35
#define	ERR_SERVERSHUTDOWN		36
#define	ERR_OUTOFTIME			37
#define	ERR_CONFIGTTS			38
#define	ERR_SYNTHTEXT			39
#define	ERR_CONFIGVERSION		40

// Log File Flag
#define LOG_TEXT	0x01	// 是否在日志文件中记录合成的文本
#define LOG_SESSION	0x02	// 是否记录每个Session过程的详细调用情况

#define WM_JTTS_NOTIFY	(WM_USER + 0x4999)

// wParam of WM_JTTS_NOTIFY or JTTSCALLBACKPROC
#define NOTIFY_BEGIN		0	// lParam: not use 
#define NOTIFY_END			1	// lParam: not use
#define NOTIFY_PROGRESS		2	// lParam: bytes of content have been read
#define NOTIFY_PROGRESS2	3	// lParam: end postion of content will be read in next session

#define STATUS_NOTINIT	0
#define STATUS_READING	1
#define STATUS_PAUSE	2
#define STATUS_IDLE		3

// CodePage，在此版本中，CP_GB2312和CP_GBK等效
#define CP_GB2312		0	
#define CP_GBK			1	
#define CP_BIG5			2

// VoiceID，在此版本中，只能使用VID_FEMALE1
#define VID_MALE1		0
#define VID_FEMALE1		1

// Volume, Speed, Pitch
#define VOLUME_MIN	0
#define VOLUME_MAX	9
#define SPEED_MIN	0
#define SPEED_MAX	9
#define PITCH_MIN	0
#define PITCH_MAX	9

// PuncMode
#define PUNC_OFF		0
#define PUNC_ON			1

// DigitMode，在此版本中，增加了DIGIT_AUTO_TELEGRAM，无法自动判断的数字串将用数字来读
#define DIGIT_AUTO_NUMBER	0
#define DIGIT_TELEGRAM		1
#define DIGIT_NUMBER		2
#define DIGIT_AUTO_TELEGRAM	3

#define DIGIT_AUTO			DIGIT_AUTO_NUMBER

// EngMode
#define ENG_AUTO			0	/* 自动方式 */
#define ENG_SAPI			1	/* 强制SAPI方式 */
#define ENG_LETTER			2	/* 强制单字母方式 */
#define ENG_LETTER_PHRASE	3	/* 单字母＋自录音词方式 */

enum JTTSPARAM
{
	PARAM_CODEPAGE,			// CP_xxx
	PARAM_VOICEID,			// VID_xxx
	PARAM_PITCH,			// PITCH_MIN - PITCH_MAX
	PARAM_VOLUME,			// VOLUME_MIN - VOLUME_MAX
	PARAM_SPEED,			// SPEED_MIN - SPEED_MAX
	PARAM_PUNCMODE,			// PUNCMODE_xxx
	PARAM_DIGITMODE,		// DIGITMODE_xxx
	PARAM_ENGMODE,			// ENGMODE_xxx
};

enum JTTSREMOTEPARAM
{
	PARAM_REMOTE_TRYTIMES,			// 合法值：1 - 100, 缺省值：10
	PARAM_REMOTE_LOADBALANCE,		// 合法值：TRUE, FALSE, 缺省值：FALSE
};

#define FORMAT_WAV			0	// PCM Native (目前为16KHz, 16Bit)
// 以下内容仅在专业版中支持
#define FORMAT_VOX_6K		1	// OKI ADPCM, 6KHz, 4bit (Dialogic Vox)
#define FORMAT_VOX_8K		2	// OKI ADPCM, 8KHz, 4bit (Dialogic Vox)
#define FORMAT_ALAW_8K		3	// A律, 8KHz, 8Bit
#define FORMAT_uLAW_8K		4	// u律, 8KHz, 8Bit
#define FORMAT_WAV_8K8B		5	// PCM, 8KHz, 8Bit
#define FORMAT_WAV_8K16B	6	// PCM, 8KHz, 16Bit
#define FORMAT_WAV_16K8B	7	// PCM, 16KHz, 8Bit
#define FORMAT_WAV_16K16B	8	// PCM, 16KHz, 16Bit

#define FORMAT_FIRST		0
#define FORMAT_LAST			8

#define JTTS_VERSION	0x0002	// version 2.0 

typedef struct 
{
	WORD wVersion;
	UINT nCodePage;
	UINT nVoiceID;
	int nPitch;
	int nVolume;
	int nSpeed;
	int nPuncMode;
	int nDigitMode;
	int nEngMode;
}JTTSCONFIG2;

struct JTTSREMOTECONFIG
{
	WORD wVersion;
	int nTryTimes;
	BOOL bLoadBalance;
};

typedef BOOL (* JTTSCALLBACKPROC)(WORD wParam, LONG lParam);
typedef BOOL (* JTTSCALLBACKPROCEX)(WORD wParam, LONG lParam, DWORD dwUserData);

//------------------------------------------------------------------------
// 系统函数

typedef  int ERRCODE; 

ERRCODE jTTSSetSerialNo(const char * pszSerialNo);
ERRCODE jTTSPreInit(int * arrayVoiceID, int nCount);
ERRCODE jTTSInit(const char * pcszLibPath);
ERRCODE jTTSEnd();
void    jTTSSetLogFile(const char* pcszLogFile, DWORD dwLogFlag);

//------------------------------------------------------------------------
// 播放函数 

// 标准版本中仅支持uDeviceID == WAVE_MAPPER(-1), 即缺省放音音设备
ERRCODE jTTSSetPlay(UINT uDeviceID, HWND hwnd, JTTSCALLBACKPROC lpfnCallback);
ERRCODE jTTSPlay(const char * pcszText);
ERRCODE jTTSStop();
ERRCODE jTTSPause();
ERRCODE jTTSResume();
int		jTTSGetStatus();

//------------------------------------------------------------------------
// 播放到文件函数 

// 标准版本中仅支持nFormat = FORMAT_WAV, 即写成WAV文件格式
// pConfig == NULL时，使用系统的Config
// dwFlag, 目前无用，必须为0
ERRCODE jTTSPlayToFileEx();

//------------------------------------------------------------------------
// 设置函数 
ERRCODE jTTSSet2();
ERRCODE jTTSGet2();
ERRCODE jTTSSetParam();
DWORD   jTTSGetParam();

//-------------------------------------------------------------
// 远程合成参数设置
ERRCODE jTTSSetRemote();
ERRCODE jTTSGetRemote();
ERRCODE jTTSSetRemoteParam();
DWORD   jTTSGetRemoteParam();

//------------------------------------------------------------------------
// 合成过程底层函数 

// 下列jTTSSession系列函数仅在专业版中支持
ERRCODE jTTSSessionStart(D);
ERRCODE jTTSSessionConfig2();
ERRCODE jTTSSessionRemoteConfig();
ERRCODE jTTSSessionGetDataEx();
ERRCODE jTTSSessionStop();
ERRCODE jTTSSessionGetReadBytes();

// -------------------------------------------------------------------------------------
// 以下数据结构和函数仅为和1.0版本兼容
struct JTTSCONFIG
{
	UINT nCodePage;
	UINT nVoiceID;
	int nPitch;
	int nVolume;
	int nSpeed;
	int nPuncMode;
	int nDigitMode;
};

ERRCODE jTTSPlayToFileConfig();
ERRCODE jTTSSet();
ERRCODE jTTSGet();
ERRCODE jTTSSessionConfig();

// 以下函数仅为和2.0版本兼容
ERRCODE jTTSSessionGetData();
ERRCODE jTTSSetPlayToFile();
ERRCODE jTTSPlayToFile();
ERRCODE jTTSPlayToFileConfig2();

// 以下函数仅为和2.1版本兼容
ERRCODE jTTSSetCodePage(int nCodePage);
int		jTTSGetCodePage();
ERRCODE jTTSSetVoiceID(int nVoiceID);
int		jTTSGetVoiceID();
ERRCODE jTTSSetPitch(int nPitch);
int		jTTSGetPitch();
ERRCODE jTTSSetVolume(int nVolume);
int		jTTSGetVolume();
ERRCODE jTTSSetSpeed (int nSpeed);
int		jTTSGetSpeed();
ERRCODE jTTSSetPuncMode(int bPuncMode);
int		jTTSGetPuncMode();
ERRCODE jTTSSetDigitMode(int nDigitMode);
int		jTTSGetDigitMode();
ERRCODE jTTSSetEngMode(int nEngMode);
int		jTTSGetEngMode();
ERRCODE jTTSSetRemoteTryTime(int nTryTime);
int		jTTSGetRemoteTryTime();
ERRCODE jTTSSetRemoteLoadBalance(BOOL bLoadBalance);
BOOL	jTTSGetRemoteLoadBalance();



BOOL	g_CallBackProc(WORD wParam, LONG lParam);
static BOOL		g_bFinishPlay = FALSE;
static BOOL		g_bInit = FALSE;
static BOOL		bOutputFile = FALSE;
static char		outputFileName[256];

#define	JTTS_LIBRARY_PATH1	"\\tts\\jttts\\bin"
#define	JTTS_LIBRARY_PATH2	"\\语音合成\\bin"


int	InitAudioLibraryJT(int nPitch, int nVolume, int nSpeed, char *outputFile)
{
	int		iErr;
	char	err_str[128];
	
	//This structure includes all parameters used by audio composing.
	JTTSCONFIG2 sConfig =
	{ 
		2,					//version number
		CP_GB2312,			//code set (CP_GB2312, CP_GBK, CP_BIG5) 
		VID_FEMALE1,		//voice type 
		nPitch,				//base pitch (0-9). more bigger, more higher.
		nVolume,			//volume of voice (0-9). more bigger, more higher.
		nSpeed,				//speed  of voice (0-9). more bigger, more faster.
		PUNC_OFF,			//the punctuation can be read or not.(PUNC_ON,PUNC_OFF)
		DIGIT_AUTO_TELEGRAM,//the mode of reading digital string.
							//( DIGIT_TELEGRAM, DIGIT_NUMBER, DIGIT_AUTO_NUMBER, DIGIT_AUTO_TELEGRAM )
		ENG_LETTER			//the mode of reading English
							//( ENG_AUTO,  ENG_SAPI, ENG_LETTER,  ENG_LETTER_PHRASE )
	};

	//set serial number
	if( (iErr=jTTSSetSerialNo("0E7A438C6736"))!=ERR_NONE )
	{
		sprintf(err_str, "jTTSSetSerialNo() error! (errcode=%d)\n",iErr); 
		Err_log(err_str);
		return -1;
	}

	//initialize audio library
	if( (iErr=jTTSInit(JTTS_LIBRARY_PATH1))!=ERR_NONE )
	{
		if( (iErr=jTTSInit(JTTS_LIBRARY_PATH2))!=ERR_NONE )
		{
			sprintf(err_str, "jTTSInit() error! (errcode=%d, path=%s)\n",iErr,JTTS_LIBRARY_PATH2);  
			Err_log(err_str);
			return -1;
		}
	}

	if(GetExLangState())
		sConfig.nEngMode= ENG_AUTO;
	else
		sConfig.nEngMode= ENG_LETTER;

	//set parameter for audio composing
	if( (iErr= jTTSSet2(&sConfig))!=ERR_NONE )
	{
		sprintf(err_str, "jTTSSet2() error! (errcode=%d)\n",iErr);  
		Err_log(err_str);
		return -1;
	}

	if(outputFile)
	{
		strcpy(outputFileName, outputFile);
		bOutputFile = TRUE;
	}
	else
	{
		//set audio device(-1 means default device),  set callback function.
		if( (iErr=jTTSSetPlay(-1, NULL, g_CallBackProc ))!=ERR_NONE )
		{
			sprintf(err_str, "jTTSSetPlay() error! (errcode=%d)\n",iErr);  
			Err_log(err_str);
			return -1;
		}
	}

	Err_log("Success to init JT audio lib");
	g_bInit= TRUE;
	return 0;
}


int PlayStrAudioJT(char *str)
{
	int		iErr;
	char	err_str[128];

	if(!g_bInit)
	{  
		sprintf(err_str, "No init JT audio kernal\n"); 
		Err_log(err_str);
		return -1;
	}

	if(bOutputFile)
	{
		if((iErr=jTTSPlayToFileEx(str, outputFileName, FORMAT_WAV_8K8B, NULL, 0, NULL, 0x12345678))!=ERR_NONE )
		{
			sprintf(err_str, "jTTSPlayToFileEx() error! (errcode=%d)\n", iErr);  
			Err_log(err_str);
			return -1;
		}
		return 0;
	}


	//read text and audio play 
	g_bFinishPlay= FALSE;
	if( (iErr=jTTSPlay(str))!=ERR_NONE )
	{  
		sprintf(err_str, "jTTSPlay() error! (errcode=%d)\n",iErr); 
		Err_log(err_str);
		if(iErr==ERR_PLAYING)
			Sleep(2000);
		return -1;
	}
	else
	{	//because jTTSPlay() create a thread for audio play, so waiting for thread finished.
		while( !g_bFinishPlay ) 
			Sleep(100);			
	}
	Sleep(500);
	return 0;
}

int CreateStrAudFileJT(char *str, char *outAudFile)
{
	int		iErr;
	char	err_str[128];

	if(!g_bInit)
	{  
		sprintf(err_str, "No init JT audio kernal\n"); 
		Err_log(err_str);
		return -1;
	}

	if(!outAudFile)
	{  
		sprintf(err_str, "outAudFile is null\n"); 
		Err_log(err_str);
		return -1;
	}

	if((iErr=jTTSPlayToFileEx(str, outAudFile, FORMAT_WAV_8K8B, NULL, 0, NULL, 0x12345678))!=ERR_NONE )
	{
		sprintf(err_str, "jTTSPlayToFileEx() error! (errcode=%d)\n", iErr);  
		Err_log(err_str);
		return -1;
	}
	Sleep(100);
	return 0;
}


BOOL g_CallBackProc(WORD wParam, LONG lParam)
{
	switch(wParam)
	{
	case NOTIFY_BEGIN:
		//printf("Callback Func - Notify - Begin\n");
		break;
	case NOTIFY_END:
		//printf("Callback Func - Notify - End\n");
		g_bFinishPlay = TRUE;
		break;
	case NOTIFY_PROGRESS:
		//printf("Callback Func - Notify - %ld End\n", lParam);
		break;
	case NOTIFY_PROGRESS2:
		//printf("Callback Func - Notify - Will Read To %ld\n", lParam);
		break;
	default:
		//printf("Callback Func - Notify - Unknow\n");
		break;
	}
	return TRUE;
}


int ReleaseAudioLibraryJT(void)
{
	if(g_bInit)
		jTTSEnd();
	g_bInit= 0;
	return 0;
}
