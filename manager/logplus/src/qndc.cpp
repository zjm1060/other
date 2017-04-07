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

QMutex	QNDC::m_gMutex;
QSharedObjectPtr<QNDC> QNDC::m_pInstance;
QThreadStorage<DiagnosticContextStack*>	QNDC::g_threadStorage;

QNDC::QNDC()
{
}

QNDC::~QNDC()
{
}

QSharedObjectPtr<QNDC> QNDC::instance()
{
	if (m_pInstance.get() == 0)
	{
		m_gMutex.lock();
		if (m_pInstance.get() == 0)
		{
			m_pInstance=QSharedObjectPtr<QNDC>(new QNDC);
		}
		m_gMutex.unlock();
	}

	return m_pInstance;
}

void QNDC::clear()
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
		logLog().error(QString("NDC::clear()- exception occured: %s").arg(e.what()));
	}
	catch(...) 
	{
		logLog().error("NDC::clear()- exception occured");
	}
}

DiagnosticContextStack* QNDC::stack()
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

DiagnosticContextStack QNDC::cloneStack()
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
		logLog().error(QString("NDC::cloneStack()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::cloneStack()- exception occured");
	}

	return DiagnosticContextStack();
}

void QNDC::inherit(const DiagnosticContextStack& s)
{
	try
	{
		g_threadStorage.setLocalData(new DiagnosticContextStack(s));
	}
	catch(std::exception& e)
	{
		logLog().error(QString("NDC::inherit()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::inherit()- exception occured");
	}
}

QString QNDC::get()
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
		logLog().error(QString("NDC::get()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::get()- exception occured");
	}

	return "";
}

int QNDC::depth()
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
		logLog().error(QString("NDC::getDepth()- exception occured: %s").arg(e.what()));
	}
	catch(...) {
		logLog().error("NDC::getDepth()- exception occured");
	}

	return 0;
}

QString QNDC::pop()
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
		logLog().error(QString("NDC::pop()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::pop()- exception occured");
	}

	return "";
}

QString QNDC::peek()
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
		logLog().error(QString("NDC::peek()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::peek()- exception occured");
	}

	return "";
}

void QNDC::push(const QString& message)
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
		logLog().error(QString("NDC::push()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::push()- exception occured");
	}
}

void QNDC::remove()
{
	try
	{
		g_threadStorage.setLocalData(NULL);
	}
	catch(std::exception& e)
	{
		logLog().error(QString("NDC::remove()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::remove()- exception occured");
	}
}

void QNDC::setMaxDepth(int maxDepth)
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
		logLog().error(QString("NDC::setMaxDepth()- exception occured: %s").arg(e.what()));
	}
	catch(...)
	{
		logLog().error("NDC::setMaxDepth()- exception occured");
	}
}

QNDCContextCreator::QNDCContextCreator(const QString& msg) 
{ 
	QNDC::instance()->push(msg); 
}


QNDCContextCreator::~QNDCContextCreator() 
{ 
	QNDC::instance()->pop(); 
}
