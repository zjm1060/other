#include "qwmifactory.h"

#include "define.h"

QMutex QWmiFactory::m_mutex;
QWmiFactory* QWmiFactory::m_pInstance=0;

QWmiFactory::QWmiFactory()
:m_pLoc(NULL),
m_pSvc(NULL)
{
	m_bInitilize=init();
}

QWmiFactory::~QWmiFactory()
{
	foreach (QAbstractWmi* pWmi, m_mapWmi)
	{
		SAFE_DELETE_CLASS(pWmi);
	}

	release();
}

void QWmiFactory::release()
{
	if (m_bInitilize)
	{
		SAFE_RELEASE_COM(m_pSvc);
		SAFE_RELEASE_COM(m_pLoc);
		CoUninitialize();
	}
}

QWmiFactory* QWmiFactory::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QWmiFactory;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void QWmiFactory::registerWmi(QAbstractWmi* wmi)
{
	Q_ASSERT_NO_NULL(wmi);

	if (!m_mapWmi.contains(wmi->type()))
	{
		m_mapWmi.insert(wmi->type(), wmi);
	}
}

QAbstractWmi* QWmiFactory::wmiInstance(int type)
{
	return m_mapWmi.value(type, NULL);
}

QVariant QWmiFactory::exec(int type)
{
	if (!m_bInitilize)
	{
		return NULL;
	}

	QAbstractWmi* pWmi=m_mapWmi.value(type);
	if (pWmi != NULL)
	{
		return pWmi->exec(m_pSvc);
	}

	return NULL;
}

int QWmiFactory::execMethod(int type, int method, const QStringList& arguments)
{
	if (!m_bInitilize)
	{
		return QAbstractWmi::ExecMethodComInvalid;
	}

	QAbstractWmi* pWmi=m_mapWmi.value(type);
	if (pWmi != NULL)
	{
		return pWmi->execMethod(m_pSvc, method, arguments);
	}

	return QAbstractWmi::ExecMethodComInvalid;
}

bool QWmiFactory::init()
{
	HRESULT hr;

	// Step 1:Initialize COM

	hr =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hr))
	{
#ifdef DEBUG
		cout << "Failed to initialize COM library. Error code = 0x" << hex << hr << endl;
#endif
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
		cout << "Failed to initialize security. Error code = 0x" << hex << hr << endl;
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
		cout << "Failed to create IWbemLocator object."<< " Err code = 0x"<< hex << hr << endl;
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
		cout << "Could not connect. Error code = 0x" << hex << hr << endl;
		release();
		return 1;                // Program has failed.
	}

	cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

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
		cout << "Could not set proxy blanket. Error code = 0x" << hex << hr << endl;
		release();
		return false;               // Program has failed.
	}


	return true;
}

