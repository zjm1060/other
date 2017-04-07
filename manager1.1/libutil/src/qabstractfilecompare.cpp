#include "qabstractfilecompare.h"

#include "qfileex.h"

QAbstractFileCompare::QAbstractFileCompare()
{

}

QAbstractFileCompare::~QAbstractFileCompare()
{

}

void QAbstractFileCompare::setNextChain(const QAbstractFileComparePtr& ptr, bool replace)
{
	if (ptr == NULL)
	{
		return;
	}

	if (!replace)
	{
		ptr->setNextChain(m_pChildChain, replace);
	}
	
	m_pChildChain=ptr;
}

QAbstractFileComparePtr& QAbstractFileCompare::nextChain()
{
	return m_pChildChain;
}

QVector<QString> QAbstractFileCompare::parseString(const QString& text)
{
	QString strText=text.trimmed();
	if (strText.isEmpty() || strText.startsWith(QChar('!')) || strText.startsWith(QChar('#')))
	{
		return QVector<QString>();
	}

	QVector<QString> labels;

	QRegExp rx("\\s+|\\'|\"|=|,");

	int nPos=0;
	int nCur=0;
	bool bQuoteField=false;
	while ((nCur = rx.indexIn(strText, nCur)) != -1)
	{
		QString str=rx.cap(0);

		nCur+= rx.matchedLength();

		if (str=="\'" || str=="\"")
		{
			bQuoteField=!bQuoteField;

			if (!bQuoteField)
			{
				str=strText.mid(nPos, nCur-nPos).trimmed();
			}
		}
		else if (!bQuoteField)
		{
			str=strText.mid(nPos, nCur-nPos-1).trimmed();
		}

		if (!bQuoteField)
		{
			if (!str.isEmpty())
			{
				labels<<str;
			}

			nPos=nCur;
		}
	}

	if (nPos != strText.length())
	{
		QString str=strText.right(strText.length()-nPos);
		if (!str.isEmpty())
		{
			labels<<str;
		}
	}

	return labels;
}

void QAbstractFileCompare::writeProperty(QTextStream& stream, const QString& key, const QString& value)
{
	stream<<"\t\t"<<key<<" = "<<value;
}

void QAbstractFileCompare::writeGroup(QTextStream& stream, const QString& group, const QString& key, const QString& value)
{
	stream<<group<<"\t"<<key<<" = "<<value;
}

QAbstractFileCompare::FileCompareResult QAbstractFileCompare::parse(const QString& filename)
{
	FileCompareResult result;
	result.type=COMPARE_UNKNOW_FILE;
	result.errcode=NoError;
	result.model=NULL;
	result.filename=filename;

	if (!match(filename))
	{
		if (m_pChildChain != NULL)
		{
			return m_pChildChain->parse(filename);
		}
		else
		{
			return result;
		}
	}
	else
	{
		result.type=type();

		QFile file(filename);
		if (file.exists())
		{
			parseFile(file, result);
		}
		else
		{
			result.errcode=FileNoExistError;
		}
	}

	return result;
}

bool QAbstractFileCompare::isModified(QAbstractFileCompare::FileCompareResult& source, QAbstractFileCompare::FileCompareResult& target)
{
	if (source.type != target.type)
	{
		return true;
	}

	if (source.type == type())
	{
		return checkModified(source, target);
	}
	else if (m_pChildChain != NULL)
	{
		return m_pChildChain->isModified(source, target);
	}

	return false;
}

bool QAbstractFileCompare::isHeaderHidden(const FileCompareResult& source, const FileCompareResult& target) const
{
	if (source.type == type() || target.type == type())
	{
		return isHeaderHidden();
	}
	else if (m_pChildChain != NULL)
	{
		return m_pChildChain->isHeaderHidden(source, target);
	}

	return false;
}

void QAbstractFileCompare::compare(QAbstractFileCompare::FileCompareResult& source, QAbstractFileCompare::FileCompareResult& target)
{
	if (target.type == type() || ((target.type == COMPARE_UNKNOW_FILE || target.errcode != NoError) && source.type == type()))
	{
		if (target.type == type() && source.type != type())
		{
			source.type=target.type;
			source.errcode=NoError;
			source.data=QVariant();
			source.titles=target.titles;
		}
		else if (target.type == COMPARE_UNKNOW_FILE)
		{
			target.type=source.type;
			target.errcode=NoError;
			target.data=QVariant();
			target.titles=source.titles;
		}

		if (target.errcode != NoError)
		{
			target.titles=source.titles;
		}

		compareFile(source, target);
	}
	else if (m_pChildChain != NULL)
	{
		m_pChildChain->compare(source, target);
	}
}

int QAbstractFileCompare::check(FileCompareResult& result, QPair<QModelIndex, QModelIndex>& error)
{
	if (result.type == type())
	{
		if (result.model != NULL && error.first != QModelIndex())
		{
			result.model->setData(error.first, QVariant(), VerifyItemRole);

			if (error.second != QModelIndex())
			{
				result.model->setData(error.second, QVariant(), VerifyItemRole);
			}
		}

		return checkValidity(result, error);
	}
	else if (m_pChildChain != NULL)
	{
		return m_pChildChain->check(result, error);
	}

	return UnknowError;
}

int QAbstractFileCompare::checkValidity(FileCompareResult& result, QPair<QModelIndex, QModelIndex>& error)
{
	Q_UNUSED(result);
	Q_UNUSED(error);

	return NoError;
}

int QAbstractFileCompare::write(FileCompareResult& source, FileCompareResult& target, QPair<QModelIndex, QModelIndex>& error)
{
	Q_UNUSED(source);

	if (target.filename.isEmpty())
	{
		return FileNameIsEmptyError;
	}

	if (target.type != type() && m_pChildChain != NULL)
	{
		return m_pChildChain->write(source, target, error);
	}

	int errCode=check(target, error);
	if (errCode != NoError)
	{
		return errCode;
	}

	QFileEx file(target.filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return FileOpenError;
	}

	return write(&file, target, error);
}

int QAbstractFileCompare::write(QIODevice* device, FileCompareResult& result, QPair<QModelIndex, QModelIndex>& error)
{
	if (device == NULL || result.model == NULL)
	{
		return false;
	}

	if (!device->isWritable())
	{
		return false;
	}

	if (result.type == type())
	{
		return writeData(device, result);
	}
	else if (m_pChildChain != NULL)
	{
		return m_pChildChain->write(device, result, error);
	}

	return false;
}

bool QAbstractFileCompare::isHeaderHidden() const
{
	return false;
}

QVector<RECORD_STATION_ENTRY> parseRecordDefFile(QTextStream& stream)
{
	QVector<RECORD_STATION_ENTRY> records;

	int nNumStn=-1;
	int nStnValid=0;
	int nNumGroup=-1;
	int nGroupValid=0;
	while (!stream.atEnd())
	{
		QString strText=stream.readLine();

		QVector<QString> labels=QAbstractFileCompare::parseString(strText);
		if (labels.isEmpty())
		{
			continue;
		}

		if (labels[0] == "END")
		{
			break;
		}

		if (labels.size() == 3 && labels[0] == "STATION" && labels[1] == "NAME")
		{
			nStnValid=1;

			for(int i=0; i<records.size(); i++)
			{
				if(records[i].stnName == labels[2])
				{
					nStnValid=0;
					break;
				}
			}

			if (nStnValid == 1)
			{
				nNumStn++;

				nGroupValid=0;
				nNumGroup=-1;

				records.append(RECORD_STATION_ENTRY());
				records[nNumStn].stnName=labels[2];
			}

			continue;
		}

		if (nStnValid == 0)
		{
			continue;
		}

		if (labels.size() == 3 && labels[0] == "GROUP" && labels[1] == "NAME")
		{
			nGroupValid=1;

			for(int i=0; i<records[nNumStn].groups.size(); i++)
			{
				if(records[nNumStn].groups[i].grpName == labels[2])
				{
					nGroupValid=0;
					break;
				}
			}

			if (nGroupValid == 1)
			{
				nNumGroup++;

				records[nNumStn].groups.append(RECORD_GROUP_ENTRY());
				records[nNumStn].groups[nNumGroup].grpName=labels[2];
			}

			continue;
		}

		if (nGroupValid == 0)
		{
			continue;
		}

		if (labels.size() == 2)
		{
			if (labels[0] ==  "INCLUDE")
			{
				records[nNumStn].groups[nNumGroup].records<<labels[1];
				continue;
			}
		}
	}

	return records;
}

QVector<STATION_ENTRY> parseStationDefFile(QTextStream& stream)
{
	QVector<STATION_ENTRY> stations;

	int nNumStn=-1;
	int nStnValid=0;
	while (!stream.atEnd())
	{
		QString strText=stream.readLine();

		QVector<QString> labels=QAbstractFileCompare::parseString(strText);
		if (labels.isEmpty())
		{
			continue;
		}

		if (labels[0] == "END")
		{
			break;
		}

		if (labels.size() == 3 && labels[0] == "STATION" && labels[1] == "NAME")
		{
			nStnValid=1;

			for(int i=0; i<stations.size(); i++)
			{
				if(stations[i].stnName == labels[2])
				{
					nStnValid=0;
					break;
				}
			}

			if (nStnValid == 1)
			{
				nNumStn++;

				stations.append(STATION_ENTRY());
				stations[nNumStn].stnName=labels[2];
			}

			continue;
		}

		if (nStnValid == 0)
		{
			continue;
		}

		if (labels.size() == 2)
		{
			if (labels[0] ==  "STN_ID")
			{
				bool bOk;
				int nStnId=labels[1].toInt(&bOk);

				if (!bOk || nStnId<1 || nStnId>255)
				{
					nStnValid=0;

					stations.remove(nNumStn);
					nNumStn--;

					continue;
				}

				for(int i=0; i<stations.size(); i++)
				{
					if((uchar)nStnId == stations[i].stnId)
					{
						nStnValid=0;
						stations.remove(nNumStn);
						nNumStn--;
						break;
					}
				}

				if (nStnValid == 1)
				{
					stations[nNumStn].stnId=(uchar)nStnId;
				}

				continue;
			}
			else if (labels[0] ==  "LONGNAME")
			{
				stations[nNumStn].stnLongname=labels[1];
				continue;
			}
			else if (labels[0] ==  "COMMENT")
			{
				stations[nNumStn].stnComment=labels[1];
				continue;
			}
			else if (labels[0] ==  "EXLONGNAME")
			{
				stations[nNumStn].stnExLongname=labels[1];
				continue;
			}
			else if (labels[0] ==  "EXCOMMENT")
			{
				stations[nNumStn].stnExComment=labels[1];
				continue;
			}
			else if (labels[0] ==  "GROUP_MAX")
			{
				bool bOk;
				uint nNumGroup=labels[1].toUInt(&bOk);

				if (bOk)
				{
					stations[nNumStn].grp_num=nNumGroup;
				}
				continue;
			}
		}
	}

	return stations;
}

QVector<GROUP_ENTRY_EX> parseGroupDefFile(QTextStream& stream)
{
	QVector<GROUP_ENTRY_EX> groups;

	int nNumStn=-1;
	int nStnValid=0;
	int nNumGroup=-1;
	int nGroupValid=0;
	while (!stream.atEnd())
	{
		QString strText=stream.readLine();

		QVector<QString> labels=QAbstractFileCompare::parseString(strText);
		if (labels.isEmpty())
		{
			continue;
		}

		if (labels[0] == "END")
		{
			break;
		}

		if (labels.size() == 3 && labels[0] == "STATION" && labels[1] == "NAME")
		{
			nStnValid=1;

			for(int i=0; i<groups.size(); i++)
			{
				if(groups[i].stnName == labels[2])
				{
					nStnValid=0;
					break;
				}
			}

			if (nStnValid == 1)
			{
				nNumStn++;

				nGroupValid=0;
				nNumGroup=-1;

				groups.append(GROUP_ENTRY_EX());
				groups[nNumStn].stnName=labels[2];
			}

			continue;
		}

		if (nStnValid == 0)
		{
			continue;
		}

		if (labels.size() == 3 && labels[0] == "GROUP" && labels[1] == "NAME")
		{
			nGroupValid=1;

			for(int i=0; i<groups[nNumStn].entries.size(); i++)
			{
				if(groups[nNumStn].entries[i].grpName == labels[2])
				{
					nGroupValid=0;
					break;
				}
			}

			if (nGroupValid == 1)
			{
				nNumGroup++;

				groups[nNumStn].entries.append(GROUP_ENTRY());
				groups[nNumStn].entries[nNumGroup].grpName=labels[2];
				groups[nNumStn].entries[nNumGroup].grpId=nNumGroup;
			}

			continue;
		}

		if (nGroupValid == 0)
		{
			continue;
		}

		if (labels.size() == 2)
		{
			if (labels[0] ==  "DEV_ID")
			{
				bool bOk;
				int nGrpId=labels[1].toInt(&bOk);

				if (!bOk || nGrpId<1 || nGrpId>255)
				{
					nGroupValid=0;

					groups[nNumStn].entries.remove(nNumGroup);
					nNumGroup--;
					continue;
				}

				for(int i=0; i<groups[nNumStn].entries.size(); i++)
				{
					if((uchar)nGrpId == groups[nNumStn].entries[i].grpId)
					{
						nGroupValid=0;

						groups[nNumStn].entries.remove(nNumGroup);
						nNumGroup--;
						break;
					}
				}

				if (nGroupValid == 1)
				{
					groups[nNumStn].entries[nNumGroup].grpId=(uchar)nGrpId;
				}

				continue;
			}
			else if (labels[0] ==  "LONGNAME")
			{
				groups[nNumStn].entries[nNumGroup].grpLongname=labels[1];
				continue;
			}
			else if (labels[0] ==  "COMMENT")
			{
				groups[nNumStn].entries[nNumGroup].grpComment=labels[1];
				continue;
			}
			else if (labels[0] == "RTU_TYPE")
			{
				groups[nNumStn].entries[nNumGroup].grpTypeName=labels[1];

				continue;
			}
			else if (labels[0] ==  "EXLONGNAME")
			{
				groups[nNumStn].entries[nNumGroup].grpExLongname=labels[1];
				continue;
			}
			else if (labels[0] ==  "EXCOMMENT")
			{
				groups[nNumStn].entries[nNumGroup].grpExComment=labels[1];
				continue;
			}
			else if (labels[0] ==  "CONDITION_DEV")
			{
				groups[nNumStn].entries[nNumGroup].grpConditionDev=labels[1];
				continue;
			}
			else if (labels[0] ==  "CONDITION_TYPE")
			{
				groups[nNumStn].entries[nNumGroup].grpConditionType=labels[1];
				continue;
			}
			else if (labels[0] ==  "CONDITION_ID")
			{
				groups[nNumStn].entries[nNumGroup].grpConditionID=labels[1];
				continue;
			}
		}
	}

	return groups;
}
