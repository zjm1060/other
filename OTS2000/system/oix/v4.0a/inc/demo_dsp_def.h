/**********************************
  SYN by hcl 2007.12.10
***********************************/
#define MAX_DEMO_DSP_NUM        50
#define MAX_DEMO_CLASS_NUM      40
#define	DEMO_DSP_OFF	0
#define	DEMO_DSP_ON	1

/*int	demo_dsp_cnt;*/


extern	char	demo_dsp_name_str[MAX_DEMO_DSP_NUM][MAX_FILE_NAME_LEN];
extern	short	demo_dsp_flag/*[]*/;
int demo_class_cnt;

typedef struct 
{
    char class_name[30];    /* max_class=10;name_len=30*/
    char demo_dsp_name_str[MAX_DEMO_DSP_NUM][MAX_FILE_NAME_LEN];
    int demo_dsp_cnt;
}DEMO_DSP_CLASS;

typedef struct 
{
    char added_name[30];    /* max_class=10;name_len=30*/
    char selected_name[30];    
}CUR_CLASS;

CUR_CLASS cur_class[MAX_CRT_NUMBER];

int demo_period,selected_class[MAX_CRT_NUMBER];

