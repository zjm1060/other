#ifndef _REVERTPLAYER_H_
#define _REVERTPLAYER_H_

#include <QtGui/qwidget.h>
#include <QMdiSubWindow>
#include "ui_RevertPlayer.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"

class QMdiSubWindow;
Q_DECLARE_METATYPE(DATA_REVERT);
class CRevertPlayer:public QWidget,public Ui_RevertPlayer
{
	Q_OBJECT
public:
	CRevertPlayer(QWidget *parent=0);
	~CRevertPlayer();
	void loadChinese();
	void loadEnglish();
private:
	short revert_status;//0:工况再现未开始或停止 1:工况再现运行中 2:工况再现暂停 
	short speed; 
	QString revertMsg;
	DATA_REVERT cur_revert;
	QDateTime cur_time,start_time,end_time;
	int totle_sec,h,m,s;
	QTimer *timer;
	void init();
	void displayRevertList();
private Q_SLOTS:
	void isRevertListShow();
	void setSpeedLabel(int);
	void startOrPauseRevert();
	void stopRevert();
	void forwardRevert();
	void backwardRevert();
	void revertSelected(const QModelIndex &index);
	void updateDisplay();
	void setRevertProgress(int);
	void PopupRevertDetail(const QModelIndex &index);
protected:
	void showEvent(QShowEvent *);
	void closeEvent(QCloseEvent *);
	public slots:
		void setLanguage( int );
};

#endif