#include "subscore.h"

SubScore::SubScore(QWidget* parent)
:QWidget(parent)
{
	//this->setAttribute(Qt::WA_DeleteOnClose);

	setupUi(this);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	showable = true;
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setEditTriggers(QTableView::NoEditTriggers);
	if (flag_simutrainee)
	{
		label_TutorSc->setVisible(false);
		label_6->setVisible(false);
	}
	subscoreModel = new QStandardItemModel;

}
SubScore::~SubScore()
{
/*	dlg_created_flag[USERSCOREDLG] = false;*/
}
void SubScore::setScoreInfo(int send_task_id,float score,float tutorscore)
{
	//subscoreModel->clear();
	DB_FMT data;
	InitDbData(&data,TABLE_SCORE);
	data.cond_flag.flag_score.send_task_id = TRUE;
	data.cond_seg.seg_score.send_task_id = send_task_id;
	strcpy(data.addon_str,"order by step");
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		qDebug("查询数据库失败!");
		return;
	}
	label_username->setText(QString::fromLocal8Bit(data_score[0].userinfo_longname));
	label_taskname->setText(QString::fromLocal8Bit(data_score[0].task_name));
	label_TutorSc->setText(QString::number(tutorscore));
	label_TotalSc->setText(QString::number(score));
	if(data.rownum>=1)
	{
		setsubScoreModel(&data);
		showable = true;
	}
	else
	{
		QMessageBox().critical(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("当前任务没有查询到相应评分结果!"),QMessageBox::Ok);
		showable = false;
	//	this->close();
	}
}

void  SubScore::setsubScoreModel(DB_FMT *data)
{
	subscoreModel->clear();
	subscoreModel->setColumnCount(3);  
	subscoreModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("题号"));  
	subscoreModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit(" 题 目 名 称 ")); 
	subscoreModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("得分情况"));
	//subscoreModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("主机名")); 
	for(int i = 0; i < data->rownum;i++)
	{
		//QStandardItem* item0 = new QStandardItem(QString::number(data_score[i].step));  
		QStandardItem* item0 = new QStandardItem(QString::fromLocal8Bit("第")+QString::number(i+1)+QString::fromLocal8Bit("步"));  
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_score[i].subject_name));  
		QString score =  QString("%1").arg(data_score[i].score);
		QStandardItem* item2 = new QStandardItem(score);  
		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2; 
		subscoreModel->appendRow(items); 
	}
	tableView->setModel(subscoreModel);
	tableView->setColumnWidth(0,100);
	tableView->setColumnWidth(1,450);
	tableView->setColumnWidth(2,100);
}
void SubScore::loadChinese()
{	
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
	}
	else
		translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void SubScore::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		 translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
	}
	else
		translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void SubScore::showEvent(QShowEvent *event)
{
	event->accept();
}
void  SubScore::setLanguage(int language)
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