#include "Localopr.h"

CLocalOprTable::CLocalOprTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	creatDialog();
	connect(listLocalOprType,SIGNAL(currentRowChanged(int)),this,SLOT(displayLocalOpr(int)));
	connect(buttonTypeAdd,SIGNAL(clicked()),this,SLOT(openLocalOprTypeAdd()));
	connect(buttonTypeModify,SIGNAL(clicked()),this,SLOT(openLocalOprTypeModify()));
	connect(buttonTypeDel,SIGNAL(clicked()),this,SLOT(delLocalOprType()));
	connect(buttonAdd,SIGNAL(clicked()),this,SLOT(openLocalOprAdd()));
	connect(buttonModify,SIGNAL(clicked()),this,SLOT(openLocalOprModify()));
	connect(buttonDel,SIGNAL(clicked()),this,SLOT(delLocalOpr()));
	connect(buttonRefresh,SIGNAL(clicked()),this,SLOT(displayLocalOprType()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));
}
void CLocalOprTable::showEvent(QShowEvent *event)
{
	displayLocalOprType();
	event->accept();
}
void CLocalOprTable::creatDialog()
{
	dialog = new QDialog(this);
	dialog->setModal(true);
	btnEnter = new QPushButton(dialog);
	btnExit = new QPushButton(dialog);
	lineEdit = new QLineEdit(dialog);
	QGridLayout *gridLayout = new QGridLayout(dialog);
	btnEnter->setText(QString::fromLocal8Bit("确定"));
	btnExit->setText(QString::fromLocal8Bit("取消"));
	gridLayout->addWidget(lineEdit, 0, 0, 1, 2);
	gridLayout->addWidget(btnEnter, 1, 0, 1, 1);
	gridLayout->addWidget(btnExit, 1, 1, 1, 1);
	dialog->setLayout(gridLayout);
	dialog->resize(300,80);
	connect(btnExit,SIGNAL(clicked()),dialog,SLOT(close()));
}
void CLocalOprTable::displayLocalOprType()
{
	DB_FMT data;
	data.table_id=TABLE_OPR_TYPE;
	InitDbData(&data,data.table_id);
	if (SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	listLocalOprType->clear();
	for (int i=0;i<data.rownum;i++)
	{
		QListWidgetItem * item = new QListWidgetItem;
		item->setText(QString::fromLocal8Bit(data_opr_type[i].name));
		item->setData(Qt::UserRole,QVariant(data_opr_type[i].opr_type));
		listLocalOprType->addItem(item);
	}
}
void CLocalOprTable::displayLocalOpr(int row)
{
	if (row == -1) return;
	short opr_type = listLocalOprType->item(row)->data(Qt::UserRole).toInt();
	DB_FMT data;
	data.table_id=TABLE_LOCAL_OPR;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_local_opr.opr_type = TRUE;
	data.cond_seg.seg_local_opr.opr_type = opr_type;
	if (SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	listLocalOpr->clear();
	listLocalOpr->setRowCount(0);
	listLocalOpr->setColumnCount(1);
	for (int i=0;i<data.rownum;i++)
	{
		listLocalOpr->insertRow(i);
		QTableWidgetItem * item = new QTableWidgetItem;
		item->setText(QString::fromLocal8Bit(data_local_opr[i].name));
		item->setData(Qt::UserRole,QVariant(data_local_opr[i].opr_id));
		item->setData(Qt::UserRole+1,QVariant(data_local_opr[i].opr_type));
		listLocalOpr->setItem(i,0,item);
	}
}
void CLocalOprTable::openLocalOprTypeAdd()
{
	dialog->setWindowTitle(QString::fromLocal8Bit("添加现地操作类型"));
	dialog->show();
	lineEdit->clear();
	lineEdit->setMaxLength(10);
	btnEnter->disconnect();
	connect(btnEnter,SIGNAL(clicked()),this,SLOT(addLocalOprType()));
}
void CLocalOprTable::openLocalOprTypeModify()
{
	if (listLocalOprType->currentRow()==-1) return;
	dialog->setWindowTitle(QString::fromLocal8Bit("修改现地操作类型"));
	dialog->show();
	lineEdit->clear();
	lineEdit->setMaxLength(10);
	lineEdit->setText(listLocalOprType->currentItem()->text());
	btnEnter->disconnect();
	connect(btnEnter,SIGNAL(clicked()),this,SLOT(modifyLocalOprType()));
}
void CLocalOprTable::openLocalOprAdd()
{
	dialog->setWindowTitle(QString::fromLocal8Bit("添加现地操作"));
	dialog->show();
	lineEdit->clear();
	lineEdit->setMaxLength(25);
	btnEnter->disconnect();
	connect(btnEnter,SIGNAL(clicked()),this,SLOT(addLocalOpr()));
}
void CLocalOprTable::openLocalOprModify()
{
	if (listLocalOpr->currentRow()==-1) return;
	dialog->setWindowTitle(QString::fromLocal8Bit("修改现地操作"));
	dialog->show();
	lineEdit->clear();
	lineEdit->setMaxLength(25);
	lineEdit->setText(listLocalOpr->currentItem()->text());
	btnEnter->disconnect();
	connect(btnEnter,SIGNAL(clicked()),this,SLOT(modifyLocalOpr()));
}
void CLocalOprTable::addLocalOprType()
{
	DB_FMT data;
	int max_id;
	data.table_id=TABLE_OPR_TYPE;
	InitDbData(&data,data.table_id);

	data.flag.flag_opr_type.opr_type = TRUE;
	data.flag.flag_opr_type.name = TRUE;

	data.seg.seg_opr_type.opr_type = GetInexistenceIdInHdb(TABLE_OPR_TYPE);
	strcpy(data.seg.seg_opr_type.name,lineEdit->text().toLocal8Bit().data());
	if( InsertOneRec(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	displayLocalOprType();
	dialog->hide();
}
void CLocalOprTable::modifyLocalOprType()
{
	DB_FMT data;
	data.table_id=TABLE_OPR_TYPE;
	InitDbData(&data,data.table_id);
	data.flag.flag_opr_type.name = TRUE;
	strcpy(data.seg.seg_opr_type.name,lineEdit->text().toLocal8Bit().data());

	data.cond_flag.flag_opr_type.opr_type = TRUE;
	data.cond_seg.seg_opr_type.opr_type = listLocalOprType->currentItem()->data(Qt::UserRole).toInt();
	if( UpdateOneRec(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	displayLocalOprType();
	dialog->hide();
}
void CLocalOprTable::delLocalOprType()
{
	if (listLocalOprType->currentRow()==-1) return;
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
	foreach(QListWidgetItem *item,listLocalOprType->selectedItems())
	{
		DB_FMT data;
		InitDbData(&data,TABLE_LOCAL_OPR);
		data.cond_flag.flag_local_opr.opr_type = TRUE;
		data.cond_seg.seg_local_opr.opr_type = item->data(Qt::UserRole).toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			continue;
		}
		InitDbData(&data,TABLE_OPR_TYPE);
		data.cond_flag.flag_opr_type.opr_type = TRUE;
		data.cond_seg.seg_opr_type.opr_type = item->data(Qt::UserRole).toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			continue;
		}
		listLocalOprType->takeItem(listLocalOprType->row(item));
	}
}
void CLocalOprTable::addLocalOpr()
{
	DB_FMT data;
	data.table_id=TABLE_LOCAL_OPR;
	InitDbData(&data,data.table_id);

	data.flag.flag_local_opr.opr_id = TRUE;
	data.flag.flag_local_opr.opr_type = TRUE;
	data.flag.flag_local_opr.name = TRUE;
	data.flag.flag_local_opr.msg = TRUE;

	data.seg.seg_local_opr.opr_id = GetInexistenceIdInHdb(TABLE_LOCAL_OPR);
	data.seg.seg_local_opr.opr_type = listLocalOprType->currentItem()->data(Qt::UserRole).toInt();
	strcpy(data.seg.seg_local_opr.name,lineEdit->text().toLocal8Bit().data());
	strcpy(data.seg.seg_local_opr.msg," ");
	if( InsertOneRec(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	displayLocalOpr(listLocalOprType->currentRow());
	dialog->hide();
}
void CLocalOprTable::modifyLocalOpr()
{
	DB_FMT data;
	data.table_id=TABLE_LOCAL_OPR;
	InitDbData(&data,data.table_id);
	data.flag.flag_local_opr.name = TRUE;
	strcpy(data.seg.seg_local_opr.name,lineEdit->text().toLocal8Bit().data());

	data.cond_flag.flag_local_opr.opr_type = TRUE;
	data.cond_seg.seg_local_opr.opr_type = listLocalOpr->currentItem()->data(Qt::UserRole+1).toInt();
	data.cond_flag.flag_local_opr.opr_id = TRUE;
	data.cond_seg.seg_local_opr.opr_id = listLocalOpr->currentItem()->data(Qt::UserRole).toInt();
	if( UpdateOneRec(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	displayLocalOpr(listLocalOprType->currentRow());
	dialog->hide();
}
void CLocalOprTable::delLocalOpr()
{
	if (listLocalOpr->currentRow()==-1) return;
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
	foreach(QTableWidgetItem*item,listLocalOpr->selectedItems())
	{
		DB_FMT data;
		InitDbData(&data,TABLE_LOCAL_OPR);
		data.cond_flag.flag_local_opr.opr_type = TRUE;
		data.cond_seg.seg_local_opr.opr_type = listLocalOpr->currentItem()->data(Qt::UserRole+1).toInt();
		data.cond_flag.flag_local_opr.opr_id = TRUE;
		data.cond_seg.seg_local_opr.opr_id = listLocalOpr->currentItem()->data(Qt::UserRole).toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			continue;
		}
		listLocalOpr->removeRow(listLocalOpr->currentRow());
	}
}