#include "revert.h"

CRevertTable::CRevertTable(QWidget* parent)
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
	select_stn = false;
	select_stndev = false;
	actionAddRevert = new QAction(QString::fromLocal8Bit("添加工况再现"), this);
	actionModifyRevert = new QAction(QString::fromLocal8Bit("修改工况再现"), this);
	actionDelRevert = new QAction(QString::fromLocal8Bit("删除工况再现"), this);
	stndev = new CStnDevTable(this);
	setControlAtt();
	//displayRevert();
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displayRevert()));
	connect(listRevert,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(buttonRevertDel,SIGNAL(clicked()),this,SLOT(delRevert()));
	connect(buttonRevertAdd,SIGNAL(clicked()),this,SLOT(openRevertAdd()));
	connect(buttonRevertModify,SIGNAL(clicked()),this,SLOT(openRevertModify()));
	connect(actionAddRevert,SIGNAL(triggered()),this,SLOT(openRevertAdd()));
	connect(actionModifyRevert,SIGNAL(triggered()),this,SLOT(openRevertModify()));
	connect(actionDelRevert,SIGNAL(triggered()),this,SLOT(delRevert()));
	connect(buttonStnDev,SIGNAL(clicked()),this,SLOT(openStnDev()));
	connect(buttonExit,SIGNAL(clicked()),stndev,SLOT(close()));
	connect(stndev,SIGNAL(update(int)),this,SLOT(displayUpdate(int)));
	connect(stndev,SIGNAL(update(int,int)),this,SLOT(displayUpdate(int,int)));
}
void CRevertTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void  CRevertTable::setLanguage(int language)
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
void CRevertTable::setControlAtt()
{
	listRevert->setSelectionBehavior(QAbstractItemView::SelectRows);
	listRevert->setSelectionMode(QAbstractItemView::SingleSelection); 
	listRevert->setEditTriggers(QTableWidget::NoEditTriggers);
	listRevert->setSortingEnabled(true);
}
void CRevertTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CRevertTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CRevertTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listRevert->itemAt(listRevert->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddRevert);
		popMenu->addAction(actionModifyRevert);
		popMenu->addAction(actionDelRevert);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CRevertTable::moveEvent ( QMoveEvent * event )
{
	if (stndev != NULL )
	{
		stndev->mvwith = true;
		stndev->newpt= event->pos();
		this->pos() = event->pos();
		stndev->moveEvent(event);
	}	
}
void CRevertTable::closeEvent(QCloseEvent *event)
{
	this->close();
	stndev->close();
}
void CRevertTable::openStnDev()
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
		displayRevert();
	}
}
void CRevertTable::displayUpdate(int stn_id)
{
	select_stn = true;
	stnId = stn_id;
	displayRevert();
}
void CRevertTable::displayUpdate(int stn_id,int dev_id)
{
	select_stndev = true;
	stnId = stn_id;
	devId = dev_id;
	displayRevert();
}
int CRevertTable::displayRevert()
{
	DB_FMT data;
	unsigned char stn_id,dev_id;
	InitDbData(&data,TABLE_REVERT);
	if (select_stn)
	{
		data.cond_flag.flag_revert.station_id = TRUE;
		data.cond_seg.seg_revert.station_id = stnId;
	}
	if (select_stndev)
	{
		data.cond_flag.flag_revert.station_id = TRUE;
		data.cond_seg.seg_revert.station_id = stnId;
		data.cond_flag.flag_revert.dev_id = TRUE;
		data.cond_seg.seg_revert.dev_id = devId;
	}
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	listRevert->setRowCount(data.rownum);
	for(int i=0;i<data.rownum;i++)
	{	
		listRevert->setItem(i,0,new QTableWidgetItem(QString::number(data_revert[i].revert_id)));
		listRevert->setItem(i,1,new QTableWidgetItem(QString::fromLocal8Bit(data_revert[i].name)));
		listRevert->setItem(i,2,new QTableWidgetItem(QString::fromLocal8Bit(data_revert[i].userinfo_longname)));
		listRevert->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit(data_revert[i].start_time)));
		listRevert->setItem(i,4,new QTableWidgetItem(QString::fromLocal8Bit(data_revert[i].end_time)));
		listRevert->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit(data_revert[i].filename)));
		listRevert->setItem(i,6,new QTableWidgetItem(QString::fromLocal8Bit(data_revert[i].remark)));
		stn_id = data_revert[i].station_id;
		dev_id = data_revert[i].dev_id;
		listRevert->setItem(i,7,new QTableWidgetItem(QString::fromLocal8Bit(data_station[stn_id].station_longname )));
		listRevert->setItem(i,8,new QTableWidgetItem(QString::fromLocal8Bit(data_dev[stn_id][dev_id].dev_longname)));
		if (data_revert[i].src == 0)
		{
			listRevert->setItem(i,9,new QTableWidgetItem(QString::fromLocal8Bit("来源于仿真任务")));
		}
		else if (data_revert[i].src == 1)
		{
			listRevert->setItem(i,9,new QTableWidgetItem(QString::fromLocal8Bit("来源于监控系统")));
		}
		if (data_revert[i].type == 1)
		{
			listRevert->setItem(i,10,new QTableWidgetItem(QString::fromLocal8Bit("画面历史工况")));
		}
		else if (data_revert[i].type == 2)
		{
			listRevert->setItem(i,10,new QTableWidgetItem(QString::fromLocal8Bit("设备历史工况")));
		}	
	}
	select_status = false;
	select_stn = false;
	select_stndev = false;
	listRevert->clearSelection();
	if (data.rownum != 0)
	{
		listRevert->resizeColumnsToContents();
	}
	return data.rownum;
}
void CRevertTable::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CRevertTable::delRevert()
{
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要删除的工况再现!"),QMessageBox::Ok);
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
		InitDbData(&data,TABLE_REVERT);
		QTableWidgetItem* flag_revert_id = listRevert->item(curRow,0);
		data.cond_flag.flag_revert.revert_id=TRUE;
		data.cond_seg.seg_revert.revert_id = flag_revert_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
	rownums = displayRevert();
}
void CRevertTable::openRevertAdd()
{
	revertadd = new CRevertAddTable(this);
	revertadd->editStartTime->setDateTime(QDateTime::currentDateTime());
	revertadd->editEndTime->setDateTime(QDateTime::currentDateTime());
	revertadd->flag_add=true;
	revertadd->buttonApply->setVisible(false);
	revertadd->init();
	revertadd->show();
	connect(revertadd,SIGNAL(insertRecord()),this,SLOT(addRevert()));
}
void CRevertTable::openRevertModify()
{
	int revert_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的工况再现!"),QMessageBox::Ok);
	}
	else
	{
		revertmodify = new CRevertAddTable(this);
		revertmodify->flag_modify=true;
		QTableWidgetItem* item_id = listRevert->item(curRow,0);
		revert_id = item_id->text().toInt();
		curRevertId = revert_id;
		QTableWidgetItem* name = listRevert->item(curRow,1);
		revertmodify->editRevertName->setText(name->text());
		QTableWidgetItem* start_time = listRevert->item(curRow,3);
		revertmodify->editStartTime->setDateTime(QDateTime::fromString(start_time->text(),QString("yyyy-MM-dd HH:mm:ss")));
		QTableWidgetItem* end_time = listRevert->item(curRow,4);
		revertmodify->editEndTime->setDateTime(QDateTime::fromString(end_time->text(),QString("yyyy-MM-dd HH:mm:ss")));
		QTableWidgetItem* filename = listRevert->item(curRow,5);
		revertmodify->editFileName->setText(filename->text());
		QTableWidgetItem* station = listRevert->item(curRow,7);
		if (station->text() == QString(""))
		{
			revertmodify->editStation->setCurrentIndex(0);
		}
		else
		{
			revertmodify->editStation->setCurrentIndex(revertmodify->editStation->findText(station->text(),Qt::MatchContains));
		}
		QTableWidgetItem* dev = listRevert->item(curRow,8);
		if (dev->text() == QString(""))
		{
			revertmodify->editDev->setCurrentIndex(0);
		}
		else
		{
			revertmodify->editDev->setCurrentIndex(revertmodify->editDev->findText(dev->text(),Qt::MatchContains));
		}
		QTableWidgetItem* src = listRevert->item(curRow,9);
		if (src->text() == QString::fromLocal8Bit("来源于仿真任务"))
		{
			revertmodify->editSrc->setCurrentIndex(0);
		}
		else if (src->text() == QString::fromLocal8Bit("来源于监控系统"))
		{
			revertmodify->editSrc->setCurrentIndex(1);
		}
		QTableWidgetItem* type = listRevert->item(curRow,10);
		if (type->text() == QString::fromLocal8Bit("画面历史工况"))
		{
			revertmodify->editType->setCurrentIndex(0);
		}
		else if (type->text() == QString::fromLocal8Bit("设备历史工况"))
		{
			revertmodify->editType->setCurrentIndex(1);
		}
		QTableWidgetItem* remark = listRevert->item(curRow,6);
		revertmodify->editRemark->setPlainText(remark->text());
		revertmodify->setWindowTitle(QString::fromLocal8Bit("修改工况再现"));	
		revertmodify->show();
		connect(revertmodify,SIGNAL(modifyRecord()),this,SLOT(modifyRevert()));
	}
}
void CRevertTable::addRevert()
{
	DB_FMT data;
	InitDbData(&data,TABLE_REVERT);

	//设置表中每个字段的flag
	data.flag.flag_revert.revert_id =TRUE;
	data.flag.flag_revert.name=TRUE;
	data.flag.flag_revert.user_id=TRUE;
	data.flag.flag_revert.start_time=TRUE;
	data.flag.flag_revert.end_time=TRUE;
	data.flag.flag_revert.filename=TRUE;
	data.flag.flag_revert.remark=TRUE;
	data.flag.flag_revert.src=TRUE;
	data.flag.flag_revert.type=TRUE;
	data.flag.flag_revert.dev_id=TRUE;
	data.flag.flag_revert.station_id=TRUE;

	//填充表中每个字段的值

	revertadd->seg.revert_id = GetInexistenceIdInHdb(TABLE_REVERT);
	revertadd->seg.user_id = loginUser->login_user_id;
	data.seg.seg_revert = revertadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}

	displayRevert();
}
void CRevertTable::modifyRevert()
{

	DB_FMT data;
	InitDbData(&data,TABLE_REVERT);

	//设置表中每个字段的flag
	data.flag.flag_revert.name=TRUE;
	data.flag.flag_revert.user_id=TRUE;
	data.flag.flag_revert.start_time=TRUE;
	data.flag.flag_revert.end_time=TRUE;
	data.flag.flag_revert.filename=TRUE;
	data.flag.flag_revert.remark=TRUE;
	data.flag.flag_revert.src=TRUE;
	data.flag.flag_revert.type=TRUE;
	data.flag.flag_revert.dev_id=TRUE;
	data.flag.flag_revert.station_id=TRUE;



	//填充表中每个字段的值
	revertmodify->seg.user_id = loginUser->login_user_id;
	data.seg.seg_revert = revertmodify->seg;

	//设置查询条件
	data.cond_flag.flag_revert.revert_id=TRUE;
	data.cond_seg.seg_revert.revert_id = curRevertId;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	displayRevert();
}
/**************************************************CRevertAddTable******************************************************/
CRevertAddTable::CRevertAddTable(QWidget* parent)
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
	flag_add=false;
	flag_modify=false;
	displayStation();
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(editStation,SIGNAL(currentIndexChanged(int)),this,SLOT(displayDev(int)));
}
void CRevertAddTable::init()
{
	editSrc->setCurrentIndex(0);
	editType->setCurrentIndex(0);
	editStation->setCurrentIndex(0);
	editDev->setCurrentIndex(0);
}
void CRevertAddTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CRevertAddTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CRevertAddTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void  CRevertAddTable::setLanguage(int language)
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
void CRevertAddTable::displayStation()
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
void CRevertAddTable::displayDev(int index)
{
	editDev->clear();
	editDev->insertItem(0,QString::fromLocal8Bit("无设备"));
	UCHAR stnid = editStation->itemData(index).toInt();
	for (int j=1;j<=data_station[stnid].dev_num;j++)
	{
		editDev->insertItem(j,QString::fromLocal8Bit(data_dev[stnid][j].dev_longname),QVariant(data_dev[stnid][j].dev_id));
	}
}
void CRevertAddTable::apply()
{		
	strcpy(seg.name,editRevertName->text().toLocal8Bit().data());
	strcpy(seg.start_time,editStartTime->dateTime().toString(editStartTime->displayFormat()).toLocal8Bit().data());
	strcpy(seg.end_time,editEndTime->dateTime().toString(editEndTime->displayFormat()).toLocal8Bit().data());
	strcpy(seg.filename,editFileName->text().toLocal8Bit().data());
	seg.station_id = editStation->itemData(editStation->currentIndex()).toInt();
	seg.dev_id = editDev->itemData(editDev->currentIndex()).toInt();
	seg.src = editSrc->currentIndex();
	seg.type = editType->currentIndex()+1;
	strcpy(seg.remark,editRemark->toPlainText().toLocal8Bit().data());
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}
void CRevertAddTable::enter()
{
	apply();
	editRevertName->clear();
	editFileName->clear();
	editStation->setCurrentIndex(-1);
	editDev->setCurrentIndex(-1);
	editRemark->clear();
	this->close();
	flag_add=false;
	flag_modify=false;
}