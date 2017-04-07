
#include	"../inc/os_type_def.h"
#include	"../inc/dms_text_def.h"
#include	"../inc/dms_com.h"
#include	"../inc/dms_functions.h"


int	InitializeDatabaseFile ( char *new_file );
int	InitErrorFile(char *err_file);
int	DefineDatabase(void);
int	GenerateDatabase (void);
int	UpdateDatabase(BOOL isReset);
int GenerateAlmMsgDef(void);
int GenerateCmdMsgDef(void);
int DoublePtDatabase(void);
void ReverseDatabaseDefineFiles(char *output_path);
void	ReverseDatabase(void);
int	DefineDatabaseIoGroup(void);

int		err_count;
char	cur_dbfile[256];
char	new_dbfile[256];
char	err_file[256];

BOOL	enable_hash_statistic_flag;
BOOL	clear_overflow_flag;

#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "../../../lan/v4.0a/lib/lan.lib")


void PopupErrorInfoFile(void)
{
#ifdef	WINDOWS_OS
	system("notepad /home/ems/h9000/gen/dmserror.rec &");
#else
	system("/usr/dt/bin/dtpad /home/ems/h9000/gen/dmserror.rec &");
#endif

}

int main(int argc, char *argv[])
{
	int		ret;
	int		restore_flag;

#ifdef WINDOWS_OS
	COORD console_size= {256,1000};
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), console_size);
#endif


	printf ( "\n******************* DATABASE GENERATION START *******************\n" );

	chdir(SYS_DEFINE_PATH);

	if(argc>1 && !strcmp(argv[1], "-rev"))
	{
		ReverseDatabase();
		return 0;
	}


	sprintf(cur_dbfile, "../dat/%s", DATABASE_NAME );
	sprintf(new_dbfile, "../dat/%s", DATABASE_NEW );
	sprintf(err_file, "../gen/%s", DMS_ERROR_FILE );

	if(argc>1 && !strcmp(argv[1], "-no"))
		restore_flag= 0;
	else
		restore_flag= 1;

	if(argc>1 && !strcmp(argv[1], "-st"))
		enable_hash_statistic_flag= 1;
	else
		enable_hash_statistic_flag= 0;
	
	if(argc>1 && !strcmp(argv[1], "-overflow"))
		clear_overflow_flag= 1;
	else
		clear_overflow_flag= 0;

	if(enable_hash_statistic_flag)
	{
		FILE	*fp;
		if(fp=fopen("/home/ems/h9000/gen/hash_statistic.txt", "w"))
			fclose(fp);
	}
	
	err_count= 0;
	if( -1 == InitErrorFile(err_file) )
	{
		printf ( "\nInitialize %s failed!", err_file );
		exit(0);
	}
	if( -1 == InitializeDatabaseFile(new_dbfile) )
	{
		printf ( "\nInitialize %s failed!", new_dbfile );
		exit(0);
	}
	if( -1 == MountDBFile( new_dbfile, 0 ) )
	{
		printf ( "\nMount %s failed! exit...\n", new_dbfile );
		exit(0);
	}
	if( -1 == DefineDatabase())
	{
		printf("\nDefine database failed! exit...\n");
		UnMountDB();
		PopupErrorInfoFile();
		exit(0);
	}
	if( -1 == GenerateDatabase())
	{
		printf("\nGenerate database failed! exit...\n");
		UnMountDB();
		PopupErrorInfoFile();
		exit(0);
	}

	if( -1 == DoublePtDatabase())
	{
		printf("\nDouble database failed! exit...\n");
		UnMountDB();
		PopupErrorInfoFile();
		exit(0);
	}

	if( -1 == GenerateAlmMsgDef())
	{
		printf("\nGenerate alm msg def failed! exit...\n");
		UnMountDB();
		PopupErrorInfoFile();
		exit(0);
	}

	if( -1 == GenerateCmdMsgDef())
	{
		printf("\nGenerate cmd msg def failed! exit...\n");
		UnMountDB();
		PopupErrorInfoFile();
		exit(0);
	}
	
	if( -1 == DefineDatabaseIoGroup())
	{
		printf("\nGenerate iogroup def failed! exit...\n");
		UnMountDB();
		PopupErrorInfoFile();
		exit(0);
	}


	UnMountDB();

	printf ( "\n\n***** Find %d errors *****", err_count );
	
	printf ( "\n\nDatabase size: %d\n", sizeof(DBS_GBL_COMMON)); 

	ret= UpdateDatabase(restore_flag);

	if(-1==ret)
		printf ( "\n\nUpdate database failed, sorry\n" );
	else
		printf ( "\n\nUpdate database successfully, Congratulations!\n" );

	if ( err_count != 0 )
	{
		printf ( "\nPlease check error record file in gen for details" );
		PopupErrorInfoFile();
	}
	printf ("\n\n\n*******************  DATABASE GENERATION END  *******************\n\n\n");
	return 0;
}


void	ReverseDatabase(void)
{
/*	char	answer[128], reverse_path[256];
	struct stat		statbuf;

	printf("\nReverse to database define files");
	printf("\nNeed to realize, sorry");

	return;

	while(1)
	{
		printf ( DMS_IF_COVER_DEFINE_FILES );
		gets ( answer );

		if ( answer[0] == 'y' || answer[0] == 'Y' )
		{
			strcpy(reverse_path, DATABASE_DEFINE_PATH);
			reverse_path[strlen(reverse_path)-1]= 0;
		}
		else if ( answer[0] == 'n' || answer[0] == 'N' )
		{
			printf ( DMS_INPUT_DEFINE_FILE_PATH );
			gets ( reverse_path );
		}
		else
			continue;

		if(-1==(stat(reverse_path, &statbuf)))
		{
			printf("\npath invalid: %s", reverse_path);
			continue;
		}
		printf("\n\noutput path: %s", reverse_path);
		break;
	}
#ifdef	WINDOWS_OS
	reverse_path[strlen(reverse_path)]= '\\';
#else 
	reverse_path[strlen(reverse_path)]= '/';
#endif

	reverse_path[strlen(reverse_path)+1]= 0;

	printf("\n\nNow begin to reverse, please wait...");

	ReverseDatabaseDefineFiles(reverse_path);

	printf("\n\nReverse end\n");*/
}

