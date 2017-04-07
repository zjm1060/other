#ifndef _CALLINTERLOCKNAME_H
#define _CALLINTERLOCKNAME_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "callh9000dllspec.h"
#include "calldblib.h"
#include "dms_data_def.h"

#define	INTERLOCK_NUM	3000	/*1200*/
#define	OR_NUM		INTERLOCK_NUM*2
#define	AND_NUM		OR_NUM*6
#define	LAYER_NUM	20

#define	O_NAME_SIZE	60	/*36*/
#define	LONG_NAME_SIZE	100	/*72*/
/*#define NAME_SIZE	60*/	/*Replaced by POINT_NAME_SIZE*/
#define	DATA_NAME_LONG	16
/*
#define	INTERLOCK_FILE_DEF	"InterLockFile.def"
*/
#define OPEN		0
#define CLOSE		1

#define	LINE_BUF_CHAR	256

#define	TAB		9
#define CR		0xd
#define LF		0xa
/*
#ifdef WINDOWS_OS
#define LF		0xd
#else
#define LF		0xa
#endif
*/
#define STAT_1		4
#define STAT_0		0

/* Define Entry Logic operator type for each entry */
#define	BIN_LG		0
#define	LESS		1
#define	LESS_E		2
#define	LARGE		3
#define	LARGE_E		4
#define	EQUAL		5
#define	N_EQUAL		6
#define	CHANFAIL	16

/* Define Bracket Flag for each entry */
#define	BRACKET_F	(unsigned short)0xFFFF

/* Define Bracket Flag for each entry */
#define	NOT_MEET	0
#define	MEET		1
#define	NOT_MEET_FAIL	2
#define	MEETWITH_FAIL	3

#define	VERSTR	"INTLK V4.0A"
#define	VERLEN	32

typedef struct
{
	unsigned short	opr;
	short	layer;
	union
	{
		struct
		{
			short	g_num;
			short	offset;
		}group;
		unsigned int	status;
		float		value;
	}data;
	char	name[POINT_NAME_SIZE];
}I_Entry;

typedef struct
{
	short	layer;
	short	e_num;
	short	offset;
}I_Group;

typedef struct
{
	char		o_name[O_NAME_SIZE];
	short		g_num;
	short		offset;
	char		long_name[LONG_NAME_SIZE];
}InterLock;

//int	InterLockFileLoad();
//int	InterLock_Judge(char *);
//void	InterLockFileFree();
static	QString		filehead = "BEGIN:";
static	QString		pt_flag = "POINT:";
static	QString		fileflag = "INCLUDE";
static	QString		filetail = "END";

#define	GENTXTFILE	"InterLockGenName.txt"
#define BAKFILE "InterLockGen.bak"
#define ERRFILE "InterLockErr.rec"
#define INTERLOCK_DATA_FILE  "InterLockGen.dat"

#define APP_DEFINE_PATH "E:/home/ems/h9000/cfgmgr/def/"

class _CALLH9000DLL_EXPORT CallInterlockname:QObject
{
	Q_OBJECT
public:
	CallInterlockname(QString pathname);//带路径名称
	~CallInterlockname();
private:
	QString m_path; //文件所在文件夹路径名，不包括文件名
	QString m_pathname;
	int		n,r,line_num,f;
	QFile		*fp,*fpdef,*fp1,*fpb,*ferr,*ftxt;
	QString		oper ;
	QString		filebuf ;

	QString		def_filename ;
	QString		intlk_filename ;
	QString		gen_filename ;
	QString		gentxt_filename ;
	QString		bak_filename ;
	QString		err_filename ;

	DMS_COMMON	dbptr;

	short		pt_num;
	short		gt_num;
	short		et_num;

	short		gt_num_rec;
	short		et_num_rec;
	short		layer_num;
	short		gt_buf[LAYER_NUM];
	short		et_buf[LAYER_NUM];

	InterLock	interlock[INTERLOCK_NUM];
	I_Group		group[OR_NUM];
	I_Entry		entry[AND_NUM];

	Calldblib * m_pCalldblib;

	QTextStream def_fileStream;
	QTextStream err_fileStream;
	QString def_fileline ;
	int initClass();

	int open_deffile(QFile* pf,QString filepath);
	int DefFgets_Line();
	int openerr_file(QFile* pf,QString filepath);

//	QStringList linelist;//存放正则表达式分解后的

	QStringList getlinelist(QString strin,QRegExp rx);
	int checkPoint(QFile* pf);
};

#endif