
/*	calculating the turbine's revolution under the condition that the
 *	guide vane closes at the constant speed during the transient process;  
 *	and then, calculating the derivative of the dynamic angular velocity
 *	with repect to time in this period.    
 *	edited by LiPeng.
 *	file name: revolution_1.h
 */

#include	   "/DTSINC/turb_constants.h"


#define	TAU_X	1-A_X/A_0
#define	SIGMA_1	LV_PEN/(G*HD_0*T_S)  
#define	SIGMA_2	LV_T_P/(G*HD_Z_0*T_S)


//calculating "omega_j", the rotating inertia coefficient of the water
//stream in the area where the turbine occupies.   
double    Calc_Omega_J ( )
{    
	double  temp_1, temp_2;

	temp_1	= RELA_W_1*pow ( RELA_D_1, 4.0 );
	temp_2	= RELA_W_2*pow ( RELA_D_2, 4.0 );
	temp_1	= temp_1-temp_2;
	temp_1	= temp_1*PI*RHO/32.0;

	return  temp_1;

}
 

//calculating "OMEGA_M", the flowing inertia coefficient of the water
//stream in the area where the turbine occupies.
double  Calc_Omega_M ( )
{
	double  temp_1, temp_2;

	temp_1	= pow ( RELA_D_1,2.0 )/PSI_1;
	temp_1	= temp_1/tan ( BETA_1*PI/180.0 );
	temp_2	= pow ( RELA_D_2,2.0 )/PSI_2;
	temp_2	= temp_2/tan ( BETA_2*PI/180.0 );
	temp_1	= temp_1-temp_2;
	temp_1	= temp_1*RHO/8.0;

	return  temp_1;

}


//calculating "tan ( delta)", the tangent of the spiral angle of the scroll
//case. Approximately,"DELTA" ranges from 20 degrees to 30 degrees.
double  Calc_Tangent_Delta ( )
{
	double  temp_var;

	temp_var	= sqrt ( DIST_S_C*DIST_S_C-R_S_C*R_S_C );
	temp_var	= DIST_S_C-temp_var;
	temp_var	= temp_var*2.0*PI;
	temp_var	= temp_var+H_1*log( R_A/R_B);
	temp_var	= temp_var/H_1;
	temp_var	= temp_var/( PHAI_0*PI/180.0 );

	return  temp_var;

}


/* calculating "time_a", the acceleration-time constant of the unit. */
double  Calc_Time_a ( )
{
	double   temp;

	temp	= G_D_D*REVO_0*REVO_0/(364.7563*N_0 );

	return   temp;

}


/* calculating "rela_j". */
double  Calc_Rela_J ( )
{
	double   temp, omega_j;

	omega_j	= Calc_Omega_J ( );
	temp	= omega_j*pow ( D_NOMI, 5.0 )/J_ROTA;

	return   temp;

}
 

/* calculating "sigma_p". */
double	Calc_Sigma_P ( )
{
	double   temp, tangent_delta;

	tangent_delta	= Calc_Tangent_Delta ( );
	temp	= pow ( PHAI_0*PI/180.0, 2.0 )/tangent_delta;
	temp	= temp/(4.0*PI);
	temp	= temp+log( R_0 );
	temp	= temp*AREA_P/( 2.0*PI*L_1*H_0 );
	temp	= temp+1.0;
	temp	= temp*SIGMA_1+SIGMA_2;

	return   temp;

}	//in this module, the nose angle of the scroll case ( PHAI_0 ) should 
	//be shifted from unit of degree to unit of radian. 


/* calculating "tau_n", the ratio of acceleration-time under the 
	    runaway state.   */
double  Calc_Tau_N ( )
{
	double   temp_tau_n, rela_j, time_a;

	rela_j	= Calc_Rela_J ( );
	time_a	= Calc_Time_a ( );
	temp_tau_n	= 2.0*time_a*( 1.0+rela_j );
	temp_tau_n	= T_S/temp_tau_n;
	temp_tau_n	= temp_tau_n+1.0;
	temp_tau_n	= 1.15/temp_tau_n;
	temp_tau_n	= temp_tau_n*TAU_X;

	return   temp_tau_n;

}

//calculating "time_n", the specific time related to the runaway state.
double  Calc_Time_n ( )
{
	double   temp;

	temp	= Calc_Tau_N ( );
	temp	= temp*T_S;

	return   temp;

} 
	  
/* calculating the the revolution during the lag time ( T_C ). */
double  Calc_Rota_Speed_C ( double time )
{
	double   temp, time_a, rela_j;

	time_a	= Calc_Time_a ( );
	rela_j	= Calc_Rela_J ( );
	temp	= time/time_a;
	temp	= temp/( 1.0+rela_j );
	temp	= temp+1.0;
	temp	= temp*REVO_0;

	return   temp;

}
	
/* calculating the revolution after the lag time ( T_C ) to the time ( time_n )
	   when the revolution reaches the maximum value.  */
double  Calc_Rota_Speed_1 ( double time )
{ 
	double   sum, temp, tau, rela_j, time_a, tau_n, sigma_p, n_c_m;

	rela_j	= Calc_Rela_J ( );
	time_a	= Calc_Time_a ( );
	sigma_p	= Calc_Sigma_P ( );
	tau_n	= Calc_Tau_N ( );
	n_c_m	= Calc_Rota_Speed_C ( T_C );
	//"n_c_m" is the revolution at the time of "T_C".
	tau	= time/T_S;
	sum	= tau;
	temp	= tau*tau/( 2.0*tau_n );
	sum	= sum-temp;
	temp	= 2.0*sigma_p*tau/( 2.0-sigma_p );
	sum	= sum+temp;
	temp	= sigma_p*tau*tau/( 2.0-sigma_p )/tau_n;
	sum	= sum-temp;
	temp	= pow (( 1.0-tau), ( 2.0/sigma_p ));
	temp	= temp*sigma_p*sigma_p/( 2.0-sigma_p );
	sum	= sum+temp;
	temp	= pow (( 1.0-tau), ( 2.0/sigma_p ));
	temp	= temp*sigma_p*sigma_p/( 2.0-sigma_p )/tau_n;
	sum	= sum-temp;
	temp	= pow (( 1.0-tau), ( 2.0/sigma_p+1.0 ));
	temp	= temp*2.0*sigma_p*sigma_p/(4.0-sigma_p*sigma_p )/tau_n;
	sum	= sum+temp;
	temp	= 2.0*pow (sigma_p, 2.0 )*tau_n-pow (sigma_p, 3.0 )*( 1.0-tau_n );
	temp	= temp/(4.0-sigma_p*sigma_p )/tau_n;
	sum	= sum-temp;
			
	temp	= sum*T_S/( 1.0+rela_j )/time_a;
	temp	= temp*REVO_0;
	temp	= temp+n_c_m;

	return   temp;

}
 
/* calculating the approximate maximum of the revolution (n_m) at the 
	   time of "time_n".  */
double  Calc_N_M ( )
{
	double    temp, rela_j, time_a, sigma_p, time_n, n_c_m;

	rela_j	= Calc_Rela_J ( );
	time_a	= Calc_Time_a ( );
	sigma_p	= Calc_Sigma_P ( );
	time_n	= Calc_Time_n ( );
	n_c_m	= Calc_Rota_Speed_C ( T_C );  
	temp	= sigma_p/2.0+1.0;
	temp	= temp*time_n;
	temp	= temp/2.0/( 1.0+rela_j )/time_a;
	temp	= temp*REVO_0;
	temp	= temp+n_c_m;

	return    temp;

}


/* calculating the revolution in the remaining period when the revolution 
	   decreases.   */
double  Calc_Rota_Speed_2 ( double time )
{
	double    temp, rela_j, time_a, sigma_p, time_n, n_m;

	rela_j	= Calc_Rela_J ( );
	time_a	= Calc_Time_a ( );
	sigma_p	= Calc_Sigma_P ( );
	time_n	= Calc_Time_n ( );
	n_m	= Calc_N_M ( );
	temp	= time*time*( 1.0+sigma_p );
	temp	= temp/2.0/( 1.0+rela_j )/time_a;
	temp	= temp/time_n;
	temp	= temp*REVO_0;
	temp	= n_m-temp;

	return    temp;   

}
 
/* calculating the revolutoin in the whole transient process. */
double  Calc_Rota_Speed( double variable_time )
{
	//approximately, the virtual parameter "variable_time" cannot 
	//overpass the time of "T_C+T_S".
	double   temp_revo, time_n;

	time_n	= Calc_Time_n ( );

	if ( variable_time <= T_C )
		temp_revo	= Calc_Rota_Speed_C ( variable_time );
	else if ( variable_time <= ( time_n+T_C ))
		temp_revo	= Calc_Rota_Speed_1 ( variable_time-T_C );
	else	
		temp_revo	= Calc_Rota_Speed_2 ( variable_time-time_n-T_C );

	return temp_revo;

}
   

/* calculating the derivative of the dynamic angular velocity with respect 
	   to time in the corresponding time interval.  */	
double  Deri_Angu_Velo_C ( double time )
{
	double    temp, deri_var;

	temp	= Calc_Rela_J ( );
	deri_var	= M_0/J_ROTA/( 1.0+temp );

	return    deri_var;

}


double  Deri_Angu_Velo_1 ( double time )
{ 
	double   deri_var, temp, rela_j, sigma_p, time_n, tau;

	rela_j	= Calc_Rela_J ( );
	sigma_p	= Calc_Sigma_P ( );
	time_n	= Calc_Time_n ( );
	tau	= time/T_S;
	temp	= pow (( 1.0-tau), ( 2.0/sigma_p-1.0 ));
	temp	= 1.0-temp;
	temp	= temp*2.0*sigma_p/( 2.0-sigma_p );
	temp	= 1.0+temp;
	temp	= temp*( 1.0-time/time_n );
	temp	= temp*M_0/J_ROTA/( 1.0+rela_j );
	deri_var	= temp;

	return   deri_var;

}
	  
double  Deri_Angu_Velo_2 ( double time )
{
	double   deri_var, temp, rela_j, sigma_p, time_n;

	rela_j	= Calc_Rela_J ( );
	sigma_p	= Calc_Sigma_P ( );
	temp	= 2.0*sigma_p/( 2.0-sigma_p );
	time_n	= Calc_Time_n ( );
	temp	= 1.0+temp;
	temp	= temp*time/time_n;
	temp	= temp*M_0/J_ROTA/( 1.0+rela_j );
	deri_var	= -temp;

	return   deri_var;

}
 

/* calculating the derivative of the dynamic angular velocity with respect
	to time in the whole transient process. */
double  Deri_Angu_Velo ( double variable_time )
{
	//approximately, the virtual parameter "variable_time" cannot overpass
	//the time of "T_C+T_S".
	double    temp_deri_var, time_n;

	time_n	= Calc_Time_n ( );
	if ( variable_time <= T_C )
		temp_deri_var	= Deri_Angu_Velo_C ( variable_time );
	else if ( variable_time <= ( time_n+T_C ))
		temp_deri_var	= Deri_Angu_Velo_1 ( variable_time-T_C );
	else	 
		temp_deri_var	= Deri_Angu_Velo_2 ( variable_time-time_n-T_C );

	return temp_deri_var;
}
    



