#ifndef _CALLDBLIB_H
#define _CALLDBLIB_H
#include <QObject>
#include "callh9000dllspec.h"
#include "dms_data_def.h"
#include "dms_functions.h"

#define  DBPATH "/home/ems/h9000/cfgmgr/dat/DATABASE.OUT"
class _CALLH9000DLL_EXPORT Calldblib:QObject
{
	Q_OBJECT
public:
	Calldblib(QString dbpath);
	~Calldblib();

	int	MountDB();

	int UnMountDB();
	int	qGetPtIdByNameStr( char *ptNameStr, POINTER *point );
	int qGetPtDataByStr( char *nameStr, float *fValue, unsigned int *chan_fail, POINTER *point );

	/*Get database entry info*/
	//int	GetPtIdByNameStr( char *ptNameStr, POINTER *point );
	//int	ReadEntryById( POINTER *point , DMS_COMMON *common_all);
	int	qReadEntryByNameStr( char *ptNameStr, DMS_COMMON *common_all);
	int	qGetAnaPtEngLmtParaByNameStr( char *ptNameStr, ANA_ENG_PARA *eng_para, LMT_PARA *lmt_para );

	int	qInterLockFileLoad(void);
	int qInterLock_Judge(char *pt);
	void qInterLockFree(void);

	DMS_COMMON	dbptr;
private:
	QString m_dbpath;
	DBS_GBL_COMMON  *m_db_ptr;

	void initclass();

	


};
#endif