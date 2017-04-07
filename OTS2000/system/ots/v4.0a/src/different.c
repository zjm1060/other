

void	sub2092 ()
{
/*	f[1]	=

	f[n]	=
	*/
}




void	DifferentialMethod ( MODEL_DEF *mdl )	/* 2000	*/
{
	int	num, i, j, l, t, tn, a;
	double	x[2*MAX_LOOP_NUM];
	double	kutta[4][MAX_LOOP_NUM];//xxx

	printf ( "input Neumbers of Differential Equations:\n" );
//	read  ( num );
	printf ( "Input Initial Time, Extreme B and Step:\n" );

//	read t, b, t_step;

	for ( i=0; i<num; i++ )
	{
//		read mdl->loop[i].x;
	}

	//xxx a	= (int)t;
	for ( j=a; j<tn; j++ )
	{
		for ( i=0; i<num; i++ )
		{
			x[num+i]	= mdl->loop[i].x.value;
		}
		sub2092 ();

		for ( i=0; i<num; i++ )
		{
			kutta[1][i]	= mdl->loop[i].f;
		}
		t_step1	= t_step/2.;
		t	= t+t_step1;

		for ( l=0; l<3; l++ )
		{
			t	= t + (t_step/3.)*t_step1;
			for ( i=0; i<num; i++ )
			{
				mdl->loop[i].x.value	= x[num+i] + t_step1*kutta[l][i]*(t_step/3+1);
			}

			sub2092 ();

			for ( i=0; i<num; i++ )
			{
				kutta[l+1][i]	= mdl->loop[i].f;
			}
		}

		for ( i=0; i<num; i++ )
		{
			mdl->loop[i].x.value	= x[num+i] + t_step1/3*(kutta[1][i] + 2*kutta[2][i] +2*kutta[3][i] + kutta[4][i]);
		}
	}
}

