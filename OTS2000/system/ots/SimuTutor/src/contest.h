#ifndef _CONTEST_H_
#define _CONTEST_H_

#include <QMessageBox>
#include <QSqlQuery>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "ui_contest.h"
#include "ui_contestadd.h"
#include "qcommon.h"

class CContestAddTable : public QDialog,public Ui_CContestAddTable
{
	Q_OBJECT
public:
	CContestAddTable(QWidget *parent=0);
 	DATA_CONTEST seg;
 	bool flag_add,flag_modify;

	void setControlAtt();
 	int sceneId(char *);
 	int faultId(char *);
	int taskId(char *);
 	void loadChinese();
 	void loadEnglish();
 	int  curRow,curColumn;
 	bool select_status;
 	int contest_id;
 	QString remarks;
 	float totle_score;
 	QItemSelectionModel *selectionModel;
 private:

protected:
 	void showEvent(QShowEvent *event);
signals:
 	void insertRecord();
 	void modifyRecord();
private slots:
 		void apply();
 		void enter();
public slots:
		void displayTask();
		void displayFault();
		void displayScene();
		void setLanguage( int );
};

class CContestTable : public QDialog,public Ui_CContestTable
{
	Q_OBJECT
	public:
		CContestTable(QWidget *parent=0);
 		bool select_status;
 		int rownums,curRow,curColumn;
		int curTaskId,curContestId,contest_id;
		QAction* actionAddContest;
		QAction* actionModifyContest;
		QAction* actionDelContest;
 		CContestAddTable* contestadd;
 		CContestAddTable* contestmodify;
 		void setControlAtt();
 		void contextMenuEvent(QContextMenuEvent *);
		void closeEvent(QCloseEvent *);
 		void loadChinese();
		void loadEnglish();		
protected:
	void showEvent(QShowEvent *event);
public slots:
		int displayContest();
 		void selectCell(int,int);
 		void delContest();
 		void openContestAdd();
 		void openContestModify();
 		void addContest();
		void modifyContest();
		void setLanguage( int );
};
#endif