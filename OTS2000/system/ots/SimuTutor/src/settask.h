#ifndef _SETTASK_H_
#define _SETTASK_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include <QMessageBox>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "ui_settask.h"
#include "ui_setscenefault.h"


class CSetTaskTable : public QWidget,public Ui_CSetTaskTable
{
	Q_OBJECT
	public:
		CSetTaskTable(QWidget *parent=0);
		~CSetTaskTable();
		int current_user_id,current_host_id,current_task_id,current_scene_id,current_fault_id;
		QStandardItemModel *taskModel,*sceneModel,*faultModel;
		QSortFilterProxyModel *taskFilterModel,*sceneFilterModel,*faultFilterModel;
		QItemSelectionModel *taskSelectionModel,*sceneSelectionModel,*faultSelectionModel;
		QDialog *isRevertTable;
		QPushButton *buttonEnter;
		QPushButton *buttonExit;
		QLabel *info;
		QRadioButton *noSave;
		QRadioButton *saveDbin;
		QRadioButton *saveDev;
		QTimer *refresh;
		QItemSelectionModel *selectionModel;
		void loadChinese();
		void loadEnglish();	
		int mode;//0:教员 1：学员学习
		bool sortbyusertype,examonly;
		int user_id,usertype;
	private:
		int defaultTimeLimit;
		void readConfig();
		void setControlAtt();
		void updateUsersAndGroups();
protected:
	void showEvent(QShowEvent *);
	public slots:
		void setTaskModel();
		void setSceneModel();
		void setFaultModel();
		void setTask();
		void singleMode(bool);
		void multiMode(bool);
		void sendTask();
		void displayStn();
		void displayDev(int);
		void taskFilterTypeChanged(int index);
		void sceneFilterTypeChanged(int index);
		void faultFilterTypeChanged(int index);
		void setTaskFilter();
		void setSceneFilter();
		void setFaultFilter();
		void setSortbyusertype(bool checked);
		void setExamOnly(bool checked);
		void setLanguage( int );
signals:
		void sig_tasksetted();
};


class CSetSceneFaultTable : public QWidget,public Ui_CSetSceneFaultTable
{
	Q_OBJECT
public:
	CSetSceneFaultTable(QWidget *parent=0);
	~CSetSceneFaultTable();
	QStandardItemModel *sceneModel,*faultModel;
	QSortFilterProxyModel *sceneFilterModel,*faultFilterModel;
	QItemSelectionModel *selectionModel;
	void setSceneModel();
	void setFaultModel();
	void updateUsersAndGroups();
	void initScene();
	void initFault();
	void loadChinese();
	void loadEnglish();	
private:
	QMultiMap<int, QList<int>> mapGroups;
protected:
	void showEvent(QShowEvent *);
signals:
	void sigSendScene(int);
	void sigSendFault(int);
	void closedlg();
private slots:
	void refreshList();
	void setScene();
	void setFault();
	void setFilter();
	void onclose();
	public slots:
		void setLanguage( int );
};

#endif