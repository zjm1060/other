#include "SimuTutor.h"
CTeacherManage::CTeacherManage(QWidget* parent)
			  :QMainWindow(parent)
{	  
	setupUi(this);
	frame = new QFrame(centralwidget);
	labelName = new QLabel(frame);
	labelLogo = new QLabel(frame);
	labelNameCo = new QLabel(frame);
	frame->resize(1000,650);
	frame->setObjectName(QString::fromUtf8("frame"));
	QVBoxLayout *layout = new QVBoxLayout;
	QVBoxLayout *layoutLogo = new QVBoxLayout(frame);
	layout->addWidget(frame);
	layout->setContentsMargins(0,0,0,0);
	centralwidget->setLayout(layout);
	layoutLogo->addWidget(labelLogo);
	layoutLogo->addWidget(labelName);
	layoutLogo->addWidget(labelNameCo);
	frame->setLayout(layoutLogo);
	writeConfig();
	readConfig();
	ifshowtip = true;
	lang_is_english = false;
	lang_is_chinese = true;
	actionChinese->setCheckable(true);
	actionEnglish->setCheckable(true);
	actionChinese->setChecked(true);
	actionEnglish->setChecked(false);
	buttonSendMsg = new QToolButton(this);
	labelLogoToolBar = new QLabel(this);
	actionHelp->setShortcut(tr("F1"));
	createiTrayIcon();
	loginUser = new CLogin(this);
	sendmessagetable = new CSendMessageTable(this);
	toolBar->addWidget(buttonSendMsg);
	buttonSendMsg->resize(50,40);
	buttonSendMsg->setIcon(QIcon(QString::fromLocal8Bit(":/pic/images/20071224091514244[1].PNG")));
	QWidget* spacer = new QWidget(this);
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	toolBar->addWidget(spacer);
	toolBar->addWidget(labelLogoToolBar);
	labelLogoToolBar->setStyleSheet(QString::fromUtf8("border-image: url(:pic/images/logo.png);"));
	labelLogoToolBar->setMinimumSize(150,55);
	labelLogoToolBar->setMaximumSize(150,55);
	actionTrainInfo->setVisible(false);//hide train_info menu by dm 2013.04.01
	menu_language->menuAction()->setVisible(false);//隐藏菜单中的QMenu
	actionLoginStatus->setText(QString::fromLocal8Bit("用户尚未登录"));

	connect(loginUser,SIGNAL(sigUserLogin(int,const QString &)),this,SLOT(userLogin(int,const QString &)));
	connect(actionChinese,SIGNAL(triggered()),this,SLOT(loadChinese()));
	connect(actionEnglish,SIGNAL(triggered()),this,SLOT(loadEnglish()));
	connect(actionLogin, SIGNAL(triggered()), this, SLOT(openLoginTable()));
	connect(actionLogin_menu, SIGNAL(triggered()), this, SLOT(openLoginTable()));
	connect(actionExit,SIGNAL(triggered()),this,SLOT(userLogout()));
	connect(actionExit_menu,SIGNAL(triggered()),this,SLOT(userLogout()));
	connect(actionQuit,SIGNAL(triggered()),this,SLOT(isQuit()));
	connect(iTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	connect(timerButtonChat, SIGNAL(timeout()), this, SLOT(blinkChatButton()));
	connect(sendmessagetable, SIGNAL(sigRecvMsg()), this, SLOT(recvMsg()));
	connect(actionAbout,SIGNAL(triggered()),this,SLOT(about()));
	connect(actionHelp,SIGNAL(triggered()),this,SLOT(help()));
	connect(actionStudentInfo_menu, SIGNAL(triggered()), this, SLOT(openStudentInfoTable()));
	connect(actionLoginRec,SIGNAL(triggered()),this,SLOT(openLoginRecTable()));
	connect(action_user_score,SIGNAL(triggered()),this,SLOT(openDlgScore()));
	connect(actionPrivilege,SIGNAL(triggered()),this,SLOT(openPrivilegeManager()));
	connect(actionSetTask,SIGNAL(triggered()),this,SLOT(openSetTaskTable()));
	connect(actionSetTask_menu,SIGNAL(triggered()),this,SLOT(openSetTaskTable()));
	connect(actionSetScene,SIGNAL(triggered()),this,SLOT(openSetSceneTable()));
	connect(actionSetFault,SIGNAL(triggered()),this,SLOT(openSetFaultTable()));
	connect(actionStudentInfo, SIGNAL(triggered()), this, SLOT(openLoginStudentTable()));
	connect(actionLoginStudent,SIGNAL(triggered()),this,SLOT(openLoginStudentTable()));
	connect(actionTrainInfo,SIGNAL(triggered()),this,SLOT(openTrainInfoTable()));
	connect(buttonSendMsg, SIGNAL(clicked()), this, SLOT(openSendMessageTable()));
	connect(actionSendMessage_menu, SIGNAL(triggered()), this, SLOT(openSendMessageTable()));
	connect(actionModel,SIGNAL(triggered()),this,SLOT(openModelForm()));
	connect(actionModel_menu,SIGNAL(triggered()),this,SLOT(openModelForm()));
	connect(actionSequence,SIGNAL(triggered()),this,SLOT(openSequence()));
	connect(actionFault, SIGNAL(triggered()), this, SLOT(openFaultTable()));
	connect(actionTask,SIGNAL(triggered()),this,SLOT(openTaskTable()));
	connect(actionScene,SIGNAL(triggered()),this,SLOT(openSceneTable()));
	connect(actionRevert,SIGNAL(triggered()),this,SLOT(openRevertTable()));
	connect(actionOprManage,SIGNAL(triggered()),this,SLOT(openLocalOpr()));
	connect(actionOprManage_menu,SIGNAL(triggered()),this,SLOT(openLocalOpr()));
	connect(actionHistoryRec,SIGNAL(triggered()),this,SLOT(openHistoryRecTable()));
	connect(actionHistoryRec_menu,SIGNAL(triggered()),this,SLOT(openHistoryRecTable()));
	connect(actionExam,SIGNAL(triggered()),this,SLOT(openExamTable()));
	connect(actionStationConfig_menu, SIGNAL(triggered()), this, SLOT(openStationConfigTable()));
	connect(actionStationConfig, SIGNAL(triggered()), this, SLOT(openStationConfigTable()));
	connect(actionALL_reset,SIGNAL(triggered()),this,SLOT(onactionALL_reset()));
	connect(actionSaveScene,SIGNAL(triggered()),this,SLOT(openSaveSceneTable()));
	connect(actionCallData,SIGNAL(triggered()),this,SLOT(openCallDataTable()));
	connect(actionSetPointMask,SIGNAL(triggered()),this,SLOT(openSetPointMaskTable()));
}
CTeacherManage::~CTeacherManage()
{
	userLogout();
	CloseHDB(db_conn);
}
void CTeacherManage::loadChinese()
{
	actionChinese->setCheckable(true);
	actionChinese->setChecked(true);
	actionEnglish->setChecked(false);
	if(lang_is_chinese)
		return;
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	lang_is_chinese=true;
	lang_is_english=false;
	retranslateUi(this);
	loginUser->loadChinese();
	faulttable->loadChinese();
	stationconfigtable->loadChinese();
	sendmessagetable->loadChinese();
	studentinfotable->loadChinese();
	scenetable->loadChinese();
	tasktable->loadChinese();
	loginstudenttable->loadChinese();
	reverttable->loadChinese();
	modelform->loadChinese();
	historyrectable->loadChinese();
	settasktable->loadChinese();
	pSequence->loadChinese();
	loginrec->loadChinese();
	traininfo->loadChinese();
}

void CTeacherManage::loadEnglish()
{
	actionEnglish->setCheckable(true);
	actionEnglish->setChecked(true);
	actionChinese->setChecked(false);
	if(lang_is_english)
		return;
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	lang_is_english=true;
	lang_is_chinese=false;
	retranslateUi(this);
	loginUser->loadEnglish();
	faulttable->loadEnglish();
	stationconfigtable->loadEnglish();
	sendmessagetable->loadEnglish();
	studentinfotable->loadEnglish();
	scenetable->loadEnglish();
	tasktable->loadEnglish();
	loginstudenttable->loadEnglish();
	reverttable->loadEnglish();
	modelform->loadEnglish();
	historyrectable->loadEnglish();
	settasktable->loadEnglish();
	pSequence->loadEnglish();
	loginrec->loadEnglish();
	traininfo->loadEnglish();
}
void CTeacherManage::readConfig()
{
	QString myfile = SimuTutorConfigPath;
	char verTmp[20];
	QSettings mySetting(myfile,QSettings::IniFormat);
	mySetting.setIniCodec(QTextCodec::codecForName("GB2312"));
	strcpy(tch_bg_pic,mySetting.value("background/bgpic").toString().toLocal8Bit().data());
	QPoint pos = mySetting.value("window/pos",QVariant(QPoint(200,200))).toPoint();
	QSize size = mySetting.value("window/size",QVariant(QSize(1000,720))).toSize();
	this->resize(size);
	this->move(pos);
}
void CTeacherManage::writeConfig()
{
	QString myfile = SimuTutorConfigPath;
	QSettings mySetting(myfile,QSettings::IniFormat);
	mySetting.setIniCodec(QTextCodec::codecForName("GB2312"));
	mySetting.setValue("window/pos",QVariant(this->pos()));
	mySetting.setValue("window/size",QVariant(this->size()));
}
void CTeacherManage::bgPic()
{
	QImage image1;
	QPixmap logo1(QString::fromLocal8Bit(":/pic/images/logo.png"));
	QPixmap logo2 = logo1.scaled(150,150);
	QPalette   palette,p_labelName,p_labelNameCo;
	QFont f_labelName(QString::fromLocal8Bit("黑体"), 70, QFont::Bold);
	QFont f_labelNameCo(QString::fromLocal8Bit("楷体"), 40, QFont::Bold);
//	f_labelName.setPointSize(fontsize);
	QDir dir;
	dir.setPath(QString::fromLocal8Bit("/home/ems/h9000/ots/images"));
	if (!dir.exists())
		dir.setPath(QString::fromLocal8Bit("/home/ems/h9000/bin/images"));
	image1.load(dir.absoluteFilePath("background.jpg"));
/*
	labelLogo->setAlignment(Qt::AlignLeft | Qt::AlignTop);  
	labelName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  
	labelNameCo->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); 
	p_labelName.setColor(QPalette::WindowText,Qt::blue);
	p_labelNameCo.setColor(QPalette::WindowText,Qt::blue);
	labelLogo->setPixmap(logo2);
	labelName->setText(QString::fromLocal8Bit("SimuTutor"));
	labelName->setPalette(p_labelName);
	labelName->setFont(f_labelName);
	labelNameCo->setText(QString::fromLocal8Bit("北京中水科水电科技开发有限公司"));
	labelNameCo->setPalette(p_labelNameCo);
	labelNameCo->setFont(f_labelNameCo);
*/
	QImage image2 = image1.scaled(frame->frameSize());
	palette.setBrush(frame->backgroundRole(),QBrush(image2));
	frame->setPalette(palette);
	frame->setAutoFillBackground(true);
	frame->show();
}
void CTeacherManage::resizeEvent ( QResizeEvent *event )
{
	event->ignore();
	writeConfig();
	bgPic();
}
void CTeacherManage::moveEvent( QMoveEvent *event)
{
	event->ignore();
	writeConfig();
}
void CTeacherManage::createActions() 
{ //托盘上的 
	minimizeAction = new QAction(QString::fromLocal8Bit("最小化 (&I)"), this); 
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide())); 
	maximizeAction = new QAction(QString::fromLocal8Bit("最大化 (&X)"), this); 
	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized())); 
	restoreAction = new QAction(QString::fromLocal8Bit("还原 (&R)"), this); 
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal())); 
	quitAction = new QAction(QString::fromLocal8Bit("退出 (&Q)"), this); 
	quitAction->setShortcut(QString::fromLocal8Bit("CTRL+Q"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(isQuit())); 
} 
void CTeacherManage::createiTrayIcon()
{
	iTrayIcon=new QSystemTrayIcon(this);	
	iTrayIcon->setIcon(QIcon(":/pic/main.ico"));	
	iTrayIcon->setToolTip(QString::fromLocal8Bit("OTS2000教员站"));
	createActions(); 
	createTrayIcon(); 
	iTrayIcon->show();
}
void CTeacherManage::createTrayIcon() 
{
	trayIconMenu = new QMenu(this); 
	trayIconMenu->addAction(minimizeAction); 
	trayIconMenu->addAction(maximizeAction); 
	trayIconMenu->addAction(restoreAction); 
	trayIconMenu->addSeparator(); 
	trayIconMenu->addAction(quitAction); 
	iTrayIcon->setContextMenu(trayIconMenu); 
}//托盘
void CTeacherManage::iconActivated(QSystemTrayIcon::ActivationReason reason) 
{ 	
	//处理鼠标单击双击事件
	switch (reason) 
	{ 
	/*case QSystemTrayIcon::Trigger: 	
		if(isHidden()){
			this->showNormal();
			this->focusWidget();}
		break; */
	case QSystemTrayIcon::DoubleClick: 
		this->showNormal();
		this->activateWindow();
		this->focusWidget();
		break;
	default:
		break; 
	} 
} 
void CTeacherManage::closeEvent(QCloseEvent *event)//关闭窗口最小化
{
	event->ignore();
	closeTable = new QDialog(this);
	closeTable->setAttribute(Qt::WA_DeleteOnClose);
	chk = new QCheckBox(closeTable);
	chkMin = new QRadioButton(closeTable);
	chkClose = new QRadioButton(closeTable);
	lbl = new QLabel(closeTable);
	butEnter = new QPushButton(closeTable);
	butExit = new QPushButton(closeTable);
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
	connect(chk,SIGNAL(stateChanged(int)),this,SLOT(oncheckbox(int)));
	if (ifshowtip) 
	{
		closeTable->exec();
	}
	else
		this->hide();
}
void CTeacherManage::checkQuit()
{
	if (chkMin->isChecked())
	{
		closeTable->hide();
		this->hide();
	} 
	else if(chkClose->isChecked())
	{
		qApp->quit();
	}
}
void CTeacherManage::oncheckbox(int state)
{
	ifshowtip=state;
}
void CTeacherManage::isQuit()
{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
	msgBox.setInformativeText(QString::fromLocal8Bit("确定要退出OTS2000教员站？"));
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
		qApp->quit();
	}
	else 
		this->show();
}
void CTeacherManage::about()
{
	QMessageBox::about(this, tr("关于 OTS2000"),
		tr("<p>The <b>OTS2000</b> simulation station " \
		"helps to train workers at hydro-plant to learn how to " \
		"use the complicated H9000 system effiently..\n"\
		"  Copyright by BITC.</p>"));
}
void CTeacherManage::help()
{
	QString filePath = QString::fromLocal8Bit("/home/ems/h9000/ots/help/help.doc");
	QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).absoluteFilePath())); 
}
 /*************************************** 弹出窗口的槽函数***********************************************/
void CTeacherManage::openLoginTable()
{
	if (!loginUser->login_status)
	{
		loginUser->init();
		loginUser->show();
	}
}
void CTeacherManage::openFaultTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_FAULT))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	faulttable = new CFaultTable(this);
	faulttable->displayFault();
	faulttable->show();
	faulttable->listFault->resizeColumnsToContents();
}
void CTeacherManage::openStationConfigTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_HOST))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	stationconfigtable = new CStationConfigTable(this);
	stationconfigtable->displayStation();
	stationconfigtable->show();
}
void CTeacherManage::openSendMessageTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SEND_MESSAGE))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	sendmessagetable->show();
	buttonSendMsg->setStyleSheet("");
}
void CTeacherManage::openStudentInfoTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_USER_INFO))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	studentinfotable = new CStudentInfoTable(this);
	studentinfotable->show();
}
void CTeacherManage::openSceneTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SCENE))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	scenetable = new CSceneTable(this);
	scenetable->displayScene();
	scenetable->show();
	scenetable->listScene->resizeColumnsToContents();
}
void CTeacherManage::openTaskTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_TASK))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	tasktable = new CTaskTable(this);
	tasktable->displayTask();
	tasktable->show();
	tasktable->listTask->resizeColumnsToContents();
}
void CTeacherManage::openLoginStudentTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_LOGIN_USER))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	loginstudenttable = new CLoginStudentTable(this);
	UpdateTrainInfoModel();
	loginstudenttable->show();
}
void CTeacherManage::openRevertTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_REVERT))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	reverttable = new CRevertTable(this);
	reverttable->displayRevert();
	reverttable->show();
}
void CTeacherManage::openModelForm()	//yyp
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_MODEL))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	modelform = new CModelForm(this);
	modelform->displayModel();
	modelform->show();
}

void CTeacherManage::openHistoryRecTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_RECORD))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	historyrectable = new CHistoryRecTable(this);
	historyrectable->setUserId(loginUser->login_user_id);
	historyrectable->show();
}
void CTeacherManage::openSetTaskTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SET_TASK))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	settasktable = new CSetTaskTable(this);
	settasktable->show();
}
void CTeacherManage::openSetSceneTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SET_SCENE))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	setscenetable = new CSetSceneFaultTable(this);
	setscenetable->initScene();
	setscenetable->show();
	setscenetable->setSceneModel();
}
void CTeacherManage::openSetFaultTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SET_FAULT))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	setfaulttable = new CSetSceneFaultTable(this);
	setfaulttable->initFault();
	setfaulttable->show();
	setfaulttable->setFaultModel();
}
void CTeacherManage::openLocalOpr()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_LOCAL_OPR))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	localopr = new CLocalOprTable(this);
	localopr->show();
}
void CTeacherManage::openSequence()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SEQUENCE))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	pSequence = new SequenceList(this);
	pSequence->show();
}
void CTeacherManage::openLoginRecTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_LOGIN_REC))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	loginrec = new CLoginRecTable(this);
	loginrec->show();
}
void CTeacherManage::openTrainInfoTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	traininfo = new CTrainInfoTable(this);
	traininfo->show();
}
void CTeacherManage::openPrivilegeManager()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_PRIV_MANAGER))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	privmgr = new CPrivilegeMananger(this);
	privmgr->show();
}
void CTeacherManage::openSaveSceneTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SAVE_SCENE))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	savescenetable = new CSaveSceneTable(this);
	savescenetable->show();
}
void CTeacherManage::openDlgScore()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SCORE))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	scorerec = new CScoreRec(this);
	scorerec->show();
}
void CTeacherManage::openSetPointMaskTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SET_POINT_MASK))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	setpointmasktable = new CSetPointMaskTable(this);
	setpointmasktable->show();
}
void CTeacherManage::onactionALL_reset()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_SCENE_RESET))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	pSceneReset = new SceneReset(this);
	pSceneReset->show();
}
void CTeacherManage::openExamTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_EXAM))
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
 /*************************************** 弹出窗口的槽函数***********************************************/
void CTeacherManage::userLogin(int user_id,const QString &login_name)
{
	actionLoginStatus->setText(QString::fromLocal8Bit("当前登录用户：%1").arg(login_name));
	sendmessagetable->setUserId(user_id);
}
void CTeacherManage::userLogout()
{
	if (loginUser->login_status)
	{
// 		QMessageBox msgBox;
// 		msgBox.setIcon(QMessageBox::Information);
// 		msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
// 		msgBox.setInformativeText(QString::fromLocal8Bit("确定要退出当前用户登陆？"));
// 		msgBox.setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
// 		msgBox.setDefaultButton(QMessageBox::Cancel);
// 		if (msgBox.exec() != QMessageBox::Ok ) 
// 			return;
		DB_FMT data;
		InitDbData(&data,TABLE_LOGIN_INFO);
		data.flag.flag_login_info.logout_time=TRUE;
		GetCurTimeStr(data.seg.seg_login_info.logout_time);
		
		data.cond_flag.flag_login_info.user_id=TRUE;
		data.cond_flag.flag_login_info.login_time=TRUE;
		data.cond_seg.seg_login_info.user_id = loginUser->login_user_id;
		strcpy(data.cond_seg.seg_login_info.login_time,loginUser->login_time.toLocal8Bit().data());
		if( UpdateOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
		actionLoginStatus->setText(QString::fromLocal8Bit("用户尚未登录"));
		loginUser->init();
		//教员用户注销不修改train_info表 remove by dm 2012.6.20
		/*InitDbData(&data,TABLE_TRAIN_INFO);
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
		data.flag.flag_train_info.fault_id =TRUE;
		data.seg.seg_train_info.fault_id = CONST_NEG;	
		data.flag.flag_train_info.scene_id = TRUE;
		data.seg.seg_train_info.scene_id = CONST_NEG;	
		data.flag.flag_train_info.send_task_id = TRUE;
		data.seg.seg_train_info.send_task_id = CONST_NEG;	
		data.flag.flag_train_info.task_id = TRUE;
		data.seg.seg_train_info.task_id = CONST_NEG;	
		data.flag.flag_train_info.user_id = TRUE;
		data.seg.seg_train_info.user_id = CONST_NEG;
		data.cond_flag.flag_train_info.host_id =TRUE;
		data.cond_seg.seg_train_info.host_id = g_host_id;
		if (UpdateOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}*/
		//logintable->heartBeatThread->terminate();    //终止线程  
		//logintable->heartBeatThread->wait();    //阻塞等待
		//logintable->heartBeatThread->exit();
		//delete logintable->heartBeatThread;
	}
}

void CTeacherManage::initStnView()
{
	GULONG	stn_num,grp_num;
	UCHAR	stn_id,dev_id;
	char	stn_comment[STATION_COMMENT_SIZE];
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	text[128];
	LCUID	lcuno;
	QVariant varStn,varLcuno;
	stndevTable = new QDialog(this);
	stndevTable->setAttribute(Qt::WA_DeleteOnClose);
	stndevTable->setModal(true);
	buttonEnter = new QPushButton(stndevTable);
	buttonExit = new QPushButton(stndevTable);
	stnView = new QTreeWidget(stndevTable);
	stnView->setSortingEnabled(false); //不自动排序
	stnView->setDragEnabled(false); //不可拖放
	stnView->setAutoScroll(true); //可以卷动
	stnView->setColumnCount(1); //列数
	stnView->header()->setHidden(1); //隐藏最顶上的表头
	stnView->setRootIsDecorated(true); //让根项目前面也有树状虚线 
	//callDataTable->setWindowTitle(QString::fromLocal8Bit("监控数据召唤"));
	QGridLayout *layout = new QGridLayout(stndevTable);
	//layout->setSizeConstraint(QLayout::SetFixedSize);
	buttonEnter->setText(QString::fromLocal8Bit("确定"));
	buttonExit->setText(QString::fromLocal8Bit("取消"));
	layout->addWidget(stnView, 0, 0, 1, 2);
	layout->addWidget(buttonEnter, 1, 0, 1, 1);
	layout->addWidget(buttonExit, 1, 1, 1, 1);
	stndevTable->resize(250, 400);
	stndevTable->setLayout(layout);	
	connect(buttonExit,SIGNAL(clicked()), stndevTable, SLOT(close()));
	connect(stnView,SIGNAL(itemChanged( QTreeWidgetItem *, int)),this,SLOT(treeItemChanged(QTreeWidgetItem *,int)));
	GetStationNum(&stn_num);
	for(int i=1; i<=stn_num; i++) 
	{  		
		GetStnIdByIndex(i, &stn_id);

		if( GetStnCommentById(stn_id,(char*)( stn_comment))==-1 )
			continue;

		sprintf(text, "%d. %s", stn_id, stn_comment);
		QTreeWidgetItem *itemStn = new QTreeWidgetItem(stnView, QStringList(QString::fromLocal8Bit(text)));
		varStn.setValue(stn_id);
		itemStn->setData(1,0,varStn);
		itemStn->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		itemStn->setCheckState (0, Qt::Unchecked);
		if(-1==GetGroupNumById(stn_id, &grp_num))
			continue;

		for(dev_id=1; dev_id<=grp_num; dev_id++) 
		{  
			if(-1==GetGroupCommentById(stn_id, dev_id, dev_comment))
				continue;

			sprintf(text, "%d. %s", dev_id, dev_comment);
			QTreeWidgetItem *itemDev = new QTreeWidgetItem(itemStn, QStringList(QString::fromLocal8Bit(text)));
			lcuno.stnid = stn_id;
			lcuno.devid = dev_id;
			varLcuno.setValue(lcuno);
			itemDev->setData(1,0,varLcuno);
			itemDev->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			itemDev->setCheckState (0, Qt::Unchecked);
		}
	}
}

void CTeacherManage::openCallDataTable()
{
	if (!loginUser->login_status)
	{
		PopupMessageBox(this,"用户未登录，请先登录系统",QMessageBox::Information);
		return;
	}
	if (!CheckUserPrivilege(loginUser->login_user_id,TeacherPrivType::PRIV_CALL_DATA))
	{
		PopupMessageBox(this,"对不起，您没有此操作权限，请联系管理员！",QMessageBox::Warning);
		return;
	}
	initStnView();
	stndevTable->setWindowTitle(QString::fromLocal8Bit("监控数据召唤"));
	buttonEnter->setText(QString::fromLocal8Bit("召唤"));
	buttonEnter->disconnect();
	connect(buttonEnter,SIGNAL(clicked()), this, SLOT(callData()));
	stndevTable->show();
}

void CTeacherManage::treeItemChanged(QTreeWidgetItem *item, int column)
{ 
	if(Qt::Checked == item->checkState(0))
	{  
		int count = item->childCount();     
		if (count > 0)
		{         
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Checked);         
			}     
		}
		else
		{
			updateParentItem(item);
		}
	}
	else if(Qt::Unchecked == item->checkState(0))
	{   
		int count = item->childCount();
		if (count > 0)
		{         
			for (int i = 0; i < count; i++)
			{        
				item->child(i)->setCheckState(0, Qt::Unchecked);
			}     
		}
		else
		{
			updateParentItem(item);
		}
	}
}
void CTeacherManage::updateParentItem(QTreeWidgetItem* item)
{    
	QTreeWidgetItem *parent = item->parent();     
	if (parent == NULL)
	{        
		return;     
	}     //选中的子节点个数     
	int selectedCount = 0;     
	int childCount = parent->childCount();     
	for (int i = 0; i < childCount; i++)
	{         
		QTreeWidgetItem *childItem = parent->child(i);         
		if (childItem->checkState(0) == Qt::Checked)
		{             
			selectedCount++;         
		}     
	}     
	if (selectedCount <= 0)
	{     //未选中状态         
		parent->setCheckState(0, Qt::Unchecked);     
	}
	else if(selectedCount > 0 && selectedCount < childCount)
	{     //部分选中状态         
		parent->setCheckState(0, Qt::PartiallyChecked);     
	}
	else if(selectedCount == childCount)
	{     //选中状态     
		parent->setCheckState(0, Qt::Checked); 
	}//    
}

void CTeacherManage::callData()
{
	int		stn_num,dev_num;
	LCUID	lcuno;
	QVariant varStn,varLcuno;
	stn_num = stnView->topLevelItemCount();
	for(int i=0;i<stn_num;i++)
	{
		dev_num = stnView->topLevelItem(i)->childCount();
		for (int j=0;j<dev_num;j++)
		{
			if(stnView->topLevelItem(i)->child(j)->checkState(0) == Qt::Checked)
			{
				varLcuno = stnView->topLevelItem(i)->child(j)->data(1,0);
				lcuno = varLcuno.value<LCUID>();
				BdPollingCmd(lcuno.stnid,lcuno.devid);
			}
		}
	}
	stndevTable->close();
	PopupMessageBox(this,"召唤数据完成!",QMessageBox::Information);
}

void CTeacherManage::saveScene()
{
	DB_FMT data;
	int		stn_num,dev_num;
	UCHAR	stn_id;
	LCUID	lcuno;
	int		num=0;
	QString	stn_name;
	QVariant varStn,varLcuno;
	stn_num = stnView->topLevelItemCount();
	for(int i=0;i<stn_num;i++)
	{
		varStn = stnView->topLevelItem(i)->data(1,0);
		stn_id = varStn.value<UCHAR>();
		if(stnView->topLevelItem(i)->checkState(0) == Qt::Checked || 
		   stnView->topLevelItem(i)->checkState(0) == Qt::PartiallyChecked)
		{
			InitDbData(&data,TABLE_SCENE);
			//设置表中每个字段的flag
			data.flag.flag_scene.scene_id =TRUE;
			data.flag.flag_scene.user_id =TRUE;
			data.flag.flag_scene.name=TRUE;
			data.flag.flag_scene.src=TRUE;
			data.flag.flag_scene.datetime=TRUE;
			data.flag.flag_scene.remark=TRUE;
			data.flag.flag_scene.dev_id=TRUE;
			data.flag.flag_scene.station_id=TRUE;
			data.flag.flag_scene.filename=TRUE;

			//填充表中每个字段的值
			stn_name = stnView->topLevelItem(i)->text(0);
			stn_name = stn_name.section(' ', -1);
			stn_name.append(QString::fromLocal8Bit("工况"));
			data.seg.seg_scene.scene_id = GetInexistenceIdInHdb(TABLE_SCENE);
			data.seg.seg_scene.src = 2;
			data.seg.seg_scene.user_id = loginUser->login_user_id;
			GetCurTimeStr(data.seg.seg_scene.datetime);	
			strcpy(data.seg.seg_scene.remark,"");
			strcpy(data.seg.seg_scene.filename,"");
			strcpy(data.seg.seg_scene.name,stn_name.toLocal8Bit().data());
			data.seg.seg_scene.station_id = stn_id;
			data.seg.seg_scene.dev_id = 0;
			//调用接口函数，参数为DB_FMT结构体
			if( InsertOneRec(&data) != TRUE)
			{
				//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
				HdbLogWrite(data.sql_str,data.sql_error);
				continue;
			}
		}
		dev_num = stnView->topLevelItem(i)->childCount();
		for (int j=0;j<dev_num;j++)
		{
			if(stnView->topLevelItem(i)->child(j)->checkState(0) == Qt::Checked)
			{
				varLcuno = stnView->topLevelItem(i)->child(j)->data(1,0);
				lcuno = varLcuno.value<LCUID>();
				//saveSceneThread[num++] = new SaveSceneThread(lcuno.stnid,lcuno.devid,scene_id,this);
				//saveSceneRunnable[num++] = new SaveSceneRunnable(lcuno.stnid,lcuno.devid,scene_id);
			}
		}
		for (int k=0;k<num;k++)
		{
			//saveSceneThread[k]->start();
			//QThreadPool::globalInstance()->start(saveSceneRunnable[k]);
		}
	}
	stndevTable->close();
}
void CTeacherManage::recvMsg()
{
	flagSendMsg = false;
	if (sendmessagetable->isHidden())
		timerButtonChat->start(1000);
}
void CTeacherManage::blinkChatButton()
{
	if (!flagSendMsg)
	{
		buttonSendMsg->setStyleSheet(QString::fromUtf8("background-color: rgb(199,100,100);"));
		flagSendMsg = true;
	}
	else
	{
		buttonSendMsg->setStyleSheet("");
		flagSendMsg = false;
	}

}