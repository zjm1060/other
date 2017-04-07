#include "scenedata.h"

CSceneDataTable::CSceneDataTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	setControlAtt();
	this->setModal(true);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	actionAddSceneData= new QAction(QString::fromLocal8Bit("添加工况数据"), this);
	actionModifySceneData = new QAction(QString::fromLocal8Bit("修改工况数据"), this);
	actionDelSceneData = new QAction(QString::fromLocal8Bit("删除工况数据"), this);
	dataSourceModel = new QStandardItemModel(this);
	dataModel = new QSortFilterProxyModel(this);
	dataModel->setSourceModel(dataSourceModel);
	dataModel->setDynamicSortFilter(true);
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(openSceneDataAdd()));
	connect(buttonModify,SIGNAL(clicked()),this,SLOT(openSceneDataModify()));
	connect(buttonDel,SIGNAL(clicked()),this,SLOT(delSceneData()));
	connect(actionAddSceneData,SIGNAL(triggered()),this,SLOT(openSceneDataAdd()));
	connect(actionModifySceneData,SIGNAL(triggered()),this,SLOT(openSceneDataModify()));
	connect(actionDelSceneData,SIGNAL(triggered()),this,SLOT(delSceneData()));
	connect(comboStn,SIGNAL(currentIndexChanged(int)),this,SLOT(displayDev(int)));
	connect(comboDev,SIGNAL(currentIndexChanged(int)),this,SLOT(displayType(int)));
	connect(comboType,SIGNAL(currentIndexChanged(int)),this,SLOT(setFilter(int)));
	connect(buttonSearch,SIGNAL(clicked()),this,SLOT(searchFromName()));
	connect(buttonShowAll,SIGNAL(clicked()),this,SLOT(displayAll()));
	displayStn();
}
void CSceneDataTable::showEvent(QShowEvent *event)
{
	setDataSourceModel();
	comboStn->setCurrentIndex(0);
	editName->clear();
	event->accept();
}
void  CSceneDataTable::setLanguage(int language)
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
	setDataSourceModel();
	comboStn->setCurrentIndex(0);
	editName->clear();
}
void CSceneDataTable::setControlAtt()
{
	viewSceneData->setSelectionBehavior(QAbstractItemView::SelectRows);
	viewSceneData->setEditTriggers(QTableView::NoEditTriggers);
	//viewSceneData->verticalHeader()->setVisible(false);
	viewSceneData->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	viewSceneData->setSortingEnabled(true);
}
void CSceneDataTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	scenedataadd->loadChinese();
// 	scenedatamodify->loadChinese();
}
void CSceneDataTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	scenedataadd->loadEnglish();
// 	scenedatamodify->loadEnglish();
}
void CSceneDataTable::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	popMenu->addAction(actionAddSceneData);
	popMenu->addAction(actionModifySceneData);
	popMenu->addAction(actionDelSceneData);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单

void CSceneDataTable::displayStn()
{
	GULONG		stn_num;
	char	stn_comment[STATION_COMMENT_SIZE];
	GetStationNum(&stn_num);
	comboStn->insertItem(0,QString::fromLocal8Bit("所有厂站"));
	for(int i=1,j=1; i<=stn_num; i++) 
	{  		
		UCHAR stn_id;
		GetStnIdByIndex(i, &stn_id);

		if( GetStnCommentById(stn_id,(char*)( stn_comment))==-1 )
			continue;
		comboStn->insertItem(j,QString::fromLocal8Bit(stn_comment),QVariant(stn_id));
		j++;
	}
}
void CSceneDataTable::displayDev(int index)
{
	comboDev->clear();
	if (index==0)
	{
		comboDev->insertItem(0,QString::fromLocal8Bit("所有设备"));
	}
	else
	{
		UCHAR stn_id,dev_id;
		GULONG		grp_num;
		int i;
		char	dev_comment[GROUP_COMMENT_SIZE];
		stn_id = comboStn->itemData(comboStn->currentIndex()).toInt();
		GetGroupNumById(stn_id, &grp_num);
		comboDev->insertItem(0,QString::fromLocal8Bit("所有设备"));
		for(dev_id=1,i=1; dev_id<=grp_num; dev_id++) 
		{  	
			if(-1==GetGroupCommentById(stn_id, dev_id, dev_comment))
				continue;
			comboDev->insertItem(i,QString::fromLocal8Bit(dev_comment),QVariant(dev_id));
			i++;
		}
	}
}
void CSceneDataTable::displayType(int index)
{
	comboType->clear();
	if (index==0)
	{
		comboType->insertItem(0,QString::fromLocal8Bit("所有类型"));
	}
	else
	{
		UCHAR stn_id,dev_id,data_type;
		GULONG rec_num,entry_num;
		int j;
		char	datatype_longname[RECORD_LONGNAME_SIZE];
		stn_id = comboStn->itemData(comboStn->currentIndex()).toInt();
		dev_id = comboDev->itemData(comboDev->currentIndex()).toInt();
		if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
			return;
		comboType->insertItem(0,QString::fromLocal8Bit("所有类型"));
		for(data_type=1,j=0; j<rec_num; data_type++) 
		{
			if(data_type>=MAX_DATATYPE_NUM)
				break;
			if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
				continue;
			GetRecLongnameByType(data_type, datatype_longname);
			comboType->insertItem(data_type,QString::fromLocal8Bit(datatype_longname),QVariant(data_type));
			j++;
		}
	}
}
void CSceneDataTable::searchFromName()
{
	QString rx_str;
	rx_str = editName->text();
	QRegExp rx(rx_str,Qt::CaseInsensitive,QRegExp::FixedString);
	dataModel->setFilterRegExp(rx);
	comboStn->setCurrentIndex(0);
}
void CSceneDataTable::displayAll()
{
	dataModel->setFilterRegExp(QRegExp("/*"));
}

void CSceneDataTable::setFilter(int index)
{
	char	stn_name[STATION_NAME_SIZE];
	char	dev_name[GROUP_NAME_SIZE];
	char	datatype_name[RECORD_NAME_SIZE];
	UCHAR stn_id,dev_id,data_type;
	QString rx_str;
	if (comboStn->currentIndex()!=0)
	{
		stn_id = comboStn->itemData(comboStn->currentIndex()).toInt();
		GetStnNameById(stn_id,stn_name);
		if (comboDev->currentIndex()!=0)
		{
			dev_id = comboDev->itemData(comboDev->currentIndex()).toInt();
			GetGroupNameById(stn_id,dev_id,dev_name);
			if (comboType->currentIndex()!=0)
			{
				data_type = comboType->itemData(comboType->currentIndex()).toInt();
				GetRecNameByType(data_type,datatype_name);
				rx_str = QString("%1.%2.%3.").arg(QString::fromLocal8Bit(stn_name)).arg(QString::fromLocal8Bit(dev_name)).arg(QString::fromLocal8Bit(datatype_name));
				QRegExp rx(rx_str,Qt::CaseInsensitive,QRegExp::FixedString);
				dataModel->setFilterRegExp(rx);
			}
			else
			{
				rx_str = QString("%1.%2.").arg(QString::fromLocal8Bit(stn_name)).arg(QString::fromLocal8Bit(dev_name));
				QRegExp rx(rx_str,Qt::CaseInsensitive,QRegExp::FixedString);
				dataModel->setFilterRegExp(rx);
			}
		}
		else
		{	
			rx_str = QString("%1.").arg(QString::fromLocal8Bit(stn_name));
			QRegExp rx(rx_str,Qt::CaseInsensitive,QRegExp::FixedString);
			dataModel->setFilterRegExp(rx);
		}
	}
	else
	{
		rx_str = QString("/*");
		dataModel->setFilterRegExp(QRegExp(rx_str));
	}
}
void CSceneDataTable::openSceneDataAdd()
{
	scenedataadd = new CSceneDataAddTable(this);
	scenedataadd->flag_add=true;
	scenedataadd->buttonApply->setVisible(false);
	scenedataadd->setModal(true);
	scenedataadd->show();
	connect(scenedataadd,SIGNAL(insertRecord()),this,SLOT(addSceneData()));
}
void CSceneDataTable::openSceneDataModify()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() != 0)
	{
		scenedatamodify = new CSceneDataAddTable(this);
		int scene_id;
		int row;
		QString logicalname,status,data;
		scenedatamodify->editLogicalName->setDisabled(true);
		scenedatamodify->buttonPointName->setDisabled(true);
		scenedatamodify->flag_modify=true;
		row = indexes.at(0).row();
		logicalname = dataModel->data(dataModel->index(row,0)).toString();
		status = dataModel->data(dataModel->index(row,1)).toString();
		data = dataModel->data(dataModel->index(row,2)).toString();
		scenedatamodify->editLogicalName->setText(logicalname);
		scenedatamodify->editStatus->setText(status);
		scenedatamodify->editData->setText(data);
		scenedatamodify->setWindowTitle(QString::fromLocal8Bit("修改工况数据"));
		scenedatamodify->setModal(true);
		scenedatamodify->show();
		connect(scenedatamodify,SIGNAL(modifyRecord()),this,SLOT(modifySceneData()));
	}
}
void CSceneDataTable::addSceneData()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SCENE_DATA);
	//设置表中每个字段的flag
	data.flag.flag_scene_data.scene_id =TRUE;
	data.flag.flag_scene_data.logicalname=TRUE;
	data.flag.flag_scene_data.status=TRUE;
	data.flag.flag_scene_data.data=TRUE;

	//填充表中每个字段的值

	scenedataadd->seg.scene_id = curSceneId;
	data.seg.seg_scene_data = scenedataadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(scenedataadd->seg.logicalname));  
	QStandardItem* item1 = new QStandardItem(QString::number(scenedataadd->seg.status));  
	QStandardItem* item2 = new QStandardItem(QString::number(scenedataadd->seg.data));  
	item0->setTextAlignment(Qt::AlignLeft);
	item1->setTextAlignment(Qt::AlignCenter);
	item2->setTextAlignment(Qt::AlignCenter);
	QList<QStandardItem*> items;  
	items << item0 << item1 << item2; 
	dataSourceModel->appendRow(items);
}

void CSceneDataTable::modifySceneData()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size() > 1)
	{
		QMessageBox msgBox(this);
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle (QString::fromLocal8Bit("警告!"));
		msgBox.setInformativeText(QString::fromLocal8Bit("您选择了多条工况数据，如果确认修改会修改所有选中数据，请谨慎修改，如果不需要修改请点击取消!"));
		msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Cancel) 
		{
			return;
		}
	}
	for (int i=indexes.size()-1;i>=0;i--)
	{
		int row = indexes.at(i).row();
		QString logicalname;
		DB_FMT data;
		InitDbData(&data,TABLE_SCENE_DATA);

		//设置表中每个字段的flag
		data.flag.flag_scene_data.status=TRUE;
		data.flag.flag_scene_data.data=TRUE;

		//填充表中每个字段的值
		data.seg.seg_scene_data = scenedatamodify->seg;

		//设置查询条件
		data.cond_flag.flag_scene_data.scene_id = TRUE;
		data.cond_flag.flag_scene_data.logicalname = TRUE;
		data.cond_seg.seg_scene_data.scene_id = curSceneId;
		logicalname = dataModel->data(dataModel->index(row,0)).toString();
		strcpy(data.cond_seg.seg_scene_data.logicalname,logicalname.toLocal8Bit().data());
		if( UpdateOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		} 
		dataModel->setData(dataModel->index(row,1),QVariant(scenedatamodify->seg.status));
		dataModel->setData(dataModel->index(row,2),QVariant(scenedatamodify->seg.data));
	}
}

void CSceneDataTable::delSceneData()
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
		delSceneDataFromHdb(indexes.at(i).row());
		dataModel->removeRow(indexes.at(i).row());
	}
}

void CSceneDataTable::delSceneDataFromHdb(int row)
{
	QString logicalname;
	DB_FMT data;
	InitDbData(&data,TABLE_SCENE_DATA);
	data.cond_flag.flag_scene_data.scene_id=TRUE;
	data.cond_flag.flag_scene_data.logicalname=TRUE;
	data.cond_seg.seg_scene_data.scene_id = curSceneId;
	logicalname = dataModel->data(dataModel->index(row,0)).toString();
	strcpy(data.cond_seg.seg_scene_data.logicalname,logicalname.toLocal8Bit().data());
	if( DeleteOneRec(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}

void CSceneDataTable::setDataSourceModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SCENE_DATA);
	data.cond_flag.flag_scene_data.scene_id = TRUE;
	data.cond_seg.seg_scene_data.scene_id=curSceneId;
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal); 
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("搜索数据库,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	viewSceneData->setModel(NULL);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return;
	}
	progressDlg->setRange(0,data.rownum-1);
	dataSourceModel->clear();
	dataSourceModel->setColumnCount(3);  
	dataSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("点逻辑名"));  
	dataSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("状态")); 
	dataSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("数据"));
	for (int i=0;i<data.rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit(data_scene_data[i].logicalname));  
		QStandardItem* item1 = new QStandardItem(QString::number(data_scene_data[i].status));  
		QStandardItem* item2 = new QStandardItem(QString::number(data_scene_data[i].data));  
		item0->setTextAlignment(Qt::AlignLeft);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2; 
		dataSourceModel->appendRow(items); 
		progressDlg->setValue(i); 
		if(progressDlg->wasCanceled())  
			break;
	}
	viewSceneData->setModel(dataModel);
	selectionModel = viewSceneData->selectionModel();
	viewSceneData->setColumnWidth(0,230);
	viewSceneData->setColumnWidth(1,33);
	viewSceneData->setColumnWidth(2,65);
}
/*******************************************************CSceneDataAddTable********************************************************/
CSceneDataAddTable::CSceneDataAddTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	flag_add=false;
	flag_modify=false;
	this->setWindowModality(Qt::WindowModal);  
	connect(buttonApply,SIGNAL(clicked()),this,SLOT(apply()));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(enter()));
	connect(buttonPointName,SIGNAL(clicked()),this,SLOT(openPointName()));
}
void CSceneDataAddTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void  CSceneDataAddTable::setLanguage(int language)
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
void CSceneDataAddTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CSceneDataAddTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CSceneDataAddTable::setPointName(const QString &pointName)
{
	editLogicalName->setText(pointName);
}

void CSceneDataAddTable::openPointName()
{
	pointNameTable = new CPointNameTable(this);
	pointNameTable->show();
	pointNameTable->InitTreeView();
	pointNameTable->SetSelected(editLogicalName->text());
	connect(pointNameTable,SIGNAL(sig_curPointName(const QString &)),this,SLOT(setPointName(const QString &)));
}

void CSceneDataAddTable::apply()
{		
	strcpy(seg.logicalname,editLogicalName->text().toLocal8Bit().data());	
	seg.status = editStatus->text().toInt();
	seg.data = editData->text().toFloat();
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}

void CSceneDataAddTable::enter()
{
	apply();
	editLogicalName->clear();
	editStatus->clear();
	editData->clear();
	this->close();
	flag_add=false;
	flag_modify=false;
}
