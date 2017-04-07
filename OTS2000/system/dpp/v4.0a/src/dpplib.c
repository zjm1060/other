/************************************************************************************************************************************************
FILENAME:	dpplib.c
FUNCTION:	
	
	functions of dpplib :Mod16(),read_bit(),

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
2006.04.20		ycx
************************************************************************************************************************************************/
#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"

/*modified by wzg 20070505*/
int Mod8(int len)
{ 
	return (len+7)/8;
}


int Mod16(int len)
{ 
	return (len+15)/16;
}
/*
int Mod8(int len)
{ 
   int i =0;
   
   if(len>0)
    {
      while(1)
       { if(len<=8) { i = i + 1 ; break ; }
         else         { len = len - 8 ; i = i + 1 ; } 
       }
    }
   return(i);
}


int Mod16(int len)
{ 
   int i =0;
   
   if(len>0)
    {
      while(1)
       { if(len<=16) { i = i + 1 ; break ; }
         else         { len = len - 16 ; i = i + 1 ; } 
       }
    }
   return(i);
}*/


 /* WORD high-bit & low-bit exchange ,Convert DIN & DII bit  0 to 15 */
 unsigned short hb_lb(unsigned short word_buf)
{  
  unsigned short cnvt_buf = 0;
  unsigned short buf,j;

  for (j=0;j<16;j++)
    {
    buf=(word_buf >> j) & 1;
    buf=buf << (15-j);
    cnvt_buf=buf | cnvt_buf;
    }
  return cnvt_buf;
  
}


unsigned short  read_bit(unsigned short stat,unsigned char start_bit,unsigned char stop_bit)
{
    unsigned short stat1,stat2,i;

	i=stop_bit-start_bit+1;

	if(i>16) return 0;	/*add by ycx 20070514*/
	else
	{
		i=16-i;		
		stat2=0xffff >> i;
		stat1=(stat >> start_bit)&stat2 ;
		return (stat1);
	}

	

}


unsigned short write_bit (unsigned short stat,unsigned short set_bit,unsigned short bit_val)
{
/*modified by wzg 20070505*/
    return ((bit_val&1)<<set_bit) | (stat&(~(1<<set_bit)));


/*    unsigned short stat1;
    
    stat1=(bit_val << set_bit)|stat ;
    return (stat1);
*/
}


