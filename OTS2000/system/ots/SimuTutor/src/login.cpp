#include "login.h"

CLogin *loginUser;
CLogin::CLogin(QWidget* parent)
	:QDialog(parent)
{
	setupUi(this);
	init();
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	//this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	editPassword->setEchoMode(QLineEdit::Password);
	connect(buttonLogin,SIGNAL(clicked()),this,SLOT(teacherLogin()));
}
void CLogin::loadChinese()
{
	QTranslator translator;
	translator.load("Simututor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CLogin::loadEnglish()
{
	QTranslator translator;
	translator.load("Simututor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CLogin::init()
{
	login_user_id = -1;
	user_role = ROLE_SYSTEM;
	login_user_name.clear();
	login_user_longname.clear();
	login_user_password.clear();
	login_time.clear();
	login_status = false;
	editUserName->clear();
	editPassword->clear();
	editUserName->setFocus();
}
void CLogin::teacherLogin()
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

			if(JudgeUserRole(login_user_id,ROLE_STUDENT) == TRUE)
			{
				login_user_id = -1;
				login_user_name.clear();
				QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("所登陆用户不是系统用户或教员，不能登录!"),QMessageBox::Ok);
				editUserName->clear();
				editPassword->clear();
			}
			else
			{
				userLogin(login_user_id);
				emit sigUserLogin(login_user_id,login_user_longname);
				login_status = true;
				heart_beat_user_info.user_id = login_user_id;
				heart_beat_user_info.host_id = g_host_id;
				strcpy(heart_beat_user_info.login_time,login_time.toLocal8Bit().data());
				heart_beat_user_info.timeout = 0;
				//heartBeatThread = new HeartBeatThread(heart_beat_user_info,NULL);
				//heartBeatThread->start();
				QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("登录成功!"),QMessageBox::Ok);
				editUserName->clear();
				editPassword->clear();
				this->close();
			}
		}
		else
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("登录失败!"),QString::fromLocal8Bit("用户名或密码错误!"),QMessageBox::Ok);
			editUserName->clear();
			editPassword->clear();
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
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return -1;
	}
	if (data.rownum == 0)
	{
		return 0;
	}
	else
	{
		login_user_id = data_userinfo[0].user_id;
		login_user_name = QString::fromLocal8Bit(data_userinfo[0].name);
		login_user_longname = QString::fromLocal8Bit(data_userinfo[0].longname);
		login_user_password = QString::fromLocal8Bit(data_userinfo[0].password);
		user_role = data_userinfo[0].role;
		return 1;
	}
}
void CLogin::showEvent(QShowEvent *event)
{
	event->accept();
}
void  CLogin::setLanguage(int language)
{	
	QTranslator translator;
	if(flag_simutrainee)
	{
		if (language == 0)
		{
			translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
		}
		else if (language == 1)
		{
			translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_en");
		}
	}
	else
	{
		if (language == 0)
		{
			translator.load("SimuTutor_zh_CN");
		}
		else if (language == 1)
		{
			translator.load("SimuTutor_en");
		}
	}	
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CLogin::userLogin(int current_user_id)
{
	DB_FMT data;
	char host_name[1024];
	gethostname(host_name,sizeof(host_name));
	InitDbData(&data,TABLE_LOGIN_INFO);	
	data.flag.flag_login_info.user_id=TRUE;
	data.flag.flag_login_info.login_time=TRUE;
	data.flag.flag_login_info.host_id=TRUE;
	
	data.seg.seg_login_info.user_id=current_user_id;
	GetCurTimeStr(data.seg.seg_login_info.login_time);
	login_time = QString::fromLocal8Bit(data.seg.seg_login_info.login_time);
	data.seg.seg_login_info.host_id=GetHostIdByName(host_name);
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("插入数据失败!");
		return;
	}
	//教员用户登陆不修改train_info表 remove by dm 2012.6.20
/*	InitDbData(&data,TABLE_TRAIN_INFO);
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = g_host_id;
	SelectRecs(&data);
	if (data.rownum > 0)
	{
		InitDbData(&data,TABLE_TRAIN_INFO);
		data.flag.flag_train_info.user_id = TRUE;
		data.flag.flag_train_info.login_status = TRUE;
		data.seg.seg_train_info.user_id = current_user_id;
		data.seg.seg_train_info.login_status = 1;

		data.cond_flag.flag_train_info.host_id = TRUE;
		data.cond_seg.seg_train_info.host_id = g_host_id;
		if (UpdateOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);
			HdbLogWrite(data.sql_str,data.sql_error);
			qDebug("修改数据库失败!");
			return;
		}
	}
	else
	{
		InitDbData(&data,TABLE_TRAIN_INFO);
		data.flag.flag_train_info.user_id = TRUE;
		data.flag.flag_train_info.login_status = TRUE;
		data.seg.seg_train_info.user_id = current_user_id;
		data.seg.seg_train_info.login_status = 1;

		data.cond_flag.flag_train_info.host_id = TRUE;
		data.cond_seg.seg_train_info.host_id = g_host_id;
		if (InsertOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据库失败!"),QMessageBox::Ok);
			HdbLogWrite(data.sql_str,data.sql_error);
			qDebug("插入数据库失败!");
			return;
		}
	}	*/
}