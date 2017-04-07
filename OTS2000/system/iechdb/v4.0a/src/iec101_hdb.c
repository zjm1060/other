/************************************************************
*  FILE NAME:           iec101_app_fun.c      				*
*  FUNCTION:            Controlling Station					*
*						Application Layer Functions			*
*  's NAME :  			              						*
*  DATE:                March,2007                			*
*															*
************************************************************/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#include "..\inc\iec101_com.h"

#define	DEBUG


extern int DswNum;							
extern int SswNum;							
extern int AlPtr;
extern int AlPtrEnd;							
extern BHDSW ArrayDsw[MAX_DSW_NUM];
extern BHAL ArrayAL[MAX_BHAL_NUM];

extern	BOOL	lan_out();
extern	float	GetAnaValue();
static  Lda_Def lda;
static  Cda_Def	cda;

char	LoginID[50];
char	SIDSTR[100];
char	strPwd[];
BOOL	HdbConn = FALSE;

int InsertOneBHAl(BHDSW dsw,BHAL al);
void	Write_Hdb(); 
int DisconnectHDB(Lda_Def * lda,Cda_Def *cda);
int ConnectHDB(Lda_Def * lda_c,Cda_Def *cda, char *MyStr );
void OracleError(Lda_Def * lda);
int UpdateDswPoint();

void	Write_Hdb()
{

	int FirstLoop=TRUE;
	strcpy(LoginID,"TELCOM/TELCOM@HISTA");
	MySleep(1000);
	AlPtrEnd = AlPtr;
	while(1)
	{
		/*************** connect hdb *****************/
		if(HdbConn != TRUE)
		{
			for(;;)
			{
				if( ConnectHDB(&lda,&cda,LoginID)==FALSE )
				{	
					printf("ConnectHDB() Orcal error !   ( %s )\n",LoginID);  
					HdbConn = FALSE;
					MySleep(10000);
					continue; 
				}
				else
				{	
					printf("\n==== Connect '%s' Success \n",LoginID); 
					HdbConn=TRUE;
					break;
				}
			}
		}
		if(FirstLoop == TRUE)
		{
			if(UpdateDswPoint()== FALSE)
			{
				HdbConn=FALSE;
				break;
			}
			FirstLoop = FALSE;
		}
		MySleep(100);

		while(AlPtrEnd != AlPtr)
		{
			if(InsertOneBHAl(ArrayDsw[ArrayAL[AlPtrEnd].id],ArrayAL[AlPtrEnd])!=TRUE)
			{
				HdbConn=FALSE;
				break;
			}
			AlPtrEnd++;
			if(AlPtrEnd>=MAX_BHAL_NUM)
			{
				AlPtrEnd = 0;
			}
		}

	}
}

int InsertOneBHAl(BHDSW dsw,BHAL al)
{
	char	InsertStr[]="insert into telcom.bh_alarm( dev_id, info_id, datetime, status) \
		 values(:DEVID, :INFOID, :TIME, :STATUS)";
	char tmpstr[500];
	sprintf(log_str,"insert into telcom.bh_alarm( dev_id, info_id, datetime, status) values(%d, %d ,'%s' ,%d) ",
		dsw.bhdevid, dsw.infoid, al.update_time, al.status);
	strcpy(tmpstr,log_str);
	log_write(INF,1,log_str);
	
	/* Parse the PL/SQL block */
	if( oparse(&cda, tmpstr, (sb4) -1, (sword) TRUE, (ub4) 2) )
    {
        	OracleError(&cda);
        	err_write("Oparse Orcal error !\n"); 
			return(FALSE);	
	}
   	/* Execute the PL/SQL block.*/
	if( oexec(&cda) )
	{
      	OracleError(&cda);
      	err_write("Oexec Orcal error !\n"); 
		log_write(ERR,1,tmpstr);
		/* return(FALSE) ;  */
	}	
	return TRUE;
} 

int UpdateDswPoint()
{
	char	InsertStr[200]="insert into telcom.bh_point( dev_id, info_id, pt_id) \
		 values(%d, %d, %d)";	
	char	UpdateStr[200]="update telcom.bh_point set pt_id = %d where dev_id=%d and info_id = %d";
	char	SelectStr[200]="select pt_id from telcom.bh_point where dev_id=%d and info_id =%d";	
	char tmpstr[500];	
	int	i;
	int pt_id;
	int IsExist;
	for(i=0;i<DswNum;i++)
	{
		IsExist = TRUE;
		sprintf(tmpstr,SelectStr,ArrayDsw[i].bhdevid,ArrayDsw[i].infoid);
		/* err_write(tmpstr); */
		if( oparse(&cda, tmpstr, (sb4) -1, (sword) TRUE, (ub4) 2) )
		{
        		OracleError(&cda);
        		err_write("Oparse Orcal error !\n"); 
				return(FALSE);	
		}

		if (odefin(&cda, 1, (ub1 *) &pt_id, (sword) sizeof(int),SQLT_INT,
			-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
		{
			err_write("oci error: odefin(pt_id)\n");
			OracleError(&cda);
			return(FALSE);   
		}

		if (oexec(&cda))
		{
			err_write("oci  error: oexec()\n");
			OracleError(&cda);
			return(FALSE);
		}
		if( ofetch(&cda)!=0 )
 		{     
			IsExist = FALSE;
		}
		else
		{
			IsExist = TRUE;
			if(pt_id == ArrayDsw[i].id)
			{
				continue;
			}
		}
		if(IsExist != TRUE)
		{
			sprintf(tmpstr,InsertStr,ArrayDsw[i].bhdevid,ArrayDsw[i].infoid,ArrayDsw[i].id);
		}
		else
		{
			sprintf(tmpstr,UpdateStr,ArrayDsw[i].id,ArrayDsw[i].bhdevid,ArrayDsw[i].infoid);
		}
		/* err_write(tmpstr); */
		if( oparse(&cda, tmpstr, (sb4) -1, (sword) TRUE, (ub4) 2) )
		{
			OracleError(&cda);
			err_write("Oparse Update Orcal error !\n"); 
			return(FALSE);
		}
		if( oexec(&cda) )
		{
			OracleError(&cda);
			err_write("Oexec Orcal error !\n"); 
			return(FALSE);  
		}
	}

	return TRUE;
}

void OracleError(Lda_Def * lda)
{  	
	char msgbuf[512];
  	int n=oerhms(lda, lda->rc, (char *)msgbuf, (int) sizeof(msgbuf) );
  	sprintf(log_str,"%.*s\n",n,msgbuf);
	err_write(log_str);
}

int ConnectHDB(Lda_Def * lda_c,Cda_Def *cda, char *MyStr )
{	
	static 	ub4 hstb[512]; 	
   	char LoginString[100];
	strcpy(LoginString,MyStr);
	/* Connect to Oracle */   	
	
	if (olog(lda_c, (ub1 *)hstb, LoginString, -1, (char *)0, -1,(char *) 0, -1, (ub4)OCI_LM_DEF))
	{	
		sprintf(log_str,"Unable to connect as %s\n", MyStr); 
		err_write(log_str);
		return(FALSE);	
	}
 	/* Open a cursor for executing the PL/SQL block. */
  	if (oopen(cda, lda_c, (text *) 0, -1, 0, (char *) 0, -1))
  	{	OracleError(cda); return(FALSE);	}
	
	sprintf(log_str," Connect hdb as %s sucessfully!\n", MyStr); 
	log_write(INF,1,log_str);
  	if (ocon(lda_c))
  	{	OracleError(lda_c); return(FALSE);	 } 
  	return(TRUE);
}

int DisconnectHDB(Lda_Def * lda,Cda_Def *cda)
{
	/*Close the cursor on which the PL/SQL block executed.*/
  	if (oclose(cda))
  	{	
		OracleError(cda); 
		return(FALSE);	
	} 
     	/* Log off. */
	if (ologof(lda))
  	{	
		OracleError(cda); 
		return(FALSE);    	
	}
    MySleep(10000);
 			
  	return(TRUE);
}


