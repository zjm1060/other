#include "Login.h"

CLogin *loginUser;
CLogin::CLogin(QWidget* parent)
	:QDialog(parent)
{
	setupUi(this);
	init();
	connect(buttonLogin,SIGNAL(clicked()),this,SLOT(studentLogin()) );
	connect(buttonRegister,SIGNAL(clicked() ),this,SLOT(openStudentInfoAddTable()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(reject()));
}
void CLogin::init()
{
	login_user_id = -1;
	login_user_name.clear();
	login_user_longname.clear();
	login_user_password.clear();
	login_time.clear();
	login_status = false;
	editUserName->clear();
	editPassword->clear();
	editUserName->setFocus();
	QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
	QValidator *validator = new QRegExpValidator(rx, this);
	editHdbIp->setValidator(validator);
	editHdbIp->setInputMask("000.000.000.000;");//设置IP输入
	editHdbIp->setReadOnly(true);
	editHdbIp->setText(QString::fromLocal8Bit(g_ots_hdb_ip));
	buttonLogin->setDefault(true);
	readConfig();
}
void CLogin::openStudentInfoAddTable()
{
	studentinfoaddtable = new CStudentInfoAddTable(this);
	connect(studentinfoaddtable,SIGNAL(insertRecord()),this,SLOT(addUser()));
	studentinfoaddtable->flag_add=true;
	studentinfoaddtable->setWindowTitle(QString::fromLocal8Bit("用户注册"));
	studentinfoaddtable->buttonApply->setHidden(TRUE);
	studentinfoaddtable->buttonEnter->setText(QString::fromLocal8Bit("注册"));
	studentinfoaddtable->editRole->setCurrentIndex(0);
	studentinfoaddtable->editRole->setDisabled(TRUE);
	studentinfoaddtable->show();
}

void CLogin::addUser()
{
	if(IsUserExist(QString::fromLocal8Bit(studentinfoaddtable->seg.name)) == TRUE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("该用户已存在!"),QMessageBox::Ok);	
		return;
	}
	DB_FMT data;
	DATA_USERINFO seg;
	InitDbData(&data,TABLE_USERINFO);
	data.flag.flag_userinfo.user_id =TRUE;
	data.flag.flag_userinfo.name=TRUE;
	data.flag.flag_userinfo.longname=TRUE;
	data.flag.flag_userinfo.password=TRUE;
	data.flag.flag_userinfo.role=TRUE;
	data.flag.flag_userinfo.sex=TRUE;
	data.flag.flag_userinfo.edu=TRUE;
	data.flag.flag_userinfo.company=TRUE;
	data.flag.flag_userinfo.department=TRUE;
	data.flag.flag_userinfo.duty=TRUE;
	data.flag.flag_userinfo.email=TRUE;
	data.flag.flag_userinfo.mtel=TRUE;
	data.flag.flag_userinfo.tel=TRUE;
	data.flag.flag_userinfo.idcard=TRUE;
	data.flag.flag_userinfo.privilege=TRUE;
	data.flag.flag_userinfo.groups_id=TRUE;
	data.flag.flag_userinfo.groups_role=TRUE;
	//填充表中每个字段的值
	data.seg.seg_userinfo.user_id = GetInexistenceIdInHdb(TABLE_USERINFO);
	strcpy(data.seg.seg_userinfo.name,studentinfoaddtable->seg.name);
	strcpy(data.seg.seg_userinfo.longname,studentinfoaddtable->seg.longname);
	strcpy(data.seg.seg_userinfo.password,studentinfoaddtable->seg.password);
	data.seg.seg_userinfo.role = studentinfoaddtable->seg.role;
	data.seg.seg_userinfo.sex= studentinfoaddtable->seg.sex;
	data.seg.seg_userinfo.edu = studentinfoaddtable->seg.edu;
	strcpy(data.seg.seg_userinfo.company,studentinfoaddtable->seg.company);
	strcpy(data.seg.seg_userinfo.department,studentinfoaddtable->seg.department);
	strcpy(data.seg.seg_userinfo.duty,studentinfoaddtable->seg.duty);
	strcpy(data.seg.seg_userinfo.email,studentinfoaddtable->seg.email);
	strcpy(data.seg.seg_userinfo.mtel,studentinfoaddtable->seg.mtel);
	strcpy(data.seg.seg_userinfo.tel,studentinfoaddtable->seg.tel);
	strcpy(data.seg.seg_userinfo.idcard,studentinfoaddtable->seg.idcard);
	QString user_priv;
	if (is_user_priv)
		user_priv = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
	else
		user_priv = "0000000000000000000000000000000000000000";
	strcpy(data.seg.seg_userinfo.privilege,user_priv.toLocal8Bit().data());
	data.seg.seg_userinfo.groups_id=-1;
	data.seg.seg_userinfo.groups_role=-1;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		QMessageBox().critical(studentinfoaddtable,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("注册失败!"),QMessageBox::Ok);		
		return;
	}
	else
		QMessageBox().information(studentinfoaddtable,QString::fromLocal8Bit("信息!"),QString::fromLocal8Bit("注册成功!"),QMessageBox::Ok);
	FreeDbData(&data,TABLE_USERINFO);
}

void CLogin::studentLogin()
{
	QString login_name,login_password;
	login_name = editUserName->text();
	login_password = editPassword->text();
	if(login_name==NULL)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("用户名不能为空!"),QMessageBox::Ok);
		return;
	}
	else if(login_password==NULL)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("密码不能为空!"),QMessageBox::Ok);
		return;
	}
	else 
	{
		if(checkLoginInfo(login_name,login_password) == 1)
		{
			if(JudgeUserRole(login_user_id,ROLE_STUDENT) != TRUE)
			{
				QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("所登陆用户不是学员用户，不能登录!"),QMessageBox::Ok);
				editUserName->clear();
				editPassword->clear();
				editUserName->setFocus();
				this->reject();
				return;
			}
			if (JudgeLoginState(login_user_id) == TRUE)
			{
				QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("该用户已在其它客户端登录!"),QMessageBox::Ok);
				editUserName->clear();
				editPassword->clear();
				editUserName->setFocus();
				this->reject();
				return;
			}
			else
			{
				userLogin(login_user_id);
				QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("登录成功!"),QMessageBox::Ok);
				shm_ots_addr->user_id = g_user_id = login_user_id;
				shm_ots_addr->login_status = TRUE;
				login_status = true;
				SetObjCond();
				emit sigUserLogin(login_user_id,login_user_longname);
				this->accept();
			}
		}
		else
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("登录失败!"),QString::fromLocal8Bit("用户名或密码错误!"),QMessageBox::Ok);
			editUserName->clear();
			editPassword->clear();
			editUserName->setFocus();
		}
	}
}
int CLogin::checkLoginInfo(const QString &name,const QString &password)
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.cond_flag.flag_userinfo.name = TRUE;
	data.cond_flag.flag_userinfo.password = TRUE;
	strcpy(data.cond_seg.seg_userinfo.name,name.toLocal8Bit().data());
	strcpy(data.cond_seg.seg_userinfo.password,password.toLocal8Bit().data());
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		qDebug("查询数据库失败!");
		return -1;
	}
	if (data.rownum == 0)
	{
		FreeDbData(&data,TABLE_USERINFO);
		return 0;
	}
	else
	{
		login_user_id = data_userinfo[0].user_id;
		login_user_name = QString::fromLocal8Bit(data_userinfo[0].name);
		login_user_longname = QString::fromLocal8Bit(data_userinfo[0].longname);
		login_user_password = QString::fromLocal8Bit(data_userinfo[0].password);
		FreeDbData(&data,TABLE_USERINFO);
		return 1;
	}
}

void CLogin::userLogin(int current_user_id)
{
	DB_FMT data;
	char host_name[1024];
	InitDbData(&data,TABLE_LOGIN_INFO);	
	data.flag.flag_login_info.user_id=TRUE;
	data.flag.flag_login_info.login_time=TRUE;
	data.flag.flag_login_info.host_id=TRUE;
	
	data.seg.seg_login_info.user_id=current_user_id;
	GetCurTimeStr(data.seg.seg_login_info.login_time);
	login_time = QString::fromLocal8Bit(data.seg.seg_login_info.login_time);
	data.seg.seg_login_info.host_id=g_host_id;
	if( InsertOneRec(&data) != TRUE)
	{	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		qDebug("插入数据失败!");
		return;
	}
	InitDbData(&data,TABLE_TRAIN_INFO);
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = g_host_id;
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		return;
	}
	if (data.rownum > 0)
	{
		InitDbData(&data,TABLE_TRAIN_INFO);
		data.flag.flag_train_info.user_id = TRUE;
		data.flag.flag_train_info.login_status = TRUE;
		data.flag.flag_train_info.simu_mode = TRUE;
		data.flag.flag_train_info.speed = TRUE;
		data.flag.flag_train_info.status = TRUE;
		data.flag.flag_train_info.task_status = TRUE;
		data.flag.flag_train_info.scene_status = TRUE;
		data.flag.flag_train_info.fault_status = TRUE;
		data.seg.seg_train_info.user_id = current_user_id;
		data.seg.seg_train_info.login_status = 1;
		data.seg.seg_train_info.simu_mode = 0;
		data.seg.seg_train_info.speed = 8;
		data.seg.seg_train_info.status=0;
		data.seg.seg_train_info.task_status=0;
		data.seg.seg_train_info.scene_status=0;
		data.seg.seg_train_info.fault_status=0;

		data.cond_flag.flag_train_info.host_id = TRUE;
		data.cond_seg.seg_train_info.host_id = g_host_id;
		if (UpdateOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_TRAIN_INFO);
			qDebug("修改数据库失败!");
			return;
		}
		FreeDbData(&data,TABLE_TRAIN_INFO);
	}
	else
	{
		InitDbData(&data,TABLE_TRAIN_INFO);
		data.flag.flag_train_info.user_id = TRUE;
		data.flag.flag_train_info.login_status = TRUE;
		data.flag.flag_train_info.host_id = TRUE;
		data.flag.flag_train_info.simu_mode = TRUE;
		data.flag.flag_train_info.speed = TRUE;
		data.flag.flag_train_info.status = TRUE;
		data.flag.flag_train_info.task_status = TRUE;
		data.flag.flag_train_info.scene_status = TRUE;
		data.flag.flag_train_info.fault_status = TRUE;
		data.seg.seg_train_info.user_id = current_user_id;
		data.seg.seg_train_info.login_status = 1;
		data.seg.seg_train_info.host_id = g_host_id;
		data.seg.seg_train_info.simu_mode = 0;
		data.seg.seg_train_info.speed = 8;
		data.seg.seg_train_info.status=0;
		data.seg.seg_train_info.task_status=0;
		data.seg.seg_train_info.scene_status=0;
		data.seg.seg_train_info.fault_status=0;
		if (InsertOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_TRAIN_INFO);
			qDebug("插入数据库失败!");
			return;
		}
		FreeDbData(&data,TABLE_TRAIN_INFO);
	}	
}
int CLogin::SetObjCond()
{
	unsigned char	stnid,devid;
	POINTER pt;
	DMS_COMMON      rs;
	ENTRY_DEVOBJ_VAL obj_value;
	FB_STATUS1	fb_status1;
	short			old_status;
	LCUID lcuno;
	unsigned int stn_num,dev_num,point_num;
	int i,j;

	GetStationNum(&stn_num);
	for(j=1;j<=stn_num;j++)
	{
		UCHAR	stnid;
		if(-1==GetStnIdByIndex(j, &stnid))
			continue;
		if(-1==GetGroupNumById(stnid, &dev_num))
			continue;
		for(devid=1;devid<=dev_num;devid++)
		{
			point_num=0;
			if(-1==GetEntryNumById(stnid, devid, DEVOBJ_TYPE, &point_num) )
			{
				//return FALSE;
				continue;
			}
			if(point_num<=0) continue;
			pt.stn_id = stnid;
			pt.dev_id = devid;
			pt.data_type = DEVOBJ_TYPE;

			for(i=0;i<point_num;i++)
			{
				pt.pt_id = i;
				if(ReadEntryById( &pt, &rs) ==-1) continue;
				obj_value = rs.var.devobj;
				if(obj_value.fb_status1.condc0 == 1 && obj_value.fb_status1.condc1 == 1)
				{
					continue;
				}
				obj_value.fb_status1.condc0 =1;
				obj_value.fb_status1.condc1 =1;
				SetDevObjPtVarById( &pt, obj_value );
			}
		}
	}
	return TRUE;
}
void CLogin::readConfig()
{
	QString myfile = SimuTraineeConfigPath;
	QSettings mySetting(myfile,QSettings::IniFormat);
	mySetting.setIniCodec(QTextCodec::codecForName("GB2312"));
	is_user_priv = mySetting.value("user/priv",true).toBool();
}
/*********************************************************CChangePassword******************************************************************/
CChangePassword::CChangePassword(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setAttribute(Qt::WA_DeleteOnClose);
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(buttonCancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(editPasswordOld,SIGNAL(textEdited(const QString &)),this,SLOT(getOld(const QString &)));
	connect(editPasswordNew,SIGNAL(textEdited(const QString &)),this,SLOT(getNew(const QString &)));
	connect(editPasswordNewConfirm,SIGNAL(textEdited(const QString &)),this,SLOT(getNewConfirm(const QString &)));
	connect(this,SIGNAL(sigUpdateOld(const QString &)),label_4,SLOT(setText(const QString &)));
	connect(this,SIGNAL(sigUpdateNew(const QString &)),label_5,SLOT(setText(const QString &)));
	connect(this,SIGNAL(sigUpdateNewConfirm(const QString &)),label_6,SLOT(setText(const QString &)));
}
void CChangePassword::showEvent(QShowEvent *event)
{
	editPasswordOld->clear();
	editPasswordNew->clear();
	editPasswordNewConfirm->clear();
	label_4->clear();
	label_5->clear();
	label_6->clear();
	event->accept();
}
void CChangePassword::enter()
{
	if (password_old == loginUser->login_user_password && password_new == password_new_confirm)
	{
		DB_FMT data;
		InitDbData(&data,TABLE_USERINFO);
		data.flag.flag_userinfo.password=TRUE;	
		strcpy(data.seg.seg_userinfo.password,password_new.toLocal8Bit().data());

		data.cond_flag.flag_userinfo.user_id=TRUE;	
		data.cond_seg.seg_userinfo.user_id=loginUser->login_user_id;
		if (UpdateOneRec(&data)==(int)FALSE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			QMessageBox::information( this,QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("修改失败"),	QMessageBox::Ok);
			FreeDbData(&data,TABLE_USERINFO);
			return;
		}
		else				
		{
			QMessageBox::information( this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("修改密码成功！"),QMessageBox::Ok);
			editPasswordOld->clear();
			editPasswordNew->clear();
			editPasswordNewConfirm->clear();
			accept();
		}
		FreeDbData(&data,TABLE_USERINFO);
	}
	else
	{
		QMessageBox::information( this,QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("输入有误，请重新输入"),
			QMessageBox::Ok);
		editPasswordOld->setFocus();
	}

}
void CChangePassword::getOld(const QString &str)
{
	QString tip;
	password_old = str;
	if (password_old == loginUser->login_user_password)
	{
		label_4->setStyleSheet("color: rgb(0,255,0);");
		tip = QString::fromLocal8Bit("输入正确");
	}	
	else
	{
		label_4->setStyleSheet("color: rgb(255,0,0);");
		tip = QString::fromLocal8Bit("输入与原密码不匹配");
	}
	emit sigUpdateOld(tip);
}
void CChangePassword::getNew(const QString &str)
{
	QString tip;
	password_new = str;
	if (password_new == password_new_confirm)
	{
		label_6->setStyleSheet("color: rgb(0,255,0);");
		tip = QString::fromLocal8Bit("输入正确");
	}	
	else
	{
		label_6->setStyleSheet("color: rgb(255,0,0);");
		tip = QString::fromLocal8Bit("两次输入不一致");
	}
	emit sigUpdateNewConfirm(tip);
}
void CChangePassword::getNewConfirm(const QString &str)
{
	QString tip;
	password_new_confirm = str;
	if (password_new == password_new_confirm)
	{
		label_6->setStyleSheet("color: rgb(0,255,0);");
		tip = QString::fromLocal8Bit("输入正确");
	}	
	else
	{
		label_6->setStyleSheet("color: rgb(255,0,0);");
		tip = QString::fromLocal8Bit("两次输入不一致");
	}
	emit sigUpdateNewConfirm(tip);
}