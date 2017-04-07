#include "traininfo.h"

CTrainInfoTable::CTrainInfoTable(QWidget* parent)
:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	select_status = false;
	actionAddTrainInfo = new QAction(QString::fromLocal8Bit("添加培训信息"), this);
	actionModifyTrainInfo = new QAction(QString::fromLocal8Bit("修改培训信息"), this);
	actionDelTrainInfo = new QAction(QString::fromLocal8Bit("删除培训信息"), this);
	refresh = new QTimer(this);
	setControlAtt();
	displayTrainInfo();
	UpdateTrainInfoModel();
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displayTrainInfo()));
	connect(listTrainInfo,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(buttonTrainInfoDel,SIGNAL(clicked()),this,SLOT(delTrainInfo()));
	connect(buttonTrainInfoAdd,SIGNAL(clicked()),this,SLOT(openTrainInfoAdd()));
	connect(buttonTrainInfoModify,SIGNAL(clicked()),this,SLOT(openTrainInfoModify()));
	connect(actionAddTrainInfo,SIGNAL(triggered()),this,SLOT(openTrainInfoAdd()));
	connect(actionModifyTrainInfo,SIGNAL(triggered()),this,SLOT(openTrainInfoModify()));
	connect(actionDelTrainInfo,SIGNAL(triggered()),this,SLOT(delTrainInfo()));
	connect(buttonExport,SIGNAL(clicked()),this,SLOT(exportExcel()));
	//connect(refresh, SIGNAL(timeout()), this, SLOT(update()));
	//refresh->start(5000);
}
void CTrainInfoTable::setControlAtt()
{
	listTrainInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	listTrainInfo->setSelectionMode(QAbstractItemView::SingleSelection); 
	listTrainInfo->setEditTriggers(QTableWidget::NoEditTriggers);
	listTrainInfo->setSortingEnabled(true);
	listTrainInfo->hideColumn(0);
	listTrainInfo->hideColumn(2);
	listTrainInfo->hideColumn(5);
	listTrainInfo->hideColumn(7);
	listTrainInfo->hideColumn(9);
}
void CTrainInfoTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CTrainInfoTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CTrainInfoTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listTrainInfo->itemAt(listTrainInfo->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddTrainInfo);
		popMenu->addAction(actionModifyTrainInfo);
		popMenu->addAction(actionDelTrainInfo);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单

void CTrainInfoTable::exportExcel()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.xls)"));
	if (fileName.isEmpty())
		return;
	ExportExcelObject obj;
	obj.setOutputFilePath(fileName);
	obj.setOutputSheetTitle("sheet");
	obj.setTableView(listTrainInfo);
	obj.addField(1, "用户名", "char(10)");
	obj.addField(3, "主机名", "char(10)");
	obj.addField(7, "任务名称", "char(50)");
	obj.addField(9, "工况名称", "char(50)");
	obj.addField(11, "故障名称", "char(50)");
	obj.addField(12, "登陆状态", "char(6)");
	obj.addField(13, "运行状态", "char(6)");
	obj.addField(14, "仿真速度", "int");
	obj.addField(15, "默认画面", "char(20)");
	obj.addField(16, "任务状态", "char(10)");
	obj.addField(17, "工况状态", "char(10)");
	obj.addField(18, "故障状态", "char(10)");
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("正在导出......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	progressDlg->setRange(0,listTrainInfo->model()->rowCount());  
	connect(&obj, SIGNAL(exportedRowCount(int)),progressDlg,SLOT(setValue(int)));
	connect(progressDlg, SIGNAL(canceled()),&obj,SLOT(cancel()));
	int retVal = obj.export2Excel();
	if( retVal > 0)
	{
		progressDlg->cancel();
		QMessageBox::information(this, QString::fromLocal8Bit("完成"),
			QString(QString::fromLocal8Bit("共导出%1条记录")).arg(retVal)
			);
	}
}

void CTrainInfoTable::openTrainInfoAdd()
{
	traininfoadd = new CTrainInfoAddTable(this);
	connect(traininfoadd,SIGNAL(insertRecord()),this,SLOT(addTrainInfo()));
	traininfoadd->editUserName->setEnabled(false);
	traininfoadd->editUserName->setEnabled(false);
	traininfoadd->editSendTaskId->setEnabled(false);
	traininfoadd->editTaskId->setEnabled(false);
	traininfoadd->editSceneId->setEnabled(false);
	traininfoadd->editFaultId->setEnabled(false);
	traininfoadd->editLoginStatus->setEnabled(false);
	traininfoadd->editStatus->setEnabled(false);
	traininfoadd->editSpeed->setEnabled(false);
	traininfoadd->editDbinFile->setEnabled(false);
	traininfoadd->editTaskStatus->setEnabled(false);
	traininfoadd->editSceneStatus->setEnabled(false);
	traininfoadd->editFaultStatus->setEnabled(false);
	traininfoadd->flag_add=true;
	traininfoadd->buttonApply->setVisible(false);
	traininfoadd->show();
}
void CTrainInfoTable::openTrainInfoModify()
{
	int host_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的任务!"),QMessageBox::Ok);
	}
	else
	{
		traininfomodify = new CTrainInfoAddTable(this);
		connect(traininfomodify,SIGNAL(modifyRecord()),this,SLOT(modifyTrainInfo()));
		traininfomodify->flag_modify=true;
		QTableWidgetItem* item_id = listTrainInfo->item(curRow,2);
		host_id = item_id->text().toInt();
		curHostId=host_id;
		QTableWidgetItem* user_name = listTrainInfo->item(curRow,1);
		traininfomodify->editUserName->setText(user_name->text());
		QTableWidgetItem* host_name = listTrainInfo->item(curRow,3);
		traininfomodify->editHostName->setText(host_name->text());
		QTableWidgetItem* send_task_id = listTrainInfo->item(curRow,4);
		if (send_task_id->text().toInt()!=CONST_NEG)
		{
			traininfomodify->editSendTaskId->setText(send_task_id->text());
		}
		else
		{
			traininfomodify->editSendTaskId->setText(QString(""));
		}
		QTableWidgetItem* task_id = listTrainInfo->item(curRow,5);
		if (task_id->text().toInt()!=CONST_NEG)
		{
			traininfomodify->editTaskId->setText(task_id->text());
		}
		else
		{
			traininfomodify->editTaskId->setText(QString(""));
		}
		QTableWidgetItem* scene_id = listTrainInfo->item(curRow,7);
		if (scene_id->text().toInt()!=CONST_NEG)
		{
			traininfomodify->editSceneId->setText(scene_id->text());
		}	
		else
		{
			traininfomodify->editSceneId->setText(QString(""));
		}
		QTableWidgetItem* fault_id = listTrainInfo->item(curRow,9);
		if (fault_id->text().toInt()!=CONST_NEG)
		{
			traininfomodify->editFaultId->setText(fault_id->text());
		}
		else
		{
			traininfomodify->editFaultId->setText(QString(""));
		}
		QTableWidgetItem* login_status = listTrainInfo->item(curRow,11);
		if (login_status->text() == QString::fromLocal8Bit("未登录"))
		{
			traininfomodify->editLoginStatus->setCurrentIndex(0);
		}
		else if (login_status->text() == QString::fromLocal8Bit("已登录"))
		{
			traininfomodify->editLoginStatus->setCurrentIndex(1);
		}
		QTableWidgetItem* status = listTrainInfo->item(curRow,12);
		if (status->text() == QString::fromLocal8Bit("停止"))
		{
			traininfomodify->editStatus->setCurrentIndex(0);
		}
		else if (status->text() == QString::fromLocal8Bit("运行"))
		{
			traininfomodify->editStatus->setCurrentIndex(1);
		}
		else if (status->text() == QString::fromLocal8Bit("暂停"))
		{
			traininfomodify->editStatus->setCurrentIndex(2);
		}
		QTableWidgetItem* speed = listTrainInfo->item(curRow,13);
		traininfomodify->editSpeed->setText(speed->text());
		QTableWidgetItem* dbin_file = listTrainInfo->item(curRow,14);
		traininfomodify->editDbinFile->setText(dbin_file->text());
		QTableWidgetItem* task_status = listTrainInfo->item(curRow,15);
		if (task_status->text() == QString::fromLocal8Bit("无新任务"))
		{
			traininfomodify->editTaskStatus->setCurrentIndex(0);
		}
		else if (task_status->text() == QString::fromLocal8Bit("新的任务"))
		{
			traininfomodify->editTaskStatus->setCurrentIndex(1);
		}
		else if (task_status->text() == QString::fromLocal8Bit("任务已处理"))
		{
			traininfomodify->editTaskStatus->setCurrentIndex(2);
		}
		else if (task_status->text() == QString::fromLocal8Bit("任务结束"))
		{
			traininfomodify->editTaskStatus->setCurrentIndex(3);
		}
		QTableWidgetItem* sence_status = listTrainInfo->item(curRow,16);
		if (sence_status->text() == QString::fromLocal8Bit("无新工况"))
		{
			traininfomodify->editSceneStatus->setCurrentIndex(0);
		}
		else if (sence_status->text() == QString::fromLocal8Bit("新的工况"))
		{
			traininfomodify->editSceneStatus->setCurrentIndex(1);
		}
		else if (sence_status->text() == QString::fromLocal8Bit("工况已处理"))
		{
			traininfomodify->editSceneStatus->setCurrentIndex(2);
		}
		QTableWidgetItem* fault_status = listTrainInfo->item(curRow,17);
		if (fault_status->text() == QString::fromLocal8Bit("无新故障"))
		{
			traininfomodify->editFaultStatus->setCurrentIndex(0);
		}
		else if (fault_status->text() == QString::fromLocal8Bit("新的故障"))
		{
			traininfomodify->editFaultStatus->setCurrentIndex(1);
		}
		else if (fault_status->text() == QString::fromLocal8Bit("故障已处理"))
		{
			traininfomodify->editFaultStatus->setCurrentIndex(2);
		}
		traininfomodify->setWindowTitle(QString::fromLocal8Bit("修改培训信息"));
		traininfomodify->editHostName->setEnabled(false);
		traininfomodify->show();
	}
}
int CTrainInfoTable::displayTrainInfo()
{
	DB_FMT data;
	unsigned char stn_id,dev_id;
	InitDbData(&data,TABLE_TRAIN_INFO);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	listTrainInfo->setRowCount(data.rownum);
	for(int i=0;i<data.rownum;i++)
	{	
		listTrainInfo->setItem(i,0,new QTableWidgetItem(QString::number(data_train_info[i].user_id)));
		listTrainInfo->setItem(i,1,new QTableWidgetItem(QString::fromLocal8Bit(data_train_info[i].userinfo_longname)));
		listTrainInfo->setItem(i,2,new QTableWidgetItem(QString::number(data_train_info[i].host_id)));
		listTrainInfo->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit(data_train_info[i].host_name)));
		if (data_train_info[i].send_task_id!=CONST_NEG)
		{
			listTrainInfo->setItem(i,4,new QTableWidgetItem(QString::number(data_train_info[i].send_task_id)));
		}
		else
		{
			listTrainInfo->setItem(i,4,new QTableWidgetItem(QString::fromLocal8Bit("")));
		}
		listTrainInfo->setItem(i,5,new QTableWidgetItem(QString::number(data_train_info[i].task_id)));
		listTrainInfo->setItem(i,6,new QTableWidgetItem(QString::fromLocal8Bit(data_train_info[i].task_name)));
		listTrainInfo->setItem(i,7,new QTableWidgetItem(QString::number(data_train_info[i].scene_id)));
		listTrainInfo->setItem(i,8,new QTableWidgetItem(QString::fromLocal8Bit(data_train_info[i].scene_name)));
		listTrainInfo->setItem(i,9,new QTableWidgetItem(QString::number(data_train_info[i].fault_id)));
		listTrainInfo->setItem(i,10,new QTableWidgetItem(QString::fromLocal8Bit(data_train_info[i].fault_name)));
		if (data_train_info[i].login_status==0)
		{
			listTrainInfo->setItem(i,11,new QTableWidgetItem(QString::fromLocal8Bit("未登录")));
		}
		else if (data_train_info[i].login_status==1)
		{
			listTrainInfo->setItem(i,11,new QTableWidgetItem(QString::fromLocal8Bit("已登录")));
		}
		if (data_train_info[i].status==0)
		{
			listTrainInfo->setItem(i,12,new QTableWidgetItem(QString::fromLocal8Bit("停止")));
		}
		else if (data_train_info[i].status==1)
		{
			listTrainInfo->setItem(i,12,new QTableWidgetItem(QString::fromLocal8Bit("运行")));
		}
		else if (data_train_info[i].status==2)
		{
			listTrainInfo->setItem(i,12,new QTableWidgetItem(QString::fromLocal8Bit("暂停")));
		}
		listTrainInfo->setItem(i,13,new QTableWidgetItem(QString::number(data_train_info[i].speed)));
		listTrainInfo->setItem(i,14,new QTableWidgetItem(QString::fromLocal8Bit(data_train_info[i].dbin_file)));
		if (data_train_info[i].task_status==0)
		{
			listTrainInfo->setItem(i,15,new QTableWidgetItem(QString::fromLocal8Bit("无新任务")));
		}
		else if (data_train_info[i].task_status==1)
		{
			listTrainInfo->setItem(i,15,new QTableWidgetItem(QString::fromLocal8Bit("新的任务")));
		}
		else if (data_train_info[i].task_status==2)
		{
			listTrainInfo->setItem(i,15,new QTableWidgetItem(QString::fromLocal8Bit("任务已处理")));
		}
		else if (data_train_info[i].task_status==0)
		{
			listTrainInfo->setItem(i,15,new QTableWidgetItem(QString::fromLocal8Bit("任务结束")));
		}
		if (data_train_info[i].scene_status==0)
		{
			listTrainInfo->setItem(i,16,new QTableWidgetItem(QString::fromLocal8Bit("无新工况")));
		}
		else if (data_train_info[i].scene_status==1)
		{
			listTrainInfo->setItem(i,16,new QTableWidgetItem(QString::fromLocal8Bit("新的工况")));
		}
		else if (data_train_info[i].scene_status==2)
		{
			listTrainInfo->setItem(i,16,new QTableWidgetItem(QString::fromLocal8Bit("工况已处理")));
		}
		if (data_train_info[i].fault_status==0)
		{
			listTrainInfo->setItem(i,17,new QTableWidgetItem(QString::fromLocal8Bit("无新故障")));
		}
		else if (data_train_info[i].fault_status==1)
		{
			listTrainInfo->setItem(i,17,new QTableWidgetItem(QString::fromLocal8Bit("新的故障")));
		}
		else if (data_train_info[i].fault_status==2)
		{
			listTrainInfo->setItem(i,17,new QTableWidgetItem(QString::fromLocal8Bit("故障已处理")));
		}
	}
	select_status = false;
	listTrainInfo->clearSelection();
	if (data.rownum != 0)
	{
		listTrainInfo->resizeColumnsToContents();
	}
	return data.rownum;
}
void CTrainInfoTable::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CTrainInfoTable::delTrainInfo()
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
		InitDbData(&data,TABLE_TRAIN_INFO);
		QTableWidgetItem* flag_host_id = listTrainInfo->item(curRow,2);
		data.cond_flag.flag_train_info.host_id=TRUE;
		data.cond_seg.seg_train_info.host_id = flag_host_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
	rownums = displayTrainInfo();
}
void CTrainInfoTable::addTrainInfo()
{
	DB_FMT data;
	int max_id;
	InitDbData(&data,TABLE_TRAIN_INFO);

	//设置表中每个字段的flag
	data.flag.flag_train_info.host_id = TRUE;


	//填充表中每个字段的值

	data.seg.seg_train_info = traininfoadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	UpdateTrainInfoModel();
	rownums = displayTrainInfo();
}
void CTrainInfoTable::modifyTrainInfo()
{
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);

	//设置表中每个字段的flag
	data.flag.flag_train_info.user_id = TRUE;
	data.flag.flag_train_info.host_id = TRUE;
	data.flag.flag_train_info.send_task_id = TRUE;
	data.flag.flag_train_info.task_id = TRUE;
	data.flag.flag_train_info.scene_id = TRUE;
	data.flag.flag_train_info.fault_id = TRUE;
	data.flag.flag_train_info.login_status = TRUE;
	data.flag.flag_train_info.status = TRUE;
	data.flag.flag_train_info.speed = TRUE;
	data.flag.flag_train_info.dbin_file = TRUE;
	data.flag.flag_train_info.task_status = TRUE;
	data.flag.flag_train_info.scene_status = TRUE;
	data.flag.flag_train_info.fault_status = TRUE;


	//填充表中每个字段的值
	data.seg.seg_train_info = traininfomodify->seg;

	//设置查询条件
	data.cond_flag.flag_train_info.host_id=TRUE;
	data.cond_seg.seg_train_info.host_id = curHostId;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	UpdateTrainInfoModel();
	rownums = displayTrainInfo();
}

/*************************************************CTrainInfoAddTable*****************************************************/
CTrainInfoAddTable::CTrainInfoAddTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	flag_add=false;
	flag_modify=false;
	setControlAtt();
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
}
void CTrainInfoAddTable::setControlAtt()
{
	editLoginStatus->setCurrentIndex(-1);
	editStatus->setCurrentIndex(-1);
	editTaskStatus->setCurrentIndex(-1);
	editSceneStatus->setCurrentIndex(-1);
	editFaultStatus->setCurrentIndex(-1);
}
void CTrainInfoAddTable::apply()
{		
	if (editUserName->text()==QString(""))
	{
		seg.user_id = CONST_NEG;
	} 
	else
	{
		seg.user_id = userId(editUserName->text().toLocal8Bit().data());
	}
	seg.host_id = hostId(editHostName->text().toLocal8Bit().data());
	if (editSendTaskId->text()==QString(""))
	{
		seg.send_task_id = CONST_NEG;
	}
	else
	{
		seg.send_task_id = editSendTaskId->text().toInt();
	}
	if (editTaskId->text()==QString(""))
	{
		seg.task_id = CONST_NEG;
	} 
	else
	{
		seg.task_id = editTaskId->text().toInt();
	}
	if (editSceneId->text()==QString(""))
	{
		seg.scene_id = CONST_NEG;
	} 
	else
	{
		seg.scene_id = editSceneId->text().toInt();
	}
	if (editFaultId->text()==QString(""))
	{
		seg.fault_id = CONST_NEG;
	} 
	else
	{
		seg.fault_id = editFaultId->text().toInt();
	}
	seg.login_status = editLoginStatus->currentIndex();
	seg.status = editStatus->currentIndex();
	seg.speed = editSpeed->text().toInt();
	strcpy(seg.dbin_file,editDbinFile->text().toLocal8Bit().data());
	seg.task_status = editTaskStatus->currentIndex();
	seg.scene_status = editSceneStatus->currentIndex();
	seg.fault_status = editFaultStatus->currentIndex();
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}
void CTrainInfoAddTable::enter()
{
	apply();
	editUserName->clear();
	editHostName->clear();
	editSendTaskId->clear();
	editTaskId->clear();
	editSceneId->clear();
	editFaultId->clear();
	editLoginStatus->setCurrentIndex(-1);
	editStatus->setCurrentIndex(-1);
	editSpeed->clear();
	editDbinFile->clear();
	editTaskStatus->setCurrentIndex(-1);
	editSceneStatus->setCurrentIndex(-1);
	editFaultStatus->setCurrentIndex(-1);
	this->close();
	flag_add=false;
	flag_modify=false;
}
int CTrainInfoAddTable::userId(char* user_name)
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.cond_flag.flag_userinfo.longname = TRUE;
	strcpy(data.cond_seg.seg_userinfo.longname,user_name);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	return data_userinfo[0].user_id;
}
int CTrainInfoAddTable::hostId(char* host_name)
{
	DB_FMT data;
	InitDbData(&data,TABLE_HOST);
	data.cond_flag.flag_host.name = TRUE;
	strcpy(data.cond_seg.seg_userinfo.name,host_name);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	return data_host[0].host_id;
}