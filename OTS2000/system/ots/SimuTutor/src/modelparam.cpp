#include "modelparam.h"
#include <QMessageBox>

CModelParamForm::CModelParamForm(QWidget* parent)
:QDialog(parent)
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
	actionAddModelParam = new QAction(QString::fromLocal8Bit("添加模型参数"), this);
	actionModifyModelParam = new QAction(QString::fromLocal8Bit("修改模型参数"), this);
	actionDelModelParam = new QAction(QString::fromLocal8Bit("删除模型参数"), this);
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(openModelParamAdd()));
	connect(buttonModify,SIGNAL(clicked()),this,SLOT(openModelParamModify()));
	connect(buttonDel,SIGNAL(clicked()),this,SLOT(delModelParam()));
	connect(listModelParam,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));
	connect(actionAddModelParam,SIGNAL(triggered()),this,SLOT(openModelParamAdd()));
	connect(actionModifyModelParam,SIGNAL(triggered()),this,SLOT(openModelParamModify()));
	connect(actionDelModelParam,SIGNAL(triggered()),this,SLOT(delModelParam()));
	displayModelParam();
}

void CModelParamForm::setControlAtt()
{
	listModelParam->setSelectionBehavior(QAbstractItemView::SelectRows);
	listModelParam->setSelectionMode(QAbstractItemView::SingleSelection); 
	listModelParam->setEditTriggers(QTableWidget::NoEditTriggers);
	listModelParam->setSortingEnabled(true);
}
void CModelParamForm::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=listModelParam->itemAt(listModelParam->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddModelParam);
		popMenu->addAction(actionModifyModelParam);
		popMenu->addAction(actionDelModelParam);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CModelParamForm::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	modelparamadd->loadChinese();
// 	modelparammodify->loadChinese();
}
void CModelParamForm::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
// 	modelparamadd->loadEnglish();
// 	modelparammodify->loadEnglish();
}
void CModelParamForm::showEvent(QShowEvent *e)
{
	e->accept();
}
void  CModelParamForm::setLanguage(int language)
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
void CModelParamForm::openModelParamAdd()
{
	modelparamadd = new CModelParamAddForm(this);
	modelparamadd->flag_add=true;
	modelparamadd->buttonApply->setVisible(false);
	modelparamadd->show();
	connect(modelparamadd,SIGNAL(insertRecord()),this,SLOT(addModelParam()));
}

void CModelParamForm::openModelParamModify()
{
	int model_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的模型参数!"),QMessageBox::Ok);
	}
	else
	{
		modelparammodify = new CModelParamAddForm(this);
		modelparammodify->flag_modify=true;
		QTableWidgetItem* name = listModelParam->item(curRow,0);
		modelparammodify->editName->setText(name->text());
		QTableWidgetItem* type = listModelParam->item(curRow,1);
		if(type->text()==QString::fromLocal8Bit("输入"))
		{
			modelparammodify->editType->setCurrentIndex(0);
		}
		else if(type->text()==QString::fromLocal8Bit("输出"))
		{
			modelparammodify->editType->setCurrentIndex(1);
		}
		else if(type->text()==QString::fromLocal8Bit("内部参数"))
		{
			modelparammodify->editType->setCurrentIndex(2);
		}
		QTableWidgetItem* logicalname = listModelParam->item(curRow,2);
		modelparammodify->editLogicalname->setText(logicalname->text());
		QTableWidgetItem* datatype = listModelParam->item(curRow,3);
		if(datatype->text()==QString::fromLocal8Bit("H9000点名"))
		{
			modelparammodify->editDataType->setCurrentIndex(0);
		}
		else if(datatype->text()==QString::fromLocal8Bit("常值"))
		{
			modelparammodify->editDataType->setCurrentIndex(1);
		}
		modelparammodify->setWindowTitle(QString::fromLocal8Bit("修改模型参数"));
		modelparammodify->show();
		connect(modelparammodify,SIGNAL(modifyRecord()),this,SLOT(modifyModelParam()));
	}
}
void CModelParamForm::addModelParam()
{
	DB_FMT data;
	InitDbData(&data,TABLE_VAR_RELATION);
	//设置表中每个字段的flag
	data.flag.flag_var_relation.model_id =TRUE;
	data.flag.flag_var_relation.type=TRUE;
	data.flag.flag_var_relation.name=TRUE;
	data.flag.flag_var_relation.data_type=TRUE;
	data.flag.flag_var_relation.logicalname=TRUE;
	//data.flag.flag_var_relation.remark=TRUE;

	//填充表中每个字段的值

	modelparamadd->seg.model_id = curModelId;
	data.seg.seg_var_relation = modelparamadd->seg;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}

	displayModelParam();

}
void CModelParamForm::delModelParam()
{
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要删除的模型参数!"),QMessageBox::Ok);
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

		data.cond_flag.flag_var_relation.model_id=TRUE;
		data.cond_flag.flag_var_relation.name=TRUE;
		data.cond_seg.seg_var_relation.model_id = curModelId;

		QTableWidgetItem* modelparam_name = listModelParam->item(curRow,0);
		strcpy(data.cond_seg.seg_var_relation.name,modelparam_name->text().toLocal8Bit().data());

		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
		displayModelParam();
	}
}
int CModelParamForm::displayModelParam()
{
	DB_FMT data;
	QString TypeStr,DataTypeStr;
	unsigned char stn_id,dev_id;
	InitDbData(&data,TABLE_VAR_RELATION);
	data.cond_flag.flag_var_relation.model_id = TRUE;
	data.cond_seg.seg_var_relation.model_id=curModelId;
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	listModelParam->setRowCount(data.rownum);
	for(int j=0;j<data.rownum;j++)
	{
		listModelParam->setItem(j,0,new QTableWidgetItem(QString::fromLocal8Bit(data_var_relation[j].name)));
		if(data_var_relation[j].type == 0)
		{
			TypeStr=QString::fromLocal8Bit("输入");
		}
		else if(data_var_relation[j].type == 1)
		{
			TypeStr=QString::fromLocal8Bit("输出");
		}
		else if(data_var_relation[j].type == 2)
		{
			TypeStr=QString::fromLocal8Bit("内部参数");
		}
		listModelParam->setItem(j,1,new QTableWidgetItem(TypeStr));
		listModelParam->setItem(j,2,new QTableWidgetItem(QString::fromLocal8Bit(data_var_relation[j].logicalname)));;
		if(data_var_relation[j].data_type == 0)
		{
			DataTypeStr=QString::fromLocal8Bit("H9000点名");
		}
		else if(data_var_relation[j].data_type == 1)
		{
			DataTypeStr=QString::fromLocal8Bit("常值");
		}

		listModelParam->setItem(j,3,new QTableWidgetItem(DataTypeStr));
	}
	select_status = false;
	listModelParam->clearSelection();
	if (data.rownum != 0)
	{
		listModelParam->resizeColumnsToContents();
	}	
	return data.rownum;
}

void CModelParamForm::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void CModelParamForm::modifyModelParam()
{

	DB_FMT data;
	InitDbData(&data,TABLE_VAR_RELATION);

	//设置表中每个字段的flag
	data.flag.flag_var_relation.name=TRUE;
	data.flag.flag_var_relation.type=TRUE;
	data.flag.flag_var_relation.logicalname=TRUE;
	data.flag.flag_var_relation.data_type=TRUE;


	//填充表中每个字段的值
	data.seg.seg_var_relation = modelparammodify->seg;

	//设置查询条件
	data.cond_flag.flag_var_relation.name = TRUE;
	QTableWidgetItem* name = listModelParam->item(curRow,0);
	strcpy(data.cond_seg.seg_var_relation.name,name->text().toLocal8Bit().data());
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	displayModelParam();
}
/************************************************CModelParamAddForm*****************************************************/
CModelParamAddForm::CModelParamAddForm(QWidget* parent)
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

void CModelParamAddForm::setControlAtt()
{
	editType->setCurrentIndex(-1);
	editDataType->setCurrentIndex(-1);
}
void CModelParamAddForm::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CModelParamAddForm::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CModelParamAddForm::showEvent(QShowEvent *e)
{
	e->accept();
}
void  CModelParamAddForm::setLanguage(int language)
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
void CModelParamAddForm::apply()
{		
	QString TypeStr,DataTypeStr;
	strcpy(seg.name,editName->text().toLocal8Bit().data());	
	strcpy(seg.logicalname,editLogicalname->text().toLocal8Bit().data());
	TypeStr = editType->currentText();
	if (TypeStr==QString::fromLocal8Bit("输入"))
	{
		seg.type = 0;
	}
	else if (TypeStr==QString::fromLocal8Bit("输出"))
	{
		seg.type = 1;
	}
	else if (TypeStr==QString::fromLocal8Bit("内部参数"))
	{
		seg.type = 2;
	}

	DataTypeStr = editDataType->currentText();
	if (DataTypeStr==QString::fromLocal8Bit("H9000点名"))
	{
		seg.data_type = 0;
	}
	else if (DataTypeStr==QString::fromLocal8Bit("常值"))
	{
		seg.data_type = 1;
	}
	if (flag_add)
	{
		emit insertRecord();
	}
	if (flag_modify)
	{
		emit modifyRecord();
	}
}

void CModelParamAddForm::enter()
{
	apply();
	editName->clear();
	editLogicalname->clear();
	editType->setCurrentIndex(-1);
	editDataType->setCurrentIndex(-1);
	this->close();
	flag_add=false;
	flag_modify=false;
}
