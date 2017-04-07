#include "setcontest.h"

CSetContestTable::CSetContestTable(QWidget* parent)
	:QWidget(parent)
{
	setupUi(this);
	dlg_created_flag[SETCONTESTDLG] = true;
	this->setAttribute(Qt::WA_DeleteOnClose);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	connect(buttonAddSend,SIGNAL(clicked()),this,SLOT(AddsendContest()));
	connect(buttonContestFilter,SIGNAL(clicked()),this,SLOT(setContestFilter()));
	connect(buttonContestShowAll,SIGNAL(clicked()),this,SLOT(setContestFilter()));
	connect(pushButton_exit,SIGNAL(clicked()),this,SLOT(close()));
	connect(pushButtonStart,SIGNAL(clicked()),this,SLOT(StartContest()));
	connect(pushButtonDel,SIGNAL(clicked()),this,SLOT(DelContest()));
	setControlAtt();
}
CSetContestTable::~CSetContestTable()
{
	dlg_created_flag[SETCONTESTDLG] = false;
}
void CSetContestTable::setControlAtt()
{
	contestModel = new QStandardItemModel;
	sendcontestModel = new QStandardItemModel;
	contestFilterModel = new QSortFilterProxyModel;

	listContest->setSelectionBehavior(QAbstractItemView::SelectRows);
	listContest->setSelectionMode(QAbstractItemView::SingleSelection); 
	listContest->setEditTriggers(QTableWidget::NoEditTriggers);
	
	listUserAndGroups->setSelectionMode(QAbstractItemView::MultiSelection); 
	listUserAndGroups->setEditTriggers(QTableWidget::NoEditTriggers);
	selectionModel = listUserAndGroups->selectionModel();

	listContestToStart->setSelectionMode(QAbstractItemView::MultiSelection); 
	listContestToStart->setEditTriggers(QTableWidget::NoEditTriggers);

	contestFilterModel->setSourceModel(contestModel);
	contestFilterModel->setDynamicSortFilter(true);
	contestFilterModel->setFilterKeyColumn(0);

	listContest->setModel(contestFilterModel);
	contestSelectionModel = listContest->selectionModel();

	listContestToStart->setModel(sendcontestModel);
	sendcontestSelectionModel = listContestToStart->selectionModel();

	current_sendcontest_id = -1;
	current_contest_id = -1;
	current_task_id = -1;
	current_scene_id = -1;
	current_fault_id = -1;
	readConfig();
	spinTimeLimit->setValue(defaultTimeLimit);
}
void CSetContestTable::readConfig()
{
	QString myfile = SimuTutorConfigPath;
	QSettings mySetting(myfile,QSettings::IniFormat);
	mySetting.setIniCodec(QTextCodec::codecForName("GB2312"));
	defaultTimeLimit = mySetting.value("task/timeLimit",60).toInt();
}
void CSetContestTable::showEvent(QShowEvent *event)
{
	updateUsersAndGroups();
	setContestModel();
	setSendContestModel();
	event->accept();
}
void  CSetContestTable::setLanguage(int language)
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
	updateUsersAndGroups();
	setContestModel();
	setSendContestModel();
}
void CSetContestTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CSetContestTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CSetContestTable::updateUsersAndGroups()
{
	listUserAndGroups->clear();
	DB_FMT data;
	InitDbData(&data,TABLE_TRAIN_INFO);
	data.cond_flag.flag_train_info.login_status = TRUE;
	data.cond_seg.seg_train_info.login_status = 1;
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_TRAIN_INFO);
		return;
	}
	for(int i=0;i<data.rownum;i++)	
	{
		if (JudgeUserRole(data_train_info[i].user_id,ROLE_STUDENT) == TRUE)
		{
			int groups_id = GetUserGroupsId(data_train_info[i].user_id);
			if (groups_id > 0||data_train_info[i].simu_mode==2) 
			{
				if (JudgeUserGroupsRole(data_train_info[i].user_id,GROUPS_ROLE_LEADER) == TRUE)
				{
					DB_FMT data;
					InitDbData(&data,TABLE_GROUPS);
					data.cond_flag.flag_groups.groups_id = TRUE;
					data.cond_seg.seg_groups.groups_id = groups_id;
					if( SelectRecs(&data) != TRUE)
					{
						HdbLogWrite(data.sql_str,data.sql_error);
						FreeDbData(&data,TABLE_GROUPS);
						return;
					}
					QListWidgetItem *groupsItem = new QListWidgetItem;
					QString name;
					name = QString::fromLocal8Bit(data_groups[0].name)+QString::fromLocal8Bit("(分组)");
					groupsItem->setText(name);
					groupsItem->setData(2,QVariant(groups_id));
					groupsItem->setData(3,QVariant(data_train_info[i].user_id));
					groupsItem->setData(4,QVariant(data_train_info[i].host_id));
					listUserAndGroups->addItem(groupsItem);
					FreeDbData(&data,TABLE_GROUPS);
				}
			}
		}
	}
	FreeDbData(&data,TABLE_TRAIN_INFO);
}


void CSetContestTable::setContestModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_CONTEST);
	data.cond_flag.flag_contest.open_state = TRUE;
	data.cond_seg.seg_contest.open_state = 1;
	if( SelectRecs(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_CONTEST);
		return;
	}
	contestModel->clear();
	contestModel->setColumnCount(1);  
	for (int i=0;i<data.rownum;i++)
	{
		QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(data_contest[i].name)); 
		item->setData(QVariant(data_contest[i].contest_id));
		item->setData(QVariant(data_contest[i].task_name),2);
		item->setData(QVariant(data_contest[i].scene_name),3);
		item->setData(QVariant(data_contest[i].fault_name),4);
		item->setData(QVariant(data_contest[i].task_id),5);
		item->setData(QVariant(data_contest[i].scene_id),6);
		item->setData(QVariant(data_contest[i].fault_id),7);
		contestModel->appendRow(item); 
	}
	listContest->clearSelection();
	FreeDbData(&data,TABLE_CONTEST);
}

void CSetContestTable::setContestFilter()
{
	QPushButton *button = qobject_cast<QPushButton*>(sender());
	if (button == buttonContestFilter)
	{
		QString str = editContestFilter->text();
		contestFilterModel->setFilterRegExp(QRegExp(str));
	}
	else
	{
		editContestFilter->clear();
		contestFilterModel->setFilterRegExp(QRegExp("/*"));
	}
}
void CSetContestTable::AddsendContest()
{
	if (selectionModel->hasSelection()==false||contestSelectionModel->hasSelection()==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("没有选中用户组或没有选中竞赛!"),QMessageBox::Ok);
	}
	QList<QListWidgetItem *> userlist = listUserAndGroups->selectedItems();
	if (contestSelectionModel->hasSelection()==true)
	{
		QModelIndexList indexes = contestSelectionModel->selectedRows();
		QStandardItem *item = contestModel->itemFromIndex(contestFilterModel->mapToSource(contestFilterModel->index(indexes.at(0).row(),0)));
		current_contest_id = item->data().toInt();
	}
	for (int i=0;i<userlist.size();i++)
	{
		current_group_id = userlist.at(i)->data(2).toInt();
		DB_FMT data;
		int id = GetMaxIdInHdb(TABLE_SEND_CONTEST)+1;
		InitDbData(&data,TABLE_SEND_CONTEST);
		data.flag.flag_send_contest.send_contest_id =TRUE;
		data.flag.flag_send_contest.contest_id =TRUE;
//		data.flag.flag_send_contest.send_time =TRUE;
		data.flag.flag_send_contest.group_id =TRUE;
		data.flag.flag_send_contest.contest_state =TRUE;
		data.flag.flag_send_contest.time_len =TRUE;
		data.seg.seg_send_contest.send_contest_id = id;
		data.seg.seg_send_contest.group_id = current_group_id;
//		GetCurTimeStr(data.seg.seg_send_contest.send_time);
		data.seg.seg_send_contest.contest_id = current_contest_id;
		data.seg.seg_send_contest.contest_state =0;//竞赛执行状态: 0：未启动 1：已下发 2：已接受 3：完成 4：异常完成
		data.seg.seg_send_contest.time_len = spinTimeLimit->value()*60;
		if( InsertOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("向%1发送任务失败!").arg(userlist.at(i)->text()),QMessageBox::Ok);
			FreeDbData(&data,TABLE_SEND_TASK);
			continue;
		}
	}
//	setContest();
}
void CSetContestTable::setContest()
{
	QList<QListWidgetItem *> userlist = listUserAndGroups->selectedItems();
	if (contestSelectionModel->hasSelection()==true)
	{
		QModelIndexList indexes = contestSelectionModel->selectedRows();
		QStandardItem *item = contestModel->itemFromIndex(contestFilterModel->mapToSource(contestFilterModel->index(indexes.at(0).row(),0)));
		current_task_id = item->data().toInt();
	}
	for (int i=0;i<userlist.size();i++)
	{
		current_user_id = userlist.at(i)->data(3).toInt();
		current_host_id = userlist.at(i)->data(4).toInt();
		DB_FMT data;
		int id = GetMaxIdInHdb(TABLE_SEND_TASK)+1;
		InitDbData(&data,TABLE_SEND_TASK);
		data.flag.flag_send_task.send_task_id =TRUE;
		data.flag.flag_send_task.task_id =TRUE;
		data.flag.flag_send_task.send_time =TRUE;
		data.flag.flag_send_task.user_id =TRUE;
		data.flag.flag_send_task.status =TRUE;
		data.flag.flag_send_task.time_len =TRUE;
		data.seg.seg_send_task.send_task_id = id;
		data.seg.seg_send_task.task_id = current_task_id;
		GetCurTimeStr(data.seg.seg_send_task.send_time);
		data.seg.seg_send_task.user_id = current_user_id;
		data.seg.seg_send_task.status =0;
		data.seg.seg_send_task.time_len = spinTimeLimit->value()*60;
		if( InsertOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("向%1发送任务失败!").arg(userlist.at(i)->text()),QMessageBox::Ok);
			FreeDbData(&data,TABLE_SEND_TASK);
			continue;
		}
		FreeDbData(&data,TABLE_SEND_TASK);
		InitDbData(&data,TABLE_TRAIN_INFO);
		data.flag.flag_train_info.send_task_id = TRUE;
		data.flag.flag_train_info.task_id = TRUE;
		data.flag.flag_train_info.task_status = TRUE;
		data.flag.flag_train_info.is_record = TRUE;
		data.flag.flag_train_info.status = TRUE;
		data.seg.seg_train_info.status = 0;
		data.seg.seg_train_info.send_task_id = id;
		data.seg.seg_train_info.task_id = current_task_id;	
		data.seg.seg_train_info.task_status = 1;
		data.cond_flag.flag_train_info.user_id =TRUE;
		data.cond_seg.seg_train_info.user_id = current_user_id;
		data.cond_flag.flag_train_info.host_id = TRUE;
		data.cond_seg.seg_train_info.host_id = current_host_id;
		if( UpdateOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("向%1发送任务失败!").arg(userlist.at(i)->text()),QMessageBox::Ok);
			FreeDbData(&data,TABLE_TRAIN_INFO);
			continue;
		}
		else//////////////////////////////////////////////
		{
			shm_ots_addr->data_task.flag_new_send_task = 1;
			MySleep(500);
			emit sig_tasksetted();
		}//////////////////////////////////////
		UpdateTrainInfoModel();

		FreeDbData(&data,TABLE_TRAIN_INFO);
	}
	//this->close();
	/*int ret = QMessageBox().information(this,QString::fromLocal8Bit("信息!"),QString::fromLocal8Bit("发送任务成功!"),QMessageBox::Ok);
	if (ret == QMessageBox::Ok)
	{
		this->close();
	}*/
}
void CSetContestTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QPoint p(20,20);
	//QTableWidgetItem* pTableWidgetItem=tableWidget->itemAt(tableWidget->mapFromGlobal(QCursor::pos()-p));
	if (contestSelectionModel->hasSelection()==true)
	{
		QModelIndexList indexes = contestSelectionModel->selectedRows();
		QModelIndex indexes2 = listContest->indexAt(listContest->mapFromGlobal(QCursor::pos()));
 		if (indexes2.isValid () )
		{
			QStandardItem *item = contestModel->itemFromIndex(contestFilterModel->mapToSource(contestFilterModel->index(indexes.at(0).row(),0)));
			QString text = QString::fromLocal8Bit("<br>任务名称:%1").arg(item->data(2).toString())
				+QString::fromLocal8Bit("<br>工况名称:%1").arg(item->data(3).toString())
				+QString::fromLocal8Bit("<br>故障名称:%1").arg(item->data(4).toString());
			QRect	m_rect(QCursor::pos().x(),QCursor::pos().y(),300,300);
			QToolTip::showText(QCursor::pos(), text, this,m_rect);
	 	}
	}
}
void CSetContestTable::setSendContestModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SEND_CONTEST);
	data.cond_flag.flag_send_contest.contest_state = TRUE;
	data.cond_seg.seg_send_contest.contest_state = 0;
	if( SelectRecs(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_SEND_CONTEST);
		return;
	}
	sendcontestModel->clear();
	sendcontestModel->setColumnCount(2);  
	for (int i=0;i<data.rownum;i++)
	{
		QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(data_send_contest[i].contest_name)); 
		item->setData(QVariant(data_send_contest[i].send_contest_id));
		item->setData(QVariant(data_send_contest[i].send_time),5);
		item->setData(QVariant(data_send_contest[i].group_id),3);
		item->setData(QVariant(data_send_contest[i].contest_id),4);	
		sendcontestModel->appendRow(item); 
	}
	listContestToStart->clearSelection();
}
void CSetContestTable::StartContest()
{	
	//先更新一下用户组显示
	updateUsersAndGroups();
	////////////////////////SEND CONTEST MODEL中对应关系/////////////////////////////////
// 	item->setData(QVariant(data_send_contest[i].send_contest_id));
// 	item->setData(QVariant(data_send_contest[i].send_time),5);
// 	item->setData(QVariant(data_send_contest[i].group_id),3);
// 	item->setData(QVariant(data_send_contest[i].contest_id),4);	
	/////////////////////////////////////////////////////////////////////////////////
	if (sendcontestSelectionModel->hasSelection()==true)
	{
		QModelIndexList indexes = sendcontestSelectionModel->selectedIndexes () ;
		for (int i  =  0 ; i < indexes.count() ; i ++ )
		{
			QStandardItem *item = sendcontestModel->itemFromIndex(indexes.at(i));
			current_sendcontest_id = item->data().toInt();
			current_contest_id =  item->data(4).toInt();
			DB_FMT data;
			InitDbData(&data,TABLE_SEND_CONTEST);
			data.cond_flag.flag_send_contest.send_contest_id = TRUE;
			data.cond_seg.seg_send_contest.send_contest_id = current_sendcontest_id;
			data.flag.flag_send_contest.contest_state = TRUE;
			data.seg.seg_send_contest.contest_state = 1;
			data.flag.flag_send_contest.send_time = TRUE;
			GetCurTimeStr(data.seg.seg_send_contest.send_time);
			if( UpdateOneRec(&data) != TRUE)
			{		
				HdbLogWrite(data.sql_str,data.sql_error);
				FreeDbData(&data,TABLE_SEND_CONTEST);
				return;
			}

			current_group_id = item->data(3).toInt();
			bool grouploginedin = false;
			for(int j = 0 ; j < listUserAndGroups->count(); j++)
			{
				if (listUserAndGroups->itemAt(j,0)->data(2) == current_group_id)
				{
					current_user_id = listUserAndGroups->itemAt(j,0)->data(3).toInt();
					current_host_id = listUserAndGroups->itemAt(j,0)->data(4).toInt();
					grouploginedin = true;
				}
			}
			if (grouploginedin == false)
			{
				QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("当前竞赛组没有登录!"),QMessageBox::Ok);
				return;
			}
	//////////////////////// CONTEST MODEL中对应关系/////////////////////////////////
// 			item->setData(QVariant(data_contest[i].task_id),5);
// 			item->setData(QVariant(data_contest[i].scene_id),6);
// 			item->setData(QVariant(data_contest[i].fault_id),7);
			for(int j = 0 ; j < contestModel->rowCount(); j++)
			{
				QModelIndex index= contestModel->index(j,0);
				QStandardItem *item = contestModel->itemFromIndex(index);
				current_task_id = item->data(5).toInt();
				current_scene_id =  item->data(6).toInt();
				current_fault_id =  item->data(7).toInt();
			}
			int id = GetMaxIdInHdb(TABLE_SEND_TASK)+1;
			InitDbData(&data,TABLE_SEND_TASK);
			data.flag.flag_send_task.send_task_id =TRUE;
			data.flag.flag_send_task.task_id =TRUE;
			data.flag.flag_send_task.send_time =TRUE;
			data.flag.flag_send_task.user_id =TRUE;
			data.flag.flag_send_task.status =TRUE;
			data.flag.flag_send_task.time_len =TRUE;
			data.seg.seg_send_task.send_task_id = id;
			data.seg.seg_send_task.task_id = current_task_id;
			GetCurTimeStr(data.seg.seg_send_task.send_time);
			data.seg.seg_send_task.user_id = current_user_id;
			data.seg.seg_send_task.status =0;
			data.seg.seg_send_task.time_len = spinTimeLimit->value()*60;
			if( InsertOneRec(&data) != TRUE)
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("发送竞赛失败!"),QMessageBox::Ok);
				FreeDbData(&data,TABLE_SEND_TASK);
				continue;
			}
			FreeDbData(&data,TABLE_SEND_TASK);
			InitDbData(&data,TABLE_TRAIN_INFO);
			data.flag.flag_train_info.send_task_id = TRUE;
			data.flag.flag_train_info.task_id = TRUE;
			data.flag.flag_train_info.task_status = TRUE;
			data.flag.flag_train_info.is_record = TRUE;
			data.flag.flag_train_info.status = TRUE;
			data.seg.seg_train_info.status = 0;
			data.seg.seg_train_info.send_task_id = id;
			data.seg.seg_train_info.task_id = current_task_id;	
			data.seg.seg_train_info.task_status = 1;
			data.cond_flag.flag_train_info.user_id =TRUE;
			data.cond_seg.seg_train_info.user_id = current_user_id;
			data.cond_flag.flag_train_info.host_id = TRUE;
			data.cond_seg.seg_train_info.host_id = current_host_id;
			if( UpdateOneRec(&data) != TRUE)
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("发送竞赛失败!"),QMessageBox::Ok);
				FreeDbData(&data,TABLE_TRAIN_INFO);
				continue;
			}
			else//////////////////////////////////////////////
			{
				shm_ots_addr->data_task.flag_new_send_task = 1;
				MySleep(500);
				//emit sig_tasksetted();
			}//////////////////////////////////////
			UpdateTrainInfoModel();
			FreeDbData(&data,TABLE_TRAIN_INFO);
		}
	}
}
void CSetContestTable::DelContest()
{
	if (sendcontestSelectionModel->hasSelection()==true)
	{
		QModelIndexList indexes = sendcontestSelectionModel->selectedIndexes () ;
		bool test = indexes.isEmpty();
		for (int i  =  0 ; i < indexes.count() ; i ++ )
		{
			QStandardItem *item = sendcontestModel->itemFromIndex(indexes.at(i));
			int del_send_contest_id = item->data().toInt();
			//current_contest_id =  item->data(4).toInt();
			DB_FMT data;
			InitDbData(&data,TABLE_SEND_CONTEST);
			data.cond_flag.flag_send_contest.send_contest_id = TRUE;
			data.cond_seg.seg_send_contest.contest_state = del_send_contest_id;
			data.flag.flag_send_contest.contest_state = TRUE;
			data.seg.seg_send_contest.contest_state = 0;
			if( DeleteOneRec(&data) != TRUE)
			{		
				HdbLogWrite(data.sql_str,data.sql_error);
				FreeDbData(&data,TABLE_SEND_CONTEST);
				continue;
			}
		}
		setSendContestModel();
	}
	else
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请选择要删除的下发竞赛条目!"),QMessageBox::Ok);
	}
}