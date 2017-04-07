#include <stdio.h>
#include <windows.h>

#include	"resource.h"
#include	"..\inc\ipm_k_def.h"
#include	"..\inc\ipm.h"
#include	"../../../dms/v4.0a/inc/dms_com.h"

char *SearchLog = {"\\home\\ems\\h9000\\tmp\\searchFilesLog.txt"}  ;
char *HomePath = {"\\home\\ems\\h9000\\dsp\\"}  ; 
struct FIINDSTR 
{
	char *fileStr ;
	char *findStr ;
	int  isReversed ; 
} findArg ,*pfindArg;
void*	FindStrInFILES(void *arg);
////////////////////////////////////////////////
void FindFile(LPCTSTR lpszPath) {  
	WIN32_FIND_DATA wfd; 
	HANDLE hFind ; 
	char  szFile[MAX_PATH];
    TCHAR szFind[MAX_PATH];   
    lstrcpy(szFind,lpszPath);
	
    if(!IsRoot(szFind)) lstrcat(szFind,"\\");   
    lstrcat(szFind,"*.*");   
    
    hFind=FindFirstFile(szFind,&wfd);
	
    if(hFind==INVALID_HANDLE_VALUE)   return;   
    do{   
        if(lstrcmp(wfd.cFileName, ".")==0||lstrcmp(wfd.cFileName, "..")==0)   continue;   
		   
		lstrcpy(szFile,lpszPath);   
		if(!IsRoot(szFile)) lstrcat(szFile,"\\");   
		lstrcat(szFile,wfd.cFileName);   
		if((GetFileAttributes(szFile)&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){   
			FindFile(szFile);   //递归搜索子目录 
		}
		else { 
		} //Do   your   things   
	}  while (FindNextFile(hFind,&wfd));   
	CloseHandle(hFind);   
}




BOOL IsRoot(LPCTSTR lpszPath)
{
	TCHAR szRoot[4];
	wsprintf(szRoot, "%c:\\", lpszPath[0]);
	return (lstrcmp(szRoot, lpszPath) == 0);
}

int FindInAll( LPCTSTR FileName  , LPCTSTR FindStr,int isReversed)
{
	
	TCHAR szFind[MAX_PATH];
    FILE *logFile; 
	int  i ;    
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	struct tm *newtime;
	time_t long_time;
	 char *msg[2] = {"包含","不包含"} ;
	char  ErrMsg[128] ; 
	time( &long_time );                /* Get time as long integer. */
	newtime = localtime( &long_time ); /* Convert to local time. */
	
	
	lstrcpy(&szFind,  HomePath );
	if (!IsRoot(szFind))
		lstrcat(szFind, "\\");
	lstrcat(szFind, FileName); // 找所有文件
	 
	if ((logFile = fopen(SearchLog, "w"))==NULL) 
	{
		sprintf(ErrMsg,"无法打开日志：%s,请检查!",SearchLog);
		MessageBox(ghWnd, ErrMsg, "IPM", MB_OK);
		return -1; 	
	}
    

	fprintf(logFile,"本次查询时间为：%04d/%02d/%02d  %02d:%02d:%02d......\n" ,newtime->tm_year +1900 ,
		newtime->tm_mon +1 ,newtime->tm_mday ,newtime->tm_hour ,newtime->tm_min ,newtime->tm_sec )   ;
    fprintf(logFile  ,"在'%s'文件中搜索%s字符串'%s'的文件为：\n",FileName,msg[isReversed],FindStr)  ; 

	hFind = FindFirstFile(szFind, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
		return 0;
	i = 0 ;
	do
	{
		if (wfd.cFileName[0] == '.')
			continue; // 过滤这两个目录
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			TCHAR szFile[MAX_PATH];
			if (IsRoot(HomePath))
				wsprintf(szFile, "%s%s", HomePath, wfd.cFileName);
			else
				wsprintf(szFile, "%s\\%s", HomePath, wfd.cFileName);
			FindInAll(szFile,FindStr,isReversed); // 如果找到的是目录，则进入此目录进行递归
		}
		else
		{
			TCHAR szFile[MAX_PATH];
			if (IsRoot(HomePath))
				wsprintf(szFile, "%s%s", HomePath, wfd.cFileName);
			else
				wsprintf(szFile, "%s\\%s", HomePath, wfd.cFileName);
			printf("%s\n",szFile);
			
			// 对文件进行操作
			if  (FindStrInOneFile (szFile,FindStr,logFile,isReversed )==FALSE)
				continue  ;  //  goto next find process ;;;;
			else // 找到了一个文件
				i ++ ; 
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind); // 关闭查找句柄
	fprintf (logFile ," >>找到了共计%d个文件！！@@@@ \n",i);	
	fclose(logFile) ;
	return  i ; 
}

int 	OpenDspFileForSearch (char	*filename )

{
	FILE	*fp;
	char	full_name[MAX_FILE_NAME_LEN]={""};
	char	part_name[50]={""};
	
	
	fp=	FileOpen( filename); 
	if ( fp == NULL)
	{
		
		printf ("\n Open File Error!!!!\n"); 
		 return  FALSE;
	}	
	if (ReadGdbaseDataFindFile (fp)==FALSE)
		return  FALSE;
		
	fclose (fp);
	return  TRUE; 

}



int	FindInFileGdbase ( FILE  *fp,char * fileName , char *FindStr,int isReversed   )

{
	int		i,j;
	DMS_COMMON	dms_cmm;
	NEW_GBRCD		rcd;
	char	err_num[40]="";	
	char 	str[256];
	int  res  = FALSE ; 
	int	dberr_cnt =0;
		for (i=0,j=0; i < GDBASE_FILE.nextpos; i++)
		{		
			rcd	= GDBASE_FILE.buffer[i];						
			if( strcmp ( rcd.lnk.dbname ,"") !=0)
			{
				if (isReversed)
				{
					if (  !strstr(rcd.lnk.dbname , FindStr))
					{
						fprintf (fp, ">> %s\t\t\t----%s------ \n",rcd.lnk.dbname,fileName); 
						res = TRUE ; 
						j++ ; 
					}
					
				}
				else
				{
		 			if (  strstr(rcd.lnk.dbname , FindStr))
					{
						printf ("\n find a file to fit!!! \n");
						fprintf (fp ," >>%s\t\t\t----%s------ \n",rcd.lnk.dbname,fileName);
						return   TRUE ; 										
					}
				}
			}
		}
		
	return  res  ; 
}

int  FindStrInOneFile(LPCTSTR FileName  , LPCTSTR FindStr,FILE *fp,int isReversed)
{

	if  (OpenDspFileForSearch (FileName)==FALSE)
		return FALSE ;   
    if (FindInFileGdbase ( fp,FileName, FindStr ,isReversed  ) ==FALSE)
		return FALSE ; 
	//fprintf(fp,"  ++++++%s -------- %s  \n" , FileName  ,FindStr) ; 
	return  TRUE ; 
		
}



char FindString[MAX_FILE_NAME_LEN];
char FileString[MAX_FILE_NAME_LEN];

BOOL APIENTRY FileSearch(HWND hDlg,UINT message, UINT wParam, LONG lParam)
{
	int  isReversed =0  ; 
    switch (message){
	case WM_INITDIALOG:
		return(TRUE);
		
	case WM_COMMAND:
		if ( LOWORD(wParam) == IDCANCEL ){
			EndDialog(hDlg,0);
			return(TRUE);
		}

		if ( LOWORD(wParam) == IDOK){

            GetDlgItemText(hDlg, IDC_EDIT1, FileString, MAX_FILE_NAME_LEN);
			GetDlgItemText(hDlg, IDC_EDIT3, FindString, MAX_FILE_NAME_LEN);

			isReversed = SendMessage(GetDlgItem(hDlg, IDC_FILEREVERSED),
				BM_GETCHECK,0,0L);
			findArg.fileStr  =  FileString ; 
			findArg.findStr  =  FindString  ;
			findArg.isReversed  = isReversed ; 
			pfindArg  = &findArg ;
			if(!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FindStrInFILES, (void *)pfindArg , 0, NULL))
            MessageBox(hDlg, "无法创建线程!", "IPM", MB_OK);
		//	FindInAll( FileString  , FindString,isReversed)	;       
			EndDialog(hDlg,0);
			return(TRUE);
		}
		break;
    }
    return(FALSE);

}

void  PopFileSearchDlg()
{
	DialogBox (ghInst,
		MAKEINTRESOURCE(IDD_FILE_SEARCH),
		ghWnd,
		FileSearch);

}

void*	FindStrInFILES(void *arg)
{
	    int FindNum ; 
		char  msg[256] ; 
		pfindArg = (struct FINDARG * )arg  ; 
		FindNum = FindInAll( pfindArg->fileStr   ,pfindArg->findStr , pfindArg->isReversed )	;
		if (FindNum!=-1)
		{
			sprintf(msg,"查找了%d个文件!",FindNum );
			MessageBox(ghWnd, msg, "IPM", MB_OK);
		}		
		return   NULL ; 
 
}

