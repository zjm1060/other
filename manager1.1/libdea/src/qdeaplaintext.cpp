#include "qdeaplaintext.h"

#include "extern.h"

QDeaPlainText::QDeaPlainText()
	: QIDEA()
{

}

QDeaPlainText::~QDeaPlainText()
{

}

QByteArray QDeaPlainText::encrypt(const QByteArray& data)
{
	return data;
}

QByteArray QDeaPlainText::decrypt(const QByteArray& data)
{
	return data;
}

REGISTER_DEA_INSTANCE(QDeaPlainText)
