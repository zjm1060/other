#ifndef _LOGINSTUDENT_H_
#define _LOGINSTUDENT_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "ui_loginstudent.h"
#include "ui_trainmanager.h"
#include "settask.h"
#include "qcommon.h"
class CTrainManagerTable : public QDialog,public Ui_CTrainManagerTable
{
	Q_OBJECT
	public:
		CTrainManagerTable(QWidget *parent=0);
		void setUserInfo(int,int);
		void getSimuInfo();
	private:
		int user_id,host_id;
		int simu_mode,simu_speed,simu_status;
		QList<int> list_speed_level;
	protected:
		void showEvent(QShowEvent *);
	private Q_SLOTS:
		void setSimuMode(int index);
		void setSimuSpeed(int value);
		void setSimuStart();
		void setSimuPause();
		void setSimuStop();
};

class CLoginStudentTable : public QDialog,public Ui_CLoginStudentTable
{
	Q_OBJECT
	public:
		CLoginStudentTable(QWidget *parent=0);
		int rownums,rows,columns;
		bool select_status;
		QAction* actionSetTask;
		QAction* actionSetScene;
		QAction* actionSetFault;
		QAction* actionTrainManager;
		QAction* actionLogout;
		QTimer *refresh;
		CSetSceneFaultTable *setscenetable,*setfaulttable;
		CTrainManagerTable *trainmanagertable;
		QSortFilterProxyModel *model;
		QItemSelectionModel *selectionModel;
		void setControlAtt();
		void contextMenuEvent(QContextMenuEvent *);
		void loadChinese();
		void loadEnglish();
	public slots:
		void selectCell(int,int);
		void openSetTaskTable();
		void openSetSceneTable();
		void openSetFaultTable();
		void openTrainManagerTable();
		void setScene(int);
		void setFault(int);
		void logoutForce();
		void updateModel();
};
#endif