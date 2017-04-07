#include "qlogextern.h"

void registerLogLayout()
{
	REGISTER_PRODUCT(QLogLayoutFactoryRegistry::instance(), QLayoutFactory, QSimpleLogLayoutFactory, QSimpleLogLayout);
	REGISTER_PRODUCT(QLogLayoutFactoryRegistry::instance(), QLayoutFactory, QTTCCLogLayoutFactory, QTTCCLogLayout);
	REGISTER_PRODUCT(QLogLayoutFactoryRegistry::instance(), QLayoutFactory, QPatternLogLayoutFactory, QPatternLogLayout);
}

void registerFilter()
{
	REGISTER_PRODUCT(QFilterFactoryRegistry::instance(), QFilterFactory, QDenyAllFilterFactory, QDenyAllFilter);
	REGISTER_PRODUCT(QFilterFactoryRegistry::instance(), QFilterFactory, QLogLevelMatchFilterFactory, QLogLevelMatchFilter);
	REGISTER_PRODUCT(QFilterFactoryRegistry::instance(), QFilterFactory, QLogLevelRangeFilterFactory, QLogLevelRangeFilter);
	REGISTER_PRODUCT(QFilterFactoryRegistry::instance(), QFilterFactory, QStringMatchFilterFactory, QStringMatchFilter);
}

void registerLog()
{
	registerLogLayout();
	registerFilter();
}