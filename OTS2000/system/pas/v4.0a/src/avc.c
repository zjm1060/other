/*AVC program
 cc -o avc.dxe avc.c /DBLIB/dblib.dlb /LANLIB/lan.lib -lm*/

#define	DEBUG

#define ALARM

#include	<unistd.h>
#include	<sys/mman.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<errno.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<sys/time.h>

#include	"/DBINC/dms_com.h"
#include	"/DPSINC/dps_type_def.h"
#include	"/LANINC/lan_common.h"
#include	"/PASINC/avc.h"

	DMS_COMMON	common_aa;

	struct	tm	tmpt;
	time_t		tim;

	LAN_HEAD	lanhd;
	COMMAND_MSG	ml_buf[GEN_NUM],ms_buf;
	SYS_MSG		bj_buf;
	char		name_buf[36];

	int	fg[GEN_NUM],fbh;

	float	pgd[GEN_NUM];
	float	qgd[GEN_NUM];
	float	qrt[GEN_NUM];
	float	qhx[GEN_NUM];
	float	qfx[GEN_NUM];
	float	qmx[GEN_NUM];
	float	qmn[GEN_NUM];

	float	dv,dq,qav,mv;
	short	num,wait,yhbc[GEN_NUM],fgbc[GEN_NUM];
	short	qc_stat,iost,ns;
	char	hostname[12];
	char	zj[36];

void Quit (int sn)
{
	UnMountDB();
	printf ("AVC正常退出!(Ctrl_c or kill -2 pid)\n");
	lan_finish(0);
	exit (0);
}

int     Rdb (char *name )
{
        int     data_type;
        int	ret;

        ret = ReadEntryByNameStr ( name, &common_aa);
        if ( ret == -1)
        {
                printf ("avc.c:%s No this point in the database when ReadEntryByNameStr!\n", name );
                return -1;
        }
        data_type = common_aa.point.data_type;
        return ( data_type );
}

int Rmxv()
{
	int	j;
	int	data_type;

	for (j=0;j<MV_NUM;j++ )
	{
		if ( ReadEntryByNameStr ( mxva[j],&common_aa) == -1)
			continue;
		mv= common_aa.var.anlg.fValue;
	
		if ((mv>MVHX)||(mv<MVLX))
			continue;
		else
		{
#ifdef DEBUG
			printf ("\nAVC: %s母线电压=%5.1f\n",mxva[j],mv);
#endif
			return(1);
		}
	}
	return(-1);
}

int	Gjzf (int n)
{
	int	data_type;

	if ( ReadEntryByNameStr ( czdj[n],&common_aa) == -1)
		return(-1);
	if ( common_aa.var.ind.status.state!=STAT_1)
	{
/*
#ifdef DEBUG
		printf ("AVC: %d号发电机不是成组控制!\n",n+1);
#endif
*/
		return(0);
	}

	if (ReadEntryByNameStr(rtuon[n],&common_aa)==-1)
		return(-1);
	if ( common_aa.var.ind.status.state != STAT_1)
	{
/*
#ifdef DEBUG
		printf ("AVC: %d号RTU 离线\n",n+1);
#endif
*/
		return(0);
	}
/*
	if (eadEntryByNameStr(jsj[n],&common_aa)==-1)
		return(-1);
	if (common_aa.var.ind.status.state != STAT_1)
	{
#ifdef DEBUG
		printf ("AVC: %d RTU 非计算机控制！\n",n+1);
#endif
		return(0);
	}
*/
	if (ReadEntryByNameStr(jzst[n],&common_aa)==-1)
		return(-1);
	if (common_aa.var.ind.status.state != STAT_1)
	{
/*
#ifdef DEBUG
		printf ("AVC: %d号发电机在非运行状态!\n",n+1);
#endif
*/
		return(0);
	}

	if (ReadEntryByNameStr(lczd[n],&common_aa)==-1)
		return(-1);
	if (common_aa.var.ind.status.state != STAT_1)
	{
/*
#ifdef DEBUG
		printf ("AVC: %d号机励磁调节器不是自动!\n",n+1);
#endif
*/
		return(0);
	}

	if (ReadEntryByNameStr(ygrt[n],&common_aa)==-1)
		return(-1);
	qc_stat= common_aa.var.anlg.value.status.chan_fail | common_aa.var.anlg.value.status.man_set;
	iost= common_aa.fixed.iost.scanEnable;
/*
	if (qc_stat||(iost==0))
		return(0);
*/
	pgd[n]= common_aa.var.anlg.fValue;
	if ((pgd[n]>PHX)||(pgd[n]<PLX))
	{
#ifdef DEBUG
		printf ("AVC:%d号机有功 =%5.1f 不合理！\n",n+1,pgd[n]);
#endif
		pgd[n]=PHX;
	}

	if (ReadEntryByNameStr(wgrt[n],&common_aa)==-1)
		return(-1);
	qc_stat= common_aa.var.anlg.value.status.chan_fail | common_aa.var.anlg.value.status.man_set;
	iost= common_aa.fixed.iost.scanEnable;
/*
	if (qc_stat||(iost==0))
		return(0);
*/
	qrt[n]= common_aa.var.anlg.fValue;

	if (ReadEntryByNameStr(wghx[n],&common_aa)==-1)
		return(-1);
	qhx[n]= common_aa.var.anlg.fValue;

	if (ReadEntryByNameStr(wgfx[n],&common_aa)==-1)
		return(-1);
	qfx[n]= common_aa.var.anlg.fValue;

	if (pgd[n]<=PEX)
	{
		qmx[n]=QEX;
		qmn[n]=QEX;
	}
	else
	{
		qmx[n]=pqqx[( int)(pgd[n]-PEX)];
		qmn[n]=fpqqx[( int)(pgd[n]-PEX)];
	}

	if (qhx[n]<qmx[n])
		qmx[n]=qhx[n];

	if (qfx[n]<qmn[n])
		qmn[n]=-qfx[n];
	else
		qmn[n]=-qmn[n];
/*
#ifdef DEBUG
	printf ("%d号机 无功上限=%8.3f 下限=%8.3f 实发无功=%8.3f 有功=%8.3f\n",
		n+1,qmx[n],qmn[n],qrt[n],pgd[n]);
#endif
*/
	return(1);
}

int	Dzyx(int pt)
{
	if (ReadEntryByNameStr(dzal[pt][0],&common_aa)==-1)
		return(-1);
	qc_stat= common_aa.var.anlg.value.status.lmt_state;
	if (qc_stat==ANA_ALM_HI_ALARM||qc_stat==ANA_ALM_HI_WARN)
		return(0);

	if (ReadEntryByNameStr(dzal[pt][1],&common_aa)==-1)
		return(-1);
	qc_stat= common_aa.var.anlg.value.status.lmt_state;
	if (qc_stat==ANA_ALM_HI_ALARM||qc_stat==ANA_ALM_HI_WARN)
		return(0);

	if (ReadEntryByNameStr(dzal[pt][2],&common_aa)==-1)
		return(-1);
	qc_stat= common_aa.var.anlg.value.status.lmt_state;
	if (qc_stat==ANA_ALM_HI_ALARM||qc_stat==ANA_ALM_HI_WARN)
		return(0);

	if (ReadEntryByNameStr(lca[pt],&common_aa)==-1)
		return(-1);
	qc_stat= common_aa.var.anlg.value.status.lmt_state;
	if (qc_stat==ANA_ALM_HI_ALARM||qc_stat==ANA_ALM_HI_WARN)
		return(0);

	return(1);
}

int	Nettr (int nb)
{
	float	ys;
	int	iValue;

	ys=qav-(short)qav;
	qav=(short)qav;

	if (ys>=0.5)
		qav=qav+1.0;
	if (ys<=-0.5)
		qav=qav-1.0;

	if (qav>qmx[nb])
		qav=qmx[nb];
	else
		if (qav<qmn[nb])
			qav=qmn[nb];
/* When add the WG,the real data is greater then the object value,keep the real value*/
/*
	if (dv>0.0)
	{
		if ((qav<=qrt[nb])&&(qrt[nb]<qmx[nb])&&(num>1))
		{
			dq=dq-qrt[nb];
#ifdef DEBUG
			printf ("nb=%d qav=%8.3f<qrt=%8.3f dq=%8.3f\n",nb+1,qav,qrt[nb],dq);
#endif
			return(0);
		}
	}
	else
		if ((qav>=qrt[nb])&&(qrt[nb]>qmn[nb])&&(num>1))
		{
			dq=dq-qrt[nb];
#ifdef DEBUG
			printf ("nb=%d qav=%8.3f>qrt=%8.3f dq=%8.3f\n",nb,qav,qrt[nb],dq);
#endif
			return(0);
		}
*/
	dq=dq-qav;

	if (ReadEntryByNameStr(wggd[nb],&common_aa)==-1)
		return(-1);
	
	ml_buf[nb].type_id =C_RPOWER;
	ml_buf[nb].stn_id =common_aa.point.stn_id;
	ml_buf[nb].dev_id =common_aa.point.dev_id;
	ml_buf[nb].data_type=common_aa.point.data_type;
	ml_buf[nb].point_id = common_aa.point.pt_id;

	ml_buf[nb].status.opr=0;
	ml_buf[nb].status.state=0;
	ml_buf[nb].status.src=2;
	iValue=(int)(qav*1000);
	ml_buf[nb].data.iValue=iValue;
	ml_buf[nb].ctrl_addr=common_aa.fixed.ctrl_addr;
	GetLocalHostId(&ml_buf[nb].host_id);
	ml_buf[nb].usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	ml_buf[nb].second=tmpt.tm_sec;
	ml_buf[nb].minute=tmpt.tm_min;
	ml_buf[nb].hour=tmpt.tm_hour;
	ml_buf[nb].day=tmpt.tm_mday;
	ml_buf[nb].month=tmpt.tm_mon+1;
	ml_buf[nb].year=tmpt.tm_year+1900;
	strcpy(ml_buf[nb].usr_login_name,"AVC");
	lanhd.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&ml_buf[nb].point_id);
	Swap2Byte(&ml_buf[nb].status);
	Swap4Byte(&ml_buf[nb].data.iValue);
	Swap2Byte(&ml_buf[nb].ctrl_addr);
	Swap2Byte(&ml_buf[nb].year);
	Swap2Byte(&lanhd.length);
#endif
/* net transmit*/
/*
	if (ReadEntryByNameStr(kbh,&common_aa)==-1)
		return(-1);
	if ( common_aa.var.ind.status.state == 1)
*/
	if (fbh==1)
	{
/*close	loop	control*/

		lanhd.dp_type=COMM_TIME;
/*
		lanhd.dest_id =nb+1;
*/
		lanhd.dest_id =ml_buf[nb].dev_id;
		lanhd.src_id =ml_buf[nb].host_id;
		lanhd.dest_stn =ml_buf[nb].stn_id;
		lanhd.packet_num =0;

		sleep (INTVAL);

		lan_out ((char *)&ml_buf[nb],lanhd,(unsigned char)0);
		wait=1;
#ifdef DEBUG
		printf ("AVC 闭环控制 机组=%d 无功给定=%8.3f 未分配无功=%8.3f\n",nb+1,qav,dq);
#endif
	}
	else
	{
#ifdef DEBUG
		printf ("开环控制 机组=%d 无功给定=%8.3f 未分配无功=%8.3f\n",nb+1,qav,dq);
#endif
	}

/*write to database */

	if (ReadEntryByNameStr(wgyh[nb],&common_aa)==-1)
		return(-1);
	if (iValue!= common_aa.var.anlg.fValue||yhbc[nb]>=BCNUM)
	{
		ml_buf[nb].type_id =C_OP_VALUE_SET;
		ml_buf[nb].stn_id =common_aa.point.stn_id;
		ml_buf[nb].dev_id =common_aa.point.dev_id;
		ml_buf[nb].data_type=common_aa.point.data_type;
		ml_buf[nb].point_id = common_aa.point.pt_id;
#ifdef SUN_OS
		Swap2Byte(&ml_buf[nb].point_id);
#endif

		lanhd.dp_type=COMM_RECORD_TIME;
/*
		GetLocalHostId(&lanhd.src_id);
*/
		lanhd.dest_id =0;
		lanhd.src_id =ml_buf[nb].host_id;
		lanhd.dest_stn =ml_buf[nb].stn_id;
		lanhd.packet_num =0;
		sleep (INTVAL);

		lan_out ((char *)&ml_buf[nb],lanhd,(unsigned char)0);
		yhbc[nb]=0;
	}
	else
		yhbc[nb]++;

	return(2);
}

void	Msgset (POINTER ptr,short state)
{
	ms_buf.type_id =C_SYS_MODE_SET;
	ms_buf.stn_id =ptr.stn_id;
	ms_buf.dev_id =ptr.dev_id;
	ms_buf.data_type=ptr.data_type;
	ms_buf.point_id = ptr.pt_id;

	ms_buf.status.opr=0;
	ms_buf.status.state=state;
	ms_buf.status.src=2;
	ms_buf.data.iValue=0;
	ms_buf.ctrl_addr=common_aa.fixed.ctrl_addr;
	GetLocalHostId(&ms_buf.host_id);
	ms_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	ms_buf.second=tmpt.tm_sec;
	ms_buf.minute=tmpt.tm_min;
	ms_buf.hour=tmpt.tm_hour;
	ms_buf.day=tmpt.tm_mday;
	ms_buf.month=tmpt.tm_mon+1;
	ms_buf.year=tmpt.tm_year+1900;
	strcpy(ms_buf.usr_login_name,"AVC");
	lanhd.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&ms_buf.point_id);
	Swap2Byte(&ms_buf.status);
/*
	Swap4Byte(&ms_buf.data.iValue);
	Swap2Byte(&ms_buf.ctrl_addr);
*/
	Swap2Byte(&ms_buf.year);
	Swap2Byte(&lanhd.length);
#endif
	lanhd.dp_type=COMM_RECORD_TIME;
/*
	GetLocalHostId(&lanhd.src_id);
*/
	lanhd.dest_id =0;
	lanhd.src_id =ms_buf.host_id;
	lanhd.dest_stn =ms_buf.stn_id;
	lanhd.packet_num =0;
	lan_out ((char *)&ms_buf,lanhd,(unsigned char)0);
}

void	Almsg(POINTER ptr,short state)
{
	bj_buf.type_id =DPS_IND_ALM;
	bj_buf.stn_id =ptr.stn_id;
	bj_buf.dev_id =ptr.dev_id;
	bj_buf.data_type=ptr.data_type;
	bj_buf.number = 1;
	bj_buf.point_id = ptr.pt_id;

	bj_buf.status=state;
	bj_buf.ms=0;
	GetLocalHostId(&bj_buf.host_id);

	tim=time (NULL);
	tmpt=*localtime (&tim);
	bj_buf.sec=tmpt.tm_sec;
	bj_buf.min=tmpt.tm_min;
	bj_buf.hour=tmpt.tm_hour;
	bj_buf.day=tmpt.tm_mday;
	bj_buf.month=tmpt.tm_mon+1;
	bj_buf.year=tmpt.tm_year+1900;
	lanhd.length=sizeof(SYS_MSG);

#ifdef SUN_OS
	Swap2Byte(&bj_buf.number);
	Swap2Byte(&bj_buf.point_id);
	Swap2Byte(&bj_buf.status);
	Swap2Byte(&bj_buf.ms);
	Swap2Byte(&bj_buf.year);
	Swap2Byte(&lanhd.length);
#endif
	lanhd.dp_type=BJ_DB_TIME;
/*
	GetLocalHostId(&lanhd.src_id);
*/
	lanhd.dest_id =0;
	lanhd.src_id =bj_buf.host_id;
	lanhd.dest_stn =bj_buf.stn_id;
	lanhd.packet_num =0;
	lan_out ((char *)&bj_buf,lanhd,(unsigned char)0);
}

void	main()

{
	key_t	shmkey;
	float	vh,vl,vq,qs;
	int	fvq,fsl,fmv,fav,fnc,fsm;
	int	i,tu,fm;
	float	fValue;
	short	tmp,point;

	gethostname (hostname,12);
	for (ns=0;ns<2;ns++ )
	{
		if (strcmp (hostname,zjname[ns])==0)
		break;
	}
	if (ns>=2)
	{
		printf ("AVC:AVC退出，不能在本机运行！\n");
		pause;
/*
		exit (-1);
*/
	}

	signal(SIGINT,Quit);

	if(-1==MountDB(DATABASE_NAME))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
/*
	Lan_init ();
*/
	get_lan_shm(1);
	fvq=fsl=fmv=fav=fnc=fsm=0;

	for ( i=0;i<GEN_NUM;i++ )
	{
		yhbc[i]=fgbc[i]=0;
	}
for (;;)
{
	wait=0;
/*
	if (ReadEntryByNameStr(zj,&common_aa)==-1)
		goto PAUSE;
	if (common_aa.var.ind.status.state!=0)
		goto PAUSE;
*/
	if( GetCurServCtrlPrvg()!=0 )
		goto PAUSE;

	if (ReadEntryByNameStr(vron,&common_aa)==-1)
			goto PAUSE;
	if (!common_aa.var.ind.status.state)
	{
/*
#ifdef ALARM
		printf ("AVC:功能未投入, AVC不运行!\n");
#endif
*/
		goto PAUSE;
	}

	num=0;

	for ( i=0;i<GEN_NUM;i++ )
		fg[i]=Gjzf ( i);

	if (ReadEntryByNameStr(atmn,&common_aa)==-1)
		goto NEXT;
	qc_stat= common_aa.var.ind.status.state;
	if (ReadEntryByNameStr(kbh,&common_aa)==-1)
		goto NEXT;
	if (common_aa.var.ind.status.state==STAT_1 && qc_stat==STAT_1)
	{
		fbh=1;
	}
	else
	{
		fbh=0;
	}
/*
#ifdef ALARM
		printf ("AVC:手动控制，AVC不能运行！\n");
#endif
		goto NEXT;
	}

	if (ReadEntryByNameStr(rclc,&common_aa)==-1)
		goto NEXT;
	if ( common_aa.var.ind.status.state)
	{
#ifdef ALARM
		printf ("AVC:控制权为远方，AVC不能运行！\n");
#endif
		goto NEXT;
	}
*/
	if (ReadEntryByNameStr(dqv,&common_aa)==-1)
		goto NEXT;
	vq= common_aa.var.anlg.fValue;
	if ((vq<0.0)||(vq>VQX))
	{
#ifdef ALARM
		if (!fvq)
		{
			fvq=1;
			printf ("AVC:母线电压调差系数超出范围，AVC不能运行！\n");
		}
#endif
		goto NEXT;
	}
	fvq=0;

	if (ReadEntryByNameStr(rtuon[GEN_NUM],&common_aa)==-1)
		goto NEXT;
	if ( common_aa.var.ind.status.state == 1)
	{
#ifdef ALARM
		if (!fsl)
		{
			fsl=1;
			printf ("AVC:开关站RTU离线, AVC不能运行！\n");
		}
#endif	
		goto NEXT;
	}
	fsl=0;

/* now Judge if	the bus	voltages is in reasonable area*/

	fm=Rmxv();

	if (fm!=1)
	{
#ifdef ALARM
		if (fmv==0)
		{
			fmv=1;
			printf ("AVC:母线电压不合理，AVC不能运行！\n");
		}
#endif
		goto NEXT;
	}
	fmv=0;

/*Get the time and get the high & low voltage limits*/

	if (ReadEntryByNameStr(avcmd,&common_aa)==-1)
		goto NEXT;
	if ( common_aa.var.ind.status.state == 1)
	{
#ifdef DEBUG
		printf ("AVC:为电压曲线方式\n");
#endif
		tim=time (NULL);
		tmpt=*localtime (&tim);

		if (ReadEntryByNameStr(mvh[tmpt.tm_hour],&common_aa)==-1)
			goto NEXT;
		vh= common_aa.var.anlg.fValue;

		if (ReadEntryByNameStr(mvl[tmpt.tm_hour],&common_aa)==-1)
			goto NEXT;
		vh= common_aa.var.anlg.fValue;
	}
	else
	{
		if (ReadEntryByNameStr(mvgdh,&common_aa)==-1)
			goto NEXT;
		vh= common_aa.var.anlg.fValue;

		if (ReadEntryByNameStr(mvgdl,&common_aa)==-1)
			goto NEXT;
		vl= common_aa.var.anlg.fValue;
	}

	dv=(vh+vl)/2-mv;
#ifdef DEBUG
	printf ("AVC: 电压差=%8.3f 母线电压=%8.3f 上限=%8.3f 下限=%8.3f\n",
		dv,mv,vh,vl);
#endif

	if ((mv>vl)&&(mv<vh))
	{
#ifdef ALARM
		if (fav==0)
		{
			fav=1;
			printf ("AVC: 母线电压正常！\n");
		}
#endif
		goto NEXT;
	}
	fav=0;

/*	Make	dv-->dq transfer*/

	if (dv>DVX)
		dv=DVX;
	else
		if (dv<-DVX)
	dv=-DVX;

	dq=dv*vq;
#ifdef DEBUG
	printf ("AVC: 无功净增值=%8.3f",dq);
	for ( i=0;i<GEN_NUM;i++ )
		printf (" fg[%d]=%d",i,fg[i]);
	printf ("\n");

/*	printf ("AVC: fg[0]=%d fg[1]=%d fg[2]=%d fg[3]=%d 无功净增值=%8.3f\n",
		fg[0],fg[1],fg[2],fg[3],dq);
*/
#endif
/*	If DZ_A is over limit then get the least adjuestable generator*/

	if (dv>=0.0)
	{
		 qs=MAXNUM;

		 for ( i=0;i<GEN_NUM;i++ )
		{
			if (fg[i]==1)
			{
				if (qrt[i]>=0.0)
				{
					fg[i]=Dzyx( i);
					if (fg[i]!=1)
					{
#ifdef ALARM
						printf ("AVC: %d号机定子电流越限，该机退出AVC。\n",i+1);
#endif
						continue;
					}
				}	
				if (qrt[i]==qmx[i])
					fg[i]=0;
				else
				{
					if (qmx[i]<qs)
					{
						qs=qmx[i];
						tu=i;
					}
					num++;
					dq=dq+qrt[i];
				}
			}
		}
	}
	else
	{
		qs=-MAXNUM;

		for ( i=0;i<GEN_NUM;i++ )
		{
			if (fg[i]==1)
			{
				if (qrt[i]<0.0)
				{
					fg[i]=Dzyx( i);
					if (fg[i]!=1)
					{
#ifdef ALARM
						printf ("AVC: %d号机定子电流越限，该机退出AVC。\n",i+1);
#endif
						continue;
					}
				}
				if (qrt[i]==qmn[i])
					fg[i]=0;
				else
				{
					if (qmn[i]>qs)
					{
						qs=qmn[i];
						tu=i;
					}
					num++;
					dq=dq+qrt[i];
				}
			}
		}
	}
#ifdef ALARM
	if (num==0)
	{
		if (fnc==0)
		{
			fnc=1;
			printf ("AVC没有可控制机组！\n");
		}
	}
	else
		fnc=0;
#endif

/* According to the state of flag fg[i],set unit group controlable or not. */

NEXT:
	for ( i=0;i<GEN_NUM;i++ )
	{
		if (ReadEntryByNameStr( czkt[i],&common_aa)==-1)
			continue;
		tmp = common_aa.var.ind.status.state;
		if ((fg[i]==1 && tmp ==0)||(fg[i]!=1 && tmp)||fgbc[i]>=BCNUM)
		{
			if (fg[i]==1)
				tmp =STAT_1;
			else
				tmp =STAT_0;

			sleep (INTVAL);
			point = common_aa.point.pt_id;
/*			Msgset ( i+1,MSG_SET,point,tmp);
*/
			Almsg( common_aa.point,tmp);
			fgbc[i]=0;
		}
		else
			fgbc[i]++;
	}

	if (num==0)
		goto NEXT1;

/* According to the number of controllable unit to distribute the "dq"*/

#ifdef DEBUG
	printf ("AVC 启动调节 可控机组数=%d,调差系数=%8.3f,无功目标值=%8.3f\n",num,vq,dq);
	printf ("调控机组号=%d",tu+1);

	for ( i=0;i<GEN_NUM;i++ )
		printf (" fg[%d]=%d",i,fg[i]);
	printf ("\n");

#endif

/* compute the average Q of each unit and transmit it over net*/

	while (num>0)
	{
		qav=dq/num;

		fg[tu]=Nettr (tu);
	
		num--;
	
		if (num==0)
			break;

		if (dv>0.0)
		{
			qs=MAXNUM;
			for ( i=0;i<GEN_NUM;i++ )
			{
				if ((fg[i]==1)&&(qmx[i]<qs))
				{
					qs=qmx[i];
					tu=i;
				}
			}
		}
		else
		{
			qs=-MAXNUM;
			for ( i=0;i<GEN_NUM;i++ )
			{
				if ((fg[i]==1)&&(qmn[i]>qs))
				{
					qs=qmn[i];
					tu=i;
				}
			}
		}
	}
#ifdef ALARM
	if ((dq>1.0)||(dq<-1.0))
	{
		if (fsm==1)
		{
			printf ("AVC:无功目标值无法全部分配！\n");
		}
		fsm++;
	}	
	else
		fsm=0;
#endif

NEXT1:
	for ( i=0;i<GEN_NUM;i++ )
	{
		if (fg[i]!=2)
		{
			if (ReadEntryByNameStr(wgrt[i],&common_aa)==-1)
				continue;
			fValue= common_aa.var.anlg.fValue;

			if (ReadEntryByNameStr(wgyh[i],&common_aa)==-1)
				continue;
			if (fValue!= common_aa.var.anlg.fValue||yhbc[i]>=BCNUM)
			{
				ml_buf[i].type_id =C_OP_VALUE_SET;
				ml_buf[i].stn_id =common_aa.point.stn_id;
				ml_buf[i].dev_id =common_aa.point.dev_id;
				ml_buf[i].data_type=common_aa.point.data_type;
				ml_buf[i].point_id = common_aa.point.pt_id;

				ml_buf[i].status.opr=0;
				ml_buf[i].status.state=0;
				ml_buf[i].status.src=2;
				ml_buf[i].data.iValue=(int)(fValue*1000);
				ml_buf[i].ctrl_addr=common_aa.fixed.ctrl_addr;
				GetLocalHostId(&ml_buf[i].host_id);
				ml_buf[i].usr_id=0;

				tim=time (NULL);
				tmpt=*localtime (&tim);
				ml_buf[i].second=tmpt.tm_sec;
				ml_buf[i].minute=tmpt.tm_min;
				ml_buf[i].hour=tmpt.tm_hour;
				ml_buf[i].day=tmpt.tm_mday;
				ml_buf[i].month=tmpt.tm_mon+1;
				ml_buf[i].year=tmpt.tm_year+1900;
				strcpy(ml_buf[i].usr_login_name,"AVC");
				lanhd.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
				Swap2Byte(&ml_buf[i].point_id);
				Swap2Byte(&ml_buf[i].status);
				Swap4Byte(&ml_buf[i].data.iValue);
				Swap2Byte(&ml_buf[i].ctrl_addr);
				Swap2Byte(&ml_buf[i].year);
				Swap2Byte(&lanhd.length);
#endif
				lanhd.length=ML_SIZE;
				lanhd.dp_type=LAN_BC;
				lanhd.dest_id =0;
				GetLocalHostId(&lanhd.src_id);
				sleep (INTVAL);

				lanhd.dp_type=COMM_RECORD_TIME;
/*
				GetLocalHostId(&lanhd.src_id);
*/
				lanhd.dest_id =0;
				lanhd.src_id =ml_buf[i].host_id;
				lanhd.dest_stn =ml_buf[i].stn_id;
				lanhd.packet_num =0;
				lan_out ((char *)&ml_buf[i],lanhd,(unsigned char)0);
				yhbc[i]=0;
			}
			else
				yhbc[i]++;
		}
	}
PAUSE:
	if (wait==1)
		sleep (CYCLE1);
	else
		sleep (CYCLE2);
}
}

