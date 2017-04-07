#include "contest.h"

CContestTable::CContestTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
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
	this->setAttribute(Qt::WA_DeleteOnClose);
	select_status = false;

	actionAddContest = new QAction(QString::fromLocal8Bit("添加竞赛"), this);
	actionModifyContest = new QAction(QString::fromLocal8Bit("修改竞赛"), this);
	actionDelContest = new QAction(QString::fromLocal8Bit("删除竞赛"), this);
	setControlAtt();
	displayContest();
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displayContest()));
 	connect(listContest,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
 	connect(buttonContestDel,SIGNAL(clicked()),this,SLOT(delContest()));
 	connect(buttonContestAdd,SIGNAL(clicked()),this,SLOT(openContestAdd()));
	connect(buttonContestModify,SIGNAL(clicked()),this,SLOT(openContestModify()));
 	connect(actionAddContest,SIGNAL(triggered()),this,SLOT(openContestAdd()));
	connect(actionModifyContest,SIGNAL(triggered()),this,SLOT(openContestModify()));
 	connect(actionDelContest,SIGNAL(triggered()),this,SLOT(delContest()));
// 	connect(buttonStnDev,SIGNAL(clicked()),this,SLOT(openStnDev()));
 //	connect(buttonExit,SIGNAL(clicked()),stndev,SLOT(close()));

}
void CContestTable::setControlAtt()
{
	listContest->setSelectionBehavior(QAbstractItemView::SelectRows);
	listContest->setSelectionMode(QAbstractItemView::SingleSelection); 
	listContest->setEditTriggers(QTableWidget::NoEditTriggers);
	//listContest->hideColumn(6);
	listContest->setSortingEnabled(true);
}
void CContestTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	contestadd->loadChinese();
// 	contestmodify->loadChinese();
}
void CContestTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	contestadd->loadEnglish();
// 	contestmodify->loadEnglish();
}
void CContestTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listContest->itemAt(listContest->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddContest);
		popMenu->addAction(actionModifyContest);
		popMenu->addAction(actionDelContest);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CContestTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void CContestTable::closeEvent(QCloseEvent *event)
{
	this->close();
}
void  CContestTable::setLanguage(int language)
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
void CContestTable::openContestAdd()
{
	contestadd = new CContestAddTable(this);
	contestadd->flag_add=true;
	contestadd->buttonApply->hide();
	contestadd->contest_id = GetInexistenceIdInHdb(TABLE_CONTEST);
	contestadd->show();
	connect(contestadd,SIGNAL(insertRecord()),this,SLOT(addContest()));
}
void CContestTable::openContestModify()
{
	int task_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的任务!"),QMessageBox::Ok);
	}
	else
	{
		contestmodify = new CContestAddTable(this);
		contestmodify->flag_modify=true;
		contestmodify->displayFault();
		contestmodify->displayTask();
		contestmodify->displayScene();
		QTableWidgetItem* item_id = listContest->item(curRow,0);
		contest_id = item_id->text().toInt();
		curContestId=contest_id;
		contestmodify->contest_id = contest_id;
		QTableWidgetItem* name = listContest->item(curRow,1);
		contestmodify->editName->setText(name->text());
		QTableWidgetItem* contest__state = listContest->item(curRow,2);
		if(contest__state->text()==QString::fromLocal8Bit("开放中"))
		{
			contestmodify->comboBox_Open->setCurrentIndex(1);	
		}
		else if(contest__state->text()==QString::fromLocal8Bit("停用中"))
		{
			contestmodify->comboBox_Open->setCurrentIndex(0);
		}
		else 
		{
			contestmodify->comboBox_Open->setCurrentIndex(-1);
		}
		QTableWidgetItem* task = listContest->item(curRow,3);
		if (task->text() == QString(""))
		{
			contestmodify->editTaskName->setCurrentIndex(0);
		}
		else
		{
			contestmodify->editTaskName->setCurrentIndex(contestmodify->editTaskName->findText(task->text(),Qt::MatchContains));
		}
		QTableWidgetItem* scenename = listContest->item(curRow,4);
		if (scenename->text()==QString(""))
		{
			contestmodify->editScene->setCurrentIndex(0);
		}
		else
		{
			contestmodify->editScene->setCurrentIndex(contestmodify->editScene->findText(scenename->text(),Qt::MatchContains));
		}
		QTableWidgetItem* faultname = listContest->item(curRow,5);
		if (faultname->text()==QString(""))
		{
			contestmodify->editFault->setCurrentIndex(0);
		} 
		else
		{
			QString str = faultname->text();
			int i =contestmodify->editFault->findText(faultname->text(),Qt::MatchContains);
			contestmodify->editFault->setCurrentIndex(contestmodify->editFault->findText(faultname->text(),Qt::MatchContains));
		}
		contestmodify->setWindowTitle(QString::fromLocal8Bit("修改竞赛"));
		QTableWidgetItem* remark = listContest->item(curRow,7);
		contestmodify->editRemark->setText(remark->text());
		contestmodify->show();	
		connect(contestmodify,SIGNAL(modifyRecord()),this,SLOT(modifyContest()));
	}
}

int CContestTable::displayContest()
{
	DB_FMT data;
	InitDbData(&data,TABLE_CONTEST);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	if (data.rownum>0)
	{
			listContest->setRowCount(data.rownum);
	}
	else
		return 0;
	for(int i=0;i<data.rownum;i++)
	{	
		listContest->setItem(i,0,new QTableWidgetItem(QString::number(data_contest[i].contest_id)));
		listContest->setItem(i,1,new QTableWidgetItem(QString::fromLocal8Bit(data_contest[i].name)));
		if (data_contest[i].open_state == 0)
		{
			listContest->setItem(i,2,new QTableWidgetItem(QString::fromLocal8Bit("停用中")));
		}
		else
			listContest->setItem(i,2,new QTableWidgetItem(QString::fromLocal8Bit("开放中")));
		listContest->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit(data_contest[i].task_name)));
		listContest->setItem(i,4,new QTableWidgetItem(QString::fromLocal8Bit(data_contest[i].scene_name)));
		listContest->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit(data_contest[i].fault_name)));
		listContest->setItem(i,6,new QTableWidgetItem(QString::fromLocal8Bit(data_contest[i].datetime)));
		listContest->setItem(i,7,new QTableWidgetItem(QString::fromLocal8Bit(data_contest[i].remark)));
	}
	listContest->clearSelection();
	listContest->resizeColumnsToContents();
	return data.rownum;
}
void CContestTable::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CContestTable::delContest()
{
	if(select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要删除的任务!"),QMessageBox::Ok);
	}
	else
	{
		QMessageBox msgBox(this);
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
		msgBox.setInformativeText(QString::fromLocal8Bit("是否确认删除"));
		msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Cancel) 
		{
			return;
		}
		DB_FMT data;
		QTableWidgetItem* flag_contest_id = listContest->item(curRow,0);
		int t = flag_contest_id->text().toInt();
		//先删题目，再删任务 dzy
		InitDbData(&data,TABLE_CONTEST);		
		data.cond_flag.flag_contest.contest_id=TRUE;
		data.cond_seg.seg_contest.contest_id = flag_contest_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
	rownums = displayContest();
}
void CContestTable::addContest()
{
	DB_FMT data;
	InitDbData(&data,TABLE_CONTEST);

	//设置表中每个字段的flag
	data.flag.flag_contest.contest_id =TRUE;
	data.flag.flag_contest.name =TRUE;
	data.flag.flag_contest.open_state=TRUE;
	data.flag.flag_contest.task_id = TRUE;
	data.flag.flag_contest.scene_id=TRUE;
	data.flag.flag_task.fault_id=TRUE;
	data.flag.flag_contest.remark=TRUE;
	data.flag.flag_contest.datetime=TRUE;
	//填充表中每个字段的值

	contestadd->seg.contest_id = GetMaxIdInHdb(TABLE_CONTEST)+1;
	GetCurTimeStr( contestadd->seg.datetime);
	data.seg.seg_contest = contestadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	rownums = displayContest();
}
void CContestTable::modifyContest()
{
	DB_FMT data;
	InitDbData(&data,TABLE_CONTEST);
	//设置表中每个字段的flag
	data.flag.flag_contest.name=TRUE;
	data.flag.flag_contest.datetime=TRUE;
	data.flag.flag_contest.task_id=TRUE;
	data.flag.flag_contest.open_state=TRUE;
	data.flag.flag_contest.remark=TRUE;
	data.flag.flag_contest.scene_id=TRUE;
	data.flag.flag_contest.fault_id=TRUE;

	//填充表中每个字段的值
	GetCurTimeStr(contestmodify->seg.datetime);
	data.seg.seg_contest = contestmodify->seg;

	//设置查询条件
	data.cond_flag.flag_contest.contest_id=TRUE;
	data.cond_seg.seg_contest.contest_id = curContestId;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	rownums = displayContest();
}

/**************************************************CContestAddTable*********************************************************/
CContestAddTable::CContestAddTable(QWidget* parent)
:QDialog(parent)
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
	flag_add=false;
	flag_modify=false;
	displayTask();
	displayScene();
	displayFault();
	setControlAtt();
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
}
void CContestAddTable::setControlAtt()
{
	comboBox_Open->setCurrentIndex(-1);
	editName->setText("");
	editTaskName->setCurrentIndex(-1);
	editScene->setCurrentIndex(-1);
	editFault->setCurrentIndex(-1);
	editRemark->setText("");
}
void CContestAddTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void CContestAddTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CContestAddTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CContestAddTable::displayTask()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TASK);
	data.cond_flag.flag_task.task_userange = TRUE;
	data.cond_seg.seg_task.task_userange = 0;//考试专用
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	editTaskName->clear();
	if(data.rownum<=0)
		return;
	editTaskName->insertItem(0,QString::fromLocal8Bit("无"));
	for (int i=1;i<data.rownum+1;i++)
	{
		editTaskName->insertItem(i,QString::fromLocal8Bit(data_task[i].name));
	}
}
void CContestAddTable::displayScene()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SCENE);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	editScene->clear();
	if(data.rownum<=0)
		return;
	editScene->insertItem(0,QString::fromLocal8Bit("无"));
	for (int i=1;i<data.rownum;i++)
	{
		editScene->insertItem(i,QString::fromLocal8Bit(data_scene[i].name));
	}
}
void CContestAddTable::displayFault()
{
	DB_FMT data;
	InitDbData(&data,TABLE_FAULT);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	editFault->clear();
	if(data.rownum<=0)
		return;
	editFault->insertItem(0,QString::fromLocal8Bit("无"));
	for (int i=1;i<data.rownum;i++)
	{
		editFault->insertItem(i,QString::fromLocal8Bit(data_fault[i].name));
	}
}
void CContestAddTable::apply()
{	
	if (editTaskName->currentIndex()==0||editTaskName->currentText()==QString(""))
	{
		seg.task_id = CONST_NEG;
		//strcpy(seg.scene_name,CONST_NEG_STR);
	}
	else
	{
		seg.task_id = taskId(editTaskName->currentText().toLocal8Bit().data());
		//strcpy(seg.scene_name,editScene->currentText().toLocal8Bit().data());
	}
	if (editScene->currentIndex()==0||editScene->currentText()==QString(""))
	{
		seg.scene_id = CONST_NEG;
		//strcpy(seg.scene_name,CONST_NEG_STR);
	}
	else
	{
		seg.scene_id = sceneId(editScene->currentText().toLocal8Bit().data());
		//strcpy(seg.scene_name,editScene->currentText().toLocal8Bit().data());
	}
	if (editFault->currentIndex()==0||editFault->currentText()==QString(""))
	{
		seg.fault_id = CONST_NEG;
		//strcpy(seg.fault_id,CONST_NEG_STR);
	}
	else
	{
		seg.fault_id = faultId(editFault->currentText().toLocal8Bit().data());
		//strcpy(seg.fault_name,editFault->currentText().toLocal8Bit().data());
	}

	seg.open_state = comboBox_Open->currentIndex();
	strcpy(seg.name,editName->text().toLocal8Bit().data());	
	strcpy(seg.remark,remarks.toLocal8Bit().data());
	char buf[100];
	memcpy(buf,editRemark->toPlainText().toLocal8Bit().data(),100);
	strcpy(seg.remark,buf);	
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}
void  CContestAddTable::setLanguage(int language)
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
int CContestAddTable::taskId(char *task_name)
{
	DB_FMT data;
	InitDbData(&data,TABLE_TASK);
	data.cond_flag.flag_task.name = TRUE;
	strcpy(data.cond_seg.seg_task.name,task_name);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	return data_task[0].task_id;
}
int CContestAddTable::sceneId(char *scene_name)
{
	DB_FMT data;
	InitDbData(&data,TABLE_SCENE);
	data.cond_flag.flag_scene.name = TRUE;
	strcpy(data.cond_seg.seg_scene.name,scene_name);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	return data_scene[0].scene_id;
}
int CContestAddTable::faultId(char *fault_name)
{
	DB_FMT data;
	InitDbData(&data,TABLE_FAULT);
	data.cond_flag.flag_fault.name = TRUE;
	strcpy(data.cond_seg.seg_fault.name,fault_name);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	return data_fault[0].fault_id;
}
void CContestAddTable::enter()
{
	apply();
	flag_add=false;
	flag_modify=false;
	setControlAtt();//DZY 2014
	accept();
}
