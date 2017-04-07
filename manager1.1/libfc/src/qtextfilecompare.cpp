#include "qtextfilecompare.h"

#include <QStandardItemModel>

#include "common.h"

QTextFileCompare::QTextFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QTextFileCompare::~QTextFileCompare()
{

}

bool QTextFileCompare::match(const QString& filename) const
{
	return isTextFormat(filename);
}

void QTextFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QVector<QString> lines;

	QTextStream stream(&file);
	while (!stream.atEnd())
	{
		QString line = stream.readLine();
		
		lines.append(line);
	}

	result.data=qVariantFromValue(lines);

	result.titles<<"";
}

void QTextFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
							const QVector<QString>& data, bool append/* =true */)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=(curIntexTarget != endIndexTarget && curIntexTarget != data.size());

		setRowData(target, data[curIntexTarget], append);

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

int QTextFileCompare::setRowData(QAbstractItemModel* model, const QString& data, int rowRole)
{
	int nRow=model->rowCount();
	model->insertRow(nRow);

	QModelIndex index=model->index(nRow, LineTitle);
	((QStandardItemModel*)model)->itemFromIndex(index)->setEditable(false);
	model->setData(index, data);
	model->setData(index, rowRole, RowRole);
	model->setData(index, CommonItemRole, ItemRole);

	return nRow;
}

bool QTextFileCompare::compareDiff(QAbstractItemModel* source, int srow, QAbstractItemModel* target, int trow)
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

void QTextFileCompare::fill(int num, QAbstractItemModel* model)
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

void QTextFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QString> lcs;
	QVector<QString> src=qVariantValue<QVector<QString> >(source.data);
	QVector<QString> tar=qVariantValue<QVector<QString> >(target.data);

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

		while (nIndexSource<peer.first && nIndexTarget<peer.second)
		{
			int srow=setRowData(source.model, src[nIndexSource], ModifyRowRole);
			int trow=setRowData(target.model, tar[nIndexTarget], ModifyRowRole);

			for (int i=0; i<TitleTypeEnd; i++)
			{
				source.model->setData(source.model->index(srow, i), DeleteItemRole, ItemRole);
				target.model->setData(target.model->index(trow, i), ModifyItemRole, ItemRole);
			}

			nIndexSource++;
			nIndexTarget++;
		}

		//target
		diff(source.model, target.model, nIndexTarget, peer.second, tar, AppendRowRole);

		//source
		diff(target.model, source.model, nIndexSource, peer.first, src, DeleteRowRole);

		if (nIndexTarget<tar.size() && nIndexSource<src.size())
		{
			setRowData(source.model, src[peer.first], CommonRowRole);
			setRowData(target.model, tar[peer.second], CommonRowRole);

			nIndexTarget++;
			nIndexSource++;
		}
	}

// 	if (source.model->rowCount() < target.model->rowCount())
// 	{
// 		int nNum=target.model->rowCount()-source.model->rowCount();
// 		fill(nNum, source.model);
// 	}
// 	else if (source.model->rowCount() > target.model->rowCount())
// 	{
// 		int nNum=source.model->rowCount()-target.model->rowCount();
// 		fill(nNum, target.model);
// 	}
}

bool QTextFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QString> lcs;
	QVector<QString> src=qVariantValue<QVector<QString> >(source.data);
	QVector<QString> tar=qVariantValue<QVector<QString> >(target.data);

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

int QTextFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	QTextStream stream(device);

	QAbstractItemModel* pModel=dynamic_cast<QAbstractItemModel*>(result.model);
	if (pModel != NULL)
	{
		int nNumRows=pModel->rowCount();
		for (int nRow=0; nRow<nNumRows; nRow++)
		{
			QModelIndex index=pModel->index(nRow, LineTitle);
			int nRowRole=pModel->data(index, RowRole).toInt();
			if (nRowRole == DeleteRowRole)
			{
				continue;
			}

			QVariant var=pModel->data(index, AssignRowRole);
			if ((!var.isValid() && nRowRole == BlankRowRole) || (var.isValid() && var.toInt() == BlankRowRole))
			{
				continue;
			}

			int nItemRole=pModel->data(index, ItemRole).toInt();
			if (nItemRole == DeleteItemRole)
			{
				continue;
			}

			var=pModel->data(index, Qt::EditRole);
			if (var.isValid())
			{
				QString strText=var.toString();
				stream<<strText;
			}

			stream<<"\n";
		}
	}

	return NoError;
}

bool QTextFileCompare::isHeaderHidden() const
{
	return true;
}

REGISTER_FC_INSTANCE(QTextFileCompare) 
