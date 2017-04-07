#ifndef _STNDEV_H_
#define _STNDEV_H_

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "ui_stndev.h"

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
	private:
	signals:
		void update(int);
		void update(int,int);
	private slots:
		void displayStnDev();
		void select(QTreeWidgetItem *);
};
#endif