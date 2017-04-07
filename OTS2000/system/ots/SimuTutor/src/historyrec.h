#ifndef _HISTORYREC_H_
#define _HISTORYREC_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include <QMessageBox>
#include <QProcess>
#include <Windows.h>
#include <iostream>
#include <QThread>
#include <QTableView>
#include <QStyleOptionViewItem>
#include <QPrinter>
#include <QSortFilterProxyModel>
#include <QProgressDialog>

#ifdef WIN32
#include <ActiveQt/QAxFactory>
#include <qaxobject.h> 
#endif

#include "ui_historyrec.h"
#include "ui_selecttask.h"
#include "excelobject.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"


//#include "ui_excel.h"

class PrintView : public QTableView
{
	Q_OBJECT

public:
	PrintView();

	public Q_SLOTS:
		void print(QPrinter *printer);
};
/*class CExcelTable : public QDialog,public Ui_CExcelTable
{
	Q_OBJECT
	public:
		CExcelTable(QWidget *parent=0);
		void readTemplateDef();
		void readConfig();
		void writeConfig();
		void loadChinese();
		void loadEnglish();
	signals:
		void exp();
	private slots:
		void exportExcel();
};*/
class CSelectTaskTable : public QDialog,public Ui_CSelectTaskTable
{
	Q_OBJECT
	public:
		CSelectTaskTable(QWidget *parent=0);
		void setControlAtt();
		int curRow,curColumn,select_status,curTaskId;
		void loadChinese();
		void loadEnglish();
		QItemSelectionModel *selectionModel;
		void setUserId(int user_id){m_user_id = user_id;};
	protected:
		void showEvent ( QShowEvent * event );
		void contextMenuEvent(QContextMenuEvent *);
	private:
		QAction* actionDel;
		int m_user_id;
	private slots:
		void showSendTaskMenu(const QPoint &);
		void delSendTask();
		void delSendTaskFromHdb(int);
		void delRecordFromHdb(int);
	public slots:
		int displayTask();
};
class CHistoryRecTable : public QWidget,public Ui_CHistoryRecTable
{
	Q_OBJECT
	public:
		CHistoryRecTable(QWidget *parent=0);
		~CHistoryRecTable();
		QProcess* recPrint;
		QObject *parent;
		int check_stat;
		int task_user_id;
		QAction* actionDelRec;
		QAxObject *excel;
		CSelectTaskTable *selecttasktable;
		//CExcelTable *exceltable;
		QStandardItemModel *recSourceModel;
		QSortFilterProxyModel *recModelType,*recModelName;
		QItemSelectionModel *selectionModel;
			int m_send_task_id;
		void shmPrintCond();
		void loadChinese();
		void loadEnglish();
		void delRecFromHdb(int);
		void init();
		void setUserId(int user_id){m_user_id = user_id;};
		void setSendTaskId(int send_task_id){m_send_task_id = send_task_id;};
		void initdb();
		void single_user_show();
		void func_minimal();//学员查自己任务时使用
	protected:
		void contextMenuEvent(QContextMenuEvent *);
		void showEvent(QShowEvent *);
	private:	
		int m_user_id;
		QSqlDatabase db_hisrec;//dzy 2014
	public slots:
		void displayRec();
		void displayTodayRec();
		void setLanguage( int );
	private slots:
		void print();
		void openSelectTaskTable();
		void selectTask();
		void displayUserName();
		void displayGroups();
		void exportToExcel();
		void setRecSourceModel(DB_FMT *);
		void setFilterType();
		void setFilterSearchName(const QString &);
		void delRec();
		void setFilterSearchType(const QString &);
		void delRecord();
};
class printCond
{
public:
	bool searchTask;
	bool searchToday;
	int checkOperate;
	int checkAlarm;
	int checkInfo;
	char startDatetime[22];
	char endDatetime[22];
	char start_time[22];
	char end_time[22];
	int task_user_id;
	bool flag_user_id;
};

//新增学习记录查询 邓子夜
#include "ui_studyrec.h"

class CStudyRecTable : public QWidget,public Ui_CStudyRecTable
{
	Q_OBJECT
public:
	CStudyRecTable(QWidget *parent=0);
	~CStudyRecTable();
	QProcess* recPrint;
	QObject *parent;
	int check_stat;
	int task_user_id;
	QAction* actionDelRec;
	QAxObject *excel;
	//CExcelTable *exceltable;
	QStandardItemModel *recSourceModel;
	QSortFilterProxyModel *recModelType,*recModelName;
	QItemSelectionModel *selectionModel;
//	int m_send_task_id;
	void shmPrintCond();
	void loadChinese();
	void loadEnglish();
	void delRecFromHdb(int);
	void init();
	void setUserId(int user_id){m_user_id = user_id;};
//	void setSendTaskId(int send_task_id){m_send_task_id = send_task_id;};
	void initdb();
	void single_user_show();
	void func_minimal();//学员查自己任务时使用
protected:
	void contextMenuEvent(QContextMenuEvent *);
	void showEvent(QShowEvent *);
private:	
	int m_user_id;
	QSqlDatabase db_hisrec;//dzy 2014
public slots:
	void displayRec();
	void displayTodayRec();
	void setLanguage( int );
private slots:
	void print();
	void displayUserName();
	void displayPlans();
	void exportToExcel();
	void setRecSourceModel(DB_FMT *);
	void setFilterType();
	void setFilterSearchName(const QString &);
	void delRec();
	void setFilterSearchType(const QString &);
	void delRecord();
};

#endif