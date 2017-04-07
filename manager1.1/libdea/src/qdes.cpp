#include "qdes.h"

#include "extern.h"

const static char g_desKey[]={32,46,12,3,95,70,8,103};

QDES::QDES()
	: QAbstractDES()
{

}

QDES::~QDES()
{

}

QByteArray QDES::encrypt(const QByteArray& data)
{
	return run(data, ENCRYPT);
}

QByteArray QDES::decrypt(const QByteArray& data)
{
	return run(data, DECRYPT);
}

QByteArray QDES::run(const QByteArray& data, int type)
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

	QAbstractDES::run(buffer.data(), src, nSize, g_desKey, sizeof(g_desKey), type);

	SAFE_DELETE_ARRAY(src);

	return buffer;
}

REGISTER_DEA_INSTANCE(QDES)
