#include "Sequence.h"//dzy
SequenceList::SequenceList(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	actionAddSequence= new QAction(QString::fromLocal8Bit("添加流程"), this);
	actionModifySequence = new QAction(QString::fromLocal8Bit("修改流程"), this);
	actionDelSequence = new QAction(QString::fromLocal8Bit("删除流程"), this);
	stndev = new CStnDevTable(this);
	select_status = false;
	select_stn = false;
	select_stndev = false;
	sequenceConfig = new QProcess(this);
	rownums = refreshList();
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); 
	tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
	tableWidget->setSortingEnabled(true);
//	connect(pushButtonpara,SIGNAL(clicked()),this,SLOT(onBtnParam()));
	connect(pushButtonAdd,SIGNAL(clicked()),this,SLOT(onBtnAdd()));
	connect(pushButtonModi,SIGNAL(clicked()),this,SLOT(onBtnModify()));
	connect(pushButtonDel,SIGNAL(clicked()),this,SLOT(onBtnDel()));
	connect(actionAddSequence,SIGNAL(triggered()),this,SLOT(onBtnAdd()));
	connect(actionModifySequence,SIGNAL(triggered()),this,SLOT(onBtnModify()));
	connect(actionDelSequence,SIGNAL(triggered()),this,SLOT(onBtnDel()));
	connect(pushButtonQuit,SIGNAL(clicked()),this,SLOT(onBtnCancel()));
	connect(pushButtonRefresh,SIGNAL(clicked()),this,SLOT(onRefresh()));
	connect(tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(selectCell(int,int)));	
	connect(buttonSequenceConfig,SIGNAL(clicked()),this,SLOT(openSequenceConfig()));
	connect(buttonSequenceInto,SIGNAL(clicked()),this,SLOT(intoSequence()));
	connect(buttonStnDev,SIGNAL(clicked()),this,SLOT(openStnDev()));
	connect(pushButtonQuit,SIGNAL(clicked()),stndev,SLOT(close()));
	connect(stndev,SIGNAL(update(int)),this,SLOT(displayUpdate(int)));
	connect(stndev,SIGNAL(update(int,int)),this,SLOT(displayUpdate(int,int)));

}
void SequenceList::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	pSequenceEdit->loadChinese();
}
void SequenceList::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
	pSequenceEdit->loadEnglish();
}
void SequenceList::contextMenuEvent(QContextMenuEvent * event)
{
	event->ignore();
	QMenu* popMenu = new QMenu(this);
	QPoint p(20,23);
	QTableWidgetItem* pTableWidgetItem=tableWidget->itemAt(tableWidget->mapFromGlobal(QCursor::pos()-p));
	if(pTableWidgetItem != NULL)
	{
		popMenu->addAction(actionAddSequence);
		popMenu->addAction(actionModifySequence);
		popMenu->addAction(actionDelSequence);
	}
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void SequenceList::moveEvent ( QMoveEvent * event )
{
	if (stndev != NULL )
	{
		stndev->mvwith = true;
		stndev->newpt= event->pos();
		this->pos() = event->pos();
		stndev->moveEvent(event);
	}	
}
void SequenceList::closeEvent(QCloseEvent *event)
{
	this->close();
	stndev->close();
}
void SequenceList::openStnDev()
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
	}
}
void SequenceList::onBtnAdd()
{
	pSequenceEdit = new SequenceEdit(this);
	pSequenceEdit->show();
	pSequenceEdit->modi = false;
	connect(pSequenceEdit,SIGNAL(insertRecord()),this,SLOT(addSequence()));
}
void SequenceList::onBtnDel()
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
		InitDbData(&data,TABLE_SEQUENCE);
		QTableWidgetItem* flag_seq_id = tableWidget->item(curRow,0);
		data.cond_flag.flag_sequence.seq_id=TRUE;
		data.cond_seg.seg_sequence.seq_id = flag_seq_id->text().toInt();
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("删除数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
	rownums = refreshList();
}
void SequenceList::onBtnModify()
{
	int seq_id;
	if (select_status==false)
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("没有选中需要修改的模型!"),QMessageBox::Ok);
	}
	else
	{
		pSequenceEdit = new SequenceEdit(this);
		QTableWidgetItem* item_id = tableWidget->item(curRow,0);
		seq_id = item_id->text().toInt();
		curSeqId=seq_id;
		QTableWidgetItem* name = tableWidget->item(curRow,1);
		pSequenceEdit->lename->setText(name->text());
		QTableWidgetItem* longname = tableWidget->item(curRow,2);
		pSequenceEdit->lechname->setText(longname->text());
		QTableWidgetItem* station_id = tableWidget->item(curRow,3);
		//debug yyp
		pSequenceEdit->lestation->setText(QString("1"));

		QTableWidgetItem* dev_id = tableWidget->item(curRow,4);
		//debug yyp
		pSequenceEdit->leunit->setText(QString("1"));
		pSequenceEdit->modi = true;
		pSequenceEdit->show();
		connect(pSequenceEdit,SIGNAL(modifyRecord()),this,SLOT(modiSequence()));
	}
	rownums = refreshList();
}
// void SequenceList::onBtnParam()
// {
// 
// }
void SequenceList::onBtnCancel()
{
	QDialog::accept();
}
void SequenceList::onRefresh()
{
	rownums = refreshList();
}
void SequenceList::displayUpdate(int stn_id)
{
	select_stn = true;
	stnId = stn_id;
	refreshList();
}
void SequenceList::displayUpdate(int stn_id,int dev_id)
{
	select_stndev = true;
	stnId = stn_id;
	devId = dev_id;
	refreshList();
}
int SequenceList::refreshList()
{
	DB_FMT data;
	DATA_SEQUENCE seg;
	unsigned char stn_id,dev_id;
	InitDbData(&data,TABLE_SEQUENCE);
	if (select_stn)
	{
		data.cond_flag.flag_sequence.station_id = TRUE;
		data.cond_seg.seg_sequence.station_id = stnId;
	}
	if (select_stndev)
	{
		data.cond_flag.flag_sequence.station_id = TRUE;
		data.cond_seg.seg_sequence.station_id = stnId;
		data.cond_flag.flag_sequence.dev_id = TRUE;
		data.cond_seg.seg_sequence.dev_id = devId;
	}
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return -1;
	}
	tableWidget->setRowCount(data.rownum);
	for(int j=0;j<data.rownum;j++)
	{
		tableWidget->setItem(j,0,new QTableWidgetItem(QString::number(data_sequence[j].seq_id)));
		tableWidget->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit(data_sequence[j].name)));
		tableWidget->setItem(j,2,new QTableWidgetItem(QString::fromLocal8Bit(data_sequence[j].long_name)));
		tableWidget->setItem(j,3,new QTableWidgetItem(QString::fromLocal8Bit(data_sequence[j].datetime)));
		stn_id = data_sequence[j].station_id;
		dev_id = data_sequence[j].dev_id;
		tableWidget->setItem(j,4,new QTableWidgetItem(QString::fromLocal8Bit( data_station[stn_id].station_name )));
		tableWidget->setItem(j,5,new QTableWidgetItem(QString::fromLocal8Bit(data_dev[stn_id][dev_id].dev_name)));
	}
	select_status = false;
	select_stn = false;
	select_stndev = false;
	tableWidget->clearSelection();
	if (data.rownum != 0)
	{
		tableWidget->resizeColumnsToContents();
	}
	return data.rownum;
}
void SequenceList::selectCell(int row, int column)
{
	select_status = true;
	curRow = row;
	curColumn = column;
}
void SequenceList::addSequence()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SEQUENCE);

	//设置表中每个字段的flag
	data.flag.flag_sequence.seq_id =TRUE;
	data.flag.flag_sequence.name=TRUE;
	data.flag.flag_sequence.datetime=TRUE;
	data.flag.flag_sequence.dev_id=TRUE;
	data.flag.flag_sequence.station_id=TRUE;
	data.flag.flag_sequence.long_name=TRUE;

	pSequenceEdit->seg.seq_id = GetInexistenceIdInHdb(TABLE_SEQUENCE);
	GetCurTimeStr( pSequenceEdit->seg.datetime);
	data.seg.seg_sequence = pSequenceEdit->seg;

	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	rownums = refreshList();
}
void SequenceList::modiSequence()
{
	DB_FMT data;
	InitDbData(&data,TABLE_SEQUENCE);

	//设置表中每个字段的flag
	data.flag.flag_sequence.name=TRUE;
	data.flag.flag_sequence.long_name=TRUE;
	data.flag.flag_sequence.dev_id=TRUE;
	data.flag.flag_sequence.station_id=TRUE;

	//填充表中每个字段的值
	data.seg.seg_sequence = pSequenceEdit->seg;

	//设置查询条件
	data.cond_flag.flag_sequence.seq_id=TRUE;
	data.cond_seg.seg_sequence.seq_id = curSeqId;
	if( UpdateOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("修改数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	rownums = refreshList();
}
SequenceEdit::SequenceEdit(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	modi = false;
	connect(pushButtonOK,SIGNAL(clicked()),this,SLOT(onBtnOK()));
	connect(pushButtonCancel,SIGNAL(clicked()),this,SLOT(onBtnCancel()));
}
void SequenceEdit::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void SequenceEdit::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void SequenceEdit::onBtnOK()
{
	strcpy(seg.name,lename->text().toLocal8Bit().data());	
	strcpy(seg.long_name,lechname->text().toLocal8Bit().data());
	seg.station_id = lestation->text().toInt();
	seg.dev_id = leunit->text().toInt();

	if(!modi)	
		emit insertRecord();
	else
		emit modifyRecord();

	modi = false;

	this->close();

}
void SequenceEdit::onBtnCancel()
{
	QDialog::accept();
}
void SequenceList::openSequenceConfig()
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
		if (fileList.at(i).baseName().contains("_seq", Qt::CaseInsensitive))
			QDesktopServices::openUrl(QUrl::fromLocalFile(fileList.at(i).absoluteFilePath())); 
	}
}
void SequenceList::intoSequence()
{
	//统一函数参数数据体
	DB_FMT data;
	char timestr[22];
	int curid;
	int i;
	bool flag_repeat=false;
	GetCurTimeStr(timestr);
	if ( SimulogDataLoad () == -1 )
	{
		printf ( "simulog.dat error!\n" );
	}

	data.table_id=TABLE_SEQUENCE;
	InitDbData(&data,data.table_id);

	//将需要scene访问的字段flag置为1
	data.flag.flag_sequence.seq_id =TRUE;
	data.flag.flag_sequence.name =TRUE;
	data.flag.flag_sequence.long_name =TRUE;
	data.flag.flag_sequence.datetime =TRUE;
	data.flag.flag_sequence.station_id =TRUE;
	data.flag.flag_sequence.dev_id =TRUE;

	//填充scene表中每个字段的值
	strcpy(data.seg.seg_sequence.datetime,timestr);
	data.seg.seg_sequence.dev_id =1;
	data.seg.seg_sequence.station_id =1;
	QProgressDialog *progressDlg=new QProgressDialog(this); 
	QFont font("ZYSong18030",12);  
	progressDlg->setFont(font);  
	progressDlg->setWindowModality(Qt::WindowModal);  
	progressDlg->setMinimumDuration(5);  
	progressDlg->setWindowTitle(tr("please wait"));  
	progressDlg->setLabelText(tr("导入流程中,请稍后......      "));  
	progressDlg->setCancelButtonText(tr("取消"));  
	progressDlg->setRange(0,n_sequence-1);
	for(i=0;i<n_sequence;i++)
	{
		curid = GetInexistenceIdInHdb(TABLE_SEQUENCE);
		flag_repeat=false;
		data.seg.seg_sequence.seq_id = curid;
		strcpy(data.seg.seg_sequence.name,sequencedb[i].seq_name);
		strcpy(data.seg.seg_sequence.long_name,sequencedb[i].seq_name);
		for (int k=0;k<rownums;k++)
		{
			QTableWidgetItem* name= tableWidget->item(k,1);
			if (name->text()==QString(sequencedb[i].seq_name))
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
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入流程失败!"),QMessageBox::Ok);
			HdbLogWrite(data.sql_str,data.sql_error);
			return;
		}
	}
	onRefresh();
}