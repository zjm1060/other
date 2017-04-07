#include <QApplication>
#include <QFont>

#include "common.h"
#include "qdirex.h"

#include "qloglog.h"
#include "qconsoleappender.h"
#include "qlogger.h"
#include "qfileappender.h"

#include "qtranslateinstaller.h"
#include "qmainwnd.h"
#include "qglobalinfo.h"

#define LOG_FILE_PATH	"/his/hman/manager"

static void usage(char *cmdname)
{
	fprintf(stderr,"usage: %s [-d debug] [-h help]\n", cmdname);
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

//	Q_INIT_RESOURCE(mgr);

	int c;
	while ((c = parseOption(argc, argv,"dh?")) != -1)
	{
		switch ((char)c)
		{
		case 'd':
			QGlobalInfo::instance()->setRunningFlag(QGlobalInfo::EnableDebugFlag, true);
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
	langs<<"qt"<<"manager"<<"libfc"<<"libutil"<<"libctl";
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
	properties.setProperty("File", strLogPath+"/.log");
	properties.setProperty("MaxBackupIndex", nLogDays);

	QAppenderPtr fileAppender(new QDailyRollingFileAppender(properties));
	fileAppender->setName("file");
	QLogger::root()->addAppender(fileAppender);

	QMainWnd mainWnd;
	mainWnd.show();

	return app.exec();
}
