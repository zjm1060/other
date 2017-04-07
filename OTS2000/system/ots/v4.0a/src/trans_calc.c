
/* calculating the variables( discharge, water head, guide vane opening,
 * torque, revolution, efficiency, etc) during the transient process.
 * edited by LiPeng.
 * file name: trans_calc.h
 */

#include	"/DTSINC/turb_constants.h"

typedef   double   (*FUNC_PTR)( double , double );



//calculating alpha.
double	Calc_Alpha ( double time )
{
	double    temp_1, temp_2;
	double    alpha_0[7]	= {0, 10.00, 23.75, 36.67, 55.00, 67.50, 85.00};
	double    perc_a_0[7]	= {0, 0.2, 0.4, 0.6, 0.8, 0.9, 1.0}; 

	//the relative opening of the guide vane expressed by percentage.  
	temp_1	= Calc_Opening ( time );
	temp_1	= temp_1/A_MAX;
	if ( temp_1 <= perc_a_0[1] )
	{
		temp_2	= ( alpha_0[1]-alpha_0[0] )/(perc_a_0[1]-perc_a_0[0] );
		temp_2	= temp_2*( temp_1-perc_a_0[0] );
		temp_2	= temp_2+alpha_0[0];
		temp_2	= temp_2*PI/180.0;
	}
	else if ( temp_1 <= perc_a_0[2] )
	{
		temp_2	= ( alpha_0[2]-alpha_0[1] )/(perc_a_0[2]-perc_a_0[1] );
		temp_2	= temp_2*( temp_1-perc_a_0[1] );
		temp_2	= temp_2+alpha_0[1];
		temp_2	= temp_2*PI/180.0;
	}
	else if ( temp_1 <= perc_a_0[3] )
	{ 
		temp_2	= ( alpha_0[3]-alpha_0[2] )/(perc_a_0[3]-perc_a_0[2] );
		temp_2	= temp_2*( temp_1-perc_a_0[2] );
		temp_2	= temp_2+alpha_0[2];
		temp_2	= temp_2*PI/180.0;
	}
	else if ( temp_1 <= perc_a_0[4] )
	{
		temp_2	= ( alpha_0[4]-alpha_0[3] )/(perc_a_0[4]-perc_a_0[3] );
		temp_2	= temp_2*( temp_1-perc_a_0[3] );
		temp_2	= temp_2+alpha_0[3];
		temp_2	= temp_2*PI/180.0;
	}
	else if ( temp_1 <= perc_a_0[5] )
	{ 
		temp_2	= ( alpha_0[5]-alpha_0[4] )/(perc_a_0[5]-perc_a_0[4] );
		temp_2	= temp_2*( temp_1-perc_a_0[4] );
		temp_2	= temp_2+alpha_0[4];
		temp_2	= temp_2*PI/180.0;
	}     
	else
	{    
		temp_2	= ( alpha_0[6]-alpha_0[5] )/(perc_a_0[6]-perc_a_0[5] );
		temp_2	= temp_2*( temp_1-perc_a_0[5] );
		temp_2	= temp_2+alpha_0[5];
		temp_2	= temp_2*PI/180.0;
	}

	return    temp_2;

}   


/* calculating the static discharge. */
double    Calc_Q_c( double  time, int i )
{
	double    temp_1, temp_2, alpha, var_eta_c, var_omega_c;

	alpha	= Calc_Alpha ( time );   
	var_eta_c	=eta_c[i];
	var_omega_c	=omega_c[i];
	temp_1	= var_eta_c*G*HD_Z_0/var_omega_c;
	temp_2	= var_omega_c*pow ( R_2, 2.0 );
	temp_1	= temp_1+temp_2;
	temp_2	= R_2/AREA_2/tan ( BETA_2*PI/180.0 );
	temp_2	= temp_2+1.0/(2.0*PI*H_0 )/tan ( alpha );
	temp_1	= temp_1/temp_2;

	return    temp_1;

}


/* defining the expression of the derivative of the dynamic discharge
	with respect to time, which can be used in the following module of
	Runga_kutta function. "var_q_h" defines the absolute value of the 
	dynamic discharge, not the relative value of the dynamic discharge. */
double  Deri_Q_h_D_t ( double var_q_h, double time, int i )
{
	double    var_q_c, temp, sigma_p;

	var_q_c	= Calc_Q_c( time, i );
	sigma_p	= Calc_Sigma_P( );
	temp	= var_q_h/var_q_c;
	temp	= pow ( temp, 2.0 );
	temp	= temp-1.0;
	temp	= temp*Q_0/(T_S*sigma_p);
	temp	=-temp;

	return    temp;

}

/* calculating the dynamic discharge by solving the differential equation 
	according to the Runge-Kutta solution.      The identifier "variation"
	represents the step length of the iteration calculation. */  
double	Func_Runge_Kutta ( double var_q_h, double time, int i, double variation )
{
	double   temp_var_1, temp_var_2, temp_var_3, temp_var_4, temp_var_5;
	double   temp_dep_var,   temp_indep_var;
		
	temp_dep_var	= var_q_h;
	temp_indep_var	= time;
	temp_var_1	= Deri_Q_h_D_t ( temp_dep_var, temp_indep_var, i );
	temp_var_1	= temp_var_1*variation;
	       
	temp_dep_var	= var_q_h+temp_var_1/2.0;
	temp_indep_var	= time+variation/2.0;
	temp_var_2	= Deri_Q_h_D_t ( temp_dep_var, temp_indep_var, i );
	temp_var_2	= temp_var_2*variation;
	       
	temp_dep_var	= var_q_h+temp_var_2/2.0;
	temp_indep_var	= time+variation/2.0;
	temp_var_3	= Deri_Q_h_D_t ( temp_dep_var, temp_indep_var, i );
	temp_var_3	= temp_var_3*variation;
	      
	temp_dep_var	= var_q_h+temp_var_3;
	temp_indep_var	= time+variation;
	temp_var_4	= Deri_Q_h_D_t ( temp_dep_var, temp_indep_var, i );
	temp_var_4	= temp_var_4*variation;
	    
	temp_var_5	= temp_var_1+2.0*( temp_var_2+temp_var_3)+temp_var_4;
	temp_var_5	= temp_var_5/6.0;
	temp_var_5	= temp_var_5+var_q_h;

	return   temp_var_5;

}   
	  /* calculating the relative variation of the average water head of
	     the equipment. */
double    Calc_Xi_p( int i )
{
	double  temp;

	temp	= q_h[i]/q_c[i];
	temp	= pow ( temp, 2.0 );
	temp	= temp-1.0;

	return  temp;

}
	  //calculating the static torque.
double    Calc_M_c( int i )
{
	double   temp;

	temp	= R_2/AREA_2/tan ( BETA_2*PI/180.0 );
	temp	= temp+1.0/(2.0*PI*H_0 )/tan ( alpha[i] );
	temp	= temp*q_c[i];
	temp	= temp-omega_c[i]*pow ( R_2, 2.0 );
	temp	= temp*RHO*q_c[i];

	return   temp;

}
	  //calculating the dynamic torque.
double    Calc_M_h ( int i )
{
	double    temp, temp_deri;

	temp	= xi_p[i]+1.0;
	temp	= temp*m_c[i];
	/*   temp_deri	= d_q_h_d_t[i];
	temp_deri	= temp_deri*Calc_Omega_M ( );
	temp_deri	= temp_deri*pow ( D_NOMI, 2.0 );
	temp	= temp+temp_deri;     */
	temp_deri	= d_omega_h_d_t[i];
	temp_deri	= temp_deri*Calc_Omega_J ( );
	temp_deri	= temp_deri*pow ( D_NOMI, 5.0 );
	temp	= temp-temp_deri;      

	return    temp;

}





  
