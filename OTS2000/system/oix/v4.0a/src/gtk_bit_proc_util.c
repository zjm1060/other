/*************************************
	2006.2 Modified by zyp for v4.0 ;
	Windows Have  SYN to  UNIX  by chi 07.05.10 ;

**************************************/


#define		DEBUG

 
#include	"../inc/oix.h"



short   GetCurWSCtrlPrvg()
{	
	char            host_name[15];
	short           status;
        UCHAR           host_id;
	char    	entry_name[60]={""} ;
	short		db_err;
	
	
        GetLocalHostId(&host_id);
        printf("host_id=%d\n",host_id);
        GetHostNameById(host_id,host_name);
        printf("host_name=%s\n",host_name);
        GetHostStatePtNameStr(host_name, entry_name);
#ifdef DEBUG
printf("GetCurWSCtrlPrvg::entry_name=%s\n",entry_name);
#endif
        db_err = GetIndPtStateByNameStr( entry_name, &status );
        if(db_err==-1)
        {
                printf("read db err!!! entry_name=%s\n",entry_name);
                return db_err;
        }
        else
                return  status;
	
}

short   GetOthCtrlPrvg()
{
	char            other_name[MAX_HOST_NAME_SIZE];
	short           status;
	char    	entry_name[DBNAME_LEN]={""} ;
	short		db_err;

	GetDhswHostName(other_name);

	GetHostStatePtNameStr(other_name, entry_name);

#ifdef DEBUG
printf("GetOthCtrlPrvg::entry_name=%s\n",entry_name);
#endif
	db_err=GetIndPtStateByNameStr( entry_name, &status );
	if(db_err==-1)
	{
		printf("read db err!!! entry_name=%s\n",entry_name);
		return db_err;
	}
	else
		return ( status );
}

short   GetCurServCtrlPrvg()
{
	char            host_name[ENTRY_NAME_SIZE];
	short           status;
	char    	entry_name[DBNAME_LEN];
	short		db_err;

	if (IsServerStation())
	{
		GetHostStatePtNameStr(host_name, entry_name);
#ifdef DEBUG
printf("GetCurServCtrlPrvg::entry_name=%s\n",entry_name);
#endif
		db_err = GetIndPtStateByNameStr( entry_name, &status );
		if(db_err==-1)
		{
			printf("read db err!!! entry_name=%s\n",entry_name);
			return db_err;
		}
		else
			return  status;		
	}
	else
		return ( -1 ); 
}


short  GetCurCtrlMode ( char	*db_name )
{
	short           status;
	short		db_err;

	db_err=GetIndPtStateByNameStr( db_name, &status );
	if(db_err==-1)
	{
		printf("read db err!!! entry_name=%s\n",db_name);
		return db_err;
	}
	else
		return ( status );
}

int    GetHost_Cmd()
{
	char    src_id;
	
	if(GetLocalHostId(&src_id)==-1)
	{
		printf("GetLocalHostId err!!\n");
		return -1;
	}
	else
	{
		/*printf("in GetHost_Cmd:: src_id=%d\n",src_id);*/
        	return ( src_id );
	}
}

int    GetSrc_Id()
{
	int     IsCascade=IsLocalHostDispatch();
    	if(IsCascade==TRUE)
        	return CASCADE_CMD_SOURCE;
    	else
        	return LOCAL_CMD_SOURCE;
}

/* this is for cascade system, caculate control  plant lopp state    */ 

short	GetPlantCtrlMode ( GBRCD   *rcd, char	*db_name )
{
	short           status;
	short		len_del, len_valid;
	char		tmp_name[DBNAME_LEN] =  {""};
	short		db_err;

	if ( ( strlen ( rcd->lnk.dbname )) > 0 )
	{
		if ( strchr ( rcd->lnk.dbname, '_') != (char * ) NULL )
		{
			len_del = strlen ( strchr ( rcd->lnk.dbname, '_') );
			len_valid = strlen ( rcd->lnk.dbname ) - len_del+1;
		}
		else
			len_valid = strlen ( rcd->lnk.dbname );
		strncpy ( tmp_name, rcd->lnk.dbname, len_valid );
		strcat ( tmp_name, db_name );

		db_err=GetIndPtStateByNameStr( tmp_name, &status );
		if(db_err==-1)
		{
			printf("read db err!!! entry_name=%s\n",tmp_name);
			return db_err;
		}
		else
			return ( status );
	}
	else
		return ( 1 );
}

