#include "qtripledes.h"

#include "extern.h"

const static char g_tripleDESKey[]={21,3,57,9,35,12,4,11,79,126,36,9,65,29,3,60};

QTripleDES::QTripleDES()
	: QAbstractDES()
{

}

QTripleDES::~QTripleDES()
{

}

QByteArray QTripleDES::encrypt(const QByteArray& data)
{
	return run(data, ENCRYPT);
}

QByteArray QTripleDES::decrypt(const QByteArray& data)
{
	return run(data, DECRYPT);
}

QByteArray QTripleDES::run(const QByteArray& data, int type)
{
	if (data.isEmpty())
	{
		return data;
	}

	int nSize=(data.size()+7)&0xfffffff8;
	char* src=new char[nSize];
	memset(src, 0, nSize);
	memcpy_s(src, nSize, data.data(), data.size());

	QByteArray buffer;
	buffer.resize(nSize);

	QAbstractDES::run(buffer.data(), src, nSize, g_tripleDESKey, sizeof(g_tripleDESKey), type);

	SAFE_DELETE_ARRAY(src);

	return buffer;
}

REGISTER_DEA_INSTANCE(QTripleDES)
