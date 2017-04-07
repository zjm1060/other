#ifndef _HOSTPAGE_H_
#define _HOSTPAGE_H_

#include <Qt>
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>

#include "ui_hostpage.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"

class Hostpage: public QWidget,public Ui_hostpage
{
	Q_OBJECT
public:
	Hostpage(QWidget *parent=0);
	~Hostpage();	
	QString login_user_longname;
	DATA_USERINFO u_info;
	void loadChinese();
	void loadEnglish();
	int show_count;//œ‘ æ¥Œ ˝
protected:
	void showEvent(QShowEvent *);
public slots:
	void on_toolButton_ModeS();
	void on_toolButton_ModeE();
	void on_toolButton_ModeC();
	void on_pushButton_EtStudy();
	void on_pushButton_EtExam();
	void on_pushButton_EtComplet();
	void on_pushButton_oprrecord();
	void on_pushButton_studyrecord();
	void on_pushButton_scorerecord();
	void on_toolButton_personinfo();
	void update();
	void setLanguage( int );
signals:
	void STclicked();
	void SOclicked();
	void ETclicked();
	void EOclicked();
	void CPTclicked();

	void sig_showrecords();
	void sig_showstudyrecords();
	void sig_showscores();
	void sig_showpersoninfo();
};

#endif