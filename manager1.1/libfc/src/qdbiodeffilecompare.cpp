#include "qdbiodeffilecompare.h"

QDbioDefFileCompare::QDbioDefFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QDbioDefFileCompare::~QDbioDefFileCompare()
{

}

bool QDbioDefFileCompare::match(const QString& filename) const
{
	if (filename.endsWith(IOGROUP_DEF_FILE))
	{
		return true;
	}

	return false;
}

void QDbioDefFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QTextStream stream(&file);
	QVector<DBIO_DEF_ENTRY> records=parseIOGroupDefFile(stream);
	result.data=qVariantFromValue(records);

	result.titles<<tr("Station Name")<<tr("Group Name")<<tr("IOGroup ID")<<tr("Comment")<<tr("Number")<<tr("Status");
}

void QDbioDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		  const QVector<QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=curIntexTarget != endIndexTarget && endIndexTarget != data.size();

		int nRow=target->rowCount(tindex);
		target->insertRow(nRow, tindex);

		QModelIndex pindex=target->index(nRow, GroupNameTitle, tindex);
		target->setData(pindex, data[curIntexTarget].grpName, Qt::DisplayRole);
		target->setData(pindex, CommonItemRole, ItemRole);
		target->setData(pindex, append? AppendRowRole:DeleteRowRole, RowRole);

		QModelIndex cindex;
		if (bBlank)
		{
			nRow=source->rowCount(sindex);
			source->insertRow(nRow, sindex);

			cindex=source->index(nRow, GroupNameTitle, sindex);
			source->setData(cindex, BlankRowRole, RowRole);
		}

		for (int i=0; i<data[curIntexTarget].iogrps.size(); i++)
		{
			const DBIO_IOGROUP_DEF_ENTRY& iogroup=data[curIntexTarget].iogrps[i];

			fillBlank(source, target, cindex, pindex, iogroup, bBlank);
		}
	}
}

void QDbioDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		  const QVector<QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=curIntexTarget != endIndexTarget && endIndexTarget != data.size();

		int nRow=target->rowCount(tindex);
		target->insertRow(nRow, tindex);

		QModelIndex pindex=target->index(nRow, IOGroupIDTitle, tindex);
		target->setData(pindex, data[curIntexTarget].id, Qt::DisplayRole);
		target->setData(pindex, CommonItemRole, ItemRole);
		target->setData(pindex, append? AppendRowRole:DeleteRowRole, RowRole);

		QModelIndex index=target->index(nRow, CommentTitle, tindex);
		target->setData(index, data[curIntexTarget].comment, Qt::DisplayRole);
		target->setData(index, CommonItemRole, ItemRole);

		QModelIndex cindex;
		if (bBlank)
		{
			nRow=source->rowCount(sindex);
			source->insertRow(nRow, sindex);

			cindex=source->index(nRow, IOGroupIDTitle, sindex);
			source->setData(cindex, BlankRowRole, RowRole);
		}

		for (int i=0; i<data[curIntexTarget].status.size(); i++)
		{
			const DBIO_IOGROUP_STATUS_ENTRY& status=data[curIntexTarget].status[i];

			fillBlank(source, target, cindex, pindex, status, bBlank);
		}
	}
}

void QDbioDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		  const QVector<QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=curIntexTarget != endIndexTarget && endIndexTarget != data.size();

		int nRow=target->rowCount(tindex);
		target->insertRow(nRow, tindex);

		QModelIndex pindex=target->index(nRow, NumberTitle, tindex);
		target->setData(pindex, data[curIntexTarget].name, Qt::DisplayRole);
		target->setData(pindex, CommonItemRole, ItemRole);
		target->setData(pindex, append? AppendRowRole:DeleteRowRole, RowRole);

		QModelIndex index=target->index(nRow, StatusTitle, tindex);
		target->setData(pindex, data[curIntexTarget].status, Qt::DisplayRole);
		target->setData(pindex, CommonItemRole, ItemRole);

		QModelIndex cindex;
		if (bBlank)
		{
			nRow=source->rowCount(sindex);
			source->insertRow(nRow, sindex);

			cindex=source->index(nRow, NumberTitle, sindex);
			source->setData(cindex, BlankRowRole, RowRole);
		}
	}
}

void QDbioDefFileCompare::fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY>& data)
{
	while (num-- > 0)
	{
		int nRow=model->rowCount();
		model->insertRow(nRow);

		QModelIndex cindex=model->index(nRow, StationNameTitle);
		model->setData(cindex, BlankRowRole, RowRole);

		const QVector<DBIO_GROUP_DEF_ENTRY>& groups=data[data.size()-1-num].groups;
		fill(groups.size(), model, groups, cindex);
	}
}

void QDbioDefFileCompare::fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY>& data, QModelIndex pindex)
{
	while (num-- > 0)
	{
		int nRow=model->rowCount(pindex);
		model->insertRow(nRow, pindex);

		QModelIndex cindex=model->index(nRow, GroupNameTitle, pindex);
		model->setData(cindex, BlankRowRole, RowRole);

		const QVector<DBIO_IOGROUP_DEF_ENTRY>& iogroups=data[data.size()-1-num].iogrps;
		fill(iogroups.size(), model, iogroups, cindex);
	}
}

void QDbioDefFileCompare::fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY>& data, QModelIndex pindex)
{
	while (num-- > 0)
	{
		int nRow=model->rowCount(pindex);
		model->insertRow(nRow, pindex);

		QModelIndex cindex=model->index(nRow, IOGroupIDTitle, pindex);
		model->setData(cindex, BlankRowRole, RowRole);

		const QVector<DBIO_IOGROUP_STATUS_ENTRY>& states=data[data.size()-1-num].status;
		fill(states.size(), model, states, cindex);
	}
}

void QDbioDefFileCompare::fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY>& data,
							   QModelIndex pindex)
{
	Q_UNUSED(data);

	while (num-- > 0)
	{
		int nRow=model->rowCount(pindex);
		model->insertRow(nRow, pindex);

		QModelIndex cindex=model->index(nRow, NumberTitle, pindex);
		model->setData(cindex, BlankRowRole, RowRole);
	}
}

void QDbioDefFileCompare::compareGroups(const QVector<DBIO_GROUP_DEF_ENTRY>& sdata, const QVector<DBIO_GROUP_DEF_ENTRY>& tdata, 
									   QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY> lcs;

	QVector<QPair<int, int> > results=lcs.parse(sdata, tdata);
	results.append(QPair<int, int>(sdata.size(), tdata.size()));

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(source, target, nIndexTarget, peer.second, tdata, sindex, tindex);

		//source
		diff(target, source, nIndexSource, peer.first, sdata, tindex, sindex, false);

		if (nIndexTarget<tdata.size() && nIndexSource<sdata.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int nRow=source->rowCount(sindex);
			source->insertRow(nRow, sindex);

			QModelIndex pindex=source->index(nRow, GroupNameTitle, sindex);
			source->setData(pindex, sdata[peer.first].grpName, Qt::DisplayRole);
			source->setData(pindex, CommonItemRole, ItemRole);
			source->setData(pindex, CommonRowRole, RowRole);

			nRow=target->rowCount(tindex);
			target->insertRow(nRow, tindex);

			QModelIndex cindex=target->index(nRow, GroupNameTitle, tindex);
			target->setData(cindex, tdata[peer.second].grpName, Qt::DisplayRole);
			target->setData(cindex, CommonItemRole, ItemRole);
			target->setData(cindex, CommonRowRole, RowRole);

			compareIOGroups(sdata[peer.first].iogrps, tdata[peer.second].iogrps, source, target, pindex, cindex);
		}
	}

	if (source->rowCount(sindex) < target->rowCount(tindex))
	{
		int nNum=target->rowCount(tindex)-source->rowCount(sindex);

		fill(nNum, source, tdata, sindex);
	}
	else if (source->rowCount(sindex) > target->rowCount(tindex))
	{
		int nNum=source->rowCount(sindex)-target->rowCount(tindex);

		fill(nNum, target, sdata, tindex);
	}
}

void QDbioDefFileCompare::compareIOGroups(const QVector<DBIO_IOGROUP_DEF_ENTRY>& sdata, const QVector<DBIO_IOGROUP_DEF_ENTRY>& tdata, 
					 QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY> lcs;

	QVector<QPair<int, int> > results=lcs.parse(sdata, tdata);
	results.append(QPair<int, int>(sdata.size(), tdata.size()));

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(source, target, nIndexTarget, peer.second, tdata, sindex, tindex);

		//source
		diff(target, source, nIndexSource, peer.first, sdata, tindex, sindex, false);

		if (nIndexTarget<tdata.size() && nIndexSource<sdata.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int nRow=source->rowCount(sindex);
			source->insertRow(nRow, sindex);

			QModelIndex pindex=source->index(nRow, IOGroupIDTitle, sindex);
			source->setData(pindex, sdata[peer.first].id, Qt::DisplayRole);
			source->setData(pindex, CommonItemRole, ItemRole);
			source->setData(pindex, CommonRowRole, RowRole);

			QModelIndex index=source->index(nRow, CommentTitle, sindex);
			source->setData(index, sdata[peer.first].comment, Qt::DisplayRole);
			source->setData(index, CommonItemRole, ItemRole);

			nRow=target->rowCount(tindex);
			target->insertRow(nRow, tindex);

			QModelIndex cindex=target->index(nRow, IOGroupIDTitle, tindex);
			target->setData(cindex, tdata[peer.second].id, Qt::DisplayRole);
			target->setData(cindex, CommonItemRole, ItemRole);
			target->setData(cindex, CommonRowRole, RowRole);

			index=target->index(nRow, CommentTitle, tindex);
			target->setData(index, tdata[peer.second].comment, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			compareStates(sdata[peer.first].status, tdata[peer.second].status, source, target, pindex, cindex);
		}
	}

	if (source->rowCount(sindex) < target->rowCount(tindex))
	{
		int nNum=target->rowCount(tindex)-source->rowCount(sindex);

		fill(nNum, source, tdata, sindex);
	}
	else if (source->rowCount(sindex) > target->rowCount(tindex))
	{
		int nNum=source->rowCount(sindex)-target->rowCount(tindex);

		fill(nNum, target, sdata, tindex);
	}
}

void QDbioDefFileCompare::compareStates(const QVector<DBIO_IOGROUP_STATUS_ENTRY>& sdata, const QVector<DBIO_IOGROUP_STATUS_ENTRY>& tdata,
										QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<DBIO_IOGROUP_STATUS_ENTRY> lcs;

	QVector<QPair<int, int> > results=lcs.parse(sdata, tdata);
	results.append(QPair<int, int>(sdata.size(), tdata.size()));

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(source, target, nIndexTarget, peer.second, tdata, sindex, tindex);

		//source
		diff(target, source, nIndexSource, peer.first, sdata, tindex, sindex, false);

		if (peer.first < sdata.size() && peer.second < tdata.size())
		{
			nIndexSource++;
			nIndexTarget++;

			int nRow=target->rowCount(tindex);
			target->insertRow(nRow, tindex);
			QModelIndex index=target->index(nRow, NumberTitle, tindex);
			target->setData(index, tdata[peer.second].name, Qt::DisplayRole);
			target->setData(index, CommonRowRole, RowRole);

			index=target->index(nRow, StatusTitle, tindex);
			target->setData(index, tdata[peer.second].status, Qt::DisplayRole);

			nRow=source->rowCount(sindex);
			source->insertRow(nRow, sindex);
			index=source->index(nRow, NumberTitle, sindex);
			source->setData(index, sdata[peer.first].name, Qt::DisplayRole);
			source->setData(index, CommonRowRole, RowRole);

			index=source->index(nRow, StatusTitle, sindex);
			source->setData(index, sdata[peer.first].status, Qt::DisplayRole);
		}
	}

	if (source->rowCount(sindex) < target->rowCount(tindex))
	{
		int nNum=target->rowCount(tindex)-source->rowCount(sindex);

		fill(nNum, source, tdata, sindex);
	}
	else if (source->rowCount(sindex) > target->rowCount(tindex))
	{
		int nNum=source->rowCount(sindex)-target->rowCount(tindex);

		fill(nNum, target, sdata, tindex);
	}
}

void QDbioDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,
							  int endIndexTarget, const QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY>& data, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=curIntexTarget != endIndexTarget && endIndexTarget != data.size();

		int nRow=target->rowCount();
		target->insertRow(nRow);

		QModelIndex pindex=target->index(nRow, StationNameTitle);
		target->setData(pindex, data[curIntexTarget].stnName, Qt::DisplayRole);
		target->setData(pindex, CommonItemRole, ItemRole);
		target->setData(pindex, append? AppendRowRole:DeleteRowRole, RowRole);

		QModelIndex cindex;
		if (bBlank)
		{
			nRow=source->rowCount();
			source->insertRow(nRow);

			cindex=source->index(nRow, StationNameTitle);
			source->setData(cindex, BlankRowRole, RowRole);
		}

		for (int i=0; i<data[curIntexTarget].groups.size(); i++)
		{
			const DBIO_GROUP_DEF_ENTRY& group=data[curIntexTarget].groups[i];

			fillBlank(source, target, cindex, pindex, group, bBlank);
		}
	}
}

void QDbioDefFileCompare::fillBlank(QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex,
			   const DBIO_GROUP_DEF_ENTRY& group, bool blank)
{
	int nRow=target->rowCount(tindex);
	target->insertRow(nRow, tindex);

	QModelIndex pindex=target->index(nRow, GroupNameTitle, tindex);
	target->setData(pindex, group.grpName, Qt::DisplayRole);
	target->setData(pindex, CommonItemRole, ItemRole);

	QModelIndex cindex;
	if (blank)
	{
		nRow=source->rowCount(sindex);
		source->insertRow(nRow, sindex);

		cindex=source->index(nRow, GroupNameTitle, sindex);
		source->setData(cindex, BlankRowRole, RowRole);
	}

	for (int i=0; i<group.iogrps.size(); i++)
	{
		const DBIO_IOGROUP_DEF_ENTRY& iogroup=group.iogrps[i];

		fillBlank(source, target, cindex, pindex, iogroup, blank);
	}
}

void QDbioDefFileCompare::fillBlank(QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex,
									const DBIO_IOGROUP_DEF_ENTRY& iogroup, bool blank)
{
	int nRow=target->rowCount(tindex);
	target->insertRow(nRow, tindex);

	QModelIndex pindex=target->index(nRow, IOGroupIDTitle, tindex);
	target->setData(pindex, iogroup.id, Qt::DisplayRole);
	target->setData(pindex, CommonItemRole, ItemRole);

	QModelIndex index=target->index(nRow, CommentTitle, tindex);
	target->setData(index, iogroup.comment, Qt::DisplayRole);

	QModelIndex cindex;
	if (blank)
	{
		nRow=source->rowCount(sindex);
		source->insertRow(nRow, sindex);

		cindex=source->index(nRow, IOGroupIDTitle, sindex);
		source->setData(cindex, BlankRowRole, RowRole);
		source->setData(cindex, CommonItemRole, ItemRole);
	}

	for (int i=0; i<iogroup.status.size(); i++)
	{
		const DBIO_IOGROUP_STATUS_ENTRY& status=iogroup.status[i];

		fillBlank(source, target, cindex, pindex, status, blank);
	}
}

void QDbioDefFileCompare::fillBlank(QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex,
			   const DBIO_IOGROUP_STATUS_ENTRY& status, bool blank)
{
	int nRow=target->rowCount(tindex);
	target->insertRow(nRow, tindex);

	QModelIndex index=target->index(nRow, NumberTitle, tindex);
	target->setData(index, status.name, Qt::DisplayRole);
	target->setData(index, CommonItemRole, ItemRole);

	index=target->index(nRow, StatusTitle, tindex);
	target->setData(index, status.status, Qt::DisplayRole);

	if (blank)
	{
		nRow=source->rowCount(sindex);
		source->insertRow(nRow, sindex);

		index=source->index(nRow, NumberTitle, sindex);
		source->setData(index, BlankRowRole, RowRole);
		source->setData(index, CommonItemRole, ItemRole);
	}
}

void QDbioDefFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QDbioDefFileCompare::DBIO_DEF_ENTRY> lcs;
	QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> src=qVariantValue<QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> >(source.data);
	QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> tar=qVariantValue<QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> >(target.data);

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
		diff(source.model, target.model, nIndexTarget, peer.second, tar);

		//source
		diff(target.model, source.model, nIndexSource, peer.first, src, false);

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

			QModelIndex cindex=target.model->index(nRow, StationNameTitle);
			target.model->setData(cindex, tar[peer.second].stnName, Qt::DisplayRole);
			target.model->setData(cindex, CommonItemRole, ItemRole);
			target.model->setData(cindex, CommonRowRole, RowRole);

			compareGroups(src[peer.first].groups, tar[peer.second].groups, source.model, target.model, pindex, cindex);
		}
	}

	if (source.model->rowCount() < target.model->rowCount())
	{
		int nNum=target.model->rowCount()-source.model->rowCount();
		fill(nNum, source.model, tar);
	}
	else if (source.model->rowCount() > target.model->rowCount())
	{
		int nNum=source.model->rowCount()-target.model->rowCount();
		fill(nNum, target.model, src);
	}
}

bool QDbioDefFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QDbioDefFileCompare::DBIO_DEF_ENTRY> lcs;
	QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> src=qVariantValue<QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> >(source.data);
	QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> tar=qVariantValue<QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> >(target.data);

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

int QDbioDefFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	return NoError;
}

QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY> QDbioDefFileCompare::parseIOGroupDefFile(QTextStream& stream)
{
	QVector<DBIO_DEF_ENTRY> records;

	QRegExp rx("^STATE\\d+$");

	int nNumStn=-1;
	int nStnValid=0;
	int nNumGroup=-1;
	int nGroupValid=0;
	int nNumIOGroup=-1;
	int nIOGroupValid=0;
	while (!stream.atEnd())
	{
		QString strText=stream.readLine();

		QVector<QString> labels=parseString(strText);
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

				records.append(DBIO_DEF_ENTRY());
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

				nIOGroupValid=0;
				nNumIOGroup=-1;

				records[nNumStn].groups.append(DBIO_GROUP_DEF_ENTRY());
				records[nNumStn].groups[nNumGroup].grpName=labels[2];
			}

			continue;
		}

		if (labels.size() == 2 && labels[0] == "IOGROUP_ID")
		{
			nIOGroupValid=1;

			bool bOk;
			int nID=labels[1].toInt(&bOk);
			if (!bOk)
			{
				nIOGroupValid=0;
			}

			if (nIOGroupValid == 0)
			{
				continue;
			}

			for(int i=0; i<records[nNumStn].groups[nNumGroup].iogrps.size(); i++)
			{
				if(records[nNumStn].groups[nNumGroup].iogrps[i].id == nID)
				{
					nGroupValid=0;
					break;
				}
			}

			if (nGroupValid == 1)
			{
				nNumIOGroup++;

				records[nNumStn].groups[nNumGroup].iogrps.append(DBIO_IOGROUP_DEF_ENTRY());
				records[nNumStn].groups[nNumGroup].iogrps[nNumIOGroup].id=nID;
			}

			continue;
		}

		if (nIOGroupValid == 0)
		{
			continue;
		}

		if (labels.size() >= 1)
		{
			if (rx.indexIn(labels[0]) != -1)
			{
				int nStatusValid=1;
				for (int i=0; i<records[nNumStn].groups[nNumGroup].iogrps[nNumIOGroup].status.size(); i++)
				{
					if (records[nNumStn].groups[nNumGroup].iogrps[nNumIOGroup].status[i].name == labels[0])
					{
						nStatusValid=0;
						break;
					}
				}

				if (nStatusValid == 1)
				{
					DBIO_IOGROUP_STATUS_ENTRY status;
					status.name=labels[0];

					if (labels.size() == 2)
					{
						status.status=labels[1];
					}

					records[nNumStn].groups[nNumGroup].iogrps[nNumIOGroup].status<<status;
				}
			}
			else if (labels[0] ==  "IOGROUP_COMMENT")
			{
				if (labels.size() == 2)
				{
					records[nNumStn].groups[nNumGroup].iogrps[nNumIOGroup].comment=labels[1];
				}
				continue;
			}
			else if (labels[0] ==  "IOGROUP_EXCOMMENT")
			{
				if (labels.size() == 2)
				{
					records[nNumStn].groups[nNumGroup].iogrps[nNumIOGroup].exComment=labels[1];
				}
				continue;
			}
		}
	}

	return records;
}

bool operator==(const QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY& source, const QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY& target)
{
	return source.name==target.name;
}

bool operator==(const QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY& source, const QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY& target)
{
	return source.id==target.id;
}

bool operator==(const QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY& source, const QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY& target)
{
	return source.grpName==target.grpName;
}

bool operator==(const QDbioDefFileCompare::DBIO_DEF_ENTRY& source, const QDbioDefFileCompare::DBIO_DEF_ENTRY& target)
{
	return source.stnName==target.stnName;
}

REGISTER_FC_INSTANCE(QDbioDefFileCompare) 
