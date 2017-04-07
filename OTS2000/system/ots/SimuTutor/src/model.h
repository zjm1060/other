#ifndef _MODEL_H_
#define _MODEL_H_

#include <QProcess>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "ui_model.h"
#include "modelparam.h"
#include "ui_modeladd.h"
#include "stndev.h"
#include "qcommon.h"

class CModelAddForm : public QDialog,public Ui_CModelAddForm
{
	Q_OBJECT
public:
	CModelAddForm(QWidget *parent=0);
	void setControlAtt();
	bool flag_add,flag_modify;
	DATA_MODEL seg;
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
signals:
	void insertRecord();
	void modifyRecord();
private slots:
	void apply();
	void enter();
	void displayStation();
	void displayDev(int stn);
public slots:
	void setLanguage( int );
};
class CModelForm : public QDialog,public Ui_CModelForm
{
	Q_OBJECT
public:
	CModelForm(QWidget *parent=0);
	CModelAddForm* modeladd; 
	CModelAddForm* modelmodify;
	CModelParamForm* modelparam;
	CStnDevTable* stndev;
	int curRow;
	int curColumn;
	int curModelId;
	int rownums;
	int stnId,devId;
	bool select_status;
	bool select_stn;
	bool select_stndev;
	QAction* actionAddModel;
	QAction* actionModifyModel;
	QAction* actionDelModel;
	QAction* actionModelParam;
	QProcess* modelConfig;
	void setControlAtt();
	void moveEvent (QMoveEvent *);
	void contextMenuEvent(QContextMenuEvent *);
	void closeEvent(QCloseEvent *);
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
public slots:
		void openModelAdd();
		void openModelModify();
		void openModelParam();
		void addModel();
		void selectCell(int,int);
		void delModel();
		void modifyModel();
		int displayModel();
		void openModelConfig();
		void intoModel();
		void openStnDev();
		void displayUpdate(int);
		void displayUpdate(int,int);
		void setLanguage( int );
};


#endif