#include "qmessageparser.h"

#include <QLibrary>

#include "qlogger.h"

QMessageParser::QMessageParser()
{
}

QMessageParser::~QMessageParser()
{
	m_mapMessage.clear();
}

QMessageParser* QMessageParser::instance()
{
	static QMessageParser* pInstance=new QMessageParser;

	return pInstance;
}

bool QMessageParser::registerMessage(const QIMessagePtr& msg)
{
	if (msg == 0)
	{
		return false;
	}

	if (!m_mapMessage.contains(msg->type()))
	{
		m_mapMessage.insert(msg->type(), msg);
	}

	return true;
}

QIMessagePtr QMessageParser::messageInstance(int type)
{
	return m_mapMessage.value(type, QIMessagePtr(NULL));
}

bool QMessageParser::loadLibrary(const QString& filename, const char* symbol)
{
	CreateMessageInstanceFunc func;

	QLibrary library(filename);

	if (!library.load())
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Load library \"%1\" failed!").arg(filename));
	}

	func=(CreateMessageInstanceFunc)library.resolve(symbol);
	
	if (func != 0)
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QIMessagePtr> instances;
		func(instances);

		foreach(QIMessagePtr pMsg, instances)
		{
			if (pMsg != 0)
			{
				instance()->registerMessage(pMsg);
			}
		}
	}
	else
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" failed! %3").arg(symbol).arg(filename)
			.arg(library.errorString()));
		return false;
	}

	return true;
}
