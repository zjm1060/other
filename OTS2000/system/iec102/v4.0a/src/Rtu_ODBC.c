/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/
/*********iec102 by zyf for ankong to connect SYBASE**********/
/************************change for all************************/
/*程序只编写接收遥测中的正向总有功，反向总有功，正向总无功，反向总无功。
  (积分周期/负荷曲线周期=15分钟)*/

#include "\home\ems\system\iec102\v4.0a\inc\iec102_com.h"


RETCODE Insert_HDB(int dev_id,int point_id,char *gen_db_time,double dlNo1End)
{
  char	      INSERT_CMD[160];
  RETCODE     retcode; 
  char      datetime[100], SELECT_CMD[200];
	UCHAR	szTableName[LONGNAME_LEN], szColumnName[LONGNAME_LEN], szOrder[LONGNAME_LEN];
	SDWORD	cbTableName, cbColumnName, cbOrder;
    sprintf(INSERT_CMD, command_get_ana_table_name, dev_id, point_id);
    retcode = SQLAllocStmt(hdbc, &hstmt); /* Statement handle */
	if (retcode != SQL_SUCCESS) 
	{
	printf("\nSQLAllocStmt Error !!!");
	return((RETCODE) -1);
	}

	retcode = SQLExecDirect(hstmt, INSERT_CMD, SQL_NTS); 
	if (retcode == SQL_SUCCESS)
		{ 

		retcode = SQLFetch(hstmt); 
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{ 
			/* Get data for columns */ 

			SQLGetData(hstmt, 1, SQL_C_CHAR, szTableName, LONGNAME_LEN, &cbTableName); 
			SQLGetData(hstmt, 2, SQL_C_CHAR, szColumnName, LONGNAME_LEN, &cbColumnName); 
			SQLGetData(hstmt, 3, SQL_C_CHAR, szOrder, LONGNAME_LEN, &cbOrder); 
			}
		else
			{ 
			printf("\nGet TableName, ColumnName, Order ERROR!");
			retcode = SQLFreeStmt(hstmt, SQL_DROP);
			if (retcode != SQL_SUCCESS)
				{
				printf("\nSQLFreeStmt Error !!!");
				}
			return((RETCODE) -1);
			}
		}
	else
		{
		printf("\n SQLExecDirect Error!!!");
		retcode = SQLFreeStmt(hstmt, SQL_DROP);
		if (retcode != SQL_SUCCESS)
			{
			printf("\nSQLFreeStmt Error !!!");
			}
		return((RETCODE) -1);
		}
	
	retcode = SQLFreeStmt(hstmt, SQL_DROP);
	if (retcode != SQL_SUCCESS)
		{
		printf("\nSQLFreeStmt Error !!!");
		}
    strcpy( datetime,"" );
	retcode = SQLAllocStmt(hdbc, &hstmt);
	if (retcode != SQL_SUCCESS) 
	{   
		printf("\nbjsd.exe: SQLAllocStmt Error !!!");
	    return(FALSE);
	}
	sprintf(SELECT_CMD, SELECT_LATE_TIME,szTableName, dev_id, point_id);
   	printf("@@@@ %s\n",SELECT_CMD);
	retcode = SQLExecDirect(hstmt, SELECT_CMD, SQL_NTS); 
	if (retcode == SQL_SUCCESS)
	{ 
		retcode = SQLFetch(hstmt); 
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{   
            SQLGetData(hstmt, 1, SQL_C_CHAR, datetime, 50, &cbTableName);			 		
		}
		else
		{ 
			printf("\nbjsd.exe: bjsd.exe: Get Data ERROR!");
			retcode = SQLFreeStmt(hstmt, SQL_DROP);
			if (retcode != SQL_SUCCESS)
			{
				printf("\nbjsd.exe: SQLFreeStmt Error !!!");
			}
			return(FALSE);
		}
	}
	else
	{
		printf("\nbjsd.exe: SQLExecDirect Error!!!");
		retcode = SQLFreeStmt(hstmt, SQL_DROP);
		if (retcode != SQL_SUCCESS)
		{
			printf("\nbjsd.exe: SQLFreeStmt Error !!!");
		}
		return(FALSE);
	}  
    
	retcode = SQLFreeStmt(hstmt, SQL_DROP);
	if (retcode != SQL_SUCCESS)
	{
		printf("\nbjsd.exe: SQLFreeStmt Error !!!");
	}
    if(strcmp(datetime,gen_db_time) ==0)      return(FALSE);
	sprintf(INSERT_CMD, command_put_data, szTableName, dev_id, point_id, gen_db_time, dlNo1End);
	printf("\nSELECT = %s", INSERT_CMD);

	retcode = SQLAllocStmt(hdbc, &hstmt); /* Statement handle */
	if (retcode != SQL_SUCCESS) 
	{
	printf("\nSQLAllocStmt Error !!!");
	return((RETCODE) -1);
	}

	retcode = SQLExecDirect(hstmt, INSERT_CMD, SQL_NTS); 
	if (retcode != SQL_SUCCESS)
		{
		printf("\n SQLExecDirect Error!!!");
		retcode = SQLFreeStmt(hstmt, SQL_DROP);
		if (retcode != SQL_SUCCESS)
			{
			printf("\nSQLFreeStmt Error !!!");
			}
		return((RETCODE) -1);
		}

	retcode = SQLFreeStmt(hstmt, SQL_DROP);
	if (retcode != SQL_SUCCESS)
		{
		printf("\nSQLFreeStmt Error !!!");
		}

	return(retcode);

}

void Update_HDB( int dev_id,int point_id,char *gen_db_time,long dlNo1End,long dlNo2End,long dlNo3End,long dlNo4End)
{ char	      UPDATE_CMD[160];
  RETCODE     retcode;   
   
  sprintf(UPDATE_CMD, UPDATE_NEW_DATA, dlNo1End,dlNo2End,dlNo3End,dlNo4End ,
	                                   dev_id, 
	                                   point_id   );  
							       
    retcode = SQLAllocStmt(hdbc, &hstmt); 
	if (retcode != SQL_SUCCESS) 
	{
	printf("\nRTU_COMM.EXE:  SQLAllocStmt Error !!!\n");
	goto EXIT_POINT;
	}
    printf("\nRTU_COMM.EXE: %s\n",UPDATE_CMD);
	retcode = SQLExecDirect(hstmt, UPDATE_CMD, SQL_NTS); 
	if (retcode != SQL_SUCCESS)
		{
		printf("\nRTU_COMM.EXE:  SQLExecDirect Error!!!");
		retcode = SQLFreeStmt(hstmt, SQL_DROP);
		if (retcode != SQL_SUCCESS)
			{
			printf("\nRTU_COMM.EXE:  SQLFreeStmt Error !!!");
			}
		goto EXIT_POINT;
		}

	retcode = SQLFreeStmt(hstmt, SQL_DROP);
	if (retcode != SQL_SUCCESS)
		{
		printf("\nRTU_COMM.EXE:  SQLFreeStmt Error !!!");
		}

EXIT_POINT:return; 
} 

BOOL Read_HDB(char *Time0, int dev_id, int point_id)
{
  RETCODE	retcode;
  char      datetime[100], SELECT_CMD[200];
  SDWORD	cbTableName;


    strcpy( datetime,"" );
	retcode = SQLAllocStmt(hdbc, &hstmt);
	if (retcode != SQL_SUCCESS) 
	{   
		printf("\nbjsd.exe: SQLAllocStmt Error !!!");
	    return(FALSE);
	}
	sprintf(SELECT_CMD, SELECT_LATE_TIME, dev_id, point_id);
   	printf("@@@@ %s\n",SELECT_CMD);
	retcode = SQLExecDirect(hstmt, SELECT_CMD, SQL_NTS); 
	if (retcode == SQL_SUCCESS)
	{ 
		retcode = SQLFetch(hstmt); 
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{   
            SQLGetData(hstmt, 1, SQL_C_CHAR, datetime, 50, &cbTableName);			 		
		}
		else
		{ 
			printf("\nbjsd.exe: bjsd.exe: Get Data ERROR!");
			retcode = SQLFreeStmt(hstmt, SQL_DROP);
			if (retcode != SQL_SUCCESS)
			{
				printf("\nbjsd.exe: SQLFreeStmt Error !!!");
			}
			return(FALSE);
		}
	}
	else
	{
		printf("\nbjsd.exe: SQLExecDirect Error!!!");
		retcode = SQLFreeStmt(hstmt, SQL_DROP);
		if (retcode != SQL_SUCCESS)
		{
			printf("\nbjsd.exe: SQLFreeStmt Error !!!");
		}
		return(FALSE);
	}  
    
	retcode = SQLFreeStmt(hstmt, SQL_DROP);
	if (retcode != SQL_SUCCESS)
	{
		printf("\nbjsd.exe: SQLFreeStmt Error !!!");
	}
    if(strcmp(datetime,Time0)==0)     return(TRUE);
	else                              return(FALSE);

}

RETCODE	DatabaseConnect(DSName, UID, Password, henv, hdbc)
HENV	*henv;
HDBC	*hdbc;
UCHAR FAR	*DSName, *UID, *Password;
{
RETCODE retcode;
retcode = SQLAllocEnv(henv);              /* Environment handle */
if (retcode != SQL_SUCCESS)
	{
	printf("\nbjsd.exe: SQLAllocEnv Error !!!");
	return(1);
	}
retcode = SQLAllocConnect(*henv, hdbc); /* Connection handle */
if (retcode != SQL_SUCCESS)
	{
	printf("\nbjsd.exe: SQLAllocConnect Error !!!");
	return(2);
	}
/* Set login timeout to 5 seconds. */
SQLSetConnectOption(*hdbc, SQL_LOGIN_TIMEOUT, 5);
/* Connect to data source */
retcode = SQLConnect(*hdbc, DSName, SQL_NTS, UID, SQL_NTS, Password, SQL_NTS);
if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
	printf("\nbjsd.exe: SQLConnect Error !!!\n");
	return(3);
	}
return(SQL_SUCCESS);
}


/* 据数库解除链接调用。*/
RETCODE	DatabaseDisconnect(henv, hdbc)
HENV    *henv;
HDBC    *hdbc;
{
RETCODE retcode;
retcode = SQLDisconnect(*hdbc);
if (retcode != SQL_SUCCESS)
	{
	printf("\nbjsd.exe: SQLDisconnect Error !!!");
	return(2);
	}
retcode = SQLFreeConnect(*hdbc);
if (retcode != SQL_SUCCESS)
	{
	printf("\nbjsd.exe: SQLFreeConnect Error !!!");
	return(3);
	}
retcode = SQLFreeEnv(*henv);
if (retcode != SQL_SUCCESS)
	{
	printf("\nbjsd.exe: SQLFreeEnv Error !!!");
	return(4);
	}
return(SQL_SUCCESS);
}

