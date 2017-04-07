/************************************************************************************************************************************************
FILENAME:	ReadNewData.c
FUNCTION:	
		functions of read new data from share memory of plc_data to dpp_data:ReadNewValue_ANA(),ReadNewValue_IND(),ReadNewValue_IMP()
		

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


/************************************** read ANA new value,quality and force *******************************/
		
void ReadNewValue_ANA(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,UCHAR *ana_data,UCHAR *ana_qlt,UCHAR *ana_en,DPP_DATA *dpp)
{
	unsigned short		k,j,n;
	unsigned short	    *ana_new;
	unsigned short      *ana_fault;
	unsigned short      *ana_force;
	
		
	switch( db_type)
	{
		case ANA_TYPE:
			if(length>AIALL_SIZE) 	{Err_log("ai length > AIALL_SIZE\n");return;}

			ana_new		= dpp->ai_new;
			ana_fault	= dpp->ai_fault;
			ana_force	= dpp->ai_force;
			break;
			
		case OUTA_TYPE:
			if(length>AO_SIZE) 	{Err_log("ao length > AO_SIZE\n");return;}

			ana_new		= dpp->ao_new;
			ana_fault	= dpp->ao_fault;
			ana_force	= dpp->ao_force;
			break;
			
		default:
			Err_log("no this data type of ANA\n");
			return;
	}

	/***** read AI/AO value *****/

	 j=offset;
	 n=0;

	 if(length>0)
	 {
		 for(k=0;k<length;k++)
		 { 		
			 *(ana_new+j)=*(ana_data+n)+(*(ana_data+n+1)<<8);
			 
			 n=n+2; j=j+1; 
		 }
	 }	 

	/***** read AI/AO FAULT and FORCE status *****/

	 GetWordFromChar(offset,length,ana_qlt,ana_fault);

	 GetWordFromChar(offset,length,ana_en,ana_force);

	 /*wzg 20070506
	 for(k=0; k<length; k++)
	 {
		unsigned short	dest_byte_pos, dest_bit_pos, src_byte_pos, src_bit_pos;
		dest_byte_pos= (offset+k)/16;
		dest_bit_pos= (offset+k)%16;
		src_byte_pos= k/8;
		src_bit_pos= k%8;
		ana_fault[dest_byte_pos]= write_bit(ana_fault[dest_byte_pos], dest_bit_pos, (unsigned short)((ana_qlt[src_byte_pos]>>(src_bit_pos))&0x01));
		ana_force[dest_byte_pos]= write_bit(ana_force[dest_byte_pos], dest_bit_pos, (unsigned short)((ana_en[src_byte_pos]>>(src_bit_pos))&0x01));
	 }*/


}

/************************************** read IMP new value,quality and force *******************************/

void ReadNewValue_IMP(LCUID lcuno,USHORT offset,USHORT length,UCHAR *pi_data,UCHAR *pi_qlt,UCHAR *pi_en,DPP_DATA *dpp)
{

	
	unsigned short   k,j,n;
	
	if(length>PI_SIZE) 	{Err_log("imp length > PI_SIZE\n");return;}

	/***** read PULSE value *****/

		j= offset;
		n=0;

		if(length>0)
		{
			for(k=0;k<length;k++)
			{
				
			        *(dpp->pi_new+j)=*(pi_data+n)+(*(pi_data+n+1)<<8)+(*(pi_data+n+2)<<16)+(*(pi_data+n+3)<<24);

				n=n+4; j=j+1; 
			}
		}
		/***** read PULSE CHANNEL FAULT and FORCE status *****/

		GetWordFromChar(offset,length,pi_qlt,dpp->pi_fault);

		GetWordFromChar(offset,length,pi_en,dpp->pi_force);

	/*wzg 20070506
	 for(k=0; k<length; k++)
	 {
		unsigned short	dest_byte_pos, dest_bit_pos, src_byte_pos, src_bit_pos;
		dest_byte_pos= (offset+k)/16;
		dest_bit_pos= (offset+k)%16;
		src_byte_pos= k/8;
		src_bit_pos= k%8;
		dpp->pi_fault[dest_byte_pos]= write_bit(dpp->pi_fault[dest_byte_pos], dest_bit_pos, (unsigned short)((pi_qlt[src_byte_pos]>>(src_bit_pos))&0x01));
		dpp->pi_force[dest_byte_pos]= write_bit(dpp->pi_force[dest_byte_pos], dest_bit_pos, (unsigned short)((pi_en[src_byte_pos]>>(src_bit_pos))&0x01));
	 }*/

}

/***************** read IND  bitmap, quality and force value  **********************/

void ReadNewValue_IND(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,UCHAR *di_data,UCHAR *di_qlt,UCHAR *di_en,DPP_DATA *dpp)
{

	unsigned short      *ind_new; 		
	unsigned short      *ind_fault;
	unsigned short      *ind_force;
			
	switch( db_type)
	{
		case SYSMSG_TYPE:			
			if(length>MSG_SIZE) 	{Err_log("msg length > MSG_SIZE\n");return;}	
								
			ind_new = dpp->msg_new;
			ind_fault=dpp->msg_fault;
			ind_force=dpp->msg_force;

			break;
			
		case POL_TYPE:
			if(length>DIN_SIZE) 	{Err_log("pol length > DIN_SIZE\n");return;}
			
			ind_new = dpp->pol_new;
			ind_fault=dpp->pol_fault;
			ind_force=dpp->pol_force;

			break;
			
		case OUTS_TYPE:
			if(length>DOUT_SIZE) 	{Err_log("dout length > DOUT_SIZE\n");return;}
			
			ind_new = dpp->dout_new;
			ind_fault=dpp->dout_fault;
			ind_force=dpp->dout_force;

			break;
			
	     case SOE_TYPE:
	        if(length>DII_SIZE) 	{Err_log("soe length > DII_SIZE\n");return;}
	       					
			ind_new = dpp->dii_new;
			ind_fault=dpp->dii_fault;
			ind_force=dpp->dii_force;
			break;

		default:
			Err_log("no this data type of IND\n");
			return;
			
	}

	/***** read IND value *****/

	GetWordFromChar(offset,length,di_data,ind_new);


	/***** read IND CHANNEL FAULT and FORCE status *****/

	GetWordFromChar(offset,length,di_qlt,ind_fault);

	GetWordFromChar(offset,length,di_en,ind_force);

	/*wzg 20070506
	 for(k=0; k<length; k++)
	 {
		unsigned short	dest_byte_pos, dest_bit_pos, src_byte_pos, src_bit_pos;
		dest_byte_pos= (offset+k)/16;
		dest_bit_pos= (offset+k)%16;
		src_byte_pos= k/8;
		src_bit_pos= k%8;
		ind_new[dest_byte_pos]= write_bit(ind_new[dest_byte_pos], dest_bit_pos, (unsigned short)((di_data[src_byte_pos]>>(src_bit_pos))&0x01));
		ind_fault[dest_byte_pos]= write_bit(ind_fault[dest_byte_pos], dest_bit_pos, (unsigned short)((di_qlt[src_byte_pos]>>(src_bit_pos))&0x01));
		ind_force[dest_byte_pos]= write_bit(ind_force[dest_byte_pos], dest_bit_pos, (unsigned short)((di_en[src_byte_pos]>>(src_bit_pos))&0x01));
	 }*/

}


  
void ReadNewValue_PARAM(LCUID lcuno,USHORT offset,USHORT length,UCHAR *param_data,DPP_DATA *dpp)
{

	unsigned short	n,j,k;
    

	/***** read PARAM value *****/	
	
	if(length>PARAM_SIZE)	{Err_log("param length>PARAM_SIZE\n");return;}
			

	 j=offset;
	 n=0;

	 if(length>0)
	 {
		 for(k=0;k<length;k++)
		 { 		
		 
			 *(dpp->param_new+j)=*(param_data+n)+(*(param_data+n+1)<<8);
			 
			 n=n+2; j=j+1; 
		 }
	 }	


}


void ReadNewValue_OBJ(LCUID lcuno,USHORT offset,USHORT length, struct obj_data_seg *obj_data,DPP_DATA *dpp)
{

	unsigned short	j,k;
	unsigned short  st[3];
	
 	if(length>OBJ_SIZE)	{Err_log("OBJ length>OBJ_SIZE\n");return;}

	 j=offset;
	

	 if(length>0)
	 {
		 for(k=0;k<length;k++)
		 { 		
		 	
			st[0]	=(obj_data+k)->status1[0]+((obj_data+k)->status1[1]<<8);/*printf("k=%d,st[0]=%d\n",k,st[0]);*/
			st[1]	=(obj_data+k)->status2[0]+((obj_data+k)->status2[1]<<8);
			st[2]	=(obj_data+k)->param[0]+((obj_data+k)->param[1]<<8);
			
			memcpy(dpp->obj_new+j,st,6);
			 
			j=j+1; 
		 }
	 }	


}


void GetWordFromChar(USHORT offset,USHORT length,UCHAR *qchar,USHORT *qword)
{
	unsigned short		m,j,k,n,qlt,qlt0;
	unsigned short		bytelen,jend,mend,nlast,blast;

	/***** get short array from char array *****/
	
	j = offset/16;		/*the first short array no*/
	m = offset%16;		/*the first short array bit no:[0,15]*/

	jend = (offset+length)/16;	/*the last short array no*/
	mend = (offset+length)%16;	/*the last short array bit no:[0,15]*/

	bytelen = Mod8(length);	

	nlast = bytelen-1;			/*the last char array no*/
	blast = length-nlast*8;		/*the last char array bit num:[1,8]*/


	if(bytelen>0)
	{
		if(m<8)
		{
			
			qlt = read_bit(*(qword+j),0,m-1);

			if(nlast==0)
			{

				qlt0 = read_bit(*(qword+j),m+blast,15);

				*(qword+j)= qlt|((*qchar)<<m)|(qlt0<<(m+blast));

			}
			else if(nlast==1)
			{			
				if(m+8+blast>=16)
				{
					qlt0=0;
				}
				else
				{
					qlt0 = read_bit(*(qword+j),m+8+blast,15);
				}

				*(qword+j)= qlt|((*qchar)<<m)|((*(qchar+1))<<(8+m))|(qlt0<<(m+8+blast));

				if(m+8+blast>16)
				{
					j=j+1;

					qlt = read_bit(*(qchar+1),8-m,blast-1);

					qlt0 = read_bit(*(qword+j),m+blast-8,15);

					*(qword+j)= qlt |(qlt0<<(m+blast-8));		
				}		
			}
			else/*if (nlast>=2)*/
			{

				*(qword+j)= qlt|((*qchar)<<m)|((*(qchar+1))<<(8+m));

				n=1;j=j+1;

				while((j<jend && mend ==0) ||(j<=jend && mend!=0))
				{
					
					qlt = read_bit(*(qchar+n),8-m,7);

					if(n+1==nlast)
					{
						qlt0 = read_bit(*(qword+j),m+blast,15);

						*(qword+j)= qlt | ((*(qchar+n+1))<<m)| (qlt0<<(m+blast));

					}
					else if(n+2==nlast)
					{	
						qlt0 = read_bit(*(qword+j),m+8+blast,15);

						*(qword+j)= qlt | (*(qchar+n+1)<<m)|(*(qchar+n+2)<<(8+m))|(qlt0<<(m+8+blast));

						if(m+8+blast>16)
						{
							j=j+1;

							qlt = read_bit(*(qchar+n+2),8-m,blast-1);

							qlt0 = read_bit(*(qword+j),blast+m-8,15);

							*(qword+j)= qlt | (qlt0<<(blast+m-8));

						}
					}
					else
					{

						*(qword+j)= qlt | (*(qchar+n+1)<<m)|(*(qchar+n+2)<<(8+m));
					}
					n=n+2; j=j+1;
	
				}
			}				
		}
		else if(m == 8)
		{		
			qlt = read_bit(*(qword+j),0,m-1);

			if(nlast==0)
			{

				qlt0 = read_bit(*(qword+j),m+blast,15);

				*(qword+j)= qlt|((*qchar)<<m)|(qlt0<<(m+blast));

			}

			else if(nlast==1)
			{
				
				*(qword+j)= qlt|((*qchar)<<m);

				j=j+1;

				qlt0 = read_bit(*(qword+j),blast,15);

				*(qword+j)= (*(qchar+1))|(qlt0<<blast);		
						
			}

			else/*if (nlast>=2)*/
			{

				*(qword+j)= qlt | ((*qchar)<<m);	

				n=1;j=j+1;

				while((j<jend && mend ==0) ||(j<=jend && mend!=0))
				{
					
					if(n+1==nlast)
					{
						qlt0 = read_bit(*(qword+j),8+blast,15);

						*(qword+j)= *(qchar+n) | ((*(qchar+n+1))<<8)| (qlt0<<(8+blast));

					}
					else if(n+2==nlast)
					{	

						*(qword+j)= *(qchar+n) | ((*(qchar+n+1))<<8);

						qlt0 = read_bit(*(qword+j),blast,15);

						j=j+1;

						*(qword+j)= (*(qchar+n+2))|(qlt0<<blast);
					}
					else
					{

						*(qword+j)= *(qchar+n) | (*(qchar+n+1)<<m);				
					}
					n=n+2; j=j+1;					
				}						
			}
		}else	/*if(m>8)*/
		{	
			qlt = read_bit(*(qword+j),0,m-1);

			if(nlast==0)
			{

				if(m+blast>=16)
				{
					qlt0=0;
				}
				else
				{
					qlt0 = read_bit(*(qword+j),m+blast,15);
				}

				*(qword+j)= qlt|((*qchar)<<m)|(qlt0<<(m+blast));

				if(m+blast>16)
				{
					j=j+1;

					qlt = read_bit(*qchar,16-m,blast-1);

					qlt0 = read_bit(*(qword+j),m+blast-16,15);

					*(qword+j)= qlt |(qlt0<<(m+blast-16));		
				}		

			}
			else if(nlast==1)
			{
				
				*(qword+j)= qlt|((*qchar)<<m);

				j=j+1;

				qlt = read_bit(*qchar,16-m,7);

				qlt0 = read_bit(*(qword+j),m-8+blast,15);

				*(qword+j)= qlt| ((*(qchar+1))<<(m-8))|(qlt0<<(m-8+blast));		
						
			}
			else/*if (nlast>=2)*/
			{

				*(qword+j)= qlt | ((*qchar)<<m);

				n=0;j=j+1;

				while((j<jend && mend ==0) ||(j<=jend && mend!=0))
				{
					
					qlt = read_bit(*(qchar+n),16-m,7);

					if(n+1==nlast)
					{

						qlt0 = read_bit(*(qword+j),m-8+blast,15);

						*(qword+j)= qlt | ((*(qchar+n+1))<<(m-8))| (qlt0<<(m-8+blast));

					}
					else if(n+2==nlast)
					{	

						qlt0 = read_bit(*(qword+j),m+blast,15);

						*(qword+j)= qlt | (*(qchar+n+1)<<(m-8))| ((*(qchar+n+2))<<m)|(qlt0<<(m+blast));

						if(m+blast>16)
						{
							j=j+1;

							qlt = read_bit(*(qchar+n+2),16-m,blast-1);

							qlt0 = read_bit(*(qword+j),blast+m-16,15);

							*(qword+j)= qlt|(qlt0<<(blast+m-16));		
						}
					}
					else
					{

						*(qword+j)= qlt | (*(qchar+n+1))<<(m-8) |(*(qchar+n+2)<<m);
					}

					n=n+2;j=j+1;

				}
			}				
		}			
	}
}
