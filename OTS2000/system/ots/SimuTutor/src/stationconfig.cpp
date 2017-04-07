#include "stationconfig.h"

CStationConfigTable::CStationConfigTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
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
	select_status = false;
	actionAddStation = new QAction(QString::fromLocal8Bit("添加主机"), this);
	actionModifyStation = new QAction(QString::fromLocal8Bit("修改主机"), this);
	actionDelStation = new QAction(QString::fromLocal8Bit("删除主机"), this);
	setControlAtt();
	//displayStation();
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displayStation()));
	connect(listStation,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(buttonStationDel,SIGNAL(clicked()),this,SLOT(delStation()));
	connect(buttonStationAdd,SIGNAL(clicked()),this,SLOT(openStationAdd()));
	connect(buttonStationModify,SIGNAL(clicked()),this,SLOT(openStationModify()));
	connect(actionAddStation,SIGNAL(triggered()),this,SLOT(openStationAdd()));
	connect(actionModifyStation,SIGNAL(triggered()),this,SLOT(openStationModify()));
	connect(actionDelStation,SIGNAL(triggered()),this,SLOT(delStation()));
}
void CStationConfigTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listStation->itemAt(listStation->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddStation);
		popMenu->addAction(actionModifyStation);
		popMenu->addAction(actionDelStation);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单

void CStationConfigTable::setControlAtt()
{
	listStation->setSelectionBehavior(QAbstractItemView::SelectRows);
	listStation->setSelectionMode(QAbstractItemView::SingleSelection); 
	listStation->setEditTriggers(QTableWidget::NoEditTriggers);
	listStation->setSortingEnabled(true);
}
void CStationConfigTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	stationadd->loadChinese();
// 	stationmodify->loadChinese();
}
void CStationConfigTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	stationadd->loadEnglish();
// 	stationmodify->loadEnglish();
}
void CStationConfigTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void  CStationConfigTable::setLanguage(int language)
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
int CStationConfigTable::displayStation()
{
	DB_FMT data;
	InitDbData(&data,TABLE_HOST);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	listStation->setRowCount(data.rownum);
	for(int i=0;i<data.rownum;i++)
	{	
		listStation->setItem(i,0,new QTableWidgetItem(QString::number(data_host[i].host_id)));
		listStation->setItem(i,1,new QTableWidgetItem(QString::fromLocal8Bit(data_host[i].name)));
		listStation->setItem(i,2,new QTableWidgetItem(QString::fromLocal8Bit(data_host[i].oth_name)));
		listStation->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit(data_host[i].ip_a)));
		listStation->setItem(i,4,new QTableWidgetItem(QString::fromLocal8Bit(data_host[i].ip_b)));
		if (data_host[i].is_student==1)
		{
			listStation->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit("是")));
		} 
		else if(data_host[i].is_student==0)
		{
			listStation->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit("否")));
		}
		if (data_host[i].is_teacher==1)
		{
			listStation->setItem(i,6,new QTableWidgetItem(QString::fromLocal8Bit("是")));
		} 
		else if (data_host[i].is_teacher==0)
		{
			listStation->setItem(i,6,new QTableWidgetItem(QString::fromLocal8Bit("否")));
		}
		if (data_host[i].is_server==1)
		{
			listStation->setItem(i,7,new QTableWidgetItem(QString::fromLocal8Bit("是")));
		} 
		else if(data_host[i].is_server==0)
		{
			listStation->setItem(i,7,new QTableWidgetItem(QString::fromLocal8Bit("否")));
		}
		listStation->setItem(i,8,new QTableWidgetItem(QString::number(data_host[i].server_id)));
	}
	select_status = false;
	listStation->clearSelection();
	if (data.rownum != 0)
	{
		listStation->resizeColumnsToContents();
	}	
	return data.rownum;
}
void CStationConfigTable::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CStationConfigTable::delStation()
{
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要删除的主机!"),QMessageBox::Ok);
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
		InitDbData(&data,TABLE_HOST);
		data.cond_flag.flag_host.host_id=TRUE;
		QTableWidgetItem* flag_host_id = listStation->item(curRow,0);
		data.cond_seg.seg_host.host_id = flag_host_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
	rownums = displayStation();
}
void CStationConfigTable::openStationAdd()
{
	stationadd = new CStationAddTable(this);
	stationadd->flag_add=true;
	stationadd->buttonApply->setVisible(false);
	stationadd->show();
	connect(stationadd,SIGNAL(insertRecord()),this,SLOT(addStation()));
}
void CStationConfigTable::openStationModify()
{
	int host_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的主机!"),QMessageBox::Ok);
	}
	else
	{
		stationmodify = new CStationAddTable(this);
		stationmodify->flag_modify=true;
		QTableWidgetItem* item_id = listStation->item(curRow,0);
		host_id = item_id->text().toInt();
		curHostId = host_id;
		QTableWidgetItem* name = listStation->item(curRow,1);
		stationmodify->editHostName->setText(name->text());
		QTableWidgetItem* oth_name = listStation->item(curRow,2);
		stationmodify->editOthName->setText(oth_name->text());
		QTableWidgetItem* ip_a = listStation->item(curRow,3);
		stationmodify->editIpA->setText(ip_a->text());
		QTableWidgetItem* ip_b = listStation->item(curRow,4);
		stationmodify->editIpB->setText(ip_b->text());

		QTableWidgetItem* is_student = listStation->item(curRow,5);
		QString isStudent = is_student->text();
		if (isStudent == QString::fromLocal8Bit("是"))
		{
			stationmodify->checkStudent->setCheckState(Qt::Checked);
		} 
		else if (isStudent == QString::fromLocal8Bit("否"))
		{
			stationmodify->checkStudent->setCheckState(Qt::Unchecked);
		}

		QTableWidgetItem* is_teacher = listStation->item(curRow,6);
		QString isTeacher = is_teacher->text();
		if (isTeacher == QString::fromLocal8Bit("是"))
		{
			stationmodify->checkTeacher->setCheckState(Qt::Checked);
		} 
		else if (isTeacher == QString::fromLocal8Bit("否"))
		{
			stationmodify->checkTeacher->setCheckState(Qt::Unchecked);
		}

		QTableWidgetItem* is_server = listStation->item(curRow,7);
		QString isServer = is_server->text();
		if (isServer == QString::fromLocal8Bit("是"))
		{
			stationmodify->checkServer->setCheckState(Qt::Checked);
		} 
		else if (isServer == QString::fromLocal8Bit("否"))
		{
			stationmodify->checkServer->setCheckState(Qt::Unchecked);
		}

		QTableWidgetItem* server_id = listStation->item(curRow,8);
		stationmodify->editServerId->setText(server_id->text());
		stationmodify->setWindowTitle(QString::fromLocal8Bit("修改工作站"));
		stationmodify->show();
		connect(stationmodify,SIGNAL(modifyRecord()),this,SLOT(modifyStation()));
	}
}
void CStationConfigTable::addStation()
{
	DB_FMT data;
	InitDbData(&data,TABLE_HOST);

	//设置表中每个字段的flag
	data.flag.flag_host.host_id =TRUE;
	data.flag.flag_host.name=TRUE;
	data.flag.flag_host.oth_name=TRUE;
	data.flag.flag_host.ip_a=TRUE;
	data.flag.flag_host.ip_b=TRUE;
	data.flag.flag_host.is_student=TRUE;
	data.flag.flag_host.is_teacher=TRUE;
	data.flag.flag_host.is_server=TRUE;
	data.flag.flag_host.server_id=TRUE;

	//填充表中每个字段的值

	stationadd->seg.host_id = GetInexistenceIdInHdb(TABLE_HOST);
	data.seg.seg_host = stationadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}

	displayStation();
}
void CStationConfigTable::modifyStation()
{

	DB_FMT data;
	InitDbData(&data,TABLE_HOST);

	//设置表中每个字段的flag
	data.flag.flag_host.name=TRUE;
	data.flag.flag_host.oth_name=TRUE;
	data.flag.flag_host.ip_a=TRUE;
	data.flag.flag_host.ip_b=TRUE;
	data.flag.flag_host.is_student=TRUE;
	data.flag.flag_host.is_teacher=TRUE;
	data.flag.flag_host.is_server=TRUE;
	data.flag.flag_host.server_id=TRUE;



	//填充表中每个字段的值
	data.seg.seg_host = stationmodify->seg;

	//设置查询条件
	data.cond_flag.flag_host.host_id=TRUE;
	data.cond_seg.seg_host.host_id = curHostId;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	displayStation();
}
/****************************************************CStationAddTable****************************************************/
CStationAddTable::CStationAddTable(QWidget* parent)
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
void CStationAddTable::setControlAtt()
{
	QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
	QValidator *validator = new QRegExpValidator(rx, this);	
	//editIpA->setInputMask("000.000.000.000");
	editIpA->setValidator(validator);
	//editIpB->setInputMask("000.000.000.000");
	editIpB->setValidator(validator);
}
void CStationAddTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CStationAddTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CStationAddTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void  CStationAddTable::setLanguage(int language)
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
void CStationAddTable::apply()
{		
	strcpy(seg.name,editHostName->text().toLocal8Bit().data());
	strcpy(seg.oth_name,editOthName->text().toLocal8Bit().data());
	strcpy(seg.ip_a,editIpA->text().toLocal8Bit().data());	
	strcpy(seg.ip_b,editIpB->text().toLocal8Bit().data());
	if (checkStudent->checkState()==Qt::Checked)
	{
		seg.is_student = 1;
	} 
	else
	{
		seg.is_student = 0;
	}
	if (checkTeacher->checkState()==Qt::Checked)
	{
		seg.is_teacher = 1;
	} 
	else
	{
		seg.is_teacher = 0;
	}
	if (checkServer->checkState()==Qt::Checked)
	{
		seg.is_server = 1;
	} 
	else
	{
		seg.is_server = 0;
	}
	seg.server_id = editServerId->text().toInt();
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}

void CStationAddTable::enter()
{
	apply();
	editHostName->clear();
	editOthName->clear();
	editIpA->clear();
	editIpB->clear();
	editServerId->clear();
	checkStudent->setCheckState(Qt::Unchecked);
	checkTeacher->setCheckState(Qt::Unchecked);
	checkServer->setCheckState(Qt::Unchecked);
	this->close();
	flag_add=false;
	flag_modify=false;
}
