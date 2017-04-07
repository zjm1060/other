#include	<windows.h>
#include	<winbase.h>
#include	<iostream>   
#include	<string>   
#include	<io.h>

using   namespace   std;

   
void   filesearch(string   path, char *last_path)   
{   
	struct	_finddata_t   filefind;   
	string	curr=path+"\\*.*";   
	int		done=0,handle;
	char	*file_ptr;
	string	rm_file;

	if((handle=_findfirst(curr.c_str(),&filefind))==-1)
		return;   

	while(!(done=_findnext(handle,&filefind)))   
	{   
		if(!strcmp(filefind.name,".."))
			continue;   

		if(_A_SUBDIR & filefind.attrib)   
		{   
			curr=path+"\\"+filefind.name;				
			
			if(_A_HIDDEN &filefind.attrib)
			{
				char	cmdstr[256];
				sprintf(cmdstr, "rmdir %s /q /s", curr.c_str());
				cout<<cmdstr<<endl;
				system(cmdstr);
			}
			else
				filesearch(curr, filefind.name);
		}   
		else  /*file*/   
		{   
			file_ptr= &filefind.name[strlen(filefind.name)-4];
			if(!strcmp(file_ptr, ".obj") || !strcmp(file_ptr, ".dxe"))
			{
				rm_file= path+"\\"+filefind.name;
				cout<<"rm  "<<rm_file<<endl;
				SetFileAttributes(rm_file.c_str(), FILE_ATTRIBUTE_NORMAL);
				remove(rm_file.c_str());
			}

			if(!strcmp(last_path, "inc"))
			{
				file_ptr= &filefind.name[strlen(filefind.name)-2];
				if(strcmp(file_ptr, ".h") && strcmp(file_ptr, ".H"))
				{
					rm_file= path+"\\"+filefind.name;
					cout<<"rm  "<<rm_file<<endl;
					SetFileAttributes(rm_file.c_str(), FILE_ATTRIBUTE_NORMAL);
					remove(rm_file.c_str());
				}
			}
			else if(!strcmp(last_path, "lib"))
			{
				file_ptr= &filefind.name[strlen(filefind.name)-4];
				if(strcmp(file_ptr, ".lib"))
				{
					rm_file= path+"\\"+filefind.name;
					cout<<"rm  "<<rm_file<<endl;
					SetFileAttributes(rm_file.c_str(), FILE_ATTRIBUTE_NORMAL);
					remove(rm_file.c_str());
				}
			}
			else if(!strcmp(last_path, "Debug")
				||!strcmp(last_path, "WinDebug")||!strcmp(last_path, "Release"))
			{
				file_ptr= &filefind.name[strlen(filefind.name)-4];
				if(strcmp(file_ptr, ".exe"))
				{
					rm_file= path+"\\"+filefind.name;
					cout<<"rm  "<<rm_file<<endl;
					SetFileAttributes(rm_file.c_str(), FILE_ATTRIBUTE_NORMAL);
					remove(rm_file.c_str());
				}
			}
			else if(!strcmp(last_path, "src"))
			{
				file_ptr= &filefind.name[strlen(filefind.name)-4];
				if(!strcmp(file_ptr, ".exe") || !strcmp(file_ptr, ".dxe"))
				{
					rm_file= path+"\\"+filefind.name;
					cout<<"rm  "<<rm_file<<endl;
					SetFileAttributes(rm_file.c_str(), FILE_ATTRIBUTE_NORMAL);
					remove(rm_file.c_str());
				}
			}
			
		}   
	}           
	_findclose(handle);               
} 


  
int   main()   
{           
	string   path= "\\home\\ems\\system";  

	filesearch(path,"");   
	system("PAUSE");   
	return   0;   
}   
  








