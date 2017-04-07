#include "qmainwnd.h"

#include <QResizeEvent>
#include <QPixmapCache>

#include "common.h"
#include "qinfomanager.h"
#include "qmessageparser.h"
#include "qfilecomparefactory.h"
#include "qprocesssocket.h"
#include "qlogger.h"
#include "qdeafactory.h"

QMainWnd::QMainWnd()
	: QMainWindow()
{
	setWindowTitle(tr("Manager")+ MANAGER_VERSION_STR);
	setWindowIcon(QIcon(":/images/manager.png"));

	m_pView=new QViewController(this);

	QProcessSocket::instance()->setView(m_pView);

	m_pWndGraphics=new QWndGraphics(this);
	setCentralWidget(m_pWndGraphics);

	//加载数据加密模块
	QInfoManager::instance()->append(QInfoManager::Information, tr("Loading data encryption module..."));
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
		QInfoManager::instance()->append(QInfoManager::Information, tr("Loaded data encryption module successfully."));
	}
	else
	{
		QInfoManager::instance()->append(QInfoManager::Error, tr("Data encryption module loading failed!"));
	}

	//加载消息模块
	QInfoManager::instance()->append(QInfoManager::Information, tr("Loading message module..."));
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
		QInfoManager::instance()->append(QInfoManager::Information, tr("Loaded message module successfully."));
	}
	else
	{
		QInfoManager::instance()->append(QInfoManager::Error, tr("Message module loading failed!"));
	}

	//加载文件比较模块
	QInfoManager::instance()->append(QInfoManager::Information, tr("Loading file comparison module..."));
#ifdef	Q_OS_WIN
#ifdef QT_DEBUG
	if (QFileCompareFactory::instance()->loadLibrary("libfcd1", "CreateFileCompareInstance"))
#else
	if (QFileCompareFactory::instance()->loadLibrary("libfc1", "CreateFileCompareInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QFileCompareFactory::instance()->loadLibrary("libfcd", "CreateFileCompareInstance"))
#else
	if (QFileCompareFactory::instance()->loadLibrary("libfc", "CreateFileCompareInstance"))
#endif
#endif
	{
		QInfoManager::instance()->append(QInfoManager::Information, tr("Loaded file comparison module successfully."));
	}
	else
	{
		QInfoManager::instance()->append(QInfoManager::Warning, tr("File comparison module loading failed!"));
	}

	QProcessSocket::instance()->connectToHost();

	showMaximized();
}

QMainWnd::~QMainWnd()
{
}

void QMainWnd::closeEvent(QCloseEvent* event)
{
	QProcessSocket::instance()->close();

	if (m_pWndGraphics != NULL)
	{
		m_pWndGraphics->closeEvent();
	}

	event->accept();
}

