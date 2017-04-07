#include "SimuTool.h"

SimuTool::SimuTool(QWidget* parent)
//:QToolBox(parent)
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
	init();	
	setButtonConnect();
}
SimuTool::~SimuTool()
{
	//UnMountDB();
	//CloseHDB(db_conn);
}
void SimuTool::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void SimuTool::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void SimuTool::showEvent(QShowEvent *event)
{
// 	if (flag_langueage == 0)
// 	{
// 		loadChinese();
// 	}
// 	else if (flag_langueage == 1)
// 	{
// 		loadEnglish();
// 	}
	is_hiden = false;
	event->accept();
}
void SimuTool::hideEvent ( QHideEvent * event )
{
	is_hiden = true;
	event->accept();
}
void SimuTool::init()
{
	is_hiden = false;
	cur_index = -1;
	buttonMydocument->setVisible(false);
	toolButton_13->setVisible(false);
	toolButton_19->setVisible(false);
	
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	actionHidetool = new QAction(QString::fromLocal8Bit("隐藏工具栏"), this);
	actionClosetool = new QAction(QString::fromLocal8Bit("关闭工具栏"), this);

	this->setCurrentIndex(0);
	connect(actionHidetool,SIGNAL(triggered()),this,SLOT(on_act_hide()));
	connect(actionClosetool,SIGNAL(triggered()),this,SLOT(on_act_close()));
	connect(this,SIGNAL(currentChanged ( int )),this,SLOT(MysetIndex( int )));
	connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(on_menuList(const QPoint &)));
}

void SimuTool::setButtonConnect()
{	
	connect(button_userinfo, SIGNAL( clicked() ),this,SLOT( on_button_userinfo() ) );
	connect(button_record, SIGNAL(clicked()),this,SLOT(on_button_record()));
	connect(button_userscore, SIGNAL(clicked()),this,SLOT(on_button_userscore()));
	connect(button_logout, SIGNAL(clicked()),this,SLOT(on_button_logout()));
	connect(button_task, SIGNAL(clicked()),this,SLOT(on_button_task()));
	connect(button_revert, SIGNAL(clicked()),this,SLOT(on_button_revert()));
	connect(button_localOprate, SIGNAL(clicked()),this,SLOT(on_button_localOprate()));
	connect(button_openOix, SIGNAL(clicked()),this,SLOT(on_button_openOix()));
	connect(button_Exam, SIGNAL(clicked()),this,SLOT(on_button_Exam()));
	connect(button_simuControl, SIGNAL(clicked()),this,SLOT(on_button_simuControl()));
	connect(button_setScene, SIGNAL(clicked()),this,SLOT(on_button_setScene()));
	connect(button_setFault, SIGNAL(clicked()),this,SLOT(on_button_setFault()));
	connect(button_sendMessage, SIGNAL(clicked()),this,SLOT(on_button_sendMessage()));
	connect(button_setDesplay, SIGNAL(clicked()),this,SLOT(on_button_setDesplay()));
	connect(button_help, SIGNAL(clicked()),this,SLOT(on_button_help()));
	connect(buttondocuments, SIGNAL(clicked()),this,SLOT(on_button_doccuments()));
	//connect(button_userinfo, SIGNAL(clicked()),this,SLOT(on_button_userinfo()));
	//connect(button_userinfo, SIGNAL(clicked()),this,SLOT(on_button_userinfo()));
	//connect(button_userinfo, SIGNAL(clicked()),this,SLOT(on_button_userinfo()));
	//connect(button_userinfo, SIGNAL(clicked()),this,SLOT(on_button_userinfo()));
	//connect(button_userinfo, SIGNAL(clicked()),this,SLOT(on_button_userinfo()));
	//connect(button_userinfo, SIGNAL(clicked()),this,SLOT(on_button_userinfo()));
	//connect(button_userinfo, SIGNAL(clicked()),this,SLOT(on_button_userinfo()));
}

void SimuTool::mousePressEvent(QMouseEvent *event){   
	this->windowPos = this->pos();   
	this->mousePos = event->globalPos();  
	this->dPos = mousePos - windowPos;  
}  
void SimuTool::mouseMoveEvent(QMouseEvent *event){   
/*	this->move(event->globalPos() - this->dPos);  */
}  
void SimuTool::moveEvent ( QMoveEvent * event )
{   
// 	if (mvwith != false)
// 	{
// 		this->move(newpt.x()-this->width()-8,newpt.y()-30);
// 		mvwith = false;
// 	}	
// 	event->accept();
}  
	
void SimuTool::on_menuList(const QPoint &pos)
{
	QMenu* popMenu = new QMenu(this);
	popMenu->addAction(actionHidetool);
	popMenu->addAction(actionClosetool);
	popMenu->exec(QCursor::pos());
} 
void SimuTool::on_button_userinfo()
{
	emit button_userinfo_clicked();
}
void SimuTool::on_button_record()
{
	emit button_record_clicked();
}
void SimuTool::on_button_userscore()
{
	emit button_userscore_clicked();
}
void SimuTool::on_button_logout()
{
	emit button_logout_clicked();
}
void SimuTool::on_button_task()
{
	emit button_task_clicked();
}
void SimuTool::on_button_revert()
{
	emit button_revert_clicked();
}
void SimuTool::on_button_localOprate()
{
	emit button_localOprate_clicked();
}
void SimuTool::on_button_openOix()
{
	emit button_openOix_clicked();
}
void SimuTool::on_button_Exam()
{
	emit button_Exam_clicked();
}
void SimuTool::on_button_simuControl()
{
	emit button_simuControl_clicked();
}
void SimuTool::on_button_setScene()
{
	emit button_setScene_clicked();
}
void SimuTool::on_button_setFault()
{
	emit button_setFault_clicked();
}
void SimuTool::on_button_sendMessage()
{
	emit button_sendMessage_clicked();
}
void SimuTool::on_button_setDesplay()
{
	emit button_setDesplay_clicked();
}
void SimuTool::on_button_doccuments()
{
	emit button_doccuments_clicked();
}
void SimuTool::on_button_help()
{
	emit button_help_clicked();
}
void SimuTool::on_act_hide()
{
	this->hide();
}
void SimuTool::on_act_close()
{
	emit button_logout_clicked();
//	this->close();
}void SimuTool::MysetIndex( int index )
{
// 	if (index==4&&cur_index>=0)
// 	{
// 		setCurrentIndex(cur_index);
// 	}
// 	else
		cur_index = index;
}
void  SimuTool::setLanguage(int language)
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