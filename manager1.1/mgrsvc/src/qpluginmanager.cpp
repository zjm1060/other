#include "qpluginmanager.h"

#include <QCoreApplication>
#include <QProcess>

#include "qlogger.h"

QPluginManager::QPluginManager(QObject *parent)
	: QObject(parent)
{

}

QPluginManager::~QPluginManager()
{

}

QPluginManager* QPluginManager::instance()
{
	static QPluginManager* pInstance=new QPluginManager;

	return pInstance;
}

void QPluginManager::activate(const QString& module)
{
	QString strProgram=QCoreApplication::applicationFilePath()+" -m "+module; 
	if (!QProcess::startDetached(strProgram))
	{
		logplusError(MGRSVC_LOGGER_STR, QString("Run module %1 failed! Quit...").arg(module));

		exit(0);
	}
}
