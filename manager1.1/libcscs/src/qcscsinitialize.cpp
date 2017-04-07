#include "qcscsinitialize.h"

#include "qlogger.h"

#ifdef Q_OS_WIN
#define WINDOWS_OS
#endif

#include "dms_com.h"
#include "ipc_common.h"
#include "dps_rec_def.h"
#include "dps_type_def.h"

#undef sleep


QCscsInitialize::QCscsInitialize()
:m_bInit(false)
,m_bAutoExit(false)
{

}

QCscsInitialize::~QCscsInitialize()
{

}

QCscsInitialize* QCscsInitialize::instance()
{
	static QCscsInitialize* pInstance=new QCscsInitialize;

	return pInstance;
}

bool QCscsInitialize::initialized()
{
	QMutexLocker locker(&m_mutex);

	return m_bInit;
}

#ifdef __cplusplus
extern "C" {
extern DBS_GBL_COMMON  *db_ptr;
}
#endif

bool QCscsInitialize::mountDB()
{
	if (MountDB(NULL)==-1 )
	{
		logplusError(LIBCSCS_LOGGER_STR, "MountDB( ) Error");

		return false; 
	}
	
	logplusDebug(LIBCSCS_LOGGER_STR, "MountDB( ) success");

	return true;
}

void QCscsInitialize::unmountDB()
{
	UnMountDB();
}

void QCscsInitialize::initialize()
{
	if (!initialized())
	{
		QMutexLocker locker(&m_mutex);

		//首先调用下列函数确保sysmgr已完成前面的拷贝远程文件和创建系统运行环境，避免锁住DATABASE.OUT
		if(LinktoSemSet(SEM_SGZY)==-1) 
			return;

		if (MountDB(NULL)==-1 )
		{
			logplusError(LIBCSCS_LOGGER_STR, "MountDB( ) Error");

			return; 
		}
		else
		{
			logplusDebug(LIBCSCS_LOGGER_STR, "MountDB( ) success");
		}

		int nNumProc=GetRunProcessNum();
		char process[MAX_PROCESS_NAME_SIZE];

		for (int i=0; i<nNumProc; i++)
		{
			if(-1 != GetRunProcess(i, process))
			{
				if (strcmp(process, H9000_SYSMGR_MANE ) == 0)
				{
					m_bAutoExit=true;

					logplusInfo(LIBCSCS_LOGGER_STR, "HMAN automatically exits following H9000!");
				}
			}
		}

		if( lan_out_init () == FALSE)
		{
			logplusError(LIBCSCS_LOGGER_STR, "Lan_init failure");

			return;
		}
		else
		{
			logplusDebug(LIBCSCS_LOGGER_STR, "Lan_init success");
		}

		m_bInit=true;
	}
}

void QCscsInitialize::destroy()
{
	if (initialized())
	{
		QMutexLocker locker(&m_mutex);

		UnMountDB();
		lan_finish();

		m_bInit=false;

		logplusInfo(LIBCSCS_LOGGER_STR, "Destroy h9000 module!");

		if (m_bAutoExit)
		{
			exit(0);
		}
	}
}

bool QCscsInitialize::sendAlarm(const QString& longname, int level, double value)
{
	DMS_COMMON common_aa;
	if (ReadEntryByNameStr(longname.toLatin1().data(), &common_aa)==-1)
	{
		logplusDebug(LIBCSCS_LOGGER_STR, QString("%1 is not found in h9000 database.!!").arg(longname));
		return false;
	}

	SP_ANA_INFO_PKG cmd;

	cmd.type_id=DPS_ANA_ALM;
	cmd.stn_id=common_aa.point.stn_id;
	cmd.dev_id=common_aa.point.dev_id;
	cmd.data_type=common_aa.point.data_type;
	cmd.pt_num=1;
	Swap2Byte(&cmd.pt_num);

	cmd.pt_id=common_aa.point.pt_id;
	Swap2Byte(&cmd.pt_id);

	memset(&cmd.ana_status, 0, sizeof(ANLG_STATUS));
	cmd.ana_status.lmt_state=(USHORT)level;
	cmd.ana_status.alm_type=1;

	int nData=(int)(value*1000);
	memcpy_s(cmd.data, _countof(cmd.data), &nData, sizeof(int));
	Swap4Byte(&cmd.data);

	char		*command;
	LAN_HEAD	cmd_head;	
	time_t	newtime;
	struct	tm	devtime;

	newtime = time(NULL);
	devtime = *localtime( &newtime);

#ifdef SUN_OS
	Swap4Byte(&newtime);
#endif
	cmd.second 		= devtime.tm_sec;
	cmd.minute 		= devtime.tm_min;
	cmd.hour 		= devtime.tm_hour;
	cmd.day 		= devtime.tm_mday;
	cmd.month		= devtime.tm_mon+1;
	cmd.year 		= devtime.tm_year+1900; 

#ifdef SUN_OS	
	Swap2Byte(&cmd.year);
#endif

	command 	= ( char *)&cmd;
	GetLocalHostId(&cmd.host_id);

	cmd_head.length 	= sizeof (cmd);	
	Swap2Byte(&cmd_head.length);

	cmd_head.src_id 	= cmd.host_id; 
	cmd_head.dest_id 	= 0;
	cmd_head.dest_stn 	= cmd.stn_id;
	cmd_head.dp_type 	= BJ_DB_TIME;
	cmd_head.packet_num 	= 0;

	lan_out ( command, cmd_head, 0);

	return true;
}
