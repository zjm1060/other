#include "qgroupdeffilecompare.h"

QGroupDefFileCompare::QGroupDefFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QGroupDefFileCompare::~QGroupDefFileCompare()
{

}

bool QGroupDefFileCompare::match(const QString& filename) const
{
	if (filename.endsWith(GROUP_DEF_FILE))
	{
		return true;
	}

	return false;
}

void QGroupDefFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QTextStream stream(&file);
	QVector<GROUP_ENTRY_EX> stations=parseGroupDefFile(stream);
	result.data=qVariantFromValue(stations);

	result.titles<<tr("Station Name")<<tr("Device ID")<<tr("Device Name")<<tr("RTU Type")<<tr("Long Name")<<tr("Comment")
		<<tr("Condition Device")<<tr("Condition Type")<<tr("Condition ID");
}

void QGroupDefFileCompare::compareGroup(const QVector<GROUP_ENTRY>& src, const QVector<GROUP_ENTRY>& tar, QTreeModel* source, 
										QTreeModel* target, QModelIndex psrc, QModelIndex ptar)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<GROUP_ENTRY> lcs;

	QVector<QPair<int, int> > results=lcs.parse(src, tar);
	results.append(QPair<int, int>(src.size(), tar.size()));

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		for (;nIndexTarget<=peer.second && nIndexTarget<tar.size(); nIndexTarget++)
		{
			int nRow=target->rowCount(ptar);
			target->insertRow(nRow, ptar);

			QModelIndex index=target->index(nRow, GroupIDTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpId, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			if (nIndexTarget != peer.second)
			{
				target->setData(index, AppendRowRole, RowRole);
			}
			else
			{
				target->setData(index, CommonRowRole, RowRole);
			}

			index=target->index(nRow, DeviceNameTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpName, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			index=target->index(nRow, RTUTypeTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpTypeName, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			index=target->index(nRow, LongNameTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpLongname, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			index=target->index(nRow, CommentTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpComment, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			index=target->index(nRow, ConditionDeviceTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpConditionDev, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			index=target->index(nRow, ConditionDeviceTypeTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpConditionType, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			index=target->index(nRow, ConditionDeviceIDTitle, ptar);
			target->setData(index, tar[nIndexTarget].grpConditionID, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			if (nIndexTarget != peer.second && peer.second != tar.size())
			{
				nRow=source->rowCount(psrc);
				source->insertRow(nRow, psrc);

				index=source->index(nRow, GroupIDTitle, psrc);
				source->setData(index, BlankRowRole, RowRole);
			}
		}

		//source
		for (;nIndexSource<=peer.first && nIndexSource<src.size(); nIndexSource++)
		{
			int nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			QModelIndex index=source->index(nRow, GroupIDTitle, psrc);
			source->setData(index, src[nIndexSource].grpId, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			if (nIndexSource != peer.first)
			{
				source->setData(index, DeleteRowRole, RowRole);
			}
			else
			{
				source->setData(index, CommonRowRole, RowRole);
			}

			index=source->index(nRow, DeviceNameTitle, psrc);
			source->setData(index, src[nIndexSource].grpName, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			index=source->index(nRow, RTUTypeTitle, psrc);
			source->setData(index, src[nIndexSource].grpTypeName, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			index=source->index(nRow, LongNameTitle, psrc);
			source->setData(index, src[nIndexSource].grpLongname, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			index=source->index(nRow, CommentTitle, psrc);
			source->setData(index, src[nIndexSource].grpComment, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			index=source->index(nRow, ConditionDeviceTitle, psrc);
			source->setData(index, src[nIndexSource].grpConditionDev, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			index=source->index(nRow, ConditionDeviceTypeTitle, psrc);
			source->setData(index, src[nIndexSource].grpConditionType, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			index=source->index(nRow, ConditionDeviceIDTitle, psrc);
			source->setData(index, src[nIndexSource].grpConditionID, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			if (nIndexSource != peer.first && peer.first != src.size())
			{
				nRow=target->rowCount(ptar);
				target->insertRow(nRow, ptar);

				index=target->index(nRow, GroupIDTitle, ptar);
				target->setData(index, BlankRowRole, RowRole);
			}
		}

		if (nIndexTarget<tar.size() && nIndexSource<src.size())
		{
			if (src[peer.first].grpName != tar[peer.second].grpName)
			{
				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyRowRole, RowRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyRowRole, RowRole);

				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyItemRole, ItemRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyItemRole, ItemRole);
			}

			if (src[peer.first].grpTypeName != tar[peer.second].grpTypeName)
			{
				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyRowRole, RowRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyRowRole, RowRole);

				source->setData(source->index(nIndexSource, RTUTypeTitle, psrc), ModifyItemRole, ItemRole);
				target->setData(target->index(nIndexTarget, RTUTypeTitle, ptar), ModifyItemRole, ItemRole);
			}

			if (src[peer.first].grpLongname != tar[peer.second].grpLongname)
			{
				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyRowRole, RowRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyRowRole, RowRole);

				source->setData(source->index(nIndexSource, LongNameTitle, psrc), ModifyItemRole, ItemRole);
				target->setData(target->index(nIndexTarget, LongNameTitle, ptar), ModifyItemRole, ItemRole);
			}

			if (src[peer.first].grpComment != tar[peer.second].grpComment)
			{
				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyRowRole, RowRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyRowRole, RowRole);

				source->setData(source->index(nIndexSource, CommentTitle, psrc), ModifyItemRole, ItemRole);
				target->setData(target->index(nIndexTarget, CommentTitle, ptar), ModifyItemRole, ItemRole);
			}

			if (src[peer.first].grpConditionDev != tar[peer.second].grpConditionDev)
			{
				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyRowRole, RowRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyRowRole, RowRole);

				source->setData(source->index(nIndexSource, ConditionDeviceTitle, psrc), ModifyItemRole, ItemRole);
				target->setData(target->index(nIndexTarget, ConditionDeviceTitle, ptar), ModifyItemRole, ItemRole);
			}

			if (src[peer.first].grpConditionType != tar[peer.second].grpConditionType)
			{
				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyRowRole, RowRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyRowRole, RowRole);

				source->setData(source->index(nIndexSource, ConditionDeviceTypeTitle, psrc), ModifyItemRole, ItemRole);
				target->setData(target->index(nIndexTarget, ConditionDeviceTypeTitle, ptar), ModifyItemRole, ItemRole);
			}

			if (src[peer.first].grpConditionID != tar[peer.second].grpConditionID)
			{
				source->setData(source->index(nIndexSource, GroupIDTitle, psrc), ModifyRowRole, RowRole);
				target->setData(target->index(nIndexTarget, GroupIDTitle, ptar), ModifyRowRole, RowRole);

				source->setData(source->index(nIndexSource, ConditionDeviceIDTitle, psrc), ModifyItemRole, ItemRole);
				target->setData(target->index(nIndexTarget, ConditionDeviceIDTitle, ptar), ModifyItemRole, ItemRole);
			}
		}
	}

	if (source->rowCount(psrc) < target->rowCount(ptar))
	{
		int nNum=target->rowCount(ptar)-source->rowCount(psrc);

		while (nNum-- > 0)
		{
			int nRow=source->rowCount(psrc);
			source->insertRow(nRow, psrc);

			QModelIndex index=source->index(nRow, GroupIDTitle, psrc);
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

			QModelIndex index=target->index(nRow, GroupIDTitle, ptar);
			target->setData(index, BlankRowRole, RowRole);
		}
	}
}

void QGroupDefFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<GROUP_ENTRY_EX> lcs;
	QVector<GROUP_ENTRY_EX> src=qVariantValue<QVector<GROUP_ENTRY_EX> >(source.data);
	QVector<GROUP_ENTRY_EX> tar=qVariantValue<QVector<GROUP_ENTRY_EX> >(target.data);

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

			for (int i=0; i<tar[nIndexTarget].entries.size(); i++)
			{
				const GROUP_ENTRY& group=tar[nIndexTarget].entries[i];

				nRow=target.model->rowCount(pindex);
				target.model->insertRow(nRow, pindex);

				index=target.model->index(nRow, GroupIDTitle, pindex);
				target.model->setData(index, group.grpId, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				index=target.model->index(nRow, DeviceNameTitle, pindex);
				target.model->setData(index, group.grpName, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				index=target.model->index(nRow, RTUTypeTitle, pindex);
				target.model->setData(index, group.grpTypeName, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				index=target.model->index(nRow, LongNameTitle, pindex);
				target.model->setData(index, group.grpLongname, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				index=target.model->index(nRow, CommentTitle, pindex);
				target.model->setData(index, group.grpComment, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				index=target.model->index(nRow, ConditionDeviceTitle, pindex);
				target.model->setData(index, group.grpConditionDev, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				index=target.model->index(nRow, ConditionDeviceTypeTitle, pindex);
				target.model->setData(index, group.grpConditionType, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				index=target.model->index(nRow, ConditionDeviceIDTitle, pindex);
				target.model->setData(index, group.grpConditionID, Qt::DisplayRole);
				target.model->setData(index, CommonItemRole, ItemRole);

				if (nIndexTarget != peer.second && peer.second != tar.size())
				{
					nRow=source.model->rowCount(pchild);
					source.model->insertRow(nRow, pchild);

					index=source.model->index(nRow, StationNameTitle, pchild);
					source.model->setData(index, BlankRowRole, RowRole);
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

			for (int i=0; i<src[nIndexSource].entries.size(); i++)
			{
				const GROUP_ENTRY& group=src[nIndexSource].entries[i];

				nRow=source.model->rowCount(pindex);
				source.model->insertRow(nRow, pindex);

				index=source.model->index(nRow, GroupIDTitle, pindex);
				source.model->setData(index, group.grpId, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				index=source.model->index(nRow, DeviceNameTitle, pindex);
				source.model->setData(index, group.grpName, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				index=source.model->index(nRow, RTUTypeTitle, pindex);
				source.model->setData(index, group.grpTypeName, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				index=source.model->index(nRow, LongNameTitle, pindex);
				source.model->setData(index, group.grpLongname, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				index=source.model->index(nRow, CommentTitle, pindex);
				source.model->setData(index, group.grpComment, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				index=source.model->index(nRow, ConditionDeviceTitle, pindex);
				source.model->setData(index, group.grpConditionDev, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				index=source.model->index(nRow, ConditionDeviceTypeTitle, pindex);
				source.model->setData(index, group.grpConditionType, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				index=source.model->index(nRow, ConditionDeviceIDTitle, pindex);
				source.model->setData(index, group.grpConditionID, Qt::DisplayRole);
				source.model->setData(index, CommonItemRole, ItemRole);

				if (nIndexSource != peer.first && peer.first != src.size())
				{
					nRow=target.model->rowCount(pchild);
					target.model->insertRow(nRow, pchild);

					index=target.model->index(nRow, StationNameTitle, pchild);
					target.model->setData(index, BlankRowRole, RowRole);
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

			compareGroup(src[peer.first].entries, tar[peer.second].entries, (QTreeModel*)source.model, (QTreeModel*)target.model, pindex, pchild);
		}
	}

	if (source.model->rowCount() < target.model->rowCount())
	{
		int nNum=target.model->rowCount()-source.model->rowCount();

		while (nNum-- > 0)
		{
			int nRow=source.model->rowCount();
			source.model->insertRow(nRow);

			QModelIndex index=source.model->index(nRow, StationNameTitle);
			source.model->setData(index, BlankRowRole, RowRole);

			const QVector<GROUP_ENTRY>& groups=tar[tar.size()-1-nNum].entries;
			for (int i=0; i<groups.size(); i++)
			{
				nRow=source.model->rowCount(index);
				source.model->insertRow(nRow, index);

				QModelIndex child=source.model->index(nRow, GroupIDTitle, index);
				source.model->setData(child, BlankRowRole, RowRole);
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

			QModelIndex index=target.model->index(nRow, StationNameTitle);
			target.model->setData(index, BlankRowRole, RowRole);

			const QVector<GROUP_ENTRY>& groups=src[src.size()-1-nNum].entries;
			for (int i=0; i<groups.size(); i++)
			{
				nRow=target.model->rowCount(index);
				target.model->insertRow(nRow, index);

				QModelIndex child=target.model->index(nRow, GroupIDTitle, index);
				target.model->setData(child, BlankRowRole, RowRole);
			}
		}
	}
}

bool QGroupDefFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<GROUP_ENTRY_EX> lcs;
	QVector<GROUP_ENTRY_EX> src=qVariantValue<QVector<GROUP_ENTRY_EX> >(source.data);
	QVector<GROUP_ENTRY_EX> tar=qVariantValue<QVector<GROUP_ENTRY_EX> >(target.data);

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

int QGroupDefFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	return NoError;
}

bool operator==(const GROUP_ENTRY& source, const GROUP_ENTRY& target)
{
	return source.grpId==target.grpId;
}

bool operator==(const GROUP_ENTRY_EX& source, const GROUP_ENTRY_EX& target)
{
	return source.stnName==target.stnName;
}

REGISTER_FC_INSTANCE(QGroupDefFileCompare) 
