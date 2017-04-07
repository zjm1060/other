#ifndef _CWAITCONTEST_H
#define _CWAITCONTEST_H

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QWidget>   
#include <QVariant>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QProgressDialog>
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"

#include "ui_WaitContest.h"

class CWaitContest:public QWidget,public Ui_WaitContest
{
	 	//Q_OBJECT
public:
	 	CWaitContest(QWidget *parent=0);
		~CWaitContest();
		void loadChinese();
		void loadEnglish();
		QString  textstr;
private:
	void setContesttoshow();
protected:
	void showEvent(QShowEvent *);
public slots:
	void setLanguage( int );
// // private slots:
// 
// 		// 	void modeChanged(int);
// 		//	void displaysubScore();
// 		// 	void filterChanged(const QString &);
// 		// 	void exportToExcel();
// 
 };

#endif