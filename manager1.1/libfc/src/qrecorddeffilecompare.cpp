#include "qrecorddeffilecompare.h"

QRecordDefFileCompare::QRecordDefFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QRecordDefFileCompare::~QRecordDefFileCompare()
{

}

bool QRecordDefFileCompare::match(const QString& filename) const
{
	if (filename.endsWith(RECORD_DEF_FILE))
	{
		return true;
	}

	return false;
}

void QRecordDefFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QTextStream stream(&file);
	QVector<RECORD_STATION_ENTRY> stations=parseRecordDefFile(stream);
	result.data=qVariantFromValue(stations);

	result.titles<<tr("Station Name")<<tr("Device Name")<<tr("Include File");
}

void QRecordDefFileCompare::compareGroup(const QVector<RECORD_GROUP_ENTRY>& src, const QVector<RECORD_GROUP_ENTRY>& tar, QTreeModel* source, 
										QTreeModel* target, QModelIndex psrc, QModelIndex ptar)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<RECORD_GROUP_ENTRY> lcs;

	QVector<QPair<int, int> > results=lcs.parse(src, tar);
	results.append(QPair<int, int>(src.size(), tar.size()));

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		for (;nIndexTarget<peer.second && nIndexTarget<tar.size(); nIndexTarget++)
		{
			int nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);

			QModelIndex index=target->index(nRow, DeviceNameTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpName, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			target->setData(index, AppendRowRole, RowRole);

			QModelIndex pindex=index;

			QModelIndex pchild;
			if (nIndexTarget != peer.second && peer.second != tar.size())
			{
				nRow=source->rowCount(psrc);
				source->insertRow(nRow, psrc);

				pchild=source->index(nRow, DeviceNameTitle, psrc);
				source->setData(pchild, BlankRowRole, RowRole);
			}

			for (int i=0; i<tar[nIndexTarget].records.size(); i++)
			{
				const QString& record=tar[nIndexTarget].records[i];

				nRow=target->rowCount(pindex);
				target->insertRow(nRow, pindex);

				QModelIndex ppindex=target->index(nRow, IncludeFileTitle, pindex);
				target->setData(ppindex, record, Qt::DisplayRole);
				target->setData(ppindex, CommonItemRole, ItemRole);

				QModelIndex ppchild;
				if (nIndexTarget != peer.second && peer.second != tar.size())
				{
					nRow=source->rowCount(pchild);
					source->insertRow(nRow, pchild);

					ppchild=source->index(nRow, IncludeFileTitle, pchild);
					source->setData(ppchild, BlankRowRole, RowRole);
				}
			}
		}

		//source
		for (;nIndexSource<peer.first && nIndexSource<src.size(); nIndexSource++)
		{
			int nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			QModelIndex index=source->index(nRow, DeviceNameTitle, psrc);
			source->setData(index, src[nIndexSource].grpName, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			source->setData(index, DeleteRowRole, RowRole);

			QModelIndex pindex=index;

			QModelIndex pchild;
			if (nIndexSource != peer.first && peer.first != src.size())
			{
				nRow=target->rowCount(ptar);
				target->insertRow(nRow, ptar);

				pchild=target->index(nRow, DeviceNameTitle, ptar);
				target->setData(pchild, BlankRowRole, RowRole);
			}

			for (int i=0; i<src[nIndexSource].records.size(); i++)
			{
				const QString& record=src[nIndexSource].records[i];

				nRow=source->rowCount(pindex);
				source->insertRow(nRow, pindex);

				QModelIndex ppindex=source->index(nRow, IncludeFileTitle, pindex);
				source->setData(ppindex, record, Qt::DisplayRole);
				source->setData(ppindex, CommonItemRole, ItemRole);

				QModelIndex ppchild;
				if (nIndexSource != peer.first && peer.first != src.size())
				{
					nRow=target->rowCount(pchild);
					target->insertRow(nRow, pchild);

					ppchild=target->index(nRow, IncludeFileTitle, pchild);
					target->setData(ppchild, BlankRowRole, RowRole);
				}
			}
		}

		if (nIndexTarget<tar.size() && nIndexSource<src.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			QModelIndex pindex=source->index(nRow, DeviceNameTitle, psrc);
			source->setData(pindex, src[peer.first].grpName, Qt::DisplayRole);
			source->setData(pindex, CommonItemRole, ItemRole);
			source->setData(pindex, CommonRowRole, RowRole);

			nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);

			QModelIndex pchild=target->index(nRow, DeviceNameTitle, ptar);
			target->setData(pchild, tar[peer.second].grpName, Qt::DisplayRole);
			target->setData(pchild, CommonItemRole, ItemRole);
			target->setData(pchild, CommonRowRole, RowRole);

			compareRecords(src[peer.first].records, tar[peer.second].records, source, target, pindex, pchild);
		}
	}

	if (source->rowCount() < target->rowCount())
	{
		int nNum=target->rowCount()-source->rowCount();

		while (nNum-- > 0)
		{
			int nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			QModelIndex pindex=source->index(nRow, DeviceNameTitle, psrc);
			source->setData(pindex, BlankRowRole, RowRole);

			const QVector<QString>& records=tar[tar.size()-1-nNum].records;
			for (int i=0; i<records.size(); i++)
			{
				nRow=source->rowCount(pindex);
				source->insertRow(nRow, pindex);

				QModelIndex index=source->index(nRow, IncludeFileTitle, pindex);
				source->setData(index, BlankRowRole, RowRole);
			}
		}
	}
	else
	{
		int nNum=source->rowCount()-target->rowCount();

		while (nNum-- > 0)
		{
			int nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);

			QModelIndex pindex=target->index(nRow, DeviceNameTitle, ptar);
			target->setData(pindex, BlankRowRole, RowRole);

			const QVector<QString>& records=src[src.size()-1-nNum].records;
			for (int i=0; i<records.size(); i++)
			{
				nRow=target->rowCount(pindex);
				target->insertRow(nRow, pindex);

				QModelIndex index=target->index(nRow, IncludeFileTitle, pindex);
				target->setData(index, BlankRowRole, RowRole);
			}
		}
	}
}

void QRecordDefFileCompare::compareRecords(const QVector<QString>& src, const QVector<QString>& tar, QTreeModel* source, QTreeModel* target,
					QModelIndex psrc, QModelIndex ptar)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<QString> lcs;

	QVector<QPair<int, int> > results=lcs.parse(src, tar);
	results.append(QPair<int, int>(src.size(), tar.size()));

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		for (;nIndexTarget<peer.second && nIndexTarget<tar.size(); nIndexTarget++)
		{
			int nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);

			QModelIndex index=target->index(nRow, IncludeFileTitle, ptar);
			target->setData(index, tar[nIndexTarget], Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);
			target->setData(index, AppendRowRole, RowRole);

			nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			index=source->index(nRow, IncludeFileTitle, psrc);
			source->setData(index, BlankRowRole, RowRole);
		}

		//source
		for (;nIndexSource<peer.first && nIndexSource<src.size(); nIndexSource++)
		{
			int nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			QModelIndex index=source->index(nRow, IncludeFileTitle, psrc);
			source->setData(index, src[nIndexSource], Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);
			source->setData(index, DeleteRowRole, RowRole);

			nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);

			index=target->index(nRow, IncludeFileTitle, ptar);
			target->setData(index, BlankRowRole, RowRole);
		}

		if (peer.first < src.size() && peer.second < tar.size())
		{
			int nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);
			QModelIndex index=target->index(nRow, IncludeFileTitle, ptar);
			target->setData(index, tar[peer.second], Qt::DisplayRole);
			target->setData(index, CommonRowRole, RowRole);

			nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);
			index=source->index(nRow, IncludeFileTitle, psrc);
			source->setData(index, src[peer.first], Qt::DisplayRole);
			source->setData(index, CommonRowRole, RowRole);
		}
	}

	if (source->rowCount(psrc) < target->rowCount(ptar))
	{
		int nNum=target->rowCount(ptar)-source->rowCount(psrc);

		while (nNum-- > 0)
		{
			int nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			QModelIndex index=source->index(nRow, IncludeFileTitle, psrc);
			source->setData(index, BlankRowRole, RowRole);
		}
	}
	else
	{
		int nNum=source->rowCount(psrc)-target->rowCount(ptar);

		while (nNum-- > 0)
		{
			int nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);

			QModelIndex index=target->index(nRow, IncludeFileTitle, ptar);
			target->setData(index, BlankRowRole, RowRole);
		}
	}
}

void QRecordDefFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<RECORD_STATION_ENTRY> lcs;
	QVector<RECORD_STATION_ENTRY> src=qVariantValue<QVector<RECORD_STATION_ENTRY> >(source.data);
	QVector<RECORD_STATION_ENTRY> tar=qVariantValue<QVector<RECORD_STATION_ENTRY> >(target.data);

	QVector<QPair<int, int> > results=lcs.parse(src, tar);
	results.append(QPair<int, int>(src.size(), tar.size()));

 	source.model=new QTreeModel(source.titles.size());
 	((QTreeModel*)source.model)->setHorizontalHeaderLabels(source.titles);

	target.model=new QTreeModel(target.titles.size());
	((QTreeModel*)target.model)->setHorizontalHeaderLabels(target.titles);
 
	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		for (;nIndexTarget<peer.second && nIndexTarget<tar.size(); nIndexTarget++)
		{
			int nRow=target.model->rowCount();
			target.model->insertRow(nRow);

			QModelIndex index=target.model->index(nRow, StationNameTitle);
			target.model->setData(index, tar[nIndexTarget].stnName, Qt::DisplayRole);
			target.model->setData(index, CommonItemRole, ItemRole);

			target.model->setData(index, AppendRowRole, RowRole);

			QModelIndex pindex=index;

			QModelIndex pchild;
			if (nIndexTarget != peer.second && peer.second != tar.size())
			{
				nRow=source.model->rowCount();
				source.model->insertRow(nRow);

				pchild=source.model->index(nRow, StationNameTitle);
				source.model->setData(pchild, BlankRowRole, RowRole);
			}

			for (int i=0; i<tar[nIndexTarget].groups.size(); i++)
			{
				const RECORD_GROUP_ENTRY& group=tar[nIndexTarget].groups[i];

				nRow=target.model->rowCount(pindex);
				target.model->insertRow(nRow, pindex);

				QModelIndex ppindex=target.model->index(nRow, DeviceNameTitle, pindex);
				target.model->setData(ppindex, group.grpName, Qt::DisplayRole);
				target.model->setData(ppindex, CommonItemRole, ItemRole);

				QModelIndex ppchild;
				if (nIndexTarget != peer.second && peer.second != tar.size())
				{
					nRow=source.model->rowCount(pchild);
					source.model->insertRow(nRow, pchild);

					ppchild=source.model->index(nRow, DeviceNameTitle, pchild);
					source.model->setData(ppchild, BlankRowRole, RowRole);
				}

				for (int j=0; j<group.records.size(); j++)
				{
					nRow=target.model->rowCount(ppindex);
					target.model->insertRow(nRow, ppindex);

					index=target.model->index(nRow, IncludeFileTitle, ppindex);
					target.model->setData(index, group.records[j], Qt::DisplayRole);
					target.model->setData(index, CommonItemRole, ItemRole);

					if (nIndexTarget != peer.second && peer.second != tar.size())
					{
						nRow=source.model->rowCount(ppchild);
						source.model->insertRow(nRow, ppchild);

						index=source.model->index(nRow, IncludeFileTitle, ppchild);
						source.model->setData(index, BlankRowRole, RowRole);
					}
				}
			}
		}

		//source
		for (;nIndexSource<peer.first && nIndexSource<src.size(); nIndexSource++)
		{
			int nRow=source.model->rowCount();
			source.model->insertRow(nRow);

			QModelIndex index=source.model->index(nRow, StationNameTitle);
			source.model->setData(index, src[nIndexSource].stnName, Qt::DisplayRole);
			source.model->setData(index, CommonItemRole, ItemRole);

			source.model->setData(index, DeleteRowRole, RowRole);

			QModelIndex pindex=index;

			QModelIndex pchild;
			if (nIndexSource != peer.first && peer.first != src.size())
			{
				nRow=target.model->rowCount();
				target.model->insertRow(nRow);

				pchild=target.model->index(nRow, StationNameTitle);
				target.model->setData(pchild, BlankRowRole, RowRole);
			}

			for (int i=0; i<src[nIndexSource].groups.size(); i++)
			{
				const RECORD_GROUP_ENTRY& group=src[nIndexSource].groups[i];

				nRow=source.model->rowCount(pindex);
				source.model->insertRow(nRow, pindex);

				QModelIndex ppindex=source.model->index(nRow, DeviceNameTitle, pindex);
				source.model->setData(ppindex, group.grpName, Qt::DisplayRole);
				source.model->setData(ppindex, CommonItemRole, ItemRole);

				QModelIndex ppchild;
				if (nIndexSource != peer.first && peer.first != src.size())
				{
					nRow=target.model->rowCount(pchild);
					target.model->insertRow(nRow, pchild);

					ppchild=target.model->index(nRow, DeviceNameTitle, pchild);
					target.model->setData(ppchild, BlankRowRole, RowRole);
				}

				for (int j=0; j<group.records.size(); j++)
				{
					nRow=source.model->rowCount(ppindex);
					source.model->insertRow(nRow, ppindex);

					index=source.model->index(nRow, IncludeFileTitle, ppindex);
					source.model->setData(index, group.records[j], Qt::DisplayRole);
					source.model->setData(index, CommonItemRole, ItemRole);

					if (nIndexSource != peer.first && peer.first != src.size())
					{
						nRow=target.model->rowCount(ppchild);
						target.model->insertRow(nRow, ppchild);

						index=target.model->index(nRow, IncludeFileTitle, ppchild);
						target.model->setData(index, BlankRowRole, RowRole);
					}
				}
			}
		}

		if (nIndexTarget<tar.size() && nIndexSource<src.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int nRow=source.model->rowCount();
			source.model->insertRow(nRow);

			QModelIndex pindex=source.model->index(nRow, StationNameTitle);
			source.model->setData(pindex, src[peer.first].stnName, Qt::DisplayRole);
			source.model->setData(pindex, CommonItemRole, ItemRole);
			source.model->setData(pindex, CommonRowRole, RowRole);

			nRow=target.model->rowCount();
			target.model->insertRow(nRow);

			QModelIndex pchild=target.model->index(nRow, StationNameTitle);
			target.model->setData(pchild, tar[peer.second].stnName, Qt::DisplayRole);
			target.model->setData(pchild, CommonItemRole, ItemRole);
			target.model->setData(pchild, CommonRowRole, RowRole);

			compareGroup(src[peer.first].groups, tar[peer.second].groups, (QTreeModel*)source.model, (QTreeModel*)target.model, pindex, pchild);
		}
	}

	if (source.model->rowCount() < target.model->rowCount())
	{
		int nNum=target.model->rowCount()-source.model->rowCount();

		while (nNum-- > 0)
		{
			int nRow=source.model->rowCount();
			source.model->insertRow(nRow);

			QModelIndex pindex=source.model->index(nRow, StationNameTitle);
			source.model->setData(pindex, BlankRowRole, RowRole);

			const QVector<RECORD_GROUP_ENTRY>& groups=tar[tar.size()-1-nNum].groups;
			for (int i=0; i<groups.size(); i++)
			{
				nRow=source.model->rowCount(pindex);
				source.model->insertRow(nRow, pindex);

				QModelIndex index=source.model->index(nRow, DeviceNameTitle, pindex);
				source.model->setData(index, BlankRowRole, RowRole);
				for (int j=0; j<groups[i].records.size(); j++)
				{
					nRow=source.model->rowCount(index);
					source.model->insertRow(nRow, index);

					QModelIndex child=source.model->index(nRow, IncludeFileTitle, index);
					source.model->setData(child, BlankRowRole, RowRole);
				}
			}
		}
	}
	else
	{
		int nNum=source.model->rowCount()-target.model->rowCount();

		while (nNum-- > 0)
		{
			int nRow=target.model->rowCount();
			target.model->insertRow(nRow);

			QModelIndex pindex=target.model->index(nRow, StationNameTitle);
			target.model->setData(pindex, BlankRowRole, RowRole);

			const QVector<RECORD_GROUP_ENTRY>& groups=src[src.size()-1-nNum].groups;
			for (int i=0; i<groups.size(); i++)
			{
				nRow=target.model->rowCount(pindex);
				target.model->insertRow(nRow, pindex);

				QModelIndex index=target.model->index(nRow, DeviceNameTitle, pindex);
				target.model->setData(index, BlankRowRole, RowRole);
				for (int j=0; j<groups[i].records.size(); j++)
				{
					QModelIndex child=target.model->index(nRow, IncludeFileTitle, index);
					target.model->setData(child, BlankRowRole, RowRole);
				}
			}
		}
	}
}

bool QRecordDefFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<RECORD_STATION_ENTRY> lcs;
	QVector<RECORD_STATION_ENTRY> src=qVariantValue<QVector<RECORD_STATION_ENTRY> >(source.data);
	QVector<RECORD_STATION_ENTRY> tar=qVariantValue<QVector<RECORD_STATION_ENTRY> >(target.data);

	if (src.size() != tar.size())
	{
		return true;
	}

	QVector<QPair<int, int> > results=lcs.parse(src, tar);
	if (results.size() != src.size())
	{
		return true;
	}

	return false;
}

int QRecordDefFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	return NoError;
}

bool operator==(const RECORD_GROUP_ENTRY& source, const RECORD_GROUP_ENTRY& target)
{
	return source.grpName==target.grpName;
}

bool operator==(const RECORD_STATION_ENTRY& source, const RECORD_STATION_ENTRY& target)
{
	return source.stnName==target.stnName;
}

REGISTER_FC_INSTANCE(QRecordDefFileCompare) 
