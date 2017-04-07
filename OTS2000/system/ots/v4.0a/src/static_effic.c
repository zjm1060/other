
/* calculating the static efficiency of the turine during the transient
 * process.
 * edited by LiPeng.
 * file name: static_effic.h
 */

#include	"/DTSINC/turb_constants.h"

#define	 M		 0.3
//a factor, which ranges from 0.15 to 0.35 for Fransis turbine, and from
//0.25 to 0.40 for Kplan turbine.

#define	 N		 0.7
//another factor, which ranges from 0.85 to 0.65 for Fransis turbine, and
//from 0.75 to 0.60 for Kplan turbine; the value of "N" increases while
//the specific revolution increases; and another equation should also be
//satisfied: M+N= 1.0

/* calculating the opening of the guide vane. */
double	Calc_Opening ( double time )
{
	double	temp;
	if ( time <= T_C )
		temp	= A_0;
	else	
		temp	= (T_S+T_C-time )*A_0/T_S;
	return	temp;
}

/* calculating the partial derivative of the efficiency with respect to
	the unit revolution under the condition that the opening of the guide
	vane equals to the optimum opening; to calculate the partial derivative,
	the matrix of efficiency(effi[2]) and unit revolution(unit_revo[2])
	are introduced in this module.  */  
double	Calc_Partial_Deri ( )
{
	double	temp;
	double	effi[2]={0.88, 0.76};
	double	unit_revo[2]={80.0, 95.0}; 
	temp	= (effi[0]-effi[1])/(unit_revo[0]-unit_revo[1]);
	temp	= fabs( temp);
	return	temp;
}			
					
/* calculating the static efficiency.  */		
double	Calc_Eta_c ( double	time )
{
	double	temp, temp_1, opening, omega_h;
	double	partial_deri;
	opening	= Calc_Opening ( time );	 //calculating the opening.
	temp	= Calc_Rota_Speed ( time );	 //calculating the revolution.
	omega_h	= temp*PI/30.0;	//calculating the dynamic angular velocity.
	partial_deri	= Calc_Partial_Deri ( );
	temp_1	= (A_OPM-opening)/(A_OPM-A_X);
	temp	= 1.0-M*pow( temp_1, 2.0 )-N*pow( temp_1, 4.0 );
	temp	= temp*ETA_0;
	//here, "temp" expresses the static efficiency on the condition
	//that the revolution(or the angular velocity) keeps constant.
	temp_1	= omega_h-OMEGA_0;
	temp_1	= temp_1*30.0/PI;
	temp_1	= temp_1*opening/A_OPM;
	temp_1	= temp_1*D_NOMI/sqrt (HD_Z_0 );
	temp_1	= temp_1*partial_deri;
	temp	= temp-temp_1;
	return	temp;
}



