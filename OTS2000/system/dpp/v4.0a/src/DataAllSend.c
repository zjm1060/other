/************************************************************************************************************************************************
FILENAME:	DataAllSend.c
FUNCTION:	
		functions of  form allsend data packet of h9000 updata format:AllSendPro()
		functions of  form allsend data packet :AllSend_ANA(),AllSend_IND(),AllSend_SOE()
		
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


				 
void AllSendPro(LCUID lcuno,unsigned char* Packet,unsigned short PacketLength) 
{  
	LAN_HEAD  pakhead; 

	if(PacketLength>0)
	 {
		 pakhead.length=PacketLength;
		 pakhead.dp_type=ALL_DB_TIME;
         pakhead.dest_id=0;
		 GetLocalHostId((UCHAR *)&pakhead.src_id);
		 pakhead.dest_stn=0;
    
		Record_AllSendBuf(pakhead,Packet);
				
		 #ifdef  SUN_OS
			Swap2Byte(&pakhead.length);
		 #endif
		 
		 lan_out((char *)Packet,pakhead,0);
	  }		
      
      
}

void AllSend_ANA(LCUID lcuno,unsigned char type_id,unsigned char db_type,unsigned short offset, unsigned short length,AIALLSTRUCT *all,DATE_TIME tm) 
{
	short k,n,m;
        unsigned char Packet[2046];
	unsigned short  PacketLength;
	
	if(length>0)
	{
	
		Packet[0]= type_id;
		Packet[1]= lcuno.stnid;
        	Packet[2]= lcuno.devid;
		Packet[3]= db_type;
		
		m = length;
	

	  do{
	
		if ( m > ANA_ALL_MAX) n = ANA_ALL_MAX;
		else n=m;
    
		PacketLength=8+n*6+8;/*printf("db_type=%d  all send num=%d\n",db_type,n);*/
     	 
       		memcpy(Packet+4,&n,2);

		#ifdef SUN_OS
		     Swap2Byte(Packet+4);  
		#endif
    	
		memcpy(Packet+6,&offset,2);
		
		#ifdef SUN_OS
		     Swap2Byte(Packet+6);    
		#endif

	    for(k=0;k<n;k++)
		{
			memcpy(Packet+8+k*6,&((all+offset+k)->status) ,2);   
			Swap2Byte(Packet+8+k*6); 

			memcpy(Packet+10+k*6, &((all+offset+k)->data),4);  
			Swap4Byte(Packet+10+k*6);
		}
	 	
		GetLocalHostId(&Packet[6*n+8]);
	  	Packet[6*n+9]= tm.second;
	  	Packet[6*n+10]= tm.minute;
	  	Packet[6*n+11]= tm.hour;
	  	Packet[6*n+12]= tm.day;
	  	Packet[6*n+13]= tm.month;

		memcpy(Packet+6*n+14,&(tm.year),2);		
		#ifdef SUN_OS
		      Swap2Byte(Packet+6*n+14);    
		#endif
	
        	AllSendPro(lcuno,Packet,PacketLength);  
        	
        	m = m - n; offset = offset+n;
        	
    	    }while( m!=0);
    	}

}

void AllSend_IMP(LCUID lcuno,unsigned char type_id,unsigned char db_type,unsigned short offset, unsigned short length,PIALLSTRUCT *all,DATE_TIME tm) 
{
	short k,n,m;
        unsigned char Packet[2046];
	unsigned short  PacketLength;
	
	if(length>0)
	{
	
		Packet[0]= type_id;
		Packet[1]= lcuno.stnid;
        	Packet[2]= lcuno.devid;
		Packet[3]= db_type;
		
		m = length;
	

	  do{
	
		if ( m > ANA_ALL_MAX) n = ANA_ALL_MAX;
		else n=m;
    
		PacketLength=8+n*6+8;/*printf("db_type=%d  all send num=%d\n",db_type,n);*/
     	 
       		memcpy(Packet+4,&n,2);

		#ifdef SUN_OS
		     Swap2Byte(Packet+4);  
		#endif
    	
		memcpy(Packet+6,&offset,2);
		
		#ifdef SUN_OS
		     Swap2Byte(Packet+6);    
		#endif

      	for(k=0;k<n;k++)
       	{
			memcpy(Packet+8+k*6,&((all+offset+k)->status) ,2);   
			Swap2Byte(Packet+8+k*6); 

			memcpy(Packet+10+k*6, &((all+offset+k)->data),4);  
			Swap4Byte(Packet+10+k*6); 
		}
	 	
		GetLocalHostId(&Packet[6*n+8]);
	  	Packet[6*n+9]= tm.second;
	  	Packet[6*n+10]= tm.minute;
	  	Packet[6*n+11]= tm.hour;
	  	Packet[6*n+12]= tm.day;
	  	Packet[6*n+13]= tm.month;

		memcpy(Packet+6*n+14,&(tm.year),2);		
		#ifdef SUN_OS
		      Swap2Byte(Packet+6*n+14);    
		#endif
	
        	AllSendPro(lcuno,Packet,PacketLength);  
        	
        	m = m - n; offset = offset+n;
        	
    	    }while( m!=0);
    	}

}
 
void AllSend_IND(LCUID lcuno,unsigned char db_type,unsigned short offset, unsigned short length, IND_DATA *all,DATE_TIME tm) 
{
	unsigned short k,n,m;
        unsigned char Packet[2046];
	unsigned short    PacketLength;
	
	if(length>0)
	{
	
		*(Packet+0)= DPS_STATUS_ALL;
		*(Packet+1)= lcuno.stnid;
       		*(Packet+2)= lcuno.devid;
		*(Packet+3)= db_type;

		m=length;
	

	   do{
		if ( m >IND_ALL_MAX)  n = IND_ALL_MAX;
		else n=m;
	
       	 	PacketLength=8+n*2+8;/* printf("db_type=%d all send num=%d\n",db_type,n); */

       		memcpy(Packet+4,&n,2);

		#ifdef SUN_OS
		     Swap2Byte(Packet+4);  
		#endif
    	
		memcpy(Packet+6,&offset,2);
		
		#ifdef SUN_OS
		     Swap2Byte(Packet+6);    
		#endif

		for (k=0;k<n;k++) 
		{
			memcpy(Packet+8+k*2,all+offset+k,2); 


			#ifdef SUN_OS
			    Swap2Byte(Packet+8+2*k);    
			#endif
			
		}	
		
		GetLocalHostId(&Packet[2*n+8]);
		Packet[2*n+9]= tm.second;
        	Packet[2*n+10]= tm.minute;
		Packet[2*n+11]= tm.hour;
		Packet[2*n+12]= tm.day;
		Packet[2*n+13]= tm.month;

		memcpy(Packet+2*n+14,&(tm.year),2);
		
		#ifdef SUN_OS
		      Swap2Byte(Packet+2*n+14);    
		#endif
			
		AllSendPro(lcuno,Packet,PacketLength);
		
		m = m -n ;offset = offset+n;
	
	    }while(m!=0);
	}
}


void AllSend_SOE(LCUID lcuno,unsigned short length,DI_STRUCT * dii_obj,DATE_TIME tm) 
{
	unsigned short k,n,m,no,off;
        unsigned char Packet[2046];
	unsigned short    PacketLength;
	
	if(length>0)
	{

		m=length;off=0;

		do{
			if ( m >SOE_PACKET_MAX)  n = SOE_PACKET_MAX;
			else n=m;
		
			for (k=0;k<n;k++) 
		       {

			*(Packet+0+k*18)= DPS_STATUS_ALL;
			*(Packet+1+k*18)= lcuno.stnid;
        		*(Packet+2+k*18)= lcuno.devid;
			*(Packet+3+k*18)= SOE_TYPE;
			
			no=1;

       			memcpy(Packet+4+k*18,&no,2);

			#ifdef SUN_OS
		     		Swap2Byte(Packet+4+k*18);  
			#endif
    	
			memcpy(Packet+6+k*18,&(dii_obj+off+k)->point_id,2);
		
			#ifdef SUN_OS
		     		Swap2Byte(Packet+6+k*18);    
			#endif

			memcpy(Packet+8+k*18,&(dii_obj+off+k)->status,2); 


			#ifdef SUN_OS
			    Swap2Byte(Packet+8+k*18);    
			#endif
		
			GetLocalHostId(&Packet[10+k*18]);
			
			Packet[11+k*18]= tm.second;
        		Packet[12+k*18]= tm.minute;
			Packet[13+k*18]= tm.hour;
			Packet[14+k*18]= tm.day;
			Packet[15+k*18]= tm.month;

			memcpy(Packet+16+k*18,&(tm.year),2);
		
			#ifdef SUN_OS
		      		Swap2Byte(Packet+16+k*18);    
			#endif	
		
			}
		
		  	PacketLength=n*18;/* printf("soe change in all_send_type of num=%d\n",n); */
		
			AllSendPro(lcuno,Packet,PacketLength);
		
			m = m -n ;off = off+n;
	
	    	}while(m!=0);
	
	}
}


void AllSend_OBJ(LCUID lcuno,unsigned char type_id,unsigned char db_type,unsigned short offset, unsigned short length,OBJALLSTRUCT *all,DATE_TIME tm) 
{
	short k,n,m;
        unsigned char Packet[2046];
	unsigned short  PacketLength;
	
	if(length>0)
	{
	
		Packet[0]= type_id;
		Packet[1]= lcuno.stnid;
        	Packet[2]= lcuno.devid;
		Packet[3]= db_type;
		
		m = length;
	

	  do{
	
		if ( m > ANA_ALL_MAX) n = ANA_ALL_MAX;
		else n=m;
    
		PacketLength=8+n*6+8;	/*printf("db_type=%d  all send num=%d\n",db_type,n);*/
     	 
       		memcpy(Packet+4,&n,2);

		#ifdef SUN_OS
		     Swap2Byte(Packet+4);  
		#endif
    	
		memcpy(Packet+6,&offset,2);
		
		#ifdef SUN_OS
		     Swap2Byte(Packet+6);    
		#endif

	       	for(k=0;k<n;k++)
        	{
			memcpy(Packet+8+k*6,&((all+offset+k)->fb_status1) ,2);   

			#ifdef SUN_OS
			       Swap2Byte(Packet+8+k*6); 
			#endif

			memcpy(Packet+10+k*6, &((all+offset+k)->fb_status2),2);  
 
			#ifdef SUN_OS
				Swap2Byte(Packet+10+k*6);  
			#endif

			memcpy(Packet+12+k*6, &((all+offset+k)->param),2);  
 
			#ifdef SUN_OS
				Swap2Byte(Packet+12+k*6);  
			#endif
			
		}
	 	
		GetLocalHostId(&Packet[6*n+8]);
	  	Packet[6*n+9]= tm.second;
	  	Packet[6*n+10]= tm.minute;
	  	Packet[6*n+11]= tm.hour;
	  	Packet[6*n+12]= tm.day;
	  	Packet[6*n+13]= tm.month;

		memcpy(Packet+6*n+14,&(tm.year),2);		
		#ifdef SUN_OS
		      Swap2Byte(Packet+6*n+14);    
		#endif
	
        	AllSendPro(lcuno,Packet,PacketLength);  
        	
        	m = m - n; offset = offset+n;
        	
    	    }while( m!=0);
    	}

}

void Record_AllSendBuf(LAN_HEAD phead, unsigned char *PacketBuf)
{	
	#ifdef DEBUG
	
	FILE *fp;
	int i,result,fh,pno,plen,x;
	
	struct _stat buf;	
	unsigned long  No,fsize;
	
	#ifdef WINDOWS_OS	
		char filename[256]={"\\home\\ems\\h9000\\his\\trap\\RecordAllSendBuf.txt"};
		char cmd[256]={"del "};
	#else
		char filename[256]={"//home//ems//h9000//his//trap//RecordAllSendBuf.txt"};
		char cmd[256]={"rm "};
	#endif


	 if((fh =_open( filename,_O_RDONLY))!=-1)
	 {
		/* printf("open file\n");*/

		 result =_fstat(fh,&buf);

		 if(result!=0)
			 printf("Bad file handle\n");
		 else
		 {
			 x=_close(fh);
			/* printf("close file\n");*/
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
		/*printf("open file\n");*/

		if(fp== NULL)	{printf(" can't create %s \n",filename); return;}
		
		rewind(fp);
		
		No=0;
		
		fprintf(fp,"%d\n",No);
		
		fclose(fp);	
		/*printf("close file\n");*/

	}
		
	fp = fopen(filename,"r+");
	/*printf("open file\n");*/

	 rewind(fp);

	 fscanf(fp,"%d",&No);
	 
	 /*printf("22 No=%d\n",No);*/
	 
	 fseek(fp,0,SEEK_END);
	
	 fprintf(fp,"\nNo=%d\n",No);
	 
	 plen=phead.length; pno=0;
	 
	 fprintf(fp,"length=%d, dp_type=%d, dest_id=%d, src_id=%d, dest_stn=%d, packet_no=%d\n", plen,phead.dp_type,phead.dest_id,phead.src_id,phead.dest_stn,pno);
		 	 
	 for(i=0;i<plen;i++)
	  
	 {	
		if(i!=0 && !(i%8))   fprintf(fp,"    ");
		if(i!=0 && !(i%24))  fprintf(fp,"\n");
		 	
		fprintf(fp,"%02x ",PacketBuf[i]);
		  					 
	  }
	  
	 No++;
		 
	 rewind(fp);
	 
	 fprintf(fp,"%d\n",No);		 
	 
	 fclose(fp);
	 /*printf("close file\n");*/
	

	#endif
}
