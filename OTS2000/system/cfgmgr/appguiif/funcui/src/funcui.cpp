#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QMainWindow>

#include "funcui.h"
#include "testui.h"
//#include "netdef.h"


extern "C" {
	
	FUNCUIAPP_EXPORT FuncUI* FUNCAppInterface;
	
	FUNCUIAPP_EXPORT void* CreateFUNCAppInterface()
	{
		FuncUI* instance = new FuncUI();
		
		return instance;
	}
	
	FUNCUIAPP_EXPORT void DeleteFUNCAppInterface(void* ptr)
	{
		FuncUI* instance = (FuncUI*)ptr;
		if( instance )	delete instance;
	}
	
} /* end of extern "C" */

FuncUI::FuncUI()
: UIAppInterface()
{



	m_regmain[0]='\0';
	m_regpasswd[0]='\0';

	m_ykman[0]='\0';
	m_ykmanpwd[0]='\0';

	m_cardMenuID = -1;
	m_cardActType = -1;

//	m_pscdservice= new Scadaservice();
//	parent.set_main_qwidget(this);

//	m_dataprocess = new QProcess(this);

//	m_eventprocess = new QProcess(this);

}

FuncUI::~FuncUI()
{
	

}
QWidget * FuncUI::createMainView(QMainWindow *parent)
{
	return 0;
	
	
}
/*
void FuncUI::createAppMenuBar( QMainWindow* parent )
{


	if( !parent ) return;
	m_parent = parent;

	//operate

	m_operatepane = new QAction(QIcon(":/images/operate.png"), tr("&Operate  "), this);
	m_operatepane->setShortcut(tr("Ctrl+O"));
	m_operatepane->setStatusTip(tr("Operate devive"));
	connect(m_operatepane, SIGNAL(triggered()), this, SLOT(slotOperate()));

	//menu
	m_dataexplore=m_parent->menuBar()->addMenu(tr("Data &Explore"));
	m_dataexplore->addAction(m_operatepane);


}*/
void FuncUI::createAppMenuBar( QMainWindow* parent ,QMdiArea* parentmdiarea)
{
	
/*	
	if( !parent ||!parentmdiarea) return;
	m_parent = parent;
	m_parentMdiarea=parentmdiarea;


	//operate


	m_operatepane = new QAction(QIcon(":/images/operate.png"), tr("&Operate  "), this);
	m_operatepane->setShortcut(tr("Ctrl+O"));
	m_operatepane->setStatusTip(tr("Operate devive"));
	connect(m_operatepane, SIGNAL(triggered()), this, SLOT(slotOperate()));



	////explore realtime data
	//m_exploreRealtimeData = new QAction(QIcon(":/images/realdata.png"), tr("&Real  Data..."), this);
	//m_exploreRealtimeData->setShortcut(tr("Ctrl+R"));
	//m_exploreRealtimeData->setStatusTip(tr("Explore Realtime Data"));
	//connect(m_exploreRealtimeData, SIGNAL(triggered()), this, SLOT(slotExploreRealData()));



	//m_exploreRealtimeEvent = new QAction(QIcon(":/images/realevent.png"), tr("Real &Event ..."), this);
	//m_exploreRealtimeEvent->setShortcut(tr("Ctrl+E"));
	//m_exploreRealtimeEvent->setStatusTip(tr("Explore Realtime Event"));
	//connect(m_exploreRealtimeEvent, SIGNAL(triggered()), this, SLOT(slotExploreRealEvent()));



	//m_exploreHisEvent = new QAction(QIcon(":/images/hisevent.png"), tr("&His Event ..."), this);
	//m_exploreHisEvent->setShortcut(tr("Ctrl+H"));
	//m_exploreHisEvent->setStatusTip(tr("Explore His Event"));
	//connect(m_exploreHisEvent, SIGNAL(triggered()), this, SLOT(slotExploreHisEvent()));

	//m_exploreHisData = new QAction(QIcon(":/images/hisdata.png"), tr("His &Data ..."), this);
	//m_exploreHisData->setShortcut(tr("Ctrl+D"));
	//m_exploreHisData->setStatusTip(tr("Explore His Data"));
	//connect(m_exploreHisData, SIGNAL(triggered()), this, SLOT(slotExploreHisData()));

	//m_snapsave = new QAction(QIcon(":/images/snapshot.png"), tr("&Snap save ..."), this);
	//m_snapsave->setShortcut(tr("Ctrl+S"));
	//m_snapsave->setStatusTip(tr("Explore Snap Save"));
	//connect(m_snapsave, SIGNAL(triggered()), this, SLOT(slotSnapSave()));

	////m_snapsave = new QAction(QIcon(":/images/snapshot.png"), tr("&Snap save ..."), this);
	////m_snapsave->setShortcut(tr("Ctrl+S"));
	////m_snapsave->setStatusTip(tr("Explore Snap Save"));
	////connect(m_snapsave, SIGNAL(triggered()), this, SLOT(slotSnapSave()));


	//m_cardoperate = new QAction(QIcon(":/images/cardoperate.png"), tr("&Card Operate ..."), this);
	//m_cardoperate->setShortcut(tr("Ctrl+O"));
	//m_cardoperate->setStatusTip(tr("Explore Card Operate"));
	//connect(m_cardoperate, SIGNAL(triggered()), this, SLOT(slotCardOperate()));


	//menu


	m_dataexplore=m_parent->menuBar()->addMenu(tr("Data &Explore"));


	m_dataexplore->addAction(m_operatepane);*/
	/*m_dataexplore->addSeparator();
	m_dataexplore->addAction(m_exploreRealtimeData);
	m_dataexplore->addAction(m_exploreRealtimeEvent);
	m_dataexplore->addAction(m_exploreHisEvent);
	m_dataexplore->addAction(m_exploreHisData);
	m_dataexplore->addAction(m_cardoperate);*/
}
void FuncUI::createAppMenuBar( MdiMainWindow* parent )
{
	if( !parent ) return;
	m_parent = parent;

	m_parentMdiarea=m_parent->m_mdiArea;
	//operate


	m_operatepane = new QAction(QIcon(":/images/operate.png"), tr("&Operate  "), this);
	m_operatepane->setShortcut(tr("Ctrl+O"));
	m_operatepane->setStatusTip(tr("Operate devive"));
	connect(m_operatepane, SIGNAL(triggered()), this, SLOT(slotOperate()));

	m_dataexplore=m_parent->menuBar()->addMenu(tr("Data &Explore"));


	m_dataexplore->addAction(m_operatepane);

}
QToolBar* FuncUI::createAppToolBar( QMainWindow* parent  )
{
	if( !parent ) return NULL;
//	m_parent = parent;
	
	//m_appTools=m_parent->addToolBar(tr("FuncUI ToolBar"));
    m_appTools= new QToolBar(tr("FuncUI ToolBar"),parent);
    parent->addToolBar(m_appTools);
	m_appTools->addAction(m_operatepane);
	
	//m_appTools->addAction(m_exploreRealtimeData);
	//m_appTools->addAction(m_exploreRealtimeEvent);
	//m_appTools->addAction(m_exploreHisEvent);
	//m_appTools->addAction(m_exploreHisData);
	//m_appTools->addAction(m_snapsave);
	//m_appTools->addAction(m_cardoperate);
	
	
	return m_appTools;
}
QToolBar* FuncUI::createAppToolBar( MdiMainWindow	* parent  )
{
	return NULL;
}
QToolBar* FuncUI::createAppToolBar2( QMainWindow* parent )
{
	return NULL;
}


void FuncUI::mouseDownOnGraph( Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y )
{
	
	
}

void FuncUI::mouseUpOnGraph( Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y )
{
}

void FuncUI::keyDownOnGraph( int& nChar, Qt::KeyboardModifiers nShiftState )
{
	
	
}
void FuncUI::mouseDoubleClickedOnGraph(Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y )
{
	
	
}


void FuncUI::exeAfterLoaded()
{	
	
}


void FuncUI::installGraphInstance( void* graphObj )
{
	
}


void FuncUI::slotOperate()
{

	Ctestwdg* graphicsview=new Ctestwdg();
	//m_parent->setCentralWidget(graphicsview);
	m_parentMdiarea->addSubWindow(graphicsview);
	graphicsview->showMaximized();
	//graphicsview->show();
	//m_parent->getMDIArea()->addSubWindow(graphicsview);
	//return;

	//QDesktopWidget *desktop = QApplication::desktop();
	//int desktop_w = desktop->width();     
	//int desktop_h = desktop->height();

	//if (m_operatepanedlg) 
	//{
	//	//m_operatepanedlg->destroy(true, true);
	//	m_operatepanedlg->close();
	//	delete m_operatepanedlg;
	//	m_operatepanedlg = NULL;
	//}

	//if(m_operatepanedlg ==0)
	//{
	//	m_operatepanedlg=  new operatePane(this);
	//}
	//else
	//{
	//	m_operatepanedlg->initPara();
	//}
	//int operatw=m_operatepanedlg->width ();
	//m_operatepanedlg->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Drawer);
	//int prisrcno=desktop->primaryScreen();
	//QRect firstsrc_rect;
	//if(prisrcno >0)
 //     firstsrc_rect=desktop->screenGeometry (prisrcno);
 //   else
 //     firstsrc_rect=desktop->screenGeometry (0);

	//desktop_w=firstsrc_rect.width();
	//m_operatepanedlg->move(desktop_w-operatw-40,40);
	//m_operatepanedlg->show();


}

void FuncUI::slotCardOperate()
{
	


}

void FuncUI::setCurrentOperateFlag(int operateflag)
{
	
}

void FuncUI::setCurrentCardInfo(int menuID, int actType, char *cBmpName, char *cTipText)
{
	
}


void FuncUI::slotSnapSave()
{
	
	
}

void FuncUI::slotChkRealMsg()
{
  

}



void FuncUI::slotRBtnDownComp()
{

	
}

void FuncUI::slotExploreRealData()
{
	
}

void FuncUI::slotExploreHisData()
{
	
}

void FuncUI::slotExploreRealEvent()
{
	
}

void FuncUI::slotExploreHisEvent()
{
	
}