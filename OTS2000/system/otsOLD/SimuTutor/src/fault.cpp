#include "fault.h"

CFaultTable::CFaultTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	select_stn = false;
	select_stndev = false;
	select_status = false;
	actionFaultData = new QAction(QString::fromLocal8Bit("故障数据"), this);
	actionAddFault = new QAction(QString::fromLocal8Bit("添加故障"), this);
	actionModifyFault = new QAction(QString::fromLocal8Bit("修改故障"), this);
	actionDelFault = new QAction(QString::fromLocal8Bit("删除故障"), this);
	stndev = new CStnDevTable(this);
	setControlAtt();
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displayFault()));
	connect(listFault,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(buttonFaultDel,SIGNAL(clicked()),this,SLOT(delFault()));
	connect(buttonFaultData,SIGNAL(clicked()),this,SLOT(openFaultData()));
	connect(buttonFaultAdd,SIGNAL(clicked()),this,SLOT(openFaultAdd()));
	connect(buttonFaultModify,SIGNAL(clicked()),this,SLOT(openFaultModify()));
	connect(actionFaultData,SIGNAL(triggered()),this,SLOT(openFaultData()));
	connect(actionAddFault,SIGNAL(triggered()),this,SLOT(openFaultAdd()));
	connect(actionModifyFault,SIGNAL(triggered()),this,SLOT(openFaultModify()));
	connect(actionDelFault,SIGNAL(triggered()),this,SLOT(delFault()));
	connect(buttonStnDev,SIGNAL(clicked()),this,SLOT(openStnDev()));
	connect(buttonExit,SIGNAL(clicked()),stndev,SLOT(close()));
	connect(stndev,SIGNAL(update(int)),this,SLOT(displayUpdate(int)));
	connect(stndev,SIGNAL(update(int,int)),this,SLOT(displayUpdate(int,int)));
}
void CFaultTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void CFaultTable::setControlAtt()
{
	listFault->setSelectionBehavior(QAbstractItemView::SelectRows);
	listFault->setSelectionMode(QAbstractItemView::SingleSelection); 
	listFault->setEditTriggers(QTableWidget::NoEditTriggers);
	listFault->setSortingEnabled(true);
}
void CFaultTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	faultadd->loadChinese();
	faultmodify->loadChinese();
	faultdata->loadChinese();
}

void CFaultTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	faultadd->loadEnglish();
	faultmodify->loadEnglish();
	faultdata->loadEnglish();
}
void CFaultTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listFault->itemAt(listFault->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionFaultData);
		popMenu->addAction(actionAddFault);
		popMenu->addAction(actionModifyFault);
		popMenu->addAction(actionDelFault);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CFaultTable::moveEvent ( QMoveEvent * event )
{
	if (stndev != NULL )
	{
		stndev->mvwith = true;
		stndev->newpt= event->pos();
		this->pos() = event->pos();
		stndev->moveEvent(event);
	}	
}
void CFaultTable::closeEvent(QCloseEvent *event)
{
	this->close();
	stndev->close();
}
void CFaultTable::openStnDev()
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
		stndev->close();
		displayFault();
	}
}
void CFaultTable::displayUpdate(int stn_id)
{
	select_stn = true;
	stnId = stn_id;
	displayFault();
}
void CFaultTable::displayUpdate(int stn_id,int dev_id)
{
	select_stndev = true;
	stnId = stn_id;
	devId = dev_id;
	displayFault();
}
int CFaultTable::displayFault()
{
	DB_FMT data;
	unsigned char stn_id,dev_id;
	InitDbData(&data,TABLE_FAULT);
	if (select_stn)
	{
		data.cond_flag.flag_fault.station_id = TRUE;
		data.cond_seg.seg_fault.station_id = stnId;
	}
	if (select_stndev)
	{
		data.cond_flag.flag_fault.station_id = TRUE;
		data.cond_seg.seg_fault.station_id = stnId;
		data.cond_flag.flag_fault.dev_id = TRUE;
		data.cond_seg.seg_fault.dev_id = devId;
	}
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);	
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return -1;
	}
	listFault->setRowCount(data.rownum);
	for(int i=0;i<data.rownum;i++)
	{	
		listFault->setItem(i,0,new QTableWidgetItem(QString::number(data_fault[i].fault_id)));
		listFault->setItem(i,1,new QTableWidgetItem(QString::fromLocal8Bit(data_fault[i].name)));
		listFault->setItem(i,2,new QTableWidgetItem(QString::fromLocal8Bit(data_fault[i].userinfo_longname)));
		if (data_fault[i].type == 0)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("系统事故")));
		if (data_fault[i].type == 1)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("线路设备事故")));
		if (data_fault[i].type == 2)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("主变设备事故")));
		if (data_fault[i].type == 3)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("机组设备事故")));
		if (data_fault[i].type == 4)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("厂用电事故")));
		if (data_fault[i].type == 5)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("直流事故")));
		if (data_fault[i].type == 6)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("公用设备事故")));
		if (data_fault[i].type == 7)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("机械设备事故")));
		if (data_fault[i].type == 8)	
			listFault->setItem(i,3,new QTableWidgetItem(QString::fromLocal8Bit("其他设备事故")));

		listFault->setItem(i,4,new QTableWidgetItem(QString::fromLocal8Bit(data_fault[i].datetime)));
		listFault->setItem(i,5,new QTableWidgetItem(QString::fromLocal8Bit(data_fault[i].remark)));
		stn_id = data_fault[i].station_id;
		dev_id = data_fault[i].dev_id;
		listFault->setItem(i,6,new QTableWidgetItem(QString::fromLocal8Bit(data_station[stn_id].station_longname )));
		listFault->setItem(i,7,new QTableWidgetItem(QString::fromLocal8Bit(data_dev[stn_id][dev_id].dev_longname)));
	}
	listFault->clearSelection();
	select_stn = false;
	select_stndev = false;
	select_status = false;
	if (data.rownum != 0)
	{
		listFault->resizeColumnsToContents();
	}
	return data.rownum;
}
void CFaultTable::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CFaultTable::delFault()
{
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要删除的故障!"),QMessageBox::Ok);
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
		InitDbData(&data,TABLE_FAULT_DATA);
		QTableWidgetItem* flag_fault_id = listFault->item(curRow,0);
		data.cond_flag.flag_fault_data.fault_id=TRUE;
		data.cond_seg.seg_fault_data.fault_id = flag_fault_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除故障数据失败!"),QMessageBox::Ok);
		}
		InitDbData(&data,TABLE_FAULT);
		data.cond_flag.flag_fault.fault_id=TRUE;
		data.cond_seg.seg_fault.fault_id = flag_fault_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除故障失败!"),QMessageBox::Ok);		
		}
	}
	rownums = displayFault();
}
void CFaultTable::openFaultData()
{
	int fault_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要设置数据的故障!"),QMessageBox::Ok);
	}
	else
	{
		faultdata = new CFaultDataTable(this);
		QTableWidgetItem* item_id = listFault->item(curRow,0);
		fault_id = item_id->text().toInt();
		faultdata->curFaultId=fault_id;
		faultdata->displayFaultData();
		faultdata->show();
	}
}
void CFaultTable::openFaultAdd()
{
	faultadd = new CFaultAddTable(this);
	faultadd->flag_add=true;
	faultadd->buttonApply->setVisible(false);
	faultadd->show();
	connect(faultadd,SIGNAL(insertRecord()),this,SLOT(addFault()));
}
void CFaultTable::openFaultModify()
{
	int fault_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的故障!"),QMessageBox::Ok);
	}
	else
	{
		faultmodify = new CFaultAddTable();
		faultmodify->flag_modify=true;
		QTableWidgetItem* item_id = listFault->item(curRow,0);
		fault_id = item_id->text().toInt();
		curFaultId=fault_id;
		QTableWidgetItem* name = listFault->item(curRow,1);
		faultmodify->editName->setText(name->text());
		QTableWidgetItem* type = listFault->item(curRow,3);
		if (type->text() == QString(""))
			faultmodify->editType->setCurrentIndex(0);
		else
			faultmodify->editType->setCurrentIndex(faultmodify->editType->findText(type->text(),Qt::MatchContains));
		QTableWidgetItem* station = listFault->item(curRow,6);
		if (station->text() == QString(""))
		{
			faultmodify->editStation->setCurrentIndex(0);
		}
		else
		{
			faultmodify->editStation->setCurrentIndex(faultmodify->editStation->findText(station->text(),Qt::MatchContains));
		}
		QTableWidgetItem* dev = listFault->item(curRow,7);
		if (dev->text() == QString(""))
		{
			faultmodify->editDev->setCurrentIndex(0);
		}
		else
		{
			faultmodify->editDev->setCurrentIndex(faultmodify->editDev->findText(dev->text(),Qt::MatchContains));
		}
		QTableWidgetItem* remark = listFault->item(curRow,5);
		faultmodify->editRemark->setText(remark->text());
		faultmodify->setWindowTitle(QString::fromLocal8Bit("修改故障"));
		faultmodify->show();
		connect(faultmodify,SIGNAL(modifyRecord()),this,SLOT(modifyFault()));
	}
}
void CFaultTable::addFault()
{
	DB_FMT data;
	InitDbData(&data,TABLE_FAULT);

	//设置表中每个字段的flag
	data.flag.flag_fault.fault_id =TRUE;
	data.flag.flag_fault.name=TRUE;
	data.flag.flag_fault.user_id=TRUE;
	data.flag.flag_fault.type=TRUE;
	data.flag.flag_fault.datetime=TRUE;
	data.flag.flag_fault.remark=TRUE;
	data.flag.flag_fault.dev_id=TRUE;
	data.flag.flag_fault.station_id=TRUE;

	//填充表中每个字段的值

	faultadd->seg.fault_id = GetInexistenceIdInHdb(TABLE_FAULT);
	GetCurTimeStr( faultadd->seg.datetime);	
	faultadd->seg.user_id = loginUser->login_user_id;
	data.seg.seg_fault = faultadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入故障失败!"),QMessageBox::Ok);		
	}

	displayFault();
}
void CFaultTable::modifyFault()
{

	DB_FMT data;
	InitDbData(&data,TABLE_FAULT);

	//设置表中每个字段的flag
	data.flag.flag_fault.name=TRUE;
	data.flag.flag_fault.user_id=TRUE;
	data.flag.flag_fault.type=TRUE;
	data.flag.flag_fault.datetime=TRUE;
	data.flag.flag_fault.remark=TRUE;
	data.flag.flag_fault.dev_id=TRUE;
	data.flag.flag_fault.station_id=TRUE;


	//填充表中每个字段的值
	GetCurTimeStr( faultmodify->seg.datetime);
	faultmodify->seg.user_id = loginUser->login_user_id;
	data.seg.seg_fault = faultmodify->seg;

	//设置查询条件
	data.cond_flag.flag_fault.fault_id=TRUE;
	data.cond_seg.seg_fault.fault_id = curFaultId;
	if( UpdateOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改故障失败!"),QMessageBox::Ok);		
	}
	displayFault();
}
/***************************************************CFaultAddTable********************************************************/
CFaultAddTable::CFaultAddTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	flag_add=false;
	flag_modify=false;
	displayStation();
	setControlAtt();
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(editStation,SIGNAL(currentIndexChanged(int)),this,SLOT(displayDev(int)));
}
void CFaultAddTable::setControlAtt()
{
	editStation->setCurrentIndex(-1);
	editDev->setCurrentIndex(-1);
}
void CFaultAddTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CFaultAddTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CFaultAddTable::displayStation()
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
void CFaultAddTable::displayDev(int index)
{
	editDev->clear();
	editDev->insertItem(0,QString::fromLocal8Bit("无设备"));
	UCHAR stnid = editStation->itemData(index).toInt();
	for (int j=1;j<=data_station[stnid].dev_num;j++)
	{
		editDev->insertItem(j,QString::fromLocal8Bit(data_dev[stnid][j].dev_longname),QVariant(data_dev[stnid][j].dev_id));
	}
}
void CFaultAddTable::apply()
{		
	strcpy(seg.name,editName->text().toLocal8Bit().data());	
	seg.type = editType->currentIndex();
	seg.station_id = editStation->itemData(editStation->currentIndex()).toInt();
	seg.dev_id = editDev->itemData(editDev->currentIndex()).toInt();
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
void CFaultAddTable::enter()
{
	apply();
	editName->clear();
	editStation->setCurrentIndex(-1);
	editDev->setCurrentIndex(-1);
	editRemark->clear();
	this->close();
	flag_add=false;
	flag_modify=false;
}