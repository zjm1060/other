#ifndef _LOGINREC_H_
#define _LOGINREC_H_

#include <QProgressDialog>
#include <QStandardItemModel>

#include "ui_loginrec.h"
#include "qcommon.h"
#include "excelobject.h"

class CLoginRecTable : public QDialog,public Ui_CLoginRecTable
{
	Q_OBJECT
	public:
		CLoginRecTable(QWidget *parent=0);
		QAction* actionDelLoginRec;
		QStandardItemModel *loginRecModel;
		QItemSelectionModel *selectionModel;
		void setControlAtt();
		void loadChinese();
		void loadEnglish();
		void init();
	private:
		void setLoginRecModel(DB_FMT *);
		void displayUser();
		void displayHost();
		int getTaskNum(char *start_time,char *end_time,int *task_num);
		int getOptNum(char *start_time,char *end_time,int *cmd_num,int *alm_num,int *msg_num);
	protected:
		void contextMenuEvent(QContextMenuEvent *);
		void showEvent(QShowEvent *);
	private slots:
		void displayLoginRec();
		void displayTodayLoginRec();
		void exportToExcel();
		void delLoginRec();
		void delLoginRecFromHdb(int);
	public slots:
		void setLanguage( int );
};
#endif