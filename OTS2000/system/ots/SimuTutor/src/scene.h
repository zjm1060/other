#ifndef _SCENE_H_
#define _SCENE_H_


#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "ui_scene.h"
#include "scenedata.h"
#include "ui_sceneadd.h"
#include "login.h"
#include "stndev.h"
#include "qcommon.h"

class CSceneAddTable : public QDialog,public Ui_CSceneAddTable
{
	Q_OBJECT
public:
	CSceneAddTable(QWidget *parent=0);
	DATA_SCENE seg;
	bool flag_add,flag_modify;
	void setControlAtt();
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
signals:
	void insertRecord();
	void modifyRecord();
public slots:
	void setLanguage( int );
private slots:
	void apply();
	void enter();
	void displayStation();
	void displayDev(int stn);
};
class CSceneTable : public QDialog,public Ui_CSceneTable
{
	Q_OBJECT
public:
	CSceneTable(QWidget *parent=0);
	CSceneAddTable* sceneadd;
	CSceneAddTable* scenemodify;
	CSceneDataTable* scenedata;
	CStnDevTable* stndev;
	int curRow;
	int curColumn;
	int curSceneId;
	int rownums;
	int stnId,devId;
	bool select_status;
	bool select_stn;
	bool select_stndev;
	QAction* actionAddScene;
	QAction* actionModifyScene;
	QAction* actionDelScene;
	QAction* actionCopyScene;
	QAction* actionSceneData;
	QAction* actionAddToMergerList;
	QAction* actionDelMergerScene;
	void setControlAtt();
	void moveEvent (QMoveEvent *);
	//void contextMenuEvent(QContextMenuEvent *);
	void closeEvent(QCloseEvent *);
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
private slots:
	void menuListScene(const QPoint &);
	void menuListSceneMerger(const QPoint &);
public slots:
		int displayScene();
		void selectCell(int,int);
		void delScene();
		void openSceneData();
		void openSceneAdd();
		void openSceneModify();
		void openStnDev();
		void addScene();
		void modifyScene();
		void intoScene();
		void copyScene();
		void displayUpdate(int);
		void displayUpdate(int,int);
		void addSceneToMergerList();
		void addSceneToMergerList(int,int);
		void delFromMergerList();
		void clearMerger();
		void runSceneMerger();
		void setLanguage( int );
};
#endif