#include "SceneReset.h"

SceneReset::SceneReset(QWidget* parent):QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	InitTreeView();
	treeWidget->header()->setHidden(1);
	pushButtonInit->setEnabled(false);
	QRegExp regx("^[0-9]{1,6}$");
	QValidator *validator = new QRegExpValidator(regx,lineEditINT);
	lineEditINT->setValidator(validator); 
	QRegExp regx2("^[0-9.]{1,9}$");
	QValidator *validator2 = new QRegExpValidator(regx2,lineEditANA);
	lineEditANA->setValidator(validator2);
	connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(ItemSelected(QTreeWidgetItem *)));
	connect(pushButton_confirm,SIGNAL(clicked()),this,SLOT(on_confirm()));
	connect(pushButtonInit,SIGNAL(clicked()),this,SLOT(on_init()));
	connect(pushButton_OK,SIGNAL(clicked()),this,SLOT(on_ok()));
	connect(pushButton_CANCEL,SIGNAL(clicked()),this,SLOT(on_cancel()));
}

void SceneReset::InitTreeView()
{
	char	stn_comment[STATION_COMMENT_SIZE];
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	char	entry_longname[ENTRY_LONGNAME_SIZE];
	char	text[128];
	UCHAR	dev_id, data_type;
	GULONG		stn_num,grp_num,rec_num,entry_num;
	int		i,id;
	D_type dt;
	QVariant var;
	GetStationNum(&stn_num);
	for(i=1; i<=stn_num; i++) 
	{  		
		UCHAR stn_id;
		GetStnIdByIndex(i, &stn_id);
		dt.stnid = stn_id;
		if( GetStnCommentById(stn_id,(char*)( stn_comment))==-1 )	continue;
		sprintf(text, "%d. %s", stn_id, stn_comment);
		QTreeWidgetItem *itemStn = new QTreeWidgetItem(treeWidget, QStringList(QString::fromLocal8Bit(text)));
		if(-1==GetGroupNumById(stn_id, &grp_num))	continue;
		for(dev_id=1; dev_id<=grp_num; dev_id++) 
		{  	
			int	j;
			if(-1==GetGroupCommentById(stn_id, dev_id, dev_comment))
				continue;
			dt.devid = dev_id;
			sprintf(text, "%d. %s", dev_id, dev_comment);
			QTreeWidgetItem *itemDev = new QTreeWidgetItem(itemStn, QStringList(QString::fromLocal8Bit(text)));
			if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
				continue;
			for(data_type=1,j=0; j<rec_num; data_type++) 
			{
				if(data_type>=MAX_DATATYPE_NUM)
					break;
				if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
					continue;
				GetRecLongnameByType(data_type, datatype_longname);
				j++;
				sprintf(text, "%d. %s", data_type, datatype_longname);
				QTreeWidgetItem *itemDataType = new QTreeWidgetItem(itemDev, QStringList(QString::fromLocal8Bit(text)));
				itemDataType->setCheckState(0,Qt::Unchecked);	
				dt.data_type = (UCHAR)data_type;
				var.setValue(dt);
				itemDataType->setData(1,0,var);
			}
		}
	}
}


void SceneReset::ItemSelected(QTreeWidgetItem *curItem)
{
	QVariant var;	
	var = curItem->data(1,0);
	D_type dt;
	dt = var.value<D_type>();
	QString str;
	str = QString::number(dt.stnid)+"."+QString::number(dt.devid)+"."+QString::number(dt.data_type);
	if (curItem->checkState(0) == 0&&typelist.length()>0)
	{		
		typelist.removeOne ( str );
	}
	if (curItem->checkState(0) == 2)
	{		
		typelist.append ( str );
	}		
}

void SceneReset::on_confirm()
{
	QString str;
	str = lineEditPSW->text();
	if(str == "1234")
	{
		pushButtonInit->setEnabled(true);
		lineEditPSW->setEnabled(false);
		pushButton_confirm->setEnabled(false);
	}
}
void SceneReset::on_init()
{
	if(typelist.length()<=0)
		return;
	D_type dt;
	QStringList list1;
	flagint = flagfloat = false;
	if (lineEditINT->text().length()>0)
	{
		ivalue = lineEditINT->text().toInt();
		flagint = true;
	}
	if (lineEditANA->text().length()>0)
	{
		fvalue = lineEditANA->text().toFloat();
		flagfloat = true;
	}
	
	for(int i = 0; i <typelist.length(); i++)
	{
		list1 = typelist.at(i).split(".");
		dt.stnid = UCHAR(list1.at(0).toShort());
		dt.devid =  UCHAR(list1.at(1).toShort());
		dt.data_type =  UCHAR(list1.at(2).toShort());
		resetpt(dt.stnid,dt.devid,dt.data_type);
	}
	pushButtonInit->setEnabled(false);
	lineEditPSW->clear();
	lineEditPSW->setEnabled(true);
	lineEditANA->clear();
	lineEditINT->clear();
	pushButton_confirm->setEnabled(true);
	typelist.clear ();
}

void SceneReset::resetpt(UCHAR stn_id,UCHAR dev_id,UCHAR data_type)
{
	GULONG		entry_num;
	int		i,id;
	float f_value;
	POINTER	pt;
	DMS_COMMON	common_aa;
	DATE_TIME	time={0};
	WR_VAR_UNION	wr_var={0};
	IOST	iost={0};
	ENTRY_DEVOBJ_VAL devobj_var;
	//FB_STATUS1	fb_status1;
	if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
		return;		
	pt.stn_id= stn_id;
	pt.dev_id= dev_id;
	pt.data_type= data_type;
	for(id=0; id<entry_num; id++)
	{
		pt.pt_id= id;	
		if(-1==ReadEntryById( &pt, &common_aa ))
		{
			printf("\nNo this point in database" );
			break;
		}
		GetSysTime(&time);
		if (data_type == 1&&flagfloat == true)
		{
			f_value = fvalue;
			wr_var.anlg= common_aa.var.anlg.value;
			wr_var.anlg.data= (int)(f_value*1000);
			SetAnaPtfValueById( &pt, f_value, time);
		}
		if((data_type==4 || data_type ==5 || data_type==14)&&flagint==true)
		{
			wr_var.ind= common_aa.var.ind.status;
			wr_var.ind.rt_state= ivalue;
			SetPtValueIostById(  &pt, wr_var, iost, time, RW_RT_VALUE );						
		}
		if(data_type==6&&flagint==true)
		{
			wr_var.ind= common_aa.var.ind.status;
			wr_var.ind.rt_state= ivalue;
			SetPtValueIostById(  &pt, wr_var, iost, time, RW_RT_VALUE );						
		}
// 		if (data_type == 12)
// 		{
// 			time_t          tt,return_t;
// 			struct          tm *ttime;
// 			//	return_t=time(&tt);
// 			ttime = localtime( &tt);
// 			devobj_var.cmd_time.day =ttime->tm_mday;
// 			devobj_var.cmd_time.hour=ttime->tm_hour;
// 			devobj_var.cmd_time.minute =ttime->tm_min;
// 			devobj_var.cmd_time.second=ttime->tm_sec;
// 			devobj_var.cmd_time.year=ttime->tm_year+1900;
// 			devobj_var.cmd_time.month=ttime->tm_mon+1;
// 			devobj_var.fb_status1 = common_aa.var.devobj.fb_status1;
// 			devobj_var.fb_status1.p0 = 1;
// 			devobj_var.fb_status1.p1 = 0;
// 			SetDevObjPtVarById(&pt,devobj_var); 	
// 		}
	}
	QMessageBox::information( this,QString::fromLocal8Bit("提示"),
		QString::fromLocal8Bit("数据初始化成功!"),
		QMessageBox::Ok);
	//QMessageBox().Information(this,QString::fromLocal8Bit("恭喜！"),QString::fromLocal8Bit("数据初始化成功!"),QMessageBox::Ok);
}

void SceneReset::on_ok()
{
	QDialog::accept();
}
void SceneReset::on_cancel()
{
	this->close();
}