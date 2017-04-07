		
	/* 只允许定义常量！！！ */		
	/*  定义OIX中用到的一些不便于放在其它头文件中的常量  */
	/**********************************
       SYN by hcl 2007.12.10
    ***********************************/
	
#define	    ALARM_FONT	0
#define	    DBTAB_FONT	0
#define	    SGZY_FONT	0
#define	    TREND_FONT	0
#define	    SBO_FONT	0

#define	    ALM_AUD_TIME  3


/*  define  different  color for interlock  check  */  
#define UNPASS_COLOR    RED1    
#define PASSED_COLOR    GREEN	
#define SYMBOL_COLOR	YELLOW

/*  define  different  color for data quality fault or manual  */  
#define FAULT_COLOR	RED1
#define MANUAL_COLOR	YELLOW

#define RAISE_TIME  10
/****************hcl for double screen*************/
 /*显示器分辨率设置*/

#define LENFORPOINTER 1000     /*用于合并传递crt_n的参数*/
#define STND_WIDTH 1270	       /*1280标准宽度*/
#define STND_HEIGHT 1024       /*标准高度*/

#define MAX_DSP_MOD 10

#define KICKOFFTIME  60*60*1000   /*60分钟不动超级用户被踢!*/


//int SCREEN_WIDTH_MOD[MAX_DSP_MOD]={1024,1280,1600};

/*************************************************/


/************************************************************************/
/*  打印图片 add by hcl                                */
#define SOFTCPY_MODE 0   /*画面软拷贝模式 */
#define NORMAL_MODE  1   /*默认值打印画面，直接屏幕截图模式*/
#define RTP_MODE     2   /*打印报表画面，屏幕取反打印*/
#define SGZY_MODE    3   /*打印趋势，事故追忆，屏幕取反打印*/

/************************************************************************/
#define LAYERNUM 16 //画面层数
int SelectLayer[MAX_CRT_NUMBER][LAYERNUM];
typedef struct{
int UseFilter;
int changeflag;
}USERFILETER;
USERFILETER UseLayerFilter[MAX_CRT_NUMBER];

#define EVENT_MAX_NUM 3 //事件一览表最大分栏

