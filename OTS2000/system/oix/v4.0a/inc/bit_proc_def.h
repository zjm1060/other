/* 
 * File:   _bit_proc_def.h
 * Author: chi.hailong;
 *
 * Created on 2006年8月30日, 上午10:48
    UNIX  have SYN to WINDOWS  by chi  07.05.10
    LAST UPDATA 2007.12.10
 */

#ifndef _BIT_PROC_DEF_H
#define	_BIT_PROC_DEF_H

#ifdef	__cplusplus
extern "C" {
#endif

 
#define POINT_0		0
#define	POINT_1		1		
#define	POINT_2		2
#define	POINT_3		3
#define	POINT_4		4
#define	POINT_5		5

#define	NEGA_SIGN_OFF	0
#define	NEGA_SIGN_ON	1	

#define	ERR_STATUS	-1
#define	ERR_POINT_LOC	-1

#define	PNT_CALC_YES	1
#define	PNT_CALC_NO	0	

/* following is copy from llb */


#define	ANA_LMT_NORM		0
#define	ANA_LMT_OVER_HH		1
#define	ANA_LMT_OVER_H 		2
#define	ANA_LMT_OVER_L		3
#define	ANA_LMT_OVER_LL		4
#define	ANA_LMT_BACK_H		5
#define	ANA_LMT_BACK_L		6
#define	ANA_LMT_TREND		7


#define	M_IOST_TYPE		15



#define	DEV_TYPE_RELAY		0
#define	DEV_TYPE_2BREAKER	1
#define DEV_TYPE_2VALVE		2
#define DEV_TYPE_2DEVICE	3
#define	DEV_TYPE_3BREAKER	4
#define	DEV_TYPE_3VALVE		5
#define	DEV_TYPE_3DEVICE	6
#define	DEV_TYPE_GENERATOR	7
#define DEV_TYPE_LCUMSG		8
#define	DEV_TYPE_STREAM		9
#define	DEV_TYPE_NETMSG		10
#define	DEV_TYPE_HOSTMSG	11
#define	DEV_TYPE_LOGMSG 	12
#define DEV_TYPE_ONOFFMSG	13

#define	RELAY_BACK		0
#define	RELAY_ACTION		1

#define	BREAKER2_OPEN		0
#define	BREAKER2_CLOSE		1

#define	VALVE2_CLOSE		0
#define	VALVE2_OPEN		1

#define	DEVICE2_OFF		0
#define	DEVICE2_ON		1

#define	BREAKER3_OPEN		0
#define	BREAKER3_CLOSE		1
#define	BREAKER3_TRANSIT	2
#define	BREAKER3_ERR		3

#define	VALVE3_CLOSE		0
#define	VALVE3_OPEN		1
#define	VALVE3_TRANSIT		2
#define	VALVE3_ERR		3


#define	DEVICE3_OFF		0
#define	DEVICE3_ON		1
#define	DEVICE3_TRANSIT		2
#define	DEVICE3_ERR		3

#define LCUMSG_NORMAL           0
#define LCUMSG_FAILURE          1


/*	bits definition for IOST	*/
#define BitS_POINT_NUM_DSP	0	/* added for v3.0 */
#define	BitN_POINT_NUM_DSP	3

#define	BitS_SCAN	5
#define	BitN_SCAN	1

#define	BitS_ALM	6
#define	BitN_ALM	1

#define	BitS_ACK	7
#define	BitN_ACK	1

#define	BitS_TAG	8	/* bits 8-9 are used as tag. If tag is on, any control is prohibited. */
#define	BitN_TAG	2

#define	IOST_TAG_OFF	0
#define	IOST_TAG_ON	1


#define	ENABLE		1
#define	DISABLE		0

#define	DQ_NORM		0
#define	DQ_MAN_SET	1
#define	DQ_FAULT	2

#define	ACKN	0
#define	UNACKN	1		

/*	bits definition for IOATTR	*/
#define	BitS_TYPE	0
#define	BitN_TYPE	5	/* v3.0 modified 4 to 5 */


#define	BitS_AUDIO	14	/* v3.0 modified 4 to 14 */
#define	BitN_AUDIO	1
/*
#define	BitS_MIMIC	5
#define	BitN_MIMIC	1
*/
#define	BitS_HDB	5
#define	BitN_HDB	1

#define	BitS_CALC	6
#define BitN_CALC	1

#define BitS_SGCZ       7
#define	BitN_SGCZ	1


#define BitS_KEY	8
#define BitN_KEY	1

#define	BitS_CTRL	9	
#define	BitN_CTRL	1

#define	BitS_SBO	10	
#define	BitN_SBO	1

#define	BitS_Trnd	11
#define	BitN_Trnd	1

#define BitS_Cascade       12
#define BitN_Cascade       1

#define BitS_SGZY	13
#define BitN_SGZY	1

#define BitS_REVS	15	/* v3.0 added for define reverse point */
#define BitN_REVS	1

/*	bits    definition for STATUS	*/
#define	BitS_DQ		0
#define	BitN_DQ		2

#define	BitS_STATUS	2
#define	BitN_STATUS	4	

#define	BitS_FAULT	7
#define	BitN_FAULT 	1

#define	BitS_INT_STATIC_TYPE	8	
#define	BitN_INT_STATIC_TYPE	3	


/*	bits definition for Analog Data	*/
#define	BitS_ANA_LMT	2
#define	BitN_ANA_LMT	3

#define	VALID_NUM 	6	
#define	IMP_VALID_NUM	6
#define	BitS_ANA_POINT	5
#define	BitN_ANA_POINT	2

#define	BitS_ANA_SIGN	7
#define	BitN_ANA_SIGN	1

/*	bits definition for Analog Limt Set */
#define	BitS_ANA_LMT_TYPE	2
#define	BitN_ANA_LMT_TYPE	3

/*	bits definition for Imp  Limt Set */
#define	BitS_IMP_LMT_POINT	4	
#define	BitN_IMP_LMT_POINT	3	
#define BitS_IMP_LMT_SIGN	7	
#define BitN_IMP_LMT_SIGN 	1
#define	BitS_IMP_LMT_TYPE	8 
#define	BitN_IMP_LMT_TYPE	4	



/*      bits definition for Set CMD  v3.0  */

#define	BitS_SetParamType	0	
#define	BitN_SetParamType	5	/* v3.0 modified from 4 to 5	*/


/*	bits definition for	command   	*/ 
#define BitS_AGC_TYPE   8
#define BitN_AGC_TYPE   2
#define AGCML           3

#define	BitS_CMD_TYPE	4	
#define	BitN_CMD_TYPE	6	/* v3.0 modified from 4 to 6 */

#define BitS_CMD_SOURCE		10	/* v3.0 modified from 8 to 10 */
#define BitN_CMD_SOURCE 	3	/* v3.0 modified from 2 to 3 */

#define BitS_IOST_EN_DEN        5
#define BitN_IOST_EN_DEN        2   /* 2000.1.6 */

#define CTRL_YES	1
#define	CTRL_NO		0

/* bits definition for Analog Point and Sign in Command */
#define BitS_CMD_ANA_POINT      1
#define BitN_CMD_ANA_POINT      2

#define BitS_CMD_ANA_SIGN       0
#define BitN_CMD_ANA_SIGN       1

#define	LCU_NUM		17

	extern	char	*scan_str[2];
	extern	char	*alm_str[2];
	extern	char	*ack_str[2];
	extern	char	*dq_str[4];
	extern	char	*audio_str[2];
	extern	char	*mimic_str[2];

typedef struct
{
	POINTER		ptr;
	short	outaddr;
	short	rt_status;
	short	pointer_loc;
	short	nega_sign;
	float	rt_value;
}	ANA_DATA;

typedef struct
{
	short		ctrl_pnt_type;
	POINTER		ptr;
	short	outaddr;
	short	obj_status;
}	INT_DATA;

typedef struct
{
		short	data_1;
	char	data_2;
	char	data_3;
}	TWO_SHORT;


typedef struct
{
		short	status_part;
	short	data_part;
}	H9000_DATA;

typedef union
{
		TWO_SHORT	sht;
	int		int_data;
}	SHORT_TO_INT;		/* this is write for impulse command data */

SHORT_TO_INT	shtint_data;

typedef union
{
		H9000_DATA	stand_data;
	float		float_data;
}	SHORT_TO_FLOAT;			/* this is write for impulse command data */SHORT_TO_FLOAT	shtfloat_data;



#ifdef	__cplusplus
}
#endif

#endif
