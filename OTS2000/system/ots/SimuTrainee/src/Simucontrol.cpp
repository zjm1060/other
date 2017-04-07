#include "Simucontrol.h"
Simucontrol::Simucontrol(QWidget *parent/* =0 */)	
:QWidget(parent)
{
	dlg_created_flag[SIMUCONTROLDLG] = true;
	this->setAttribute(Qt::WA_DeleteOnClose);
	setupUi(this);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	init();
	connect(m_simuctr_horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setSimuSpeedLabel(int)));
	connect(this, SIGNAL(sigSpeedChanged(const QString &)),m_simuctr_label,SLOT(setText(const QString &)));
	connect(pushButton_APPLY, SIGNAL(clicked()), this, SLOT(setSimuSpeed()));
	connect(m_simuctr_buttonEnter, SIGNAL(clicked()), this, SLOT(onok()));
	connect(m_simuctr_buttonCancel, SIGNAL(clicked()), this, SLOT(onclose()));
	connect(buttonStart, SIGNAL(clicked()), this, SLOT(simuStart()));
	connect(buttonpause, SIGNAL(clicked()), this, SLOT(simuStop()));
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(simuStop()));
}
Simucontrol::~Simucontrol()
{
	dlg_created_flag[SIMUCONTROLDLG] = false;
}
void Simucontrol::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void Simucontrol::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void  Simucontrol::setLanguage(int language)
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
void  Simucontrol::init()
{
	this->setWindowTitle(QString::fromLocal8Bit("仿真速度控制"));	
	m_simuctr_horizontalSlider->setMinimum(-3);
	m_simuctr_horizontalSlider->setMaximum(4);
	simuSpeed = shm_ots_addr->speed;
	int val = (int)(log((double)simuSpeed)/log(2.0));
// 	if (simuSpeed <= 0 )
// 	{
// 		val =3;
// 	}
	m_simuctr_label->setText(QString::fromLocal8Bit("%1倍速").arg(QString::number(simuSpeed/8.0)));
	m_simuctr_horizontalSlider->setValue(val-3);
	m_simuctr_horizontalSlider->setPageStep(1);
//	m_simuctr_horizontalSlider->setOrientation(Qt::Horizontal);
	m_simuctr_horizontalSlider->setTickPosition(QSlider::TicksBothSides);	 

}
void Simucontrol::simuStart()
{	
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id=TRUE;
	data.cond_seg.seg_train_info.user_id= login_user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id= g_host_id;
	data.flag.flag_train_info.status=TRUE;
	data.seg.seg_train_info.status=1;
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		return;
	}
	FreeDbData(&data,TABLE_TRAIN_INFO);
	buttonStart->setEnabled(false);
	buttonStop->setEnabled(true);
}
void Simucontrol::simuStop()
{
	// 	if (!CheckUserPrivilege(login_user_id,StudentPrivType::PRIV_SIMU_STOP,shm_ots_addr->simu_mode))
	// 	{
	// 		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
	// 		return;
	// 	}
	// 	int groups_id = GetUserGroupsId(loginUser->login_user_id);
	// 	if (groups_id != -1 && JudgeUserGroupsRole(loginUser->login_user_id,GROUPS_ROLE_LEADER) == FALSE)
	// 	{
	// 		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能停止仿真，等待组长停止!"),QMessageBox::Ok);
	// 		return;
	// 	}
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
	msgBox.setInformativeText(QString::fromLocal8Bit("您确定停止当前仿真？"));
	msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Ok)
	{
		DB_FMT data;
		data.table_id = TABLE_TRAIN_INFO;
		InitDbData(&data,data.table_id);
		data.cond_flag.flag_train_info.user_id=TRUE;
		data.cond_seg.seg_train_info.user_id= login_user_id;
		data.cond_flag.flag_train_info.host_id = TRUE;
		data.cond_seg.seg_train_info.host_id= g_host_id;
		data.flag.flag_train_info.status=TRUE;
		data.seg.seg_train_info.status=0;
		if (UpdateOneRec(&data)!=TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_TRAIN_INFO);
			return;
		}
		FreeDbData(&data,TABLE_TRAIN_INFO);
		buttonStart->setEnabled(true);
		buttonStop->setEnabled(false);
	}
}

void Simucontrol::setSimuSpeedLabel(int value)
{
	double val=pow(2.0,value);
	simuSpeed = (short)(val*8);
	QString str = QString::fromLocal8Bit("%1倍速").arg(QString::number(val));
	emit sigSpeedChanged(str);
}
void Simucontrol::setSimuSpeedMenu()
{
	switch(shm_ots_addr->speed)
	{
		// 	case 1:
		// 		action1_8->setChecked(true);
		// 		break;
		// 	case 2:
		// 		action1_4->setChecked(true);
		// 		break;
		// 	case 4:
		// 		action1_2->setChecked(true);
		// 		break;
		// 	case 8:
		// 		action1->setChecked(true);
		// 		break;
		// 	case 16:
		// 		action2->setChecked(true);
		// 		break;
		// 	case 32:
		// 		action4->setChecked(true);
		// 		break;
		// 	case 64:
		// 		action8->setChecked(true);
		// 		break;
		// 	case 128:
		// 		action16->setChecked(true);
		// 		break;
	default:
		break;
	}
}
void Simucontrol::setSimuSpeed()
{
	// 	if (!CheckUserPrivilege(login_user_id,StudentPrivType::PRIV_SIMU_SPEED,shm_ots_addr->simu_mode))
	// 	{
	// 		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
	// 		return;
	// 	}
	// 	QAction *action = qobject_cast<QAction*>(sender());
	// 	if (action == action1_8)
	// 		simuSpeed = 1;
	// 	else if (action == action1_4)
	// 		simuSpeed = 2;
	// 	else if (action == action1_2)
	// 		simuSpeed = 4;
	// 	else if (action == action1)
	// 		simuSpeed = 8;
	// 	else if (action == action2)
	// 		simuSpeed = 16;
	// 	else if (action == action4)
	// 		simuSpeed = 32;
	// 	else if (action == action8)
	// 		simuSpeed = 64;
	// 	else if (action == action16)
	// 		simuSpeed = 128;
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id=TRUE;
	data.cond_seg.seg_train_info.user_id= login_user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id= g_host_id;
	data.flag.flag_train_info.speed=TRUE;
	data.seg.seg_train_info.speed = simuSpeed;
	if (UpdateOneRec(&data)!=(int)TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		return;
	}
	FreeDbData(&data,TABLE_TRAIN_INFO);
}
void Simucontrol::onclose()
{
	emit closedlg();
	this->close();
}
void Simucontrol::onok()
{
	setSimuSpeed();
	onclose();
}