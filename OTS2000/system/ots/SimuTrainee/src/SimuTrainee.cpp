#include "SimuTrainee.h"

SimuTrainee::SimuTrainee(QWidget* parent)
:QMainWindow(parent)
{
	flag_simutrainee = true;
	setupUi(this);
// 	this->setMaximumSize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
// 	this->setWindowFlags(Qt::Window);
	d_SimuTool = new SimuTool(this);
	connect(this,SIGNAL(sigLanguageChanged( int )),d_SimuTool,SLOT(setLanguage( int )));
	d_SimuTool->hide();
	setToolBoxConnect();
	dockWidget->setWidget(d_SimuTool);
	dockWidget->setFeatures(QDockWidget::DockWidgetClosable);
	dockWidget->setWindowTitle(tr("侧工具箱"));
	taskManage = new CTaskManage;
	dlg_task_clicked = false;

	sendMessageDialog = new CSendMessageTable(NULL);
	connect(sendMessageDialog, SIGNAL(sigRecvMsg()), this, SLOT(recvMsg()));
	sendMessageDialog->hide();
	dlg_msg_clicked = false;
	//connect(sendMessageDialog, SIGNAL(sigRecvMsg()), this, SLOT(recvMsg()));
	timerButtonMsg = new QTimer;
	connect(timerButtonMsg, SIGNAL(timeout()), this, SLOT(blinkMsgButton()));

	displayThread = NULL;
	m_NetstateThread = NULL;
	heartBeatThread = NULL;
	m_threadmsgbox = new QMessageBox;

	//UDP
	connect(udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
	mdiArea = new MdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setViewMode(QMdiArea::TabbedView);
	mdiArea->setTabShape(QTabWidget::Triangular);
	mdiArea->setObjectName("m_mdiArea");
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),	this, SLOT(updateMenus()));
	windowMapper = new QSignalMapper(this);
	connect(windowMapper, SIGNAL(mapped(QWidget *)),this, SLOT(setActiveSubWindow(QWidget *)));
	connect(this,SIGNAL(sigModeChanged(int )),this,SLOT(on_mode_changed(int)));

	actionBaseinfo = new 	QAction(this); 
	QIcon iconinfo;
	iconinfo.addFile(QString::fromUtf8(":/pic/images/cfguser.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionBaseinfo->setIcon(iconinfo);
	actionBaseinfo->setToolTip(tr("个人信息"));
	actionHostPage = new QAction(this)/*(tr("主页"),this)*/; 
	QIcon iconhost;
	iconhost.addFile(QString::fromUtf8(":/pic/images/hosts.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionHostPage->setIcon(iconhost);
	actionHostPage->setToolTip(tr("主页"));
	actionToolbox = new 	QAction(this); 
	QIcon icontool;
	icontool.addFile(QString::fromUtf8(":/pic/images/toolbox.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionToolbox->setIcon(icontool);
	actionToolbox->setToolTip(tr("侧工具箱"));
	tbNewSms = new 	QToolButton; 
	labelStatusBar=new QLabel;
	QIcon icon5;
	icon5.addFile(QString::fromUtf8(":/pic/images/chat.png"), QSize(), QIcon::Normal, QIcon::Off);
	tbNewSms->setIcon(icon5);
	tbNewSms->setToolTip(tr("即时通讯"));
	tbNewSms->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	tbNewSms->setWhatsThis(tr("即时通讯"));
	//设置快捷键
/*	action_CTRL_H->setShortcut(tr("F2"));*/
// 	action_logout->setShortcut(tr("CTRL+Q"));
// 	action_help->setShortcut(tr("F1"));

 	connect(action_oprrecord, SIGNAL(triggered()), this, SLOT(show_record()));
	connect(action_studyrecord, SIGNAL(triggered()), this, SLOT(show_studyrecord()));
 	connect(action_hisscoe, SIGNAL(triggered()), this, SLOT(show_userscore()));
	connect(action_logout, SIGNAL(triggered()), this, SLOT(show_logout()));
 	connect(action_simustart, SIGNAL(triggered()), this, SLOT(on_simustart()));
 	connect(action_simustop, SIGNAL(triggered()), this, SLOT(on_simustop()));
 	connect(action0_125X, SIGNAL(triggered()), this, SLOT(on_action_1_8__triggered()));
 	connect(action0_25X, SIGNAL(triggered()), this, SLOT(on_action_1_4__triggered()));
 	connect(action0_5X, SIGNAL(triggered()), this, SLOT(on_action_1_2__triggered()));
 	connect(action1X, SIGNAL(triggered()), this, SLOT(on_action_1__triggered()));
 	connect(action2X, SIGNAL(triggered()), this, SLOT(on_action_2__triggered()));
	connect(action4X, SIGNAL(triggered()), this, SLOT(on_action_4__triggered()));
	connect(action8X, SIGNAL(triggered()), this, SLOT(on_action_8__triggered()));
    connect(action_otheropr, SIGNAL(triggered()), this, SLOT(show_localOprate()));
 	connect(action_taketool, SIGNAL(triggered()), this, SLOT(show_localOprate1()));
  	connect(action_report, SIGNAL(triggered()), this, SLOT(show_localOprate2()));
 	connect(action_OTS2000, SIGNAL(triggered()), this, SLOT(show_OTS2000()));
	connect(action_help, SIGNAL(triggered()), this, SLOT(show_help()));
  	connect(action_loadscene, SIGNAL(triggered()), this, SLOT(show_setScene()));
  	connect(action_setfault, SIGNAL(triggered()), this, SLOT(show_setFault()));
	connect(action_settask, SIGNAL(triggered()), this, SLOT(show_settask()));
	connect(action_CTRL_H, SIGNAL(triggered()), this, SLOT(hidemenu()));
	//画面
	connect(action_ontop,SIGNAL(toggled(bool)),this,SLOT(on_action_setontop(bool)));
	connect(action_simpstyle, SIGNAL(triggered()), this, SLOT(on_action_simplestyle()));
	connect(action_elegantstyle, SIGNAL(triggered()), this, SLOT(on_action_elegantstyle()));
	connect(action_defaultstyle, SIGNAL(triggered()), this, SLOT(on_action_defaultstyle()));
	//language
	flag_langueage = 0 ;
	action_Chinese->setCheckable(true);
	action_English->setCheckable(true);
	action_Chinese->setChecked(true);
	action_English->setChecked(false);
	connect(action_Chinese, SIGNAL(triggered()), this, SLOT(loadChinese()));
	connect(action_English, SIGNAL(triggered()), this, SLOT(loadEnglish()));
	init();		
	on_action_defaultstyle();
	createiTrayIcon();
	createToolBars();
}
SimuTrainee::~SimuTrainee()
{
	//UnMountDB();
	flag_simutrainee = false;
	delete sendMessageDialog;
	CloseHDB(db_conn);
}
void SimuTrainee::init()
{
	pSetSimuTip = new CSetSimuTip(this);
	//首先初始化登陆界面
	if(loginUser==NULL)
	{
		loginUser = new CLogin;
		groups_id = -1;
		trainmode = -1;
		loginUser->init();
		role_lerder = false;
		moveToCenter(loginUser);
		loginUser->show();
		connect(loginUser,SIGNAL(sigUserLogin(int,const QString &,int)),this,SLOT(on_userloged(int,const QString &,int)));
	}
	else
	{
		//清空主窗口界面

		//重现登录窗口
		loginUser->init();
		moveToCenter(loginUser);
		loginUser->show();
	}
}
void SimuTrainee::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
void SimuTrainee::show_toolbox()
{
	if(loginUser->login_status)
	{	
		if(d_SimuTool!=NULL)
		{	
			d_SimuTool->setWindowFlags(Qt::FramelessWindowHint);
	//		d_SimuTool->setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint); //无任务栏显示
			d_SimuTool->move(1,1);
			d_SimuTool->show();
		}
// 		if(d_SimuTool!=NULL)
// 		{	
// 			d_SimuTool->setWindowFlags(Qt::FramelessWindowHint);
// 			//		d_SimuTool->setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint); //无任务栏显示
// 			d_SimuTool->move(2,2);
// 			d_SimuTool->show();
//		}
	}
}
void SimuTrainee::show_hide_toolbox()
{
	if(loginUser->login_status)
	{	
// 		if(d_SimuTool!=NULL)
// 		{	
// 			if (d_SimuTool->is_hiden==false)
// 			{			
// 				d_SimuTool->hide();
// 				//this->move(0,0);		
// 				//this->resize(m_ActScreenX ,m_ActScreenY);
// 				//this->show();
// 			}
// 			else
// 			{
// 				int dtoolbox_with = d_SimuTool->width();
// 				int dtoolbox_height = d_SimuTool->height();				
// 				this->resize(m_ActScreenX - dtoolbox_with-8,m_ActScreenY);							
// 				d_SimuTool->setWindowFlags(Qt::FramelessWindowHint);
// 				d_SimuTool->resize(dtoolbox_with,m_ActScreenY+25);	
// 				d_SimuTool->move(1,1);	
// 				this->move(d_SimuTool->x()+ dtoolbox_with+2,2);	
// 				d_SimuTool->show();
// 				show_toolbox();
// 			}
// 		}
		if (dockWidget->isHidden())
		{
			dockWidget->show();
		}
		else
			dockWidget->hide();
	}
}

void SimuTrainee::setToolBoxConnect()
{
	if(d_SimuTool==NULL)
	{
		return;
	}
	else
	{
		connect(d_SimuTool, SIGNAL(button_userinfo_clicked()), this, SLOT(show_userinfo()));
		connect(d_SimuTool, SIGNAL(button_record_clicked()), this, SLOT(show_record()));
		connect(d_SimuTool, SIGNAL(button_userscore_clicked()), this, SLOT(show_userscore()));
		connect(d_SimuTool, SIGNAL(button_logout_clicked()), this, SLOT(show_logout()));
		connect(d_SimuTool, SIGNAL(button_task_clicked()), this, SLOT(show_task()));
		connect(d_SimuTool, SIGNAL(button_revert_clicked()), this, SLOT(show_revert()));
		connect(d_SimuTool, SIGNAL(button_localOprate_clicked()), this, SLOT(show_localOprate()));
		connect(d_SimuTool, SIGNAL(button_openOix_clicked()), this, SLOT(show_openOix()));
		connect(d_SimuTool, SIGNAL(button_Exam_clicked()), this, SLOT(show_Exam()));
		connect(d_SimuTool, SIGNAL(button_simuControl_clicked()), this, SLOT(show_simuControl()));
		connect(d_SimuTool, SIGNAL(button_setScene_clicked()), this, SLOT(show_setScene()));
		connect(d_SimuTool, SIGNAL(button_setFault_clicked()), this, SLOT(show_setFault()));
		connect(d_SimuTool, SIGNAL(button_sendMessage_clicked()), this, SLOT(show_sendMessage()));
		connect(d_SimuTool, SIGNAL(button_doccuments_clicked()), this, SLOT(show_doc()));
		connect(d_SimuTool, SIGNAL(button_setDesplay_clicked()), this, SLOT(show_setDesplay()));
		connect(d_SimuTool, SIGNAL(button_help_clicked()), this, SLOT(show_help()));
	}
}

void SimuTrainee::loadChinese()
{
	flag_langueage = 0;//0:汉语 1：english
	action_Chinese->setCheckable(true);
	action_Chinese->setChecked(true);
	action_English->setChecked(false);
// 	if(lang_is_chinese)
// 		return;
	QTranslator translator;
	translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	lang_is_chinese=true;
	lang_is_english=false;
	retranslateUi(this);
	this->show();
	emit sigLanguageChanged(flag_langueage);
}

void SimuTrainee::loadEnglish()
{
	flag_langueage = 1;
	action_English->setCheckable(true);
	action_English->setChecked(true);
	action_Chinese->setChecked(false);
// 	if(lang_is_english)
// 		return;
	QTranslator translator;
	translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	lang_is_english=true;
	lang_is_chinese=false;
	retranslateUi(this);
	this->show();
	emit sigLanguageChanged(flag_langueage);
}
void SimuTrainee:: on_userloged(int user_id,const QString &user_name,int mode)
{
	//d_SimuTool->setMaximumHeight(m_ActScreenY);
	shm_ots_addr->data_task.flag_new_send_task = 0;
	//show_toolbox();
// 	int dtoolbox_with = d_SimuTool->width();
// 	int dtoolbox_height = d_SimuTool->height();
//	this->setFixedSize(m_ActScreenX - dtoolbox_with-20, dtoolbox_height-40);
	this->showMaximized();
// 	this->setMinimumSize(m_ActScreenX-dtoolbox_with-30, dtoolbox_height);
// 	this->setMaximumSize(m_ActScreenX, m_ActScreenY);
// 	this->move(dtoolbox_with+8,0);
// 	this->show();

	groups_id = GetUserGroupsId(user_id);
	login_user_type = GetUserTypeId(user_id);
	role_lerder = JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER);
	heart_beat_user_info.user_id = user_id;
	heart_beat_user_info.host_id = g_host_id;
	strcpy(heart_beat_user_info.login_time,loginUser->login_time.toLocal8Bit().data());
	heart_beat_user_info.timeout = 0;
	login_user_longname = user_name;

	sendMessageDialog->setUserId(user_id);
	initUdpsocket();
////////////DZY REMOVED FOR DEBUG 20140728////////////////////////////
// 	if (heartBeatThread == NULL)
// 	{
// 		heartBeatThread = new HeartBeatThread(heart_beat_user_info,this);
// 		heartBeatThread->start();	
// 	}
// 	else
// 	{
// 		heartBeatThread->stop_flag = false;
// 		heartBeatThread->start();	
// 	}
	 //m_mornitor_controlor =new Controler;//dzy new added for teacher watch
	if (displayThread == NULL)
	{
		displayThread = new DisplayThread(this);
		connect(displayThread, SIGNAL(sigForceLogout()), this, SLOT(forceLogout()));
		connect(displayThread, SIGNAL(sigNewTask()), this, SLOT(newTaskReceived()));
		connect(displayThread, SIGNAL(sigTrainmodelchanged(int)), this, SLOT(change_ots_mode(int)));
		connect(displayThread, SIGNAL(sigSimustatechanged(unsigned char)), this, SLOT(on_simustatechanged(unsigned char)));
		connect(displayThread,SIGNAL(sigGroupinfoChanged()),this,SLOT(updatehostpage()));
		displayThread->start();
	}
	else
	{
		displayThread->stop_flag = false;
		connect(displayThread, SIGNAL(sigForceLogout()), this, SLOT(forceLogout()));
		connect(displayThread, SIGNAL(sigNewTask()), this, SLOT(newTaskReceived()));
		connect(displayThread, SIGNAL(sigTrainmodelchanged(int)), this, SLOT(change_ots_mode(int)));
		displayThread->start();	
	}
////////////DZY REMOVED FOR DEBUG 20140728////////////////////////////
// 	if (m_NetstateThread == NULL)
// 	{	
// 		m_NetstateThread = new NetstateThread();
// 		m_NetstateThread->start();
// 		connect(m_NetstateThread, SIGNAL(showtryconeectfail()),this, SLOT(show_tryconnectfail()));
// 		connect(m_NetstateThread, SIGNAL(showtryconeect(int,bool)),this, SLOT(show_tryconnec(int,bool)));
// 	}
// 	else
// 	{
// 		m_NetstateThread->stop_flag = false;
// 		m_NetstateThread->start();	
// 		connect(m_NetstateThread, SIGNAL(showtryconeect(int,bool)),this, SLOT(show_tryconnec(int,bool)));
// 		connect(m_NetstateThread, SIGNAL(showtryconeectfail()),this, SLOT(show_tryconnectfail()));
// 	}
	sendMessageDialog->setUserId(user_id);
	show_hostpage();
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
	data.seg.seg_train_info.user_id=login_user_id;
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
	shm_ots_addr->runflag = 0;//dzy new added simu stop when quit.
	//shm_ots_addr->login_status = FALSE;
	loginUser->init();

	if (dlg_created_flag[TASKDLG])
	{
		if (shm_ots_addr->data_task.task_status==2)
		{
			d_CTaskManage->taskFinish(true);
		}
		d_CTaskManage->close();
	}
	groups_id = -1;
	role_lerder = false;
	trainmode = -1;
	dlg_task_clicked = false;
	dlg_msg_clicked = false;
	////////////DZY REMOVED FOR DEBUG 20140728////////////////////////////
// 	if (heartBeatThread->isRunning())
// 	{
// 		heartBeatThread->stop(); 
// 		heartBeatThread->wait();
// 		heartBeatThread->quit();
// 	//	heartBeatThread->deleteLater();
// 	}
	if (displayThread->isRunning())
	{
		disconnect(displayThread, SIGNAL(sigForceLogout()), this, SLOT(forceLogout()));
		disconnect(displayThread, SIGNAL(sigNewTask()), this, SLOT(newTaskReceived()));
		disconnect(displayThread, SIGNAL(sigTrainmodelchanged(int)), this, SLOT(change_ots_mode(int)));
		disconnect(displayThread, SIGNAL(sigSimustatechanged(unsigned char)), this, SLOT(on_simustatechanged(unsigned char)));
		displayThread->stop_flag = true;
		displayThread->quit();	
		//displayThread->deleteLater();
	}
////////////DZY REMOVED FOR DEBUG 20140728////////////////////////////
// 	if (m_NetstateThread->isRunning())
// 	{
// 		disconnect(m_NetstateThread, SIGNAL(showtryconeect(int)),this, SLOT(show_tryconnec(int)));
// 		disconnect(m_NetstateThread, SIGNAL(showtryconeectfail()),this, SLOT(show_tryconnectfail()));
// 		m_NetstateThread->stop_flag = true;
// 		m_NetstateThread->stop();	
// 		m_NetstateThread->quit();	
// // 		m_NetstateThread->deleteLater();
// 	}
		//d_SimuTool->close();
		mdiArea->closeAllSubWindows();

	//buttonStart->setEnabled(true);
	//buttonStop->setEnabled(true);
	//buttonSendMessage->setStyleSheet("");
	timerButtonMsg->stop();
	shm_ots_addr->data_scene.lockflag = FALSE;
	comBroadcast->clear();
	//m_mornitor_controlor->~Controler();
}
void SimuTrainee::show_hostpage()
{
	if (dlg_created_flag[HOSTPAGEDLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[HOSTPAGEDLG]);
		return;
	}
	else 
	{
		subwindow[HOSTPAGEDLG] = new MdiChild;
		d_hostpage = new Hostpage(this);
/*		subwindow[HOSTPAGEDLG]->setStyleSheet("border-color: rgb(0, 255, 0);background-color: rgb(171, 212, 255);");*/
		if(dlg_created_flag[USERINFODLG] == true)
			d_hostpage->u_info = d_userinfo->u_info;
		else			
		{
			d_userinfo = new UserInfo(this);
			d_userinfo->getuserinfo();
			d_hostpage->u_info.user_range = d_userinfo->u_info.user_range;
			d_hostpage->u_info.user_type_id = d_userinfo->u_info.user_type_id;
			d_hostpage->u_info.groups_id = d_userinfo->u_info.groups_id;
			strcpy(d_hostpage->u_info.groups_name ,d_userinfo->u_info.groups_name);
			d_userinfo->~UserInfo();
		}
		d_hostpage->login_user_longname = login_user_longname;
		d_hostpage->update();
		connect(d_hostpage,SIGNAL(STclicked()),this,SLOT(on_STclicked()));
		connect(d_hostpage,SIGNAL(SOclicked()),this,SLOT(on_SOclicked()));
		connect(d_hostpage,SIGNAL(ETclicked()),this,SLOT(on_ETclicked()));
		connect(d_hostpage,SIGNAL(EOclicked()),this,SLOT(on_EOclicked()));
		connect(d_hostpage,SIGNAL(CPTclicked()),this,SLOT(on_CPTclicked()));
		connect(d_hostpage,SIGNAL(sig_showrecords()),this,SLOT(show_record()));
		connect(d_hostpage,SIGNAL(sig_showstudyrecords()),this,SLOT(show_studyrecord()));
		connect(d_hostpage,SIGNAL(sig_showscores()),this,SLOT(show_userscore()));
		connect(d_hostpage,SIGNAL(sig_showpersoninfo()),this,SLOT(show_userinfo()));

		subwindow[HOSTPAGEDLG]->setWidget(d_hostpage);
		mdiArea->addSubWindow(subwindow[HOSTPAGEDLG]);
	//	d_hostpage->setStyleSheet("background-image: url(:/pic/images/background/hostpagebackround.png);");
		connect(this,SIGNAL(sigLanguageChanged( int )),d_hostpage,SLOT(setLanguage( int )));
		subwindow[HOSTPAGEDLG]->showMaximized();
		subwindow[HOSTPAGEDLG]->setOption(QMdiSubWindow::RubberBandResize,false);
	}		
}

MdiChild *SimuTrainee::activeMdiChild()
{
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
		return qobject_cast<MdiChild *>(activeSubWindow);
	return 0;
}
//======================SIMUTOOL SLOTS:=======================
void SimuTrainee::show_userinfo()
{
	if (dlg_created_flag[USERINFODLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[USERINFODLG]);
		return;
	}
	else 
	{
		//MdiChild *child = new MdiChild;
		subwindow[USERINFODLG] = new MdiChild;
		d_userinfo = new UserInfo(this);
		d_userinfo->getuserinfo();

		QSize *sizemd = new QSize(mdiArea->size());
		int i =sizemd->width();
		int j = sizemd->height();
		d_userinfo->resize(i,j); 
		subwindow[USERINFODLG]->setWidget(d_userinfo);
//		subwindow[USERINFODLG]->setStyleSheet("background-color: rgb(200, 250, 250);");
		mdiArea->addSubWindow(subwindow[USERINFODLG]);
		connect(d_userinfo,SIGNAL(closedlg()),subwindow[USERINFODLG],SLOT(close()));
		connect(d_userinfo,SIGNAL(sigLanguageChanged( int )),d_userinfo,SLOT(setLanguage( int )));
		subwindow[USERINFODLG]->showMaximized();
	//	subwindow[USERINFODLG]->setOption(QMdiSubWindow::RubberBandResize,false);
				d_userinfo->show();
	}		
}
void SimuTrainee::show_record()
{	
	if(shm_ots_addr->simu_mode != 0 )
	{
		PopupMessageBox(this,"当前模式不可查询历史操作记录",QMessageBox::Information);
		return;
	}

	if (dlg_created_flag[HISTORYRECDLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[HISTORYRECDLG]);
	}
	else 
	{
		subwindow[HISTORYRECDLG] = new MdiChild;
		d_historyrec = new CHistoryRecTable(this);
		d_historyrec->task_user_id = login_user_id;
		d_historyrec->setUserId(login_user_id);
		d_historyrec->single_user_show();	
		subwindow[HISTORYRECDLG]->setWidget(d_historyrec);
		mdiArea->addSubWindow(subwindow[HISTORYRECDLG]);	
// 		subwindow[HISTORYRECDLG]->setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
// 		subwindow[HISTORYRECDLG]->setWindowFlags(windowFlags() &~ Qt::WindowCloseButtonHint);
		connect(this,SIGNAL(sigLanguageChanged( int )),d_historyrec,SLOT(setLanguage( int )));
		subwindow[HISTORYRECDLG]->showMaximized();
	}
}
void SimuTrainee::show_studyrecord()
{
	if(shm_ots_addr->simu_mode != 0 )
	{
		PopupMessageBox(this,"当前模式不可查询历史操作记录",QMessageBox::Information);
		return;
	}

	if (dlg_created_flag[STUDYRECDLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[HISTORYRECDLG]);
	}
	else 
	{
		subwindow[STUDYRECDLG] = new MdiChild;
		d_studyrec = new CStudyRecTable(this);
		d_studyrec->task_user_id = login_user_id;
		d_studyrec->setUserId(login_user_id);
		d_studyrec->single_user_show();	
		subwindow[STUDYRECDLG]->setWidget(d_studyrec);
		mdiArea->addSubWindow(subwindow[STUDYRECDLG]);	
		// 		subwindow[HISTORYRECDLG]->setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
		// 		subwindow[HISTORYRECDLG]->setWindowFlags(windowFlags() &~ Qt::WindowCloseButtonHint);
		connect(this,SIGNAL(sigLanguageChanged( int )),d_studyrec,SLOT(setLanguage( int )));
		subwindow[STUDYRECDLG]->showMaximized();
	}
}
void SimuTrainee::show_userscore()
{
	if(shm_ots_addr->simu_mode != 0 )
	{
		PopupMessageBox(this,"当前模式不可查询成绩",QMessageBox::Information,false);
		return;
	}
	if (dlg_created_flag[USERSCOREDLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[USERSCOREDLG]);
		return;
	}
	else 
	{
		subwindow[USERSCOREDLG] = new MdiChild;
		d_CScoreRec = new CScoreRec(this);
		subwindow[USERSCOREDLG]->setWidget(d_CScoreRec);
		connect(this,SIGNAL(sigLanguageChanged( int )),d_CScoreRec,SLOT(setLanguage( int )));
		mdiArea->addSubWindow(subwindow[USERSCOREDLG]);
		subwindow[USERSCOREDLG]->showMaximized();
	}
}
void SimuTrainee::show_logout()
{
	action_ontop->setChecked(false);
	this->setWindowFlags(0); 
	this->show();
	if (loginUser->login_status)
	{	
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
		msgBox.setInformativeText(QString::fromLocal8Bit("确定要退出登陆？"));
		msgBox.setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Ok ) 
		{
			cleanUserInfo();
			this->hide();
			loginUser->show();
		}
		else
			return;
	}
}
void SimuTrainee::show_task()
{
	if (dlg_created_flag[TASKDLG] == true&&dlg_task_clicked)
	{
//		if (d_CTaskManage->isHidden())
//		{
			subwindow[TASKDLG]->closeable = false;
			d_CTaskManage->show();
//		}
		mdiArea->setActiveSubWindow(subwindow[TASKDLG]);
		return;
	}
	else 
	{
	//	d_CTaskManage->~CTaskManage();
		d_CTaskManage = new CTaskManage(this);
		dlg_task_clicked = true;
		subwindow[TASKDLG] = new MdiChild;
		subwindow[TASKDLG]->setWidget(d_CTaskManage);
		d_CTaskManage->setWindowTitle(tr("当前任务"));
		mdiArea->addSubWindow(subwindow[TASKDLG]);
		connect(d_CTaskManage,SIGNAL(closedlg()),subwindow[TASKDLG],SLOT(close()));
		connect(this,SIGNAL(sigLanguageChanged( int )),d_CTaskManage,SLOT(setLanguage( int )));
		subwindow[TASKDLG]->closeable = false;
		subwindow[TASKDLG]->showMaximized();
		return;
	}
}
void SimuTrainee::show_settask()
{
	if (shm_ots_addr->simu_mode !=0 )
	{
		PopupMessageBox(this,"当前模式下学员不可自行设置任务",QMessageBox::Warning);
		return;
	}
	else if (shm_ots_addr->data_task.task_status==2&&dlg_created_flag[TASKDLG]==true)
	{
		PopupMessageBox(this,"当前有任务正在执行",0);
		return;
	}
	if (dlg_created_flag[SETTASKDLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[SETTASKDLG]);
		return;
	}
	else 
	{
		subwindow[SETTASKDLG] = new MdiChild;
		d_CSetTaskTable = new CSetTaskTable(this);
		d_CSetTaskTable->mode = 1;//学员学习模式
		d_CSetTaskTable->checkBox_examonly->setEnabled(false);
		d_CSetTaskTable->checkBox_examonly->setChecked(false);
		d_CSetTaskTable->checkBox_examonly->setVisible(false);
		d_CSetTaskTable->setWindowTitle(QString::fromLocal8Bit("选择学习操作"));

		d_CSetTaskTable->radioMulti->setEnabled(false);
		subwindow[SETTASKDLG]->setWidget(d_CSetTaskTable);
		mdiArea->addSubWindow(subwindow[SETTASKDLG]);
		connect(d_CSetTaskTable,SIGNAL(sig_tasksetted()),subwindow[SETTASKDLG],SLOT(close()));
		subwindow[SETTASKDLG]->showMaximized();
		connect(this,SIGNAL(sigLanguageChanged( int )),d_CSetTaskTable,SLOT(setLanguage( int )));
		d_CSetTaskTable->listUserAndGroups->setEnabled(false);
		subwindow[SETTASKDLG]->setOption(QMdiSubWindow::RubberBandResize,false);
	}		
}
void SimuTrainee::show_revert()
{
	if(shm_ots_addr->simu_mode != 0 )
	{
		PopupMessageBox(this,"当前模式不可查看工况再现",QMessageBox::Information,false);
		return;
	}
	if (dlg_created_flag[REVERTDLG] == true)
	{	
		mdiArea->setActiveSubWindow(subwindow[REVERTDLG]);
		return;
	}
	else 
	{
		subwindow[REVERTDLG] = new MdiChild;
		d_RevertPlayer = new CRevertPlayer(this);
		d_RevertPlayer->buttonListRevert->click();
// 				d_RevertPlayer->buttonListRevert->click();
// 		d_RevertPlayer->isRevertListShow();
// 				d_RevertPlayer->isRevertListShow();
		subwindow[REVERTDLG]->setWidget(d_RevertPlayer);
	connect(this,SIGNAL(sigLanguageChanged( int )),d_RevertPlayer,SLOT(setLanguage( int )));
		mdiArea->addSubWindow(subwindow[REVERTDLG]);
		subwindow[REVERTDLG]->showMaximized();
	}
}
void SimuTrainee::show_localOprate()
{
	if (dlg_created_flag[LOCALOPRDLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[LOCALOPRDLG]);
		return;
	}
	else 
	{
		subwindow[LOCALOPRDLG] = new MdiChild;
		d_CLocalOprate = new CLocalOprate(this);
		subwindow[LOCALOPRDLG]->setWidget(d_CLocalOprate);
		mdiArea->addSubWindow(subwindow[LOCALOPRDLG]);
		connect(d_CLocalOprate,SIGNAL(closedlg()),subwindow[LOCALOPRDLG],SLOT(close()));

		subwindow[LOCALOPRDLG]->showMaximized();
	}
}

void SimuTrainee::show_localOprate1()
{
	if (dlg_created_flag[LOCALOPRDLG] == true)
	{
		d_CLocalOprate->tabWidget->setCurrentIndex (1);
		subwindow[LOCALOPRDLG]->setWidget(d_CLocalOprate);
		mdiArea->setActiveSubWindow(subwindow[LOCALOPRDLG]);
		return;
	}
	else 
	{
		subwindow[LOCALOPRDLG] = new MdiChild;
		d_CLocalOprate = new CLocalOprate(this);
		d_CLocalOprate->tabWidget->setCurrentIndex (1);
		subwindow[LOCALOPRDLG]->setWidget(d_CLocalOprate);
		mdiArea->addSubWindow(subwindow[LOCALOPRDLG]);
		connect(d_CLocalOprate,SIGNAL(closedlg()),subwindow[LOCALOPRDLG],SLOT(close()));
		subwindow[LOCALOPRDLG]->showMaximized();
	}
}
void SimuTrainee::show_localOprate2()
{
	if (dlg_created_flag[LOCALOPRDLG] == true)
	{
		d_CLocalOprate->tabWidget->setCurrentIndex (4);
		subwindow[LOCALOPRDLG]->setWidget(d_CLocalOprate);
		mdiArea->setActiveSubWindow(subwindow[LOCALOPRDLG]);
		return;
	}
	else 
	{
		subwindow[LOCALOPRDLG] = new MdiChild;
		d_CLocalOprate = new CLocalOprate(this);
		d_CLocalOprate->tabWidget->setCurrentIndex (4);
		subwindow[LOCALOPRDLG]->setWidget(d_CLocalOprate);
		mdiArea->addSubWindow(subwindow[LOCALOPRDLG]);
		connect(d_CLocalOprate,SIGNAL(closedlg()),subwindow[LOCALOPRDLG],SLOT(close()));
		subwindow[LOCALOPRDLG]->showMaximized();
	}
}
void SimuTrainee::show_openOix()
{
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
void SimuTrainee::show_Exam()
{
	QString program = QString::fromLocal8Bit("/home/ems/h9000/bin/exam/examination_total_form.exe");
	QStringList arguments;
	arguments.append(QString::fromLocal8Bit("otsargs"));
	arguments.append(loginUser->login_user_name);
	arguments.append(loginUser->login_user_password);
	QProcess *processExam = new QProcess(this);
	processExam->start(program,arguments);
}
void SimuTrainee::show_simuControl()
{
	if (shm_ots_addr->simu_mode==2&&JudgeUserGroupsRole(login_user_id,GROUPS_ROLE_LEADER)!= TRUE)
	{
		PopupMessageBox(this,"当前竞赛模式下组员不可进行仿真控制",0);
		return;
	}
	else if (shm_ots_addr->data_task.task_status==2&&dlg_created_flag[TASKDLG]==true)
	{
		PopupMessageBox(this,"当前任务正在执行",0);
		return;
	}
	if (dlg_created_flag[SIMUCONTROLDLG] == true)
	{
		mdiArea->setActiveSubWindow(subwindow[SIMUCONTROLDLG]);
		return;
	}
	else 
	{
		simuControlwidget = new Simucontrol;
		//dlg_simuctrol_clicked = true;
		subwindow[SIMUCONTROLDLG] = new MdiChild;
		subwindow[SIMUCONTROLDLG]->setWidget(simuControlwidget);
		mdiArea->addSubWindow(subwindow[SIMUCONTROLDLG]);
		connect(simuControlwidget,SIGNAL(closedlg()),subwindow[SIMUCONTROLDLG],SLOT(close()));
		connect(this,SIGNAL(sigLanguageChanged( int )),simuControlwidget,SLOT(setLanguage( int )));
		subwindow[SIMUCONTROLDLG]->showMaximized();
		return;
	}
}
void SimuTrainee::show_setScene()
{
	if(shm_ots_addr->simu_mode != 0 )
	{
		PopupMessageBox(this,"当前模式不可自行设置工况",QMessageBox::Information,false);
		return;
	}
	if (dlg_created_flag[SETSCENEFAULTDLG] == true)
	{
		connect(setSceneFaultDialog,SIGNAL(closedlg()),subwindow[SETSCENEFAULTDLG],SLOT(close()));
		setSceneFaultDialog->initScene();
		setSceneFaultDialog->setSceneModel();
		for (int i=0;i<setSceneFaultDialog->listUserAndGroups->count();i++)
		{
			if (groups_id == -1)
			{
				int user_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
				if (user_id == login_user_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
			else
			{
				int m_groups_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
				if (m_groups_id == groups_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
		}
		setSceneFaultDialog->buttonRefresh->setEnabled(false);
		setSceneFaultDialog->listUserAndGroups->setEnabled(false);
		mdiArea->setActiveSubWindow(subwindow[SETSCENEFAULTDLG]);
		return;
	}
	else 
	{
		subwindow[SETSCENEFAULTDLG] = new MdiChild;
		setSceneFaultDialog = new CSetSceneFaultTable(this);
		setSceneFaultDialog->initScene();
		setSceneFaultDialog->setSceneModel();
		for (int i=0;i<setSceneFaultDialog->listUserAndGroups->count();i++)
		{
			if (groups_id == -1)
			{
				int user_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
				if (user_id == login_user_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
			else
			{
				int m_groups_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(5).toInt();
				if (m_groups_id == groups_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
		}
		setSceneFaultDialog->buttonRefresh->setEnabled(false);
		setSceneFaultDialog->listUserAndGroups->setEnabled(false);
		subwindow[SETSCENEFAULTDLG]->setWidget(setSceneFaultDialog);
		mdiArea->addSubWindow(subwindow[SETSCENEFAULTDLG]);
		connect(setSceneFaultDialog,SIGNAL(closedlg()),subwindow[SETSCENEFAULTDLG],SLOT(close()));
		connect(this,SIGNAL(sigLanguageChanged( int )),setSceneFaultDialog,SLOT(setLanguage( int )));
		subwindow[SETSCENEFAULTDLG]->showMaximized();
	}
}
void SimuTrainee::show_setFault()
{
	if(shm_ots_addr->simu_mode != 0 )
	{
		PopupMessageBox(this,"当前模式不可自行设置故障",QMessageBox::Information,false);
		return;
	}
	if (dlg_created_flag[SETSCENEFAULTDLG] == true)
	{
		setSceneFaultDialog->initFault();
		setSceneFaultDialog->setFaultModel();
		for (int i=0;i<setSceneFaultDialog->listUserAndGroups->count();i++)
		{
			if (groups_id == -1||groups_id == 0)//dzy added ||groups_id == 0
			{
				int user_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
				if (user_id == login_user_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
			else if (groups_id >0)
			{
				int m_groups_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
				if (m_groups_id == groups_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
		}
		connect(setSceneFaultDialog,SIGNAL(closedlg()),subwindow[SETSCENEFAULTDLG],SLOT(close()));
		setSceneFaultDialog->buttonRefresh->setEnabled(false);
		setSceneFaultDialog->listUserAndGroups->setEnabled(false);
		mdiArea->setActiveSubWindow(subwindow[SETSCENEFAULTDLG]);
		return;
	}
	else 
	{
		subwindow[SETSCENEFAULTDLG] = new MdiChild;
		setSceneFaultDialog = new CSetSceneFaultTable(this);
		setSceneFaultDialog->initFault();
		setSceneFaultDialog->setFaultModel();
		for (int i=0;i<setSceneFaultDialog->listUserAndGroups->count();i++)
		{
			if (groups_id == -1)
			{
				int user_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(3).toInt();
				if (user_id == login_user_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
			else
			{
				int m_groups_id = setSceneFaultDialog->listUserAndGroups->item(i)->data(5).toInt();
				if (m_groups_id == groups_id)
				{
					setSceneFaultDialog->listUserAndGroups->setCurrentItem(setSceneFaultDialog->listUserAndGroups->item(i));
					setSceneFaultDialog->listUserAndGroups->item(i)->setCheckState(Qt::Checked);
					break;
				}
			}
		}
		setSceneFaultDialog->buttonRefresh->setEnabled(false);
		setSceneFaultDialog->listUserAndGroups->setEnabled(false);
		connect(setSceneFaultDialog,SIGNAL(closedlg()),subwindow[SETSCENEFAULTDLG],SLOT(close()));
		subwindow[SETSCENEFAULTDLG]->setWidget(setSceneFaultDialog);
		connect(this,SIGNAL(sigLanguageChanged( int )),setSceneFaultDialog,SLOT(setLanguage( int )));
		mdiArea->addSubWindow(subwindow[SETSCENEFAULTDLG]);
		subwindow[SETSCENEFAULTDLG]->showMaximized();
	}
}
void SimuTrainee::show_sendMessage()
{
// 	if (dlg_created_flag[MSGDLG] == true&&dlg_msg_clicked == true)
// 	{
// 		timerButtonMsg->stop();
// 		mdiArea->setActiveSubWindow(subwindow[MSGDLG]);
// 		return;
// 	}
// 	else 
// 	{
/*		sendMessageDialog->~CSendMessageTable();*/
/*		subwindow[MSGDLG] = new MdiChild;*/
		//sendMessageDialog = new CSendMessageTable(this);
		dlg_msg_clicked = true;
		timerButtonMsg->stop();
		sendMessageDialog->show();

		//tbNewSms->setVisible(false);
		//subwindow[MSGDLG]->setWidget(sendMessageDialog);
		//mdiArea->addSubWindow(subwindow[MSGDLG]);
		//subwindow[MSGDLG]->showMaximized();

//	sendMessageDialog->show();
//buttonSendMessage->setStyleSheet("");

}
void SimuTrainee::show_setDesplay()
{
// 	if (isDialogExisted("SM_DSPCONFIGDLG","M_DSPCONFIGDLG",DSPCONFIGDLG))
// 	{
// 		//d_historyrec->single_user_show();
// 		return;
// 	}
}

void SimuTrainee::show_doc()
{
	if(shm_ots_addr->simu_mode>0)
		return;
	if (dlg_created_flag[MYDOCDLG] == true )
	{
		mdiArea->setActiveSubWindow(subwindow[MYDOCDLG]);
		return;
	}
	else 
	{ 
		subwindow[MYDOCDLG] = new MdiChild;
		d_mydoc = new Mydoc(this);
		
		subwindow[MYDOCDLG]->setWidget(d_mydoc);
		mdiArea->addSubWindow(subwindow[MYDOCDLG]);
		connect(d_mydoc,SIGNAL(sig_myrec()),this,SLOT(show_studyrecord()));
		connect(this,SIGNAL(sigLanguageChanged( int )),d_mydoc,SLOT(setLanguage( int )));
		subwindow[MYDOCDLG]->showMaximized();
	}
}
void SimuTrainee::show_help()
{
	QString filePath = QString::fromLocal8Bit("/home/ems/h9000/bin/help/help.doc");
	QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).absoluteFilePath())); 
}

void  SimuTrainee::show_wait_contest()
{
	if(shm_ots_addr->simu_mode<2)
		return;
	if (dlg_created_flag[WAIT_CONTEST] == true )
	{
		mdiArea->setActiveSubWindow(subwindow[WAIT_CONTEST]);
		return;
	}
	else 
	{ 
		subwindow[WAIT_CONTEST] = new MdiChild;
		d_wait_contest = new CWaitContest();
		subwindow[WAIT_CONTEST]->setWidget(d_wait_contest);
		mdiArea->addSubWindow(subwindow[WAIT_CONTEST]);		
		connect(this,SIGNAL(sigLanguageChanged( int )),d_wait_contest,SLOT(setLanguage( int )));
		subwindow[WAIT_CONTEST]->showMaximized();
	}
}
 void SimuTrainee::createToolBars()
 { 
     toolBar->addAction(actionHostPage);
     toolBar->addAction(actionBaseinfo);
     toolBar->addAction(actionToolbox);
	 toolBar->addWidget(tbNewSms);
	 labelLogoToolBar = new QLabel(this);
	 toolBar->addWidget(labelStatusBar);
	 QWidget* spacer1= new QWidget(this);
	 spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	 spacer1->setMinimumWidth(200);
	 toolBar->addWidget(spacer1);
	 comBroadcast = new QComboBox(this);
	 comBroadcast->setMinimumContentsLength(300);
	 comBroadcast->setStyle(QStyleFactory::create("Cleanlooks"));
//	comBroadcast->setVisible(false);
	 comBroadcast->setInsertPolicy(QComboBox::InsertAtCurrent);
	 toolBar->addWidget(comBroadcast); 
//	comBroadcast->setStyleSheet("background : (0x00,0xff,0x00,0x00);border :1px ;background : (0x00,0xff,0x00,0x00);");
	 QWidget* spacer = new QWidget(this);
	 spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	 toolBar->addWidget(spacer);
	 toolBar->addWidget(labelLogoToolBar);
	 toolBar->setMovable(false);
	 QSize *qs = new QSize(35,35);
	 toolBar->setIconSize(*qs);
	 labelLogoToolBar->setStyleSheet(QString::fromUtf8("border-image: url(:pic/images/logo.png);"));
	 labelLogoToolBar->setMinimumSize(120,44);
	 labelLogoToolBar->setMaximumSize(1250,44);
	 connect(actionBaseinfo,SIGNAL(triggered()),this,SLOT(show_userinfo()));
	 connect(actionHostPage,SIGNAL(triggered()),this,SLOT(show_hostpage()));
	 connect(actionToolbox,SIGNAL(triggered()),this,SLOT(show_hide_toolbox()));	 
	 connect(action_CTRL_L,SIGNAL(triggered()),this,SLOT(show_hide_toolbox()));
	 connect(tbNewSms,SIGNAL(clicked()),this,SLOT(show_sendMessage()));
 }
void SimuTrainee::createiTrayIcon()
{
	iTrayIcon=new QSystemTrayIcon(this);	
	iTrayIcon->setIcon(QIcon(":/pic/ots2.ico"));	
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
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(checkQuit())); 

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
void SimuTrainee:: showEvent(QShowEvent * event)
{
	if (this->isMinimized ())
	{
	//	event->ignore();
		if (d_SimuTool->isHidden() )
		{	
			d_SimuTool->show();
		}	  		
		this->show();
		show_hide_toolbox();
	}
	event->accept();
}
void  SimuTrainee::hideEvent ( QHideEvent * event )
{
// 	if (this->isMinimized ())
// 	{
// 		d_SimuTool->hide();
// 	}
	event->accept();
}
void SimuTrainee::moveEvent ( QMoveEvent * event )
{
	event->accept();	
// 	if (d_SimuTool != NULL&&d_SimuTool->isHidden() == false )
// 	{	
// 		d_SimuTool->mvwith = true;
// 		d_SimuTool->newpt= event->pos();
// 		int dtoolbox_with = d_SimuTool->width();
// 		d_SimuTool->resize(dtoolbox_with,this->height()+25);	
// 		this->pos() = event->pos();
// 		d_SimuTool->moveEvent(event);
// 	}	
}
void SimuTrainee::closeEvent(QCloseEvent *event)//关闭窗口最小化
{
	event->ignore();
	if (!loginUser->login_status)
	{
		shm_ots_addr->data_task.task_status = 1;
		this->~SimuTrainee();
		qApp->quit();
	}
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
bool SimuTrainee::winEvent ( MSG * message,long *result)
{
	switch (message->message)
	{
	case WM_OPEN_SIMU_MAIN:			
		this->showNormal();
		this->activateWindow();
		break;
	case WM_OPEN_SETTASK:
		show_settask();
		break;
	case WM_OPEN_SETSCENE:
		show_setScene();
		break;
	case WM_OPEN_SETFAULT:
		show_setFault();
		break;
	case WM_OPEN_CURRENTTASK:
		show_task();
		break;
	case WM_OPEN_REVERT:	
		show_revert();
		break;
	case WM_OPEN_LOCAL_OPR:	
		show_localOprate();
		break;
	case WM_OPEN_HISTORY:	
		show_record();
		break;
	case WM_OPEN_SENDMSG:	
		show_sendMessage();
		break;
	case WM_OPEN_EXAM:	
		show_Exam();
		break;
	}
	return QMainWindow::winEvent(message, result);
}
void SimuTrainee::show_tryconnectfail()
{
	m_threadmsgbox->setIcon(QMessageBox::Information);
	m_threadmsgbox->setWindowTitle (QString::fromLocal8Bit("提示"));
	m_threadmsgbox->setInformativeText(QString::fromLocal8Bit("多次尝试连接失败，程序将退出，请检查本机及服务器之间网络链接。"));	
	m_threadmsgbox->setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
	int ret =m_threadmsgbox->exec();
	if (ret == QMessageBox::Ok )
	{	
		cleanUserInfo();
		shm_ots_addr->data_task.task_status = 1;
		qApp->quit();
	}
	else
	{
		m_NetstateThread->cout_connected = 0;
		return;
	}
}
void SimuTrainee::show_tryconnec(int i,bool connectedOK)
{
	m_threadmsgbox->setIcon(QMessageBox::Information);
	m_threadmsgbox->setWindowTitle (QString::fromLocal8Bit("提示"));
	if (!connectedOK)
	{
		m_threadmsgbox->setInformativeText(QString::fromLocal8Bit("连接服务器失败，正在尝试第")+QString::number(i)+QString::fromLocal8Bit("次连接"));
	}
	else
	{
		m_threadmsgbox->setInformativeText(QString::fromLocal8Bit("重新连接历史服务器成功"));
		m_NetstateThread->cout_connected = 0;
	}
	m_threadmsgbox->setStandardButtons(QMessageBox::Ok );
	m_threadmsgbox->exec();
}
void SimuTrainee::recvMsg()
{
	flagSendMsg = false;
	if (sendMessageDialog->isHidden())
	{
		timerButtonMsg->start(1000);	
	}
	else
	{
		tbNewSms->setStyleSheet("");
		//tbNewSms->setVisible(false);
		return;
	}
	tbNewSms->setVisible(true);
//	tbNewSms->setText(QString::fromLocal8Bit("有新消息"));
/*	tbNewSms->setStyleSheet("background-color: rgb(255, 164, 173);");*/
	//sendMessageDialog->show();
}
void SimuTrainee::blinkMsgButton()
{
	//show_sendMessage();
	if (!flagSendMsg)
	{
		tbNewSms->setStyleSheet(QString::fromUtf8("background-color: rgb(199,100,100);"));
		flagSendMsg = true;
	}
	else
	{
		tbNewSms->setStyleSheet("");
		flagSendMsg = false;
	}

}