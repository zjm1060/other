#ifndef _FAULTDATA_H_
#define _FAULTDATA_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>

#include "ui_faultdata.h"
#include "ui_faultdataadd.h"
#include "pointname.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 

class CFaultDataAddTable: public QDialog,public Ui_CFaultDataAddTable
{
	Q_OBJECT
public:
	CFaultDataAddTable(QWidget *parent=0);
	DATA_FAULT_DATA seg;
	bool flag_add,flag_modify;
	CPointNameTable* pointNameTable;
	void setControlAtt();
	void loadChinese();
	void loadEnglish();
public:
signals:
	void insertRecord();
	void modifyRecord();
	public slots:
		void apply();
		void enter();
		void changeType(int);
		void openPointName();
		void setPointName(const QString &);
};
class CFaultDataTable : public QDialog,public Ui_CFaultDataTable
{
	Q_OBJECT
	public:
		CFaultDataTable(QWidget *parent=0);
		CFaultDataAddTable* faultdataadd;
		CFaultDataAddTable* faultdatamodify;
		int curRow;
		int curColumn;
		int curFaultId;
		bool select_status;
		QAction* actionAddFaultData;
		QAction* actionModifyFaultData;
		QAction* actionDelFaultData;
		void setControlAtt();
		void contextMenuEvent(QContextMenuEvent *);
		void loadChinese();
		void loadEnglish();
	private:
	public slots:
		void openFaultDataAdd();
		void openFaultDataModify();
		void addFaultData();
		void selectCell(int,int);
		void delFaultData();
		void modifyFaultData();
		int displayFaultData();
};
#endif