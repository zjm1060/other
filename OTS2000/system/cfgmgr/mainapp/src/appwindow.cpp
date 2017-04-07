#include <QLibrary>
#include <QMessageBox>
#include <QCloseEvent>
#include "appwindow.h"
#include "gemainwindow.h"

//#include "testui.h"

AppWindow::AppWindow( GeMainWindow* pMainWnd, QWidget* parent, int winno,const char* name/*,  Qt::WindowFlags wflags*/ )
: QMainWindow( parent/*,  QFlags(wflags)*/ )
{
	m_pMainWnd=pMainWnd;
    setAttribute(Qt::WA_DeleteOnClose);
	m_datawinno=winno;

//    setWindowFlags(wflags);
	if(pMainWnd)
	{
	   
//		if( pMainWnd->m_createGraphOperProc && pMainWnd->m_deleteGraphOperProc ) 
		{ 
	//		m_graphOperInterface = (UIGraphInterface*)(pMainWnd->m_createGraphOperProc)(); 
	//		if( m_graphOperInterface )      
	//		{
 //               m_pgraphview=m_graphOperInterface->createMainView((QMainWindow *)m_pMainWnd,this);
 //              // m_pgraphview=new Ctestwdg();//m_graphOperInterface->createMainView((QMainWindow *)m_pMainWnd,this);
	////			m_graphOperInterface->setDataStreamPara(m_pMainWnd->m_netstream,m_datawinno);  
 //               if(m_pgraphview)
	//			{
	//				setCentralWidget(m_pgraphview);
	//				m_bzoomFit = true;
	//			}
	//		}
		}


	}

// 
// 	m_createGarcGisrProc=0;
//     m_deleteGarcGisProc=0;
//     m_garcGisInterface=0;

	
	
	
}

AppWindow::~AppWindow()
{
	
 //   if(m_graphOperInterface)
	//{
	//	
	//   if(m_pMainWnd)
	//   {
	//	 m_pMainWnd->stopRdbStream(m_datawinno);
	//     m_pMainWnd->m_deleteGraphOperProc(m_graphOperInterface);
	//   }
	//   m_graphOperInterface=0;
	//}





// 	if(m_garcGisInterface)
// 	{
//        m_deleteGarcGisProc(m_garcGisInterface);
// 	   m_garcGisInterface=0;
// 	}
}

// graph control ,yk yt
// bool AppWindow::loadGraphControl(QString graphCtrlID, QString graphIID)
// {
// 	QLibrary *graphctrlif;
// 	graphctrlif=new QLibrary(graphCtrlID);
// 	if(!graphctrlif) return  false;
// 	
// 	bool loadflag=graphctrlif->load();
// 	if(loadflag)
// 	{
// 		m_createGraphProc = (CreateUIGraphInstanceProc)graphctrlif->resolve( "Create"+graphIID.toLocal8Bit()); 
// 		m_deleteGraphProc = (DeleteUIGraphInstanceProc)graphctrlif->resolve( "Delete"+graphIID.toLocal8Bit()); 
// 		if( m_createGraphProc && m_deleteGraphProc ) 
// 		{ 
// 			m_graphInterface = (UIGraphInterface*)m_createGraphProc(); 
// 			if( m_graphInterface )      
// 			{
// 				loadflag=true;
// 			}
// 			else
// 			{
// 				loadflag=false;
// 			}
// 			
// 		}
// 		else
// 		{
// 			qDebug("can not find graph control component \n");
// 			
// 		}
// 	}
// 	else
// 	{
// 		qDebug("can't find graph control component \n");
// 		QMessageBox::warning(0, QObject::tr("Load Library"),
// 			QObject::tr("Cannot load library%1,erroris %2")
// 			.arg(graphCtrlID).arg(graphctrlif->errorString()));
// 
// 	}
// 	
// 	return loadflag;
// 	
// }

// bool AppWindow::loadAppInterface( QString appLibName, QString appInterfaceName )
// {
// 	QLibrary *funcif;
// 	funcif=new QLibrary(appLibName);
// 	if(!funcif) return  false;
// 	
// 	bool loadflag=funcif->load();
// 	if(loadflag)
// 	{
// 		m_createAppProc = (CreateAppInstanceProc)funcif->resolve( "Create"+appInterfaceName.toLocal8Bit()); 
// 		m_deleteAppProc = (DeleteAppInstanceProc)funcif->resolve( "Delete"+appInterfaceName.toLocal8Bit()); 
// 		if( m_createAppProc && m_deleteAppProc ) 
// 		{ 
// 			m_appInterface = (UIAppInterface*)m_createAppProc(); 
// 			if( m_appInterface )      
// 			{
// 				m_appInterface->createAppMenuBar((QMainWindow *)m_pMainWnd);
// 				QToolBar* pToolBar = m_appInterface->createAppToolBar((QMainWindow *)m_pMainWnd);
// 				
// 				loadflag=true;
// 			}
// 			else
// 			{
// 				loadflag=false;
// 			}
// 		}
// 		else
// 		{
// 			qDebug("can not find app %s component \n",appLibName.toLocal8Bit());
// 		}
// 	}
// 	else
// 	{
// 		qDebug("can not find app %s component \n",appLibName.toLocal8Bit());
// 		QMessageBox::warning(0, QObject::tr("Load Library"),
// 			QObject::tr("Cannot load library%1,erroris %2")
// 			.arg(appLibName).arg(funcif->errorString()));
// 		
// 	}
// 	return loadflag;
// 	
// }

// bool AppWindow::loadGraphOperator( QString operLibName, QString operInterfaceName )
// {
// 	QLibrary *graphoperif;
// 	QWidget  *pgraphview=0;
// 	graphoperif=new QLibrary(operLibName);
// 	if(!graphoperif) return  false;
// 	
// 	bool loadflag=graphoperif->load();
// 	if(loadflag)
// 	{
// 		m_createGraphOperProc = (CreateUIGraphInstanceProc)graphoperif->resolve( "Create"+operInterfaceName.toLocal8Bit()); 
// 		m_deleteGraphOperProc = (DeleteUIGraphInstanceProc)graphoperif->resolve( "Delete"+operInterfaceName.toLocal8Bit()); 
// 		if( m_createGraphOperProc && m_deleteGraphOperProc ) 
// 		{ 
// 			m_graphOperInterface = (UIGraphInterface*)m_createGraphOperProc(); 
// 			if( m_graphOperInterface )      
// 			{
// // 				m_graphOperInterface->createAppMenuBar((QMainWindow *)m_pMainWnd);
// // 				QToolBar* pToolBar = m_graphOperInterface->createAppToolBar((QMainWindow *)m_pMainWnd);
//                 pgraphview=m_graphOperInterface->createMainView((QMainWindow *)m_pMainWnd);
//                 m_graphOperInterface->setDataStreamPara(m_pMainWnd->m_netstream,m_datawinno);  
//                 if(pgraphview)
// 				{
// 					setCentralWidget(pgraphview);
// 				}
// 				loadflag=true;
// 			}
// 			else
// 			{
// 				loadflag=false;
// 			}
// 		}
// 		else
// 		{
// 			qDebug("can not find GraphOperator %s component \n",operLibName.toLocal8Bit());
// 		}
// 	}
// 	else
// 	{
// 		qDebug("can not find GraphOperator %s component \n",operLibName.toLocal8Bit());
// 		QMessageBox::warning(0, QObject::tr("Load Library"),
// 			QObject::tr("Cannot load library%1,erroris %2")
// 			.arg(operLibName).arg(graphoperif->errorString()));
// 
// 	}
// 	return loadflag;	
// 	
// 	
// }
// 
// bool AppWindow::loadGraphMan( QString operLibName, QString operInterfaceName )
// {
// 	QLibrary *graphmanif;
// 	QWidget  *pgraphview=0;
// 	graphmanif=new QLibrary(operLibName);
// 	if(!graphmanif) return  false;
// 	
// 	bool loadflag=graphmanif->load();
// 	if(loadflag)
// 	{
// 		m_createGraphManProc = (CreateAppInstanceProc)graphmanif->resolve( "Create"+operInterfaceName.toLocal8Bit()); 
// 		m_deleteGraphManProc = (DeleteAppInstanceProc)graphmanif->resolve( "Delete"+operInterfaceName.toLocal8Bit()); 
// 		if( m_createGraphManProc && m_deleteGraphManProc ) 
// 		{ 
// 			m_graphManInterface = (UIAppInterface*)m_createGraphManProc(); 
// 			if( m_graphManInterface )      
// 			{
// // 				m_graphManInterface->createAppMenuBar((QMainWindow *)m_pMainWnd);
// // 				QToolBar* pToolBar = m_graphManInterface->createAppToolBar((QMainWindow *)m_pMainWnd);
//                 
// 				loadflag=true;
// 			}
// 			else
// 			{
// 				loadflag=false;
// 			}
// 		}
// 		else
// 		{
// 			qDebug("can not find GraphMan %s component \n",operLibName.toLocal8Bit());
// 		}
// 	}
// 	else
// 	{
// 		qDebug("can not find GraphOperator %s component \n",operLibName.toLocal8Bit());
// 		QMessageBox::warning(0, QObject::tr("Load Library"),
// 			QObject::tr("Cannot load library%1,erroris %2")
// 			.arg(operLibName).arg(graphmanif->errorString()));
// 		
// 	}
// 	return loadflag;	
// 	
// 	
// }
// 
// bool AppWindow::loadIasViewer( QString iasLibName, QString iasInterfaceName )
// {
// 	
// 	
// 	
// 	QLibrary *piasviewlib;
// 	piasviewlib=new QLibrary(iasLibName);
// 	bool loadflag=piasviewlib->load();
// 	if(loadflag)
// 	{
// 		m_createiasviewproc = (CreateAppInstanceProc)piasviewlib->resolve( "Create"+iasInterfaceName.toLocal8Bit()); 
// 		m_deleteiasviewproc = (DeleteAppInstanceProc)piasviewlib->resolve( "Delete"+iasInterfaceName.toLocal8Bit()); 
// 		if( m_createiasviewproc && m_deleteiasviewproc ) 
// 		{ 
// 			m_eventiasview = (IasView*)m_createiasviewproc(); 
// 			if( m_eventiasview )      
// 			{
// 				loadflag=true;
// 				
// 				addToolBar(Qt::BottomToolBarArea,m_eventiasview->createAppToolBar(this));
// 				
// 			}
// 			else
// 			{
// 				loadflag=false;
// 			}
// 			
// 		}
// 		else
// 		{
// 			qDebug("can not find iasviewer dll \n");
// 			
// 		}
// 	}
// 	else
// 	{
// 		qDebug("can nort find iasviewer dll");
// 	}
// 	
// 	return loadflag;
// 	
// }

// bool AppWindow::loadTableViewer( QString tableLibName, QString tableInterfaceName )
// {
// 	QLibrary *taboperif;
// 	QWidget  *ptabview=0;
// 	taboperif=new QLibrary(tableLibName);
// 	if(!taboperif) return  false;
// 	
// 	bool loadflag=taboperif->load();
// 	if(loadflag)
// 	{
// 		m_createGraphOperProc = (CreateAppInstanceProc)graphoperif->resolve( "Create"+operInterfaceName.toLocal8Bit()); 
// 		m_deleteGraphOperProc = (DeleteAppInstanceProc)graphoperif->resolve( "Delete"+operInterfaceName.toLocal8Bit()); 
// 		if( m_createGraphOperProc && m_deleteGraphOperProc ) 
// 		{ 
// 			m_graphOperInterface = (UIAppInterface*)m_createGraphOperProc(); 
// 			if( m_graphOperInterface )      
// 			{
// 				m_graphOperInterface->createAppMenuBar((QMainWindow *)m_pMainWnd);
// 				QToolBar* pToolBar = m_graphOperInterface->createAppToolBar((QMainWindow *)m_pMainWnd);
//                 pgraphview=m_graphOperInterface->createMainView((QMainWindow *)m_pMainWnd);
//                 m_graphOperInterface->setDataStreamPara(m_pMainWnd->m_netstream,m_datawinno);  
//                 if(pgraphview)
// 				{
// 					setCentralWidget(pgraphview);
// 				}
// 				loadflag=true;
// 			}
// 			else
// 			{
// 				loadflag=false;
// 			}
// 		}
// 		else
// 		{
// 			qDebug("can not find GraphOperator %s component \n",operLibName.toLocal8Bit());
// 		}
// 	}
// 	else
// 	{
// 		qDebug("can not find GraphOperator %s component \n",operLibName.toLocal8Bit());
// 		QMessageBox::warning(0, QObject::tr("Load Library"),
// 			QObject::tr("Cannot load library%1,erroris %2")
// 			.arg(operLibName).arg(graphoperif->errorString()));
// 		
// 	}
// 	return loadflag;
// }
  
// bool AppWindow::loadGarcGis( QString gisLibName, QString gisInterfaceName )
// {
// 	QLibrary *garcgisif;
// 	QWidget  *pgraphview=0;
// 	garcgisif=new QLibrary(gisLibName);
// 	if(!garcgisif) return  false;
// 
// 	bool loadflag=garcgisif->load();
// 	if(loadflag)
// 	{
// 		m_createGarcGisrProc = (CreateAppInstanceProc)garcgisif->resolve( "Create"+gisInterfaceName.toLocal8Bit()); 
// 		m_deleteGarcGisProc = (DeleteAppInstanceProc)garcgisif->resolve( "Delete"+gisInterfaceName.toLocal8Bit()); 
// 		if( m_createGarcGisrProc && m_deleteGarcGisProc ) 
// 		{ 
// 			m_garcGisInterface = (UIAppInterface*)m_createGarcGisrProc(); 
// 			if( m_garcGisInterface )      
// 			{
// 				m_garcGisInterface->createAppMenuBar((QMainWindow *)m_pMainWnd);
// 				QToolBar* pToolBar = m_garcGisInterface->createAppToolBar((QMainWindow *)m_pMainWnd);
// 				pgraphview=m_garcGisInterface->createMainView((QMainWindow *)m_pMainWnd);
// 				//m_garcGisInterface->setDataStreamPara(m_pMainWnd->m_netstream,m_datawinno);  
// 				if(pgraphview)
// 				{
// 					setCentralWidget(pgraphview);
// 				}
// 				loadflag=true;
// 			}
// 			else
// 			{
// 				loadflag=false;
// 			}
// 		}
// 		else
// 		{
// 			qDebug("can not find GraphOperator %s component \n",gisLibName.toLocal8Bit());
// 		}
// 	}
// 	else
// 	{
// 		qDebug("can not find GraphOperator %s component \n",gisLibName.toLocal8Bit());
// 		QMessageBox::warning(0, QObject::tr("Load Library"),
// 			QObject::tr("Cannot load library%1,erroris %2")
// 			.arg(gisLibName).arg(garcgisif->errorString()));
// 
// 	}
// 	return loadflag;	
// 
// 
// }
void AppWindow::resizeEvent(QResizeEvent* event)
{
	
}


void AppWindow::mouseMoveEvent( QMouseEvent *e )
{
	
}

// void AppWindow::closeEvent ( QCloseEvent* e )
// {

//     

// 	//prevent app window close

// 	e->ignore();

// 	return;

// 
// }


GraphicsView *AppWindow::getGraphicsView()
{
	return 0;
}

GeDocument *AppWindow::getDocument()
{
	return 0;  
}

//add by ligang for water meter--begin
void AppWindow::contextMenuEvent(QContextMenuEvent *event)
{

	//char title[FIELDINFOMAXLEN+10];
	//	char msgStr[256];
	//QMenu menu(this);
	//QAction *TimeAct=NULL;
	//
	////	int recno;
	//
	//TimeAct= new QAction(QIcon(":/app/images/settime.png"),tr("SetTime"), this);
	//connect(TimeAct,SIGNAL(triggered()),this,SLOT(slotRightbuttonEdit()));
	//
	//
	//menu.addAction(TimeAct);
	//menu.addSeparator();
	//menu.exec(QCursor::pos());
}

void AppWindow::slotRightbuttonEdit()
{//????,???????????
	
}
//add by ligang for water meter--end
// void AppWindow::installGraphInstance2GraphMan()
// {
//   if(!m_graphManInterface) return;
//   if(!m_graphOperInterface) return;
//   m_graphManInterface->installGraphInstance(m_graphOperInterface);
// 
// 
// }
// 
// void AppWindow::installGraphInstance2ScadaUI()
// {
//   if(!m_graphOperInterface)  return;
//   if(!m_appInterface) return;
//   m_appInterface->installGraphInstance(m_graphOperInterface);
// 
//   connect( m_graphOperInterface, SIGNAL( MouseDown(Qt::MouseButtons,Qt::KeyboardModifiers,int,int) ),
// 	  m_appInterface, SLOT( mouseDownOnGraph(Qt::MouseButtons,Qt::KeyboardModifiers,int,int) ) );
// 
// }