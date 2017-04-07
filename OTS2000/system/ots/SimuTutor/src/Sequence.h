#include "ui_SequenceList.h"//dzy
#include "ui_SequenceEdit.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "stndev.h"
#include <QtGui>
#include <QProcess>
#include <QDialog>
#include "qcommon.h"

class SequenceEdit;
typedef struct
{
	int seq_id;
	int opr_type;
	QString name;
	QString long_name;
	int station_id;
	int dev_id;
}Sequence;
class SequenceList:public QDialog,public Ui_SequenceList
{
	Q_OBJECT
public:
	SequenceList(QWidget *parent=0);
	int refreshList();
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
private:
	SequenceEdit* pSequenceEdit;
	CStnDevTable* stndev;
	int stnId,devId;
	bool select_status;
	bool select_stn;
	bool select_stndev;
	int curRow;
	int curColumn;
	int curSeqId;
	int rownums;
	QAction* actionAddSequence;
	QAction* actionModifySequence;
	QAction* actionDelSequence;
	QProcess* sequenceConfig;
	void moveEvent (QMoveEvent *);
	void contextMenuEvent(QContextMenuEvent *);
	void closeEvent(QCloseEvent *);
private slots:
	void onBtnAdd();
	void onBtnDel();
	void onBtnModify();
//	void onBtnParam();
	void onBtnCancel();
	void onRefresh();
	void addSequence();
	void modiSequence();
	void selectCell(int i,int j);
	void openSequenceConfig();
	void intoSequence();
	void openStnDev();
	void displayUpdate(int);
	void displayUpdate(int,int);
signals:
public slots:
	void setLanguage( int );
};

class SequenceEdit:public QDialog,public Ui_SequenceEdit
{
	Q_OBJECT
public:
	SequenceEdit(QWidget *parent=0);
	DATA_SEQUENCE seg;
	bool modi;
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
public slots:
	void setLanguage( int );
private slots:
	void onBtnOK();
	void onBtnCancel();
signals:
	void insertRecord();
	void modifyRecord();
};