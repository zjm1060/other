#if defined(Q_WS_WIN)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include <QApplication>
#include <QtGlobal>
#include <QtGui>
#include "SimuTutor.h"
#include <QSystemSemaphore>
#include <QSharedMemory>

class  QSystemSemaphore;
class QSharedMemory;
QSystemSemaphore sema("OTS_TEACHER",1,QSystemSemaphore::Open); 
QSharedMemory mem("TEACHER_MANAGER");

int check_service()
{
	QDir Path;
	QString appPath = qApp->applicationDirPath();
	QString curDrive = appPath.mid(0,2);//得到当前程序的盘符 例如“C:”
	Path.setPath(curDrive+QString("\\home\\ems\\h9000\\bin"));
	QString strFile = Path.absoluteFilePath("SimuService.exe");
	QProcess* pProcess=new QProcess;
	pProcess->startDetached(strFile);
	return 1;
}
int initDB()
{
	ReadOtsDefFile();
	if(ConnectHDB(db_conn)!=(int)TRUE)
	{
		printf("ConnectHDB error!");
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("不能连接到历史数据库服务器!"),QMessageBox::Ok);
		return 0;
	}
	if( InitOTSHdb()!=TRUE )
	{
		printf("InitOTSHdb error!");
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("初始化数据库失败!"),QMessageBox::Ok);
		return 0;
	}
	if( MountDB(DB_NAME)==-1 )
	{
		printf("MountDB error!");
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("MountDB错误!"),QMessageBox::Ok);
		return 0;
	}	
	if(lan_init_all() == (int)FALSE)
	{
		printf("==== client_service: call Lan_init_all() error\n");	
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("call Lan_init_all() error!"),QMessageBox::Ok);		
		return 0;
	}
	if(get_ots_shm()==(int)FALSE||get_hdb_shm()==(int)FALSE)
	{
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("读取共享数据区失败!"),QMessageBox::Ok);		
		return 0;
	}
	//CloseProcess(QString::fromLatin1("SimuService"));
	if (!mem.create(1))
	{ 
		QMessageBox().information(NULL,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("程序已运行"),QMessageBox::Ok);		
		sema.release();
		exit(0);
	} 
	else
	{
		return 1;		
	}
}
int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(TeacherManage);
	QApplication app(argc, argv);
	QString translatorFileName = QLatin1String("qt_");
	translatorFileName += QLocale::system().name();
	QTranslator *translator;
	translator= new QTranslator(&app);
	//if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
	if (translator->load(translatorFileName,"./plugins"))
		app.installTranslator(translator);

	sema.acquire();
	int nRet=initDB();
	sema.release();
	if (!nRet)
	{ 	 
		QMessageBox::information( NULL,QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("连接数据库失败，请检查配置,确认H9000系统已启动"),
			QMessageBox::Ok);
		exit(0);
	}
// 	if (!jujgeIstUTorMachine())
// 	{
// 		QMessageBox::information( NULL,QString::fromLocal8Bit("提示"),
// 			QString::fromLocal8Bit("当前主机没有运行教员站的权限，程序将退出"),
// 			QMessageBox::Ok);
// 		exit(0);
// 	}
	if (check_service()!=(int)TRUE)
	{
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("Simuservice未启动!"),QMessageBox::Ok);		
		exit(0);
	}
	CTeacherManage mainwin;
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	QApplication::addLibraryPath("./plugins");
	mainwin.showMaximized();
	//QRect desktoprect = QApplication::desktop()->availableGeometry();
	//mainwin.setFixedSize(desktoprect.width(),desktoprect.height());
	//mainwin.move(0,0);
	//mainwin.show();
	return app.exec();
}
