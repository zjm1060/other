

#include        "../inc/os_type_def.h"

#include        "../inc/dms_std.h"
#include        "../inc/dms_structure_def.h"
#include        "../inc/dms_util_def.h"
#include        "../inc/dms_com.h"
#include        "../inc/dms_functions.h"


DBS_GBL_COMMON  *db_ptr= (DBS_GBL_COMMON *)-1;
static int	h9000_error_id;

int GetH9000ErrorId(void)
{
	return h9000_error_id;
}


int GetAnaPtLmtStr(void);

BOOL CheckHostLicenseValid(void)
{
	extern int host_license_valid;

	GetAnaPtLmtStr();
	return host_license_valid;
}

void GetAnaPtLmtValid(void)
{
	extern int host_license_valid;

	if(!host_license_valid)
	{
		printf("\n无合法授权文件, H9000系统无法运行");
		exit(0);
	}
}


typedef struct  
{
	char	mac[24];
	char	hostname[24];
	char	stnname[24];
	char	code[24];
} HOST_INFO;

HOST_INFO host_info;
int host_license_valid;

#define MX_AAAAA ( (((z>>5)^(y<<2))+((y>>3)^(z<<4)))^((sum^y)+(k[(p&3)^e]^z)) )

int GetAnaPtLmtValue(unsigned long* var, unsigned long n, unsigned long* k) //decode
{
    unsigned long z, y, sum=0, e, DELTA=0x9e3779b9;
    long p, q;
	unsigned long v[16];
	unsigned long i;
	
	for(i=0; i<n; i++)
	{
		v[i]= *(var+i);
		Swap4Byte(&v[i]);
	}

	y= v[0];

    if (n > 1) 
	{       
		z=v[n-1];
		q = 6 + 52/n;
		sum = q*DELTA ;
		while (sum != 0) 
		{
			e = (sum >> 2) & 3;
			for (p=n-1; p>0; p--) 
			{
				z = v[p-1];
				y = v[p] -= MX_AAAAA;
			}
			z = v[n-1];
			y = v[0] -= MX_AAAAA;
			sum -= DELTA;
		}

		for(i=0; i<n; i++)
		{
			var[i]= v[i];
			Swap4Byte(&var[i]);
		}
	

		return 0 ;
    } 
    return 1;
}




BOOL GetAnaPtLmtBase(char *mac_addr);

int GetAnaPtLmtStr(void)
{
	int	i;
	UCHAR	stn_id;
	GULONG	keycode[4];
	char	buf[256];
	char	stn_name[STATION_NAME_SIZE];
	HOST_INFO *info= &host_info;
	char *q= info->code;
	char	mac[6];
	char	code[24];
	
	if(-1==GetHostStnId(info->hostname, &stn_id))
	{
		host_license_valid= 0;
		return 4;
	}	
	GetStnNameById(stn_id, stn_name);
	if(strcmp(stn_name, info->stnname))
	{
		host_license_valid= 0;
		return 3;
	}	
	
	AsciiToHex(info->mac, mac, 6);
	if(!GetAnaPtLmtBase(mac))
	{
		host_license_valid= 0;
		return 2;
	}

	GetNameStrHash(info->mac, &keycode[2]);
	GetNameStrHash(info->hostname, &keycode[1]);
	GetNameStrHash(info->stnname, &keycode[3]);
	sprintf(buf, "%s%s%s", info->mac, info->hostname, info->stnname);
	GetNameStrHash(buf, &keycode[0]);	
		
	memcpy(code, info->mac, 12);
	memcpy(&code[12], info->hostname, 12);

	GetAnaPtLmtValue((unsigned long*)info->code, 6, (unsigned long*)keycode);
	for(i=0; i<24; i++)
	{
		if((code[i])^(*q++))
		{
			host_license_valid= 0;
			return 1;
		}
	}
	host_license_valid= 1;
	return 0;
}

int GetHostLicenseInfo(void)
{
	FILE	*fp;
	char	hostname[256];
	char	filename[256];
	char	buf[256];
	
	if(gethostname(hostname, sizeof(hostname)))
	{
		SendTraceMsg(0, 0, "Fail to gethostname");
		return -1;
	}

	sprintf(filename, "%s%s", SYS_DEFINE_PATH, "h9000_license.def");
	fp= fopen(filename, "r");
	if(fp==NULL)
	{
		SendTraceMsg(0, 0, "Fail to open %s", filename);
		return -1;
	}
	
	while(fgets(buf, 256, fp))
	{
		char	host[256];
		char	mac[256];
		char	stnname[256];
		char	code[256];

		if(buf[0]=='#' ||buf[0]=='!')
			continue;

		if(sscanf(buf, "%s %s %s %s", host, stnname, mac, code)!=4)
			continue;

		if(strlen(mac) != 12)
			continue;
		
		if(strlen(code) != sizeof(host_info.code)*2)
			continue;
		
		if(!strcmp(host, hostname))
		{
			memset(&host_info, 0, sizeof(host_info));
			strncpy(host_info.hostname, hostname, sizeof(host_info.hostname));
			strncpy(host_info.stnname, stnname, sizeof(host_info.stnname));
			strncpy(host_info.mac, mac, sizeof(host_info.mac));
			AsciiToHex(code, host_info.code, sizeof(host_info.code));

			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return -1;
}


int	MountDB(char *filename)
{

	if(-1==MountDBFile(NULL, 1))
		return -1;

#ifdef WINDOWS_OS
	{
		OSVERSIONINFOA	version;

		memset(&version, 0, sizeof(version));
		version.dwOSVersionInfoSize= sizeof(version);
		if(GetVersionEx(&version) && version.dwMajorVersion>5) //WIN2000/2003/XP is 5, WIN7/VISTA/2008 is 6
			return 0;
	}
#endif
	/*   在仿真系统中取消认证
	if(-1==GetHostLicenseInfo())
	{
		h9000_error_id= H9000_ERROR_NO_LICENSE;
		return -1;
	}

	if(GetAnaPtLmtStr())
	{
		h9000_error_id= H9000_ERROR_ILLEGAL_LICENSE;
		return -1;
	}
	*/
	return 0;
}

int UnMountDB(void)
{
	int	ret;

	ret= UnMountFile(db_ptr, sizeof(DBS_GBL_COMMON));
	db_ptr= (DBS_GBL_COMMON *)-1;
	
	return ret;
}


int	MountDBFile(char *filename, BOOL loadConfigFile)
{
	char	dbfile[256], configfile[256];
	char	err_str[128];
	char	*ptr;

	if( db_ptr != (DBS_GBL_COMMON *)-1 )
		return 0;

	if( filename == NULL )
	{
		sprintf(dbfile, "%s%s", DATABASE_OUT_PATH, DATABASE_NAME);
		sprintf(configfile, "%s%s", SYS_DEFINE_PATH, SYS_DEFINE_FILE);
	}
#ifdef	WINDOWS_OS
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
#else
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
#endif

	db_ptr= MountFile(dbfile, sizeof(DBS_GBL_COMMON));
    if (db_ptr == (DBS_GBL_COMMON*)-1)
	{
        Err_log("Fail to mount database file");
		h9000_error_id= H9000_ERROR_DB;
        return -1;
	}

	if(loadConfigFile)
	{
		if(-1==LoadEmsDefine(configfile))
		{
			sprintf(err_str, "Failed to load %s", configfile);
			Err_log(err_str);
			UnMountDB();
			h9000_error_id= H9000_ERROR_CONFIGFILE;
			return -1;
		}
	}
	else
		return 0;

/*
#ifdef	WINDOWS_OS
	if(IsHostNoHardDisk())
	{
		void	*db_mem_ptr;

		hFileMap = MapFile((void*)-1, 0, sizeof(DBS_GBL_COMMON), "DATABASE.OUT"); 
		if (hFileMap == NULL)
		{
			Err_log("Map memory file failed");
			UnMountDB();
			return -1;
		}
		
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
	}
#endif
*/
	return 0;
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


void*	MountFile(char *filename, GULONG mount_size)
{
	DWORD	file_size;
	HANDLE	hFile, hFileMap;
	char	err_str[128];
	void	*mem_ptr;

	if(filename == NULL || !filename[0])
		return (void*)-1;

	/* Open the database file (open)  */
	hFile =CreateMapFile(filename); 
	if(hFile == (HANDLE) -1)
	{
		sprintf(err_str, "Fail to open %s", filename);
        Err_log(err_str);
        return (void*)-1;
	}

	file_size= GetFileSize(hFile, NULL);
	if(file_size != mount_size)
	{
		sprintf(err_str, "file %s length error: file %d, need %d",
			filename, file_size, mount_size);
        Err_log(err_str);
		CloseHandle(hFile);
        return (void*)-1;
	}
	
	/* Map the database file	*/
	hFileMap = MapFile(hFile, 0, 0, NULL); 
	if (hFileMap == NULL)
	{
		sprintf(err_str, "Fail ot map file %s", filename);
		Err_log(err_str);
		CloseHandle( hFile );
        return (void*)-1;
	}
	
	/* Get the map memory address */ 
	mem_ptr = MapView(&hFileMap);
	CloseHandle( hFileMap );
	CloseHandle( hFile );
    if (mem_ptr == (void*)-1)
	{
		sprintf(err_str, "Fail ot mapview file %s", filename);
		Err_log(err_str);
        return (void*)-1;
	}

	return mem_ptr;
}


int	UnMountFile(void* mount_ptr, GULONG mount_size)
{
	int	ret;

	if(mount_ptr == (void*)-1)
		return 0;

	ret= UnmapViewOfFile(mount_ptr);
	
	if(ret)
		return 0;
	else
		return -1;
}


#endif /*WINDOWS_OS*/



#ifdef SUN_OS

void*	MountFile(char *filename, GULONG mount_size)
{
	struct stat			statbuf;
	char				dbfile[256], configfile[256];
    INTEGER				file_size, prot, flags;
	int					fd;
	char				err_str[128];
	void				*mem_ptr;

	fd = open(filename, O_RDWR);
    if (fd == -1)
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		return (void*)-1;
	}

	/* Get the size (fstat) */
	fstat(fd, &statbuf);
    file_size = statbuf.st_size;
	if(file_size != mount_size)
	{
		sprintf(err_str, "file %s length error: file %d, need %d",
			filename, file_size, mount_size);
        Err_log(err_str);
		close(fd);
        return (void*)-1;
	}

	/* Map the file */
	prot = PROT_READ | PROT_WRITE;

	flags = MAP_SHARED;
	/*flags = MAP_FILE | MAP_SHARED;  MAP_FILE is only available in Tru64 UNIX zy 2003.4.4*/         

    mem_ptr = mmap((caddr_t)0, mount_size, prot, flags, fd, 0);
	close( fd );
    if (mem_ptr == (void*)-1)
	{
		sprintf(err_str, "Fail to mmap %s", filename);
		Err_log(err_str);
        return (void*)-1;
	}

	return mem_ptr;
}


int	UnMountFile(void* mount_ptr, GULONG mount_size)
{
	if(mount_ptr == (void*)-1)
		return 0;

    return munmap((caddr_t)mount_ptr, mount_size);
}
#endif /*SUN_OS*/


