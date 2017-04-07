

/*	timed_task.c

	This program is created by wdk and used to timed task handling.

	Created by wdk 2003.5.11.
*/

#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
#include "/home/ems/system/ots/SimuEngine/inc/SimuEngine_fun.h"

void	SetTimerByID ( int timer_id )
{
	struct	tm ltime;
	time_t t;
	TIMER_DEF	*timer	= &timerdb[timer_id];

	printf ( "in SetTimerByID: timer_id= %d\n", timer_id );
	if ( timer_id == -1 || timer_id >= n_timer )
	{
		return;
	}
	
	timer->rts = PRC_ACTIVATED;
	printf ( "in SetTimerByID: timer_id= %d	rts= %d\n", timer_id, timerdb[timer_id].rts );

	/*	time to be set when sequence or model is activated. */
	time ( &t );
	ltime = *localtime ( &t );

}

/*
void	UpdateXclock (int crt_n)
{
	struct	tm ltime;
	time_t t;

	time (&t);
	if( t % 2 != 0 ) {
		Sleep(1000);   // delay 1 second.
		time (&t);
	}
	ltime =*localtime (&t);
}
*/


void	ActivateTimedTask ()
{
	int	i;
	TIMER_DEF	*timer;
	
	//printf ( "in ActivateTimedTask.\n" );

	for ( i=0; i<n_timer; i++ )
	{
		timer = &timerdb[i];
		if ( timer->rts == PRC_ACTIVATED )
		{
			/*	to be finished: time to be checked.	*/
			if ( timer->prc_type == PRC_SEQUENCE )
			{
				printf ( "in ActivateTimedTask (Sequence): %s.\n", timer->name );
				ActivateSequenceByName ( timer->name, -1 );
			}
			else if ( timer->prc_type == PRC_MODEL )
			{
				printf ( "in ActivateTimedTask (model): %s.\n", timer->name );
				ActivateModelByName ( timer->name,10,0 );
			}
		}
	}
}
