#include "subject.h"

CSubjectEdit::CSubjectEdit(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	flag_add=false;
	flag_modify=false;
	connect(buttonSave,SIGNAL(clicked()),this,SLOT(save()));
	connect(buttonCancel,SIGNAL(clicked()),this,SLOT(close()));
	connect(buttonEditCondition,SIGNAL(clicked()),this,SLOT(openSubjectCondition()));
	connect(buttonEditPreCondition,SIGNAL(clicked()),this,SLOT(openPreSubjectCondition()));
	connect(buttonClearCondition,SIGNAL(clicked()),editCondition,SLOT(clear()));
	connect(buttonClearPreCondition,SIGNAL(clicked()),editPreCondition,SLOT(clear()));
}
void CSubjectEdit::showEvent(QShowEvent *event)
{
	event->accept();
}
void CSubjectEdit::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton ret = MaybeSave(this);
	if (ret ==  QMessageBox::Save) 
	{
		save();
		event->accept();
	} 
	else if (ret ==  QMessageBox::Discard)
	{
		event->accept();
	}
	else 
	{
		event->ignore();
	}
}
void CSubjectEdit::openSubjectCondition()
{
	subjectcondition = new CSubjectCondition(this);
	subjectcondition->show();
	connect(subjectcondition,SIGNAL(sigConditionChanged(const QString &)),this,SLOT(setSubjectCondition(const QString &)));
	QString conditionStr = editCondition->toPlainText();
	addConditionStrToTable(conditionStr,subjectcondition->tableWidget);
}
void CSubjectEdit::openPreSubjectCondition()
{
	subjectcondition = new CSubjectCondition(this);
	subjectcondition->show();
	connect(subjectcondition,SIGNAL(sigConditionChanged(const QString &)),this,SLOT(setPreSubjectCondition(const QString &)));
	QString conditionStr = editPreCondition->toPlainText();
	addConditionStrToTable(conditionStr,subjectcondition->tableWidget);
}
void CSubjectEdit::addConditionStrToTable(const QString &conditionStr,QTableWidget *table)
{
	int curRow=0;
	QStringList conditionStrList;
	if (!conditionStr.isEmpty())
	{
		QString	tmpStr;
		for (int i=0;i<conditionStr.size();i++)
		{
			if (conditionStr.at(i) != '&' && conditionStr.at(i) != '|' 
				&& conditionStr.at(i) != '(' && conditionStr.at(i) != ')')
			{
				tmpStr.append(conditionStr.at(i));
			}
			else
			{
				tmpStr = tmpStr.trimmed();
				if(tmpStr.isEmpty())
					conditionStrList << conditionStr.at(i);
				else
					conditionStrList << tmpStr << conditionStr.at(i);
				tmpStr.clear();
			}
		}
		tmpStr = tmpStr.trimmed();
		if (!tmpStr.isEmpty())
			conditionStrList << tmpStr;
	}
	for (int i=0;i<conditionStrList.count();i++)
	{
		QStringList tmpList;
		table->setRowCount(curRow+1);
		if (i%2==0)
		{
			tmpList = conditionStrList.at(i).split(": ",QString::SkipEmptyParts);
			if (tmpList.at(0) == "ALARM")
			{
				QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("一般操作"));
				item->setData(Qt::UserRole,"ALARM");
				item->setTextAlignment(Qt::AlignCenter);
				table->setItem(curRow,0,item);
			}
			else if (tmpList.at(0) == "CMD")
			{
				QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("命令下达"));
				item->setData(Qt::UserRole,"CMD");
				item->setTextAlignment(Qt::AlignCenter);
				table->setItem(curRow,0,item);
			}
			else if (tmpList.at(0) == "CHECK")
			{
				QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("设备检查"));
				item->setData(Qt::UserRole,"CHECK");
				item->setTextAlignment(Qt::AlignCenter);
				table->setItem(curRow,0,item);
			}
			else if (tmpList.at(0) == "OPR")
			{
				QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("现地操作"));
				item->setData(Qt::UserRole,"OPR");
				item->setTextAlignment(Qt::AlignCenter);
				table->setItem(curRow,0,item);
			}
			table->setItem(curRow,2,new QTableWidgetItem(tmpList.at(1)));
			curRow++;
		}
		else
		{
			QComboBox *comboBox = new QComboBox(this);
			comboBox->addItem("AND");
			comboBox->addItem("OR");
			table->setCellWidget(curRow,1,comboBox);
			if (conditionStrList.at(i)=="&")
			{
				comboBox->setCurrentIndex(0);
			}
			else
			{
				comboBox->setCurrentIndex(1);
			}
		}
	}
}
void CSubjectEdit::save()
{
	if (flag_add)
	{
		if (editSubjectName->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请填写试题名称后保存!"),QMessageBox::Ok);	
			return;
		}
		addSubject();
	}
	if (flag_modify)
	{
		if (editSubjectName->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请填写试题名称后保存!"),QMessageBox::Ok);	
			return;
		}
		modifySubject();
	}
	this->accept();
}
void CSubjectEdit::addSubject()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SUBJECT);

	//设置表中每个字段的flag
	data.flag.flag_subject.subject_id = TRUE;
	data.flag.flag_subject.name = TRUE;
	data.flag.flag_subject.task_id = TRUE;
	data.flag.flag_subject.step = TRUE;
	data.flag.flag_subject.type = TRUE;
	data.flag.flag_subject.datetime = TRUE;
	data.flag.flag_subject.judge_logic = TRUE;
	data.flag.flag_subject.interlock_name = TRUE;
	data.flag.flag_subject.pre_condition = TRUE;
	data.flag.flag_subject.subject_tip = TRUE;
	data.flag.flag_subject.time_limit = TRUE;
	data.flag.flag_subject.score = TRUE;
	data.flag.flag_subject.is_keystep = TRUE;
	//填充表中每个字段的值
	data.seg.seg_subject.subject_id = GetInexistenceIdInHdb(TABLE_SUBJECT);
	strcpy(data.seg.seg_subject.name,editSubjectName->text().toLocal8Bit().data());
	data.seg.seg_subject.task_id = task_id;
	data.seg.seg_subject.step = step;
	data.seg.seg_subject.type = 0;
	GetCurTimeStr(data.seg.seg_subject.datetime);
	strcpy(data.seg.seg_subject.judge_logic,editCondition->toPlainText().toLocal8Bit().data());
	strcpy(data.seg.seg_subject.interlock_name,"");
	strcpy(data.seg.seg_subject.pre_condition,editPreCondition->toPlainText().toLocal8Bit().data());
	strcpy(data.seg.seg_subject.subject_tip,editStepTip->text().toLocal8Bit().data());
	data.seg.seg_subject.time_limit = 0;
	data.seg.seg_subject.score = editScore->value();
	data.seg.seg_subject.is_keystep = short(groupBoxPreCondition->isChecked());

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}
void CSubjectEdit::modifySubject()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SUBJECT);

	//设置表中每个字段的flag
	data.flag.flag_subject.name = TRUE;
	data.flag.flag_subject.datetime = TRUE;
	data.flag.flag_subject.judge_logic = TRUE;
	data.flag.flag_subject.pre_condition = TRUE;
	data.flag.flag_subject.subject_tip = TRUE;
	data.flag.flag_subject.time_limit = TRUE;
	data.flag.flag_subject.score = TRUE;
	data.flag.flag_subject.is_keystep = TRUE;
	//填充表中每个字段的值
	strcpy(data.seg.seg_subject.name,editSubjectName->text().toLocal8Bit().data());
	GetCurTimeStr(data.seg.seg_subject.datetime);
	strcpy(data.seg.seg_subject.judge_logic,editCondition->toPlainText().toLocal8Bit().data());
	strcpy(data.seg.seg_subject.pre_condition,editPreCondition->toPlainText().toLocal8Bit().data());
	strcpy(data.seg.seg_subject.subject_tip,editStepTip->text().toLocal8Bit().data());
	data.seg.seg_subject.time_limit = editTime->value();
	data.seg.seg_subject.score = editScore->value();
	data.seg.seg_subject.is_keystep = short(groupBoxPreCondition->isChecked());

	data.cond_flag.flag_subject.subject_id=TRUE;
	data.cond_seg.seg_subject.subject_id = subject_id;
	//调用接口函数，参数为DB_FMT结构体
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
}
void CSubjectEdit::setSubjectCondition(const QString &condition)
{
	editCondition->setPlainText(condition);
}
void CSubjectEdit::setPreSubjectCondition(const QString &condition)
{
	editPreCondition->setPlainText(condition);
}
/****************************************************************CSubjectCondition*************************************************************/
CSubjectCondition::CSubjectCondition(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setModal(true);
	this->setAttribute(Qt::WA_DeleteOnClose);
	actionDel = new QAction(QString::fromLocal8Bit("删除"), this);
	connect(buttonAddAlarm,SIGNAL(clicked()),this,SLOT(addCondition()));
	connect(buttonAddCmd,SIGNAL(clicked()),this,SLOT(addCondition()));
	connect(buttonAddCheck,SIGNAL(clicked()),this,SLOT(addCondition()));
	connect(buttonAddOpr,SIGNAL(clicked()),this,SLOT(addCondition()));
	connect(comboSymbolAlarm,SIGNAL(currentIndexChanged(int)),this,SLOT(alarmSymbolChanged(int)));
	connect(comboCmdTypeId,SIGNAL(currentIndexChanged(int)),this,SLOT(cmdTypeChanged(int)));
	connect(comboOprType,SIGNAL(currentIndexChanged(int)),this,SLOT(oprTypeChanged(int)));
	connect(buttonPointAlarm,SIGNAL(clicked()),this,SLOT(openPointNameAlarm()));
	connect(buttonPointCmd,SIGNAL(clicked()),this,SLOT(openPointNameCmd()));
	connect(buttonPointCheck,SIGNAL(clicked()),this,SLOT(openPointNameCheck()));
	connect(buttonSave,SIGNAL(clicked()),this,SLOT(save()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));
	connect(actionDel,SIGNAL(triggered()),this,SLOT(delCondition()));
	connect(tableWidget,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(openMenuDel(const QPoint &)));
	connect(editLogicalNameCmd,SIGNAL(textChanged(const QString &)),this,SLOT(displayCmdType(const QString &)));
	connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(restoreCondition(int,int)));
	connect(buttonSaveAlarm,SIGNAL(clicked()),this,SLOT(modifyCondition()));
	connect(buttonSaveCmd,SIGNAL(clicked()),this,SLOT(modifyCondition()));
	connect(buttonSaveCheck,SIGNAL(clicked()),this,SLOT(modifyCondition()));
	connect(buttonSaveOpr,SIGNAL(clicked()),this,SLOT(modifyCondition()));
	displayOprType();
	displaySymbol();
	tableWidget->setColumnWidth(0,100);
	tableWidget->setColumnWidth(1,80);
	selectionModel = tableWidget->selectionModel();
	tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	tableWidget->verticalHeader()->hide();
	buttonSaveAlarm->setEnabled(false);
	buttonSaveCmd->setEnabled(false);
	buttonSaveCheck->setEnabled(false);
	buttonSaveOpr->setEnabled(false);
}
void CSubjectCondition::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton ret = MaybeSave(this);
	if (ret ==  QMessageBox::Save) 
	{
		save();
		event->accept();
	} 
	else if (ret ==  QMessageBox::Discard)
	{
		event->accept();
	}
	else 
	{
		event->ignore();
	}
}
void CSubjectCondition::openMenuDel(const QPoint &pos)
{
	QMenu* popMenu = new QMenu(this);
	popMenu->addAction(actionDel);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}
void CSubjectCondition::displaySymbol()
{
	QStringList symbol;
	symbol << QString::fromLocal8Bit("等于   =") << QString::fromLocal8Bit("小于   <") << QString::fromLocal8Bit("大于   >") << QString::fromLocal8Bit("介于  [ ]");
	comboSymbolAlarm->addItems(symbol);
}
void CSubjectCondition::displayCmdType(const QString &text)
{
	comboCmdTypeId->clear();
	if (text.isEmpty())
		return;
	comboCmdTypeId->addItem(QString::fromLocal8Bit("机组操作(C_UNIT)"),QVariant(C_UNIT));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("断路器操作(C_BREAKER)"),QVariant(C_BREAKER));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("隔离开关操作(C_SWITCH)"),QVariant(C_SWITCH));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("有功功率调节(C_APOWER)"),QVariant(C_APOWER));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("无功功率调节(C_RPOWER)"),QVariant(C_RPOWER));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("一般设备操作(C_DEVICE)"),QVariant(C_DEVICE));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("闸门操作(C_GATE)"),QVariant(C_GATE));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("主变抽头操作(C_TRANS)"),QVariant(C_TRANS));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("单步升压(C_VOLT_STEP)"),QVariant(C_VOLT_STEP));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("单步升频(C_SPEED_STEP)"),QVariant(C_SPEED_STEP));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("开关量强制(C_ONOFF_FORCE)"),QVariant(C_ONOFF_FORCE));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("模拟量强制(C_ANA_FORCE)"),QVariant(C_ANA_FORCE));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("IO控制退出(C_IO_CTRL_BLOCK)"),QVariant(C_IO_CTRL_BLOCK));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("LCU设备运行方式设置(C_DEV_MODE)"),QVariant(C_DEV_MODE));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("信号复归(C_SIGNAL_RESET)"),QVariant(C_SIGNAL_RESET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("LCU在线/离线(C_LCU_ONOFF)"),QVariant(C_LCU_ONOFF));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("LCU时钟同步(TIME_SYNC)"),QVariant(TIME_SYNC));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("双机切换(C_HOST_SYS)"),QVariant(C_HOST_SYS));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("PLC双CPU切换(C_PLC_SWITCH)"),QVariant(C_PLC_SWITCH));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("网络切换(C_NET_SWITCH)"),QVariant(C_NET_SWITCH));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("运行参数给定(C_OP_VALUE_SET)"),QVariant(C_OP_VALUE_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("系统运行方式设置(C_SYS_MODE_SET)"),QVariant(C_SYS_MODE_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("召唤数据(C_DATA_POLLING)"),QVariant(C_DATA_POLLING));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("PLC定值区操作(C_LCU_PARAM)"),QVariant(C_LCU_PARAM));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("模拟量/整定值参数设置(DPS_ANA_PARAM_SET)"),QVariant(DPS_ANA_PARAM_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("脉冲量参数设置(DPS_IMP_PARAM_SET)"),QVariant(DPS_IMP_PARAM_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("开关量动作次数统计设置(DPS_C_ONOFF_SET)"),QVariant(DPS_C_ONOFF_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("IOST状态设置(DPS_IOST_SET)"),QVariant(DPS_IOST_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("IOATTR属性设置(DPS_IOATTR_SET)"),QVariant(DPS_IOATTR_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("IO组号设置(DPS_IO_GROUP_SET)"),QVariant(DPS_IO_GROUP_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("模拟量/整定值/曲线点手动设置(DPS_MAN_VALUE_SET)"),QVariant(DPS_MAN_VALUE_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("开关量手动设置(DPS_MAN_STAT_SET)"),QVariant(DPS_MAN_STAT_SET));
	comboCmdTypeId->addItem(QString::fromLocal8Bit("曲线点数据手动设置(DPS_MAN_CURVE_SET)"),QVariant(DPS_MAN_CURVE_SET));
}
void CSubjectCondition::displayOprType()
{
	DB_FMT data;
	InitDbData(&data,TABLE_OPR_TYPE);
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询TABLE_OPR_TYPE失败!");
	}
	for(int i=0;i<data.rownum;i++)
	{
		comboOprType->addItem(data_opr_type[i].name,QVariant(data_opr_type[i].opr_type));
	}
}
void CSubjectCondition::openPointNameAlarm()
{
	pointNameTable = new CPointNameTable(this);
	pointNameTable->show();
	pointNameTable->InitTreeView();
	pointNameTable->SetSelected(editLogicalNameAlarm->text());
	connect(pointNameTable,SIGNAL(sig_curPointName(const QString &)),this,SLOT(setPointNameAlarm(const QString &)));
}
void CSubjectCondition::openPointNameCmd()
{
	pointNameTable = new CPointNameTable(this);
	pointNameTable->show();
	pointNameTable->InitTreeView();
	pointNameTable->SetSelected(editLogicalNameCmd->text());
	connect(pointNameTable,SIGNAL(sig_curPointName(const QString &)),this,SLOT(setPointNameCmd(const QString &)));
}
void CSubjectCondition::openPointNameCheck()
{
	pointNameTable = new CPointNameTable(this);
	pointNameTable->show();
	pointNameTable->InitTreeView();
	pointNameTable->SetSelected(editLogicalNameCheck->text());
	connect(pointNameTable,SIGNAL(sig_curPointName(const QString &)),this,SLOT(setPointNameCheck(const QString &)));
}
void CSubjectCondition::setPointNameAlarm(const QString &pointName)
{
	editLogicalNameAlarm->setText(pointName);
}
void CSubjectCondition::setPointNameCmd(const QString &pointName)
{
	editLogicalNameCmd->setText(pointName);
}
void CSubjectCondition::setPointNameCheck(const QString &pointName)
{
	editLogicalNameCheck->setText(pointName);
}
void CSubjectCondition::alarmSymbolChanged(int index)
{
	if (index==3)
	{
		editValueAlarm2->setEnabled(true);
	}
	else
	{
		editValueAlarm2->setEnabled(false);
	}
}
void CSubjectCondition::cmdTypeChanged(int index)
{
	POINTER point;
	if (GetPtIdByNameStr(editLogicalNameCmd->text().toLocal8Bit().data(),&point)==-1)
	{
		return;
	}
	int type_id = comboCmdTypeId->itemData(index).toInt();
	comboCmdOpt->clear();
	for (int i=0;i<MAX_CMD_OPER_TYPE_NUM;i++)
	{
		char str[STATE_STR_LEN];
		GetPtCmdMsgStrById(&point,type_id,i,str);
		if (QString::fromLocal8Bit(str).contains("Err"))//排除未定义的命令字符串
			strcpy(str,"未定义");
		comboCmdOpt->addItem(QString("%1 %2").arg(i).arg(QString::fromLocal8Bit(str)),QVariant(i));
	}
}
void CSubjectCondition::oprTypeChanged(int index)
{
	DB_FMT data;
	InitDbData(&data,TABLE_LOCAL_OPR);
	data.cond_flag.flag_local_opr.opr_type = TRUE;
	data.cond_seg.seg_local_opr.opr_type = comboOprType->itemData(index).toInt();
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询TABLE_LOCAL_OPR失败!");
		return;
	}
	comboOprName->clear();
	for(int i=0;i<data.rownum;i++)
	{
		comboOprName->addItem(data_local_opr[i].name,QVariant(data_local_opr[i].opr_id));
	}
}
void CSubjectCondition::modifyCondition()
{
	POINTER point;
	QPushButton *button = qobject_cast<QPushButton*>(sender());
	if (button == buttonSaveAlarm)
	{
		if (editLogicalNameAlarm->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请输入点逻辑名!"),QMessageBox::Ok);
			return;
		}
		if (GetPtIdByNameStr(editLogicalNameAlarm->text().toLocal8Bit().data(),&point)==-1)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名输入错误!"),QMessageBox::Ok);
			return;
		}
		if (IsPointExist(&point)==FALSE)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名不存在!"),QMessageBox::Ok);
			return;
		}
		setConditionAlarm(tableWidget->currentRow());
		buttonSaveAlarm->setEnabled(false);
	}
	else if (button == buttonSaveCmd)
	{
		if (editLogicalNameCmd->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请输入点逻辑名!"),QMessageBox::Ok);
			return;
		}
		if (GetPtIdByNameStr(editLogicalNameCmd->text().toLocal8Bit().data(),&point)==-1)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名输入错误!"),QMessageBox::Ok);
			return;
		}
		if (IsPointExist(&point)==FALSE)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名不存在!"),QMessageBox::Ok);
			return;
		}
		setConditionCmd(tableWidget->currentRow());
		buttonSaveCmd->setEnabled(false);
	}
	else if (button == buttonSaveCheck)
	{
		if (editLogicalNameCheck->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请输入点逻辑名!"),QMessageBox::Ok);
			return;
		}
		if (GetPtIdByNameStr(editLogicalNameCheck->text().toLocal8Bit().data(),&point)==-1)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名输入错误!"),QMessageBox::Ok);
			return;
		}
		if (IsPointExist(&point)==FALSE)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名不存在!"),QMessageBox::Ok);
			return;
		}
		setConditionCheck(tableWidget->currentRow());
		buttonSaveCheck->setEnabled(false);
	}
	else if (button == buttonSaveOpr)
	{
		setConditionOpr(tableWidget->currentRow());
		buttonSaveOpr->setEnabled(false);
	}
}
void CSubjectCondition::addCondition()
{
	POINTER point;
	QPushButton *button = qobject_cast<QPushButton*>(sender());
	int row = tableWidget->rowCount();
	if (button == buttonAddAlarm)
	{
		if (editLogicalNameAlarm->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请输入点逻辑名!"),QMessageBox::Ok);
			return;
		}
		if (GetPtIdByNameStr(editLogicalNameAlarm->text().toLocal8Bit().data(),&point)==-1)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名输入错误!"),QMessageBox::Ok);
			return;
		}
		if (IsPointExist(&point)==FALSE)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名不存在!"),QMessageBox::Ok);
			return;
		}
		tableWidget->insertRow(row);
		setConditionAlarm(row);
	}
	else if (button == buttonAddCmd)
	{
		if (editLogicalNameCmd->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请输入点逻辑名!"),QMessageBox::Ok);
			return;
		}
		if (GetPtIdByNameStr(editLogicalNameCmd->text().toLocal8Bit().data(),&point)==-1)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名输入错误!"),QMessageBox::Ok);
			return;
		}
		if (IsPointExist(&point)==FALSE)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名不存在!"),QMessageBox::Ok);
			return;
		}
		tableWidget->insertRow(row);
		setConditionCmd(row);
	}
	else if (button == buttonAddCheck)
	{
		if (editLogicalNameCheck->text().isEmpty())
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("请输入点逻辑名!"),QMessageBox::Ok);
			return;
		}
		if (GetPtIdByNameStr(editLogicalNameCheck->text().toLocal8Bit().data(),&point)==-1)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名输入错误!"),QMessageBox::Ok);
			return;
		}
		if (IsPointExist(&point)==FALSE)
		{
			QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("点逻辑名不存在!"),QMessageBox::Ok);
			return;
		}
		tableWidget->insertRow(row);
		setConditionCheck(row);
	}
	else if (button == buttonAddOpr)
	{
		tableWidget->insertRow(row);
		setConditionOpr(row);
	}
	buttonSaveAlarm->setEnabled(false);
	buttonSaveCmd->setEnabled(false);
	buttonSaveCheck->setEnabled(false);
	buttonSaveOpr->setEnabled(false);
}
void CSubjectCondition::delCondition()
{
	if (selectionModel->hasSelection())
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
		if (indexes.at(0).row()==0)
			tableWidget->removeCellWidget(1,1);
		tableWidget->removeRow(indexes.at(0).row());
		buttonSaveAlarm->setEnabled(false);
		buttonSaveCmd->setEnabled(false);
		buttonSaveCheck->setEnabled(false);
		buttonSaveOpr->setEnabled(false);
	}
}
void CSubjectCondition::restoreCondition(int row,int column)
{
	QTableWidgetItem *item = tableWidget->item(row,0);
	QStringList list_cond,list_type_id,list_cmd_opt,list_opr;
	QString cond = tableWidget->item(row,2)->text();
	if (item->data(Qt::UserRole).toString() == "ALARM")
	{
		list_cond = cond.split(" ",QString::SkipEmptyParts);
		if (cond.contains("="))
		{
			editLogicalNameAlarm->setText(list_cond.at(0));
			comboSymbolAlarm->setCurrentIndex(0);
			editValueAlarm1->setValue(list_cond.at(2).toInt());
		}
		else if (cond.contains("<"))
		{
			editLogicalNameAlarm->setText(list_cond.at(0));
			comboSymbolAlarm->setCurrentIndex(1);
			editValueAlarm1->setValue(list_cond.at(2).toInt());
		}
		else if (cond.contains(">"))
		{
			editLogicalNameAlarm->setText(list_cond.at(0));
			comboSymbolAlarm->setCurrentIndex(2);
			editValueAlarm1->setValue(list_cond.at(2).toInt());
		}
		else
		{
			editLogicalNameAlarm->setText(list_cond.at(0));
			comboSymbolAlarm->setCurrentIndex(3);
			editValueAlarm1->setValue(list_cond.at(2).toInt());
			editValueAlarm2->setValue(list_cond.at(4).toInt());
		}
		buttonSaveAlarm->setEnabled(true);
		buttonSaveCmd->setEnabled(false);
		buttonSaveCheck->setEnabled(false);
		buttonSaveOpr->setEnabled(false);
	}
	else if (item->data(Qt::UserRole).toString() == "CMD")
	{
		list_cond = cond.split(" ",QString::SkipEmptyParts);
		list_type_id = list_cond.at(1).split(":",QString::SkipEmptyParts);
		list_cmd_opt = list_cond.at(2).split(":",QString::SkipEmptyParts);
		editLogicalNameCmd->setText(list_cond.at(0));
		comboCmdTypeId->setCurrentIndex(comboCmdTypeId->findData(QVariant(list_type_id.at(1).toInt())));
		comboCmdOpt->setCurrentIndex(comboCmdOpt->findData(QVariant(list_cmd_opt.at(1).toInt())));
		buttonSaveAlarm->setEnabled(false);
		buttonSaveCmd->setEnabled(true);
		buttonSaveCheck->setEnabled(false);
		buttonSaveOpr->setEnabled(false);
	}
	else if (item->data(Qt::UserRole).toString() == "CHECK")
	{
		list_cond = cond.split(" ",QString::SkipEmptyParts);
		editLogicalNameCheck->setText(list_cond.at(0));
		if (list_cond.at(1) == "NORMAL")
			comboCheckState->setCurrentIndex(0);
		else
			comboCheckState->setCurrentIndex(1);
		buttonSaveAlarm->setEnabled(false);
		buttonSaveCmd->setEnabled(false);
		buttonSaveCheck->setEnabled(true);
		buttonSaveOpr->setEnabled(false);
	}
	else if (item->data(Qt::UserRole).toString() == "OPR")
	{
		list_cond = cond.split(" ",QString::SkipEmptyParts);
		list_opr = list_cond.at(0).split(".",QString::SkipEmptyParts);
		comboOprType->setCurrentIndex(comboOprType->findData(QVariant(list_opr.at(0).toInt())));
		comboOprName->setCurrentIndex(comboOprName->findData(QVariant(list_opr.at(1).toInt())));
		buttonSaveAlarm->setEnabled(false);
		buttonSaveCmd->setEnabled(false);
		buttonSaveCheck->setEnabled(false);
		buttonSaveOpr->setEnabled(true);
	}
}
void CSubjectCondition::setConditionAlarm(int row)
{
	QComboBox *comboBox = new QComboBox(this);
	QString conditionStr;
	if (comboSymbolAlarm->currentIndex()==0)
	{
		conditionStr = QString("%1 = %2")
						.arg(editLogicalNameAlarm->text())
						.arg(editValueAlarm1->value());
	}
	else if (comboSymbolAlarm->currentIndex()==1)
	{
		conditionStr = QString("%1 < %2")
						.arg(editLogicalNameAlarm->text())
						.arg(editValueAlarm1->value());
	}
	else if (comboSymbolAlarm->currentIndex()==2)
	{
		conditionStr = QString("%1 > %2")
						.arg(editLogicalNameAlarm->text())
						.arg(editValueAlarm1->value());
	}
	else if (comboSymbolAlarm->currentIndex()==3)
	{
		conditionStr = QString("%1 BETWEEN %2 AND %3")
						.arg(editLogicalNameAlarm->text())
						.arg(editValueAlarm1->value())
						.arg(editValueAlarm2->value());
	}
	comboBox->addItem("AND");
	comboBox->addItem("OR");
	QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("一般操作"));
	item->setData(Qt::UserRole,"ALARM");
	item->setTextAlignment(Qt::AlignCenter);
	tableWidget->setItem(row,0,item);
	if (row!=0)
		tableWidget->setCellWidget(row,1,comboBox);
	tableWidget->setItem(row,2,new QTableWidgetItem(conditionStr));
}
void CSubjectCondition::setConditionCmd(int row)
{
	QComboBox *comboBox = new QComboBox(this);
	QString conditionStr;
	conditionStr = QString("%1 TYPE_ID:%2 CMD_OPT:%3")
					.arg(editLogicalNameCmd->text())
					.arg(comboCmdTypeId->itemData(comboCmdTypeId->currentIndex()).toInt())
					.arg(comboCmdOpt->itemData(comboCmdOpt->currentIndex()).toInt());
	comboBox->addItem("AND");
	comboBox->addItem("OR");
	QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("命令下达"));
	item->setData(Qt::UserRole,"CMD");
	item->setTextAlignment(Qt::AlignCenter);
	tableWidget->setItem(row,0,item);
	if (row!=0)
		tableWidget->setCellWidget(row,1,comboBox);
	tableWidget->setItem(row,2,new QTableWidgetItem(conditionStr));
}
void CSubjectCondition::setConditionCheck(int row)
{
	QComboBox *comboBox = new QComboBox(this);
	QString conditionStr;
	if (comboCheckState->currentIndex()==0)
	{
		conditionStr = QString("%1 NORMAL")
						.arg(editLogicalNameCheck->text());
	}
	else if (comboCheckState->currentIndex()==1)
	{
		conditionStr = QString("%1 ANORMAL")
						.arg(editLogicalNameCheck->text());
	}
	comboBox->addItem("AND");
	comboBox->addItem("OR");
	QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("设备检查"));
	item->setData(Qt::UserRole,"CHECK");
	item->setTextAlignment(Qt::AlignCenter);
	tableWidget->setItem(row,0,item);
	if (row!=0)
		tableWidget->setCellWidget(row,1,comboBox);
	tableWidget->setItem(row,2,new QTableWidgetItem(conditionStr));
}
void CSubjectCondition::setConditionOpr(int row)
{
	QComboBox *comboBox = new QComboBox(this);
	QString conditionStr;
	conditionStr = QString("%1.%2")
					.arg(comboOprType->itemData(comboOprType->currentIndex()).toInt())
					.arg(comboOprName->itemData(comboOprName->currentIndex()).toInt());
	comboBox->addItem("AND");
	comboBox->addItem("OR");
	QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("现地操作"));
	item->setData(Qt::UserRole,"OPR");
	item->setTextAlignment(Qt::AlignCenter);
	tableWidget->setItem(row,0,item);
	if (row!=0)
		tableWidget->setCellWidget(row,1,comboBox);
	tableWidget->setItem(row,2,new QTableWidgetItem(conditionStr));
}
void CSubjectCondition::save()
{
	QString conditionStr;
	for (int i=0;i<tableWidget->rowCount();i++)
	{
		QString tmp;
		if (tableWidget->cellWidget(i,1)!=NULL)
		{
			QComboBox *combox = qobject_cast<QComboBox*>(tableWidget->cellWidget(i,1));
			if (combox->currentIndex()==0)
			{
				conditionStr.append(" & ");
			}
			else if (combox->currentIndex()==1)
			{
				conditionStr.append(" | ");
			}
		}
		tmp = QString("%1: %2")
				.arg(tableWidget->item(i,0)->data(Qt::UserRole).toString())
				.arg(tableWidget->item(i,2)->text());
		conditionStr.append(tmp);
	}
	emit sigConditionChanged(conditionStr);
	this->accept();
}