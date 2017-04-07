#include "task.h"
#include "excelobject.h"

CTaskTable::CTaskTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	select_status = false;
	select_stn = false;
	select_stndev = false;
	select_grep = false;
	select_type = false;
	actionAddTask = new QAction(QString::fromLocal8Bit("添加任务"), this);
	actionModifyTask = new QAction(QString::fromLocal8Bit("修改任务"), this);
	actionDelTask = new QAction(QString::fromLocal8Bit("删除任务"), this);
	stndev = new CStnDevTable;
	setControlAtt();
	//displayTask();
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displayTask()));
	connect(listTask,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(buttonTaskDel,SIGNAL(clicked()),this,SLOT(delTask()));
	connect(buttonTaskAdd,SIGNAL(clicked()),this,SLOT(openTaskAdd()));
	connect(buttonTaskModify,SIGNAL(clicked()),this,SLOT(openTaskModify()));
	connect(actionAddTask,SIGNAL(triggered()),this,SLOT(openTaskAdd()));
	connect(actionModifyTask,SIGNAL(triggered()),this,SLOT(openTaskModify()));
	connect(actionDelTask,SIGNAL(triggered()),this,SLOT(delTask()));
	connect(buttonStnDev,SIGNAL(clicked()),this,SLOT(openStnDev()));
	connect(buttonExit,SIGNAL(clicked()),stndev,SLOT(close()));
	connect(stndev,SIGNAL(update(int)),this,SLOT(displayUpdate(int)));
	connect(stndev,SIGNAL(update(int,int)),this,SLOT(displayUpdate(int,int)));
	//dzy
	connect(buttonSelectByCond,SIGNAL(clicked()),this,SLOT(selectTask()));
	connect(comboBox,SIGNAL(currentIndexChanged(int )),this,SLOT(displaytaskselect(int)));
}
void CTaskTable::setControlAtt()
{
	listTask->setSelectionBehavior(QAbstractItemView::SelectRows);
	listTask->setSelectionMode(QAbstractItemView::SingleSelection); 
	listTask->setEditTriggers(QTableWidget::NoEditTriggers);
	listTask->hideColumn(6);
	listTask->setSortingEnabled(true);
}
void CTaskTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	taskadd->loadChinese();
	taskmodify->loadChinese();
}
void CTaskTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	taskadd->loadEnglish();
	taskmodify->loadEnglish();
}
void CTaskTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listTask->itemAt(listTask->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddTask);
		popMenu->addAction(actionModifyTask);
		popMenu->addAction(actionDelTask);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CTaskTable::moveEvent ( QMoveEvent * event )
{
	if (stndev != NULL )
	{
		stndev->mvwith = true;
		stndev->newpt= event->pos();
		this->pos() = event->pos();
		stndev->moveEvent(event);
	}	
}
void CTaskTable::closeEvent(QCloseEvent *event)
{
	this->close();
	stndev->close();
}
void CTaskTable::openStnDev()
{
	if (stndev == NULL||stndev->isHidden())
	{
		QPoint pt=this->pos();
		int x = pt.x()+this->width();
		stndev->xP = this->width();
		stndev->move(x+7,pt.y());
		stndev->show();
	}
	else
	{
		select_stn = false;
		select_stndev = false;
		stndev->hide();
		displayTask();
	}
}
void CTaskTable::openTaskAdd()
{
	taskadd = new CTaskAddTable(this);
	taskadd->flag_add=true;
	taskadd->buttonApply->hide();
	taskadd->task_id = GetInexistenceIdInHdb(TABLE_TASK);
	taskadd->show();
	connect(taskadd,SIGNAL(insertRecord()),this,SLOT(addTask()));
}
void CTaskTable::openTaskModify()
{
	int task_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的任务!"),QMessageBox::Ok);
	}
	else
	{
		taskmodify = new CTaskAddTable(this);
		taskmodify->flag_modify=true;
		taskmodify->displayFault();
		QTableWidgetItem* item_id = listTask->item(curRow,0);
		task_id = item_id->text().toInt();
		curTaskId=task_id;
		taskmodify->task_id = task_id;
		QTableWidgetItem* name = listTask->item(curRow,1);
		taskmodify->editName->setText(name->text());
		QTableWidgetItem* type = listTask->item(curRow,5);
		if(type->text()==QString::fromLocal8Bit("正常操作"))
		{
			taskmodify->editType->setCurrentIndex(0);	
		}
		else if(type->text()==QString::fromLocal8Bit("故障处理"))
		{
			taskmodify->editType->setCurrentIndex(1);
		}
		else 
		{
			taskmodify->editType->setCurrentIndex(-1);
		}
		QTableWidgetItem* station = listTask->item(curRow,9);
		if (station->text() == QString(""))
		{
			taskmodify->editStation->setCurrentIndex(0);
		}
		else
		{
			taskmodify->editStation->setCurrentIndex(taskmodify->editStation->findText(station->text(),Qt::MatchContains));
		}
		QTableWidgetItem* dev = listTask->item(curRow,10);
		if (dev->text() == QString(""))
		{
			taskmodify->editDev->setCurrentIndex(0);
		}
		else
		{
			taskmodify->editDev->setCurrentIndex(taskmodify->editDev->findText(dev->text(),Qt::MatchContains));
		}
		QTableWidgetItem* remark = listTask->item(curRow,8);
		taskmodify->editRemark->setText(remark->text());
		QTableWidgetItem* grpname = listTask->item(curRow,2);
		taskmodify->editGrpName->setCurrentIndex(taskmodify->editGrpName->findText(grpname->text()));
		QTableWidgetItem* scenename = listTask->item(curRow,3);
		if (scenename->text()==QString(""))
		{
			taskmodify->editScene->setCurrentIndex(0);
		}
		else
		{
			taskmodify->editScene->setCurrentIndex(taskmodify->editScene->findText(scenename->text()));
		}
		QTableWidgetItem* faultname = listTask->item(curRow,4);
		if (faultname->text()==QString(""))
		{
			taskmodify->editFault->setCurrentIndex(0);
		} 
		else
		{
			taskmodify->editFault->setCurrentIndex(taskmodify->editFault->findText(faultname->text()));
		}
		taskmodify->setWindowTitle(QString::fromLocal8Bit("修改任务"));
		taskmodify->show();	
		connect(taskmodify,SIGNAL(modifyRecord()),this,SLOT(modifyTask()));
	}
}
void CTaskTable::displayUpdate(int stn_id)
{
	select_stn = true;
	stnId = stn_id;
	displayTask();
}
void CTaskTable::displayUpdate(int stn_id,int dev_id)
{
	select_stndev = true;
	stnId = stn_id;
	devId = dev_id;
	displayTask();
}
int CTaskTable::displayTask()
{
	DB_FMT data;
	unsigned char stn_id,dev_id;
	InitDbData(&data,TABLE_TASK);
	if (select_stn)
	{
		data.cond_flag.flag_task.station_id = TRUE;
		data.cond_seg.seg_task.station_id = stnId;
	}
	if (select_stndev)
	{
		data.cond_flag.flag_task.station_id = TRUE;
		data.cond_seg.seg_task.station_id = stnId;
		data.cond_flag.flag_task.dev_id = TRUE;
		data.cond_seg.seg_task.dev_id = devId;
	}
	if (select_grep)
	{
		data.cond_flag.flag_task.task_grp_id = TRUE;
		data.cond_seg.seg_task.task_grp_id = taskgrpid;
	}
	if (select_type)
	{
		data.cond_flag.flag_task.type = TRUE;
		data.cond_seg.seg_task.type = typ;
	}
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	listTask->setRowCount(data.rownum);
	for(int i=0;i<data.rownum;i++)
	{	
		listTask->setItem(i,0,new QTableWidgetItem(QString::number(data_task[i].task_id)));
		listTask->setItem(i,1,new QTableWidgetItem(QString::fromLocal8Bit(data_task[i].name)));
		listTask->setItem(i,2,new QTableWidgetItem(QString::fromLocal8Bit(data_task[i].task_grp_name)));
		listTask->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit(data_task[i].scene_name)));
		listTask->setItem(i,4,new QTableWidgetItem(QString::fromLocal8Bit(data_task[i].fault_name)));
		if (data_task[i].type==1)
		{
			listTask->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit("正常操作")));
		}
		else if (data_task[i].type==2)
		{
			listTask->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit("故障处理")));
		}
		else
		{
			listTask->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit("")));
		}
		listTask->setItem(i,6,new QTableWidgetItem(QString::fromLocal8Bit(data_task[i].userinfo_longname)));
		listTask->setItem(i,7,new QTableWidgetItem(QString::fromLocal8Bit(data_task[i].datetime)));
		listTask->setItem(i,8,new QTableWidgetItem(QString::fromLocal8Bit(data_task[i].remark)));
		stn_id = data_task[i].station_id;
		dev_id = data_task[i].dev_id;
		listTask->setItem(i,9,new QTableWidgetItem(QString::fromLocal8Bit(data_station[stn_id].station_longname )));
		listTask->setItem(i,10,new QTableWidgetItem(QString::fromLocal8Bit(data_dev[stn_id][dev_id].dev_longname)));
	}
	select_status = false;
	select_stndev = false;
	select_status = false;
	select_grep = false;
	select_type = false;
	listTask->clearSelection();
	if (data.rownum != 0)
	{
		listTask->resizeColumnsToContents();
	}	
	return data.rownum;
}
void CTaskTable::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CTaskTable::delTask()
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
		QTableWidgetItem* flag_task_id = listTask->item(curRow,0);
		int t = flag_task_id->text().toInt();
		//先删题目，再删任务 dzy
		InitDbData(&data,TABLE_SUBJECT);
		data.cond_flag.flag_subject.task_id = TRUE;
		data.cond_seg.seg_subject.task_id = t;
		SelectRecs(&data);
		for(int n = 0; n < data.rownum; n++)
		{
			data.cond_flag.flag_subject.task_id = TRUE;
			data.cond_seg.seg_subject.task_id = t;
			DeleteOneRec(&data);
		}	

		InitDbData(&data,TABLE_TASK);		
		data.cond_flag.flag_task.task_id=TRUE;
		data.cond_seg.seg_task.task_id = flag_task_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
	rownums = displayTask();
}
void CTaskTable::addTask()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TASK);

	//设置表中每个字段的flag
	data.flag.flag_task.task_id = TRUE;
	data.flag.flag_task.name =TRUE;
	data.flag.flag_task.task_grp_id =TRUE;
	data.flag.flag_task.task_grp_name=TRUE;
	data.flag.flag_task.type=TRUE;
	data.flag.flag_task.datetime=TRUE;
	data.flag.flag_task.remark=TRUE;
	data.flag.flag_task.dev_id=TRUE;
	data.flag.flag_task.station_id=TRUE;
	data.flag.flag_task.scene_id=TRUE;
	//data.flag.flag_task.scene_name=TRUE;
	data.flag.flag_task.fault_id=TRUE;
	//data.flag.flag_task.fault_name=TRUE;

	//填充表中每个字段的值

	taskadd->seg.task_id = GetInexistenceIdInHdb(TABLE_TASK);
	GetCurTimeStr( taskadd->seg.datetime);
	data.seg.seg_task = taskadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	rownums = displayTask();
}
void CTaskTable::modifyTask()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TASK);
	//设置表中每个字段的flag
	data.flag.flag_task.name=TRUE;
	data.flag.flag_task.datetime=TRUE;
	data.flag.flag_task.task_grp_id=TRUE;
	data.flag.flag_task.task_grp_name=TRUE;
	data.flag.flag_task.type=TRUE;
	data.flag.flag_task.dev_id=TRUE;
	data.flag.flag_task.station_id=TRUE;
	data.flag.flag_task.remark=TRUE;
	data.flag.flag_task.scene_id=TRUE;
	data.flag.flag_task.fault_id=TRUE;

	//填充表中每个字段的值
	GetCurTimeStr(taskmodify->seg.datetime);
	data.seg.seg_task = taskmodify->seg;

	//设置查询条件
	data.cond_flag.flag_task.task_id=TRUE;
	data.cond_seg.seg_task.task_id = curTaskId;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	rownums = displayTask();
}
void CTaskTable::displaytaskselect(int i)
{
	comboBox_2->clear();
	comboBox_2->insertItem(0,QString::fromLocal8Bit("请选择"));
	if (i<1)
	{
		return;
	}	
	else if (i==1)
	{		
		DB_FMT data;
		InitDbData(&data,TABLE_TASK_GRP);
		SelectRecs(&data);
		for(int i=0;i<data.rownum;i++)
		{
			comboBox_2->insertItem(i+1,QString::fromLocal8Bit(data_task_grp[i].name));
		}
	}
	else if (i == 2)
	{
		comboBox_2->insertItem(1,QString::fromLocal8Bit("正常操作"));
		comboBox_2->insertItem(2,QString::fromLocal8Bit("故障处理"));
	}

}

void CTaskTable::selectTask()
{
	if (comboBox->currentIndex()>0&&comboBox_2->currentIndex()>0)
	{
		if (comboBox->currentIndex() == 1)
		{
			select_grep = true;
			select_type = false;
			QString str = comboBox_2->currentText ();
			char c[30];
			strcpy(c,str.toLocal8Bit().data());
			DB_FMT data;
			InitDbData(&data,TABLE_TASK_GRP);
			data.cond_flag.flag_task_grp.name = TRUE;
			strcpy(data.cond_seg.seg_task_grp.name,c);
			SelectRecs(&data);
			taskgrpid = data_task_grp[0].task_grp_id;
		}
		if (comboBox->currentIndex() == 2)
		{
			select_type = true;
			select_grep = false;
			QString str = comboBox_2->currentText ();
			if (str == QString::fromLocal8Bit("正常操作"))
			{
				typ = 1;
			}
			if (str == QString::fromLocal8Bit("故障处理"))
			{
				typ = 2;
			}		
		}
		
		int m = listTask->rowCount() ;
		for(int s = 0;s<m;s++)
		{
			listTask->removeRow(s);
		}
		
		int s = displayTask();

	}
	else
	{
		select_grep = false;
		select_type = false;
		int s = displayTask();
		return;
	}
}


/**************************************************CTaskAddTable*********************************************************/
CTaskAddTable::CTaskAddTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	flag_add=false;
	flag_modify=false;
	displayStation();
	displayTaskGrpName();
	displayScene();
	displayFault();
	setControlAtt();
	selectionModel = tableWidget->selectionModel();
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displaySubject()));
	connect(editStation,SIGNAL(currentIndexChanged(int)),this,SLOT(displayDev(int)));   
	connect(buttonAddSubject,SIGNAL(clicked()),this,SLOT(openAddSubject()));
	connect(buttonModifySubject,SIGNAL(clicked()),this,SLOT(openModifySubject()));
	connect(buttonDelSubject,SIGNAL(clicked()),this,SLOT(delSubject()));
	connect(buttonImportSubject,SIGNAL(clicked()),this,SLOT(importSubject()));
	connect(buttonExportSubject,SIGNAL(clicked()),this,SLOT(exportSubject()));
	connect(buttonMoveUp,SIGNAL(clicked()),this,SLOT(moveUp()));
	connect(buttonMoveDown,SIGNAL(clicked()),this,SLOT(moveDown()));
	connect(buttonAverage,SIGNAL(clicked()),this,SLOT(setScoreAuto()));
}
void CTaskAddTable::setControlAtt()
{
	editType->setCurrentIndex(-1);
	editGrpName->setCurrentIndex(-1);
	editStation->setCurrentIndex(-1);
	editDev->setCurrentIndex(-1);
	editScene->setCurrentIndex(-1);
	editFault->setCurrentIndex(-1);
	tableWidget->setRowCount(0);	
	tableWidget->setAlternatingRowColors(true);
	tableWidget->setColumnWidth(0,200);
	tableWidget->setColumnWidth(1,80);
	tableWidget->setColumnWidth(2,80);
}
void CTaskAddTable::showEvent(QShowEvent *event)
{
	displaySubject();
	event->accept();
}
void CTaskAddTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CTaskAddTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CTaskAddTable::displayStation()
{
	editStation->insertItem(0,QString::fromLocal8Bit("无厂站"));
	for(int i=1;i<=g_stn_num;i++)
	{
		UCHAR	stnid;
		if(-1==GetStnIdByIndex(i, &stnid))
			continue;
		editStation->insertItem(i,QString(data_station[stnid].station_longname),QVariant(data_station[stnid].station_id));	
	}
}
void CTaskAddTable::displayDev(int index)
{
	editDev->clear();
	editDev->insertItem(0,QString::fromLocal8Bit("无设备"));
	UCHAR stnid = editStation->itemData(index).toInt();
	for (int j=1;j<=data_station[stnid].dev_num;j++)
	{
		editDev->insertItem(j,QString::fromLocal8Bit(data_dev[stnid][j].dev_longname),QVariant(data_dev[stnid][j].dev_id));
	}
}
void CTaskAddTable::displayTaskGrpName()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TASK_GRP);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	editGrpName->clear();
	for(int i=0;i<data.rownum;i++)
	{
		editGrpName->insertItem(i,QString::fromLocal8Bit(data_task_grp[i].name));
	}
}
void CTaskAddTable::displayScene()
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
	editScene->insertItem(0,QString::fromLocal8Bit("无"));
	for (int i=1;i<data.rownum;i++)
	{
		editScene->insertItem(i,QString::fromLocal8Bit(data_scene[i].name));
	}
}
void CTaskAddTable::displayFault()
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
	editFault->insertItem(0,QString::fromLocal8Bit("无"));
	for (int i=1;i<data.rownum;i++)
	{
		editFault->insertItem(i,QString::fromLocal8Bit(data_fault[i].name));
	}
}
void CTaskAddTable::apply()
{	
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
	seg.task_grp_id = taskGrpId(editGrpName->currentText().toLocal8Bit().data());
	strcpy(seg.name,editName->text().toLocal8Bit().data());	
	strcpy(seg.task_grp_name,editGrpName->currentText().toLocal8Bit().data());
	seg.type = editType->currentIndex()+1;
	seg.station_id = editStation->itemData(editStation->currentIndex()).toInt();
	seg.dev_id = editDev->itemData(editDev->currentIndex()).toInt();
	strcpy(seg.remark,remarks.toLocal8Bit().data());
	char buf[400];
	memcpy(buf,editRemark->toPlainText().toLocal8Bit().data(),400);
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
int CTaskAddTable::taskGrpId(char* task_grp_name)
{
	DB_FMT data;
	InitDbData(&data,TABLE_TASK_GRP);
	data.cond_flag.flag_task_grp.name = TRUE;
	strcpy(data.cond_seg.seg_task_grp.name,task_grp_name);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	return data_task_grp[0].task_grp_id;
}
int CTaskAddTable::sceneId(char *scene_name)
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
int CTaskAddTable::faultId(char *fault_name)
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
void CTaskAddTable::enter()
{
	apply();
	flag_add=false;
	flag_modify=false;
	accept();
}
void CTaskAddTable::displaySubject()
{
	totle_score=0.00;
	DB_FMT data;
	InitDbData(&data,TABLE_SUBJECT);
	data.cond_flag.flag_subject.task_id = TRUE;
	data.cond_seg.seg_subject.task_id = task_id;
	if (SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}	
	tableWidget->clearContents();
	tableWidget->setRowCount(data.rownum);
	for(int i=0;i<data.rownum;i++)
	{
		ExecSqlQStr(QString("update ots.subject set step=%1+1 where subject_id = %2 and task_id = %3").arg(i).arg(data_subject[i].subject_id).arg(task_id));
		QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit(data_subject[i].name));
		item->setData(Qt::UserRole,data_subject[i].subject_id);
		item->setData(Qt::UserRole+1,data_subject[i].is_keystep);
		item->setData(Qt::UserRole+2,QVariant(data_subject[i].judge_logic));
		item->setData(Qt::UserRole+3,QVariant(data_subject[i].pre_condition));
		item->setData(Qt::UserRole+4,QVariant(data_subject[i].step));
		item->setData(Qt::UserRole+5,QVariant(data_subject[i].datetime));
		tableWidget->setItem(i,0,item);
		tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(data_subject[i].score)));
		tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(data_subject[i].time_limit)));
		tableWidget->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit(data_subject[i].subject_tip)));
		totle_score+=data_subject[i].score;
		if (data_subject[i].is_keystep)
		{
			QTableWidgetItem *itemHeader = new QTableWidgetItem(QIcon(":/pic/images/star.png"),NULL);
			tableWidget->setVerticalHeaderItem(i,itemHeader);
		}	
		else
		{
			QTableWidgetItem *itemHeader = new QTableWidgetItem("    ");
			tableWidget->setVerticalHeaderItem(i,itemHeader);
		}

	}
	labelStepCount->setText(QString::number(data.rownum));
	labelTotleScore->setText(QString::number(totle_score));
}
void CTaskAddTable::openAddSubject()
{
	subjecteditadd = new CSubjectEdit(this);
	subjecteditadd->flag_add = true;
	subjecteditadd->task_id = task_id;
	subjecteditadd->step = tableWidget->rowCount()+1;
	subjecteditadd->editTaskName->setText(editName->text());
	subjecteditadd->editTaskName->setReadOnly(true);
	subjecteditadd->editCondition->setReadOnly(true);
	subjecteditadd->editPreCondition->setReadOnly(true);
	subjecteditadd->groupBoxPreCondition->setChecked(false);
	subjecteditadd->show();
	connect(subjecteditadd,SIGNAL(accepted()),this,SLOT(displaySubject()));
}
void CTaskAddTable::openModifySubject()
{
	if (selectionModel->hasSelection())
	{
		subjecteditmodify = new CSubjectEdit(this);
		subjecteditmodify->flag_modify = true;
		subjecteditmodify->task_id = task_id;
		subjecteditmodify->editTaskName->setText(editName->text());
		subjecteditmodify->editTaskName->setReadOnly(true);
		subjecteditmodify->editCondition->setReadOnly(true);
		subjecteditmodify->editPreCondition->setReadOnly(true);
		QTableWidgetItem* subjectNameItem = tableWidget->item(tableWidget->currentRow(),0);
		subjecteditmodify->editSubjectName->setText(subjectNameItem->text());
		subjecteditmodify->subject_id = subjectNameItem->data(Qt::UserRole).toInt();
		QTableWidgetItem* scoreItem = tableWidget->item(tableWidget->currentRow(),1);
		subjecteditmodify->editScore->setValue(scoreItem->text().toInt());
		QTableWidgetItem* timeItem = tableWidget->item(tableWidget->currentRow(),2);
		subjecteditmodify->editTime->setValue(timeItem->text().toInt());
		QTableWidgetItem* stepTipItem = tableWidget->item(tableWidget->currentRow(),3);
		subjecteditmodify->editStepTip->setText(stepTipItem->text());
		subjecteditmodify->editCondition->setPlainText(subjectNameItem->data(Qt::UserRole+2).toString());
		subjecteditmodify->editPreCondition->setPlainText(subjectNameItem->data(Qt::UserRole+3).toString());
		if (subjectNameItem->data(Qt::UserRole+1).toInt()==0)
		{
			subjecteditmodify->groupBoxPreCondition->setChecked(false);
		}
		else
		{
			subjecteditmodify->groupBoxPreCondition->setChecked(true);
		}
		subjecteditmodify->show();
		connect(subjecteditmodify,SIGNAL(accepted()),this,SLOT(displaySubject()));
	}
}
void CTaskAddTable::delSubject()
{
	if (selectionModel->hasSelection())
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
		QModelIndexList indexes = selectionModel->selectedRows(0);
		for (int i=indexes.size()-1;i>=0;i--)
		{
			int row = indexes.at(i).row();
			QTableWidgetItem* subjectNameItem = tableWidget->item(row,0);
			DB_FMT data;
			InitDbData(&data,TABLE_SUBJECT);
			data.cond_flag.flag_subject.subject_id = TRUE;
			data.cond_seg.seg_subject.subject_id = subjectNameItem->data(Qt::UserRole).toInt();
			if( DeleteOneRec(&data) != TRUE)
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				return;
			}
			tableWidget->removeRow(row);
		}
		displaySubject();
	}
}
void CTaskAddTable::importSubject()
{	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),qApp->applicationDirPath(),tr("Excel Files (*.xls)"));
	if (!fileName.isEmpty())
	{
		DB_FMT data;
		ExcelObjectODBC excel(fileName,"sheet");
		if (excel.Open())
		{
			QProgressDialog *progressDlg=new QProgressDialog(this); 
			QFont font("ZYSong18030",12);  
			progressDlg->setFont(font);  
			progressDlg->setWindowModality(Qt::WindowModal);  
			progressDlg->setMinimumDuration(5);  
			progressDlg->setWindowTitle(tr("please wait"));  
			progressDlg->setLabelText(tr("导入任务题目中,请稍后......      "));  
			progressDlg->setCancelButtonText(tr("取消"));  
			QSqlQueryModel *model = excel.GetExcelSheetModel();
			int step = tableWidget->rowCount();
			progressDlg->setRange(0,model->rowCount());
			QString condition,preCondition,name;
			float score;
			short is_keystep;
			int mode;
			for (int i=0;i<=model->rowCount();i++)
			{
				QString code;
				if (i == model->rowCount())	code = "STEP";
				else code = model->record(i).value(QString::fromLocal8Bit("CODE")).toString();
				if (code == "STEP" && !condition.isEmpty())
				{
					InitDbData(&data,TABLE_SUBJECT);

					//设置表中每个字段的flag
					data.flag.flag_subject.subject_id = TRUE;
					data.flag.flag_subject.name = TRUE;
					data.flag.flag_subject.task_id = TRUE;
					data.flag.flag_subject.step = TRUE;
					data.flag.flag_subject.type = TRUE;
					data.flag.flag_subject.datetime = TRUE;
					data.flag.flag_subject.judge_logic = TRUE;
					data.flag.flag_subject.interlock_name = TRUE;
					data.flag.flag_subject.pre_condition = TRUE;
					data.flag.flag_subject.subject_tip = TRUE;
					data.flag.flag_subject.time_limit = TRUE;
					data.flag.flag_subject.score = TRUE;
					data.flag.flag_subject.is_keystep = TRUE;
					//填充表中每个字段的值
					data.seg.seg_subject.subject_id = GetInexistenceIdInHdb(TABLE_SUBJECT);
					strcpy(data.seg.seg_subject.name,name.toLocal8Bit().data());
					data.seg.seg_subject.task_id = task_id;
					data.seg.seg_subject.step = ++step;
					data.seg.seg_subject.type = 0;
					GetCurTimeStr(data.seg.seg_subject.datetime);
					strcpy(data.seg.seg_subject.judge_logic,condition.toLocal8Bit().data());
					strcpy(data.seg.seg_subject.interlock_name,"");
					strcpy(data.seg.seg_subject.pre_condition,preCondition.toLocal8Bit().data());
					strcpy(data.seg.seg_subject.subject_tip,name.toLocal8Bit().data());
					data.seg.seg_subject.time_limit = 0;
					data.seg.seg_subject.score = score;
					data.seg.seg_subject.is_keystep = is_keystep;
					progressDlg->setValue(i); 
					if(progressDlg->wasCanceled())  
						break;
					//调用接口函数，参数为DB_FMT结构体
					if( InsertOneRec(&data) != TRUE)
					{		
						HdbLogWrite(data.sql_str,data.sql_error);
						continue;
					}
					condition.clear();
				}
				if (i == model->rowCount()) break;
				if (code == "STEP")
				{
					mode = 0;
					name = model->record(i).value(QString::fromLocal8Bit("NAME")).toString().simplified().left(25);
					score = model->record(i).value(QString::fromLocal8Bit("SCORE")).toDouble();
					is_keystep = model->record(i).value(QString::fromLocal8Bit("IS_KEYSTEP")).toDouble();
					condition = getCondition(model->record(i).value(QString::fromLocal8Bit("TYPE")).toString(),
											model->record(i).value(QString::fromLocal8Bit("LOGICALNAME")).toString(),
											model->record(i).value(QString::fromLocal8Bit("TYPE_ID")).toDouble(),
											model->record(i).value(QString::fromLocal8Bit("STATUS")).toDouble(),
											model->record(i).value(QString::fromLocal8Bit("FVALUE")).toDouble());
				}
				if (code == "PRE")
				{
					mode = 1;
					preCondition = getCondition(model->record(i).value(QString::fromLocal8Bit("TYPE")).toString(),
												model->record(i).value(QString::fromLocal8Bit("LOGICALNAME")).toString(),
												model->record(i).value(QString::fromLocal8Bit("TYPE_ID")).toDouble(),
												model->record(i).value(QString::fromLocal8Bit("STATUS")).toDouble(),
												model->record(i).value(QString::fromLocal8Bit("FVALUE")).toDouble());
				}
				if (code == "&" || code == "|")
				{
					if (mode==0)
					{
						QString str = getCondition(model->record(i).value(QString::fromLocal8Bit("TYPE")).toString(),
												model->record(i).value(QString::fromLocal8Bit("LOGICALNAME")).toString(),
												model->record(i).value(QString::fromLocal8Bit("TYPE_ID")).toDouble(),
												model->record(i).value(QString::fromLocal8Bit("STATUS")).toDouble(),
												model->record(i).value(QString::fromLocal8Bit("FVALUE")).toDouble());
						condition.append(QString(" %1 %2").arg(code).arg(str));
					}
					else if (mode==1)
					{
						QString str = getCondition(model->record(i).value(QString::fromLocal8Bit("TYPE")).toString(),
												model->record(i).value(QString::fromLocal8Bit("LOGICALNAME")).toString(),
												model->record(i).value(QString::fromLocal8Bit("TYPE_ID")).toDouble(),
												model->record(i).value(QString::fromLocal8Bit("STATUS")).toDouble(),
												model->record(i).value(QString::fromLocal8Bit("FVALUE")).toDouble());
						preCondition.append(QString(" %1 %2").arg(code).arg(str));
					}
				}
			}
			displaySubject();																						
		}
	}
}
void CTaskAddTable::exportSubject()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.xls)"));
	if (!fileName.isEmpty())
	{
		DB_FMT data;
		InitDbData(&data,TABLE_SUBJECT);
		data.cond_flag.flag_subject.task_id = TRUE;
		data.cond_seg.seg_subject.task_id = task_id;
		if( SelectRecs(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
		ExcelObjectODBC excel(fileName,"sheet");
		excel.AddHeader(0, "CODE", "char(5)");
		excel.AddHeader(1, "NAME", "char(100)");
		excel.AddHeader(2, "TYPE", "char(10)");
		excel.AddHeader(3, "LOGICALNAME","char(38)");
		excel.AddHeader(4, "TYPE_ID", "int");
		excel.AddHeader(5, "STATUS", "int");
		excel.AddHeader(6, "FVALUE", "int");
		excel.AddHeader(7, "SCORE", "int");
		excel.AddHeader(8, "IS_KEYSTEP", "int");
		if (excel.Open())
		{
			QVariantList rowValues;
			for (int i=0;i<data.rownum;i++)
			{
				int type_id,status,score;
				short is_keystep;
				float fvalue;
				QString code,name,type,logicalname;
				QString conditions = QString::fromLocal8Bit(data_subject[i].judge_logic);
				QString preConditions = QString::fromLocal8Bit(data_subject[i].pre_condition);
				QStringList conditionList,preConditionList;
				if (conditions.isEmpty())
				{
					continue;
				}
				splitConditionsToList(conditions,conditionList);
				splitConditionsToList(preConditions,preConditionList);
				code = QString("STEP");
				name = QString::fromLocal8Bit(data_subject[i].name);
				score = data_subject[i].score;
				is_keystep = data_subject[i].is_keystep;
				for (int j=0;j<conditionList.count();j++) 
				{
					if (conditionList.at(j).contains('&') || conditionList.at(j).contains('|')
						|| conditionList.at(j).contains('(') || conditionList.at(j).contains(')'))
					{
						code = conditionList.at(j);
						name.clear();
						score = 0;
						is_keystep = 0;
						continue;
					}
					rowValues.clear();
					splitCondition(conditionList.at(j),type,logicalname,&type_id,&status,&fvalue);
					rowValues.append(QVariant(code));
					rowValues.append(QVariant(name));
					rowValues.append(QVariant(type));
					rowValues.append(QVariant(logicalname));
					rowValues.append(QVariant(type_id));
					rowValues.append(QVariant(status));
					rowValues.append(QVariant(fvalue));
					rowValues.append(QVariant(score));
					rowValues.append(QVariant(is_keystep));
					excel.WriteRow(rowValues);
				}
				if (!preConditions.isEmpty())
				{
					code = QString("PRE");
					name.clear();
					score = 0;
					is_keystep = 0;
				}
				for (int j=0;j<preConditionList.count();j++) 
				{
					if (preConditionList.at(j).contains('&') || preConditionList.at(j).contains('|')
						|| preConditionList.at(j).contains('(') || preConditionList.at(j).contains(')'))
					{
						code = preConditionList.at(j);
						continue;
					}
					rowValues.clear();
					splitCondition(preConditionList.at(j),type,logicalname,&type_id,&status,&fvalue);
					rowValues.append(QVariant(code));
					rowValues.append(QVariant(name));
					rowValues.append(QVariant(type));
					rowValues.append(QVariant(logicalname));
					rowValues.append(QVariant(type_id));
					rowValues.append(QVariant(status));
					rowValues.append(QVariant(fvalue));
					rowValues.append(QVariant(score));
					rowValues.append(QVariant(is_keystep));
					excel.WriteRow(rowValues);
				}
			}
		}
		QMessageBox().information(this,QString::fromLocal8Bit("信息!"),QString::fromLocal8Bit("导出任务题目成功!"),QMessageBox::Ok);	
	}
}
void CTaskAddTable::splitConditionsToList(const QString &conditons,QStringList &conditonList)
{
	QString	tmpStr;
	for (int i=0;i<conditons.size();i++)
	{
		if (conditons.at(i) != '&' && conditons.at(i) != '|' 
			&& conditons.at(i) != '(' && conditons.at(i) != ')')
		{
			tmpStr.append(conditons.at(i));
		}
		else
		{
			tmpStr = tmpStr.trimmed();
			if(tmpStr.isEmpty())
				conditonList << conditons.at(i);
			else
				conditonList << tmpStr << conditons.at(i);
			tmpStr.clear();
		}
	}
	tmpStr = tmpStr.trimmed();
	if (!tmpStr.isEmpty())
		conditonList << tmpStr;
}
void CTaskAddTable::splitCondition(QString conditon,QString &type,QString &logicalname,int *type_id,int *status,float *fvalue)
{
	conditon = conditon.trimmed();
	QStringList strList = conditon.split(" ",QString::SkipEmptyParts);
	if (strList.at(0) == "ALARM:")
	{
		type = QString("ALARM");
		logicalname = strList.at(1);
		if (logicalname.contains(".ANA.")) *type_id = DPS_ANA_ALM;
		else if (logicalname.contains(".IMP.")) *type_id = DPS_IMP_ALM;
		else *type_id = DPS_IND_ALM;
		
		if (strList.contains("="))
		{
			switch(*type_id)
			{
				case	DPS_ANA_ALM:
				case	DPS_IMP_ALM:
					*status = 0;
					*fvalue = strList.at(3).toFloat();
				default:
					*status = strList.at(3).toInt();
					*fvalue = 0;
			}
		}
		else if (strList.contains(">"))
		{
			*status = 0;
			*fvalue = strList.at(3).toFloat();
		}
		else if (strList.contains("<"))
		{
			*status = 0;
			*fvalue = strList.at(3).toFloat();
		}
		else
		{
			*status = 0;
			*fvalue = 0;
		}
	}
	else if (strList.at(0) == "CMD:")
	{
		type = QString("CMD");
		logicalname = strList.at(1);
		*type_id = strList.at(2).split(":",QString::SkipEmptyParts).at(1).toInt();
		*status = strList.at(3).split(":",QString::SkipEmptyParts).at(1).toInt();
		*fvalue = 0;
	}
	else if (strList.at(0) == "OPR:")
	{
		type = QString("OPR");
		logicalname = strList.at(1);
		*type_id = 0;
		*status = 0;
		*fvalue = 0;
	}
	else if (strList.at(0) == "CHECK:")
	{
		type = QString("CHECK");
		logicalname = strList.at(1);
		if (strList.at(2) == "NORMAL")	*status = 0;
		else if (strList.at(2) == "ANORMAL")	*status = 1;
		*type_id = 0;
		*fvalue = 0;
	}
}
QString CTaskAddTable::getCondition(const QString &type,const QString &logicalname,int type_id,int status,float fvalue) const
{
	QString conditionStr;
	if (type == "ALARM")
	{
		switch(type_id)
		{
			case	DPS_ANA_ALM:
			case	DPS_IMP_ALM:
				conditionStr = QString("%1: %2 = %3")
					.arg("ALARM")
					.arg(logicalname)
					.arg(fvalue);
				break;
			default:
				conditionStr = QString("%1: %2 = %3")
					.arg("ALARM")
					.arg(logicalname)
					.arg(status);
				break;
		}
	}
	else if (type == "CMD")
	{
		COMMAND_STATUS	cmd_status;	
		memcpy(&(cmd_status),&(status),2);
		conditionStr = QString("%1: %2 TYPE_ID:%3 CMD_OPT:%4")
			.arg("CMD")
			.arg(logicalname)
			.arg(type_id)
			.arg(cmd_status.opr);
	}
	else if (type == "OPR")
	{
		conditionStr = QString("%1: %2")
			.arg("OPR")
			.arg(logicalname);
	}
	else if (type == "CHECK")
	{
		conditionStr = QString("%1: %2 ANORMAL")
			.arg("CHECK")
			.arg(logicalname);
	}
	return conditionStr;
}
void CTaskAddTable::moveUp()
{
	if (selectionModel->hasSelection())
	{
		QModelIndexList indexes = selectionModel->selectedRows(0);
		int row = indexes.at(0).row();
		if (row == 0)
			return;
		QTableWidgetItem* subjectNameItem = tableWidget->item(row,0);
		int subject_id = subjectNameItem->data(Qt::UserRole).toInt();
		int step = subjectNameItem->data(Qt::UserRole+4).toInt();
		char sql_str[100];
		sprintf(sql_str,"update ots.subject set step=%d where step=%d and task_id=%d",step,step-1,task_id);
		ExecSql(sql_str);
		sprintf(sql_str,"update ots.subject set step=%d where subject_id=%d and task_id=%d",step-1,subject_id,task_id);
		ExecSql(sql_str);
		displaySubject();
		tableWidget->selectRow(row-1);
	}
	
}
void CTaskAddTable::moveDown()
{
	if (selectionModel->hasSelection())
	{
		QModelIndexList indexes = selectionModel->selectedRows(0);
		int row = indexes.at(0).row();
		if (row == tableWidget->rowCount()-1)
			return;
		QTableWidgetItem* subjectNameItem = tableWidget->item(row,0);
		int subject_id = subjectNameItem->data(Qt::UserRole).toInt();
		int step = subjectNameItem->data(Qt::UserRole+4).toInt();
		char sql_str[100];
		sprintf(sql_str,"update ots.subject set step=%d where step=%d and task_id=%d",step,step+1,task_id);
		ExecSql(sql_str);
		sprintf(sql_str,"update ots.subject set step=%d where subject_id=%d and task_id=%d",step+1,subject_id,task_id);
		ExecSql(sql_str);
		displaySubject();
		tableWidget->selectRow(row+1);
	}
}
void CTaskAddTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QPoint p(20,20);
	QTableWidgetItem* pTableWidgetItem=tableWidget->itemAt(tableWidget->mapFromGlobal(QCursor::pos()-p));
 	if(pTableWidgetItem != NULL)
	{
		QTableWidgetItem* subjectNameItem = tableWidget->item(tableWidget->currentRow(),0);
		QTableWidgetItem* scoreItem = tableWidget->item(tableWidget->currentRow(),1);
		QString text = QString::fromLocal8Bit("题目I D :%1").arg(subjectNameItem->data(Qt::UserRole).toInt())
			+QString::fromLocal8Bit("<br>题目名称:%1").arg(subjectNameItem->text())
			+QString::fromLocal8Bit("<br>题目分数:%1").arg(scoreItem->text())
			+QString::fromLocal8Bit("<br>当前步骤:%1").arg(subjectNameItem->data(Qt::UserRole+4).toInt())
			+QString::fromLocal8Bit("<br>创建时间:%1").arg(subjectNameItem->data(Qt::UserRole+5).toString())
			+QString::fromLocal8Bit("<br>题目逻辑:%1").arg(subjectNameItem->data(Qt::UserRole+2).toString());
		QToolTip::showText(QCursor::pos(), text, this);
	}
}
void CTaskAddTable::setScoreAuto()
{
	int count = tableWidget->rowCount();
	if (count<=0)
		return;
	int stepScore = 100/count;
	int rStepScore = 100%count;
	char sql_str[100];
	sprintf(sql_str,"update ots.subject set score=%d where task_id=%d",stepScore,task_id);
	ExecSql(sql_str);
	for (int i=1;i<=rStepScore;i++)
	{
		sprintf(sql_str,"update ots.subject set score=%d where task_id=%d and step=%d",stepScore+1,task_id,i);
		ExecSql(sql_str);
	}
	displaySubject();
}
/*void CTaskAddTable::displayTips()
{
	QString str="";
	for (int i = 0 ;i <tableWidget->rowCount();i++)
	{
		str = str+QString::fromLocal8Bit("第")+QString::number(i+1)+QString::fromLocal8Bit("步:");
		str = str + sublistinfo.at(i).steptip+"\n";
	}
	editRemark->setText(str);
	//保存所有里写数据库

}*/