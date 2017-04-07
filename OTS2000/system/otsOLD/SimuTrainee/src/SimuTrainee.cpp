#include "SimuTrainee.h"

SimuTrainee::SimuTrainee(QWidget* parent)
:QMainWindow(parent)
{
	setupUi(this);
	init();
	connect(actionModeNormal, SIGNAL(triggered()), this, SLOT(changeModeNormal()));
	connect(actionModeSimply, SIGNAL(triggered()), this, SLOT(changeModeSimply()));
	connect(actionStyle1, SIGNAL(triggered()), this, SLOT(changeStyle1()));
	connect(actionStyle2, SIGNAL(triggered()), this, SLOT(changeStyle2()));
	connect(actionStyle3, SIGNAL(triggered()), this, SLOT(changeStyle3()));
	connect(actionOnTop,SIGNAL(toggled(bool)),this,SLOT(setOnTop(bool)));
	connect(displayThread, SIGNAL(sigUpdateStatusBar(const QString &)), this, SLOT(updateStatusBar(const QString &)));
	connect(displayThread, SIGNAL(sigUpdateCurTaskName(const QString &)), this, SLOT(updateCurTaskName(const QString &)));
	connect(displayThread, SIGNAL(sigUpdateSimuProgress(int)), this, SLOT(updateSimuProgress(int)));
	connect(timerButtonMsg, SIGNAL(timeout()), this, SLOT(blinkMsgButton()));
	connect(sendMessageDialog, SIGNAL(sigRecvMsg()), this, SLOT(recvMsg()));
	connect(actionExit,SIGNAL(triggered()),this,SLOT(isQuit()));
	
	connect(loginUser, SIGNAL(sigUserLogin(int,const QString &)), this, SLOT(login(int,const QString &)));
	connect(displayThread, SIGNAL(sigForceLogout()), this, SLOT(forceLogout()));
	connect(displayThread, SIGNAL(sigNewTask()), this, SLOT(newTaskReceived()));
	connect(action1_8, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(action1_4, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(action1_2, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(action1, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(action2, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(action4, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(action8, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(action16, SIGNAL(triggered()), this, SLOT(setSimuSpeed()));
	connect(menuSimuSpeed, SIGNAL(aboutToShow()), this, SLOT(setSimuSpeedMenu()));
	connect(taskManage, SIGNAL(sigTaskTimeChanged(const QTime &)), this, SLOT(setTaskTime(const QTime &)));
	connect(taskManage, SIGNAL(sigTaskStarted()), this, SLOT(taskStarted()));
	connect(taskManage, SIGNAL(sigTaskFinished()), this, SLOT(taskFinished()));
	connect(actionTaskFinished, SIGNAL(triggered()), this, SLOT(taskFinish()));
	connect(buttonTaskFinished,SIGNAL(clicked()),this,SLOT(taskFinish()));

	connect(actionLogin, SIGNAL(triggered()), this, SLOT(openLogin()));
	connect(buttonLogin,SIGNAL(clicked()),this,SLOT(openLogin()));
	connect(actionLogout, SIGNAL(triggered()), this, SLOT(logout()));
	connect(buttonLogout,SIGNAL(clicked()),this,SLOT(logout()));
	connect(buttonStart,SIGNAL(clicked()),this,SLOT(simuStart()));
	connect(buttonStop,SIGNAL(clicked()),this,SLOT(simuStop()));
	connect(actionSetTask,SIGNAL(triggered()), this, SLOT(openSetTask()));
	connect(buttonSetTask,SIGNAL(clicked()),this,SLOT(openSetTask()));
	connect(actionSetScene,SIGNAL(triggered()), this, SLOT(openSetScene()));
	connect(buttonSetScene,SIGNAL(clicked()),this,SLOT(openSetScene()));
	connect(actionSetFault,SIGNAL(triggered()), this, SLOT(openSetFault()));
	connect(buttonSetFault,SIGNAL(clicked()),this,SLOT(openSetFault()));
	connect(actionLocalOprate,SIGNAL(triggered()), this, SLOT(openLocalOperate()));
	connect(buttonLocalOprate,SIGNAL(clicked()),this,SLOT(openLocalOperate()));
	connect(buttonOpenOix,SIGNAL(clicked()),this,SLOT(openOix()));
	connect(actionTask,SIGNAL(triggered()), this, SLOT(openTask()));
	connect(buttonTask,SIGNAL(clicked()),this,SLOT(openTask()));
	connect(actionRecord,SIGNAL(triggered()), this, SLOT(openRecord()));
	connect(buttonRecord,SIGNAL(clicked()),this,SLOT(openRecord()));
	connect(buttonRevert,SIGNAL(clicked()),this,SLOT(openRevert()));
	connect(buttonSpeed,SIGNAL(clicked()),this,SLOT(openSimuSpeed()));
	connect(actionExam,SIGNAL(triggered()), this, SLOT(openExam()));
	connect(buttonExam,SIGNAL(clicked()),this,SLOT(openExam()));
	connect(actionSendMessage,SIGNAL(triggered()), this, SLOT(openSendMessage()));
	connect(buttonSendMessage,SIGNAL(clicked()),this,SLOT(openSendMessage()));
	connect(actionChangePassword, SIGNAL(triggered()), this, SLOT(openChangePassword()));
	connect(actionSetSimuTip,SIGNAL(triggered()),this,SLOT(openSetSimuTip()));
	connect(actionAbout,SIGNAL(triggered()),this,SLOT(about()));
	connect(actionHelp,SIGNAL(triggered()),this,SLOT(help()));
}
SimuTrainee::~SimuTrainee()
{
	//UnMountDB();
	CloseHDB(db_conn);
}
void SimuTrainee::init()
{
	isWarn=TRUE;
	loginUser = new CLogin(this);
	taskManage = new CTaskManage(this);
	sendMessageDialog = new CSendMessageTable(NULL);
	pSetSimuTip = new CSetSimuTip(this);
	labelStatusBar = new QLabel;
	timerButtonMsg = new QTimer;
	windowSimply = NULL;
	newTaskTipDialog = NULL;
	displayThread = new DisplayThread(this);
	createiTrayIcon();
	actionModeNormal->setShortcut(tr("CTRL+D"));
	actionModeSimply->setShortcut(tr("CTRL+S"));
	actionExit->setShortcut(tr("CTRL+Q"));
	actionHelp->setShortcut(tr("F1"));
	taskTime->setSegmentStyle(QLCDNumber::Flat);	
	QPalette pal = taskTime->palette();
	pal.setColor(QPalette::Normal,QPalette::WindowText,Qt::darkBlue);
	taskTime->setPalette(pal);
	taskTime->setNumDigits(8);
	taskTime->display("00:00:00");
	SimuProgress->setTextVisible(false);
	QActionGroup *actionGroupSpeed = new QActionGroup(this);
	actionGroupSpeed->addAction(action1_8);
	actionGroupSpeed->addAction(action1_4);
	actionGroupSpeed->addAction(action1_2);
	actionGroupSpeed->addAction(action1);
	actionGroupSpeed->addAction(action2);
	actionGroupSpeed->addAction(action4);
	actionGroupSpeed->addAction(action8);
	actionGroupSpeed->addAction(action16);
	actionGroupSpeed->setExclusive(true);
	menuSimuSpeed->setEnabled(false);
	QActionGroup *actionGroupMode = new QActionGroup(this);
	actionGroupMode->addAction(actionModeNormal);
	actionGroupMode->addAction(actionModeSimply);
	actionGroupMode->setExclusive(true);
	actionModeNormal->setChecked(true);
	QActionGroup *actionGroupStyle = new QActionGroup(this);
	actionGroupStyle->addAction(actionStyle1);
	actionGroupStyle->addAction(actionStyle2);
	actionGroupStyle->addAction(actionStyle3);
	actionGroupStyle->setExclusive(true);
	actionStyle1->setChecked(true);
	labelInfo->setText(QString::fromLocal8Bit("用户尚未登录到服务器"));
	labelCurTaskName->setText(QString::fromLocal8Bit("当前任务名：当前无任务"));
}
bool SimuTrainee::winEvent ( MSG * message,long *result)
{
	switch (message->message)
	{
		case WM_OPEN_SIMU_MAIN:			
			this->showNormal();
			this->activateWindow();
			break;
		case WM_OPEN_SETTASK:
			openSetTask();
			break;
		case WM_OPEN_SETSCENE:
			openSetScene();
			break;
		case WM_OPEN_SETFAULT:
			openSetFault();
			break;
		case WM_OPEN_CURRENTTASK:
			openTask();
			break;
		case WM_OPEN_REVERT:	
			openRevert();
			break;
		case WM_OPEN_LOCAL_OPR:	
			openLocalOperate();
			break;
		case WM_OPEN_HISTORY:	
			openRecord();
			break;
		case WM_OPEN_SENDMSG:	
			openSendMessage();
			break;
		case WM_OPEN_EXAM:	
			openExam();
			break;
	}
	return QMainWindow::winEvent(message, result);
}
void SimuTrainee::closeEvent(QCloseEvent *event)//关闭窗口最小化
{
	event->ignore();
	closeTable = new QDialog(this);
	closeTable->setAttribute(Qt::WA_DeleteOnClose);
	QCheckBox *chk = new QCheckBox(closeTable);
	chkMin = new QRadioButton(closeTable);
	chkClose = new QRadioButton(closeTable);
	QLabel *lbl = new QLabel(closeTable);
	QPushButton *butEnter = new QPushButton(closeTable);
	QPushButton *butExit = new QPushButton(closeTable);
	closeTable->setWindowTitle(QString::fromLocal8Bit("关闭提示"));
	QGridLayout *lay = new QGridLayout(closeTable);
	lay->setSizeConstraint(QLayout::SetFixedSize);
	chk->setText(QString::fromLocal8Bit("不再提醒"));
	lbl->setText(QString::fromLocal8Bit("您点击了关闭按钮，您是想："));
	chkMin->setText(QString::fromLocal8Bit("最小化到系统托盘区，不退出程序"));
	chkClose->setText(QString::fromLocal8Bit("退出程序"));
	butEnter->setText(QString::fromLocal8Bit("确定"));
	butExit->setText(QString::fromLocal8Bit("取消"));
	lay->setRowMinimumHeight(3,50);
	lay->setRowMinimumHeight(4,0);
	lay->addWidget(lbl,0,0,Qt::AlignCenter);
	lay->addWidget(chkMin,1,0,1,2);
	lay->addWidget(chkClose,2,0,1,2);
	lay->addWidget(chk,3,0);
	lay->addWidget(butEnter,3,1);
	lay->addWidget(butExit,3,2);
	closeTable->setLayout(lay);	 
	closeTable->setModal(true);
	chkMin->setChecked(TRUE);
	connect(butEnter,SIGNAL(clicked()),this,SLOT(checkQuit()));
	connect(butExit,SIGNAL(clicked()),closeTable,SLOT(reject()));
	connect(chk,SIGNAL(stateChanged(int)),this,SLOT(onCheckBox(int)));
	if (isWarn) 
	{
		closeTable->exec();
	}
	else
		this->hide();
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

//建立精简显示
void SimuTrainee::creatWindowSimply()
{	
	windowSimply = new QMainWindow;
	windowSimply->setWindowTitle(QString::fromLocal8Bit("OTS2000"));
	windowSimply->resize(40,25);
	windowSimply->setWindowFlags(Qt::WindowTitleHint | Qt::FramelessWindowHint);
	windowSimply->statusBar()->setVisible(FALSE);
	QMenu *menu = new QMenu(QString::fromLocal8Bit(" 学员站操作 "),this); 
	QAction *action = new QAction(QString::fromLocal8Bit("切换至正常模式"),menu); 
	action->setShortcut(QString::fromLocal8Bit("CTRL+D"));
	connect(action, SIGNAL(triggered()), this, SLOT(changeModeNormal()));
	menu->setStyleSheet("font: 12pt");
	menu->addMenu(menuSys);
	menu->addMenu(menuCtrl);
	menu->addMenu(menuTrain);
	menu->addMenu(menuOther);
	menu->addMenu(menuHelp);
	menu->addAction(action);
	windowSimply->menuBar()->addMenu(menu);
}
void SimuTrainee::changeModeNormal()
{
	if (isHidden())
	{
		windowSimply->close();
		windowSimply->deleteLater();
		windowSimply = NULL;
		this->show();
	}
}
void SimuTrainee::changeModeSimply()
{
	if (windowSimply == NULL)
	{
		this->hide();
		creatWindowSimply();
		windowSimply->show();
	}
}
void SimuTrainee::iconActivated(QSystemTrayIcon::ActivationReason reason) 
{ 	
	//处理鼠标单击双击事件
	switch (reason) 
	{ 
		case QSystemTrayIcon::DoubleClick: 
			this->showNormal();
			this->activateWindow();
			this->focusWidget();
			break;
		default:
			break;
	} 
} 
void SimuTrainee::createiTrayIcon()
{
	iTrayIcon=new QSystemTrayIcon(this);	
	iTrayIcon->setIcon(QIcon(":/pic/images/ots.ico"));	
	iTrayIcon->setToolTip(QString::fromLocal8Bit("OTS2000学员站"));
	createActions(); 
	createTrayIcon(); 
	iTrayIcon->show();
	connect(iTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason))); 
}
void SimuTrainee::createActions() 
{ //托盘上的 
	actionMinimize = new QAction(QString::fromLocal8Bit("最小化 (&I)"), this); 
	connect(actionMinimize, SIGNAL(triggered()), this, SLOT(hide())); 
	actionMaximize = new QAction(QString::fromLocal8Bit("最大化 (&X)"), this); 
	connect(actionMaximize, SIGNAL(triggered()), this, SLOT(showMaximized())); 
	actionRestore = new QAction(QString::fromLocal8Bit("还原 (&R)"), this); 
	connect(actionRestore, SIGNAL(triggered()), this, SLOT(showNormal())); 
	actionQuit = new QAction(QString::fromLocal8Bit("退出 (&Q)"), this); 
	actionQuit->setShortcut(QString::fromLocal8Bit("CTRL+Q"));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit())); 

} 
void SimuTrainee::createTrayIcon() 
{
	trayIconMenu = new QMenu(this); 
	trayIconMenu->addAction(actionMinimize); 
	trayIconMenu->addAction(actionMaximize); 
	trayIconMenu->addAction(actionRestore); 
	trayIconMenu->addSeparator(); 
	trayIconMenu->addAction(actionQuit); 
	iTrayIcon->setContextMenu(trayIconMenu); 
}//托盘

void SimuTrainee::changeStyle1()
{
	QFile file(":/pic/qss/default.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
}
void SimuTrainee::changeStyle2()
{
	QFile file(":/pic/qss/pagefold.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
}
void SimuTrainee::changeStyle3()
{
	QFile file(":/pic/qss/coffee.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
}
void SimuTrainee::setOnTop(bool status)
{
	if (status)
		this->setWindowFlags(Qt::WindowStaysOnTopHint); 
	else
		this->setWindowFlags(Qt::Window); 
	show();
}
void SimuTrainee::updateSimuProgress(int prgrate)
{
	SimuProgress->setValue(prgrate);
}
void SimuTrainee::updateCurTaskName(const QString &taskName)
{
	QString str = QString::fromLocal8Bit("当前任务名：%1").arg(taskName);
	labelCurTaskName->setText(str);
}
void SimuTrainee::updateStatusBar(const QString &strStatusBar)
{
	//QMainWindow::statusBar()->showMessage(strStatusBar);
	labelStatusBar->setText(strStatusBar);
	labelStatusBar->setStyleSheet("color: rgb(121, 121, 121);font: 75 12pt 宋体;");
	QMainWindow::statusBar()->addWidget(labelStatusBar);
	if (shm_ots_addr->runflag == 0 || shm_ots_addr->runflag == 2)
	{
		buttonStart->setEnabled(true);
		buttonStop->setEnabled(false);
	}
	else if (shm_ots_addr->runflag == 1)
	{
		buttonStart->setEnabled(false);
		buttonStop->setEnabled(true);
	}
}
void SimuTrainee::recvMsg()
{
	flagSendMsg = false;
	if (sendMessageDialog->isHidden())
		timerButtonMsg->start(1000);
}
void SimuTrainee::blinkMsgButton()
{
	if (!flagSendMsg)
	{
		buttonSendMessage->setStyleSheet(QString::fromUtf8("background-color: rgb(199,100,100);"));
		flagSendMsg = true;
	}
	else
	{
		buttonSendMessage->setStyleSheet("");
		flagSendMsg = false;
	}

}
/***************************************************************login*******************************************************************/
void SimuTrainee::openLogin()
{
	if (!loginUser->login_status)
	{
		loginUser->init();
		loginUser->show();
	}
}
void SimuTrainee::login(int user_id,const QString &user_name)
{
	menuSimuSpeed->setEnabled(true);
	actionChangePassword->setEnabled(true);
	buttonStart->setEnabled(true);
	buttonStop->setEnabled(false);
	sendMessageDialog->setUserId(user_id);
	labelInfo->setText(QString::fromLocal8Bit("当前登录用户：%1").arg(user_name));
	displayThread->start();
	heart_beat_user_info.user_id = user_id;
	heart_beat_user_info.host_id = g_host_id;
	strcpy(heart_beat_user_info.login_time,loginUser->login_time.toLocal8Bit().data());
	heart_beat_user_info.timeout = 0;
	heartBeatThread = new HeartBeatThread(heart_beat_user_info,this);
	heartBeatThread->start();
	updateStatusBar(QString::fromLocal8Bit("   任务状态:任务未接收   仿真状态:停止   仿真速度:%1   培训模式:培训模式").arg(QString::number(shm_ots_addr->speed/8.0)));
}
void SimuTrainee::logout()
{
	if (loginUser->login_status)
	{	
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
		msgBox.setInformativeText(QString::fromLocal8Bit("确定要退出当前用户登陆？"));
		msgBox.setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Ok ) 
			cleanUserInfo();				
	}
}
void SimuTrainee::forceLogout()
{
	if (loginUser->login_status)
	{
		PopupMessageBox(this,"当前登录用户已被管理员强制踢出!",QMessageBox::Information);
		cleanUserInfo();
	}
}
void SimuTrainee::cleanUserInfo()
{
	DB_FMT data;
	data.table_id=TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.flag.flag_train_info.login_status=TRUE;	
	data.seg.seg_train_info.login_status=0;
	data.flag.flag_train_info.status = TRUE;
	data.seg.seg_train_info.status=0;
	data.flag.flag_train_info.task_status = TRUE;
	data.seg.seg_train_info.task_status=0;
	data.flag.flag_train_info.scene_status = TRUE;
	data.seg.seg_train_info.scene_status=0;
	data.flag.flag_train_info.fault_status = TRUE;
	data.seg.seg_train_info.fault_status=0;
	data.flag.flag_train_info.user_id=TRUE;	
	data.seg.seg_train_info.user_id=loginUser->login_user_id;
	data.cond_flag.flag_train_info.host_id =TRUE;
	data.cond_seg.seg_train_info.host_id = g_host_id;
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
	}
	FreeDbData(&data,TABLE_TRAIN_INFO);
	data.table_id = TABLE_LOGIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_login_info.host_id = TRUE;
	data.cond_seg.seg_login_info.host_id = g_host_id;
	data.cond_flag.flag_login_info.login_time = TRUE;
	strcpy(data.cond_seg.seg_login_info.login_time,loginUser->login_time.toLocal8Bit().data());
	data.flag.flag_login_info.logout_time = TRUE;
	GetCurTimeStr(data.seg.seg_login_info.logout_time);
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_LOGIN_INFO);
	}
	FreeDbData(&data,TABLE_LOGIN_INFO);
	//shm_ots_addr->login_status = FALSE;
	loginUser->init();
	if (heartBeatThread->isRunning())
	{
		heartBeatThread->stop(); 
		heartBeatThread->wait();
		heartBeatThread->quit();
	}
	if (displayThread->isRunning())
	{
		//displayThread->stop();
		//displayThread->wait();
		displayThread->quit();		
	}
	labelInfo->setText(QString::fromLocal8Bit("用户尚未登录到服务器"));
	labelCurTaskName->setText(QString::fromLocal8Bit("当前任务名：当前无任务"));
	menuSimuSpeed->setEnabled(false);
	actionChangePassword->setEnabled(false);
	buttonStart->setEnabled(true);
	buttonStop->setEnabled(true);
	buttonSendMessage->setStyleSheet("");
	timerButtonMsg->stop();
	shm_ots_addr->data_scene.lockflag = FALSE;
}
void SimuTrainee::openChangePassword()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	changePasswordDialog = new CChangePassword(this);
	changePasswordDialog->show();
}
/*********************************************************************simuControl**********************************************************************/
void SimuTrainee::simuStart()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
// 	if (shm_ots_addr->simu_mode == 1)
// 	{
// 		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
// 		return;
// 	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_SIMU_START,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	int groups_id = GetUserGroupsId(loginUser->login_user_id);
	if (groups_id != -1 && JudgeUserGroupsRole(loginUser->login_user_id,GROUPS_ROLE_LEADER) == FALSE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能开始仿真，等待组长开始!"),QMessageBox::Ok);
		return;
	}
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id=TRUE;
	data.cond_seg.seg_train_info.user_id=loginUser->login_user_id;
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
void SimuTrainee::simuStop()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
// 	if (shm_ots_addr->simu_mode == 1)
// 	{
// 		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
// 		return;
// 	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_SIMU_STOP,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	int groups_id = GetUserGroupsId(loginUser->login_user_id);
	if (groups_id != -1 && JudgeUserGroupsRole(loginUser->login_user_id,GROUPS_ROLE_LEADER) == FALSE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能停止仿真，等待组长停止!"),QMessageBox::Ok);
		return;
	}
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
		data.cond_seg.seg_train_info.user_id=loginUser->login_user_id;
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
void SimuTrainee::openSimuSpeed()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (shm_ots_addr->simu_mode == 1)
	{
		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
		return;
	}
	int groups_id = GetUserGroupsId(loginUser->login_user_id);
	if (groups_id != -1 && JudgeUserGroupsRole(loginUser->login_user_id,GROUPS_ROLE_LEADER) == FALSE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能设置仿真速度，等待组长设置!"),QMessageBox::Ok);
		return;
	}
	QDialog *speedDialog = new QDialog(this);
	speedDialog->setAttribute(Qt::WA_DeleteOnClose);
	QGridLayout *layout = new QGridLayout;
	speedDialog->resize(300, 80);
	speedDialog->setWindowTitle(QString::fromLocal8Bit("仿真速度控制"));
	QLabel *label = new QLabel; 
	QSlider *horizontalSlider = new QSlider;
	QPushButton *buttonEnter = new QPushButton(QString::fromLocal8Bit("确定"));	
	QPushButton *buttonCancel = new QPushButton(QString::fromLocal8Bit("取消"));
	buttonEnter->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	buttonCancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QSpacerItem *horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalSlider->setMinimum(-3);
	horizontalSlider->setMaximum(4);
	simuSpeed = shm_ots_addr->speed;
	int val = (int)(log((double)simuSpeed)/log(2.0));
	label->setText(QString::fromLocal8Bit("%1倍速").arg(QString::number(simuSpeed/8.0)));
	horizontalSlider->setValue(val-3);
	horizontalSlider->setPageStep(1);
	horizontalSlider->setOrientation(Qt::Horizontal);
	horizontalSlider->setTickPosition(QSlider::TicksBothSides);
	layout->addWidget(horizontalSlider, 0, 0, 1, 4);
	layout->addWidget(label, 1, 0);
	layout->addItem(horizontalSpacer, 1, 1);
	layout->addWidget(buttonEnter, 1, 2);
	layout->addWidget(buttonCancel, 1, 3);
	speedDialog->setLayout(layout);	 
	speedDialog->setModal(true);
	connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setSimuSpeedLabel(int)));
	connect(this, SIGNAL(sigSpeedChanged(const QString &)),label,SLOT(setText(const QString &)));
	connect(buttonEnter, SIGNAL(clicked()), this, SLOT(setSimuSpeed()));
	connect(buttonEnter, SIGNAL(clicked()), speedDialog, SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), speedDialog, SLOT(reject()));
	speedDialog->show();	
}
void SimuTrainee::setSimuSpeedLabel(int value)
{
	double val=pow(2.0,value);
	simuSpeed = (short)(val*8);
	QString str = QString::fromLocal8Bit("%1倍速").arg(QString::number(val));
	emit sigSpeedChanged(str);
}
void SimuTrainee::setSimuSpeedMenu()
{
	switch(shm_ots_addr->speed)
	{
		case 1:
			action1_8->setChecked(true);
			break;
		case 2:
			action1_4->setChecked(true);
			break;
		case 4:
			action1_2->setChecked(true);
			break;
		case 8:
			action1->setChecked(true);
			break;
		case 16:
			action2->setChecked(true);
			break;
		case 32:
			action4->setChecked(true);
			break;
		case 64:
			action8->setChecked(true);
			break;
		case 128:
			action16->setChecked(true);
			break;
		default:
			break;
	}
}
void SimuTrainee::setSimuSpeed()
{
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_SIMU_SPEED,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	QAction *action = qobject_cast<QAction*>(sender());
	if (action == action1_8)
		simuSpeed = 1;
	else if (action == action1_4)
		simuSpeed = 2;
	else if (action == action1_2)
		simuSpeed = 4;
	else if (action == action1)
		simuSpeed = 8;
	else if (action == action2)
		simuSpeed = 16;
	else if (action == action4)
		simuSpeed = 32;
	else if (action == action8)
		simuSpeed = 64;
	else if (action == action16)
		simuSpeed = 128;
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id=TRUE;
	data.cond_seg.seg_train_info.user_id=loginUser->login_user_id;
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
/*********************************************************************dialog show*************************************************************************/
void SimuTrainee::openOix()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	HWND   hWnd=FindWindow(NULL,L"OIX/H9000          北京中水科水电科技开发有限公司");	
	if(hWnd == NULL)
	{
		QDir Path;
		QString appPath = qApp->applicationDirPath();
		QString curDrive = appPath.mid(0,2);//得到当前程序的盘符 例如“C:”
		Path.setPath(curDrive+QString("\\home\\ems\\h9000\\bin"));
		QString strFile = Path.absoluteFilePath("oix.exe");
		QProcess* pProcess=new QProcess(this);
		pProcess->startDetached(strFile);
	}
	else
	{
		SwitchToThisWindow(hWnd,TRUE);
		SetActiveWindow(hWnd); 
		showMinimized();
	}
}
void SimuTrainee::openRevert()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_REVERT,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	revertPlayerDialog = new CRevertPlayer(NULL);
	revertPlayerDialog->show();
}
void SimuTrainee::openTask()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	taskManage->show();
}
void SimuTrainee::openRecord()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
// 	if (shm_ots_addr->simu_mode == 1)
// 	{
// 		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
// 		return;
// 	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_RECORD,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	recordDialog = new CHistoryRecTable(this);
	recordDialog->init();
	recordDialog->show();
	recordDialog->comboSearchType->setCurrentIndex(0);
	recordDialog->comboSearchType->setEnabled(false);
	recordDialog->comboSearchName->setCurrentIndex(recordDialog->comboSearchName->findData(QVariant(loginUser->login_user_id)));
	recordDialog->comboSearchName->setEnabled(false);
	recordDialog->setUserId(loginUser->login_user_id);
}
void SimuTrainee::openLocalOperate()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_LOCAL_OPR,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	localOprateDialog = new CLocalOprate(this);
	localOprateDialog->show();
}
void SimuTrainee::openSendMessage()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
// 	if (shm_ots_addr->simu_mode == 1)
// 	{
// 		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
// 		return;
// 	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_SEND_MESSAGE,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	sendMessageDialog->show();
	buttonSendMessage->setStyleSheet("");
	timerButtonMsg->stop();
}
void SimuTrainee::openSetScene()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
// 	if (shm_ots_addr->simu_mode == 1)
// 	{
// 		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
// 		return;
// 	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_SET_SCENE,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	int groups_id = GetUserGroupsId(loginUser->login_user_id);
	if (groups_id != -1 && JudgeUserGroupsRole(loginUser->login_user_id,GROUPS_ROLE_LEADER) == FALSE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能设置工况!"),QMessageBox::Ok);
		return;
	}			
	setSceneDialog = new CSetSceneFaultTable(this);
	setSceneDialog->initScene();
	setSceneDialog->show();
	setSceneDialog->setSceneModel();
	for (int i=0;i<setSceneDialog->listUserAndGroups->count();i++)
	{
		if (groups_id == -1)
		{
			int user_id = setSceneDialog->listUserAndGroups->item(i)->data(3).toInt();
			if (user_id == loginUser->login_user_id)
			{
				setSceneDialog->listUserAndGroups->setCurrentItem(setSceneDialog->listUserAndGroups->item(i));
				setSceneDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
				break;
			}
		}
		else
		{
			int m_groups_id = setSceneDialog->listUserAndGroups->item(i)->data(3).toInt();
			if (m_groups_id == groups_id)
			{
				setSceneDialog->listUserAndGroups->setCurrentItem(setSceneDialog->listUserAndGroups->item(i));
				setSceneDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
				break;
			}
		}
	}
	setSceneDialog->buttonRefresh->setEnabled(false);
	setSceneDialog->listUserAndGroups->setEnabled(false);
}
void SimuTrainee::openSetFault()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
// 	if (shm_ots_addr->simu_mode == 1)
// 	{
// 		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
// 		return;
// 	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_SET_FAULT,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	int groups_id = GetUserGroupsId(loginUser->login_user_id);
	if (groups_id != -1 && JudgeUserGroupsRole(loginUser->login_user_id,GROUPS_ROLE_LEADER) == FALSE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能设置故障!"),QMessageBox::Ok);
		return;
	}			
	setFaultDialog = new CSetSceneFaultTable(this);
	setFaultDialog->initFault();
	setFaultDialog->show();
	setFaultDialog->setFaultModel();
	for (int i=0;i<setFaultDialog->listUserAndGroups->count();i++)
	{
		if (groups_id == -1)
		{
			int user_id = setFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
			if (user_id == loginUser->login_user_id)
			{
				setFaultDialog->listUserAndGroups->setCurrentItem(setFaultDialog->listUserAndGroups->item(i));
				setFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
				break;
			}
		}
		else
		{
			int m_groups_id = setFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
			if (m_groups_id == groups_id)
			{
				setFaultDialog->listUserAndGroups->setCurrentItem(setFaultDialog->listUserAndGroups->item(i));
				setFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
				break;
			}
		}
	}
	setFaultDialog->buttonRefresh->setEnabled(false);
	setFaultDialog->listUserAndGroups->setEnabled(false);
}
void SimuTrainee::openSetTask()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
// 	if (shm_ots_addr->simu_mode == 1)
// 	{
// 		PopupMessageBox(this,"用户处于考试模式，不能进行此操作",QMessageBox::Warning);
// 		return;
// 	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_SET_TASK,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	int groups_id = GetUserGroupsId(loginUser->login_user_id);
	if (groups_id != -1 && JudgeUserGroupsRole(loginUser->login_user_id,GROUPS_ROLE_LEADER) == FALSE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户组身份为组员，不能下达任务!"),QMessageBox::Ok);
		return;
	}			
	setTaskDialog = new CSetTaskTable(this);
	setTaskDialog->show();
	for (int i=0;i<setTaskDialog->listUserAndGroups->count();i++)
	{
		int user_id = setTaskDialog->listUserAndGroups->item(i)->data(3).toInt();
		if (user_id == loginUser->login_user_id)
		{
			setTaskDialog->listUserAndGroups->setCurrentItem(setTaskDialog->listUserAndGroups->item(i));
			break;
		}
	}
	setTaskDialog->listUserAndGroups->setEnabled(false);
}
void SimuTrainee::openExam()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,StudentPrivType::PRIV_EXAM,shm_ots_addr->simu_mode))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	QString program = QString::fromLocal8Bit("/home/ems/h9000/bin/exam/examination_total_form.exe");
	QStringList arguments;
	arguments.append(QString::fromLocal8Bit("otsargs"));
	arguments.append(loginUser->login_user_name);
	arguments.append(loginUser->login_user_password);
	QProcess *processExam = new QProcess(this);
	processExam->start(program,arguments);
}
void SimuTrainee::openSetSimuTip()
{
	pSetSimuTip->init();
	pSetSimuTip->show();
}
void SimuTrainee::about()
{
	QMessageBox::about(this, tr("关于 OTS2000"),
		tr("<p>The <b>OTS2000</b> simulation station " \
		"helps to train workers at hydro-plant to learn how to " \
		"use the complicated H9000 system effiently..\n"\
		"  Copyright by BITC.</p>"));
}
void SimuTrainee::help()
{	
	QString filePath = QString::fromLocal8Bit("/home/ems/h9000/bin/help/help.doc");
	QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).absoluteFilePath())); 
}

/******************************************************************task**********************************************************************************/
void SimuTrainee::newTaskReceived()
{
	TASK_INFO rcv_task_info;
	rcv_task_info.send_task_id = shm_ots_addr->data_task.send_task_id;
	rcv_task_info.task_id = shm_ots_addr->data_task.task_id;
	rcv_task_info.scene_id = shm_ots_addr->data_task.scene_id;
	rcv_task_info.fault_id = shm_ots_addr->data_task.fault_id;
	rcv_task_info.is_record = shm_ots_addr->data_task.is_record;
	rcv_task_info.status = 0;
	rcv_task_info.task_status = 1;
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
	newTaskTipDialog = new CNewTaskTip(NULL);
	newTaskTipDialog->setTaskInfo(&rcv_task_info);
	newTaskTipDialog->show();
	newTaskTipDialog->activateWindow();
	moveToCenter(newTaskTipDialog);
}
void SimuTrainee::setTaskTime(const QTime &time)
{	
	taskTime->display(time.toString("hh:mm:ss"));
}
void SimuTrainee::taskFinish()
{
	taskManage->taskFinish(false);
}
void SimuTrainee::taskStarted()
{
	buttonStart->setEnabled(false);
	buttonStop->setEnabled(true);
}
void SimuTrainee::taskFinished()
{
	buttonStart->setEnabled(true);
	buttonStop->setEnabled(false);
}