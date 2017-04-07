#ifndef _STATIONCONFIG_H_
#define _STATIONCONFIG_H_

#include "ui_stationconfig.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "ui_stationadd.h"

class CStationAddTable: public QDialog,public Ui_CStationAddTable
{
	Q_OBJECT
public:
	CStationAddTable(QWidget *parent=0);
	DATA_HOST seg;
	bool flag_add,flag_modify;
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
};
class CStationConfigTable : public QDialog,public Ui_CStationConfigTable
{
	Q_OBJECT
public:
	CStationConfigTable(QWidget *parent=0);
	CStationAddTable* stationadd;
	CStationAddTable* stationmodify;
	int curRow;
	int curColumn;
	int curHostId;
	int rownums;
	bool select_status;
	QAction* actionAddStation;
	QAction* actionModifyStation;
	QAction* actionDelStation;
	void setControlAtt();
	void contextMenuEvent(QContextMenuEvent *);
	void loadChinese();
	void loadEnglish();
private:
	public slots:
		int displayStation();
		void selectCell(int,int);
		void delStation();
		void openStationAdd();
		void openStationModify();
		void addStation();
		void modifyStation();
};
#endif