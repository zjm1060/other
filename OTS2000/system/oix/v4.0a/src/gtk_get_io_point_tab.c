
/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 		2005.9.23 
	Last Upate 	2005.9.28  
	SYNC by hcl 2007.12.7
     

---------------------------------------------------------------------*/



/*
	2006.3 updated by zyp for v4.0
*/

#include	<stdio.h>
#include	<gtk/gtk.h>

#define	DEBUG

#include	"../inc/oix.h"

 
POINTER		pnt_tab_ptr[MAX_CRT_NUMBER];
int	db_number[MAX_CRT_NUMBER];

extern void	Redraw (GtkWidget*);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern int OpenDspFile (int crt_n, char	*filename);
extern void	ProgDoAckCelBrdcst (int crt_n );


void	FillIostCmdBuf ( int crt_n, DMS_COMMON	*dms_cmm, CMD_STATUS cmd_status,short io_type );

#define		ERR_DB_NUMBER	-1

void GetPtrTable (int crt_n ,char dbname[POINT_NAME_SIZE])
{
	DMS_COMMON	dms_rcd;	
	
	if ( ReadEntryByNameStr ( /*ptr_tab_rcd[crt_n]->lnk.dbname*/dbname, &dms_rcd ) == -1 )
	{		
	PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);
		gdk_beep();  
		return;
	}
	
	switch( dms_rcd.point.data_type )
	{
	case	POL_TYPE:
	case	SOE_TYPE:
	case	SYSMSG_TYPE:
	case	ANA_TYPE:
	case 	OUTA_TYPE:
	case	OUTS_TYPE:
	case	PARAM_TYPE:
	case	IMP_TYPE:
	case	CALC_TYPE:
		pnt_tab_ptr[crt_n]      = dms_rcd.point;
		break;
	default:		
		gdk_beep();  
		return;
		break;
	}
	
	GetEntryNumById ( pnt_tab_ptr[crt_n].stn_id,pnt_tab_ptr[crt_n].dev_id,pnt_tab_ptr[crt_n].data_type,&db_number[crt_n]);

	if ( db_number[crt_n] == ERR_DB_NUMBER )
	{
		printf(" OIX: GetNumberByID: get ERR_DB_NUMBER\n");
		return;
	}

	switch	( pnt_tab_ptr[crt_n].data_type )
	{
	case	POL_TYPE:
		OpenDspFile ( crt_n, "io_pol_point_data.dbin" );
		break;

	case	SOE_TYPE:
		OpenDspFile ( crt_n, "io_int_point_data.dbin" );
		break;

	case	OUTS_TYPE:
		OpenDspFile ( crt_n, "io_outs_point_data.dbin" );
		break;

	case	ANA_TYPE:
		OpenDspFile ( crt_n, "io_ana_point_data.dbin" );
		break;

	case	OUTA_TYPE:
		OpenDspFile ( crt_n, "io_out_ana_point_data.dbin" );
		break;
	case	PARAM_TYPE:
		OpenDspFile ( crt_n, "io_param_point_data.dbin" );
		break;

	case	IMP_TYPE:
		OpenDspFile ( crt_n, "io_puls_point_data.dbin" );
		break;

	case	SYSMSG_TYPE:
		OpenDspFile ( crt_n, "io_sysmsg_point_data.dbin" );
		break;

	default:
		break;
	}	 	
}

void GetPointParamTab(int crt_n,int data_type,int data_type_sav,char *param_tab_dsp_name)
{	
	if ( pnt_tab_ptr[crt_n].data_type != data_type )
	{
		pnt_tab_ptr[crt_n].data_type	= data_type;
			
		GetEntryNumById ( pnt_tab_ptr[crt_n].stn_id,pnt_tab_ptr[crt_n].dev_id,pnt_tab_ptr[crt_n].data_type,&db_number[crt_n]);
		if ( db_number[crt_n] == ERR_DB_NUMBER)
		{
			pnt_tab_ptr[crt_n].data_type = data_type_sav;
			gdk_beep();  
			return;
		}
		else
		{
			if ( pnt_tab_ptr[crt_n].pt_id > db_number[crt_n] )
				pnt_tab_ptr[crt_n].pt_id	= db_number[crt_n]-1;
			OpenDspFile ( crt_n, param_tab_dsp_name);
			Redraw (canvas[crt_n] );
		}
	}
}



void	ChangePointTable (int crt_n,int cmd_type )
{
	short	data_type_sav;
	int     i,stn_num;
	int dev_num;
	char 	dev_name[GROUP_NAME_SIZE];
	
	data_type_sav = pnt_tab_ptr[crt_n].data_type;
	switch	( cmd_type )
	{
	case	PrgN_PREV_POINT:
		if (pnt_tab_ptr[crt_n].pt_id > 0)
		{
			pnt_tab_ptr[crt_n].pt_id--;
			Redraw ( canvas[crt_n]);
		}
		else
		{
			gdk_beep();  
		}
		break;

	case	PrgN_NEXT_POINT:
		if (pnt_tab_ptr[crt_n].pt_id < db_number[crt_n]-1 )
		{
			pnt_tab_ptr[crt_n].pt_id++;
			Redraw (canvas[crt_n] );
		}
		else
		{
			gdk_beep();  
		}
		break;

	case    PrgN_PREV_LCU:
		if (pnt_tab_ptr[crt_n].dev_id > 1 )
		{
			for( i=pnt_tab_ptr[crt_n].dev_id-1; i>0; i--)
			{
				GetGroupNameById((char)(pnt_tab_ptr[crt_n].stn_id),(char)i,(char *)dev_name);
				if(strcmp(dev_name, "")!=0)
					break;
			}
			if( i==0)
				gdk_beep();  
			else
			{
				pnt_tab_ptr[crt_n].dev_id= i;			
				Redraw ( canvas[crt_n]);
			}
		}
		else	
			gdk_beep(); 		
		break;

	case    PrgN_NEXT_LCU:
		if(-1==GetGroupNumById(pnt_tab_ptr[crt_n].stn_id, &dev_num))
		return ;

		if (pnt_tab_ptr[crt_n].dev_id < dev_num )	
		{
			for( i=pnt_tab_ptr[crt_n].dev_id+1; i<= dev_num; i++)
			{
				printf("BBBBB %d\n", i);
				GetGroupNameById(pnt_tab_ptr[crt_n].stn_id,(UCHAR)i,dev_name);
				if( strcmp(dev_name, "")!=0 )
					break;
			}
			if( i> StnDef[pnt_tab_ptr[crt_n].stn_id].dev_num  )
				gdk_beep();  
			else
			{
				pnt_tab_ptr[crt_n].dev_id= i;			
				Redraw (canvas[crt_n] );
			}			
		}
		else		
			gdk_beep();  		
		break;

	case    PrgN_PREV_STN:
		if (pnt_tab_ptr[crt_n].stn_id > 1 )
		{
			for( i=pnt_tab_ptr[crt_n].stn_id-1; i>0; i--)
			{
				GetStnNameById(pnt_tab_ptr[crt_n].stn_id,dev_name);
				if(strcmp(dev_name, "")!=0)
					break;
			}
			if( i==0)
				gdk_beep();  
			else
			{
				pnt_tab_ptr[crt_n].stn_id= i;			
				Redraw ( canvas[crt_n]);
			}
		}
		else	
			gdk_beep(); 		
		break;

	case    PrgN_NEXT_STN:
		GetStationNum(&stn_num);
		if (pnt_tab_ptr[crt_n].stn_id < stn_num )	
		{
			for( i=pnt_tab_ptr[crt_n].stn_id+1; i<= stn_num; i++)
			{	
				GetStnNameById(pnt_tab_ptr[crt_n].stn_id,dev_name);				
				if( strcmp(dev_name, "")!=0 )
					break;
			}
			if( i> stn_num )
				gdk_beep();  
			else
			{
				pnt_tab_ptr[crt_n].stn_id= i;			
				Redraw (canvas[crt_n] );
			}			
		}
		else		
			gdk_beep();  		
		break;

	case	PrgN_ANALOG_TAB:		
		GetPointParamTab(crt_n,ANA_TYPE,data_type_sav,"io_ana_point_data.dbin");
		break;
                
        case    PrgN_OBJ_TAB:   /* added by chi */
                GetPointParamTab(crt_n,DEVOBJ_TYPE,data_type_sav,"io_obj_point_data.dbin");
                break;

	case	PrgN_OUTA_TAB:
		GetPointParamTab(crt_n,OUTA_TYPE,data_type_sav,"io_out_ana_point_data.dbin");
		break;

	case	PrgN_PARAM_TAB:
		GetPointParamTab(crt_n,PARAM_TYPE,data_type_sav,"io_param_point_data.dbin");
		break;

	case	PrgN_IMPULSE_TAB:		
		GetPointParamTab(crt_n,IMP_TYPE,data_type_sav,"io_puls_point_data.dbin");
		break;

	case	PrgN_IND_POL_TAB:		
		GetPointParamTab(crt_n,POL_TYPE,data_type_sav,"io_pol_point_data.dbin");
		break;

	case	PrgN_OUTS_TAB:		
		GetPointParamTab(crt_n,OUTS_TYPE,data_type_sav,"io_outs_point_data.dbin");
		break;

	case	PrgN_IND_INT_TAB:
		GetPointParamTab(crt_n,SOE_TYPE,data_type_sav,"io_int_point_data.dbin");
		break;

	case	PrgN_SYSMSG_TAB:		
		GetPointParamTab(crt_n,SYSMSG_TYPE,data_type_sav,"io_sysmsg_point_data.dbin");
		break;

	default:
		break;
	}
}


void	FillIostCmdBuf ( int crt_n, DMS_COMMON	*dms_cmm, CMD_STATUS cmd_status,short io_type )
{	
	cmd_buf[crt_n].stn_id		= dms_cmm->point.stn_id;
	cmd_buf[crt_n].dev_id		= dms_cmm->point.dev_id;
	cmd_buf[crt_n].point_id		= dms_cmm->point.pt_id;
	cmd_buf[crt_n].data_type	= dms_cmm->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= 0;	

	cmd_buf[crt_n].usr_id    =(unsigned char) cur_usr_id;
	
	switch ( cmd_buf[crt_n].data_type )
	{
		case	ANA_TYPE:
		case	POL_TYPE:
		case	SYSMSG_TYPE:
		case	SOE_TYPE:
			if( io_type == IOST_TYPE )
				cmd_buf[crt_n].type_id 	= DPS_IOST_SET;
			else
				cmd_buf[crt_n].type_id	= DPS_IOATTR_SET;
			break;

		default:
			break;
	}
}


void ProgDbIoSet (int crt_n, GBRCD	*rcd )
{
	DMS_COMMON      dms_cmm;
	short	ioatr_class;
	
	if ( ReadEntryByNameStr (rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
		printf ( "OIX:  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}

	FillIostCmdBuf ( crt_n, &dms_cmm, cmd_buf[crt_n].status, IOATR_TYPE );
	
	ioatr_class = rcd->lnk.dyn.data_class;
	cmd_buf[crt_n].status.opr = ioatr_class;

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "SetCls" ) == 0 ) 	
		cmd_buf[crt_n].status.state = ENABLE;
	
	else if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "SetOpen" ) == 0 )
	cmd_buf[crt_n].status.state = DISABLE;
		

	ProgDoAckCelBrdcst ( crt_n );  
}
	

