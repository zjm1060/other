#include "qabstractwmi.h"

#include "qlogger.h"
#include "qimessage.h"

QAbstractWmi::QAbstractWmi()
{
}

QAbstractWmi::~QAbstractWmi()
{

}

QVariant QAbstractWmi::exec(IWbemServices* service)
{
	Q_ASSERT_NO_NULL(service);

	QMutexLocker locker(&m_mutex);

	beferQuery();

	HRESULT hr;

	IEnumWbemClassObject* pEnumerator=NULL;

	QStringList listQuery=query();
	for (int i=0; i<listQuery.size(); i++)
	{
		hr = service->ExecQuery(
			bstr_t("WQL"), 
			bstr_t(listQuery[i].toLocal8Bit().data()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator);

		if (FAILED(hr))
		{
			logplusDebug(LIBUTIL_LOGGER_STR, QString("Query failed, Error code = %1").arg(hr));
			break;
		}

		indicate(pEnumerator, service, i);

		SAFE_RELEASE_COM(pEnumerator);
	}

	afterQuery();

	return result();
}

void QAbstractWmi::indicate(IEnumWbemClassObject* enumerator, IWbemServices* service, int index)
{
	Q_ASSERT_NO_NULL(enumerator);
	Q_ASSERT_NO_NULL(service);
	Q_ASSERT(index >= 0);

	HRESULT hr;

	IWbemClassObject* object=NULL;
	ULONG uReturn= 0;

	while (enumerator)
	{
		hr= enumerator->Next(WBEM_INFINITE, 1, &object, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		Q_ASSERT_NO_NULL(object);

		record(object, service, index);

		SAFE_RELEASE_COM(object);
	}
}

void QAbstractWmi::beferQuery()
{

}

void QAbstractWmi::afterQuery()
{

}

void QAbstractWmi::clear()
{

}

int QAbstractWmi::execMethod(IWbemServices* service, int method, const QStringList& arguments)
{
	Q_UNUSED(service);
	Q_UNUSED(method);
	Q_UNUSED(arguments);

	return ExecMethodSucceed;
}

quint16 QAbstractWmi::findUInt16(IWbemClassObject* object, LPCWSTR wszName)
{
	Q_ASSERT_NO_NULL(object);

	quint16 nResult=-1;

	HRESULT hr;
	VARIANT var;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (!FAILED(hr))
	{
		if (var.vt != VT_NULL)
		{
			nResult=V_UI2(&var);
		}
	}
	VariantClear(&var);

	return nResult;
}

quint32 QAbstractWmi::findUInt32(IWbemClassObject* object, LPCWSTR wszName)
{
	Q_ASSERT_NO_NULL(object);

	quint32 nResult=-1;

	HRESULT hr;
	VARIANT var;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (!FAILED(hr))
	{
		if (var.vt != VT_NULL)
		{
			nResult=V_UI4(&var);
		}
	}
	VariantClear(&var);

	return nResult;
}

quint64  QAbstractWmi::findUInt64(IWbemClassObject* object, LPCWSTR wszName)
{
	Q_ASSERT_NO_NULL(object);

	quint64 nResult=-1;

	HRESULT hr;
	VARIANT var;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (!FAILED(hr))
	{
		if (var.vt != VT_NULL)
		{
			nResult=QString::fromLocal8Bit((char*)_bstr_t(V_BSTR(&var))).toULongLong();
		}
	}
	VariantClear(&var);

	return nResult;
}

QString QAbstractWmi::findString(IWbemClassObject* object, LPCWSTR wszName)
{
	Q_ASSERT_NO_NULL(object);

	QString str;

	HRESULT hr;
	VARIANT var;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (!FAILED(hr))
	{
		if (var.vt != VT_NULL)
		{
			str=QString::fromLocal8Bit((char*)_bstr_t(V_BSTR(&var)));
		}
	}
	VariantClear(&var);

	return str;
}

bool QAbstractWmi::findBoolean(IWbemClassObject* object, LPCWSTR wszName)
{
	Q_ASSERT_NO_NULL(object);

	bool bValue=false;

	HRESULT hr;
	VARIANT var;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (!FAILED(hr))
	{
		if (var.vt != VT_NULL)
		{
			bValue=V_BOOL(&var);
		}
	}
	VariantClear(&var);

	return bValue;
}

QVector<QString> QAbstractWmi::findStringArray(IWbemClassObject* object, LPCWSTR wszName)
{
	QVector<QString> array;

	HRESULT hr;

	VARIANT var;

	LONG lstart, lend;
	SAFEARRAY* sa;
	BSTR bstr;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (SUCCEEDED(hr))
	{
		if (var.vt != VT_NULL)
		{
			sa = var.parray;
			if(SUCCEEDED(SafeArrayGetLBound( sa, 1, &lstart )) && SUCCEEDED(SafeArrayGetUBound( sa, 1, &lend )))
			{
				QString str;

				for (LONG i=lstart; i<=lend; i++)
				{
					if(SUCCEEDED(SafeArrayGetElement(sa, &i, &bstr)))
					{
						str=QString::fromLocal8Bit(_com_util::ConvertBSTRToString(bstr));
						array.append(str);
					}
				}
			}
		}
	}
	VariantClear(&var);

	return array;
}

QVector<quint16> QAbstractWmi::findUInt16Array(IWbemClassObject* object, LPCWSTR wszName)
{
	QVector<quint16> array;

	HRESULT hr;

	VARIANT var;

	LONG lstart, lend;
	SAFEARRAY* sa;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (SUCCEEDED(hr))
	{
		if (var.vt != VT_NULL)
		{
			sa = var.parray;
			if(SUCCEEDED(SafeArrayGetLBound( sa, 1, &lstart )) && SUCCEEDED(SafeArrayGetUBound( sa, 1, &lend )))
			{
				quint16 value;
				for (LONG i=lstart; i<=lend; i++)
				{
					if(SUCCEEDED(SafeArrayGetElement(sa, &i, &value)))
					{
						array.append(value);
					}
				}
			}
		}
	}
	VariantClear(&var);

	return array;
}

qreal QAbstractWmi::findReal(IWbemClassObject* object, LPCWSTR wszName)
{
	Q_ASSERT_NO_NULL(object);

	qreal dResult=-1;

	HRESULT hr;
	VARIANT var;

	VariantInit(&var);
	hr = object->Get(wszName, 0, &var, 0, 0);
	if (!FAILED(hr))
	{
		if (var.vt != VT_NULL)
		{
			dResult=(qreal)V_R8(&var);
		}
	}
	VariantClear(&var);

	return dResult;
}
