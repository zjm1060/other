#include "historyrec.h"

printCond print_cond;
printCond* lpprint_cond;
CHistoryRecTable::CHistoryRecTable(QWidget* parent)
	:QWidget(parent,Qt::WindowMinMaxButtonsHint)
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
//	this->setModal(true);
	actionDelRec = new QAction(QString::fromLocal8Bit("删除记录"), this);
	//exceltable = new CExcelTable;
	recPrint = new QProcess(parent);
	recSourceModel = new QStandardItemModel;
	recModelType = new QSortFilterProxyModel;
	recModelName = new QSortFilterProxyModel;
	recModelType->setSourceModel(recSourceModel);
	recModelType->setDynamicSortFilter(true);
	recModelName->setSourceModel(recModelType);
	recModelName->setDynamicSortFilter(true);
	connect(actionDelRec,SIGNAL(triggered()),this,SLOT(delRec()));
	connect(checkCmd,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(checkAlarm,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(checkOpt,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(checkCheck,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(checkInfo,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(buttonTodayDisplay,SIGNAL(clicked()),this,SLOT(displayTodayRec()));
	connect(buttonPrint,SIGNAL(clicked()),this,SLOT(exportToExcel()));
	connect(buttonSearch,SIGNAL(clicked()),this,SLOT(displayRec()));
	connect(buttonSelectTask,SIGNAL(clicked()),this,SLOT(openSelectTaskTable()));
	connect(buttonRecordDel,SIGNAL(clicked()),this,SLOT(delRecord()));
	//connect(exceltable,SIGNAL(exp()),this,SLOT(exportExcel()));
	connect(comboSearchType,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(setFilterSearchType(const QString &)));
	connect(comboSearchName,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(setFilterSearchName(const QString &)));
	connect(comboSearchName,SIGNAL(editTextChanged(const QString &)),this,SLOT(setFilterSearchName(const QString &)));
	init();
	if (flag_simutrainee)
	{
		dlg_created_flag[HISTORYRECDLG] = true;
		m_user_id = login_user_id;
	}
}
CHistoryRecTable::~CHistoryRecTable()
{
	if (flag_simutrainee)
	{
		dlg_created_flag[HISTORYRECDLG] = false;
	}
}
void CHistoryRecTable::showEvent(QShowEvent *event)
{

// 	if (flag_langueage == 0)
// 	{
// 		loadChinese();
// 	}
// 	else if (flag_langueage == 1)
// 	{
// 		loadEnglish();
// 	}
	comboSearchType->setCurrentIndex(0);
	comboSearchName->setCurrentIndex(0);
	timeEnd->setDateTime(QDateTime::currentDateTime());
	timeStart->setDateTime(QDateTime::currentDateTime().addSecs(-3000));
	recSourceModel->clear();
	recSourceModel->setColumnCount(5);  
	recSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
	recSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间")); 
	recSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("毫秒"));
	recSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("事件描述"));  
	recSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("主机名")); 
	viewRec->setColumnWidth(0,70);
	viewRec->setColumnWidth(1,130);
	viewRec->setColumnWidth(2,40);
	viewRec->setColumnWidth(3,630);
	viewRec->setColumnWidth(4,90);
	viewRec->horizontalHeader()->setStretchLastSection(true);
	//displayUserName();
	//displayGroups();
	event->accept();
}
void  CHistoryRecTable::setLanguage(int language)
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
void CHistoryRecTable::init()
{
	initdb();//dzy 2014
	m_user_id = -1;
	m_send_task_id = -1;
	checkCmd->setCheckState(Qt::Checked);
	checkAlarm->setCheckState(Qt::Checked);
	checkOpt->setCheckState(Qt::Checked);
	checkCheck->setCheckState(Qt::Checked);
	checkInfo->setCheckState(Qt::Checked);
	timeStart->setCalendarPopup(true);
	timeEnd->setCalendarPopup(true); 
	viewRec->setSelectionBehavior(QAbstractItemView::SelectRows);
	viewRec->setEditTriggers(QTableView::NoEditTriggers);
	viewRec->verticalHeader()->setVisible(false);
	viewRec->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	recModelName->setFilterRegExp(QRegExp("/*"));
	recModelType->setFilterRegExp(QRegExp("/*"));
	comboSearchType->addItem(QString::fromLocal8Bit("用户名"));
	comboSearchType->addItem(QString::fromLocal8Bit("组名"));
	QCompleter *completer = new QCompleter(this);
	completer->setModel(comboSearchName->model());
	comboSearchName->setCompleter(completer);
#ifndef WIN32
	buttonPrint->setEnabled(false);
#endif
	
}
void CHistoryRecTable::initdb()
{
	///HN2014 
	int server_port;
	if (strcmp(g_ots_hdb_type,"ORACLE")==0)
	{		
		server_port = 1521;
		db_hisrec =  QSqlDatabase::addDatabase("QOCI","dbhisrec"); 
	}
	else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
	{
		server_port = 3306;
		db_hisrec =  QSqlDatabase::addDatabase("QMYSQL","dbhisrec"); 
	}
	//	dbopr = QSqlDatabase::addDatabase("QMYSQL","dblocalpr"); 
	db_hisrec.setHostName(g_ots_hdb_ip);   
	db_hisrec.setDatabaseName(g_ots_hdb_sid);   
	db_hisrec.setUserName("ots");   
	db_hisrec.setPassword("ots");   
	db_hisrec.setPort(server_port);   
	if (db_hisrec.open())   
	{
		if (strcmp(g_ots_hdb_type,"MYSQL")==0)
			db_hisrec.exec(QString::fromLocal8Bit("SET NAMES 'utf8'")); 
	}
	else
	{
		return;
	}
}
void CHistoryRecTable::single_user_show()
{
	labelName->setVisible(false);
	buttonRecordDel->setVisible(false);
	comboSearchName->setVisible(false);
	comboSearchType->setVisible(false);
//	this->show();
}
void CHistoryRecTable::func_minimal()
{
	labelName->setVisible(false);
	buttonRecordDel->setVisible(false);
	comboSearchName->setVisible(false);
	comboSearchType->setVisible(false);
	buttonSelectTask->setVisible(false);
	buttonSearch->setVisible(false);
	buttonTodayDisplay->setVisible(false);
	buttonPrint->setVisible(false);
	timeStart->setVisible(false);
	timeEnd->setVisible(false);
	label_9->setVisible(false);
	label_10->setVisible(false);
	label->setVisible(false);
	groupBox->setVisible(false);
	//comboSearchType->setEnabled(false);
}
void CHistoryRecTable::loadChinese()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
	}
	else
		translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	//selecttasktable->loadChinese();
	//exceltable->loadChinese();
}
void CHistoryRecTable::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
	}
	else
		translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	//selecttasktable->loadEnglish();
	//exceltable->loadEnglish();
}

void CHistoryRecTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	//QTableWidgetItem* pTableWidgetItem=listFault->itemAt(listFault->mapFromGlobal(QCursor::pos()-p));
	//if(pTableWidgetItem != NULL)
	//{
	//	popMenu->addAction(actionDelRec);
	//}
	popMenu->addAction(actionDelRec);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单

void CHistoryRecTable::openSelectTaskTable()
{
	selecttasktable = new CSelectTaskTable(this);
	selecttasktable->show();
	selecttasktable->setUserId(m_user_id);
	connect(selecttasktable->buttonEnter,SIGNAL(clicked()),this,SLOT(selectTask()));
}
void CHistoryRecTable::displayUserName()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	comboSearchName->clear();
	comboSearchName->insertItem(0,QString::fromLocal8Bit("全部用户"));
	if( SelectRecs(&data,db_hisrec) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		qDebug("查询数据库失败!");
		return;
	}
	for(int i=0;i<data.rownum;i++)
	{
		comboSearchName->insertItem(i+1,QString::fromLocal8Bit(data_userinfo[i].longname),QVariant(data_userinfo[i].user_id));
	}
	FreeDbData(&data,TABLE_USERINFO);
}

void CHistoryRecTable::displayGroups()
{
	DB_FMT data;
	InitDbData(&data,TABLE_GROUPS);
	comboSearchName->clear();
	comboSearchName->insertItem(0,QString::fromLocal8Bit("无分组"),QVariant(-1));
	if( SelectRecs(&data,db_hisrec) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS);
		qDebug("查询数据库失败!");
		return;
	}
	for(int i=0;i<data.rownum;i++)
	{
		if (data_groups[i].groups_id == -1 || data_groups[i].groups_id == 0)
			continue;
		comboSearchName->insertItem(i+1,QString::fromLocal8Bit(data_groups[i].name),QVariant(data_groups[i].groups_id));
	}
	FreeDbData(&data,TABLE_GROUPS);
}
void CHistoryRecTable::displayTodayRec()
{
	QDateTime time_start,time_end;
	time_start.setDate(QDate::currentDate());
	time_start.setTime(QTime(0,0,0,0));
	time_end.setDate(QDate::currentDate());
	time_end.setTime(QTime(23,59,59,999));
	timeStart->setDateTime(time_start);
	timeEnd->setDateTime(time_end);
	displayRec();
}

void CHistoryRecTable::displayRec()
{
	if (flag_simutrainee&&shm_ots_addr->data_task.task_status ==2)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("当前任务执行中，请在任务完成后查询"),QMessageBox::Ok);
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);
	data.cond_flag.flag_record.datetime = TRUE;
	data.cond_flag.flag_record.datetime_end = TRUE;
	strcpy(data.cond_seg.seg_record.datetime,timeStart->dateTime().toString(timeStart->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_seg.seg_record.datetime_end,timeEnd->dateTime().toString(timeEnd->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_flag.flag_record.datetime_opr,">=");
	strcpy(data.cond_flag.flag_record.datetime_end_opr,"<=");
	if (m_send_task_id != -1)
	{
		data.cond_flag.flag_record.send_task_id = TRUE;
		data.cond_seg.seg_record.send_task_id = m_send_task_id;
	}
	if (m_user_id != -1&&m_user_id!=0)//dzy added  0系统用户
	{
		data.cond_flag.flag_record.user_id = TRUE;
		data.cond_seg.seg_record.user_id = m_user_id;
	}
	else if (comboSearchType->currentText() == QString::fromLocal8Bit("用户名")) //dzy modified
	{
		if (comboSearchName->currentText() != QString::fromLocal8Bit("全部用户"))
		{
			data.cond_flag.flag_record.user_id = TRUE;
			int index = comboSearchName->findText(comboSearchName->currentText());
			if (index == -1)
				data.cond_seg.seg_record.user_id = index;
			else
				data.cond_seg.seg_record.user_id = comboSearchName->itemData(index).toInt();
		}
	}
	else if (comboSearchType->currentText() == QString::fromLocal8Bit("组名"))
	{
		data.cond_flag.flag_record.groups_id = TRUE;
		int index = comboSearchName->findText(comboSearchName->currentText());
		if (index == -1)
			data.cond_seg.seg_record.groups_id = index;
		else
			data.cond_seg.seg_record.groups_id = comboSearchName->itemData(index).toInt();	
	}
	if(flag_simutrainee == true)
	{
		data.cond_flag.flag_record.user_id = TRUE;
		data.cond_seg.seg_record.user_id = m_user_id;
	}
	if (m_user_id!=-1&&m_send_task_id!=-1)//dzy added send_task_id && user_id可以确定时间范围 取消时间限制
	{
		data.cond_flag.flag_record.datetime = FALSE;
		data.cond_flag.flag_record.datetime_end = FALSE;
	}
	setRecSourceModel(&data);
	m_send_task_id = -1;
	comboSearchType->setCurrentIndex(0);
	comboSearchName->setCurrentIndex(0);
	timeEnd->setDateTime(QDateTime::currentDateTime());
	timeStart->setDateTime(QDateTime::currentDateTime().addSecs(-3000));
}

void CHistoryRecTable::setRecSourceModel(DB_FMT *data)
{
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("搜索数据库,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	viewRec->setModel(NULL);
	if( SelectRecs(data,db_hisrec) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data->sql_str,data->sql_error);
		FreeDbData(data,TABLE_RECORD);
		qDebug("查询数据库失败!");
		return;
	}
	if (data->rec_num > MAX_ROW_NUM_LARGE)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("信息!"),QString::fromLocal8Bit("对不起，你所查询的记录数已大于最大显示数量，\n可能会造成记录显示不全，请重新输入查询条件!"),QMessageBox::Ok);
	}
	progressDlg->setRange(0,data->rownum-1);
	recSourceModel->clear();
	recSourceModel->setColumnCount(5);  
	recSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
	recSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间")); 
	recSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("毫秒"));
	recSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("事件描述")); 
	recSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("主机名"));
	for (int i=0;i<data->rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_record[i].userinfo_longname));  
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_record[i].datetime));  
		QStandardItem* item2 = new QStandardItem(QString::number(data_record[i].ms));  
		QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(data_record[i].content)); 
		QStandardItem* item4 = new QStandardItem(QString::fromLocal8Bit(data_record[i].host_name)); 
		QStandardItem* item5 = new QStandardItem(QString::fromLocal8Bit(data_record[i].logicalname));
		QStandardItem* item6 = new QStandardItem(QString::number(data_record[i].type)); 
		QStandardItem* item7 = new QStandardItem(QString::fromLocal8Bit(data_record[i].groups_name));  
		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		item4->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2 << item3 << item4 << item5 << item6 << item7; 
		recSourceModel->insertRow(0,items); 
		progressDlg->setValue(i); 
		if(progressDlg->wasCanceled())  
			break;
	}
	viewRec->setModel(recModelName);
	selectionModel = viewRec->selectionModel();
	viewRec->hideColumn(5);
	viewRec->hideColumn(6);
	viewRec->hideColumn(7);
	viewRec->setColumnWidth(0,70);
	viewRec->setColumnWidth(1,130);
	viewRec->setColumnWidth(2,40);
	viewRec->setColumnWidth(3,630);
	viewRec->setColumnWidth(4,90);
	FreeDbData(data,TABLE_RECORD);
}
void CHistoryRecTable::setFilterSearchType(const QString &text)
{
	labelName->setText(text+QString::fromLocal8Bit("："));
	if (text == QString::fromLocal8Bit("用户名"))
	{
		displayUserName();
	}
	else if (text == QString::fromLocal8Bit("组名"))
	{
		displayGroups();
	}
}
void CHistoryRecTable::setFilterSearchName(const QString &text)
{
	if (comboSearchType->currentText() == QString::fromLocal8Bit("用户名"))
	{
		recModelName->setFilterKeyColumn(0);
		if (comboSearchName->currentText() != QString::fromLocal8Bit("全部用户"))
		{
			recModelName->setFilterRegExp(QRegExp(text));
		}
		else
		{
			recModelName->setFilterRegExp(QRegExp("/*"));
		}
	}
	else if (comboSearchType->currentText() == QString::fromLocal8Bit("组名"))
	{
		recModelName->setFilterKeyColumn(7);
		recModelName->setFilterRegExp(QRegExp(text));
	}
}

void CHistoryRecTable::setFilterType()
{
	QString strRegExp = QString("-1");
	if (checkCmd->checkState()==Qt::Checked)
		strRegExp.append("|0");
	if (checkAlarm->checkState()==Qt::Checked)
		strRegExp.append("|1");
	if (checkOpt->checkState()==Qt::Checked)
		strRegExp.append("|3");
	if (checkCheck->checkState()==Qt::Checked)
		strRegExp.append("|4");
	if (checkInfo->checkState()==Qt::Checked)
		strRegExp.append("|2");

	recModelType->setFilterKeyColumn(6);
	recModelType->setFilterRegExp(QRegExp(strRegExp));
}
void CHistoryRecTable::delRec()
{
	if (JudgeUserRole(m_user_id,ROLE_STUDENT) == TRUE)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("您没有权限删除记录!"),QMessageBox::Ok);
		return;
	}
	if (recSourceModel->rowCount()<0)
		return;
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
		delRecFromHdb(indexes.at(i).row());
		recModelName->removeRow(indexes.at(i).row());
	}
}

void CHistoryRecTable::delRecFromHdb(int row)
{
	QString time,ms;
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);
	time = recModelName->data(recModelName->index(row,1)).toString();
	ms = recModelName->data(recModelName->index(row,2)).toString();
	data.cond_flag.flag_record.datetime = TRUE;
	data.cond_flag.flag_record.ms = TRUE;
	strcpy(data.cond_seg.seg_record.datetime,time.toLocal8Bit().data());
	data.cond_seg.seg_record.ms = ms.toShort();
	if( DeleteOneRec(&data,db_hisrec) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_RECORD);
		return;
	}
	FreeDbData(&data,TABLE_RECORD);
}
void CHistoryRecTable::selectTask()
{
	QModelIndexList indexes = selecttasktable->selectionModel->selectedRows(0);
	if (!selecttasktable->selectionModel->hasSelection())
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("请选择任务!"),QMessageBox::Ok);
		return;
	}
	else if (indexes.size() != 1)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("只能选择一条任务!"),QMessageBox::Ok);
		return;
	}
	else
	{
		int curRow = indexes.at(0).row();
		QTableWidgetItem* start_time = selecttasktable->listTask->item(curRow,2);
		QTableWidgetItem* end_time = selecttasktable->listTask->item(curRow,3);
		timeStart->setDateTime(QDateTime::fromString(start_time->text(),QString("yyyy-MM-dd HH:mm:ss")));
		timeEnd->setDateTime(QDateTime::fromString(end_time->text(),QString("yyyy-MM-dd HH:mm:ss")));
		m_send_task_id = selecttasktable->listTask->item(curRow,0)->data(Qt::UserRole).toInt();
	//	m_task_id =  selecttasktable->listTask->item(curRow,1)->data(Qt::UserRole).toInt();
		selecttasktable->close();
	//	displayRec();
	}
}
void CHistoryRecTable::delRecord()
{
	if (JudgeUserRole(m_user_id,ROLE_STUDENT) == TRUE)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("您没有权限删除记录!"),QMessageBox::Ok);
		return;
	}
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
	msgBox.setInformativeText(QString::fromLocal8Bit("是否确认删除\n%1\n到\n%2\n的历史记录")
								.arg(timeStart->dateTime().toString(timeStart->displayFormat()))
								.arg(timeEnd->dateTime().toString(timeEnd->displayFormat())));
	msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Cancel) 
	{
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);
	data.cond_flag.flag_record.datetime = TRUE;
	data.cond_flag.flag_record.datetime_end = TRUE;
	strcpy(data.cond_seg.seg_record.datetime,timeStart->dateTime().toString(timeStart->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_seg.seg_record.datetime_end,timeEnd->dateTime().toString(timeEnd->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_flag.flag_record.datetime_opr,">=");
	strcpy(data.cond_flag.flag_record.datetime_end_opr,"<=");
	if( DeleteOneRec(&data,db_hisrec) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}
void CHistoryRecTable::print()
{
#ifdef   WIN32
	//shmPrintCond();
	//recPrint->start("print.exe");
	//exceltable->show();
#else
	#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::ScreenResolution);
	QPrintPreviewDialog dlg(&printer);
	PrintView view;
	view.setModel(listRec->model());
	connect(&dlg, SIGNAL(paintRequested(QPrinter *)),
	&view, SLOT(print(QPrinter *)));
	dlg.exec();
	#endif
#endif 

	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.xls)"));
	if (fileName.isEmpty())
		return;
	ExportExcelObject obj;
	obj.setOutputFilePath(fileName);
	obj.setOutputSheetTitle("sheet");
	obj.setTableView(viewRec);
	obj.addField(0, "用户名", "char(10)");
	obj.addField(1, "时间", "datetime");
	obj.addField(2, "毫秒", "int");
	obj.addField(3, "事件描述", "char(200)");
	obj.addField(4, "主机名", "char(10)");
	obj.addField(5, "点名", "char(20)");
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("正在导出......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	progressDlg->setRange(0,viewRec->model()->rowCount());  
	connect(&obj, SIGNAL(exportedRowCount(int)),progressDlg,SLOT(setValue(int)));
	connect(progressDlg, SIGNAL(canceled()),&obj,SLOT(cancel()));
	int retVal = obj.export2Excel();
	if( retVal > 0)
	{
		if (obj.initExcelObject() == TRUE)//设置导出的excel文件格式
		{
			//obj.setColumnWidth(1,10);
			//obj.setColumnWidth(2,20);
			//obj.setColumnWidth(3,6);
			//obj.setColumnWidth(4,60);
			//obj.setColumnWidth(5,12);
			//obj.setColumnWidth(6,30);
			obj.resizeColumnsToContents();
			obj.setColumnFormat(2,"yyyy/m/d h:mm:ss");
			obj.setAlignment(0,1,Qt::AlignCenter);
			obj.setAlignment(0,2,Qt::AlignCenter);
			obj.setAlignment(0,3,Qt::AlignCenter);
			obj.setAlignment(0,4,Qt::AlignLeft);
			obj.setAlignment(0,5,Qt::AlignCenter);
			obj.setAlignment(0,6,Qt::AlignCenter);
			obj.setAlignment(1,0,Qt::AlignCenter);
			obj.save();
		}
		progressDlg->cancel();
		QMessageBox::information(this, QString::fromLocal8Bit("完成"),
			QString(QString::fromLocal8Bit("共导出%1条记录")).arg(retVal));
	}
}
void CHistoryRecTable::exportToExcel()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.xls)"));
	if (fileName.isEmpty())
		return;
	ExcelObjectODBC excel(fileName);
	excel.SetSheetName(QString::fromLocal8Bit("sheet"));
	excel.AddHeader(0, "用户名", "char(10)");
	excel.AddHeader(1, "时间", "char(22)");
	excel.AddHeader(2, "毫秒", "int");
	excel.AddHeader(3, "事件描述", "char(200)");
	excel.AddHeader(4, "主机名", "char(10)");
	excel.AddHeader(5, "点名", "char(20)");
	if (excel.Open())
	{
		QProgressDialog *progressDlg=new QProgressDialog(this); 
		QFont font("ZYSong18030",12);  
		progressDlg->setFont(font);  
		progressDlg->setWindowModality(Qt::WindowModal);  
		progressDlg->setMinimumDuration(5);  
		progressDlg->setWindowTitle(tr("please wait"));  
		progressDlg->setLabelText(tr("正在导出......      "));  
		progressDlg->setCancelButtonText(tr("取消"));  
		progressDlg->setRange(0,viewRec->model()->rowCount());  
		connect(&excel, SIGNAL(exportedRowCount(int)),progressDlg,SLOT(setValue(int)));
		connect(progressDlg, SIGNAL(canceled()),&excel,SLOT(cancel()));
		int retVal = excel.ExportTableViewToExcel(viewRec);
		if( retVal > 0)
		{
			progressDlg->cancel();
			QMessageBox::information(this, QString::fromLocal8Bit("完成"),
				QString(QString::fromLocal8Bit("共导出%1条记录")).arg(retVal));
		}
	}
}
//void CHistoryRecTable::exportExcel()
//{
//	QDir excelPath;
//	QDir recPath;
//	QString appPath = qApp->applicationDirPath();
//	QString curDrive = appPath.mid(0,2);
// 	excelPath.setPath(curDrive+QString(ExcelPath));
//	recPath.setPath(curDrive+QString(HistoryPath));
//	QString absExcelPath = excelPath.absoluteFilePath("excel.xls");
//	absExcelPath.replace(QString("/"),QString("\\\\"));
//	QString recName = exceltable->comboProName->currentText();
//	QString date = QDate::currentDate().toString(QString("yyyy.MM.dd"));
//	QString extName(".xls");
//	QString absRecPath = recPath.absoluteFilePath(recName);
//	excel = new QAxObject( "Excel.Application", this ); //获取一个Excel对象
//	if (!excel)
//	{
//		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("初始化Excel错误，可能没有安装office组件!"),QMessageBox::Ok);
//		return;
//	}
//	QAxObject *workbooks = excel->querySubObject( "Workbooks" ); //得到Workbooks集合的指针 
//	if (!workbooks)
//	{
//		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("初始化Excel错误，可能没有安装office组件!"),QMessageBox::Ok);
//		return;
//	}
//	QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)", absExcelPath ); //打开硬盘上的一个Excel文档 
//	if (!workbook)
//	{
//		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("Excel文件打开错误!"),QMessageBox::Ok);
//		return;
//	}
//	QAxObject *worksheets = workbook->querySubObject("worksheets");
//	QAxObject *worksheet = workbook->querySubObject("worksheets(int)", 1);
//	QAxObject *usedrange = worksheet->querySubObject("usedrange");//得到用户已使用区域
//	/********************************************合并单元格*********************************************/
//	QAxObject *range = worksheet->querySubObject("Range(const QVariant&)", QVariant(QString("A1:G1")));
//	range->setProperty("HorizontalAlignment", 0xFFFFEFF4);
//	range->setProperty("VerticalAlignment", 0xFFFFEFF4);
//	range->setProperty("WrapText", false);
//	range->setProperty("Orientation", 0);
//	range->setProperty("AddIndent", false);
//	range->setProperty("IndentLevel", 0);
//	range->setProperty("ShrinkToFit", false);
//	range->setProperty("ReadingOrder", 0xFFFFEC76);
//	range->setProperty("MergeCells", true);
//	/****************************************************************************************************/
//	QAxObject* title1 = worksheet->querySubObject( "Cells( int, int )", 2, 1 );
//	title1->dynamicCall( "SetValue(const QString&)", QString::fromLocal8Bit("序号") );
//	QAxObject* title2 = worksheet->querySubObject( "Cells( int, int )", 2, 2 );
//	title2->dynamicCall( "SetValue(const QString&)", QString::fromLocal8Bit("用户名") );
//	QAxObject* title3 = worksheet->querySubObject( "Cells( int, int )", 2, 3 );
//	title3->dynamicCall( "SetValue(const QString&)", QString::fromLocal8Bit("时间") );
//	QAxObject* title4 = worksheet->querySubObject( "Cells( int, int )", 2, 4 );
//	title4->dynamicCall( "SetValue(const QString&)", QString::fromLocal8Bit("毫秒") );
//	QAxObject* title5 = worksheet->querySubObject( "Cells( int, int )", 2, 5 );
//	title5->dynamicCall( "SetValue(const QString&)", QString::fromLocal8Bit("事件描述") );
//	QAxObject* title6 = worksheet->querySubObject( "Cells( int, int )", 2, 6 );
//	title6->dynamicCall( "SetValue(const QString&)", QString::fromLocal8Bit("点名") );
//	QAxObject* title7 = worksheet->querySubObject( "Cells( int, int )", 2, 7 );
//	title7->dynamicCall( "SetValue(const QString&)", QString::fromLocal8Bit("主机名") );
//	for (int i=0;i<recModelType->rowCount();i++)
//	{
//		QAxObject* cell = worksheet->querySubObject( "Cells( int, int )", i+3, 1 );
//		cell->dynamicCall( "SetValue(const QString&)", QString::number(i+1) );
//		for (int j=0;j<6;j++)
//		{
//			QAxObject* cell = worksheet->querySubObject( "Cells( int, int )", i+3, j+2 );
//			cell->dynamicCall( "SetValue(const QString&)", recModelType->data(recModelType->index(i, j)).toString());
//		}
//	}
//	QString cell;
//	cell = QString("A2")+QString(":")+QString("G")+QString::number(recModelType->rowCount()+2);
//	QAxObject* contentrange = worksheet->querySubObject("Range(const QVariant&)",QVariant(cell));
//	contentrange->setProperty( "VerticalAlignment", QVariant(-4108));//设置为垂直居中对齐方式
//	contentrange->setProperty( "HorizontalAlignment", QVariant(-4108));//设置为水平居中对齐方式
//	QAxObject* border = contentrange->querySubObject("Borders");
//	border->setProperty( "LineStyle", QVariant(1));//设置表格边框样式
//	QAxObject* font = contentrange->querySubObject("Font");
//	font->setProperty( "Name", QVariant(QString::fromLocal8Bit("宋体")));//设置字体为宋体
//	font->setProperty( "Name", QVariant(QString::fromLocal8Bit("Times New Roman")));//设置字体为Times New Roman
//	font->setProperty( "Size", QVariant(11));//设置字体大小为11
//	QAxObject* title = worksheet->querySubObject( "Cells( int, int )", 1, 1 );
//	title->dynamicCall( "SetValue(const QString&)", recName+QString::fromLocal8Bit("模拟操作历史记录") );
//	title = title->querySubObject("Font");
//	title->setProperty( "Size", QVariant(23));//设置标题的字体大小为23
//	QAxObject *column = worksheet->querySubObject("columns(const QString&)", QString("E:E"));
//	column->setProperty( "HorizontalAlignment", QVariant(-4131));//设置第F列为水平左对齐方式
//	QAxObject* titlerange = worksheet->querySubObject("Range(const QVariant&)",QVariant(QString("A1:G2")));
//	titlerange->setProperty( "HorizontalAlignment", QVariant(-4108));
//	titlerange = titlerange->querySubObject("Font");
//	titlerange->setProperty("Bold" , true);
//	QAxObject *rows = worksheet->querySubObject("rows");
//	rows->setProperty( "RowHeight", QVariant(20));//设置行高为20
//	QAxObject *titlerow = worksheet->querySubObject("rows(int)", 1);
//	titlerow->dynamicCall( "AutoFit()");//设置第一行行高为自动
//	/********************************************设置列宽**********************************************/
//	QAxObject *columnA = worksheet->querySubObject("columns(const QString&)", QString("A:A"));
//	columnA->setProperty( "ColumnWidth", QVariant(5));//设置第一列列宽为5
//	QAxObject *columnB = worksheet->querySubObject("columns(const QString&)", QString("B:B"));
//	columnB->setProperty( "ColumnWidth", QVariant(10));
//	QAxObject *columnC = worksheet->querySubObject("columns(const QString&)", QString("C:C"));
//	columnC->setProperty( "ColumnWidth", QVariant(15));
//	QAxObject *columnD = worksheet->querySubObject("columns(const QString&)", QString("D:D"));
//	columnD->setProperty( "ColumnWidth", QVariant(5));
//	QAxObject *columnE = worksheet->querySubObject("columns(const QString&)", QString("E:E"));
//	columnE->setProperty( "ColumnWidth", QVariant(76));
//	QAxObject *columnF = worksheet->querySubObject("columns(const QString&)", QString("F:F"));
//	columnF->setProperty( "ColumnWidth", QVariant(30));
//	QAxObject *columnG = worksheet->querySubObject("columns(const QString&)", QString("G:G"));
//	columnG->setProperty( "ColumnWidth", QVariant(12));
//	/**************************************************************************************************/
//	excel->dynamicCall(" SetScreenUpdating(bool)",true);
//	if (QFile::exists(absRecPath+date+extName))
//	{
//		for(int i=1;i<100;i++)
//		{
//			if (!QFile::exists(absRecPath+date+QString("-")+QString::number(i)+extName))
//			{
//				workbook->querySubObject( "SaveAs(const QString&)",absRecPath+date+QString("-")+QString::number(i)+extName);
//				break;
//			}
//		}
//	}
//	else
//	{
//		workbook->querySubObject( "SaveAs(const QString&)",absRecPath+date+extName); 
//	}
//	
//	excel->dynamicCall( "SetVisible(bool)", TRUE );
//	exceltable->reject();
//}
void CHistoryRecTable::shmPrintCond()
{
	HANDLE hMemShare;
	hMemShare = CreateFileMapping((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,sizeof(printCond),L"MemShare");
	if(hMemShare == NULL)
	{
		printf("分配共享内存失败");
		return;
	}
	lpprint_cond = (printCond *)MapViewOfFile(hMemShare, FILE_MAP_WRITE,0,0,sizeof(printCond));
	if(lpprint_cond == NULL)
	{
		printf("映射共享内存失败");
		return;
	}
	*lpprint_cond = print_cond;
	UnmapViewOfFile(lpprint_cond);
}
/*******************************************************CSelectTaskTable*****************************************************************/
CSelectTaskTable::CSelectTaskTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	setControlAtt();
	actionDel = new QAction(QString::fromLocal8Bit("删除记录"), this);
	selectionModel = listTask->selectionModel();
	connect(listTask,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(showSendTaskMenu(const QPoint &)));
	connect(actionDel,SIGNAL(triggered()),this,SLOT(delSendTask()));
}
void CSelectTaskTable::setControlAtt()
{
	listTask->setSelectionBehavior(QAbstractItemView::SelectRows);
	listTask->setSelectionMode(QAbstractItemView::ExtendedSelection); 
	listTask->setEditTriggers(QTableWidget::NoEditTriggers);
	listTask->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	listTask->setSortingEnabled(true);
	listTask->setColumnWidth(0,220);
	listTask->setColumnWidth(1,70);
	listTask->setColumnWidth(2,130);
	listTask->setColumnWidth(3,130);
}
void CSelectTaskTable::contextMenuEvent(QContextMenuEvent *event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	popMenu->addAction(actionDel);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}
void CSelectTaskTable::showSendTaskMenu(const QPoint &pos)
{
	QMenu* popMenu = new QMenu(this);
	popMenu->addAction(actionDel);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}
void CSelectTaskTable::loadChinese()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
	}
	else
		translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CSelectTaskTable::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
	}
	else
		translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CSelectTaskTable::showEvent ( QShowEvent * event )
{
	displayTask();
}
int CSelectTaskTable::displayTask()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SEND_TASK);
	if (flag_simutrainee)
	{
		data.cond_flag.flag_userinfo.user_id = TRUE;
		data.cond_seg.seg_userinfo.user_id = login_user_id;
	}
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_SEND_TASK);
		qDebug("查询数据库失败!");
		return -1;
	}
	listTask->clearContents();
	listTask->setRowCount(data.rownum);
	int rownum = data.rownum;
	for(int i=0;i<rownum;i++)
	{		
		QTableWidgetItem* itemTaskName = new QTableWidgetItem(QString::fromLocal8Bit(data_send_task[i].task_name));
		itemTaskName->setData(Qt::UserRole,data_send_task[i].send_task_id);
		QTableWidgetItem* itemUserName = new QTableWidgetItem(QString::fromLocal8Bit(data_send_task[i].userinfo_longname));
		itemUserName->setTextAlignment(Qt::AlignCenter);
		listTask->setItem(i,0,itemTaskName);
		listTask->setItem(i,1,itemUserName);
		listTask->setItem(i,2,new QTableWidgetItem(QString::fromLocal8Bit(data_send_task[i].start_time)));
		listTask->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit(data_send_task[i].end_time)));
	}
	FreeDbData(&data,TABLE_SEND_TASK);
	return rownum;
}
void CSelectTaskTable::delSendTask()
{
	if (JudgeUserRole(m_user_id,ROLE_STUDENT) == TRUE)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("您没有权限删除记录!"),QMessageBox::Ok);
		return;
	}
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
	if (selectionModel->hasSelection())
	{
		QModelIndexList indexes = selectionModel->selectedRows(0);
		for (int i=indexes.size()-1;i>=0;i--)
		{
			int send_task_id = listTask->item(indexes.at(i).row(),0)->data(Qt::UserRole).toInt();
			delRecordFromHdb(send_task_id);
			delSendTaskFromHdb(send_task_id);
			listTask->removeRow(indexes.at(i).row());
		}
	}
}
void CSelectTaskTable::delSendTaskFromHdb(int send_task_id)
{
	DB_FMT data;
	InitDbData(&data,TABLE_SEND_TASK);
	data.cond_flag.flag_send_task.send_task_id = TRUE;
	data.cond_seg.seg_send_task.send_task_id = send_task_id;
	if( DeleteOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_SEND_TASK);
		return;
	}
	FreeDbData(&data,TABLE_SEND_TASK);
}
void CSelectTaskTable::delRecordFromHdb(int send_task_id)
{
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);
	data.cond_flag.flag_record.send_task_id = TRUE;
	data.cond_seg.seg_record.send_task_id = send_task_id;
	if( DeleteOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_RECORD);
		return;
	}
}
/***************************************************CExcelTable*****************************************************/
//CExcelTable::CExcelTable(QWidget* parent)
//:QDialog(parent)
//{
//	setupUi(this);
//	this->setAttribute(Qt::WA_DeleteOnClose);
//	this->setModal(true);
//	readConfig();
//	//readTemplateDef();
//	comboProName->setCurrentIndex(-1);
//	comboProName->setEditable(true);
//	connect(buttonExport,SIGNAL(clicked()),this,SLOT(exportExcel()));
//}
//void CExcelTable::loadChinese()
//{
//	QTranslator translator;
//	translator.load("SimuTutor_zh_CN");
//	qApp->installTranslator(&translator);
//	retranslateUi(this);
//}
//
//void CExcelTable::loadEnglish()
//{
//	QTranslator translator;
//	translator.load("SimuTutor_en");
//	qApp->installTranslator(&translator);
//	retranslateUi(this);
//}
//void CExcelTable::exportExcel()
//{
//	if (comboProName->currentText() == QString(""))
//	{
//		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("请选择或输入工程名称!"),QMessageBox::Ok);
//		return;
//	}
//	else
//	{
//		writeConfig();
//		emit exp();
//		readConfig();
//	}	
//}
//void CExcelTable::readConfig()
//{
//	QString myfile = SimuTutorConfigPath;
//	QSettings mySetting(myfile,QSettings::IniFormat);
//	mySetting.setIniCodec(QTextCodec::codecForName("GB2312"));
//	mySetting.beginGroup("proname");
//	QStringList keys = mySetting.childKeys();
//	comboProName->clear();
//	for (int i=0;i<keys.size();i++)
//	{
//		comboProName->insertItem(i,mySetting.value(keys.at(i)).toString());
//	}
//}
//void CExcelTable::writeConfig()
//{
//	QString myfile = SimuTutorConfigPath;
//	QSettings mySetting(myfile,QSettings::IniFormat);
//	mySetting.setIniCodec(QTextCodec::codecForName("GB2312"));
//	mySetting.beginGroup("proname");
//	QStringList keys = mySetting.childKeys();
//	for (int i=0;i<keys.size();i++)
//	{
//		if(comboProName->currentText() == mySetting.value(keys.at(i)).toString())
//		{
//			return;
//		}
//	}
//	mySetting.setValue(QString("name")+QString::number(keys.size()+1),comboProName->currentText());
//}
//void CExcelTable::readTemplateDef()
//{
//	char FileName[100]="/home/ems/system/ots/SimuTutor/proname.def";
//	char StrTmp[100];
//	int i=0;
//	FILE *fp;
//	if (( fp = fopen ( FileName,"r"))==NULL)
//	{
//		printf ("==== can't open (w) the file %s! \n",FileName );
//		return;
//	}
//	else
//	{
//		 printf ( "\n==== open (w) %s  success! \n",FileName  ); 
//	}
//	while ( fscanf(fp,"%s",StrTmp)!=EOF )
//	{
//		if(strcmp(StrTmp,"BEGIN")==0) break;
//	}        
//	while ( fscanf(fp,"%s",StrTmp)!=EOF )
//	{
//		if(strcmp(StrTmp,"NAME")==0)
//		{
//			fscanf(fp,"%s",StrTmp);
//			comboProName->insertItem(i++,QString(StrTmp));
//		}
//
//		if(strcmp(StrTmp,"END")==0)
//		{
//			break;
//		}
//	}
//	fclose(fp);
//}
/************************************************PrintView*********************************************************/
PrintView::PrintView()
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void PrintView::print(QPrinter *printer)
{
#ifndef QT_NO_PRINTER
	resize(printer->width(), printer->height());
	render(printer);
#endif
}
/*****************************************************CStudyRec*******************************************************/
CStudyRecTable::CStudyRecTable(QWidget* parent)
	:QWidget(parent,Qt::WindowMinMaxButtonsHint)
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
//	this->setModal(true);
	actionDelRec = new QAction(QString::fromLocal8Bit("删除记录"), this);
	connect(actionDelRec,SIGNAL(triggered()),this,SLOT(delRec()));
	//exceltable = new CExcelTable;
	recPrint = new QProcess(parent);
	recSourceModel = new QStandardItemModel;
	recModelType = new QSortFilterProxyModel;
	recModelName = new QSortFilterProxyModel;
	recModelType->setSourceModel(recSourceModel);
	recModelType->setDynamicSortFilter(true);
	recModelName->setSourceModel(recModelType);
	recModelName->setDynamicSortFilter(true);

	connect(checkOprate,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(checDoc,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(checkOther,SIGNAL(clicked()),this,SLOT(setFilterType()));
	connect(buttonTodayDisplay,SIGNAL(clicked()),this,SLOT(displayTodayRec()));
	connect(buttonPrint,SIGNAL(clicked()),this,SLOT(exportToExcel()));
	connect(buttonSearch,SIGNAL(clicked()),this,SLOT(displayRec()));
	//connect(buttonSelectTask,SIGNAL(clicked()),this,SLOT(openSelectTaskTable()));
	connect(buttonRecordDel,SIGNAL(clicked()),this,SLOT(delRecord()));
	connect(comboSearchType,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(setFilterSearchType(const QString &)));
	connect(comboSearchName,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(setFilterSearchName(const QString &)));
	connect(comboSearchName,SIGNAL(editTextChanged(const QString &)),this,SLOT(setFilterSearchName(const QString &)));
	init();
	if (flag_simutrainee)
	{
		dlg_created_flag[STUDYRECDLG] = true;
		m_user_id = login_user_id;
		label->setText("");
		label->setVisible(false);
	}
}
CStudyRecTable::~CStudyRecTable()
{
	if (flag_simutrainee)
	{
		dlg_created_flag[STUDYRECDLG] = false;
	}
}
void CStudyRecTable::showEvent(QShowEvent *event)
{
	comboSearchType->setCurrentIndex(0);
	comboSearchName->setCurrentIndex(0);
	timeEnd->setDateTime(QDateTime::currentDateTime());
	timeStart->setDateTime(QDateTime::currentDateTime().addSecs(-3000));
	recSourceModel->clear();
	recSourceModel->setColumnCount(5);  
	recSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
	recSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间")); 
	recSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("类型"));
	recSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("学习内容"));  //////////??????????????????????????????????????????????
	recSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("所得学分")); 
	viewRec->setColumnWidth(0,70);
	viewRec->setColumnWidth(1,130);
	viewRec->setColumnWidth(2,40);
	viewRec->setColumnWidth(3,630);
	viewRec->setColumnWidth(4,90);
	viewRec->horizontalHeader()->setStretchLastSection(true);
	//displayUserName();
	//displayGroups();
	event->accept();
}
void  CStudyRecTable::setLanguage(int language)
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
void CStudyRecTable::init()
{
	initdb();//dzy 2014
	m_user_id = -1;
//	m_send_task_id = -1;
	checkOprate->setCheckState(Qt::Checked);
	checDoc->setCheckState(Qt::Checked);
	checkOther->setCheckState(Qt::Checked);

	timeStart->setCalendarPopup(true);
	timeEnd->setCalendarPopup(true); 
	viewRec->setSelectionBehavior(QAbstractItemView::SelectRows);
	viewRec->setEditTriggers(QTableView::NoEditTriggers);
	viewRec->verticalHeader()->setVisible(false);
	viewRec->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	recModelName->setFilterRegExp(QRegExp("/*"));
	recModelType->setFilterRegExp(QRegExp("/*"));
	comboSearchType->addItem(QString::fromLocal8Bit("用户名"));
	comboSearchType->addItem(QString::fromLocal8Bit("学习计划名"));
	QCompleter *completer = new QCompleter(this);
	completer->setModel(comboSearchName->model());
	comboSearchName->setCompleter(completer);
#ifndef WIN32
	buttonPrint->setEnabled(false);
#endif
	
}
void CStudyRecTable::initdb()
{
	///HN2014 
	int server_port;
	if (strcmp(g_ots_hdb_type,"ORACLE")==0)
	{		
		server_port = 1521;
		db_hisrec =  QSqlDatabase::addDatabase("QOCI","dbstudyrec"); 
	}
	else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
	{
		server_port = 3306;
		db_hisrec =  QSqlDatabase::addDatabase("QMYSQL","dbstudyrec"); 
	}
	//	dbopr = QSqlDatabase::addDatabase("QMYSQL","dblocalpr"); 
	db_hisrec.setHostName(g_ots_hdb_ip);   
	db_hisrec.setDatabaseName(g_ots_hdb_sid);   
	db_hisrec.setUserName("ots");   
	db_hisrec.setPassword("ots");   
	db_hisrec.setPort(server_port);   
	if (db_hisrec.open())   
	{
		if (strcmp(g_ots_hdb_type,"MYSQL")==0)
			db_hisrec.exec(QString::fromLocal8Bit("SET NAMES 'utf8'")); 
	}
	else
	{
		return;
	}
}
void CStudyRecTable::single_user_show()
{
	labelName->setVisible(false);
	buttonRecordDel->setVisible(false);
	comboSearchName->setVisible(false);
	comboSearchType->setVisible(false);
//	this->show();
}
void CStudyRecTable::func_minimal()
{
	labelName->setVisible(false);
	buttonRecordDel->setVisible(false);
	comboSearchName->setVisible(false);
	comboSearchType->setVisible(false);
	//buttonSelectTask->setVisible(false);
	buttonSearch->setVisible(false);
	buttonTodayDisplay->setVisible(false);
	buttonPrint->setVisible(false);
	timeStart->setVisible(false);
	timeEnd->setVisible(false);
	label_9->setVisible(false);
	label_10->setVisible(false);
	label->setVisible(false);
	groupBox->setVisible(false);
	//comboSearchType->setEnabled(false);
}
void CStudyRecTable::loadChinese()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
	}
	else
		translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CStudyRecTable::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
	}
	else
		translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CStudyRecTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	//QTableWidgetItem* pTableWidgetItem=listFault->itemAt(listFault->mapFromGlobal(QCursor::pos()-p));
	//if(pTableWidgetItem != NULL)
	//{
	//	popMenu->addAction(actionDelRec);
	//}
	popMenu->addAction(actionDelRec);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单


void CStudyRecTable::displayUserName()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	comboSearchName->clear();
	comboSearchName->insertItem(0,QString::fromLocal8Bit("全部用户"));
	if( SelectRecs(&data,db_hisrec) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		qDebug("查询数据库失败!");
		return;
	}
	for(int i=0;i<data.rownum;i++)
	{
		comboSearchName->insertItem(i+1,QString::fromLocal8Bit(data_userinfo[i].longname),QVariant(data_userinfo[i].user_id));
	}
	FreeDbData(&data,TABLE_USERINFO);
}

void CStudyRecTable::displayPlans()
{
	DB_FMT data;
	InitDbData(&data,TABLE_STUDY_PLAN);
	comboSearchName->clear();
	comboSearchName->insertItem(0,QString::fromLocal8Bit("无内容"),QVariant(-1));
	if( SelectRecs(&data,db_hisrec) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_STUDY_PLAN);
		qDebug("查询数据库失败!");
		return;
	}
	for(int i=0;i<data.rownum;i++)
	{
		if (data_study_plan[i].plan_id == -1)
			continue;
		comboSearchName->insertItem(i+1,QString::fromLocal8Bit(data_study_plan[i].name),QVariant(data_study_plan[i].plan_id));
	}
	FreeDbData(&data,TABLE_STUDY_PLAN);
}
void CStudyRecTable::displayTodayRec()
{
	QDateTime time_start,time_end;
	time_start.setDate(QDate::currentDate());
	time_start.setTime(QTime(0,0,0,0));
	time_end.setDate(QDate::currentDate());
	time_end.setTime(QTime(23,59,59,999));
	timeStart->setDateTime(time_start);
	timeEnd->setDateTime(time_end);
	displayRec();
}

void CStudyRecTable::displayRec()
{
	if (flag_simutrainee&&shm_ots_addr->data_task.task_status ==2)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("当前任务执行中，请在任务完成后查询"),QMessageBox::Ok);
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_STUDY_RECORD);
	if (timeStart->dateTime()!=timeEnd->dateTime())
	{
		data.cond_flag.flag_study_record.date_time = TRUE;
		data.cond_flag.flag_study_record.datetime_end = TRUE;
	}
	strcpy(data.cond_seg.seg_study_record.date_time,timeStart->dateTime().toString(timeStart->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_seg.seg_study_record.datetime_end,timeEnd->dateTime().toString(timeEnd->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_flag.flag_study_record.datetime_opr,">=");
	strcpy(data.cond_flag.flag_study_record.datetime_end_opr,"<=");
	if (m_user_id != -1&&m_user_id!=0)//dzy added  0系统用户
	{
		data.cond_flag.flag_study_record.user_id = TRUE;
		data.cond_seg.seg_study_record.user_id = m_user_id;
	}
	else if (comboSearchType->currentText() == QString::fromLocal8Bit("用户名")) //dzy modified
	{
		if (comboSearchName->currentText() != QString::fromLocal8Bit("全部用户"))
		{
			data.cond_flag.flag_study_record.user_id = TRUE;
			int index = comboSearchName->findText(comboSearchName->currentText());
			if (index == -1)
				data.cond_seg.seg_study_record.user_id = index;
			else
				data.cond_seg.seg_study_record.user_id = comboSearchName->itemData(index).toInt();
		}
	}
	else if (comboSearchType->currentText() == QString::fromLocal8Bit("学习计划名"))
	{
		data.cond_flag.flag_study_record.plan_id = TRUE;
		int index = comboSearchName->findText(comboSearchName->currentText());
		if (index == -1)
			data.cond_seg.seg_study_record.plan_id = index;
		else
			data.cond_seg.seg_study_record.plan_id  = comboSearchName->itemData(index).toInt();	
	}
	if(flag_simutrainee == true)
	{
		data.cond_flag.flag_study_record.user_id = TRUE;
		data.cond_seg.seg_study_record.user_id = m_user_id;
	}
// 	if (m_user_id!=-1&&m_send_task_id!=-1)//dzy added send_task_id && user_id可以确定时间范围 取消时间限制
// 	{
// 		data.cond_flag.flag_study_record.date_time = FALSE;
// 		data.cond_flag.flag_study_record.datetime_end = FALSE;
// 	}
	setRecSourceModel(&data);
	comboSearchType->setCurrentIndex(0);
	comboSearchName->setCurrentIndex(0);
	timeEnd->setDateTime(QDateTime::currentDateTime());
	timeStart->setDateTime(QDateTime::currentDateTime().addSecs(-360000));
}

void CStudyRecTable::setRecSourceModel(DB_FMT *data)
{
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("搜索数据库,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	viewRec->setModel(NULL);
	if( SelectRecs(data,db_hisrec) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data->sql_str,data->sql_error);
		FreeDbData(data,TABLE_STUDY_RECORD);
		qDebug("查询数据库失败!");
		return;
	}
	if (data->rec_num > MAX_ROW_NUM_LARGE)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("信息!"),QString::fromLocal8Bit("对不起，你所查询的记录数已大于最大显示数量，\n可能会造成记录显示不全，请重新输入查询条件!"),QMessageBox::Ok);
	}
	progressDlg->setRange(0,data->rownum-1);
	recSourceModel->clear();
	recSourceModel->setColumnCount(5);  
	recSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
	recSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间")); 
	recSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("类型"));
	recSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("学习内容"));  //////////??????????????????????????????????????????????
	recSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("所得学分")); 
	for (int i=0;i<data->rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_study_record[i].user_name));  
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_study_record[i].date_time));  
		QStandardItem* item2 = new QStandardItem(QString::number(data_study_record[i].type));  
		QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(data_study_record[i].plan_name)); 
		QStandardItem* item4 = new QStandardItem(QString::number(data_study_record[i].points)); 
		QStandardItem* item5 = new QStandardItem(QString::number(data_study_record[i].plan_id)); 
		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		item4->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2 << item3 << item4 << item5; 
		recSourceModel->insertRow(0,items); 
		progressDlg->setValue(i); 
		if(progressDlg->wasCanceled())  
			break;
	}
	viewRec->setModel(recModelName);
	selectionModel = viewRec->selectionModel();
	viewRec->hideColumn(5);
	viewRec->setColumnWidth(0,100);
	viewRec->setColumnWidth(1,150);
	viewRec->setColumnWidth(2,75);
	viewRec->setColumnWidth(3,630);
	viewRec->setColumnWidth(4,90);
	FreeDbData(data,TABLE_RECORD);
}
void CStudyRecTable::setFilterSearchType(const QString &text)
{
	labelName->setText(text+QString::fromLocal8Bit(":"));
	if (text == QString::fromLocal8Bit("用户名"))
	{
		displayUserName();
	}
	else if (text == QString::fromLocal8Bit("学习计划名"))
	{
		displayPlans();
	}
}
void CStudyRecTable::setFilterSearchName(const QString &text)
{
	if (comboSearchType->currentText() == QString::fromLocal8Bit("用户名"))
	{
		recModelName->setFilterKeyColumn(0);
		if (comboSearchName->currentText() != QString::fromLocal8Bit("全部用户"))
		{
			recModelName->setFilterRegExp(QRegExp(text));
		}
		else
		{
			recModelName->setFilterRegExp(QRegExp("/*"));
		}
	}
	else if (comboSearchType->currentText() == QString::fromLocal8Bit("学习计划名"))
	{
		recModelName->setFilterKeyColumn(7);
		recModelName->setFilterRegExp(QRegExp(text));
	}
}

void CStudyRecTable::setFilterType()////////////////////////////////////////////////
{
	QString strRegExp = QString("-1");
	if (checkOprate->checkState()==Qt::Checked)
		strRegExp.append("|0");
	if (checDoc->checkState()==Qt::Checked)
		strRegExp.append("|1");
	if (checkOther->checkState()==Qt::Checked)
		strRegExp.append("|2");
	recModelType->setFilterKeyColumn(6);
	recModelType->setFilterRegExp(QRegExp(strRegExp));
}
void CStudyRecTable::delRec()
{
	if (JudgeUserRole(m_user_id,ROLE_STUDENT) == TRUE)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("您没有权限删除记录!"),QMessageBox::Ok);
		return;
	}
	if (recSourceModel->rowCount()<0)
		return;
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
		delRecFromHdb(indexes.at(i).row());
		recModelName->removeRow(indexes.at(i).row());
	}
}

void CStudyRecTable::delRecFromHdb(int row)
{
	QString time,ms;
	DB_FMT data;
	InitDbData(&data,TABLE_STUDY_RECORD);
	time = recModelName->data(recModelName->index(row,1)).toString();
	ms = recModelName->data(recModelName->index(row,2)).toString();
	data.cond_flag.flag_record.datetime = TRUE;
	data.cond_flag.flag_record.ms = TRUE;
	strcpy(data.cond_seg.seg_record.datetime,time.toLocal8Bit().data());
	data.cond_seg.seg_record.ms = ms.toShort();
	if( DeleteOneRec(&data,db_hisrec) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_RECORD);
		return;
	}
	FreeDbData(&data,TABLE_RECORD);
}

void CStudyRecTable::delRecord()
{
	if (flag_simutrainee)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("您没有权限删除记录!"),QMessageBox::Ok);
		return;
	}
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
	msgBox.setInformativeText(QString::fromLocal8Bit("是否确认删除\n%1\n到\n%2\n的历史记录")
								.arg(timeStart->dateTime().toString(timeStart->displayFormat()))
								.arg(timeEnd->dateTime().toString(timeEnd->displayFormat())));
	msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Cancel) 
	{
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_STUDY_RECORD);
	data.cond_flag.flag_study_record.date_time = TRUE;
	data.cond_flag.flag_study_record.datetime_end = TRUE;
	strcpy(data.cond_seg.seg_study_record.date_time,timeStart->dateTime().toString(timeStart->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_seg.seg_study_record.datetime_end,timeEnd->dateTime().toString(timeEnd->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_flag.flag_study_record.datetime_opr,">=");
	strcpy(data.cond_flag.flag_study_record.datetime_end_opr,"<=");
	if( DeleteOneRec(&data,db_hisrec) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}
void CStudyRecTable::print()
{
// #ifdef   WIN32
// 	//shmPrintCond();
// 	//recPrint->start("print.exe");
// 	//exceltable->show();
// #else
// 	#ifndef QT_NO_PRINTER
// 	QPrinter printer(QPrinter::ScreenResolution);
// 	QPrintPreviewDialog dlg(&printer);
// 	PrintView view;
// 	view.setModel(listRec->model());
// 	connect(&dlg, SIGNAL(paintRequested(QPrinter *)),
// 	&view, SLOT(print(QPrinter *)));
// 	dlg.exec();
// 	#endif
// #endif 
// 
// 	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
// 		tr("Excel Files (*.xls)"));
// 	if (fileName.isEmpty())
// 		return;
// 	ExportExcelObject obj;
// 	obj.setOutputFilePath(fileName);
// 	obj.setOutputSheetTitle("sheet");
// 	obj.setTableView(viewRec);
// 	obj.addField(0, "用户名", "char(10)");
// 	obj.addField(1, "时间", "datetime");
// 	obj.addField(2, "毫秒", "int");
// 	obj.addField(3, "事件描述", "char(200)");
// 	obj.addField(4, "主机名", "char(10)");
// 	obj.addField(5, "点名", "char(20)");
// 	QProgressDialog *progressDlg=new QProgressDialog(this); 
// 	QFont font("ZYSong18030",12);  
// 	progressDlg->setFont(font);  
// 	progressDlg->setWindowModality(Qt::WindowModal);  
// 	progressDlg->setMinimumDuration(5);  
// 	progressDlg->setWindowTitle(tr("please wait"));  
// 	progressDlg->setLabelText(tr("正在导出......      "));  
// 	progressDlg->setCancelButtonText(tr("取消"));  
// 	progressDlg->setRange(0,viewRec->model()->rowCount());  
// 	connect(&obj, SIGNAL(exportedRowCount(int)),progressDlg,SLOT(setValue(int)));
// 	connect(progressDlg, SIGNAL(canceled()),&obj,SLOT(cancel()));
// 	int retVal = obj.export2Excel();
// 	if( retVal > 0)
// 	{
// 		if (obj.initExcelObject() == TRUE)//设置导出的excel文件格式
// 		{
// 			//obj.setColumnWidth(1,10);
// 			//obj.setColumnWidth(2,20);
// 			//obj.setColumnWidth(3,6);
// 			//obj.setColumnWidth(4,60);
// 			//obj.setColumnWidth(5,12);
// 			//obj.setColumnWidth(6,30);
// 			obj.resizeColumnsToContents();
// 			obj.setColumnFormat(2,"yyyy/m/d h:mm:ss");
// 			obj.setAlignment(0,1,Qt::AlignCenter);
// 			obj.setAlignment(0,2,Qt::AlignCenter);
// 			obj.setAlignment(0,3,Qt::AlignCenter);
// 			obj.setAlignment(0,4,Qt::AlignLeft);
// 			obj.setAlignment(0,5,Qt::AlignCenter);
// 			obj.setAlignment(0,6,Qt::AlignCenter);
// 			obj.setAlignment(1,0,Qt::AlignCenter);
// 			obj.save();
// 		}
// 		progressDlg->cancel();
// 		QMessageBox::information(this, QString::fromLocal8Bit("完成"),
// 			QString(QString::fromLocal8Bit("共导出%1条记录")).arg(retVal));
// 	}
}
void CStudyRecTable::exportToExcel()
{
// 	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
// 		tr("Excel Files (*.xls)"));
// 	if (fileName.isEmpty())
// 		return;
// 	ExcelObjectODBC excel(fileName);
// 	excel.SetSheetName(QString::fromLocal8Bit("sheet"));
// 	excel.AddHeader(0, "用户名", "char(10)");
// 	excel.AddHeader(1, "时间", "char(22)");
// 	excel.AddHeader(2, "毫秒", "int");
// 	excel.AddHeader(3, "事件描述", "char(200)");
// 	excel.AddHeader(4, "主机名", "char(10)");
// 	excel.AddHeader(5, "点名", "char(20)");
// 	if (excel.Open())
// 	{
// 		QProgressDialog *progressDlg=new QProgressDialog(this); 
// 		QFont font("ZYSong18030",12);  
// 		progressDlg->setFont(font);  
// 		progressDlg->setWindowModality(Qt::WindowModal);  
// 		progressDlg->setMinimumDuration(5);  
// 		progressDlg->setWindowTitle(tr("please wait"));  
// 		progressDlg->setLabelText(tr("正在导出......      "));  
// 		progressDlg->setCancelButtonText(tr("取消"));  
// 		progressDlg->setRange(0,viewRec->model()->rowCount());  
// 		connect(&excel, SIGNAL(exportedRowCount(int)),progressDlg,SLOT(setValue(int)));
// 		connect(progressDlg, SIGNAL(canceled()),&excel,SLOT(cancel()));
// 		int retVal = excel.ExportTableViewToExcel(viewRec);
// 		if( retVal > 0)
// 		{
// 			progressDlg->cancel();
// 			QMessageBox::information(this, QString::fromLocal8Bit("完成"),
// 				QString(QString::fromLocal8Bit("共导出%1条记录")).arg(retVal));
// 		}
// 	}
}

void CStudyRecTable::shmPrintCond()
{
	HANDLE hMemShare;
	hMemShare = CreateFileMapping((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,sizeof(printCond),L"MemShare");
	if(hMemShare == NULL)
	{
		printf("分配共享内存失败");
		return;
	}
	lpprint_cond = (printCond *)MapViewOfFile(hMemShare, FILE_MAP_WRITE,0,0,sizeof(printCond));
	if(lpprint_cond == NULL)
	{
		printf("映射共享内存失败");
		return;
	}
	*lpprint_cond = print_cond;
	UnmapViewOfFile(lpprint_cond);
}

