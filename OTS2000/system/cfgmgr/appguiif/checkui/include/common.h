#ifndef _COMMON_HEADER
#define _COMMON_HEADER
#include <QString>
#include <QStringList>
//////////////////////////////////////////////////////////////////////////
#define	INTERLOCK_NUM	3000	/*1200*/
#define	OR_NUM		INTERLOCK_NUM*2
#define	AND_NUM		OR_NUM*6
#define	LAYER_NUM	20

#define	O_NAME_SIZE	60	/*36*/
#define	LONG_NAME_SIZE	100	/*72*/
#define	DATA_NAME_LONG	16

#define OPEN		0
#define CLOSE		1

#define	LINE_BUF_CHAR	256

#define	TAB		9
#define CR		0xd
#define LF		0xa

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
//////////////////////////////////////////////////////////////////////////
#define	 FILETYPENUM 6
enum FileType
{
	FTInterLock=0,
	FTCalcu,
	FTTemp,
	FTWater,
	FTCommu,
	FTDsp,


};

/**/
typedef struct  
{
	QString filename;	//文件名
	int validflag;		//是否被#注释
	int selectflag;	//是否选中
}FilePoint;

typedef struct {
	QString filepath;
	QStringList filetype;
	QString errfile;
}Dbincfg;

typedef struct{
	QString filetype;
	QString filepath;
	QString keyword;
}Dcompath;
typedef struct {
	QString filename;
	QStringList filetype;
	int nameindex;
	int checkrange;
	int LO_RANGEindex;
	int HI_RANGEindex;

	int checkinterlock;
	int interlockIndex;

}Dcomfile;
typedef struct {
	QString iccppath;
	QString iec101path;
	QString iec104path;
	QList<Dcompath> dcompathlist;
	QString errfile;
	QList<Dcomfile> dcomfilelist;
}Dcomcfg;

typedef struct {
	QString calcufile;
	QString calcufile1;
	QString errfile;
	QString filepath;
}Calcucfg;

typedef struct {
	QString interlockfile;
	QString errfile;
	QString filepath;
}Interlockcfg;

typedef struct {
	QString tempfile;
	QString errfile;
	QString filepath;
}Tempcfg;

typedef struct {
	QString waterfile;
	QString errfile;
	QString filepath;
}Watercfg;


typedef struct {
	QStringList fileTypeName;
}MainWincfg;

typedef struct {
	QString filetype;
	QStringList filenamelist;
	QList<FilePoint> filepointnamelist;
}Type_Name;
int loadLock_CalcuFile(QString filepath,QList<FilePoint>*filenamelist); //读闭锁、计算文本定义文件CalcuFile.def，InterLockFile.def

QList<FilePoint>loadLock_CalcuFile(QString filepath);
#endif