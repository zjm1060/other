#if defined(Q_WS_WIN)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include <QtGlobal>
#include <QApplication>
#include <QtGui>
#include <QTranslator>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include "SimuTrainee.h"
class  QSystemSemaphore;
class QSharedMemory;
QSystemSemaphore sema("OTS_SIMUTRAINEE",1,QSystemSemaphore::Open); 
QSharedMemory mem("SimuTrainee");// 全局对象名 
int InitHDB()
{	
	// 确保只运行一次 	
	if (!mem.create(1))// 如果全局对象以存在则退出 
	{ 
		QMessageBox().information(NULL,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("程序已运行"),QMessageBox::Ok);		
		sema.release();// 如果是 Unix 系统，会自动释放。 
		return FALSE;
	} 
	if(MountDB(DB_NAME) == -1)
	{
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("call MountDB(DATABASE_NAME) error!"),QMessageBox::Ok);		
		return FALSE;
	}
	if(lan_init_all() == (int)FALSE)
	{
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("call Lan_init_all() error!"),QMessageBox::Ok);		
		return FALSE;
	}
	if(get_ots_shm()==(int)FALSE||get_hdb_shm()==(int)FALSE)
	{
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("读取共享数据区失败!"),QMessageBox::Ok);		
		return FALSE;
	}
	else
	{
		shm_hdb_addr->data_revert.speed = 8;//以后要改吧...0728????????
	}
	ReadOtsDefFile();
	db_conn.setDatabaseName("SimuTrainee");
	if(ConnectHDB(db_conn)==(int)FALSE)
	{
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("连接历史数据库失败!"),QMessageBox::Ok);		
		return FALSE;
	}	

	if( InitOTSHdb()!=(int)TRUE )
	{
		QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("InitOTSHdb失败!"),QMessageBox::Ok);		
		return FALSE;
	} 
	return TRUE;
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(SimuTrainee);

	QApplication app(argc, argv);
	QString translatorFileName = QLatin1String("qt_");
	translatorFileName += QLocale::system().name();
	QTranslator *translator;
	translator= new QTranslator(&app);
	//if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
	if (translator->load(translatorFileName,"./plugins"))
		app.installTranslator(translator);

	sema.acquire();// 在临界区操作共享内存   SharedMemory
	//连接历史数据库
	int nRet=InitHDB();
	sema.release();// 临界区
	if (!nRet)
	{ 	 
		QMessageBox::information( NULL,QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("SIMUTRAINEE启动失败,请检查配置,确认H9000系统已启动"),
			QMessageBox::Ok);
		exit(0);
	}
 	SimuTrainee window;	
	//window.setAttribute(Qt::WA_QuitOnClose,true);
	//window.setAttribute(Qt::WA_DeleteOnClose,true);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	QApplication::addLibraryPath("./plugins");
	window.show();
	return app.exec();
	UnMountDB ();
}