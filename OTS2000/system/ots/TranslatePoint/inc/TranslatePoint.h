#ifndef _TRANSLATEPOINT_H
#define _TRANSLATEPOINT_H

#include <QtGui>
#include <QApplication>
#include "inc/PopupMsg.h"
#include "inc/common.h"

class CTranslatePoint : public QMainWindow
{
	Q_OBJECT
	public:
		CTranslatePoint(QWidget *parent=0);
		~CTranslatePoint();
	protected:
		void closeEvent(QCloseEvent *event);
	private:
		/************************************close dialog*******************************/
		QDialog *closeTable;
		QCheckBox   *chk;
		QRadioButton *chkMin;
		QRadioButton *chkClose;
		QLabel      *lbl;
		QPushButton *butEnter;
		QPushButton *butExit;
		QSystemTrayIcon* iTrayIcon;
		QMenu* trayIconMenu;
		QAction* minimizeAction;
		QAction* maximizeAction;
		QAction* restoreAction;
		QAction* quitAction;
		QAction* startAction;
		bool flagStart;
		bool ifshowtip;
		void createActions();
		void createTrayIcon();
		void createTrayMenu();
		/************************************close dialog*******************************/
		QClipboard *board;
		CPopupMsg *pPopupMsg;
	signals:
	private slots:
		void isQuit();
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
		void setStartGrab();
		void oncheckbox(int state);
		void checkQuit();
		void openPopupMsg();
};
#endif