#ifndef QGENFILECOMPARE_H
#define QGENFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"

class FC_EXPORT QGenFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QGenFileCompare(QObject *parent=0);
	~QGenFileCompare();

	enum {Type=COMPARE_GEN_FILE};

	typedef struct
	{
		QString	cabinet_no;
		QString	unit_no;       	
		QString	slot_no;		
		QString	channel_no;	        
		QString	terminal_no;	
	} HARD_ADDR;

	typedef struct
	{
		QString	procType;
		QString	almCba;
		QString	statistic;
		QString	fault;
		QString	fail;
		QString	ctrl;
		QString	soe;
		QString	trendAlm;
		QString	keySignal;
		QString	faultRec;
		QString	audAlm;
		QString	normalClose;
		QString	doublePt;
		QString	sysVirtual;
		QString	lcuVirtual;
		QString	cascade;
		QString	hdbRec;
		QString	hdbCurve;
		QString	onAlm;
		QString	offAlm;
		QString	normalState;
		QString	res;
		QString	hdbPeriod;
	} IOATTR;

	typedef struct
	{
		QString	cmd;
		QString	alm;
	} MSG_SELECTOR;

	typedef	struct		/* Entry descriptor - fixed portion */
	{
		QString		entryName;	/* Entry's name	*/
		QString		entryLongname;	/* Entry's longname	*/
		QString		entryExLongname;
		QString		dsp_filename;
		HARD_ADDR	hard_addr;
		QString		io_group;
		IOATTR		ioattr;
		QString		pairName;
		QString		pairType;
		QString		ctrl_addr;
		MSG_SELECTOR msg_sel;
		QString		id;
	} ENTRY_FIXED;

	typedef struct
	{
		QString	D_BAND;
		QString HDB_BAND;
		QString	ENG_CVT;
		QString	HI_RANGE;		
		QString	LO_RANGE;		
		QString	ZERO;			
		QString	COMPSATE;
		QString	HI_CODE;
		QString	LO_CODE;
		QString HDB_CONDITION;
		QString CONSTRAINT;
		QString CONSTRAINTGROUP_ID;
	} ANA_ENG_PARA;

	typedef struct
	{
		QString	HI_ALARM; 
		QString	HI_WARN;
		QString	HI_RECOVER;
		QString	LO_ALARM;
		QString	LO_WARN;
		QString	LO_RECOVER;
		QString	TR_WARN;
		QString	TR_RECOVER;
	} LMT_PARA;

	typedef struct
	{
		QString        unitName;
		ANA_ENG_PARA    eng_para;
		LMT_PARA    lmt_para;
	} ENTRY_ANLG_VAL;

	typedef struct
	{
		QString	PT;		
		QString	CT;		
		QString	ENG_CVT;
		QString	CONSTANT;
		QString	DAY_RATE;
		QString	YEAR_RATE;
	} IMP_ENG_PARA;

	typedef struct
	{
		IMP_ENG_PARA	eng_para;		
	} ENTRY_IMP_VAL;

	typedef struct
	{
		QString		start_pt_name;
		QString		pt_num;
	} ENTRY_CHARA_VAL;

	typedef struct
	{
		QString		seg_num;
		ANA_ENG_PARA    eng_para;
	} ENTRY_CURVE_VAL;

	typedef struct
	{
		ENTRY_ANLG_VAL	anlg;
		struct
		{
			ENTRY_IMP_VAL	entry;		
		} imp;
		ENTRY_CHARA_VAL	chara;
		ENTRY_ANLG_VAL	calc;
		ENTRY_CURVE_VAL	curve;
	} RD_VAR_UNION;

	typedef struct 
	{
		ENTRY_FIXED fixed;
		RD_VAR_UNION var;
	}RECORD_ENTRY;

	typedef struct
	{
		QString recName;

		QVector<RECORD_ENTRY> entries;
	}RECORD_DEF_ENTRY;

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);
	virtual int checkValidity(FileCompareResult& result, QPair<QModelIndex, QModelIndex>& error);

private:
	void diff(int recordType, QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		const QVector<QGenFileCompare::RECORD_ENTRY>& data, bool append=true);

	void fill(int num, QAbstractItemModel* model);

	int setData(int recordType, QAbstractItemModel* model, const QGenFileCompare::RECORD_ENTRY& data, int rowRole);
	void setItemData(QAbstractItemModel* model, int row, int column, const QVariant& var, int itemRole);

	bool compareDiff(QAbstractItemModel* source, int srow, QAbstractItemModel* target, int trow, int recordType, const QVector<int>& ignored);

	QVector<int> ignoredColumns(const QString& filename) const;

private:
	enum CommonTitleType
	{
		LogicalNameTitle,		//逻辑名
		LongNameTitle,			//汉字名
		CommonTitleEnd
	};

	enum AnaTitleType
	{
		HHLimitTitle=CommonTitleEnd,			//上上限值
		HLimitTitle,			//上限值
		HRLimitTitle,			//上限复死区
		LRLimitTitle,			//下限复死区
		LLimitTitle,			//下限值
		LLLimitTitle,			//下下限值
		DBandTitle,				//死区值
		HDBBandTitle,			//历史数据死区值
		ENGConvertTitle,		//转换系数
		HighRangeTitle,			//上量程
		LowRangeTitle,			//下量程
		HighCodeTitle,			//原码上量程
		LowCodeTitle,			//原码下量程
		TrendLimitTitle,		//趋势越限值
		RTLimitTitle,			//趋势复限死区
		DSPFileTitle,			//数据库文件
		FaultPointTitle,		//事故点
		FailPointTitle,			//故障点
		AudioAlarmTitle,		//语音报警点
		TrendAlarmTitle,		//趋势报警点
		CascadePointTitle,		//送梯调点
		ControlPointTitle,		//控制点
		ControlAddrTitle,		//控制地址
		ZeroTitle,				//零点值
		CompensateTitle,		//补偿值
		AlarmCBATitle,			//报警闭锁点
		VirtualPointTitle,		//系统虚拟点
		LCUVirtualTitle,		//LCU虚拟点
		CMDMsgTitle,			//命令字符串组
		UnitTitle,				//单位
		PairTypeTitle,			//配对类型
		PairPointTitle,			//配对点逻辑名
		HDBRecordTitle,			//写历史库
		HDBPeriodTitle,			//定时数据产生周期
		HDBCurveTitle,			//产生秒级数据
		IOGroupTitle,			//分组号
		TerminalNoTitle,		//端子号
		CabinetNoTitle,			//机柜号
		UnitNoTitle,			//机箱号
		SlotNoTitle,			//板槽号
		ChannelNoTitle,			//通道号
		HDBConditionTitle,		//条件写库
		ConstraintTitle,		//约束计算
		ConstraintGroupIDTitle,	//约束条件组ID
		IDTitle,					//ID号
		AnaTitleEnd
	};

	enum ImpTitleType
	{
		IMP_ProcTypeTitle=CommonTitleEnd,	//接点类型
		IMP_ConvertTitle,			//转换系数
		IMP_PTTitle,				//PT
		IMP_CTTitle,				//CT
		IMP_DayRateTitle,			//日倍率
		IMP_YearRateTitle,			//年倍率
		IMP_ConstantTitle,			//翻转常数
		IMP_DspFileTitle,			//画面文件
		IMP_CascadePointTitle,		//送梯调点
		IMP_HDBRecordTitle,			//写历史库
		IMP_HDBPeriodTitle,			//定时数据产生周期
		IMP_IOGroupTitle,			//分组号
		IMP_TerminalNoTitle,		//端子号
		IMP_CabinetNoTitle,			//机柜号
		IMP_UnitNoTitle,			//机箱号
		IMP_SlotNoTitle,			//板槽号
		IMP_ChannelNoTitle,			//通道号
		IMP_IDTitle,				//ID号
		IMPTitleEnd
	};

	enum IntTitleType
	{
		INT_ProcTypeTitle=CommonTitleEnd,	//接点类型
		INT_MSGDefTitle,			//事件字符串组
		INT_CMDMsgTitle,			//命令字符串组
		INT_AudioAlarmTitle,		//语音报警点
		INT_StatisticsTitle,		//统计点
		INT_FailRecordTitle,		//事故追忆点
		INT_FaultPointTitle,		//事故点
		INT_FailPointTitle,			//故障点
		INT_KeySignalTitle,			//重要点
		INT_DspFileTitle,			//画面文件
		INT_ControlPointTitle,		//控制点
		INT_ControlAddrTitle,		//控制地址
		INT_SOE,					//事件顺序记录点
		INT_AlarmCBATitle,			//报警闭锁点
		INT_NormalStateTitle,		//接点常态
		INT_ONAlarm,				//0->1产生事件
		INT_OFFAlarm,				//1->0产生事件
		INT_VirtualPointTitle,		//系统虚拟点
		INT_LCUVirtualTitle,		//LCU虚拟点
		INT_CascadePointTitle,		//送梯调点
		INT_TriplePointTitle,		//三态点
		INT_NormalCloseTitle,		//三态点关闭接点
		INT_PairTypeTitle,			//三态点配对类型
		INT_PairPointTitle,			//三态点配对点逻辑名
		INT_HDBRecordTitle,			//写历史库
		INT_IOGroupTitle,			//分组号
		INT_TerminalNoTitle,		//端子号
		INT_CabinetNoTitle,			//机柜号
		INT_UnitNoTitle,			//机箱号
		INT_SlotNoTitle,			//板槽号
		INT_ChannelNoTitle,			//通道号
		INT_IDTitle,				//ID号
		INTTitleEnd
	};

	enum CurveTitleType
	{
		CUR_NumPoints=CommonTitleEnd,	//数据个数
		CUR_ENGConvertTitle,		//转换系数
		CUR_HighRangeTitle,			//上量程
		CUR_LowRangeTitle,			//下量程
		CUR_HDBRecordTitle,			//写历史库
		CUR_IDTitle,				//ID号
		CURTitleEnd
	};

	int recordTypeFromName(const QString& name) const;
	RECORD_DEF_ENTRY parseEntryFile(QTextStream& stream);
};

FC_EXPORT bool operator==(const QGenFileCompare::RECORD_ENTRY& source, const QGenFileCompare::RECORD_ENTRY& target);
FC_EXPORT bool operator==(const QGenFileCompare::HARD_ADDR& source, const QGenFileCompare::HARD_ADDR& target);

Q_DECLARE_METATYPE(QGenFileCompare::RECORD_DEF_ENTRY)

#endif // QGENFILECOMPARE_H
