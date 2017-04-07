#include "waitcontest.h"

CWaitContest::CWaitContest(QWidget *parent):QWidget(parent)
{
	dlg_created_flag[WAIT_CONTEST] = true;
	setupUi(this);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
}

CWaitContest::~CWaitContest()
{
	dlg_created_flag[WAIT_CONTEST] = false;
}

void CWaitContest::showEvent(QShowEvent *event)
{
	setContesttoshow();
	event->accept();
}
void  CWaitContest::setLanguage(int language)
{	
	QTranslator translator;
	if (language == 0)
	{
		translator.load("SimuTrainee_zh_CN");
	}
	else if (language == 1)
	{
		translator.load("SimuTrainee_en");
	}
	qApp->installTranslator(&translator);
	retranslateUi(this);
	setContesttoshow();
}
void CWaitContest::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CWaitContest::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CWaitContest::setContesttoshow()
{
// 	QDateTime time_start,time_end;
// 	time_start.setDate(QDate::currentDate());
// 	time_start.setTime(QTime(0,0,0,0));
// 	time_end.setDate(QDate::currentDate());
// 	time_end.setTime(QTime(23,59,59,999));
	textstr.clear();

	int group_id = GetUserGroupsId(login_user_id);
	if (group_id ==0 || group_id == -1)
	{
		textstr = QString::fromLocal8Bit("当前用户无有效竞赛分组，如有疑问，请联系教练员或者系统管理员。");
		textBrowser_curContest->setText(textstr);
		return;
	}

	DB_FMT data;
	data.table_id = TABLE_SEND_CONTEST;
	InitDbData(&data,data.table_id);
// 	data.cond_flag.flag_send_contest.start_time = TRUE;
// 	strcpy(data.cond_seg.seg_send_contest.datetime,timeStart->dateTime().toString(timeStart->displayFormat()).toLocal8Bit().data());
// 	strcpy(data.cond_seg.seg_send_contest.datetime_end,timeEnd->dateTime().toString(timeEnd->displayFormat()).toLocal8Bit().data());
// 	strcpy(data.cond_flag.flag_record.datetime_opr,">=");
// 	strcpy(data.cond_flag.flag_record.datetime_end_opr,"<=");
	data.cond_flag.flag_send_contest.contest_state = TRUE;
	data.cond_seg.seg_send_contest.contest_state = 0;
	data.cond_flag.flag_send_contest.group_id = TRUE;
	data.cond_seg.seg_send_contest.group_id = group_id;
	if (SelectRecs(&data) !=(int)TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	if (data.rownum<=0)
	{
		textstr = QString::fromLocal8Bit("当前分组无有效竞赛，如有疑问，请联系教练员或者系统管理员。");
		textBrowser_curContest->setText(textstr);
		return;
	}	
	else
	{
		for (int i = 0 ; i < data.rownum ; i++ )
		{
			textstr = QString::fromLocal8Bit("%1/n").arg(data_send_contest[i].contest_name);

		}
		textstr = QString::fromLocal8Bit("当前分组竞赛列表如下:/n")+textstr;
		textBrowser_curContest->setText(textstr);
	}
}