#ifndef _FAULT_H_
#define _FAULT_H_

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "ui_fault.h"
#include "ui_faultadd.h"
#include "faultdata.h"
#include "login.h"
#include "stndev.h"
#include "qcommon.h"

class CFaultAddTable : public QDialog,public Ui_CFaultAddTable
{
	Q_OBJECT
public:
	CFaultAddTable(QWidget *parent=0);
	DATA_FAULT seg;
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
		void displayStation();
		void displayDev(int stn);
		void setLanguage( int );
};
class CFaultTable : public QDialog,public Ui_CFaultTable
{
	Q_OBJECT
public:
	CFaultTable(QWidget *parent=0);
	CFaultAddTable* faultadd;
	CFaultAddTable* faultmodify;
	CFaultDataTable* faultdata;
	CStnDevTable* stndev;
	int curRow;
	int curColumn;
	int curFaultId;
	int rownums;
	int stnId,devId;
	bool select_status;
	bool select_stn;
	bool select_stndev;
	QAction* actionAddFault;
	QAction* actionModifyFault;
	QAction* actionDelFault;
	QAction* actionFaultData;
	void setControlAtt();
	void moveEvent (QMoveEvent *);
	void contextMenuEvent(QContextMenuEvent *);
	void closeEvent(QCloseEvent *);
	void loadChinese();
	void loadEnglish();
	private:
	protected:
		void showEvent(QShowEvent *);
	public slots:
		int displayFault();
		void selectCell(int,int);
		void delFault();
		void openFaultData();
		void openFaultAdd();
		void openFaultModify();
		void addFault();
		void modifyFault();
		void openStnDev();
		void displayUpdate(int);
		void displayUpdate(int,int);
		void setLanguage( int );
};
#endif