#ifndef _SETCONTEST_H_
#define _SETCONTEST_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include <QMessageBox>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "ui_setcontest.h"

class CSetContestTable : public QWidget,public Ui_CSetContestTable
{
	Q_OBJECT
	public:
		CSetContestTable(QWidget *parent=0);
		~CSetContestTable();
		int current_contest_id,current_sendcontest_id,current_user_id,current_host_id,current_task_id,current_scene_id,current_fault_id,current_group_id;
		QStandardItemModel *contestModel,*sendcontestModel;
		QSortFilterProxyModel *contestFilterModel;
		QItemSelectionModel *contestSelectionModel,*sendcontestSelectionModel;
		QDialog *isRevertTable;
		QPushButton *buttonEnter;
		QPushButton *buttonExit;
		QLabel *info;
		QRadioButton *noSave;
		QRadioButton *saveDbin;
		QRadioButton *saveDev;
		QTimer *refresh;
		QItemSelectionModel *selectionModel,*setcontestselectionModel;
		void loadChinese();
		void loadEnglish();	
		void contextMenuEvent(QContextMenuEvent * event);
	private:
		int defaultTimeLimit;
		void readConfig();
		void setControlAtt();
		void updateUsersAndGroups();
	protected:
		void showEvent(QShowEvent *);
	public slots:
		void setContestModel();
		void setContest();
		void setContestFilter();
		void setSendContestModel();
		void AddsendContest();
		void StartContest();
		void DelContest();
		void setLanguage( int );
signals:
		void sig_tasksetted();
};

#endif