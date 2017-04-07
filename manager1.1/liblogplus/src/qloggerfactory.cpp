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

QLoggerPtr QDefaultLoggerFactory::makeNewLoggerInstance(const QString& name, const QHierarchyPtr& hierarchy)
{
	QLoggerImplementPtr impl(new QLoggerImplement(name, hierarchy));
	return QLoggerPtr(new QLogger(impl));
}
