
#include <stdio.h>
#include <math.h>


#define	NFUNC	76

typedef struct
{
	char	func_name[20];
	int	type;
	int	input[3];
	int	order;
}	FUNC_BLOCK_TYPE;

typedef struct
{
	char	block_name[20];
	float	input[3];
	float	init;
}	BLOCK_INPUT_FACT;

typedef struct
{
	float	value[3];
}	FUNC_GEN_VALUE;

typedef struct
{
	float	fact[4];
}	KUTTA_RUNGE_FACT_DEF;


FUNC_BLOCK_TYPE	iu[75];
float	line[61];
int	n1, n2, constant_numb, udelayer_numb, delayer_numb;
int	output_ch, n7, block_numb, integrator_numb, m1[5], m2, m3;
float	t1, t2;
float	ct[NFUNC], block_output[NFUNC];
float	rtemp[150][6];
int	iv[30], delayer[30];
FUNC_GEN_VALUE	func[11];
BLOCK_INPUT_FACT	pfact[NFUNC];
float	f_lk[30], y_iout[30], w_iout[30];
KUTTA_RUNGE_FACT_DEF	krf[30];
float	out1, out2, out3, out4;
float	istep, demi_istep, am5;
int	n_func[NFUNC], n_delay[5], ia1, ia2;
int	k1, k2, k3, k4, k5, k6, kprint, kk, is9;


FILE	*fp;

void	ReadProcessData ()
{
	int	i, j, k, l, i6, j1, iprint;
	int	len	= 4;

	printf ( "Please Enter Channel Code of Output Equipment:" );
	fread ( &output_ch, len, 1, fp );
	n7	= 0;
	fread ( &i, len, 1, fp );
	fread ( &i6, len, 1, fp );
	fread ( &j, len, 1, fp );
	fread ( &k, len, 1, fp );
	fread ( &l, len, 1, fp );

	while ( i <= 0 )
	{
		iu[i].type	= i6;
		iu[i].input[1]	= j;
		iu[i].input[2]	= k;
		iu[i].input[3]	= l;
		n7++;

		fread ( &i, len, 1, fp );
		fread ( &i6, len, 1, fp );
		fread ( &j, len, 1, fp );
		fread ( &k, len, 1, fp );
		fread ( &l, len, 1, fp );
	}

	for ( i=0; i<n7; i++ )
	{
		while ( iu[i].type < 0 )
		{
			printf ( "illegal type block, re-enter type\n" );
			fread ( &iu[i].type, len, 1, fp );
		}

		if ( iu[i].type < 41 ) continue;

	}

	constant_numb	= 0;
	udelayer_numb	= 0;
	delayer_numb	= 0;
	integrator_numb	= 0;
	kk	= 0;

	for ( i=0; i<n7; i++ )
	{
		i6	= iu[i].type;
		if ( i6 != 9 ) continue;
		else
		{
			n2++;
			iu[i].order	= n2;
		}
	}

	if ( n2 != 0 )
	{
		for ( i=0; i<n7; i++ )
		{
			if ( iu[i].type == 9 )
			{
				j	= iu[i].order;
				printf ( "please enter the 11 data of func. generator, %d\n", j );

				for ( j1=0; j1<11; j1++ )
				{
					fread ( &func[j1].value[j], len, 1, fp );
				}
			}
			else
				continue;
		}
	}
	printf ( "plaese enter the second part data\n" );

	fread ( &i, len, 1, fp );
	fread ( &ct[0], len, 1, fp );
	fread ( &ct[1], len, 1, fp );
	fread ( &ct[2], len, 1, fp );
	fread ( &ct[3], len, 1, fp );

	if ( i != 0 )
	{
		for ( j=0; j<3; j++ )
		{
			pfact[i].input[j]	= ct[j];
		}
		pfact[i].init	= ct[3];

	}

	printf ( "input third part data\n" );

	fread ( &istep, len, 1, fp );
	fread ( &t2, len, 1, fp );
	fread ( &t1, len, 1, fp );
	fread ( &k1, len, 1, fp );
	fread ( &k2, len, 1, fp );
	fread ( &k3, len, 1, fp );
	fread ( &k4, len, 1, fp );
	fread ( &k5, len, 1, fp );
	fread ( &k6, len, 1, fp );

	demi_istep	= istep/2.;
	am5	= t1/istep;

	printf ( "do you want a configuration recap? answer yes=7/no=9\n" );

label_2:

	fread ( &iprint, len, 1, fp );

	if ( iprint == 7 )
	{
		printf ( "block type\n" );
	}
	else if ( iprint == 9 )
	{
		goto label_1;
	}
	else
	{
		goto label_2;
	}

label_1:

	for ( i=0; i<n7; i++ )
	{
		if ( iu[i].type != 0 )
		{
			printf ( "%d, %d, %d, %d, %d, 4%f\n", i, iu[i].type, iu[i].input[0],
				iu[i].input[1], iu[i].input[2], pfact[i].input[0], pfact[i].input[1],
				pfact[i].input[2], pfact[i].init );
		}
		else
			continue;
	}


}

void	sub3280 ()
{
	int	i, i3, i5, i6, j, k, l, j1, l2, k7;
	float	p1, p2;

	for ( i3=0; i3<integrator_numb; i3++ )
	{
		i	= iv[i3];
	}
	n1	= constant_numb+2;

	i	= n_func[n1];
	i6	= iu[i].type;
	j	= iu[i].input[0];
	k	= iu[i].input[1];
	l	= iu[i].input[2];

	if ( j == 0 )
	{
		j1	= 0;
		out2	= 0.0;
	}
	else
	{
		j1	= abs ( j );
		out2	= block_output[j1];
	}

	if ( k == 0 )
	{
		k7	= 0;
		out3	= 0.0;
	}
	else
	{
		k7	= abs ( k );
		out3	= block_output[k7];
	}

	if ( l == 0 )
	{
		l2	= 0;
		out4	= 0.0;
	}
	else
	{
		l2	= abs ( l );
		out4	= block_output[l2];
	}

	p1	= pfact[i].input[0];
	p2	= pfact[i].input[1];

	if ( i6 >= 20 )
	{
		i5	= i6-19;
		if ( i5 >= 11 )
		{
			i5	= i5-10;
			switch	( i5 )
			{
			case	1:
				break;
			case	2:
				break;
			case	3:
				break;

			case	4:
			case	5:
			case	6:
			case	7:
			case	8:
				break;

			case	9:
				break;
			case	10:
				break;
			case	11:
				break;
			default:
				break;
			}
		}
		else
		{
			switch	( i5 )
			{
			case	1:
				break;
			case	2:
				break;
			case	3:
				break;
			case	4:
				break;
			case	5:
				break;
			case	6:
				break;
			case	7:
				break;
			case	8:
				break;
			case	9:
				break;
			case	10:
				break;
			default:
				break;
			}
		}
	}
	else if ( i6 >= 11 )
	{
		i5	= i6-10;
		switch	( i5 )
		{
		case	1:
			break;
		case	2:
			break;
		case	3:
			break;
		case	4:
			break;
		case	5:
			break;
		case	6:
			break;
		case	7:
			break;
		case	8:
			break;
		case	9:
			break;
		default:
			break;
		}
	}
	else
	{
		switch	( i6 )
		{
		case	1:
			break;
		case	2:
			break;
		case	3:
			break;
		case	4:
			break;
		case	5:
			break;
		case	6:
			break;
		case	7:
			break;
		case	8:
			break;
		case	9:
			break;
		case	10:
			break;
		default:
			break;
		}
	}


}

void	prog_2830 ()
{
	int	m, k8;

	ia2++;

	rtemp[ia2][0]	= block_output[k1];
	rtemp[ia2][1]	= block_output[k2];
	rtemp[ia2][2]	= block_output[k3];
	rtemp[ia2][3]	= block_output[k4];
	rtemp[ia2][4]	= block_output[k5];
	rtemp[ia2][5]	= block_output[k6];

	m2	= 0;
	m2++;

	k8	= 2;
	for ( m=0; m<integrator_numb; m++ )
	{
		switch	( ia1 )
		{
		case	1:
		case	2:
			w_iout[m]	= y_iout[m];
			y_iout[m]	= w_iout[m]+demi_istep*f_lk[m];
			break;

		case	3:
			y_iout[m]	= w_iout[m]+demi_istep*f_lk[m];
			break;

		case	4:
			y_iout[m]	= w_iout[m]+istep*f_lk[m];
			break;

		case	5:
			exit ();
			break;

		default:
			break;
		}
	}


}



void	prog_a ()
{
	int	i, i3, i4, i6, k, k8, l1, j;

	for ( i=0; i<n7; i++ )
	{
		i6	= iu[i].type;
		if ( i6 == 33 )
		{
			iu[i].type	= 12;
			iu[i].input[1]	= i;
			pfact[i].input[0]	= pfact[i].input[0]/pfact[i].input[1];
			pfact[i].input[1]	= -1.0/pfact[i].input[1];
		}
		else if ( i6 == 34 )
		{
			n7++;
			iu[n7].type	= 12;
			iu[n7].input[0]	= iu[i].input[0];
			iu[n7].input[1]	= n7;
			iu[n7].input[2]	= 0;
			pfact[n7].input[0]	= 1.0/pfact[i].input[1];
			pfact[n7].input[1]	= -pfact[n7].input[0];
			pfact[n7].input[2]	= 0.0;
			pfact[n7].init	= pfact[i].init;
			iu[i].type	= 26;
			iu[i].input[1]	= n7;
			pfact[i].input[0]	= pfact[i].input[0]/pfact[i].input[1];
			pfact[i].input[1]	= -pfact[i].input[0];
			pfact[i].input[2]	= 0.0;
			pfact[i].init	= 0.0;
		}
		else if ( i6 == 35 )
		{
			n7++;
			iu[n7].type	= 12;
			iu[n7].input[0]	= iu[i].input[0];
			iu[n7].input[1]	= n7;
			iu[n7].input[2]	= 0;
			pfact[n7].input[0]	= (pfact[i].input[0]*(1.0-pfact[i].input[1]/pfact[i].input[2]))
				/pfact[i].input[2];
			pfact[n7].input[1]	= -1.0/pfact[i].input[2];
			pfact[n7].input[2]	= 0.0;
			pfact[n7].init	= pfact[i].init;
			iu[i].type	= 26;
			iu[i].input[1]	= n7;
			pfact[i].input[0]	= pfact[i].input[0]*pfact[i].input[1]/pfact[i].input[2];
			pfact[i].input[1]	= 1.0;
			pfact[i].input[2]	= 0.0;
			pfact[i].init	= 0.0;
		}
		else if ( i6 == 36 )
		{
			n7++;
			iu[n7].type	= 12;
			iu[n7].input[0]	= iu[i].input[0];
			iu[n7].input[1]	= i;
			iu[n7].input[2]	= n7;
			pfact[n7].input[0]	= 1.0/pfact[i].input[0];
			pfact[n7].input[1]	= -pfact[n7].input[0];
			pfact[n7].input[2]	= -pfact[i].input[1]*pfact[n7].input[0];
			pfact[n7].init	= pfact[i].input[2];
			iu[i].type	= 12;
			iu[i].input[1]	= n7;
			pfact[i].input[0]	= 1.0;
			pfact[i].input[1]	= 0.0;
			pfact[i].input[2]	= 0.0;
		}
		else if ( i6 == 37 )
		{
			n7++;
			iu[n7].type	= 12;
			iu[n7].input[0]	= iu[i].input[0];
			iu[n7].input[1]	= 0;
			iu[n7].input[2]	= 0;
			pfact[n7].input[0]	= pfact[i].input[1];
			pfact[n7].input[1]	= 0.0;
			pfact[n7].input[2]	= 0.0;
			pfact[n7].init	= pfact[i].input[2];
			iu[i].type	= 26;
			iu[i].input[1]	= n7;
			pfact[i].input[0]	= 1.0;
			pfact[i].input[1]	= 0.0;
			pfact[i].input[2]	= 0.0;
		}
	}

	for ( i=0; i<n7; i++ )
	{
		i6	= iu[i].type;
		if ( i6 == 24 )
		{
			udelayer_numb++;
			delayer[udelayer_numb]	= i;
		}
		else if ( i6 == 12 )
		{
			integrator_numb++;
			iv[integrator_numb]	= i;
			iu[i].order	= integrator_numb;
		}
		else if ( i6 == 14 )
		{
			constant_numb++;
			n_func[constant_numb]	= i;
		}
		else
		{
			continue;
		}
		iu[i].type	= -i6;
	}

	n_func[constant_numb+1]	= 76;
	block_numb	= constant_numb+1;

label_i:

	for ( i=0; i<n7; i++ )
	{
		if ( iu[i].type < 0 )
		{
			for ( k=0; k<3; k++ )
			{
				l1	= abs ( iu[i].input[k] );
				if ( l1 != 0 )
				{
					if ( udelayer_numb != 0 )
					{
						for ( j=0; j<udelayer_numb; j++ )
						{
							if ( l1 == delayer[j] ) break;
						}
					}

					if ( integrator_numb <= 0 )
					{
						printf ( "system must contain one Integrator.\n" );
label_b:
						printf ( "check data statement and restart.\n" );
						exit ();
					}
					else
					{
						for ( j=0; j<integrator_numb; j++ )
						{
							if ( l1 == iv[j] ) goto label_k;
						}

						for ( j=0; j<block_numb; j++ )
						{
							if ( l1 == n_func[j] ) goto label_k;
						}
					}
				}
label_k:
				;
			}
			block_numb++;
			n_func[block_numb]	= i;
			iu[i].type	= -iu[i].type;
			goto label_i;
		}
	}

	i4	= 0;
	for ( i=0; i<n7; i++ )
	{
		if ( iu[i].type < 0 )
		{
			i4	= 2;
			iu[i].type	= -iu[i].type;
			printf ( "sort failure block %d.\n", i );
		}
	}

	if ( i4 > 0 ) goto label_b;

	k8	= 1;

	is9	= 0;
	for ( i=0; i<n7; i++ )
	{
		i6	= iu[i].type;
		if ( i6 == 13 )
		{
			delayer_numb++;
			n_delay[delayer_numb]	= i;
			m1[delayer_numb]	= (int) ( 4*pfact[i].input[0]/demi_istep );
		}
	}

	for ( i=0; i<integrator_numb; i++ )
	{
		krf[i].fact[0]	= 0.0;
		krf[i].fact[1]	= 0.0;
		krf[i].fact[2]	= 0.0;
		krf[i].fact[3]	= 0.0;
	}

	ia2	= 0;
	ia1	= 0;

	kk	= 0;
	for ( n1=0; n1<block_numb; n1++ )
	{
		i	= n_func[n1];
		if ( i == 76 )
		{
			block_output[i]	= 0.;
		}
		else
		{
			block_output[i]	= pfact[i].input[0];
		}

	}

	for ( i3=0; i3<integrator_numb; i3++ )
	{
		i	= iv[i3];
		y_iout[i3]	= pfact[i].init;
	}

	prog_2830 ();

	if ( ia1 == 3 )
	{
		m3++;
	}
	else
	{
		block_output[75]	= block_output[75]+demi_istep;
	}

	k8	= 3;


	sub3280 ();



}





void	main ()
{

	ReadProcessData ();

	if ( kk == 0 )
	{
		prog_a ();
	}
	else
	{
	}
}