
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QSignalMapper>
#include <QDockWidget>
#include <QToolBar>
#include <QMenu>
#include <QToolBox>
#include <QListWidget>
#include <QSplashScreen>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include "gedocument.h"
#include "appwindow.h"
#include "mdiMainWin.h"
#include "uiappinterface.h"
class AppWindow;
class GraphicsView;
class GeDocument;
class MdiMainWindow;



class GeMainWindow :public MdiMainWindow//public QMainWindow
{
    Q_OBJECT

public:
	GeMainWindow(QSplashScreen *splash);
    ~GeMainWindow();

	static GeMainWindow* getAppWindow() 
	{
        return appWindow;
    }
	QMdiArea* getMdiArea() 
	{
        return m_mdiArea;
    }

	 AppWindow* getMDIWindow() 
	{
		if (m_mdiArea!=NULL) 
		{
			return ( AppWindow*)m_mdiArea->activeSubWindow();
		}
		else 
		{
			return NULL;
		}
    }


	 AppWindow *instanceApp();
	void closeEvent ( QCloseEvent* e );

	QMdiArea *getMDIArea(){return m_mdiArea;}
public:

	//app ,one instance for main process
	CreateAppInstanceProc m_createAppProc;
	DeleteAppInstanceProc m_deleteAppProc;
	UIAppInterface* m_appFuncInterface;  //func application

    QTimer *m_pmaintimer;    //main frame timer,for privileg operate

	char m_regmain[32];	//
	char m_regpasswd[32];	//

protected:
    int m_winno;   //winno for data stream 
private slots:
   void updateMenus();
   void setActiveSubWindow(QWidget *window);
   void openFile();
   void usrLogin();

   void printFile();

   void onoffgis();

   void slotZoomIn();
   void slotZoomOut();
   void slotZoomFit();
   void slotOpenGraphHot(QString filename);
   void slotHand();

private:
   void createGisToolBar();
   void createToolBar();
   void createZoomBar();
   void createActions();
   void createMenus();
   void createFileMenu();
   void createWindowMenu();
   void createHelpMenu();
   void createMainWidget();
    AppWindow *createMdiChild();
   QMdiSubWindow *findMdiChild(const QString &fileName);
    AppWindow *activeMdiChild();

   bool loadAppInterface( QString appLibName, QString appInterfaceName );
   bool loadHdAppInterface( QString appLibName, QString appInterfaceName );
   bool loadGraphMan( QString operLibName, QString operInterfaceName );
  void initialOpenFile();
   void rightTimeCheck();

   void installRegman2ScadaUI(char *regman,char *regmanpasswd);
public:

private:
    static GeMainWindow* appWindow;
     AppWindow* m_appwin ;
	/** Workspace for MDI */
//	QMdiArea *m_mdiArea;
//    QSignalMapper *m_windowMapper;
	
    QDockWidget* m_entity_general_property;

    QDockWidget* m_entity_dynamic_property;

    QDockWidget *m_toolbox_dockwidget;
    QToolBox    *m_powertoolbox;
    QListWidget *m_basic_item;
    QListWidget *m_power_item;
    QListWidget *m_composite_item;

   
	QToolBar *fileToolBar;
   	QMenu *fileMenu;
    QMenu  *helpMenu;
	QMenu  *m_pwindowMenu;

	QAction * usrLoginAct;


    QAction * openFileAct;
	QAction * printAct;

	QAction *exitAct;

	QAction *closeAct;
	QAction *closeAllAct;
	QAction *tileAct;
	QAction *cascadeAct;
    QAction * nextAct;
	QAction *previousAct;
    
	QAction *aboutAct;
	QAction *aboutQtAct;
	QAction *onOffgis;

	QToolBar *ZoomBar;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *zoomFitAct;
	QAction *handAct;

	bool m_bHandFlag;
 
private slots:
	  void about();
	  void exitslot();
};

#endif
