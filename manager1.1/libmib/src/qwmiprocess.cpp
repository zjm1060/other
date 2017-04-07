#include "qwmiprocess.h"
#include "qimessage.h"
#include "qlogger.h"

QWmiProcess::QWmiProcess(QObject *parent)
: QObject(parent)
{
}

QWmiProcess::~QWmiProcess()
{
}

void QWmiProcess::clear()
{
	m_vecProcess.clear();
	m_mapUserName.clear();
}

void QWmiProcess::beferQuery() 
{
	m_vecProcess.clear(); 

	QMutableMapIterator<quint32, ProccessUserName> it(m_mapUserName);

	while (it.hasNext())
	{
		it.next();

		it.value().nBeatCount++;
	}
}

void QWmiProcess::afterQuery()
{
	QMutableMapIterator<quint32, ProccessUserName> it(m_mapUserName);

	while (it.hasNext())
	{
		it.next();

		if (it.value().nBeatCount > 0)
		{
			it.remove();
		}
	}
}

QStringList QWmiProcess::query()
{
	QStringList listQuery;
	listQuery<<"SELECT ExecutablePath, Name, ProcessId, SessionId, WorkingSetSize, KernelModeTime, UserModeTime FROM Win32_Process";

	return listQuery;
}

void QWmiProcess::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	SystemProcess process;

	process.executablePath=findString(object, L"ExecutablePath");
	process.name=findString(object, L"Name");
	process.processId=findUInt32(object, L"ProcessId");
	process.sessionId=findUInt32(object, L"SessionId");
	process.workingSetSize=findUInt64(object, L"WorkingSetSize");
	process.processorTime=findUInt64(object, L"KernelModeTime");

	quint64 nUserModeTime=findUInt64(object, L"UserModeTime");
	if (nUserModeTime != -1)
	{
		process.processorTime+=nUserModeTime;
	}
	else
	{
		process.processorTime=-1;
	}

	if (m_mapUserName.contains(process.processId))
	{
		ProccessUserName& usrName=m_mapUserName[process.processId];
		process.usrName=usrName.processName;
		usrName.nBeatCount=0;
	}
	else
	{
		QString strPath=QString("Win32_Process.Handle=%1").arg(process.processId);
		IWbemClassObject* pOutParams=NULL;

		HRESULT hr;

		VARIANT var;
		VariantInit(&var);
		hr = service->ExecMethod(_bstr_t(strPath.toStdWString().data()), L"GetOwner", 0, NULL, NULL, &pOutParams, NULL);
		if (SUCCEEDED(hr))
		{
			hr = pOutParams->Get(L"User", 0, &var, 0, 0);
			if (SUCCEEDED(hr))
			{
				if (var.vt != VT_NULL)
				{
					process.usrName=QString::fromLocal8Bit((char*)_bstr_t(V_BSTR(&var)));

					ProccessUserName usrName;
					usrName.nBeatCount=0;
					usrName.processName=process.usrName;

					m_mapUserName.insert(process.processId, usrName);
				}
			}
		}
		SAFE_RELEASE_COM(pOutParams);
		VariantClear(&var);
	}

	m_vecProcess.append(process);
}

int QWmiProcess::execMethod(IWbemServices* service, int method, const QStringList& arguments)
{
	Q_ASSERT_NO_NULL(service);

	HRESULT hr;

	int nRet=ExecMethodFailed;

	IWbemClassObject* pClass = NULL;
	hr = service->GetObject(L"Win32_Process", 0, NULL, &pClass, NULL);
	if (SUCCEEDED(hr))
	{
		IWbemClassObject* pInParamsDefinition = NULL;
		IWbemClassObject* pOutMethod = NULL;
		IWbemClassObject* pClassInstance = NULL;

		switch (method)
		{
		case CommandCreateProcess:
			{
				if (arguments.size() == 0)
				{
					break;
				}

				QString strText;
				QStringListIterator it(arguments);
				while (it.hasNext())
				{
					strText+=it.next()+" ";
				}

				logplusInfo(LIBMIB_LOGGER_STR, tr("Try to create the new task:")+strText);

				QString strFile=arguments[0].trimmed();
				QString strDirectory;
				QRegExp exp("^[A-Za-z]:/");
				if (exp.indexIn(strFile) != -1)
				{
					strDirectory=strFile.left(3);
				}

				HINSTANCE hInstance=ShellExecuteW(NULL, QString("open").toStdWString().data(), arguments[0].toStdWString().data(), 
					arguments.size() >=2? arguments[1].toStdWString().data():NULL, 
					strDirectory.isEmpty()? NULL:strDirectory.toStdWString().data(), SW_SHOWDEFAULT);

				if ((int)hInstance > 32)
				{
					logplusInfo(LIBMIB_LOGGER_STR, tr("Create the task successfully: %1").arg(strText));

					nRet=ExecMethodSucceed;
				}
				else 
				{
					logplusInfo(LIBMIB_LOGGER_STR, tr("Failed to create the task: %1").arg(strText));

					if ((int)hInstance == ERROR_FILE_NOT_FOUND || (int)hInstance == ERROR_PATH_NOT_FOUND)
					{
						nRet=ExecMethodPathNotFound;
					}
					else if ((int)hInstance == ERROR_BAD_FORMAT)
					{
						nRet=ExecMethodInvalidParameter;
					}
					else if ((int)hInstance == SE_ERR_ACCESSDENIED)
					{
						nRet=ExecMethodAccessDenied;
					}
					else
					{
						nRet=ExecMethodFailed;
					}
				}
				
// 				hr = pClass->GetMethod(L"Create", 0, &pInParamsDefinition, NULL);
// 				if (FAILED(hr))
// 				{
// 					break;
// 				}
// 
// 				hr = pInParamsDefinition->SpawnInstance(0, &pClassInstance);
// 				if (FAILED(hr))
// 				{
// 					break;
// 				}
// 
// 				// Create the values for the in-parameters
// 				VARIANT varCommand;
// 				VariantInit(&varCommand);
// 				varCommand.vt = VT_BSTR;
// 				varCommand.bstrVal = _bstr_t(arguments[0].toStdWString().data());
// 
// 				// Store the value for the in-parameters
// 				hr = pClassInstance->Put(L"CommandLine", 0, &varCommand, 0);
// 				VariantClear(&varCommand);
// 				if (FAILED(hr))
// 				{
// 					break;
// 				}
// 
// 				if (arguments.size() >= 2)
// 				{
// 					VariantInit(&varCommand);
// 					varCommand.vt = VT_BSTR;
// 					varCommand.bstrVal = _bstr_t(arguments[1].toStdWString().data());
// 					hr = pClassInstance->Put(L"CurrentDirectory", 0, &varCommand, 0);
// 					VariantClear(&varCommand);
// 					if (FAILED(hr))
// 					{
// 						break;
// 					}
// 				}
// 
// 				// Execute Method
// 				IWbemClassObject* pOutParams = NULL;
// 				hr = service->ExecMethod(L"Win32_Process", L"Create", 0, NULL, pClassInstance, &pOutParams, NULL);
// 
// 				if (SUCCEEDED(hr))
// 				{
// 					VARIANT varReturnValue;
// 					VariantInit(&varReturnValue);
// 					hr = pOutParams->Get(_bstr_t(L"ReturnValue"), 0, &varReturnValue, NULL, 0);
// 
// 					if (SUCCEEDED(hr))
// 					{
// 						quint32 nValue=(quint32)V_UINT(&varReturnValue);
// 						if (nValue == 0)
// 						{
// 							nRet=ExecMethodSucceed;
// 						}
// 					}
// 					
// 					VariantClear(&varReturnValue);
// 				}
// 
// 				SAFE_RELEASE_COM(pOutParams);
			}
			break;
		case CommandTerminateProcess:
			{
				if (arguments.size() == 0)
				{
					break;
				}

				logplusInfo(LIBMIB_LOGGER_STR, tr("Try to terminate the process:pid=%1").arg(arguments[0]));

				hr = pClass->GetMethod(L"Terminate", 0, &pInParamsDefinition, &pOutMethod);
				if (FAILED(hr))
				{
					break;
				}

				hr = pInParamsDefinition->SpawnInstance(0, &pClassInstance);
				if (FAILED(hr))
				{
					break;
				}

				VARIANT pcVal;
				VariantInit(&pcVal);
				V_VT(&pcVal) = VT_I4;

				// Store the value for the in parameters
				hr = pClassInstance->Put(L"Reason", 0, &pcVal, 0);
				VariantClear(&pcVal);
				if (FAILED(hr))
				{
					break;
				}

				// Execute Method
				QString strArg=QString("Win32_Process.Handle=%1").arg(arguments[0]);
				hr = service->ExecMethod(_bstr_t(strArg.toStdWString().data()),
					L"Terminate", 0, NULL, pClassInstance, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					logplusInfo(LIBMIB_LOGGER_STR, tr("Failed to terminate the process: pid=%1").arg(arguments[0]));

					nRet=ExecMethodSucceed;
				}
				else
				{
					logplusInfo(LIBMIB_LOGGER_STR, tr("Terminate the process successfully: pid=%1").arg(arguments[0]));
				}
			}
			break;
		default:
			nRet=ExecMethodInvalid;
			break;
		}

		SAFE_RELEASE_COM(pClassInstance);
		SAFE_RELEASE_COM(pOutMethod);
		SAFE_RELEASE_COM(pInParamsDefinition);
	}

	SAFE_RELEASE_COM(pClass);

	return nRet;
}

REGISTER_WMI_INSTANCE(QWmiProcess)
