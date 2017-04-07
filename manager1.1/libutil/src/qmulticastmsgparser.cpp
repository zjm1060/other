#include "qmulticastmsgparser.h"

#include <QLibrary>

#include "qlogger.h"

QMulticastMsgParser::QMulticastMsgParser()
{
}

QMulticastMsgParser::~QMulticastMsgParser()
{
	m_mapMessage.clear();
}

QMulticastMsgParser* QMulticastMsgParser::instance()
{
	static QMulticastMsgParser* pInstance=new QMulticastMsgParser;

	return pInstance;
}

bool QMulticastMsgParser::registerMessage(const QIMulticastMsgPtr& msg)
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

QIMulticastMsgPtr QMulticastMsgParser::messageInstance(int type)
{
	return m_mapMessage.value(type, QIMulticastMsgPtr(NULL));
}

bool QMulticastMsgParser::loadLibrary(const QString& filename, const char* symbol)
{
	CreateMulticastMsgInstanceFunc func;

	QLibrary library(filename);

	if (!library.load())
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Load library \"%1\" failed!").arg(filename));
	}

	func=(CreateMulticastMsgInstanceFunc)library.resolve(symbol);
	
	if (func != 0)
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QIMulticastMsgPtr> instances;
		func(instances);

		foreach(QIMulticastMsgPtr pMsg, instances)
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
