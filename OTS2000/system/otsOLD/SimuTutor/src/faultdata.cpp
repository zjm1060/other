#include "faultdata.h"

CFaultDataTable::CFaultDataTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	setControlAtt();
	select_status = false;
	actionAddFaultData= new QAction(QString::fromLocal8Bit("添加故障数据"), this);
	actionModifyFaultData = new QAction(QString::fromLocal8Bit("修改故障数据"), this);
	actionDelFaultData = new QAction(QString::fromLocal8Bit("删除故障数据"), this);
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(openFaultDataAdd()));
	connect(buttonModify,SIGNAL(clicked()),this,SLOT(openFaultDataModify()));
	connect(buttonDel,SIGNAL(clicked()),this,SLOT(delFaultData()));
	connect(listFaultData,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(actionAddFaultData,SIGNAL(triggered()),this,SLOT(openFaultDataAdd()));
	connect(actionModifyFaultData,SIGNAL(triggered()),this,SLOT(openFaultDataModify()));
	connect(actionDelFaultData,SIGNAL(triggered()),this,SLOT(delFaultData()));
	displayFaultData();
}
void CFaultDataTable::setControlAtt()
{
	listFaultData->setSelectionBehavior(QAbstractItemView::SelectRows);
	listFaultData->setSelectionMode(QAbstractItemView::SingleSelection); 
	listFaultData->setEditTriggers(QTableWidget::NoEditTriggers);
	listFaultData->setSortingEnabled(true);
}
void CFaultDataTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	faultdataadd->loadChinese();
	faultdatamodify->loadChinese();
}

void CFaultDataTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	faultdataadd->loadEnglish();
	faultdatamodify->loadEnglish();
}
void CFaultDataTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listFaultData->itemAt(listFaultData->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddFaultData);
		popMenu->addAction(actionModifyFaultData);
		popMenu->addAction(actionDelFaultData);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单

int CFaultDataTable::displayFaultData()
{
	DB_FMT data;
	InitDbData(&data,TABLE_FAULT_DATA);
	data.cond_flag.flag_fault_data.fault_id = TRUE;
	data.cond_seg.seg_fault_data.fault_id=curFaultId;
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return -1;
	}
	listFaultData->setRowCount(data.rownum);
	for(int j=0;j<data.rownum;j++)
	{
		listFaultData->setItem(j,0,new QTableWidgetItem(QString::fromLocal8Bit(data_fault_data[j].logicalname)));
		if (data_fault_data[j].type == 0)
		{
			listFaultData->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit("简单故障")));
		}
		else if (data_fault_data[j].type == 1)
		{
			listFaultData->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit("动态故障")));
		}
		listFaultData->setItem(j,2,new QTableWidgetItem(QString::number(data_fault_data[j].init_value)));
		listFaultData->setItem(j,3,new QTableWidgetItem(QString::number(data_fault_data[j].dst_value)));
		listFaultData->setItem(j,4,new QTableWidgetItem(QString::number(data_fault_data[j].norm_value)));
		listFaultData->setItem(j,5,new QTableWidgetItem(QString::number(data_fault_data[j].chg_time)));
		listFaultData->setItem(j,6,new QTableWidgetItem(QString::number(data_fault_data[j].chg_mode)));
		listFaultData->setItem(j,7,new QTableWidgetItem(QString::number(data_fault_data[j].delay)));
	}
	listFaultData->clearSelection();
	select_status = false;
	if (data.rownum != 0)
	{
		listFaultData->resizeColumnsToContents();
	}
	return data.rownum;
}
void CFaultDataTable::openFaultDataAdd()
{
	faultdataadd = new CFaultDataAddTable(this);
	faultdataadd->flag_add=true;
	faultdataadd->buttonApply->setVisible(false);
	faultdataadd->show();
	connect(faultdataadd,SIGNAL(insertRecord()),this,SLOT(addFaultData()));
}
void CFaultDataTable::openFaultDataModify()
{
	int fault_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的故障数据!"),QMessageBox::Ok);
	}
	else
	{
		faultdatamodify = new CFaultDataAddTable(this);
		faultdatamodify->flag_modify=true;	
		QTableWidgetItem* logicalname = listFaultData->item(curRow,0);
		faultdatamodify->editLogicalName->setText(logicalname->text());
		if(listFaultData->item(curRow,1)->text() == QString::fromLocal8Bit("简单故障"))
		{
			faultdatamodify->editType->setCurrentIndex(0);
		}
		else if(listFaultData->item(curRow,1)->text() == QString::fromLocal8Bit("动态故障"))
		{
			faultdatamodify->editType->setCurrentIndex(1);
		}
		QTableWidgetItem* initvalue = listFaultData->item(curRow,2);
		faultdatamodify->editInitValue->setValue(initvalue->text().toFloat());
		QTableWidgetItem* dstvalue = listFaultData->item(curRow,3);
		faultdatamodify->editDstValue->setValue(dstvalue->text().toFloat());
		QTableWidgetItem* normvalue = listFaultData->item(curRow,4);
		faultdatamodify->editNormValue->setValue(normvalue->text().toFloat());
		QTableWidgetItem* chgtime = listFaultData->item(curRow,5);
		faultdatamodify->editChgTime->setValue(chgtime->text().toInt());
		QTableWidgetItem* chgmode = listFaultData->item(curRow,6);
		faultdatamodify->editChgMode->setText(chgmode->text());
		QTableWidgetItem* delay = listFaultData->item(curRow,7);
		faultdatamodify->editDelayTime->setValue(delay->text().toInt());
		faultdatamodify->setWindowTitle(QString::fromLocal8Bit("修改故障数据"));
		faultdatamodify->show();
		connect(faultdatamodify,SIGNAL(modifyRecord()),this,SLOT(modifyFaultData()));
	}
}
void CFaultDataTable::addFaultData()
{
	DB_FMT data;
	InitDbData(&data,TABLE_FAULT_DATA);
	//设置表中每个字段的flag
	data.flag.flag_fault_data.fault_id =TRUE;
	data.flag.flag_fault_data.logicalname=TRUE;
	data.flag.flag_fault_data.type=TRUE;
	data.flag.flag_fault_data.init_value=TRUE;
	data.flag.flag_fault_data.dst_value=TRUE;
	data.flag.flag_fault_data.norm_value=TRUE;
	data.flag.flag_fault_data.chg_time=TRUE;
	data.flag.flag_fault_data.chg_mode=TRUE;
	data.flag.flag_fault_data.delay=TRUE;

	//填充表中每个字段的值

	faultdataadd->seg.fault_id = curFaultId;
	data.seg.seg_fault_data = faultdataadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	//	QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入故障数据失败!"),QMessageBox::Ok);		
	}

	displayFaultData();

}
void CFaultDataTable::delFaultData()
{
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要删除的故障数据!"),QMessageBox::Ok);
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
		data.cond_flag.flag_fault_data.fault_id=TRUE;
		data.cond_flag.flag_fault_data.logicalname=TRUE;

		data.cond_seg.seg_fault_data.fault_id = curFaultId;
		QTableWidgetItem* logicalname = listFaultData->item(curRow,0);
		strcpy(data.cond_seg.seg_fault_data.logicalname,logicalname->text().toLocal8Bit().data());
		if( DeleteOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error); 
			return;
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除故障数据失败!"),QMessageBox::Ok);		
		}
		displayFaultData();
	}
}
void CFaultDataTable::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CFaultDataTable::modifyFaultData()
{

	DB_FMT data;
	InitDbData(&data,TABLE_FAULT_DATA);

	//设置表中每个字段的flag
	data.flag.flag_fault_data.logicalname=TRUE;
	data.flag.flag_fault_data.type=TRUE;
	data.flag.flag_fault_data.init_value=TRUE;
	data.flag.flag_fault_data.dst_value=TRUE;
	data.flag.flag_fault_data.norm_value=TRUE;
	data.flag.flag_fault_data.chg_time=TRUE;
	data.flag.flag_fault_data.chg_mode=TRUE;
	data.flag.flag_fault_data.delay=TRUE;

	//填充表中每个字段的值
	data.seg.seg_fault_data = faultdatamodify->seg;

	//设置查询条件
	data.cond_flag.flag_fault_data.logicalname = TRUE;
	data.cond_flag.flag_fault_data.fault_id = TRUE;

	data.cond_seg.seg_fault_data.fault_id = curFaultId;
	QTableWidgetItem* logicalname = listFaultData->item(curRow,0);
	strcpy(data.cond_seg.seg_fault_data.logicalname,logicalname->text().toLocal8Bit().data());
	if( UpdateOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改故障数据失败!"),QMessageBox::Ok);		
	}
	displayFaultData();
}
/************************************************CFaultDataAddTable*******************************************************/
CFaultDataAddTable::CFaultDataAddTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	flag_add=false;
	flag_modify=false;	
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(buttonPointName,SIGNAL(clicked()),this,SLOT(openPointName()));
	connect(editType,SIGNAL(currentIndexChanged(int)),this,SLOT(changeType(int)));
	editType->addItem(QString::fromLocal8Bit("简单故障"));
	editType->addItem(QString::fromLocal8Bit("动态故障"));
}
void CFaultDataAddTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CFaultDataAddTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CFaultDataAddTable::changeType(int index)
{
	if (index == 0)
	{
		editInitValue->setEnabled(false);
		editDstValue->setEnabled(false);
		editInitValue->cleanText();
		editDstValue->cleanText();
	}
	else if(index == 1)
	{
		editInitValue->setEnabled(true);
		editDstValue->setEnabled(true);
	}
}

void CFaultDataAddTable::setPointName(const QString &pointName)
{
	editLogicalName->setText(pointName);
}

void CFaultDataAddTable::openPointName()
{
	pointNameTable = new CPointNameTable(this);
	pointNameTable->show();
	pointNameTable->InitTreeView();
	pointNameTable->SetSelected(editLogicalName->text());
	connect(pointNameTable,SIGNAL(sig_curPointName(const QString &)),this,SLOT(setPointName(const QString &)));
}

void CFaultDataAddTable::apply()
{		
	strcpy(seg.logicalname,editLogicalName->text().toLocal8Bit().data());	
	seg.type = editType->currentIndex();
	seg.init_value = editInitValue->value();
	seg.dst_value = editDstValue->value();
	seg.norm_value = editNormValue->value();
	seg.chg_time = editChgTime->value();
	seg.chg_mode = editChgMode->text().toInt();
	seg.delay = editDelayTime->value();
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}

void CFaultDataAddTable::enter()
{
	apply();
	editLogicalName->clear();
	editInitValue->clear();
	editDstValue->clear();
	editNormValue->clear();
	editChgTime->clear();
	editChgMode->clear();
	editDelayTime->clear();
	this->close();
	flag_add=false;
	flag_modify=false;	
}