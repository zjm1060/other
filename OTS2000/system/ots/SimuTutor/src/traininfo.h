#ifndef _TRAININFO_H_
#define _TRAININFO_H_

#include <QMessageBox>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "excelobject.h"
#include "ui_traininfo.h"
#include "ui_traininfoadd.h"

class CTrainInfoAddTable : public QDialog,public Ui_CTrainInfoAddTable
{
	Q_OBJECT
public:
	CTrainInfoAddTable(QWidget *parent=0);
	DATA_TRAIN_INFO seg;
	bool flag_add,flag_modify;
	void setControlAtt();
	int userId(char*);
	int hostId(char*);
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *event);
signals:
	void insertRecord();
	void modifyRecord();
private slots:
	void apply();
	void enter();
public slots:
	void setLanguage( int );
};
class CTrainInfoTable : public QDialog,public Ui_CTrainInfoTable
{
	Q_OBJECT
public:
	CTrainInfoTable(QWidget *parent=0);
	bool select_status;
	int rownums,curRow,curColumn;
	int curHostId;
	QAction* actionAddTrainInfo;
	QAction* actionModifyTrainInfo;
	QAction* actionDelTrainInfo;
	CTrainInfoAddTable* traininfoadd;
	CTrainInfoAddTable* traininfomodify;
	QTimer *refresh;
	void setControlAtt();
	void contextMenuEvent(QContextMenuEvent *);
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *event);
public slots:
	int displayTrainInfo();
	void selectCell(int,int);
	void delTrainInfo();
	void openTrainInfoAdd();
	void openTrainInfoModify();
	void addTrainInfo();
	void modifyTrainInfo();
	void exportExcel();
	void setLanguage( int );
};
#endif