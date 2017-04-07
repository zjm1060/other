#ifndef _SIMUCONTROL_H
#define _SIMUCONTROL_H

#include "ui_Simucontrol.h"

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"


class Simucontrol:public QWidget,public Ui_Simucontrol
{
	Q_OBJECT	
public:
	Simucontrol(QWidget *parent=0);
	~Simucontrol();
		short simuSpeed;
		void loadChinese();
		void loadEnglish();
protected:

private:
	void init();
Q_SIGNALS:	
	void sigSpeedChanged(QString str);
	void closedlg();
private Q_SLOTS:
	void simuStart();
	void simuStop();
	void setSimuSpeedLabel(int value);
	void  setSimuSpeed();
	void setSimuSpeedMenu();
	void onclose();
	void onok();
	public slots:
		void setLanguage( int );
};

#endif