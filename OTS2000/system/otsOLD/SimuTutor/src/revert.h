#ifndef _REVERT_H_
#define _REVERT_H_

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "ui_revert.h"
#include "ui_revertadd.h"
#include "login.h"
#include "stndev.h"

class CRevertAddTable : public QDialog,public Ui_CRevertAddTable
{
	Q_OBJECT
public:
	CRevertAddTable(QWidget *parent=0);
	bool flag_add,flag_modify;
	DATA_REVERT seg;
	void loadChinese();
	void loadEnglish();
	void init();
signals:
	void insertRecord();
	void modifyRecord();
	private slots:
		void apply();
		void enter();
		void displayStation();
		void displayDev(int stn);
};
class CRevertTable : public QDialog,public Ui_CRevertTable
{
	Q_OBJECT
public:
	CRevertTable(QWidget *parent=0);
	CRevertAddTable* revertadd;
	CRevertAddTable* revertmodify;
	CStnDevTable* stndev;
	int curRow;
	int curColumn;
	int curRevertId;
	int rownums;
	int stnId,devId;
	bool select_status;
	bool select_stn;
	bool select_stndev;
	int user_id;
	QAction* actionAddRevert;
	QAction* actionModifyRevert;
	QAction* actionDelRevert;
	void setControlAtt();
	void moveEvent (QMoveEvent *);
	void contextMenuEvent(QContextMenuEvent *);
	void closeEvent(QCloseEvent *);
	void loadChinese();
	void loadEnglish();
private:
	public slots:
		int displayRevert();
		void selectCell(int,int);
		void delRevert();
		void openRevertAdd();
		void openRevertModify();
		void addRevert();
		void modifyRevert();
		void openStnDev();
		void displayUpdate(int);
		void displayUpdate(int,int);
};
#endif