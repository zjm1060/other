#include "studentinfo.h"
#include "/home/ems/system/ots/SimuTutor/src/login.h"
#include <QMessageBox>

CStudentInfoTable::CStudentInfoTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	setControlAtt();
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	select_status = false;
	max_user_id = 0;
	time_start.setDate(QDate::currentDate());
	time_start.setTime(QTime(0,0,0,0));
	time_end.setDate(QDate::currentDate());
	time_end.setTime(QTime(23,59,59,999));
	popMenu=NULL;
	userSourceModel = new QStandardItemModel;
	userModel = new QSortFilterProxyModel;
	userModel->setSourceModel(userSourceModel);
	userModel->setDynamicSortFilter(true);
	//connect(buttonLookAt, SIGNAL(clicked()), this, SLOT(openStudentInfoLookAtTable()));
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(openStudentInfoAddTable()));
	connect(buttonModify,SIGNAL(clicked()),this,SLOT(openStudentInfoModifyTable()));
	connect(buttonDel,SIGNAL(clicked()),this,SLOT(delUser()));
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(setUserModel()));
	connect(comboFilterType,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(changeNameFilter(const QString &)));
	connect(comboFilter,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(setNameFilter(const QString &)));
	connect(buttonSearch,SIGNAL(clicked()),this,SLOT(filterButtonClicked()));
	connect(buttonShowAll,SIGNAL(clicked()),this,SLOT(filterButtonClicked()));
	connect(buttonGroups,SIGNAL(clicked()),this,SLOT(openGroupsTable()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));
	//	connect(buttonExit,SIGNAL(clicked()),this,SLOT(onclose()));
}
void CStudentInfoTable::showEvent(QShowEvent *event)
{
// 	if (flag_langueage == 0)
// 	{
// 		loadChinese();
// 	}
// 	else if (flag_langueage == 1)
// 	{
// 		loadEnglish();
// 	}
	setUserModel();
	comboFilterType->setCurrentIndex(-1);
	userModel->setFilterRegExp(QRegExp("/*"));
	event->accept();
}
void CStudentInfoTable::setControlAtt()
{
	viewUserInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	viewUserInfo->setSelectionMode(QAbstractItemView::SingleSelection);
	viewUserInfo->setEditTriggers(QTableView::NoEditTriggers);
	viewUserInfo->setSortingEnabled(true);
}
void CStudentInfoTable::loadChinese()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		bool i = translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
		i = true;
	}
	else
		translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	studentinfoaddtable->loadChinese();
// 	studentinfomodifytable->loadChinese();
	//studentinfolookattable->loadChinese();
}
void CStudentInfoTable::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		bool i = translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
		i = true;
	}
	else
		translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	studentinfoaddtable->loadEnglish();
// 	studentinfomodifytable->loadEnglish();
	//studentinfolookattable->loadEnglish();
}
void  CStudentInfoTable::setLanguage(int language)
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
	setUserModel();
	comboFilterType->setCurrentIndex(-1);
	userModel->setFilterRegExp(QRegExp("/*"));
}
void CStudentInfoTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	if(popMenu)//保证同时只存在一个menu，及时释放内存
	{
		delete popMenu;
		popMenu = NULL;
	}
	popMenu = new QMenu(viewUserInfo);
	QAction* actionAddUser= new QAction("添加用户", this);
	QAction* actionModifyUser= new QAction("修改用户", this);
	QAction* actionDelUser = new QAction("删除用户", this);
	QAction* actionResetPwd = new QAction("初始化用户密码",this);
	//QAction* actionLookAtUser= new QAction("查看用户", this);
	connect(actionAddUser,SIGNAL(triggered()),this,SLOT(openStudentInfoAddTable()));
	connect(actionModifyUser,SIGNAL(triggered()),this,SLOT(openStudentInfoModifyTable()));
	connect(actionDelUser,SIGNAL(triggered()),this,SLOT(delUser()));
	connect(actionResetPwd,SIGNAL(triggered()),this,SLOT(resetPwd()));
	//connect(actionLookAtUser,SIGNAL(triggered()),this,SLOT(openStudentInfoLookAtTable()));
	popMenu->addAction(actionAddUser);
	popMenu->addAction(actionModifyUser);
	popMenu->addAction(actionDelUser);
	popMenu->addAction(actionResetPwd);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CStudentInfoTable::resetPwd()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	int row = indexes.at(0).row();
	QStandardItem *item = userSourceModel->itemFromIndex(userModel->mapToSource(userModel->index(row,0)));
	int user_id = item->data().toInt();
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.flag.flag_userinfo.password=TRUE;
	strcpy(data.seg.seg_userinfo.password,"11");
	data.cond_flag.flag_userinfo.user_id=TRUE;
	data.cond_seg.seg_userinfo.user_id = user_id;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	FreeDbData(&data,TABLE_USERINFO);
	userModel->setData(userModel->index(row,3),QVariant("11"));
	QMessageBox().information(this,QString::fromLocal8Bit("信息!"),QString::fromLocal8Bit("初始化密码成功!"),QMessageBox::Ok);
}
void CStudentInfoTable::openGroupsTable()
{
	groupstable = new CGroupsTable(this);
	groupstable->show();
}

void CStudentInfoTable::openStudentInfoLookAtTable()
{
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要查看的用户!"),QMessageBox::Ok);
	}
	else
	{
		//studentinfolookattable = new CStudentInfoLookAtTable(this);
		//connect(studentinfolookattable->comboLoginRecord,SIGNAL(currentIndexChanged(int)),this,SLOT(displayLoginRecord(int)));
		//connect(studentinfolookattable->comboOperateRecord,SIGNAL(currentIndexChanged(int)),this,SLOT(displayOperateRecord(int)));
		//connect(studentinfolookattable->comboAlarmRecord,SIGNAL(currentIndexChanged(int)),this,SLOT(displayAlarmRecord(int)));
		//studentinfolookattable->show();
		//studentinfolookattable->tabWidget->setCurrentIndex(0);
		//studentinfolookattable->comboOperateRecord->clear();
		//studentinfolookattable->comboAlarmRecord->clear();
		//studentinfolookattable->comboLoginRecord->clear();
		DB_FMT data;
		InitDbData(&data,TABLE_TRAIN_INFO);	
		data.cond_flag.flag_train_info.user_id = TRUE;
		QTableWidgetItem* flag_user_id;// = listStudentInfo->item(rows,0);
		data.cond_seg.seg_train_info.user_id = flag_user_id->text().toInt();

		if( SelectRecs(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_TRAIN_INFO);
			return;
		}
		if(data_train_info[0].login_status == 1)
		{
			//studentinfolookattable->radioLogined->setChecked(TRUE);
			//studentinfolookattable->radioUnLogined->setCheckable(FALSE);
		}
		else if(data_train_info[0].login_status == 0)
		{
			//studentinfolookattable->radioUnLogined->setChecked(TRUE);
			//studentinfolookattable->radioLogined->setCheckable(FALSE);
		}
		FreeDbData(&data,TABLE_TRAIN_INFO);

		InitDbData(&data,TABLE_LOGIN_INFO);	
		data.cond_flag.flag_train_info.user_id = TRUE;
		data.cond_seg.seg_train_info.user_id = flag_user_id->text().toInt();
		if( SelectRecs(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_LOGIN_INFO);
			return;
		}
		for (int i=0;i<data.rownum;i++)
		{
			//studentinfolookattable->comboLoginRecord->insertItem(i,QString::number(i+1));
		}
		FreeDbData(&data,TABLE_LOGIN_INFO);
		QTableWidgetItem* flag_user_name;// = listStudentInfo->item(rows,1);
		InitDbData(&data,TABLE_RECORD);
		data.cond_flag.flag_record.user_id = TRUE;
		data.cond_flag.flag_record.type = TRUE;
		data.cond_flag.flag_record.datetime = TRUE;
		data.cond_flag.flag_record.datetime_end = TRUE;
		strcpy(data.cond_seg.seg_record.userinfo_longname,flag_user_name->text().toLocal8Bit().data());
		data.cond_seg.seg_record.type = 0;
		strcpy(data.cond_seg.seg_record.datetime,time_start.toString(QString("yyyy-MM-dd HH:mm:ss")).toLocal8Bit().data());
		strcpy(data.cond_seg.seg_record.datetime_end,time_end.toString(QString("yyyy-MM-dd HH:mm:ss")).toLocal8Bit().data());
		strcpy(data.cond_flag.flag_record.datetime_opr,">=");
		strcpy(data.cond_flag.flag_record.datetime_end_opr,"<=");
		if( SelectRecs(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_RECORD);
			return;
		}
		for (int i=0;i<data.rownum;i++)
		{
			/*studentinfolookattable->comboOperateRecord->insertItem(i,QString::number(i+1));*/
		}
		FreeDbData(&data,TABLE_RECORD);
		InitDbData(&data,TABLE_RECORD);
		data.cond_flag.flag_record.user_id = TRUE;
		data.cond_flag.flag_record.type = TRUE;
		data.cond_flag.flag_record.datetime = TRUE;
		data.cond_flag.flag_record.datetime_end = TRUE;
		strcpy(data.cond_seg.seg_record.userinfo_longname,flag_user_name->text().toLocal8Bit().data());
		data.cond_seg.seg_record.type = 1;
		strcpy(data.cond_seg.seg_record.datetime,time_start.toString(QString("yyyy-MM-dd HH:mm:ss")).toLocal8Bit().data());
		strcpy(data.cond_seg.seg_record.datetime_end,time_end.toString(QString("yyyy-MM-dd HH:mm:ss")).toLocal8Bit().data());
		strcpy(data.cond_flag.flag_record.datetime_opr,">=");
		strcpy(data.cond_flag.flag_record.datetime_end_opr,"<=");
		if( SelectRecs(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_RECORD);
			return;
		}
		for (int i=0;i<data.rownum;i++)
		{
			//studentinfolookattable->comboAlarmRecord->insertItem(i,QString::number(i+1));
		}
		FreeDbData(&data,TABLE_RECORD);
	}
}
void CStudentInfoTable::displayLoginRecord(int index)
{
	DB_FMT data;
	InitDbData(&data,TABLE_LOGIN_INFO);	
	data.cond_flag.flag_train_info.user_id = TRUE;
	QTableWidgetItem* flag_user_id;// = listStudentInfo->item(rows,0);
	data.cond_seg.seg_train_info.user_id = flag_user_id->text().toInt();
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_LOGIN_INFO);
		return;
	}
	FreeDbData(&data,TABLE_LOGIN_INFO);
	//studentinfolookattable->labelLoginTime->setText(QString::fromLocal8Bit(data_login_info[index].login_time));
	//studentinfolookattable->labelLogoutTime->setText(QString::fromLocal8Bit(data_login_info[index].logout_time));
	//studentinfolookattable->labelHostName->setText(QString::fromLocal8Bit(data_login_info[index].host_name));
}
void CStudentInfoTable::displayOperateRecord(int index)
{
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);	
	data.cond_flag.flag_record.user_id = TRUE;
	data.cond_flag.flag_record.type = TRUE;
	QTableWidgetItem* flag_user_name;// = listStudentInfo->item(rows,1);
	strcpy(data.cond_seg.seg_record.userinfo_longname,flag_user_name->text().toLocal8Bit().data());
	data.cond_seg.seg_record.type = 0;
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_RECORD);
		return;
	}
	FreeDbData(&data,TABLE_RECORD);
	//studentinfolookattable->labelOperateTime->setText(QString::fromLocal8Bit(data_record[index].datetime));
	//studentinfolookattable->labelOperateMs->setText(QString::number(data_record[index].ms)+=QString(" ms"));
	//studentinfolookattable->labelStationId_operate->setText(QString::number(data_record[index].station_id));
	//studentinfolookattable->labelDevId_operate->setText(QString::number(data_record[index].dev_id));
	//studentinfolookattable->textOperateEvent->setPlainText(QString::fromLocal8Bit(data_record[index].content));
}
void CStudentInfoTable::displayAlarmRecord(int index)
{
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);	
	data.cond_flag.flag_record.user_id = TRUE;
	data.cond_flag.flag_record.type = TRUE;
	QTableWidgetItem* flag_user_name;// = listStudentInfo->item(rows,1);
	strcpy(data.cond_seg.seg_record.userinfo_longname,flag_user_name->text().toLocal8Bit().data());
	data.cond_seg.seg_record.type = 1;
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_RECORD);
		return;
	}
	FreeDbData(&data,TABLE_RECORD);
	//studentinfolookattable->labelAlarmTime->setText(QString::fromLocal8Bit(data_record[index].datetime));
	//studentinfolookattable->labelAlarmMs->setText(QString::number(data_record[index].ms)+=QString(" ms"));
	//studentinfolookattable->labelStationId_alarm->setText(QString::number(data_record[index].station_id));
	//studentinfolookattable->labelDevId_alarm->setText(QString::number(data_record[index].dev_id));
	//studentinfolookattable->textAlarmEvent->setPlainText(QString::fromLocal8Bit(data_record[index].content));

}
void CStudentInfoTable::openStudentInfoAddTable()
{
	studentinfoaddtable = new CStudentInfoAddTable(this);
	studentinfoaddtable->flag_add=true;
	studentinfoaddtable->buttonApply->setVisible(false);
	studentinfoaddtable->show();
	connect(studentinfoaddtable,SIGNAL(insertRecord()),this,SLOT(addUser()));
}
void CStudentInfoTable::openStudentInfoModifyTable()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() != 0)
	{
		studentinfomodifytable = new CStudentInfoAddTable(this);
		connect(studentinfomodifytable,SIGNAL(modifyRecord()),this,SLOT(modifyUser()));
		int scene_id;
		int row;
		QString logicalname,status,data;
		row = indexes.at(0).row();
		studentinfomodifytable->flag_modify=true;
		studentinfomodifytable->editName->setText(userModel->data(userModel->index(row,0)).toString());
		studentinfomodifytable->editLongName->setText(userModel->data(userModel->index(row,1)).toString());
		QString role = userModel->data(userModel->index(row,2)).toString();
		if(role==QString::fromLocal8Bit("学员"))
		{
			studentinfomodifytable->editRole->setCurrentIndex(0);	
		}
		else if(role==QString::fromLocal8Bit("教员"))
		{
			studentinfomodifytable->editRole->setCurrentIndex(1);
		}
		else if(role==QString::fromLocal8Bit("系统"))
		{
			studentinfomodifytable->editRole->setCurrentIndex(2);
		}
		studentinfomodifytable->editPassword->setText(userModel->data(userModel->index(row,3)).toString());
		QString sex = userModel->data(userModel->index(row,6)).toString();
		if(sex==QString::fromLocal8Bit("男"))
		{
			studentinfomodifytable->editSex->setCurrentIndex(0);	
		}
		else if(sex==QString::fromLocal8Bit("女"))
		{
			studentinfomodifytable->editSex->setCurrentIndex(1);
		}
		studentinfomodifytable->editIdCard->setText(userModel->data(userModel->index(row,7)).toString());
		QString edu = userModel->data(userModel->index(row,8)).toString();
		if(edu==QString::fromLocal8Bit("大学以下"))
		{
			studentinfomodifytable->editEdu->setCurrentIndex(0);	
		}
		else if(edu==QString::fromLocal8Bit("专科"))
		{
			studentinfomodifytable->editEdu->setCurrentIndex(1);
		}
		else if(edu==QString::fromLocal8Bit("本科"))
		{
			studentinfomodifytable->editEdu->setCurrentIndex(2);
		}
		else if(edu==QString::fromLocal8Bit("研究生及以上"))
		{
			studentinfomodifytable->editEdu->setCurrentIndex(3);
		}
		studentinfomodifytable->editTel->setText(userModel->data(userModel->index(row,9)).toString());
		studentinfomodifytable->editMTel->setText(userModel->data(userModel->index(row,10)).toString());
		studentinfomodifytable->editEmail->setText(userModel->data(userModel->index(row,11)).toString());
		studentinfomodifytable->editCompany->setText(userModel->data(userModel->index(row,12)).toString());
		studentinfomodifytable->editDepartment->setText(userModel->data(userModel->index(row,13)).toString());
		studentinfomodifytable->editDuty->setText(userModel->data(userModel->index(row,14)).toString());
		//	dzy
		QString userrange = userModel->data(userModel->index(row,15)).toString();
		if(userrange==QString::fromLocal8Bit("一级"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(0);	
		}
		else if(userrange==QString::fromLocal8Bit("二级"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(1);
		}
		else if(userrange==QString::fromLocal8Bit("三级"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(2);
		}
// 		else if(userrange==QString::fromLocal8Bit("四级"))
// 		{
// 			studentinfomodifytable->comboBox_userRange->setCurrentIndex(3);
// 		}
// 		else if(userrange==QString::fromLocal8Bit("五级"))
// 		{
// 			studentinfomodifytable->comboBox_userRange->setCurrentIndex(4);
// 		}
// 		else if(userrange==QString::fromLocal8Bit("六级"))
// 		{
// 			studentinfomodifytable->comboBox_userRange->setCurrentIndex(5);
// 		}
// 		else if(userrange==QString::fromLocal8Bit("七级"))
// 		{
// 			studentinfomodifytable->comboBox_userRange->setCurrentIndex(6);
// 		}
// 		else if(userrange==QString::fromLocal8Bit("八级"))
// 		{
// 			studentinfomodifytable->comboBox_userRange->setCurrentIndex(7);
// 		}
// 		else if(userrange==QString::fromLocal8Bit("九级"))
// 		{
// 			studentinfomodifytable->comboBox_userRange->setCurrentIndex(8);
// 		}
// 		else if(userrange==QString::fromLocal8Bit("十级"))
// 		{
// 			studentinfomodifytable->comboBox_userRange->setCurrentIndex(9);
// 		}
		QString usertype = userModel->data(userModel->index(row,16)).toString();
		if(usertype==QString::fromLocal8Bit("新学员"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(0);	
		}
		else if(usertype==QString::fromLocal8Bit("初级工"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(1);
		}
		else if(usertype==QString::fromLocal8Bit("中级工"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(2);
		}
		else if(usertype==QString::fromLocal8Bit("高级工"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(3);
		}
		else if(usertype==QString::fromLocal8Bit("技师"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(4);
		}
		else if(usertype==QString::fromLocal8Bit("高级技师"))
		{
			studentinfomodifytable->comboBox_userRange->setCurrentIndex(5);
		}

		QString usergroup = userModel->data(userModel->index(row,17)).toString();
		if (usergroup ==QString::fromLocal8Bit("无分组") )
		{
			studentinfomodifytable->editGroup->setCurrentIndex(0);
		}
		else{
			for(int i=1;i<studentinfomodifytable->editGroup->count();i++ )
			{
				if (studentinfomodifytable->editGroup->itemData(i,2)==usergroup)
				{
					studentinfomodifytable->editGroup->setCurrentIndex(i);
				}
			}
		}

		studentinfomodifytable->setWindowTitle(QString::fromLocal8Bit("修改用户信息"));
		studentinfomodifytable->show();
	}
}

void CStudentInfoTable::addUser()
{
	if(IsUserExist(QString::fromLocal8Bit(studentinfoaddtable->seg.name)) == TRUE)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("该用户已存在!"),QMessageBox::Ok);	
		return;
	}
	DB_FMT data;
	DATA_USERINFO seg;
	int id = GetInexistenceIdInHdb(TABLE_USERINFO);
	InitDbData(&data,TABLE_USERINFO);
	data.flag.flag_userinfo.user_id =TRUE;
	data.flag.flag_userinfo.name=TRUE;
	data.flag.flag_userinfo.longname=TRUE;
	data.flag.flag_userinfo.password=TRUE;
	data.flag.flag_userinfo.role=TRUE;
	data.flag.flag_userinfo.sex=TRUE;
	data.flag.flag_userinfo.edu=TRUE;
	data.flag.flag_userinfo.company=TRUE;
	data.flag.flag_userinfo.department=TRUE;
	data.flag.flag_userinfo.duty=TRUE;
	data.flag.flag_userinfo.email=TRUE;
	data.flag.flag_userinfo.mtel=TRUE;
	data.flag.flag_userinfo.tel=TRUE;
	data.flag.flag_userinfo.idcard=TRUE;
	data.flag.flag_userinfo.privilege=TRUE;
	data.flag.flag_userinfo.groups_id=TRUE;
	data.flag.flag_userinfo.groups_role=TRUE;
	//dzy
	data.flag.flag_userinfo.user_range=TRUE;
	data.flag.flag_userinfo.user_type_id=TRUE;
	//填充表中每个字段的值
	//data.seg.seg_userinfo.user_id = max_user_id+1;
	data.seg.seg_userinfo.user_id = id;
	strcpy(data.seg.seg_userinfo.name,studentinfoaddtable->seg.name);
	strcpy(data.seg.seg_userinfo.longname,studentinfoaddtable->seg.longname);
	strcpy(data.seg.seg_userinfo.password,studentinfoaddtable->seg.password);
	data.seg.seg_userinfo.role = studentinfoaddtable->seg.role;
	data.seg.seg_userinfo.sex= studentinfoaddtable->seg.sex;
	data.seg.seg_userinfo.edu = studentinfoaddtable->seg.edu;
	strcpy(data.seg.seg_userinfo.company,studentinfoaddtable->seg.company);
	strcpy(data.seg.seg_userinfo.department,studentinfoaddtable->seg.department);
	strcpy(data.seg.seg_userinfo.duty,studentinfoaddtable->seg.duty);
	strcpy(data.seg.seg_userinfo.email,studentinfoaddtable->seg.email);
	strcpy(data.seg.seg_userinfo.mtel,studentinfoaddtable->seg.mtel);
	strcpy(data.seg.seg_userinfo.tel,studentinfoaddtable->seg.tel);
	strcpy(data.seg.seg_userinfo.idcard,studentinfoaddtable->seg.idcard);
	strcpy(data.seg.seg_userinfo.privilege,"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");	
	//DZY	
	data.seg.seg_userinfo.user_range =studentinfoaddtable->seg.user_range;
	data.seg.seg_userinfo.user_type_id =studentinfoaddtable->seg.user_type_id;

	data.seg.seg_userinfo.groups_id=-1;
	data.seg.seg_userinfo.groups_role=-1;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.name));
	item0->setData(QVariant(id));
	QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.longname));  
	QStandardItem* item2;
	if (studentinfoaddtable->seg.role==0)
		item2 = new QStandardItem(QString::fromLocal8Bit("学员")); 
	else if (studentinfoaddtable->seg.role==1)
		item2 = new QStandardItem(QString::fromLocal8Bit("教员")); 
	else if (studentinfoaddtable->seg.role==2)
		item2 = new QStandardItem(QString::fromLocal8Bit("系统"));  
	QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.password)); 
	QStandardItem* item4 = new QStandardItem(QString::fromLocal8Bit("无分组"));
	item4->setData(QVariant(0));
	QStandardItem* item5 = new QStandardItem(QString::fromLocal8Bit(""));
	QStandardItem* item6;
	if (studentinfoaddtable->seg.sex==0)
		item6 = new QStandardItem(QString::fromLocal8Bit("男")); 
	else if (studentinfoaddtable->seg.sex==1)
		item6 = new QStandardItem(QString::fromLocal8Bit("女"));
	QStandardItem* item7 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.idcard));
	QStandardItem* item8;
	if (studentinfoaddtable->seg.edu==0)
		item8 = new QStandardItem(QString::fromLocal8Bit("大学以下")); 
	else if (studentinfoaddtable->seg.edu==1)
		item8 = new QStandardItem(QString::fromLocal8Bit("专科")); 
	else if (studentinfoaddtable->seg.edu==2)
		item8 = new QStandardItem(QString::fromLocal8Bit("本科")); 
	else if (studentinfoaddtable->seg.edu==3)
		item8 = new QStandardItem(QString::fromLocal8Bit("研究生及以上")); 
	QStandardItem* item9 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.tel));
	QStandardItem* item10 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.mtel));
	QStandardItem* item11 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.email));
	QStandardItem* item12 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.company));
	QStandardItem* item13 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.department));
	QStandardItem* item14 = new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.duty));
//dzy
	QStandardItem* item15;
	if (studentinfoaddtable->seg.user_range==0)
		item15 = new QStandardItem(QString::fromLocal8Bit("一级")); 
	else if (studentinfoaddtable->seg.user_range==1)
		item15 = new QStandardItem(QString::fromLocal8Bit("二级")); 
	else if (studentinfoaddtable->seg.user_range==2)
		item15 = new QStandardItem(QString::fromLocal8Bit("三级")); 
// 	else if (studentinfoaddtable->seg.user_range==3)
// 		item15 = new QStandardItem(QString::fromLocal8Bit("四级")); 
// 	else if (studentinfoaddtable->seg.user_range==4)
// 		item15 = new QStandardItem(QString::fromLocal8Bit("五级")); 
// 	else if (studentinfoaddtable->seg.user_range==5)
// 		item15 = new QStandardItem(QString::fromLocal8Bit("六级")); 
// 	else if (studentinfoaddtable->seg.user_range==6)
// 		item15 = new QStandardItem(QString::fromLocal8Bit("七级")); 
// 	else if (studentinfoaddtable->seg.user_range==7)
// 		item15 = new QStandardItem(QString::fromLocal8Bit("八级")); 
// 	else if (studentinfoaddtable->seg.user_range==8)
// 		item15 = new QStandardItem(QString::fromLocal8Bit("九级")); 
// 	else if (studentinfoaddtable->seg.user_range==9)
// 		item15 = new QStandardItem(QString::fromLocal8Bit("十级")); 
	QStandardItem* item16;
	if (studentinfoaddtable->seg.user_type_id==0)
		item16 = new QStandardItem(QString::fromLocal8Bit("新员工")); 
	else if (studentinfoaddtable->seg.user_type_id==1)
		item16 = new QStandardItem(QString::fromLocal8Bit("初级工")); 
	else if (studentinfoaddtable->seg.user_type_id==2)
		item16 = new QStandardItem(QString::fromLocal8Bit("中级工")); 
	else if (studentinfoaddtable->seg.user_type_id==3)
		item16 = new QStandardItem(QString::fromLocal8Bit("高级工")); 
	else if (studentinfoaddtable->seg.user_type_id==4)
		item16 = new QStandardItem(QString::fromLocal8Bit("技师")); 
	else if (studentinfoaddtable->seg.user_type_id==5)
		item16 = new QStandardItem(QString::fromLocal8Bit("高级技师")); 
	//////////////////DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
	QStandardItem* item17= new QStandardItem(QString::fromLocal8Bit(studentinfoaddtable->seg.groups_name));

	item0->setTextAlignment(Qt::AlignCenter);
	item1->setTextAlignment(Qt::AlignCenter);
	item2->setTextAlignment(Qt::AlignCenter);
	item3->setTextAlignment(Qt::AlignCenter);
	item4->setTextAlignment(Qt::AlignCenter);
	item5->setTextAlignment(Qt::AlignCenter);
	item6->setTextAlignment(Qt::AlignCenter);
	item7->setTextAlignment(Qt::AlignCenter);
	item8->setTextAlignment(Qt::AlignCenter);
	item9->setTextAlignment(Qt::AlignCenter);
	item10->setTextAlignment(Qt::AlignCenter);
	item11->setTextAlignment(Qt::AlignCenter);
	item12->setTextAlignment(Qt::AlignCenter);
	item13->setTextAlignment(Qt::AlignCenter);
	item14->setTextAlignment(Qt::AlignCenter);
	item15->setTextAlignment(Qt::AlignCenter);
	item16->setTextAlignment(Qt::AlignCenter);
	item17->setTextAlignment(Qt::AlignCenter);
	QList<QStandardItem*> items;  
	items << item0 << item1 << item2 << item3 << item4 << item5 << item6 << item7 << item8 << item9 << item10 << item11 << item12 << item13 << item14 << item15 << item16 << item17; 
	userSourceModel->appendRow(items);
	FreeDbData(&data,TABLE_USERINFO);
}

void CStudentInfoTable::modifyUser()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	int row = indexes.at(0).row();
	QStandardItem *item = userSourceModel->itemFromIndex(userModel->mapToSource(userModel->index(row,0)));
	int user_id = item->data().toInt();
	if (user_id == 0 && loginUser->login_user_id !=0)//system自己可以修改system
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("普通教员无法修改system用户!"),QMessageBox::Ok);
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.flag.flag_userinfo.user_id =FALSE;
	data.flag.flag_userinfo.name=FALSE;
	data.flag.flag_userinfo.longname=TRUE;
	data.flag.flag_userinfo.password=TRUE;
	data.flag.flag_userinfo.role=TRUE;
	data.flag.flag_userinfo.sex=TRUE;
	data.flag.flag_userinfo.edu=TRUE;
	data.flag.flag_userinfo.company=TRUE;
	data.flag.flag_userinfo.department=TRUE;
	data.flag.flag_userinfo.duty=TRUE;
	data.flag.flag_userinfo.email=TRUE;
	data.flag.flag_userinfo.mtel=TRUE;
	data.flag.flag_userinfo.tel=TRUE;
	data.flag.flag_userinfo.idcard=TRUE;
	//DZY 14 04 11
	data.flag.flag_userinfo.user_range=TRUE;
	data.flag.flag_userinfo.user_type_id=TRUE;
	data.flag.flag_userinfo.groups_id=TRUE;

	data.seg.seg_userinfo = studentinfomodifytable->seg;
	data.cond_flag.flag_userinfo.user_id=TRUE;
	//QStandardItem *item = userSourceModel->itemFromIndex(userModel->mapToSource(userModel->index(row,0)));
	//int user_id = item->data().toInt();
	data.cond_seg.seg_userinfo.user_id = user_id;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	userModel->setData(userModel->index(row,0),QVariant(studentinfomodifytable->seg.name));
	userModel->setData(userModel->index(row,1),QVariant(studentinfomodifytable->seg.longname));
	if (studentinfomodifytable->seg.role==0)
		userModel->setData(userModel->index(row,2),QVariant("学员"));
	else if (studentinfomodifytable->seg.role==1)
		userModel->setData(userModel->index(row,2),QVariant("教员"));
	else if (studentinfomodifytable->seg.role==2)
		userModel->setData(userModel->index(row,2),QVariant("系统"));
	userModel->setData(userModel->index(row,3),QVariant(studentinfomodifytable->seg.password));
	if (studentinfomodifytable->seg.sex==0)
		userModel->setData(userModel->index(row,6),QVariant("男"));
	else if (studentinfomodifytable->seg.sex==1)
		userModel->setData(userModel->index(row,6),QVariant("女"));
	userModel->setData(userModel->index(row,7),QVariant(studentinfomodifytable->seg.idcard));
	if (studentinfomodifytable->seg.edu==0)
		userModel->setData(userModel->index(row,8),QVariant("大学以下"));
	else if (studentinfomodifytable->seg.edu==1)
		userModel->setData(userModel->index(row,8),QVariant("专科"));
	else if (studentinfomodifytable->seg.edu==2)
		userModel->setData(userModel->index(row,8),QVariant("本科"));
	else if (studentinfomodifytable->seg.edu==3)
		userModel->setData(userModel->index(row,8),QVariant("研究生及以上"));
	userModel->setData(userModel->index(row,9),QVariant(studentinfomodifytable->seg.tel));
	userModel->setData(userModel->index(row,10),QVariant(studentinfomodifytable->seg.mtel));
	userModel->setData(userModel->index(row,11),QVariant(studentinfomodifytable->seg.email));
	userModel->setData(userModel->index(row,12),QVariant(studentinfomodifytable->seg.company));
	userModel->setData(userModel->index(row,13),QVariant(studentinfomodifytable->seg.department));
	userModel->setData(userModel->index(row,14),QVariant(studentinfomodifytable->seg.duty));
	if (studentinfomodifytable->seg.user_type_id/2==0)
		userModel->setData(userModel->index(row,15),QVariant("一级"));
	else if (studentinfomodifytable->seg.user_type_id/2==1)
		userModel->setData(userModel->index(row,15),QVariant("二级"));
	else if (studentinfomodifytable->seg.user_type_id/2==2)
		userModel->setData(userModel->index(row,15),QVariant("三级"));
// 	else if (studentinfomodifytable->seg.user_range==3)
// 		userModel->setData(userModel->index(row,15),QVariant("四级"));
// 	else if (studentinfomodifytable->seg.user_range==4)
// 		userModel->setData(userModel->index(row,15),QVariant("五级"));
// 	else if (studentinfomodifytable->seg.user_range==5)
// 		userModel->setData(userModel->index(row,15),QVariant("六级"));
// 	else if (studentinfomodifytable->seg.user_range==6)
// 		userModel->setData(userModel->index(row,15),QVariant("七级"));
// 	else if (studentinfomodifytable->seg.user_range==7)
// 		userModel->setData(userModel->index(row,15),QVariant("八级"));
// 	else if (studentinfomodifytable->seg.user_range==8)
// 		userModel->setData(userModel->index(row,15),QVariant("九级"));
// 	else if (studentinfomodifytable->seg.user_range==9)
// 		userModel->setData(userModel->index(row,15),QVariant("十级"));
	if (studentinfomodifytable->seg.user_type_id==0)
		userModel->setData(userModel->index(row,16),QVariant("新员工"));
	else if (studentinfomodifytable->seg.user_type_id==1)
		userModel->setData(userModel->index(row,16),QVariant("初级工"));
	else if (studentinfomodifytable->seg.user_type_id==2)
		userModel->setData(userModel->index(row,16),QVariant("中级工"));
	else if (studentinfomodifytable->seg.user_type_id==3)
		userModel->setData(userModel->index(row,16),QVariant("高级工"));
	else if (studentinfomodifytable->seg.user_type_id==4)
		userModel->setData(userModel->index(row,16),QVariant("技师"));
	else if (studentinfomodifytable->seg.user_type_id==4)
		userModel->setData(userModel->index(row,16),QVariant("高级技师"));

	userModel->setData(userModel->index(row,17),QVariant(studentinfomodifytable->seg.groups_name));

	FreeDbData(&data,TABLE_USERINFO);
}

void CStudentInfoTable::delUser()
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
		QStandardItem *item = userSourceModel->itemFromIndex(userModel->mapToSource(userModel->index(indexes.at(i).row(),0)));
		int user_id = item->data().toInt();
		if (user_id == 0)
		{
			QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("不允许删除system用户!"),QMessageBox::Ok);
			return;
		}
		delUserFromHdb(user_id);
		userModel->removeRow(indexes.at(i).row());
	}
}

void CStudentInfoTable::delUserFromHdb(int user_id)
{
	QString time,ms;
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.cond_flag.flag_userinfo.user_id = TRUE;
	data.cond_seg.seg_userinfo.user_id = user_id;
	if( DeleteOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	FreeDbData(&data,TABLE_USERINFO);
}

void CStudentInfoTable::filterButtonClicked()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	userModel->setFilterKeyColumn(1);
	if(btn == buttonSearch)
	{
		userModel->setFilterRegExp(QRegExp(editName->text(),Qt::CaseInsensitive,QRegExp::FixedString));
	}
	if(btn == buttonShowAll)
	{
		userModel->setFilterRegExp(QRegExp("/*"));
		comboFilterType->setCurrentIndex(-1);
	}
}

void CStudentInfoTable::changeNameFilter(const QString & text)
{
	comboFilter->clear();
	if (text == QString::fromLocal8Bit("角色"))
	{
		comboFilter->addItem(QString::fromLocal8Bit("学员"));
		comboFilter->addItem(QString::fromLocal8Bit("教员"));
		comboFilter->addItem(QString::fromLocal8Bit("系统"));
	}
	if (text == QString::fromLocal8Bit("性别"))
	{
		comboFilter->addItem(QString::fromLocal8Bit("男"));
		comboFilter->addItem(QString::fromLocal8Bit("女"));
	}
	if (text == QString::fromLocal8Bit("组名"))
	{
		DB_FMT data;
		InitDbData(&data,TABLE_GROUPS);
		if( SelectRecs(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_GROUPS);
			qDebug("查询数据库失败!");
			return;
		}
		for (int i=0;i<data.rownum;i++)
		{
			comboFilter->addItem(QString::fromLocal8Bit(data_groups[i].name));
		}
		FreeDbData(&data,TABLE_GROUPS);
	}
	if (text == QString::fromLocal8Bit("工种"))
	{
		DB_FMT data;
		InitDbData(&data,TABLE_USER_TYPE);
		if( SelectRecs(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_GROUPS);
			qDebug("查询数据库失败!");
			return;
		}
		for (int i=0;i<data.rownum;i++)
		{
			comboFilter->addItem(QString::fromLocal8Bit(data_user_type[i].user_type_name));
		}
		FreeDbData(&data,TABLE_GROUPS);
	}
	if (comboFilterType->currentIndex() == -1)
	{
		comboFilter->setCurrentIndex(-1);
	}
}

void CStudentInfoTable::setNameFilter(const QString & text)
{
	if (comboFilterType->currentText() == QString::fromLocal8Bit("角色"))
	{
		userModel->setFilterKeyColumn(2);
		userModel->setFilterRegExp(QRegExp(text));
	}
	if (comboFilterType->currentText() == QString::fromLocal8Bit("性别"))
	{
		userModel->setFilterKeyColumn(6);
		userModel->setFilterRegExp(QRegExp(text));
	}
	if (comboFilterType->currentText() == QString::fromLocal8Bit("组名"))
	{
		userModel->setFilterKeyColumn(4);
		userModel->setFilterRegExp(QRegExp(text));
	}
	if (comboFilterType->currentText() == QString::fromLocal8Bit("工种"))
	{
		userModel->setFilterKeyColumn(16);
		userModel->setFilterRegExp(QRegExp(text));
	}
}

void CStudentInfoTable::setUserModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("搜索数据库,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	viewUserInfo->setModel(NULL);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		qDebug("查询数据库失败!");
		return;
	}
	progressDlg->setRange(0,data.rownum-1);
	userSourceModel->clear();
	userSourceModel->setColumnCount(18);  
	userSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
	userSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("姓名")); 
	userSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("角色"));
	userSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("密码")); 
	userSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("组名"));
	userSourceModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("组角色"));
	userSourceModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("性别"));
	userSourceModel->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("身份证号"));
	userSourceModel->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("学历"));
	userSourceModel->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("办公电话"));
	userSourceModel->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("移动电话"));
	userSourceModel->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("电子邮件"));
	userSourceModel->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("单位"));
	userSourceModel->setHeaderData(13,Qt::Horizontal,QString::fromLocal8Bit("部门"));
	userSourceModel->setHeaderData(14,Qt::Horizontal,QString::fromLocal8Bit("职务"));
	userSourceModel->setHeaderData(15,Qt::Horizontal,QString::fromLocal8Bit("学员级别"));
	userSourceModel->setHeaderData(16,Qt::Horizontal,QString::fromLocal8Bit("学员工种"));
	userSourceModel->setHeaderData(17,Qt::Horizontal,QString::fromLocal8Bit("分组信息"));
	for (int i=0;i<data.rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].name));
		item0->setData(QVariant(data_userinfo[i].user_id));
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].longname));  
		QStandardItem* item2;
		if (data_userinfo[i].role==0)
			item2 = new QStandardItem(QString::fromLocal8Bit("学员")); 
		else if (data_userinfo[i].role==1)
			item2 = new QStandardItem(QString::fromLocal8Bit("教员")); 
		else if (data_userinfo[i].role==2)
			item2 = new QStandardItem(QString::fromLocal8Bit("系统"));  
		else 
			item2 = new QStandardItem(QString::fromLocal8Bit("")); 
		QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].password)); 
		QStandardItem* item4;
		if (data_userinfo[i].groups_id == -1)
			item4 = new QStandardItem(QString::fromLocal8Bit("无分组"));
		else
			item4 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].groups_name));
		item4->setData(QVariant(data_userinfo[i].groups_id));
		QStandardItem* item5;
		if (data_userinfo[i].groups_id == -1||data_userinfo[i].groups_id == 0)
		{
			item5 = new QStandardItem(QString::fromLocal8Bit("")); 
		}
		else if (data_userinfo[i].groups_id > 0)
		{
			if (data_userinfo[i].groups_role==0)
				item5 = new QStandardItem(QString::fromLocal8Bit("组员")); 
			else if (data_userinfo[i].groups_role==1)
				item5 = new QStandardItem(QString::fromLocal8Bit("组长"));
		}
		else 
			item5 = new QStandardItem(QString::fromLocal8Bit("")); 
		QStandardItem* item6;
		if (data_userinfo[i].sex==0)
			item6 = new QStandardItem(QString::fromLocal8Bit("男")); 
		else if (data_userinfo[i].sex==1)
			item6 = new QStandardItem(QString::fromLocal8Bit("女"));
		else 
			item6 = new QStandardItem(QString::fromLocal8Bit("")); 
		QStandardItem* item7 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].idcard));
		QStandardItem* item8;
		if (data_userinfo[i].edu==0)
			item8 = new QStandardItem(QString::fromLocal8Bit("大学以下")); 
		else if (data_userinfo[i].edu==1)
			item8 = new QStandardItem(QString::fromLocal8Bit("专科")); 
		else if (data_userinfo[i].edu==2)
			item8 = new QStandardItem(QString::fromLocal8Bit("本科")); 
		else if (data_userinfo[i].edu==3)
			item8 = new QStandardItem(QString::fromLocal8Bit("研究生及以上")); 
		else 
			item8 = new QStandardItem(QString::fromLocal8Bit("")); 
		QStandardItem* item9 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].tel));
		QStandardItem* item10 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].mtel));
		QStandardItem* item11 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].email));
		QStandardItem* item12 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].company));
		QStandardItem* item13 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].department));
		QStandardItem* item14 = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].duty));
		
		QStandardItem* item15;
		if (data_userinfo[i].user_type_id/2==0)
			item15 = new QStandardItem(QString::fromLocal8Bit("一级")); 
		else if (data_userinfo[i].user_type_id/2==1)
			item15 = new QStandardItem(QString::fromLocal8Bit("二级")); 
		else if (data_userinfo[i].user_type_id/2==2)
			item15 = new QStandardItem(QString::fromLocal8Bit("三级")); 
		else 
			item15 = new QStandardItem(QString::fromLocal8Bit("")); 
		QStandardItem* item16;
		if (data_userinfo[i].user_type_id==0)
			item16 = new QStandardItem(QString::fromLocal8Bit("新员工")); 
		else if (data_userinfo[i].user_type_id==1)
			item16 = new QStandardItem(QString::fromLocal8Bit("初级工")); 
		else if (data_userinfo[i].user_type_id==2)
			item16 = new QStandardItem(QString::fromLocal8Bit("中级工")); 
		else if (data_userinfo[i].user_type_id==3)
			item16 = new QStandardItem(QString::fromLocal8Bit("高级工")); 
		else if (data_userinfo[i].user_type_id==4)
			item16 = new QStandardItem(QString::fromLocal8Bit("技师")); 
		else if (data_userinfo[i].user_type_id==5)
			item16 = new QStandardItem(QString::fromLocal8Bit("高级技师")); 
		else 
			item16 = new QStandardItem(QString::fromLocal8Bit("")); 
		QStandardItem* item17= new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].groups_name));

		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		item3->setTextAlignment(Qt::AlignCenter);
		item4->setTextAlignment(Qt::AlignCenter);
		item5->setTextAlignment(Qt::AlignCenter);
		item6->setTextAlignment(Qt::AlignCenter);
		item7->setTextAlignment(Qt::AlignCenter);
		item8->setTextAlignment(Qt::AlignCenter);
		item9->setTextAlignment(Qt::AlignCenter);
		item10->setTextAlignment(Qt::AlignCenter);
		item11->setTextAlignment(Qt::AlignCenter);
		item12->setTextAlignment(Qt::AlignCenter);
		item13->setTextAlignment(Qt::AlignCenter);
		item14->setTextAlignment(Qt::AlignCenter);
		item15->setTextAlignment(Qt::AlignCenter);
		item16->setTextAlignment(Qt::AlignCenter);
		item17->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2 << item3 << item4 << item5 << item6 << item7 << item8 << item9 << item10 << item11 << item12 << item13 << item14<< item15 << item16<< item17; 
		userSourceModel->appendRow(items); 
		progressDlg->setValue(i); 
		if(progressDlg->wasCanceled())  
			break;
	}
	viewUserInfo->setModel(userModel);
	viewUserInfo->hideColumn(3);
	selectionModel = viewUserInfo->selectionModel();
	viewUserInfo->resizeColumnsToContents();
	FreeDbData(&data,TABLE_USERINFO);
}
/*************************************************CStudentInfoAddTable****************************************************/
CStudentInfoAddTable::CStudentInfoAddTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	DB_FMT data;
	InitDbData(&data,TABLE_GROUPS);
	if( SelectRecs(&data) != TRUE)
	{	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS);
	}
	for (int i=0;i<data.rownum;i++)
	{
		if (data_groups[i].groups_id != -1 /*&& data_groups[i].groups_id != 0*/)
		{
			QListWidgetItem *groupsItem = new QListWidgetItem;
			groupsItem->setText(QString::fromLocal8Bit(data_groups[i].name));
			groupsItem->setData(2,QVariant(data_groups[i].name));
			groupsItem->setData(3,QVariant(data_groups[i].groups_id));
			editGroup->addItem(groupsItem->text(),groupsItem->data(3));
			int p =groupsItem->data(3).toInt();
		//	p++;
		}
	}
	FreeDbData(&data,TABLE_GROUPS);

	this->setAttribute(Qt::WA_DeleteOnClose);
	flag_add=false;
	flag_modify=false;
	setControlAtt();
	this->setModal(true);

	
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(comboBox_userType,SIGNAL(currentIndexChanged(int)),this,SLOT(range_setCurrentIndex(int)));
}
void CStudentInfoAddTable::setControlAtt()
{
	editPassword->setEchoMode(QLineEdit::Password);
	//editSex->setCurrentIndex(-1);
	//editEdu->setCurrentIndex(-1);
	//editRole->setCurrentIndex(-1);
	QRegExp rx_idcard("^[0-9]*[1-9][0-9]*[A-Za-z]+$");
	QValidator *validator_idcard = new QRegExpValidator(rx_idcard, this);
	editIdCard->setValidator(validator_idcard);
	QRegExp rx_tel("^[0-9]*[1-9][0-9]*$");
	QValidator *validator_tel = new QRegExpValidator(rx_tel, this);
	editTel->setValidator(validator_tel);
	QRegExp rx_mtel("^[0-9]*[1-9][0-9]*$");
	QValidator *validator_mtel = new QRegExpValidator(rx_mtel, this);
	editMTel->setValidator(validator_mtel);
}
void CStudentInfoAddTable::loadChinese()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		bool i = translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
		i = true;
	}
	else
		translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CStudentInfoAddTable::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		bool i = translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
		i = true;
	}
	else
		translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CStudentInfoAddTable::showEvent(QShowEvent *event)
{
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	event->accept();
}
void  CStudentInfoAddTable::setLanguage(int language)
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
int CStudentInfoAddTable::apply()
{		
	QString strRole,strSex,strEdu,strU_type,strU_range;
	if (editName->text().isEmpty())
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("用户名不能为空!"),QMessageBox::Ok);	
		return -1;
	}
	strcpy(seg.name,editName->text().toLocal8Bit().data());
	//dzy new added   14/07/04
 	if(IsUserExist(QString::fromLocal8Bit(seg.name)) == TRUE&&flag_add)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("该用户已存在!"),QMessageBox::Ok);	
		return -1;
	}//
	if (editLongName->text().isEmpty())
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("姓名不能为空!"),QMessageBox::Ok);	
		return -1;
	}
	strcpy(seg.longname,editLongName->text().toLocal8Bit().data());
	strRole = editRole->currentText();
	if (strRole==QString::fromLocal8Bit("学员"))
	{
		seg.role = 0;
	}
	else if (strRole==QString::fromLocal8Bit("教员"))
	{
		seg.role = 1;
	}
	else if (strRole==QString::fromLocal8Bit("系统"))
	{
		seg.role = 2;
	}
	else
	{
		seg.role = -1;
	}
	strSex = editSex->currentText();
	if (strSex==QString::fromLocal8Bit("男"))
	{
		seg.sex = 0;
	}
	else if (strSex==QString::fromLocal8Bit("女"))
	{
		seg.sex = 1;
	}
	else
	{
		seg.sex = -1;
	}
	if (editPassword->text().isEmpty())
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("密码不能为空!"),QMessageBox::Ok);	
		return -1;
	}
	strcpy(seg.password,editPassword->text().toLocal8Bit().data());
	if (editIdCard->text().isEmpty())
		strcpy(seg.idcard,"");
	else
		strcpy(seg.idcard,editIdCard->text().toLocal8Bit().data());
	strEdu = editEdu->currentText();
	if (strEdu==QString::fromLocal8Bit("大学以下"))
	{
		seg.edu = 0;
	}
	else if(strEdu==QString::fromLocal8Bit("专科"))
	{
		seg.edu = 1;
	}
	else if(strEdu==QString::fromLocal8Bit("本科"))
	{
		seg.edu = 2;
	}
	else if(strEdu==QString::fromLocal8Bit("研究生及以上"))
	{
		seg.edu = 3;
	}
	else
	{
		seg.edu = -1;
	}
	

	if (editTel->text().isEmpty())
		strcpy(seg.tel,"");
	else
		strcpy(seg.tel,editTel->text().toLocal8Bit().data());
	if (editMTel->text().isEmpty())
		strcpy(seg.mtel,"");
	else
		strcpy(seg.mtel,editMTel->text().toLocal8Bit().data());
	if (editEmail->text().isEmpty())
		strcpy(seg.email,"");
	else
		strcpy(seg.email,editEmail->text().toLocal8Bit().data());
	if (editCompany->text().isEmpty())
		strcpy(seg.company,"");
	else
		strcpy(seg.company,editCompany->text().toLocal8Bit().data());
	if (editDepartment->text().isEmpty())
		strcpy(seg.department,"");
	else
		strcpy(seg.department,editDepartment->text().toLocal8Bit().data());
	if (editDuty->text().isEmpty())
		strcpy(seg.duty,"");
	else
		strcpy(seg.duty,editDuty->text().toLocal8Bit().data());
	//DZY
	seg.user_range = comboBox_userRange->currentIndex();
	seg.user_type_id = comboBox_userType->currentIndex();
	if(editGroup->currentText ().isEmpty()||editGroup->currentText () == QString::fromLocal8Bit("请选择"))
	{
		strcpy(seg.groups_name,"无分组");
		seg.groups_id = -1;
	}
	else
	{
		strcpy(seg.groups_name,editGroup->currentText ().toLocal8Bit().data());
		DB_FMT data;
		InitDbData(&data,TABLE_GROUPS);
		data.cond_flag.flag_groups.name = TRUE;
		strcpy(data.cond_seg.seg_groups.name,seg.groups_name);
		if( SelectRecs(&data) != TRUE)
		{	
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_GROUPS);
		}
		else
			seg.groups_id = data_groups[0].groups_id;
// 		int p = editGroup->currentIndex();
// 		QVariant v =  editGroup->itemData(p,3);
// 		seg.groups_id =v.toInt();
	}
		
	//
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
	return 1;
}
void CStudentInfoAddTable::enter()
{
	if (apply() == 1)
	{
		editName->clear();
		editLongName->clear();
		editRole->setCurrentIndex(0);
		editSex->setCurrentIndex(0);
		editPassword->clear();
		editIdCard->clear();
		editEdu->setCurrentIndex(0);
		editTel->clear();
		editMTel->clear();
		editEmail->clear();
		editCompany->clear();
		editDepartment->clear();
		editDuty->clear();
		//DZY
		comboBox_userRange->setCurrentIndex(0);
		comboBox_userType->setCurrentIndex(0);
		editGroup->setCurrentIndex(0);

		this->close();
		flag_add=false;
		flag_modify=false;
	}
			this->close();
}
void CStudentInfoAddTable::range_setCurrentIndex(int i)
{
	comboBox_userRange->setCurrentIndex(i/2);
}
/***********************************************CGroupsTable*************************************************************/
CGroupsTable::CGroupsTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	menuNoGroupsUser=NULL;
	this->setModal(true);
	creatGroupAddTable();
	creatNoGroupUserWidget();
	menuNoGroupsUser = new QMenu(this);
	actionGroupsHead = new QAction(QString::fromLocal8Bit("设置为组长"), this);
	actionDelUser = new QAction(QString::fromLocal8Bit("删除学员"), this);
	actionAddAllNoGroupsUser = new QAction(QString::fromLocal8Bit("添加所有未分组学员"), this);
	listGroups->setSelectionBehavior(QAbstractItemView::SelectRows);
	listGroups->setSelectionMode(QAbstractItemView::SingleSelection);
	listGroups->setEditTriggers(QTableView::NoEditTriggers);
	listGroupsUser->setSelectionBehavior(QAbstractItemView::SelectRows);
	listGroupsUser->setSelectionMode(QAbstractItemView::ExtendedSelection);
	listGroupsUser->setEditTriggers(QTableView::NoEditTriggers);
	listGroupsUser->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(listGroups,SIGNAL(currentItemChanged(QListWidgetItem *,QListWidgetItem *)),this,SLOT(showGroupsUsers(QListWidgetItem *)));
	connect(buttonAddGroups,SIGNAL(clicked()),this,SLOT(openAddGroupsTable()));
	connect(buttonModifyGroups,SIGNAL(clicked()),this,SLOT(openModifyGroupsTable()));
	connect(buttonDelGroups,SIGNAL(clicked()),this,SLOT(delGroups()));
	connect(buttonDelUser,SIGNAL(clicked()),this,SLOT(delGroupsUser()));
	connect(actionDelUser,SIGNAL(triggered()),this,SLOT(delGroupsUser()));
	connect(buttonSetGroupsHead,SIGNAL(clicked()),this,SLOT(setGroupsHead()));
	connect(actionGroupsHead,SIGNAL(triggered()),this,SLOT(setGroupsHead()));
	connect(actionAddAllNoGroupsUser,SIGNAL(triggered()),this,SLOT(addAllNoGroupsUser()));
	connect(listGroupsUser,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(menuListGroupUser(const QPoint &)));
	connect(buttonNoGroupsUser,SIGNAL(clicked()),this,SLOT(openNoGroupUser()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));	
}
void CGroupsTable::showEvent(QShowEvent *event)
{
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	displayGroups();
	updateNoGroupsUserMenu();
	//buttonNoGroupsUser->setMenu(menuNoGroupsUser);
	listGroups->setCurrentRow(0);
	event->accept();
}
void  CGroupsTable::setLanguage(int language)
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
void CGroupsTable::loadChinese()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		bool i = translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
		i = true;
	}
	else
		translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CGroupsTable::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		bool i = translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
		i = true;
	}
	else
		translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CGroupsTable::menuListGroupUser(const QPoint &pos)
{
	QMenu* popMenu = new QMenu(this);
	QMenu* groupsMenu = new QMenu(popMenu);
	QAction *actionChgGroups = popMenu->addMenu(groupsMenu);
	actionChgGroups->setText(QString::fromLocal8Bit("移动学员至"));
	popMenu->addAction(actionGroupsHead);
	popMenu->addAction(actionDelUser);
	popMenu->addAction(actionChgGroups);
	popMenu->addAction(actionAddAllNoGroupsUser);
	DB_FMT data;
	InitDbData(&data,TABLE_GROUPS);
	if( SelectRecs(&data) != TRUE)
	{	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS);
		return;
	}
	for (int i=0;i<data.rownum;i++)
	{
		if (data_groups[i].groups_id != -1 && data_groups[i].groups_id != 0)
		{
			QAction *actionName = new QAction(QString::fromLocal8Bit(data_groups[i].name),this);
			actionName->setData(QVariant(data_groups[i].groups_id));
			groupsMenu->addAction(actionName);
			connect(actionName,SIGNAL(triggered()),this,SLOT(chgUserGroups()));
		}
	}
	FreeDbData(&data,TABLE_GROUPS);
	
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CGroupsTable::creatGroupAddTable()
{
	groupAddTable = new QDialog(this);
	//groupAddTable->setAttribute(Qt::WA_DeleteOnClose);
	groupAddTable->setModal(true);
	btnEnter = new QPushButton(groupAddTable);
	btnExit = new QPushButton(groupAddTable);
	lineEdit = new QLineEdit(groupAddTable);
	QGridLayout *gridLayout = new QGridLayout(groupAddTable);
	btnEnter->setText(QString::fromLocal8Bit("确定"));
	btnExit->setText(QString::fromLocal8Bit("取消"));
	gridLayout->addWidget(lineEdit, 0, 0, 1, 2);
	gridLayout->addWidget(btnEnter, 1, 0, 1, 1);
	gridLayout->addWidget(btnExit, 1, 1, 1, 1);
	groupAddTable->setLayout(gridLayout);
	connect(btnExit,SIGNAL(clicked()),groupAddTable,SLOT(close()));
}
void CGroupsTable::creatNoGroupUserWidget()
{
	noGroupsUserSourceModel = new QStandardItemModel;
	noGroupsUserModel = new QSortFilterProxyModel;
	widgetNoGroupsUser = new QWidget(this,Qt::Popup);
	QListView *listUser = new QListView(widgetNoGroupsUser);
	QLineEdit *editName = new QLineEdit(widgetNoGroupsUser);
	QVBoxLayout *layout = new QVBoxLayout(widgetNoGroupsUser);
	listUser->setSelectionMode(QAbstractItemView::SingleSelection); 
	listUser->setEditTriggers(QTableWidget::NoEditTriggers);
	listUser->setModel(noGroupsUserModel);
	noGroupsUserModel->setSourceModel(noGroupsUserSourceModel);
	noGroupsUserModel->setDynamicSortFilter(true);
	layout->addWidget(editName);
	layout->addWidget(listUser);
	widgetNoGroupsUser->setLayout(layout);
	connect(editName,SIGNAL(textChanged(const QString &)),this,SLOT(filterNoGroupUser(const QString &)));
	connect(listUser,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(addGroupsUser(const QModelIndex &)));
}
void CGroupsTable::openAddGroupsTable()
{
	lineEdit->clear();
	groupAddTable->setWindowTitle(QString::fromLocal8Bit("添加"));
	groupAddTable->show();
	btnEnter->disconnect();
	connect(btnEnter,SIGNAL(clicked()),this,SLOT(addGroups()));
}
void CGroupsTable::openModifyGroupsTable()
{
	if (listGroups->currentRow()==-1)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("没有选中任何分组!"),QMessageBox::Ok);	
		return;
	}
	groupAddTable->setWindowTitle(QString::fromLocal8Bit("修改"));
	groupAddTable->show();
	lineEdit->setText(listGroups->currentItem()->text());
	btnEnter->disconnect();
	connect(btnEnter,SIGNAL(clicked()),this,SLOT(modifyGroups()));
}
void CGroupsTable::openNoGroupUser()
{
	widgetNoGroupsUser->show();
	widgetNoGroupsUser->resize(200,height()+35);
	QPoint pt=pos();
	int x = pt.x()+width();
	widgetNoGroupsUser->move(x+16,pt.y());
}
void CGroupsTable::filterNoGroupUser(const QString &text)
{
	noGroupsUserModel->setFilterRegExp(QRegExp(text));
}
void CGroupsTable::addGroups()
{
	groupAddTable->close();
	QString groupsName=lineEdit->text();
	if (groupsName.isEmpty())
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("分组名称不能为空!"),QMessageBox::Ok);	
		return;
	}
	DB_FMT data;
	int id = GetInexistenceIdInHdb(TABLE_GROUPS);
	InitDbData(&data,TABLE_GROUPS);
	//设置表中每个字段的flag
	data.flag.flag_groups.groups_id =TRUE;
	data.flag.flag_groups.name=TRUE;

	//填充表中每个字段的值
	data.seg.seg_groups.groups_id = id;
	strcpy(data.seg.seg_groups.name,groupsName.toLocal8Bit().data());

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS);
		return;
	}
	QListWidgetItem *groupsItem = new QListWidgetItem;
	groupsItem->setText(groupsName);
	groupsItem->setData(3,QVariant(id));
	listGroups->addItem(groupsItem);
	FreeDbData(&data,TABLE_GROUPS);
}
void CGroupsTable::delGroups()
{
	if (listGroups->count()!=0)
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
		if (listGroupsUser->count()!=0)//如果分组下有用户，先删除用户
		{
			QMessageBox msgBox(this);
			msgBox.setIcon(QMessageBox::Information);
			msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
			msgBox.setInformativeText(QString::fromLocal8Bit("分组下有用户，如果删除该分组，该分组下的用户分组信息也将清除，是否继续？"));
			msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Ok);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) 
			{
				return;
			}
			for (int i=0;i<listGroupsUser->count();i++)
			{
				DB_FMT data;
				InitDbData(&data,TABLE_USERINFO);
				data.flag.flag_userinfo.groups_id=TRUE;
				data.flag.flag_userinfo.groups_role=TRUE;
				data.seg.seg_userinfo.groups_id = -1;
				data.seg.seg_userinfo.groups_role = -1;

				data.cond_flag.flag_userinfo.user_id=TRUE;
				data.cond_seg.seg_userinfo.user_id = listGroupsUser->item(i)->data(3).toInt();
				if( UpdateOneRec(&data) != TRUE)
				{
					HdbLogWrite(data.sql_str,data.sql_error);
					FreeDbData(&data,TABLE_USERINFO);
					return;
				}
				FreeDbData(&data,TABLE_USERINFO);
			}
			listGroupsUser->clear();
			updateNoGroupsUserMenu();
		}
		DB_FMT data;
		InitDbData(&data,TABLE_GROUPS);
		data.cond_flag.flag_groups.groups_id=TRUE;
		data.cond_seg.seg_groups.groups_id = listGroups->currentItem()->data(3).toInt();
		if (data.cond_seg.seg_groups.groups_id == 0 )
		{
			PopupMessageBox(this,"服务器分组不可删除",QMessageBox::Information,false);
			return;
		}
		if( DeleteOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_GROUPS);
			return;
		}
		listGroups->takeItem(listGroups->currentRow());
		FreeDbData(&data,TABLE_GROUPS);
	}
}
void CGroupsTable::modifyGroups()
{
	groupAddTable->close();
	QString groupsName=lineEdit->text();
	if (groupsName.isEmpty())
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("分组名称不能为空!"),QMessageBox::Ok);	
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_GROUPS);

	//设置表中每个字段的flag
	data.flag.flag_groups.name=TRUE;

	//填充表中每个字段的值
	strcpy(data.seg.seg_groups.name,groupsName.toLocal8Bit().data());

	//设置查询条件
	data.cond_flag.flag_groups.groups_id = TRUE;
	data.cond_seg.seg_groups.groups_id = listGroups->currentItem()->data(3).toInt();
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS);
		return;
	} 
	listGroups->currentItem()->setText(groupsName);
	FreeDbData(&data,TABLE_GROUPS);
}
void CGroupsTable::displayGroups()
{
	listGroups->clear();
	DB_FMT data;
	InitDbData(&data,TABLE_GROUPS);
	if( SelectRecs(&data) != TRUE)
	{	
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS);
		return;
	}
	for (int i=0;i<data.rownum;i++)
	{
		if (data_groups[i].groups_id != -1 && data_groups[i].groups_id != 0)
		{
			QListWidgetItem *groupsItem = new QListWidgetItem;
			groupsItem->setText(QString::fromLocal8Bit(data_groups[i].name));
			groupsItem->setData(3,QVariant(data_groups[i].groups_id));
			listGroups->addItem(groupsItem);
		}
	}
	FreeDbData(&data,TABLE_GROUPS);
}
void CGroupsTable::chgUserGroups()
{
	QList<QListWidgetItem *> userItems = listGroupsUser->selectedItems();
	int sel_num = userItems.count();
	if (sel_num == 0)
		return;
	QAction *action = qobject_cast<QAction*>(sender());
	int groups_id = action->data().toInt();
	for (int i=0;i<sel_num;i++)
	{
		DB_FMT data;
		InitDbData(&data,TABLE_USERINFO);
		data.flag.flag_userinfo.groups_id=TRUE;
		data.flag.flag_userinfo.groups_role=TRUE;
		data.seg.seg_userinfo.groups_id = groups_id;
		data.seg.seg_userinfo.groups_role = 0;
		data.cond_flag.flag_userinfo.user_id=TRUE;
		data.cond_seg.seg_userinfo.user_id = userItems.at(i)->data(3).toInt();
		if( UpdateOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_USERINFO);
			return;
		}
		writeGroupsRecord(userItems.at(i)->data(3).toInt(),groups_id,0);
		FreeDbData(&data,TABLE_USERINFO);
	}
	showGroupsUsers(listGroups->currentItem());
}
void CGroupsTable::addGroupsUser(const QModelIndex &index)
{
	if (listGroups->count()==0)
		return;
	//QAction *action = qobject_cast<QAction*>(sender());
	int user_id = noGroupsUserSourceModel->itemFromIndex(noGroupsUserModel->mapToSource(index))->data().toInt();
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.flag.flag_userinfo.groups_id=TRUE;
	data.flag.flag_userinfo.groups_role=TRUE;
	data.seg.seg_userinfo.groups_id = listGroups->currentItem()->data(3).toInt();
	if (listGroupsUser->count()==0)
		data.seg.seg_userinfo.groups_role = 1;
	else
		data.seg.seg_userinfo.groups_role = 0;
	data.cond_flag.flag_userinfo.user_id=TRUE;
	data.cond_seg.seg_userinfo.user_id = user_id;
	if( UpdateOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	writeGroupsRecord(user_id,listGroups->currentItem()->data(3).toInt(),data.seg.seg_userinfo.groups_role);
	showGroupsUsers(listGroups->currentItem());
	//menuNoGroupsUser->removeAction(action);
	updateNoGroupsUserMenu();
	FreeDbData(&data,TABLE_USERINFO);
}
void CGroupsTable::addAllNoGroupsUser()
{
	for (int i=0;i<listNoGroupsUserId.size();i++)
	{
		DB_FMT data;
		InitDbData(&data,TABLE_USERINFO);
		data.flag.flag_userinfo.groups_id=TRUE;
		data.flag.flag_userinfo.groups_role=TRUE;
		data.seg.seg_userinfo.groups_id = listGroups->currentItem()->data(3).toInt();
		data.seg.seg_userinfo.groups_role = 0;
		data.cond_flag.flag_userinfo.user_id=TRUE;
		data.cond_seg.seg_userinfo.user_id = listNoGroupsUserId.at(i);
		if( UpdateOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_USERINFO);
			return;
		}
		writeGroupsRecord(listNoGroupsUserId.at(i),listGroups->currentItem()->data(3).toInt(),data.seg.seg_userinfo.groups_role);
		FreeDbData(&data,TABLE_USERINFO);
	}
	showGroupsUsers(listGroups->currentItem());
	updateNoGroupsUserMenu();
}
void CGroupsTable::delGroupsUser()
{
	QList<QListWidgetItem *> userItems = listGroupsUser->selectedItems();
	int sel_num = userItems.count();
	if (sel_num!=0)
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
		for (int i=0;i<sel_num;i++)
		{
			DB_FMT data;
			InitDbData(&data,TABLE_USERINFO);
			data.flag.flag_userinfo.groups_id=TRUE;
			data.flag.flag_userinfo.groups_role=TRUE;
			data.seg.seg_userinfo.groups_id = -1;
			data.seg.seg_userinfo.groups_role = -1;

			data.cond_flag.flag_userinfo.user_id=TRUE;
			data.cond_seg.seg_userinfo.user_id = userItems.at(i)->data(3).toInt();
			if( UpdateOneRec(&data) != TRUE)
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				FreeDbData(&data,TABLE_USERINFO);
				return;
			}
			FreeDbData(&data,TABLE_USERINFO);
			writeGroupsRecord(listGroupsUser->currentItem()->data(3).toInt(),-1,-1);
			//listGroupsUser->takeItem(userItems.at(i));
			//listGroupsUser->removeItemWidget(userItems.at(i));
		}
	}
	showGroupsUsers(listGroups->currentItem());
	updateNoGroupsUserMenu();
}
void CGroupsTable::setGroupsHead()
{
	QList<QListWidgetItem *> userItems = listGroupsUser->selectedItems();
	int sel_num = userItems.count();
	if (sel_num != 1)
		return;
	for (int i=0;i<listGroupsUser->count();i++)
	{
		int user_id = listGroupsUser->item(i)->data(3).toInt();
		if (JudgeUserGroupsRole(user_id,GROUPS_ROLE_LEADER) == TRUE)
		{
			DB_FMT data;
			InitDbData(&data,TABLE_USERINFO);
			data.flag.flag_userinfo.groups_role=TRUE;
			data.seg.seg_userinfo.groups_role = 0;

			data.cond_flag.flag_userinfo.user_id=TRUE;
			data.cond_seg.seg_userinfo.user_id = user_id;
			if( UpdateOneRec(&data) != TRUE)
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				FreeDbData(&data,TABLE_USERINFO);
				return;
			}
			writeGroupsRecord(user_id,listGroups->currentItem()->data(3).toInt(),0);
			FreeDbData(&data,TABLE_USERINFO);
		}
	}
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.flag.flag_userinfo.groups_role=TRUE;
	data.seg.seg_userinfo.groups_role = 1;

	data.cond_flag.flag_userinfo.user_id=TRUE;
	data.cond_seg.seg_userinfo.user_id = listGroupsUser->currentItem()->data(3).toInt();
	if( UpdateOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	writeGroupsRecord(listGroupsUser->currentItem()->data(3).toInt(),listGroups->currentItem()->data(3).toInt(),1);
	showGroupsUsers(listGroups->currentItem());
	FreeDbData(&data,TABLE_USERINFO);
}
void CGroupsTable::showGroupsUsers(QListWidgetItem *groupsItem)
{
	if(groupsItem==NULL) return;
	int groups_id = groupsItem->data(3).toInt();
	listGroupsUser->clear();
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.cond_flag.flag_userinfo.groups_id=TRUE;
	data.cond_seg.seg_userinfo.groups_id = groups_id;
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	for (int i=0;i<data.rownum;i++)
	{
		if (data_userinfo[i].role == ROLE_STUDENT)
		{
			QString str;
			QListWidgetItem *userItem = new QListWidgetItem;
			if (data_userinfo[i].groups_role == 1)
				str = QString::fromLocal8Bit(data_userinfo[i].longname)+QString::fromLocal8Bit("   (组长)");
			else if (data_userinfo[i].groups_role == 0)
				str = QString::fromLocal8Bit(data_userinfo[i].longname)+QString::fromLocal8Bit("   (组员)");
			userItem->setText(str);
			userItem->setData(3,QVariant(data_userinfo[i].user_id));
			listGroupsUser->addItem(userItem);
		}
	}
	listGroupsUser->setCurrentRow(0);
	FreeDbData(&data,TABLE_USERINFO);
}
void CGroupsTable::updateNoGroupsUserMenu()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	data.cond_flag.flag_userinfo.groups_id=TRUE;
	data.cond_seg.seg_userinfo.groups_id = -1;
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		return;
	}
	menuNoGroupsUser->clear();
	listNoGroupsUserId.clear();
	noGroupsUserSourceModel->clear();
	for (int i=0;i<data.rownum;i++)
	{
		if (data_userinfo[i].role == ROLE_STUDENT)
		{
			QAction* actionUser= new QAction(QIcon(QString::fromLocal8Bit(":/pic/images/user7.png")),QString::fromLocal8Bit(data_userinfo[i].longname), this);
			actionUser->setData(QVariant(data_userinfo[i].user_id));
			connect(actionUser,SIGNAL(triggered()),this,SLOT(addGroupsUser()));
			menuNoGroupsUser->addAction(actionUser);
			QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].longname)); 
			item->setData(QVariant(data_userinfo[i].user_id));
			item->setIcon(QIcon(QString::fromLocal8Bit(":/pic/images/user7.png")));
			noGroupsUserSourceModel->appendRow(item); 
			listNoGroupsUserId.append(data_userinfo[i].user_id);
		}
	}
	FreeDbData(&data,TABLE_USERINFO);
	//menuNoGroupsUser->setFixedWidth(buttonNoGroupsUser->width());
}
void CGroupsTable::writeGroupsRecord(int user_id,int groups_id,short groups_role)
{
	DB_FMT data;
	InitDbData(&data,TABLE_GROUPS_RECORD);
	//设置表中每个字段的flag
	data.flag.flag_groups_record.user_id =TRUE;
	data.flag.flag_groups_record.groups_id=TRUE;
	data.flag.flag_groups_record.chg_time=TRUE;
	data.flag.flag_groups_record.groups_role=TRUE;

	//填充表中每个字段的值
	data.seg.seg_groups_record.user_id = user_id;
	data.seg.seg_groups_record.groups_id = groups_id;
	data.seg.seg_groups_record.groups_role = groups_role;
	GetCurTimeStr(data.seg.seg_groups_record.chg_time);

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_GROUPS_RECORD);
		return;
	}
	FreeDbData(&data,TABLE_GROUPS_RECORD);
}

/**********************************************CStudentInfoLookAtTable****************************************************/
/*CStudentInfoLookAtTable::CStudentInfoLookAtTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	radioLogined->setChecked(false);
	radioUnLogined->setChecked(false);
	textOperateEvent->setReadOnly(true);
	textAlarmEvent->setReadOnly(true);
}
void CStudentInfoLookAtTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CStudentInfoLookAtTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
ComboBoxDelegate::ComboBoxDelegate(QObject *parent):QItemDelegate(parent)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem & option ,const QModelIndex & index ) const
{
	QDateTimeEdit *editor = new QDateTimeEdit(parent);
	editor->setDisplayFormat("dd/M/yyyy");
	editor->setCalendarPopup(true);
	return editor;    
}
void ComboBoxDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
	QDateTimeEdit *dateEditor = qobject_cast<QDateTimeEdit *>(editor);
	if (dateEditor) 
	{
		dateEditor->setDate(QDate::fromString(index.model()->data(index, Qt::EditRole).toString(), "d/M/yyyy"));
	}
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
									const QModelIndex &index) const
{
	QDateTimeEdit *dateEditor = qobject_cast<QDateTimeEdit *>(editor);
	if (dateEditor)
	{
		model->setData(index, dateEditor->date().toString("dd/M/yyyy"));
	}
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
	editor->setGeometry(option.rect);
}*/