#include "userinfo.h"

UserInfo::UserInfo(QWidget* parent)
:QWidget(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	initgroupinfo();
	dlg_created_flag[USERINFODLG] = true;
	this->setAttribute(Qt::WA_DeleteOnClose);
//	this->setModal(true);
	popMenu=NULL;
	d_CChangePassword = new CChangePassword;
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));
	connect(pushButtonChgPsd,SIGNAL(clicked()),this,SLOT(resetPwd()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(onclose()));
}
UserInfo::~UserInfo()
{
	dlg_created_flag[USERINFODLG] = false;
}
void UserInfo::getuserinfo()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.cond_flag.flag_userinfo.user_id=TRUE;
	data.cond_seg.seg_userinfo.user_id = login_user_id;
	SelectRecs(&data);
	if(data.rownum!=1)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询用户信息失败失败!"),QMessageBox::Ok);
		this->close();
	}
	else
	{
		editName->setText(QString::fromLocal8Bit(data_userinfo[0].name));
		editLongName->setText(QString::fromLocal8Bit(data_userinfo[0].longname));
		editPassword->setText(QString::fromLocal8Bit(data_userinfo[0].password));
		d_CChangePassword->password_old = editPassword->text();
		editIdCard->setText(QString::fromLocal8Bit(data_userinfo[0].idcard));
		editCompany->setText(QString::fromLocal8Bit(data_userinfo[0].company));
		editDepartment->setText(QString::fromLocal8Bit(data_userinfo[0].department));
		editDuty->setText(QString::fromLocal8Bit(data_userinfo[0].duty));
		editMTel->setText(QString::fromLocal8Bit(data_userinfo[0].mtel));
		editTel->setText(QString::fromLocal8Bit(data_userinfo[0].tel));
		editEmail->setText(QString::fromLocal8Bit(data_userinfo[0].email));
		editRole->setCurrentIndex(data_userinfo[0].role);
		editEdu->setCurrentIndex(data_userinfo[0].edu);
		editSex->setCurrentIndex(data_userinfo[0].sex);
		comboBox_userType->setCurrentIndex(data_userinfo[0].user_type_id);
		comboBox_userRange->setCurrentIndex(data_userinfo[0].user_range);
		u_info.user_id = data_userinfo[0].user_id;
		u_info.role = data_userinfo[0].role;
		u_info.sex = data_userinfo[0].sex;
		u_info.edu = data_userinfo[0].edu;
		u_info.groups_id = data_userinfo[0].groups_id;
		u_info.groups_role = data_userinfo[0].groups_role;
		u_info.user_range = data_userinfo[0].user_range; //用户级别
		u_info.user_type_id = data_userinfo[0].user_type_id;//用户工种
		strcpy(u_info.name,data_userinfo[0].name);
		strcpy(u_info.longname,data_userinfo[0].longname);
		strcpy(u_info.password,data_userinfo[0].password);
		strcpy(u_info.company,data_userinfo[0].company);
		strcpy(u_info.department,data_userinfo[0].department);
		strcpy(u_info.duty,data_userinfo[0].duty);
		strcpy(u_info.email,data_userinfo[0].email);
		strcpy(u_info.mtel,data_userinfo[0].mtel);
		strcpy(u_info.tel,data_userinfo[0].tel);
		strcpy(u_info.idcard,data_userinfo[0].idcard);
		strcpy(u_info.privilege,data_userinfo[0].privilege);
		strcpy(u_info.groups_name,data_userinfo[0].groups_name);
		if (data_userinfo[0].groups_id ==-1 )
		{
			editGroup->setCurrentIndex(0);
		}
		else{
			for(int i=1;i<editGroup->count(),editGroup->currentIndex()==0;i++ )
			{
				editGroup->setCurrentIndex(i);
				if (editGroup->currentText()!=QString::fromLocal8Bit(data_userinfo[0].groups_name) )
				{
					editGroup->setCurrentIndex(0);
				}
			}
		}
	}
}
void UserInfo::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void UserInfo::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void UserInfo::showEvent(QShowEvent *event)
{
// 	if (flag_langueage == 0)
// 	{
// 		loadChinese();
// 	}
// 	else if (flag_langueage == 1)
// 	{
// 		loadEnglish();
// 	}
	event->ignore();
}
void  UserInfo::setLanguage(int language)
{	
	QTranslator translator;
	if (language == 0)
	{
		translator.load("SimuTrainee_zh_CN");
	}
	else if (language == 1)
	{
		translator.load("SimuTrainee_en");
	}
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void UserInfo::resetPwd()
{
	d_CChangePassword->show();
}

void UserInfo::initgroupinfo()
{
	DB_FMT data;
	InitDbData(&data,TABLE_GROUPS);
	if( SelectRecs(&data) != TRUE)
	{	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS);
	}
	for (int i=0;i<data.rownum;i++)
	{
		if (data_groups[i].groups_id != -1 /*&& data_groups[i].groups_id != 0*/)
		{
			QListWidgetItem *groupsItem = new QListWidgetItem;
			groupsItem->setText(QString::fromLocal8Bit(data_groups[i].name));
			groupsItem->setData(2,QVariant(data_groups[i].name));
			groupsItem->setData(3,QVariant(data_groups[i].groups_id));
			editGroup->addItem(groupsItem->text(),groupsItem->data(3));
		}
	}
	FreeDbData(&data,TABLE_GROUPS);
}
void UserInfo::modifyUser()
{
	
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.flag.flag_userinfo.user_id =FALSE;
	data.flag.flag_userinfo.name=TRUE;
	data.flag.flag_userinfo.longname=TRUE;
//	data.flag.flag_userinfo.password=TRUE;
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
	data.seg.seg_userinfo = u_seg;
	data.cond_flag.flag_userinfo.user_id=TRUE;
	data.cond_seg.seg_userinfo.user_id = login_user_id;
	if( UpdateOneRec(&data) != TRUE)
	{	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	FreeDbData(&data,TABLE_USERINFO);
}


int UserInfo::apply()
{		
	QString strRole,strSex,strEdu;
	if (editName->text().isEmpty())
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("用户名不能为空!"),QMessageBox::Ok);	
		return -1;
	}
	strcpy(u_seg.name,editName->text().toLocal8Bit().data());
	if (editLongName->text().isEmpty())
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("姓名不能为空!"),QMessageBox::Ok);	
		return -1;
	}
	strcpy(u_seg.longname,editLongName->text().toLocal8Bit().data());
	strRole = editRole->currentText();
	if (strRole==QString::fromLocal8Bit("学员"))
	{
		u_seg.role = 0;
	}
	else if (strRole==QString::fromLocal8Bit("教员"))
	{
		u_seg.role = 1;
	}
	else if (strRole==QString::fromLocal8Bit("系统"))
	{
		u_seg.role = 2;
	}
	else
	{
		u_seg.role = -1;
	}
	strSex = editSex->currentText();
	if (strSex==QString::fromLocal8Bit("男"))
	{
		u_seg.sex = 0;
	}
	else if (strSex==QString::fromLocal8Bit("女"))
	{
		u_seg.sex = 1;
	}
	else
	{
		u_seg.sex = -1;
	}
// 	if (editPassword->text().isEmpty())
// 	{
// 		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("密码不能为空!"),QMessageBox::Ok);	
// 		return -1;
// 	}
// 	strcpy(seg.password,editPassword->text().toLocal8Bit().data());
	if (editIdCard->text().isEmpty())
		strcpy(u_seg.idcard,"");
	else
		strcpy(u_seg.idcard,editIdCard->text().toLocal8Bit().data());
	strEdu = editEdu->currentText();
	if (strEdu==QString::fromLocal8Bit("大学以下"))
	{
		u_seg.edu = 0;
	}
	else if(strEdu==QString::fromLocal8Bit("专科"))
	{
		u_seg.edu = 1;
	}
	else if(strEdu==QString::fromLocal8Bit("本科"))
	{
		u_seg.edu = 2;
	}
	else if(strEdu==QString::fromLocal8Bit("研究生及以上"))
	{
		u_seg.edu = 3;
	}
	else
	{
		u_seg.edu = -1;
	}
	if (editTel->text().isEmpty())
		strcpy(u_seg.tel,"");
	else
		strcpy(u_seg.tel,editTel->text().toLocal8Bit().data());
	if (editMTel->text().isEmpty())
		strcpy(u_seg.mtel,"");
	else
		strcpy(u_seg.mtel,editMTel->text().toLocal8Bit().data());
	if (editEmail->text().isEmpty())
		strcpy(u_seg.email,"");
	else
		strcpy(u_seg.email,editEmail->text().toLocal8Bit().data());
	if (editCompany->text().isEmpty())
		strcpy(u_seg.company,"");
	else
		strcpy(u_seg.company,editCompany->text().toLocal8Bit().data());
	if (editDepartment->text().isEmpty())
		strcpy(u_seg.department,"");
	else
		strcpy(u_seg.department,editDepartment->text().toLocal8Bit().data());
	if (editDuty->text().isEmpty())
		strcpy(u_seg.duty,"");
	else
		strcpy(u_seg.duty,editDuty->text().toLocal8Bit().data());
	modifyUser();
	return 1;
}
void UserInfo::enter()
{
	if (apply() == 1)
	{
		emit closedlg();
		editName->clear();
		editLongName->clear();
		editRole->setCurrentIndex(0);
		editSex->setCurrentIndex(0);
		editPassword->clear();
		editIdCard->clear();
		editEdu->setCurrentIndex(0);
		comboBox_userType->setCurrentIndex(0);
		comboBox_userRange->setCurrentIndex(0);
		editTel->clear();
		editMTel->clear();
		editEmail->clear();
		editCompany->clear();
		editDepartment->clear();
		editDuty->clear();
		this->close();
	}
}
void UserInfo::onclose()
{
	emit closedlg();
	editName->clear();
	editLongName->clear();
	editRole->setCurrentIndex(0);
	editSex->setCurrentIndex(0);
	editPassword->clear();
	editIdCard->clear();
	editEdu->setCurrentIndex(0);
	comboBox_userType->setCurrentIndex(0);
	comboBox_userRange->setCurrentIndex(0);
	editTel->clear();
	editMTel->clear();
	editEmail->clear();
	editCompany->clear();
	editDepartment->clear();
	editDuty->clear();
	this->close();
}
/*********************************************************CChangePassword******************************************************************/
CChangePassword::CChangePassword(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	setModal(true);
//	setAttribute(Qt::WA_DeleteOnClose);
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
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	editPasswordOld->clear();
	editPasswordNew->clear();
	editPasswordNewConfirm->clear();
	label_4->clear();
	label_5->clear();
	label_6->clear();
	event->accept();
}
void CChangePassword::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CChangePassword::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CChangePassword::enter()
{
	if (password_old == paswordedited && password_new == password_new_confirm)
	{
		DB_FMT data;
		InitDbData(&data,TABLE_USERINFO);
		data.flag.flag_userinfo.password=TRUE;	
		strcpy(data.seg.seg_userinfo.password,password_new.toLocal8Bit().data());

		data.cond_flag.flag_userinfo.user_id=TRUE;	
		data.cond_seg.seg_userinfo.user_id=login_user_id;
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
	paswordedited = str;
	if (password_old == paswordedited)
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