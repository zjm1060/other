#include <QtCore/QCoreApplication>
#include "SimuService.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	/*if(-1==InitSysLog(H9000_MODULE_SIMUSERVICE, "SimuService"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}*/
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== SimuService.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}

	if(lan_init_all() == FALSE)
	{
		printf("==== SimuService: call Lan_init_all() error\n");		
		exit(0);
	}

	if(-1==InterLockFileLoad())
	{
		printf("Fail to Load InterLock File!\n");
		exit(0);
	}

	if(get_rec_shm() == FALSE)
	{
		printf("==== SimuService: call Lan_init_all() error\n");		
		exit(0);
	}

	SimuService serv;
	serv.Run();
	return a.exec();
}
