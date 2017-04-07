#ifndef _SAVESCENE_H_
#define _SAVESCENE_H_
#include "ui_savescene.h"
#include "login.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 

class CSaveSceneTable : public QDialog,public Ui_CSaveSceneTable
{
	Q_OBJECT
public:
	CSaveSceneTable(QWidget *parent=0);
private:
	QTimer *timer;
	QTime time;
	int val;
protected:
	void closeEvent(QCloseEvent *);
private slots:
	void buttonClicked(QAbstractButton *);
	void radioClicked(bool);
	void displayStation();
	void displayDev(int);
	void judgeFinished();
};

#endif