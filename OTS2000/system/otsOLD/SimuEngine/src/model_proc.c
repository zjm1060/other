/*	model_proc.c

	This program is originated from simulog.c for continuous syetem 
	real time simulation.

	Created by wdk 2003.5.10.
*/


#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
#include "/home/ems/system/ots/SimuEngine/inc/SimuEngine_fun.h"

extern	FILE	*fp_gen, *fp_list;
extern int		count;
extern CURVE_SHM_DATA	*pCurve;		//yyp debug

int	GetNLF_IdByName ( char *nlf_name )
{
	int	i, nlf_id = -1;
	for ( i=0; i<n_nlf; i++)
	{
		if ( !strcmp ( nlfdb[i].name, nlf_name ) )
		{
			nlf_id	= i;
			break;
		}
	}
	
	if ( nlf_id == -1 )
		printf( "\nin GetNLF_IdByName: %s is not found.\n", nlf_name );
	
	return	nlf_id;
}


void	PreInsensitiveTFB ( TFB_DEF *tfb )	/* 820	*/
{
	if ( fabs (tfb->input) >= tfb->c1 )
	{
		if ( tfb->input >= tfb->c1 )
		{
			tfb->output.value	= tfb->input - tfb->c1;
		}
		else
		{
			tfb->output.value	= tfb->input + tfb->c1;
		}
	}
	else
	{
		tfb->output.value	= 0.;
	}

}


void	PreSaturedTFB ( TFB_DEF *tfb )	/* 800	*/
{
	if ( fabs(tfb->input) >= tfb->c1 )
	{
		if ( tfb->input >= tfb->c1 )
		{
			tfb->output.value	= tfb->c1;
		}
		else
		{
			tfb->output.value	= -tfb->c1;
		}
	}
	else
	{
		tfb->output.value	= tfb->input;
	}
}


void	PreGearGapTFB ( TFB_DEF *tfb )
{
	if ( tfb->input > tfb->prev_input )
	{
		if ( tfb->prev_output <= tfb->input - tfb->c1 )
		{
			tfb->output.value	= tfb->prev_output;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
		else
		{
			tfb->output.value	= tfb->input - tfb->c1;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
	}
	else if ( tfb->input < tfb->prev_input )
	{
		if ( tfb->prev_output >= tfb->input + tfb->c1 )
		{
			tfb->output.value	= tfb->prev_output;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
		else
		{
			tfb->output.value	= tfb->input + tfb->c1;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
	}
	else
	{
			tfb->output.value	= tfb->prev_output;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
	}
}


void	PreRelayTFB ( TFB_DEF *tfb )
{
	if ( fabs(tfb->input) >= 0. )
	{
		if ( tfb->input >= 0. )
		{
			if ( tfb->input >= 0. )
			{
				tfb->output.value	= tfb->c1;
			}
			else
			{
				tfb->output.value	= -tfb->c1;
			}
		}
		else
		{
			tfb->output.value	=0.;
		}
	}
}

void	PreInsensitiveRelayTFB ( TFB_DEF *tfb )
{

	if ( fabs ( tfb->input ) >= tfb->e1 )
	{
		if ( tfb->input >= tfb->e1 )
		{
			tfb->output.value	= tfb->c1;
		}
		else
		{
			tfb->output.value	= -tfb->c1;
		}
	}
	else
	{
		tfb->output.value	=0.;
	}
}


void	PreInertieRelayTFB ( TFB_DEF *tfb )
{

	if ( tfb->input > 0. )
	{
		if ( tfb->input >= tfb->e1 )
		{
			tfb->output.value	= tfb->c1;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
		else
		{
			tfb->output.value	= tfb->prev_output;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
	}
	else if ( tfb->input < 0. )
	{
		if ( tfb->input <= tfb->e1 )
		{
			tfb->output.value	= -tfb->c1;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
		else
		{
			tfb->output.value	= tfb->prev_output;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
		}
	}
	else
	{
			tfb->output.value	= tfb->prev_output;
			tfb->prev_input	= tfb->input;
			tfb->prev_output	= tfb->output.value;
	}
}


void	PreNonlinearTFB ( TFB_DEF *tfb )
{
	int	nlf_id, xn, yn, data_type;
	double	f, x0, y0;
	NLF_DEF	*nlf;
	DMS_COMMON	dms_cmm;
	//Relationship Curve  yyp
	nlf_id	= GetNLF_IdByName ( tfb->nlf_name );
	if ( nlf_id == -1 ) return;

	nlf	= &nlfdb[nlf_id];
	if ( nlf->nlf_type == N1D )
	{

		if ( GetAnaValueFromName ( nlf->xname, (float *)&x0 ) != TRUE )
		{
			printf( "This point is not Found in the database file !\n" );
			return;
		}


		x0	= 409;		/* xxxx a test value, to be removed	*/
		xn	= nlf->x.end - nlf->x.head;
		f	= BiInterpolation ( xn, x0, (&nlfdata)->x[nlf->x.head],
				(&nlfdata)->f[nlf_ptr[nlf->f.head].head] );
	}
	else if ( nlfdb[nlf_id].nlf_type == N2D )
	{
		if ( GetAnaValueFromName ( nlf->xname, (float *)&x0 ) != TRUE )
		{
			printf( "This point is not Found in the database file !\n" );
			return;
		}
		if ( GetAnaValueFromName ( nlf->yname, (float *)&y0 ) != TRUE )
		{
			printf( "This point is not Found in the database file !\n" );
			return;
		}			
		x0	= 2;		/* xxxx a test value, to be removed	*/
		y0	= 60;		/* xxxx a test value, to be removed	*/
		xn	= nlf->x.end - nlf->x.head;
		yn	= nlf->y.end - nlf->y.head;
		f	= TriInterpolation ( xn, yn, x0, y0, (&nlfdata)->x[nlf->x.head],
			(&nlfdata)->y[nlf->y.head], (&nlfdata)->f[nlf_ptr[nlf->f.head].head] );
	}

}


void	PreNonLinearityProc ( MODEL_DEF *mdl )
{
	int	i;
	TFB_DEF	*tfb;

	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		tfb	= &tfbdb[i];
		switch	( tfb->linearity )
		{
		case	LINEAR:
			break;

		case	PRE_SATURATION:
			PreSaturedTFB ( tfb );
			tfb->input	= tfb->output.value;
			break;

		case	PRE_INSENSITIVE:
			PreInsensitiveTFB ( tfb );
			tfb->input	= tfb->output.value;
			break;

		case	PRE_GEAR_GAP:
			PreGearGapTFB ( tfb );
			tfb->input	= tfb->output.value;
			break;

		case	PRE_RELAY:
			PreRelayTFB ( tfb );
			tfb->input	= tfb->output.value;
			break;

		case	PRE_INSENSITIVE_RELAY:
			PreInsensitiveRelayTFB ( tfb );
			tfb->input	= tfb->output.value;
			break;

		case	PRE_INERTIE_RELAY:
			PreInertieRelayTFB ( tfb );
			tfb->input	= tfb->output.value;
			break;

		case	PRE_NONLINEAR:
			PreNonlinearTFB ( tfb );
			break;

		default:
			break;
		}
	}

}

void	PostNonLinearityProc ( MODEL_DEF *mdl )	/* 1000	*/
{
	int	i;
	TFB_DEF	*tfb;

	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		tfb	= &tfbdb[i];
		tfb->input	= tfb->output.value;

		switch	( tfb->linearity )
		{
		case	POST_SATURATION:
			PreSaturedTFB ( tfb );
			break;

		case	POST_INSENSITIVE:
			PreInsensitiveTFB ( tfb );
			break;

		case	POST_GEAR_GAP:
			PreGearGapTFB ( tfb );
			break;

		case	POST_RELAY:
			PreRelayTFB ( tfb );
			break;

		case	POST_INSENSITIVE_RELAY:
			PreInsensitiveRelayTFB ( tfb );
			break;

		case	POST_INERTIE_RELAY:
			PreInertieRelayTFB ( tfb );
			break;

		case	POST_NONLINEAR:
			break;

		default:
			break;
		}
	}   
}


void	WriteModelOutput ( MODEL_DEF *mdl )
{
	int	i;
	TFB_DEF	*tfb;


	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		tfb	= &tfbdb[i];
		if ( tfb->output.flag == ON )
		{
			BdANAByName ( tfb->output.dbname, tfb->output.value );
		}
	}
}

void	GetTFB_Coef ( double t_step, TFB_DEF *tfb )
{
	double	a0, a1, a2, a3, a4, a5, a6, a7;

	if ( tfb->order == FT_GENERAL )
	{
		if ( tfb->a.data == 0 )
		{
			if ( tfb->d.data == 0 )
			{
				tfb->type	= INTEGRATION;
				tfb->k	= tfb->c.data/tfb->b.data;
				tfb->a.data	= 0.;
				tfb->b.data	= 0.;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
			else
			{
				tfb->type	= PROPORTIONAL_INTEGRATION;
				tfb->k	= tfb->c.data/tfb->b.data;
				tfb->a.data	= 0.;
				tfb->b.data	= tfb->d.data/tfb->c.data;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
		}
		else if ( tfb->d.data == 0 )
		{
			if ( tfb->b.data == 0 )
			{
				tfb->type	= PROPORTIONAL;
				tfb->k	= tfb->c.data/tfb->a.data;
				tfb->a.data	= 0.;
				tfb->b.data	= 0.;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
			else
			{
				tfb->type	= INERTIE;
				tfb->k	= tfb->c.data/tfb->b.data;
				tfb->a.data	= tfb->a.data/tfb->b.data;
				tfb->b.data	= 0.;
				tfb->c.data	= 0.;
				tfb->d.data	= 0.;
			}
		}
		else if ( tfb->b.data == 0 )
		{
			tfb->type	= DIFFERENTIAL;
			tfb->b.data	= tfb->d.data/tfb->a.data;
			tfb->a.data	= tfb->c.data/tfb->a.data;
			tfb->c.data	= 0.;
			tfb->d.data	= 0.;
		}
		else
		{
			tfb->type	= INERTIE_PROPORTIONAL;
			tfb->k	= tfb->d.data/tfb->b.data;
			tfb->a.data	= tfb->a.data/tfb->b.data;
			tfb->b.data	= tfb->c.data/tfb->d.data;
			tfb->c.data	= 0.;
			tfb->d.data	= 0.;
		}
	}
	else if ( tfb->order == FT_SECOND_ORDER )
	{
		tfb->type	= OSCILATION2;
		a0	= sqrt( tfb->b.data- tfb->a.data*tfb->a.data/4. );
		a1	= tfb->a.data/2.;
		a2	= tfb->a.data/(2.*a0);
		a3	= tfb->b.data/a0;
		a4	= 1/tfb->b.data;
		a5	= (tfb->b.data*t_step-tfb->a.data)/(tfb->b.data*tfb->b.data);
		a6	= exp( -a1*t_step)* sin(a0*t_step);
		a7	= exp( -a1*t_step)* cos(a0*t_step);
	}
	tfb->x1.value	= 0.;
	tfb->x2.value	= 0.;
	tfb->prev_input	= 0.;

	switch	( tfb->type )
	{
	case	INTEGRATION:
	case	PROPORTIONAL_INTEGRATION:
		tfb->e	= 1.;
		tfb->f	= tfb->k*t_step;
		tfb->g	= tfb->k*t_step*t_step/2.;
		break;

	case	PROPORTIONAL:
		tfb->e	= 0.;
		tfb->f	= tfb->k;
		tfb->g	= tfb->k*t_step;
		break;

	case	DIFFERENTIAL:
		tfb->e	= 0.;
		tfb->f	= tfb->a.data;
		tfb->g	= tfb->a.data*t_step+tfb->b.data;
		break;

	case	OSCILATION2:
		tfb->e2[0][0]	= a7+a2*a6;
		tfb->e2[0][1]	= a6/a0;
		tfb->e2[1][0]	= -a3*a6;
		tfb->e2[1][1]	= a7-a2*a6;
		tfb->f2[0]	= a4*(1.-a2*a6-a7);
		tfb->f2[1]	= a4*(a0+(a2*a2/a0))*a6;
		tfb->g2[0]	= a5+a4*a4*((a1*a1-a0*a0)*a6+2.*a0*a1*a7)/a0;
		tfb->g2[1]	= a4*(1.-(a1*a1*a1/a0+a1*a0)*a4*a6-a7);
		break;

	default:
		tfb->e	= exp ( -tfb->a.data*t_step );
		tfb->f	= tfb->k*( 1.-tfb->e )/tfb->a.data;
		tfb->g	= tfb->k*t_step/tfb->a.data+tfb->k*( tfb->e-1.)/(tfb->a.data*tfb->a.data);
		break;
	}
}

void	GetTFB_RealTimeData ( TFB_COEF_DEF *tfbc )
{
	int	nlf_id, xn, yn, data_type;
	double	f, x0, y0;
	NLF_DEF	*nlf;
	DMS_COMMON	dms_cmm;

	if ( tfbc->nlf_tag != IS_NLF )
	{
		tfbc->data	= tfbc->dtmp;
		return;
	}

	//added by yyp 2009.4.27
	if( strstr(tfbc->nlf_name, ".") )
	{
		if ( ReadEntryByNameStr ( tfbc->nlf_name, &dms_cmm ) == -1 )
		{
			printf( "%s point is not found in the database file !\n", tfbc->nlf_name );
			return;
		}

		data_type	= dms_cmm.point.data_type;	// wdk 2008.3.22

		if ( data_type == DPS_ANALOG )
		{
			tfbc->data 	= dms_cmm.var.anlg.fValue;	
		}
		else
		{
			printf( "%s point is not analog entry !\n", tfbc->nlf_name );
			return;
		}		
		return;
	}	

	nlf_id	= GetNLF_IdByName ( tfbc->nlf_name );
	if ( nlf_id == -1 ) 
	{
		printf( "NLF Function is not found in the database!\n", tfbc->nlf_name );
		return;
	}
	
	nlf	= &nlfdb[nlf_id];
	if ( nlf->nlf_type == N1D )
	{

		if ( ReadEntryByNameStr ( nlf->xname, &dms_cmm ) == -1 )
		{
			printf( "%s point is not found in the database file !\n", nlf->xname );
			return;
		}

		data_type	= dms_cmm.point.data_type;	// wdk 2008.3.22

		if ( data_type == DPS_ANALOG )
		{
			x0 	= dms_cmm.var.anlg.fValue;	
		}
		else
		{
			printf( "%s point is not analog entry !\n", nlf->xname );
			return;
		}

		xn	= nlf->x.end - nlf->x.head;
		f	= BiInterpolation ( xn, x0, (&nlfdata)->x[nlf->x.head],
			(&nlfdata)->f[nlf_ptr[nlf->f.head].head] );
		tfbc->data	= tfbc->dtmp*f;

	}
	else if ( nlfdb[nlf_id].nlf_type == N2D )
	{
		if ( ReadEntryByNameStr ( nlf->xname, &dms_cmm ) == -1 )
		{
			printf( "%s point is not found in the database file !\n", nlf->xname );
			return;
		}

		data_type	= dms_cmm.point.data_type;	//wdk 2008.3.22

		if ( data_type == DPS_ANALOG )
		{
			x0 	= dms_cmm.var.anlg.fValue;	
		}
		else
		{
			printf( "%s point is not analog entry !\n", nlf->xname );
			return;
		}

		if ( ReadEntryByNameStr ( nlf->xname, &dms_cmm ) == -1 )
		{
			printf( "%s point is not found in the database file !\n", nlf->yname );
			return;
		}
		data_type	= dms_cmm.point.data_type;	//wdk 2008.3.22

		if ( data_type == DPS_ANALOG )
		{
			y0 	= dms_cmm.var.anlg.fValue;	
		}
		else
		{
			printf( "%s point is not analog entry !\n", nlf->yname );
			return;
		}
		xn	= nlf->x.end - nlf->x.head;
		yn	= nlf->y.end - nlf->y.head;
		f	= TriInterpolation ( xn, yn, x0, y0, (&nlfdata)->x[nlf->x.head],
			(&nlfdata)->y[nlf->y.head], (&nlfdata)->f[nlf_ptr[nlf->f.head].head] );

		tfbc->data	= tfbc->dtmp*f;

	}

}



void	GetTFB_Output ( MODEL_DEF *mdl )
{
	int	i;
	double	delta;
	TFB_DEF	*tfb;
	double	t_step	= mdl->t_step;
	//printf( "\nOutput:" );
	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		tfb	= &tfbdb[i];
		delta	= ( tfb->input-tfb->prev_input)/t_step;
		tfb->prev_input	= tfb->input;

		if ( tfb->coef_type == IS_NLF )	// if nlf, calculate coefficient data   
										// in real-time if neccessary.
		{
			GetTFB_RealTimeData ( &tfb->a );
			GetTFB_RealTimeData ( &tfb->b );
			GetTFB_RealTimeData ( &tfb->c );
			GetTFB_RealTimeData ( &tfb->d );
			GetTFB_Coef ( t_step, tfb );
		}
		
		if ( tfb->order == FT_SECOND_ORDER )
		{
			//yyp ??? 2008.09.04  ¶þ½×
			tfb->x1.value	= tfb->e2[0][0]*tfb->x1.value+tfb->e2[0][1]*tfb->x2.value+ tfb->f2[0]*tfb->input+ tfb->g2[0]*delta;
			tfb->x2.value	= tfb->e2[1][0]*tfb->x1.value+tfb->e2[1][1]*tfb->x2.value +tfb->f2[1]*tfb->input +tfb->g2[1]*delta;
			tfb->output.value	= tfb->d.data*tfb->x1.value + tfb->c.data*tfb->x2.value;
		}
		else	//Ò»½×
		{
			tfb->x.value	= tfb->e * tfb->x.value + tfb->f * tfb->input + tfb->g * delta;
			
			switch	( tfb->type )
			{
			case	PROPORTIONAL_INTEGRATION:
				//key point
				tfb->output.value	= tfb->x.value + tfb->b.data *tfb->k * tfb->input + tfb->b.data * tfb->k * t_step * delta;
				break;

			case	INERTIE_PROPORTIONAL:
				tfb->output.value	= (tfb->b.data- tfb->a.data)*tfb->x.value+ tfb->k*(tfb->input+t_step*delta );
				break;
			// fix the bug for K tfb added  by yyp 2009.10.9
			case	PROPORTIONAL:
				tfb->output.value	= tfb->f * tfb->input;
				break;
			default:
				tfb->output.value	= tfb->x.value;
			}
		}
		//printf( "\t%.4f", tfb->output );
	}

}

//yyp load tfb external input 
void	GetTFB_Input ( MODEL_DEF *mdl )
{
	int	i, j, j1, data_type;
	TFB_DEF	*tfb;
	DMS_COMMON	dms_cmm;

	//printf( "\nInput:" );
	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		tfb	= &tfbdb[i];
		/* load tfb external input if an external noise, such as frequency fluctuation, power setting,
			is attached to the tfb. The input is considered as negative feedback	*/
		if ( tfb->vin.flag == ON )
		{
			tfb->input = tfb->vin.value;
		}
		else
		{
			tfb->input	= 0.;
		}
		for ( j=mdl->spare.head; j<mdl->spare.end; j++ )
		{
			// input of the tfb equal to the external input plus the output of former tfb  yyp
			if ( sparedb[j].i == i )
			{
				j1 = sparedb[j].j;
				tfb->input	= tfb->input+sparedb[j].value*tfbdb[j1].output.value;
			}
		}
	}
}


int	GetModelIdByName ( char *model_name )
{
	int	i, mdl_id = -1;
	for ( i=0; i<n_model; i++)
	{
		if ( !strcmp ( mdb[i].name, model_name ) )
		{
			mdl_id	= i;
			//printf( "in GetModelIdByName: %s is found when i= %d\n", mdb[i].name, i );
			break;
		}
	}

	if ( mdl_id == -1 )
		printf( "in GetModelIdByName: %s is not found.\n", model_name );
	
	return	mdl_id;

}

void	ActivateModelByID ( int mdl_id, int speed )
{
	int	i, j, j1, data_type;
	TFB_DEF	*tfb;
	DMS_COMMON	dms_cmm;
	MODEL_DEF	*mdl	= &mdb[mdl_id];
		
	if ( mdl_id == -1 )
	{
		return;
	}
	mdl->speed = speed;	
	if(mdl->rts == PRC_ACTIVATED)
	{
		StopModelByID(mdl_id);
		MySleep(1000);
		//return;
	}

	mdl->rts = PRC_ACTIVATED;
	mdl->time.initial	= time_cnt;
	mdl->time.t	= 0;
//	pCurve->isstop = 0;

	//added by yyp 09.02.23

	for ( i=mdl->tfb.head; i<mdl->tfb.end; i++ )
	{
		tfbdb[i].output.value = 0;
		tfbdb[i].x1.value = 0;
		tfbdb[i].x2.value = 0;
		tfbdb[i].x.value = 0;
		tfb	= &tfbdb[i];
		tfb->vin.value = 0;
		/* load tfb external input if an external noise, such as frequency fluctuation, power setting,
			is attached to the tfb. The input is considered as negative feedback	*/
		if ( tfb->vin.flag == ON )
		{
			//added by yyp 2009.7.13
			if(CheckStrIsNumeric(tfb->vin.dbname) == TRUE)
			{
				tfb->vin.value=atof(tfb->vin.dbname);
			}
			else
			{
				if ( ReadEntryByNameStr ( tfb->vin.dbname, &dms_cmm ) == -1 )
				{
					//yyp 2008.09.01
					printf (  "This point %s is not Found in the database file !\n",tfb->vin.dbname );
					continue;
				}
				data_type	= dms_cmm.point.data_type;	//wdk 2008.3.22
				if ( data_type == ANA_TYPE ||   data_type == PARAM_TYPE )
				{
					tfb->vin.value 	= dms_cmm.var.anlg.fValue;	
				}
			}
		}
		else
		{
			tfb->vin.value =0;
		}
	}

	//get current input value


	printf( "\n\tActive Model Name:\t%s\n", mdl->name );
}


void	StopModelByID ( int mdl_id )
{
	MODEL_DEF	*mdl	= &mdb[mdl_id];

	if ( mdl_id == -1 )
	{
		return;
	}
	if(mdl->rts == PRC_INACTIVATED)
	{
		return;
	}
	mdl->rts = PRC_INACTIVATED;
	printf("Stop Model %s\n",mdl->name);

}


void	ExecuteModelById ( int mdl_id )
{
	int	i;
	int	t2;
	MODEL_DEF	*mdl	= &mdb[mdl_id];
	if ( mdl->rts == PRC_INACTIVATED ) return;
	mdl->t_step = 0.01;  //yyp debug
	//yyp debug
	//printf("in model %s\n",mdl->name);
	GetTFB_Input ( mdl ); 			//get input of the tfb
	PreNonLinearityProc ( mdl );	
	GetTFB_Output ( mdl );			//key function of tfb
	PostNonLinearityProc ( mdl );

	mdl->time.t	= mdl->time.t + mdl->t_step;

	return;
}

void	ActivateModelByName ( char *name, int speed, int seq_id )
{
	int	model_id;

	model_id	= GetModelIdByName ( name );

	if ( model_id != -1 )
	{
		mdb[model_id].seq_id = seq_id;
		ActivateModelByID ( model_id, speed );
	}
}


void	StopModelByName ( char *name )
{
	int	model_id;

	model_id	= GetModelIdByName ( name );

	if ( model_id != -1 )
	{
		StopModelByID ( model_id );
	}
}


