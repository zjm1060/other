

#include <stdio.h>
#include <windows.h>

#include "/users/ems/dms/v1.0a/inc/dmsstd.h"
#include "/users/ems/dms/v1.0a/inc/dmsdef.h"
#include "/users/ems/dms/v1.0a/inc/dmscom.h"
#include "/users/ems/hist/v1.0a/inc/histdb.h"


/*
** hist_put_data_id()
** 
** Purpose:
**	Put data to history database server .
** 
** Return:
** 	EX_EXIT_ERROR  or EX_EXIT_SUCCEED
**
*/

RETCODE	hist_put_data_id(p_ptr, hdb_date, hdb_time, data)
char	*hdb_date, *hdb_time, *data;
//float		data;
POINTER	*p_ptr;
{
	RETCODE	retcode;
	char	hdb_date_and_time[30];
	char	Rvalue[16];
	int		svalue, pvalue, ptype;

	UCHAR	szTableName[LONGNAME_LEN], szColumnName[LONGNAME_LEN], szOrder[LONGNAME_LEN];
	SDWORD	cbTableName, cbColumnName, cbOrder;

	strcpy(Rvalue, data);

	hdb_date_and_time[0] = '\0';
	strcat(hdb_date_and_time, hdb_date);
	strcat(hdb_date_and_time, " ");
	strcat(hdb_date_and_time, hdb_time);

	svalue = p_ptr->dev_id;
	ptype = p_ptr->data_type;
	pvalue = p_ptr->point_id;


    if(ptype == ANA_TYPE)
        {
        sprintf(SELECT, command_get_ana_table_name, svalue, pvalue);
        }

    if(ptype == POL_TYPE)
        {
        sprintf(SELECT, command_get_ind_table_name, svalue, pvalue);
        }

printf("\nSELECT = %s", SELECT);

	retcode = SQLAllocStmt(hdbc, &hstmt); /* Statement handle */
	if (retcode != SQL_SUCCESS) 
	{
	printf("\nSQLAllocStmt Error !!!");
	return((RETCODE) -1);
	}

	retcode = SQLExecDirect(hstmt, SELECT, SQL_NTS); 
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

//	strcat(hdb_date_and_time, hdb_date);
//	strcat(hdb_date_and_time, " ");
//	strcat(hdb_date_and_time, hdb_time);

	retcode = SQLFreeStmt(hstmt, SQL_DROP);
	if (retcode != SQL_SUCCESS)
		{
		printf("\nSQLFreeStmt Error !!!");
		}

printf("\nhdb_date_and_time = %s", hdb_date_and_time);
printf("\ntable_name->entry_name = %s", szTableName);
printf("\nsvalue = %d", svalue);
printf("\npvalue = %d", pvalue);
printf("\ndata = %s", Rvalue);

	sprintf(SELECT, command_put_data, szTableName, svalue, pvalue, hdb_date_and_time, Rvalue);

printf("\nSELECT = %s", SELECT);

	retcode = SQLAllocStmt(hdbc, &hstmt); /* Statement handle */
	if (retcode != SQL_SUCCESS) 
	{
	printf("\nSQLAllocStmt Error !!!");
	return((RETCODE) -1);
	}

	retcode = SQLExecDirect(hstmt, SELECT, SQL_NTS); 
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
