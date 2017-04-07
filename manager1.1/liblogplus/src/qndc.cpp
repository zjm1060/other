#include "qndc.h"

DiagnosticContext::DiagnosticContext(const QString& message, DiagnosticContext* parent)
: message(message),
fullMessage( (  (parent == NULL) ? message : parent->fullMessage + " " + message) )
{
}


DiagnosticContext::DiagnosticContext(const QString& message)
: message(message),
fullMessage(message)
{
}

QMutex	QNdc::m_mutex;
QNdcPtr QNdc::m_pInstance;
QThreadStorage<DiagnosticContextStack*>	QNdc::g_threadStorage;

QNdc::QNdc()
{
}

QNdc::~QNdc()
{
}

QNdcPtr QNdc::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=QNdcPtr(new QNdc);
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void QNdc::clear()
{
	try 
	{
		DiagnosticContextStack* ptr = stack();
		if(ptr != NULL) 
		{
			g_threadStorage.setLocalData(NULL);
		}
	}
	catch(std::exception& e) 
	{
		loglog()->error("QNdc::%s- exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...) 
	{
		loglog()->error("QNdc::%s- exception occurred", __FUNCTION__);
	}
}

DiagnosticContextStack* QNdc::stack()
{
	if (g_threadStorage.hasLocalData())
	{
		return g_threadStorage.localData();
	}
	else
	{
		return NULL;
	}
}

DiagnosticContextStack QNdc::cloneStack()
{
	try 
	{
		DiagnosticContextStack* ptr = stack();
		if(ptr != NULL)
		{
			return DiagnosticContextStack(*ptr);
		}
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}

	return DiagnosticContextStack();
}

void QNdc::inherit(const DiagnosticContextStack& s)
{
	try
	{
		g_threadStorage.setLocalData(new DiagnosticContextStack(s));
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}
}

QString QNdc::get()
{
	try
	{
		DiagnosticContextStack* ptr = stack();
		if(ptr != NULL && !ptr->isEmpty())
		{
			return ptr->top().fullMessage;
		}
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}

	return "";
}

int QNdc::depth()
{
	try
	{
		DiagnosticContextStack* ptr = stack();
		if(ptr != NULL)
		{
			return ptr->size();
		}
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...) 
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}

	return 0;
}

QString QNdc::pop()
{
	try
	{
		DiagnosticContextStack* ptr = stack();
		if(ptr != NULL && !ptr->isEmpty())
		{
			DiagnosticContext dc = ptr->top();
			ptr->pop();

			if (ptr->isEmpty())
			{
				g_threadStorage.setLocalData(NULL);
			}

			return dc.message;
		}
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}

	return "";
}

QString QNdc::peek()
{
	try
	{
		DiagnosticContextStack* ptr = stack();
		if(ptr != NULL && !ptr->isEmpty())
		{
			return ptr->top().message;
		}
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}

	return "";
}

void QNdc::push(const QString& message)
{
	try
	{
		DiagnosticContextStack* ptr = stack();
		if (ptr == NULL)
		{
			ptr=new DiagnosticContextStack();
			g_threadStorage.setLocalData(ptr);
		}

		if(ptr->isEmpty())
		{
			ptr->push( DiagnosticContext(message, NULL) );
		}
		else
		{
			DiagnosticContext dc = ptr->top();
			ptr->push( DiagnosticContext(message, &dc) );
		}
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}
}

void QNdc::remove()
{
	try
	{
		g_threadStorage.setLocalData(NULL);
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}
}

void QNdc::setMaxDepth(int maxDepth)
{
	try {
		DiagnosticContextStack* ptr = stack();
		if(ptr != NULL)
		{
			while(maxDepth < ptr->size())
			{
				ptr->pop();
			}
		}
	}
	catch(std::exception& e)
	{
		loglog()->error("QNdc::%s - exception occurred: %s", __FUNCTION__, e.what());
	}
	catch(...)
	{
		loglog()->error("QNdc::%s - exception occurred", __FUNCTION__);
	}
}

QNdcContextCreator::QNdcContextCreator(const QString& msg) 
{ 
	QNdc::instance()->push(msg); 
}


QNdcContextCreator::~QNdcContextCreator() 
{ 
	QNdc::instance()->pop(); 
}
