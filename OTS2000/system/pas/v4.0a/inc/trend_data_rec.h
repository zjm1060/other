/*	trend_data_rec.h head file for trend_data_rec */

#define DATA_NUM		(24*3600)	/*One day in second*/
#define MAX_CURVE_NUM		60
#define CURVE_NAME_LONG		80
#define CYCLE			1


typedef struct
{
	POINTER		buffer[MAX_CURVE_NUM];
	SHORT		curve_num;
}CURVE_DEF;

typedef struct
{
	int		nextpt;
	SHORT		reloadflag;
	SHORT		overlapflag;
	SHORT		isstop;
	time_t		nTime;			//记录时间秒钟数

	union
	{
		float	fvalue[MAX_CURVE_NUM][DATA_NUM];
		int		status[MAX_CURVE_NUM][DATA_NUM];
	} dat;
}CURVE_SHM_DATA;


