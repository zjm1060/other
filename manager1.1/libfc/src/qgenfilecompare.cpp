#include "qgenfilecompare.h"

#include <QStandardItemModel>

#include "common.h"
#include "qutilreadwritexmlfile.h"

#define	MAX_DATATYPE_NUM	16
enum DATA_TYPE
{
	ANA_TYPE=1, IMP_TYPE=3, SOE_TYPE=4, POL_TYPE=5, 
	SYSMSG_TYPE=6, OUTA_TYPE=7, OUTS_TYPE=8, 
	CALC_TYPE=9, CURVE_TYPE=10, PARAM_TYPE=12, 
	CHARA_TYPE=13, DEVOBJ_TYPE=14, TIME_TYPE=15
};

static	char *recordname[] = 
{
	(char *)"",      (char *)"ANA",   (char *)"",     (char *)"IMP",  (char *)"INT", 
	(char *)"POL",   (char *)"SYS",(char *)"OUA", (char *)"OUS", (char *)"CAL", 
	(char *)"CUR", (char *)"", (char *)"PAM",     (char *)"CHA", (char *)"OBJ",
	(char *)"TME"
};

QGenFileCompare::QGenFileCompare(QObject *parent)
	: QObject(parent)
{
	
}

QGenFileCompare::~QGenFileCompare()
{

}

bool QGenFileCompare::match(const QString& filename) const
{
	bool bMatch=false;

	QFile file(filename);
	if (file.exists() && file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		QTextStream stream(&file);
		while (!stream.atEnd())
		{
			QString strText=stream.readLine().trimmed();
			if (strText.isEmpty())
			{
				continue;
			}

			QVector<QString> labels=parseString(strText);
			if (labels.isEmpty())
			{
				break;
			}

			if (labels.size() == 3 && labels[0] == "RECORD" && labels[1] == "NAME")
			{
				if (recordTypeFromName(labels[2]) != -1)
				{
					bMatch=true;
				}
			}

			break;
		}

		file.close();
	}
	
	return bMatch;
}

void QGenFileCompare::parseFile(QFile& file, FileCompareResult& result)
{
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		result.errcode=FileOpenError;
		return;
	}

	QTextStream stream(&file);
	RECORD_DEF_ENTRY record=parseEntryFile(stream);
	file.close();

	result.data=qVariantFromValue(record);

	result.titles<<tr("Logical Name")<<tr("Long Name");
	switch (recordTypeFromName(record.recName))
	{
	case ANA_TYPE:
	case OUTA_TYPE:
	case PARAM_TYPE:
	case CALC_TYPE:
	case CHARA_TYPE:
		result.titles<<tr("Upper Limit")<<tr("High Limit")<<tr("HR_Limit")<<tr("LR_Limit")<<tr("Low Limit")<<tr("Lowest Limit")
			<<tr("Dead Band")<<tr("HDB Band")<<tr("Conversion Factor")<<tr("High Range")<<tr("Low Range")<<tr("High Code")<<tr("Low Code")
			<<tr("Trend Limit")<<tr("RT_Limit")<<tr("DSP File")<<tr("Fault Point")<<tr("Fail Point")<<tr("Audio Alarm")<<tr("Trend Alarm")
			<<tr("Cascade Point")<<tr("Control Point")<<tr("Control Address")<<tr("Zero")<<tr("Compensate")<<tr("Alarm CBA")<<tr("Virtual Point")
			<<tr("LCU Virtual")<<tr("Command Group")<<tr("Unit")<<tr("Pair Type")<<tr("Pair Point")<<tr("HDB Record")<<tr("HDB Period")<<tr("HDB Curve")<<tr("IO Group")
			<<tr("Terminal No")<<tr("Cabinet No")<<tr("Unit No")<<tr("Slot No")<<tr("Channel No")<<tr("HDB Condition")<<tr("Constraint")
			<<tr("Constraint Group")<<tr("ID");
		break;
	case IMP_TYPE:
		result.titles<<tr("Proctype")<<tr("Conversion Factor")<<tr("PT")<<tr("CT")<<tr("Day Rate")<<tr("Year Rate")<<tr("Constant")<<tr("DSP File")
			<<tr("Cascade Point")<<tr("HDB Record")<<tr("HDB Period")<<tr("IO Group")<<tr("Terminal No")<<tr("Cabinet No")<<tr("Unit No")
			<<tr("Slot No")<<tr("Channel No")<<tr("ID");
		break;
	case SOE_TYPE:
	case POL_TYPE:
	case OUTS_TYPE:
	case SYSMSG_TYPE:
	case DEVOBJ_TYPE:
		result.titles<<tr("Proctype")<<tr("Message Group")<<tr("Command Group")<<tr("Audio Alarm")<<tr("Statistics")<<tr("Fail Record")
			<<tr("Fault Point")<<tr("Fail Point")<<tr("Key Point")<<tr("DSP File")<<tr("Control Point")<<tr("Control Address")<<tr("SOE")
			<<tr("Alarm CBA")<<tr("Normal State")<<tr("ON Alarm")<<tr("OFF Alarm")<<tr("Virtual Point")<<tr("LCU Virtual")<<tr("Cascade Point")
			<<tr("Triple Point")<<tr("Normal Close")<<tr("Pair Type")<<tr("Pair Point")<<tr("HDB Record")<<tr("IO Group")<<tr("Terminal No")
			<<tr("Cabinet No")<<tr("Unit No")<<tr("Slot No")<<tr("Channel No")<<tr("ID");
		break;
	case CURVE_TYPE:
		result.titles<<tr("Points")<<tr("Conversion Factor")<<tr("High Range")<<tr("Low Range")<<tr("HDB Record")<<tr("ID");
		break;
	case TIME_TYPE:
		break;
	default:
		Q_ASSERT(false);
		break;
	}
}

void QGenFileCompare::setItemData(QAbstractItemModel* model, int row, int column, const QVariant& var, int itemRole)
{
	QModelIndex index=model->index(row, column);
	((QStandardItemModel*)model)->itemFromIndex(index)->setEditable(false);
	model->setData(index, var);
	model->setData(index, itemRole, ItemRole);
}

int QGenFileCompare::setData(int recordType, QAbstractItemModel* model, const QGenFileCompare::RECORD_ENTRY& data, int rowRole)
{
	int nRow=model->rowCount();
	model->insertRow(nRow);

	QModelIndex index=model->index(nRow, LogicalNameTitle);
	((QStandardItemModel*)model)->itemFromIndex(index)->setEditable(false);
	model->setData(index, data.fixed.entryName);
	model->setData(index, CommonItemRole, ItemRole);
	model->setData(index, rowRole, RowRole);

	setItemData(model, nRow, LongNameTitle, data.fixed.entryLongname, CommonItemRole);

	switch (recordType)
	{
	case ANA_TYPE:
	case OUTA_TYPE:
	case PARAM_TYPE:
	case CALC_TYPE:
	case CHARA_TYPE:
		{
			setItemData(model, nRow, HHLimitTitle, data.var.anlg.lmt_para.HI_ALARM, CommonItemRole);
			setItemData(model, nRow, HLimitTitle, data.var.anlg.lmt_para.HI_WARN, CommonItemRole);
			setItemData(model, nRow, HRLimitTitle, data.var.anlg.lmt_para.HI_RECOVER, CommonItemRole);
			setItemData(model, nRow, LRLimitTitle, data.var.anlg.lmt_para.LO_RECOVER, CommonItemRole);
			setItemData(model, nRow, LLimitTitle, data.var.anlg.lmt_para.LO_WARN, CommonItemRole);
			setItemData(model, nRow, LLLimitTitle, data.var.anlg.lmt_para.LO_ALARM, CommonItemRole);
			setItemData(model, nRow, DBandTitle, data.var.anlg.eng_para.D_BAND, CommonItemRole);
			setItemData(model, nRow, HDBBandTitle, data.var.anlg.eng_para.HDB_BAND, CommonItemRole);
			setItemData(model, nRow, ENGConvertTitle, data.var.anlg.eng_para.ENG_CVT, CommonItemRole);
			setItemData(model, nRow, HighRangeTitle, data.var.anlg.eng_para.HI_RANGE, CommonItemRole);
			setItemData(model, nRow, LowRangeTitle, data.var.anlg.eng_para.LO_RANGE, CommonItemRole);
			setItemData(model, nRow, HighCodeTitle, data.var.anlg.eng_para.HI_CODE, CommonItemRole);
			setItemData(model, nRow, LowCodeTitle, data.var.anlg.eng_para.LO_CODE, CommonItemRole);
			setItemData(model, nRow, TrendLimitTitle, data.var.anlg.lmt_para.TR_WARN, CommonItemRole);
			setItemData(model, nRow, RTLimitTitle, data.var.anlg.lmt_para.TR_RECOVER, CommonItemRole);
			setItemData(model, nRow, DSPFileTitle, data.fixed.dsp_filename, CommonItemRole);
			setItemData(model, nRow, FaultPointTitle, data.fixed.ioattr.fault, CommonItemRole);
			setItemData(model, nRow, FailPointTitle, data.fixed.ioattr.fail, CommonItemRole);
			setItemData(model, nRow, AudioAlarmTitle, data.fixed.ioattr.audAlm, CommonItemRole);
			setItemData(model, nRow, TrendAlarmTitle, data.fixed.ioattr.trendAlm, CommonItemRole);
			setItemData(model, nRow, CascadePointTitle, data.fixed.ioattr.cascade, CommonItemRole);
			setItemData(model, nRow, ControlPointTitle, data.fixed.ioattr.ctrl, CommonItemRole);
			setItemData(model, nRow, ControlAddrTitle, data.fixed.ctrl_addr, CommonItemRole);
			setItemData(model, nRow, ZeroTitle, data.var.anlg.eng_para.ZERO, CommonItemRole);
			setItemData(model, nRow, CompensateTitle, data.var.anlg.eng_para.COMPSATE, CommonItemRole);
			setItemData(model, nRow, AlarmCBATitle, data.fixed.ioattr.almCba, CommonItemRole);
			setItemData(model, nRow, VirtualPointTitle, data.fixed.ioattr.sysVirtual, CommonItemRole);
			setItemData(model, nRow, LCUVirtualTitle, data.fixed.ioattr.lcuVirtual, CommonItemRole);
			setItemData(model, nRow, CMDMsgTitle, data.fixed.msg_sel.cmd, CommonItemRole);
			setItemData(model, nRow, UnitTitle, data.var.anlg.unitName, CommonItemRole);
			setItemData(model, nRow, PairTypeTitle, data.fixed.pairType, CommonItemRole);
			setItemData(model, nRow, PairPointTitle, data.fixed.pairName, CommonItemRole);
			setItemData(model, nRow, HDBRecordTitle, data.fixed.ioattr.hdbRec, CommonItemRole);
			setItemData(model, nRow, HDBPeriodTitle, data.fixed.ioattr.hdbPeriod, CommonItemRole);
			setItemData(model, nRow, HDBCurveTitle, data.fixed.ioattr.hdbCurve, CommonItemRole);
			setItemData(model, nRow, IOGroupTitle, data.fixed.io_group, CommonItemRole);
			setItemData(model, nRow, TerminalNoTitle, data.fixed.hard_addr.terminal_no, CommonItemRole);
			setItemData(model, nRow, CabinetNoTitle, data.fixed.hard_addr.cabinet_no, CommonItemRole);
			setItemData(model, nRow, UnitNoTitle, data.fixed.hard_addr.unit_no, CommonItemRole);
			setItemData(model, nRow, SlotNoTitle, data.fixed.hard_addr.slot_no, CommonItemRole);
			setItemData(model, nRow, ChannelNoTitle, data.fixed.hard_addr.channel_no, CommonItemRole);
			setItemData(model, nRow, HDBConditionTitle, data.var.anlg.eng_para.HDB_CONDITION, CommonItemRole);
			setItemData(model, nRow, ConstraintTitle, data.var.anlg.eng_para.CONSTRAINT, CommonItemRole);
			setItemData(model, nRow, ConstraintGroupIDTitle, data.var.anlg.eng_para.CONSTRAINTGROUP_ID, CommonItemRole);
			setItemData(model, nRow, IDTitle, data.fixed.id, CommonItemRole);
		}
		break;
	case IMP_TYPE:
		{
			setItemData(model, nRow, IMP_ProcTypeTitle, data.fixed.ioattr.procType, CommonItemRole);
			setItemData(model, nRow, IMP_ConvertTitle, data.var.imp.entry.eng_para.ENG_CVT, CommonItemRole);
			setItemData(model, nRow, IMP_PTTitle, data.var.imp.entry.eng_para.PT, CommonItemRole);
			setItemData(model, nRow, IMP_CTTitle, data.var.imp.entry.eng_para.CT, CommonItemRole);
			setItemData(model, nRow, IMP_DayRateTitle, data.var.imp.entry.eng_para.DAY_RATE, CommonItemRole);
			setItemData(model, nRow, IMP_YearRateTitle, data.var.imp.entry.eng_para.YEAR_RATE, CommonItemRole);
			setItemData(model, nRow, IMP_ConstantTitle, data.var.imp.entry.eng_para.CONSTANT, CommonItemRole);
			setItemData(model, nRow, IMP_DspFileTitle, data.fixed.dsp_filename, CommonItemRole);
			setItemData(model, nRow, IMP_CascadePointTitle, data.fixed.ioattr.cascade, CommonItemRole);
			setItemData(model, nRow, IMP_HDBRecordTitle, data.fixed.ioattr.hdbRec, CommonItemRole);
			setItemData(model, nRow, IMP_HDBPeriodTitle, data.fixed.ioattr.hdbPeriod, CommonItemRole);
			setItemData(model, nRow, IMP_IOGroupTitle, data.fixed.io_group, CommonItemRole);
			setItemData(model, nRow, IMP_TerminalNoTitle, data.fixed.hard_addr.terminal_no, CommonItemRole);
			setItemData(model, nRow, IMP_CabinetNoTitle, data.fixed.hard_addr.cabinet_no, CommonItemRole);
			setItemData(model, nRow, IMP_UnitNoTitle, data.fixed.hard_addr.unit_no, CommonItemRole);
			setItemData(model, nRow, IMP_SlotNoTitle, data.fixed.hard_addr.slot_no, CommonItemRole);
			setItemData(model, nRow, IMP_ChannelNoTitle, data.fixed.hard_addr.channel_no, CommonItemRole);
			setItemData(model, nRow, IMP_IDTitle, data.fixed.id, CommonItemRole);
		}
		break;
	case SOE_TYPE:
	case POL_TYPE:
	case OUTS_TYPE:
	case SYSMSG_TYPE:
	case DEVOBJ_TYPE:
		{
			setItemData(model, nRow, INT_ProcTypeTitle, data.fixed.ioattr.procType, CommonItemRole);
			setItemData(model, nRow, INT_MSGDefTitle, data.fixed.msg_sel.alm, CommonItemRole);
			setItemData(model, nRow, INT_CMDMsgTitle, data.fixed.msg_sel.cmd, CommonItemRole);
			setItemData(model, nRow, INT_AudioAlarmTitle, data.fixed.ioattr.audAlm, CommonItemRole);
			setItemData(model, nRow, INT_StatisticsTitle, data.fixed.ioattr.statistic, CommonItemRole);
			setItemData(model, nRow, INT_FailRecordTitle, data.fixed.ioattr.faultRec, CommonItemRole);
			setItemData(model, nRow, INT_FaultPointTitle, data.fixed.ioattr.fault, CommonItemRole);
			setItemData(model, nRow, INT_FailPointTitle, data.fixed.ioattr.fail, CommonItemRole);
			setItemData(model, nRow, INT_KeySignalTitle, data.fixed.ioattr.keySignal, CommonItemRole);
			setItemData(model, nRow, INT_DspFileTitle, data.fixed.dsp_filename, CommonItemRole);
			setItemData(model, nRow, INT_ControlPointTitle, data.fixed.ioattr.ctrl, CommonItemRole);
			setItemData(model, nRow, INT_ControlAddrTitle, data.fixed.ctrl_addr, CommonItemRole);
			setItemData(model, nRow, INT_SOE, data.fixed.ioattr.soe, CommonItemRole);
			setItemData(model, nRow, INT_AlarmCBATitle, data.fixed.ioattr.almCba, CommonItemRole);
			setItemData(model, nRow, INT_NormalStateTitle, data.fixed.ioattr.normalState, CommonItemRole);
			setItemData(model, nRow, INT_ONAlarm, data.fixed.ioattr.onAlm, CommonItemRole);
			setItemData(model, nRow, INT_OFFAlarm, data.fixed.ioattr.offAlm, CommonItemRole);
			setItemData(model, nRow, INT_VirtualPointTitle, data.fixed.ioattr.sysVirtual, CommonItemRole);
			setItemData(model, nRow, INT_LCUVirtualTitle, data.fixed.ioattr.lcuVirtual, CommonItemRole);
			setItemData(model, nRow, INT_CascadePointTitle, data.fixed.ioattr.cascade, CommonItemRole);
			setItemData(model, nRow, INT_TriplePointTitle, data.fixed.ioattr.doublePt, CommonItemRole);
			setItemData(model, nRow, INT_NormalCloseTitle, data.fixed.ioattr.normalClose, CommonItemRole);
			setItemData(model, nRow, INT_PairTypeTitle, data.fixed.pairType, CommonItemRole);
			setItemData(model, nRow, INT_PairPointTitle, data.fixed.pairName, CommonItemRole);
			setItemData(model, nRow, INT_HDBRecordTitle, data.fixed.ioattr.hdbRec, CommonItemRole);
			setItemData(model, nRow, INT_IOGroupTitle, data.fixed.io_group, CommonItemRole);
			setItemData(model, nRow, INT_TerminalNoTitle, data.fixed.hard_addr.terminal_no, CommonItemRole);
			setItemData(model, nRow, INT_CabinetNoTitle, data.fixed.hard_addr.cabinet_no, CommonItemRole);
			setItemData(model, nRow, INT_UnitNoTitle, data.fixed.hard_addr.unit_no, CommonItemRole);
			setItemData(model, nRow, INT_SlotNoTitle, data.fixed.hard_addr.slot_no, CommonItemRole);
			setItemData(model, nRow, INT_ChannelNoTitle, data.fixed.hard_addr.channel_no, CommonItemRole);
			setItemData(model, nRow, INT_IDTitle, data.fixed.id, CommonItemRole);
		}
		break;
	case CURVE_TYPE:
		{
			setItemData(model, nRow, CUR_NumPoints, data.var.curve.seg_num, CommonItemRole);
			setItemData(model, nRow, CUR_ENGConvertTitle, data.var.curve.eng_para.ENG_CVT, CommonItemRole);
			setItemData(model, nRow, CUR_HighRangeTitle, data.var.curve.eng_para.HI_RANGE, CommonItemRole);
			setItemData(model, nRow, CUR_LowRangeTitle, data.var.curve.eng_para.LO_RANGE, CommonItemRole);
			setItemData(model, nRow, CUR_HDBRecordTitle, data.fixed.ioattr.hdbRec, CommonItemRole);
			setItemData(model, nRow, CUR_IDTitle, data.fixed.id, CommonItemRole);
		}
		break;
	case TIME_TYPE:
		break;
	default:
		Q_ASSERT(false);
		break;
	}

	return nRow;
}

void QGenFileCompare::diff(int recordType, QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		  const QVector<QGenFileCompare::RECORD_ENTRY>& data, bool append)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	for (;curIntexTarget<endIndexTarget && curIntexTarget<data.size(); curIntexTarget++)
	{
		bool bBlank=(curIntexTarget != endIndexTarget && curIntexTarget != data.size());

		setData(recordType, target, data[curIntexTarget], append);

		if (bBlank)
		{
			int nRow=source->rowCount();
			source->insertRow(nRow);

			QModelIndex index=source->index(nRow, LogicalNameTitle);
			source->setData(index, BlankRowRole, RowRole);

			for (int i=0; i<source->columnCount(); i++)
			{
				index=source->index(nRow, i);
				((QStandardItemModel*)source)->itemFromIndex(index)->setEditable(false);
			}
		}
	}
}

void QGenFileCompare::fill(int num, QAbstractItemModel* model)
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

bool QGenFileCompare::compareDiff(QAbstractItemModel* source, int srow, QAbstractItemModel* target, int trow, int recordType, const QVector<int>& ignored)
{
	bool bDiff=false;

	Q_ASSERT_NO_NULL(source);
	Q_ASSERT_NO_NULL(target);

	int nNumColumn=0;
	switch (recordType)
	{
	case ANA_TYPE:
	case OUTA_TYPE:
	case PARAM_TYPE:
	case CALC_TYPE:
	case CHARA_TYPE:
		{
			nNumColumn=AnaTitleEnd;
		}
		break;
	case IMP_TYPE:
		{
			nNumColumn=IMPTitleEnd;
		}
		break;
	case SOE_TYPE:
	case POL_TYPE:
	case OUTS_TYPE:
	case SYSMSG_TYPE:
	case DEVOBJ_TYPE:
		{
			nNumColumn=INTTitleEnd;
		}
		break;
	case CURVE_TYPE:
		{
			nNumColumn=CURTitleEnd;
		}
		break;
	case TIME_TYPE:
		break;
	default:
		Q_ASSERT(false);
		break;
	}

	for (int i=0; i<nNumColumn; i++)
	{
		if (ignored.contains(i))
		{
			continue;
		}

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

QVector<int> QGenFileCompare::ignoredColumns(const QString& filename) const
{
	QVector<int> ignored;

	QString strConfigure=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
		.arg("database"));
	QReadMergeFromXmlFile reader(strConfigure, fileSuffix(filename));
	if (reader.read())
	{
		ignored=reader.result();
	}

	return ignored;
}

void QGenFileCompare::compareFile(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QGenFileCompare::RECORD_ENTRY> lcs;
	QGenFileCompare::RECORD_DEF_ENTRY src=qVariantValue<QGenFileCompare::RECORD_DEF_ENTRY>(source.data);
	QGenFileCompare::RECORD_DEF_ENTRY tar=qVariantValue<QGenFileCompare::RECORD_DEF_ENTRY>(target.data);

	QVector<QPair<int, int> > results=lcs.parse(src.entries, tar.entries);
	results.append(QPair<int, int>(src.entries.size(), tar.entries.size()));

 	source.model=new QStandardItemModel(0, source.titles.size());
 	((QStandardItemModel*)source.model)->setHorizontalHeaderLabels(source.titles);

	target.model=new QStandardItemModel(0, target.titles.size());
	((QStandardItemModel*)target.model)->setHorizontalHeaderLabels(target.titles);

	QVector<int> ignored=ignoredColumns(target.filename);
 
	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(recordTypeFromName(tar.recName), source.model, target.model, nIndexTarget, peer.second, tar.entries, AppendRowRole);

		//source
		diff(recordTypeFromName(src.recName), target.model, source.model, nIndexSource, peer.first, src.entries, DeleteRowRole);

		if (nIndexTarget<tar.entries.size() && nIndexSource<src.entries.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int srow=setData(recordTypeFromName(src.recName), source.model, src.entries[peer.first], CommonRowRole);
			int trow=setData(recordTypeFromName(tar.recName), target.model, tar.entries[peer.second], CommonRowRole);

			if (compareDiff(source.model, srow, target.model, trow, recordTypeFromName(src.recName), ignored))
			{
				source.model->setData(source.model->index(srow, LogicalNameTitle), ModifyRowRole, RowRole);
				target.model->setData(target.model->index(trow, LogicalNameTitle), ModifyRowRole, RowRole);
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

int QGenFileCompare::recordTypeFromName(const QString& name) const
{
	for (int i=0; i<_countof(recordname); i++)
	{
		if (name == recordname[i])
		{
			return i;
		}
	}

	return -1;
}

bool QGenFileCompare::checkModified(FileCompareResult& source, FileCompareResult& target)
{
	QLongestCommonSubsequenceV2<QGenFileCompare::RECORD_ENTRY> lcs;
	QGenFileCompare::RECORD_DEF_ENTRY src=qVariantValue<QGenFileCompare::RECORD_DEF_ENTRY>(source.data);
	QGenFileCompare::RECORD_DEF_ENTRY tar=qVariantValue<QGenFileCompare::RECORD_DEF_ENTRY>(target.data);

	if (src.recName != tar.recName || src.entries.size() != tar.entries.size())
	{
		return true;
	}

	QVector<QPair<int, int> > results=lcs.parse(src.entries, tar.entries);
	if (results.size() != src.entries.size())
	{
		return true;
	}

	results.append(QPair<int, int>(src.entries.size(), tar.entries.size()));

	source.model=new QStandardItemModel(0, source.titles.size());
	((QStandardItemModel*)source.model)->setHorizontalHeaderLabels(source.titles);

	target.model=new QStandardItemModel(0, target.titles.size());
	((QStandardItemModel*)target.model)->setHorizontalHeaderLabels(target.titles);

	QVector<int> ignored=ignoredColumns(target.filename);

	int nIndexSource=0, nIndexTarget=0;
	for (int i=0; i<results.size(); i++)
	{
		const QPair<int, int>& peer=results[i];

		//target
		diff(recordTypeFromName(tar.recName), source.model, target.model, nIndexTarget, peer.second, tar.entries, AppendRowRole);

		//source
		diff(recordTypeFromName(src.recName), target.model, source.model, nIndexSource, peer.first, src.entries, DeleteRowRole);

		if (nIndexTarget<tar.entries.size() && nIndexSource<src.entries.size())
		{
			nIndexTarget++;
			nIndexSource++;

			int srow=setData(recordTypeFromName(src.recName), source.model, src.entries[peer.first], CommonRowRole);
			int trow=setData(recordTypeFromName(tar.recName), target.model, tar.entries[peer.second], CommonRowRole);

			if (compareDiff(source.model, srow, target.model, trow, recordTypeFromName(src.recName), ignored))
			{
				return true;
			}
		}
	}

	return false;
}

int QGenFileCompare::checkValidity(FileCompareResult& result, QPair<QModelIndex, QModelIndex>& error)
{
	QAbstractItemModel* pModel=dynamic_cast<QAbstractItemModel*>(result.model);
	if (pModel == NULL)
	{
		return NoError;
	}

	QHash<quint32, int> logicalNames;
	QString strLogicalName;

	int nNumRows=pModel->rowCount();
	for (int i=0; i<nNumRows; i++)
	{
		QModelIndex index=pModel->index(i, 0);
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

		index=pModel->index(i, LogicalNameTitle);
		strLogicalName=pModel->data(index).toString();

		if (strLogicalName.isEmpty())
		{
			pModel->setData(index, true, VerifyItemRole);
			error.first=index;

			return KeyPropertyIsEmpryError;
		}

		qint32 nKey=qHash(strLogicalName);
		if (logicalNames.contains(nKey))
		{
			QList<int> values=logicalNames.values(nKey);
			foreach (int value, values)
			{
				QModelIndex tindex=pModel->index(value, LogicalNameTitle);
				if (pModel->data(tindex).toString() == strLogicalName)
				{
					pModel->setData(index, true, VerifyItemRole);
					pModel->setData(tindex, true, VerifyItemRole);

					error.first=index;
					error.second=tindex;

					return KeyPropertyRepeatError;
				}
			}	
		}

		logicalNames.insertMulti(nKey, i);
	}

// 	QGenFileCompare::RECORD_DEF_ENTRY entry=qVariantValue<QGenFileCompare::RECORD_DEF_ENTRY>(result.data);
// 
// 	int nRecordType=recordTypeFromName(entry.recName);
// 	switch (nRecordType)
// 	{
// 	case ANA_TYPE:
// 	case OUTA_TYPE:
// 	case PARAM_TYPE:
// 	case CALC_TYPE:
// 	case CHARA_TYPE:
// 		{
// 			
// 		}
// 		break;
// 	case IMP_TYPE:
// 		{
// 			
// 		}
// 		break;
// 	case SOE_TYPE:
// 	case POL_TYPE:
// 	case OUTS_TYPE:
// 	case SYSMSG_TYPE:
// 	case DEVOBJ_TYPE:
// 		{
// 			
// 		}
// 		break;
// 	case CURVE_TYPE:
// 		{
// 			
// 		}
// 		break;
// 	case TIME_TYPE:
// 		break;
// 	default:
// 		Q_ASSERT(false);
// 		break;
// 	}

	return NoError;
}

int QGenFileCompare::writeData(QIODevice* device, FileCompareResult& result)
{
	QTextStream stream(device);

	QGenFileCompare::RECORD_DEF_ENTRY entry=qVariantValue<QGenFileCompare::RECORD_DEF_ENTRY>(result.data);

	stream<<"RECORD";
	COMPARE_FILE_WRITE_PROPERTY(stream, "NAME", entry.recName);
	stream<<"\n";

	QAbstractItemModel* pModel=dynamic_cast<QAbstractItemModel*>(result.model);
	if (pModel != NULL)
	{
		QGenFileCompare::RECORD_DEF_ENTRY entry=qVariantValue<QGenFileCompare::RECORD_DEF_ENTRY>(result.data);

		int nRecordType=recordTypeFromName(entry.recName);
		QStringList titles;
		titles<<"NAME"<<"LONGNAME";

		switch (nRecordType)
		{
		case ANA_TYPE:
		case OUTA_TYPE:
		case PARAM_TYPE:
		case CALC_TYPE:
		case CHARA_TYPE:
			{
				titles<<"HH_LMT"<<"H_LMT"<<"HR_LMT"<<"LR_LMT"<<"L_LMT"<<"LL_LMT"<<"D_BAND"<<"HDB_BAND"<<"ENG_CVT"
					<<"HI_RANGE"<<"LO_RANGE"<<"HI_CODE"<<"LO_CODE"<<"TR_LIM"<<"RT_LIM"<<"DSP_FILE"<<"FAULT_PNT"<<"FAIL_PNT"<<"AUDIO_ALM"
					<<"TREND_ALM"<<"CASCADE_PNT"<<"CONTROL"<<"CTRL_ADDR"<<"ZERO"<<"COMPENSATE"<<"ALM_CBA"<<"VIRTUAL_PNT"<<"LCU_VIRTUAL"
					<<"CMD_MSG"<<"UNIT_NAME"<<"PAIR_TYPE"<<"PAIR_POINT"<<"HDB_REC"<<"HDB_PERIOD"<<"HDB_CURVE"<<"IO_GROUP"<<"TERMINAL_NO"<<"CABINET_NO"
					<<"UNIT_NO"<<"SLOT_NO"<<"CHANNEL_NO"<<"HDB_CONDITION"<<"CONSTRAINT"<<"CONSTRAINTGROUP_ID"<<"ID";
			}
			break;
		case IMP_TYPE:
			{
				titles<<"PROCTYPE"<<"CVT_F"<<"PT"<<"CT"<<"DAY_RATE"<<"YEAR_RATE"<<"CONSTANT"<<"DSP_FILE"<<"CASCADE_PNT"<<"HDB_REC"<<"HDB_PERIOD"
					<<"IO_GROUP"<<"TERMINAL_NO"<<"CABINET_NO"<<"UNIT_NO"<<"SLOT_NO"<<"CHANNEL_NO"<<"ID";
			}
			break;
		case SOE_TYPE:
		case POL_TYPE:
		case OUTS_TYPE:
		case SYSMSG_TYPE:
		case DEVOBJ_TYPE:
			{
				titles<<"PROCTYPE"<<"MSG_DEF"<<"CMD_MSG"<<"AUDIO_ALM"<<"STATISTIC"<<"EN_FAIL_REC"<<"FAULT_PNT"<<"FAIL_PNT"<<"KEY_SIGNAL"
					<<"DSP_FILE"<<"CONTROL"<<"CTRL_ADDR"<<"SOE"<<"ALM_CBA"<<"NORM_STAT"<<"ON_ALM"<<"OFF_ALM"<<"VIRTUAL_PNT"<<"LCU_VIRTUAL"
					<<"CASCADE_PNT"<<"DOUBLE_PNT"<<"NORMAL_CLOSE"<<"PAIR_TYPE"<<"PAIR_POINT"<<"HDB_REC"<<"IO_GROUP"<<"TERMINAL_NO"
					<<"CABINET_NO"<<"UNIT_NO"<<"SLOT_NO"<<"CHANNEL_NO"<<"ID";
			}
			break;
		case CURVE_TYPE:
			{
				titles<<"NUM"<<"ENG_CVT"<<"HI_RANGE"<<"LO_RANGE"<<"HDB_REC"<<"ID";
			}
			break;
		case TIME_TYPE:
			break;
		default:
			Q_ASSERT(false);
			break;
		}

		int nNumRows=pModel->rowCount();
		int nRowCount=0;
		for (int nRow=0; nRow<nNumRows; nRow++)
		{
			QModelIndex index=pModel->index(nRow, 0);
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

			stream<<"ENTRY";

			QStringListIterator it(titles);
			int nCol=0;
			while (it.hasNext())
			{
				const QString& strTitle=it.next();

				index=pModel->index(nRow, nCol++);
				int nItemRole=pModel->data(index, ItemRole).toInt();
				if (nItemRole == DeleteItemRole)
				{
					continue;
				}

				QVariant var=pModel->data(index, Qt::EditRole);
				if (var.isValid())
				{
					QString strText=var.toString();
					if (!strText.isEmpty())
					{
						COMPARE_FILE_WRITE_PROPERTY(stream, strTitle, strText);
					}
				}
			}

			COMPARE_FILE_WRITE_PROPERTY(stream, "PNT_ADD", nRowCount++);
			COMPARE_FILE_WRITE_PROPERTY(stream, "AOR_GRP", "GEN");

			stream<<"\n";
		}

		stream<<"\n";
	}

	stream<<"END"<<"\n";
	stream.flush();

	return NoError;
}

QGenFileCompare::RECORD_DEF_ENTRY QGenFileCompare::parseEntryFile(QTextStream& stream)
{
	RECORD_DEF_ENTRY record;

	int nNumEntry=-1;
	int nEntryValid=0;
	int nRecordType=-1;

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

		if (labels.size() == 3 && labels[0] == "RECORD" && labels[1] == "NAME")
		{
			if ((nRecordType=recordTypeFromName(labels[2])) != -1)
			{
				nEntryValid=0;
				nNumEntry=-1;

				record.recName=labels[2];
			}

			continue;
		}

		if (nRecordType == -1)
		{
			continue;
		}

		if (labels.size() == 3 && labels[0] == "ENTRY" && labels[1] == "NAME")
		{
			nEntryValid=1;

			for(int i=0; i<record.entries.size(); i++)
			{
				if(record.entries[i].fixed.entryName == labels[2])
				{
					nEntryValid=0;
					break;
				}
			}

			if (nEntryValid == 1)
			{
				nNumEntry++;

				record.entries.append(RECORD_ENTRY());
				record.entries[nNumEntry].fixed.entryName=labels[2];
			}

			continue;
		}

		if (labels.size() == 2)
		{
			if (labels[0] ==  "LONGNAME")
			{
				record.entries[nNumEntry].fixed.entryLongname=labels[1];
				continue;
			}
			else if (labels[0] ==  "EXLONGNAME")
			{
				record.entries[nNumEntry].fixed.entryExLongname=labels[1];
				continue;
			}
			else if (labels[0] ==  "DSP_FILE")
			{
				record.entries[nNumEntry].fixed.dsp_filename=labels[1];
				continue;
			}
			else if (labels[0] ==  "TERMINAL_NO")
			{
				record.entries[nNumEntry].fixed.hard_addr.terminal_no=labels[1];
				continue;
			}
			else if (labels[0] ==  "CABINET_NO")
			{
				record.entries[nNumEntry].fixed.hard_addr.cabinet_no=labels[1];
				continue;
			}
			else if (labels[0] ==  "UNIT_NO")
			{
				record.entries[nNumEntry].fixed.hard_addr.unit_no=labels[1];
				continue;
			}
			else if (labels[0] ==  "SLOT_NO")
			{
				record.entries[nNumEntry].fixed.hard_addr.slot_no=labels[1];
				continue;
			}
			else if (labels[0] ==  "CHANNEL_NO")
			{
				record.entries[nNumEntry].fixed.hard_addr.channel_no=labels[1];
				continue;
			}
			else if (labels[0] == "IO_GROUP")
			{
				record.entries[nNumEntry].fixed.io_group=labels[1];
				continue;
			}
			else if (labels[0] == "CMD_MSG")
			{
				record.entries[nNumEntry].fixed.msg_sel.cmd=labels[1];
				continue;
			}
			else if (labels[0] == "MSG_DEF")
			{
				record.entries[nNumEntry].fixed.msg_sel.alm=labels[1];
				continue;
			}
			else if (labels[0] == "CTRL_ADDR")
			{
				record.entries[nNumEntry].fixed.ctrl_addr=labels[1];
				continue;
			}
			else if (labels[0] == "PAIR_POINT")
			{
				record.entries[nNumEntry].fixed.pairName=labels[1];
				continue;
			}
			else if (labels[0] == "PAIR_TYPE")
			{
				record.entries[nNumEntry].fixed.pairType=labels[1];
				continue;
			}
			else if (labels[0] == "ID")
			{
				record.entries[nNumEntry].fixed.id=labels[1];
				continue;
			}
			else if(labels[0] ==  "STATISTIC")
			{
				record.entries[nNumEntry].fixed.ioattr.statistic = labels[1];
				continue;
			}
			else if(labels[0] == "FAULT_PNT")
			{
				record.entries[nNumEntry].fixed.ioattr.fault = labels[1];
				continue;
			}
			else if(labels[0] ==  "FAIL_PNT")
			{
				record.entries[nNumEntry].fixed.ioattr.fail = labels[1];
				continue;
			}
			else if(labels[0] == "SBO") /*reserved for old format*/
			{
				record.entries[nNumEntry].fixed.ioattr.ctrl = labels[1];
				continue;
			}
			else if(labels[0] == "CONTROL")
			{
				record.entries[nNumEntry].fixed.ioattr.ctrl = labels[1];
				continue;
			}
			else if(labels[0] == "SOE")
			{
				record.entries[nNumEntry].fixed.ioattr.soe = labels[1];
				continue;
			}
			else if(labels[0] == "TREND_ALM")
			{
				record.entries[nNumEntry].fixed.ioattr.trendAlm = labels[1];
				continue;
			}
			else if(labels[0] == "KEY_SIGNAL")
			{
				record.entries[nNumEntry].fixed.ioattr.keySignal = labels[1];
				continue;
			}
			else if(labels[0] == "EN_FAIL_REC")
			{
				record.entries[nNumEntry].fixed.ioattr.faultRec = labels[1];
				continue;
			}
			else if(labels[0] == "AUDIO_ALM")
			{
				record.entries[nNumEntry].fixed.ioattr.audAlm = labels[1];
				continue;
			}
			else if(labels[0] == "REVERSE_PNT")
			{
				record.entries[nNumEntry].fixed.ioattr.normalClose = labels[1];
				continue;
			}
			else if(labels[0] == "NORMAL_CLOSE")
			{
				record.entries[nNumEntry].fixed.ioattr.normalClose = labels[1];
				continue;
			}
			else if(labels[0] == "DOUBLE_PNT")
			{
				record.entries[nNumEntry].fixed.ioattr.doublePt = labels[1];
				continue;
			}
			else if(labels[0] == "VIRTUAL_PNT")
			{
				record.entries[nNumEntry].fixed.ioattr.sysVirtual = labels[1];
				continue;
			}
			else if(labels[0] == "LCU_VIRTUAL")
			{
				record.entries[nNumEntry].fixed.ioattr.lcuVirtual = labels[1];
				continue;
			}
			else if(labels[0] == "CASCADE_PNT")
			{
				record.entries[nNumEntry].fixed.ioattr.cascade = labels[1];
				continue;
			}
			else if(labels[0] == "HDB_REC")
			{
				record.entries[nNumEntry].fixed.ioattr.hdbRec = labels[1];
				continue;
			}
			else if(labels[0] == "INTO_HDB")	 /*reserved for old format*/
			{
				record.entries[nNumEntry].fixed.ioattr.hdbRec = labels[1];
				continue;
			}
			else if(labels[0] == "HDB_CURVE")
			{
				record.entries[nNumEntry].fixed.ioattr.hdbCurve = labels[1];
				continue;
			}
			else if(labels[0] == "ALM_CBA")
			{
				record.entries[nNumEntry].fixed.ioattr.almCba = labels[1];
				continue;
			}
			else if(labels[0] == "ON_ALM")
			{
				record.entries[nNumEntry].fixed.ioattr.onAlm = labels[1];
				continue;
			}
			else if(labels[0] == "OFF_ALM")
			{
				record.entries[nNumEntry].fixed.ioattr.offAlm = labels[1];
				continue;
			}
			else if(labels[0] == "NORM_STAT")
			{
				record.entries[nNumEntry].fixed.ioattr.normalState = labels[1];
				continue;
			}
			else if(labels[0] == "PROCTYPE")
			{
				record.entries[nNumEntry].fixed.ioattr.procType = labels[1];
				continue;
			}
			else if (labels[0] == "HDB_PERIOD")
			{
				record.entries[nNumEntry].fixed.ioattr.hdbPeriod=labels[1];
				continue;
			}
		}

		switch(nRecordType)
		{
// 		case CHARA_TYPE:
// 			if(labels[0] == "START_PT_NAME")
// 			{
// 				record.entries[nNumEntry].var.chara.start_pt_name=labels[1];
// 			}
// 			else if(labels[0] == "PT_NUM")
// 			{
// 				record.entries[nNumEntry].var.chara.pt_num=labels[1];
// 			}
		case ANA_TYPE:
		case OUTA_TYPE:
		case PARAM_TYPE:
		case CALC_TYPE:
		case CHARA_TYPE:
			if (labels.size() == 2)
			{
				if(labels[0] == "UNIT_NAME")
				{
					record.entries[nNumEntry].var.anlg.unitName=labels[1];
				}
				else if(labels[0] == "HH_LMT")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.HI_ALARM= labels[1];
				}
				else if(labels[0] == "H_LMT")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.HI_WARN= labels[1];
				}	
				else if(labels[0] == "TR_LIM")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.TR_WARN= labels[1];
				}	
				else if(labels[0] == "HR_LMT")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.HI_RECOVER= labels[1];
				}	
				else if(labels[0] == "L_LMT")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.LO_WARN= labels[1];
				}	
				else if(labels[0] == "LL_LMT")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.LO_ALARM= labels[1];
				}	
				else if(labels[0] == "RT_LIM")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.TR_RECOVER= labels[1];
				}	
				else if(labels[0] == "LR_LMT")
				{
					record.entries[nNumEntry].var.anlg.lmt_para.LO_RECOVER= labels[1];
				}	
				else if(labels[0] == "D_BAND")
				{
					record.entries[nNumEntry].var.anlg.eng_para.D_BAND= labels[1];
				}
				else if(labels[0] == "HDB_BAND")
				{
					record.entries[nNumEntry].var.anlg.eng_para.HDB_BAND= labels[1];
				}
				else if(labels[0] == "ENG_CVT")
				{
					record.entries[nNumEntry].var.anlg.eng_para.ENG_CVT= labels[1];
				}	
				else if(labels[0] == "HI_RANGE")
				{
					record.entries[nNumEntry].var.anlg.eng_para.HI_RANGE= labels[1];
				}	
				else if(labels[0] == "LO_RANGE")
				{
					record.entries[nNumEntry].var.anlg.eng_para.LO_RANGE= labels[1];
				}	
				else if(labels[0] == "ZERO")
				{
					record.entries[nNumEntry].var.anlg.eng_para.ZERO= labels[1];
				}	
				else if(labels[0] == "COMPENSATE")
				{
					record.entries[nNumEntry].var.anlg.eng_para.COMPSATE= labels[1];
				}	
				else if(labels[0] == "HI_CODE")
				{
					record.entries[nNumEntry].var.anlg.eng_para.HI_CODE=labels[1];
				}	
				else if(labels[0] == "LO_CODE")
				{
					record.entries[nNumEntry].var.anlg.eng_para.LO_CODE= labels[1];
				}
				else if (labels[0] == "HDB_CONDITION")
				{
					record.entries[nNumEntry].var.anlg.eng_para.HDB_CONDITION= labels[1];
				}
				else if (labels[0] == "CONSTRAINT")
				{
					record.entries[nNumEntry].var.anlg.eng_para.CONSTRAINT= labels[1];
				}
				else if (labels[0] == "CONSTRAINTGROUP_ID")
				{
					record.entries[nNumEntry].var.anlg.eng_para.CONSTRAINTGROUP_ID= labels[1];
				}
			}
			break;
		case SOE_TYPE:
		case POL_TYPE:
		case OUTS_TYPE:
		case SYSMSG_TYPE:
			break;
		case IMP_TYPE:
			if(labels[0] == "CVT_F")
			{
				record.entries[nNumEntry].var.imp.entry.eng_para.ENG_CVT= labels[1];
			}
			else if(labels[0] == "CONSTANT")
			{
				record.entries[nNumEntry].var.imp.entry.eng_para.CONSTANT= labels[1];
			}
			else if(labels[0] == "DAY_RATE")
			{
				record.entries[nNumEntry].var.imp.entry.eng_para.DAY_RATE= labels[1];
			}
			else if(labels[0] == "YEAR_RATE")
			{
				record.entries[nNumEntry].var.imp.entry.eng_para.YEAR_RATE= labels[1];
			}
			else if(labels[0] == "CT")
			{
				record.entries[nNumEntry].var.imp.entry.eng_para.CT= labels[1];
			}
			else if(labels[0] == "PT")
			{
				record.entries[nNumEntry].var.imp.entry.eng_para.PT= labels[1];
			}
			break;
		case CURVE_TYPE:
			if(labels[0] == "NUM")
			{
				record.entries[nNumEntry].var.curve.seg_num=labels[1];
			}
			else if(labels[0] == "HI_RANGE")
			{
				record.entries[nNumEntry].var.curve.eng_para.HI_RANGE=labels[1];
			}
			else if(labels[0] == "LO_RANGE")
			{
				record.entries[nNumEntry].var.curve.eng_para.LO_RANGE=labels[1];
			}
			else if(labels[0] == "ENG_CVT")
			{
				record.entries[nNumEntry].var.curve.eng_para.ENG_CVT=labels[1];
			}
			break;
		default:
			break;
		}
	}

	return record;
}

bool operator==(const QGenFileCompare::RECORD_ENTRY& source, const QGenFileCompare::RECORD_ENTRY& target)
{
	if (!source.fixed.entryName.isEmpty() || !target.fixed.entryName.isEmpty())
	{
		if (source.fixed.entryName == target.fixed.entryName)
		{
			return true;
		}
	}

	if (!source.fixed.id.isEmpty() || !target.fixed.id.isEmpty())
	{
		if (source.fixed.id == target.fixed.id)
		{
			return true;
		}
	}

	return (source.fixed.hard_addr == target.fixed.hard_addr);
}

bool operator==(const QGenFileCompare::HARD_ADDR& source, const QGenFileCompare::HARD_ADDR& target)
{
	if (!source.cabinet_no.isEmpty() || !target.cabinet_no.isEmpty() || !source.unit_no.isEmpty() || !target.unit_no.isEmpty() ||
		!source.slot_no.isEmpty() || !target.slot_no.isEmpty() || !source.channel_no.isEmpty() || !target.channel_no.isEmpty() || 
		!source.terminal_no.isEmpty() || !target.terminal_no.isEmpty())
	{
		return ((source.cabinet_no == target.cabinet_no) && (source.unit_no == target.unit_no) && 
			(source.slot_no == target.slot_no) && (source.channel_no == target.channel_no) && (source.terminal_no == target.terminal_no));
	}

	return false;
}

REGISTER_FC_INSTANCE(QGenFileCompare) 
