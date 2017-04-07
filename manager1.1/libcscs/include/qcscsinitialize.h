#ifndef QCSCSINITIALIZE_H
#define QCSCSINITIALIZE_H

#include <QMutex>

#include "global.h"

#define H9000_SYSMGR_MANE	"sysmgr"

class CSCS_EXPORT QCscsInitialize
{
protected:
	QCscsInitialize();

public:
	~QCscsInitialize();

public:
	static QCscsInitialize* instance();

	static bool mountDB();
	static void unmountDB();

public:
	void initialize();
	void destroy();

public:
	bool sendAlarm(const QString& longname, int level, double value);

public:
	bool initialized();

private:
	QMutex		m_mutex;

	bool	m_bInit;
	bool	m_bAutoExit;	//是否自动退出
};

#endif // QCSCSINITIALIZE_H
