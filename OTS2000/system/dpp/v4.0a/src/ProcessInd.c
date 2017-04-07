/************************************************************************************************************************************************
FILENAME:	ProcessInd.c
FUNCTION:	
		functions of  processing ind data including pol,soe,sysmsg,dout type to form all send packet :Process_IND_ALL()
		functions of  processing ind data including pol,soe,sysmsg,dout type to form alarm packet:Process_IND()
		functions of  record alarm packet before lan_out:Record_IND()
		functions of  record all send packet before lan_out:Record_IND_ALL()

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:	2006.04.20		ycx
************************************************************************************************************************************************/
#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"


/****************************** IND bitmap all process ********************************/

void Process_IND_ALL(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,DPP_DATA *dpp)
{
	unsigned short          i0,i,j;
	IND_DATA          		fmt;	

	unsigned short          *ind_new; 		
	unsigned short          *ind_fault;
	unsigned short          *ind_force;
	IND_DATA                *ind_all;	
	
/*	memset(ind_new,0,(DIN_SIZE/16+1)*2);
	memset(ind_fault,0,(DIN_SIZE/16+1)*2);
	memset(ind_force,0,(DIN_SIZE/16+1)*2);	*/
	
	switch( db_type)
	{
		
		case SOE_TYPE:
			if(length>DII_SIZE) 	{Err_log("soe length > DII_SIZE\n");return;}
			ind_new = dpp->dii_new;
			ind_fault=dpp->dii_fault;
			ind_force=dpp->dii_force;
			ind_all	= dpp->dii_all;

			break;
			
		case SYSMSG_TYPE:
			if(length>MSG_SIZE) 	{Err_log("msg length > MSG_SIZE\n");return;}
			ind_new = dpp->msg_new;
			ind_fault=dpp->msg_fault;
			ind_force=dpp->msg_force;
			ind_all	= dpp->msg_all;

			break;
			
		case POL_TYPE:
			if(length>DIN_SIZE) 	{Err_log("pol length > DIN_SIZE\n");return;}
            		ind_new = dpp->pol_new;
			ind_fault=dpp->pol_fault;
			ind_force=dpp->pol_force;
			ind_all	= dpp->din_all;

			break;
			
		case OUTS_TYPE:
			if(length>DOUT_SIZE) 	{Err_log("dout length > DOUT_SIZE\n");return;}
           		ind_new = dpp->dout_new;
			ind_fault=dpp->dout_fault;
			ind_force=dpp->dout_force;
			ind_all	= dpp->dout_all;

			break;
			
		default:
			Err_log("no this data type of IND\n");
			return;
	}

	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 
		
		memset(&ind_all[i],0,2);
		 
		/*modified by wzg 20100709
		if  (dpp->ind_db[i].iost.scanEnable  ==  1) */ 
		{
			   
			   memcpy(&fmt,&(dpp->ind_db[i].status),2);
			   
			   fmt.alm_type = IND_ALM_TYPE_NORM;
			   
			   fmt.rt_state = (ind_new[j] >> i0) & 0x01;

			   if (((ind_force[j] >> i0) & 0x01) == 1) fmt.man_set=1;    /* force status */
			   else fmt.man_set=0;
			   
			   if (((ind_fault[j] >> i0) & 0x01) == 1) fmt.chan_fail=1;    /* Channel fault */
			   else fmt.chan_fail=0;
          
			   ind_all[i]=fmt;
		   
   
		}		
	}
	
}
	
/****** IND bitmap change alarm record processs*******/

void Process_IND(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm)
{
	unsigned short          i0,i,j,m,n,off,num,st;
	unsigned short 		vary,vary1,vary2,fault,force;
	
	DIALMBUFSTRUCT   	indbuf[ALM_BUF_SIZE];
	DI_STRUCT		di_object[DIN_SIZE];	
	unsigned short		ptr1,ptr2;
	unsigned short		wordlen;
	
	unsigned short          *ind_new;		
	unsigned short          *ind_fault;
	unsigned short          *ind_force;
	unsigned short          *ind_old;
	unsigned short          *ind_fault_old;
	unsigned short          *ind_force_old;


	switch( db_type)
	{
		case SYSMSG_TYPE:
			if(length>MSG_SIZE) 	{Err_log("msg length > MSG_SIZE\n");return;}
		   	ptr1  = dpp->msg_ptr1;
			ptr2  = dpp->msg_ptr2;
                        
			ind_new = dpp->msg_new;
			ind_fault=dpp->msg_fault;
			ind_force=dpp->msg_force;

			ind_old=dpp->msg_old;
			ind_fault_old=dpp->msg_fault_old;
			ind_force_old=dpp->msg_force_old;
			break;

		case POL_TYPE:
			if(length>DIN_SIZE) 	{Err_log("pol length > DIN_SIZE\n");return;}
			ptr1  = dpp->din_ptr1;
			ptr2  = dpp->din_ptr2;
              		
            		ind_new = dpp->pol_new;
			ind_fault=dpp->pol_fault;
			ind_force=dpp->pol_force;

			ind_old =dpp->pol_old;
			ind_fault_old=dpp->pol_fault_old;
			ind_force_old=dpp->pol_force_old;

			break;

		case OUTS_TYPE:
			if(length>DOUT_SIZE) 	{Err_log("dout length > DOUT_SIZE\n");return;}	
			ptr1  = dpp->dout_ptr1;
			ptr2  = dpp->dout_ptr2;
                        
            		ind_new = dpp->dout_new;
			ind_fault=dpp->dout_fault;
			ind_force=dpp->dout_force;

			ind_old =dpp->dout_old;
			ind_fault_old=dpp->dout_fault_old;
			ind_force_old=dpp->dout_force_old;

			break;

		default:
			Err_log("no this data type of IND\n");
			return;
	}

	/*1111111111111111*/
	
	num=0;

	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 
		
	  	vary = ind_old[j]  ^  ind_new[j];   /* Find the Change bit */

	  	if (((( vary>>i0) & 0x01) == 1)&&(dpp->ind_db[i].iost.scanEnable  ==  1))
		   	
	  	{
			   memset(&di_object[num], 0, sizeof(di_object[num]));

			   memcpy(&di_object[num].status,&(dpp->ind_db[i].status),2);
			   			   
			   di_object[num].point_id=i;
			   di_object[num].status.alm_type = IND_ALM_TYPE_CHANGE;
			   di_object[num].status.rt_state = (ind_new[j] >> i0) & 0x01 ;
			   
			   memcpy(&st,&di_object[num].status,2);
			   
			   /*printf("point_id=%d  st=%x\n",di_object[num].point_id,st);*/
			   
			   num++;
			   
			 

	  	} 				
		  
	 } 

	/*222222222*/
	
	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 		
		
		vary2 = ind_fault_old[j]  ^  ind_fault[j]; 

		if (((( vary2>>i0) & 0x01) == 1)&&(dpp->ind_db[i].iost.scanEnable  ==  1))
	       {
			memset(&di_object[num], 0, sizeof(di_object[num]));
	    	    
	  	    fault = (ind_fault[j] >> i0) & 0x01;
	  	    memcpy(&di_object[num].status,&(dpp->ind_db[i].status),2);
	  	    
		    di_object[num].point_id=i;
		    
		    di_object[num].status.alm_type = IND_ALM_TYPE_CHANFAIL;
		    di_object[num].status.chan_fail = fault;
		    di_object[num].status.rt_state = (ind_new[j] >> i0) & 0x01;
		    
		    num++;		  							  
	       }

	}


	/*3333333333*/

	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 
		
				
		vary1 = ind_force_old[j]  ^  ind_force[j];  

		if (((( vary1>>i0) & 0x01) == 1)&&(dpp->ind_db[i].iost.scanEnable  ==  1))
	  	{
	    	    
			memset(&di_object[num], 0, sizeof(di_object[num]));
			
			force = (ind_force[j] >> i0) & 0x01;
	    	    memcpy(&di_object[num].status,&(dpp->ind_db[i].status),2);

		    di_object[num].point_id=i;
		     
		    di_object[num].status.alm_type = IND_ALM_TYPE_MANFORCE;
		    di_object[num].status.man_set  = force;
		    di_object[num].status.rt_state = (ind_new[j] >> i0) & 0x01;

		    num++;		  
		 }	
					
		  
	 }

	


	m = num;off=0;
	
	
	
	if( num>0)
	{
		do{
	       
		if ( m > IND_ALM_MAX) n = IND_ALM_MAX;
		else n=m;

		memset(&indbuf[ptr1], 0, sizeof(indbuf[ptr1]));
		
		indbuf[ptr1].type_id = DPS_IND_ALM;

		indbuf[ptr1].stn_id =lcuno.stnid;

		indbuf[ptr1].dev_id =lcuno.devid;

		indbuf[ptr1].data_type=db_type;

		indbuf[ptr1].number = n;

		memcpy(indbuf[ptr1].di_object,&di_object[off],n*sizeof(DI_STRUCT));

		indbuf[ptr1].ms=tm.ms;

		GetLocalHostId(&indbuf[ptr1].host_id);

		indbuf[ptr1].second=tm.second;

		indbuf[ptr1].minute=tm.minute;

		indbuf[ptr1].hour=tm.hour;

		indbuf[ptr1].day=tm.day;

		indbuf[ptr1].month=tm.month;

		indbuf[ptr1].year=tm.year;
		
		/*printf("111111111111 num =%d\n",num);*/
		
		Record_IND(ptr1,indbuf);
		  
		 
		if( ptr1 == ALM_BUF_SIZE-1)	ptr1 = 0;

		else ptr1++;

		m = m - n; off = off+n;

	
		}while(m!=0);

		switch( db_type)
		{		
			case SYSMSG_TYPE:				
				if(ptr2<ptr1)			
						memcpy(dpp->msgbuf+ptr2, indbuf+ptr2,(ptr1-ptr2)*sizeof(DIALMBUFSTRUCT));
				else{
						memcpy(dpp->msgbuf+ptr2, indbuf+ptr2,(ALM_BUF_SIZE-ptr2)*sizeof(DIALMBUFSTRUCT));
						if(ptr1>0)
						memcpy(dpp->msgbuf, indbuf,ptr1*sizeof(DIALMBUFSTRUCT));
					}

				dpp->msg_ptr1 = ptr1;
				dpp->msg_ptr2 = ptr2;
			break;

			case POL_TYPE:
				if(ptr2<ptr1)				
					memcpy(dpp->dinbuf+ptr2, indbuf+ptr2,(ptr1-ptr2)*sizeof(DIALMBUFSTRUCT));
				else{
						memcpy(dpp->dinbuf+ptr2, indbuf+ptr2,(ALM_BUF_SIZE-ptr2)*sizeof(DIALMBUFSTRUCT));
						if(ptr1>0)
						memcpy(dpp->dinbuf, indbuf,ptr1*sizeof(DIALMBUFSTRUCT));
					}
			
				dpp->din_ptr1 = ptr1;
				dpp->din_ptr2 = ptr2;
			break;

			case OUTS_TYPE:
				if(ptr2<ptr1)				
					memcpy(dpp->doutbuf+ptr2, indbuf+ptr2,(ptr1-ptr2)*sizeof(DIALMBUFSTRUCT));
				else{
						memcpy(dpp->doutbuf+ptr2, indbuf+ptr2,(ALM_BUF_SIZE-ptr2)*sizeof(DIALMBUFSTRUCT));
						if(ptr1>0)
						memcpy(dpp->doutbuf, indbuf,ptr1*sizeof(DIALMBUFSTRUCT));
					}

        		dpp->dout_ptr1 = ptr1;
				dpp->dout_ptr2 = ptr2;
			break;

			default:
			break;
		}	
  	}
	
	j = offset/16;	
	wordlen = Mod16(offset+length);	
	for (i=j;i<wordlen;i++)  
	{	
		ind_old[i] = ind_new[i];
    	ind_force_old[i] = ind_force[i];
		ind_fault_old[i] = ind_fault[i];
	}		
}


void Record_IND(unsigned short ptr,DIALMBUFSTRUCT *indbuf)
{	
#ifdef DEBUG
	
	FILE *fp;
	int i,num,result,fh;
	unsigned short status;
	
	struct _stat buf;	
	unsigned long  No,fsize;

	#ifdef WINDOWS_OS	
		char filename[256]={"\\home\\ems\\h9000\\his\\trap\\ind_alm.txt"};
		char cmd[256]={"del "};
	#else
		char filename[256]={"//home//ems//h9000//his//trap//ind_alm.txt"};
		char cmd[256]={"rm "};
	#endif


	 if((fh = _open(filename,_O_RDONLY))!=-1)
	 {
		 result =_fstat(fh,&buf);

		 if(result!=0)
			 printf("Bad file handle\n");
		 else
		 {
			 _close(fh);
			 

			 if((fsize = buf.st_size )>=RECORD_FILE_SIZE) 
				
			 {
				 printf("%s size=%ld\n",filename,fsize);
				 strcat(cmd,filename);				 
				 printf("%s\n",cmd);
				 system(cmd); 
			 }
			 
		 }
	 }

	
	if (_access(filename,0)==-1)
	{
		printf("%s is not exist\n",filename);
			
		fp = fopen(filename,"w+");
		
		if(fp== NULL)	{printf(" can't create %s \n",filename); return;}
		
		rewind(fp);
		
		No=0;
		
		fprintf(fp,"%d\n",No);
		
		fclose(fp);	
	}
	
	fp = fopen(filename,"r+");

	     rewind(fp);
	
	     fscanf(fp,"%d",&No);
		 
	     /*printf("11No=%d\n",No);*/

		 fseek(fp,0,SEEK_END);
	
		 fprintf(fp,"\nNo=%d,ptr=%d\n",No,ptr);
		 
		 fprintf(fp,"type_id=%d ,stn_id=%d ,dev_id=%d ,data_type=%d ,number=%d \n",(indbuf+ptr)->type_id,
		 			   	    	   (indbuf+ptr)->stn_id,
		 			   	    	   (indbuf+ptr)->dev_id,
		 			   	    	   (indbuf+ptr)->data_type,
		 			   	    	   (indbuf+ptr)->number);
		 num = (indbuf+ptr)->number;
		 
		 for(i=0;i<num;i++) 
		 {	
		 	memcpy(&status,&((indbuf+ptr)->di_object[i].status),2);
		 	
		 	fprintf(fp,"pt_id=%5d,status=%4x\n",(indbuf+ptr)->di_object[i].point_id,status);
		  					     
		  }
		 
		 fprintf(fp,"ms=%d,host_id=%d,sec=%d,min=%d,hour=%d,day=%d,month=%d,year=%d\n\n",
			                                    (indbuf+ptr)->ms,
			                                    (indbuf+ptr)->host_id,
		 				     	    (indbuf+ptr)->second,(indbuf+ptr)->minute,
		 				     	    (indbuf+ptr)->hour,(indbuf+ptr)->day,
		 				     	    (indbuf+ptr)->month,(indbuf+ptr)->year);		 				     
		 	
		 No++;
		 
		 rewind(fp);	 
		 
		 fprintf(fp,"%d",No);

		 fclose(fp);
	
#endif
}

void Record_IND_ALL(UCHAR db_type,unsigned short offset,unsigned short length,IND_DATA *ind_all)
{	
#ifdef DEBUG

	FILE *fp;
	
	int i,result,fh;
	
	struct _stat buf;	
	unsigned long  No,fsize;

	#ifdef WINDOWS_OS	
		char filename[256]={"\\home\\ems\\h9000\\his\\trap\\ind_all.txt"};
		char cmd[256]={"del "};
	#else
		char filename[256]={"//home//ems//h9000//his//trap//ind_all.txt"};
		char cmd[256]={"rm "};
	#endif

	 if((fh = _open(filename,_O_RDONLY))!=-1)
	 {
		 result =_fstat(fh,&buf);

		 if(result!=0)
			 printf("Bad file handle\n");
		 else
		 {
			  _close(fh);

			 if((fsize = buf.st_size )>= RECORD_FILE_SIZE) 				
			 {		
				 printf("%s size=%ld\n",filename,fsize);
			 	 strcat(cmd,filename);
				 printf("%s\n",cmd);
				 system(cmd); 
			 }
			 
		 }
	 }

	
	if (_access(filename,0)==-1)
	{
		printf("%s is not exist\n",filename);
			
		fp = fopen(filename,"w+");
		
		if(fp== NULL)	{printf(" can't create %s \n",filename); return;}
		
		rewind(fp);
		
		No=0;
		
		fprintf(fp,"%d\n",No);
		
		fclose(fp);	
	}

	fp = fopen(filename,"r+");
	
	 rewind(fp);

	 fscanf(fp,"%d",&No);		
	 
	 /*printf("22No=%d\n",No);*/
	     
	 fseek(fp,0,SEEK_END);
	
	 fprintf(fp,"\nNo=%d,db_type=%d,len=%d\n",No,db_type,length);
	 
	 for(i=offset;i<offset+length;i++){ fprintf(fp,"==%4x",*(ind_all+i));if((i+1)%10 ==0) fprintf(fp,"\n");}
		 	           		
	 No++;
	 
	 rewind(fp);
	 
	 fprintf(fp,"%d\n",No);		 
	 
	 fclose(fp);
	

#endif 
  
}
