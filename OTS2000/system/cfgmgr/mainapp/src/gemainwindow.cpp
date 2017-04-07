
#include <QtGui>

#include "gemainwindow.h"



GeMainWindow::GeMainWindow(QSplashScreen *splash)
{
	m_winno=0;   //data stream window number	
	m_appwin=0;
	
	m_pmaintimer= 0;
	m_pmaintimer= new QTimer(this);

//	connect(m_pmaintimer, SIGNAL(timeout()), this, SLOT(chkMainWindow()));
 //    m_pmaintimer->start(1000);
//	

	//initialize component

	m_createAppProc=0;
    m_deleteAppProc=0;
    m_appFuncInterface=0;

	m_mdiArea = new QMdiArea;
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(m_mdiArea);
    connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
		this, SLOT(updateMenus()));
    m_windowMapper = new QSignalMapper(this);
    connect(m_windowMapper, SIGNAL(mapped(QWidget *)),
		this, SLOT(setActiveSubWindow(QWidget *)));



	createActions();
	
	createToolBar();
	
	createFileMenu();
//	createGisToolBar();
//	createZoomBar();

	setWindowTitle(tr("cfgmgr interface"));



// 	//scada service
// 
// 	m_pscdservice= new Scadaservice();

	//real data service
//	m_netstream = new RdbMixFieldStream();
	
	

	//load ias component ,and instance it
//    loadIasViewer(QString::fromLocal8Bit("iasviewer"),QString::fromLocal8Bit("IasViewerUIInstance"));

	//load goper function pointer,no instance
//    loadGraphOperator(QString::fromLocal8Bit("goper"),QString::fromLocal8Bit("GraphOperInstance"));


	//load graph manipulate engine,instance it
//	loadGraphMan(QString::fromLocal8Bit("gman"),QString::fromLocal8Bit("GraphManInstance"));

	//load  funcui component,instance it
//	loadAppInterface(QString::fromLocal8Bit("funcui"),QString::fromLocal8Bit("FUNCAppInterface"));

	loadAppInterface(QString::fromLocal8Bit("checkui"),QString::fromLocal8Bit("CHECKAppInterface"));


	//load hd operator component,instance it
//	loadHdAppInterface(QString::fromLocal8Bit("hdoperui"),QString::fromLocal8Bit("HDOperAppInterface"));


//	createMdiChild();
//	initialOpenFile();

	
	createWindowMenu();
	createHelpMenu();

	m_regmain[0]='\0';
	m_regpasswd[0]='\0';

}

GeMainWindow::~GeMainWindow()
{


	
	if(m_appFuncInterface)
	{
		m_deleteAppProc(m_appFuncInterface);
		m_appFuncInterface=0;
	}




// 	if(m_pscdservice)
// 		delete m_pscdservice;




}


//************************************
// Method:    initialOpenFile
// FullName:  GeMainWindow::initialOpenFile
// Access:    private 
// Returns:   void
// Qualifier:open index.pic if it exists,otherwise,open a untiled file
//************************************
void GeMainWindow::initialOpenFile()
{
	/*
	char* tmp_env = NULL;
	char graph_home[100];
	char tmpfilename[255];
	
	tmp_env = getenv("WSCD_HOME");
	if( tmp_env )
	{
		sprintf(graph_home,"%s%s",tmp_env,"/pic/");
	}
	else
	{
#if defined(WIN32)
		strcpy( graph_home, "d:/wapp/pic/" );
#elif defined(__sun)
		strcpy( graph_home, "/home/wscd2000/pic/" );

#endif
	}
    strcpy(tmpfilename,graph_home);
	strcat(tmpfilename,"index.pic");

   	QFile graphgile(tmpfilename);


	AppWindow *pchild ;
	if ((graphgile.size() == 0)  
		||(!graphgile.exists()))
	{
	     pchild=createMdiChild();
		 if(!pchild) return ;
		 pchild->setWindowTitle("untiled");
		 pchild->setFileName("untiled");

	}
	else
	{
	      pchild=createMdiChild();
		  if(!pchild) return ;
		  if(!pchild->m_graphOperInterface) return;
//		  pchild->m_graphOperInterface->loadPicFile(tmpfilename);
//		  pchild->setWindowTitle(tmpfilename);
//		  pchild->setFileName(tmpfilename);
	}
*/
}
void GeMainWindow::exitslot()
{
	exit(0);
}
void GeMainWindow::createActions()
{
	
	//usr login 
	exitAct= new QAction(QIcon(":app/images/login.png"),tr("&exit"), this);
	//exitAct->setShortcut(tr("Ctrl+"));
	exitAct->setStatusTip(tr("exit  "));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(exitslot()));
	
	//usr login 
	usrLoginAct= new QAction(QIcon(":app/images/login.png"),tr("&Login"), this);
	usrLoginAct->setShortcut(tr("Ctrl+L"));
	usrLoginAct->setStatusTip(tr("usr Login  "));
	connect(usrLoginAct, SIGNAL(triggered()), this, SLOT(usrLogin()));
	
	//open graph file 	
	openFileAct = new QAction(QIcon(":/images/opengraph.png"), tr("&Open..."), this);
    openFileAct->setShortcut(tr("Ctrl+O"));
    openFileAct->setStatusTip(tr("Open an existing graph file"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));
		
	
	//print 	
	printAct =new QAction(QIcon(":/images/printgraph.png"), tr("&Print"), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setStatusTip(tr("Print the graph to printer"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(printFile()));
	
	
	//multi window opr
	closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setShortcut(tr("Ctrl+F4"));
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()),
		m_mdiArea, SLOT(closeActiveSubWindow()));
	
    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()),
		m_mdiArea, SLOT(closeAllSubWindows()));
	
    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), m_mdiArea, SLOT(tileSubWindows()));
	
    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), m_mdiArea, SLOT(cascadeSubWindows()));
	
	nextAct = new QAction(tr("Ne&xt"), this);
	nextAct->setStatusTip(tr("Move the focus to the next window"));
	connect(nextAct, SIGNAL(triggered()),
		m_mdiArea, SLOT(activateNextSubWindow()));

	previousAct = new QAction(tr("Pre&vious"), this);
	previousAct->setStatusTip(tr("Move the focus to the previous "
		"window"));
	connect(previousAct, SIGNAL(triggered()),
		m_mdiArea, SLOT(activatePreviousSubWindow()));



	//help
	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));


	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	   
}
void GeMainWindow::createToolBar()
{
	
	return;
	fileToolBar=addToolBar(tr("File"));
//    fileToolBar->addAction(openFileAct);
	fileToolBar->addSeparator();
    fileToolBar->addAction(usrLoginAct);

	
}


void GeMainWindow::createFileMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));

//	fileMenu->addAction(openFileAct);
//	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
}
void GeMainWindow::createWindowMenu()
{
	m_pwindowMenu=menuBar()->addMenu(tr("&Window"));
	m_pwindowMenu->addAction(closeAct);
	m_pwindowMenu->addAction(closeAllAct);
	m_pwindowMenu->addSeparator();

	m_pwindowMenu->addAction(tileAct);
	m_pwindowMenu->addAction(cascadeAct);
	m_pwindowMenu->addSeparator();

	m_pwindowMenu->addAction(nextAct);
	m_pwindowMenu->addAction(previousAct);
}
void GeMainWindow::createHelpMenu()
{
	     helpMenu = menuBar()->addMenu(tr("&Help"));
	     helpMenu->addAction(aboutAct);
	     helpMenu->addAction(aboutQtAct);
}


void GeMainWindow::createGisToolBar()
{
	QAction* onOffgis= new QAction(QIcon(":/images/opengis.png"),tr("&ON OFF GIS"), this);
	onOffgis->setStatusTip(tr("ON/OFF GIS  "));
	connect(onOffgis, SIGNAL(triggered()), this, SLOT(onoffgis()));

	QToolBar * gistoolbar ;//=new QToolBar(this);
	gistoolbar=addToolBar(tr("Gis"));
	gistoolbar->addAction(onOffgis);
}

void GeMainWindow::createZoomBar()
{
	//zoom in 
	zoomInAct= new QAction(QIcon(":/images/zoomin.png"),tr("&ZoomIn"), this);
	zoomInAct->setShortcut(tr("Ctrl+I"));
	zoomInAct->setStatusTip(tr("Pragh zoom in"));
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(slotZoomIn()));

	//zoom out 
	zoomOutAct= new QAction(QIcon(":/images/zoomout.png"),tr("&ZoomOut"), this);
	zoomOutAct->setShortcut(tr("Ctrl+O"));
	zoomOutAct->setStatusTip(tr("Pragh zoom out"));
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(slotZoomOut()));

	//zoom fit 
	zoomFitAct= new QAction(QIcon(":/images/zoomfit.png"),tr("&ZoomFit"), this);
	zoomFitAct->setShortcut(tr("Ctrl+F"));
	zoomFitAct->setStatusTip(tr("Pragh zoom fit"));
	connect(zoomFitAct, SIGNAL(triggered()), this, SLOT(slotZoomFit()));

	handAct=new QAction(QIcon(":/images/hand.png"), tr("drag"), this);
	handAct->setCheckable(true);
	handAct->setShortcut(tr("Ctrl+O"));
	handAct->setStatusTip(tr("Hand"));
	connect(handAct, SIGNAL(triggered()), this, SLOT(slotHand()));

	ZoomBar=addToolBar(tr("Zoom"));
	ZoomBar->addAction(zoomInAct);
	ZoomBar->addAction(zoomOutAct);
	ZoomBar->addAction(zoomFitAct);
	ZoomBar->addAction(handAct);
	ZoomBar->addSeparator();
}


AppWindow *GeMainWindow::createMdiChild()
{
	// AppWindow *child = new AppWindow(this,this,"app window");
	
	
    AppWindow *child=  instanceApp();
	
    return child;
}

QMdiSubWindow *GeMainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
	
    foreach (QMdiSubWindow *window, m_mdiArea->subWindowList()) {
        AppWindow *mdiChild = qobject_cast<AppWindow *>(window->widget());
		QString currentfilename = mdiChild->currentFile();
		currentfilename.replace(QString::fromLocal8Bit("\\"), QString::fromLocal8Bit("/"));
		if (mdiChild->currentFile() == canonicalFilePath || currentfilename == canonicalFilePath)
		{
		   return window;
		}
    }
    return 0;
}

void GeMainWindow::openFile()
{
	char* tmp_env = NULL;
	char graph_home[100];
	
	tmp_env = getenv("WSCD_HOME");
	if( tmp_env )
	{
		sprintf(graph_home,"%s%s",tmp_env,"/pic");
	}
	else
		strcpy( graph_home, "d:/wapp/pic" );
	
	
	QString tmpfilename = QFileDialog::getOpenFileName(this,
		tr("open graph file dialog"),
		graph_home, 
		tr("Pic File (*.pic)"));
	
   	QFile graphgile(tmpfilename);
	
	if (graphgile.size() == 0)  return;


    AppWindow  *existingAppWin;

   // QString fileName = QFileDialog::getOpenFileName(this);
/*    if (!tmpfilename.isEmpty()) 
	{
        QMdiSubWindow *existing = findMdiChild(tmpfilename);
        if (existing) 
		{

            existingAppWin=(AppWindow*)existing->widget();
			if(existingAppWin->m_graphOperInterface)
			{
				existingAppWin->m_graphOperInterface->loadPicFile(tmpfilename.toLocal8Bit().data());
               
			}
			installGraphInstance2GraphMan(existingAppWin);
	        installGraphInstance2ScadaUI(existingAppWin);
    	
            m_mdiArea->setActiveSubWindow(existing);
            return;
        }
		
        AppWindow *child = createMdiChild();
		if(!child) return ;
		if(!child->m_graphOperInterface) return;
		child->m_graphOperInterface->loadPicFile(tmpfilename.toLocal8Bit().data());
		child->setWindowTitle(tmpfilename);
		child->setFileName(tmpfilename);
		
    }*/
}

void GeMainWindow::about()
{
	QMessageBox::about(this, tr("About cfgmgr"),
		tr("The <b>cfgmgr</b>  interface  using Qt."));
}


void GeMainWindow::updateMenus()
{
	AppWindow* existingAppWin = activeMdiChild();
/*
	if (existingAppWin)
	{
		if (existingAppWin->m_graphOperInterface)
		{
			existingAppWin->m_graphOperInterface->setHandFlag(m_bHandFlag);
		}
	}*/
}

AppWindow *GeMainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<AppWindow *>(activeSubWindow->widget());
    return 0;
}
void GeMainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    m_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}


void GeMainWindow::printFile()
{



}

void GeMainWindow::usrLogin()
{
	int Index;
    char Pass[20];
/*
	if (m_passwordclass) delete m_passwordclass;
	m_passwordclass = 0;
	m_passwordclass = new CPasswordClass();

	if (m_passwordclass->CallParaFromLib () == 0 )
	{		
		statusBar()->showMessage(tr("加载节点权限配置错误 !"), 2000);
		return;
	}

	if (m_passwordclass)
	{
		m_passwordclass->SetMaxCount (120);//120 from config file of cfgmgr
		m_passwordclass->SetTimeCount (0);
	}

	installPasswd2ScadaUI(m_passwordclass);
 //printf("installPasswd2ScadaUI end\n");
	ZPasswordDlg Dlg (this, m_passwordclass, Index, Pass, 0);

    if (Dlg.exec () == QDialog::Accepted)
	{

        if (Index == 0)
		{
            m_passwordclass->SetManIndex (0);
			usrLoginAct->setIcon(QIcon(":app/images/login.png"));
        }
        else if (Index>0 && Index<m_passwordclass->GetManNum())
		{
            if (strcmp ((m_passwordclass->GetManAttr()+Index)->Password, Pass) != 0)
			{
			//	printf("Password=%s,Pass=%s\n",(m_passwordclass->GetManAttr()+Index)->Password, Pass);
				QMessageBox::warning (this, tr("passwd error !"), tr("operator"));
				usrLoginAct->setIcon(QIcon(":app/images/login.png"));
			}
            else
			{
                m_passwordclass->SetManIndex(Index);
				strcpy (m_regmain, (m_passwordclass->GetManAttr()+Index)->ManName);
				strcpy (m_regpasswd, (m_passwordclass->GetManAttr()+Index)->Password);

				installRegman2ScadaUI(m_regmain,m_regpasswd);
				usrLoginAct->setIcon(QIcon(":app/images/loginok.png"));
            }
        }
		
		if (m_power) delete m_power;
		m_power = NULL;
		m_power = new SPower;
		memset(m_power, 0, sizeof(SPower));
		if (m_passwordclass && m_passwordclass->GetManAttr())
			memcpy(m_power, &(m_passwordclass->GetCurrentMan()->Power), sizeof(SPower));
		
// 		for (int i = 0; i < ChildFrameNum; i++)
// 		{
// 			if (ChildFrameList[i])
// 			{
// 				CView *pView = ChildFrameList[i]->GetActiveView ();
// 				if (pView)
// 				{
// 					CMmiDoc *pDoc = ((CMmiView*)pView)->GetDocument ();
// 					if (pDoc && pDoc->m_pWorkClass)
// 					{
// 						SetGrClassInfo (pDoc->m_pWorkClass);
// 					}
// 				}
// 			}
// 		}
    }
	else
	{
        m_passwordclass->SetTimeCount (0);
        m_passwordclass->SetManIndex (0);
		usrLoginAct->setIcon(QIcon(":app/images/login.png"));
	}
	*/
}
void GeMainWindow::onoffgis()
{
	printf("D:/wscd2000/bin/garcgis.exe \n");
	system("D:/wscd2000/bin/garcgis.exe");
}




AppWindow *GeMainWindow::instanceApp()
{
	
	
//	AppWindow* m_appwin = NULL;

	m_appwin = new AppWindow( this, this, m_winno,0);// Qt::WindowSystemMenuHint);//*/Qt::WA_DeleteOnClose );
	if(!m_appwin) return 0;
//	m_appwin->setDataWindowNum(m_winno);  //m_winno is setted by AppWindow construt funtion
	m_winno ++;
	m_mdiArea->addSubWindow(m_appwin);

	if (m_appwin)
	{
//		connect(m_appwin->m_graphOperInterface, SIGNAL(GraphHot(QString)),
//			this, SLOT(slotOpenGraphHot(QString)));
	}
	


	//delete	
//	m_appwin->loadGraphControl(QString::fromLocal8Bit("gctrl"),QString::fromLocal8Bit("GraphInterfaceInstance"));
	
// 	
// 	//graph kernel engine
// 	m_appwin->loadGraphOperator(QString::fromLocal8Bit("goper"),QString::fromLocal8Bit("GraphOperInstance"));
// 	//graph manipulate engine
// 	m_appwin->loadGraphMan(QString::fromLocal8Bit("gman"),QString::fromLocal8Bit("GraphManInstance"));
	

	

//    installGraphInstance2GraphMan(m_appwin);
//	installGraphInstance2ScadaUI(m_appwin);
    //installGraphInstance2HdOperUI(m_appwin);

	//hcl
	//m_appwin->loadGarcGis(QString::fromLocal8Bit("garcgis"),QString::fromLocal8Bit("GarcGisInstance"));
	//	m_appwin->connectAppWithGraph();

	
	m_appwin->showMaximized();
	//
	m_appwin->setFocus();
	
	return m_appwin;
}


void GeMainWindow::closeEvent ( QCloseEvent* e )
{
	m_mdiArea->closeAllSubWindows();
    if (activeMdiChild()) 
	{
        e->ignore();
    } else 
	{
        e->accept();
    }

}

bool GeMainWindow::loadAppInterface( QString appLibName, QString appInterfaceName )
{
	QLibrary *funcif;
	funcif=new QLibrary(appLibName);
	if(!funcif) return  false;
	
	bool loadflag=funcif->load();
	if(loadflag)
	{
		m_createAppProc = (CreateAppInstanceProc)funcif->resolve( "Create"+appInterfaceName.toLocal8Bit()); 
		m_deleteAppProc = (DeleteAppInstanceProc)funcif->resolve( "Delete"+appInterfaceName.toLocal8Bit()); 
		if( m_createAppProc && m_deleteAppProc ) 
		{ 
			m_appFuncInterface = (UIAppInterface*)m_createAppProc(); 
			if( m_appFuncInterface )      
			{
				m_appFuncInterface->createAppMenuBar(this);
				QToolBar* pToolBar = m_appFuncInterface->createAppToolBar(this);
				
				loadflag=true;
			}
			else
			{
				loadflag=false;
			}
		}
		else
		{
			qDebug("can not find app %s component \n",appLibName.toLocal8Bit().data());
		}
	}
	else
	{
		qDebug("can not find app %s component \n",appLibName.toLocal8Bit().data());
		QMessageBox::warning(0, QObject::tr("Load Library"),
			QObject::tr("Cannot load library%1,erroris %2")
			.arg(appLibName).arg(funcif->errorString()));
		
	}
	return loadflag;
	
}


bool GeMainWindow::loadGraphMan( QString operLibName, QString operInterfaceName )
{
	QLibrary *graphmanif;
	QWidget  *pgraphview=0;
	graphmanif=new QLibrary(operLibName);
	if(!graphmanif) return  false;
	
	bool loadflag=graphmanif->load();
	if(loadflag)
	{
	//	m_createGraphManProc = (CreateAppInstanceProc)graphmanif->resolve( "Create"+operInterfaceName.toLocal8Bit()); 
	//	m_deleteGraphManProc = (DeleteAppInstanceProc)graphmanif->resolve( "Delete"+operInterfaceName.toLocal8Bit()); 
	//	if( m_createGraphManProc && m_deleteGraphManProc ) 
	//	{ 
	//		m_graphManInterface = (UIAppInterface*)m_createGraphManProc(); 
	//		if( m_graphManInterface )      
	//		{
	//			// 				m_graphManInterface->createAppMenuBar((QMainWindow *)m_pMainWnd);
	//			// 				QToolBar* pToolBar = m_graphManInterface->createAppToolBar((QMainWindow *)m_pMainWnd);
 //               
	//			loadflag=true;
	//		}
	//		else
	//		{
	//			loadflag=false;
	//		}
	//	}
	//	else
	//	{
	//		qDebug("can not find GraphMan %s component \n",operLibName.toLocal8Bit().data());
	//	}
	//}
	//else
	//{
	//	qDebug("can not find GraphOperator %s component \n",operLibName.toLocal8Bit().data());
	//	QMessageBox::warning(0, QObject::tr("Load Library"),
	//		QObject::tr("Cannot load library%1,erroris %2")
	//		.arg(operLibName).arg(graphmanif->errorString()));
	//	
	}
	return loadflag;	
	
	
}



void GeMainWindow::slotZoomIn()
{
	//AppWindow* existingAppWin = NULL;
	//existingAppWin = activeMdiChild();

	//if (existingAppWin)
	//{
	//	if (existingAppWin->m_graphOperInterface)
	//	{
	//		existingAppWin->m_graphOperInterface->graphZoomIn();
	//	}
	//}
}

void GeMainWindow::slotZoomOut()
{
	/*AppWindow* existingAppWin = activeMdiChild();

	if (existingAppWin)
	{
		if (existingAppWin->m_graphOperInterface)
		{
			existingAppWin->m_graphOperInterface->graphZoomOut();
		}
	}*/
}

void GeMainWindow::slotZoomFit()
{
	/*AppWindow* existingAppWin = activeMdiChild();

	if (existingAppWin && m_mdiArea)
	{
		QRect rect = m_mdiArea->rect();

		if (existingAppWin->m_graphOperInterface)
		{
			existingAppWin->m_graphOperInterface->graphZoomFit(rect, existingAppWin->m_bzoomFit);
		}
		if (existingAppWin->m_bzoomFit)
		{
			zoomInAct->setEnabled(false);
			zoomOutAct->setEnabled(false);
		}
		else
		{
			zoomInAct->setEnabled(true);
			zoomOutAct->setEnabled(true);
		}
		existingAppWin->m_bzoomFit = !existingAppWin->m_bzoomFit;
	}*/
}

void GeMainWindow::slotOpenGraphHot(QString filename)
{
	/*AppWindow  *existingAppWin;

	if (!filename.isEmpty()) 
	{
		QMdiSubWindow *existing = findMdiChild(filename);
		if (existing) 
		{

			existingAppWin=(AppWindow*)existing->widget();
			if(existingAppWin->m_graphOperInterface)
			{
				existingAppWin->m_graphOperInterface->loadPicFile(filename.toLocal8Bit().data());

			}

			m_mdiArea->setActiveSubWindow(existing);
			return;
		}

		AppWindow *child = createMdiChild();
		if(!child) return ;
		if(!child->m_graphOperInterface) return;
		child->m_graphOperInterface->loadPicFile(filename.toLocal8Bit().data());
		QString canonicalFilePath = QFileInfo(filename).canonicalFilePath();
		child->setWindowTitle(canonicalFilePath);
		child->setFileName(canonicalFilePath);
	}*/
}

void GeMainWindow::slotHand()
{
	//QAction *action = qobject_cast<QAction *>(sender());
	//if (action)
	//{
	//	if(action->isChecked())
	//	{
	//		AppWindow* existingAppWin = activeMdiChild();

	//		if (existingAppWin && m_mdiArea)
	//		{
	//			if (existingAppWin->m_graphOperInterface)
	//			{
	//				m_bHandFlag = true;
	//				existingAppWin->m_graphOperInterface->setHandFlag(m_bHandFlag);
	//			}
	//			m_mdiArea->setCursor(Qt::OpenHandCursor);
	//		}
	//	}
	//	else
	//	{
	//		AppWindow* existingAppWin = activeMdiChild();

	//		if (existingAppWin && m_mdiArea)
	//		{
	//			if (existingAppWin->m_graphOperInterface)
	//			{
	//				m_bHandFlag = false;
	//				existingAppWin->m_graphOperInterface->setHandFlag(m_bHandFlag);
	//			}
	//			m_mdiArea->setCursor(Qt::ArrowCursor);
	//		}
	//	}
	//}
}