#ifndef _SAVESCENE_H_
#define _SAVESCENE_H_
#include "ui_savescene.h"
#include "login.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"

class CSaveSceneTable : public QDialog,public Ui_CSaveSceneTable
{
	Q_OBJECT
public:
	CSaveSceneTable(QWidget *parent=0);
	void loadChinese();
	void loadEnglish();
private:
	QTimer *timer;
	QTime time;
	int val;
protected:
	void closeEvent(QCloseEvent *);
	void showEvent(QShowEvent *);
private slots:
	void buttonClicked(QAbstractButton *);
	void radioClicked(bool);
	void displayStation();
	void displayDev(int);
	void judgeFinished();
	public slots:
		void setLanguage( int );
};

#endif