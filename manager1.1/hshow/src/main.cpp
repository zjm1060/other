#include <QApplication>
#include <QDesktopWidget>

#include "common.h"
#include "qdirex.h"

#include "qloglog.h"
#include "qconsoleappender.h"
#include "qlogger.h"
#include "qfileappender.h"

#include "qtranslateinstaller.h"

#include "qhshow.h"
#include "qglobalinfo.h"
#include "qdeafactory.h"
#include "qmessageparser.h"
#include "qipcsocket.h"

#define LOG_FILE_PATH	"/his/hman/hshow"

static void usage(char *cmdname)
{
	fprintf(stderr,"usage: %s [-d debug] [-h help]\n", cmdname);
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

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
	langs<<"qt"<<"hshow";
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

	QDesktopWidget* pDesktop=app.desktop();
	if (pDesktop == NULL)
	{
		return 1;
	}

	//加载数据加密模块
	logplusInfo(HSHOW_LOGGER_STR, QObject::tr("Loading data encryption module..."));
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QDEAFactory::instance()->loadLibrary("libdead1", "CreateDEAInstance"))
#else
	if (QDEAFactory::instance()->loadLibrary("libdea1", "CreateDEAInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QDEAFactory::instance()->loadLibrary("libdead", "CreateDEAInstance"))
#else
	if (QDEAFactory::instance()->loadLibrary("libdea", "CreateDEAInstance"))
#endif
#endif
	{
		logplusInfo(HSHOW_LOGGER_STR, QObject::tr("Loaded data encryption module successfully."));
	}
	else
	{
		logplusInfo(HSHOW_LOGGER_STR, QObject::tr("Data encryption module loading failed!"));
	}

	//加载消息模块
	logplusInfo(HSHOW_LOGGER_STR, QObject::tr("Loading message module..."));
#ifdef	Q_OS_WIN
#ifdef QT_DEBUG
	if (QMessageParser::instance()->loadLibrary("libmsgd1", "CreateMessageInstance"))
#else
	if (QMessageParser::instance()->loadLibrary("libmsg1", "CreateMessageInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QMessageParser::instance()->loadLibrary("libmsgd", "CreateMessageInstance"))
#else
	if (QMessageParser::instance()->loadLibrary("libmsg", "CreateMessageInstance"))
#endif
#endif
	{
		logplusInfo(HSHOW_LOGGER_STR, QObject::tr("Loaded message module successfully."));
	}
	else
	{
		logplusInfo(HSHOW_LOGGER_STR, QObject::tr("Message module loading failed!"));
	}

	QIpcSocket::instance()->connectToHost();

	int nNumScreen=pDesktop->numScreens();
	for (int i=0; i<nNumScreen; i++)
	{
		QHShow::createWidget(pDesktop->screenGeometry(i));
	}

	return app.exec();
}
