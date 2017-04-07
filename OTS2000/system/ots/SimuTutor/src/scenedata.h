#ifndef _SCENEDATA_H_
#define _SCENEDATA_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include "qcommon.h"
#include "ui_scenedata.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "ui_scenedataadd.h"
#include "pointname.h"

class CSceneDataAddTable: public QDialog,public Ui_CSceneDataAddTable
{
	Q_OBJECT
public:
	CSceneDataAddTable(QWidget *parent=0);
	CPointNameTable* pointNameTable;
	DATA_SCENE_DATA seg;
	bool flag_add,flag_modify;
	void setControlAtt();
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
signals:
	void insertRecord();
	void modifyRecord();
public slots:
	void apply();
	void enter();
	void openPointName();
	void setPointName(const QString &);
	void setLanguage( int );
};
class CSceneDataTable : public QDialog,public Ui_CSceneDataTable
{
	Q_OBJECT
public:
	CSceneDataTable(QWidget *parent=0);
	CSceneDataAddTable* scenedataadd; 
	CSceneDataAddTable* scenedatamodify;
	int curRow;
	int curColumn;
	int curSceneId;
	bool select_status;
	QAction* actionAddSceneData;
	QAction* actionModifySceneData;
	QAction* actionDelSceneData;
	void setControlAtt();
	void contextMenuEvent(QContextMenuEvent *);
	void loadChinese();
	void loadEnglish();
private:
	QStandardItemModel *dataSourceModel;
	QSortFilterProxyModel *dataModel;
	QItemSelectionModel *selectionModel;
	void delSceneDataFromHdb(int row);
protected:
	void showEvent(QShowEvent *);
public slots:
		void openSceneDataAdd();
		void openSceneDataModify();
		void addSceneData();
		void delSceneData();
		void modifySceneData();
		void setDataSourceModel();
		void displayStn();
		void displayDev(int);
		void displayType(int);
		void setFilter(int);
		void searchFromName();
		void displayAll();
		void setLanguage( int );
};
#endif