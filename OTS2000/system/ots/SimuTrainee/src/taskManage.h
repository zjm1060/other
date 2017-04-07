#ifndef _TASKMANAGE_H
#define _TASKMANAGE_H

#include "ui_taskManage.h"
#include "calculation.h"
#include "SimuTip.h"
#include "/home/ems/system/ots/SimuTutor/src/excelobject.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
typedef struct _TASK_INFO
{
	int send_task_id;
	int task_id;
	QString task_name;
	int scene_id;
	QString scene_name;
	int fault_id;
	QString fault_name;
	int stn_id;
	QString stn_name;
	int dev_id;
	QString dev_name;
	QString task_tip;
	short type;
	char is_record;
	int time_len;
	QTime task_time;
	float score;
	QString send_time;
	QString start_time;
	QString end_time;
	short status;   /* 0 -- stop  1--run  2--pause */
	short task_status;/* 0 -- 任务未接收  1--任务已接收  2--任务已启动  3--任务已完成  4--任务非正常结束 */
	int task_grp_id;
	short task_startable;/* 0 -- 不可开始 1--可以开始 */
public:
	void operator =(_TASK_INFO TaskInfo)
	{
		send_task_id = TaskInfo.send_task_id;
		task_id = TaskInfo.task_id;
		task_name = TaskInfo.task_name;
		scene_id = TaskInfo.scene_id;
		scene_name = TaskInfo.scene_name;
		fault_id = TaskInfo.fault_id;
		fault_name = TaskInfo.fault_name;
		stn_id = TaskInfo.stn_id;
		stn_name = TaskInfo.stn_name;
		dev_id = TaskInfo.dev_id;
		dev_name = TaskInfo.dev_name;
		task_tip = TaskInfo.task_tip;
		type = TaskInfo.type;
		is_record = TaskInfo.is_record;
		time_len = TaskInfo.time_len;
		task_time = TaskInfo.task_time;
		score = TaskInfo.score;
		send_time = TaskInfo.send_time;
		start_time = TaskInfo.start_time;
		end_time = TaskInfo.end_time;
		status = TaskInfo.status;
		task_status = TaskInfo.task_status;
		task_grp_id = TaskInfo.task_grp_id;
		task_startable = TaskInfo.task_startable;
	}
}TASK_INFO;
Q_DECLARE_METATYPE(TASK_INFO);

class CalcScoreThread : public QThread
{
	Q_OBJECT
public:
	CalcScoreThread(CTaskCalculation *calc,QObject *parent=NULL){m_calc=calc;};
	void run();
private:
	QMutex mutex;
	CTaskCalculation *m_calc;
};

class CScore : public QDialog
{
	Q_OBJECT
public:
	CScore(QWidget *parent=0,int task_id=0,int send_task_id=0);
	~CScore();
	inline float getScore(){return score;};
	void setMinimumDuration(int ms){showTime = ms;};
	void loadChinese();
	void loadEnglish();
	void translateLanguage();

private Q_SLOTS:
	void showWaitDialog();
	void showResultDialog();
	void showDetailDialog();
	void timeOut();
protected:
	void paintEvent(QPaintEvent *event);
	void showEvent(QShowEvent *event);
Q_SIGNALS:
private:
	int m_task_id;
	int m_send_task_id;
	QSqlDatabase db_calc;
	float score;
	CalcScoreThread *calcScoreThread;
	CTaskCalculation *calc;
	QTimer *calcTimer;
	QTimer *showTimer;
	int showTime;
	QWidget *waitDialog;
	QWidget *resultDialog;
	QWidget *detailDialog;
	QLabel *labelWait;
	QPushButton *buttonWait;
	QPushButton *buttonDetail;
	QPushButton *buttonExit;
	void creatWaitDialog();
	void creatResultDialog();
	void creatDetailDialog();
};

class CTaskManage:public QWidget,public Ui_CTaskManage
{
	Q_OBJECT
public:
	CTaskManage(QWidget *parent=0);
	~CTaskManage();
	void init();
	bool isCurTaskRun();
	bool flagthread;
	int taskNum();
	void getCurTaskInfo(TASK_INFO *task_info);
	void clearCurTask();
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *event);
public Q_SLOTS:
	void taskStart();
	void taskReStart();
	void taskFinish(bool isUnusual);
	void addTask(TASK_INFO *task_info);
	void setCurTaskInfo(TASK_INFO *task_info);
	void setLanguage(int language);
private Q_SLOTS:
	void delTask();
	void clearTaskList();
	void setCurTask();
	void taskTimeCountDown();
	void timeOut();
	void saveTaskToExcel();
	void switchSimuTip(int state);
	void displayTaskRec(int);
	void PopupTaskRecDetail(const QModelIndex &index);
	void onexit();
Q_SIGNALS:
	void sigTaskTimeChanged(const QTime &);
	void sigTimeOut();
	void sigTaskStarted();
	void sigTaskFinished();
	void closedlg();
private:
	TASK_INFO cur_task_info;
	int task_num;
	QTimer *timerTaskTime;
	QItemSelectionModel *selectionModel;
	CScore *scoreDialog;
};
// class CTaskManageWidget:public CTaskManage
// {
// 	Q_OBJECT
// public:
// 	CTaskManageWidget();
// 	~CTaskManageWidget();
// };
class CNewTaskTip:public QWidget
{
	Q_OBJECT
public:
	CNewTaskTip(QWidget *parent=0);
	~CNewTaskTip();
	void setTaskInfo(TASK_INFO *task_info);
	void loadChinese();
	void loadEnglish();
private:
	QGridLayout *mainLayout;
	QLabel *labelTaskName,*labelTaskTime,*labelTitle,*labelTip;
	QPushButton *buttonSetCurTask,*buttonAddTaskList,*buttonIgnoreTask;
	TASK_INFO m_task_info;
	QTimer *timer;
	bool flagTitle;
	void setupUi();
private Q_SLOTS:
	void setCurTask();
	void addTaskList();
	void ignoreTask();
	void updateTitle();
public slots:
signals:
	void sig_taskaccepted();
protected:
	void showEvent(QShowEvent *event);
	void paintEvent(QPaintEvent *event);
	void closeEvent(QCloseEvent *event);
};

extern CTaskManage *taskManage;

#endif