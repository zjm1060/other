/************************************************************************************************************************************************
FILENAME:	DataChangeSend.c
FUNCTION:	
		functions of  form data packet of h9000 updata format:ChangeSendPro()
		functions of  form data change packet :ChangeSend_ANA(),ChangeSend_IND(),ChangeSend_SOE()
		
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


				 
void ChangeSendPro(LCUID lcuno,unsigned char *Packet,unsigned short PacketLength) 
{
	 LAN_HEAD    pakhead;
	
	 if(PacketLength>0)
	 {
		 pakhead.length=PacketLength;
		 pakhead.dp_type=BJ_DB_TIME;
		 pakhead.dest_id=0;
		 GetLocalHostId(&pakhead.src_id);
		 pakhead.dest_stn=0;
		 
		 Record_ChangeBuf(pakhead,Packet);
		 
		 #ifdef  SUN_OS
			Swap2Byte(&pakhead.length);
		 #endif
		 
		 
		 
		 lan_out((char *)Packet,pakhead,0);
	  }
                 
}


USHORT ChangeSend_ANA(LCUID lcuno,USHORT head,USHORT tail,AIALMBUFSTRUCT *data_buf)
{            
	unsigned short PacketLength,num,i,sum,j;
	unsigned char Packet[2046];
	unsigned short len;

	j=0;PacketLength=0;sum=0;
	
	while(tail!=head)
	{			
	        	
	        num=(data_buf+tail)->number; 
          
                len = num*8+14; sum=sum+num;
                
		PacketLength = PacketLength+len;
			        		
		if(PacketLength  <=  PACKETLEN_MAX)
		{
                  		
 			memcpy(Packet+j,&((data_buf+tail)->type_id),4);
		
			memcpy(Packet+4+j,&num,2);
		
                	#ifdef  SUN_OS
				Swap2Byte(Packet+4+j);
	        	#endif

	       		for(i=0;i<num;i++)
			{

				memcpy(Packet+6+i*8+j,&((data_buf+tail)->ai_object[i].point_id),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+6+i*8+j);  
				#endif

				memcpy(Packet+8+i*8+j,&((data_buf+tail)->ai_object[i].status),2);
				
				#ifdef SUN_OS
					Swap2Byte(Packet+8+i*8+j);  
				#endif
	
				memcpy(Packet+10+i*8+j,&((data_buf+tail)->ai_object[i].data),4);

				#ifdef SUN_OS
					Swap4Byte(Packet+10+i*8+j);  
				#endif

			}

			memcpy(Packet+8*num+6+j,&((data_buf+tail)->host_id),6);
	
			memcpy(Packet+8*num+12+j,&((data_buf+tail)->year),2);
	
         		#ifdef SUN_OS
				Swap2Byte(Packet+8*num+12+j);
			#endif
				
			j = len+j;
			
			tail++;
		
			if(tail==ALM_BUF_SIZE) tail=0;	
			
			if( tail == head) 
			{ 
				/*printf("111....num=%d ANA change send \n",sum);*/ 
				
				ChangeSendPro(lcuno,Packet,PacketLength); break;
			}	 
		 }
			
		num=(data_buf+tail)->number; 
          
               	len = num*4+16;
                
		PacketLength = PacketLength+len;
		
		if(PacketLength	<= PACKETLEN_MAX){PacketLength = PacketLength-len;continue;}
				
		else			/* if(PacketLength >  PACKETLEN_MAX) */	
		{	
			PacketLength = PacketLength-len;
			
			/* printf("222....num=%d ANA change send\n",sum); */
			
			ChangeSendPro(lcuno,Packet,PacketLength);
			
			j=0;	PacketLength=0; sum=0;
		
	
		}
		
	}
	return tail;
}  

USHORT ChangeSend_IMP(LCUID lcuno,USHORT head,USHORT tail,PIALMBUFSTRUCT *data_buf)
{            
	unsigned short PacketLength,num,i,sum,j;
	unsigned char Packet[2046];
	unsigned short len;

	j=0;PacketLength=0;sum=0;
	
	while(tail!=head)
	{			
	        	
	        num=(data_buf+tail)->number; 
          
                len = num*8+14; sum=sum+num;
                
		PacketLength = PacketLength+len;
			        		
		if(PacketLength  <=  PACKETLEN_MAX)
		{
                  		
 			memcpy(Packet+j,&((data_buf+tail)->type_id),4);
		
			memcpy(Packet+4+j,&num,2);
		
                	#ifdef  SUN_OS
				Swap2Byte(Packet+4+j);
	        	#endif

	       		for(i=0;i<num;i++)
			{

				memcpy(Packet+6+i*8+j,&((data_buf+tail)->ai_object[i].point_id),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+6+i*8+j);  
				#endif

				memcpy(Packet+8+i*8+j,&((data_buf+tail)->ai_object[i].status),2);
				
				#ifdef SUN_OS
					Swap2Byte(Packet+8+i*8+j);  
				#endif
	
				memcpy(Packet+10+i*8+j,&((data_buf+tail)->ai_object[i].data),4);

				#ifdef SUN_OS
					Swap4Byte(Packet+10+i*8+j);  
				#endif

			}

			memcpy(Packet+8*num+6+j,&((data_buf+tail)->host_id),6);
	
			memcpy(Packet+8*num+12+j,&((data_buf+tail)->year),2);
	
         		#ifdef SUN_OS
				Swap2Byte(Packet+8*num+12+j);
			#endif
				
			j = len+j;
			
			tail++;
		
			if(tail==ALM_BUF_SIZE) tail=0;	
			
			if( tail == head) 
			{ 
				/*printf("111....num=%d ANA change send \n",sum);*/ 
				
				ChangeSendPro(lcuno,Packet,PacketLength); break;
			}	 
		 }
			
		num=(data_buf+tail)->number; 
          
               	len = num*4+16;
                
		PacketLength = PacketLength+len;
		
		if(PacketLength	<= PACKETLEN_MAX){PacketLength = PacketLength-len;continue;}
				
		else			/* if(PacketLength >  PACKETLEN_MAX) */	
		{	
			PacketLength = PacketLength-len;
			
			/* printf("222....num=%d ANA change send\n",sum); */
			
			ChangeSendPro(lcuno,Packet,PacketLength);
			
			j=0;	PacketLength=0; sum=0;
		
	
		}
		
	}
	return tail;
}  
 
  
USHORT ChangeSend_IND(LCUID lcuno,USHORT head,USHORT tail,DIALMBUFSTRUCT *data_buf)
{  
	unsigned short PacketLength,num,i,j,sum;
	unsigned char  Packet[2046];
	unsigned short len;
	
	
	j=0;PacketLength=0;sum=0;
	
	while(tail!=head)
	{			
	  
	        num=(data_buf+tail)->number; 
	        
	        /*printf(" 0 num=%d,sum=%d  tail=%d head=%d\n",num,sum,tail,head);*/
          
               	len = num*4+16; sum = sum+num;
                
		PacketLength = PacketLength+len;
			
		if(PacketLength  <=  PACKETLEN_MAX)
		{
               		
 			memcpy(Packet+j,&((data_buf+tail)->type_id),4);
		
			memcpy(Packet+4+j,&num,2);
		
                	#ifdef  SUN_OS
				Swap2Byte(Packet+4+j);
	        	#endif

	       		for(i=0;i<num;i++)
			{

				memcpy(Packet+6+i*4+j,&((data_buf+tail)->di_object[i].point_id),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+6+i*4+j);  
				#endif

				memcpy(Packet+8+i*4+j,&((data_buf+tail)->di_object[i].status),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+8+i*4+j);  
				#endif
		
			}
			memcpy(Packet+4*num+6+j,&((data_buf+tail)->ms),2);

			#ifdef SUN_OS
				Swap2Byte(Packet+4*num+6+j);  
			#endif

			memcpy(Packet+4*num+8+j,&((data_buf+tail)->host_id),6);
	
			memcpy(Packet+4*num+14+j,&((data_buf+tail)->year),2);
	
         		#ifdef SUN_OS
				Swap2Byte(Packet+4*num+14+j);
			#endif
				
			j = len+j;
			
			tail++;if(tail==ALM_BUF_SIZE) tail=0;	
			
			if( tail == head) { /*printf("111....num=%d IND change send \n",sum); */ChangeSendPro(lcuno,Packet,PacketLength); break;}
		} 
		
		/*printf(" 1 num=%d,sum=%d \n",num,sum);*/
		
		num=(data_buf+tail)->number; 
		
		/*printf(" 2 num=%d,sum=%d \n",num,sum);*/
          
               	len = num*4+16;
                
		PacketLength = PacketLength+len;
		
		if(PacketLength	<= PACKETLEN_MAX){PacketLength = PacketLength-len;continue;}
				
		else			/* if(PacketLength >  PACKETLEN_MAX) */	
		{	
			PacketLength = PacketLength-len;
			
			 /* printf("222....sum=%d IND change send\n",sum);  */
			
			ChangeSendPro(lcuno,Packet,PacketLength);
			
			j=0;	PacketLength=0; sum=0;
		
		}
		
			 			
	 }
		
	return tail;
}    
  

USHORT ChangeSend_SOE(LCUID lcuno,USHORT head,USHORT tail,DIALMBUFSTRUCT *data_buf)
{  
	unsigned short PacketLength,num,i,j,no,m,n,k;
	unsigned char  Packet[2046];
	unsigned short len;
	
	
		
	if(tail>head) no = ALM_BUF_SIZE-tail+head;
	else	no = head - tail;
	
	m=no;
	
	/*printf(" SOE no=%d,m=%d , tail=%d ,head=%d\n",no,m,tail,head);
*/	
	if( no>0) 
	{
		do{
		
			if(m > SOE_PACKET_MAX) n = SOE_PACKET_MAX;
			else	n=m;
			
			PacketLength = 0;j=0;
			
			for(k=0;k<n;k++)
			{			
	        		num=(data_buf+tail)->number; 
          
                		len = num*4+16;
                
				PacketLength = PacketLength+len;	        		
                		
 				memcpy(Packet+j,&((data_buf+tail)->type_id),4);
		
				memcpy(Packet+4+j,&num,2);
		
                		#ifdef  SUN_OS
					Swap2Byte(Packet+4+j);
	        		#endif

	       			for(i=0;i<num;i++)
				{	

				memcpy(Packet+6+i*4+j,&((data_buf+tail)->di_object[i].point_id),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+6+i*4+j);  
				#endif

				memcpy(Packet+8+i*4+j,&((data_buf+tail)->di_object[i].status),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+8+i*4+j);  
				#endif
		
				}
			
				memcpy(Packet+4*num+6+j,&((data_buf+tail)->ms),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+4*num+6+j);  
				#endif

				memcpy(Packet+4*num+8+j,&((data_buf+tail)->host_id),6);
	
				memcpy(Packet+4*num+14+j,&((data_buf+tail)->year),2);
	
         			#ifdef SUN_OS
					Swap2Byte(Packet+4*num+14+j);
				#endif
				
				j = len+j;
			
				tail++;if(tail==ALM_BUF_SIZE) tail=0;
				
				 		
		 	}
			
/*
		 printf("333....SOE change PacketLength=%d\n",PacketLength); 
*/
			
			ChangeSendPro(lcuno,Packet,PacketLength);
			
			m = m-n;
			
		}while(m!=0);
			
					
	 }

	return tail;
}

USHORT ChangeSend_OBJ(LCUID lcuno,USHORT head,USHORT tail,OBJBUFSTRUCT *data_buf)
{  
	unsigned short PacketLength,num,i,j,no,m,n,k;
	unsigned char  Packet[2046];
	unsigned short len;
	
	
		
	if(tail>head) no = ALM_BUF_SIZE-tail+head;
	else	no = head - tail;
	
	m=no;
	
	/*printf(" OBJ no=%d,m=%d , tail=%d ,head=%d\n",no,m,tail,head);*/

	if( no>0) 
	{
		do{
		
			if(m > OBJ_PACKET_MAX) n = OBJ_PACKET_MAX;
			else	n=m;
			
			PacketLength = 0;j=0;
			
			for(k=0;k<n;k++)
			{			
	        		num=(data_buf+tail)->number; 
          
                		len = num*8+16;
                
				PacketLength = PacketLength+len;	        		
                		
 				memcpy(Packet+j,&((data_buf+tail)->type_id),4);
		
				memcpy(Packet+4+j,&num,2);
		
                		#ifdef  SUN_OS
					Swap2Byte(Packet+4+j);
	        		#endif

	       			for(i=0;i<num;i++)
				{	
					memcpy(Packet+6+i*8+j,&((data_buf+tail)->obj_object[i].point_id),2);

					#ifdef SUN_OS
						Swap2Byte(Packet+6+i*8+j);  
					#endif

					memcpy(Packet+8+i*8+j,&((data_buf+tail)->obj_object[i].fb_status1),2);
				
					#ifdef SUN_OS
						Swap2Byte(Packet+8+i*8+j);  
					#endif

					memcpy(Packet+10+i*8+j,&((data_buf+tail)->obj_object[i].fb_status2),2);
				
					#ifdef SUN_OS
						Swap2Byte(Packet+10+i*8+j);  
					#endif

					memcpy(Packet+12+i*8+j,&((data_buf+tail)->obj_object[i].param),2);
				
					#ifdef SUN_OS
						Swap2Byte(Packet+12+i*8+j);  
					#endif
	
				}
			
				memcpy(Packet+8*num+6+j,&((data_buf+tail)->ms),2);

				#ifdef SUN_OS
					Swap2Byte(Packet+8*num+6+j);  
				#endif

				memcpy(Packet+8*num+8+j,&((data_buf+tail)->host_id),6);
	
				memcpy(Packet+8*num+14+j,&((data_buf+tail)->year),2);
	
         			#ifdef SUN_OS
					Swap2Byte(Packet+8*num+14+j);
				#endif
				
				j = len+j;
			
				tail++;if(tail==ALM_BUF_SIZE) tail=0;
				
				 		
		 	}
			
			/*printf("333....OBJALM change PacketLength=%d\n",PacketLength); */
			
			ChangeSendPro(lcuno,Packet,PacketLength);
			
			m = m-n;
			
		}while(m!=0);
			
					
	 }

	return tail;
}



void Record_ChangeBuf(LAN_HEAD phead, unsigned char *PacketBuf)
{	
#ifdef DEBUG

	FILE *fp;
	int i,result,fh;
	
	struct _stat buf;	
	unsigned long  No,fsize;

	int pno,plen;

	#ifdef WINDOWS_OS	
		char filename[256]={"\\home\\ems\\h9000\\his\\trap\\RecordChangeBuf.txt"};
		char cmd[256]={"del "};
	#else
		char filename[256]={"//home//ems//h9000//his//trap//RecordChangeBuf.txt"};
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
	

#endif
}

