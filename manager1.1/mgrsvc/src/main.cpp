#include <QCoreApplication>

#include "global.h"
#include "common.h"
#include "qdirex.h"

#include "qloglog.h"
#include "qconsoleappender.h"
#include "qlogger.h"
#include "qfileappender.h"
#include "qndc.h"

#define LOG_FILE_PATH	"/his/hman/mgrsvc"

#ifndef Q_OS_WIN
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h> 
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h> 
#include	<sys/mman.h>
#include	<sys/time.h>
#include 	<sys/signal.h>
#include 	<unistd.h>
#include 	<time.h>
#include 	<utime.h>
#include 	<strings.h>
#include <errno.h>
#endif

#include "qmgrsrc.h"
#include "qmibsrc.h"
#include "qtranslateinstaller.h"
#include "qglobalinfo.h"

static void usage(char *cmdname)
{
	fprintf(stderr,"usage: %s [-d debug] [-t remove topology discovery] [-h help]\n", cmdname);
}

static void createLockFile(char* filename)
{
#ifdef Q_OS_WIN
	HANDLE	hFile;

	QString strFileName=appendPath(getAppDir(), filename);

	hFile= CreateFile(strFileName.toStdWString().data(), GENERIC_READ|GENERIC_WRITE, 
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		printf("The task has been running! Quit...\n");
		exit(0);
	}
#else
	int		fd, val;
	struct	flock lock;
	char	buf[16];
	
	QString strFileName=appendPath(getAppDir(), filename);

	if( (fd=open(strFileName.toLocal8Bit().data(), O_WRONLY|O_CREAT, 0644)) == -1 )
	{
		printf("Failed to open lock file!\n");
		exit(-1);
	}

	lock.l_type= F_WRLCK;
	lock.l_start= 0;
	lock.l_whence= SEEK_SET;
	lock.l_len= 0;
	if( fcntl(fd, F_SETLK, &lock) < 0 )
	{
		if(errno==EACCES || errno==EAGAIN)	
		{
			printf("The task has been running! Quit...\n");

			exit(0);
		}
		else
		{

			printf("Fail to write_lock!\n");

			exit(-1);
		}
	}

	if( ftruncate(fd, 0) < 0 )
	{
		printf("Fail to truncate lock file!\n");

		exit(-1);
	}

	sprintf(buf, "%d\n", getpid());
	if( write(fd, buf, strlen(buf)) != strlen(buf) )
	{
		printf("Fail to write pid to lock file!\n");

		exit(-1);
	}

	val= fcntl(fd, F_GETFD, 0);
	if(val<0)
	{
		printf("Fail to fcntl F_GETFD to lock file!\n");

		exit(-1);
	}

	val |= FD_CLOEXEC;
	if(fcntl(fd, F_SETFD, val)<0)
	{
		printf("Fail to fcntl F_SETFD to lock file!\n");

		exit(-1);
	}
#endif
}

int main(int argc, char* argv[])
{
#ifndef Q_OS_WIN
	setuid(0);
#endif

	QCoreApplication app(argc, argv);

	bool bModule=false;
	QString strModule;

	int c;
	while ((c = parseOption(argc, argv,"dtmh?")) != -1)
	{
		switch ((char)c)
		{
		case 'd':
			QGlobalInfo::instance()->setRunningFlag(QGlobalInfo::EnableDebugFlag, true);
			break;
		case 't':
			QGlobalInfo::instance()->setRunningFlag(QGlobalInfo::EnableTopologyFlag, false);
			break;
		case 'm':
			{
				int nIndex=parseOption(0, NULL, NULL);
				strModule=argv[nIndex];
				if (!strModule.isEmpty())
				{
					bModule=true;
				}
			}
			break;
		case 'h':
		case '?':
			usage(argv[0]);
			exit(1);
			break;
		default:
			usage(argv[0]);
			exit(1);
			break;
		}
	}

	QStringList langs;
	langs<<"qt"<<"manager";
	QTranslateInstaller::instance()->setTranslators(langs);
	QTranslateInstaller::instance()->install();

#ifdef QT_DEBUG
	QLogLog::instance()->setStatus(QLogLog::InternalDebugging, true);
#endif

	QString strLogPath=getAppParentDir()+LOG_FILE_PATH;
	QDirEx dir;
	dir.mkpath(strLogPath);

	if (!QGlobalInfo::instance()->runningFlag(QGlobalInfo::EnableDebugFlag))
	{
		QLogger::root()->setLogLevel(INFO_LOG_LEVEL);
	}

	QAppenderPtr consoleAppender(new QConsoleAppender());
	consoleAppender->setName("console");
	consoleAppender->setLayout(qLogLayoutFactoryRegistryInstance()->get("QSimpleLogLayout")->createObject(QProperties()));
	QLogger::root()->addAppender(consoleAppender);

	int nLogDays=parseConfig(getAppParentDir()+INI_FILE, "main/logDays", 30).toInt();
	if (nLogDays <= 0)
	{
		nLogDays=1;
	}

	QProperties properties;

	properties.setProperty("LogToStdErr", "false");
	properties.setProperty("ImmediateFlush", "true");

	properties.setProperty("Layout", "QPatternLogLayout");
	properties.setProperty("Layout.NdcMaxDepth", 5);

#ifdef QT_DEBUG
	properties.setProperty("Layout.ConversionPattern", "%d [%i-%t] %p %c <%x> %F:%L - %m%n");
#else
	properties.setProperty("Layout.ConversionPattern", "%d [%i-%t] %p %c <%x> - %m%n");
#endif

	properties.setProperty("Schedule", "DAILY");

	if (!bModule)
	{
		properties.setProperty("File", strLogPath+"/.log");
	}
	else
	{
		properties.setProperty("File", strLogPath+"/"+strModule+".log");
	}

	properties.setProperty("MaxBackupIndex", nLogDays);

	QAppenderPtr fileAppender(new QDailyRollingFileAppender(properties));
	fileAppender->setName("file");
	QLogger::root()->addAppender(fileAppender);

	logplusInfo(MGRSVC_LOGGER_STR, QString("mgrsrc is running in %1@%2").arg(getUserName(), getHostName()));

	QNdc::instance()->push(getUserName()+"@"+getHostName());

	if (!bModule)
	{
		createLockFile(".mgrsvc.lock");

		bool bSuperUser=parseConfig(getAppParentDir()+INI_FILE, "topology/enabled", true).toBool();

#ifndef	Q_OS_WIN
		if (!bSuperUser && getuid() != 0)
		{
			return 0;
		}
#endif

		QMgrSrc* mgrsrc=new QMgrSrc(bSuperUser);
	}
	else
	{
		if (strModule == "hman.mib")
		{
			QMibSrc* mibsrc=new QMibSrc;
		}
	}

	return app.exec();
}
