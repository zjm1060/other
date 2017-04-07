#ifndef _STNDEV_H_
#define _STNDEV_H_

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "ui_stndev.h"
#include "qcommon.h"
class CStnDevTable : public QDialog,public Ui_CStnDevTable
{
	Q_OBJECT
public:
	CStnDevTable(QWidget *parent=0);	
	QPoint newpt;
	QIcon icon;
	bool mvwith;
	int xP;
	void moveEvent ( QMoveEvent * event );	
	void loadChinese();
	void loadEnglish();	
protected:
	void showEvent(QShowEvent *);
signals:
	void update(int);
	void update(int,int);
private slots:
	void displayStnDev();
	void select(QTreeWidgetItem *);
public slots:
	void setLanguage( int );
};
#endif