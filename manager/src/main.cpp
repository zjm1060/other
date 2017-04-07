#include <QApplication>
#include <QFont>
#include <QDir>
#include <QTranslator>
//#include <QTextCodec>

#include "common.h"
#include "extern.h"
#include "qmainwnd.h"
#include "qnontopomainwnd.h"

#include "qloglog.h"
#include "qconsoleappender.h"
#include "qlogger.h"
#include "qfileappender.h"
#include "qndc.h"

#define LOG_FILE_PATH	"/his/manager"
#define LANG_FILE_PATH	"/ini"

int main(int argc, char* argv[])
{
	registerWmi();
	registerMessage();

	QApplication app(argc, argv);

	QString strLang=parseConfig(getAppParentDir()+INI_FILE, "main/lang", "zh").toString();
	if (strLang == "zh")
	{
		QTranslator* translator=new QTranslator;
		translator->load(getAppParentDir()+LANG_FILE_PATH+"/manager_zh.qm");
		app.installTranslator(translator);
	}

	QLogLog::instance()->setStatus(QLogLog::InternalDebugging, true);

	QString strLogPath=getAppParentDir()+LOG_FILE_PATH;
	QDir dir;
	dir.mkpath(strLogPath);

	QSharedAppenderPtr consoleAppender(new QConsoleAppender());
	consoleAppender->setName("console");
	consoleAppender->setLayout( std::auto_ptr<QLogLayout>(new QSimpleLogLayout()) );
	QLogger::root().addAppender(consoleAppender);

	int nLogDays=parseConfig(getAppParentDir()+INI_FILE, "main/logDays", 10).toInt();
	if (nLogDays <= 0)
	{
		nLogDays=1;
	}

	QSharedAppenderPtr fileAppender(new QDailyRollingFileAppender(strLogPath+"/.log", QDailyRollingFileAppender::DAILY, true, nLogDays));
	fileAppender->setName("file");
	fileAppender->setLayout( std::auto_ptr<QLogLayout>(new QTTCCLogLayout()) );
	QLogger::root().addAppender(fileAppender);

	LOGPLUS_INFO_STR(QLogger::root(), getHostName()+"."+getUserName()+" entering...");

	QNDC::instance()->push(getHostName()+"."+getUserName());

// 	QTextCodec* codec=QTextCodec::codecForName("Latin1");
// 	QTextCodec::setCodecForCStrings(codec);
// 	QTextCodec::setCodecForTr(codec);
//	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

#ifndef Q_OS_WIN
	
	int nFontSize=parseConfig(getAppParentDir()+INI_FILE, "main/fontsize", 10).toInt();
	if (nFontSize <= 0)
	{
		nFontSize=10;
	}

	QFont font=QApplication::font();
	font.setPointSize(nFontSize);
	font.setBold(false);
	QApplication::setFont(font);
#endif

	bool bSuperUser=parseConfig(getAppParentDir()+INI_FILE, "topology/enabled", true).toBool();

#ifndef	Q_OS_WIN
	if (getuid() != 0)
	{
		bSuperUser=false;
	}
#endif

	if (bSuperUser)
	{
		QMainWnd* mainWnd=new QMainWnd;
		mainWnd->show();
	}
	else
	{
		QNonTopoMainWnd* mainWnd=new QNonTopoMainWnd;
		mainWnd->show();
	}

	return app.exec();
}
