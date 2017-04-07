  /* igc.c program
    cc -o IGC.dxe igc.c /DBLIB/dblib.dlb auto gen
    20110718 chenxs */

#include "../../../pas/v4.0a/inc/IGC.h" 

int init()
{
	int i=0;
	lacal_datetime_now=get_lacal_datetime_now();
	run_count=0;
	run_count_for_avg=0;
	all_run_millisecond=0;
	last_run_millisecond=0;
	avg_run_millisecond=0;
	max_run_millisecond=0;
	min_run_millisecond=0xFFFFFFFF;
	igc_CLK_TCK=get_CLK_TCK();
	for(i=0;i<Ifloat;i++)
		I_floats[i]=0;
	for(i=0;i<Idouble;i++)
		I_doubles[i]=0;
	for(i=0;i<Iuint;i++)
		I_uints[i]=0;
	for(i=0;i<Ilong;i++)
		I_longs[i]=0;
	for(i=0;i<Iulong;i++)
		I_ulongs[i]=0;
	for(i=0;i<Ibool;i++)
		I_bools[i]=0;
	for(i=0;i<Ishort;i++)
		I_shorts[i]=0;
	for(i=0;i<Iushort;i++)
		I_ushorts[i]=0;
	for(i=0;i<Ichar;i++)
		I_chars[i]=0;
	for(i=0;i<Iuchar;i++)
		I_uchars[i]=0;
	for(i=0;i<Idatetime;i++)
		I_datetimes[i]=get_lacal_datetime_now();
	for(i=0;i<IH9000_IGC_data;i++)
	{
		I_H9000_IGC_datas[i].value=0;
		I_H9000_IGC_datas[i].ok_flag=0;
		I_H9000_IGC_datas[i].chan_fail=1;
		I_H9000_IGC_datas[i].change_time.day=1;
		I_H9000_IGC_datas[i].change_time.hour=1;
		I_H9000_IGC_datas[i].change_time.minute=1;
		I_H9000_IGC_datas[i].change_time.month=1;
		I_H9000_IGC_datas[i].change_time.second=1;
		I_H9000_IGC_datas[i].change_time.year=1;
		I_H9000_IGC_datas[i].update_time.day=1;
		I_H9000_IGC_datas[i].update_time.hour=1;
		I_H9000_IGC_datas[i].update_time.minute=1;
		I_H9000_IGC_datas[i].update_time.month=1;
		I_H9000_IGC_datas[i].update_time.second=1;
		I_H9000_IGC_datas[i].update_time.year=1;
	}
	for(i=0;i<IH9000_IGC_IND_data;i++)
	{
		I_H9000_IGC_IND_datas[i].value=0;
		I_H9000_IGC_IND_datas[i].ok_flag=0;
		I_H9000_IGC_IND_datas[i].chan_fail=1;
		I_H9000_IGC_IND_datas[i].change_time.day=1;
		I_H9000_IGC_IND_datas[i].change_time.hour=1;
		I_H9000_IGC_IND_datas[i].change_time.minute=1;
		I_H9000_IGC_IND_datas[i].change_time.month=1;
		I_H9000_IGC_IND_datas[i].change_time.second=1;
		I_H9000_IGC_IND_datas[i].change_time.year=1;
		I_H9000_IGC_IND_datas[i].update_time.day=1;
		I_H9000_IGC_IND_datas[i].update_time.hour=1;
		I_H9000_IGC_IND_datas[i].update_time.minute=1;
		I_H9000_IGC_IND_datas[i].update_time.month=1;
		I_H9000_IGC_IND_datas[i].update_time.second=1;
		I_H9000_IGC_IND_datas[i].update_time.year=1;
	}
	for(i=0;i<IH9000_IGC_ANA_data;i++)
	{
		I_H9000_IGC_ANA_datas[i].value=0;
		I_H9000_IGC_ANA_datas[i].ok_flag=0;
		I_H9000_IGC_ANA_datas[i].chan_fail=1;
		I_H9000_IGC_ANA_datas[i].change_time.day=1;
		I_H9000_IGC_ANA_datas[i].change_time.hour=1;
		I_H9000_IGC_ANA_datas[i].change_time.minute=1;
		I_H9000_IGC_ANA_datas[i].change_time.month=1;
		I_H9000_IGC_ANA_datas[i].change_time.second=1;
		I_H9000_IGC_ANA_datas[i].change_time.year=1;
		I_H9000_IGC_ANA_datas[i].update_time.day=1;
		I_H9000_IGC_ANA_datas[i].update_time.hour=1;
		I_H9000_IGC_ANA_datas[i].update_time.minute=1;
		I_H9000_IGC_ANA_datas[i].update_time.month=1;
		I_H9000_IGC_ANA_datas[i].update_time.second=1;
		I_H9000_IGC_ANA_datas[i].update_time.year=1;
		I_H9000_IGC_ANA_datas[i].D_BAND=1;
		I_H9000_IGC_ANA_datas[i].HH_LMT=0;
		I_H9000_IGC_ANA_datas[i].H_LMT=0;
		I_H9000_IGC_ANA_datas[i].L_LMT=0;
		I_H9000_IGC_ANA_datas[i].LL_LMT=0;
		I_H9000_IGC_ANA_datas[i].HR_LMT=0;
		I_H9000_IGC_ANA_datas[i].LR_LMT=0;
		I_H9000_IGC_ANA_datas[i].ENG_CVT=0;
		I_H9000_IGC_ANA_datas[i].HI_RANGE=0;
		I_H9000_IGC_ANA_datas[i].LO_RANGE=0;
		I_H9000_IGC_ANA_datas[i].ZERO=0;
		I_H9000_IGC_ANA_datas[i].COMPENSATE=0;
		I_H9000_IGC_ANA_datas[i].TR_LIM=0;
		I_H9000_IGC_ANA_datas[i].RT_LIM=0;
	}
	for(i=0;i<Itimer;i++)
	{
		I_H9000_timers[i].autorestart=no;
		I_H9000_timers[i].full_count=600;
		I_H9000_timers[i].real_count=0;
	    I_H9000_timers[i].unit=unit_second;
		I_H9000_timers[i].enable=no;
		I_H9000_timers[i].reached_times=0;
		I_H9000_timers[i].inited=0;
		I_H9000_timers[i].lastdotime.day=lacal_datetime_now.day;
		I_H9000_timers[i].lastdotime.hour=lacal_datetime_now.hour;
		I_H9000_timers[i].lastdotime.minute=lacal_datetime_now.minute;
		I_H9000_timers[i].lastdotime.month=lacal_datetime_now.month;
		I_H9000_timers[i].lastdotime.second=lacal_datetime_now.second;
		I_H9000_timers[i].lastdotime.year=lacal_datetime_now.year;
	}
	for(i=0;i<Itimer_sysuse;i++)
	{
		h9000_igc_timeer_sysuse[i].autorestart=no;
		h9000_igc_timeer_sysuse[i].full_count=600;
		h9000_igc_timeer_sysuse[i].real_count=0;
	    h9000_igc_timeer_sysuse[i].unit=unit_second;
		h9000_igc_timeer_sysuse[i].enable=no;
		h9000_igc_timeer_sysuse[i].reached_times=0;
		h9000_igc_timeer_sysuse[i].inited=0;
		h9000_igc_timeer_sysuse[i].lastdotime.day=lacal_datetime_now.day;
		h9000_igc_timeer_sysuse[i].lastdotime.hour=lacal_datetime_now.hour;
		h9000_igc_timeer_sysuse[i].lastdotime.minute=lacal_datetime_now.minute;
		h9000_igc_timeer_sysuse[i].lastdotime.month=lacal_datetime_now.month;
		h9000_igc_timeer_sysuse[i].lastdotime.second=lacal_datetime_now.second;
		h9000_igc_timeer_sysuse[i].lastdotime.year=lacal_datetime_now.year;
		h9000_igc_char_sysuse[i]=0;
		h9000_igc_cmd_num_sysuse[i]=-1;
	}
	for(i=0;i<Ialarms;i++)
	{
	I_H9000_alarms[i].ok_flag=0;
	I_H9000_alarms[i].hostname[0]=0;
	I_H9000_alarms[i].logicname[0]=0;
	I_H9000_alarms[i].longname[0]=0;
	I_H9000_alarms[i].message[0]=0;
	I_H9000_alarms[i].repeatAlm[0]=0;
	I_H9000_alarms[i].sn_str[0]=0;
	I_H9000_alarms[i].state_str[0]=0;
	I_H9000_alarms[i].time_str[0]=0;
	I_H9000_alarms[i].usrname[0]=0;
	}
	return 1;
}


  void   userfuc0igc_test__setvalue_by___def ()
  {  
  /*用于测试分别通过逻辑名、点号进行开关量与模拟量数据设置测试*/
if(h9000_igc_timeer_sysuse[1].enable==no)
{
h9000_igc_timeer_sysuse[1].enable=yes;
h9000_igc_timeer_sysuse[1].autorestart=no;
h9000_igc_timeer_sysuse[1].unit=0;
h9000_igc_timeer_sysuse[1].full_count=4;
}
if(h9000_igc_timeer_sysuse[1].real_count>=h9000_igc_timeer_sysuse[1].full_count)
{
    
  
  	
    	char * ind_setvalue_dataname="CD.CD_SYS.POL.XJB1FFBU12W_DI51";
    	char * ana_setvalue_dataname="CD.CD_SYS.ANA.XJB_PRIORITY_A";
    	unsigned int stnid0=2,stnid1=2; 
    	unsigned int devid0=1,devid1=1; 
    	unsigned int typeid0=5,typeid1=1; 
    	unsigned int pointid0=0,pointid1=0; 
    	unsigned int setvalue_point_len=2;
    	
    	H9000_IGC_data myH9000_IGC_data;
    	
    	printf("\n&&&&&&&&igc_test__setvalue_by__ start\n");
    
    	myH9000_IGC_data=get_data_value_by_logicalname(ind_setvalue_dataname);
    	
    	if(myH9000_IGC_data.ok_flag==1)
    	{
    		if(myH9000_IGC_data.value==0)
    		{
    			set_ind_value_by_logicalname(ind_setvalue_dataname,IND_ALM_STATUS_1,0,IGC_DATA_MAN_SET_NOCHG,IGC_DATA_CHAN_FAIL_NOCHG,1);
    		}
    		else
    		{
    			set_ind_value_by_logicalname(ind_setvalue_dataname,IND_ALM_STATUS_0,0,IGC_DATA_MAN_SET_NOCHG,IGC_DATA_CHAN_FAIL_NOCHG,1);
    		}
    	}
    	else
    	{
    		printf("\n!!%s  Err .ok_flag!=1",ind_setvalue_dataname);
    	}
    	myH9000_IGC_data.ok_flag=0;
    	myH9000_IGC_data=get_data_value_by_pointid(stnid0,devid0,typeid0,pointid0);
    	
    	if(myH9000_IGC_data.ok_flag==1)
    	{
    		if(myH9000_IGC_data.value==0)
    		{
    			set_int_value_by_pointid(stnid0,devid0,typeid0,pointid0,pointid0+setvalue_point_len-1,IND_ALM_STATUS_1,0,IGC_DATA_MAN_SET_NOCHG,IGC_DATA_CHAN_FAIL_NOCHG,1);
    		}
    		else
    		{
    			set_int_value_by_pointid(stnid0,devid0,typeid0,pointid0,pointid0+setvalue_point_len-1,IND_ALM_STATUS_0,0,IGC_DATA_MAN_SET_NOCHG,IGC_DATA_CHAN_FAIL_NOCHG,1);
    		}
    	}
    	else
    	{
    		printf("\n!!%d.%d.%d.%d N=%d  Err .ok_flag!=1",stnid0,devid0,typeid0,pointid0,setvalue_point_len-1);
    	}
    
    
    
    	myH9000_IGC_data.ok_flag=0;
    	myH9000_IGC_data=get_data_value_by_logicalname(ana_setvalue_dataname);
    	
    	if(myH9000_IGC_data.ok_flag==1)
    	{
    		if(myH9000_IGC_data.value>200)
    			myH9000_IGC_data.value=0;
    		else
    			myH9000_IGC_data.value=myH9000_IGC_data.value+1;
    	 
    		set_ana_value_by_logicalname(ana_setvalue_dataname,myH9000_IGC_data.value,0,-1,-1,-1,1);
    	}
    	else
    	{
    		printf("\n!!%s  Err .ok_flag!=1",ana_setvalue_dataname);
    	}
    
    	myH9000_IGC_data.ok_flag=0;
    		myH9000_IGC_data=get_data_value_by_pointid(stnid1,devid1,typeid1,pointid1);
    	
    	if(myH9000_IGC_data.ok_flag==1)
    	{
    		if(myH9000_IGC_data.value>200)
    			myH9000_IGC_data.value=0;
    		else
    			myH9000_IGC_data.value=myH9000_IGC_data.value+1;
    		
    		set_ana_value_by_pointid(stnid1,devid1,typeid1,pointid1,pointid1+setvalue_point_len-1,myH9000_IGC_data.value,0,-1,-1,-1,1);
    	}
    	else
    	{
    			printf("\n!!%d.%d.%d.%d N=%d  Err .ok_flag!=1",stnid1,devid1,typeid1,pointid1,setvalue_point_len-1);
    	}
    	
    	printf("\n##&&&&&&igc_test__setvalue_by__ end\n\n");
  }  }
int main(int argc, char *argv[])
{
  int igc_i=0;
  clock_t start_ct,end_ct;
  long igc_time_seclen;
  long temp_ts=0;
  int ws_ctrl_prvg,ws_ctrl_prvg_loop;
  #ifdef	WINDOWS_OS
      WSADATA		WSAData;
  #endif
  #ifdef	WINDOWS_OS
  if(igc_WSAStartup()==-1)
  {
    igc_Err_log("Fail to initialize net!\n");
    exit(-1);
  }
  #endif
  if(-1==igc_MountDB(NULL))
  {
    igc_Err_log("Fail to Mount RDB");
    exit(-1);
  }
  if(igc_lan_out_init()==-1)
  {
    igc_Err_log("Fail to init lan_out");
    exit(-1);
  }
  if(igc_InterLockFileLoad()==-1)
  {
    igc_Err_log("Fail to igc_InterLockFileLoad");
    exit(-1);
  }
  if(IGC_Init_RECORD_IPC()==-1)
  {
    igc_Err_log("Fail to IGC_Init_RECORD_IPC");
    exit(-1);
  }
  if(-1==igc_InitStrTextDefine())
  {
  printf("Fail to igc_InitStrTextDefine");
  exit(-1);
  }
  init();
   ws_ctrl_prvg=-1;
   ws_ctrl_prvg_loop=200;
  for(;;)
  {
  if(ws_ctrl_prvg_loop==200)
  	ws_ctrl_prvg=IGC_GetCurentHostCtrlPrvg(1);
  else
  	ws_ctrl_prvg=IGC_GetCurentHostCtrlPrvg(0);
  ws_ctrl_prvg_loop++;
  if(ws_ctrl_prvg_loop>10)
  {
  	ws_ctrl_prvg_loop=0;
  }
  if(ws_ctrl_prvg!=1)
  {
  	if(ws_ctrl_prvg!=1) printf("\nthis is not the main host,igc is standby\n");
  	sleep_second_igclib(10);
  	continue;
  }
	   start_ct = clock();
	  lacal_datetime_now=get_lacal_datetime_now();
	  for(igc_i=0;igc_i<Itimer;igc_i++)
	  {
		  if(I_H9000_timers[igc_i].enable==yes)
		  {
			  if(I_H9000_timers[igc_i].inited==0)
			  {
				  I_H9000_timers[igc_i].inited=1;
				  I_H9000_timers[igc_i].lastdotime.day=lacal_datetime_now.day;
				  I_H9000_timers[igc_i].lastdotime.hour=lacal_datetime_now.hour;
				  I_H9000_timers[igc_i].lastdotime.minute=lacal_datetime_now.minute;
				  I_H9000_timers[igc_i].lastdotime.month=lacal_datetime_now.month;
				  I_H9000_timers[igc_i].lastdotime.second=lacal_datetime_now.second;
				  I_H9000_timers[igc_i].lastdotime.year=lacal_datetime_now.year;
				  I_H9000_timers[igc_i].real_count=0;
				  I_H9000_timers[igc_i].reached_times=0;
			  }
			  else
			  {
				  temp_ts=total_time_seconds(I_H9000_timers[igc_i].lastdotime,lacal_datetime_now);
				  switch(I_H9000_timers[igc_i].unit)
				  {
				  case unit_second:
					  break;
				  case unit_minute:
					  temp_ts=temp_ts/60;
					  break;
				  case unit_hour:
					  temp_ts=temp_ts/3600;
					  break;
				  case unit_day:
					  temp_ts=temp_ts/86400;
					  break;
				  default:
					  break;
				  }
				  if(temp_ts<0)
					  temp_ts=0-temp_ts;
				  I_H9000_timers[igc_i].real_count=(unsigned int)temp_ts;
				  if(I_H9000_timers[igc_i].real_count>=I_H9000_timers[igc_i].full_count)
				  {
					  if(I_H9000_timers[igc_i].autorestart==yes)
					  {	
						  I_H9000_timers[igc_i].real_count=0;
						  I_H9000_timers[igc_i].reached_times++;
					  }
					  else
					  {
						  I_H9000_timers[igc_i].enable=no;
					  }
					  I_H9000_timers[igc_i].inited=0;
				  }
			  }
		  }
	  }
	  for(igc_i=0;igc_i<Itimer_sysuse;igc_i++)
	  {
		  if(h9000_igc_timeer_sysuse[igc_i].enable==yes)
		  {
			  if(h9000_igc_timeer_sysuse[igc_i].inited==0)
			  {
				  h9000_igc_timeer_sysuse[igc_i].inited=1;
				  h9000_igc_timeer_sysuse[igc_i].lastdotime.day=lacal_datetime_now.day;
				  h9000_igc_timeer_sysuse[igc_i].lastdotime.hour=lacal_datetime_now.hour;
				  h9000_igc_timeer_sysuse[igc_i].lastdotime.minute=lacal_datetime_now.minute;
				  h9000_igc_timeer_sysuse[igc_i].lastdotime.month=lacal_datetime_now.month;
				  h9000_igc_timeer_sysuse[igc_i].lastdotime.second=lacal_datetime_now.second;
				  h9000_igc_timeer_sysuse[igc_i].lastdotime.year=lacal_datetime_now.year;
				  h9000_igc_timeer_sysuse[igc_i].real_count=0;
				  h9000_igc_timeer_sysuse[igc_i].reached_times=0;
			  }
			  else
			  {
				  temp_ts=total_time_seconds(h9000_igc_timeer_sysuse[igc_i].lastdotime,lacal_datetime_now);
				  switch(h9000_igc_timeer_sysuse[igc_i].unit)
				  {
				  case unit_second:
					  break;
				  case unit_minute:
					  temp_ts=temp_ts/60;
					  break;
				  case unit_hour:
					  temp_ts=temp_ts/3600;
					  break;
				  case unit_day:
					  temp_ts=temp_ts/86400;
					  break;
				  default:
					  break;
				  }
				  if(temp_ts<0)
					  temp_ts=0-temp_ts;
				  h9000_igc_timeer_sysuse[igc_i].real_count=(unsigned int)temp_ts;
				  if(h9000_igc_timeer_sysuse[igc_i].real_count>=h9000_igc_timeer_sysuse[igc_i].full_count)
				  {
					  if(h9000_igc_timeer_sysuse[igc_i].autorestart==yes)
					  {	
						  h9000_igc_timeer_sysuse[igc_i].real_count=0;
						  h9000_igc_timeer_sysuse[igc_i].reached_times++;
					  }
					  else
					  {
						  h9000_igc_timeer_sysuse[igc_i].enable=no;
					  }
					  h9000_igc_timeer_sysuse[igc_i].inited=0;
				  }
			  }
		  }
	  }

	    userfuc0igc_test__setvalue_by___def ();
	  sleep_millisecond_igclib(loop_sleep_minisec);
	  end_ct = clock();
	  run_count_for_avg++;
	  if(run_count<99999999)
	      run_count++;
	  last_run_millisecond =(unsigned int)((end_ct-start_ct)*1000/CLOCKS_PER_SEC);
	  if(min_run_millisecond>last_run_millisecond)
		  min_run_millisecond=last_run_millisecond;
	  if(max_run_millisecond<last_run_millisecond)
		  max_run_millisecond=last_run_millisecond;
	  all_run_millisecond+=last_run_millisecond;
	  if(run_count_for_avg>0)
		  avg_run_millisecond=all_run_millisecond/run_count_for_avg;
	  if(all_run_millisecond>99999999)
	  {
		  run_count_for_avg=0;
		  last_run_millisecond=0;
		  min_run_millisecond=0;
		  max_run_millisecond=0;
		  all_run_millisecond=0;
		  avg_run_millisecond=0;
	  }
  }
  igc_UnMountDB();
  igc_InterLockFree();
}
  void h9000_igc_cmd_opfuc_do(int cmdnum)
  {
    int i=0;
  	switch(cmdnum)
  	{
  	default:
  		printf("h9000_igc_cmd_opfuc_do Err,no this  cmdNum=-1060834700",cmdnum);
 		}
   }
  void h9000_igc_cmd_op_fuc()
  {
      int i=0;
  	int get_igc_cmd_num_sysuse=-1;
  	for(;;)
  	{
  		sleep_second_igclib(1000);
  		//get_igc_cmd_num_sysuse=......
  	     if(get_igc_cmd_num_sysuse>0)
  		{
  			for(i=0;i<Itimer_sysuse;i++)
              {
  	            if(h9000_igc_cmd_num_sysuse[i]==get_igc_cmd_num_sysuse)
                  {
                     h9000_igc_cmd_opfuc_do(get_igc_cmd_num_sysuse);
                      break;
                  }
              }
          }
      }
  }
