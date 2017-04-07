#include "qloglayoutfactory.h"

QLogLayout::QLogLayout()
:m_levelManager(QLogLevelManager::instance())
{

}

QLogLayout::QLogLayout(const QProperties&)
:m_levelManager(QLogLevelManager::instance())
{

}

QLogLayout::~QLogLayout()
{

}

QLogLayoutFactoryBase::QLogLayoutFactoryBase()
{

}

QLogLayoutFactoryBase::~QLogLayoutFactoryBase()
{

}

QSharedObjectPtr<QLogLayoutFactoryRegistry > qLogLayoutFactoryRegistryInstance()
{
	return QLogLayoutFactoryRegistry::instance();
}
