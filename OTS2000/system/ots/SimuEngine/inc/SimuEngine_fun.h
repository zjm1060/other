
/* simulog_rtdb_def.h file
*/

/*----------------------------------------------------------------------------------------------*/
/*					SIMULOG DATABASE															*/
/*----------------------------------------------------------------------------------------------*/
#ifndef _SIMULOG_FUN_HEAD
#define _SIMULOG_FUN_HEAD



int stop_sequence;
int runflag;
int fz_style;		//仿真风格，0: 表示动态风格，即弹出子画面		1:	简单风格，不弹出子画面
int	cur_seq_id;
int cur_seq_step;
time_t cur_seq_time;




void	ActivateSequenceByName ( );
void	ActivateModelByName ( );
void	ActivateTimedTask ( );
double	BiInterpolation ();
void	CheckCmdBuffer ();
void	DoDataCalculation ();
void	ExecuteModelById ();
void	GetCmdNameStringFromCmdbuf ();
//void	OTS_RTExectution ();
void * ThreadSeq( void * arg );
void * ThreadMdl( void * arg );


void	SetTimerByID ( int timer_id );

void	StopSequenceByName ( char *sq_name );
void	StopSequenceById ( int sq_id );
void	StopModelByName ( char *mdl_name );
void	StopModelByID(int mdl_id);


#endif
