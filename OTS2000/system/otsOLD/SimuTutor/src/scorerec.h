#ifndef _SCOREREC_H_
#define _SCOREREC_H_
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QProgressDialog>
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "excelobject.h"
#include "ui_scorerec.h"

class CScoreRec:public QDialog,public Ui_CScoreRec
{
	Q_OBJECT
public:
	CScoreRec(QWidget *parent=0);
private:
	QStandardItemModel *recSourceModel;
	QSortFilterProxyModel *recModel;
	QItemSelectionModel *selectionModel;
protected:
	void showEvent(QShowEvent *);
private slots:
	void setScoreRecModel(DB_FMT *data);
	void modeChanged(int);
	void displayTodayRec();
	void displayScoreRec();
	void filterChanged(const QString &);
	void exportToExcel();
};

#endif