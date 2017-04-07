#ifndef _CALCULATION_H
#define _CALCULATION_H

#include <QString>
#include <QStringList>
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "Login.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>

#define SUBJECT_TYPE_ALARM	0   //报警信息
#define	SUBJECT_TYPE_CMD	1	//命令
#define	SUBJECT_TYPE_OPR	2	//现地操作
#define SUBJECT_TYPE_CHECK	3	//设备检查

#define SUBJECT_STATUS_FINISH	0
#define SUBJECT_STATUS_UNFINISH	1
#define SUBJECT_STATUS_UNJUDGE	2

typedef struct
{
	DATA_SUBJECT data_subject;
	int status;
}SUBJECT_INFO;

typedef struct _RECORD_INFO
{
	QDateTime	datetime;
	short	type;
	char	station_id;	
	char	dev_id;	
	int		task_id;
	int		send_task_id;
	QString	logicalname;
	short	type_id;
	short 	status;
	float	fvalue;
public:
	void operator =(const _RECORD_INFO &RecordInfo)
	{
		datetime = RecordInfo.datetime;
		type = RecordInfo.type;
		station_id = RecordInfo.station_id;
		dev_id = RecordInfo.dev_id;
		task_id = RecordInfo.task_id;
		send_task_id = RecordInfo.send_task_id;
		logicalname = RecordInfo.logicalname;
		type_id = RecordInfo.type_id;
		status = RecordInfo.status;
		fvalue = RecordInfo.fvalue;
	}
	bool operator ==(const _RECORD_INFO &RecordInfo)
	{
		return (datetime == RecordInfo.datetime &&
				type == RecordInfo.type &&
				station_id == RecordInfo.station_id &&
				dev_id == RecordInfo.dev_id &&
				task_id == RecordInfo.task_id &&
				send_task_id == RecordInfo.send_task_id &&
				logicalname == RecordInfo.logicalname &&
				type_id == RecordInfo.type_id &&
				status == RecordInfo.status &&
				fvalue == RecordInfo.fvalue);
	}
}RECORD_INFO;
Q_DECLARE_METATYPE(SUBJECT_INFO);
Q_DECLARE_METATYPE(RECORD_INFO);
class CTaskCalculation : public QObject
{
	Q_OBJECT
public:
	CTaskCalculation(int task_id = 0,int send_task_id = 0,QSqlDatabase db = db_conn);
	~CTaskCalculation();
	float getTaskScore(){return score;};
	int getStepNum(){return mapSubjectInfo.count();};
	int getStepStatus(int step){return mapSubjectInfo[step].status;};
	DATA_SUBJECT *getStepSubjectInfo(int step){return &mapSubjectInfo[step].data_subject;};
	void setTaskId(int task_id){m_task_id = task_id;};
	void setSendTaskId(int send_task_id){m_send_task_id = send_task_id;};
	void setSqlDatabase(QSqlDatabase db){m_db = db;};
	int getTaskId(){return m_task_id;};
	int getSendTaskId(){return m_send_task_id;};
	bool judgeStepFinished(int step);
	bool judgeStepPreConditionFinished(int step);
	int updateTaskRecordToHash();
	int updateTaskSubjectToMap(); 
	int calcTaskScore();
private:
	int m_task_id;
	int m_send_task_id;
	QSqlDatabase m_db;
	float score;
	int curStep;
	QMultiHash<int,RECORD_INFO> usedRecordInfo;
	QMultiHash<QString,RECORD_INFO> hashRecordInfo;
	QMap<int,SUBJECT_INFO> mapSubjectInfo;
	bool isRecordUsed(RECORD_INFO *);
	int math(int n1,int n2,QChar op);
	int precedence(QChar op);
	int judgeCondExp(const QString &cond_exp);
	int getValueByRpn(const QString &rpn,int *value);
	int chgExpToRpn(const QString &exp,QString &rpn);
	void splitSubject(const QString &str_subject,QStringList &list_subject);
	int getAlarmPointResultFromRecord(const QString &pointName,short *type_id,short *status,float *fValue);
	int getCmdPointResultFromRecord(const QString &pointName,short *type_id,short *status,float *fValue);
	int getLocalOprFromRecord(const QString &opr_id,int *isExist);
	int getPtCheckFromRecord(const QString &pointName,short *status);
	void writeScoreToHdb(int step,float score);
};

#endif