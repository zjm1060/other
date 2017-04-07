

/* defining some macro names.
 * edited by LiPeng.
 * file name: constants.h
 */ 

#define	RESULT_FILE_NAME	"/DTSDATA/turbine_sim_result.data"


	/* some physical constants. */
#define	RHO	1000.0
		//the density of the water; unit: kilogram/cubic meter.
#define	PI	3.1416 
		//the ratio of the circumference of a circle to its diameter, which is
		//3.14159265 appmately.
#define	G	9.81
		//the acceleration of gravity; unit: meter/square meter.

		/* some dimension constants of the turbine; unit: meter. */
#define	RUNNER_DIAMETER_NOMINAL		5.5
		//the nominal diameter of the turbine. 
#define	HEIGHT_OF_GUIDE_VANE	 1.1
		//the height of the guide blade.
#define	RADIUS_OF_GUIDE_VANE_DISTRIBUTED_CIRCLE  3.2
		//the radius of the circle formed by the center of the guide vanes' group.
#define	HEIGHT_OF_STAY_VANE	  1.11
		//the height of the stay vane.
#define	RADIUS_OF_STAY_VANE_OUTER_EDGE	4.375
		//the radius of the circle at the outer edge of the stay vanes' group.
#define	RADIUS_OF_STAY_VANE_INNER_EDGE	3.65
		//the radius of the circle at the inner edge of the stay vanes' group.
#define	RADIUS_OF_SCROLL_CASE_INLET	3.75
		//the radius of the scroll case's inlet.
#define	DISTANCE_SCROLL_CASE_INLET_CENTER	 7.518
		//the distance between the center of the scroll case's inlet and the axial 
		//of the turbine.
#define	RADIUS_OF_RUNNER_OUTLET		1.5
		//the radius of the runner outlet.
#define	AREA_OF_WHEEL_OUTLET_DISCHARGE	20.0
		//the discharge area at the outlet edge of the runner.
#define	DIAMETER_OF_INLET_M_SURFACE	4.9555
		//the average diameter of the runner at the inlet of the middle flowing 
		//surface.
#define	DIAMETER_OF_OUTLET_M_SURFACE	3.0 
		//the average diameter of the runner at the outlet of the middle flowing 
		//surface.
#define	WIDTH_OF_INLET_M_SURFACE	1.675
		//the width of the discharge section at the inlet of the middle flowing 
		//surface.
#define	WIDTH_OF_OUTLET_M_SURFACE	2.122
		//the width of the discharge section at the outlet of the middle flowing 
		//surface.
#define	LENGTH_OF_PENSTOCK		322.20
		//the length of the penstock. 
#define	AREA_OF_PENSTOCK		50.6582
		//the area of the penstock.
#define	LENGTH_OF_TAIL_PIPE	  250.07 
		//the length of the tail pipe.

		/* some relative value of the dimension constants. */ 
#define	RELA_D_1		0.901
		//the relative value of "DIAMETER_OF_INLET_M_SURFACE".
#define	RELA_D_2		0.6
		//the relative value of "DIAMETER_OF_OUTLET_M_SURFACE".
#define	RELA_W_1		0.30454
		//the relative value of "WIDTH_OF_INLET_M_SURFACE".
#define	RELA_W_2		0.444425
		//the relative value of "WIDTH_OF_OUTLET_M_SURFACE".  

#define	SQUEEZING_FACTOR_INLET		0.9631
		//the blade's squeezing factor to the flow at the inlet of the middle 
		//flowing surface, which ranges from 0.95 to 1.00 approximately.
#define	SQUEEZING_FACTOR_OUTLET		0.9749
		//the blade's squeezing factor to the flow at the outlet of the middle
		//flowing surface, which also ranges from 0.95 to 1.00 approximately.

		/* some angle constants of the turbine; unit: degree. */
#define	ANGLE_OF_BLADE_INLET		60.0
		//the setting angle of the blade at the inlet of the runner, which 
		//decreases while the specific speed increases.
#define	ANGLE_OF_BLADE_OUTLET		15.0
		//the setting angle of the blade at the outlet of the runner, which 
		//ranges from 15 degrees to 20 degrees approximately.
#define	NOSE_ANGLE_SCROLL_CASE		341.0
		//the nose angle of the scroll case.

		/* some discharge constants; unit: cubic meter per second. */
#define	DISCHARGE_NOMINAL		307
		//the nominal discharge of the turbine.
#define	DISCHARGE_DESIGNED		307
		//the designed discharge of the turbine.
#define	DISCHARGE_INITIAL		307
		//the initial discharge of the transition process.  

		/* some water head constants; unit: meter. */
#define	WATER_HEAD_NOMINAL		112
		//the nominal productive head of the station.
#define	WATER_HEAD_DESIGNED		112		  
		//the designed water head of the station.
#define	WATER_HEAD_EXTERNAL_INITIAL	112
		//the initial average water head of the equipment (i.e.the external head),
		//of which the effect of the scroll case and the tail pipe is considered.
#define	WATER_HEAD_INTERNAL_INITIAL	116.45
		//the initial water head of the turbine (i.e.the internal head).

		/* some water pressure constants; unit: Newton per square meter. */
#define	WATER_PRESSURE_INITIAL
		//the initial static water pressure of the specific point in the turbine 
		//room of the water diversion system.
	  
		/* some flowing velocity constants; unit: meter per second. */
#define	VELOCITY_OF_PENSTOCK_INITIAL	6.060
		//the initial average flow velocity in the penstock.
#define	VELOCITY_OF_TAIL_PIPE_INITIAL	3.1487
		//the initial average flow velocity in the tail pipe.
#define	VELOCITY_OF_SCROLL_CASE		9.14 
		//the flow velocity at the inlet of the scroll case under designing 
		//condition, which is used to calculate "tan(DELTA)".

		/* some rotation constants. */
#define	MOMENT_OF_INERTIA		70000.0
		//the moment of inertia of the set of the turbine and the generator,
		//unit: ton*square meter.
#define	ROTATIONAL_INERTIA		MOMENT_OF_INERTIA/4.0
		//the rotational inertia of the set of the turbine and the generator
		//unit: ton*square meter.
#define	TORQUE_NOMINAL		23376.68
		//the nominal torque of the turbine, unit: kilo Newton*meter.
#define	TORQUE_INITIAL		23376.68
		//the initial torque of the transition process.
#define	ANGULAR_VELOCITY_NOMINAL	13.09
		//the nominal angular velocity of the turbine, unit: radian per second.
#define	ANGULAR_VELOCITY_INITIAL	13.09
		//the initial angular velocity of the transition process.
#define	REVOLUTION_NOMINAL		125
		//the nominal revolution of the turbine, unit: rpm. 
#define	REVOLUTION_INITIAL		125
		//the turbine's initial revolution of the transition process.

#define	OUTPUT_RATED		306000
		//the nominal output of the turbine, unit: kilowatt.
#define	OUTPUT_INITIAL		306000
		//the initial output of the turbine, unit: kilowatt.
				 
		/* some time constants of the guide vane; unit: second. */
#define	TIME_GUIDE_VANE_LAG	0.3
		//the lag time of the guide vane before it closes.
#define	TIME_GUIDE_VANE_CLOSURE	10.0
		//the whole closure time of the guide vane in the transient process,
		//during which the guide vane closes at the constant speed. 
#define	TIME_TURNING_POINT
		//the specific time when the guide vane's closing speed changes.
#define	TIME_EQUIVALENT_CLOSURE_1
		//the equivalent closure time of the guide vane before its speed changes.
#define	TIME_EQUIVALENT_CLOSURE_2
		//the equivalent closure time of the guide vane after its speed changes.
		
		/* some opening constants of the guide vane, unit: mm. */
#define	A_MAX	460.0
		//the maximum opening of the guide vane.
#define	A_0	400.0 
		//the guide vane's initial opening in the transient process.
#define	A_G
		//the specific opening in the time of "TIME_TURNING_POINT".
#define	A_N
		//the runaway opening of the guide vane.
#define	A_X	40.0
		//the no_load opening of the guide vane.
#define	A_OPM	400.0
		//the optimum opening of the guide vane.
 
#define	LV_PEN	1952.5
		//the total value of the penstock length mutiplied by the flow velocity
		//in the penstock area, it is calculated under the condition that the 
		//initial discharge equals to the nominal discharge of the turbine.
		//unit: square meter per second. 
#define	LV_T_P	1140.7
		//the total value of the tail pipe length multiplied by the flow velocity 
		//in the tail pipe area, it is calculated under the condition that the
		//initial discharge equals to the nominal discharge of the turbine.
		//unit: square meter per second.

		/* some simlified forms of the constants listed above. */
#define	D_NOMI	RUNNER_DIAMETER_NOMINAL
#define	H_0	HEIGHT_OF_GUIDE_VANE
#define	R_0	RADIUS_OF_GUIDE_VANE_DISTRIBUTED_CIRCLE
#define	H_1	HEIGHT_OF_STAY_VANE
#define	R_A	RADIUS_OF_STAY_VANE_OUTER_EDGE
#define	R_B	RADIUS_OF_STAY_VANE_INNER_EDGE  
#define	R_S_C	RADIUS_OF_SCROLL_CASE_INLET
#define	DIST_S_C	DISTANCE_SCROLL_CASE_INLET_CENTER
#define	R_2	RADIUS_OF_RUNNER_OUTLET
#define	AREA_2	AREA_OF_WHEEL_OUTLET_DISCHARGE
#define	D_1_M	DIAMETER_OF_INLET_M_SURFACE
#define	D_2_M	DIAMETER_OF_OUTLET_M_SURFACE
#define	W_1_M	WIDTH_OF_INLET_M_SURFACE
#define	W_2_M	WIDTH_OF_OUTLET_M_SURFACE 
#define	L_1	LENGTH_OF_PENSTOCK
#define	AREA_P	AREA_OF_PENSTOCK
#define	L_2	LENGTH_OF_TAIL_PIPE

#define	PSI_1	SQUEEZING_FACTOR_INLET
#define	PSI_2	SQUEEZING_FACTOR_OUTLET

#define	BETA_1	ANGLE_OF_BLADE_INLET	  
#define	BETA_2	ANGLE_OF_BLADE_OUTLET
#define	PHAI_0	NOSE_ANGLE_SCROLL_CASE

#define	Q_NOMI	DISCHARGE_NOMINAL
#define	Q_D	DISCHARGE_DESIGNED
#define	Q_0	DISCHARGE_INITIAL

#define	HD_NOMI	WATER_HEAD_NOMINAL
#define	HD_D	WATER_HEAD_DESIGNED
#define	HD_Z_0	WATER_HEAD_EXTERNAL_INITIAL
#define	HD_0	WATER_HEAD_INTERNAL_INITIAL 

#define	P_0	WATER_PRESSURE_INITIAL

#define	V_S_C	VELOCITY_OF_SCROLL_CASE  
#define	V_1_0	VELOCITY_OF_PENSTOCK_INITIAL
#define	V_2_0	VELOCITY_OF_TAIL_PIPE_INITIAL

#define	G_D_D	MOMENT_OF_INERTIA	
#define	J_ROTA	ROTATIONAL_INERTIA
#define	M_NOMI	TORQUE_NOMINAL
#define	M_0	TORQUE_INITIAL
#define	OMEGA_NOMI  ANGULAR_VELOCITY_NOMINAL 
#define	OMEGA_0	ANGULAR_VELOCITY_INITIAL
#define	REVO_NOMI	REVOLUTION_NOMINAL
#define	REVO_0	REVOLUTION_INITIAL 

#define	N_RAT	OUTPUT_RATED
#define	N_0	OUTPUT_INITIAL

#define	T_C	TIME_GUIDE_VANE_LAG
#define	T_S	TIME_GUIDE_VANE_CLOSURE
#define	T_G	TIME_TURNING_POINT
#define	T_S_1	TIME_EQUIVALENT_CLOSURE_1
#define	T_S_2	TIME_EQUIVALENT_CLOSURE_2 

	/* some macro names related to the iteration process. */
#define	STEP_NUM	105
	//the value(integer) of the steps in the iteration process.
#define	STEP_LENGTH	0.1
	//the step length of the iteration process.
#define	EPSILON_ETA_C	0.015
	//the derivation between the static efficiency.
#define	EPSILON_OMEGA_C	0.2	 
	//the derivation between the static angular velocity.
#define	MODI_FAC_ETA_C	0.05
	//the modifying factor for the static efficiency in the iteration process. 
#define	MODI_FAC_OMEGA_C	0.2
	//the modifying factor for the static angular velocity in the iteration process.

#define	ETA_M	0.925
	//the optimum efficiency of the turbine.
#define	ETA_0	1000.0*M_0*OMEGA_0/(RHO*G*Q_0*HD_Z_0)
	//the initial efficiency of the turbine.


/* var_dec.h	*/


/* declaring some variables needed in the file of "trans_calc.h" and the file 
 * of "iter_calc.c".
 * edited by LiPeng.
 * file name: var_dec.h
 */


double  alpha[STEP_NUM+1];
	//the array of the angle "alpha".
double  n_h[STEP_NUM+1];
	//the array of the revolution.
double  omega_h[STEP_NUM+1];
	//the array of the dynamic angular velocity.
double  omega_c[STEP_NUM+1];
	//the array of the static angular velocity.
double  d_omega_h_d_t[STEP_NUM+1];
	//the array of the derivative of the angular velocity
	//with respect to time.
double  eta_c[STEP_NUM+1];
	//the array of the static efficiency.
double  eta_h[STEP_NUM+1];
	//the array of the dynamic efficiency.
double  q_c[STEP_NUM+1];
	//the array of the static discharge.
double  q_h[STEP_NUM+1];
	//the array of the dynamic discharge.
double  d_q_h_d_t[STEP_NUM+1];
	//the array of the derivative of the dynamic discharge
	//with respect top time.
double  xi_p[STEP_NUM+1];
	//the array of the relative variation of the average
	//water head of the turbine.
double  m_c[STEP_NUM+1];
	//the array of the static torque.
double  m_h[STEP_NUM+1];
	//the array of the dynamic torque.
double  hd_z_p[STEP_NUM+1];
	//the array of the average dynamic water head of the
	//turbine equipment.




