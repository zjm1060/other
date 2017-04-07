#include "qmsgdeffilecompare.h"

QMsgDefFileCompare::QMsgDefFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QMsgDefFileCompare::~QMsgDefFileCompare()
{

}

bool QMsgDefFileCompare::match(const QString& filename) const
{
	if (filename.endsWith(MSG_DEF_FILE))
	{
		return true;
	}

	return false;
}

void QMsgDefFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QTextStream stream(&file);
	QVector<MSG_DEF_ENTRY> msgs=parseMsgDefFile(stream);
	result.data=qVariantFromValue(msgs);

	result.titles<<tr("Proc Type")<<tr("Group Name")<<tr("Status")<<tr("Meaning");
}

void QMsgDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		  const QVector<QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		int nRow=target->rowCount(tindex);
		target->insertRow(nRow, tindex);

		QModelIndex pindex=target->index(nRow, GroupNameTitle, tindex);
		target->setData(pindex, data[curIntexTarget].grpName, Qt::DisplayRole);
		target->setData(pindex, CommonItemRole, ItemRole);
		target->setData(pindex, append? AppendRowRole:DeleteRowRole, RowRole);

		QModelIndex cindex;
		if (curIntexTarget != endIndexTarget && endIndexTarget != data.size())
		{
			nRow=source->rowCount(sindex);
			source->insertRow(nRow, sindex);

			cindex=source->index(nRow, GroupNameTitle, sindex);
			source->setData(cindex, BlankRowRole, RowRole);
		}

		for (int i=0; i<data[curIntexTarget].states.size(); i++)
		{
			const MSG_STATE_DEF_ENTRY& state=data[curIntexTarget].states[i];

			nRow=target->rowCount(pindex);
			target->insertRow(nRow, pindex);

			QModelIndex index=target->index(nRow, StatusTitle, pindex);
			target->setData(index, state.index, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			index=target->index(nRow, MeaningTitle, pindex);
			target->setData(index, state.def, Qt::DisplayRole);
			target->setData(index, CommonItemRole, ItemRole);

			if (curIntexTarget != endIndexTarget && endIndexTarget != data.size())
			{
				nRow=source->rowCount(cindex);
				source->insertRow(nRow, cindex);

				index=source->index(nRow, StatusTitle, cindex);
				source->setData(index, BlankRowRole, RowRole);
			}
		}
	}
}

void QMsgDefFileCompare::fill(int num, QAbstractItemModel* model, const QVector<QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY>& data, QModelIndex pindex)
{
	while (num-- > 0)
	{
		int nRow=model->rowCount(pindex);
		model->insertRow(nRow, pindex);

		QModelIndex cindex=model->index(nRow, GroupNameTitle, pindex);
		model->setData(cindex, BlankRowRole, RowRole);

		const QVector<MSG_STATE_DEF_ENTRY>& states=data[data.size()-1-num].states;
		for (int i=0; i<states.size(); i++)
		{
			nRow=model->rowCount(cindex);
			model->insertRow(nRow, cindex);

			QModelIndex index=model->index(nRow, StatusTitle, cindex);
			model->setData(index, BlankRowRole, RowRole);
		}
	}
}

void QMsgDefFileCompare::compareGroups(const QVector<MSG_GROUP_DEF_ENTRY>& sdata, const QVector<MSG_GROUP_DEF_ENTRY>& tdata, 
									   QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY> lcs;

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

			QModelIndex pindex=setData(source, GroupNameTitle, sdata[peer.first].grpName, sindex);
			QModelIndex cindex=setData(target, GroupNameTitle, tdata[peer.second].grpName, tindex);

			compareStates(sdata[peer.first].states, tdata[peer.second].states, source, target, pindex, cindex);
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

void QMsgDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		  const QVector<QMsgDefFileCompare::MSG_STATE_DEF_ENTRY>& data, const QModelIndex& parent, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=(curIntexTarget != endIndexTarget && curIntexTarget != data.size());

		setData(target, data[curIntexTarget], parent, append);

		if (bBlank)
		{
			int nRow=source->rowCount(parent);
			source->insertRow(nRow, parent);

			QModelIndex index=source->index(nRow, 0, parent);
			source->setData(index, BlankRowRole, RowRole);
		}
	}
}

void QMsgDefFileCompare::fill(int num, QAbstractItemModel* model, const QModelIndex& parent)
{
	while (num-- > 0)
	{
		int nRow=model->rowCount(parent);
		model->insertRow(nRow, parent);

		QModelIndex index=model->index(nRow, 0, parent);
		model->setData(index, BlankRowRole, RowRole);
	}
}

int QMsgDefFileCompare::setData(QAbstractItemModel* model, const QMsgDefFileCompare::MSG_STATE_DEF_ENTRY& data, const QModelIndex& parent, int rowRole)
{
	int nRow=model->rowCount(parent);
	model->insertRow(nRow, parent);

	QModelIndex index=model->index(nRow, 0, parent);
	model->setData(index, CommonItemRole, ItemRole);
	model->setData(index, rowRole, RowRole);

	setItemData(model, nRow, StatusTitle, parent, data.index, CommonItemRole);
	setItemData(model, nRow, MeaningTitle, parent, data.def, CommonItemRole);

	return nRow;
}

void QMsgDefFileCompare::setItemData(QAbstractItemModel* model, int row, int column, const QModelIndex& parent, const QVariant& var, int itemRole)
{
	QModelIndex index=model->index(row, column, parent);
	model->setData(index, var, Qt::DisplayRole);
	model->setData(index, itemRole, ItemRole);
}

bool QMsgDefFileCompare::compareDiff(QAbstractItemModel* source, int srow, const QModelIndex& sparent, 
									 QAbstractItemModel* target, int trow, const QModelIndex& tparent)
{
	bool bDiff=false;

	Q_ASSERT_NO_NULL(source);
	Q_ASSERT_NO_NULL(target);

	for (int i=0; i<TitleTypeEnd; i++)
	{
		QVariant vs=source->data(source->index(srow, i, sparent));
		QVariant vt=target->data(target->index(trow, i, tparent));

		if (vs != vt)
		{
			bDiff=true;

			source->setData(source->index(srow, i, sparent), DeleteItemRole, ItemRole);
			target->setData(target->index(trow, i, tparent), ModifyItemRole, ItemRole);
		}
	}

	return bDiff;
}

void QMsgDefFileCompare::compareStates(const QVector<MSG_STATE_DEF_ENTRY>& sdata, const QVector<MSG_STATE_DEF_ENTRY>& tdata,
										QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex)
{
	if (target == NULL || source == NULL)
	{
		return;
	}

	QLongestCommonSubsequenceV2<MSG_STATE_DEF_ENTRY> lcs;

	QVector<QPair<int, int> > results=lcs.parse(sdata, tdata);
	results.append(QPair<int, int>(sdata.size(), tdata.size()));

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(source, target, nIndexTarget, peer.second, tdata, tindex, AppendRowRole);

		//source
		diff(target, source, nIndexSource, peer.first, sdata, sindex, DeleteRowRole);

		if (nIndexTarget<tdata.size() && nIndexSource<sdata.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int srow=setData(source, sdata[peer.first], sindex, CommonRowRole);
			int trow=setData(target, tdata[peer.second], tindex, CommonRowRole);

			if (compareDiff(source, srow, sindex, target, trow, tindex))
			{
				source->setData(source->index(srow, 0, sindex), ModifyRowRole, RowRole);
				target->setData(target->index(trow, 0, tindex), ModifyRowRole, RowRole);
			}
		}
	}

	if (source->rowCount(sindex) < target->rowCount(tindex))
	{
		int nNum=target->rowCount(tindex)-source->rowCount(sindex);
		fill(nNum, source, sindex);
	}
	else if (source->rowCount(sindex) > target->rowCount(tindex))
	{
		int nNum=source->rowCount(sindex)-target->rowCount(tindex);
		fill(nNum, target, tindex);
	}
}

void QMsgDefFileCompare::diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,
							  int endIndexTarget, const QVector<QMsgDefFileCompare::MSG_DEF_ENTRY>& data, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		int nRow=target->rowCount();
		target->insertRow(nRow);

		QModelIndex ppindex=target->index(nRow, TypeTitle);
		target->setData(ppindex, data[curIntexTarget].typeName, Qt::DisplayRole);
		target->setData(ppindex, CommonItemRole, ItemRole);
		target->setData(ppindex, append? AppendRowRole:DeleteRowRole, RowRole);

		QModelIndex ccindex;
		if (curIntexTarget != endIndexTarget && endIndexTarget != data.size())
		{
			nRow=source->rowCount();
			source->insertRow(nRow);

			ccindex=source->index(nRow, TypeTitle);
			source->setData(ccindex, BlankRowRole, RowRole);
		}

		for (int i=0; i<data[curIntexTarget].groups.size(); i++)
		{
			const MSG_GROUP_DEF_ENTRY& group=data[curIntexTarget].groups[i];

			nRow=target->rowCount(ppindex);
			target->insertRow(nRow, ppindex);

			QModelIndex pindex=target->index(nRow, GroupNameTitle, ppindex);
			target->setData(pindex, group.grpName, Qt::DisplayRole);
			target->setData(pindex, CommonItemRole, ItemRole);

			QModelIndex cindex;
			if (curIntexTarget != endIndexTarget && endIndexTarget != data.size())
			{
				nRow=source->rowCount(ccindex);
				source->insertRow(nRow, ccindex);

				cindex=source->index(nRow, GroupNameTitle, ccindex);
				source->setData(cindex, BlankRowRole, RowRole);
			}

			for (int j=0; j<group.states.size(); j++)
			{
				nRow=target->rowCount(pindex);
				target->insertRow(nRow, pindex);

				QModelIndex index=target->index(nRow, StatusTitle, pindex);
				target->setData(index, group.states[j].index, Qt::DisplayRole);
				target->setData(index, CommonItemRole, ItemRole);

				index=target->index(nRow, MeaningTitle, pindex);
				target->setData(index, group.states[j].def, Qt::DisplayRole);

				if (curIntexTarget != endIndexTarget && endIndexTarget != data.size())
				{
					nRow=source->rowCount(cindex);
					source->insertRow(nRow, cindex);

					index=source->index(nRow, StatusTitle, cindex);
					source->setData(index, BlankRowRole, RowRole);
					source->setData(index, CommonItemRole, ItemRole);

					index=source->index(nRow, MeaningTitle, cindex);
					source->setData(index, BlankRowRole, RowRole);
				}
			}
		}
	}
}

QModelIndex QMsgDefFileCompare::setData(QAbstractItemModel* model, int column, const QVariant& data, const QModelIndex& parent)
{
	int nRow=model->rowCount(parent);
	model->insertRow(nRow, parent);

	QModelIndex pindex=model->index(nRow, column, parent);
	model->setData(pindex, data, Qt::DisplayRole);
	model->setData(pindex, CommonItemRole, ItemRole);
	model->setData(pindex, CommonRowRole, RowRole);

	return pindex;
}

bool QMsgDefFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QMsgDefFileCompare::MSG_DEF_ENTRY> lcs;
	QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> src=qVariantValue<QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> >(source.data);
	QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> tar=qVariantValue<QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> >(target.data);

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

void QMsgDefFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QMsgDefFileCompare::MSG_DEF_ENTRY> lcs;
	QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> src=qVariantValue<QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> >(source.data);
	QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> tar=qVariantValue<QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> >(target.data);

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

			QModelIndex pindex=setData(source.model, TypeTitle, src[peer.first].typeName);
			QModelIndex cindex=setData(target.model, TypeTitle, tar[peer.second].typeName);

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

void QMsgDefFileCompare::fill(int num, QAbstractItemModel* model, const QVector<QMsgDefFileCompare::MSG_DEF_ENTRY>& data)
{
	while (num-- > 0)
	{
		int nRow=model->rowCount();
		model->insertRow(nRow);

		QModelIndex ppindex=model->index(nRow, TypeTitle);
		model->setData(ppindex, BlankRowRole, RowRole);

		const QVector<MSG_GROUP_DEF_ENTRY>& groups=data[data.size()-1-num].groups;
		for (int i=0; i<groups.size(); i++)
		{
			nRow=model->rowCount(ppindex);
			model->insertRow(nRow, ppindex);

			QModelIndex pindex=model->index(nRow, GroupNameTitle, ppindex);
			model->setData(pindex, BlankRowRole, RowRole);
			for (int j=0; j<groups[i].states.size(); j++)
			{
				nRow=model->rowCount(pindex);
				model->insertRow(nRow, pindex);

				QModelIndex index=model->index(nRow, StatusTitle, pindex);
				model->setData(index, BlankRowRole, RowRole);
			}
		}
	}
}

int QMsgDefFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	return NoError;
}

QVector<QMsgDefFileCompare::MSG_DEF_ENTRY> QMsgDefFileCompare::parseMsgDefFile(QTextStream& stream)
{
	QVector<MSG_DEF_ENTRY> msgs;

	int nNumProc=-1;
	int nProcValid=0;
	int nNumGroup=-1;
	int nGroupValid=0;
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

		if (labels.size() == 3 && labels[0] == "PROCTYPE" && labels[1] == "NAME")
		{
			nProcValid=1;

			for(int i=0; i<msgs.size(); i++)
			{
				if(msgs[i].typeName == labels[2])
				{
					nProcValid=0;
					break;
				}
			}

			if (nProcValid == 1)
			{
				nNumProc++;

				nGroupValid=0;
				nNumGroup=-1;

				msgs.append(MSG_DEF_ENTRY());
				msgs[nNumProc].typeName=labels[2];
			}

			continue;
		}

		if (nProcValid == 0)
		{
			continue;
		}

		if (labels.size() == 3 && labels[0] == "GROUP" && labels[1] == "NAME")
		{
			nGroupValid=1;

			for(int i=0; i<msgs[nNumProc].groups.size(); i++)
			{
				if(msgs[nNumProc].groups[i].grpName == labels[2])
				{
					nGroupValid=0;
					break;
				}
			}

			if (nGroupValid == 1)
			{
				nNumGroup++;

				msgs[nNumProc].groups.append(MSG_GROUP_DEF_ENTRY());
				msgs[nNumProc].groups[nNumGroup].grpName=labels[2];
			}

			continue;
		}

		if (nGroupValid == 0)
		{
			continue;
		}

		if (labels.size() >= 2)
		{
			if (labels[0] ==  "STATE")
			{
				MSG_STATE_DEF_ENTRY state;
				bool bOk;
				state.index=labels[1].toInt(&bOk);
				if (bOk)
				{
					if (labels.size() == 3)
					{
						state.def=labels[2];
					}

					msgs[nNumProc].groups[nNumGroup].states<<state;
				}

				continue;
			}
		}
	}

	return msgs;
}

bool operator==(const QMsgDefFileCompare::MSG_STATE_DEF_ENTRY& source, const QMsgDefFileCompare::MSG_STATE_DEF_ENTRY& target)
{
	return source.index==target.index;
}

bool operator==(const QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY& source, const QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY& target)
{
	return source.grpName==target.grpName;
}

bool operator==(const QMsgDefFileCompare::MSG_DEF_ENTRY& source, const QMsgDefFileCompare::MSG_DEF_ENTRY& target)
{
	return source.typeName==target.typeName;
}

REGISTER_FC_INSTANCE(QMsgDefFileCompare) 
