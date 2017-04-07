
/*
 * 	Calculating all the variables related to the transient process.
 * 	edited by LiPeng.
 * 	file name: iter_calc_1.c
 *
 */

#include	<math.h>    
#include	<stdio.h>
#include	</DTSINC/turb_constants.h>



main ( )
{
	int     i;
	int     outer_var, inner_var;
		//the controlling variable of the outer and inner iteration.
	double  temp_eta_c, temp_omega_c;
		//the temporary variables representing the static efficiency
		//and the static angular velocity in the iteration process.
 
	FILE    *fp;
	if (( fp = fopen ( RESULT_FILE_NAME, "wb" )) == NULL )
	{
		printf ( "cannot open this file\n" );
		return;
	}
     
	/* calculating the angle of "alpha". */
	for ( i=0; i<=STEP_NUM; i++ )
	{
		alpha[i] = Calc_Alpha ( i*STEP_LENGTH );
	}

	/* calculating the revolution. */
	for ( i=0; i<=STEP_NUM; i++ )
	{
		n_h[i] = Calc_Rota_Speed( i*STEP_LENGTH );
	}

	/* calculating the dynamic angular velocity. */
	for ( i=0; i<=STEP_NUM; i++ )
	{
		omega_h[i] =PI*n_h[i]/30.0;
	}

	/* calculating the derivative of the dynamic angular velocity with
		respect to time. */
	for ( i=0; i<=STEP_NUM; i++ )
	{
		d_omega_h_d_t[i] = Deri_Angu_Velo( i*STEP_LENGTH );
	}

	/* printing the array of alpha[STEP_NUM+1], n_h[STEP_NUM+1], 
		omega_h[STEP_NUM+1], d_omega_h_d_t[STEP_NUM+1].  */
	fprintf ( fp, "\n" );
	fprintf ( fp, "the array of alpha[], n_h[], omega_h[], d_omega_h_d_t[].\n" );

	for ( i=0; i<=STEP_NUM; i++ )
	{
		fprintf ( fp, "i= %d;\n", i);
		fprintf ( fp, "alpha[%d] = %f degree, n_h[%d] = %f rpm\n",
			i, alpha[i]*180.0/PI, i, n_h[i] );
		fprintf ( fp, "omega_h[%d] = %f rad/s, d_omega_h_d_t[%d] = %f rad/ss\n",
			i, omega_h[i], i, d_omega_h_d_t[i] );
		fprintf ( fp, "\n" );
	}
  
	/* setting some initial values. */
	omega_c[0] = OMEGA_0;	 //initial value of the static angular velocity.
	eta_c[0] = ETA_0;	  //initial valua of the static efficiency.
	eta_h[0] = ETA_0;	  //initial value of the dynamic efficiency.
	q_c[0] = Calc_Q_c ( 0.0, 0 );  //initial value of the static discharge.
	q_h[0] = Calc_Q_c ( 0.0, 0 );  //initial value of the dynamic discharge.
	d_q_h_d_t[0] = Deri_Q_h_D_t ( q_h[0], 0.0, 0 ); 
		//initial value of the derivative of the dynamic discharge.
	xi_p[0] = Calc_Xi_p ( 0 );     
		//initial value of the relative variation of the average water head.
	m_c[0] = Calc_M_c ( 0 );	 //initial value of the static torque.
	m_h[0] = Calc_M_h ( 0 );	 //initial value of the dynamic torque.
	hd_z_p[0] = HD_Z_0;	
		//initial value of the dynamic water head of the turbine equipment.
  
	fprintf ( fp, "outer_var=0;\n" );
	fprintf ( fp, "omega_c[0] = %f rad/s\n", omega_c[0] );
	fprintf ( fp, "eta_c[0] = %f, eta_h[0] = %f\n", eta_c[0], eta_h[0] );
	fprintf ( fp, "q_c[0] = %f mmm/s, q_h[0] = %f mmm/s, d_q_h_d_t[0] = %f mmm/ss\n", 
		q_c[0], q_h[0], d_q_h_d_t[0] );
	fprintf ( fp, "xi_p[0] = %f, hd_z_p[0] = %f m\n", xi_p[0], hd_z_p[0] );
	fprintf ( fp, "m_c[0] = %f kN*m, m_h[0] = %f kN*m\n", m_c[0]/1000.0, m_h[0]/1000.0 );
	fprintf ( fp, "\n" );

	/* the outer iteration calculation of the transition process. */
	for ( outer_var=1; outer_var<=STEP_NUM; outer_var++ )
	{  
		eta_c[outer_var] = Calc_Eta_c ( outer_var*STEP_LENGTH );
		omega_c[outer_var] = omega_c[outer_var-1];
		q_c[outer_var] = Calc_Q_c ( outer_var*STEP_LENGTH, outer_var );
		q_h[outer_var] = Func_Runge_Kutta ( q_h[outer_var-1], ( outer_var-1)*STEP_LENGTH,
					   ( outer_var-1), STEP_LENGTH );
		d_q_h_d_t[outer_var] = Deri_Q_h_D_t ( q_h[outer_var], outer_var*STEP_LENGTH, outer_var );
		xi_p[outer_var] = Calc_Xi_p ( outer_var );
		m_c[outer_var] = Calc_M_c ( outer_var );
		m_h[outer_var] = Calc_M_h ( outer_var );
		temp_omega_c = omega_h[outer_var]/sqrt (1.0+xi_p[outer_var] );
		temp_eta_c = temp_omega_c*m_c[outer_var]/( RHO*G*HD_Z_0*q_c[outer_var] );
		
		fprintf ( fp, "outer_var= %d;\n", outer_var );
		fprintf ( fp, "omega_c[%d] = %f rad/s\n", outer_var, omega_c[outer_var] );
		fprintf ( fp, "eta_c[%d] = %f\n", outer_var, eta_c[outer_var] );
		fprintf ( fp, "q_c[%d] = %f mmm/s, q_h[%d] = %f mmm/s, d_q_h_d_t[%d] = %f mmm/ss\n", 
			outer_var, q_c[outer_var], outer_var, q_h[outer_var],
			outer_var, d_q_h_d_t[outer_var] );

		fprintf ( fp, "xi_p[%d] = %f\n", outer_var,xi_p[outer_var] );
		fprintf ( fp, "m_c[%d] = %f kN*m, m_h[%d] = %f kN*m\n", outer_var,
			m_c[outer_var]/1000.0, outer_var, m_h[outer_var]/1000.0 );
		fprintf ( fp, "temp_omega_c = %f rad/s\n", temp_omega_c );
		fprintf ( fp, "temp_eta_c = %f\n", temp_eta_c );
    
		/* in the inner iteration, the array of "n_h[outer_var], omega_h[outer_var], 
		d_omega_h_d_t[outer_var]"are not modified.  */
		if   (eta_c[outer_var]>=0.0 )
		{     
			inner_var=1;	  //setting the initial value of "inner_var".
			while (( fabs ( temp_eta_c-eta_c[outer_var] ) >= EPSILON_ETA_C )
				||( fabs ( temp_omega_c-omega_c[outer_var] ) >= EPSILON_OMEGA_C )) 
			{
				if ( fabs ( temp_eta_c-eta_c[outer_var] ) >= EPSILON_ETA_C )  
				    eta_c[outer_var] = (1.0-MODI_FAC_ETA_C )*eta_c[outer_var]
						+MODI_FAC_ETA_C*temp_eta_c;
				if ( fabs ( temp_omega_c-omega_c[outer_var] ) >= EPSILON_OMEGA_C )
					omega_c[outer_var] = (1.0-MODI_FAC_OMEGA_C )*omega_c[outer_var]
						+MODI_FAC_OMEGA_C*temp_omega_c;    

				q_c[outer_var] = Calc_Q_c ( outer_var*STEP_LENGTH, outer_var );
				q_h[outer_var] = Func_Runge_Kutta ( q_h[outer_var-1], ( outer_var-1)*STEP_LENGTH,
						( outer_var-1), STEP_LENGTH );
				d_q_h_d_t[outer_var] = Deri_Q_h_D_t ( q_h[outer_var], outer_var*STEP_LENGTH, outer_var );
				xi_p[outer_var] = Calc_Xi_p ( outer_var );
				m_c[outer_var] = Calc_M_c ( outer_var );
				m_h[outer_var] = Calc_M_h ( outer_var );
				temp_omega_c = omega_h[outer_var]/sqrt (1.0+xi_p[outer_var] );
				temp_eta_c = temp_omega_c*m_c[outer_var]/( RHO*G*HD_Z_0*q_c[outer_var] );
				fprintf ( fp, "*** the followings are modified by the inner iteration of No.%d: ****\n",
				inner_var );
				fprintf ( fp, "inner_var= %d;\n", inner_var );
				fprintf ( fp, "omega_c[%d] = %f rad/s, temp_omega_c = %f rad/s\n", outer_var, omega_c[outer_var],
				temp_omega_c );
				fprintf ( fp, "eta_c[%d] = %f, temp_eta_c = %f\n", outer_var, eta_c[outer_var],temp_eta_c ); 
				fprintf ( fp, "q_c[%d] = %f mmm/s, q_h[%d] = %f mmm/s, d_q_h_d_t[%d] = %f mmm/ss\n", outer_var,
				q_c[outer_var], outer_var, q_h[outer_var], outer_var, d_q_h_d_t[outer_var] ); 
				fprintf ( fp, "xi_p[%d] = %f\n", outer_var,xi_p[outer_var] );
				fprintf ( fp, "m_c[%d] = %f kN*m, m_h[%d] = %f kN*m\n", outer_var, m_c[outer_var]/1000.0,
				outer_var, m_h[outer_var]/1000.0 );
				inner_var++;    //calculating the times of the inner iteration.
			}  //the ending line of the inner iteration of "while" expression.
		}  //the ending line of the "if"expresion.

		hd_z_p[outer_var] = HD_Z_0*(1.0+xi_p[outer_var] );
		eta_h[outer_var] = m_h[outer_var]*omega_h[outer_var]/( RHO*G*hd_z_p[outer_var]*q_h[outer_var] );

		/* ### attention: if the static efficiency is negative (eta_c[outer_var]<0.0 ), the inner  
		iteration about the static efficiency and the static angular velocity is omitted. ###  */

		if ( inner_var >=2 )
		{
		     fprintf ( fp, "### after being modified by the inner iteration, ###\n" );
		     fprintf ( fp, "### the variables are listed as follows:	###\n" );
		}
		else
		     fprintf ( fp, "### the variables are not modified by the inner iteration! ###\n" );

		fprintf ( fp, "omega_c[%d] = %f rad/s\n", outer_var, omega_c[outer_var] );
		fprintf ( fp, "eta_c[%d] = %f, eta_h[%d] = %f\n", outer_var, eta_c[outer_var],
			outer_var, eta_h[outer_var] );
		fprintf ( fp, "q_c[%d] = %f mmm/s, q_h[%d] = %f mmm/s, d_q_h_d_t[%d] = %f mmm/ss\n",
			outer_var, q_c[outer_var], outer_var, q_h[outer_var],
			outer_var, d_q_h_d_t[outer_var] );
		fprintf ( fp, "xi_p[%d] = %f, hd_z_p[%d] = %f m\n", outer_var,
			xi_p[outer_var], outer_var,hd_z_p[outer_var] );
		fprintf ( fp, "m_c[%d] = %f kN*m, m_h[%d] = %f kN*m\n", outer_var,
			m_c[outer_var]/1000.0, outer_var, m_h[outer_var]/1000.0 );
		fprintf ( fp, "\n" );    
	}   //the ending line of the outer iteration.
	   
	/* printing all the variables appeared in this program. */
	for ( i=0; i<=STEP_NUM; i++ )
	{
		fprintf ( fp, "i= %d, time= %f s;\n", i, i*STEP_LENGTH );  
		fprintf ( fp, "alpha[%d] = %f degree\n", i, alpha[i]*180.0/PI );
		fprintf ( fp, "n_h[%d] = %f rpm, omega_h[%d] = %f rad/s, d_omega_h_d_t[%d] = %f rad/ss\n",
			    i, n_h[i], i, omega_h[i], i, d_omega_h_d_t[i] );
		fprintf ( fp, "omega_c[%d] = %f rad/s\n", i, omega_c[i] );
		fprintf ( fp, "m_c[%d] = %f kN*m, m_h[%d] = %f kN*m\n", i, m_c[i]/1000.0, i, m_h[i]/1000.0 );
		fprintf ( fp, "q_c[%d] = %f mmm/s, q_h[%d] = %f mmm/s, d_q_h_d_t[%d] = %f mmm/ss\n",
			    i, q_c[i], i, q_h[i], i, d_q_h_d_t[i] );
		fprintf ( fp, "xi_p[%d] = %f, hd_z_p[%d] = %f m\n", i, xi_p[i], i, hd_z_p[i] );
		fprintf ( fp, "eta_c[%d] = %f, eta_h[%d] = %f\n", i, eta_c[i], i, eta_h[i] );
	}

	fclose ( fp );

}	//the ending line of the main function.



