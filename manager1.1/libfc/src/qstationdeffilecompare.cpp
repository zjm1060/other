#include "qstationdeffilecompare.h"

#include <QStandardItemModel>

QStationDefFileCompare::QStationDefFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QStationDefFileCompare::~QStationDefFileCompare()
{

}

bool QStationDefFileCompare::match(const QString& filename) const
{
	if (filename.endsWith(STATION_DEF_FILE))
	{
		return true;
	}

	return false;
}

void QStationDefFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QTextStream stream(&file);
	QVector<STATION_ENTRY> stations=parseStationDefFile(stream);
	result.data=qVariantFromValue(stations);

	result.titles<<tr("Station ID")<<tr("Station Name")<<tr("Maximum")<<tr("Long Name")<<tr("Comment");
}

void QStationDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		  const QVector<STATION_ENTRY>& data, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=(curIntexTarget != endIndexTarget && curIntexTarget != data.size());

		setData(target, data[curIntexTarget], append);

		if (bBlank)
		{
			int nRow=source->rowCount();
			source->insertRow(nRow);

			QModelIndex index=source->index(nRow, 0);
			source->setData(index, BlankRowRole, RowRole);

			for (int i=0; i<source->columnCount(); i++)
			{
				index=source->index(nRow, i);
				((QStandardItemModel*)source)->itemFromIndex(index)->setEditable(false);
			}
		}
	}
}

int QStationDefFileCompare::setData(QAbstractItemModel* model, const STATION_ENTRY& data, int rowRole)
{
	int nRow=model->rowCount();
	model->insertRow(nRow);

	QModelIndex index=model->index(nRow, StationIDTitle);
	((QStandardItemModel*)model)->itemFromIndex(index)->setEditable(false);
	model->setData(index, data.stnId);
	model->setData(index, CommonItemRole, ItemRole);
	model->setData(index, rowRole, RowRole);

	setItemData(model, nRow, StationNameTitle, data.stnName, CommonItemRole);
	setItemData(model, nRow, NumOfGroupsTitle, data.grp_num, CommonItemRole);
	setItemData(model, nRow, LongNameTitle, data.stnLongname, CommonItemRole);
	setItemData(model, nRow, CommentTitle, data.stnComment, CommonItemRole);

	return nRow;
}

void QStationDefFileCompare::setItemData(QAbstractItemModel* model, int row, int column, const QVariant& var, int itemRole)
{
	QModelIndex index=model->index(row, column);
	((QStandardItemModel*)model)->itemFromIndex(index)->setEditable(false);
	model->setData(index, var);
	model->setData(index, itemRole, ItemRole);
}

bool QStationDefFileCompare::compareDiff(QAbstractItemModel* source, int srow, QAbstractItemModel* target, int trow)
{
	bool bDiff=false;

	Q_ASSERT_NO_NULL(source);
	Q_ASSERT_NO_NULL(target);

	for (int i=0; i<TitleTypeEnd; i++)
	{
		QVariant vs=source->data(source->index(srow, i));
		QVariant vt=target->data(target->index(trow, i));

		if (vs != vt)
		{
			bDiff=true;

			source->setData(source->index(srow, i), DeleteItemRole, ItemRole);
			target->setData(target->index(trow, i), ModifyItemRole, ItemRole);
		}
	}

	return bDiff;
}

void QStationDefFileCompare::fill(int num, QAbstractItemModel* model)
{
	while (num-- > 0)
	{
		int nRow=model->rowCount();
		model->insertRow(nRow);

		QModelIndex index=model->index(nRow, 0);
		model->setData(index, BlankRowRole, RowRole);

		for (int i=0; i<model->columnCount(); i++)
		{
			index=model->index(nRow, i);
			((QStandardItemModel*)model)->itemFromIndex(index)->setEditable(false);
		}
	}
}

void QStationDefFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<STATION_ENTRY> lcs;
	QVector<STATION_ENTRY> src=qVariantValue<QVector<STATION_ENTRY> >(source.data);
	QVector<STATION_ENTRY> tar=qVariantValue<QVector<STATION_ENTRY> >(target.data);

	QVector<QPair<int, int> > results=lcs.parse(src, tar);
	results.append(QPair<int, int>(src.size(), tar.size()));

 	source.model=new QStandardItemModel(0, source.titles.size());
 	((QStandardItemModel*)source.model)->setHorizontalHeaderLabels(source.titles);

	target.model=new QStandardItemModel(0, target.titles.size());
	((QStandardItemModel*)target.model)->setHorizontalHeaderLabels(target.titles);
 
	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(source.model, target.model, nIndexTarget, peer.second, tar, AppendRowRole);

		//source
		diff(target.model, source.model, nIndexSource, peer.first, src, DeleteRowRole);

		if (nIndexTarget<tar.size() && nIndexSource<src.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int srow=setData(source.model, src[peer.first], CommonRowRole);
			int trow=setData(target.model, tar[peer.second], CommonRowRole);

			if (compareDiff(source.model, srow, target.model, trow))
			{
				source.model->setData(source.model->index(srow, 0), ModifyRowRole, RowRole);
				target.model->setData(target.model->index(trow, 0), ModifyRowRole, RowRole);
			}
		}
	}

	if (source.model->rowCount() < target.model->rowCount())
	{
		int nNum=target.model->rowCount()-source.model->rowCount();
		fill(nNum, source.model);
	}
	else if (source.model->rowCount() > target.model->rowCount())
	{
		int nNum=source.model->rowCount()-target.model->rowCount();
		fill(nNum, target.model);
	}
}

bool QStationDefFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<STATION_ENTRY> lcs;
	QVector<STATION_ENTRY> src=qVariantValue<QVector<STATION_ENTRY> >(source.data);
	QVector<STATION_ENTRY> tar=qVariantValue<QVector<STATION_ENTRY> >(target.data);

	if (src.size() != tar.size())
	{
		return true;
	}

	QVector<QPair<int, int> > results=lcs.parse(src, tar);
	if (results.size() != src.size())
	{
		return true;
	}

	results.append(QPair<int, int>(src.size(), tar.size()));

	source.model=new QStandardItemModel(0, source.titles.size());
	((QStandardItemModel*)source.model)->setHorizontalHeaderLabels(source.titles);

	target.model=new QStandardItemModel(0, target.titles.size());
	((QStandardItemModel*)target.model)->setHorizontalHeaderLabels(target.titles);

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(source.model, target.model, nIndexTarget, peer.second, tar, AppendRowRole);

		//source
		diff(target.model, source.model, nIndexSource, peer.first, src, DeleteRowRole);

		if (nIndexTarget<tar.size() && nIndexSource<src.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int srow=setData(source.model, src[peer.first], CommonRowRole);
			int trow=setData(target.model, tar[peer.second], CommonRowRole);

			if (compareDiff(source.model, srow, target.model, trow))
			{
				return true;
			}
		}
	}

	return false;
}

int QStationDefFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	return NoError;
}

bool operator==(const STATION_ENTRY& source, const STATION_ENTRY& target)
{
	return source.stnId==target.stnId;
}

REGISTER_FC_INSTANCE(QStationDefFileCompare) 
