
#include	<stdio.h>


int	GetInterpolationIndex ( int m, double x0, double x[] )
{
	int	i, k;

	for ( i=0; i<m-3; i++ )
	{
		if ( x0 <= x[i+1] )
		{
			k	= i;
			break;
		}
		else
		{
			k	= m-2;
		}
	}

	if ( k != 0 )
	{
		if ( x0-x[k] >= x[k+1]-x0 )
		{
			k--;
		}
	}

	return k;

}

void	GetInterpolationFactor ( double x[], double x0, double u[] )
{

	u[0]	= (x0-x[1])*(x0-x[2])/((x[0]-x[1])*(x[0]-x[2]));
	u[1]	= (x0-x[0])*(x0-x[2])/((x[1]-x[0])*(x[1]-x[2]));
	u[2]	= (x0-x[0])*(x0-x[1])/((x[2]-x[0])*(x[2]-x[1]));

}


double	BiInterpolation ( int m, double x0, double x[], double fc[] )
{
	int	i, k;
	double	u[3], f;

	k	= GetInterpolationIndex ( m, x0, x );
	GetInterpolationFactor ( &x[k], x0, u );

	f	= 0.;
	for ( i=0; i<3; i++ )
	{
		f	= f+u[i]*fc[k+i];
	}

#ifdef	DEBUG
	printf ( "in BiInterpolation: k= %d\tf= %5.2f\n", k, f );
#endif

	return f;

}


double	TriInterpolation ( int mx, int my, double x0, double y0, double x[], double y[], double fc[] )
{
	int	i, j, k, l, addr;
	double	f, u[3], v[3];

	k	= GetInterpolationIndex ( mx, x0, x );
	GetInterpolationFactor ( &x[k], x0, u );

	l	= GetInterpolationIndex ( my, y0, y );
	GetInterpolationFactor ( &y[l], y0, v );

	f	= 0.;
	for ( i=0; i<3; i++ )
	{
		addr	= (l+i)*mx+k;
		for ( j=0; j<3; j++ )
		{
			f	= f+u[j]*v[i]*fc[addr+j];
		}
	}

#ifdef	DEBUG
	printf ( "in TriInterpolation: k= %d\tl= %d\tf= %5.2f\n", k, l, f );
#endif

	return f;
	
}



