
#include	"../inc/os_type_def.h"
#include	"../inc/dms_com.h"
#include	"../inc/dms_functions.h"
#include	"../inc/dms_util_def.h"
#include	"../inc/dms_structure_def.h"
#include	"../inc/dms_text_def.h"

void	PrintEntryFixed( DMS_COMMON *common_aa );
void	PrintDIVar( DMS_COMMON *common_aa );
void	PrintAIVar( DMS_COMMON *common_aa );
void	PrintImpVar( DMS_COMMON *common_aa );
void	PrintCalcVar( DMS_COMMON *common_aa );
void	PrintCharaVar( DMS_COMMON *common_aa );
void	PrintCurveVar( DMS_COMMON *common_aa );
void	PrintDevObjVar( DMS_COMMON *common_aa );
void	WriteIndPoint(POINTER *point);
void	WriteAnaPoint(POINTER *point);
void	WriteImpPoint(POINTER *point);
void	WriteCharaPoint(POINTER *point);
void	WriteCurvePoint(POINTER *point);
void	WriteDevObjPoint(POINTER *point);


#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "../../../lan/v4.0a/lib/lan.lib")


void	main()	
{
	DMS_COMMON	common_aa;
	char		pt_name[POINT_NAME_SIZE];
	POINTER		point;
	char		answer[128];

	if(-1==MountDBFile(NULL, 1))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}


LOOP:

	printf ( DMS_STR_INPUT_POINT );
	gets( pt_name);

	if(!strcmp(pt_name, "q") || !strcmp(pt_name, "Q"))
		exit(0);

	if(-1==GetPtIdFromStr(pt_name, &point))
	{
		printf("\nNo this point in database" );
		goto LOOP;
	}
	
LOOP1:
	if(-1==ReadEntryById( &point, &common_aa ))
	{
		printf("\nNo this point in database" );
		goto LOOP;
	}

	PrintEntryFixed(&common_aa);
	switch	( point.data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		PrintDIVar(&common_aa);
		break;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		PrintAIVar(&common_aa);
		break;

	case	IMP_TYPE:
		PrintImpVar(&common_aa);
		break;

	case	CHARA_TYPE:
		PrintCharaVar(&common_aa);
		break;

	case	CALC_TYPE:
		PrintCalcVar(&common_aa);
		break;

	case	CURVE_TYPE:
		PrintCurveVar(&common_aa);
		break;

	case	DEVOBJ_TYPE:
		PrintDevObjVar(&common_aa);
		break;

	default:
		goto LOOP;
	}

	printf( DMS_STR_SELECT_OP_MODE );
	gets( answer );
	if(answer[0]!='Y' && answer[0]!='y' )
		goto LOOP;

	switch	( point.data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		WriteIndPoint(&point);
		break;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
	case	CALC_TYPE:
		WriteAnaPoint(&point);
		break;

	case	IMP_TYPE:
		WriteImpPoint(&point);
		break;

	case	CHARA_TYPE:
		printf("\nCurrently writing chara type points is unsupported!");
		break;

	case	CURVE_TYPE:
		WriteCurvePoint(&point);
		break;

	case	DEVOBJ_TYPE:
		WriteDevObjPoint(&point);
		break;
	}
	goto LOOP1;
}

void WritePointIost(POINTER *point)
{
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_VIEWPOINT );
	printf( DMS_STR_MODY_SCAN_ENABLE );
	printf( DMS_STR_MODY_ALM_ENABLE );
	printf( DMS_STR_MODY_ALM_NOACK );
	printf( DMS_STR_MODY_CTRL_LOCK );
	printf( DMS_STR_MODY_NOUPDATE );
	printf( DMS_STR_MODY_ALM_REPEAT );
	printf( DMS_STR_MODY_TAG_STATE );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IOST_INPUT );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>8)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%d", &input);
	gets( answer );

	switch( select )
	{
	case	1:
		iost.viewPoint= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_VIEWPOINT );
		break;

	case	2:
		iost.scanEnable= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_SCAN_ENABLE );
		break;

	case	3:
		iost.almEnable= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_ALM_ENABLE );
		break;

	case	4:
		iost.almNoAck= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_ALM_NOACK );
		break;

	case	5:
		iost.ctrlLock= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_CTRL_LOCK );
		break;

	case	6:
		iost.noUpdate= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_UPDATED );
		break;

	case	7:
		iost.almRepeat= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_ALM_REPEAT );
		break;

	case	8:
		iost.tagState= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_TAG_STATE );
		break;

	default:
		break;
	}
}


void WritePointIoattr(POINTER *point)
{
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	IOATTR	ioattr={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
 	printf( DMS_STR_MODY_PROCTYPE );		
 	printf( DMS_STR_MODY_ALMCBA );			
 	printf( DMS_STR_MODY_STATISTIC );		
 	printf( DMS_STR_MODY_FAULT );			
 	printf( DMS_STR_MODY_FAIL );			
 	printf( DMS_STR_MODY_CTRL );			
 	printf( DMS_STR_MODY_SOE );			
 	printf( DMS_STR_MODY_TRENDALM );		
	printf( DMS_STR_MODY_KEYSIGNAL );		
	printf( DMS_STR_MODY_FAILREC );		
	printf( DMS_STR_MODY_AUDALM );			
	printf( DMS_STR_MODY_REVERSE );		
	printf( DMS_STR_MODY_DOUBLEPT );		
	printf( DMS_STR_MODY_SYSVIRTUAL );		
	printf( DMS_STR_MODY_LCUVIRTUAL );		
	printf( DMS_STR_MODY_CASCADE );		
	printf( DMS_STR_MODY_HDBREC );			
	printf( DMS_STR_MODY_HDBCURVE );		
	printf( DMS_STR_MODY_ON_ALM );		
	printf( DMS_STR_MODY_OFF_ALM );		
	printf( DMS_STR_MODY_HDBPERIOD );		
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IOATTR_INPUT );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>21)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%d", &input);
	gets( answer );

	switch( select )
	{
	case	1:
		ioattr.procType= input;
		SetPtIoattrById( point, ioattr, RW_PROCTYPE );
		break;

	case	2:
		ioattr.almCba= input;
		SetPtIoattrById( point, ioattr, RW_ALM_CBA );
		break;

	case	3:
		ioattr.statistic= input;
		SetPtIoattrById( point, ioattr, RW_STATISTIC );
		break;

	case	4:
		ioattr.fault= input;
		SetPtIoattrById( point, ioattr, RW_FAULT );
		break;

	case	5:
		ioattr.fail= input;
		SetPtIoattrById( point, ioattr, RW_FAIL );
		break;

	case	6:
		ioattr.ctrl= input;
		SetPtIoattrById( point, ioattr, RW_CTRL );
		break;

	case	7:
		ioattr.soe= input;
		SetPtIoattrById( point, ioattr, RW_SOE );
		break;

	case	8:
		ioattr.trendAlm= input;
		SetPtIoattrById( point, ioattr, RW_TREND_ALM );
		break;

	case	9:
		ioattr.keySignal= input;
		SetPtIoattrById( point, ioattr, RW_KEY_SIGNAL );
		break;

	case	10:
		ioattr.faultRec= input;
		SetPtIoattrById( point, ioattr, RW_FAULT_REC );
		break;

	case	11:
		ioattr.audAlm= input;
		SetPtIoattrById( point, ioattr, RW_AUD_ALM );
		break;

	case	12:
		ioattr.normalClose= input;
		SetPtIoattrById( point, ioattr, RW_NORM_CLOSE );
		break;

	case	13:
		ioattr.doublePt= input;
		SetPtIoattrById( point, ioattr, RW_DOUBLE_PT );
		break;

	case	14:
		ioattr.sysVirtual= input;
		SetPtIoattrById( point, ioattr, RW_SYS_VIRTUAL );
		break;

	case	15:
		ioattr.lcuVirtual= input;
		SetPtIoattrById( point, ioattr, RW_LCU_VIRTUAL );
		break;

	case	16:
		ioattr.cascade= input;
		SetPtIoattrById( point, ioattr, RW_CASCADE );
		break;

	case	17:
		ioattr.hdbRec= input;
		SetPtIoattrById( point, ioattr, RW_HDB_REC );
		break;

	case	18:
		ioattr.hdbCurve= input;
		SetPtIoattrById( point, ioattr, RW_HDB_CURVE );
		break;

	case	19:
		ioattr.onAlm= input;
		SetPtIoattrById( point, ioattr, RW_ON_ALM );
		break;
		
	case	20:
		ioattr.offAlm= input;
		SetPtIoattrById( point, ioattr, RW_OFF_ALM );
		break;
		
	case	21:
		ioattr.hdbPeriod= input;
		SetPtIoattrById( point, ioattr, RW_HDB_PERIOD );
		break;

	default:
		break;
	}
}


void WriteIndPointStatus(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_IND_MAN_SET );
	printf( DMS_STR_MODY_IND_CHAN_FAIL );
	printf( DMS_STR_MODY_IND_FAIL_TAG );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IND_STATUS );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>3)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%d", &input);
	gets( answer );

	if(-1==ReadEntryById( point, &common_aa ))
	{
		printf("\nNo this point in database" );
		return;
	}

	GetSysTime(&time);

	switch( select )
	{
	case	1:
		wr_var.ind= common_aa.var.ind.status;
		wr_var.ind.man_set= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_MAN_SET );
		break;

	case	2:
		wr_var.ind= common_aa.var.ind.status;
		wr_var.ind.chan_fail= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_CHAN_FAIL );
		break;

	case	3:
		wr_var.ind= common_aa.var.ind.status;
		wr_var.ind.fault_tag= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_FAULT_TAG );
		break;

	default:
		break;
	}
}

void WriteAnaPointStatus(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_ANA_CTRL_BLOCK );
	printf( DMS_STR_MODY_ANA_CHAN_FAIL );
	printf( DMS_STR_MODY_ANA_LMT_ST );
	printf( DMS_STR_MODY_ANA_TREND_ST );
	printf( DMS_STR_MODY_ANA_OVERFLOW );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_ANA_STATUS );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>5)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%d", &input);
	gets( answer );

	if(-1==ReadEntryById( point, &common_aa ))
	{
		printf("\nNo this point in database" );
		return;
	}

	GetSysTime(&time);
	switch( select )
	{
	case	1:
		wr_var.anlg.status.ctrl_block= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_CTRL_BLOCK );
		break;

	case	2:
		wr_var.anlg.status.chan_fail= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_CHAN_FAIL );
		break;

	case	3:
		wr_var.anlg.status.lmt_state= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_LMT_STATE );
		break;

	case	4:
		wr_var.anlg.status.trend_state= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_TREND_STATE );
		break;

	case	5:
		wr_var.anlg.status.overflow= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_OVERFLOW );
		break;

	default:
		break;
	}
}

void WriteIndPointStc(POINTER *point)
{
	char	answer[128];
	int	select, input;

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_IND_NORMAL_CLOSE );
	printf( DMS_STR_MODY_IND_NORMAL_OPEN );
	printf( DMS_STR_MODY_IND_FAIL_CLOSE );
	printf( DMS_STR_MODY_IND_FAIL_OPEN );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IND_STC_SEL );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>4)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%d", &input);
	gets( answer );

	switch( select )
	{
	case	1:
		SetIndPtStcById( point, (USHORT)input, RW_STC_NORMAL_CLOSE );
		break;

	case	2:
		SetIndPtStcById( point, (USHORT)input, RW_STC_NORMAL_OPEN );
		break;

	case	3:
		SetIndPtStcById( point, (USHORT)input, RW_STC_FAULT_CLOSE );
		break;

	case	4:
		SetIndPtStcById( point, (USHORT)input, RW_STC_FAULT_OPEN );
		break;

	default:
		break;
	}
}


void WriteIndPoint(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	IOST	iost={0};
	DATE_TIME	time={0};


	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_VALUE );
	printf( DMS_STR_MODY_STATUS );
	printf( DMS_STR_MODY_IOST );
	printf( DMS_STR_MODY_IOATTR );
	printf( DMS_STR_MODY_CTRLADDR );
	printf( DMS_STR_MODY_IO_GROUP );
	printf( DMS_STR_MODY_CMD_MSGSEL );
	printf( DMS_STR_MODY_ALM_MSGSEL );
	printf( DMS_STR_MODY_IND_STC );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IND_INPUT );
	
	scanf("%d", &select);
	gets( answer );

	switch( select )
	{
	case	1:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		if(-1==ReadEntryById( point, &common_aa ))
		{
			printf("\nNo this point in database" );
			break;
		}
		GetSysTime(&time);
		wr_var.ind= common_aa.var.ind.status;
		wr_var.ind.rt_state= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	2:
		WriteIndPointStatus(point);
		break;

	case	3:
		WritePointIost(point);
		break;

	case	4:
		WritePointIoattr(point);
		break;

	case	5:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetPtCtrlAddrById( point, (USHORT)input );
		break;

	case	6:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetPtIoGroupById( point, (USHORT)input );
		break;

	case	7:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetPtCmdMsgSelById( point, (UCHAR)input );
		break;

	case	8:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetIndPtAlmMsgSelById( point, (UCHAR)input );
		break;

	case	9:
		WriteIndPointStc(point);
		break;

	default:
		printf("\nunknown select");
		break;
	}
}

void WriteAnaPointEngPara(POINTER *point)
{
	char	answer[128];
	int	select;
	FLOAT	data;

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf ( DMS_STR_MODY_D_BAND );
	printf ( DMS_STR_MODY_ENG_CVT );
	printf ( DMS_STR_MODY_HI_RANGE );
	printf ( DMS_STR_MODY_LO_RANGE );
	printf ( DMS_STR_MODY_ZERO );
	printf ( DMS_STR_MODY_COMPSATE ); 
	printf ( DMS_STR_MODY_HI_CODE );
	printf ( DMS_STR_MODY_LO_CODE ); 
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_ANA_ENG_SEL );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>8)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%f", &data);
	gets( answer );

	switch( select )
	{
	case	1:
		SetAnaPtEngLmtParaById( point, data, RW_D_BAND );
		break;

	case	2:
		SetAnaPtEngLmtParaById( point, data, RW_ENG_CVT );
		break;

	case	3:
		SetAnaPtEngLmtParaById( point, data, RW_HI_RANGE );
		break;

	case	4:
		SetAnaPtEngLmtParaById( point, data, RW_LO_RANGE );
		break;

	case	5:
		SetAnaPtEngLmtParaById( point, data, RW_ZERO );
		break;

	case	6:
		SetAnaPtEngLmtParaById( point, data, RW_COMPSATE );
		break;

	case	7:
		SetAnaPtEngLmtParaById( point, data, RW_HI_CODE );
		break;

	case	8:
		SetAnaPtEngLmtParaById( point, data, RW_LO_CODE );
		break;

	default:
		break;
	}
}


void WriteAnaPointLmtPara(POINTER *point)
{
	char	answer[128];
	int	select;
	FLOAT	data;

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf ( DMS_STR_MODY_HI_WARN );
	printf ( DMS_STR_MODY_HI_ALARM );
	printf ( DMS_STR_MODY_HI_RECOVER );
	printf ( DMS_STR_MODY_LO_WARN );
	printf ( DMS_STR_MODY_LO_ALARM );
	printf ( DMS_STR_MODY_LO_RECOVER ); 
	printf ( DMS_STR_MODY_TR_WARN );
	printf ( DMS_STR_MODY_TR_RECOVER ); 
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_ANA_LMT_SEL );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>8)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%f", &data);
	gets( answer );

	switch( select )
	{
	case	1:
		SetAnaPtEngLmtParaById( point, data, RW_HI_WARN );
		break;

	case	2:
		SetAnaPtEngLmtParaById( point, data, RW_HI_ALARM );
		break;

	case	3:
		SetAnaPtEngLmtParaById( point, data, RW_HI_RECOVER );
		break;

	case	4:
		SetAnaPtEngLmtParaById( point, data, RW_LO_WARN );
		break;

	case	5:
		SetAnaPtEngLmtParaById( point, data, RW_LO_ALARM );
		break;

	case	6:
		SetAnaPtEngLmtParaById( point, data, RW_LO_RECOVER );
		break;

	case	7:
		SetAnaPtEngLmtParaById( point, data, RW_TR_WARN );
		break;

	case	8:
		SetAnaPtEngLmtParaById( point, data, RW_TR_RECOVER );
		break;

	default:
		break;
	}
}


void WriteAnaPoint(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	float	fvalue;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_VALUE );
	printf( DMS_STR_MODY_STATUS );
	printf( DMS_STR_MODY_IOST );
	printf( DMS_STR_MODY_IOATTR );
	printf( DMS_STR_MODY_CTRLADDR );
	printf( DMS_STR_MODY_IO_GROUP );
	printf( DMS_STR_MODY_CMD_MSGSEL );
	printf( DMS_STR_MODY_ENG_PARA );
	printf( DMS_STR_MODY_LMT_PARA );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_ANA_INPUT );
	
	scanf("%d", &select);
	gets( answer );

	switch( select )
	{
	case	1:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%f", &fvalue);
		gets( answer );
		if(-1==ReadEntryById( point, &common_aa ))
		{
			printf("\nNo this point in database" );
			break;
		}
		wr_var.anlg= common_aa.var.anlg.value;
		wr_var.anlg.data= (int)(fvalue*1000);
		GetSysTime(&time);
		SetAnaPtfValueById( point, fvalue, time);
		break;

	case	2:
		WriteAnaPointStatus(point);
		break;

	case	3:
		WritePointIost(point);
		break;

	case	4:
		WritePointIoattr(point);
		break;

	case	5:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetPtCtrlAddrById( point, (USHORT)input );
		break;

	case	6:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetPtIoGroupById( point, (USHORT)input );
		break;

	case	7:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetPtCmdMsgSelById( point, (UCHAR)input );
		break;

	case	8:
		WriteAnaPointEngPara(point);
		break;

	case	9:
		WriteAnaPointLmtPara(point);
		break;

	default:
		printf("\nunknown select");
		break;
	}
}


void WriteImpPointStatus(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_IMP_MAN_SET );
	printf( DMS_STR_MODY_IMP_CHAN_FAIL );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IMP_STATUS );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>2)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%d", &input);
	gets( answer );

	if(-1==ReadEntryById( point, &common_aa ))
	{
		printf("\nNo this point in database" );
		return;
	}

	GetSysTime(&time);
	switch( select )
	{
	case	1:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.man_set= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_MAN_SET );
		break;

	case	2:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.chan_fail= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_CHAN_FAIL );
		break;

	default:
		break;
	}
}

void WriteImpPointData(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select;
	float	fValue, imp_cvt;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_INPUT_DAY_PPEAK );		
	printf( DMS_STR_INPUT_MONTH_PPEAK );		
	printf( DMS_STR_INPUT_YEAR_PPEAK );		
	printf( DMS_STR_INPUT_DAY_PEAK );			
	printf( DMS_STR_INPUT_MONTH_PEAK );		
	printf( DMS_STR_INPUT_YEAR_PEAK );			
	printf( DMS_STR_INPUT_DAY_VALLEY );		
	printf( DMS_STR_INPUT_MONTH_VALLEY );		
	printf( DMS_STR_INPUT_YEAR_VALLEY );		
	printf( DMS_STR_INPUT_DAY_AVER );			
	printf( DMS_STR_INPUT_MONTH_AVER );		
	printf( DMS_STR_INPUT_YEAR_AVER );			
	printf( DMS_STR_INPUT_DAY_SUM );			
	printf( DMS_STR_INPUT_MONTH_SUM );			
	printf( DMS_STR_INPUT_YEAR_SUM );			
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IMP_DATA_SEL );

	scanf("%d", &select);
	gets( answer );
	if(select<=0 || select>15)
		return;

	printf( DMS_STR_INPUT_VALUE );
	scanf("%f", &fValue);
	gets( answer );

	if(-1==ReadEntryById( point, &common_aa ))
	{
		printf("\nNo this point in database" );
		return;
	}

	GetSysTime(&time);
	imp_cvt= common_aa.var.imp.entry.eng_para.CT*common_aa.var.imp.entry.eng_para.PT
		*common_aa.var.imp.entry.eng_para.ENG_CVT;
	switch( select )
	{
	case	1:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_DAY_PPEAK;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	2:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_MONTH_PPEAK;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	3:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_YEAR_PPEAK;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	4:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_DAY_PEAK;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	5:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_MONTH_PEAK;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	6:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_YEAR_PEAK;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	7:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_DAY_VALLY;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	8:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_MONTH_VALLY;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	9:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_YEAR_VALLY;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	10:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_DAY_NORMAL;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	11:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_MONTH_NORMAL;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	12:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_YEAR_NORMAL;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	13:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_DAY_SUM;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	14:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_MONTH_SUM;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	15:
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_D_YEAR_SUM;
		wr_var.imp.iValue= (int)(fValue/imp_cvt);
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	default:
		break;
	}
}

void WriteImpPoint(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	float	fvalue;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_IMP_VALUE );
	printf( DMS_STR_MODY_STATUS );
	printf( DMS_STR_MODY_IOST );
	printf( DMS_STR_MODY_IOATTR );
	printf( DMS_STR_MODY_IMP_IO_GROUP );
	printf( DMS_STR_MODY_BASE_VALUE );
	printf( DMS_STR_MODY_D_ACCUM );
	printf( DMS_STR_MODY_IMP_ENG_CVT );
	printf( DMS_STR_MODY_IMP_DATA );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_IMP_INPUT );
	
	scanf("%d", &select);
	gets( answer );

	if(-1==ReadEntryById( point, &common_aa ))
	{
		printf("\nNo this point in database" );
		return;
	}

	switch( select )
	{
	case	1:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		GetSysTime(&time);
		wr_var.imp.status= common_aa.var.imp.entry.status;
		wr_var.imp.status.option= RW_IMP_ACCUM;
		wr_var.imp.iValue= input;
		SetPtValueIostById( point, wr_var, iost, time, RW_RT_VALUE );
		break;

	case	2:
		WriteImpPointStatus(point);
		break;

	case	3:
		WritePointIost(point);
		break;

	case	4:
		WritePointIoattr(point);
		break;

	case	5:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		SetPtIoGroupById( point, (USHORT)input );
		break;

	case	6:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%d", &input);
		gets( answer );
		common_aa.var.imp.entry.base_value= input;
		GetSysTime(&time);
		SetImpPtVarById( point, common_aa.var.imp.entry );
		break;

	case	7:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%f", &fvalue);
		gets( answer );
		common_aa.var.imp.entry.d_accum= fvalue;
		GetSysTime(&time);
		SetImpPtVarById( point, common_aa.var.imp.entry );
		break;

	case	8:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%f", &fvalue);
		gets( answer );
		common_aa.var.imp.entry.eng_para.ENG_CVT= fvalue;
		GetSysTime(&time);
		SetImpPtVarById( point, common_aa.var.imp.entry );
		break;

	case	9:
		WriteImpPointData(point);
		break;

	default:
		printf("\nunknown select");
		break;
	}
}



void WriteCurvePoint(POINTER *point)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	char	answer[128];
	int		select, input;
	float	fvalue;
	IOST	iost={0};
	DATE_TIME	time={0};

	printf( DMS_STR_MODY_ITEM_SELECT );
	printf( DMS_STR_MODY_CURVE_VALUE );
	printf( DMS_STR_MODY_CURVE_ENG_CVT );
	printf( DMS_STR_MODY_IOST );
	printf( DMS_STR_MODY_IOATTR );
	printf( DMS_STR_MODY_CURVE_HIRANGE );
	printf( DMS_STR_MODY_CURVE_LORANGE );
	printf( DMS_STR_MODY_CURVE_IO_GROUP );
	printf( DMS_STR_NO_SELECT );
	printf( DMS_STR_MODY_CURVE_INPUT );
	
	scanf("%d", &select);
	gets( answer );

	if(-1==ReadEntryById( point, &common_aa ))
	{
		printf("\nNo this point in database" );
		return;
	}

	switch( select )
	{
	case	1:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%f", &fvalue);
		gets( answer );
		printf( DMS_STR_CURVE_SEGNO_INPUT );
		scanf("%d", &input);
		gets( answer );
		GetSysTime(&time);
		SetCurvePtDataById( point, &fvalue, (USHORT)input, 1, time );
		break;

	case	2:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%f", &fvalue);
		gets( answer );
		SetCurvePtEngParaById( point, fvalue, RW_ENG_CVT );		
		break;

	case	3:
		WritePointIost(point);
		break;

	case	4:
		WritePointIoattr(point);
		break;

	case	5:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%f", &fvalue);
		gets( answer );
		SetCurvePtEngParaById( point, fvalue, RW_HI_RANGE );		
		break;

	case	6:
		printf( DMS_STR_INPUT_VALUE );
		scanf("%f", &fvalue);
		gets( answer );
		SetCurvePtEngParaById( point, fvalue, RW_LO_RANGE );		
		break;

	default:
		printf("\nunknown select");
		break;
	}
}


void	WriteDevObjPoint(POINTER *point)
{
	DMS_COMMON	common_aa;
	char	answer[128];
	int		select, input;

	if(-1==ReadEntryById( point, &common_aa ))
	{
		printf("\nNo this point in database" );
		return;
	}

	printf( DMS_STR_MODY_OBJ_CONDC0	);	
	printf( DMS_STR_MODY_OBJ_CONDC1	);		
	printf( DMS_STR_MODY_OBJ_SWGF_N	);	
	printf( DMS_STR_MODY_OBJ_P0	);
	printf( DMS_STR_MODY_OBJ_P1	);			
	printf( DMS_STR_MODY_OBJ_A0	);			
	printf( DMS_STR_MODY_OBJ_A1	);			
	printf( DMS_STR_MODY_OBJ_C0	);			
	printf( DMS_STR_MODY_OBJ_C1	);			
	printf( DMS_STR_MODY_OBJ_MA	);			
	printf( DMS_STR_MODY_OBJ_LOCALM	);		
	printf( DMS_STR_MODY_OBJ_LCULM	);		
	printf( DMS_STR_MODY_OBJ_REMOTE	);		
	printf( DMS_STR_MODY_OBJ_O0	);			
	printf( DMS_STR_MODY_OBJ_O1	);			
	printf( DMS_STR_MODY_OBJ_ORDERM	);		
	printf( DMS_STR_MODY_OBJ_POSD0	);		
	printf( DMS_STR_MODY_OBJ_POSD1	);		
	printf( DMS_STR_MODY_OBJ_SEL	);		

	scanf("%d", &select);
	gets( answer );
	if(select<1 || select>18)
	{
		printf("\nunknown select: %d", select);
		return;
	}

	printf( DMS_STR_INPUT_VALUE );
	scanf("%d", &input);
	gets( answer );

	switch( select )
	{
	case	1:
		common_aa.var.devobj.fb_status1.condc0= input;
		break;

	case	2:
		common_aa.var.devobj.fb_status1.condc1= input;
		break;

	case	3:
		common_aa.var.devobj.fb_status1.swgf_n= input;
		break;

	case	4:
		common_aa.var.devobj.fb_status1.p0= input;
		break;

	case	5:
		common_aa.var.devobj.fb_status1.p1= input;
		break;

	case	6:
		common_aa.var.devobj.fb_status1.a0= input;
		break;

	case	7:
		common_aa.var.devobj.fb_status1.a1= input;
		break;

	case	8:
		common_aa.var.devobj.fb_status1.c0= input;
		break;

	case	9:
		common_aa.var.devobj.fb_status1.c1= input;
		break;

	case	10:
		common_aa.var.devobj.fb_status1.ma= input;
		break;

	case	11:
		common_aa.var.devobj.fb_status1.localm= input;
		break;

	case	12:
		common_aa.var.devobj.fb_status1.lculm= input;
		break;

	case	13:
		common_aa.var.devobj.fb_status1.remote= input;
		break;

	case	14:
		common_aa.var.devobj.fb_status2.q0= input;
		break;

	case	15:
		common_aa.var.devobj.fb_status2.q1= input;
		break;

	case	16:
		common_aa.var.devobj.fb_status2.orderm= input;
		break;

	case	17:
		common_aa.var.devobj.fb_status2.posd0= input;
		break;

	case	18:
		common_aa.var.devobj.fb_status2.posd1= input;
		break;
	}
	SetDevObjPtVarById(point, common_aa.var.devobj);
}


void	GetTimetoStr(DATE_TIME *time, char *str)
{
	sprintf(str, "%4d-%02d-%02d %02d:%02d:%02d:%03d", time->year, time->month, time->day,
		time->hour, time->minute, time->second, time->ms);
}



void	PrintEntryFixed( DMS_COMMON *common_aa )
{
	char	str[256];

	printf("\n%d.%d.%d.%d update_src= %d", common_aa->point.stn_id, common_aa->point.dev_id,
		common_aa->point.data_type, common_aa->point.pt_id, common_aa->fixed.update_src);
	GetPtNameById( &common_aa->point, str, NULL );
	printf("\n%s", str);
	GetPtLongnameById(&common_aa->point, str);
	printf("\n%s", str);

	/*IOST*/
	printf("\nviewPoint= %d, scanE= %d, almE= %d, almNoAck= %d\
		\nctrlLock= %d,	updated= %d, almRepeat= %d, tagState= %d", 
		common_aa->fixed.iost.viewPoint, common_aa->fixed.iost.scanEnable, 
		common_aa->fixed.iost.almEnable, common_aa->fixed.iost.almNoAck, 
		common_aa->fixed.iost.ctrlLock, common_aa->fixed.iost.noUpdate,
		common_aa->fixed.iost.almRepeat, common_aa->fixed.iost.tagState);
	
	/*IOATTR*/
	printf("\nprocType= %d, almCba= %d, statistic= %d, fault= %d \
		\nfail= %d, ctrl= %d, soe= %d, trendAlm= %d, normalState= %d\
		\nkeySignal= %d, failRec= %d, audAlm= %d, normalClose= %d \
		\ndoublePt= %d, sysVirtual= %d, lcuVirtual= %d, cascade= %d \
		\nhdbRec= %d, hdbCurve= %d, onAlm= %d, offAlm= %d, hdbPeriod= %d", 
		common_aa->fixed.ioattr.procType, common_aa->fixed.ioattr.almCba,
		common_aa->fixed.ioattr.statistic, common_aa->fixed.ioattr.fault,
		common_aa->fixed.ioattr.fail, common_aa->fixed.ioattr.ctrl,
		common_aa->fixed.ioattr.soe, common_aa->fixed.ioattr.trendAlm,
		common_aa->fixed.ioattr.normalState,
		common_aa->fixed.ioattr.keySignal, common_aa->fixed.ioattr.faultRec,
		common_aa->fixed.ioattr.audAlm, common_aa->fixed.ioattr.normalClose,
		common_aa->fixed.ioattr.doublePt, common_aa->fixed.ioattr.sysVirtual,
		common_aa->fixed.ioattr.lcuVirtual, common_aa->fixed.ioattr.cascade,
		common_aa->fixed.ioattr.hdbRec, common_aa->fixed.ioattr.hdbCurve,
		common_aa->fixed.ioattr.onAlm, common_aa->fixed.ioattr.offAlm,
		common_aa->fixed.ioattr.hdbPeriod);

	/*SELECTOR*/
	printf("\ncmdSel= %d, almSel= %d, pair_pt= %d.%d.%d.%d", 
		common_aa->fixed.msg_sel.cmd, common_aa->fixed.msg_sel.alm,
		common_aa->fixed.pair_pt.stn_id, common_aa->fixed.pair_pt.dev_id,
		common_aa->fixed.pair_pt.data_type, common_aa->fixed.pair_pt.pt_id);

	/*HARD_ADDR*/
	printf("\nterminal= %s, cabinet_no= %s, unit_no= %s, slot_no= %s, chan_no= %s",
		common_aa->fixed.hard_addr.terminal_no, common_aa->fixed.hard_addr.cabinet_no, 
		common_aa->fixed.hard_addr.unit_no,
		common_aa->fixed.hard_addr.slot_no, common_aa->fixed.hard_addr.channel_no);

	/*OUTPUT_ADDR AND DSP_FILENAME*/
	printf("\nctrl_addr= %d, io_group= %d, dsp_filename= %s", common_aa->fixed.ctrl_addr,
		common_aa->fixed.io_group, common_aa->fixed.dsp_filename);

	printf("\n");
}


void	PrintDIVar( DMS_COMMON *common_aa )
{
	char	str[128];

	/*STATUS*/
	printf("\nrt_state= %d, state= %d, fault_tag= %d, man_set= %d, chan_fail= %d",
		common_aa->var.ind.status.rt_state, 
		common_aa->var.ind.status.state, common_aa->var.ind.status.fault_tag,
		common_aa->var.ind.status.man_set, common_aa->var.ind.status.chan_fail);

	/*UPDATE_TIME*/
	GetTimetoStr(&common_aa->var.ind.update_time, str);
	printf("\nupdate_time= %s", str);

	/*CHANGE_TIME*/
	GetTimetoStr(&common_aa->var.ind.change_time, str);
	printf("\nchange_time= %s", str);

	/*STC*/
	printf("\nnormal_open= %d, normal_close= %d, fault_open= %d, fault_close= %d",
		common_aa->var.ind.stc.norm_open, common_aa->var.ind.stc.norm_close,
		common_aa->var.ind.stc.fault_open, common_aa->var.ind.stc.fault_close);
}

void	PrintAIVar( DMS_COMMON *common_aa )
{
	char	str[128];

	/*STATUS*/
	printf("\nvalue= %0.3f, fValue= %.3f, ctrl_block= %d, chan_fail= %d \
		\nlmt_state= %d, trend_state= %d, overflow= %d",
		common_aa->var.anlg.value.data/1000., common_aa->var.anlg.fValue,
		common_aa->var.anlg.value.status.ctrl_block, 
		common_aa->var.anlg.value.status.chan_fail, common_aa->var.anlg.value.status.lmt_state,
		common_aa->var.anlg.value.status.trend_state, common_aa->var.anlg.value.status.overflow);

	/*UPDATE_TIME AND UNITNAME*/
	GetTimetoStr(&common_aa->var.anlg.update_time, str);
	printf("\nupdate_time= %s, unitName= %s", str, common_aa->var.anlg.unitName);

	/*ENG_PARA*/
	printf("\nD_BAND= %.3f, ENG_CVT= %.3f, HI_RANGE= %.3f \
		\nLO_RANGE= %.3f, ZERO= %.3f, COMPSATE= %.3f\
		\nHI_CODE= %f, LO_CODE= %f",
		common_aa->var.anlg.eng_para.D_BAND, common_aa->var.anlg.eng_para.ENG_CVT,
		common_aa->var.anlg.eng_para.HI_RANGE, common_aa->var.anlg.eng_para.LO_RANGE,
		common_aa->var.anlg.eng_para.ZERO, common_aa->var.anlg.eng_para.COMPSATE,
		common_aa->var.anlg.eng_para.HI_CODE, common_aa->var.anlg.eng_para.LO_CODE);

	/*LMT_PARA*/
	printf("\nHI_ALM= %.3f, HI_WARN= %.3f, HI_RECOVER= %.3f \
		\nLO_ALM= %.3f, LO_WARN= %.3f, LO_RECOVER= %.3f \
		\nTR_WARN= %.3f, TR_RECOVER= %.3f",
		common_aa->var.anlg.lmt_para.HI_ALARM, common_aa->var.anlg.lmt_para.HI_WARN,
		common_aa->var.anlg.lmt_para.HI_RECOVER, common_aa->var.anlg.lmt_para.LO_ALARM,
		common_aa->var.anlg.lmt_para.LO_WARN, common_aa->var.anlg.lmt_para.LO_RECOVER,
		common_aa->var.anlg.lmt_para.TR_WARN, common_aa->var.anlg.lmt_para.TR_RECOVER);
}


void	PrintCalcVar( DMS_COMMON *common_aa )
{
	char	str[128];

	/*STATUS*/
	printf("\nvalue= %d, fValue= %f, ctrl_block= %d, chan_fail= %d \
		\nlmt_state= %d, trend_state= %d, overflow= %d",
		common_aa->var.anlg.value.data, common_aa->var.anlg.fValue,
		common_aa->var.anlg.value.status.ctrl_block, 
		common_aa->var.anlg.value.status.chan_fail, common_aa->var.anlg.value.status.lmt_state,
		common_aa->var.anlg.value.status.trend_state, common_aa->var.anlg.value.status.overflow);

	/*UPDATE_TIME AND UNITNAME*/
	GetTimetoStr(&common_aa->var.anlg.update_time, str);
	printf("\nupdate_time= %s, unitName= %s", str, common_aa->var.anlg.unitName);

	/*ENG_PARA*/
	printf("\nD_BAND= %.3f", common_aa->var.anlg.eng_para.D_BAND);

	/*LMT_PARA*/
	printf("\nHI_ALM= %.3f, HI_WARN= %.3f, HI_RECOVER= %.3f \
		\nLO_ALM= %.3f, LO_WARN= %.3f, LO_RECOVER= %.3f \
		\nTR_WARN= %.3f, TR_RECOVER= %.3f",
		common_aa->var.anlg.lmt_para.HI_ALARM, common_aa->var.anlg.lmt_para.HI_WARN,
		common_aa->var.anlg.lmt_para.HI_RECOVER, common_aa->var.anlg.lmt_para.LO_ALARM,
		common_aa->var.anlg.lmt_para.LO_WARN, common_aa->var.anlg.lmt_para.LO_RECOVER,
		common_aa->var.anlg.lmt_para.TR_WARN, common_aa->var.anlg.lmt_para.TR_RECOVER);
}


void	PrintImpVar( DMS_COMMON *common_aa )
{
	char	str[128];

	/*VALUE AND STATUS*/
	printf("\nrt_value= %d, base_value= %d, d_accum= %f \
		\nman_set= %d, chan_fail= %d \
		\nENG_CVT= %f, PT= %f, CT= %f, CONSTANT= %u \
		\nDAY_RATE= %d, YEAR_RATE= %d \
		\ninit_day= %d, init_month= %d, init_year= %d \
		\nlast_day= %d, last_month= %d, last_year= %d \
		\npeak_day= %d, peak_month= %d, peak_year= %d \
		\nvalley_day= %d, valley_month= %d, valley_year= %d \
		\nnormal_day= %d, normal_month= %d, normal_year= %d \
		\nsum_day= %d, sum_month= %d, sum_year= %d \
		\nppeak_day= %d, ppeak_month= %d, ppeak_year= %d \
		\ninit_day= %.3f, init_month= %.3f, init_year= %.3f \
		\nlast_day= %.3f, last_month= %.3f, last_year= %.3f \
		\npeak_day= %.3f, peak_month= %.3f, peak_year= %.3f \
		\nvalley_day= %.3f, valley_month= %.3f, valley_year= %.3f \
		\nnormal_day= %.3f, normal_month= %.3f, normal_year= %.3f \
		\nsum_day= %.3f, sum_month= %.3f, sum_year= %.3f \
		\nppeak_day= %.3f, ppeak_month= %.3f, ppeak_year= %.3f",
		common_aa->var.imp.entry.rt_value, common_aa->var.imp.entry.base_value, common_aa->var.imp.entry.d_accum, 
		common_aa->var.imp.entry.status.man_set,	common_aa->var.imp.entry.status.chan_fail, 
		common_aa->var.imp.entry.eng_para.ENG_CVT, common_aa->var.imp.entry.eng_para.PT, 
		common_aa->var.imp.entry.eng_para.CT, common_aa->var.imp.entry.eng_para.CONSTANT,
		common_aa->var.imp.entry.eng_para.DAY_RATE, common_aa->var.imp.entry.eng_para.YEAR_RATE,

		common_aa->var.imp.entry.init.day, common_aa->var.imp.entry.init.month, common_aa->var.imp.entry.init.year,
		common_aa->var.imp.entry.last.day, common_aa->var.imp.entry.last.month, common_aa->var.imp.entry.last.year,
		common_aa->var.imp.entry.peak.day, common_aa->var.imp.entry.peak.month, common_aa->var.imp.entry.peak.year,
		common_aa->var.imp.entry.valley.day, common_aa->var.imp.entry.valley.month, common_aa->var.imp.entry.valley.year,
		common_aa->var.imp.entry.normal.day, common_aa->var.imp.entry.normal.month, common_aa->var.imp.entry.normal.year,
		common_aa->var.imp.entry.sum.day, common_aa->var.imp.entry.sum.month, common_aa->var.imp.entry.sum.year,
		common_aa->var.imp.entry.ppeak.day, common_aa->var.imp.entry.ppeak.month, common_aa->var.imp.entry.ppeak.year,
		common_aa->var.imp.init.day, common_aa->var.imp.init.month, common_aa->var.imp.init.year,
		common_aa->var.imp.last.day, common_aa->var.imp.last.month, common_aa->var.imp.last.year,
		common_aa->var.imp.peak.day, common_aa->var.imp.peak.month, common_aa->var.imp.peak.year,
		common_aa->var.imp.valley.day, common_aa->var.imp.valley.month, common_aa->var.imp.valley.year,
		common_aa->var.imp.normal.day, common_aa->var.imp.normal.month, common_aa->var.imp.normal.year,
		common_aa->var.imp.sum.day, common_aa->var.imp.sum.month, common_aa->var.imp.sum.year,
		common_aa->var.imp.ppeak.day, common_aa->var.imp.ppeak.month, common_aa->var.imp.ppeak.year);

	/*UPDATE_TIME*/
	GetTimetoStr(&common_aa->var.imp.entry.update_time, str);
	printf("\nupdate_time= %s", str);
}


void	PrintCharaVar( DMS_COMMON *common_aa )
{
	char	str[128];

	/*CHARA_DATA*/
	printf("\nmax_no= %d, min_no= %d, max_rate_no= %d, max_rate_no= %d \
		\nmax_value= %.3f, min_value= %.3f, avg_value= %.3f \
		\nstd_value= %.3f, max_rate= %.3f, min_rate= %.3f",
		common_aa->var.chara.chara_data.max_no, common_aa->var.chara.chara_data.min_no,
		common_aa->var.chara.chara_data.max_rate_no, common_aa->var.chara.chara_data.min_rate_no,
		common_aa->var.chara.chara_data.max_value, common_aa->var.chara.chara_data.min_value,
		common_aa->var.chara.chara_data.avg_value, common_aa->var.chara.chara_data.std_value,
		common_aa->var.chara.chara_data.max_rate, common_aa->var.chara.chara_data.min_rate);

	/*START_PT*/
	printf("\nstart_pt_name= %s, pt_num= %d", 
		common_aa->var.chara.start_pt_name, common_aa->var.chara.pt_num);

	/*UPDATE_TIME*/
	GetTimetoStr(&common_aa->var.chara.update_time, str);
	printf("\nupdate_time= %s", str);
}


void	PrintCurveVar( DMS_COMMON *common_aa )
{
	char	str[128];
	int		i;

	/*ENG_PARA*/
	printf("\nHI_RANGE= %.3f, LO_RANGE= %.3f, ENG_CVT= %.3f",
		common_aa->var.curve.eng_para.HI_RANGE, common_aa->var.curve.eng_para.LO_RANGE,
		common_aa->var.curve.eng_para.ENG_CVT);

	/*SEG_NUM*/
	printf("\nseg_num= %d\n", common_aa->var.curve.seg_num);

	/*CHARA_DATA*/
	for(i=0; i<common_aa->var.curve.seg_num; i++)
	{
		printf("%.3f  ", common_aa->var.curve.data.value[i]);
		if(!((i+1)%10))
			printf("\n");
	}

	/*UPDATE_TIME*/
	GetTimetoStr(&common_aa->var.curve.update_time, str);
	printf("\nupdate_time= %s", str);
}


void	PrintDevObjVar( DMS_COMMON *common_aa )
{
	char	str[128], str1[128];

	/*USR_NAME AND HOST_ID*/
	printf("\nusr_name= %s, host_id= %d", 
		common_aa->var.devobj.usr_name, common_aa->var.devobj.host_id);

	/*UPDATE_TIME AND CMD_TIME*/
	GetTimetoStr(&common_aa->var.devobj.update_time, str);
	GetTimetoStr(&common_aa->var.devobj.cmd_time, str1);
	printf("\nupdate_time= %s, cmd_time= %s", str, str1);

	/*CMD_STATUS AND DATA*/
	printf("\ncmd_opr= %d, cmd_type= %d, cmd_result= %d, \
		\ncmd_src= %d, cmd_data= %d(%.3f)",
		common_aa->var.devobj.status.opr, common_aa->var.devobj.status.state,
		common_aa->var.devobj.status.result, common_aa->var.devobj.status.src,
		common_aa->var.devobj.data.iValue, common_aa->var.devobj.data.fValue);

	/*FB_STATUS1*/
	printf("\ncondc0= %d, condc1= %d, ma= %d \
		\nc0= %d, c1= %d, a0= %d, a1= %d \
		\nswgf_n= %d, p0= %d, p1= %d \
		\nlocalm= %d, lculm= %d, remote= %d",
		common_aa->var.devobj.fb_status1.condc0, common_aa->var.devobj.fb_status1.condc1,
		common_aa->var.devobj.fb_status1.ma, common_aa->var.devobj.fb_status1.c0,
		common_aa->var.devobj.fb_status1.c1, common_aa->var.devobj.fb_status1.a0,
		common_aa->var.devobj.fb_status1.a1, common_aa->var.devobj.fb_status1.swgf_n,
		common_aa->var.devobj.fb_status1.p0, common_aa->var.devobj.fb_status1.p1,
		common_aa->var.devobj.fb_status1.localm, common_aa->var.devobj.fb_status1.lculm,
		common_aa->var.devobj.fb_status1.remote);

	/*FB_STATUS2 AND PARAM*/
	printf("\no0= %d, o1= %d, orderm= %d \
		\nposd0= %d, posd1= %d, param= %d",
		common_aa->var.devobj.fb_status2.q0, common_aa->var.devobj.fb_status2.q1,
		common_aa->var.devobj.fb_status2.orderm, common_aa->var.devobj.fb_status2.posd0,
		common_aa->var.devobj.fb_status2.posd1, common_aa->var.devobj.param);
}


