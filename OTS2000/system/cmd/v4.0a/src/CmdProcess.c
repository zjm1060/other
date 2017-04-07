#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"


#include "../../../cmd/v4.0a/inc/cmd_def.h"
#include "../../../cmd/v4.0a/inc/cmd_functions.h"


int CmdProcess(struct net_cmd_seg *net_cmd_seg_ptr,struct plc_cmd_seg *plc_cmd_seg_ptr)   /*if cmd due to plc,return 1;if cmd due to CmdProcess(),return 0;if any wrong,return -1*/
{ 
	unsigned char cmd_type,stnid,devid,local_stnid,db_type;
	
	unsigned short status,devno;
	
	int  data;
	unsigned short pnt;
	
	LCUID lcuid;
	
	float hi_range,lo_range,hi_code,lo_code,xcode;	
	char	err_str[256]; 
	
	POINTER		pointer;
	DMS_COMMON	common_aa;

	SetModuleName("CMD:");

	memset(plc_cmd_seg_ptr->cmd_buf.data,0,PLCCMD_BUF_SIZE*2);
	
	stnid = net_cmd_seg_ptr->net_cmd_buf.netdata.stn_id; 
	
	GetLocalHostStnId(&local_stnid);
	
 	local_stnid=stnid; /* added 2006.4.24*/
 	
	if(stnid ==local_stnid)
	{ 
		cmd_type = net_cmd_seg_ptr->net_cmd_buf.netdata.type_id;
		
		plc_cmd_seg_ptr->cmd_buf.cmd.type_id = cmd_type;

		devid =  net_cmd_seg_ptr->net_cmd_buf.netdata.dev_id;
		
		plc_cmd_seg_ptr->cmd_buf.cmd.stn_id = stnid;
		
		plc_cmd_seg_ptr->cmd_buf.cmd.lcu_id = devid; 
		
		GetLocalHostId(&(plc_cmd_seg_ptr->cmd_buf.cmd.src_id) );

		plc_cmd_seg_ptr->cmd_buf.cmd.cmd_src_id = (unsigned char)net_cmd_seg_ptr->net_cmd_buf.netdata.status.src;
		
		plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[3] = net_cmd_seg_ptr->net_cmd_buf.netdata.second;/*second*/
		
		plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[4] = (net_cmd_seg_ptr->net_cmd_buf.netdata.hour<<8)|net_cmd_seg_ptr->net_cmd_buf.netdata.minute;     /*min,hour*/
		
		plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[5] = (net_cmd_seg_ptr->net_cmd_buf.netdata.month<<8)|net_cmd_seg_ptr->net_cmd_buf.netdata.day;      /*day,month*/
		
		plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[6] = net_cmd_seg_ptr->net_cmd_buf.netdata.year;/*year*/
		
		switch (cmd_type)
		{
				      							
			case TIME_SYNC:
			{	
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;      
				
				return 1;
			}
			case C_LCU_ONOFF:
			{	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status!=LCU_OFF_LINE && status!=LCU_ON_LINE)   /* 2 operation status is maximum of using now */
					
					{	
						sprintf(err_str,"cmd.lib find C_LCU_ONOFF status=%d beyond PLC's range ",status);
						Err_log(err_str);
						
						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						
						return -1;
					}		
				
			 	plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] =status;
			 	
				lcuid.stnid = stnid;
				lcuid.devid = devid;
				
				BdCmdSendResult(*net_cmd_seg_ptr,0);
				
				if(status==LCU_ON_LINE)		BdLCUONOFFStat(lcuid,LCU_ON);
				else				BdLCUONOFFStat(lcuid,LCU_OFF);
				
				return 0;
			}
	
			case C_UNIT:
			{	
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
			        memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);

				if (devno<LO_UNIT_ADD || devno>HI_UNIT_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_UNIT ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					
					return -1;
				}
				
				devno = devno-LO_UNIT_ADD+1;

				if (devno>USER_UNIT)	  /* 1 unit maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_UNIT devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					
					return -1;
				}      	
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr ;
	
				if (status>=ST_UNIT)   /* 32 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find  C_UNIT status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;

				memcpy(&data, &net_cmd_seg_ptr->net_cmd_buf.netdata.data,4);

				if((status==START_UNIT || status==START_TO_ONLIN) && data !=0)		
				{
					pointer.stn_id=stnid;
					pointer.dev_id=devid;
					pointer.data_type=PARAM_TYPE;
	
					if(GetEntryIdByName( "YG_GD", &pointer)==-1)
					{
						sprintf (err_str, "Cmd.lib: No YG_GD point of %d lcu %d station in the database!", devid, stnid);
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}

					if (ReadEntryById(&pointer, &common_aa)== -1)
					{
						sprintf (err_str, " cmd.lib C_UNIT DBread: No this point=%d in the database file " ,pointer.pt_id);
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
				
					lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
					
					hi_range = common_aa.var.anlg.eng_para.HI_RANGE;
					
					lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
					
					hi_code  = common_aa.var.anlg.eng_para.HI_CODE;

					if ((hi_range - lo_range) <= 0.005) 
					{	
						sprintf(err_str,"cmd.lib find C_UNIT YG_GD:(hi_range-lo_range) is too small");
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
					
					if( (float)data/1000 > hi_range)	 {data = hi_range * 1000;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}
				
					if( (float)data/1000 < lo_range)     {data = lo_range * 1000;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}

					xcode = (((float)data/1000 - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
									/(hi_range - lo_range)*(hi_code -lo_code )+lo_code;
				
					plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)xcode;
	
				}
				else if(status==JUMP_UNIT && data !=0)
				{
				
					plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = data/1000;/*printf("********no=%d\n",plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2]);*/
				
				}
				else   		       				
					plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = 0;
				
				return 1;
			}
			
			case C_BREAKER:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);
				/*
				if (devno<LO_BREAK_ADD || devno>HI_BREAK_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_BREAKER ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				*/
				devno = devno-LO_BREAK_ADD+1;
				devno = 1;
				/*
				if (devno>USER_BREAKER)	   	
				{
					sprintf(err_str,"cmd.lib find C_BREAKER devno=%d beyond PLC's range",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}     
				*/
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>=ST_BREAKER)   /* 16 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_BREAKER status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}	
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status; 		       							
				
				return 1;
			}
			
			case C_SWITCH:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				//yyp debug 2012.06.21
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);
				/*
				if (devno<LO_SWITCH_ADD || devno>HI_SWITCH_ADD) 	
				{
					sprintf(err_str,"cmd.lib find  C_SWITCH ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				*/
				
				devno = devno-LO_SWITCH_ADD+1;
				devno = 1;
				if (devno>USER_SWITCH)	  	/* 128 switches maximum of using now */
				{
					sprintf(err_str,"cmd.lib find  C_SWITCH devno=%d beyond PLC's range",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr ;
	
				if (status>ST_SWITCH)   /* 4 operation status is maximum of using now */
				{	
					sprintf(err_str,"cmd.lib find  C_SWITCH status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}	        
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;			
				
				return 1;
			}	
			case C_APOWER:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);
				
				if (devno<LO_UNIT_ADD || devno>HI_UNIT_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_APOWER ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				devno = devno-LO_UNIT_ADD+1;

				if (devno>USER_UNIT)	  /* 1 unit maximum of using now */
				{
					sprintf(err_str,"cmd.lib find  C_APOWER devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      	
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>=ST_APOWER)   /* 3 operation status is maximum of using now */
				{	
					sprintf(err_str,"cmd.lib find  C_APOWER status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				/* read active power in database*/
				
				pointer.stn_id	= stnid;
				
				pointer.dev_id	= devid;
				
				pointer.data_type = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;	
				
				if((pointer.data_type == PARAM_TYPE)||(pointer.data_type == ANA_TYPE)) /****** modified by ycx 070529 ******/
				{		
					memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
					
					pointer.pt_id = pnt;

					if (ReadEntryById(&pointer, &common_aa)== -1)
					{
						sprintf (err_str, " cmd.lib C_APOWER DBread: No this point=%d in the database file !!!" ,pnt);
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
					
					lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
					
					hi_range = common_aa.var.anlg.eng_para.HI_RANGE;
					
					lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
					
					hi_code  = common_aa.var.anlg.eng_para.HI_CODE;

					if ((hi_range - lo_range) <= 0.005) 
					{	
						sprintf(err_str,"cmd.lib find C_APOWER:(hi_range-lo_range) is too small ");
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
					
					memcpy(&data, &net_cmd_seg_ptr->net_cmd_buf.netdata.data,4);
					
					if( (float)data/1000 > hi_range)	{ data = hi_range * 1000;memcpy( &net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data,4);}
				
					if( (float)data/1000 < lo_range)    { data = lo_range * 1000;memcpy( &net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data,4);}

					xcode = (((float)data/1000 - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
						/(hi_range - lo_range)*(hi_code -lo_code )+lo_code;
				
					plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)xcode;	
					
					return 1;
				}
				else
				{
					sprintf(err_str,"cmd.lib find C_APOWER cmd: data_type=%d ,error! ",pointer.data_type);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}			
			}
			
			case C_RPOWER:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);
				
				if (devno<LO_UNIT_ADD || devno>HI_UNIT_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_RPOWER ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				devno = devno-LO_UNIT_ADD+1;

				if (devno>USER_UNIT)	  /* 1 unit maximum of using now */
				{	
					sprintf(err_str,"cmd.lib find  C_RPOWER devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      	
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>=ST_RPOWER)   /* 3 operation status is maximum of using now */
				{	
					sprintf(err_str,"cmd.lib find  C_RPOWER status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				/* read reactive power in database  */
				
				pointer.stn_id	= stnid;
				
				pointer.dev_id	= devid;
				
				pointer.data_type = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;
				
				if((pointer.data_type == PARAM_TYPE)||(pointer.data_type == ANA_TYPE))	/****** modified by ycx 070529 ******/
				{
				
					memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
					
					pointer.pt_id = pnt;

					if (ReadEntryById(&pointer, &common_aa)== -1)
					{
						sprintf (err_str, "cmd.lib C_RPOWER DBread: No this point=%d in the database file ",pnt );
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
					
					lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
					
					hi_range = common_aa.var.anlg.eng_para.HI_RANGE;

					lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
					
					hi_code  = common_aa.var.anlg.eng_para.HI_CODE;

					if ((hi_range - lo_range) <= 0.005) 
					{	
						sprintf(err_str,"cmd.lib find C_RPOWER:(hi_range-lo_range) is too small ");
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
					
					memcpy(&data, &net_cmd_seg_ptr->net_cmd_buf.netdata.data,4);
					
					if( (float)data/1000 > hi_range)	{ data = hi_range*1000; memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data,4);}
				
					if( (float)data/1000 < lo_range)    { data = lo_range*1000; memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data,4);}

					xcode = (((float)data/1000 - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
						/(hi_range - lo_range)*( hi_code-lo_code )+lo_code;
								
					plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)xcode;
		
					return 1;
				}
				else
				{
					sprintf(err_str,"cmd.lib find C_RPOWER cmd: data_type=%d ,error! ",pointer.data_type);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}			

			}
			case C_DEVICE:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);
				/*
				if (devno<LO_DEVICE_ADD || devno>HI_DEVICE_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_DEVICE ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				*/
				devno = devno-LO_DEVICE_ADD+1;
				/*
				if (devno>USER_DEVICE)	 
				{
					sprintf(err_str,"cmd.lib find C_DEVICE devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      	
				*/
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>ST_DEVICE)    /* 2 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_DEVICE status=%d beyond PLC's range",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}	       
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;			
				
				return 1;
			}			
			case C_GATE:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);
				/*	
				if (devno<LO_GATE_ADD || devno>HI_GATE_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_GATE ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				*/
				devno = devno-LO_GATE_ADD+1;
				devno = 1;
				/*
				if (devno>USER_GATE)
				{
					sprintf(err_str,"cmd.lib find  C_GATE devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      	
				*/
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>=ST_GATE)   /* 16 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_GATE status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				/* read  gate in database  */
				
				pointer.stn_id	= stnid;
				
				pointer.dev_id	= devid;				
				
				db_type  = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;
				
				if(( status==GATE_UP || status==GATE_DOWN || status==GATE_SETFLUX) && db_type != ANA_TYPE && db_type != PARAM_TYPE)
				{
					sprintf(err_str,"cmd.lib find this opr is not for IND type ");
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
				
				pointer.pt_id = pnt;
				
				if (db_type == ANA_TYPE || db_type == PARAM_TYPE)
				{
				
					pointer.data_type = db_type;				

					if (ReadEntryById(&pointer, &common_aa)== -1)
					{
						sprintf (err_str, "cmd.lib C_GATE DBread: No this point=%d in the database file  " ,pnt);
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
				
					lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
				
					hi_range = common_aa.var.anlg.eng_para.HI_RANGE;

					lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
				
					hi_code  = common_aa.var.anlg.eng_para.HI_CODE;

					if ((hi_range - lo_range) <= 0.005) 
					{	
						sprintf(err_str,"cmd.lib find C_GATE:(hi_range-lo_range) is too small ");
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
				
					memcpy(&data, &net_cmd_seg_ptr->net_cmd_buf.netdata.data,4);
					
					if( (float)data/1000 > hi_range)	 {data = hi_range*1000 ;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}
			
					if( (float)data/1000 < lo_range)     {data = lo_range*1000 ;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}

					xcode = (((float)data/1000 - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
						/(hi_range - lo_range)*( hi_code-lo_code )+lo_code;
							
					plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)xcode;

				}					

				return 1;
						
			}
			case C_TRANS:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);
				
				if (devno<LO_TRANS_ADD || devno>HI_TRANS_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_TRANS ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				devno = devno-LO_TRANS_ADD+1;

				if (devno>USER_TRANS)	  /* 32 gates maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_TRANS devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      	
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>=ST_TRANS)   /* 16 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find  C_TRANS status=%d beyond PLC's range\n",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				/* read  gate in database  */
				
				pointer.stn_id	= stnid;
				
				pointer.dev_id	= devid;				
				
				db_type  = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;

				
				if(( status== TRANS_RAISE_TO|| status==TRANS_DOWN_TO ) && db_type != ANA_TYPE && db_type != PARAM_TYPE)
				{
					sprintf(err_str,"cmd.lib find this opr is not for IND type");
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
				
				pointer.pt_id = pnt;
				
				if (db_type == ANA_TYPE || db_type == PARAM_TYPE)
				{
				
					pointer.data_type = db_type;				

					if (ReadEntryById(&pointer, &common_aa)== -1)
					{
						sprintf(err_str,"cmd.lib C_TRANS DBread: No this point=%d in the database file ",pnt );
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
				
					lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
				
					hi_range = common_aa.var.anlg.eng_para.HI_RANGE;

					lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
				
					hi_code  = common_aa.var.anlg.eng_para.HI_CODE;

					if ((hi_range - lo_range) <= 0.005) 
					{	
						sprintf(err_str,"cmd.lib find C_GATE:(hi_range-lo_range) is too small ");
						Err_log(err_str);

						BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
						return -1;
					}
				
					memcpy(&data, &net_cmd_seg_ptr->net_cmd_buf.netdata.data,4);
					
					if( (float)data/1000 > hi_range)      {data = hi_range*1000;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}
			
					if( (float)data/1000 < lo_range)      {data = lo_range*1000;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}

					xcode = (((float)data/1000 - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
						/(hi_range - lo_range)*( hi_code-lo_code )+lo_code;
							
					plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)xcode;
	
				}									
				
				return 1;

			}
			case C_DEV_MODE:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);

				if (devno<LO_DEV_MODE_ADD || devno>HI_DEV_MODE_ADD) 	
				{
					sprintf(err_str,"cmd.lib find C_DEV_MODE ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				devno = devno-LO_DEV_MODE_ADD+1;

				if (devno>USER_DEVICE_MODE)	  /* 256 devices maximum of using now */
				{	
					sprintf(err_str,"cmd.lib find C_DEV_MODE devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      	
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>ST_DEV_MODE)   /* 4 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find  C_DEV_MODE status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}	        
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;			
			
   
				return 1;
			}
			case C_VOLT_STEP:
			case C_SPEED_STEP:	
			{
			        plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
			        
			        memcpy(&devno, &net_cmd_seg_ptr->net_cmd_buf.netdata.ctrl_addr, 2);

				if (devno<LO_UNIT_ADD || devno>HI_UNIT_ADD) 	
				{
					sprintf(err_str,"cmd.lib find (C_VOLT_STEP|C_SPEED_STEP) ctrl_addr=%d wrong ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				devno = devno-LO_UNIT_ADD+1;

				if (devno>USER_UNIT)	/* 1 unit maximum of using now */  
				{
					sprintf(err_str,"cmd.lib find (C_VOLT_STEP|C_SPEED_STEP) devno=%d beyond PLC's range ",devno);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,OBJECT_NOT_EXIST);
					return -1;
				}      	
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devno;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>ST_STEP)/* 2 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find (C_VOLT_STEP|C_SPEED_STEP) status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
			
				return 1;
			}

			case C_ONOFF_FORCE:
			{			
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = pnt;
				
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
				
				if (status>=ST_ONOFF_FORCE)   /* 3 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_ONOFF_FORCE status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				db_type  = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;
				
				status = status | (db_type<<8) ;

				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)(net_cmd_seg_ptr->net_cmd_buf.netdata.data /1000);
				
				return 1;
			}
			case C_ANA_FORCE:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
						
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = pnt;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>=ST_ANA_FORCE)   /* 3 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_ANA_FORCE status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				db_type  = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;
				
				status = status | (db_type<<8) ;
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				/* read AI,AO in database  */
				
				pointer.stn_id	= stnid;
				
				pointer.dev_id	= devid;
				
				pointer.data_type = db_type;				
					
				pointer.pt_id = pnt;

				if (ReadEntryById(&pointer, &common_aa)== -1)
				{
					sprintf (err_str, "cmd.lib  C_ANA_FORCE DBread: No this point in the database file " );
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
				
				hi_range = common_aa.var.anlg.eng_para.HI_RANGE;
				lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
				
				hi_code  = common_aa.var.anlg.eng_para.HI_CODE;

				if ((hi_range - lo_range) <= 0.005) 
				{	
					sprintf(err_str,"cmd.lib find C_ANA_FORCE:(hi_range-lo_range) is too small ");
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				memcpy(&data, &net_cmd_seg_ptr->net_cmd_buf.netdata.data,4);
					
				if( (float)data/1000 > hi_range)	 {data = hi_range*1000 ;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}
			
				if( (float)data/1000 < lo_range)     {data = lo_range*1000 ;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}

				xcode = (((float)data/1000 - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
						/(hi_range - lo_range)*( hi_code-lo_code )+lo_code;
							
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)xcode;
	
				return 1;
			}
			case C_IO_CTRL_BLOCK:
			{
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;
				
				memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = pnt;
				
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = status;
			
				return 1;
			}
			
		
			case C_DATA_POLLING:
			{
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
		
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				db_type  = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;
				
				switch(db_type)
				{
				
				case ANA_TYPE:			
					shm_plc_data->seg[segid[stnid][devid]].ai_all_send_flag =1;				
					break;
					
				case OUTA_TYPE:			
					shm_plc_data->seg[segid[stnid][devid]].ao_all_send_flag =1;				
					break;
					
				case PARAM_TYPE:			
					shm_plc_data->seg[segid[stnid][devid]].param_all_send_flag =1;
					break;
					
				case IMP_TYPE:
					shm_plc_data->seg[segid[stnid][devid]].imp_all_send_flag =1;	
					break;
							
				case POL_TYPE:			
					shm_plc_data->seg[segid[stnid][devid]].pol_all_send_flag =1;				
					break;
					
				case OUTS_TYPE:			
					shm_plc_data->seg[segid[stnid][devid]].dout_all_send_flag =1;				
					break;
					
				case SOE_TYPE:	
					shm_plc_data->seg[segid[stnid][devid]].dii_all_send_flag =1;				
					break;
					
				case SYSMSG_TYPE:			
					shm_plc_data->seg[segid[stnid][devid]].msg_all_send_flag =1;				
					break;

				case DEVOBJ_TYPE:
					shm_plc_data->seg[segid[stnid][devid]].obj_all_send_flag =1;
					break;
					
				default:
					{
						sprintf(err_str,"cmd.lib find C_DATA_POLLING db_type wrong ");
						return -1;
					}
				
				}
			
				return 0;
			}
			
			case C_LCU_PARAM:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				memcpy(&pnt, &net_cmd_seg_ptr->net_cmd_buf.netdata.point_id, 2);
						
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = pnt;
	
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>ST_ANA_FORCE)   /* 2 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find  C_LCU_PARAM status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				/* read param in database  */
				
				pointer.stn_id	= stnid;
				
				pointer.dev_id	= devid;
				
				pointer.data_type = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;				
					
				pointer.pt_id = pnt;

				if (ReadEntryById(&pointer, &common_aa)== -1)
				{
					sprintf (err_str, " cmd.lib  C_LCU_PARAM DBread: No this point in the database file " );
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
				
				hi_range = common_aa.var.anlg.eng_para.HI_RANGE;
				lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
				
				hi_code  = common_aa.var.anlg.eng_para.HI_CODE;

				if ((hi_range - lo_range) <= 0.005) 
				{	
					sprintf(err_str,"cmd.lib find C_LCU_PARAM:(hi_range-lo_range) is too small ");
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}
				
				memcpy(&data, &net_cmd_seg_ptr->net_cmd_buf.netdata.data,4);
					
				if( (float)data/1000 > hi_range)	 {data = hi_range*1000 ;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}
			
				if( (float)data/1000 < lo_range)     {data = lo_range*1000 ;memcpy(&net_cmd_seg_ptr->net_cmd_buf.netdata.data,&data, 4);}

				xcode = (((float)data/1000 - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
						/(hi_range - lo_range)*( hi_code-lo_code )+lo_code;
							
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[2] = (unsigned short)xcode;
				
				return 1;
			}
			
			case C_PLC_SWITCH:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[0] = devid;
				
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
				
				if (status>=ST_PLC_SWITCH)   /* 3 operation status is maximum of using now */
				{
					sprintf(err_str,"cmd.lib find C_PLC_SWITCH status=%d beyond PLC's range",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				return 1;
			}
			case C_SIGNAL_RESET:
			{
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_flag = 1;
				
				status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	
				if (status>=ST_SIGNAL)/* 2 operation status is maximum of using now */   
				{
					sprintf(err_str,"cmd.lib find  C_SIGNAL_RESET status=%d beyond PLC's range ",status);
					Err_log(err_str);

					BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
					return -1;
				}		
				
				plc_cmd_seg_ptr->cmd_buf.cmd.cmd_w[1] = status;
				
				return 1;
			}
				
			default:
			{
				sprintf(err_str,"cmd_type=%d not processsed by cmdlib ",cmd_type);
				Err_log(err_str);

				BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
				return -1;
			}
		
	}
	}

	else 
	{
		sprintf(err_str,"cmd.lib find net_cmd's stnid %d  != HostStnId %d ",stnid,local_stnid);
		Err_log(err_str);

		BdCmdSendResult(*net_cmd_seg_ptr,CMD_FORMAT_FAULT);
		return -1;
	}


}
