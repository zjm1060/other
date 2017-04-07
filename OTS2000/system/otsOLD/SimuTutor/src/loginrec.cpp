#include "loginrec.h"

CLoginRecTable::CLoginRecTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	actionDelLoginRec = new QAction(QString::fromLocal8Bit("删除记录"), this);
	loginRecModel = new QStandardItemModel;
	init();
	connect(buttonDisplay,SIGNAL(clicked()),this,SLOT(displayLoginRec()));
	connect(buttonDisplayToday,SIGNAL(clicked()),this,SLOT(displayTodayLoginRec()));
	connect(buttonExport,SIGNAL(clicked()),this,SLOT(exportToExcel()));
	connect(actionDelLoginRec,SIGNAL(triggered()),this,SLOT(delLoginRec()));
}
void CLoginRecTable::init()
{
	editStartTime->setCalendarPopup(true);
	editEndTime->setCalendarPopup(true); 
	editEndTime->setDateTime(QDateTime::currentDateTime());
	editStartTime->setDateTime(QDateTime::currentDateTime().addSecs(-3600));
	viewLoginRec->setSelectionBehavior(QAbstractItemView::SelectRows);
	viewLoginRec->setEditTriggers(QTableView::NoEditTriggers);
	viewLoginRec->verticalHeader()->setVisible(false);
	viewLoginRec->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QCompleter *completer = new QCompleter(this);
	completer->setModel(comboUserName->model());
	comboUserName->setCompleter(completer);
}
void CLoginRecTable::showEvent(QShowEvent *event)
{
	comboUserName->setCurrentIndex(0);
	comboHostName->setCurrentIndex(0);
	editLoginTime->clear();
	displayUser();
	displayHost();
}
void CLoginRecTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CLoginRecTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CLoginRecTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	//QTableWidgetItem* pTableWidgetItem=listFault->itemAt(listFault->mapFromGlobal(QCursor::pos()-p));
	//if(pTableWidgetItem != NULL)
	//{
	//	popMenu->addAction(actionDelRec);
	//}
	popMenu->addAction(actionDelLoginRec);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单

void CLoginRecTable::displayUser()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	comboUserName->clear();
	comboUserName->insertItem(0,QString::fromLocal8Bit("全部用户"));
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);	
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return;
	}
	for(int i=0;i<data.rownum;i++)
	{
		comboUserName->insertItem(i+1,QString::fromLocal8Bit(data_userinfo[i].longname));
		comboUserName->setItemData(i+1,QVariant(data_userinfo[i].user_id));
	}
}

void CLoginRecTable::displayHost()
{
	DB_FMT data;
	InitDbData(&data,TABLE_HOST);
	comboHostName->clear();
	comboHostName->insertItem(0,QString::fromLocal8Bit("全部主机"));
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);	
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return;
	}
	for(int i=0;i<data.rownum;i++)
	{
		comboHostName->insertItem(i+1,QString::fromLocal8Bit(data_host[i].name));
		comboHostName->setItemData(i+1,QVariant(data_host[i].host_id));
	}
}

void CLoginRecTable::displayTodayLoginRec()
{
	QDateTime time_start,time_end;
	time_start.setDate(QDate::currentDate());
	time_start.setTime(QTime(0,0,0,0));
	time_end.setDate(QDate::currentDate());
	time_end.setTime(QTime(23,59,59,999));
	editStartTime->setDateTime(time_start);
	editEndTime->setDateTime(time_end);
	displayLoginRec();
}

void CLoginRecTable::displayLoginRec()
{
	DB_FMT data;
	InitDbData(&data,TABLE_LOGIN_INFO);
	data.cond_flag.flag_login_info.datatime = TRUE;
	data.cond_flag.flag_login_info.datatime_end = TRUE;
	if (comboUserName->currentText() != QString::fromLocal8Bit("全部用户"))
		data.cond_flag.flag_login_info.user_id = TRUE;
	if (comboHostName->currentIndex() != 0)
		data.cond_flag.flag_login_info.host_id = TRUE;

	strcpy(data.cond_seg.seg_login_info.datatime,editStartTime->dateTime().toString(editStartTime->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_seg.seg_login_info.datatime_end,editEndTime->dateTime().toString(editEndTime->displayFormat()).toLocal8Bit().data());
	strcpy(data.cond_flag.flag_login_info.datetime_opr,">=");
	strcpy(data.cond_flag.flag_login_info.datetime_end_opr,"<=");
	data.cond_seg.seg_login_info.user_id = GetUserIdFromLongName(comboUserName->currentText().toLocal8Bit().data());
	data.cond_seg.seg_login_info.host_id = comboHostName->itemData(comboHostName->currentIndex()).toInt();

	setLoginRecModel(&data);
}

void CLoginRecTable::setLoginRecModel(DB_FMT *data)
{
	QDateTime start_time,end_time;
	int totle_sec = 0;
	int task_num = 0;
	int cmd_num = 0;
	int alm_num = 0;
	int msg_num = 0;
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("搜索数据库,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	viewLoginRec->setModel(NULL);
	if( SelectRecs(data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data->sql_str,data->sql_error);
		qDebug("查询数据库失败!");
		return;
	}
	progressDlg->setRange(0,data->rownum-1);
	loginRecModel->clear();
	loginRecModel->setColumnCount(4);  
	loginRecModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户名"));  
	loginRecModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("登陆时间")); 
	loginRecModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("退出时间"));
	loginRecModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("主机名")); 
	for (int i=0;i<data->rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_login_info[i].userinfo_longname));  
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_login_info[i].login_time));  
		QStandardItem* item2 = new QStandardItem(QString::fromLocal8Bit(data_login_info[i].logout_time));  
		QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit(data_login_info[i].host_name)); 
		QStandardItem* item4 = new QStandardItem(QString::number(data_login_info[i].user_id)); 
		QStandardItem* item5 = new QStandardItem(QString::number(data_login_info[i].host_id)); 
		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		item3->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2 << item3 << item4 << item5; 
		loginRecModel->appendRow(items); 
		progressDlg->setValue(i); 
		start_time = QDateTime::fromString(QString::fromLocal8Bit(data_login_info[i].login_time),QString("yyyy-MM-dd HH:mm:ss"));
		end_time = QDateTime::fromString(QString::fromLocal8Bit(data_login_info[i].logout_time),QString("yyyy-MM-dd HH:mm:ss"));
		if (strcmp(data_login_info[i].logout_time,"") == TRUE)
		{
			totle_sec += start_time.secsTo(end_time);
			getTaskNum(data_login_info[i].login_time,data_login_info[i].logout_time,&task_num);
			getOptNum(data_login_info[i].login_time,data_login_info[i].logout_time,&cmd_num,&alm_num,&msg_num);
		}
		if(progressDlg->wasCanceled())  
			break;
	}
	int h,m,s;
	GetTimeFromSecs(totle_sec,&h,&m,&s);
	editLoginTime->setText(QString::fromLocal8Bit("%1小时%2分%3秒").arg(h).arg(m).arg(s));
	editTaskNum->setText(QString::number(task_num));
	editOptNum->setText(QString::fromLocal8Bit("下令:%1 报警:%2 短消息:%3").arg(cmd_num).arg(alm_num).arg(msg_num));
	viewLoginRec->setModel(loginRecModel);
	selectionModel = viewLoginRec->selectionModel();
	viewLoginRec->hideColumn(4);
	viewLoginRec->hideColumn(5);
	viewLoginRec->setColumnWidth(0,150);
	viewLoginRec->setColumnWidth(1,240);
	viewLoginRec->setColumnWidth(2,240);
	viewLoginRec->setColumnWidth(3,150);
}
int CLoginRecTable::getTaskNum(char *start_time,char *end_time,int *task_num)
{
	DB_FMT data;
	InitDbData(&data,TABLE_SEND_TASK);
	if (comboUserName->currentText() != QString::fromLocal8Bit("全部用户"))
		data.cond_flag.flag_send_task.user_id = TRUE;
	data.cond_flag.flag_send_task.start_time_start = TRUE;
	data.cond_flag.flag_send_task.end_time_end = TRUE;
	data.cond_seg.seg_send_task.user_id = GetUserIdFromLongName(comboUserName->currentText().toLocal8Bit().data());
	strcpy(data.cond_seg.seg_send_task.start_time_start,start_time);
	strcpy(data.cond_seg.seg_send_task.end_time_end,end_time);
	strcpy(data.cond_flag.flag_send_task.start_time_start_opr,">=");
	strcpy(data.cond_flag.flag_send_task.end_time_end_opr,"<=");
	if(SelectRecs(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return -1;
	}
	*task_num = *task_num + data.rownum;
	return TRUE;
}
int CLoginRecTable::getOptNum(char *start_time,char *end_time,int *cmd_num,int *alm_num,int *msg_num)
{
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);
	if (comboUserName->currentText() != QString::fromLocal8Bit("全部用户"))
		data.cond_flag.flag_record.user_id = TRUE;
	data.cond_flag.flag_record.datetime = TRUE;
	data.cond_flag.flag_record.datetime_end = TRUE;
	data.cond_seg.seg_record.user_id = GetUserIdFromLongName(comboUserName->currentText().toLocal8Bit().data());
	strcpy(data.cond_seg.seg_record.datetime,start_time);
	strcpy(data.cond_seg.seg_record.datetime_end,end_time);
	strcpy(data.cond_flag.flag_record.datetime_opr,">=");
	strcpy(data.cond_flag.flag_record.datetime_end_opr,"<=");
	if(SelectRecs(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return -1;
	}
	for (int i=0;i<data.rownum;i++)
	{
		switch(data_record[i].type)
		{
			case 0:
				*cmd_num = *cmd_num+1;
				break;
			case 1:
				*alm_num = *alm_num+1;
				break;
			case 3:
				*msg_num = *msg_num+1;
				break;
			case 4:
				*msg_num = *msg_num+1;
				break;
			default:
				break;
		}
	}
	return TRUE;
}
void CLoginRecTable::exportToExcel()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.xls)"));
	if (fileName.isEmpty())
		return;
	ExcelObjectODBC excel(fileName);
	excel.AddHeader(0, "用户名", "char(10)");
	excel.AddHeader(1, "登陆时间", "char(22)");
	excel.AddHeader(2, "退出时间", "char(22)");
	excel.AddHeader(3, "主机名", "char(10)");
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
		progressDlg->setRange(0,viewLoginRec->model()->rowCount());  
		connect(&excel, SIGNAL(exportedRowCount(int)),progressDlg,SLOT(setValue(int)));
		connect(progressDlg, SIGNAL(canceled()),&excel,SLOT(cancel()));
		int retVal = excel.ExportTableViewToExcel(viewLoginRec);
		if( retVal > 0)
		{
			progressDlg->cancel();
			QMessageBox::information(this, QString::fromLocal8Bit("完成"),
				QString(QString::fromLocal8Bit("共导出%1条记录")).arg(retVal));
		}
	}
}

void CLoginRecTable::delLoginRec()
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
		delLoginRecFromHdb(indexes.at(i).row());
		loginRecModel->removeRow(indexes.at(i).row());
	}
}

void CLoginRecTable::delLoginRecFromHdb(int row)
{
	QString time;
	int user_id;
	DB_FMT data;
	InitDbData(&data,TABLE_LOGIN_INFO);
	time = loginRecModel->data(loginRecModel->index(row,1)).toString();
	user_id = loginRecModel->data(loginRecModel->index(row,4)).toInt();
	data.cond_flag.flag_login_info.login_time = TRUE;
	data.cond_flag.flag_login_info.user_id = TRUE;
	strcpy(data.cond_seg.seg_login_info.login_time,time.toLocal8Bit().data());
	data.cond_seg.seg_login_info.user_id = user_id;
	if( DeleteOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}
