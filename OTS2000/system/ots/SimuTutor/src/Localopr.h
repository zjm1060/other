#ifndef _LOCAL_OPR_H
#define _LOCAL_OPR_H

#include "ui_localopr.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
class CLocalOprTable : public QDialog,public Ui_CLocalOprTable
{
	Q_OBJECT
public:
	CLocalOprTable(QWidget *parent=0);
protected:
	void showEvent(QShowEvent *event);
private:
	QDialog *dialog;
	QPushButton *btnEnter,*btnExit;
	QLineEdit *lineEdit;
	QGridLayout *gridLayout;
	void creatDialog();
private Q_SLOTS:
	void displayLocalOprType();
	void displayLocalOpr(int row);
	void openLocalOprTypeAdd();
	void openLocalOprTypeModify();
	void openLocalOprAdd();
	void openLocalOprModify();
	void addLocalOprType();
	void modifyLocalOprType();
	void delLocalOprType();
	void addLocalOpr();
	void modifyLocalOpr();
	void delLocalOpr();
};

#endif