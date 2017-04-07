#include "scorerec.h"

CScoreRec::CScoreRec(QWidget* parent)
:QWidget(parent)
{
	setupUi(this);
	dlg_created_flag[USERSCOREDLG] = true;
	this->setAttribute(Qt::WA_DeleteOnClose);
//	this->setModal(true);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	recSourceModel = new QStandardItemModel;
	recModel = new QSortFilterProxyModel;
	recModel->setSourceModel(recSourceModel);


	listScoreRec->setEditTriggers(QTableView::NoEditTriggers);
	listScoreRec->setSelectionBehavior(QAbstractItemView::SelectRows);
	
	pushButton->setVisible(false);	
	//dzy0708
	listScoreRec->setAlternatingRowColors(true);  
	listScoreRec->setStyleSheet("QTableView{background-color: rgb(250, 250, 215);"  "alternate-background-color: rgb(191, 213, 215);}"); 
	//////0708
	listScoreRec->setSelectionMode(QAbstractItemView::ExtendedSelection);
	listScoreRec->verticalHeader()->setVisible(false);
	connect(comboMode,SIGNAL(currentIndexChanged(int)),this,SLOT(modeChanged(int)));
	connect(comboContent,SIGNAL(currentTextChanged(const QString &)),this,SLOT(filterChanged(const QString &)));
	connect(comboContent,SIGNAL(editTextChanged(const QString &)),this,SLOT(filterChanged(const QString &)));
	connect(buttonSearch,SIGNAL(clicked()),this,SLOT(displayScoreRec()));
	connect(buttonSearchTaday,SIGNAL(clicked()),this,SLOT(displayTodayRec()));
	connect(buttonExport,SIGNAL(clicked()),this,SLOT(exportToExcel()));

	comboMode->addItem(QString::fromLocal8Bit("全部记录"));
	if (!flag_simutrainee)
	{
		comboMode->addItem(QString::fromLocal8Bit("用户名"));
	}
	if(flag_simutrainee)
	{
		this->setWindowTitle(tr("成绩查询"));
		buttonExport->setVisible(false);

	}

	comboMode->addItem(QString::fromLocal8Bit("任务名"));	
	//DZY NEW ADDED
	m_SubScore = new SubScore();
	actionSubscore =  new QAction(QString::fromLocal8Bit("查看得分明细"), this);
	connect(actionSubscore,SIGNAL(triggered()),this,SLOT(on_subscore()));
	actionSetscore =  new QAction(QString::fromLocal8Bit("教员设置分数"), this);
	connect(actionSetscore,SIGNAL(triggered()),this,SLOT(on_Actset()));
	actionFindRcords =  new QAction(QString::fromLocal8Bit("查看本任务操作记录"), this);
	connect(actionFindRcords,SIGNAL(triggered()),this,SLOT(on_ActRecd()));
	connect(pushButton,SIGNAL(clicked()),this,SLOT(on_setscore()));
}
CScoreRec::~CScoreRec()
{
	dlg_created_flag[USERSCOREDLG] = false;
}
void CScoreRec::loadChinese()
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
void CScoreRec::loadEnglish()
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
void CScoreRec::contextMenuEvent(QContextMenuEvent * event)
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
	popMenu->addAction(actionSubscore);
	if (!flag_simutrainee)
	{
		popMenu->addAction(actionSetscore);
	}
	popMenu->addAction(actionFindRcords);

	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单

void CScoreRec::showEvent(QShowEvent *event)
{
// 	if (flag_langueage == 0)
// 	{
// 		loadChinese();
// 	}
// 	else if (flag_langueage == 1)
// 	{
// 		loadEnglish();
// 	}
	comboMode->setCurrentIndex(0);
	dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());
	dateTimeEditStart->setDateTime(QDateTime::currentDateTime().addSecs(-7200));
}
void  CScoreRec::setLanguage(int language)
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
void CScoreRec::modeChanged(int index)
{
	DB_FMT data;
	comboContent->clear();
	if (!flag_simutrainee)
	{
		switch(index)
		{
		case 0:
			comboContent->addItem(QString::fromLocal8Bit("请选择方式"));
			comboContent->setEditable(false);
			labelContent->setText(QString::fromLocal8Bit("全部记录:"));
			break;
		case 1:
			InitDbData(&data,TABLE_USERINFO);
			data.cond_flag.flag_userinfo.role = TRUE;
			data.cond_seg.seg_userinfo.role = 0;
			if( SelectRecs(&data) != TRUE)
			{		
				HdbLogWrite(data.sql_str,data.sql_error);
				return;
			}
			for (int i=0;i<data.rownum;i++)
				comboContent->addItem(QString::fromLocal8Bit(data_userinfo[i].longname),QVariant(data_userinfo[i].user_id));
			comboContent->setEditable(true);
			labelContent->setText(QString::fromLocal8Bit("用户名:"));
			break;
		case 2:
			InitDbData(&data,TABLE_TASK);
			if( SelectRecs(&data) != TRUE)
			{		
				HdbLogWrite(data.sql_str,data.sql_error);
				return;
			}
			for (int i=0;i<data.rownum;i++)
				comboContent->addItem(QString::fromLocal8Bit(data_task[i].name),QVariant(data_task[i].task_id));
			comboContent->setEditable(true);
			labelContent->setText(QString::fromLocal8Bit("任务名:"));
			break;
		default:
			break;
		}
	}
	else
	{
		switch(index)
		{
		case 0:
			comboContent->addItem(QString::fromLocal8Bit("请选择方式"));
			comboContent->setEditable(false);
			labelContent->setText(QString::fromLocal8Bit("全部记录:"));
			break;
		case 1:			
			InitDbData(&data,TABLE_TASK);
			if( SelectRecs(&data) != TRUE)
			{		
				HdbLogWrite(data.sql_str,data.sql_error);
				return;
			}
			for (int i=0;i<data.rownum;i++)
				comboContent->addItem(QString::fromLocal8Bit(data_task[i].name),QVariant(data_task[i].task_id));
			comboContent->setEditable(true);
			labelContent->setText(QString::fromLocal8Bit("任务名:"));
			break;
		default:
			break;
		}
	}
}
void CScoreRec::filterChanged(const QString &text)
{

// 	if ()
// 	{
// 	}
// 	if (comboMode->currentText() == QString::fromLocal8Bit("用户名"))
// 	{
// 		recModel->setFilterKeyColumn(0);
// 		recModel->setFilterRegExp(QRegExp(text));
// 	}
// 	else if (comboMode->currentText() == QString::fromLocal8Bit("任务名"))
// 	{
// 		recModel->setFilterKeyColumn(1);
// 		recModel->setFilterRegExp(QRegExp(text));
// 	}
}
void CScoreRec::displayTodayRec()
{
	QDateTime time_start,time_end;
	time_start.setDate(QDate::currentDate());
	time_start.setTime(QTime(0,0,0,0));
	time_end.setDate(QDate::currentDate());
	time_end.setTime(QTime(23,59,59,999));
	dateTimeEditStart->setDateTime(time_start);
	dateTimeEditEnd->setDateTime(time_end);
	displayScoreRec();
}

void CScoreRec::displayScoreRec()
{
	if (flag_simutrainee&&shm_ots_addr->data_task.task_status ==2)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("当前任务执行中，请在任务完成后查询"),QMessageBox::Ok);
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_SEND_TASK);
	data.cond_flag.flag_send_task.end_time_start = TRUE;
	data.cond_flag.flag_send_task.end_time_end = TRUE;
	strcpy(data.cond_seg.seg_send_task.end_time_start,dateTimeEditStart->dateTime().toString(dateTimeEditStart->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_seg.seg_send_task.end_time_end,dateTimeEditEnd->dateTime().toString(dateTimeEditEnd->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_flag.flag_send_task.end_time_start_opr,">=");
	strcpy(data.cond_flag.flag_send_task.end_time_end_opr,"<=");
	if (comboMode->currentIndex() ==1&&!flag_simutrainee)
	{
		data.cond_flag.flag_send_task.user_id = TRUE;
		data.cond_seg.seg_send_task.user_id = comboContent->itemData(1).toInt();
	}
	if (comboMode->currentIndex() ==2&&!flag_simutrainee )
	{
		data.cond_flag.flag_send_task.task_id = TRUE;
		data.cond_seg.seg_send_task.task_id = comboContent->itemData(2).toInt();
	}
	if (flag_simutrainee)
	{
		data.cond_flag.flag_send_task.user_id = TRUE;
		data.cond_seg.seg_send_task.user_id = login_user_id;
		if (comboMode->currentIndex() ==1 )
		{
			data.cond_flag.flag_send_task.task_id = TRUE;
			data.cond_seg.seg_send_task.task_id = comboContent->itemData(1).toInt();
		}
	}
	setScoreRecModel(&data);
}
void CScoreRec::setScoreRecModel(DB_FMT *data)
{
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("搜索数据库,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	listScoreRec->setModel(NULL);
	if( SelectRecs(data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data->sql_str,data->sql_error);
		qDebug("查询数据库失败!");
		return;
	}
	progressDlg->setRange(0,data->rownum-1);
	recSourceModel->clear();
	if (flag_simutrainee)
	{
		recSourceModel->setColumnCount(5);  
		recSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
		recSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("任务名称")); 
		recSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("开始时间"));
		recSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("结束时间")); 
		recSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("总成绩"));
	}
	else
	{
		recSourceModel->setColumnCount(7);  
		listScoreRec->setItemDelegateForColumn(5, &spinBoxDelegate);  
		recSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
		recSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("任务名称")); 
		recSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("开始时间"));
		recSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("结束时间")); 
		recSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("自动判分"));
		recSourceModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("教员给分"));
		recSourceModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("总成绩"));
	}
	if (flag_simutrainee)
	{
		for (int i=0;i<data->rownum;i++)
		{
			QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].userinfo_longname)); 
			item0->setData(QVariant(data_send_task[i].user_id));

			QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].task_name));  
			item1->setData(QVariant(data_send_task[i].task_id));
			QStandardItem* item2 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].start_time));  
			item2->setData(QVariant(data_send_task[i].send_task_id));
			QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].end_time)); 
			QStandardItem* item4 = new QStandardItem(QString::number(data_send_task[i].score+data_send_task[i].tutor_score)); 
			QString str1 = QString::number(data_send_task[i].tutor_score);
			QString str2 = QString::number(data_send_task[i].score+data_send_task[i].tutor_score);
			item3->setData(QVariant(str1));
			item4->setData(QVariant(str2));
			item0->setTextAlignment(Qt::AlignCenter);
			item1->setTextAlignment(Qt::AlignCenter);
			item2->setTextAlignment(Qt::AlignCenter);
			item3->setTextAlignment(Qt::AlignCenter);
			item4->setTextAlignment(Qt::AlignCenter);
			QList<QStandardItem*> items;  
			items << item0 << item1 << item2 << item3 << item4; 
			recSourceModel->insertRow(0,items); 
			progressDlg->setValue(i); 
			if(progressDlg->wasCanceled())  
				break;
		}
	}
	else
	{
		for (int i=0;i<data->rownum;i++)
		{
			QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].userinfo_longname)); 
			item0->setData(QVariant(data_send_task[i].user_id));

			QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].task_name));  
			item1->setData(QVariant(data_send_task[i].task_id));
			QStandardItem* item2 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].start_time));  
			item2->setData(QVariant(data_send_task[i].send_task_id));
			QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(data_send_task[i].end_time)); 
			QStandardItem* item4 = new QStandardItem(QString::number(data_send_task[i].score)); 
			QStandardItem* item5 = new QStandardItem(QString::number(data_send_task[i].tutor_score)); 
			item5->setEditable(true);
			QStandardItem* item6 = new QStandardItem(QString::number(data_send_task[i].score+data_send_task[i].tutor_score)); 
			item1->setEditable(false);
			item2->setEditable(false);
			item3->setEditable(false);
			item4->setEditable(false);
			item0->setEditable(false);
			item6->setEditable(false);
			QString str1 = QString::number(data_send_task[i].tutor_score);
			QString str2 = QString::number(data_send_task[i].score+data_send_task[i].tutor_score);
			item3->setData(QVariant(str1));
			item4->setData(QVariant(str2));
			item0->setTextAlignment(Qt::AlignCenter);
			item1->setTextAlignment(Qt::AlignCenter);
			item2->setTextAlignment(Qt::AlignCenter);
			item3->setTextAlignment(Qt::AlignCenter);
			item4->setTextAlignment(Qt::AlignCenter);
			item5->setTextAlignment(Qt::AlignCenter);
			item6->setTextAlignment(Qt::AlignCenter);
			QList<QStandardItem*> items;  
			items << item0 << item1 << item2 << item3 << item4<< item5 << item6; 
			recSourceModel->insertRow(0,items); 
			progressDlg->setValue(i); 
			if(progressDlg->wasCanceled())  
				break;
		}
	}	
	listScoreRec->setModel(recModel);
	selectionModel = listScoreRec->selectionModel();
	listScoreRec->setColumnWidth(0,80);
	listScoreRec->setColumnWidth(1,390);
	listScoreRec->setColumnWidth(2,140);
	listScoreRec->setColumnWidth(3,140);
	listScoreRec->setColumnWidth(4,80);
	if (!flag_simutrainee)
	{
		listScoreRec->setColumnWidth(5,80);
		listScoreRec->setColumnWidth(6,80);
	}
	comboMode->setCurrentIndex(0);
	dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());
	dateTimeEditStart->setDateTime(QDateTime::currentDateTime().addSecs(-7200));
}
void CScoreRec::exportToExcel()
{
	if (flag_simutrainee)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("学员没有导出成绩权限!"),QMessageBox::Ok);
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.xls)"));
	if (fileName.isEmpty())
		return;
	ExcelObjectODBC excel(fileName);
	excel.AddHeader(0, "用户名", "char(10)");
	excel.AddHeader(1, "任务名称", "char(200)");
	excel.AddHeader(2, "开始时间", "char(22)");
	excel.AddHeader(3, "结束时间", "char(22)");
	excel.AddHeader(4, "成绩", "char(6)");
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
		progressDlg->setRange(0,listScoreRec->model()->rowCount());  
		connect(&excel, SIGNAL(exportedRowCount(int)),progressDlg,SLOT(setValue(int)));
		connect(progressDlg, SIGNAL(canceled()),&excel,SLOT(cancel()));
		int retVal = excel.ExportTableViewToExcel(listScoreRec);
		if( retVal > 0)
		{
			progressDlg->cancel();
			QMessageBox::information(this, QString::fromLocal8Bit("完成"),
				QString(QString::fromLocal8Bit("共导出%1条记录")).arg(retVal));
		}
	}
}

void CScoreRec::on_subscore()
{
	listScoreRec->setSelectionBehavior(QAbstractItemView::SelectRows);	
	if (flag_simutrainee&&shm_ots_addr->simu_mode!=0)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("非学习模式学员不可查询成绩得分详情!"),QMessageBox::Ok);
		return;
	}
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() == 0)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("请选择学员!"),QMessageBox::Ok);
		return;
	}
	QStandardItem *item0 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(indexes.at(0).row(),0)));
	int user_id = item0->data().toInt();
	QStandardItem *item1 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(indexes.at(0).row(),1)));
	int task_id = item1->data().toInt();
	QStandardItem *item2 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(indexes.at(0).row(),2)));
	int send_task_id = item2->data().toInt();
	QStandardItem *item3 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(indexes.at(0).row(),3)));//Tutor_SC	
	QStandardItem *item4 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(indexes.at(0).row(),4)));//ToTal_SC	
	QString temp1 = item3->data().toString();
	float tutorscore,score;
	tutorscore = temp1.toFloat();
	QString temp2  = item4->data().toString();
	score = temp2.toFloat();
	if (flag_langueage == 0)
	{
		m_SubScore->loadChinese();
	}
	else if (flag_langueage == 1)
	{
		m_SubScore->loadEnglish();
	}
	m_SubScore->setScoreInfo(send_task_id,score,tutorscore);
	if (m_SubScore->showable)
	{
		m_SubScore->show();
	}
}
void CScoreRec::on_setscore()
{
	//return;
	if (recSourceModel->rowCount()<1)
	{
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_SEND_TASK);
	data.cond_flag.flag_send_task.send_task_id =TRUE;
	data.flag.flag_send_task.tutor_score = TRUE;
	for(int i = 0 ; i <recSourceModel->rowCount(); i++)
	{
		QStandardItem *item = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(i,2)));
		int send_task_id = item->data().toInt();
		QStandardItem *item2 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(i,5)));
		//item2 = listScoreRec->itemDelegate
		QString temp1 = item2->text();
		float tutorscore = temp1.toFloat();
		if (tutorscore==float(0))
		{
			continue;
		}
		else
		{
			data.cond_seg.seg_send_task.send_task_id =send_task_id;
			data.seg.seg_send_task.tutor_score = tutorscore;
			if (!UpdateOneRec(&data))
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("更新成绩失败，检查历史库链接!"),QMessageBox::Ok);
				return;
			}
		}
	}

	QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("更新成绩成功!"),QMessageBox::Ok);
	displayScoreRec();
	return;
}
void CScoreRec::on_Actset()
{
	listScoreRec->setEditTriggers(QTableView::DoubleClicked);
	pushButton->setVisible(true);	
	listScoreRec->clearSelection();
	listScoreRec->setSelectionBehavior(QAbstractItemView::SelectColumns);
	listScoreRec->selectColumn(5);
}

void CScoreRec:: on_ActRecd()
{
	listScoreRec->setSelectionBehavior(QAbstractItemView::SelectRows);	
	if (recSourceModel->rowCount()<1)
	{
		return;
	}
	if (flag_simutrainee&&shm_ots_addr->simu_mode!=0)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("非学习模式学员不可查询成绩得分详情!"),QMessageBox::Ok);
		return;
	}
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() == 0)
	{
		QMessageBox().information(this,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("请选择相关任务进行记录查询!"),QMessageBox::Ok);
		return;
	}
	QStandardItem *item0 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(indexes.at(0).row(),0)));
	int user_id = item0->data().toInt();
	QStandardItem *item2 = recSourceModel->itemFromIndex(recModel->mapToSource(recModel->index(indexes.at(0).row(),2)));
	int send_task_id = item2->data().toInt();
	d_his= new CHistoryRecTable();
	d_his->setWindowTitle(QString::fromLocal8Bit("任务操作记录查询"));//（本功能仅可查询今天的任务操作记录）
	d_his->func_minimal();
	d_his->show();		
	d_his->m_send_task_id= send_task_id;
	d_his->setUserId(user_id);
	//d_his->displayTodayRec();
	d_his->displayRec();
}