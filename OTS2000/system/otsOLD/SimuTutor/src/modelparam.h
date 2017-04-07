#ifndef _MODELPARAM_H_
#define _MODELPARAM_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>


#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "ui_modelparam.h"
#include "ui_modelparamadd.h"

class CModelParamAddForm : public QDialog,public Ui_CModelParamAddForm
{
	Q_OBJECT
public:
	CModelParamAddForm(QWidget *parent=0);
	DATA_VAR_RELATION seg;
	bool flag_add,flag_modify;
	void setControlAtt();
	void loadChinese();
	void loadEnglish();
public:
signals:
	void insertRecord();
	void modifyRecord();
	private slots:
		void apply();
		void enter();
};
class CModelParamForm : public QDialog,public Ui_CModelParamForm
{
	Q_OBJECT
public:
	CModelParamForm(QWidget *parent=0);
	CModelParamAddForm* modelparamadd; 
	CModelParamAddForm* modelparammodify;
	int curRow;
	int curColumn;
	int curModelId;
	bool select_status;
	QAction* actionAddModelParam;
	QAction* actionModifyModelParam;
	QAction* actionDelModelParam;
	QAction* actionModelParamParam;
	void setControlAtt();
	void contextMenuEvent(QContextMenuEvent *);
	void loadChinese();
	void loadEnglish();
private:
	public slots:
		void openModelParamAdd();
		void openModelParamModify();
		void addModelParam();
		void selectCell(int,int);
		void delModelParam();
		void modifyModelParam();
		int displayModelParam();
};

#endif