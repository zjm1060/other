/************************************************************************
*  FILE NAME:           iccp_fun.c      				*
*  FUNCTION:            Controlled Station				*
*			Basic Communication Function			*
*  NAME :  			              				*
*  DATE:                Nov,2006                			*
*									*
* MODIFICATION LOG :							*
*  Date     Who   Rev                     Comments			*
* --------  ---  ------   ----------------------------------------------*
* 03/28/07  zy    2       updated					*
* 12/06/06  zy    1       Created for SUN ICCP TASE.2 application	*
************************************************************************/

#include "../inc/mit_main.h"
#include "str_util.h"

#include "cppinc.h"

#include "../inc/iccp_main.h"

#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"
/*
#define	DEBUG
*/
static char *thisFileName = __FILE__;

extern	SOEBUF	soebuf;
extern	char	infobuf[1024];

extern	ST_INT	nAssocNum[MAX_DC_NUM];		/* Added for save association number for remotes */
extern	sGWAssoc  sAssocDat[MAX_DC_NUM][2];	/* Changed to two dimension for 2 association in one remote 2003.7.30 zy */

extern	unsigned char	nSrcId;
extern	int	ClockSyncflag;
extern	int	Readflag,Testflag,Resetflag;
extern	int	Paramflag,MirrorNAckflag;
extern	int	interrogationflag,counterflag,icgroupflag,cigroupflag;
extern	int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
extern	int	localinitflag;
extern	int	scanflag;
extern	unsigned short	delayalarm;
extern	unsigned char	cot_interrogation,cot_counter;
extern	unsigned short	seladdr;
extern	short	databand;

extern	time_t	scan_tim;

extern	SPDEF	spdef;
extern	DPDEF	dpdef;
extern	MEDEF	medef;

unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
unsigned short	m_cRespParamNum,m_cRespICGrpNum,m_cRespCIGrpNum;
unsigned char	count_sq,ITGrpType;

short	organabuf[MAX_ME_NUM];
float	organafbuf[MAX_ME_NUM];
unsigned char	organastbuf[MAX_ME_NUM];

unsigned char	orgspbuf[MAX_SP_NUM],orgdpbuf[MAX_DP_NUM];


int	SendSinglePointInform()
{
	ST_CHAR           dvname[256];
	ST_BOOLEAN        dataEntered;
	MIS_DV_REF       dv_ref;
	MIS_DV           *pstDv;
	MIT_DATA_VALUE   *pstMitDataValue;
	MI_DV_DATA_UNION *pData;
	MI_REAL_Q        *realq;
	ST_INT            ret;
	ST_INT            send_num;

	unsigned char	stat;
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	int		data_type;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;

	send_num = 0;
	for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Check for SP status Change */
	{
/*
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			orgspbuf[pntnum] =0;
			continue;
		}

		strcpy(name_buf,spdef.name[pntnum]);
		ret = GetSPStatByName(name_buf,&stat);
		if(ret==-1)
		{
			sprintf(infobuf,"%s No this SP point in the database file!\n",name_buf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			orgspbuf[pntnum] =0;
			continue;
		}
*/
		if(spdef.ptr[pntnum].dev_id == 0)
		{
			orgspbuf[pntnum] =0;
			continue;
		}
		ptr = spdef.ptr[pntnum];
		if(GetSPStatById(&ptr,&stat)==-1)
		{
			orgspbuf[pntnum] =0;
			continue;
		}
		if(stat != orgspbuf[pntnum])
		{
			strcpy(dvname, spdef.iccp_name[pntnum]);
			ret = mis_find_dv(dvname, &dv_ref);
			if (ret != SD_SUCCESS)
			{
			/* Signal not found */
				sprintf(infobuf,"iccp: - signal '%s' %s not found - check configuration file\n", dvname,spdef.name[pntnum]);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}

			dv_ref = (dv_ref & MIS_INDEX_MASK);
			pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
			pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
			pData = (MI_DV_DATA_UNION *)&pstMitDataValue->data;

/*
			pData->s = (ST_UCHAR)((stat << 6) + (nValidity << 4));
*/
			pData->s = stat;
			mis_dv_change(pstMitDataValue->dvRef);

			send_num ++;

			orgspbuf[pntnum] =stat;	/* stat[7.6] Res IV RES4 */
			sprintf(infobuf,"iccp: - %s %s - StateQ - %02x\n", dvname, spdef.name[pntnum], pData->s);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
	}
	if(send_num)
	{
		Call_MI_Service();
		return(true);
	}
	return(false);
}

int	SendDoublelePointInform()
{
	ST_CHAR           dvname[256];
	ST_BOOLEAN        dataEntered;
	MIS_DV_REF       dv_ref;
	MIS_DV           *pstDv;
	MIT_DATA_VALUE   *pstMitDataValue;
	MI_DV_DATA_UNION *pData;
	MI_REAL_Q        *realq;
	ST_INT            ret;
	ST_INT            send_num;

	unsigned char	stat;
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;

	send_num = 0;
	for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* Check for SP status Change */
	{
/*
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			orgdpbuf[pntnum] =0;
			continue;
		}

		strcpy(name_buf,dpdef.name[pntnum]);
		ret = GetDPStatByName(name_buf,&stat);
		if(ret==-1)
		{
			sprintf(infobuf,"%s No this DP point in the database file!\n",name_buf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			orgdpbuf[pntnum] =0;
			continue;
		}
*/
		if(dpdef.ptr[pntnum].dev_id == 0)
		{
			orgdpbuf[pntnum] =0;
/*
			sprintf(infobuf,"+++ No this DP point %s %s in the database!\n",dpdef.name[pntnum],dpdef.iccp_name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
*/
			continue;
		}
		ptr = dpdef.ptr[pntnum];
		if(dpdef.doublept[pntnum] == 1)
		{
			if(GetDPStatById(&ptr,&stat)==-1)
			{
				orgdpbuf[pntnum] =0;
				continue;
			}
		}
		else
		{
			if(GetSPStatById(&ptr,&stat)==-1)
			{
				orgdpbuf[pntnum] =0;
				continue;
			}
		}
		if(stat != orgdpbuf[pntnum])
		{
			strcpy(dvname, dpdef.iccp_name[pntnum]);
			ret = mis_find_dv(dvname, &dv_ref);
			if (ret != SD_SUCCESS)
			{
			/* Signal not found */
				sprintf(infobuf,"iccp: - signal '%s' %s not found - check configuration file\n", dvname,dpdef.name[pntnum]);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}

			dv_ref = (dv_ref & MIS_INDEX_MASK);
			pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
			pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
			pData = (MI_DV_DATA_UNION *)&pstMitDataValue->data;

/*
			pData->s = (ST_UCHAR)((stat << 6) + (nValidity << 4));
*/
			pData->s = stat;
			mis_dv_change(pstMitDataValue->dvRef);

			send_num++;

			orgdpbuf[pntnum] =stat;	/* stat[7.6] Res IV RES4 */

			sprintf(infobuf,"iccp: - %s %s - StateQ - %02x\n", dvname, dpdef.name[pntnum], pData->s);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
	}
	if(send_num)
	{
		Call_MI_Service();
		return(true);
	}
	return(false);
}

int	SendSinglePointInformCyc()
{
	ST_CHAR			dvname[256];
	ST_BOOLEAN		dataEntered;
	MIS_DV_REF		dv_ref;
	MIS_DV			*pstDv;
	MIT_DATA_VALUE	*pstMitDataValue;
	MI_DV_DATA_UNION	*pData;
	MI_REAL_Q		*realq;
	ST_INT			ret;

	unsigned char	stat;
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,endnum;

	endnum = spdef.number;

	for(pntnum=0;pntnum<endnum;pntnum++)		/* Cycle for SP status */
	{
/*
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			orgspbuf[pntnum] =0;
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		ret = GetSPStatByName(name_buf,&stat);
		if(ret==-1)
		{
			sprintf(infobuf,"%s No this SP point in the database file!\n",name_buf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			orgspbuf[pntnum] =0;
			continue;
		}
*/
		if(spdef.ptr[pntnum].dev_id == 0)
		{
			orgspbuf[pntnum] =0;
			sprintf(infobuf,"+++ No this SP point %s %s in the database!\n",spdef.name[pntnum],spdef.iccp_name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}
		ptr = spdef.ptr[pntnum];
		if(GetSPStatById(&ptr,&stat)==-1)
		{
			orgspbuf[pntnum] =0;
			sprintf(infobuf,"+++ No this SP point %s %s in the database!\n",spdef.name[pntnum],spdef.iccp_name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}
		strcpy(dvname, spdef.iccp_name[pntnum]);
		ret = mis_find_dv(dvname, &dv_ref);
		if (ret != SD_SUCCESS)
		{
		/* Signal not found */
			sprintf(infobuf,"iccp: - signal '%s' %s not found - check configuration file\n", dvname,spdef.name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}

		dv_ref = (dv_ref & MIS_INDEX_MASK);
		pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
		pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
		pData = (MI_DV_DATA_UNION *)&pstMitDataValue->data;

/*
		pData->s = (ST_UCHAR)((stat << 6) + (nValidity << 4));
*/
		pData->s = stat;
		mis_dv_change(pstMitDataValue->dvRef);

		orgspbuf[pntnum] =stat;	/* stat[7.6] Res IV RES4 */
#ifdef DEBUG
		sprintf(infobuf,"iccp: Cyc Send - %s %s - StateQ - %02x\n", dvname, spdef.name[pntnum], pData->s);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
#endif
	}
	return(true);
}

int	SendDoublePointInformCyc()
{
	ST_CHAR			dvname[256];
	ST_BOOLEAN		dataEntered;
	MIS_DV_REF		dv_ref;
	MIS_DV			*pstDv;
	MIT_DATA_VALUE		*pstMitDataValue;
	MI_DV_DATA_UNION	*pData;
	MI_REAL_Q		*realq;
	ST_INT			ret;

	unsigned char	stat;
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,endnum;

	endnum = dpdef.number;

	for(pntnum=0;pntnum<endnum;pntnum++)		/* Cycle for DP status */
	{
/*
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			orgdpbuf[pntnum] =0;
			continue;
		}
		strcpy(name_buf,dpdef.name[pntnum]);
		ret = GetDPStatByName(name_buf,&stat);
		if(ret==-1)
		{
			sprintf(infobuf,"%s No this DP point in the database file!\n",name_buf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			orgdpbuf[pntnum] =0;
			continue;
		}
*/
		if(dpdef.ptr[pntnum].dev_id == 0)
		{
			orgdpbuf[pntnum] =0;
			sprintf(infobuf,"+++ No this DP point %s %s in the database!\n",dpdef.name[pntnum],dpdef.iccp_name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}
		ptr = dpdef.ptr[pntnum];
		if(dpdef.doublept[pntnum] == 1)
		{
			if(GetDPStatById(&ptr,&stat)==-1)
			{
				orgdpbuf[pntnum] =0;
				sprintf(infobuf,"+++ No this DP point %s %s in the database!\n",dpdef.name[pntnum],dpdef.iccp_name[pntnum]);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}
		else
		{
			if(GetSPStatById(&ptr,&stat)==-1)
			{
				orgdpbuf[pntnum] =0;
				sprintf(infobuf,"+++ No this SP point %s %s in the database!\n",dpdef.name[pntnum],dpdef.iccp_name[pntnum]);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}

		strcpy(dvname, dpdef.iccp_name[pntnum]);
		ret = mis_find_dv(dvname, &dv_ref);
		if (ret != SD_SUCCESS)
		{
		/* Signal not found */
			sprintf(infobuf,"iccp: - signal '%s' %s not found - check configuration file\n", dvname,dpdef.name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}

		dv_ref = (dv_ref & MIS_INDEX_MASK);
		pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
		pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
		pData = (MI_DV_DATA_UNION *)&pstMitDataValue->data;

/*
		pData->s = (ST_UCHAR)((stat << 6) + (nValidity << 4));
*/
		pData->s = stat;
/*		mis_dv_change(pstMitDataValue->dvRef); */

		orgdpbuf[pntnum] =stat;	/* stat[7.6] Res IV RES4 */
#ifdef DEBUG
		sprintf(infobuf,"iccp: Cyc Send - %s %s - StateQ - %02x\n", dvname, dpdef.name[pntnum], pData->s);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
#endif
	}
	return(true);
}

int	SendStateInformSOE()
{
	ST_CHAR buffer[256];
	MIT_ASSOC_INFO	*pstAssociationInfo;
	MI_INFO_BUF_HEADER buf_header;
	ST_UCHAR *info_buf;
	ST_INT buf_len;
	ST_INT msg_scope = VCC_SPEC;	 /* or ICC_SPEC */
	ST_RET ret;
	MI_CONN **connArray;
	int arrayIndex;
	MIT_MIS_VCC	  *pstMitMisVcc;
	ST_INT	infoRef;
	ST_BOOLEAN found;
	MIT_INFO_MSG *pstMitInfoMsg;
	FILE  *fPr;
	ST_CHAR ack_filename[128];
	char  strbuf[128];

	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt,pntnum;
	unsigned char nDst;
	time_t		nTim;
	struct	tm	timdat;
	SOEREC rec;
	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		nDst = soebuf.soerec[soepnt].nDst;
		pstAssociationInfo = sAssocDat[nDst][0].pAssocInf;
		pstMitMisVcc = sAssocDat[nDst][0].pMitMisVcc;
		if((pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE) && (nAssocNum[nDst] == 2))
		{
			pstAssociationInfo = sAssocDat[nDst][1].pAssocInf;
			pstMitMisVcc = sAssocDat[nDst][1].pMitMisVcc;
		}

		/*return if association not ACTIVE added 2003.7.9 zy */
		if (pstAssociationInfo->pstAssocCtrl->state == MI_ASSOC_STATE_ACTIVE)
		{
		/* Get the list of MI_ASSOC_CTRLS */
			connArray = (struct _mi_conn **)chk_calloc(list_get_sizeof(pstAssocInfoSet), 
						sizeof(MI_CONN *));
			arrayIndex = 0;
			connArray[arrayIndex++] = pstAssociationInfo->pstAssocCtrl->mi_conn;

			if (pstMitMisVcc!=NULL)
			{
				infoRef = INFOMSG_REF_SOE;	/*---- soe message ----*/

			/* see if we entered a valid number */
				pstMitInfoMsg = pstMitMisVcc->pstInfoMsgs; /* look at the last VCC */
				found = SD_FALSE;
				while ((found == SD_FALSE) && (pstMitInfoMsg != (struct Tag_MIT_INFO_MSG *)NULL))
				{
					if (pstMitInfoMsg->infoRef == infoRef)
						found = SD_TRUE;
					else
						pstMitInfoMsg = (struct Tag_MIT_INFO_MSG *)list_get_next(pstMitMisVcc->pstInfoMsgs, pstMitInfoMsg);
				}

				if (found == SD_TRUE)
				{
					info_buf = (unsigned char *)chk_malloc(pstMitInfoMsg->maxSize + 1);
					buf_len = pstMitInfoMsg->maxSize + 1;
					memset(info_buf,0,buf_len);
					//removed by yyp 2011.10.06	
					/*


					*info_buf = 0x01;
					*(info_buf + 1) = 0x2a; 

					pntnum = soebuf.soerec[soepnt].infoaddr;
					strcpy((char *)(info_buf+2), dpdef.iccp_name[pntnum]);

					*(info_buf + 34) = 0x00;
					*(info_buf + 34) = 0x00;
					*(info_buf + 35) = soebuf.soerec[soepnt].stat;
					*(info_buf + 36) = 0x00;
					*(info_buf + 37) = 0x00;

					timdat.tm_year = (unsigned char)(soebuf.soerec[soepnt].time.year - 1900);
					timdat.tm_mon = (unsigned char)soebuf.soerec[soepnt].time.month - 1;
					timdat.tm_mday = (unsigned char)soebuf.soerec[soepnt].time.day;
					timdat.tm_hour = (unsigned char)soebuf.soerec[soepnt].time.hour;
					timdat.tm_min = (unsigned char)soebuf.soerec[soepnt].time.minute;
					timdat.tm_sec = (unsigned char)soebuf.soerec[soepnt].time.second;
					timdat.tm_isdst = -1;
					nTim = mktime(&timdat);
					if(nTim == -1)
					{
						sprintf(infobuf,"iccp: - SOE mktime error! %02d/%02d/%02d %02d:%02d:%02d\n",
							timdat.tm_year,timdat.tm_mon,timdat.tm_mday,timdat.tm_hour,timdat.tm_min,timdat.tm_sec);
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						time(&nTim);
					}
					millisec = soebuf.soerec[soepnt].time.ms;

					memcpy(info_buf + 38,&nTim,4);				
					memcpy(info_buf + 42,&millisec,2);				
*/
					buf_header.InfoReference = infoRef;
					buf_header.LocalReference = 1;
					pstMitInfoMsg->msgID++;
					buf_header.MessageId = pstMitInfoMsg->msgID;
					buf_header.Size = sizeof(SOEREC);
					rec = soebuf.soerec[soepnt];
#ifdef _BIG_ENDIAN_
					Swap2Byte(&rec.infoaddr);
					Swap2Byte(&rec.stvalue);
					Swap4Byte(&rec.fvalue);
					Swap2Byte(&rec.time.ms);
					Swap2Byte(&rec.time.year);
#endif	
					memcpy(info_buf,&rec,sizeof(SOEREC));

					//ret = mis_send_IMTransfer(arrayIndex, connArray, &buf_header, buf_len - 1, info_buf, msg_scope);
					ret = mis_send_IMTransfer(arrayIndex, connArray, &buf_header, buf_len - 1, info_buf, msg_scope);

					sprintf(infobuf,"iccp: - SOE Msg - i: %d point id: %d state: %02x \n",soepnt,soebuf.soerec[soepnt].infoaddr,
							soebuf.soerec[soepnt].stat);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);

					if (ret != SD_SUCCESS)
					{
						sprintf(infobuf,"iccp: Failure Sending Information Message!!\n");
						printf("%s",infobuf);
						WriteDspInfo(infobuf);
					}
					chk_free(info_buf);
/*
					Call_MI_Service();
*/
				}
				else /*if(found == SD_TRUE)*/
				{

					sprintf(infobuf,"iccp: mit_imsg - InfoRef %d not found\n", infoRef);
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
				}
			}
			chk_free (connArray);
		}/*if (pstAssociationInfo->pstAssocCtrl->state == MI_ASSOC_STATE_ACTIVE)*/
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
		if(nInfoNum >= SEND_SOE_NUM)
		{
			break;
		}
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
/*
#ifdef DEBUG
		sprintf(infobuf,"iccp: SendStateInformSOE: NoRequestData\n");
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
#endif
*/
		return(false);
	}
	Call_MI_Service();
	return(true);
}

int	SendScheduleAckInform(int nDst, char *Ackfile)
{
	ST_CHAR buffer[256];
	MIT_ASSOC_INFO	*pstAssociationInfo;
	MI_INFO_BUF_HEADER buf_header;
	ST_UCHAR *info_buf;
	ST_INT buf_len;
	ST_INT msg_scope = VCC_SPEC;	 /* or ICC_SPEC */
	ST_RET ret;
	MI_CONN **connArray;
	int arrayIndex;
	MIT_MIS_VCC	  *pstMitMisVcc;
	ST_INT	infoRef;
	ST_BOOLEAN found;
	MIT_INFO_MSG *pstMitInfoMsg;
	FILE  *fPr;
	ST_CHAR ack_filename[128];
	char  strbuf[128];

	pstAssociationInfo = sAssocDat[nDst][0].pAssocInf;
	pstMitMisVcc = sAssocDat[nDst][0].pMitMisVcc;
	if((pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE) && (nAssocNum[nDst] == 2))
	{
		pstAssociationInfo = sAssocDat[nDst][1].pAssocInf;
		pstMitMisVcc = sAssocDat[nDst][1].pMitMisVcc;
	}

	/*return if association not ACTIVE added 2003.7.9 zy */
	if (pstAssociationInfo->pstAssocCtrl->state == MI_ASSOC_STATE_ACTIVE)
	{
		/* Get the list of MI_ASSOC_CTRLS */
		connArray = (MI_CONN **)chk_calloc(list_get_sizeof(pstAssocInfoSet), 
					sizeof(MI_CONN *));
		arrayIndex = 0;
		connArray[arrayIndex++] = pstAssociationInfo->pstAssocCtrl->mi_conn;

		if (pstMitMisVcc!=NULL)
		{
			infoRef = INFOMSG_REF_SCHEDULE;	/*---- power schedule acknowledge message ----*/

		/* see if we entered a valid number */
			pstMitInfoMsg = pstMitMisVcc->pstInfoMsgs; /* look at the last VCC */
			found = SD_FALSE;
			while ((found == SD_FALSE) && (pstMitInfoMsg != (struct Tag_MIT_INFO_MSG *)NULL))
			{
				if (pstMitInfoMsg->infoRef == infoRef)
					found = SD_TRUE;
				else
					pstMitInfoMsg = (struct Tag_MIT_INFO_MSG *)list_get_next(pstMitMisVcc->pstInfoMsgs, pstMitInfoMsg);
			}

			if (found == SD_TRUE)
			{
				info_buf = (unsigned char *)chk_malloc(pstMitInfoMsg->maxSize + 1);
				buf_len = pstMitInfoMsg->maxSize + 1;

				memset(info_buf,0,buf_len);

				strcpy(ack_filename,Ackfile);
				if ((fPr = fopen(ack_filename,"r"))== NULL)
				{
					sprintf(infobuf,"*****iccp: Can't open file %s to read Schedule Ack Message *****\n",ack_filename);
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
					return(false);
				}
				fread(info_buf,1,1024,fPr);
				fclose(fPr);

				buf_header.InfoReference = infoRef;
				buf_header.LocalReference = 2;
				pstMitInfoMsg->msgID++;
				buf_header.MessageId = pstMitInfoMsg->msgID;
				buf_header.Size = 1024;

				ret = mis_send_IMTransfer(arrayIndex, connArray, &buf_header, buf_len - 1, 
                                   info_buf, msg_scope);

				if (ret == SD_SUCCESS)
				{
					sprintf(infobuf,"iccp: Sending %s Schedule Ack Message OK!!\n",ack_filename);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
				}
				else
				{
					sprintf(infobuf,"iccp: Failure Sending %s Schedule Ack Message!!\n",ack_filename);
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
				}
				chk_free(info_buf);
/*
#ifdef WINDOWS32
				sprintf(strbuf, "move %s %s 2>/NUL", ack_filename, ICCP_TMP_PATH, "ScheduleAckFile.txt");
				system(strbuf);
#else
				sprintf(strbuf, "mv %s %s%s", ack_filename, ICCP_TMP_PATH,"ScheduleAckFile.txt");
				system(strbuf);
#endif
*/
			}
			else /*if(found == SD_TRUE)*/
			{

				sprintf(infobuf,"iccp: mit_imsg - InfoRef %d not found\n", infoRef);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
			}
		}
		chk_free (connArray);
	}/*if (pstAssociationInfo->pstAssocCtrl->state == MI_ASSOC_STATE_ACTIVE)*/
	Call_MI_Service();
	return(true);
}

int	WriteMeasureValueNC()
{
	ST_CHAR			dvname[256];
	ST_BOOLEAN		dataEntered;
	MIS_DV_REF		dv_ref;
	MIC_DV			*pstDv;
	MIT_DATA_VALUE	*pstMitDataValue;
	MI_DV_DATA_UNION	*pData;
	MI_REAL_Q		*realq;
	ST_INT			ret;
	ST_CHAR  	dv_name[256];
	MI_REQ_CTRL 	*req;
	MI_CONN 	*mi_conn;
	ST_CHAR  	buffer[256];
	MIT_ASSOC_INFO *pstMitAssoc;
	MIC_WR_DV_CTRL 	*mic_wr_dv_ctrl;
	MIC_DV_WR_INFO	*pstCurDvWrInfo;
	MIC_DV_WR_INFO  *pstDvWrInfo;
	MIC_WR_DV_CTRL  *pstDvWrCtrl;
	MIT_ASSOC_INFO	*pstAssociationInfo;
	MIT_MIC_VCC	  *pstMitMicVcc;
	MIT_INFO_MSG *pstMitInfoMsg;
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;
	int		num_dv=0;
	ST_INT		dv_scope=1;
	int		nDst;
	inf_n = 0;
	nDst = 0;
	pstDvWrInfo = (MIC_DV_WR_INFO *)chk_calloc (1, sizeof(MIC_DV_WR_INFO));
	pstAssociationInfo = getAssociationName (buffer);
	/*
	pstAssociationInfo = sAssocDat[nDst][0].pAssocInf;
	pstMitMicVcc = sAssocDat[nDst][0].pMitMisVcc;
	if((pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE) && (nAssocNum[nDst] == 2))
	{
		pstAssociationInfo = sAssocDat[nDst][1].pAssocInf;
		pstMitMicVcc = sAssocDat[nDst][1].pMitMisVcc;
	}
	*/
	/*return if association not ACTIVE added 2003.7.9 zy */
	if (pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE)
	{
		return FALSE;
	}
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(medef.ptr[pntnum].dev_id == 0)
		{
			continue;
		}
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr,&stat,&value)==-1)
		{
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			strcpy(dvname, medef.iccp_name[pntnum]);
			pstDv = mic_find_dv (pstMitMicVcc->pstMicVcc, dv_name, dv_scope); 
			if (!pstDv)
			{
			/* Signal not found */
				sprintf(infobuf,"iccp: - signal '%s' %s not found - check configuration file\n", dvname,medef.name[pntnum]);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}

		 /* Calculate the pointer to the current DV_WR_INFO		*/
		  pstCurDvWrInfo = pstDvWrInfo + num_dv;
		  pstCurDvWrInfo->mic_dv = pstDv;
		  pstCurDvWrInfo->wr_data = chk_calloc (1, sizeof(MI_DV_DATA_UNION));
			pData = (MI_DV_DATA_UNION *)&(pstCurDvWrInfo->wr_data);

			pData->rq.Flags = stat;
			pData->rq.Value = value;
		  num_dv++;
		  pstDvWrInfo = (MIC_DV_WR_INFO *)chk_realloc (pstDvWrInfo,
									  ((num_dv+1) * sizeof(MIC_DV_WR_INFO)));

			sprintf(infobuf,"iccp: - %s %s - RealQ - %5.3f - %02x databand %5.3f\n", dvname, medef.name[pntnum], pData->rq.Value, pData->rq.Flags,
				medef.databand[pntnum]);
			WriteDspInfo(infobuf);
		}
	}//end for
  if (num_dv==0)		/* we didn't have any change	*/
    {
		chk_free (pstDvWrInfo);
		return (TRUE);
    }
  else
    {
		pstDvWrCtrl = (MIC_WR_DV_CTRL *)chk_calloc (1, sizeof(MIC_WR_DV_CTRL));
		pstDvWrCtrl->num_dv = num_dv;
		pstDvWrCtrl->mic_dv_wr_info = pstDvWrInfo;
      if (pstDvWrCtrl!=NULL)
        {
        mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
        req = mic_write_dv (mi_conn, pstDvWrCtrl);
        if (req==NULL)
          {
          MIT_LOG_FLOW0 ("Write DV failed");
          }
        else
          {
          MIT_LOG_FLOW0 ("Write DV issued");
          //req->u_req_done = writeRemoteDataValuesDone;
          //decReqCapacity(pstMitAssoc);
          //req->user_info[0] = pstMitAssoc;
          }
        }
      }

	Call_MI_Service();
	return(true);
}

int	SendMeasureValueNC()
{
	ST_CHAR			dvname[256];
	ST_BOOLEAN		dataEntered;
	MIS_DV_REF		dv_ref;
	MIS_DV			*pstDv;
	MIT_DATA_VALUE	*pstMitDataValue;
	MI_DV_DATA_UNION	*pData;
	MI_REAL_Q		*realq;
	ST_INT			ret;

	unsigned char	inf_n;
	unsigned char	nLength,stat;
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	inf_n = 0;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
/*
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			sprintf(infobuf,"%s No this ANA point in the database file!\n",name_buf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}
*/
		if(medef.ptr[pntnum].dev_id == 0)
		{
/*
			sprintf(infobuf,"+++ No this ME point %s %s in the database!\n",medef.name[pntnum],medef.iccp_name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
*/
			continue;
		}
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr,&stat,&value)==-1)
		{
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			strcpy(dvname, medef.iccp_name[pntnum]);
			ret = mis_find_dv(dvname, &dv_ref);
			if (ret != SD_SUCCESS)
			{
			/* Signal not found */
				sprintf(infobuf,"iccp: - signal '%s' %s not found - check configuration file\n", dvname,medef.name[pntnum]);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}

			dv_ref = (dv_ref & MIS_INDEX_MASK);
			pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
			pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
			pData = (MI_DV_DATA_UNION *)&pstMitDataValue->data;

/*
			pData->rq.Flags = (ST_UCHAR)((pSigTASE2->nValBits << 6) + (pSigTASE2->nValidity << 4));
*/
			pData->rq.Flags = stat;
			pData->rq.Value = value;
			mis_dv_change(pstMitDataValue->dvRef);

			organastbuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 OV */
			organafbuf[pntnum] = value;
			inf_n++;
/*
			if(inf_n >= SPONT_ME_FLOAT_NUM)
			{
				inf_n = 0;
			}
*/
			sprintf(infobuf,"iccp: - %s %s - RealQ - %5.3f - %02x databand %5.3f\n", dvname, medef.name[pntnum], pData->rq.Value, pData->rq.Flags,
				medef.databand[pntnum]);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
	}
	Call_MI_Service();
	return(true);
}


int	SendMeasureValueNCCyc()
{
	ST_CHAR			dvname[256];
	ST_BOOLEAN		dataEntered;
	MIS_DV_REF		dv_ref;
	MIS_DV			*pstDv;
	MIT_DATA_VALUE	*pstMitDataValue;
	MI_DV_DATA_UNION	*pData;
	MI_REAL_Q		*realq;
	ST_INT			ret;

	unsigned char	inf_n;
	unsigned char	nLength,stat;
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	float		value;

	endnum = medef.number;

	inf_n = 0;
	for(pntnum=0;pntnum<endnum;pntnum++)
	{
/*
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			organafbuf[pntnum] = 0;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			sprintf(infobuf,"%s No this ANA point in the database file!\n",name_buf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			organafbuf[pntnum] = 0;
			continue;
		}
*/
		if(medef.ptr[pntnum].dev_id == 0)
		{
			organafbuf[pntnum] = 0;
			sprintf(infobuf,"+++ No this ME point %s %s in the database!\n",medef.name[pntnum],medef.iccp_name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr,&stat,&value)==-1)
		{
			organafbuf[pntnum] = 0;
/*
			sprintf(infobuf,"+++ No this ME point %s %s in the database!\n",medef.name[pntnum],medef.iccp_name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
*/
			continue;
		}
		strcpy(dvname, medef.iccp_name[pntnum]);
		ret = mis_find_dv(dvname, &dv_ref);
		if (ret != SD_SUCCESS)
		{
		/* Signal not found */
			sprintf(infobuf,"iccp: - signal '%s' %s not found - check configuration file\n", dvname,medef.name[pntnum]);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}

		dv_ref = (dv_ref & MIS_INDEX_MASK);
		pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
		pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
		pData = (MI_DV_DATA_UNION *)&pstMitDataValue->data;

/*
		pData->rq.Flags = (ST_UCHAR)((pSigTASE2->nValBits << 6) + (pSigTASE2->nValidity << 4));
*/
		pData->rq.Flags = stat;
		pData->rq.Value = value;
/*		mis_dv_change(pstMitDataValue->dvRef); */

		organastbuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 OV */
		organafbuf[pntnum] = value;
		inf_n++;

#ifdef DEBUG
		sprintf(infobuf,"iccp: Cyc Send- %s %s - RealQ - %5.3f - %02x\n", dvname, medef.name[pntnum], pData->rq.Value, pData->rq.Flags);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
#endif
	}
	return(true);
}

int	SendUserData()
{
	int ret;
/*
	ret = SendSinglePointInform();

	ret = SendDoublelePointInform();
*/
	ICCP_SOE_Rec();

	ret = SendStateInformSOE();

	ret = SendMeasureValueNC();
	if(ret == false)
		return(ret);

	return(true);
}

int	GetSPStatById(POINTER *ptr,unsigned char *pstat)
{
	DMS_COMMON	dbentry;
	unsigned char	stat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		sprintf(infobuf,"No this SP point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		*pstat =0;	/* QDS IV NT SB BL RES3 SPI */
		return(-1);
	}
	if(dbentry.var.ind.status.rt_state==1)
		stat = 0x80;
	else
		stat = 0x40;

/*	if(dbentry.var.ind.status.man_set)
		stat = stat | 0x20;	*/		/* SB=1 */
	if(dbentry.var.ind.status.chan_fail)
		stat = stat | 0x10;			/* IV=1; NT=1 */
/*	else if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;	*/		/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES3 SPI */
	return(0);
}

int	GetDPStatById(POINTER *ptr,unsigned char *pstat)
{
	DMS_COMMON	dbentry;
	unsigned char	stat;
	IND_DATA	dbstat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		sprintf(infobuf,"No this DP point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		*pstat =0;	/* QDS IV NT SB BL RES2 DPI */
		return(-1);
	}
	dbstat=dbentry.var.ind.status;
	if(dbstat.state == 0)
		stat = 0x40;	/* OFF:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 1)
		stat = 0x80;	/* ON:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 2)
		stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
	else
		stat = 0xC0;	/* Error:QDS IV NT SB BL RES2 DPI */

/*	if(dbstat.man_set)
		stat = stat | 0x20;		*/	/* SB=1 */
	if(dbstat.chan_fail)
		stat = stat | 0x10;			/* IV=1; NT=1 */
/*	else if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;		*/	/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES2 DPI */
	return(0);
}

int	GetMEValueById(POINTER *ptr,unsigned char *pstat,float *pValue)
{
	DMS_COMMON	dbentry;
	unsigned char	stat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		sprintf(infobuf,"No this ME point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		*pstat = 0;
		*pValue = 0.0f;
		return(-1);
	}
	if(ptr->data_type == IMP_TYPE)
	{
		*pValue=dbentry.var.imp.entry.d_accum;
		
		stat = 0;
/*		if(dbentry.var.imp.entry.status.man_set)
			stat = stat | 0x20;	*/	/* SB=1 */
		if(dbentry.var.imp.entry.status.chan_fail)
			stat = stat | 0x10;		/* IV=1; NT=1 */
		else if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		*pValue=(float)dbentry.var.anlg.value.data/1000.0;
		stat = 0;
/*		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;	*/	/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0x10;		/* IV=1; NT=1 */
/*		else if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;	*/	/* BL=1 */

	}
	*pstat = stat;
	return(0);
}

int	GetSPStatByName(char *pName,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		sprintf(infobuf,"%s No this SP point in the database file!\n",name_buf);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		*pstat =0;	/* QDS IV NT SB BL RES3 SPI */
		return(-1);
	}
	if(dbentry.var.ind.status.rt_state==1)
		stat = 0x80;
	else
		stat = 0x40;

/*	if(dbentry.var.ind.status.man_set)
		stat = stat | 0x20;	*/		/* SB=1 */
	if(dbentry.var.ind.status.chan_fail)
		stat = stat | 0x10;			/* IV=1; NT=1 */
/*	else if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;	*/		/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES3 SPI */
	return(0);
}

int	GetDPStatByName(char *pName,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;
	IND_DATA	dbstat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		sprintf(infobuf,"%s No this DP point in the database file!\n",name_buf);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		*pstat =0;	/* QDS IV NT SB BL RES2 DPI */
		return(-1);
	}
	dbstat=dbentry.var.ind.status;
	if(dbstat.state == 0)
		stat = 0x40;	/* OFF:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 1)
		stat = 0x80;	/* ON:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 2)
		stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
	else
		stat = 0xC0;	/* Error:QDS IV NT SB BL RES2 DPI */

/*	if(dbstat.man_set)
		stat = stat | 0x20;		*/	/* SB=1 */
	if(dbstat.chan_fail)
		stat = stat | 0x10;			/* IV=1; NT=1 */
/*	else if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;		*/	/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES2 DPI */
	return(0);
}

int	GetMEValueByName(char *pName,unsigned char *pstat,float *pValue)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		sprintf(infobuf,"%s No this ANA point in the database file!\n",name_buf);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		*pstat = 0;
		*pValue = 0.0f;
		return(-1);
	}
	if(dbentry.point.data_type == IMP_TYPE)
	{
		*pValue=dbentry.var.imp.entry.rt_value;
		stat = 0;
/*		if(dbentry.var.imp.entry.status.man_set)
			stat = stat | 0x20;	*/	/* SB=1 */
		if(dbentry.var.imp.entry.status.chan_fail)
			stat = stat | 0x10;		/* IV=1; NT=1 */
		else if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		*pValue=(float)dbentry.var.anlg.value.data/1000.0;
		stat = 0;
/*		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;	*/	/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0x10;		/* IV=1; NT=1 */
/*		else if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;	*/	/* BL=1 */

	}
	*pstat = stat;
	return(0);
}

int	GetIMPValueByName(char *pName,unsigned char *pstat,unsigned int *pValue)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		sprintf(infobuf,"%s No this IMP point in the database file!\n",name_buf);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		*pstat = 0;
		*pValue = 0;
		return(-1);
	}
	if(dbentry.point.data_type == IMP_TYPE)
	{
		*pValue=dbentry.var.imp.entry.rt_value;
		stat = 0;
/*		if(dbentry.var.imp.entry.status.man_set)
			stat = stat | 0x20;	*/	/* SB=1 */
		if(dbentry.var.imp.entry.status.chan_fail)
			stat = stat | 0x10;		/* IV=1; NT=1 */
		else if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		*pValue=(float)dbentry.var.anlg.value.data/1000.0;
		stat = 0;
/*		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;	*/	/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0x10;		/* IV=1; NT=1 */
/*		else if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;	*/	/* BL=1 */
	}
	*pstat = stat;
	return(0);
}
/*
void	PrintSendData(unsigned int nbsend)
{
	unsigned int d;

	for(d=0;d<nbsend;d++)
	{
		if((d%20)==0)
		{
			sprintf(infobuf,"\n<< ");
#ifdef DEBUG
			printf("\n<< ");
#endif
			WriteDspInfo(infobuf);
		}
		else if((d%10)==0)
		{
			sprintf(infobuf," ");
#ifdef DEBUG
			printf(" ");
#endif
			WriteDspInfo(infobuf);
		}
		sprintf(infobuf," %02X",sendbuf[d]);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	sprintf(infobuf,"\n");
#ifdef DEBUG
	printf("\n");
#endif
	WriteDspInfo(infobuf);
}

void	PrintRecvData(unsigned int nbrecv)
{
	unsigned int	d;

	for(d=0;d<nbrecv;d++)
	{
		if((d%20)==0)
		{
			sprintf(infobuf,"\n>> ");
#ifdef DEBUG
			printf("\n>> ");
#endif
			WriteDspInfo(infobuf);
		}
		else if((d%10)==0)
		{
			sprintf(infobuf," ");
#ifdef DEBUG
			printf(" ");
#endif
			WriteDspInfo(infobuf);
		}
		sprintf(infobuf," %02X",recvbuf[d]);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	sprintf(infobuf,"\n");
#ifdef DEBUG
	printf("\n");
#endif
	WriteDspInfo(infobuf);
}
*/

