/************************************************************************************************************************************************
FILENAME:	ReadDb.c
FUNCTION:	
		functions of  read data  from database :ReadDb_ANA(),ReadDb_IND(),ReadDb_IMP()
		

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




/**********************read ana_db parameters***********************/

void ReadDb_ANA(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,ANA_STRUCT *ana_db)
{
	DMS_COMMON	common_aa;
	POINTER		pointer;
	USHORT		i;
	
	pointer.stn_id		= lcuno.stnid;
	pointer.dev_id		= lcuno.devid;
	pointer.data_type	= db_type;
    
	/*modified by ycx 070514,check array no. valid*/
	if( (offset+length)>=AIALL_SIZE || offset>=AIALL_SIZE )
	{
		printf("ReadDb_ANA: offset=%d  len=%d 	is too big",offset,length);
		
		return;
	}
	
	for(i=offset;i<offset+length;i++)
	{
		pointer.pt_id = i;
		
		if ( ReadEntryById (&pointer ,&common_aa) == -1)
		{
		  /*	printf ( "\n dpplib::ReadDb_ANA() : No this point in the database file !" );*/
			continue;
		}

		if  (i == common_aa.point.pt_id)
		{
			(ana_db+i)->iost		= common_aa.fixed.iost;
			(ana_db+i)->ioattr      = common_aa.fixed.ioattr;
			(ana_db+i)->status		= common_aa.var.anlg.value.status;
            (ana_db+i)->data		= common_aa.var.anlg.value.data;
			(ana_db+i)->fValue		= common_aa.var.anlg.fValue;
			
			(ana_db+i)->d_band		= common_aa.var.anlg.eng_para.D_BAND;
			(ana_db+i)->eng_cvt		= common_aa.var.anlg.eng_para.ENG_CVT;
			(ana_db+i)->hi_range	= common_aa.var.anlg.eng_para.HI_RANGE;
			(ana_db+i)->lo_range	= common_aa.var.anlg.eng_para.LO_RANGE;
			(ana_db+i)->hi_code		= common_aa.var.anlg.eng_para.HI_CODE;
			(ana_db+i)->lo_code		= common_aa.var.anlg.eng_para.LO_CODE;
			(ana_db+i)->zero		= common_aa.var.anlg.eng_para.ZERO;
			(ana_db+i)->compensate	= common_aa.var.anlg.eng_para.COMPSATE;

			(ana_db+i)->hh_lim		= common_aa.var.anlg.lmt_para.HI_ALARM;
			(ana_db+i)->h_lim		= common_aa.var.anlg.lmt_para.HI_WARN;
			(ana_db+i)->hr_lmt		= common_aa.var.anlg.lmt_para.HI_RECOVER;
			(ana_db+i)->ll_lim		= common_aa.var.anlg.lmt_para.LO_ALARM;
			(ana_db+i)->l_lim		= common_aa.var.anlg.lmt_para.LO_WARN;
			(ana_db+i)->lr_lmt		= common_aa.var.anlg.lmt_para.LO_RECOVER;
			(ana_db+i)->hi_trlmt	= common_aa.var.anlg.lmt_para.TR_WARN;
			(ana_db+i)->hr_trlmt	= common_aa.var.anlg.lmt_para.TR_RECOVER;

			if (((ana_db+i)->hi_range-(ana_db+i)->lo_range==0) && ((ana_db+i)->lo_range==0)) 
				(ana_db+i)->hi_range = 1;
							

		}
	}


	
}
	
/***********************read ind_db parameters**********************/

void ReadDb_IND(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,IND_STRUCT *ind_db)
{
	DMS_COMMON	common_aa;
	POINTER		pointer;
	USHORT		i;


	pointer.stn_id = lcuno.stnid;
	pointer.dev_id = lcuno.devid;	 	 
	pointer.data_type = db_type;

	/*modified by ycx 070514*/
	if((offset+length)>=DIN_SIZE || offset>=DIN_SIZE)
	{
		printf("ReadDb_IND: offset=%d  len=%d 	is too big",offset,length);
		
		return;
	}


	for(i= offset;i<offset+length;i++)
	{
		pointer.pt_id = i;			

		if ( ReadEntryById (&pointer ,&common_aa) == -1)
		{
		  /*	printf ( "\n dpp.lib::ReadDb_IND(): No this point in the database file !!!" );*/
			continue;
		}
		if  (i == common_aa.point.pt_id)
		{
			(ind_db+i)->iost	= common_aa.fixed.iost;
			(ind_db+i)->ioattr	= common_aa.fixed.ioattr;
			(ind_db+i)->status	= common_aa.var.ind.status;		 		
		}
	}

	
}


/**********************	read IMP parameters**************************/

void ReadDb_IMP(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,IMP_STRUCT *imp_db)
{
	DMS_COMMON	common_aa;
	POINTER		pointer;
	USHORT		i;
	

	pointer.stn_id = lcuno.stnid;
	pointer.dev_id = lcuno.devid;	 	 
	pointer.data_type = IMP_TYPE;

	/*modified by ycx 070514*/
	if((offset+length)>=PI_SIZE || offset>=PI_SIZE)
	{
		printf("ReadDb_IMP: offset=%d  len=%d 	is too big",offset,length);
		
		return;
	}

	for(i=offset;i<offset+length;i++)
	{
		pointer.pt_id = i;

		if ( ReadEntryById (&pointer ,&common_aa) == -1)
		{
		  /*	printf ( "\n dpplib::ReadDb_IMP(): No this IMP point in the database file !!!" );*/
			continue;
		}

		if  (i == common_aa.point.pt_id)
		{
			(imp_db+i)->iost			= common_aa.fixed.iost;
			(imp_db+i)->ioattr			= common_aa.fixed.ioattr;
		}
	}

	
}



/**********************  read obj_db parameters  ***********************/

void ReadDb_OBJ(LCUID lcuno,UCHAR db_type,USHORT offset,USHORT length,DEVOBJ_STRUCT *obj_db)
{
	DMS_COMMON	common_aa;
	POINTER		pointer;
	USHORT		i;
	
	pointer.stn_id		= lcuno.stnid;
	pointer.dev_id		= lcuno.devid;
	pointer.data_type	= db_type;

	/*modified by ycx 070514*/
	if( (offset+length)>=OBJ_SIZE || offset>=OBJ_SIZE )
	{
		printf("ReadDb_OBJ: offset=%d  len=%d 	is too big",offset,length);
		
		return;
	}

	for(i=offset;i<offset+length;i++)
	{
		pointer.pt_id = i;
		
		if ( ReadEntryById (&pointer ,&common_aa) == -1)
		{
		  	/*printf ( "\n dpplib::ReadDb_OBJ() : No this point in the database file !" );*/
			continue;
		}

		if  (i == common_aa.point.pt_id)
		{
			(obj_db+i)->iost			= common_aa.fixed.iost;
			(obj_db+i)->ioattr         	= common_aa.fixed.ioattr;
			
			(obj_db+i)->fb_status1		= common_aa.var.devobj.fb_status1;
            (obj_db+i)->fb_status2		= common_aa.var.devobj.fb_status2;
			(obj_db+i)->param			= common_aa.var.devobj.param;
			

		}
	}


	
}
