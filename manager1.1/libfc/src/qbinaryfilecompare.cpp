#include "qbinaryfilecompare.h"

#include <QStandardItemModel>
#include <QCryptographicHash>

#include "common.h"
#include "qfileex.h"

#define BINARY_FILE_MD5_PACKET_SIZE (16*1024)

QBinaryFileCompare::QBinaryFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QBinaryFileCompare::~QBinaryFileCompare()
{

}

bool QBinaryFileCompare::match(const QString& filename) const
{
	return !isTextFormat(filename);
}

void QBinaryFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly))
	{
		result.errcode=FileOpenError;
		return;
	}

	QCryptographicHash algorithm(QCryptographicHash::Md5);

	Q_IO_DEVICE_DATA_STREAM(stream, file);
	while (!stream.atEnd())
	{
		QByteArray buffer;
		buffer.resize(BINARY_FILE_MD5_PACKET_SIZE);

		int nSize=stream.readRawData(buffer.data(), BINARY_FILE_MD5_PACKET_SIZE);

		if (nSize != -1)
		{
			buffer.resize(nSize);
			algorithm.addData(buffer);
		}
	}

	QByteArray eigenvalue=algorithm.result();

	result.data=qVariantFromValue(eigenvalue);

	result.titles<<"";
}

void QBinaryFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	source.model=NULL;
	target.model=NULL;
}

bool QBinaryFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QByteArray src=qVariantValue<QByteArray>(source.data);
	QByteArray tar=qVariantValue<QByteArray>(target.data);

	if (src != tar)
	{
		return true;
	}

	return false;
}

int QBinaryFileCompare::write(FileCompareResult& source, FileCompareResult& target, QPair<QModelIndex, QModelIndex>& error)
{
	if (target.filename.isEmpty() || source.filename.isEmpty())
	{
		return FileNameIsEmptyError;
	}

	if (target.type == type())
	{
		if (QFileEx::exists(target.filename))
		{
			QFileEx::remove(target.filename);
		}

		if (!QFileEx::copy(source.filename, target.filename))
		{
			return FileWriteError;
		}
	}
	else if (m_pChildChain != NULL)
	{
		return m_pChildChain->write(source, target, error);
	}

	return NoError;
}


int QBinaryFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	return NoError;
}

bool QBinaryFileCompare::isHeaderHidden() const
{
	return true;
}

REGISTER_FC_INSTANCE(QBinaryFileCompare) 
