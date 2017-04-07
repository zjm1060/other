/************
load_passwd_p.c -- Prompt a passwd. Meaning, all input looks like a
 * a series of *'s. Store the actual data typed by the user in
 * an internal variable. Don't allow paste operations. Handle 
 * backspacing by deleting all text from insertion point to the end of text.
 * 2006.2 updated by zyp for v4.0
 20070510 updated by wzg 
 SYNC by hcl 2007.12.09
*************/

#include	"../../../sms/v4.0a/inc/user_privilege_def.h"



void	EncodeString(CHAR *str, int len);
void	DecodeString(CHAR *str, int len);



PRIVILEGE_DB		privilege_db;


int GetPrivDbCheckSum(PRIVILEGE_DB *db)
{
	int		i, check_sum= 0;
	char	*ptr= (char*)db;
	
	for(i=sizeof(db->check_sum); i<sizeof(PRIVILEGE_DB); i++)
		check_sum += ptr[i];
	return check_sum;
}


int	LoadUserPrivilegeDatabase(void)
{
	int			i;
	FILE		*fp;
	char		privilegeFile[256];
	char		err_str[256];
	PRIVILEGE_DB		privilege_db_tmp;

	sprintf(privilegeFile, "%s%s", SYS_DATA_PATH, PRIVILEGE_DATABASE_FILE);
	fp = fopen(privilegeFile, "rb");
    if(!fp) 
	{
		sprintf(err_str, "\nFail to open %s", privilegeFile);
		Err_log(err_str);
		return -1;
	}
    
	if(sizeof(privilege_db_tmp)!=fread(&privilege_db_tmp, 1, sizeof(privilege_db_tmp), fp))
	{
		sprintf(err_str, "\nFail to read %s", privilegeFile);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}

	Swap4Byte(&privilege_db_tmp.check_sum);

	if(privilege_db_tmp.check_sum!=GetPrivDbCheckSum(&privilege_db_tmp))
	{
		sprintf(err_str, "\nillegal privilege file: %s", privilegeFile);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}

	for(i=0; i<privilege_db_tmp.user_num; i++)
	{
		DecodeString(privilege_db_tmp.user_list[i].username, sizeof(privilege_db_tmp.user_list[i].username));				
		DecodeString(privilege_db_tmp.user_list[i].passwd, sizeof(privilege_db_tmp.user_list[i].passwd));				
	}

	for(i=0; i<privilege_db_tmp.host_num; i++)
	{
		DecodeString(privilege_db_tmp.host_list[i].hostname, sizeof(privilege_db_tmp.host_list[i].hostname));				
	}

	privilege_db= privilege_db_tmp;
	fclose(fp);
	return 0;
}



int	SaveUserPrivilegeDatabase(void)
{
	int			i;
	FILE		*fp;
	char		privilegeFile[256];
	char		err_str[256];
	PRIVILEGE_DB		privilege_db_tmp;

	sprintf(privilegeFile, "%s%s", SYS_DATA_PATH, PRIVILEGE_DATABASE_FILE);
	fp = fopen(privilegeFile, "wb");
    if(!fp) 
	{
		sprintf(err_str, "\nFail to open %s", privilegeFile);
		Err_log(err_str);
		return -1;
	}

	privilege_db_tmp= privilege_db;

	for(i=0; i<privilege_db_tmp.user_num; i++)
	{
		EncodeString(privilege_db_tmp.user_list[i].username, sizeof(privilege_db_tmp.user_list[i].username));				
		EncodeString(privilege_db_tmp.user_list[i].passwd, sizeof(privilege_db_tmp.user_list[i].passwd));				
	}

	for(i=0; i<privilege_db_tmp.host_num; i++)
	{
		EncodeString(privilege_db_tmp.host_list[i].hostname, sizeof(privilege_db_tmp.host_list[i].hostname));				
	}

	privilege_db_tmp.check_sum= GetPrivDbCheckSum(&privilege_db_tmp);

	Swap4Byte(&privilege_db_tmp.check_sum);

	if(sizeof(privilege_db_tmp)!=fwrite(&privilege_db_tmp, 1, sizeof(privilege_db_tmp), fp))
	{
		sprintf(err_str, "\nFail to write %s", privilegeFile);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}

	fclose(fp);
	return 0;
}


void	EncodeString(CHAR *str, int len)
{
	int		i;

	for(i=0; i<len; i++)
		str[i]= str[i]+128;
}

void	DecodeString(CHAR *str, int len)
{
	int		i;

	for(i=0; i<len; i++)
		str[i]= str[i]-128;
}


BOOL CheckUserPrivilege(char *userName, enum USER_PRIVILEGE_TYPE priv_type)
{
	int		i;

	if(userName[0]==0)
		return FALSE;

	if(priv_type>=MAX_PRIV_TYPE_NUM)
		return FALSE;

	for(i=0; i<privilege_db.user_num; i++)
	{
		if(!strcmp(userName, privilege_db.user_list[i].username))
			return (privilege_db.user_list[i].privilege[priv_type/8]>>(priv_type%8)) & 1;
	}
	return FALSE;
}


BOOL CheckUserExist(char *userName)
{
	int		i;

	if(userName[0]==0)
		return FALSE;

	for(i=0; i<privilege_db.user_num; i++)
	{
		if(!strcmp(userName, privilege_db.user_list[i].username))
			return TRUE;
	}
	return FALSE;
}


BOOL CheckUserLogin(char *userName, char *passwd)
{
	int		i;

	if(userName[0]==0)
		return FALSE;

	for(i=0; i<privilege_db.user_num; i++)
	{
		if(!strcmp(userName, privilege_db.user_list[i].username))
		{
			if(!strcmp(passwd, privilege_db.user_list[i].passwd))
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}


int UpdateUserPasswd(char *userName, char *passwd, char *new_passwd)
{
	int		i;

	if(userName[0]==0)
		return -1;

	if(strlen(new_passwd)>=MAX_USER_PASSWD_LEN)
		return -1;

	for(i=0; i<privilege_db.user_num; i++)
	{
		if(!strcmp(userName, privilege_db.user_list[i].username))
		{
			if(!strcmp(passwd, privilege_db.user_list[i].passwd))
			{
				strcpy(privilege_db.user_list[i].passwd, new_passwd);
				return SaveUserPrivilegeDatabase();
			}
			else
				return -1;
		}
	}
	return -1;
}




BOOL CheckHostPrivilege(char *hostName, LCU_ID lcu_id)
{
	int		i, j;

	if(hostName[0]==0)
		return FALSE;

	for(i=0; i<privilege_db.host_num; i++)
	{
		if(!strcmp(hostName, privilege_db.host_list[i].hostname))
		{
			for(j=0; j<privilege_db.host_list[i].lcu_num; j++)
			{
				if((lcu_id.stn_id==privilege_db.host_list[i].lcu[j].stn_id)
					&& (lcu_id.dev_id==privilege_db.host_list[i].lcu[j].dev_id))
					return TRUE;
			}
			return FALSE;
		}
	}
	return FALSE;
}




