#include "qwmifactory.h"

#include <QLibrary>

#include "define.h"
#include "qlogger.h"
#include "qimessage.h"

QWmiFactory::QWmiFactory()
:m_bInitialize(false)
,m_pLoc(NULL)
,m_pSvc(NULL)
{
	
}

QWmiFactory::~QWmiFactory()
{
	m_mapWmi.clear();
}

void QWmiFactory::initialize()
{
	if (!m_bInitialize)
	{
		m_bInitialize=init();
	}
}

void QWmiFactory::release()
{
	if (m_bInitialize)
	{
		m_bInitialize=false;

		SAFE_RELEASE_COM(m_pSvc);
		SAFE_RELEASE_COM(m_pLoc);
		CoUninitialize();
	}
}

QWmiFactory* QWmiFactory::instance()
{
	static QWmiFactory* pInstance=new QWmiFactory;

	return pInstance;
}

void QWmiFactory::registerWmi(QAbstractWmiPtr wmi)
{
	Q_ASSERT_NO_NULL(wmi);

	if (!m_mapWmi.contains(wmi->type()))
	{
		m_mapWmi.insert(wmi->type(), wmi);
	}
}

QAbstractWmiPtr QWmiFactory::wmiInstance(int type)
{
	return m_mapWmi.value(type, QAbstractWmiPtr(NULL));
}

QVariant QWmiFactory::exec(int type)
{
	if (!m_bInitialize)
	{
		logplusDebug(LIBUTIL_LOGGER_STR, "The WMI has not been initialized!!!");
		return QVariant();
	}

	QAbstractWmiPtr pWmi=m_mapWmi.value(type);
	if (pWmi != NULL)
	{
		return pWmi->exec(m_pSvc);
	}

	return QVariant();
}

int QWmiFactory::execMethod(int type, int method, const QStringList& arguments)
{
	if (!m_bInitialize)
	{
		return ExecMethodComInvalid;
	}

	QAbstractWmiPtr pWmi=m_mapWmi.value(type);
	if (pWmi != NULL)
	{
		return pWmi->execMethod(m_pSvc, method, arguments);
	}

	return ExecMethodComInvalid;
}

bool QWmiFactory::init()
{
	HRESULT hr;

	// Step 1:Initialize COM

	hr =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hr))
	{
		logplusDebug(LIBMIB_LOGGER_STR, QString("Failed to initialize COM library, Error code = %1").arg(hr, 0, 16));
		return false;                  // Program has failed.
	}

	hr =  CoInitializeSecurity(
		NULL, 
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);

	if (FAILED(hr))
	{
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Failed to initialize security, Error code = %1").arg(hr, 0, 16));

		CoUninitialize();
		return false;                    // Program has failed.
	}

	hr = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *)&m_pLoc);

	if (FAILED(hr))
	{
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Failed to create IWbemLocator object, Error code = %1").arg(hr, 0, 16));

		CoUninitialize();
		return false;                 // Program has failed.
	}

	// Connect to the local root\cimv2 namespace
	// and obtain pointer pSvc to make IWbemServices calls.
	hr = m_pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), 
		NULL,
		NULL,
		0,
		NULL,
		0,
		0, 
		&m_pSvc
		);

	if (FAILED(hr))
	{
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Could not connect, Error code = %1").arg(hr , 0, 16));

		release();
		return 1;                // Program has failed.
	}

	logplusDebug(LIBUTIL_LOGGER_STR, "Connected to ROOT\\CIMV2 WMI namespace");

	hr = CoSetProxyBlanket(
		m_pSvc,                       // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hr))
	{
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Could not set proxy blanket, Error code = %1").arg(hr, 0, 16));

		release();
		return false;               // Program has failed.
	}


	return true;
}

bool QWmiFactory::loadLibrary(const QString& filename, const char* symbol)
{
	CreateWmiInstanceFunc func;

	QLibrary library(filename);
	func=(CreateWmiInstanceFunc)library.resolve(symbol);

	if (func != 0)
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QAbstractWmiPtr> instances;
		func(instances);

		foreach(QAbstractWmiPtr pWmi, instances)
		{
			if (pWmi != 0)
			{
				instance()->registerWmi(pWmi);
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
