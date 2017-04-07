#ifndef SCADAUI_H
#define SCADAUI_H
#include "uiappinterface.h"

#include "mdiMainWin.h"
#include "gemainwindow.h"
class QToolBar;
class QMenuBar;
class QMenu;

#define USERNAME_MAX_LEN  50
#define USERGROUP_NAME_MAX_LEN  50


#ifdef WIN32

#  ifdef FUNCUIAPP_DLL
#  define FUNCUIAPP_EXPORT __declspec(dllexport)
#  else
#  define FUNCUIAPP_EXPORT __declspec(dllimport)
#  endif

#else

#define FUNCUIAPP_EXPORT 

#endif



class FUNCUIAPP_EXPORT FuncUI: public UIAppInterface
{
	Q_OBJECT

public:
	FuncUI();
	~FuncUI();
	virtual QWidget * createMainView(QMainWindow *parent);
//	virtual void createAppMenuBar( QMainWindow* parent );	
	virtual void createAppMenuBar( QMainWindow* parent ,QMdiArea* parentmdiarea);
	virtual QToolBar* createAppToolBar( QMainWindow	* parent  );
	virtual QToolBar* createAppToolBar2( QMainWindow	* parent );

	//virtual QWidget * createMainView(MdiMainWindow *parent);
	virtual void createAppMenuBar( MdiMainWindow* parent );
	virtual QToolBar* createAppToolBar( MdiMainWindow	* parent  );
	//virtual QToolBar* createAppToolBar2( MdiMainWindow	* parent );

	virtual void installGraphInstance( void* );
	virtual void exeAfterLoaded();
//	virtual void setDataStreamPara(RdbMixFieldStream *prdbmixfieldstream,int datawino);

	void setCurrentOperateFlag(int operateflag);
	void setCurrentCardInfo(int menuID, int actType, char *cBmpName, char *cTipText);

	
public:
//	UIGraphInterface *m_graphInterface;
   
//  
	//CRealRtu *m_prtubuf;
	//Scadaservice *m_pscdservice;
	//CPasswordClass *m_passwd;


	//regman and passwd
	char m_regmain[32];	//
	char m_regpasswd[32];	//
	
	//yk man and passwd
	
	char  m_ykman[32];			//Ò£¿Ø²Ù×÷Ô±
	char  m_ykmanpwd[32];			//Ò£¿Ø²Ù×÷Ô±ÃÜÂë

	
private:
	int		m_dataSource;

	//QMainWindow* m_parent;
	MdiMainWindow* m_parent;
	QMdiArea* m_parentMdiarea;
	char m_logusername[USERNAME_MAX_LEN];
	char m_loggroupname[USERGROUP_NAME_MAX_LEN];
	

	QToolBar* m_appTools;   //scada tool bar
	QToolBar* m_graphModeTools;
	QToolBar* m_tempTools;
	
	QAction  *m_exploreRealtimeData;
	QAction  *m_exploreRealtimeEvent;
	QAction  *m_exploreHisData;
	QAction  *m_exploreHisEvent;

	QMenu* m_dataexplore;  //data operation


	QAction  *m_operatepane;
    //operate flag
	int m_operatef;

	//yk login dialog



	QAction* m_snapsave;
	QAction* m_cardoperate;

	//timer for check realtime message
	QTimer *m_pchkmsgtimer;

//	QProcess *m_dataprocess;
//	QProcess *m_eventprocess;
public:
	int m_cardMenuID;
	int m_cardActType;

public slots:
	virtual void mouseDownOnGraph( Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y );
	virtual void mouseUpOnGraph( Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y );
	virtual void keyDownOnGraph( int& nChar, Qt::KeyboardModifiers nShiftState );	
	virtual void mouseDoubleClickedOnGraph(Qt::MouseButtons  nButton, Qt::KeyboardModifiers nShiftState, int x, int y );
    void slotOperate();
	void slotSnapSave();
	void slotChkRealMsg();
	void slotCardOperate();
	void slotRBtnDownComp();
	void slotExploreRealData();
	void slotExploreRealEvent();
	void slotExploreHisEvent();
	void slotExploreHisData();

private:


};




#endif
