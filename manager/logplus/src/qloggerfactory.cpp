#include "qloggerfactory.h"

#include "qlogger.h"
#include "qhierarchy.h"
#include "qloggerimplement.h"

QDefaultLoggerFactory::QDefaultLoggerFactory()
{

}

QDefaultLoggerFactory::~QDefaultLoggerFactory()
{

}

QLogger QDefaultLoggerFactory::makeNewLoggerInstance(const QString& name, QHierarchy& hierarchy)
{
	return QLogger(new QLoggerImplement(name, hierarchy));
}
