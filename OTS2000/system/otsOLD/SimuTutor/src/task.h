#ifndef _TASK_H_
#define _TASK_H_

#include <QMessageBox>
#include <QSqlQuery>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "ui_task.h"
#include "ui_taskadd.h"
#include "stndev.h"
#include "subject.h"

class CTaskAddTable : public QDialog,public Ui_CTaskAddTable
{
	Q_OBJECT
public:
	CTaskAddTable(QWidget *parent=0);
	DATA_TASK seg;
	bool flag_add,flag_modify;
	bool flagnewsubject;
	void setControlAtt();
	int taskGrpId(char *);
	int sceneId(char *);
	int faultId(char *);
	void loadChinese();
	void loadEnglish();
	int stepcount;
	float scoresum;
	int  curRow,curColumn;
	bool select_status;
	int task_id;
	QString remarks;
	float totle_score;
	CSubjectEdit *subjecteditadd,*subjecteditmodify;
	QItemSelectionModel *selectionModel;
private:
	QString getCondition(const QString &type,const QString &logicalname,int type_id,int status,float fvalue) const;
	void splitConditionsToList(const QString &conditons,QStringList &conditonList);
	void splitCondition(QString conditon,QString &type,QString &logicalname,int *type_id,int *status,float *fvalue);
protected:
	void contextMenuEvent(QContextMenuEvent * event);
	void showEvent(QShowEvent *event);
signals:
	void insertRecord();
	void modifyRecord();
private slots:
		void apply();
		void enter();
		void displayStation();
		void displayDev(int stn);
		void displayTaskGrpName();
		void displayScene();
		void displaySubject();
		void openAddSubject();
		void openModifySubject();
		void delSubject();
		void importSubject();
		void exportSubject();
		void moveUp();
		void moveDown();
		void setScoreAuto();
public slots:
		void displayFault();
};

class CTaskTable : public QDialog,public Ui_CTaskTable
{
	Q_OBJECT
	public:
		CTaskTable(QWidget *parent=0);
		bool select_status;
		int rownums,curRow,curColumn;
		int curTaskId;
		int stnId,devId;
		short taskgrpid,typ;
		bool select_stn;
		bool select_stndev;	
		bool select_grep;
		bool select_type;
		QAction* actionAddTask;
		QAction* actionModifyTask;
		QAction* actionDelTask;
		CTaskAddTable* taskadd;
		CTaskAddTable* taskmodify;
		CStnDevTable* stndev;
		void setControlAtt();
		void contextMenuEvent(QContextMenuEvent *);
		void moveEvent (QMoveEvent *);
		void closeEvent(QCloseEvent *);
		void loadChinese();
		void loadEnglish();		
	private:
	public slots:
		int displayTask();
		void selectCell(int,int);
		void delTask();
		void openTaskAdd();
		void openTaskModify();
		void addTask();
		void openStnDev();
		void modifyTask();
		void displayUpdate(int);
		void displayUpdate(int,int);
		//dzy
		void selectTask();
		void displaytaskselect(int i);
};
#endif