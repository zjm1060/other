#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include "ui_subject_edit.h"
#include "ui_subject_condition.h"
#include "pointname.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"

class CSubjectCondition : public QDialog,public Ui_CSubjectCondition
{
	Q_OBJECT
public:
	CSubjectCondition(QWidget *parent=0);
	void loadChinese();
	void loadEnglish();
private:
	CPointNameTable* pointNameTable;
	QItemSelectionModel *selectionModel;
	QAction *actionDel;
protected:
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent *);
signals:
	void sigConditionChanged(const QString &condition);
private slots:
	void openPointNameAlarm();
	void openPointNameCmd();
	void openPointNameCheck();
	void setPointNameAlarm(const QString &pointName);
	void setPointNameCmd(const QString &pointName);
	void setPointNameCheck(const QString &pointName);
	void setConditionAlarm(int row);
	void setConditionCmd(int row);
	void setConditionCheck(int row);
	void setConditionOpr(int row);
	void delCondition();
	void alarmSymbolChanged(int index);
	void cmdTypeChanged(int index);
	void oprTypeChanged(int index);
	void displaySymbol();
	void displayCmdType(const QString &text);
	void displayOprType();
	void openMenuDel(const QPoint &pos);
	void restoreCondition(int row,int column);
	void modifyCondition();
	void addCondition();
	void save();
public slots:
	void setLanguage( int );
};

class CSubjectEdit : public QDialog,public Ui_CSubjectEdit
{
	Q_OBJECT
public:
	CSubjectEdit(QWidget *parent=0);
	bool flag_add,flag_modify;
	int task_id;
	int step;
	int subject_id;
	void loadChinese();
	void loadEnglish();
private:
	CSubjectCondition *subjectcondition;
	void addSubject();
	void modifySubject();
	void addConditionStrToTable(const QString &conditionStr,QTableWidget *table);
protected:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);
private slots:
	void openSubjectCondition();
	void openPreSubjectCondition();
	void setSubjectCondition(const QString &condition);
	void setPreSubjectCondition(const QString &condition);
	void save();
public slots:
	void setLanguage( int );
};


#endif