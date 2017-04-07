#include "qcommon.h"
#include <QCoreApplication>

UINT	priv_mask[MAX_USER_NUM][MAX_PRIV_MODE_NUM];
QMap<int,QString> PrivTeacher,PrivStudent;

QStandardItemModel *trainInfoModel = new QStandardItemModel;
QTimer *timerButtonChat = new QTimer;
USER_INFO heart_beat_user_info;
int login_user_id(-1);
void InitPrivilege()
{
	PrivTeacher.clear();
	PrivTeacher.insert(TeacherPrivType::PRIV_USER_INFO,QString::fromLocal8Bit("学员用户信息"));
	PrivTeacher.insert(TeacherPrivType::PRIV_LOGIN_REC,QString::fromLocal8Bit("学员登录历史记录"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SCORE,QString::fromLocal8Bit("学员历史成绩"));
	PrivTeacher.insert(TeacherPrivType::PRIV_PRIV_MANAGER,QString::fromLocal8Bit("用户权限"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SET_TASK,QString::fromLocal8Bit("设置任务"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SET_SCENE,QString::fromLocal8Bit("设置工况"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SET_FAULT,QString::fromLocal8Bit("设置故障"));
	PrivTeacher.insert(TeacherPrivType::PRIV_LOGIN_USER,QString::fromLocal8Bit("在线学员培训"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SEND_MESSAGE,QString::fromLocal8Bit("发送提示消息"));
	PrivTeacher.insert(TeacherPrivType::PRIV_MODEL,QString::fromLocal8Bit("模型库设置"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SEQUENCE,QString::fromLocal8Bit("流程库设置"));
	PrivTeacher.insert(TeacherPrivType::PRIV_FAULT,QString::fromLocal8Bit("故障库设置"));
	PrivTeacher.insert(TeacherPrivType::PRIV_TASK,QString::fromLocal8Bit("培训案例库"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SCENE,QString::fromLocal8Bit("静态工况库"));
	PrivTeacher.insert(TeacherPrivType::PRIV_REVERT,QString::fromLocal8Bit("工况再现库"));
	PrivTeacher.insert(TeacherPrivType::PRIV_LOCAL_OPR,QString::fromLocal8Bit("在线操作库"));
	PrivTeacher.insert(TeacherPrivType::PRIV_RECORD,QString::fromLocal8Bit("学员历史记录查询"));
	PrivTeacher.insert(TeacherPrivType::PRIV_EXAM,QString::fromLocal8Bit("专业知识考试"));
	PrivTeacher.insert(TeacherPrivType::PRIV_HOST,QString::fromLocal8Bit("配置工作站"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SCENE_RESET,QString::fromLocal8Bit("全厂工况初始"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SAVE_SCENE,QString::fromLocal8Bit("保存当前工况"));
	PrivTeacher.insert(TeacherPrivType::PRIV_CALL_DATA,QString::fromLocal8Bit("召唤监控数据"));
	PrivTeacher.insert(TeacherPrivType::PRIV_SET_POINT_MASK,QString::fromLocal8Bit("设置屏蔽点"));
	PrivStudent.clear();
	PrivStudent.insert(StudentPrivType::PRIV_SIMU_START,QString::fromLocal8Bit("仿真开始"));
	PrivStudent.insert(StudentPrivType::PRIV_SIMU_STOP,QString::fromLocal8Bit("仿真停止"));
	PrivStudent.insert(StudentPrivType::PRIV_SIMU_SPEED,QString::fromLocal8Bit("仿真速度设置"));
	PrivStudent.insert(StudentPrivType::PRIV_SET_TASK,QString::fromLocal8Bit("设置任务"));
	PrivStudent.insert(StudentPrivType::PRIV_SET_SCENE,QString::fromLocal8Bit("设置工况"));
	PrivStudent.insert(StudentPrivType::PRIV_SET_FAULT,QString::fromLocal8Bit("设置故障"));
	PrivStudent.insert(StudentPrivType::PRIV_LOCAL_OPR,QString::fromLocal8Bit("现地操作"));
	PrivStudent.insert(StudentPrivType::PRIV_RECORD,QString::fromLocal8Bit("历史记录查询"));
	PrivStudent.insert(StudentPrivType::PRIV_REVERT,QString::fromLocal8Bit("工况再现"));
	PrivStudent.insert(StudentPrivType::PRIV_EXAM,QString::fromLocal8Bit("专业知识考试"));
	PrivStudent.insert(StudentPrivType::PRIV_SEND_MESSAGE,QString::fromLocal8Bit("发送提示消息"));
	PrivStudent.insert(StudentPrivType::PRIV_SIMU_TIP,QString::fromLocal8Bit("任务提示窗口"));
	PrivStudent.insert(StudentPrivType::PRIV_TASK_RESTART,QString::fromLocal8Bit("任务重启"));
	memset(priv_mask,0,sizeof(priv_mask));
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	for (int i=0;i<data.rownum;i++)
	{
		for (int mode=0;mode<MAX_PRIV_MODE_NUM;mode++)
		{
			sscanf(data_userinfo[i].privilege+8*mode,"%08X",&priv_mask[data_userinfo[i].user_id][mode]);
		}
	}
}
bool CheckUserPrivilege(int user_id,int priv_type,int mode)
{
	if (user_id<0 || user_id>=MAX_USER_NUM)
		return false;
	char privilege[50];
	GetUserPrivilege(user_id,privilege);
	sscanf(privilege+8*mode,"%08X",&priv_mask[user_id][mode]);
	if (JudgeUserRole(user_id,ROLE_STUDENT))
	{
		if(priv_type>=StudentPrivType::PRIV_TYPE_NUM)
			return false;
		if (priv_mask[user_id][mode] & 1<<priv_type)
			return true;
		else 
			return false;
	}
	else
	{
		if(priv_type>=TeacherPrivType::PRIV_TYPE_NUM)
			return false;
		if (priv_mask[user_id][mode] & 1<<priv_type)
			return true;
		else 
			return false;
	}
}
QString getAppParentDir()
{
	QString strParentDir=getAppDir();

	int nIndex=strParentDir.lastIndexOf('/');
	if (nIndex != -1)
	{
		strParentDir=strParentDir.left(nIndex);
	}
	return strParentDir;
}
QString getAppDir()
{
	return QCoreApplication::applicationDirPath();
}
QMessageBox::StandardButton MaybeSave(QWidget *widget)
{
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(widget, QString::fromLocal8Bit("提示"),
		QString::fromLocal8Bit("是否想保存你的更改?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
	return ret;
	//if (ret == QMessageBox::Save)
	//	return true;
	//else if (ret == QMessageBox::Cancel)
	//	return false;
}
void moveToCenter(QWidget *widget)
{
	//int width=QApplication::desktop()->width(); 
	//int height=QApplication::desktop()->height();
	QRect rect = QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(widget));
	widget->move(rect.width()/2-widget->width()/2,rect.height()/2-widget->height()/2);
}
int JudgeUserRole(int user_id,short role)
{
	short role_sel;
	QString sql_str = QString::fromLocal8Bit("select role from ots.userinfo where user_id=%1").arg(user_id);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("role");
	while (query.next()) 
		role_sel = query.value(fieldNo).toString().toShort();
	return role_sel == role?1:0;
}
int JudgeUserGroupsRole(int user_id,short groups_role)
{
	short groups_role_sel;
	QString sql_str = QString::fromLocal8Bit("select groups_role from ots.userinfo where user_id=%1").arg(user_id);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("groups_role");
	while (query.next()) 
		groups_role_sel = query.value(fieldNo).toString().toShort();
	return groups_role_sel == groups_role?1:0;
}
int JudgeLoginState(int user_id)
{
	int num=0;
	QString sql_str = QString::fromLocal8Bit("select * from ots.train_info where user_id=%1 and login_status=1").arg(user_id);
	QSqlQuery query(sql_str,db_conn);
	while (query.next())
		num++;
	return num == 0?FALSE:TRUE;
}
int GetUserIdFromLongName(char *longname)
{
	int user_id;
	QString sql_str = QString::fromLocal8Bit("select user_id from ots.userinfo where longname='%1'").arg(QString::fromLocal8Bit(longname));
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("user_id");
	while (query.next()) 
		user_id = query.value(fieldNo).toString().toInt();
	return user_id;
}
int GetUserHostId(int user_id)
{
	int host_id;
	QString sql_str = QString::fromLocal8Bit("select host_id from ots.train_info where user_id=%1 and login_status=1").arg(user_id);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("host_id");
	while (query.next()) 
		host_id = query.value(fieldNo).toString().toInt();
	return host_id;
}
int GetUserGroupsId(int user_id)
{
	int groups_id;
	QString sql_str = QString::fromLocal8Bit("select groups_id from ots.userinfo where user_id=%1").arg(user_id);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("groups_id");
	while (query.next()) 
		groups_id = query.value(fieldNo).toString().toInt();
	return groups_id;
}
int GetUserIpAddr(int user_id,char *ip_a,char *ip_b)
{
	QString s_ip_a,s_ip_b;
	QString sql_str = QString::fromLocal8Bit("select ip_a,ip_b from ots.host where host_id=%1").arg(GetUserHostId(user_id));
	QSqlQuery query(sql_str,db_conn);
	int fieldNoA = query.record().indexOf("ip_a");
	int fieldNoB = query.record().indexOf("ip_b");
	while (query.next()) 
	{
		s_ip_a = query.value(fieldNoA).toString();
		s_ip_b = query.value(fieldNoB).toString();
	}
	strcpy(ip_a,s_ip_a.toLocal8Bit().data());
	strcpy(ip_b,s_ip_b.toLocal8Bit().data());
	return TRUE;
}
int GetUserLongName(int user_id,char *longName)
{
	QString s_longname;
	QString sql_str = QString::fromLocal8Bit("select longname from ots.userinfo where user_id=%1").arg(user_id);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("longname");
	while (query.next()) 
		s_longname = query.value(fieldNo).toString();
	strcpy(longName,s_longname.toLocal8Bit().data());
	return TRUE;
}
int GetUserPrivilege(int user_id,char *privilege)
{
	QString sql_str = QString::fromLocal8Bit("select privilege from ots.userinfo where user_id=%1").arg(user_id);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("privilege");
	while (query.next()) 
		strcpy(privilege,query.value(fieldNo).toString().toLocal8Bit().data());
	return TRUE;
}
int GetHostName(int host_id,char *host_name)
{
	QString s_host_name;
	QString sql_str = QString::fromLocal8Bit("select name from ots.host where host_id=%1").arg(host_id);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("name");
	while (query.next()) 
		s_host_name = query.value(fieldNo).toString();
	strcpy(host_name,s_host_name.toLocal8Bit().data());
	return TRUE;
}
int	GetLocalOprIdByName(const QString &name)
{
	int opr_id,num=0;
	QString sql_str = QString::fromLocal8Bit("select opr_id from ots.local_opr where name='%1'").arg(name);
	QSqlQuery query(sql_str,db_conn);
	int fieldNo = query.record().indexOf("opr_id");
	while (query.next()) 
	{
		opr_id = query.value(fieldNo).toString().toInt();
		num++;
	}
	if (num == 0)
		return -1;
	return opr_id;
}
int IsUserExist(const QString &user_name)
{
	int num=0;
	QString sql_str = QString::fromLocal8Bit("select * from ots.userinfo where user_id=%1 and longname='%1'").arg(user_name);
	QSqlQuery query(sql_str,db_conn);
	while (query.next())
		num++;
	return num == 0?FALSE:TRUE;
}
void PopupMessageBox(QWidget *parent,char msg[],int type)
{
	switch(type)
	{
		case QMessageBox::NoIcon:
			//QMessageBox()(parent,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit(msg),QMessageBox::Ok);
			break;
		case QMessageBox::Information:
			QMessageBox().information(parent,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit(msg),QMessageBox::Ok);
			break;
		case QMessageBox::Warning:
			QMessageBox().warning(parent,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit(msg),QMessageBox::Ok);
			break;
		case QMessageBox::Critical:
			QMessageBox().critical(parent,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit(msg),QMessageBox::Ok);
			break;
		case QMessageBox::Question:
			QMessageBox().question(parent,QString::fromLocal8Bit("提问!"),QString::fromLocal8Bit(msg),QMessageBox::Ok);
			break;
	}
}
void HeartBeat()
{
	USER_INFO user_info;
	QUdpSocket *udpSocket = new QUdpSocket;
	QByteArray datagram;
	QDataStream out(&datagram,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	user_info.user_id = g_user_id;
	user_info.host_id = g_host_id;
	out << user_info.user_id << user_info.host_id;
	udpSocket->writeDatagram(datagram.data(),datagram.size(),QHostAddress(g_ots_hdb_ip),HEART_BEAT_PORT);
}

void UpdateTrainInfoModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		qDebug("查询数据库失败!");
		return;
	}
	trainInfoModel->clear();
	trainInfoModel->setColumnCount(13);  
	trainInfoModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
	trainInfoModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("主机名")); 
	trainInfoModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("当前任务名称"));
	trainInfoModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("当前工况名称"));  
	trainInfoModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("当前故障名称"));
	trainInfoModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("登陆状态"));
	trainInfoModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("运行状态"));
	trainInfoModel->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("仿真速度"));
	trainInfoModel->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("仿真模式"));
	trainInfoModel->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("默认画面"));
	trainInfoModel->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("任务状态"));
	trainInfoModel->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("工况状态"));
	trainInfoModel->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("故障状态"));
	for (int i=0;i<data.rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_train_info[i].userinfo_longname)); 
		item0->setData(QVariant(data_train_info[i].user_id));
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_train_info[i].host_name));  
		item1->setData(QVariant(data_train_info[i].host_id));
		QStandardItem* item2 = new QStandardItem(QString::fromLocal8Bit(data_train_info[i].task_name)); 
		item2->setData(QVariant(data_train_info[i].task_id));
		QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(data_train_info[i].scene_name));  
		item3->setData(QVariant(data_train_info[i].scene_id));
		QStandardItem* item4 = new QStandardItem(QString::fromLocal8Bit(data_train_info[i].fault_name));
		item4->setData(QVariant(data_train_info[i].fault_id));
		QStandardItem* item5;
		if (data_train_info[i].login_status==0)
			item5 = new QStandardItem(QString::fromLocal8Bit("未登录")); 
		else if (data_train_info[i].login_status==1)
			item5 = new QStandardItem(QString::fromLocal8Bit("已登录")); 
		QStandardItem* item6;
		if (data_train_info[i].status==0)
			item6 = new QStandardItem(QString::fromLocal8Bit("停止")); 
		else if (data_train_info[i].status==1)
			item6 = new QStandardItem(QString::fromLocal8Bit("运行")); 
		else if (data_train_info[i].status==2)
			item6 = new QStandardItem(QString::fromLocal8Bit("暂停")); 

		QStandardItem* item7 = new QStandardItem(QString::number(data_train_info[i].speed)); 
		QStandardItem* item8;
		if (data_train_info[i].simu_mode==0)
			item8 = new QStandardItem(QString::fromLocal8Bit("培训模式")); 
		else if (data_train_info[i].simu_mode==1)
			item8 = new QStandardItem(QString::fromLocal8Bit("考试模式"));
		QStandardItem* item9 = new QStandardItem(QString::fromLocal8Bit(data_train_info[i].dbin_file)); 
		QStandardItem* item10;
		if (data_train_info[i].task_status==0)
			item10 = new QStandardItem(QString::fromLocal8Bit("无新任务")); 
		else if (data_train_info[i].task_status==1)
			item10 = new QStandardItem(QString::fromLocal8Bit("新的任务")); 
		else if (data_train_info[i].task_status==2)
			item10 = new QStandardItem(QString::fromLocal8Bit("任务已处理")); 
		else if (data_train_info[i].task_status==3)
			item10 = new QStandardItem(QString::fromLocal8Bit("任务结束")); 
		QStandardItem* item11;
		if (data_train_info[i].scene_status==0)
			item11 = new QStandardItem(QString::fromLocal8Bit("无新工况")); 
		else if (data_train_info[i].scene_status==1)
			item11 = new QStandardItem(QString::fromLocal8Bit("新的工况")); 
		else if (data_train_info[i].scene_status==2)
			item11 = new QStandardItem(QString::fromLocal8Bit("工况已处理")); 
		QStandardItem* item12;
		if (data_train_info[i].fault_status==0)
			item12 = new QStandardItem(QString::fromLocal8Bit("无新故障")); 
		else if (data_train_info[i].fault_status==1)
			item12 = new QStandardItem(QString::fromLocal8Bit("新的故障")); 
		else if (data_train_info[i].fault_status==2)
			item12 = new QStandardItem(QString::fromLocal8Bit("故障已处理")); 

		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		item3->setTextAlignment(Qt::AlignCenter);
		item4->setTextAlignment(Qt::AlignCenter);
		item5->setTextAlignment(Qt::AlignCenter);
		item6->setTextAlignment(Qt::AlignCenter);
		item7->setTextAlignment(Qt::AlignCenter);
		item8->setTextAlignment(Qt::AlignCenter);
		item9->setTextAlignment(Qt::AlignCenter);
		item10->setTextAlignment(Qt::AlignCenter);
		item11->setTextAlignment(Qt::AlignCenter);
		item12->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2 << item3 << item4 << item5 << item6 << item7 << item8 << item9 << item10 << item11 << item12; 
		trainInfoModel->appendRow(items);
	}
	FreeDbData(&data,TABLE_TRAIN_INFO);
}
void GetTimeFromSecs(int i,int *h,int *m,int *s)
{
	*h=i/3600;
	*m=i%3600/60;
	*s=i%3600%60;
}