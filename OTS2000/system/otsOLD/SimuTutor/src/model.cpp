#include "model.h"
#include <QMessageBox>

CModelForm::CModelForm(QWidget* parent)
:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	setControlAtt();
	select_status = false;
	select_stn = false;
	select_stndev = false;
	actionModelParam = new QAction(QString::fromLocal8Bit("模型参数"), this);
	actionAddModel = new QAction(QString::fromLocal8Bit("添加模型"), this);
	actionModifyModel = new QAction(QString::fromLocal8Bit("修改模型"), this);
	actionDelModel = new QAction(QString::fromLocal8Bit("删除模型"), this);
	stndev = new CStnDevTable(this);
	modelConfig = new QProcess(this);
	//rownums = displayModel();
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(openModelAdd()));
	connect(buttonModify,SIGNAL(clicked()),this,SLOT(openModelModify()));
	connect(buttonParam,SIGNAL(clicked()),this,SLOT(openModelParam()));
	connect(buttonDel,SIGNAL(clicked()),this,SLOT(delModel()));
	connect(listModel,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(actionModelParam,SIGNAL(triggered()),this,SLOT(openModelParam()));
	connect(actionAddModel,SIGNAL(triggered()),this,SLOT(openModelAdd()));
	connect(actionModifyModel,SIGNAL(triggered()),this,SLOT(openModelModify()));
	connect(actionDelModel,SIGNAL(triggered()),this,SLOT(delModel()));
	connect(buttonModelConfig,SIGNAL(clicked()),this,SLOT(openModelConfig()));
	connect(buttonModelInto,SIGNAL(clicked()),this,SLOT(intoModel()));
	connect(buttonStnDev,SIGNAL(clicked()),this,SLOT(openStnDev()));
	connect(buttonExit,SIGNAL(clicked()),stndev,SLOT(close()));
	connect(stndev,SIGNAL(update(int)),this,SLOT(displayUpdate(int)));
	connect(stndev,SIGNAL(update(int,int)),this,SLOT(displayUpdate(int,int)));
}
void CModelForm::setControlAtt()
{
	listModel->setSelectionBehavior(QAbstractItemView::SelectRows);
	listModel->setSelectionMode(QAbstractItemView::SingleSelection); 
	listModel->setEditTriggers(QTableWidget::NoEditTriggers);
	listModel->setSortingEnabled(true);
}
void CModelForm::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	modeladd->loadChinese();
	modelmodify->loadChinese();
	modelparam->loadChinese();
}
void CModelForm::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	modeladd->loadEnglish();
	modelmodify->loadEnglish();
	modelparam->loadEnglish();
}
void CModelForm::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listModel->itemAt(listModel->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionModelParam);
		popMenu->addAction(actionAddModel);
		popMenu->addAction(actionModifyModel);
		popMenu->addAction(actionDelModel);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CModelForm::moveEvent ( QMoveEvent * event )
{
	if (stndev != NULL )
	{
		stndev->mvwith = true;
		stndev->newpt= event->pos();
		this->pos() = event->pos();
		stndev->moveEvent(event);
	}	
}
void CModelForm::closeEvent(QCloseEvent *event)
{
	event->accept();
	stndev->close();
}
void CModelForm::openModelAdd()
{
	modeladd = new CModelAddForm(this);
	modeladd->flag_add=true;
	modeladd->buttonApply->setVisible(false);
	modeladd->show();
	connect(modeladd,SIGNAL(insertRecord()),this,SLOT(addModel()));
}
void CModelForm::openStnDev()
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
		displayModel();
	}
}
void CModelForm::openModelModify()
{
	int model_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的模型!"),QMessageBox::Ok);
	}
	else
	{
		modelmodify = new CModelAddForm(this);
		modelmodify->flag_modify=true;
		QTableWidgetItem* item_id = listModel->item(curRow,0);
		model_id = item_id->text().toInt();
		curModelId=model_id;
		QTableWidgetItem* name = listModel->item(curRow,1);
		modelmodify->editName->setText(name->text());
		QTableWidgetItem* longname = listModel->item(curRow,2);
		modelmodify->editLongName->setText(longname->text());
		QTableWidgetItem* station = listModel->item(curRow,4);
		if (station->text() == QString(""))
		{
			modelmodify->editStation->setCurrentIndex(0);
		}
		else
		{
			modelmodify->editStation->setCurrentIndex(modelmodify->editStation->findText(station->text(),Qt::MatchContains));
		}
		QTableWidgetItem* dev = listModel->item(curRow,5);
		if (dev->text() == QString(""))
		{
			modelmodify->editDev->setCurrentIndex(0);
		}
		else
		{
			modelmodify->editDev->setCurrentIndex(modelmodify->editDev->findText(dev->text(),Qt::MatchContains));
		}
		modelmodify->setWindowTitle(QString::fromLocal8Bit("修改模型"));
		modelmodify->show();
		connect(modelmodify,SIGNAL(modifyRecord()),this,SLOT(modifyModel()));
	}

}

void CModelForm::openModelParam()
{
	int model_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要设置参数的模型!"),QMessageBox::Ok);
	}
	else
	{
		modelparam = new CModelParamForm(this);
		QTableWidgetItem* item_id = listModel->item(curRow,0);
		model_id = item_id->text().toInt();
		modelparam->curModelId=model_id;
		modelparam->displayModelParam();
		modelparam->show();
	}
}


void CModelForm::addModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_MODEL);

	//设置表中每个字段的flag
	data.flag.flag_model.model_id =TRUE;
	data.flag.flag_model.name=TRUE;
	data.flag.flag_model.datetime=TRUE;
	data.flag.flag_model.dev_id=TRUE;
	data.flag.flag_model.station_id=TRUE;
	data.flag.flag_model.long_name=TRUE;

	//填充表中每个字段的值

	modeladd->seg.model_id = GetInexistenceIdInHdb(TABLE_MODEL);
	GetCurTimeStr( modeladd->seg.datetime);
	data.seg.seg_model = modeladd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}

	rownums = displayModel();

}
void CModelForm::delModel()
{
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要删除的模型!"),QMessageBox::Ok);
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
		InitDbData(&data,TABLE_VAR_RELATION);
		QTableWidgetItem* flag_model_id = listModel->item(curRow,0);
		data.cond_flag.flag_var_relation.model_id=TRUE;
		data.cond_seg.seg_var_relation.model_id = flag_model_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
		InitDbData(&data,TABLE_MODEL);
		data.cond_flag.flag_model.model_id=TRUE;
		data.cond_seg.seg_model.model_id = flag_model_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
		rownums = displayModel();
	}
}
void CModelForm::displayUpdate(int stn_id)
{
	select_stn = true;
	stnId = stn_id;
	displayModel();
}
void CModelForm::displayUpdate(int stn_id,int dev_id)
{
	select_stndev = true;
	stnId = stn_id;
	devId = dev_id;
	displayModel();
}
int CModelForm::displayModel()
{
	DB_FMT data;
	unsigned char stn_id,dev_id;
	InitDbData(&data,TABLE_MODEL);
	if (select_stn)
	{
		data.cond_flag.flag_model.station_id = TRUE;
		data.cond_seg.seg_model.station_id = stnId;
	}
	if (select_stndev)
	{
		data.cond_flag.flag_model.station_id = TRUE;
		data.cond_seg.seg_model.station_id = stnId;
		data.cond_flag.flag_model.dev_id = TRUE;
		data.cond_seg.seg_model.dev_id = devId;
	}
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	listModel->setRowCount(data.rownum);
	for(int j=0;j<data.rownum;j++)
	{
		listModel->setItem(j,0,new QTableWidgetItem(QString::number(data_model[j].model_id)));
		listModel->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit(data_model[j].name)));
		listModel->setItem(j,2,new QTableWidgetItem(QString::fromLocal8Bit(data_model[j].long_name)));
		listModel->setItem(j,3,new QTableWidgetItem(QString::fromLocal8Bit(data_model[j].datetime)));
		stn_id = data_model[j].station_id;
		dev_id = data_model[j].dev_id;
		listModel->setItem(j,4,new QTableWidgetItem(QString::fromLocal8Bit(data_station[stn_id].station_longname )));
		listModel->setItem(j,5,new QTableWidgetItem(QString::fromLocal8Bit(data_dev[stn_id][dev_id].dev_longname)));
	}
	select_status = false;
	select_stn = false;
	select_stndev = false;
	listModel->clearSelection();
	if (data.rownum != 0)
	{
		listModel->resizeColumnsToContents();
	}
	return data.rownum;
}

void CModelForm::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CModelForm::modifyModel()
{

	DB_FMT data;
	InitDbData(&data,TABLE_MODEL);

	//设置表中每个字段的flag
	data.flag.flag_model.name=TRUE;
	data.flag.flag_model.long_name=TRUE;
	data.flag.flag_model.datetime=TRUE;
	data.flag.flag_model.dev_id=TRUE;
	data.flag.flag_model.station_id=TRUE;


	//填充表中每个字段的值
	GetCurTimeStr( modelmodify->seg.datetime);
	data.seg.seg_model = modelmodify->seg;

	//设置查询条件
	data.cond_flag.flag_model.model_id=TRUE;
	data.cond_seg.seg_model.model_id = curModelId;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	rownums = displayModel();
}
void CModelForm::openModelConfig()
{
	QDir otsDir;
	otsDir.setPath("/home/ems/h9000/ots");
	if (!otsDir.exists())
	{
		return;
	}
	otsDir.setFilter(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Hidden);
	otsDir.setSorting(QDir::Type);
	QStringList filters;
	filters << "*.txt";
	QFileInfoList fileList = otsDir.entryInfoList(filters);
	for (int i=0;i<fileList.size();++i)
	{
		if (fileList.at(i).baseName().contains("_mdl", Qt::CaseInsensitive))
			QDesktopServices::openUrl(QUrl::fromLocalFile(fileList.at(i).absoluteFilePath())); 
	}
}
void CModelForm::intoModel()
{
	//统一函数参数数据体
	DB_FMT data;
	DMS_COMMON	dms_rec;
	int	data_type;
	int curid;
	int i,j;
	bool flag_repeat=false;
	if ( SimulogDataLoad () == -1 )
	{
		printf ( "simulog.dat error!\n" );
	}

	//填充scene表中每个字段的值
	
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("导入模型中,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	progressDlg->setRange(0,n_model-1);
	for(i=0;i<n_model;i++)
	{
		flag_repeat=false;
		data.table_id=TABLE_MODEL;
		InitDbData(&data,data.table_id);

		data.flag.flag_model.model_id =TRUE;
		data.flag.flag_model.name =TRUE;
		data.flag.flag_model.long_name =TRUE;
		data.flag.flag_model.datetime =TRUE;
		data.flag.flag_model.station_id =TRUE;
		data.flag.flag_model.dev_id =TRUE;

		curid = GetInexistenceIdInHdb(TABLE_MODEL);
		GetCurTimeStr(data.seg.seg_model.datetime);
		data.seg.seg_model.model_id = curid;
		strcpy(data.seg.seg_model.name,mdb[i].name);
		strcpy(data.seg.seg_model.long_name,mdb[i].name);
		data.seg.seg_model.station_id =1;
		data.seg.seg_model.dev_id =1;
		for (int k=0;k<rownums;k++)
		{
			QTableWidgetItem* name= listModel->item(k,1);
			QString a =name->text();
			if (name->text()==QString::fromLocal8Bit(mdb[i].name))
			{
				flag_repeat = true;
				break;
			}
		}
		if (flag_repeat==true)
		{
			continue;
		}
		progressDlg->setValue(i); 
		if(progressDlg->wasCanceled())  
			break;
		//调用接口函数，参数为DB_FMT结构体
		if( InsertOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入模型失败!"),QMessageBox::Ok);
			HdbLogWrite(data.sql_str,data.sql_error);
			continue;;
		}

		for( j=0; j<n_param; j++ )
		{
			DB_FMT cdata;
			cdata.table_id = TABLE_VAR_RELATION;
			InitDbData(&cdata,cdata.table_id);	
			//将需要scene_data访问的字段flag置为
			cdata.flag.flag_var_relation.model_id =TRUE;
			cdata.flag.flag_var_relation.name =TRUE;
			cdata.flag.flag_var_relation.type =TRUE;
			cdata.flag.flag_var_relation.logicalname =TRUE;
			cdata.flag.flag_var_relation.data_type =TRUE;

			cdata.seg.seg_var_relation.model_id = curid;
			strcpy(cdata.seg.seg_var_relation.name,paramdb[j].param_name);
			strcpy(cdata.seg.seg_var_relation.logicalname,paramdb[j].pt_name);
			cdata.seg.seg_var_relation.data_type = 0;
			QString name(paramdb[j].param_name);
			if (name.contains("INPUT", Qt::CaseInsensitive))
			{
				cdata.seg.seg_var_relation.type = 0;
			}
			else if (name.contains("OUTPUT", Qt::CaseInsensitive))
			{
				cdata.seg.seg_var_relation.type = 1;
			}
			else
			{
				cdata.seg.seg_var_relation.type = 2;
			}
			//调用接口函数，参数为DB_FMT结构体
			if( InsertOneRec(&cdata) != TRUE)
			{
				HdbLogWrite(cdata.sql_str,cdata.sql_error);
				printf("插入scenedata数据%s 失败\n", scenesegdb[j].name);
				continue;
			}						
		}
	}
	rownums = displayModel();
}
/**********************************************CModelAddForm*****************************************************/
CModelAddForm::CModelAddForm(QWidget* parent)
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
void CModelAddForm::setControlAtt()
{
	editStation->setCurrentIndex(-1);
	editDev->setCurrentIndex(-1);
}
void CModelAddForm::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CModelAddForm::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CModelAddForm::displayStation()
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
void CModelAddForm::displayDev(int index)
{
	editDev->clear();
	editDev->insertItem(0,QString::fromLocal8Bit("无设备"));
	UCHAR stnid = editStation->itemData(index).toInt();
	for (int j=1;j<=data_station[stnid].dev_num;j++)
	{
		editDev->insertItem(j,QString::fromLocal8Bit(data_dev[stnid][j].dev_longname),QVariant(data_dev[stnid][j].dev_id));
	}
}
void CModelAddForm::apply()
{		
	strcpy(seg.name,editName->text().toLocal8Bit().data());	
	strcpy(seg.long_name,editLongName->text().toLocal8Bit().data());
	seg.station_id = editStation->itemData(editStation->currentIndex()).toInt();
	seg.dev_id = editDev->itemData(editDev->currentIndex()).toInt();
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}
void CModelAddForm::enter()
{
	apply();
	editName->clear();
	editLongName->clear();
	editStation->setCurrentIndex(-1);
	editDev->setCurrentIndex(-1);
	this->close();
	flag_add=false;
	flag_modify=false;
}
