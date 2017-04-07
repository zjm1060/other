#include "qmessageparser.h"

#include "define.h"

//#include <QLibrary>

QMutex QMessageParser::m_mutex;
QMessageParser* QMessageParser::m_pInstance=0;

//typedef void (*CreateInstanceFunc)(QList<QIMessage*>& listMessage);

QMessageParser::QMessageParser()
{

}

QMessageParser::~QMessageParser()
{
	foreach(QIMessage* pMessage, m_mapMessage)
	{
		SAFE_DELETE_CLASS(pMessage);
	}

	m_mapMessage.clear();

	m_pInstance=NULL;
}

QMessageParser* QMessageParser::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QMessageParser;

// 			CreateInstanceFunc func;
// 
// #ifdef DEBUG
// 			func=(CreateInstanceFunc)QLibrary::resolve("parserd1", "CreateInstance");
// #else
// 			func=(CreateInstanceFunc)QLibrary::resolve("parser1", "CreateInstance");
// #endif
// 			
// 			if (func != 0)
// 			{
// #ifdef DEBUG
// 				printf("Succeed: Resolve \"CreateInstance\" in library \"parser\"\n");
// #endif
// 				QList<QIMessage*> listMessage;
// 				func(listMessage);
// 
// 				foreach(QIMessage* pMsg, listMessage)
// 				{
// 					if (pMsg != 0)
// 					{
// 						m_pInstance->messageRegister(pMsg);
// 					}
// 				}
// 			}
// #ifdef DEBUG
// 			else
// 			{
// 				printf("Failed: Resolve \"CreateInstance\" in library \"parser\"\n");
// 			}
// #endif
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

bool QMessageParser::messageRegister(QIMessage* msg)
{
	if (msg == 0)
	{
		return false;
	}

	m_mapMessage.insert(msg->type(), msg);

	return true;
}

QIMessage* QMessageParser::messageInstance(int type)
{
	return m_mapMessage.value(type, 0);
}
