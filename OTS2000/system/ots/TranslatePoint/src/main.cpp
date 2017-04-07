#if defined(Q_WS_WIN)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include <QApplication>
#include <QtGui>
#include "inc/TranslatePoint.h"


int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	CTranslatePoint mainwin;
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	QApplication::addLibraryPath("./plugins");
	if( MountDB(DB_NAME)==-1 )
	{
		printf("MountDB error!");
		QMessageBox().information(NULL,QString::fromLocal8Bit("´íÎó!"),QString::fromLocal8Bit("MountDB´íÎó!"),QMessageBox::Ok);
		exit(0);
	}	
	//mainwin.show();
	//QRect desktoprect = QApplication::desktop()->availableGeometry();
	//mainwin.setFixedSize(desktoprect.width(),desktoprect.height());
	//mainwin.move(0,0);
	//mainwin.show();
	return app.exec();
}
