#include "RevertPlayer.h"

CRevertPlayer::CRevertPlayer(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::WindowStaysOnTopHint); 
	init();
	connect(buttonStartPause,SIGNAL(clicked()),this,SLOT(startOrPauseRevert()));
	connect(buttonStop,SIGNAL(clicked()),this,SLOT(stopRevert()));
	connect(buttonForward,SIGNAL(clicked()),this,SLOT(forwardRevert()));
	connect(buttonBackward,SIGNAL(clicked()),this,SLOT(backwardRevert()));
	connect(buttonListRevert,SIGNAL(clicked()),this,SLOT(isRevertListShow()));
	connect(sliderSpeed,SIGNAL(valueChanged(int)),this,SLOT(setSpeedLabel(int)));
	connect(timer,SIGNAL(timeout()),this,SLOT(updateDisplay()));
	connect(listRevert,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(revertSelected(const QModelIndex &)));
	connect(sliderProgress,SIGNAL(valueChanged(int)),this,SLOT(setRevertProgress(int)));
	connect(listRevert,SIGNAL(entered(const QModelIndex &)),this,SLOT(PopupRevertDetail(const QModelIndex &)));
}
void CRevertPlayer::showEvent(QShowEvent *event)
{
	displayRevertList();
	event->accept();
}
void CRevertPlayer::closeEvent(QCloseEvent *event)
{
	event->accept();
}
void CRevertPlayer::init()
{
	timer = new QTimer;
	timer->start(100);
	speed = shm_hdb_addr->data_revert.speed;
	revert_status = 0;
	int val = (int)(log((double)speed)/log(2.0));
	memset(&cur_revert,0,sizeof(DATA_REVERT));
	labelSpeed->setText(QString::fromLocal8Bit("%1倍速").arg(QString::number(speed/8.0)));
	sliderSpeed->setValue(val-3);
	revertMsg = QString::fromLocal8Bit("工况再现名称:%1").arg(QString::fromLocal8Bit("当前没有选择工况再现"))
		+QString::fromLocal8Bit("<br>当前状态:%1").arg(QString::fromLocal8Bit("工况再现尚未开始"))
		+QString::fromLocal8Bit("<br>工况再现开始时间:%1").arg(QString::fromLocal8Bit(cur_revert.start_time))
		+QString::fromLocal8Bit("<br>工况再现结束时间:%1").arg(QString::fromLocal8Bit(cur_revert.end_time))
		+QString::fromLocal8Bit("<br>工况再现速度:%1").arg(labelSpeed->text());
	sliderProgress->setEnabled(false);
	//sliderProgress->setStyle(new QWindowsVistaStyle());
	labelMsg->setText(revertMsg);
	listRevert->setColumnCount(2);
	listRevert->setColumnWidth(0,25);
	frameRevertList->hide();
}
void CRevertPlayer::displayRevertList()
{
	DB_FMT data;
	InitDbData(&data,TABLE_REVERT);
	if (SelectRecs(&data)!=(int)TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_REVERT);
		return;
	}
	else
	{	
		listRevert->clearContents();
		for (int i=0;i<data.rownum;i++)
		{
			QVariant varRevertData;
			listRevert->insertRow(i);
			listRevert->setItem(i,0, new QTableWidgetItem(QString::number(i+1)));
			QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit(data_revert[i].name));
			varRevertData.setValue(data_revert[i]);
			item->setData(Qt::UserRole,varRevertData);
			listRevert->setItem(i,1,item);
		}
	}
	FreeDbData(&data,TABLE_REVERT);
}
void CRevertPlayer::isRevertListShow()
{
	if (frameRevertList->isHidden())
	{
		frameRevertList->show();
		buttonListRevert->setArrowType(Qt::UpArrow);
	}
	else
	{
		frameRevertList->hide();
		buttonListRevert->setArrowType(Qt::DownArrow);
	}
}
void CRevertPlayer::setSpeedLabel(int value)
{
	double val=pow(2.0,value);
	speed = (short)(val*8);
	shm_hdb_addr->data_revert.speed = speed;
	QString str = QString::fromLocal8Bit("%1倍速").arg(QString::number(val));
	labelSpeed->setText(str);
}
void CRevertPlayer::updateDisplay()
{
	if (revert_status == 1 || revert_status == 2)
	{
		int slidervalue;
		int cur_sec,ch,cm,cs;
		QString timeStr;
		if (shm_hdb_addr->data_revert.queryflag == 2)
		{
			revertMsg = QString::fromLocal8Bit("工况再现名称:%1").arg(QString::fromLocal8Bit(cur_revert.name))
				+QString::fromLocal8Bit("<br>当前状态:%1").arg(QString::fromLocal8Bit("工况再现播放完毕"))
				+QString::fromLocal8Bit("<br>工况再现开始时间:%1").arg(QString::fromLocal8Bit(cur_revert.start_time))
				+QString::fromLocal8Bit("<br>工况再现结束时间:%1").arg(QString::fromLocal8Bit(cur_revert.end_time))
				+QString::fromLocal8Bit("<br>工况再现速度:%1").arg(labelSpeed->text());
			stopRevert();
		}
		if (shm_hdb_addr->data_revert.queryflag == 3)
		{
			revertMsg = QString::fromLocal8Bit("工况再现名称:%1").arg(QString::fromLocal8Bit(cur_revert.name))
				+QString::fromLocal8Bit("<br>当前状态:%1").arg(QString::fromLocal8Bit("工况再现正在读取中"))
				+QString::fromLocal8Bit("<br>工况再现开始时间:%1").arg(QString::fromLocal8Bit(cur_revert.start_time))
				+QString::fromLocal8Bit("<br>工况再现结束时间:%1").arg(QString::fromLocal8Bit(cur_revert.end_time))
				+QString::fromLocal8Bit("<br>工况再现速度:%1").arg(labelSpeed->text());
		}
		if (shm_hdb_addr->data_revert.queryflag == 4)
		{
			revertMsg = QString::fromLocal8Bit("工况再现名称:%1").arg(QString::fromLocal8Bit(cur_revert.name))
				+QString::fromLocal8Bit("<br>当前状态:%1").arg(QString::fromLocal8Bit("工况再现读取成功,正在播放"))
				+QString::fromLocal8Bit("<br>工况再现开始时间:%1").arg(QString::fromLocal8Bit(cur_revert.start_time))
				+QString::fromLocal8Bit("<br>工况再现结束时间:%1").arg(QString::fromLocal8Bit(cur_revert.end_time))
				+QString::fromLocal8Bit("<br>工况再现速度:%1").arg(labelSpeed->text());
			cur_time = QDateTime::fromString(QString::fromLocal8Bit(shm_hdb_addr->data_revert.curtime),QString("yyyy-MM-dd HH:mm:ss"));
			cur_sec = start_time.secsTo(cur_time);
			GetTimeFromSecs(cur_sec,&ch,&cm,&cs);
			labelTime->setText(timeStr.sprintf("%02d:%02d:%02d",ch,cm,cs));
			slidervalue = cur_sec*10000000/totle_sec;
			sliderProgress->setValue(slidervalue);
		}
		labelTimeTotle->setText(timeStr.sprintf("%02d:%02d:%02d",h,m,s));
		labelMsg->setText(revertMsg);
	}
}
void CRevertPlayer::startOrPauseRevert()
{
	if (revert_status == 0)
	{ 
		if (cur_revert.revert_id == 0)//尚未选择任何工况再现不能开始
			return;
		revert_status = 1;
		shm_hdb_addr->data_revert.queryflag = 1;
		sliderProgress->setEnabled(false);
		buttonStartPause->setStyleSheet(QString::fromLocal8Bit("border-image: url(:/pic/images/暂停.PNG);"));
	}
	else if (revert_status == 2)
	{
		revert_status = 1;
		shm_hdb_addr->data_revert.stopflag = 0;
		sliderProgress->setEnabled(false);
		buttonStartPause->setStyleSheet(QString::fromLocal8Bit("border-image: url(:/pic/images/暂停.PNG);"));
	} 
	else if (revert_status == 1)
	{
		revert_status = 2;
		shm_hdb_addr->data_revert.stopflag=1;
		sliderProgress->setEnabled(true);
		buttonStartPause->setStyleSheet(QString::fromLocal8Bit("border-image: url(:/pic/images/播放.PNG);"));
	}
}
void CRevertPlayer::stopRevert()
{
	if (revert_status == 1 || revert_status == 2)
	{
		revert_status = 0;
		shm_hdb_addr->data_revert.queryflag=2;
		shm_hdb_addr->data_revert.stopflag=0;
		labelTime->setText("00:00:00");
		sliderProgress->setEnabled(false);
		buttonStartPause->setStyleSheet(QString::fromLocal8Bit("border-image: url(:/pic/images/播放.PNG);"));
	}
}
void CRevertPlayer::forwardRevert()
{
	if (revert_status != 2)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("请在暂停时使用快进功能"),QMessageBox::Ok);
		return;
	} 
	else
	{
		if (sliderProgress->value()<=10000000-5*10000000/totle_sec)
		{
			shm_hdb_addr->data_revert.timeoffset += 5;
			sliderProgress->setValue(sliderProgress->value()+5*10000000/totle_sec);
		}
	}
}
void CRevertPlayer::backwardRevert()
{
	if (revert_status != 2)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("请在暂停时使用快退功能"),QMessageBox::Ok);
		return;
	} 
	else
	{
		if (sliderProgress->value()>=5*10000000/totle_sec)
		{
			shm_hdb_addr->data_revert.timeoffset -= 5;
			sliderProgress->setValue(sliderProgress->value()-5*10000000/totle_sec);
		}
	}
}
void CRevertPlayer::revertSelected(const QModelIndex &index)
{
	cur_revert = listRevert->item(index.row(),1)->data(Qt::UserRole).value<DATA_REVERT>();
	start_time = QDateTime::fromString(QString::fromLocal8Bit(cur_revert.start_time),QString("yyyy-MM-dd HH:mm:ss"));
	end_time = QDateTime::fromString(QString::fromLocal8Bit(cur_revert.end_time),QString("yyyy-MM-dd HH:mm:ss"));
	totle_sec = start_time.secsTo(end_time);
	if (totle_sec > MAX_REVERT_REC)
	{
		QMessageBox msgBox(this);
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
		msgBox.setInformativeText(QString::fromLocal8Bit("该工况时长超出系统规定工况再现时间，如果确定选择则只回放前%1秒!").arg(MaxRevertTime));
		msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Cancel) 
		{
			return;
		}
		strcpy(cur_revert.end_time,start_time.addSecs(MAX_REVERT_REC).toString("yyyy-MM-dd HH:mm:ss").toLocal8Bit().data());
		end_time = QDateTime::fromString(QString::fromLocal8Bit(cur_revert.end_time),QString("yyyy-MM-dd HH:mm:ss"));
		totle_sec = MAX_REVERT_REC;
	}
	GetTimeFromSecs(totle_sec,&h,&m,&s);
	shm_hdb_addr->data_revert.revert_id = cur_revert.revert_id;
	shm_hdb_addr->data_revert.stn_id = cur_revert.station_id;
	shm_hdb_addr->data_revert.dev_id = cur_revert.dev_id;
	strcpy(shm_hdb_addr->data_revert.starttime,cur_revert.start_time);
	strcpy(shm_hdb_addr->data_revert.endtime,cur_revert.end_time);
	strcpy(shm_hdb_addr->data_revert.filename,cur_revert.filename);
	shm_hdb_addr->data_revert.querytype = cur_revert.type;
	stopRevert();
	startOrPauseRevert();
}
void CRevertPlayer::setRevertProgress(int value)
{
	if (revert_status == 2)
	{
		QDateTime slidertime = start_time.addSecs(value*totle_sec/10000000);
		shm_hdb_addr->data_revert.timeoffset = cur_time.secsTo(slidertime);
	}
}
void CRevertPlayer::PopupRevertDetail(const QModelIndex &index)
{
	QString stn_name,dev_name,type_name;
	DATA_REVERT revert = listRevert->item(index.row(),1)->data(Qt::UserRole).value<DATA_REVERT>();
	if (revert.station_id == 0)
		stn_name = QString::fromLocal8Bit("无厂站");
	else
		stn_name = QString::fromLocal8Bit(data_station[revert.station_id].station_longname);
	if (revert.dev_id == 0)
		dev_name = QString::fromLocal8Bit("无设备");
	else
		dev_name = QString::fromLocal8Bit(data_dev[revert.station_id][revert.dev_id].dev_longname);

	if (revert.type == 1)
		type_name = QString::fromLocal8Bit("画面历史工况");
	else if (revert.type == 2)
		type_name = QString::fromLocal8Bit("设备历史工况");

	QString text = QString::fromLocal8Bit("名称    :%1").arg(QString::fromLocal8Bit(revert.name))
					+QString::fromLocal8Bit("<br>开始时间:%1").arg(QString::fromLocal8Bit(revert.start_time))
					+QString::fromLocal8Bit("<br>结束时间:%1").arg(QString::fromLocal8Bit(revert.end_time))
					+QString::fromLocal8Bit("<br>厂站    :%1").arg(stn_name)
					+QString::fromLocal8Bit("<br>设备    :%1").arg(dev_name)
					+QString::fromLocal8Bit("<br>类型    :%1").arg(type_name);
	QToolTip::showText(QCursor::pos(), text, this);
}