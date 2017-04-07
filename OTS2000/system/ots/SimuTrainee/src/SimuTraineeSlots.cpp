#include "SimuTrainee.h"
void SimuTrainee::forceLogout()
{
	if (loginUser->login_status)
	{
		PopupMessageBox(this,"当前登录用户已被管理员强制踢出!",QMessageBox::Information);		
		cleanUserInfo();
		this->hide();
		loginUser->show();
	}
}

void SimuTrainee::change_ots_mode(int mode)
{
	QString s_mode;
	switch (mode)
	{
		case 0:
			s_mode = QString::fromLocal8Bit("培训");
			break;
		case 1:
			s_mode = QString::fromLocal8Bit("考试");
			break;
		case 2:
			s_mode = QString::fromLocal8Bit("竞赛");
			break;
		default:
			break;
	}
	if (dlg_created_flag[HOSTPAGEDLG])
	{
		d_hostpage->update();
	}
	QString  str;
	if (loginUser->train_mode == EXAMMODESELECTED&& EXAMMODESELECTED!= mode)
	{
		str = QString::fromLocal8Bit("您已经退出考试模式，即将进入")+s_mode+QString::fromLocal8Bit("模式");
		PopupMessageBox(this,str.toLocal8Bit().data(),QMessageBox::Information);
		emit 	sigModeChanged(mode);
		loginUser->train_mode = mode;
		return;
	}
	if (loginUser->train_mode == STUDYMODESELECTED&& STUDYMODESELECTED!= mode)
	{
		str = QString::fromLocal8Bit("您已经退出学习模式，即将进入")+s_mode+QString::fromLocal8Bit("模式");
		PopupMessageBox(this,str.toLocal8Bit().data(),QMessageBox::Information);
		emit 	sigModeChanged(mode);
		loginUser->train_mode = mode;
		return;
	}
	if (loginUser->train_mode == 2&& 2!= mode)
	{
		str = QString::fromLocal8Bit("您已经退出学习模式，即将进入")+s_mode+QString::fromLocal8Bit("模式");
		PopupMessageBox(this,str.toLocal8Bit().data(),QMessageBox::Information);
		emit 	sigModeChanged(mode);
		loginUser->train_mode = mode;
		return;
	}
	else
		return;
}
void  SimuTrainee::on_mode_changed(int mode)
{
	if(dlg_created_flag[TASKDLG] == true&&dlg_task_clicked)
	{
		if (shm_ots_addr->data_task.task_status==2)
		{
						
				d_CTaskManage->taskFinish(true);
				shm_ots_addr->data_task.task_status=3;
		}
		d_CTaskManage->close();
		subwindow[TASKDLG]->close();
	}
	if (mode == 0)
	{
		d_hostpage->update();
		d_hostpage->on_toolButton_ModeS();
	}
	else if (mode == 1)
	{
		d_hostpage->update();
		d_hostpage->on_toolButton_ModeE();
	}	
	else if (mode == 2)
	{
		d_hostpage->update();
		d_hostpage->on_toolButton_ModeC();
	}
	if (mode>0)//清空学习模式下的某些窗口
	{
		if (dlg_created_flag[SETTASKDLG])
		{
			d_CSetTaskTable->~CSetTaskTable();
			subwindow[SETTASKDLG]->close();
		}
		if (dlg_created_flag[SETSCENEFAULTDLG])
		{
			setSceneFaultDialog->~CSetSceneFaultTable();
			subwindow[SETSCENEFAULTDLG]->close();
		}
		if (dlg_created_flag[MYDOCDLG])
		{
			d_mydoc->~Mydoc();
			subwindow[MYDOCDLG]->close();
		}
		if (dlg_created_flag[HISTORYRECDLG])
		{
			d_historyrec->~CHistoryRecTable();
			subwindow[HISTORYRECDLG]->close();
		}
		if (dlg_created_flag[USERSCOREDLG])
		{
			d_CScoreRec->~CScoreRec();
			subwindow[USERSCOREDLG]->close();
		}
		if (dlg_created_flag[REVERTDLG])
		{
			d_RevertPlayer->~CRevertPlayer();
			subwindow[REVERTDLG]->close();
		}
	}
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id = TRUE;
	data.cond_seg.seg_train_info.user_id = login_user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = g_host_id;
	data.flag.flag_train_info.simu_mode = TRUE;
	data.seg.seg_train_info.simu_mode = mode;
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}

void SimuTrainee::on_STclicked()
{
	if (shm_ots_addr->simu_mode ==0)
	{
			show_doc();
	}
	else
	{
		PopupMessageBox(this,"当前学员模式不是学习模式",QMessageBox::Information);
		return;
	}
}
void SimuTrainee::on_SOclicked()
{
	//show_openOix();
	if (shm_ots_addr->simu_mode ==0)
	{
			show_settask();
	}
	else
	{
		PopupMessageBox(this,"当前学员模式不是学习模式",QMessageBox::Information);
		return;
	}
}
void SimuTrainee::on_ETclicked()
{
	if (shm_ots_addr->simu_mode ==1)
	{
		show_Exam();
	}
	else
	{
		PopupMessageBox(this,"当前学员模式不是考试模式",QMessageBox::Information);
		return;
	}
}
void SimuTrainee::on_EOclicked()
{
	if (shm_ots_addr->simu_mode ==1)
	{
		HWND   hWnd=FindWindow(NULL,L"OIX/H9000          北京中水科水电科技开发有限公司");	
		if(hWnd == NULL)
			show_openOix();
		else
		{
			SwitchToThisWindow(hWnd,TRUE);
			SetActiveWindow(hWnd); 
			this->showMinimized();
		}
	}
	else
	{
		PopupMessageBox(this,"当前学员模式不是考试模式",QMessageBox::Information);
		return;
	}
}
void SimuTrainee::on_CPTclicked()
{
	show_wait_contest();
}
/******************************************************************task**********************************************************************************/
void SimuTrainee::newTaskReceived()
{
	if(dlg_created_flag[NEWTASKARVDLG])
	{
		return;
	}
	TASK_INFO rcv_task_info;
	rcv_task_info.send_task_id = shm_ots_addr->data_task.send_task_id;
	rcv_task_info.task_id = shm_ots_addr->data_task.task_id;
	rcv_task_info.scene_id = shm_ots_addr->data_task.scene_id;
	rcv_task_info.fault_id = shm_ots_addr->data_task.fault_id;
	rcv_task_info.is_record = shm_ots_addr->data_task.is_record;
	rcv_task_info.status = 0;
	rcv_task_info.task_status =1;//////DZY 原来为1 改为0、、、、
	rcv_task_info.task_startable = shm_ots_addr->data_task.task_startable;
	DB_FMT data;
	data.table_id = TABLE_TASK;	
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_task.task_id=TRUE;
	data.cond_seg.seg_task.task_id = rcv_task_info.task_id;
	if (SelectRecs(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		return;
	}
	rcv_task_info.task_name = QString::fromLocal8Bit(data_task[0].name);
	if (rcv_task_info.scene_id == CONST_NEG)
		rcv_task_info.scene_name = QString::fromLocal8Bit("无工况");
	else
		rcv_task_info.scene_name = QString::fromLocal8Bit(data_task[0].scene_name);
	if (rcv_task_info.fault_id == CONST_NEG)
		rcv_task_info.fault_name = QString::fromLocal8Bit("无故障");
	else
		rcv_task_info.fault_name = QString::fromLocal8Bit(data_task[0].fault_name);
	rcv_task_info.stn_id = data_task[0].station_id;
	rcv_task_info.dev_id = data_task[0].dev_id;
	if (rcv_task_info.stn_id == 0)
		rcv_task_info.stn_name = QString::fromLocal8Bit("无厂站");
	else
		rcv_task_info.stn_name = QString::fromLocal8Bit(data_station[rcv_task_info.stn_id].station_longname);
	if (rcv_task_info.dev_id == 0)
		rcv_task_info.dev_name = QString::fromLocal8Bit("无设备");
	else
		rcv_task_info.dev_name = QString::fromLocal8Bit(data_dev[rcv_task_info.stn_id][rcv_task_info.dev_id].dev_longname);
	rcv_task_info.task_tip = QString::fromLocal8Bit(data_task[0].remark);
	rcv_task_info.type = data_task[0].type;
	//dzy new added 
	rcv_task_info.task_grp_id = data_task[0].task_grp_id; 

	FreeDbData(&data,TABLE_TRAIN_INFO);
	data.table_id = TABLE_SEND_TASK;	
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_send_task.send_task_id = TRUE;
	data.cond_seg.seg_send_task.send_task_id = rcv_task_info.send_task_id;	
	if (SelectRecs(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_SEND_TASK);
		return;
	}
	rcv_task_info.score = 0;
	rcv_task_info.time_len = data_send_task[0].time_len;
	rcv_task_info.send_time = QString::fromLocal8Bit(data_send_task[0].send_time);
	rcv_task_info.start_time = QString::fromLocal8Bit(data_send_task[0].start_time);
	rcv_task_info.end_time = QString::fromLocal8Bit(data_send_task[0].end_time);
	FreeDbData(&data,TABLE_SEND_TASK);
	if(!dlg_created_flag[NEWTASKARVDLG])
	{
		newTaskTipDialog = new CNewTaskTip(NULL);
		newTaskTipDialog->setTaskInfo(&rcv_task_info);
		newTaskTipDialog->show();
		newTaskTipDialog->activateWindow();
		moveToCenter(newTaskTipDialog);
	}
// 	else
// 	{
// 		newTaskTipDialog->setTaskInfo(&rcv_task_info);
// 		newTaskTipDialog->show();
// 		newTaskTipDialog->activateWindow();
// 		moveToCenter(newTaskTipDialog);
// 	}
	if (dlg_created_flag[TASKDLG] == false)
	{
		d_CTaskManage = new CTaskManage(this);
		d_CTaskManage->addTask(&rcv_task_info);
		d_CTaskManage->setCurTaskInfo(&rcv_task_info);
		dlg_task_clicked = true;
		subwindow[TASKDLG] = new MdiChild;
		subwindow[TASKDLG]->setWidget(d_CTaskManage);
		d_CTaskManage->setWindowTitle(QString::fromLocal8Bit("当前任务"));
		mdiArea->addSubWindow(subwindow[TASKDLG]);
		connect(d_CTaskManage,SIGNAL(closedlg()),subwindow[TASKDLG],SLOT(close()));
		subwindow[TASKDLG]->showMaximized();
	}
	else
	{
		d_CTaskManage->addTask(&rcv_task_info);
 		d_CTaskManage->setCurTaskInfo(&rcv_task_info);
		show_task();
	}
}
void SimuTrainee::setTaskTime(const QTime &time)
{	
//	taskTime->display(time.toString("hh:mm:ss"));
}
void SimuTrainee::taskFinish()
{
	taskManage->taskFinish(false);
}
void SimuTrainee::taskStarted()
{
//	buttonStart->setEnabled(false);
//	buttonStop->setEnabled(true);
}
void SimuTrainee::taskFinished()
{
//	buttonStart->setEnabled(true);
//	buttonStop->setEnabled(false);
}


void SimuTrainee::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

}
void SimuTrainee::onCheckBox(int state)
{
	if (state)
		isWarn = FALSE;
	else
		isWarn = TRUE;
}
void SimuTrainee::isQuit()
{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
	msgBox.setInformativeText(QString::fromLocal8Bit("确定要退出OTS2000学员站？"));
	msgBox.setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	//QPushButton psbOk=addButton();
	QPalette pal = msgBox.palette();
	pal.setColor( QPalette::Active, QPalette::Window, QColor(85, 85, 127,255) );
	pal.setColor( QPalette::Inactive, QPalette::Window, Qt::yellow );
	pal.setColor( QPalette::Disabled, QPalette::Window, Qt::green );
	pal.setColor( QPalette::Active, QPalette::Button, Qt::lightGray );
	pal.setColor( QPalette::Inactive, QPalette::Button, Qt::yellow );
	pal.setColor( QPalette::Disabled, QPalette::Button, Qt::green );
	//	msgBox.setPalette( pal );
	int ret = msgBox.exec();
	if (ret == QMessageBox::Ok ) 
	{
		if (loginUser->login_status)
			cleanUserInfo();
		if (taskManage->isCurTaskRun())
			taskManage->taskFinish(true);
		shm_ots_addr->data_task.task_status = 1;
		qApp->quit();
	}
	else 
		this->show();
}
void SimuTrainee::checkQuit()
{
	if (chkMin->isChecked())
	{
		closeTable->close();
		this->hide();
	} 
	else if(chkClose->isChecked())
	{
		if (loginUser->login_status)
			cleanUserInfo();
		if (taskManage->isCurTaskRun())
			taskManage->taskFinish(true);
		shm_ots_addr->data_task.task_status = 1;
		qApp->quit();
	}
}

void SimuTrainee::on_simustart()
{
// 	QSqlDatabase dbopr;
// 	ConnectHDB(dbopr,"dbstrat");
	if (shm_ots_addr->simu_mode ==2)//竞赛模式判断组的情况
	{
		int groups_id = GetUserGroupsId(login_user_id);
		if (groups_id != -1 && JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER) == FALSE)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能开始仿真，等待组长开始!"),QMessageBox::Ok);
			return;
		}
	}
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id=TRUE;
	data.cond_seg.seg_train_info.user_id=login_user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id= g_host_id;
	data.flag.flag_train_info.status=TRUE;
	data.seg.seg_train_info.status=1;
	data.flag.flag_train_info.speed=TRUE;
	data.seg.seg_train_info.speed=shm_ots_addr->speed;
	if (UpdateOneRec(&data)!=(int)true)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		QMessageBox::information( this,QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("开始仿真失败，请重新登录再尝试设置"),
			QMessageBox::Ok);
		return;
	}
	else
	{
// 		action_simustart->setEnabled(0);	
// 		action_simustop->setEnabled(1);
		//bSimuStarted = true;
		//		shm_ots_addr->runflag = 1;
		return;
	}
}

void SimuTrainee::on_simustop()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
	msgBox.setInformativeText(QString::fromLocal8Bit("您确定停止当前仿真？"));
	msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Ok ) 
	{
		///----todo....
		DB_FMT data;
		data.table_id = TABLE_TRAIN_INFO;
		InitDbData(&data,data.table_id);
		data.cond_flag.flag_train_info.user_id=TRUE;
		data.cond_seg.seg_train_info.user_id=login_user_id;
		data.cond_flag.flag_train_info.host_id = TRUE;
		data.cond_seg.seg_train_info.host_id= g_host_id;
		data.flag.flag_train_info.status=TRUE;
		data.seg.seg_train_info.status=0;
		// 		data.flag.flag_train_info.speed=TRUE;
		// 		data.rec.seg_train_info.speed=0;
		if (UpdateOneRec(&data)!=(int)true)
		{
			QMessageBox::information( this,QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("更新失败"),
				QMessageBox::Ok);
					return;
		}
		else	
		{
			shm_ots_addr->runflag = 0;
					return;
			//			setSimuSpeed.onsleep(2);
			//toolButtonStart->setEnabled(1);	
		}
	}
}
void SimuTrainee::on_action_1_8__triggered()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	simuSpeed = 1;
	setHDBSimuSpeed();
}
void SimuTrainee::on_action_1_4__triggered()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	simuSpeed = 2;
	setHDBSimuSpeed();
}
void SimuTrainee::on_action_1_2__triggered()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	simuSpeed = 4;
	setHDBSimuSpeed();
}
void SimuTrainee::on_action_1__triggered()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	simuSpeed = 8;
	setHDBSimuSpeed();
}
void SimuTrainee::on_action_2__triggered()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	simuSpeed = 16;
	setHDBSimuSpeed();
}
void SimuTrainee::on_action_4__triggered()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	simuSpeed = 32;
	setHDBSimuSpeed();
}
void SimuTrainee::on_action_8__triggered()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	simuSpeed = 64;
	setHDBSimuSpeed();
}
void SimuTrainee::setHDBSimuSpeed()
{
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id=TRUE;
	data.cond_seg.seg_train_info.user_id=login_user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id= g_host_id;
	data.flag.flag_train_info.speed=TRUE;
	data.seg.seg_train_info.speed = simuSpeed;
	//setSimuSpeed.updateHDB(&data);
	if (UpdateOneRec(&data)!=(int)TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		QMessageBox::information( this,QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("更新失败"),
			QMessageBox::Ok);
		return;
	}
}
void SimuTrainee::on_simustatechanged(unsigned char flag)
{
	if (flag == 0)
	{
		labelStatusBar->setText(QString::fromLocal8Bit("仿真停止"));
		labelStatusBar->setStyleSheet(QString::fromLocal8Bit("QLabel {background-color: rgb(255, 0, 0);	}"));
	}
	else if (flag == 1)
	{
		labelStatusBar->setText(QString::fromLocal8Bit("仿真运行"));
		labelStatusBar->setStyleSheet(QString::fromLocal8Bit("QLabel {background-color: rgb(0, 255, 0);	}"));
	}
}

void SimuTrainee::on_action_setontop(bool i)//0716 画面置顶
{
	QPoint p;
	p = this->pos();
	if (i)
	{
		this->setWindowFlags(Qt::WindowStaysOnTopHint); 
	}
	else
		this->setWindowFlags(0); 
	this->move(p.x(),p.y());
	this->show();
}

void SimuTrainee::on_action_simplestyle()
{
	qApp->setStyleSheet("");
	QFile file(":/pic/qss/pagefold.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
}
void SimuTrainee::on_action_elegantstyle()
{
	qApp->setStyleSheet("");
	QFile file(":/pic/qss/coffee.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
}
void SimuTrainee::on_action_defaultstyle()
{

	QFile file(":/pic/qss/default.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
	this->setStyleSheet("");
// 	bool i ;
// 	QFile file2(":/pic/qss/mdidialog.qss");
// 	i = file2.open(QFile::ReadOnly);
// 	QString styleSheet2 = QLatin1String(file2.readAll());
// 	qApp->setStyleSheet(styleSheet2);
// 
// 	if(d_SimuTool!=NULL)
// 	{	
// 		d_SimuTool->setStyleSheet(styleSheet);	
// 	}
}
void SimuTrainee::show_OTS2000()
{
	m_dlg = new QDialog;
	m_dlg->setWindowTitle(QString::fromLocal8Bit("关于OTS2000学员站"));
	label1 = new QLabel(m_dlg);
	label2 = new QLabel(m_dlg);
	label3 = new QLabel(m_dlg);
	label4 = new QLabel(m_dlg);
	textedt = new QTextEdit(m_dlg);
	m_layout = new QGridLayout(m_dlg);

	label1->setText(QString::fromLocal8Bit("OTS2000学员员管理系统"));
	label2->setText(QString::fromLocal8Bit("版  本 :         V 1.2         "));
	label3->setText(QString::fromLocal8Bit("版权所有:"));
	label4->setText(QString::fromLocal8Bit("北京中水科水电科技开发有限公司"));

	textedt->setText(QString::fromLocal8Bit("OTS2000系统是一个通用化设计的软件产品，实现了水电行业的仿真平台，采用自定义仿真描述语言simulog，根据设备物理原理建立模型，仿真系统与监控系统一体化设计与运行，具有丰富的培训手段。"));

	m_layout->addWidget(label1);
	m_layout->addWidget(label2);
	m_layout->addWidget(label3);
	m_layout->addWidget(label4);
	m_layout->addWidget(textedt);

	m_dlg->setLayout(m_layout);
	m_dlg->setStyleSheet("font: 75 16pt;");
	m_dlg->show();
}
void SimuTrainee::updatehostpage()
{
	if (dlg_created_flag[HOSTPAGEDLG])
	{
		d_hostpage->u_info.groups_id = shm_ots_addr->groups_id;
		d_hostpage->u_info.groups_role = shm_ots_addr->groups_role;
		d_hostpage->update();
	}
	show_hostpage();
}
void SimuTrainee::hidemenu()
{
	if (toolBar->isHidden())
	{
	//	menubar->show();
		toolBar->show();
	}
	else
	{
		//menubar->hide();
		toolBar->hide();
	}

}
void SimuTrainee::processPendingDatagrams()
{
	//comBroadcast->setVisible(true);
	comBroadcast->setStyleSheet(" ");
	//toolBar->show();
	while (udpSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(datagram.data(), datagram.size());
		QTime t = QTime::currentTime ();
		comBroadcast->insertItem(0,tr("收到广播: \" %1 \"       时间%2")
			.arg(datagram.data()).arg(t.toString("hh:mm:ss")));
// 		if (comBroadcast->count()>=1)
// 		{
			comBroadcast->setCurrentIndex(0);

		
	}
}