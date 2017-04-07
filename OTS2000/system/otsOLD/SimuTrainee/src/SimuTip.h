#ifndef SIMUTIP_H
#define SIMUTIP_H

#include <QWidget>
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include "ui_setSimuTip.h"
#include "calculation.h"

enum TIP_TYPE
{
	TIP_MODE_SINGLE,	//单一提示模式，只显示当前提示
	TIP_MODE_MULTI		//多提示模式，提示所有提示信息，当前提示显示不同颜色
};

class SimuTipThread : public QThread
{
	Q_OBJECT
public:
	SimuTipThread(int task_id,int send_task_id,int step_num,QObject *parent=NULL):m_task_id(task_id),m_send_task_id(send_task_id)
							{calc = new CTaskCalculation(m_task_id,m_send_task_id);m_step_num=step_num;stop_flag=false;};
	~SimuTipThread(){delete calc;CloseHDB(db_simutip);};
	void run();
	void stop(){stop_flag = true;};
Q_SIGNALS:
	void sigStepFinished(int step);
private:
	bool stop_flag;
	QMutex mutex;
	int m_task_id;
	int m_send_task_id;
	QSqlDatabase db_simutip;
	CTaskCalculation *calc;
	int m_step_num;
};


class CSetSimuTip : public QDialog,public Ui_CSetSimuTip
{
	Q_OBJECT
public:
	CSetSimuTip(QWidget *parent=0);
	void init();
private:
	QColor t_tipColor;
	QColor t_curTipColor;
	QColor t_backgroundColor;
	QFont t_tipFont;
	int t_mode;
	bool t_isAutoSize;
	QSize t_size;
	int t_alpha;
	void populateList();
signals:
	void sigUpdateAttrib();
	private slots:
		void setText(const QString &);
		void openSetColor();
		void openSetBackgroundColor();
		void openSetCurTipColor();
		void openSetFont();
		void changeOpacity(int);
		void setFontShow();
		void tipColorChanged(int);
		void curTipColorChanged(int);
		void setDialogMode(int);
		void setDialogSize(int);
		void setAutoSize(int);
		void backgroundColorChanged(int);
		void openSimuTip(bool);
		void buttonEnterClicked();
		void buttonApplyClicked();
protected:
	void showEvent(QShowEvent *);
	void closeEvent(QCloseEvent *);
};

class CSimuTipDlg:public QWidget
{
	 Q_OBJECT
public:
	CSimuTipDlg(QWidget *parent=0,int task_id = -1,int send_task_id = -1);
	~CSimuTipDlg();
	inline int getTaskId(){return m_task_id;};
	inline int getSendTaskId(){return m_send_task_id;};
	inline int getStepNum(){return mapTaskTip.count()-2;};
private:
	void setupUi();
	void updateTaskTipMap();
	BOOL isLocked;
	QPoint dragPosition;
	QGridLayout *layout;
	QMap<short,QLabel *> mapTaskTip;
	int curTaskTipStep;
	int m_task_id;
	int m_send_task_id;
	SimuTipThread *simuTipThread;
	QLabel *labelTip;
	int m_alpha;
public slots:
	void openSetSimuTip();
	void setText(const QString &);
	void setSize(QSize);
	void setAutoSize(bool);
	void setTipColor(QColor);
	void setBackgroundColor(QColor);
	void setFont(QFont);
	void setOpacity(int);
	void setCurTipColor(QColor);
	void setTransparent(bool);
	void updateAttrib();
private slots:
	void lock();
	void unLock();
	void setCurStepShow(int);
protected:
	void mousePressEvent(QMouseEvent *event);        //自定义一个鼠标点击事件函数
	void mouseMoveEvent(QMouseEvent *event);         //自定义一个鼠标拖动事件函数
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void paintEvent(QPaintEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void closeEvent(QCloseEvent *event);
};

extern CSimuTipDlg *dlgSimuTip;
extern CSimuTipDlg *dlgSimuTipExample;
extern CSetSimuTip *pSetSimuTip;
extern QColor tipColor;
extern QColor curTipColor;
extern QColor backgroundColor;
extern QFont tipFont;
extern int mode;
extern QPoint position;
extern QSize sizeW;
extern bool isAutoSize;
extern int alpha;
void WriteSimuTipConfig();
void ReadSimuTipConfig();
#endif