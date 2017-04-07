/*	gen_coef_calc.c program

	Created by wdk, 2004.1.31.
	Used to calculate generator model coefficients (k1-k6).

*/

//#include	<unistd.h>
#include	<fcntl.h>
#include	<math.h>
#include	<stdio.h>

#include<malloc.h>


#define	MAX_P_POINTS		20
#define	MAX_Q_POINTS		10



typedef	struct
{
	double	k1, k2, k3, k4, k5, k6;
}	K_COEF_DEF;

K_COEF_DEF	data[MAX_P_POINTS+1][MAX_Q_POINTS+1];
double	p_delta, q_delta;


#ifdef	UNIX_VERSION
char	*listfile	= "/OTSDATA/GenCoefList.txt";
#else
char	*listfile	= "/users/ems/project/ots/v1.0a/data/GenCoefList.txt";
#endif


FILE	*fp_list;

void	WriteA_OutData ()
{
	int	i, j;

	fprintf ( fp_list, "	" );
	for ( j=0; j<=MAX_P_POINTS; j++ )
	{
		fprintf ( fp_list, "%8.4f  ", p_delta*j );
	}
	fprintf ( fp_list, "\n\n" );

	for ( i=0; i<=MAX_Q_POINTS; i++ )
	{
		fprintf ( fp_list, "%8.4f\nk1	", i*q_delta );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k1 );
		}
		fprintf ( fp_list, "\nk2	" );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k2 );
		}

		fprintf ( fp_list, "\nk3	" );
			fprintf ( fp_list, "%8.4f  ", data[0][i].k3 );

		fprintf ( fp_list, "\nk4	" );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k4 );
		}
		fprintf ( fp_list, "\nk5	" );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k5 );
		}
		fprintf ( fp_list, "\nk6	" );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k6 );
		}
		fprintf ( fp_list, "\n\n" );
	}
	
}


void	WriteB_OutData ()
{
	int	i, j;

	fprintf ( fp_list, "	X_DATA:	" );
	for ( j=0; j<=MAX_P_POINTS; j++ )
	{
		fprintf ( fp_list, "%8.4f  ", p_delta*j );
	}
	fprintf ( fp_list, "\n\n" );

	fprintf ( fp_list, "\n--- k1 -------------------------\n\n" );
	for ( i=0; i<=MAX_Q_POINTS; i++ )
	{
		fprintf ( fp_list, "	Y_DATA:	%8.4f\n	F_DATA:	", i*q_delta );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k1 );
		}
		fprintf ( fp_list, "\n\n" );
	}
	

	fprintf ( fp_list, "\n--- k2 -------------------------\n\n" );
	for ( i=0; i<=MAX_Q_POINTS; i++ )
	{
		fprintf ( fp_list, "	Y_DATA:	%8.4f\n	F_DATA:	", i*q_delta );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k2 );
		}
		fprintf ( fp_list, "\n\n" );
	}

	fprintf ( fp_list, "\n--- k3 -------------------------\n\n" );
	fprintf ( fp_list, "\n	F_DATA:	" );
	fprintf ( fp_list, "%8.4f  ", data[1][1].k3 );
	fprintf ( fp_list, "\n\n" );

	fprintf ( fp_list, "\n--- k4 -------------------------\n\n" );
	for ( i=0; i<=MAX_Q_POINTS; i++ )
	{
		fprintf ( fp_list, "	Y_DATA:	%8.4f\n	F_DATA:	", i*q_delta );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k4 );
		}
		fprintf ( fp_list, "\n\n" );
	}


	fprintf ( fp_list, "\n--- k5 -------------------------\n\n" );
	for ( i=0; i<=MAX_Q_POINTS; i++ )
	{
		fprintf ( fp_list, "	Y_DATA:	%8.4f\n	F_DATA:	", i*q_delta );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k5 );
		}
		fprintf ( fp_list, "\n\n" );
	}


	fprintf ( fp_list, "\n--- k6 -------------------------\n\n" );
	for ( i=0; i<=MAX_Q_POINTS; i++ )
	{
		fprintf ( fp_list, "	Y_DATA:	%8.4f\n	F_DATA:	", i*q_delta );
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{
			fprintf ( fp_list, "%8.4f  ", data[j][i].k6 );
		}
		fprintf ( fp_list, "\n\n" );
	}



}



void	main ()
{
	int	i, j;
	double	p, q, p2q2;					// active & reactive power
	double	fi, cos_f, sin_f;			// cosine fi
	double	teta, teta_beta;			// teta-beta
	double	xd, xdp, xq, r, re, xe;		// resistance & impeadance
	double	v_ref, va, vd, vq, eq, eqa;		// voltage
	double	ia, ir, ix, id, iq, ifd;	// current
	double	xde, xdpe, xqe;
	double	ki, tmp1, tmp2, tmp3, tmp4;

	p_delta	= 1./(double)MAX_P_POINTS;
	q_delta	= 1./(double)MAX_Q_POINTS;

	va	= 1.0;
	v_ref	= 0.828;
	eq	= 2.56;
	r	= 0.001;
	re	= 0.02;
	xe	= 0.392;
	xd	= 1.7;
	xdp	= .245;
	xq	= 1.64;

	xde	= xd + xe;
	xdpe	= xdp + xe;
	xqe	= xq + xe;
	ki	= 1./(re*re + xqe*xdpe);

	q	= q_delta;
	for ( i=0; i<=MAX_Q_POINTS; i++ )
	{
		q	+= q_delta;

		p	= p_delta;
		for ( j=0; j<=MAX_P_POINTS; j++ )
		{

			if ( i == 0 && j == 0 ) 
			{
				continue;
			}

			p	+= p_delta;
			p2q2	= sqrt ( p*p + q*q );
			cos_f	= p/p2q2;
			sin_f	= q/p2q2;
			fi	= acos ( cos_f );
//			printf ( "p= %f	q= %f	cos fi= %f\n", p, q, cos_f );

			ia	= p2q2/va;
			ir	= ia*cos_f;
			ix	= -ia*sin_f;
//			printf ( "ia= %f	ir= %f	ix= %f\n", ia, ir, ix );

			tmp1	= ( xq*ir + r*ix )/( va + r*ir - xq*ix );
			teta_beta	= atan ( tmp1 );

			vd	= -va*sin ( teta_beta );
			vq	= va*cos ( teta_beta );

			id	= -ia*sin ( teta_beta + fi );
			iq	= ia*cos ( teta_beta + fi );
			eqa	= eq - ( xq - xdp )*id;
//			printf ( "va= %f	vd= %f	vq= %f	ia= %f	id= %f	iq= %f\n", va, vd, vq, ia, id, iq );

			tmp2	= ( xqe*ir + (r+re)*ix) / (v_ref -xqe*ix+(r+re)*ir);
			teta	= atan ( tmp2 );
//			printf ( "tmp2= %f	teta= %f\n", tmp2, teta );

			tmp3	= re*sin(teta) + xdpe*cos(teta);
			tmp4	= xqe*sin(teta) - re*cos(teta);
			data[j][i].k1	= ki*v_ref*( eqa*tmp3 + iq*(xq-xdp)*tmp4 );

			data[j][i].k2	= ki*(re*eqa + iq*(re*re + xqe*xqe ));	// ok
			data[j][i].k3	= xdpe/ xde;	//ok
			data[j][i].k4	= v_ref*ki*(xd-xdp)*(xqe*sin(teta)-re*cos(teta));	//ok
			data[j][i].k5	= eq*v_ref*cos(teta)/ xdpe + v_ref*v_ref*cos(2.*teta)*(xdp-xq)/(xdpe*xqe);

			tmp3	= re*cos(teta) - xqe*sin(teta);
			tmp4	= xdpe*cos(teta) + re*sin(teta);
			data[j][i].k5	= ki*v_ref* ( xdp*vq*tmp3 - xq*vd*tmp4 ) / va;

			data[j][i].k6	= vq * (( 1.-ki*xdp*xqe ) - vd*ki*xq*re )/va;	// ok

		}
	}

	if ( ( fp_list = fopen ( listfile, "w" ) ) == NULL )
	{
		printf ( "gen_coef_calc.c:can't open (w) the file %s!\n", listfile );
		return;
	}

	WriteA_OutData ();
	WriteB_OutData ();

	fclose ( fp_list );

}

