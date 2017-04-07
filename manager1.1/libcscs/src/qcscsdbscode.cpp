#include "qcscsdbscode.h"

#include <QCryptographicHash>

#ifdef Q_OS_WIN
#define WINDOWS_OS
#endif

#include "dms_com.h"
#include "ipc_common.h"
#include "dps_rec_def.h"
#include "dps_type_def.h"

#undef sleep

#pragma comment(linker,"/nodefaultlib:../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(linker,"/nodefaultlib:../../../dps/v4.0a/lib/ipclib.lib")
#pragma comment(linker,"/nodefaultlib:../../../lan/v4.0a/lib/lan.lib")
#pragma comment(linker,"/nodefaultlib:../../../dps/v4.0a/lib/dpslib.lib")
#pragma comment(linker,"/nodefaultlib:../../../syslog/v4.0a/lib/syslog.lib")

#include "qlogger.h"
#include "qcscsinitialize.h"

#ifdef __cplusplus
extern "C" {
extern DBS_GBL_COMMON  *db_ptr;
}
#endif

STATION_ENTRY*	getStnEntryPtrById(UCHAR stn);
GROUP_ENTRY*	getGroupEntryPtrById(UCHAR stn, UCHAR grp);

CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const STATION_ENTRY& entry);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const GROUP_ENTRY& entry);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const IOGROUP_DEF& entry);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const IOGROUP_INFO& entry);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const POINTER& pt);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const ENTRY_FIXED& entry);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const HARD_ADDR& addr);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const HARD_ADDR& addr);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const MSG_SELECTOR& msg);

CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const DATE_TIME& datetime);
CSCS_EXPORT QDataStream& operator<<(QDataStream& out, const DB_HEAD_BLOCK& head);

QCscsDbsCode::QCscsDbsCode()
{

}

QCscsDbsCode::~QCscsDbsCode()
{

}

QCscsDbsCode* QCscsDbsCode::instance(bool initIgnored/* =false */)
{
	if (!initIgnored && !QCscsInitialize::instance()->initialized())
	{
		return NULL;
	}

	static QCscsDbsCode* pInstance=new QCscsDbsCode;

	return pInstance;
}

CscsDatabase QCscsDbsCode::database()
{
	uint nNumStn;
	char longname[256];

	CscsDatabase db;

	GetStationNum(&nNumStn);
	for(uint i=1; i<=nNumStn; i++)
	{ 
		UCHAR stn_id;

		if(-1 == GetStnIdByIndex(i, &stn_id))
			continue;

		if( GetStnCommentById((UCHAR)stn_id, longname) == -1)
			continue;

		CscsStation station;
		station.stnId=stn_id;
		station.longname=QString::fromLocal8Bit(longname);

		uint nNumGrp;
		if(-1 != GetGroupNumById((UCHAR)stn_id, &nNumGrp))
		{
			for(uint j=1; j<=nNumGrp; j++) 
			{  		
				if(-1 == GetGroupCommentById((UCHAR)stn_id, (UCHAR)j, longname))
					continue;

				CscsGroup group;
				group.grpId=(UCHAR)j;
				group.longname=QString::fromLocal8Bit(longname);

				uint nNumRec;
				if(-1 != GetRecordNumById(stn_id, group.grpId, &nNumRec))
				{
					uint nNumEntry= 0;
					for(uint k=0, nNum=0; nNum<nNumRec; k++) 
					{  		
						if(k>=MAX_DATATYPE_NUM)
							break;

						if(-1 == GetEntryNumById(stn_id, group.grpId, (UCHAR)k, &nNumEntry))
							continue;

						nNum++;

						GetRecLongnameByType((UCHAR)k, (char *)longname);

						CscsDataType dataType;
						dataType.dataType=(UCHAR)k;
						dataType.longname=QString::fromLocal8Bit(longname);

						group.types.append(dataType);
					}
				}

				station.groups.append(group);
			}
		}

		db.stations.append(station);
	}

	return db;
}

STATION_ENTRY* getStnEntryPtrById(UCHAR stn)
{
	STATION_ENTRY* pStn=NULL;
	for(uint i=0; i<db_ptr->Head_Union.head.stn_num; i++)
	{
		if(stn == db_ptr->Station_Union.StationEntry[i].stnId)
		{
			pStn=&db_ptr->Station_Union.StationEntry[i];
			break;
		}
	}

	return pStn;
}

GROUP_ENTRY* getGroupEntryPtrById(UCHAR stn, UCHAR grp)
{
	GROUP_ENTRY* pGroup=NULL;

	STATION_ENTRY* pStn=getStnEntryPtrById(stn);
	if (pStn == NULL)
	{
		return NULL;
	}

	uint grp_num= pStn->grp_num;
	uint grp_offset= pStn->grp_offset;

	if(grp<1 || (uint)(grp-1)>grp_num)
	{
		return NULL;
	}

	pGroup=&db_ptr->Group_Union.GroupEntry[grp_offset+grp-1];
	
	return pGroup;
}

int QCscsDbsCode::command(const QString& cmd, const QVector<QString>& params)
{
	int nRet=CscsCommandNoExisted;

	if (cmd == H9000_FUNCTION_UPDATE_DATABASE_FILE)
	{
		if (!params.isEmpty())
		{
			QString strPath=params[0];

			nRet=CscsCommandFailed;

			if (QCscsInitialize::mountDB())
			{
				logplusInfo(LIBCSCS_LOGGER_STR, QString("Execute function: %1(%2,TRUE)").arg(cmd).arg(strPath));

				if (UpdateDatabaseFile(strPath.toLocal8Bit().data(), TRUE) == -1)
				{
					nRet=CscsCommandFailed;
				}
				else
				{
					nRet=CscsCommandSuccess;
				}

				logplusInfo(LIBCSCS_LOGGER_STR, QString("Execute function: %1(%2,TRUE) finished.").arg(cmd).arg(strPath));

				QCscsInitialize::unmountDB();
			}
		}
		else
		{
			nRet=CscsCommandParamError;
		}
	}

	return nRet;
}

int QCscsDbsCode::code(QByteArray& buffer, CscsEigenvalueType type, ...)
{
	va_list ap;
	va_start(ap, type);

	Q_WRITE_ONLY_DATA_STREAM(stream, buf);
	int nFlag=CscsDataSourceNoExisted;

	switch (type)
	{
	case CscsStationEntry:
		{
			UCHAR nStn=(UCHAR)va_arg(ap, int);

			STATION_ENTRY* pStn=getStnEntryPtrById(nStn);

			if(NULL != pStn)
			{
				nFlag=CscsDataSourceFound;
				stream<<*pStn;
			}
		}
		break;
	case CscsGroupEntry:
		{
			UCHAR nStn=(UCHAR)va_arg(ap, int);
			UCHAR nGrp=(UCHAR)va_arg(ap, int);

			GROUP_ENTRY* pGrp=getGroupEntryPtrById(nStn, nGrp);
			if (NULL != pGrp)
			{
				nFlag=CscsDataSourceFound;
				stream<<*pGrp;
			}
		}
		break;
	case CscsRecordEntry:
		{
			UCHAR nStn=(UCHAR)va_arg(ap, int);
			UCHAR nGrp=(UCHAR)va_arg(ap, int);
			UCHAR nDataType=(UCHAR)va_arg(ap, int);

			uint nNumRecord;
			if(-1 != GetEntryNumById(nStn, nGrp, nDataType,&nNumRecord))
			{
				POINTER pt;
				pt.stn_id=nStn;
				pt.dev_id=nGrp;
				pt.data_type=nDataType;

				DMS_COMMON dms;

				nFlag=CscsDataSourceFound;

				for (uint i=0; i<nNumRecord; i++)
				{
					pt.pt_id=(USHORT)i ;

					if( -1 != ReadEntryById(&pt, &dms) )
					{
						stream<<dms.point<<dms.fixed;
					}
				}
			}
		}
		break;
	default:
		logplusError(LIBCSCS_LOGGER_STR, QString("Invalid eigenvalue type %1").arg(type));
		break;
	}

	va_end(ap);

	if (nFlag == CscsDataSourceFound)
	{
		buffer=QCryptographicHash::hash(buf, QCryptographicHash::Md5);
	}

	return nFlag;
}

QDataStream& operator<<(QDataStream& out, const STATION_ENTRY& entry)
{
	out<<entry.stnName<<entry.stnLongname<<entry.stnComment;

#ifdef EXTEND_LANGUAGE_SYSTEM
	out<<entry.stnExLongname<<entry.stnExComment;
#endif

	out<<entry.stnType<<entry.stnId<<entry.sysLcuId<<entry.grp_num<<entry.grp_offset;

	return out;
}

QDataStream& operator<<(QDataStream& out, const GROUP_ENTRY& entry)
{
	out<<entry.grpName<<entry.grpLongname<<entry.grpComment<<entry.grpTypeName;

#ifdef EXTEND_LANGUAGE_SYSTEM
	out<<entry.grpExLongname<<entry.grpExComment;
#endif

	out<<entry.grpId<<entry.sysLcuFlag<<entry.rec_num<<entry.rec_offset<<entry.hash_key<<entry.iogroup_def;

	return out;
}

QDataStream& operator<<(QDataStream& out, const IOGROUP_DEF& entry)
{
	out<<entry.num;
	for (int i=0; i<entry.num; i++)
	{
		out<<entry.iogroup[i];
	}

	return out;
}

QDataStream& operator<<(QDataStream& out, const IOGROUP_INFO& entry)
{
	out<<entry.ioGroup<<entry.comment;

#ifdef EXTEND_LANGUAGE_SYSTEM
	out<<entry.exComment;
#endif

	out<<entry.ioGroupStatus;

	return out;
}

QDataStream& operator<<(QDataStream& out, const POINTER& pt)
{
	out<<pt.stn_id<<pt.dev_id<<pt.data_type<<pt.pt_id;

	return out;
}

QDataStream& operator<<(QDataStream& out, const ENTRY_FIXED& entry)
{
	out<<entry.entryName<<entry.entryLongname;

#ifdef EXTEND_LANGUAGE_SYSTEM
	out<<entry.entryExLongname;
#endif

	out<<entry.dsp_filename<<entry.hard_addr<<entry.io_group/*<<*(USHORT*)(&entry.iost)<<*(UINT*)(&entry.ioattr)*/<<entry.pair_pt<<entry.ctrl_addr
		<<entry.msg_sel<<entry.hash_index<<entry.hash_next<<entry.hash_key;

	return out;
}

QDataStream& operator<<(QDataStream& out, const HARD_ADDR& addr)
{
	out<<addr.cabinet_no<<addr.unit_no<<addr.slot_no<<addr.channel_no<<addr.terminal_no;

	return out;
}

QDataStream& operator<<(QDataStream& out, const MSG_SELECTOR& msg)
{
	out<<msg.alm<<msg.cmd;

	return out;
}

QDataStream& operator<<(QDataStream& out, const DATE_TIME& datetime)
{
	out<<datetime.ms<<datetime.second<<datetime.minute<<datetime.hour<<datetime.day<<datetime.month<<datetime.year;

	return out;
}

QDataStream& operator<<(QDataStream& out, const DB_HEAD_BLOCK& head)
{
	out<<head.version<<head.dbName<<head.dbLongname<<head.stn_num<<head.grp_sum<<head.rec_sum<<head.imp_sum<<head.anlg_sum<<head.status_sum
		<<head.calc_sum<<head.chara_sum<<head.curve_sum<<head.devobj_sum<<head.gen_date_time;

	return out;
}
