#ifndef QLOGEXTERN_H
#define QLOGEXTERN_H

#include "global.h"
#include "qfactory.h"
#include "qloglayout.h"
#include "qpatternloglayout.h"
#include "qfilter.h"

class LOGPLUS_EXPORT QLayoutFactory : public QFactoryBase
{
public:
	typedef QLogLayout QProductType;
	typedef std::auto_ptr<QLogLayout> QProductPtr;

public:
	QLayoutFactory(){}
	virtual ~QLayoutFactory(){}

	/**
	* Create a "Layout" object.
	*/
	virtual QProductPtr createObject(const QProperties& properties) = 0;
};

typedef QTemplateFactory<QSimpleLogLayout, QLayoutFactory>			QSimpleLogLayoutFactory;
typedef QTemplateFactory<QTTCCLogLayout, QLayoutFactory>			QTTCCLogLayoutFactory;
typedef QTemplateFactory<QPatternLogLayout, QLayoutFactory>			QPatternLogLayoutFactory;

typedef QTemplateFactoryRegistry<QLayoutFactory>	QLogLayoutFactoryRegistry;

LOGPLUS_EXPORT void registerLogLayout();

class LOGPLUS_EXPORT QFilterFactory : public QFactoryBase
{
public:
	typedef QFilter QProductType;
	typedef std::auto_ptr<QFilter> QProductPtr;

public:
	QFilterFactory(){}
	virtual ~QFilterFactory(){}

	virtual QProductPtr createObject(const QProperties& properties) = 0;
};

typedef QTemplateFactory<QDenyAllFilter, QFilterFactory>			QDenyAllFilterFactory;
typedef QTemplateFactory<QLogLevelMatchFilter, QFilterFactory>		QLogLevelMatchFilterFactory;
typedef QTemplateFactory<QLogLevelRangeFilter, QFilterFactory>		QLogLevelRangeFilterFactory;
typedef QTemplateFactory<QStringMatchFilter, QFilterFactory>		QStringMatchFilterFactory;

typedef QTemplateFactoryRegistry<QFilterFactory>	QFilterFactoryRegistry;

LOGPLUS_EXPORT void registerFilter();

LOGPLUS_EXPORT void registerLog();

#endif // EXTERN_H

