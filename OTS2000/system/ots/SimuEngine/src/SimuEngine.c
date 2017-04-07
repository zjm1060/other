/*	simulog.c

	This program is originated from calcu_fn.c in PAS subsystem by zy and
	re-apdated to be used for real time syetem simulation.

	Created by wdk 1997.11.10.
*/

#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
#include "/home/ems/system/ots/SimuEngine/inc/SimuEngine_fun.h"

#define BLOCK_MAX	10

extern	FILE	*fp_gen, *fp_list;
static	double	blocked_time;

DMS_COMMON	dms_rec;

#ifdef MSWIN32
 DWORD		threadseq,threadseq_logical,threadmdl,wait_thrid;
 WSADATA	wsaData;
#endif
#ifndef MSWIN32
 thread_t	threadseq,threadseq_logical,threadmdl,wait_thrid;
#endif
void * ThreadSeq( void * arg );
void * ThreadSeqLogical( void * arg );
void * ThreadMdl( void * arg );
int ChildThread(); 

short	ots_netcmd_ptr;
float	fspeed;
int		chg_any;
//int		logic_result;//zwj
void	LAN_ReadPtrInit ();
void	SequenceInitialize ();
void	ActiveAutoSeq();
void	StopSimulog();
int		BdFZTimes(float value);
extern CURVE_SHM_DATA	*pCurve;		//yyp debug
extern int	count;
int	first_time_run=0;

char log_str[128];

void	main(int argc, char *argv[])
{
	int i;
	printf ( "Simulog Start.\n" );

	ReadOtsDefFile();

	if(MountDB(DB_NAME) == -1)
	{
		printf("call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}

	if ( SimulogDataLoad () == -1 )
	{
		printf ( "simulog.dat error!\n" );
		exit (1);
	}

	//初始化
	for(i=0;i<=n_step;i++)
	{
		stepdb[i].old_ms = -1;
	}

	//yyp debug
	pCurve = (CURVE_SHM_DATA*) LinktoShareMem( SHM_TREND_KEY_CHAR );
	if( pCurve == (CURVE_SHM_DATA*)-1 )
	{
		Err_log(  "Fail to  Real curve shm");
		printf("trend_data_rec: RealCurve::  LinktoShareMem Error !!!!\n");
		exit(0) ;
	}
	if(get_ots_shm()==FALSE)
	{
		exit(0);
	}
	if (argc == 2 && strcmp(argv[1],"-debug") == 0)
	{
		shm_ots_addr->runflag = 1;
	}
	//yyp debug
	fspeed = 1;
	time_cnt	=0;
	printf ( "before LAN_ReadPtrInit.\n" );
	LAN_ReadPtrInit ( &ots_netcmd_ptr );
	printf ( "after LAN_ReadPtrInit.\n" );
	
	ChildThread();
	
	for(;;)
	{
		MySleep(1000);
	}
	UnMountDB ();
	SequenceFileFree ();
	printf ( "Simulog OK End.\n" );
}

int ChildThread()
{
	int 		res;
	char	log_str[256];
#ifndef 	MULTITHREAD	
	ThreadSeq(NULL);
#endif

#ifndef MSWIN32
	res =   thr_create(NULL, 0, ThreadSeq, NULL, 0, &threadseq);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== simulog: Create seq thread thr_id=%d OK\n",
			(int)threadseq);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== simulog: Create seq thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
	MySleep(1000);
	res =   thr_create(NULL, 0, ThreadSeqLogical, NULL, 0, &threadseq_logical);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== simulog: Create seq logical thread thr_id=%d OK\n",
			(int)threadseq_logical);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== simulog: Create seq logical thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
	MySleep(1000);
	res =   thr_create(NULL, 0, ThreadMdl, NULL, 0, &threadmdl);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== simulog: Create mdl thread thr_id=%d OK\n",
			(int)threadmdl);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== simulog: Create mdl thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
#else
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) &ThreadSeq, 
		 		(LPVOID)NULL,	0,	(LPDWORD)&threadseq);

	if(res!=(int)NULL)
	{
		sprintf(log_str,"==== simulog: Create seq thread thr_id=%d OK\n",
			(int)threadseq);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}
	else
	{
		sprintf(log_str,"==== simulog: Create seq thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;	    
	}
	MySleep(1000);
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) &ThreadSeqLogical, 
		 		(LPVOID)NULL,	0,	(LPDWORD)&threadseq_logical);

	if(res!=(int)NULL)
	{
		sprintf(log_str,"==== simulog: Create seq logical thread thr_id=%d OK\n",
			(int)threadseq_logical);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}
	else
	{
		sprintf(log_str,"==== simulog: Create seq logical thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;	    
	}
	MySleep(1000);
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) &ThreadMdl, 
		 		(LPVOID)NULL,	0,	(LPDWORD)&threadmdl);

	if(res!=(int)NULL)
	{
		sprintf(log_str,"==== simulog: Create mdl thread thr_id=%d OK\n",
			(int)threadmdl);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}
	else
	{
		sprintf(log_str,"==== simulog: Create thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;	    
	}
#endif

	MySleep(100);	
	return TRUE;
}

/*----------------------------------------------------------------------------------------------*/

int	CheckOneIntCondition ( int k, ENTRY_DEF *entry )
{
	int	flag;
	int	stat;
	//added by yyp 2008.3.25 
	stat	= dms_rec.var.ind.status.state;	
	if ( stat == entry->data.status )
	{
		flag	= SATISFIED;
	}
	else
	{
		flag	= UNSATISFIED;
		//added by yyp 2010.06.08

		//printf ("\tentry  %d \t %s: \t status is %d, not %d.\n", 
		//	k, entry->name, stat, entry->data.status );

	}
	return	flag;

}


int	CheckOneAnaCondition ( int k, ENTRY_DEF *entry )
{
	int	flag;
	double	value;
	value	= dms_rec.var.anlg.fValue;
	flag	= UNSATISFIED;
	switch ( entry->opr )
	{
	case	LT:
		if ( value < entry->data.value )
			flag	= SATISFIED;
 
		break;

	case	LE:
		if ( value <= entry->data.value )
			flag	= SATISFIED;
		break;

	case	GT:
		if ( value > entry->data.value )
			flag	= SATISFIED;

		break;

	case	GE:
		if ( value >= entry->data.value )
			flag	= SATISFIED;

		break;

	case	EQ:
		if ( value == entry->data.value )
			flag	= SATISFIED;
		break;

	case	NE:
		if ( value != entry->data.value )
			flag	= SATISFIED;
		break;

	default:
		;
	}

	if ( flag == UNSATISFIED )
	{
		//removed by yyp 2012.07.09
		//printf ("\tentry  %d \t %s: \t value is %.2f, not %.3f.\n", 
		//	k, entry->name, value, entry->data.value );
	}
	return	flag;

}

int	GetSceneIdByName ( char *scene_name )
{

	int	i, scene_id = -1;
	for ( i=0; i<n_scene; i++)
	{
		if ( !strcmp ( scenedb[i].scene_name, scene_name ) )
		{
			scene_id	= i;
			//printf( "in GetSceneIdByName: %s is found when i= %d\n", scenedb[i].scene_name, i );
			break;
		}
	}
	if ( scene_id == -1 )
		printf( "in GetSceneIdByName: %s is not found.\n", scene_name );
	return	scene_id;

}

void	DoSetScene ( char  *name )
{
	int	scene_id;
	int	data_type;
	int	i, init_head, init_num, init_end;
	scene_id	= GetSceneIdByName ( name );

	if ( scene_id == -1 )
	{
		return;
	}
	init_head	= scenedb[scene_id].head;
	init_num	= scenedb[scene_id].num;
	init_end	= init_head+init_num;
	for ( i=init_head; i<init_end; i++ )
	{
		if ( ReadEntryByNameStr ( scenesegdb[i].name, &dms_rec ) == -1 )
		{
			printf ("in Scene: id=%d name= %s is not found in database.\n",i,scenesegdb[i].name );
			continue;
		}
		else
		{
			data_type	= dms_rec.point.data_type;	//wdk 2008.3.22
			//yyp 2009.2.11
			MySleep(20);
			if(scenedb[scene_id].type == TYPE_NORMAL)
			{
				switch	( data_type )
				{
				case	ANA_TYPE:
				case	PARAM_TYPE:
						BdANAByName ( scenesegdb[i].name, scenesegdb[i].data.value );
						break;
				case	POL_TYPE:
				case	SOE_TYPE:
				case	SYSMSG_TYPE:
				case	DEVOBJ_TYPE:
						BdINDByName ( scenesegdb[i].name, (short)scenesegdb[i].data.status );
						break;
				default:
					break;
				}
				//added by yyp 2009.10.14
				if( strcmp(scenesegdb[i].name,"BS.BS_SYS.SYS.BY_061") !=0 )
				{
					BdIostSetCmd(scenesegdb[i].name,1);
				}

				MySleep(5);
			}
			else
			{
				switch	( data_type )
				{
				case	ANA_TYPE:
				case	PARAM_TYPE:
						BdANADisturb( scenesegdb[i] );
						break;

				case	POL_TYPE:
				case	SOE_TYPE:
				case	SYSMSG_TYPE:
				case	DEVOBJ_TYPE:
						BdINDByName ( scenesegdb[i].name, (short)scenesegdb[i].data.status );
						break;

				default:
					break;
				}
			}
		}
	}
}

int GetParamSeqFvalue(SEQUENCE_DEF *sq, char *name, float *p_value)
{
	int i;
	for(i=sq->param.head;i<sq->param.end;i++)
	{
		if( strcmp(name,paramseqdb[i].param_name) == 0 )
		{
			sscanf ( paramseqdb[i].pt_name, "%f", p_value );
			return TRUE;
		}
	}
	return FALSE;
}

void	GoOneStep ( SEQUENCE_DEF *sq, int numb )
{
	double	step_time;
	int	j, k;
	int	data_type;
	DMS_COMMON	dms_cmm_tmp;
	int	g_head	= stepdb[numb].head;
	int	g_num	= stepdb[numb].num;
	int	g_end	= g_head + g_num;
	int	e_head, e_num, e_end;
	int showid;
	int sq_id;
	int ms;
	short qlt;
	POINTER pt;
	USHORT state;
	DATE_TIME curtime;
	int ous_flag;
	int has_obj;
	int	model_id;
	unsigned int result;
	ous_flag=0;
	has_obj=0;
	sq_id = GetSequenceIdByName(sq->seq_name); 	


	GetSysTime(&curtime);
	//added by yyp 2009.3.9
	if ( stepdb[numb].delay_time > 0 )
	{
		if(stepdb[numb].old_ms == -1)
		{
			stepdb[numb].old_ms = curtime.second * 1000 + curtime.ms;
			return;
		}
		else
		{
			ms = curtime.second * 1000 + curtime.ms;	
			if( ms < stepdb[numb].old_ms )
			{
				ms = ms +60*1000;
			}
			//如果等待时间未到，则返回
			if( (ms-stepdb[numb].old_ms) < (stepdb[numb].delay_time *1000 / fspeed) )
			{
				return;
			}
		}
		//MySleep( (int) ( (float)stepdb[numb].delay_time * 1000 /fspeed) );
	}
	stepdb[numb].old_ms = -1;
	//added by yyp 2009.7.8 for sbin
	if(stepdb[numb].showid != -1 && fz_style == 1 )
	{
		showid=stepdb[numb].showid ;
		shm_ots_addr->childflag = 1;
		shm_ots_addr->child.pos.x = showdb[showid].x;
		shm_ots_addr->child.pos.y = showdb[showid].y;
		shm_ots_addr->child.time_len = showdb[showid].delay_time;
		strcpy(shm_ots_addr->child.name,showdb[showid].filename);
	}
	else
	{
		//shm_ots_addr->childflag = 2;
	}
	step_time	= sq->time.initial + sq->time.t;
	if(sq->type == SEQ_NORMAL)
	{
		sq->blocked_count ++;
		if(sq->blocked_count>=BLOCK_MAX)
		{
			printf ( "time_cnt:%10.2f	step_time:%10.2f cur_seq=%d	cur_step: %d\n", 
				time_cnt, step_time,sq_id, sq->cur_step );
			sq->blocked_count = 0;
		}
	}
	//removed by yyp 2009.7.26 

	//if ( time_cnt < step_time ) return;	/* step action time is not yet arrived*/

	if ( stepdb[numb].mask == MASK_WAIT_FOR )
	{
		sq->time.t	+= TIME_SPAN;
	}
	else if ( sq->status == PRC_BLOCKED )
	{
		sq->time.t	+= TIME_SPAN;
		sq->status = PRC_NORMAL;
	}
	else
	{
		sq->time.t	+= stepdb[numb].delay_time;
	}

	sq->cur_step++;

	for ( j=g_head; j<=g_end; j++ )
	{
		e_head	= groupdb[j].head;
		e_num	= groupdb[j].num;
		e_end	= e_head+e_num;
		data_type = 0;
		for ( k=e_head; k<e_end; k++ )
		{
			
			if( strncmp(entrydb[k].name,"PARAM",5) == 0)
			{

				if( GetParamSeqFvalue(sq, entrydb[k].name, &(dms_rec.var.anlg.fValue)) == TRUE )				
				{
					data_type = -1;
				}
				else
				{
					err_write("err: sequence entry param def no fount");
				}
			}
			else
			{
										//	printf("############entry name =%s\n", entrydb[k].name );	
				if ( ReadEntryByNameStr ( entrydb[k].name, &dms_rec ) == -1 )
				{
					fprintf ( fp_list, "\nin GoOneStep: [ entry %d ] is not found in database!\n", k );
					break;
				}
				data_type	= dms_rec.point.data_type;	//wdk 2008.3.22
			}

			if ( entrydb[k].opr == BIN_LG )
			{
				if ( data_type == SOE_TYPE || data_type == POL_TYPE || data_type == SYSMSG_TYPE || data_type == OUTS_TYPE )
				{
					if ( CheckOneIntCondition ( k, &entrydb[k] ) != SATISFIED )
					{
						//added by yyp 2010.6.8
						if(sq->type == SEQ_NORMAL  && sq->blocked_count>=BLOCK_MAX-1)
						{
							printf("sq name =%s, step =%s, entryname=%s not satisfied\n",
								sq->seq_name,stepdb[numb].o_name,entrydb[k].name);
						}
				
						break;
					}
					else
					{
						if(data_type == OUTS_TYPE)
						{
							//printf("############entry name =%s\n", entrydb[k].name );							
							GetPtIdByNameStr( entrydb[k].name, &pt );
							GetPtQlt(pt,&qlt);

							if(qlt==1)
							{
								ous_flag = 1;
						//		BdcastQlt(pt,0);//ZWJ WHY?
							}
							has_obj = 1;
						}
					}
				}
				else if  ( data_type == DEVOBJ_TYPE)
				{
					GetObjPtStateByNameStr( entrydb[k].name, &state );
					if ( state != entrydb[k].data.status )
					{
						//added by yyp 2010.6.8
						if(sq->type == SEQ_NORMAL  && sq->blocked_count >= BLOCK_MAX-1)
						{
							printf("sq name =%s, step =%s, entryname=%s not satisfied\n",
								sq->seq_name,stepdb[numb].o_name,entrydb[k].name);
						}
						break;
					}
					else
					{
	
						GetPtIdByNameStr( entrydb[k].name, &pt );
						GetPtQlt(pt,&qlt);
						if(qlt==1)
						{
							ous_flag = 1;
							BdcastQltObj(pt,0);
						}
						has_obj=1;
					}
				}
				else
				{
					fprintf ( fp_list, "\tin GoOneStep: %s isn't an ON_OFF type variable!\n",entrydb[k].name );
					break;
				}
			}
			else
			{
				if ( data_type == ANA_TYPE || data_type == PARAM_TYPE || data_type == -1)
				{

					if( entrydb[k].isnum == 0 )
					{
						//get entry value
						ReadEntryByNameStr ( entrydb[k].value_name, &dms_cmm_tmp );
						//printf("############entry name =%s\n", entrydb[k].value_name );
						entrydb[k].data.value=dms_cmm_tmp.var.anlg.fValue;
					}


					if ( CheckOneAnaCondition ( k, &entrydb[k] ) != SATISFIED )
					{
						//added by yyp 2010.6.8
						if(sq->type == SEQ_NORMAL  && sq->blocked_count>=BLOCK_MAX-1)
						{
							printf("sq name =%s, step =%s, entryname=%s not satisfied\n",
							sq->seq_name,stepdb[numb].o_name,entrydb[k].name);
						}
						break;
					}
				}
				else
				{
					fprintf ( fp_list, "Err: %s isn't an ANALOG type variable!\n",entrydb[k].name );
	/*					break;
	*/			}
			}
		}//end for k=e_head
		// if all condition is met
		if ( k >= e_end )	// 
		{
			//printf ( "in k >= e_end: time_cnt:%10.2f	cur_step: %d	mask:%d\n", 
			//	time_cnt, sq->cur_step, stepdb[numb].mask );
			if(stepdb[numb].equa_type == EQUA_CHANGE)
			{
				if(sq->first_time == 1)
				{
					sq->cur_step--;
					stepdb[numb].equa_old =1;
						return;
				}
				//if(stepdb[numb].equa_old == 1  && chg_any == 0 )
				//if last equal and no ous obj flag then return
				if(stepdb[numb].equa_old == 1)
				{
					if( ous_flag != 1 )
					{
						sq->cur_step--;
						return;
					}
					else if(ous_flag == 1 && chg_any == 1)
					{
						sq->cur_step--;
						return;						
					}
					else
					{
						
						printf("##############Change Met because of ous_flag sq name =%s, step =%s \n",
								sq->seq_name,stepdb[numb].o_name);
					}
				}
				//if met,and last not equal, but ous_flag and chg_any,then return
				else
				{
					if ( (ous_flag == 1 && chg_any == 1) || (has_obj == 1 && chg_any == 1) )
					{
						sq->cur_step--;
						stepdb[numb].equa_old = 1;	
						return;					
					}
				}


				//if(stepdb[numb].equa_old == 1  && has_obj == 1 )
				//{
				//	sq->cur_step--;
				//	return;
				//}

				printf("##############Change Met sq name =%s, step =%s \n",
								sq->seq_name,stepdb[numb].o_name);
				stepdb[numb].equa_old = 1;	

			}

			if(sq->first_time == 1)
			{
				sq->cur_step--;
				return;
			}

			sq->status = PRC_NORMAL;	/*	set sequence status normal.	*/

			if ( stepdb[numb].mask == MASK_STEP ||
				stepdb[numb].mask == MASK_ELSE_STEP )
			{
				if ( stepdb[numb].type == LOGIC )
				{
					GetPtIdByNameStr(stepdb[numb].o_name, &pt ); 
					//modified by yyp 2012.11.10
					//for ous or obj, if old value and set value equal to 0,then return
					if(stepdb[numb].result ==0 )
					{

						if( pt.data_type == OUTS_TYPE )
						{
							GetIndPtStateByNameStr( stepdb[numb].o_name,&state );
							if(state == 0)
							{
								return;
							}							
						}
						else if(pt.data_type == DEVOBJ_TYPE)
						{
							GetObjPtStateByNameStr( stepdb[numb].o_name, &state );
							if(state == 0)
							{
								return;
							}
						}
					}
					//if type is obj or ous ,then set force
					if( pt.data_type == OUTS_TYPE )
					{
						BdINDByName( stepdb[numb].o_name, (short)stepdb[numb].result );
						BdcastQltWithSta(pt, (short)stepdb[numb].result,1);
					}
				    else if( pt.data_type == DEVOBJ_TYPE)
					{
						BdObjForceByID( pt, (short)stepdb[numb].result );
					}
					else
					{
						if((stepdb[numb-1].equa_type == EQUA_LOGICAL)&&(stepdb[numb-1].result==0))
						{
						//	printf("======EQUA_LOGICAL===stepdb[%d].result=%d==\n",numb-1,stepdb[numb-1].result);//test
						//	printf("======sq->seq_name=%s==\n",sq->seq_name);//test
							stepdb[numb-1].result=1;
							if(stepdb[numb].result==1)
								BdINDByName( stepdb[numb].o_name, 0 );
							else
								BdINDByName( stepdb[numb].o_name, 1 );
						}
						else
						BdINDByName( stepdb[numb].o_name, (short)stepdb[numb].result );
					}
					return;
				}
			}
			else if ( stepdb[numb].mask == MASK_SEQUENCE ||
				stepdb[numb].mask == MASK_ELSE_SEQUENCE )
			{

				ActivateSequenceByName ( stepdb[numb].o_name, sq_id );

				return;
			}
			//added by yyp for dead loop 2009.10.13
			else if ( stepdb[numb].mask == MASK_DO_WHILE )
			{
				ActivateSequenceByName ( stepdb[numb].o_name, sq_id );
				sq->cur_step--;	
				return;
			}
			else if ( stepdb[numb].mask == MASK_MODEL ||
				stepdb[numb].mask == MASK_ELSE_MODEL )
			{
				//printf ( "\t*sequence\t[ group %d ] condition is met!\t\t*\n\n", j );
				model_id	= GetModelIdByName ( stepdb[numb].o_name );

				if ( model_id != -1 )
				{
					if(mdb[model_id].rts == PRC_ACTIVATED)
					{
						return;
					}
				}
				printf ( "****begin ActivateModelByName:%s:  step=%d sq= %s\n", sq->seq_name, sq->cur_step,stepdb[numb].o_name );
				//added by yyp 2009.10.13 needed to be modified
				if(stepdb[numb].speed <=0  || stepdb[numb].speed>100)
				{
					stepdb[numb].speed = NORMAL_SPEED;
				}
				ActivateModelByName ( stepdb[numb].o_name, stepdb[numb].speed,sq_id );
				return;
			}
			else if ( stepdb[numb].mask == MASK_STOP_SEQUENCE )
			{
				printf ( "in STOP_SEQUENCE, %s*************************************\n",stepdb[numb].o_name );
				StopSequenceByName ( stepdb[numb].o_name );
				return;
			}
			else if ( stepdb[numb].mask == MASK_STOP_MODEL )
			{
				printf ( "in STOP_MODEL****** seq %s:  step=%d %s *******************************\n",sq->seq_name,sq->cur_step, stepdb[numb].o_name );
				StopModelByName ( stepdb[numb].o_name );
				return;
			}
			else if ( stepdb[numb].mask == MASK_WAIT_FOR )
			{
				if(sq->type == SEQ_NORMAL  )
				{
					printf ( "WAIT_FOR XX: sq %s %d wait in step %d is OK.\n", sq->seq_name,sq_id, sq->cur_step );
				}
				return;
			}
			else if ( stepdb[numb].mask == MASK_TIMER_SEQUENCE ||
				stepdb[numb].mask == MASK_TIMER_MODEL )
			{
				printf ( "in set TIMER Process****** %s *******************************\n", stepdb[numb].o_name );
				SetTimerByID ( stepdb[numb].timer_id );
				return;
			}
			//added by yyp 2009.6.29
			else if (stepdb[numb].mask == MASK_SCENE)
			{
				//printf ( "in Set_Scene****** %s ************\n", stepdb[numb].o_name );
				DoSetScene ( stepdb[numb].o_name );
				//added by yyp
				//pCurve->isstop = 1;
				return;			
			}
		}//end for k >= e_end
	}//end for j=g_head

	/*	do else instructions. 
	if mask= step /else_step /sequence /else_sequence /model /else_model, then check 
	whether next_mask = else_step /else_sequence /else_model.
	if next_step is not an else condition, then the process is blocked. */

	if(stepdb[numb].equa_type == EQUA_CHANGE )
	{
		stepdb[numb].equa_old = 0;
	}

	if ( stepdb[numb].mask == MASK_STEP ||
		stepdb[numb].mask == MASK_ELSE_STEP ||
		stepdb[numb].mask == MASK_SEQUENCE ||
		stepdb[numb].mask == MASK_ELSE_SEQUENCE ||
		stepdb[numb].mask == MASK_MODEL ||
		stepdb[numb].mask == MASK_ELSE_MODEL )
	{
		if ( next_step_mask == MASK_ELSE_STEP ||
			next_step_mask == MASK_ELSE_SEQUENCE ||
			next_step_mask == MASK_ELSE_MODEL )
		{
			numb++;
			sq->cur_step++;
			sq->time.t	= sq->time.t + stepdb[numb].delay_time;
			if ( stepdb[numb].mask == MASK_ELSE_STEP ||
				stepdb[numb].mask == MASK_ELSE_SEQUENCE ||
				stepdb[numb].mask == MASK_ELSE_MODEL )
			{
				printf ( "in GoOneStep: %s in ELSE:\n", stepdb[numb].o_name );
				GoOneStep ( sq, numb );
				return;
			}
		}
		else
		{
			sq->status = PRC_BLOCKED;
			//added by yyp 2012.9.4
			if(sq->type == SEQ_NORMAL   && sq->blocked_count>=59 )
			{			
				printf ( "sq %s not satisfied in step: %d without following ELSE\n", sq->seq_name, sq->cur_step );
			}
			blocked_time	+= TIME_SPAN;
			//printf ( "blocked_time=%f	alarm_delay= %f\n", blocked_time, stepdb[numb].alarm_delay );
			if ( blocked_time > stepdb[numb].alarm_delay )
			{
				Beep ( 1000, 1000 );		// to be finished. An alarm should be issued later.
				//yyp debug
				printf ( "sq %s not satisfied in step: %d reach alarm_delay\n", sq->seq_name, sq->cur_step );				
			}
			sq->cur_step--;
			return;
		}
	}
	else if ( stepdb[numb].mask == MASK_STOP_SEQUENCE )
	{
		//printf ( "in STOP_SEQUENCE*************************************\n" );
		sq->cur_step--;
		return;
	}
	else if ( stepdb[numb].mask == MASK_STOP_MODEL )
	{
		sq->cur_step--;
		return;
	}
	/* if WAIT_FOR condition is not verified, wait until verified.	*/
	else if ( stepdb[numb].mask == MASK_WAIT_FOR )
	{
//		printf("@@@@@@@@@@@@sq->seq_name=%s@@@@@@@@@@@\n",sq->seq_name);//test
		if(stepdb[numb].equa_type == EQUA_LOGICAL) //zwj for EQUA_LOGICAL condition is not verified
		{
			//printf ( "EQUA_LOGICAL******************************\n" );
			stepdb[numb].result=0; //逻辑运算 条件不满足为0，满足为1
			return;
		}
		else
		//printf ( "WAIT_FOR: sq %s wait in step: %d\n", sq->seq_name, sq->cur_step );
		sq->cur_step--;
		return;
	}
	else if ( stepdb[numb].mask == MASK_DO_WHILE )
	{
		//printf ( "Exit Do While*************************************\n" );
		StopSequenceByName ( stepdb[numb].o_name );
	}
	if ( stepdb[numb].type == LOGIC )
	{
		printf ("\t***\t[ step %d ] condition is not met!\t\t***\n\n", numb );
		sq->cur_step--;
	}
}

void	SequenceFileFree ()
{
	free ( stepdb );
	free ( groupdb );
	free ( entrydb );
	fclose ( fp_list );
}

void	SequenceInitialize ( SEQUENCE_DEF *sq )
{
	int	data_type;
	int	i, init_head, init_num, init_end;

	init_head	= sq->init.head;
	init_num	= sq->init.num;
	init_end	= init_head+init_num;
	//yyp debug
	//printf("\n\nSequenceInitialize \n\n");
	for ( i=init_head; i<init_end; i++ )
	{
		if ( ReadEntryByNameStr ( initdb[i].name, &dms_rec ) == -1 )
		{
			printf ("in SequenceInitialize: id=%d name= %s is not found in database.\n",i,initdb[i].name );
			continue;
		}
		else
		{
			data_type	= dms_rec.point.data_type;	//wdk 2008.3.22
			//yyp 2009.2.11
			MySleep(20);
			switch	( data_type )
			{
			case	ANA_TYPE:
			case	PARAM_TYPE:
					BdANAByName ( initdb[i].name, initdb[i].data.value );
					break;

			case	POL_TYPE:
			case	OUTS_TYPE:
			case	SOE_TYPE:
			case	SYSMSG_TYPE:
			case	DEVOBJ_TYPE:
					//BdINDByName ( initdb[i].name,(short) initdb[i].data.status );
					WrIND ( initdb[i].name,(short) initdb[i].data.status );
					break;

			default:
				break;
			}
		}
	}

}


void ActivateSequenceById ( int sq_id, int parent_id )
{
	int	head	= sequencedb[sq_id].head;
	int	num	= sequencedb[sq_id].num;
	int i;
	if(fz_style == 1)
	{
		i=1;
	}
	else
	{
		i=0;
	}


	if ( sq_id != -1 )
	{
		//added by yyp 2009.7.8
		if(sequencedb[sq_id].rts == PRC_ACTIVATED)
		{
			return;
		}
		sequencedb[sq_id].rts = PRC_ACTIVATED;
		sequencedb[sq_id].cur_step = head;
		sequencedb[sq_id].time.initial	= time_cnt;
		sequencedb[sq_id].time.t	= 0;
		SequenceInitialize ( &sequencedb[sq_id] );
		sequencedb[sq_id].parentid = parent_id;
		if(	parent_id != -1 && sequencedb[sq_id].runmode != MODE_PARALLEL	)
		{
			sequencedb[parent_id].status = PRC_WAITING;
		}
		//added by yyp 2009.8.19

		if( strcmp(sequencedb[sq_id].dbin[i],"NULL") !=0)
		{
			shm_ots_addr->dbinflag = 1;
			strcpy(shm_ots_addr->dbinname,sequencedb[sq_id].dbin[i]);
		}
		//yyp debug
		//MySleep(200);
		if(sequencedb[sq_id].type == SEQ_NORMAL)
		{
			printf("Active Sequence %s\n",sequencedb[sq_id].seq_name);
		}
	}
}

void StopSequenceById ( int sq_id )
{
	int	head	= sequencedb[sq_id].head;
	int	num	= sequencedb[sq_id].num;
	int parentid;
	if ( sq_id != -1 )
	{
		sequencedb[sq_id].rts = PRC_INACTIVATED;
		parentid = sequencedb[sq_id].parentid;
		if(parentid != -1)
		{
			sequencedb[parentid].status = PRC_NORMAL;
		}
	}
}

void ExecuteSequenceById ( int sq_id )
{
	int	cur_step	= sequencedb[sq_id].cur_step;
	int	head	= sequencedb[sq_id].head;
	int	num	= sequencedb[sq_id].num;
	int i,parent_id;
	time_t tmptime;
	if ( ( cur_step >= head+num ) && ( sequencedb[sq_id].rts == PRC_ACTIVATED ) )
	{
		sequencedb[sq_id].rts = PRC_INACTIVATED;
		parent_id = sequencedb[sq_id].parentid; 
		if( parent_id != -1  && sequencedb[sq_id].runmode != MODE_PARALLEL )
		{
			sequencedb[parent_id].status = PRC_NORMAL;
		}
		if( cur_seq_id == sq_id )
		{
			cur_seq_id = -1;
			BdOTSStat(0);
			BdFZTimes(100);
		}
	}
	if( cur_seq_id == sq_id )
	{
		if(cur_seq_step != cur_step)
		{
			cur_seq_step = cur_step;
			if(num>=1)
			{
				BdFZTimes((cur_seq_step-head) * 100 / num);
			}

		}
		localtime(&tmptime);
		if( tmptime!= cur_seq_time)
		{
			//need to be modified by yyp 2009.10.15
			//BdFZTimes(tmptime-cur_seq_time);
		}
	}
	if ( sequencedb[sq_id].rts == PRC_INACTIVATED ) return;
	
	if ( sequencedb[sq_id].status == PRC_WAITING ) return;

	/* to check next step is a ELSE or not	*/
	if ( cur_step+1 < head+num )
	{
		next_step_mask	= stepdb[cur_step+1].mask;
	}
	else
	{
		next_step_mask	= MASK_STEP;	/* end also means no else */
	}
	
	//printf ( "ESQ/ID: time:%10.2f	sq_id:%d	cur_step:%d	head:%d	num:%d	mask:%d\n", 
	//	time_cnt, sq_id, cur_step, head, num, stepdb[cur_step].mask );
	//printf("sq_name =%s ; step_name =%s\n",sequencedb[sq_id].seq_name,stepdb[cur_step].o_name);

	if ( stepdb[cur_step].mask == MASK_STEP ||
		stepdb[cur_step].mask == MASK_SCENE ||
		stepdb[cur_step].mask == MASK_WAIT_FOR ||
		stepdb[cur_step].mask == MASK_DO_WHILE ||
		stepdb[cur_step].mask == MASK_MODEL ||
		stepdb[cur_step].mask == MASK_STOP_MODEL ||
		stepdb[cur_step].mask == MASK_TIMER_MODEL ||
		stepdb[cur_step].mask == MASK_STOP_SEQUENCE ||
		stepdb[cur_step].mask == MASK_TIMER_SEQUENCE ||
		stepdb[cur_step].mask == MASK_SEQUENCE )
	{

		if ( sequencedb[sq_id].status != PRC_BLOCKED ) 
		{
			blocked_time	= 0.;	// initialize when step begin.
		}

		switch ( stepdb[cur_step].type )
		{
		case	LOGIC:
			GoOneStep ( &sequencedb[sq_id], cur_step );
			break;

		case	CALC:
			DoDataCalculation ( &sequencedb[sq_id], cur_step );
			break;

		default:
			GoOneStep ( &sequencedb[sq_id], cur_step );
		}
	}
	else
	{
		sequencedb[sq_id].cur_step++;
	}
}



void	StopSequenceByName ( char *sq_name )
{
	int	sq_id;

	sq_id	= GetSequenceIdByName ( sq_name );
	if ( sq_id != -1 )
	{
		if(sequencedb[sq_id].rts == PRC_INACTIVATED)
		{
			return;
		}
		StopSequenceById ( sq_id );
		printf ( "in StopSequenceByName: sq_name= %s\n", sq_name );	
	}
	
}


void	ActivateSequenceByName ( char *sq_name, int parentid )
{
	int	sq_id;

	sq_id	= GetSequenceIdByName ( sq_name );


	if ( sq_id != -1 )
	{
		if(parentid != -1)
		{
			if(sequencedb[parentid].type == 1)
			{
				sequencedb[sq_id].type = 1;			
			}
		}
		if( sequencedb[sq_id].rts != PRC_ACTIVATED )		
		{
			ActivateSequenceById ( sq_id, parentid );
		}
	}
	else
	{
		printf("can't get sequence id %s\n", sq_name);
	}
	
}


ResetTimeCount()
{
	time_cnt	= 0;
}

void * ThreadMdl( void * arg )
{
	int i,j,k;
	int count=0;
	int	t2;
	float t_step, t_const;
	int seq_id;
	t_const = 0.001;  //yyp debug

	//modified by yyp for multiple models
	for (;;)
	{
		k++;
		if(k % 10 ==0)
		{
			printf ( "############### model main: k = %d\n", k );
		}
		MySleep( (int) ( (float)TIME_SPAN * 1000 / (fspeed) ));
		if (shm_ots_addr->runflag != 1 ) 
		{
			continue;
		}
		//added by yyp 2009.07.09
		//t_step = t_const * fspeed;
		t_step = t_const;
		t2	= (int)(TIME_SPAN/t_step);
		count++;
		if(count>=32000)
		{
			count=0;
		}
		// run models
		for ( i=0; i<n_model; i++ )
		{
			if(mdb[i].time.t >= 60	)
			{
				StopModelByID(i);
			}
			if( mdb[i].rts == PRC_INACTIVATED )
			{
				continue;
			}
			seq_id = mdb[i].seq_id;
			if(seq_id>0  && seq_id<n_sequence)
			{
				if(sequencedb[seq_id].rts == PRC_INACTIVATED )
				{
					//StopModelByID(i);
					//continue;
				}
			}
			if( count % (64/mdb[i].speed) != 0)
			{
				continue;
			}
			for ( j=0; j<t2; j++ )
			{
				ExecuteModelById ( i );
				if( j%20 ==0 )
				{
					WriteModelOutput ( &mdb[i] );  
					//MySleep((int) ((float)10 / fspeed) );
				}
			}
		}
	}
}


void * ThreadSeqLogical( void * arg )
{
	int	i, j;
	int old_runflag;
	int tmpdata;
	int con_status;
	int	sq_id;
	int num=0;
	int icount=0;
	j	= 0;
	ActiveAutoSeq();
	shm_ots_addr->chg_any = 1;

	for (;;)
	{
		j++;
		if(j % 10 ==0)
		{
			printf ( "############### seq logical main: j = %d\n", j );
		}		
		old_runflag = runflag;
		runflag = shm_ots_addr->runflag;
//		runflag = 1;// for test

		//MySleep(4000);
		//MySleep( (int) ((float )TIME_SPAN * 1000 / fspeed));
		MySleep( (int) ((float )0.2 * 1000 / fspeed));
		if(runflag == 0)			//stop
		{
			continue;
		}
		else if(runflag == 2)		//pause
		{
			continue;
		}
		/* check sequence condition  */
		num = 0;
		for ( i=0; i<n_sequence; i++ )
		{
			if( sequencedb[i].type != SEQ_NORMAL )
			{			
				ActivateSequenceByName ( sequencedb[i].seq_name, -1 );
				num++;
			}
		}
		//printf("logical sequence num=%d \n",num);
		/* run sequences	*/
		//for ( i=0; i<n_sequence; i++ )
		//{
		//	if( sequencedb[i].type != SEQ_NORMAL )
		//	{
		//		ExecuteSequenceById ( i );
		//	}
		//}

	}
}

void * ThreadSeq( void * arg )
{
	int	i, j;
	int old_runflag;
	int tmpdata;
	int	sq_id;
	int icount;
	j	= 0;
	icount = 0;
	shm_ots_addr->speed = 8;
	//shm_ots_addr->runflag = 1;
//	shm_ots_addr->chg_any = 1;//for test

	BdOTSStat(0);
	for (;;)
	{
		j++;
		if(j % 10 ==0)
		{
			printf ( "############### seq main: j = %d\n", j );
		}
		fz_style = GetFZStyle();
		tmpdata = shm_ots_addr->speed ;
		fspeed = (float) tmpdata/8;
		old_runflag = runflag;
		runflag = shm_ots_addr->runflag;
//		runflag = 1;// for test
		chg_any = shm_ots_addr->chg_any;
//		chg_any = 1;// for test
		//added by yyp 2012.09.03
		//yyp debug
		printf("###############chg_any=%d \n",shm_ots_addr->chg_any);	
		printf("###############runflag=%d \n",shm_ots_addr->runflag);
		if(chg_any==1)
		{
			icount++;
			if(icount>0)
			{
				shm_ots_addr->chg_any = 0;
				icount = 0;
				for ( i=0; i<n_sequence; i++ )
				{
					if( sequencedb[i].type != SEQ_NORMAL )
					{			
						StopSequenceByName ( sequencedb[i].seq_name);
					}
				}
				for ( i=0; i<n_sequence; i++ )
				{
					if( sequencedb[i].type != SEQ_NORMAL )
					{			
						ActivateSequenceByName ( sequencedb[i].seq_name, -1 );
					}
				}
			}

		}
		else
		{
			icount=0;
		}
		//MySleep( (int) ((float )TIME_SPAN * 1000 / fspeed));
		MySleep( (int) ((float )0.2 * 1000 / fspeed));
		CheckCmdBuffer ();
		//ActivateTimedTask ();
		if(runflag == 0)			//stop
		{
			if( old_runflag != 0 )
			{
				StopSimulog();
				BdOTSStat((short)runflag);
				cur_seq_step = -1;
				cur_seq_step = 0;
				BdFZTimes(0);
			}
			pCurve->nextpt = 0;
			pCurve->isstop=1;
			shm_ots_addr->childflag = 2;
			continue;
		}
		else if(runflag == 2)		//pause
		{
			if(old_runflag != 2)
			{
				printf("pause simulog...\n");				
				pCurve->isstop=2;
				BdOTSStat(0);
			}
			continue;
		}
		else if(runflag == 1)		//run
		{
			if(old_runflag != 1)
			{
				printf("resume simulog...\n");	
				pCurve->isstop=0;		
				shm_ots_addr->childflag = 0;
				BdOTSStat((short)runflag);
				first_time_run=1;
			}
		}


		time_cnt	= time_cnt+TIME_SPAN;
		if ( time_cnt > MAX_TIME_CNT )
		{
			ResetTimeCount ();
		}

		/* run sequences	*/
		for ( i=0; i<n_sequence; i++ )
		{
			if(first_time_run==1)
			{	sequencedb[i].first_time=1;
			//	break;
			}
			else
				sequencedb[i].first_time=0;
			//if( sequencedb[i].type == SEQ_NORMAL )
			//{
				ExecuteSequenceById ( i );
			//}
		}
		first_time_run=0;
		//shm_ots_addr->chg_any = 0;//重置工况后，第一次循环完成
		printf("############### first_time_run =%d \n",first_time_run);
#ifndef MULTITHREAD
		/* run models	*/
		for ( i=0; i<n_model; i++ )
		{
			ExecuteModelById ( i );
		}
#endif
	}
}



void ActiveAutoSeq()
{
	int	i;
	int	sq_id;
	//Active Sequence type
	for(i=0;i<seqid_auto.num;i++)
	{
		sq_id	= GetSequenceIdByName ( seqid_auto.seg[i].seqname );
		if ( sq_id != -1 )
		{
			//sequencedb[sq_id].type = 1;
			ActivateSequenceByName ( seqid_auto.seg[i].seqname, -1 );
		}
	}
}


void StopSimulog()
{
	int i;
	printf("stop simulog...\n");
	for ( i=0; i<n_sequence; i++ )
	{
		if( sequencedb[i].type == SEQ_NORMAL )
		{
			if(sequencedb[i].rts != PRC_INACTIVATED)
			{
				StopSequenceById (i);
			}
		}
	}
	/*
	for ( i=0; i<n_model; i++ )
	{
		StopModelByID (i);
	}
	*/
}


int BdFZTimes(float value)
{
	char		tmp[100],buf[100];
	POINTER		pt;
	unsigned char stnid,devid;
	//added by yyp 2012.04.10
	return TRUE;
	//added by yyp 2010.8.5
	shm_ots_addr->prgrate = (unsigned char)value;
 	stnid = 1;
	devid = g_sys_lcu[stnid];	
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".ANA.FZ_TIMES");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);

	BdANAByName(buf,value);
	return TRUE;
}


void	DoDataCalculation ( SEQUENCE_DEF *sq, int numb )
{
	DMS_COMMON	dms_rec,dms_cmm_tmp;
	double	step_time;
	int	e_end;
	int	j, k;
	float	value, temp;

	int ms;
	DATE_TIME curtime;
	GetSysTime(&curtime);
	//added by yyp 2009.3.9
	if ( stepdb[numb].delay_time > 0 )
	{
		if(stepdb[numb].old_ms == -1)
		{
			stepdb[numb].old_ms = curtime.second * 1000 + curtime.ms;
			return;
		}
		else
		{
			ms = curtime.second * 1000 + curtime.ms;	
			if( ms < stepdb[numb].old_ms )
			{
				ms = ms +60*1000;
			}
			//如果等待时间未到，则返回
			if( (ms-stepdb[numb].old_ms) < (stepdb[numb].delay_time *1000/fspeed) )
			{
				return;
			}
		}
		//MySleep( (int) ( (float)stepdb[numb].delay_time * 1000 /fspeed) );
	}

	if(sq->type == SEQ_NORMAL)
	{
		step_time	= sq->time.initial + sq->time.t;
		printf ( "time_cnt:%10.2f	step_time:%10.2f	cur_step: %d\n", 
		time_cnt, step_time, sq->cur_step );
	}
	//yyp debug
	//err_write("############# hello \n");
	//removed by yyp 2009.10.16
	//if ( time_cnt < step_time ) return;	/* step action time is not yet arrived*/

	sq->time.t	+= stepdb[numb].delay_time;
	sq->cur_step++;
	stepdb[numb].old_ms = -1;
#ifdef DEBUG
	fprintf ( fp_list, "\n[step %d]:\t%s\n", numb, stepdb[numb].o_name );
#endif

	j	= stepdb[numb].head;
	e_end	= groupdb[j].head+groupdb[j].num;

	for ( k=groupdb[j].head; k<e_end; k++ )
	{
		if ( entrydb[k].opr&RESET_C )
		{
			if( entrydb[k].isnum == 0 )
			{
				//get entry value
				ReadEntryByNameStr ( entrydb[k].value_name, &dms_cmm_tmp );
				entrydb[k].data.value=dms_cmm_tmp.var.anlg.fValue;
			}		
			temp	= (float)entrydb[k].data.value;
		}
		else
		{
			if ( ReadEntryByNameStr ( entrydb[k].name, &dms_rec ) == -1)
			{
				fprintf ( fp_list, "group=%d  entry=%d %s not find in db!\n", j, k-groupdb[j].head, entrydb[k].name );
				return;
			}
			temp	= dms_rec.var.anlg.fValue;
		}

		switch ( entrydb[k].opr&MASK_C )
		{
		case 	Simu_RESET:
			value	= temp;
			break;

		case	ADD:
			value	+= temp;
			break;

		case	MIN:
			value	-= temp;
			break;

		case	MUL:
			value	*= temp;
			break;

		case	DIV:
			if (temp==0)	
				value=0;
			else
				value	/= temp;
			break;

		case	EXP:
			value	*= value;
			break;
		
		case	SQU:
			value	= sqrt(value);
			break;

		default:
			;
		}
	}


	BdANAByName ( stepdb[numb].o_name, value );
}