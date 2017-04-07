#include "calldblib.h"
#include "dms_functions.h"
Calldblib::Calldblib(QString dbpath)
{	
	m_dbpath=dbpath;
	initclass();
}
Calldblib::~Calldblib()
{
	UnMountDB();
}

void Calldblib::initclass()
{
	
}
int	Calldblib::MountDB()
{
	int loadflag=-1;
	loadflag=MountDBFile(m_dbpath.toLocal8Bit().data(),0);

	//m_db_ptr=(DBS_GBL_COMMON *)MountFile(m_dbpath.toLocal8Bit().data(),sizeof(DBS_GBL_COMMON));
	//if (m_db_ptr == (DBS_GBL_COMMON*)-1)
	//{
	//	Err_log("Fail to mount database file");
	//	//h9000_error_id= H9000_ERROR_DB;
	//	return -1;
	//}
	return loadflag;
}
int Calldblib::UnMountDB()
{
	int	ret;

	ret= UnMountFile(m_db_ptr, sizeof(DBS_GBL_COMMON));
	m_db_ptr= (DBS_GBL_COMMON *)-1;

	return ret;
}

int	Calldblib::qGetPtIdByNameStr( char *ptNameStr, POINTER *point )
{
	
	return GetPtIdByNameStr(ptNameStr,point);
}

int Calldblib::qGetPtDataByStr( char *nameStr, float *fValue, unsigned int *chan_fail, POINTER *point )
{
	return GetPtDataByStr(nameStr, fValue, chan_fail, point );
}

int	Calldblib::qReadEntryByNameStr( char *ptNameStr, DMS_COMMON *common_all)
{
	return ReadEntryByNameStr( ptNameStr, common_all);
}

int	Calldblib::qGetAnaPtEngLmtParaByNameStr( char *ptNameStr, ANA_ENG_PARA *eng_para, LMT_PARA *lmt_para )
{
	return GetAnaPtEngLmtParaByNameStr( ptNameStr,  eng_para, lmt_para );
}

int	Calldblib::qInterLockFileLoad(void)
{
	return InterLockFileLoad();
}
int Calldblib::qInterLock_Judge(char *pt)
{
	return InterLock_Judge(pt);
}
void Calldblib::qInterLockFree(void)
{
	InterLockFree();
}