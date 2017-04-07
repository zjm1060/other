#include "calculation.h"

CTaskCalculation::CTaskCalculation(int task_id,int send_task_id,QSqlDatabase db)
{
	m_task_id = task_id;
	m_send_task_id = send_task_id;
	m_db = db;
	score = 0;
	curStep = 0;
	stop_flag = false;
	mapSubjectInfo.clear();
	hashRecordInfo.clear();
	usedRecordInfo.clear();
	//dzy
	timer_overtime = new QTimer(this);
	//connect(timer_overtime, SIGNAL(timeout()), this, SLOT(steptimeout()));
}
CTaskCalculation::~CTaskCalculation()
{
	stop_flag = true;
	timer_overtime->stop();
	mapSubjectInfo.clear();
	hashRecordInfo.clear();
	usedRecordInfo.clear();
}
int CTaskCalculation::updateTaskSubjectToMap()
{
	int num = 0;
	mapSubjectInfo.clear();
	if (stop_flag)
	{
		return 0;
	}
	QSqlQuery query(QString("select * from ots.subject where task_id = %1 order by step").arg(m_task_id),m_db);
// 	QSqlDatabase *tempdb;
// 	tempdb = new QSqlDatabase;//dzy 
// 
// 	if (ConnectHDB(*tempdb,"tempdb1"))
// 	{
// 		QSqlQuery query(QString("select * from ots.subject where task_id = %1 order by step").arg(m_task_id),*tempdb);
// 		//dzy 0706
// 		if (!query.isActive())
// 		{
// 			return 0;
// 		}
		int fieldNo1 = query.record().indexOf("subject_id");
		int fieldNo2 = query.record().indexOf("name");
		int fieldNo3 = query.record().indexOf("task_id");
		int fieldNo4 = query.record().indexOf("step");
		int fieldNo5 = query.record().indexOf("type");
		int fieldNo6 = query.record().indexOf("datetime");
		int fieldNo7 = query.record().indexOf("judge_logic");
		int fieldNo8 = query.record().indexOf("interlock_name");
		int fieldNo9 = query.record().indexOf("subject_tip");
		int fieldNo10 = query.record().indexOf("time_limit");
		int fieldNo11 = query.record().indexOf("score");
		int fieldNo12 = query.record().indexOf("pre_condition");
		int fieldNo13 = query.record().indexOf("is_keystep");

		while (query.next()&&!stop_flag) //dzy add &&!stop_flag
		{
			SUBJECT_INFO subject_info;
			subject_info.data_subject.subject_id = query.value(fieldNo1).toInt();
			qstrcpy(subject_info.data_subject.name,query.value(fieldNo2).toString().toLocal8Bit().data());
			CvtArrayToStr(subject_info.data_subject.name,strlen(subject_info.data_subject.name));
			subject_info.data_subject.task_id = query.value(fieldNo3).toInt();
			subject_info.data_subject.step = query.value(fieldNo4).toInt();
			subject_info.data_subject.type = query.value(fieldNo5).toInt();
			qstrcpy(subject_info.data_subject.datetime,query.value(fieldNo6).toDateTime().toString("yyyy-MM-dd HH:mm:ss").toLocal8Bit().data());
			qstrcpy(subject_info.data_subject.judge_logic,query.value(fieldNo7).toString().toLocal8Bit().data());
			CvtArrayToStr(subject_info.data_subject.judge_logic,strlen(subject_info.data_subject.judge_logic));
			qstrcpy(subject_info.data_subject.interlock_name,query.value(fieldNo8).toString().toLocal8Bit().data());
			CvtArrayToStr(subject_info.data_subject.interlock_name,strlen(subject_info.data_subject.interlock_name));
			qstrcpy(subject_info.data_subject.subject_tip,query.value(fieldNo9).toString().toLocal8Bit().data());
			CvtArrayToStr(subject_info.data_subject.subject_tip,strlen(subject_info.data_subject.subject_tip));
			subject_info.data_subject.time_limit = query.value(fieldNo10).toInt();
			subject_info.data_subject.score = query.value(fieldNo11).toDouble();
			qstrcpy(subject_info.data_subject.pre_condition,query.value(fieldNo12).toString().toLocal8Bit().data());
			CvtArrayToStr(subject_info.data_subject.pre_condition,strlen(subject_info.data_subject.pre_condition));
			subject_info.data_subject.is_keystep = query.value(fieldNo13).toInt();
			subject_info.status = SUBJECT_STATUS_UNJUDGE;
			mapSubjectInfo.insert(subject_info.data_subject.step,subject_info);
			num++;
		}
		return num;
// 	}
// 	else
// 		return 0;

}
int CTaskCalculation::updateTaskRecordToHash()
{
	int num = 0;
	hashRecordInfo.clear();
	if (stop_flag)
	{
		return 0;
	}
	QSqlQuery query(QString("select * from ots.record where send_task_id = %1 order by datetime").arg(m_send_task_id),m_db);
// 	QSqlDatabase *tempdb;
// 	tempdb = new QSqlDatabase;//dzy 
// 	if (ConnectHDB(*tempdb,"tempdb2"))
// 	{
	//	QSqlQuery query(QString("select * from ots.record where send_task_id = %1 order by datetime").arg(m_send_task_id),*tempdb);

		//dzy 0706
// 		if (!query.isActive())
// 		{
// 			return 0;
// 		}
		int fieldNo1 = query.record().indexOf("user_id");
		int fieldNo2 = query.record().indexOf("host_id");
		int fieldNo3 = query.record().indexOf("groups_id");
		int fieldNo4 = query.record().indexOf("datetime");
		int fieldNo5 = query.record().indexOf("ms");
		int fieldNo6 = query.record().indexOf("type");
		int fieldNo7 = query.record().indexOf("content");
		int fieldNo8 = query.record().indexOf("logicalname");
		int fieldNo9 = query.record().indexOf("type_id");
		int fieldNo10 = query.record().indexOf("status");
		int fieldNo11 = query.record().indexOf("fvalue");
		int fieldNo12 = query.record().indexOf("station_id");
		int fieldNo13 = query.record().indexOf("dev_id");
		int fieldNo14 = query.record().indexOf("task_id");
		int fieldNo15 = query.record().indexOf("send_task_id");

		while (query.next()&&!stop_flag) //dzy add &&!stop_flag
		{
			RECORD_INFO record_info;
			record_info.datetime = query.value(fieldNo4).toDateTime();
			record_info.datetime.addMSecs(query.value(fieldNo5).toInt());
			record_info.type = query.value(fieldNo6).toInt();
			record_info.logicalname = query.value(fieldNo8).toString().trimmed();
			record_info.type_id = query.value(fieldNo9).toInt();
			record_info.status = query.value(fieldNo10).toInt();
			record_info.fvalue = query.value(fieldNo11).toDouble();
			record_info.station_id = query.value(fieldNo12).toInt();
			record_info.dev_id = query.value(fieldNo13).toInt();
			record_info.task_id = query.value(fieldNo14).toInt();
			record_info.send_task_id = query.value(fieldNo15).toInt();
			if (!isRecordUsed(&record_info))
			{
				hashRecordInfo.insertMulti(QString(record_info.logicalname),record_info);
			}
			num++;
		}
		return num;
//	}
// 	else
// 		return 0;
}
bool CTaskCalculation::isRecordUsed(RECORD_INFO *record_info)
{
	for (int step=1;step<curStep;step++)
	{
		if (usedRecordInfo.values(step).contains(*record_info))
		{
			return true;
		}
	}
	return false;
}
bool CTaskCalculation::isPointInUsedinfo(QString pointname)
{
	for (int step=1;step<curStep;step++)
	{
		if (usedRecordInfo.take(step).logicalname==pointname)
		{
			return true;
		}
	}
	return false;
}
int CTaskCalculation::getAlarmPointResultFromRecord(const QString &pointName,short *type_id,short *status,float *fValue,int mode)
{
	if(mode == 0)
	{
		if (!hashRecordInfo.contains(pointName))
			return FALSE;
		QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(pointName);
		bool bIsExist = false;
		RECORD_INFO record_info;
		QDateTime datetime;
		while (i != hashRecordInfo.end() && i.key() == pointName)
		{
			if (i.value().type == 1)
			{
				if (!bIsExist)
				{
					datetime = i.value().datetime;
					record_info = i.value();
					bIsExist = true;
				}
				if (i.value().datetime < datetime)
				{
					datetime = i.value().datetime;
					record_info = i.value();
				}
			}
			++i;
		}
		if (bIsExist)
		{
			*type_id = record_info.type_id;
			*status = record_info.status;
			*fValue = record_info.fvalue;
			hashRecordInfo.remove(pointName,record_info);
			usedRecordInfo.insertMulti(curStep,record_info);
			usedRecordforPreSubjectInfo.insertMulti(pointName,record_info);//dzy new added
			curKeystep.datetime = record_info.datetime;
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(mode == 1)
	{
		if (!hashRecordInfo.contains(pointName)&&!usedRecordforPreSubjectInfo.contains(pointName))
			return FALSE;
		if (usedRecordforPreSubjectInfo.contains(pointName))
		{
			QHash<QString,RECORD_INFO>::iterator i = usedRecordforPreSubjectInfo.find(pointName);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != usedRecordforPreSubjectInfo.end() && i.key() == pointName)
			{
				if (i.value().type == 1)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime > datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return FALSE;		
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*type_id = record_info.type_id;
				*status = record_info.status;
				*fValue = record_info.fvalue;
				return TRUE;						
			}
			else				
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				return FALSE;
			}
		}
		if (hashRecordInfo.contains(pointName))
		{
			QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(pointName);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != hashRecordInfo.end() && i.key() == pointName)
			{
				if (i.value().type == 1)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime < datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return FALSE;		
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*type_id = record_info.type_id;
				*status = record_info.status;
				*fValue = record_info.fvalue;
				return TRUE;						
			}
			else				
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				return FALSE;
			}
		}
	}
}
////////////NEEEEEED TO BE MODIFIED 实时库只保存了最后状态 
int CTaskCalculation::getAlamPointfromDMS(const QString &pointName,short *type_id,short *status,float *fValue)
{
	DMS_COMMON common_aa;
	char		pt_name[POINT_NAME_SIZE];
	strcpy(pt_name,pointName.toLocal8Bit().data());
	POINTER		point;
	GetPtIdFromStr(pt_name, &point);
	ReadEntryById( &point, &common_aa );
	/**type_id = 1;*/
	switch	( point.data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		/**status =*/ di_rt_data = common_aa.var.ind.status.rt_state;
		break;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		/**fValue = */ai_rt_data = common_aa.var.anlg.fValue;
		break;

	default:
		printf("\nunsupported type: %d", point.data_type);
	}
	return 0;
}
int CTaskCalculation::getCmdPointResultFromRecord(const QString &pointName,short *type_id,short *status,float *fValue,int mode)
{
	if (mode == 0)
	{
		if (!hashRecordInfo.contains(pointName))
			return FALSE;
		QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(pointName);
		bool bIsExist = false;
		RECORD_INFO record_info;
		QDateTime datetime;
		while (i != hashRecordInfo.end() && i.key() == pointName)
		{
			if (i.value().type == 0)
			{
				if (!bIsExist)
				{
					datetime = i.value().datetime;
					record_info = i.value();
					bIsExist = true;
				}
				if (i.value().datetime < datetime)
				{
					datetime = i.value().datetime;
					record_info = i.value();
				}
			}
			++i;
		}
		if (bIsExist)
		{
			*type_id = record_info.type_id;
			*status = record_info.status;
			*fValue = record_info.fvalue;
			hashRecordInfo.remove(pointName,record_info);
			usedRecordInfo.insertMulti(curStep,record_info);
			usedRecordforPreSubjectInfo.insertMulti(pointName,record_info);//dzy new added
			curKeystep.datetime = record_info.datetime;
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(mode == 1)
	{
		if (!hashRecordInfo.contains(pointName)&&!usedRecordforPreSubjectInfo.contains(pointName))
			return FALSE;

		if (usedRecordforPreSubjectInfo.contains(pointName))
		{
			QHash<QString,RECORD_INFO>::iterator i = usedRecordforPreSubjectInfo.find(pointName);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != usedRecordforPreSubjectInfo.end() && i.key() == pointName)
			{
				if (i.value().type == 0)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime > datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return  FALSE;		
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*type_id = record_info.type_id;
				*status = record_info.status;
				*fValue = record_info.fvalue;
				return TRUE;
			}
			else
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				return FALSE;		
			}
		}

		if (hashRecordInfo.contains(pointName))
		{
			QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(pointName);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != hashRecordInfo.end() && i.key() == pointName)
			{
				if (i.value().type == 0)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime < datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return FALSE;		
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*type_id = record_info.type_id;
				*status = record_info.status;
				*fValue = record_info.fvalue;
				return TRUE;
			}
			else
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				return FALSE;		
			}
		}
	}
}
int CTaskCalculation::getLocalOprFromRecord(const QString &opr_id,int *isExist,int mode)
{
	if (mode == 0)
	{
		if (!hashRecordInfo.contains(opr_id))
			return FALSE;
		QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(opr_id);
		bool bIsExist = false;
		RECORD_INFO record_info;
		QDateTime datetime;
		while (i != hashRecordInfo.end() && i.key() == opr_id)
		{
			if (i.value().type == 3)
			{
				if (!bIsExist)
				{
					datetime = i.value().datetime;
					record_info = i.value();
					bIsExist = true;
				}
				if (i.value().datetime < datetime)
				{
					datetime = i.value().datetime;
					record_info = i.value();
				}
			}
			++i;	
		}
		if (bIsExist)
		{
			*isExist = TRUE;
			hashRecordInfo.remove(opr_id,record_info);
			usedRecordInfo.insertMulti(curStep,record_info);
			usedRecordforPreSubjectInfo.insertMulti(opr_id,record_info);//dzy new added
			curKeystep.datetime = record_info.datetime;
			return TRUE;
		}
		else 
		{
			*isExist = FALSE;
			return FALSE;
		}
	}
	else if (mode == 1)
	{
		if (!hashRecordInfo.contains(opr_id)&&!usedRecordforPreSubjectInfo.contains(opr_id))
			return FALSE;

		if (usedRecordforPreSubjectInfo.contains(opr_id))
		{
			QHash<QString,RECORD_INFO>::iterator i = usedRecordforPreSubjectInfo.find(opr_id);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != usedRecordforPreSubjectInfo.end() && i.key() == opr_id)
			{
				if (i.value().type == 3)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime > datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;	
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return FALSE;
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*isExist = TRUE;
				return TRUE;
			}
			else 
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				*isExist = FALSE;
				return FALSE;
			}
		}

		if (hashRecordInfo.contains(opr_id))
		{
			QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(opr_id);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != hashRecordInfo.end() && i.key() == opr_id)
			{
				if (i.value().type == 3)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime < datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;	
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return FALSE;		
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*isExist = TRUE;
				return TRUE;
			}
			else 
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				*isExist = FALSE;
				return FALSE;
			}
		}
	}	
}
int CTaskCalculation::getPtCheckFromRecord(const QString &pointName,short *status,int mode)
{
	if (mode == 0)
	{
		if (!hashRecordInfo.contains(pointName))
			return FALSE;
		QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(pointName);
		bool bIsExist = false;
		RECORD_INFO record_info;
		QDateTime datetime;
		while (i != hashRecordInfo.end() && i.key() == pointName)
		{
			if (i.value().type == 4)
			{
				if (!bIsExist)
				{
					datetime = i.value().datetime;
					record_info = i.value();
					bIsExist = true;
				}
				if (i.value().datetime < datetime)
				{
					datetime = i.value().datetime;
					record_info = i.value();
				}
			}
			++i;
		}
		if (bIsExist)
		{
			*status = record_info.status;
			hashRecordInfo.remove(pointName,record_info);
			usedRecordInfo.insertMulti(curStep,record_info);
			usedRecordforPreSubjectInfo.insertMulti(pointName,record_info);//dzy new added
			curKeystep.datetime = record_info.datetime;
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (mode == 1)
	{
		if (!hashRecordInfo.contains(pointName)&&!usedRecordforPreSubjectInfo.contains(pointName))
			return FALSE;

		if (usedRecordforPreSubjectInfo.contains(pointName))
		{
			QHash<QString,RECORD_INFO>::iterator i = usedRecordforPreSubjectInfo.find(pointName);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != usedRecordforPreSubjectInfo.end() && i.key() == pointName)
			{
				if (i.value().type == 4)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime < datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return FALSE;
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*status = record_info.status;
				return TRUE;
			}
			else
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				return FALSE;
			}
		}
		if (hashRecordInfo.contains(pointName))
		{
			QHash<QString,RECORD_INFO>::iterator i = hashRecordInfo.find(pointName);
			bool bIsExist = false;
			RECORD_INFO record_info;
			QDateTime datetime;
			while (i != hashRecordInfo.end() && i.key() == pointName)
			{
				if (i.value().type == 4)
				{
					if (!bIsExist)
					{
						datetime = i.value().datetime;
						record_info = i.value();
						bIsExist = true;
					}
					if (i.value().datetime < datetime)
					{
						datetime = i.value().datetime;
						record_info = i.value();
					}
				}
				++i;
			}
			if (!bIsExist)
			{
				curKeystep.status = SUBJECT_STATUS_UNSATISFIYED;
				return FALSE;
			}
			if (bIsExist&&record_info.datetime<curKeystep.datetime)
			{
				*status = record_info.status;
				return TRUE;
			}
			else
			{
				curKeystep.status = SUBJECT_STATUS_DISORDER;
				return FALSE;
			}
		}
	}
}
//ALARM: PBG.F1.ANA.LCU_G_AIPW = 0      ALARM: PBG.F1.ANA.LCU_G_AIPW BETWEEN 0 AND 2
//CMD: PBG.F1.ANA.LCU_G_AIPW TYPE_ID:0 CMD_OPT:0
//CHECK: PBG.F1.ANA.LCU_G_AIPW NORMAL
//OPR: 1.1
int CTaskCalculation::judgeCondExp(const QString &cond_exp,int mode)
{
	int result;
	QStringList list_cond_exp,list_type_id,list_cmd_opt,list_opr;
	short status,type_id;
	float fValue;
	int isExist;
	ANLG_STATUS	ana_status;
	IND_DATA	ind_status;
	COMMAND_STATUS	cmd_status;	
	short subject_type;
	if (cond_exp.split(" ",QString::SkipEmptyParts).at(0) == "ALARM:")
	{
		subject_type = SUBJECT_TYPE_ALARM;
	}
	else if (cond_exp.split(" ",QString::SkipEmptyParts).at(0) == "CMD:")
	{
		subject_type = SUBJECT_TYPE_CMD;
	}
	else if (cond_exp.split(" ",QString::SkipEmptyParts).at(0) == "CHECK:")
	{
		subject_type = SUBJECT_TYPE_CHECK;
	}
	else if (cond_exp.split(" ",QString::SkipEmptyParts).at(0) == "OPR:")
	{
		subject_type = SUBJECT_TYPE_OPR;
	}
	switch(subject_type)
	{
		case SUBJECT_TYPE_ALARM:
			if (cond_exp.contains("="))
			{
				list_cond_exp = cond_exp.split(" ",QString::SkipEmptyParts);
				if (list_cond_exp.size() != 4)  return 0;
				if (getAlarmPointResultFromRecord(list_cond_exp.at(1),&type_id,&status,&fValue,mode) == FALSE)
				{
					printf("point %s is not found in record",list_cond_exp.at(1).toLocal8Bit().data());
					result = 0;
				}
				else
				{
		 			switch(type_id)
		 			{
						case	DPS_ANA_ALM:
						case	DPS_IMP_ALM:
							memcpy(&(ana_status),&(status),2);
							if (fValue == list_cond_exp.at(3).toFloat())
								result = 1;
							else
								result = 0;
							break;
						default:
							memcpy(&(ind_status),&(status),2);
							if (ind_status.rt_state == list_cond_exp.at(3).toShort())
								result = 1;
							else
								result = 0;
							break;
		 			}
				}
			}
			else if (cond_exp.contains(">"))
			{
				list_cond_exp = cond_exp.split(" ",QString::SkipEmptyParts);
				if (list_cond_exp.size() != 4)  return 0;
				if (getAlarmPointResultFromRecord(list_cond_exp.at(1),&type_id,&status,&fValue,mode) == FALSE)
				{
					printf("point %s is not found in record",list_cond_exp.at(1).toLocal8Bit().data());
					result = 0;
				}
				else
				{
					if (fValue > list_cond_exp.at(3).toFloat())
						result = 1;
					else
						result = 0;
				}
			}
			else if (cond_exp.contains("<"))
			{
				list_cond_exp = cond_exp.split(" ",QString::SkipEmptyParts);
				if (list_cond_exp.size() != 3)  return 0;
				if (getAlarmPointResultFromRecord(list_cond_exp.at(1),&type_id,&status,&fValue,mode) == FALSE)
				{
					printf("point %s is not found in record",list_cond_exp.at(1).toLocal8Bit().data());
					result = 0;
				}
				else
				{
					if (fValue < list_cond_exp.at(3).toFloat())
						result = 1;
					else
						result = 0;
				}
			}
			else 
			{
				list_cond_exp = cond_exp.split(" ",QString::SkipEmptyParts);
				if (list_cond_exp.size() != 6)  return 0;
				if (getAlarmPointResultFromRecord(list_cond_exp.at(1),&type_id,&status,&fValue,mode) == FALSE/*&&getAlamPointfromDMS(list_cond_exp.at(1),&type_id,&status,&fValue)==FALSE*/)
				{
					printf("point %s is not found in record",list_cond_exp.at(1).toLocal8Bit().data());
					result = 0;
				}
				else
				{
					if (fValue > list_cond_exp.at(3).toFloat() && fValue < list_cond_exp.at(5).toFloat())
						result = 1;
					else
						result = 0;
				}
			}
			break;
		case SUBJECT_TYPE_CMD:
			list_cond_exp = cond_exp.split(" ",QString::SkipEmptyParts);
			if (list_cond_exp.size() != 4)  return 0;
			if (getCmdPointResultFromRecord(list_cond_exp.at(1),&type_id,&status,&fValue,mode) == FALSE)
			{
				printf("point %s is not found in record",list_cond_exp.at(1).toLocal8Bit().data());
				result = 0;
			}
			else
			{
				memcpy(&(cmd_status),&(status),2);
				list_type_id = list_cond_exp.at(2).split(":",QString::SkipEmptyParts);
				list_cmd_opt = list_cond_exp.at(3).split(":",QString::SkipEmptyParts);
				if (list_type_id.at(1).toShort() == type_id && list_cmd_opt.at(1).toShort() == cmd_status.opr && cmd_status.result == 0)
					result = 1;
				else
					result = 0;
			}
			break;
		case SUBJECT_TYPE_OPR:
			list_cond_exp = cond_exp.split(" ",QString::SkipEmptyParts);
			if (list_cond_exp.size() != 2)  return 0;
			if (getLocalOprFromRecord(list_cond_exp.at(1),&isExist,mode) == FALSE)
			{
				printf("local_opr %s is not found in record",list_cond_exp.at(1).toLocal8Bit().data());
				result = 0;
			}
			else
			{
				if (isExist)
					result = 1;
				else
					result = 0;
			}
			break;
		case SUBJECT_TYPE_CHECK:
			QStringList listStatusStr;
			listStatusStr<<"NORMAL"<<"ANORMAL";
			list_cond_exp = cond_exp.split(" ",QString::SkipEmptyParts);
			if (list_cond_exp.size() != 3)  return 0;
			if (getPtCheckFromRecord(list_cond_exp.at(1),&status,mode) == FALSE)
			{
				printf("pt_check %s is not found in record",list_cond_exp.at(1).toLocal8Bit().data());
				//WriteRec("pt_chk not found rcd!");
				//PopupMessageBox(this,"!!!!!!",0);////DZY 0706
				result = 0;
			}
			else
			{
				if (status == 0 || status == 1)
				{
					if (listStatusStr.at(status) == list_cond_exp.at(2))
						result = 1;
					else
						result = 0;
				}
				else
					result = 0;
			}
			break;
	}
	
	return result;
}

int CTaskCalculation::precedence(QChar op)
{
	switch(op.toLatin1())
	{
		case '|':
			return 1;
		case '&':
			return 2;
		default:
			return 0;
	}
}

int CTaskCalculation::math(int n1,int n2,QChar op)
{
	switch(op.toLatin1())
	{
	case '&':
		return n1&&n2;
	case '|':
		return n1||n2;
	default:
		return 0;
	}
}

/***********************转换为逆波兰表达式******************************/
int CTaskCalculation::chgExpToRpn(const QString &exp,QString &rpn)
{
	QStack<QChar>		stackOperator;
	if(exp.isEmpty())  return FALSE;	
	for (int i=0;i<exp.size();i++)
	{
		QChar chr = exp.at(i);
		if(chr.isNumber())//如果是数字，添加到数字字符串中
		{
			rpn.append(chr);
		}     
		else if (chr == '&' || chr == '|')
		{
			while(!stackOperator.isEmpty())
			{
				QChar opInStack=(QChar)stackOperator.pop();
				if(precedence(opInStack)<precedence(chr))
				{
					stackOperator.push(opInStack);
					break;
				}
				else
				{
					rpn.append(opInStack);
				}
			}
			stackOperator.push(chr);//将当前操作符压入堆栈
		}
		else if(chr== '(')//遇到左括号，直接压入堆栈中
		{
			stackOperator.push(chr);
		}
		else if(chr == ')')//遇到右括号
		{
			while(!stackOperator.isEmpty())//弹出运算符并添加到逆波兰表达式，直至遇到左括号
			{
				QChar opInStack=(QChar)stackOperator.pop();
				if(opInStack == '(' )
				{
					break;
				}
				else
				{
					rpn.append(opInStack);
				}
			}            
		}
	}

	while(!stackOperator.isEmpty())
	{
		QChar opInStack=(QChar)stackOperator.pop();
		rpn.append(opInStack);
	}
#ifdef DEBUG
	printf("rpn subject str is %s",rpn.toLocal8Bit().data());
#endif
	return TRUE;
}
/*********************************计算逆波兰表达式的值********************************/
int CTaskCalculation::getValueByRpn(const QString &rpn,int *value)
{
	QStack<int>	stack;
	*value = 0;
	if (rpn.isEmpty())	return FALSE;
	for(int i=0;i<rpn.size();i++)
	{            
		QChar chr = rpn.at(i);
		if(chr.isNumber())
		{
			stack.push(QString(chr).toInt());
			*value = QString(chr).toInt(); //added by dm表达式只有一个值，则直接赋值
		}
		else if(chr=='&' || chr=='|')
		{
			int n2=stack.pop();
			int n1=stack.pop();
			*value=math(n1,n2,chr);
			stack.push(*value);
		}
	}
	return TRUE;
}

void CTaskCalculation::splitSubject(const QString &str_subject,QStringList &list_subject)
{
	QString	tmpStr;
	for (int i=0;i<str_subject.size();i++)
	{
		if (str_subject.at(i) != '&' && str_subject.at(i) != '|' 
			&& str_subject.at(i) != '(' && str_subject.at(i) != ')')
		{
			tmpStr.append(str_subject.at(i));
		}
		else
		{
			tmpStr = tmpStr.trimmed();
			if(tmpStr.isEmpty())
				list_subject << str_subject.at(i);
			else
				list_subject << tmpStr << str_subject.at(i);
			tmpStr.clear();
		}
	}
	tmpStr = tmpStr.trimmed();
	if (!tmpStr.isEmpty())
		list_subject << tmpStr;
}
bool CTaskCalculation::judgeStepFinished(int step,int m_mode)
{
	QStringList	list_subject;
	float	subjectScore = 0;
	QString	exp,rpn;
	int rpnValue;
	curStep = step;
	curstep_timeout = false;//dzy 140723
	if (!mapSubjectInfo.contains(step))
		return false;

	//dzymodified  步骤提示增加curstep_timeout
	if(m_mode ==1)
	{
		while (!curstep_timeout)
		{
			if (QString::fromLocal8Bit(mapSubjectInfo[step].data_subject.judge_logic).isEmpty())//add by dm 2012-12-15 如果judge_logic为空则判断为成功
			{
				curKeystep.datetime.currentDateTime();
				return true;
			}
			splitSubject(QString::fromLocal8Bit(mapSubjectInfo[step].data_subject.judge_logic),list_subject);
			for (int i=0;i<list_subject.size();i++)
			{
				if (list_subject.at(i).contains('&') || list_subject.at(i).contains('|')
					|| list_subject.at(i).contains('(') || list_subject.at(i).contains(')'))
				{
					exp.append(list_subject.at(i));//分解子步骤 拎出来分隔符
				}
				else
				{
					exp.append(QString::number(judgeCondExp(list_subject.at(i))));
				}
			}
#ifdef DEBUG
			printf("simple subject str is %s",exp.toLocal8Bit().data());
#endif
			if (chgExpToRpn(exp,rpn))
			{
				if(getValueByRpn(rpn,&rpnValue))
				{
					if (rpnValue)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
		if (curstep_timeout)
		{
			return true;
		}
	}//有超时处理
	else
	{
		if (QString::fromLocal8Bit(mapSubjectInfo[step].data_subject.judge_logic).isEmpty())//add by dm 2012-12-15 如果judge_logic为空则判断为成功
		{
			return true;
		}
		///////////curKeystep.logicalname =mapSubjectInfo[step].data_subject.judge_logic;
		splitSubject(QString::fromLocal8Bit(mapSubjectInfo[step].data_subject.judge_logic),list_subject);
		for (int i=0;i<list_subject.size();i++)
		{
			if (list_subject.at(i).contains('&') || list_subject.at(i).contains('|')
				|| list_subject.at(i).contains('(') || list_subject.at(i).contains(')'))
			{
				exp.append(list_subject.at(i));//分解子步骤 拎出来分隔符
			}
			else
			{
				exp.append(QString::number(judgeCondExp(list_subject.at(i))));
			}
		}
#ifdef DEBUG
		printf("simple subject str is %s",exp.toLocal8Bit().data());
#endif
		if (chgExpToRpn(exp,rpn))
		{
			if(getValueByRpn(rpn,&rpnValue))
			{
				if (rpnValue)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
}
void CTaskCalculation::judgeStepTimeout(int step)
{
// 	if (shm_ots_addr->simu_mode == 0 )
// 	{
		timer_overtime->stop();
		int sec = mapSubjectInfo[step].data_subject.time_limit;
		step_timeout = step;
		if (sec>0)
		{
			//timer_overtime->start(sec*1000);
			 timer_overtime-> singleShot(sec*1000,this, SLOT(steptimeout()));
		//	 connect(timer_overtime, SIGNAL(timeout()), this, SLOT(steptimeout()));
		}
		else
			//timer_overtime->start(30*1000);//默认30秒切换步骤超时提示 
			timer_overtime-> singleShot(30000,this, SLOT(steptimeout()));
//	}
}
bool CTaskCalculation::judgeStepPreConditionFinished(int step)
{
	QStringList	list_subject;
	float	subjectScore = 0;
	QString	exp,rpn;
	int rpnValue;
	if (!mapSubjectInfo.contains(step))
		return false;
	if (QString::fromLocal8Bit(mapSubjectInfo[step].data_subject.pre_condition).isEmpty())//add by dm 2012-12-15 如果pre_condition为空则判断为成功
		return true;
	QString PreCondition = QString::fromLocal8Bit(mapSubjectInfo[step].data_subject.pre_condition);
	splitSubject(PreCondition,list_subject);
	int cnt  = list_subject.size();
	for (int i=0;i<list_subject.size();i++)
	{
		if (list_subject.at(i).contains('&') || list_subject.at(i).contains('|')
			|| list_subject.at(i).contains('(') || list_subject.at(i).contains(')'))
		{
			exp.append(list_subject.at(i));
		}
		else
		{
			exp.append(QString::number(judgeCondExp(list_subject.at(i),1)));
		}
	}
#ifdef DEBUG
	printf("simple subject str is %s",exp.toLocal8Bit().data());
#endif
	if (chgExpToRpn(exp,rpn))
	{
		if(getValueByRpn(rpn,&rpnValue))
		{
			if (rpnValue)
				return true;
			else
				return false;
		}
	}
	else
		return false;
}
int CTaskCalculation::calcTaskScore()//原始入口
{
	QList<int> stepList;
	int step;
	if (stop_flag)
	{
		return FALSE;
	}//dzy
	if (updateTaskSubjectToMap() == 0)
	{
		qDebug("not have any subject to this task ,updateTaskSubjectToMap() error !");
		return FALSE;
	} 
	if (updateTaskRecordToHash() == 0)
	{
		qDebug("not have any record to this task ,updateTaskRecordToHash() error !");
		//return FALSE;
	}
	QMap<int,SUBJECT_INFO>::const_iterator i = mapSubjectInfo.constBegin();
	while (i != mapSubjectInfo.constEnd()) 
	{
		stepList.append(i.key());
		++i;
	}	
	flag_keystep_unfinished = false;
	for (int i=0;i<mapSubjectInfo.count();i++)
	{
		step = stepList.at(i);
		curKeystep.status = SUBJECT_STATUS_UNFINISH;

		if (mapSubjectInfo.contains(step))
		{
			if (mapSubjectInfo[step].data_subject.is_keystep)
			{// 				if (judgeStepPreConditionFinished(step))
// 				{
// 					if (judgeStepFinished(step))
// 					{
// 						score += mapSubjectInfo[step].data_subject.score;
// 						mapSubjectInfo[step].status = SUBJECT_STATUS_FINISH;
// 						writeScoreToHdb(step,mapSubjectInfo[step].data_subject.score);
// 					}
// 					else
// 					{
// 						mapSubjectInfo[step].status = SUBJECT_STATUS_UNFINISH;
// 						writeScoreToHdb(step,0);
// 						break;
				curKeystep.step = step;
				if (judgeStepFinished(step))
				{
					if (judgeStepPreConditionFinished(step))
					{
						if (flag_keystep_unfinished)
						{
							curKeystep.status = SUBJECT_KEYBEFOREUNFINISH_BUTRIGHT;
							mapSubjectInfo[step].status = curKeystep.status;
							writeScoreToHdb(step,0);
						}
						else
						{
							curKeystep.status = SUBJECT_STATUS_FINISH;
							score += mapSubjectInfo[step].data_subject.score;
							mapSubjectInfo[step].status = curKeystep.status;
							writeScoreToHdb(step,mapSubjectInfo[step].data_subject.score);
						}
					}//judge precond finish
					else
					{
						if (flag_keystep_unfinished)
						{
							curKeystep.status = SUBJECT_KEYBEFOREUNFINISH_ANDWRONG;
							mapSubjectInfo[step].status = curKeystep.status;
							writeScoreToHdb(step,0);
						}
						else
						{
							mapSubjectInfo[step].status = curKeystep.status;
							writeScoreToHdb(step,0);
							flag_keystep_unfinished = true;
							//break;
						}	
					}//pre_unfinish
				}
				else
				{
					mapSubjectInfo[step].status = curKeystep.status;
					writeScoreToHdb(step,0);
					flag_keystep_unfinished = true;
					//break;
				}
			}//is_keystep
			else //not keystep
			{
				if (judgeStepFinished(step))
				{
					if (flag_keystep_unfinished)
					{
						curKeystep.status = SUBJECT_KEYBEFOREUNFINISH_BUTRIGHT;
						mapSubjectInfo[step].status = curKeystep.status;
						writeScoreToHdb(step,0);
					}
					else
					{
						score += mapSubjectInfo[step].data_subject.score;
						mapSubjectInfo[step].status = SUBJECT_STATUS_FINISH;
						writeScoreToHdb(step,mapSubjectInfo[step].data_subject.score);
					}
				}
				else
				{
// 					if (flag_keystep_unfinished)
// 					{
// 						curKeystep.status = SUBJECT_KEYBEFOREUNFINISH_ANDWRONG;
// 						mapSubjectInfo[step].status = curKeystep.status;
// 						writeScoreToHdb(step,0);
// 					}
// 					else
// 					{
						mapSubjectInfo[step].status = SUBJECT_STATUS_UNFINISH;
						writeScoreToHdb(step,0);
//					}
				}
			}
		}
	}
	return TRUE;
}
void CTaskCalculation::writeScoreToHdb(int step,float score)
{	
// 	QSqlDatabase *tempdb;
// 	tempdb = new QSqlDatabase;//dzy 
// 	if (ConnectHDB(*tempdb,"tempdb3"))
// 	{
// 		QSqlQuery query(*tempdb);
// 		QString sql = QString::fromLocal8Bit("insert into ots.score(user_id,host_id,send_task_id,task_id,subject_id,step,score) values(%1,%2,%3,%4,%5,%6,%7)")
// 					.arg(login_user_id).arg(g_host_id).arg(m_send_task_id).arg(m_task_id).arg(mapSubjectInfo[step].data_subject.subject_id).arg(step).arg(score);
// 		query.exec(sql);
// 	}
		QSqlQuery query(m_db);
		QString sql = QString::fromLocal8Bit("insert into ots.score(user_id,host_id,send_task_id,task_id,subject_id,step,score) values(%1,%2,%3,%4,%5,%6,%7)")				
			.arg(login_user_id).arg(g_host_id).arg(m_send_task_id).arg(m_task_id).arg(mapSubjectInfo[step].data_subject.subject_id).arg(step).arg(score);
		query.exec(sql);
}
//dzy ADDED  上面的加step！！！
// void CTaskCalculation::writeScoreToHdb(int step,float score)
// {
// 	QSqlQuery query(m_db);
// 	QString sql = QString::fromLocal8Bit("insert into ots.score(user_id,host_id,send_task_id,task_id,subject_id,score) values(%1,%2,%3,%4,%5,%6)")
// 		.arg(login_user_id).arg(g_host_id).arg(m_send_task_id).arg(m_task_id).arg(mapSubjectInfo[step].data_subject.subject_id).arg(score);
// 	query.exec(sql);
// }
void CTaskCalculation::steptimeout()
{
	curstep_timeout = true;
}
