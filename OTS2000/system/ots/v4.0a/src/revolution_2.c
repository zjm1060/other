
/* calculating the revolution of the turbine under the condition that 
 * the guide vane changes its closing speed once during the transient 
 * process; and then calculating the derivative of the angular velocity
 * with resoect to time in this period. 
 * edited by LiPeng.
 * file name: revolution_2.h
 */
		

#include	<math.h>
#include	"/DTSINC/turb_constants.h"

		
#define	TAU_X_1	1-A_X/A_0
#define	TAU_X_2	1-A_X/A_G
#define	SIGMA_1	LV_PEN/(G*HD_0*T_S_1)
#define	SIGMA_2	LV_T_P/(G*HD_Z_0*T_S_1)
	 

/* calculating "tan( delta)". */
const	double	tangent_delta= Calc_Tangent_Delta ( );

	/* calculating "omega_j". */
const	double	omega_j	= Calc_Omega_J ( );

/* calculating "time_a_1", the acceleration-time constant of the unit
	in the equivalent closing time of "T_S_1". */
const	double	time_a_1	= G_D_D*REVO_0*REVO_0/(364.7563*N_0);

/* calculating "rela_j". */
double	Calc_Rela_J ( )
{
	double	temp;
	temp	= omega_j;
	temp	= temp*pow (D_NOMI, 5.0)/J_ROTA;
	return	temp;
}


const	double	rela_j	= Calc_Rela_J ( );

/* calculating "sigma_p_1" and "sigma_p_2". */
double	Calc_Sigma_P_1 ( )
{
	double	temp;
	temp	= tangent_delta;
	temp	= pow (PHAI_0*PI/180.0, 2.0)/temp;
	temp	= temp/(4.0*PI);
	temp	= temp+log(R_0);
	temp	= temp*AREA_P/(2.0*PI*L_1*H_0);
	temp	= temp+1.0;
	temp	= temp*SIGMA_1+SIGMA_2;
	return	temp;
}  //in this module, the nose angle of the scroll case(PHAI_0) should
		//be shifted from unit of degree to unit of radian.


const  double  sigma_p_1	= Calc_Sigma_P_1 ( );
const  double  sigma_p_2	= sigma_p_1*T_S_1*A_G/( T_S_2*A_0);

/* calculating "tau_n_1", the ratio of acceleration-time under the runaway 
	state in the equivalent closing time of "T_S_1".  */
double	Calc_Tau_N_1 (  )
{
	double  temp	= T_S_1;
	temp	= temp/2.0/time_a_1/(1.0+rela_j);
	temp	= temp+1.0;
	temp	= 1.15/temp;
	temp	= temp*TAU_X_1;
	return  temp;
}


const	double	tau_n_1	= Calc_Tau_N_1 (  );
const	double	time_n_1	= tau_n_1*T_S_1;
	//"time_n_1" is the specific time related to the runaway state in the 
	//equivalent closing time of "T_S_1".  

/* calculating "m_g", the specific torque at the time of "T_G".
double	Calc_M_G(  )
{
	double  temp;
	temp	= M_0*(1-T_G/time_n_1);
	return  temp;
}
const	double	m_g	= Calc_M_G(  );
 
/* calculating "time_a_2", the acceleration-time constant of the unit
	in the equivalent closing time of "T_S_2". */
double	Calc_Time_A_2 (  )
{
	double  temp;
	temp	= M_0*(1-T_G/time_n_1);
	temp	= J_ROTA*OMEGA_0/temp;
	return  temp;
}
const	double	time_a_2	= Calc_Time_A_2 (  );

/* calculating "tau_n_2", the ratio of acceleration-time under the runaway
	state in the equivalent closing time of "T_S_2". */
double	Calc_Tau_N_2 ( )
{
	double  temp	= T_S_2;
	temp	= temp/2.0/time_a_2/(1.0+rela_j);
	temp	= temp+1.0;
	temp	= 1.15/temp;
	temp	= temp*TAU_X_2;
	return  temp;
}
const	double	tau_n_2	= Calc_Tau_N_2 (  );
const	double	time_n_2	= tau_n_2*T_S_2;
	//"time_n_2" is the specific time related to the runaway state in the
	//equivalent closing time of "T_S_2".
	
/* calculating the revolution during the lag time( T_C ).  */
double	Calc_Rota_Speed_C ( double time )
{	 
	double	temp;
	temp	= time/time_a_1/(1.0+rela_j);
	temp	= temp+1.0;
	temp	= temp*REVO_0;
	return	temp;
}
const	double	n_c_m	= Calc_Rota_Speed_C ( T_C );
	//"n_c_m" is the revolution at the time of "T_C".

/* calculating the revolution after the lag time( T_C ) to the time of 
	"T_G" when the guide vane changes its closing speed. */
double	Calc_Rota_Speed_1 ( double time )
{
	double	sum, temp, tau_1;
	tau_1	= time/T_S_1;
	sum	= tau_1;
	temp	= tau_1*tau_1/(2.0*tau_n_1);
	sum	= sum-temp;
	temp	= 2.0*sigma_p_1*tau_1/(2.0-sigma_p_1);
	sum	= sum+temp;
	temp	= sigma_p_1*tau_1*tau_1/(2.0-sigma_p_1)/tau_n_1;
	sum	= sum-temp;
	temp	= pow ((1.0-tau_1), (2.0/sigma_p_1));
	temp	= temp*sigma_p_1*sigma_p_1/(2.0-sigma_p_1);
	sum	= sum+temp;
	temp	= pow ((1.0-tau_1), (2.0/sigma_p_1));
	temp	= temp*sigma_p_1*sigma_p_1/(2.0-sigma_p_1)/tau_n_1;
	sum	= sum-temp;
	temp	= pow ((1.0-tau_1), (2.0/sigma_p_1+1.0));
	temp	= temp*2.0*sigma_p_1*sigma_p_1/(4.0-sigma_p_1*sigma_p_1)/tau_n_1;
	sum	= sum+temp;
	temp	= 2.0*pow (sigma_p_1, 2.0)*tau_n_1-pow (sigma_p_1, 3.0)*(1.0-tau_n_1);
	temp	= temp/(4.0-sigma_p_1*sigma_p_1)/tau_n_1;
	sum	= sum-temp;
	
	temp	= sum*T_S_1/time_a_1/(1.0+rela_j);
	temp	= temp*REVO_0;
	temp	= temp+n_c_m;
	return	temp;
}
const	  double	n_1_m	= Calc_Rota_Speed_1 ( T_G );
		
/* calculating the revolution after the time of "T_G" to the time(time_n_2 )
	when the revolution reaches the maximum value.  */
double	Calc_Rota_Speed_2 ( double time )
{
	double	sum, temp, tau_2;
	tau_2	= time/T_S_2;
	sum	= tau_2;
	temp	= tau_2*tau_2/(2.0*tau_n_2 );
	sum	= sum-temp;
	temp	= 2.0*sigma_p_2*tau_2/(2.0-sigma_p_2 );
	sum	= sum+temp;
	temp	= sigma_p_2*tau_2*tau_2/(2.0-sigma_p_2 )/tau_n_2;
	sum	= sum-temp;
	temp	= pow ((1.0-tau_2 ), (2.0/sigma_p_2 ));
	temp	= temp*sigma_p_2*sigma_p_2/(2.0-sigma_p_2 );
	sum	= sum+temp;
	temp	= pow ((1.0-tau_2 ), (2.0/sigma_p_2 ));
	temp	= temp*sigma_p_2*sigma_p_2/(2.0-sigma_p_2 )/tau_n_2;
	sum	= sum-temp;
	temp	= pow ((1.0-tau_2 ), (2.0/sigma_p_2+1.0));
	temp	= temp*2.0*sigma_p_2*sigma_p_2/(4.0-sigma_p_2*sigma_p_2 )/tau_n_2;
	sum	= sum+temp;
	temp	= 2.0*pow (sigma_p_2, 2.0)*tau_n_2-pow (sigma_p_2, 3.0)*(1.0-tau_n_2 );
	temp	= temp/(4.0-sigma_p_2*sigma_p_2 )/tau_n_2;
	sum	= sum-temp;
		
	temp	= sum*T_S_2/time_a_2/(1.0+rela_j);
	temp	= temp*REVO_0;
	temp	= temp+n_1_m;
	return	temp;
}
 
/* calculating the approximate maximumm of the revolution(n_m) at the 
	time of "time_n_2". */
double	Calc_Delta_2_M ( )
{
	double	 temp;
	temp	= sigma_p_2/2.0+1.0;
	temp	= temp*time_n_2; 
	temp	= temp/2.0/time_a_2/(1.0+rela_j);
	return	 temp;
}
const	double	 delta_2_m	= Calc_Delta_2_M ( );
const	double	 n_m	= n_1_m+REVO_0*delta_2_m;

/* calculating the revolution in the remaining period when the revolution
	decreases.	*/
double	Calc_Rota_Speed_3 ( double time )
{
	double	 temp;
	temp	= time*time*(1.0+sigma_p_2 );
	temp	= temp/2.0/time_a_2/(1.0+rela_j);
	temp	= temp/time_n_2;
	temp	= temp*REVO_0;
	temp	= n_m-temp;
	return	 temp;
}

/* calculating the revolutoin in the whole transient process. */
double	Calc_Rota_Speed ( double variable_time )
{
	//approximately, the virtual parameter "variable_time" cannot
	//overpass the time of "T_C+T_G+T_S_2".
	double	temp_revo;
	if ( variable_time <= T_C )
		temp_revo	= Calc_Rota_Speed_C ( variable_time );
	else if ( variable_time <= ( T_C+T_G ))
		temp_revo	= Calc_Rota_Speed_1 ( variable_time-T_C );
	else if ( variable_time <= ( T_C+T_G+time_n_2 ))
		temp_revo	= Calc_Rota_Speed_2 ( variable_time-T_C-T_G );
	else	  
		temp_revo	= Calc_Rota_Speed_3 ( variable_time-T_C-T_G-time_n_2 );
	return	temp_revo;
}

/* calculating the derivative of the dynamic angular velocity with respect 
	to time in the corresponding time interval.  */
double	Deri_Angu_Velo_C ( double time )
{
	double	deri_var;
	deri_var	= M_0/J_ROTA/(1.0+rela_j);
	return	deri_var;
}

double	Deri_Angu_Velo_1 ( double time )
{
	double	deri_var, temp, tau_1;
	tau_1	= time/T_S_1;
	temp	= pow ((1.0-tau_1), (2.0/sigma_p_1-1.0));
	temp	= 1.0-temp;
	temp	= temp*2.0*sigma_p_1/(2.0-sigma_p_1);
	temp	= 1.0+temp;
	temp	= temp*(1.0-tau_1/tau_n_1);
	temp	= temp*M_0/J_ROTA/(1.0+rela_j);
	deri_var	= temp;
	return	deri_var;
}

double	Deri_Angu_Velo_2 ( double time )
{
	double	deri_var, temp, tau_2;
	tau_2	= time/T_S_2;
	temp	= pow ((1.0-tau_2 ), (2.0/sigma_p_2-1.0));
	temp	= 1.0-temp;
	temp	= temp*2.0*sigma_p_2/(2.0-sigma_p_2 );
	temp	= 1.0+temp;
	temp	= temp*(1.0-tau_2/tau_n_2 );
	temp	= temp*m_g/J_ROTA/(1.0+rela_j);
	deri_var	= temp;
	return	deri_var;
} 

double	Deri_Angu_Velo_3 ( double time )
{
	double	deri_var, temp;
	temp	= 2.0*sigma_p_2/(2.0-sigma_p_2 );
	temp	= 1.0+temp;
	temp	= temp*time/time_n_2;
	temp	= temp*m_g/J_ROTA/(1.0+rela_j);
	deri_var	= -temp;
	return	deri_var;
}

/* calcualting the derivative of the dynamic angular velocity with respect
	to time in the whole transient process. */
double	Deri_Angu_Velo ( double variable_time )
{
	//approximately, the virtual parameter "variable_time" cannot overpass
	//the time of "T_C+T_G+T_S_2".
	double	 temp_deri_var;
	if ( variable_time <= T_C )
		temp_deri_var	= Deri_Angu_Velo_C ( variable_time );
	else if  ( variable_time <= ( T_G+T_C ))
		temp_deri_var	= Deri_Angu_Velo_1 ( variable_time-T_C ); 
	else if  ( variable_time <= ( T_C+T_G+time_n_2 ))
		temp_deri_var	= Deri_Angu_Velo_2 ( variable_time-T_C-T_G );
	else
		temp_deri_var	= Deri_Angu_Velo_3 ( variable_time-T_C-T_G-time_n_2 );
	return temp_deri_var;
}
 

