#ifndef QCOMMON_H
#define QCOMMON_H
#pragma  once
#include <QString>
#include <QUdpSocket>
#include <QList>
#include <QAbstractItemView>
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 

#define OTS_UDP_PORT		2550
#define HEART_BEAT_PORT		3550
#define HEATBEAT_RATE		1000
#define MaxRevertTime		300 //最长再现时间
#define MAX_TASK_NUM		5 //最大接收任务数
#define MAX_PRIV_MODE_NUM	5 //最大的权限模式数
#define SimuTraineeConfigPath	"/home/ems/h9000/ots/SimuTraineeConfig.ini"
#define SimuTutorConfigPath	"/home/ems/h9000/ots/SimuTutorConfig.ini"

#define ENUM_CLASS_DECLARE(cls_type, enum_type)  \
			public: enum_type  m_val;  \
			public: \
			cls_type ( const enum_type& s): m_val(s) {} ;  \
			bool operator == ( const enum_type& cs ) {   \
			if ( m_val == m_val ) return true; \
			  else return false;  }    \
			  operator enum_type() { return  m_val; };


enum    USER_ROLE  
{ 
	ROLE_STUDENT = 0,
	ROLE_TEACHER = 1,
	ROLE_SYSTEM = 2
};

enum    USER_GROUPS_ROLE  
{ 
	GROUPS_ROLE_MEMBER = 0,
	GROUPS_ROLE_LEADER = 1,
};
enum	OTS_UDP_BUFF_TYPE
{
	TYPE_OTS_MSG
};
class TeacherPrivType {
public:
	enum _TeacherPrivType {
		PRIV_USER_INFO,
		PRIV_LOGIN_REC,
		PRIV_SCORE,
		PRIV_PRIV_MANAGER,
		PRIV_SET_TASK,
		PRIV_SET_SCENE,
		PRIV_SET_FAULT,
		PRIV_LOGIN_USER,
		PRIV_SEND_MESSAGE,
		PRIV_MODEL,
		PRIV_SEQUENCE,
		PRIV_FAULT,
		PRIV_TASK,
		PRIV_SCENE,
		PRIV_REVERT,
		PRIV_LOCAL_OPR,
		PRIV_RECORD,
		PRIV_EXAM,
		PRIV_HOST,
		PRIV_SCENE_RESET,
		PRIV_SAVE_SCENE,
		PRIV_CALL_DATA,
		PRIV_SET_POINT_MASK,
		PRIV_TYPE_NUM
	};
	ENUM_CLASS_DECLARE(TeacherPrivType, _TeacherPrivType)
};
class StudentPrivType {
public:
	enum _StudentPrivType {
		PRIV_SIMU_START,
		PRIV_SIMU_STOP,
		PRIV_SIMU_SPEED,
		PRIV_SET_TASK,
		PRIV_SET_SCENE,
		PRIV_SET_FAULT,
		PRIV_LOCAL_OPR,
		PRIV_RECORD,
		PRIV_REVERT,
		PRIV_EXAM,
		PRIV_SEND_MESSAGE,
		PRIV_SIMU_TIP,
		PRIV_TASK_RESTART,
		PRIV_TYPE_NUM
	};
	ENUM_CLASS_DECLARE(StudentPrivType, _StudentPrivType)
};

enum priv
{
	priv_userinfo,
	priv_loginrec,
	priv_score,
	priv_privmgr,
	priv_settask,
	priv_loginuser,
	priv_traininfo,
	priv_sendmsg,
	priv_model,
	priv_sequence,
	priv_fault,
	priv_task,
	priv_scene,
	priv_revert,
	priv_localopr,
	priv_record,
	priv_exam,
	priv_host,
	priv_scenereset,
	priv_savescene,
	priv_calldata,
	PRIV_NUM
};
typedef struct {
	 int task_id;
	 char datetime[22];
	 char starttime[22];
	 QString sTname;
	 QString sTdate;
	 QString sTtip;
	 QString sTstate;
	 QString sRstate;//运行状态
	 QString sStaName;//厂站名
	 QString sDivName;//设备名
	 QString sFaultName;//故障名
	 QString sSceneName;//工况名
	 int     send_task_id;
	 int     taskTlimit;//时间限定
	 bool	 is_record;
	 int	iStationID;
	 int	iDiviceID;
}TaskInfo;

struct SYSINFO{
	int qss_style;
	int simu_style;
	int desp_mode;
//	int run_mode;
};
typedef SYSINFO SYSINFO;

typedef struct
{
	USHORT	length;
	QString	src_ip;
	QString	dest_ip;
	USHORT	src_user_id;
	USHORT	dest_user_id;
	USHORT	src_host_id;
	USHORT	dest_host_id;
	UCHAR	type;
}OTS_UDP_HEAD;

typedef struct 
{
	OTS_UDP_HEAD	head;
	QString			send_user_name;
	QString			send_host_name;
	QString			send_time;
	QFont			font;
	QColor			color;
	QString			content;
}OTS_MSG;

typedef struct
{
	int user_id;
	int host_id;
	char login_time[22];
	int timeout;
}USER_INFO;

extern char *PrivName[];
extern UINT	priv_mask[MAX_USER_NUM][MAX_PRIV_MODE_NUM];
extern QMap<int,QString> PrivTeacher,PrivStudent;
extern QStandardItemModel *trainInfoModel;
extern USER_INFO heart_beat_user_info;
extern QTimer *timerButtonChat;
extern int login_user_id;
/*class WaitOrLoadDialog : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString labelText READ labelText WRITE setLabelText)
	public:
		explicit WaitOrLoadDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		WaitOrLoadDialog(const QString &labelText, QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~WaitOrLoadDialog();
		void setLabel(QLabel *label);
		void setGif();
		QString labelText() const;
	private:
	public Q_SLOTS:
		void setLabelText(const QString &text);
};*/

QMessageBox::StandardButton	MaybeSave(QWidget *widget);
QString	getAppParentDir();
QString	getAppDir();
void	InitPrivilege();
bool	CheckUserPrivilege(int user_id,int priv_type,int mode=0);
int		JudgeUserRole(int user_id,short role);
int		JudgeUserGroupsRole(int user_id,short groups_role);
int		JudgeLoginState(int user_id);
int		GetLocalOprIdByName(const QString &name);
int		GetUserIdFromLongName(char *longname);
int		GetUserGroupsId(int user_id);
int		GetUserIpAddr(int user_id,char *ip_a,char *ip_b);
int		GetUserHostId(int user_id);
int		GetUserLongName(int user_id,char *longName);
int		GetUserPrivilege(int user_id,char *privilege);
int		GetHostName(int host_id,char *host_name);
int		IsUserExist(const QString &user_name);
void	PopupMessageBox(QWidget *parent,char msg[],int type);
void	HeartBeat();
void	UpdateTrainInfoModel();
void	GetTimeFromSecs(int i,int *h,int *m,int *s);
void	moveToCenter(QWidget *widget);


#endif