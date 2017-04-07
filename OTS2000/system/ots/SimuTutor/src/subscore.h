#ifndef _SUBSCORE_H_
#define _SUBSCORE_H_
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
#include "ui_subscore.h"

class SubScore:public QWidget,public Ui_SubScore
{
	Q_OBJECT
public:
	SubScore(QWidget *parent=0);
	~SubScore();
	DATA_SCORE m_score;
	bool showable;
	void /*setScoreInfo(int send_task_id);*/setScoreInfo(int send_task_id,float score,float tutorscore);
	void loadChinese();
	void loadEnglish();
private:
 	QStandardItemModel *subscoreModel;
 	QItemSelectionModel *selectionModel;
protected:
	void showEvent(QShowEvent *);
private slots:
  	void setsubScoreModel(DB_FMT *data);
// 	void modeChanged(int);
 //	void displaysubScore();
// 	void filterChanged(const QString &);
// 	void exportToExcel();
	void setLanguage(int language);

};

#endif