#ifndef _OTS_HDB_HEAD
#define _OTS_HDB_HEAD

#define MSWIN32

#include <QTime>
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QtSql/qsql.h>
#include <QSqlQuery>   
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>

#include "/home/ems/system/lan/v4.0a/inc/lan.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots.h"

#pragma warning( disable : 4996 )

//#define DEBUG
#define	REC_CMD				0
#define	REC_ALM				1
#define REC_SMS				2

#define TYPE_INT			0
#define TYPE_FLOAT			1
#define TYPE_STR			2
#define TYPE_TIME			3

#define SIM_TRAIN			0
#define SIM_OPT				1

#define QUERY_INTERVAL		2
#define MAX_ROW_NUM			1000
#define MAX_ROW_NUM_LARGE	10000
#define MAX_ROW_NUM_HUGE	160000
#define MAX_RECORD_SIZE		MAX_ROW_NUM_LARGE
#define MAX_TABLE_NUM		30
#define TABLE_NAME_SIZE		30
#define FIELD_NAME_SIZE		200
#define VALUE_STR_SIZE		500
#define OPR_SYMBOL_SIZE		10
#define MAX_FIELD_NUM		20	
#define MAX_COND_NUM		10
#define MAX_FILTER_NUM		3
#define MAX_USER_NUM		300
#define MAX_OTS_HOST_NUM	20
#define MAX_DEV_NUM_OTS		MAX_DEV_NUM_HDB

#define	TABLE_USERINFO		1
#define	TABLE_TRAIN_INFO	2
#define	TABLE_LOGIN_INFO	3
#define	TABLE_RECORD		4
#define	TABLE_SCENE			5
#define	TABLE_SCENE_DATA	6
#define	TABLE_REVERT		7
#define	TABLE_FAULT			8
#define	TABLE_FAULT_DATA	9
#define	TABLE_TASK_GRP		10
#define	TABLE_TASK			11
#define	TABLE_SEND_TASK		12
#define	TABLE_HOST			13
#define	TABLE_OPR_TYPE		14
#define	TABLE_LOCAL_OPR		15
#define	TABLE_RUN_MODEL		16
#define	TABLE_RUN_SEQUENCE	17
#define	TABLE_MODEL			18
#define	TABLE_SEQUENCE		19
#define	TABLE_VAR_RELATION	20
#define TABLE_SUBJECT		21
#define	TABLE_SCORE			22
#define TABLE_GROUPS		23
#define TABLE_GROUPS_RECORD	24
#define TABLE_USER_TYPE 	25
#define TABLE_DOC_TYPE 	26
#define TABLE_DOC 	27
#define TABLE_STUDY_PLAN 	28
#define TABLE_STUDY_RECORD 	29
#define TABLE_CONTEST 	30
#define TABLE_SEND_CONTEST 	31
#define DB_SELECT 1
#define DB_INSERT 2
#define DB_UPDATE 3
#define DB_DELETE 4
#define DB_FUZZY_SELECT 5

#define CHAR_INIT	'#'

typedef struct _data_userinfo
{
	int		user_id;
	char	name[20];
	char	longname[20];
	char	password[20];
	short	role;
	short	sex;
	short	edu;
	char	company[30];
	char	department[20];
	char	duty[20];
	char	email[30];
	char	mtel[20];
	char	tel[20];
	char	idcard[30];
	char	privilege[50];
	int		groups_id;
	short	groups_role;
//dzy added 2014 for HN
	int user_range; //用户级别
	int user_type_id;//用户工种
	//foreign key
	char	groups_name[60];
// public:
// 	void operator =(_data_userinfo userInfo)
// 	{
// 		user_id = userInfo.user_id;
// 		role = userInfo.role;
// 		sex = userInfo.sex;
// 		edu = userInfo.edu;
// 		groups_id = userInfo.groups_id;
// 		groups_role = userInfo.groups_role;
// 		user_range = userInfo.user_range; //用户级别
// 		user_type_id = userInfo.user_type_id;//用户工种
// 		strcpy(name,userInfo.name);
// 		strcpy(longname,userInfo.longname);
// 		strcpy(password,userInfo.password);
// 		strcpy(company,userInfo.company);
// 		strcpy(department,userInfo.department);
// 		strcpy(duty,userInfo.duty);
// 		strcpy(email,userInfo.email);
// 		strcpy(mtel,userInfo.mtel);
// 		strcpy(tel,userInfo.tel);
// 		strcpy(idcard,userInfo.idcard);
// 		strcpy(privilege,userInfo.privilege);
// 		strcpy(groups_name,userInfo.groups_name);
// 	}
} DATA_USERINFO;

typedef struct _flag_userinfo
{
	char	user_id;
	char	name;
	char	longname;
	char	password;
	char	role;
	char	sex;
	char	edu;
	char	company;
	char	department;
	char	duty;
	char	email;
	char	mtel;
	char	tel;
	char	idcard;
	char	privilege;
	char	groups_id;
	char	groups_role;
	//dzy added 2014 for HN
	char user_range; //用户级别
	char user_type_id;//用户工种
	//foreign key
	char	groups_name;
} FLAG_USERINFO;

typedef struct _data_train_info
{
	int		user_id;
	int		host_id;
	int		send_task_id;
	int		task_id;
	int		scene_id;
	int		fault_id;
	short	login_status;
	short	status;
	short	speed;
	char	dbin_file[50];
	char	task_status;		//0:无新任务；1:新的任务；2: 任务已处理
	char	scene_status;
	char	fault_status;
	char	is_record;
	char	simu_mode;//0 培训 1：考试 2：竞赛
	char inter_mode; //0：闭锁      1：闭锁无提示  2：解锁
	char task_startable; //0：任务不可以开始      1：任务可以开始
	//foreign key
	char	userinfo_name[20];
	char	userinfo_longname[20];
	char	host_name[20];
	char	task_name[50];
	char	scene_name[50];
	char	fault_name[50];	
} DATA_TRAIN_INFO;

typedef struct _flag_train_info
{
	char	user_id;
	char	host_id;
	char	send_task_id;
	char	task_id;
	char	scene_id;
	char	fault_id;
	char	login_status;
	char	status;
	char	speed;
	char	dbin_file;
	char	task_status;
	char	scene_status;
	char	fault_status;
	char	is_record;
	char	simu_mode;//0 培训 1：考试 2：竞赛
	char  inter_mode; //0：闭锁      1：闭锁无提示  2：解锁
	char task_startable; //0：任务不可以开始      1：任务可以开始
	//foreign key
	char	userinfo_name;
	char	userinfo_longname;
	char	host_name;
	char	task_name;
	char	scene_name;
	char	fault_name;	
} FLAG_TRAIN_INFO;

typedef struct _data_login_info
{
	int		user_id;
	char	login_time[22];
	char	logout_time[22];
	int		host_id;

	//foreign key
	char	userinfo_name[20];	
	char	userinfo_longname[20];
	char	host_name[20];

	//for query conditon
	char	datatime[22];
	char	datatime_end[22];
} DATA_LOGIN_INFO;


typedef struct _flag_login_info
{
	char	user_id;
	char	login_time;
	char	logout_time;
	char	host_id;

	//foreign key
	char	userinfo_name;
	char	userinfo_longname;
	char	host_name;

	//for query conditon
	char	datatime;
	char	datatime_end;

	//conditon opr symbol
	char	datetime_opr[OPR_SYMBOL_SIZE];
	char	datetime_end_opr[OPR_SYMBOL_SIZE];
} FLAG_LOGIN_INFO;


typedef struct _data_record
{
	int		user_id;
	int		host_id;
	int		groups_id;
	char	datetime[22];
	short	ms;
	short	type;
	char	content[100];
	char	station_id;	
	char	dev_id;	
	int		task_id;
	int		send_task_id;
	char	logicalname [38];
	short	type_id;
	short 	status;
	float	fvalue;

	//foreign key
	char	host_name[20];			
	char	userinfo_name[20];
	char	userinfo_longname[20];
	char	groups_name[60];

	//for query conditon
	char	datetime_end[22];
} DATA_RECORD;

typedef struct _flag_record
{
	char	user_id;
	char	host_id;
	char	groups_id;
	char	datetime;
	char	ms;
	char	type;
	char	content;
	char	station_id;	
	char	dev_id;		
	char	task_id;
	char	send_task_id;
	char	logicalname;
	char	type_id;
	char	status;
	char	fvalue;

	//foreign key
	char	host_name;
	char	userinfo_name;	
	char	userinfo_longname;
	char	groups_name;

	//for query conditon
	char	datetime_end;

	//conditon opr symbol
	char	datetime_opr[OPR_SYMBOL_SIZE];
	char	datetime_end_opr[OPR_SYMBOL_SIZE];
} FLAG_RECORD;


typedef struct _data_scene
{
	int		scene_id;
	char	name[50];
	int		user_id;
	short	src;
	char	datetime[22];
	char	remark[200];	
	char	station_id;	
	char	dev_id;	
	char	filename[50];	
	//foreign key
	char	userinfo_name[20];
	char	userinfo_longname[20];
	
} DATA_SCENE;


typedef struct _flag_scene
{
	char	scene_id;
	char	name;
	char	user_id;
	char	src;
	char	datetime;
	char	remark;	
	char	station_id;	
	char	dev_id;	
	char	filename;

	//foreign key
	char	userinfo_name;	
	char	userinfo_longname;
} FLAG_SCENE;

typedef struct _data_scene_data
{
	int		scene_id;
	char	logicalname[52];
	short	status;
	float	data;	
} DATA_SCENE_DATA;

typedef struct _flag_scene_data
{
	char	scene_id;
	char	logicalname;
	char	status;
	char	data;	
} FLAG_SCENE_DATA;

typedef struct _data_revert
{
	int		revert_id;
	char	name[50];
	int		user_id;
	char	start_time[22];
	char	end_time[22];
	char	filename[50];
	char	remark[200];
	char	station_id;	
	char	dev_id;	
	char	src;	
	char	type;
	
	//foreign key
	char	userinfo_name[20];	
	char	userinfo_longname[20];
} DATA_REVERT;

typedef struct _flag_revert
{
	char	revert_id;
	char	name;
	char	user_id;
	char	start_time;
	char	end_time;
	char	filename;
	char	remark;
	char	station_id;	
	char	dev_id;	
	char	src;	
	char	type;

	//foreign key
	char	userinfo_name;	
	char	userinfo_longname;
} FLAG_REVERT;

typedef struct _data_fault
{
	int		fault_id;
	char	name[50];
	int		user_id;
	short	type;
	char	datetime[22];
	char	remark[200];
	char	station_id;	
	char	dev_id;	

	//foreign key
	char	userinfo_name[20];	
	char	userinfo_longname[20];
	
} DATA_FAULT;

typedef struct _flag_fault
{
	char	fault_id;
	char	name;
	char	user_id;
	char	type;
	char	datetime;
	char	remark;
	char	station_id;	
	char	dev_id;	

	//foreign key
	char	userinfo_name;	
	char	userinfo_longname;
	
} FLAG_FAULT;

typedef struct _data_fault_data
{
	int		fault_id;
	char	logicalname[52];
	short	type;
	float	init_value;
	float	dst_value;
	float	norm_value;
	int		chg_time;
	short	chg_mode;
	short	delay;
} DATA_FAULT_DATA;

typedef struct _flag_fault_data
{
	char	fault_id;
	char	logicalname;
	char	type;
	char	init_value;
	char	dst_value;
	char	norm_value;
	char	chg_time;
	char	chg_mode;
	char	delay;
} FLAG_FAULT_DATA;

typedef struct _data_task_grp
{
	short	task_grp_id;
	char	name[30];
	char	remark[50];
} DATA_TASK_GRP;

typedef struct _flag_task_grp
{
	char	task_grp_id;
	char	name;
	char	remark;
} FLAG_TASK_GRP;

typedef struct _data_task
{
	int		task_id;
	char	name[50];
	short	task_grp_id;
	short	type;
	char	datetime[22];
	int		user_id;
	char	remark[2000];
	char	station_id;	
	char	dev_id;	
	int		scene_id;
	int		fault_id;

	//dzy added 2014 for HN
	int user_range; //用户级别
	int user_type_id;//用户工种	
	int task_userange;// 0:考试专用 1：可用于学习

	//foreign key
	char	userinfo_name[20];
	char	userinfo_longname[20];
	char	task_grp_name[30];	
	char	scene_name[50];
	char	fault_name[50];
} DATA_TASK;

typedef struct _flag_task
{
	char	task_id;
	char	name;
	char	task_grp_id;
	char	type;
	char	datetime;
	char	user_id;
	char	remark;
	char	station_id;	
	char	dev_id;	
	char	scene_id;
	char	fault_id;

	//dzy added 2014 for HN
	char user_range; //用户级别
	char user_type_id;//用户工种
	char  task_userange;// 0:考试专用 1：可用于学习
	//foreign key
	char	userinfo_name;
	char	userinfo_longname;
	char	task_grp_name;	
	char	scene_name;
	char	fault_name;
} FLAG_TASK;

typedef struct _data_send_task
{
	int		send_task_id;
	int		task_id;
	char	send_time[22];
	int		user_id;
	char	start_time[22];
	short	status;
	char	end_time[22];
	float	score;
	int		time_len;
	float	tutor_score;	//DZY ADDED 140708 教员给分
	//foreign key
	char	task_name[50];
	char	userinfo_name[20];	
	char	userinfo_longname[20];

	//for query conditon
	char	start_time_start[22];
	char	start_time_end[22];
	char	end_time_start[22];
	char	end_time_end[22];

} DATA_SEND_TASK;

typedef struct _flag_send_task
{
	char	send_task_id;
	char	task_id;
	char	send_time;
	char	user_id;
	char	start_time;
	char	status;
	char	end_time;
	char	score;
	char	time_len;
	char	tutor_score;	//DZY ADDED 140708 教员给分

	//foreign key
	char	task_name;
	char	userinfo_name;	
	char	userinfo_longname;

	//for query conditon
	char	start_time_start;
	char	start_time_end;
	char	end_time_start;
	char	end_time_end;
	
	//conditon opr symbol
	char	start_time_start_opr[OPR_SYMBOL_SIZE];
	char	start_time_end_opr[OPR_SYMBOL_SIZE];
	char	end_time_start_opr[OPR_SYMBOL_SIZE];
	char	end_time_end_opr[OPR_SYMBOL_SIZE];
} FLAG_SEND_TASK;

typedef struct _data_host
{
	int		host_id;
	char	name[20];
	char	oth_name[30];
	char	ip_a[30];
	char	ip_b[30];
	short	is_student;
	short	is_teacher;
	short	is_server;
	int	server_id;
} DATA_HOST;

typedef struct _flag_host
{
	char	host_id;
	char	name;
	char	oth_name;
	char	ip_a;
	char	ip_b;
	char	is_student;
	char	is_teacher;
	char	is_server;
	char	server_id;
} FLAG_HOST;

typedef struct _data_opr_type
{
	short	opr_type;
	char	name[60];
} DATA_OPR_TYPE;

typedef struct _flag_opr_type
{
	char	opr_type;
	char	name;
} FLAG_OPR_TYPE;

typedef struct _data_local_opr
{
	int		opr_id;
	short	opr_type;
	char	name[100];
	short	opr_mode;
	char	msg[100];
	char	show_mode;
	short	node_type;		
	int		parent_id;

	//foreign key
	char	opr_type_name[20];	
} DATA_LOCAL_OPR;

typedef struct _flag_local_opr
{
	char	opr_id;
	char	opr_type;
	char	name;
	char	opr_mode;
	char	msg;
	char	show_mode;
	char	node_type;		
	char	parent_id;
		
	//foreign key
	char	opr_type_name;	
} FLAG_LOCAL_OPR;


typedef struct _data_run_model
{
	int		model_id;
	int		host_id;
	short	status;
	char	start_time[22];	
	
	//foreign key
	char	model_name[30];		
} DATA_RUN_MODEL;

typedef struct _flag_run_model
{
	char	model_id;
	char	host_id;
	char	status;
	char	start_time;

	//foreign key
	char	model_name;		
} FLAG_RUN_MODEL;

typedef struct _data_run_sequence
{
	int		seq_id;
	int		host_id;
	short	status;
	char	start_time[22];	
	
	//foreign key
	char	sequence_name[30];			
} DATA_RUN_SEQUENCE;

typedef struct _flag_run_sequence
{
	char	seq_id;
	char	host_id;
	char	status;
	char	start_time;

	//foreign key
	char	sequence_name;		
} FLAG_RUN_SEQUENCE;

typedef struct _data_model
{
	int		model_id;
	char	name[50];
	char	long_name[50];
	char	datetime[22];
	char	station_id;	
	char	dev_id;	
		
} DATA_MODEL;

typedef struct _flag_model
{
	char	model_id;
	char	name;
	char	long_name;
	char	datetime;
	char	station_id;	
	char	dev_id;	
		
} FLAG_MODEL;


typedef struct _data_sequence
{
	int		seq_id;
	char	name[50];
	char	long_name[50];
	char	datetime[22];	
	char	station_id;	
	char	dev_id;	
		
} DATA_SEQUENCE;


typedef struct _flag_sequence
{
	char	seq_id;
	char	name;
	char	long_name;
	char	datetime;
	char	station_id;	
	char	dev_id;	
		
} FLAG_SEQUENCE;

typedef struct _data_var_relation
{
	int		model_id;
	char	name[30];
	short	type;
	char	logicalname[52];
	char	remark[100];
	short	data_type;
} DATA_VAR_RELATION;

typedef struct _flag_var_relation
{
	char	model_id;
	char	name;
	char	type;
	char	logicalname;
	char	remark;
	char	data_type;
} FLAG_VAR_RELATION;

typedef struct _data_subject
{
	int		subject_id;
	char	name[80];
	int		task_id;
	short	step;
	short	type;
	char	datetime[22];
	char	judge_logic[500];
	char	pre_condition[500];
	char	interlock_name[30];
	char	subject_tip[100];
	short	time_limit;
	float	score;
	short	is_keystep;

	//foreign key
	char	task_name[50];
}DATA_SUBJECT;

typedef struct _flag_subject
{
	char	subject_id;
	char	name;
	char	task_id;
	char	step;
	char	type;
	char	datetime;
	char	judge_logic;
	char	pre_condition;
	char	interlock_name;
	char	subject_tip;
	char	time_limit;
	char	score;
	char	is_keystep;

	//foreign key
	char	task_name;
}FLAG_SUBJECT;

typedef struct _data_score
{
	int		user_id;
	int		host_id;
	int		send_task_id;
	int		task_id;
	int		subject_id;
	float	score;	
	int		step;//DZY ADDED 140604

	//foreign key
	char	userinfo_name[20];
	char	userinfo_longname[20];
	char	host_name[20];
	char	task_name[50];
	char	subject_name[50];
}DATA_SCORE;

typedef struct _flag_score
{
	char	user_id;
	char	host_id;
	char	send_task_id;
	char	task_id;
	char	subject_id;
	char	score;
		
	char	step;  //DZY ADDED 140604

	//foreign key
	char	userinfo_name;
	char	userinfo_longname;
	char	host_name;
	char	task_name;
	char	subject_name;
}FLAG_SCORE;

typedef struct _data_groups
{
	int		groups_id;
	char	name[60];
} DATA_GROUPS;

typedef struct _flag_groups
{
	char	groups_id;
	char	name;
} FLAG_GROUPS;

typedef struct _data_groups_record
{
	int		user_id;
	int		groups_id;
	char	chg_time[22];
	short	groups_role;

	//foreign key
	char	userinfo_name[20];
	char	userinfo_longname[20];
	char	groups_name[60];

	//for query conditon
	char	datatime[22];
	char	datatime_end[22];
} DATA_GROUPS_RECORD;

typedef struct _flag_groups_record
{
	char	user_id;
	char	groups_id;
	char	chg_time;
	char	groups_role;

	//foreign key
	char	userinfo_name;
	char	userinfo_longname;
	char	groups_name;

	//for query conditon
	char	datatime;
	char	datatime_end;

	//conditon opr symbol
	char	datetime_opr[OPR_SYMBOL_SIZE];
	char	datetime_end_opr[OPR_SYMBOL_SIZE];
} FLAG_GROUPS_RECORD;
//DZY 14 ADDED BEGAIN:
typedef struct _data_user_type
{
	int		user_type_id;
	char	user_type_name[60];
} DATA_USER_TYPE;

typedef struct _flag_user_type
{
	char	user_type_id;
	char	user_type_name;
} FLAG_USER_TYPE;

typedef struct _data_doc_type
{
	int		doctype_id;
	char	name[60];
	char	cope_name[60];//文档范围
} DATA_DOC_TYPE;

typedef struct _flag_doc_type
{
	char	doctype_id;
	char	name;
	char	cope_name;//文档范围
} FLAG_DOC_TYPE;

typedef struct _data_doc
{
	int		doc_id;
	int		doctype_id;
	char	name[60];
	int     doc_app;		//0:txt 1:word 2:ppt 3:mp3
	char	update_time[22];
	int		user_type_id;
	int		studytime;	//  minute
	int		study_points;
} DATA_DOC;

typedef struct _flag_data_doc
{
	char	doc_id;
	char	doctype_id;
	char	name;
	char  doc_app;		//0:txt 1:word 2:ppt 3:mp3
	char	update_time;
	char	user_type_id;
	char	studytime;	//  minute
	char	study_points;
} FLAG_DOC;

typedef struct _data_study_plan
{
	int		plan_id;
	char	name[60];
	int		user_type_id;
	int		plan_num;
	int		doc_id;
	int		task_id;
	int		task_score; 
	int		studytime;	//  minute
	int		study_points;
} DATA_STUDY_PLAN;

typedef struct _flag_data_study_plan
{
	char		plan_id;
	char		name;
	char		user_type_id;
	char		plan_num;
	char		doc_id;
	char		task_id;
	char		task_score;
	char		studytime;	//  minute
	char		study_points;
} FLAG_STUDY_PLAN;

typedef struct _data_study_record
{
	int		user_id;
	int		host_id;
	char	date_time[22];
	int		ms;
	int		type; // NOT NULL。0：文档；1：任务；2：其他
	int		plan_id;
	int		points;
	//foreign key
	char	host_name[20];			
	char	user_name[20];
	char	plan_name[60];
	//for query conditon
	char	datetime_end[22];
} DATA_STUDY_RECORD;

typedef struct _flag_data_study_record
{
	char	user_id;
	char		host_id;
	char	date_time;
	char		ms;
	char		type; // NOT NULL。0：文档；1：任务；2：其他
	char		plan_id;
	char		points;
	char	host_name;			
	char	user_name;
	char	plan_name;
	//for query conditon
	char	datetime_end;

	//conditon opr symbol
	char	datetime_opr[OPR_SYMBOL_SIZE];
	char	datetime_end_opr[OPR_SYMBOL_SIZE];
} FLAG_STUDY_RECORD;

typedef struct _data_contest
{
	int		contest_id;
	char	name[60];
	int		task_id;
	int		scene_id;
	int		fault_id;
	short open_state;	//竞赛是否开放: 0：不开放；1：开放
	char datetime[22];
	char	remark[100];
	//foreign key
	char	task_name[50];	
	char	scene_name[50];
	char	fault_name[50];
} DATA_CONTEST;

typedef struct _flag_data_contest
{
	char	contest_id;
	char	name;
	char	task_id;
	char	scene_id;
	char	fault_id;
	char open_state;	//竞赛是否开放: 0：不开放；1：开放
	char datetime;
	char	remark;
	//foreign key
	char	task_name;	
	char	scene_name;
	char	fault_name;
} FLAG_CONTEST;

typedef struct _data_send_contest
{
	int		send_contest_id;
	int		contest_id;
	int		group_id;
	char	send_time[22];
	char	start_time[22];
	char	end_time[22];
	short contest_state;	//竞赛执行状态: 0：未启动 1：已下发 2：已接受 3：完成 4：异常完成
	int		time_len;	
	float	score;
	float	tutor_score;	//教员给分
	unsigned int place_in_contest;
	//foreign key
	char	contest_name[50];
	//for query conditon
	char	start_time_start[22];
	char	start_time_end[22];
	char	end_time_start[22];
	char	end_time_end[22];
} DATA_SEND_CONTEST;

typedef struct _flag_data_send_contest
{
	char	send_contest_id;
	char	contest_id;
	char	group_id;
	char	send_time;
	char	start_time;
	char	end_time;
	char contest_state;	//竞赛执行状态: 0：未启动 1：已下发 2：已接受 3：完成 4：异常完成
	char	time_len;	
	char	score;
	char	tutor_score;	//教员给分
	char place_in_contest;
	//foreign key
	char	contest_name;
	//for query conditon
	char	start_time_start;
	char	start_time_end;
	char	end_time_start;
	char	end_time_end;
	//conditon opr symbol
	char	start_time_start_opr[OPR_SYMBOL_SIZE];
	char	start_time_end_opr[OPR_SYMBOL_SIZE];
	char	end_time_start_opr[OPR_SYMBOL_SIZE];
	char	end_time_end_opr[OPR_SYMBOL_SIZE];
} FLAG_SEND_CONTEST;

// dzy 14 added end
union db_flag
{     
	FLAG_USERINFO	flag_userinfo;
	FLAG_TRAIN_INFO	flag_train_info;
	FLAG_LOGIN_INFO	flag_login_info;
	FLAG_RECORD	flag_record;
	FLAG_SCENE	flag_scene;
	FLAG_SCENE_DATA	flag_scene_data;
	FLAG_REVERT	flag_revert;
	FLAG_FAULT	flag_fault;
	FLAG_FAULT_DATA	flag_fault_data;
	FLAG_TASK_GRP	flag_task_grp;
	FLAG_TASK	flag_task;
	FLAG_SEND_TASK	flag_send_task;
	FLAG_HOST	flag_host;
	FLAG_OPR_TYPE	flag_opr_type;
	FLAG_LOCAL_OPR	flag_local_opr;
	FLAG_RUN_MODEL	flag_run_model;
	FLAG_RUN_SEQUENCE	flag_run_sequence;
	FLAG_MODEL	flag_model;
	FLAG_SEQUENCE	flag_sequence;
	FLAG_VAR_RELATION	flag_var_relation;
	FLAG_SUBJECT	flag_subject;
	FLAG_SCORE		flag_score;
	FLAG_GROUPS	flag_groups;
	FLAG_GROUPS_RECORD	flag_groups_record;
	FLAG_USER_TYPE 	flag_user_type;
	FLAG_DOC_TYPE 	flag_doc_type;
	FLAG_DOC 	flag_doc;
	FLAG_STUDY_PLAN 	flag_study_plan;
	FLAG_STUDY_RECORD 	flag_study_record;
	FLAG_CONTEST			flag_contest;
	FLAG_SEND_CONTEST	flag_send_contest;
};    

union db_seg
{     
	DATA_USERINFO	seg_userinfo;
	DATA_TRAIN_INFO	seg_train_info;
	DATA_LOGIN_INFO	seg_login_info;
	DATA_RECORD	seg_record;
	DATA_SCENE	seg_scene;
	DATA_SCENE_DATA	seg_scene_data;
	DATA_REVERT	seg_revert;
	DATA_FAULT	seg_fault;
	DATA_FAULT_DATA	seg_fault_data;
	DATA_TASK_GRP	seg_task_grp;
	DATA_TASK	seg_task;
	DATA_SEND_TASK	seg_send_task;
	DATA_HOST	seg_host;
	DATA_OPR_TYPE	seg_opr_type;
	DATA_LOCAL_OPR	seg_local_opr;
	DATA_RUN_MODEL	seg_run_model;
	DATA_RUN_SEQUENCE	seg_run_sequence;
	DATA_MODEL	seg_model;
	DATA_SEQUENCE	seg_sequence;
	DATA_VAR_RELATION	seg_var_relation;
	DATA_SUBJECT	seg_subject;
	DATA_SCORE		seg_score;
	DATA_GROUPS	seg_groups;
	DATA_GROUPS_RECORD	seg_groups_record;
	DATA_USER_TYPE	seg_user_type;
	DATA_DOC_TYPE 	seg_doc_type;
	DATA_DOC 	seg_doc;
	DATA_STUDY_PLAN 	seg_study_plan;
	DATA_STUDY_RECORD 	seg_study_record;
	DATA_CONTEST	seg_contest;
	DATA_SEND_CONTEST	seg_send_contest;
};
 
union db_rec
{
	DATA_USERINFO	*p_userinfo[MAX_ROW_NUM];
	DATA_TRAIN_INFO	*p_train_info[MAX_ROW_NUM];
	DATA_LOGIN_INFO	*p_login_info[MAX_ROW_NUM_LARGE];
	DATA_RECORD		*p_record[MAX_ROW_NUM_LARGE];
	DATA_SCENE		*p_scene[MAX_ROW_NUM];
	DATA_SCENE_DATA	*p_scene_data[MAX_ROW_NUM_HUGE];
	DATA_REVERT		*p_revert[MAX_ROW_NUM]; 
	DATA_FAULT		*p_fault[MAX_ROW_NUM];
	DATA_FAULT_DATA	*p_fault_data[MAX_ROW_NUM];
	DATA_TASK_GRP	*p_task_grp[MAX_ROW_NUM];
	DATA_TASK		*p_task[MAX_ROW_NUM];
	DATA_SEND_TASK	*p_send_task[MAX_ROW_NUM];
	DATA_HOST		*p_host[MAX_ROW_NUM];
	DATA_OPR_TYPE	*p_opr_type[MAX_ROW_NUM];
	DATA_LOCAL_OPR	*p_local_opr[MAX_ROW_NUM];
	DATA_RUN_MODEL	*p_run_model[MAX_ROW_NUM];
	DATA_RUN_SEQUENCE	*p_run_sequence[MAX_ROW_NUM];
	DATA_MODEL		*p_model[MAX_ROW_NUM];
	DATA_SEQUENCE	*p_sequence[MAX_ROW_NUM];
	DATA_VAR_RELATION	*p_var_relation[MAX_ROW_NUM];
	DATA_SUBJECT	*p_subject[MAX_ROW_NUM];
	DATA_SCORE		*p_score[MAX_ROW_NUM];
	DATA_GROUPS	*p_groups[MAX_ROW_NUM];
	DATA_GROUPS_RECORD	*p_groups_record[MAX_ROW_NUM];
	DATA_USER_TYPE	*p_user_type[MAX_ROW_NUM];
	DATA_DOC_TYPE	*p_doc_type[MAX_ROW_NUM];
	DATA_DOC	*p_doc[MAX_ROW_NUM];
	DATA_STUDY_PLAN 	*p_study_plan[MAX_ROW_NUM];
	DATA_STUDY_RECORD	*p_study_record[MAX_ROW_NUM];
	DATA_CONTEST 	*p_contest[MAX_ROW_NUM];
	DATA_SEND_CONTEST	*p_send_contest[MAX_ROW_NUM];
} ;

typedef struct  _db_fmt
{
	char	opr_mode;					//operate mode
	char	table_id;					//table id
	char	addon_str[300];				//addon condition or sort string
	char	sql_str[2000];				//return sql str
	int		rownum;						//return effective record num
	int		ret_code;					//return code。 1: update record no found;
	char	sql_error[200];				//return sql error;
	db_flag flag;						//table field flag
	db_seg	seg;						//table field data
	//db_rec	rec;						//table data : record 
	db_flag cond_flag;					//table field condition flag
	db_seg  cond_seg;					//table field condition data
	int		rec_num;					//return all record num
} DB_FMT;


typedef struct  _data_station
{
	unsigned char station_id;
	char station_name[12];
	char station_longname[22];
	unsigned short dev_num;
} DATA_STATION;

typedef struct  _data_dev
{
	unsigned char station_id;
	unsigned char dev_id;
	char dev_type[10];
	char dev_name[12];
	char dev_longname[22];
	int pt_start;
	int pt_end;
} DATA_DEV;

typedef struct  _data_point
{
	int	id;
	char logicalname[80];
	unsigned char station_id;
	unsigned char dev_id;
	unsigned char type_id;
	unsigned short point_id;
	char longname[100];
	float data;
	short quality_code;
} DATA_POINT;

extern DATA_STATION	data_station[MAX_STN_NUM+1];
extern DATA_DEV	data_dev[MAX_STN_NUM+1][MAX_LCU_NUM+1];
extern DATA_POINT data_point[MAX_PT_NUM];


extern DATA_USERINFO	data_userinfo[MAX_ROW_NUM],g_userinfo[MAX_ROW_NUM];
extern DATA_TRAIN_INFO	data_train_info[MAX_ROW_NUM];
extern DATA_LOGIN_INFO	data_login_info[MAX_ROW_NUM_LARGE];
extern DATA_RECORD		data_record[MAX_ROW_NUM_LARGE];
extern DATA_SCENE		data_scene[MAX_ROW_NUM];
extern DATA_SCENE_DATA	data_scene_data[MAX_ROW_NUM_HUGE];
extern DATA_REVERT		data_revert[MAX_ROW_NUM];
extern DATA_FAULT		data_fault[MAX_ROW_NUM];
extern DATA_FAULT_DATA	data_fault_data[MAX_ROW_NUM];
extern DATA_TASK_GRP	data_task_grp[MAX_ROW_NUM];
extern DATA_TASK		data_task[MAX_ROW_NUM];
extern DATA_SEND_TASK	data_send_task[MAX_ROW_NUM];
extern DATA_HOST		data_host[MAX_ROW_NUM],g_host[MAX_OTS_HOST_NUM];
extern DATA_OPR_TYPE	data_opr_type[MAX_ROW_NUM];
extern DATA_LOCAL_OPR	data_local_opr[MAX_ROW_NUM];
extern DATA_RUN_MODEL	data_run_model[MAX_ROW_NUM];
extern DATA_RUN_SEQUENCE	data_run_sequence[MAX_ROW_NUM];
extern DATA_MODEL		data_model[MAX_ROW_NUM];
extern DATA_SEQUENCE	data_sequence[MAX_ROW_NUM];
extern DATA_VAR_RELATION	data_var_relation[MAX_ROW_NUM];
extern DATA_SUBJECT		data_subject[MAX_ROW_NUM];
extern DATA_SCORE		data_score[MAX_ROW_NUM];
extern DATA_GROUPS		data_groups[MAX_ROW_NUM];
extern DATA_GROUPS_RECORD	data_groups_record[MAX_ROW_NUM];
extern DATA_USER_TYPE	data_user_type[MAX_ROW_NUM];
extern DATA_DOC_TYPE	 data_doc_type[MAX_ROW_NUM];
extern DATA_DOC	data_doc[MAX_ROW_NUM];
extern DATA_STUDY_PLAN 	data_study_plan[MAX_ROW_NUM];
extern DATA_STUDY_RECORD	data_study_record[MAX_ROW_NUM];
extern DATA_CONTEST		data_contest[MAX_ROW_NUM];
extern DATA_SEND_CONTEST	data_send_contest[MAX_ROW_NUM];
extern DB_FMT			db_data;

extern char table_name[MAX_TABLE_NUM][TABLE_NAME_SIZE];
extern char field_name[MAX_FIELD_NUM][FIELD_NAME_SIZE];
extern char field_value[MAX_FIELD_NUM][VALUE_STR_SIZE];
extern char field_table[MAX_FIELD_NUM][TABLE_NAME_SIZE];
extern unsigned char field_type[MAX_FIELD_NUM];
extern char union_table[MAX_FIELD_NUM][TABLE_NAME_SIZE];
extern short field_id[MAX_FIELD_NUM];
extern char cond_name[MAX_COND_NUM][FIELD_NAME_SIZE];
extern char cond_value[MAX_COND_NUM][VALUE_STR_SIZE];
extern char cond_opr[MAX_COND_NUM][OPR_SYMBOL_SIZE];
extern char filter_name[MAX_FILTER_NUM][FIELD_NAME_SIZE];

extern QSqlDatabase db_conn;
extern int field_num,cond_num,row_id,table_num;
extern	int	g_host_num,g_user_num,g_host_id,g_user_id,g_groups_id,g_groups_role;
extern char g_host_name[20];

int		InsertOneRec(DB_FMT *p_data,QSqlDatabase db = db_conn);
int		UpdateOneRec(DB_FMT *p_data,QSqlDatabase db = db_conn);
int		DeleteOneRec(DB_FMT *p_data,QSqlDatabase db = db_conn);
int		SelectRecs(DB_FMT *p_data,QSqlDatabase db = db_conn);
int		SelectRecsFuzzy(DB_FMT *p_data,QSqlDatabase db = db_conn);//dzy 模糊搜索
void	InitDbData(DB_FMT *p_data,unsigned char table_id);
void	FreeDbData(DB_FMT *p_data,unsigned char table_id);
void	InitTableName();
void	InitTableData(char table_id);

int		ConnectHDB(QSqlDatabase db = db_conn,const QString &connectionName = QLatin1String("defaultConnection"));
int		CloseHDB(QSqlDatabase db = db_conn);
int		ExecSql(char *sql_str,QSqlDatabase db = db_conn);
int		ExecSqlQStr(const QString &sql_str,QSqlDatabase db = db_conn);
/////////////////CONVERT fielddata//////////////////////////
int		CvtFieldData(DB_FMT *p_data,int mode);
int		cvt_userinfo(DB_FMT *p_data,int mode);
int		cvt_train_info(DB_FMT *p_data,int mode);
int		cvt_login_info(DB_FMT *p_data,int mode);
int		cvt_record(DB_FMT *p_data,int mode);
int		cvt_scene(DB_FMT *p_data,int mode);
int		cvt_scene_data(DB_FMT *p_data,int mode);
int		cvt_revert(DB_FMT *p_data,int mode);
int		cvt_fault(DB_FMT *p_data,int mode);
int		cvt_fault_data(DB_FMT *p_data,int mode);
int		cvt_task_grp(DB_FMT *p_data,int mode);
int		cvt_task(DB_FMT *p_data,int mode);
int		cvt_send_task(DB_FMT *p_data,int mode);
int		cvt_host(DB_FMT *p_data,int mode);
int		cvt_opr_type(DB_FMT *p_data,int mode);
int		cvt_local_opr(DB_FMT *p_data,int mode);
int		cvt_run_model(DB_FMT *p_data,int mode);
int		cvt_run_sequence(DB_FMT *p_data,int mode);
int		cvt_model(DB_FMT *p_data,int mode);
int		cvt_sequence(DB_FMT *p_data,int mode);
int		cvt_var_relation(DB_FMT *p_data,int mode);
int		cvt_subject(DB_FMT *p_data,int mode);
int		cvt_score(DB_FMT *p_data,int mode);
int		cvt_groups(DB_FMT *p_data,int mode);
int		cvt_groups_record(DB_FMT *p_data,int mode);
//DZY 14 ADDED FOR HN
int		cvt_user_type(DB_FMT *p_data,int mode);
int		cvt_doc_type(DB_FMT *p_data,int mode);
int		cvt_doc(DB_FMT *p_data,int mode);
int		cvt_study_plan(DB_FMT *p_data,int mode);
int		cvt_study_record(DB_FMT *p_data,int mode);
int		cvt_contest(DB_FMT *p_data,int mode);
int		cvt_send_contest(DB_FMT *p_data,int mode);
/////////////////CONVERT CONDITION//////////////////////////
int		cvt_userinfo_cond(DB_FMT *p_data,int mode);
int		cvt_train_info_cond(DB_FMT *p_data,int mode);
int		cvt_login_info_cond(DB_FMT *p_data,int mode);
int		cvt_record_cond(DB_FMT *p_data,int mode);
int		cvt_scene_cond(DB_FMT *p_data,int mode);
int		cvt_scene_data_cond(DB_FMT *p_data,int mode);
int		cvt_revert_cond(DB_FMT *p_data,int mode);
int		cvt_fault_cond(DB_FMT *p_data,int mode);
int		cvt_fault_data_cond(DB_FMT *p_data,int mode);
int		cvt_task_grp_cond(DB_FMT *p_data,int mode);
int		cvt_task_cond(DB_FMT *p_data,int mode);
int		cvt_send_task_cond(DB_FMT *p_data,int mode);
int		cvt_host_cond(DB_FMT *p_data,int mode);
int		cvt_opr_type_cond(DB_FMT *p_data,int mode);
int		cvt_local_opr_cond(DB_FMT *p_data,int mode);
int		cvt_run_model_cond(DB_FMT *p_data,int mode);
int		cvt_run_sequence_cond(DB_FMT *p_data,int mode);
int		cvt_model_cond(DB_FMT *p_data,int mode);
int		cvt_sequence_cond(DB_FMT *p_data,int mode);
int		cvt_var_relation_cond(DB_FMT *p_data,int mode);
int		cvt_subject_cond(DB_FMT *p_data,int mode);
int		cvt_score_cond(DB_FMT *p_data,int mode);
int		cvt_groups_cond(DB_FMT *p_data,int mode);
int		cvt_groups_record_cond(DB_FMT *p_data,int mode);
int		cvt_user_type_cond(DB_FMT *p_data,int mode);
int		cvt_doc_type_cond(DB_FMT *p_data,int mode);
int		cvt_doc_cond(DB_FMT *p_data,int mode);
int		cvt_study_plan_cond(DB_FMT *p_data,int mode);
int		cvt_study_record_cond(DB_FMT *p_data,int mode);
int		cvt_contest_cond(DB_FMT *p_data,int mode);
int		cvt_send_contest_cond(DB_FMT *p_data,int mode);
void	GetCondStr();


int		GetHostNameById(int host_id, char *hostname);
int		GetHostIdByName(char *hostname);
int		InitOTSHdb();
int		SelectPtHdb(int mode);
int		GetHostTable();
int		GetUserInfoTable();

int		CvtQDateTimeToStr(char *str,QDateTime dtime);
int		HdbLogWrite(char *sql,char*sql_error,QSqlDatabase db = db_conn);

#endif
