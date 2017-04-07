#include <QApplication>
#include <QtGui>
#include <QUdpSocket>

#if defined(WIN32)
// #include <QAxFactory>
#endif
#define METUX_PORT 888888

#include "gemainwindow.h"

GeMainWindow *g_pmainWin;


int main(int argc, char *argv[])
{
 //   Q_INIT_RESOURCE(cfgmgr);
	QTranslator translatorcfgmgr;	
	QTranslator translatorcheckui;	
	

    QApplication app(argc, argv);

	char cfgmgrtransfile[80];
	char checkuitransfile[80];
	


	QUdpSocket *pmutexsock= new QUdpSocket() ;
	bool retsockval=pmutexsock->bind(METUX_PORT);
   if(retsockval != true)
   {
	   QMessageBox::warning(0,QObject::tr("cfgmgr"),QObject::tr("can't start cfgmgr mutil instance"));
	   exit(0);
   }

	char *prun_home=NULL;//getenv("H9000_HOME");
	if(prun_home)
	{
		strcpy(cfgmgrtransfile,prun_home);
		strcat(cfgmgrtransfile,"/home/ems/h9000/cfgmgr/i18n/cfgmgr_zh");
		
		strcpy(checkuitransfile,prun_home);
		strcat(checkuitransfile,"/home/ems/h9000/cfgmgr/i18n/checkui_zh");

	}
	else
	{
		strcpy(cfgmgrtransfile,"/home/ems/h9000/cfgmgr/i18n/cfgmgr_zh");
		strcpy(checkuitransfile,"/home/ems/h9000/cfgmgr/i18n/checkui_zh");
		
	}

	translatorcfgmgr.load(QString::fromLocal8Bit(cfgmgrtransfile));
	translatorcheckui.load(QString::fromLocal8Bit(checkuitransfile));


	app.installTranslator(&translatorcfgmgr);
	app.installTranslator(&translatorcheckui);


	

	char splashfile[80];
	if(prun_home)
	{
		
		strcpy(splashfile,prun_home);
		strcat(splashfile,"/home/ems/h9000/cfgmgr/images/cfgmgr.png");
	}
	else
	{
		strcpy(splashfile,"/home/ems/h9000/cfgmgr/images/cfgmgr.png");
	}
    QPixmap  splashpixfile(splashfile);
	QSplashScreen *gexploresplash = new QSplashScreen( splashpixfile );
    gexploresplash->setMask( splashpixfile.mask() );
 //   gexploresplash->show();
	gexploresplash->showMessage("Loaded modules");
    app.processEvents();

	g_pmainWin=/*new GeMainWindow(0);//*/new GeMainWindow(gexploresplash);
    g_pmainWin->showMaximized();
//#if defined(WIN32)
//	parent.set_main_qwidget(g_pmainWin);
//#elif defined(__unix) 
//		parent.set_main_wnd(getpid());
//#endif

#if defined(__unix)
signal(SIGUSR1,SIG_IGN);
#endif   

	gexploresplash->finish( g_pmainWin );

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

    int ret= app.exec();


	delete gexploresplash;



	return ret;
}
