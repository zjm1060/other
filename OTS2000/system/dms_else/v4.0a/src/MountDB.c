

#include        "../inc/os_type_def.h"

#include        "../inc/dms_std.h"
#include        "../inc/dms_structure_def.h"
#include        "../inc/dms_util_def.h"
#include        "../inc/dms_com.h"
#include        "../inc/dms_functions.h"



DBS_GBL_COMMON  *db_ptr= (DBS_GBL_COMMON *)-1;
GULONG	curr_check_flag;

int	SigbusHandler(void)
{
	Err_log("SIGBUS database error: maybe is being covered");
	return 1;
}



int	MountDB(char *filename)
{
//	signal(SIGBUS, SigbusHandler);
	return MountDBFile(NULL, 1);
}


#ifdef	WINDOWS_OS

HANDLE CreateMapFile(char *FileName)
{
	HANDLE hFileHandle;

	hFileHandle= CreateFile(FileName,
                          GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

	if (hFileHandle == INVALID_HANDLE_VALUE)
		Err_log("CreateFile failed");

	return(hFileHandle);

}


HANDLE MapFile(HANDLE hFileToBeMapped, DWORD sizeHigh, DWORD sizeLow, char *MapName)
{
	HANDLE hMapHandle;

	hMapHandle= CreateFileMapping(hFileToBeMapped,
                             NULL,
                             PAGE_READWRITE,
                             sizeHigh,
                             sizeLow,
                             MapName);

	if (hMapHandle == NULL)
		Err_log("CreateFileMapping failed");

	return(hMapHandle);

}


LPVOID MapView(HANDLE *hMapHandle)
{
	LPVOID MappedPointer;

	MappedPointer= MapViewOfFile(*hMapHandle,
                             FILE_MAP_WRITE | FILE_MAP_READ,
                             0,
                             0,
                             0);
	if (MappedPointer == NULL)
		Err_log("MapViewOfFile failed");

	return(MappedPointer);

}



HANDLE OpenMap(char *MapName)
{
	HANDLE hAMap;

	hAMap= OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
                       TRUE,
                       MapName);

	if (hAMap == NULL)
		Err_log("OpenFileMapping failed");

	return(hAMap);

}




int	MountDBFile(char *filename, BOOL loadConfigFile)
{
	DWORD	file_size;
	char	dbfile[256], configfile[256];
	HANDLE	hFile, hFileMap;
	char	err_str[128];
	void	*db_mem_ptr;
	char	*ptr;

	if( db_ptr != (DBS_GBL_COMMON *)-1 )
	{
		Err_log("Database is mounted already!!!");
        return -1;
	}

	if( filename == NULL )
	{
		sprintf(dbfile, "%s%s", DATABASE_OUT_PATH, DATABASE_NAME);
		sprintf(configfile, "%s%s", SYS_DEFINE_PATH, SYS_DEFINE_FILE);
	}
	else if( (filename[0]!='\\') && (filename[0]!='/') && (filename[0]!='.') && !strstr(filename, ":\\")) 
	{
		sprintf(dbfile, "..\\dat\\%s", filename);
		sprintf(configfile, "..\\def\\%s", SYS_DEFINE_FILE);
	}
	else
	{
		sprintf(dbfile, "%s", filename);
		strcpy(configfile, filename);		
		if((ptr=strrchr(configfile, '\\'))==NULL )
			ptr=strrchr(configfile, '/');
		sprintf(ptr+1, "..\\def\\%s", SYS_DEFINE_FILE);
	}


	/* Open the database file (open)  */
	hFile =CreateMapFile(dbfile); 
	if(hFile == (HANDLE) -1)
	{
		sprintf(err_str, "Fail to open %s", dbfile);
        Err_log(err_str);
        return -1;
	}

	file_size= GetFileSize(hFile, NULL);
	if(file_size != sizeof(DBS_GBL_COMMON))
	{
		sprintf(err_str, "database file length error: file %d, need %d",
			file_size, sizeof(DBS_GBL_COMMON));
        Err_log(err_str);
		CloseHandle(hFile);
        return -1;
	}
	
	/* Map the database file	*/
	hFileMap = MapFile(hFile, 0, 0, NULL); 
	if (hFileMap == NULL)
	{
		Err_log("Map database file failed");
		CloseHandle( hFile );
        return -1;
	}
	
	/* Get the map memory address */ 
	db_ptr = (DBS_GBL_COMMON*)MapView(&hFileMap);
	CloseHandle( hFileMap );
	CloseHandle( hFile );
    if (db_ptr == (DBS_GBL_COMMON*)-1)
	{
        Err_log("MapView database file failed");
        return -1;
	}

	if(loadConfigFile)
	{
		if(-1==LoadEmsDefine(configfile))
		{
			sprintf(err_str, "Failed to load %s", configfile);
			Err_log(err_str);
			UnMountDB();
			return -1;
		}
	}
	else
		return 0;

	if(!IsHostNoHardDisk())
		return 0;

	/* Mount ShareMemory in memory rather than disk*/
	hFileMap = MapFile((void*)-1, 0, sizeof(DBS_GBL_COMMON), "DATABASE.OUT"); 
	if (hFileMap == NULL)
	{
		Err_log("Map memory file failed");
		UnMountDB();
        return -1;
	}
	
	/* Get the map memory address */ 
	db_mem_ptr = (DBS_GBL_COMMON*)MapView(&hFileMap);
	CloseHandle( hFileMap );
    if (db_mem_ptr == (DBS_GBL_COMMON*)-1)
	{
        Err_log("MapView memory file failed");
		UnMountDB();
        return -1;
	}

	memcpy(db_mem_ptr, db_ptr, sizeof(DBS_GBL_COMMON));
	UnmapViewOfFile(db_ptr);
	db_ptr= db_mem_ptr;

	return 0;
}


int UnMountDB()
{
	int	ret;

	if(db_ptr == (DBS_GBL_COMMON *)-1)
		return -1;

	ret= UnmapViewOfFile(db_ptr);
	db_ptr= (DBS_GBL_COMMON *)-1;
	
	if(ret)
		return 0;
	else
		return -1;
}


#else
int	MountDBFile(char *filename, BOOL loadConfigFile)
{
	struct stat        statbuf;
	char			   dbfile[256], configfile[256];
    INTEGER            file_size, prot, flags;
	int                db_fd;
	char				err_str[128];
	char				*ptr;

	if( db_ptr != (DBS_GBL_COMMON *)-1 )
	{
		Err_log("Database is mounted already!!!");
        return -1;
	}

	if( filename == NULL )
	{
		sprintf(dbfile, "%s%s", DATABASE_OUT_PATH, DATABASE_NAME);
		sprintf(configfile, "%s%s", SYS_DEFINE_PATH, SYS_DEFINE_FILE);
	}
	else if( (filename[0]!='/') && (filename[0]!='.') ) 
	{
		sprintf(dbfile, "../dat/%s", filename);
		sprintf(configfile, "../def/%s", SYS_DEFINE_FILE);
	}
	else
	{
		sprintf(dbfile, "%s", filename);
		strcpy(configfile, filename);		
		ptr=strrchr(configfile, '/');
		sprintf(ptr+1, "../def/%s", SYS_DEFINE_FILE);
	}

	/* Open the database file (open) */
	db_fd = open(dbfile, O_RDWR);
    if (db_fd == -1)
	{
		sprintf(err_str, "Fail to open %s", dbfile);
		Err_log(err_str);
		return -1;
	}

	/* Get the database size (fstat) */
	fstat(db_fd, &statbuf);
    file_size = statbuf.st_size;
	if(file_size != sizeof(DBS_GBL_COMMON))
	{
		sprintf(err_str, "database file length error: file %d, need %d",
			file_size, sizeof(DBS_GBL_COMMON));
        Err_log(err_str);
		close(db_fd);
        return -1;
	}

	/* Map the database file */
	prot = PROT_READ | PROT_WRITE;

#ifdef SUN_OS
	flags = MAP_SHARED;
#else
	flags = MAP_FILE | MAP_SHARED;  /*MAP_FILE is only available in Tru64 UNIX zy 2003.4.4*/         
#endif

    db_ptr = (DBS_GBL_COMMON*) mmap((caddr_t)0, sizeof(DBS_GBL_COMMON), prot, flags, db_fd, 0);
	close( db_fd );
    if (db_ptr == (DBS_GBL_COMMON*)-1)
	{
        Err_log("mmap database file failed");
        return -1;
	}

	if(loadConfigFile)
	{
		if(-1==LoadEmsDefine(configfile))
		{
			sprintf(err_str, "Failed to load %s", configfile);
			Err_log(err_str);
			UnMountDB();
			return -1;
		}
	}


	return 0;

}


int	UnMountDB(void)
{
	int	ret;

	if(db_ptr == (DBS_GBL_COMMON *)-1)
		return -1;

    ret= munmap((caddr_t)db_ptr, sizeof(DBS_GBL_COMMON));

	db_ptr= (DBS_GBL_COMMON *)-1;

	return ret;
}



#endif



