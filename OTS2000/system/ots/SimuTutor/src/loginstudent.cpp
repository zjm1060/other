#include "loginstudent.h"

CLoginStudentTable::CLoginStudentTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	select_status = false;
	actionSetTask = new QAction(QString::fromLocal8Bit("设置任务"), this);
	actionSetScene = new QAction(QString::fromLocal8Bit("设置工况"), this);
	actionSetFault = new QAction(QString::fromLocal8Bit("设置故障"), this);
	actionTrainManager = new QAction(QString::fromLocal8Bit("学员培训管理"), this);
	actionLogout = new QAction(QString::fromLocal8Bit("强制退出登录"),this);
	studentsready = studentsmode = students_state = QString::fromLocal8Bit("");
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	if( SelectRecs(&data)== TRUE)
	{
		if (data.rownum>0)
		{
			if (data_train_info[0].task_startable== 1)
			{
				studentsready = QString::fromLocal8Bit("可开始任务");
				label_curstate->setStyleSheet("color: rgb(0, 255, 0);");
			}
			else
			{
				studentsready = QString::fromLocal8Bit("不可开始任务");
				label_curstate->setStyleSheet("color: rgb(255, 0, 0);");
			}
		}
	}	
	students_state = studentsmode +studentsready;
	label_curstate->setText(students_state);
	refresh = new QTimer(this);
	model = new QSortFilterProxyModel;
	setControlAtt();
	//connect(listLoginStudent,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(buttonTask,SIGNAL(clicked()),this,SLOT(openSetTaskTable()));
	connect(buttonScene,SIGNAL(clicked()),this,SLOT(openSetSceneTable()));
	connect(buttonFault,SIGNAL(clicked()),this,SLOT(openSetFaultTable()));
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(updateModel()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));
	connect(actionSetTask,SIGNAL(triggered()),this,SLOT(openSetTaskTable()));
	connect(actionSetScene,SIGNAL(triggered()),this,SLOT(openSetSceneTable()));
	connect(actionSetFault,SIGNAL(triggered()),this,SLOT(openSetFaultTable()));
	connect(actionTrainManager,SIGNAL(triggered()),this,SLOT(openTrainManagerTable()));
	connect(actionLogout,SIGNAL(triggered()),this,SLOT(logoutForce()));
	connect(comboBox,SIGNAL(currentIndexChanged ( int )),this,SLOT(settrainmode(int)));
	connect(pushButtonLogoutAll,SIGNAL(clicked()),this,SLOT(logoutAllusers()));
	connect(pushButtonExwait,SIGNAL(clicked()),this,SLOT(setAllusersWait()));
	connect(pushButtonExamGo,SIGNAL(clicked()),this,SLOT(setAllusersStart()));
	//connect(refresh, SIGNAL(timeout()), this, SLOT(update()));
	//refresh->start(5000);
}


void CLoginStudentTable::setControlAtt()
{
	viewLoginStudent->setSelectionBehavior(QAbstractItemView::SelectRows);
	viewLoginStudent->setSelectionMode(QAbstractItemView::SingleSelection); 
	viewLoginStudent->setEditTriggers(QTableView::NoEditTriggers);
	viewLoginStudent->verticalHeader()->setVisible(false);
	model->setSourceModel(trainInfoModel);
	model->setDynamicSortFilter(true);
	viewLoginStudent->setModel(model);
	selectionModel = viewLoginStudent->selectionModel();
	model->setFilterKeyColumn(5);
	model->setFilterRegExp(QRegExp(QString::fromLocal8Bit("已登录")));
}
void CLoginStudentTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CLoginStudentTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CLoginStudentTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	/*QTableWidgetItem* pTableWidgetItem=listLoginStudent->itemAt(listLoginStudent->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionSetTask);
		popMenu->addAction(actionSetScene);
		popMenu->addAction(actionSetFault);
		popMenu->addAction(actionLogout);
	}*/
	popMenu->addAction(actionSetTask);
	popMenu->addAction(actionSetScene);
	popMenu->addAction(actionSetFault);
	popMenu->addAction(actionTrainManager);
	popMenu->addAction(actionLogout);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CLoginStudentTable::showEvent(QShowEvent *e)
{
	label_curstate->setText(students_state);
	e->accept();
}
void  CLoginStudentTable::setLanguage(int language)
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
	label_curstate->setText(students_state);
}
void CLoginStudentTable::updateModel()
{
	UpdateTrainInfoModel();
	comboBox->setCurrentIndex(0);
}

void CLoginStudentTable::openSetTaskTable()
{
	/*QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() != 0)
	{	
		CSetTaskTable* settasktable = new CSetTaskTable(this);
		//settasktable->multiMode(TRUE);
		for (int i=0;i<indexes.size();i++)
		{
			settasktable->listViewStudent->setCurrentIndex(indexes.at(i));
		}
		settasktable->show();
	}
	else
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("请选择要设置任务的学员!"),QMessageBox::Ok);
	}*/
	CSetTaskTable* settasktable = new CSetTaskTable;
	settasktable->setWindowFlags(Qt::WindowStaysOnTopHint);
	settasktable->showMaximized();
}
void CLoginStudentTable::openSetSceneTable()
{
	setscenetable = new CSetSceneFaultTable;
	setscenetable->initScene();
	setscenetable->setWindowFlags(Qt::WindowStaysOnTopHint);
	setscenetable->showMaximized();
	setscenetable->setSceneModel();
	connect(setscenetable,SIGNAL(sigSendScene(int)),this,SLOT(setScene(int)));
}
void CLoginStudentTable::openSetFaultTable()
{
	setfaulttable = new CSetSceneFaultTable;
	setfaulttable->initFault();
	setfaulttable->setWindowFlags(Qt::WindowStaysOnTopHint);
	setfaulttable->showMaximized();
	setfaulttable->setFaultModel();
	connect(setfaulttable,SIGNAL(sigSendFault(int)),this,SLOT(setFault(int)));
}
void CLoginStudentTable::openTrainManagerTable()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() == 0)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("请选择学员!"),QMessageBox::Ok);
		return;
	}
	trainmanagertable = new CTrainManagerTable(this);
	for (int i=0;i<indexes.size();i++)
	{
		QStandardItem *itemUser = trainInfoModel->itemFromIndex(model->mapToSource(model->index(indexes.at(i).row(),0)));
		int user_id = itemUser->data().toInt();
		QStandardItem *itemHost = trainInfoModel->itemFromIndex(model->mapToSource(model->index(indexes.at(i).row(),1)));
		int host_id = itemHost->data().toInt();
		trainmanagertable->setUserInfo(user_id,host_id);		
	}
	trainmanagertable->show();
}
void CLoginStudentTable::setScene(int scene_id)
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() != 0)
	{	
		for (int i=0;i<indexes.size();i++)
		{
			QStandardItem *itemUser = trainInfoModel->itemFromIndex(model->mapToSource(model->index(indexes.at(i).row(),0)));
			int user_id = itemUser->data().toInt();
			QStandardItem *itemHost = trainInfoModel->itemFromIndex(model->mapToSource(model->index(indexes.at(i).row(),1)));
			int host_id = itemHost->data().toInt();
			DB_FMT data;
			InitDbData(&data,TABLE_TRAIN_INFO);
			data.flag.flag_train_info.user_id = TRUE;
			data.seg.seg_train_info.user_id = user_id;
			data.cond_flag.flag_train_info.host_id = TRUE;
			data.cond_seg.seg_train_info.host_id = host_id;

			data.flag.flag_train_info.scene_id = TRUE;
			data.seg.seg_train_info.scene_id = scene_id;
			data.flag.flag_train_info.scene_status = TRUE;
			data.seg.seg_train_info.scene_status = 1;
			if (UpdateOneRec(&data)!=(int)TRUE)
			{
				QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("向%1设置工况失败!").arg(model->data(model->index(indexes.at(i).row(),0)).toString()),QMessageBox::Ok);
				HdbLogWrite(data.sql_str,data.sql_error);
			}
			else
			{	
				UpdateTrainInfoModel();
			}
		}
	}
}
void CLoginStudentTable::setFault(int fault_id)
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() != 0)
	{	
		for (int i=0;i<indexes.size();i++)
		{
			QStandardItem *itemUser = trainInfoModel->itemFromIndex(model->mapToSource(model->index(indexes.at(i).row(),0)));
			int user_id = itemUser->data().toInt();
			QStandardItem *itemHost = trainInfoModel->itemFromIndex(model->mapToSource(model->index(indexes.at(i).row(),1)));
			int host_id = itemHost->data().toInt();
			DB_FMT data;
			InitDbData(&data,TABLE_TRAIN_INFO);
			data.flag.flag_train_info.user_id = TRUE;
			data.seg.seg_train_info.user_id = user_id;
			data.cond_flag.flag_train_info.host_id = TRUE;
			data.cond_seg.seg_train_info.host_id = host_id;

			data.flag.flag_train_info.fault_id = TRUE;
			data.seg.seg_train_info.fault_id = fault_id;
			data.flag.flag_train_info.fault_status = TRUE;
			data.seg.seg_train_info.fault_status = 1;
			if (UpdateOneRec(&data)!=(int)TRUE)
			{
				QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("向%1设置故障失败!").arg(model->data(model->index(indexes.at(i).row(),0)).toString()),QMessageBox::Ok);
				HdbLogWrite(data.sql_str,data.sql_error);
			}
			else
			{
				UpdateTrainInfoModel();
			}	
		}
	}
}
void CLoginStudentTable::selectCell(int row, int column)
{
	select_status = true;
	rows = row;
	columns = column;
}
void  CLoginStudentTable::logoutAllusers()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	data.cond_flag.flag_train_info.login_status = TRUE;
	data.cond_seg.seg_train_info.login_status = 1;
	data.flag.flag_train_info.login_status = TRUE;
	data.flag.flag_train_info.task_id = TRUE;
	data.flag.flag_train_info.scene_id = TRUE;
	data.flag.flag_train_info.fault_id = TRUE;
	data.seg.seg_train_info.login_status = 0;
	data.seg.seg_train_info.task_id = CONST_NEG;
	data.seg.seg_train_info.scene_id = CONST_NEG;
	data.seg.seg_train_info.fault_id = CONST_NEG;
	if( UpdateOneRec(&data) != TRUE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("踢出用户失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	UpdateTrainInfoModel();
}
void CLoginStudentTable::logoutForce()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() != 0)
	{	
		for (int i=0;i<indexes.size();i++)
		{
			QStandardItem *item = trainInfoModel->itemFromIndex(model->mapToSource(model->index(indexes.at(i).row(),0)));
			int user_id = item->data().toInt();
			DB_FMT data;
			InitDbData(&data,TABLE_TRAIN_INFO);
			data.flag.flag_train_info.login_status = TRUE;
			data.flag.flag_train_info.task_id = TRUE;
			data.flag.flag_train_info.scene_id = TRUE;
			data.flag.flag_train_info.fault_id = TRUE;
			data.seg.seg_train_info.login_status = 0;
			data.seg.seg_train_info.task_id = CONST_NEG;
			data.seg.seg_train_info.scene_id = CONST_NEG;
			data.seg.seg_train_info.fault_id = CONST_NEG;

			data.cond_flag.flag_userinfo.user_id = TRUE;
			data.cond_seg.seg_userinfo.user_id = user_id;
			if( UpdateOneRec(&data) != TRUE)
			{
				QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("强制退出登录失败!"),QMessageBox::Ok);		
				HdbLogWrite(data.sql_str,data.sql_error);
				return;
			}
		}
		UpdateTrainInfoModel();
	}
	else 
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("请选择要强制退出的学员!"),QMessageBox::Ok);
	}
}
void CLoginStudentTable::settrainmode(int i)
{
	int mode;
	switch(i)
	{
		case 0:
			//studentsmode = QString::fromLocal8Bit("模式未选,");
			return;
			break;
		case 1:
			studentsmode = QString::fromLocal8Bit("自由模式,");
			students_state = studentsmode+studentsready;
			label_curstate->setText(students_state);
			return;
			//break;		
		case 2:
			studentsmode = QString::fromLocal8Bit("培训模式,");
			mode = 0;
			break;
		case 3:
			studentsmode = QString::fromLocal8Bit("考核模式,");
			mode = 1;
			break;
		case 4:
			studentsmode = QString::fromLocal8Bit("竞赛模式,");
			mode = 2;
			break;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		//qDebug("查询数据库失败!");
		return;
	}
	else
	{
		for(int n = 0;n <data.rownum;n++)
		{
			if (data_train_info[n].login_status == 1)
			{
// 				data.cond_flag.flag_train_info.user_id = TRUE;
// 				data.cond_seg.seg_train_info.user_id = data_train_info[n].user_id;
// 				data.cond_flag.flag_train_info.host_id = TRUE;
// 				data.cond_seg.seg_train_info.host_id = data_train_info[n].host_id;
				DB_FMT data2;
				InitDbData(&data2,TABLE_TRAIN_INFO);
				data2.flag.flag_train_info.simu_mode = TRUE;
				data2.seg.seg_train_info.simu_mode = mode;
				if (UpdateOneRec(&data2)!=TRUE)
				{
					HdbLogWrite(data2.sql_str,data2.sql_error);
					return;
				}
			}			
		}
	}
	students_state = studentsmode+studentsready;
	label_curstate->setText(students_state);
	UpdateTrainInfoModel();
}
void  CLoginStudentTable::setAllusersWait()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	data.flag.flag_train_info.task_startable = TRUE;
	data.seg.seg_train_info.task_startable = 0;
	if( UpdateOneRec(&data) != TRUE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户准备接受任务状态设置失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	studentsready = QString::fromLocal8Bit("不可开始任务");
	students_state = studentsmode+studentsready;
	label_curstate->setStyleSheet("color: rgb(255, 0, 0);");
	label_curstate->setText(students_state);
	//UpdateTrainInfoModel();
}
void  CLoginStudentTable::setAllusersStart()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	data.flag.flag_train_info.task_startable = TRUE;
	data.seg.seg_train_info.task_startable = 1;
	if( UpdateOneRec(&data) != TRUE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("用户可以接受任务状态设置失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	studentsready = QString::fromLocal8Bit("可开始任务");
	students_state = studentsmode+studentsready;
	label_curstate->setStyleSheet("color: rgb(0, 255, 0);");
	label_curstate->setText(students_state);
}
/*****************************************************CTrainManager********************************************************/
CTrainManagerTable::CTrainManagerTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
// 	if (flag_langueage == 0)
// 	{
// 		loadChinese();
// 	}
// 	else if (flag_langueage == 1)
// 	{
// 		loadEnglish();
// 	}
	this->setModal(true);
	user_id = 0;
	host_id = 0;
	list_speed_level << 1 << 2 << 4 << 8 << 16 << 32 << 64 << 128;
	connect(buttonSimuStart,SIGNAL(clicked()),this,SLOT(setSimuStart()));
	connect(buttonSimuPause,SIGNAL(clicked()),this,SLOT(setSimuPause()));
	connect(buttonSimuStop,SIGNAL(clicked()),this,SLOT(setSimuStop()));
	connect(comboSimuMode,SIGNAL(currentIndexChanged(int)),this,SLOT(setSimuMode(int)));
	connect(sliderSimuSpeed,SIGNAL(valueChanged(int)),this,SLOT(setSimuSpeed(int)));
	connect(comboBox_interlock,SIGNAL(currentIndexChanged(int)),this,SLOT(setinterlockmode(int)));
}
void CTrainManagerTable::showEvent(QShowEvent *event)
{
	getSimuInfo();
	comboSimuMode->setCurrentIndex(simu_mode);
	sliderSimuSpeed->setValue(list_speed_level.indexOf(simu_speed));
	comboBox_interlock->setCurrentIndex(interlock_mode);
	switch(simu_status)
	{
	case 0 :	
		buttonSimuStart->setEnabled(true);
		buttonSimuPause->setEnabled(false);
		buttonSimuStop->setEnabled(false);
		break;
	case 1 :	
		buttonSimuStart->setEnabled(false);
		buttonSimuPause->setEnabled(true);
		buttonSimuStop->setEnabled(true);
		break;
	case 2 :
		buttonSimuStart->setEnabled(true);
		buttonSimuPause->setEnabled(false);
		buttonSimuStop->setEnabled(true);
		break;
	default:	
		break;
	}
}
void  CTrainManagerTable::setLanguage(int language)
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
void CTrainManagerTable::getSimuInfo()
{
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id = TRUE;
	data.cond_seg.seg_train_info.user_id = user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = host_id;
	if(SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return;
	}
	if (data.rownum != 1)
	{
		qDebug("查询结果错误!");
		return;
	}
	simu_mode = data_train_info[0].simu_mode;
	simu_speed = data_train_info[0].speed;
	simu_status = data_train_info[0].status;
	interlock_mode = data_train_info[0].inter_mode;  
}
void CTrainManagerTable::setUserInfo(int m_user_id,int m_host_id)
{
	user_id = m_user_id;
	host_id = m_host_id;
}
void CTrainManagerTable::setSimuStart()
{
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id=TRUE;
	data.cond_seg.seg_train_info.user_id=user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id= host_id;
	data.flag.flag_train_info.status=TRUE;
	data.seg.seg_train_info.status=1;
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	buttonSimuStart->setEnabled(false);
	buttonSimuPause->setEnabled(true);
	buttonSimuStop->setEnabled(true);
}
void CTrainManagerTable::setSimuPause()
{
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id = TRUE;
	data.cond_seg.seg_train_info.user_id = user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = host_id;
	data.flag.flag_train_info.status = TRUE;
	data.seg.seg_train_info.status = 2;
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	buttonSimuStart->setEnabled(true);
	buttonSimuPause->setEnabled(false);
	buttonSimuStop->setEnabled(true);
}
void CTrainManagerTable::setSimuStop()
{
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id = TRUE;
	data.cond_seg.seg_train_info.user_id = user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = host_id;
	data.flag.flag_train_info.status = TRUE;
	data.seg.seg_train_info.status = 0;
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	buttonSimuStart->setEnabled(true);
	buttonSimuPause->setEnabled(false);
	buttonSimuStop->setEnabled(false);
}
void CTrainManagerTable::setSimuMode(int index)
{
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id = TRUE;
	data.cond_seg.seg_train_info.user_id = user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = host_id;
	data.flag.flag_train_info.simu_mode = TRUE;
	data.seg.seg_train_info.simu_mode = index;
	if (UpdateOneRec(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}
void CTrainManagerTable::setSimuSpeed(int value)
{
	DB_FMT data;
	data.table_id = TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.user_id = TRUE;
	data.cond_seg.seg_train_info.user_id = user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = host_id;
	data.flag.flag_train_info.speed=TRUE;
	data.seg.seg_train_info.speed = list_speed_level.at(value);
	if (UpdateOneRec(&data)!=(int)TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	switch(value)
	{
		case 0 :	labelSimuSpeed->setText("1/8 X"); break;
		case 1 :	labelSimuSpeed->setText("1/4 X"); break;
		case 2 :	labelSimuSpeed->setText("1/2 X"); break;
		case 3 :	labelSimuSpeed->setText("1 X"); break;
		case 4 :	labelSimuSpeed->setText("2 X"); break;
		case 5 :	labelSimuSpeed->setText("4 X"); break;
		case 6 :	labelSimuSpeed->setText("8 X"); break;
		case 7 : labelSimuSpeed->setText("16 X"); break;
	}
}

void CTrainManagerTable::setinterlockmode(int i)
{
	MySleep(1000);
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		//qDebug("查询数据库失败!");
		return;
	}
	else
	{
		InitDbData(&data,data.table_id);
		data.cond_flag.flag_train_info.user_id = TRUE;
		data.cond_seg.seg_train_info.user_id = user_id;
		data.cond_flag.flag_train_info.host_id = TRUE;
		data.cond_seg.seg_train_info.host_id = host_id;
		data.flag.flag_train_info.inter_mode=TRUE;
		data.seg.seg_train_info.inter_mode =i;
		if (UpdateOneRec(&data)!=(int)TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
}