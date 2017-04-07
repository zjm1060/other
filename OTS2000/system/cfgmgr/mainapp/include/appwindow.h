#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include "mdiMainWin.h"
#include "uiappinterface.h"
//#include "uigraphinterface.h"
//#include "iasview.h"
//#include "colormanager.h"


class GeMainWindow;
//class UIGraphInterface;
class UIAppInterface;
class GraphicsView;
class GeDocument;
class MdiMainWindow;


class AppWindow: public QMainWindow
{
	Q_OBJECT
public:
	AppWindow( GeMainWindow* pMainWnd, QWidget* parent, int winno,const char* name);//,  Qt::WindowFlags/*WidgetAttribute*/ wflags );
	~AppWindow();
// 
// 	bool loadGraphControl(QString graphCtrlID, QString graphIID=0);
// 	bool loadGraphOperator(QString operLibName, QString operInterfaceName=0);
// 	bool loadAppInterface( QString appLibName, QString appInterfaceName=0 );
// 	bool loadIasViewer( QString iasLibName, QString iasInterfaceName );
// 	bool loadTableViewer( QString tableLibName, QString tableInterfaceName );
// 	bool loadGarcGis(QString gisLibName, QString gisInterfaceName=0);
// 	bool loadGraphMan( QString operLibName, QString operInterfaceName=0 );

	void connectAppWithGraph();
	void afterInitialize();
// 	void installGraphInstance2GraphMan();//install graph instance to graph manipulate
// 	void installGraphInstance2ScadaUI();//install graph instance to scada ui

// 	UIGraphInterface* getGraphInterface() { 
// 		return m_hasGraph?m_graphInterface:NULL; };
// 	UIAppInterface* getAppInterface() {
// 		return m_hasApp?m_appInterface:NULL; };
// 	UIGraphInterface* getGraphOperInterface() {
// 		return m_hasGraphOper?m_graphOperInterface:NULL; };
		void setDataWindowNum(int winno){m_datawinno =winno;};
		
public:
	GeMainWindow* m_pMainWnd;
    QWidget *m_pgraphview;
	QString   m_appDescString;
	int       m_AppWindowType;
	bool      m_bzoomFit;


public:
    void appWindowIsActivated();
	GraphicsView *getGraphicsView();
    GeDocument *getDocument();	
	void setFileName(QString filename){m_filename=filename;};
	QString currentFile() { return m_filename; }

public slots:
	void slotRightbuttonEdit();

protected:
	void resizeEvent(QResizeEvent* event);
	void mouseMoveEvent( QMouseEvent * );
	void contextMenuEvent(QContextMenuEvent *event);
	
//	void closeEvent ( QCloseEvent* e );
    
public:
	//app 
// 	CreateAppInstanceProc m_createAppProc;
// 	DeleteAppInstanceProc m_deleteAppProc;
// 	UIAppInterface* m_appInterface;

// delete
//     //graph ctrl
// 	CreateUIGraphInstanceProc m_createGraphProc;
// 	DeleteUIGraphInstanceProc m_deleteGraphProc;
// 	UIGraphInterface* m_graphInterface;


	//graph operation,graph kernel 
// 	CreateUIGraphInstanceProc m_createGraphOperProc;
// 	DeleteUIGraphInstanceProc m_deleteGraphOperProc;
//	UIGraphInterface* m_graphOperInterface;




//     //graph manipulate  
// 	CreateAppInstanceProc m_createGraphManProc;
// 	DeleteAppInstanceProc m_deleteGraphManProc;
// 	UIAppInterface* m_graphManInterface;
// 
// 
// 	//gis operation
// 	CreateAppInstanceProc m_createGarcGisrProc;
// 	DeleteAppInstanceProc m_deleteGarcGisProc;
// 	UIAppInterface* m_garcGisInterface;
// 
// 
// 	//realtime alarm
// 
// 	CreateAppInstanceProc m_createiasviewproc;
// 	DeleteAppInstanceProc m_deleteiasviewproc;
// 	
// 	IasView* m_eventiasview;
// 
// 
// 
//    //table browse
// 
// 	CreateAppInstanceProc m_createTableOperProc;
// 	DeleteAppInstanceProc m_deleteTableOperProc;
// 	UIAppInterface* m_tableOperInterface;
// 


	bool m_hasApp;
	bool  m_firstWindowFlag;
//	QLibrary* m_appLib;
	bool m_hasGraphOper;
	bool m_hasGraph;

private:
	int m_datawinno;   //winno for rdb data fresh
	//current file name
	QString m_filename;

public:
};

#endif
