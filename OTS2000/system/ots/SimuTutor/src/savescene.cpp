#include "savescene.h"

CSaveSceneTable::CSaveSceneTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	editDsp->setReadOnly(true);
	timer = new QTimer(this);
	val = 0;
	connect(comboStn,SIGNAL(currentIndexChanged(int)),this,SLOT(displayDev(int)));
	connect(timer, SIGNAL(timeout()), this, SLOT(judgeFinished()));
	connect(buttonBox,SIGNAL(clicked(QAbstractButton *)),this,SLOT(buttonClicked(QAbstractButton *)));
	connect(radioStn,SIGNAL(toggled(bool)),this,SLOT(radioClicked(bool)));
	connect(radioDev,SIGNAL(toggled(bool)),this,SLOT(radioClicked(bool)));
	connect(radioDsp,SIGNAL(toggled(bool)),this,SLOT(radioClicked(bool)));
	radioStn->setChecked(true);
	displayStation();
}
void CSaveSceneTable::closeEvent(QCloseEvent *event)
{
	if (shm_ots_addr->data_scene.lockflag == 0)
	{
		event->accept();
	}
	else
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("正在保存工况中，请等待!"),QMessageBox::Ok);
		event->ignore();
	}
}
void CSaveSceneTable::showEvent(QShowEvent *event)
{
	event->accept();
}
void  CSaveSceneTable::setLanguage(int language)
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
void CSaveSceneTable::judgeFinished()
{
	val++;
	switch(val%7)
	{
		case 0:
			labelWait->setText(QString::fromLocal8Bit("正在保存工况，请稍后"));
			break;
		case 1:
			labelWait->setText(QString::fromLocal8Bit("正在保存工况，请稍后."));
			break;
		case 2:
			labelWait->setText(QString::fromLocal8Bit("正在保存工况，请稍后.."));
			break;
		case 3:
			labelWait->setText(QString::fromLocal8Bit("正在保存工况，请稍后..."));
			break;
		case 4:
			labelWait->setText(QString::fromLocal8Bit("正在保存工况，请稍后...."));
			break;
		case 5:
			labelWait->setText(QString::fromLocal8Bit("正在保存工况，请稍后....."));
			break;
		case 6:
			labelWait->setText(QString::fromLocal8Bit("正在保存工况，请稍后......"));
			break;
	}
	if (shm_ots_addr->data_scene.lockflag == 0)
	{
		val=0;
		timer->stop();
		int time_Diff = time.elapsed();
		float f = time_Diff/1000.0;
		labelWait->setText("");
		QMessageBox().information(this,QString::fromLocal8Bit("完成!"),
			QString(QString::fromLocal8Bit("共保存%1条记录，用时%2秒")).arg(shm_ots_addr->data_scene.pt_num).arg(f),
			QMessageBox::Ok);
		this->close();
	}
}
void CSaveSceneTable::buttonClicked(QAbstractButton *button)
{
	QString scene_name;
	if (buttonBox->button(QDialogButtonBox::Ok) == button)
	{	
		if (shm_ots_addr->data_scene.lockflag == 0)
		{		
			shm_ots_addr->user_id = loginUser->login_user_id;
			if (radioStn->isChecked())
			{
				shm_ots_addr->data_scene.stn_id = comboStn->itemData(comboStn->currentIndex()).toInt();
				shm_ots_addr->data_scene.dev_id = 0;
				shm_ots_addr->data_scene.type = 2;
				scene_name= comboStn->currentText()+QString::fromLocal8Bit("工况");
			}
			if (radioDev->isChecked())
			{
				shm_ots_addr->data_scene.stn_id = comboStn->itemData(comboStn->currentIndex()).toInt();
				shm_ots_addr->data_scene.dev_id = comboDev->itemData(comboDev->currentIndex()).toInt();
				shm_ots_addr->data_scene.type = 2;
				scene_name = comboStn->currentText()+comboDev->currentText()+QString::fromLocal8Bit("工况");
			}
			if (radioDsp->isChecked())
			{
				shm_ots_addr->data_scene.stn_id = 0;
				shm_ots_addr->data_scene.dev_id = 0;
				shm_ots_addr->data_scene.type = 1;
				if (editDsp->text().isEmpty())
				{
					QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("当前画面为空，不能保存工况!"),QMessageBox::Ok);
					return;
				}
				scene_name = editDsp->text()+QString::fromLocal8Bit("工况");
			}
			if (editSceneName->text().isEmpty())
			{
				QMessageBox msgBox(this);
				msgBox.setIcon(QMessageBox::Information);
				msgBox.setWindowTitle (QString::fromLocal8Bit("提示"));
				msgBox.setInformativeText(QString::fromLocal8Bit("您没有输入工况名称，工况将以默认名称保存，是否确认？"));
				msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Ok);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) 
				{
					return;
				}
				strcpy(shm_ots_addr->data_scene.name,scene_name.toLocal8Bit().data());
			}
			else
			{
				strcpy(shm_ots_addr->data_scene.name,editSceneName->text().toLocal8Bit().data());
			}
			strcpy(shm_ots_addr->data_scene.remark,editSceneRemark->toPlainText().toLocal8Bit().data());
			shm_ots_addr->data_scene.lockflag = 1;
			time.start();
			timer->start(500);
			shm_ots_addr->data_scene.requestflag = 1;
		}
		else
		{
			QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("正在保存工况中，请等待!"),QMessageBox::Ok);
			return;
		}
	}
	else
	{
		this->close();
	}
}
void CSaveSceneTable::radioClicked(bool checked)
{
	if (radioStn->isChecked())
	{
		comboStn->setEnabled(true);
		comboDev->setDisabled(true);
		editDsp->setDisabled(true);
		editDsp->clear();
	}
	if (radioDev->isChecked())
	{
		comboStn->setEnabled(true);
		comboDev->setEnabled(true);
		editDsp->setDisabled(true);
		editDsp->clear();
	}
	if (radioDsp->isChecked())
	{
		comboStn->setDisabled(true);
		comboDev->setDisabled(true);
		editDsp->setEnabled(true);
		editDsp->setText(QString::fromLocal8Bit(shm_ots_addr->data_point.curname));
	}
}
void CSaveSceneTable::displayStation()
{
	for(int i=1;i<=g_stn_num;i++)
	{
		UCHAR	stnid;
		if(-1==GetStnIdByIndex(i, &stnid))
			continue;
		comboStn->insertItem(i,QString::fromLocal8Bit(data_station[stnid].station_longname),QVariant(data_station[stnid].station_id));	
	}
}
void CSaveSceneTable::displayDev(int index)
{
	comboDev->clear();
	UCHAR stnid = comboStn->itemData(index).toInt();
	for (int j=1;j<=data_station[stnid].dev_num;j++)
	{
		comboDev->insertItem(j,QString::fromLocal8Bit(data_dev[stnid][j].dev_longname),QVariant(data_dev[stnid][j].dev_id));
	}
}
void CSaveSceneTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CSaveSceneTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}