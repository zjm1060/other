/*	simulog_data.h headfile

	Created by yyp, 2009.07.06.
	structure define of simulog shared memory
*/
#ifndef _SIMULOG_DATA_HEAD
#define _SIMULOG_DATA_HEAD

#define MAX_PTNUM_OIX MAX_REVERT_NUM

typedef struct screen_pos
{
	unsigned int x;
	unsigned int y;
}SCREEN_POS;

typedef struct child_dbin
{
	struct screen_pos pos;
	char name[128];
	char time_len;
}CHILD_DBIN;

typedef struct train_task
{
	char	flag_new_send_task;
	char	flag_new_scene;
	char	flag_new_fault;
	int		send_task_id;
	int		task_id;
	int		scene_id;
	int		fault_id;
	char	is_record;
	short	task_status; /* 0:尚未接收；1:已接收；2:启动；3：完成；4:非正常结束  */
	short	scene_status; /* 0:接收中  1::接收完成  */
	short	fault_status; /* 0:接收中  1::接收完成  */
}TRAIN_TASK;



typedef struct oix_pt
{
	char	cur_dbinname[128];
	short	ptnum;
	char	logicalname[MAX_PTNUM_OIX][LOGICALNAME_LEN];
}OIX_PT;


typedef struct hdb_scene_seg
{
	int scene_id;
	char timestr[30];
	int type;
	char inserttime[30];
	char remark[500];
}HDB_SCENE_SEG;

typedef struct hdb_scene
{
	char	requestflag;				/* 0: no new request;   1: new request  2: return success*/
	char	type;						/* 0: reserve;  1: write from real on cur dbin;   2: write from real on specic dev;   3: write from hdb on cur dbin;  4: write from hdb on specic dev; */
	char	name[128];
	unsigned char stn_id;
	unsigned char dev_id;
	char	filename[128];	
	int		pt_num;
	char	lockflag;		
	char	remark[200];
}HDB_SCENE;

typedef struct dbin_point
{
	char curname[128];
	short yxnum;
	short ycnum;
	char yxname[MAX_PTNUM_OIX][LOGICALNAME_LEN];
	char ycname[MAX_PTNUM_OIX][LOGICALNAME_LEN];

}DBIN_POINT;

typedef struct shm_ots
{
	struct	child_dbin child;
	unsigned char	childflag;  /* 0--none 1--new step withchild 2--new step without child */
	char	dbinname[128];		

	unsigned char	dbinflag;   /* 0--none 1--new dbin */
	unsigned char	speed;		/* 2 4 8 16 32 8-normal speed */	
	unsigned char	runflag;	/* 0 -- stop  1--run 2--pause */
	unsigned char	chg_any;	
	unsigned char	prgrate;	/* progress rate, 0-100 */
	short	simu_mode;				/* 0--train mode  1--exam mode*/
	short	login_status;
	int		hdb_write_flag;
	int		groups_id;
	int		groups_role;
	int		user_id;
	int		host_id;
	struct train_task	data_task;
	struct hdb_scene	data_scene;	
	struct dbin_point	data_point;
} SHM_OTS;

#define SHM_OTS_SIZE   sizeof(struct shm_ots)

#endif
