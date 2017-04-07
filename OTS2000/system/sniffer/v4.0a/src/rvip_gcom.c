#include	<time.h>
#include	<stdlib.h>
#include	<stdio.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../inc/Sniffer.h"
#include	"../inc/rvip_def.h"
#include	"../inc/gcom_def.h"





void*	InitIPC(char key_char);
int		InitPtTable(void);
void	GetByteFromBuffer(char *buff, int start_pos, int length, char *data);
void	IncreaseReadPoint(int *readPoint, int increase);
void	PrintRecvData(unsigned char *data, int length);
void	VipAnalysis(char *recv_buf, int length, VIP_M *vip_m);
void	GcomAnalysis(char *recv_buf, int length);



#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/dpslib.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")

#define MAX_ABB_DBNAME_SIZE	48
#define PT_NUM_PER_DS		384		
#define DS_NUM_PER_PACKET	10
#define MAX_PACKET_NUM		32

#define MAX_POINT_NUM		50000  //不能超过65535

typedef struct 
{
	char	abb_dbname[MAX_ABB_DBNAME_SIZE];
	char	dbname[POINT_NAME_SIZE+1];
	POINTER	pt;
} PT_INFO;

typedef struct
{
	PT_INFO	pt[PT_NUM_PER_DS];
} DS_DEF;

typedef struct
{
	DS_DEF	ds[DS_NUM_PER_PACKET];
} PACKET_DEF;

typedef struct
{
	PACKET_DEF	packet[MAX_PACKET_NUM];
} PT_TABLE;

typedef struct  
{
	UCHAR	packet_id;
	UCHAR	ds_id;
	USHORT	offset;
	USHORT	hash_index;
	USHORT	hash_next;
	GULONG	hash_key;
} INDEX;

typedef struct 
{
	USHORT	hash_pn;
	UINT	pt_num;
	INDEX	index[MAX_POINT_NUM];
} INDEX_TABLE;


INDEX_TABLE	index_table;
PT_TABLE	pt_table;	


#define RVIP_GCOM_PT_TABLE_FILE "rvip_gcom_pt_table.def"

#define	DEBUG



void aa(void)
{
	char	strbuf[256];
	FILE *fp, *fp1;
	
	fp= fopen("et_pt_table.txt", "r");
	if(fp==NULL)
		return;
	fp1= fopen("et_pt_table_new.txt", "w");
	if(fp1==NULL)
	{
		fclose(fp);
		return;
	}

	while( NULL != fgets(strbuf, sizeof(strbuf), fp) )
	{
		fprintf(fp1, "ET.F1.POL.%s", strbuf);
	}
	fclose(fp);
	fclose(fp1);
}








int	main(int argc, char *argv[])
{
	SHM_SNIFFER	*vip_data_seg, *gcom_data_seg;

// aa();
// exit(0);


	SetModuleName("RVIP");
	SetErrLogMode(argc, argv);
	CreateLockFile(".rvip.lock");

	if(-1==MountDB(NULL))
	{
		Err_log("Fail to MountDB!\n");
		exit(-1);
	}
	
	if(-1==lan_out_init())
	{
		Err_log("Fail to lan_out_init!\n");
		exit(-1);
	}
	
	if((SHM_SNIFFER*)-1==(vip_data_seg=(SHM_SNIFFER*)LinktoShareMem(SHM_SNIFFER_VIP_KEY)))
	{
		Err_log("Fail to get VIP recv data seg shm!\n");
		exit(-1);
	}
	else
		Err_log("Success to get VIP recv data seg shm!\n");
	
	if((SHM_SNIFFER*)-1==(gcom_data_seg=(SHM_SNIFFER*)LinktoShareMem(SHM_SNIFFER_GCOM_KEY)))
	{
		Err_log("Fail to get VIP recv data seg shm!\n");
		exit(-1);
	}
	else
		Err_log("Success to get VIP recv data seg shm!\n");
	
	if(-1==InitPtTable())
	{
		Err_log("Fail to Init db index table!\n");
		exit(-1);
	}
	
	Err_log("Start to message process\n");

	vip_data_seg->header.readPoint= vip_data_seg->header.writePoint;
	gcom_data_seg->header.readPoint= gcom_data_seg->header.writePoint;

	for(;;)
	{
		while(vip_data_seg->header.readPoint!=vip_data_seg->header.writePoint || gcom_data_seg->header.readPoint!=gcom_data_seg->header.writePoint)
		{
			int		pkg_length;

			SendTraceMsg(0,0, "GCOM curr readPos= %d writePos= %d", gcom_data_seg->header.readPoint, gcom_data_seg->header.writePoint);
			if(gcom_data_seg->header.readPoint!=gcom_data_seg->header.writePoint)
			{
				//VIP_M	vip_m;
				int		pkg_length;
				
				pkg_length= gcom_data_seg->packageLength[gcom_data_seg->header.readPoint];
#ifdef DEBUG
				SendTraceMsg(0,0,"GCOM message length: %d", pkg_length);
				PrintRecvData(gcom_data_seg->buf[gcom_data_seg->header.readPoint], pkg_length);
#endif

				if(pkg_length>sizeof(gcom_data_seg->buf[gcom_data_seg->header.readPoint]) || pkg_length<14)
					SendTraceMsg(0, 0, "GCOM recv packet length %d error: max buff size %d", pkg_length, sizeof(gcom_data_seg->buf[gcom_data_seg->header.readPoint]));
				else
				{
					GcomAnalysis(gcom_data_seg->buf[gcom_data_seg->header.readPoint], pkg_length);
				}
				
				if(gcom_data_seg->header.readPoint>=(BufferRecordCount-1))
					gcom_data_seg->header.readPoint= 0;
				else
					gcom_data_seg->header.readPoint++;
				

			}

			SendTraceMsg(0,0, "VIP curr readPos= %d writePos= %d", vip_data_seg->header.readPoint, vip_data_seg->header.writePoint);
			if(vip_data_seg->header.readPoint!=vip_data_seg->header.writePoint)
			{
				VIP_M	vip_m;
				
				pkg_length= vip_data_seg->packageLength[vip_data_seg->header.readPoint];
#ifdef DEBUG
				SendTraceMsg(0,0,"VIP message length: %d", pkg_length);
				PrintRecvData(vip_data_seg->buf[vip_data_seg->header.readPoint], pkg_length);
#endif
				
				if(pkg_length>sizeof(vip_data_seg->buf[vip_data_seg->header.readPoint]) || pkg_length<14)
					SendTraceMsg(0, 0, "VIP recv packet length %d error: max buff size %d", pkg_length, sizeof(vip_data_seg->buf[vip_data_seg->header.readPoint]));
				else
				{
					VipAnalysis(vip_data_seg->buf[vip_data_seg->header.readPoint], pkg_length, &vip_m);
				}
				
				if(vip_data_seg->header.readPoint>=(BufferRecordCount-1))
					vip_data_seg->header.readPoint= 0;
				else
					vip_data_seg->header.readPoint++;
				

			}

		}

		Sleep(10);
	}

}

//m_date: number of days from 1980-01-01 m_time: number of 0.1 ms from midnight
void ConvertTime(UINT m_date, UINT m_time, DATE_TIME *date_time)
{
	struct tm tm_time;

	tm_time.tm_year= 80;
	tm_time.tm_mon= 0;
	tm_time.tm_mday= m_date;
	tm_time.tm_hour= 0;
	tm_time.tm_min= 0;
	tm_time.tm_sec= m_time/10000;
	tm_time.tm_isdst= 0;
		
	mktime(&tm_time);

	date_time->year= tm_time.tm_year+1900;
	date_time->month= tm_time.tm_mon+1;
	date_time->day= tm_time.tm_mday;
	date_time->hour= tm_time.tm_hour;
	date_time->minute= tm_time.tm_min;
	date_time->second= tm_time.tm_sec;
	date_time->ms= ((m_time+5)/10)%1000;
	

}


int GetDataType(VIP_M *vip_m)
{
	if(vip_m->pack_id==1)
	{
		vip_m->type= AI_TYPE;
		return 0;
	}
	else if(vip_m->pack_id==2)
	{
		vip_m->type= AI_TYPE;
		return 0;
	}
	else if(vip_m->pack_id==3)
	{
		vip_m->type= DI_TYPE;
		return 0;
	}
	return -1;
}


int GetPtNameByAbbDbName(char *abb_dbname, char *pt_name, POINTER *pt)
{
	GULONG		key;
	USHORT		index, hash_pn, hash_val;
	
	if(index_table.hash_pn==0)
		return -1;

	hash_pn= index_table.hash_pn;
	
	GetNameStrHash(abb_dbname, &key);
	hash_val= key%hash_pn;
	
	index= index_table.index[hash_val].hash_index;
	if(index>=MAX_POINT_NUM)
		return -1;
	
	while(key!=index_table.index[index].hash_key 
		|| strcmp(abb_dbname, pt_table.packet[index_table.index[index].packet_id-1]. \
			ds[index_table.index[index].ds_id-1].pt[index_table.index[index].offset].abb_dbname))
	{
		if(index_table.index[hash_val].hash_next==0xFFFF)
			return -1;
		hash_val= index_table.index[hash_val].hash_next;
		index= index_table.index[hash_val].hash_index;
		if(index>=MAX_POINT_NUM)
			return -1;
	}
	if(index>=MAX_POINT_NUM)
		return -1;

	if(pt_name)
		strcpy(pt_name, pt_table.packet[index_table.index[index].packet_id-1]. \
				ds[index_table.index[index].ds_id-1].pt[index_table.index[index].offset].dbname);
	*pt= pt_table.packet[index_table.index[index].packet_id-1]. \
			ds[index_table.index[index].ds_id-1].pt[index_table.index[index].offset].pt;
	return 0;
}

void GcomAnalysis(char *recv_buf, int length)
{
	GCOM_DL_HEADER	dl_header;
	GCOM_DATA_HEADER data_header;
	GCOM_DATA_ADDR	data_addr;
	GCOM_EVENT_DATA	data_body;
	UINT	m_date, m_time;
	DATE_TIME	date_time;
	POINTER	pt;
	USHORT	state;
	char	*state_str[3]= {"正常", "报警", "错误"};

	printf("\ndl_header size= %d, data_header size= %d, data_addr size= %d",
		sizeof(GCOM_DL_HEADER), sizeof(GCOM_DATA_HEADER), sizeof(GCOM_DATA_ADDR));

	printf("\nGCOM_EVENT_DATA size= %d", sizeof(GCOM_EVENT_DATA));

	if(length<20)
	{
		SendTraceMsg(0,0,"recv data length too short: %d", length);
		return;
	}

	memcpy(&dl_header, recv_buf, 17);
	dl_header.length= ntohs(dl_header.length);
	if(length<(dl_header.length+14))
	{
		SendTraceMsg(0,0,"recv data length error: length= %d dl.length= %d", length, dl_header.length);
		return;
	}
	memcpy(&data_header, recv_buf+17, 3);
	SendTraceMsg(0,0,"recv data header: type= %d prio= %d node= %d seq= %d", 
		data_header.type_prio.type, data_header.type_prio.prio, data_header.node, data_header.seq);

	if(data_header.type_prio.type != GCOM_DATA)
	{
		SendTraceMsg(0,0,"recv message isn't DATA message: type= %d", data_header.type_prio.type);
		return;
	}

	memcpy(&data_addr, recv_buf+20, 14);
	data_addr.dest_ch_id= ntohs(data_addr.dest_ch_id);
	data_addr.resp_ch_id= ntohs(data_addr.resp_ch_id);
	data_addr.size= ntohs(data_addr.size);

	SendTraceMsg(0,0,"data dest_ch_id= %d resp_ch_id= %d id= %d size= %d", 
		data_addr.dest_ch_id, data_addr.resp_ch_id, data_addr.id, data_addr.size);

	if(length<(data_addr.size+32))
	{
		SendTraceMsg(0,0,"recv data length error: length= %d data.size= %d", length, data_addr.size);
		return;
	}
	if(data_addr.dest_ch_id != CHAN_ID_PROC_EVENT && data_addr.dest_ch_id != CHAN_ID_SYSTEM_EVENT)
	{
		SendTraceMsg(0,0,"recv data ch_id isn't event: %d", data_addr.dest_ch_id);
		return;
	}

	if(data_addr.dest_ch_id == CHAN_ID_PROC_EVENT)
	{
		if(data_addr.id != SIGNAL_ID_PROC_EVENT)
		{
			SendTraceMsg(0,0,"recv data id isn't process event: %d", data_addr.id);
			return;
		}

		memcpy(&data_body, recv_buf+34, sizeof(data_body));
		data_body.symname_length= ntohs(data_body.symname_length);
		data_body.descr_length= ntohs(data_body.descr_length);
		data_body.value_TR= ntohs(data_body.value_TR);
		
		if(data_body.symname_length>20)
		{
			SendTraceMsg(0,0,"recv symname length error: %d", data_body.symname_length);
			return;
		}
		if(data_body.descr_length>28)
		{
			SendTraceMsg(0,0,"recv descr length error: %d", data_body.descr_length);
			return;
		}
		data_body.symname[data_body.symname_length]= 0;
		data_body.descr[data_body.descr_length]= 0;
		
		memcpy(&m_date, &data_body.date, 4);
		m_date= ntohl(m_date);
		memcpy(&m_time, &data_body.time, 4);
		m_time= ntohl(m_time);
		ConvertTime(m_date, m_time, &date_time);
		
		if(data_body.unkown1[10]==0x04 && data_body.unkown1[15]==0x02)
			state= 0; //norm
		else if(data_body.unkown1[10]==0x03 && data_body.unkown1[15]==0x01)
			state= 1; //alarm
		else
			state= 2; //error
		
		SendTraceMsg(0,0,"RECV MESSAGE: %s %s %s %04d-%02d-%02d %02d:%02d:%02d.%03d", 
			data_body.symname, data_body.descr, state_str[state],
			date_time.year, date_time.month, date_time.day, date_time.hour,
			date_time.minute, date_time.second, date_time.ms);

		if(-1==GetPtNameByAbbDbName(data_body.symname, NULL, &pt))
		{
			SendTraceMsg(0,0,"Fail to find %s in define table", data_body.symname);
			return;
		}

		BroadCastQualityIndPtState(pt, 0, state, 0, &date_time);
	}
	else if(data_addr.dest_ch_id == CHAN_ID_SYSTEM_EVENT)
	{

	}
}


void VipAnalysis(char *recv_buf, int length, VIP_M *vip_m)
{
	int i,j;
	unsigned short i_num,temp;
	u_long			ltemp;
	DATE_TIME		gtime;
	POINTER			pt;

	memset (vip_m, 0, sizeof(VIP_M));
	
	memcpy((char *)&temp,&recv_buf[0],2);
	vip_m->size = ntohs(temp);
	memcpy((char *)&temp,&recv_buf[2],2);
	vip_m->ch_id = ntohs(temp);
	memcpy((char *)&temp,&recv_buf[4],2);
	vip_m->pack_id = ntohs(temp);
	memcpy((char *)&temp,&recv_buf[6],2);
	vip_m->no_group = ntohs(temp);
	memcpy((char *)&temp,&recv_buf[8],2);
	vip_m->v_flag = ntohs(temp);
	memcpy((char *)&temp,&recv_buf[10],2);
	vip_m->s_tick = ntohs(temp);
	memcpy((char *)&temp,&recv_buf[12],2);
	vip_m->reservered = ntohs(temp);
	
	SendTraceMsg(0, 0, "size= %d ch_id= %d pack_id= %d no_group= %d", vip_m->size, vip_m->ch_id, vip_m->pack_id, vip_m->no_group);
	if(vip_m->size != length)
	{
		SendTraceMsg(0, 0, "error: message size %d isn't equal to length", vip_m->size, length);
		return;
	}

	if(vip_m->pack_id<1 || vip_m->pack_id>MAX_PACKET_NUM)
	{
		SendTraceMsg(0,0,"recv packet id error: %d 1~%d", vip_m->pack_id, MAX_PACKET_NUM);
		return;
	}

	GetSysTime(&gtime);
	if(-1==GetDataType(vip_m))
	{
		SendTraceMsg(0, 0, "Fail to get packet data type: pack_id=%d", vip_m->pack_id);
		return;
	}

	if(vip_m->type==AI_TYPE)
	{
		if(length != (54+94*vip_m->no_group))
		{
			SendTraceMsg(0, 0, "error: message length %d isn't equal to 54+no_group*94", length);
			return;
		}
		
		for (i=0;i<10;i++) 
		{
			memcpy((char *)&ltemp, &recv_buf[14+i*4], 4);
			vip_m->quality[i] = ntohl(ltemp);
		}
		
		if(vip_m->no_group!=10)
		{
			SendTraceMsg(0, 0, "error: message group num %d isn't 10", vip_m->no_group);
			return;
		}
		
		for (i=0;i<vip_m->no_group;i++) 
		{
			int	pt_num;
			char info_buf[2048];

			memcpy ((char *)&temp, &recv_buf[54 + i * 94], 2);
			i_num = ntohs(temp);
			if(i_num!=MAX_VIP_AI_NUM_IN_GROUP)
			{
				SendTraceMsg(0, 0, "error: ai num %d in group %d isn't %d", i_num, i+1, MAX_VIP_AI_NUM_IN_GROUP);
				return;
			}

			pt_num= 0;
			for (j=0;j<i_num;j++) 
			{
				USHORT chan_fail;
				float	value;

				if(pt_table.packet[vip_m->pack_id-1].ds[i].pt[j].dbname[0]==0)
					continue;

				memcpy ((char *)&ltemp, &recv_buf[56 + i * 94 + j * 4], 4);
				ltemp= ntohl(ltemp);
				memcpy(&value, &ltemp, 4);

				chan_fail= (vip_m->quality[i]>>j) & 0x01;

				pt= pt_table.packet[vip_m->pack_id-1].ds[i].pt[j].pt;

				FillAnaAllSendInfo(pt, value, chan_fail, &gtime, (SP_ANA_INFO_PKG*)&info_buf[pt_num*sizeof(SP_ANA_INFO_PKG)]);
				pt_num++;
			}

			if(pt_num>0)
				BroadCastDataInfo(ALL_DB_TIME, info_buf, pt_num*sizeof(SP_ANA_INFO_PKG), 0);
		}

	}
	else if(vip_m->type==DI_TYPE)
	{
		UINT	quality;

		if(length != (54+94*vip_m->no_group))
		{
			SendTraceMsg(0, 0, "error: message length %d isn't equal to 54+no_group*94", length);
			return;
		}
		
		if(vip_m->no_group!=10)
		{
			SendTraceMsg(0, 0, "error: message group num %d isn't 10", vip_m->no_group);
			return;
		}
		

		for (i=0;i<vip_m->no_group;i++) 
		{
			memcpy((char *)&ltemp, &recv_buf[14+i*4], 4);
			quality = ntohl(ltemp);

			memcpy ((char *)&temp, &recv_buf[54 + i * 94], 2);
			i_num = ntohs(temp);
			if(i_num!=MAX_VIP_AI_NUM_IN_GROUP)
			{
				SendTraceMsg(0, 0, "error: ai num %d in group %d isn't %d", i_num, i+1, MAX_VIP_AI_NUM_IN_GROUP);
				return;
			}

			for (j=0;j<12;j++) 
			{
				int pt_num;
				char info_buf[2048];

				int k;
				memcpy ((char *)&ltemp, &recv_buf[56 + i * 94 + j * 8], 4);
				ltemp= ntohl(ltemp);

				pt_num= 0;
				for(k=0;k<32;k++)
				{
					USHORT state, chan_fail;

					if(pt_table.packet[vip_m->pack_id-1].ds[i].pt[j].dbname[0]==0)
						continue;
					
					state= (USHORT)((ltemp >> k) & 0x01);
					chan_fail= (quality >> k) & 0x01;

					quality= (vip_m->quality[i]>>j) & 0x01;

					pt= pt_table.packet[vip_m->pack_id-1].ds[i].pt[j*32+k].pt;

					FillIndAllSendInfo(pt, state, chan_fail, &gtime, (SP_IND_ALLSEND_PKG*)&info_buf[pt_num*sizeof(SP_IND_ALLSEND_PKG)]);
				}

				if(pt_num>0)
					BroadCastDataInfo(ALL_DB_TIME, info_buf, pt_num*sizeof(SP_IND_ALLSEND_PKG), 0);

				if(j<11)
				{
					memcpy((char *)&ltemp, &recv_buf[56 + i * 94 + j * 8 + 4], 4);
					quality = ntohl(ltemp);
				}
			}
		}
	}
}



void	PrintRecvData(unsigned char *data, int length)
{
	int d;
	

	printf("\n");
	for(d=1;d<=length;d++)
	{
		printf("%02x ",data[d-1]);
		if((d%10)==0)
			printf("\n");
	}
	printf("\n");
}


int MakeEntryHashTable(void)
{
	GULONG	id;
	USHORT	hash_val, idle_pos;
	GULONG	key;
	USHORT	hash_pn;
	UCHAR	pack_id;
	UCHAR	ds_id;
	USHORT	offset;
	
	hash_pn= FindLastPrimeNumber(MAX_POINT_NUM);
	index_table.hash_pn= hash_pn;
	
	for(id=0; id<MAX_POINT_NUM; id++)
	{
		index_table.index[id].hash_index= 0xFFFF;
		index_table.index[id].hash_next= 0xFFFF;
	}
	
	for(id=0; id<index_table.pt_num; id++)
	{
		pack_id= index_table.index[id].packet_id;
		ds_id= index_table.index[id].ds_id;
		offset= index_table.index[id].offset;

		GetNameStrHash(pt_table.packet[pack_id-1].ds[ds_id-1].pt[offset].abb_dbname, &index_table.index[id].hash_key);
		key= index_table.index[id].hash_key;
		hash_val= key%hash_pn;
		if(index_table.index[hash_val].hash_index!=0xFFFF)
			continue;
		index_table.index[hash_val].hash_index= id;
	}
	
	idle_pos=0;
	for(id=0; id<index_table.pt_num; id++)
	{
		char	*id_name, *hash_val_name;
		
		pack_id= index_table.index[id].packet_id;
		ds_id= index_table.index[id].ds_id;
		offset= index_table.index[id].offset;			
		id_name= pt_table.packet[pack_id-1].ds[ds_id-1].pt[offset].abb_dbname;
		
		key= index_table.index[id].hash_key;
		hash_val= key%hash_pn;
		if(index_table.index[hash_val].hash_index==id)
		{
			continue;
		}
		for(; idle_pos<MAX_POINT_NUM; idle_pos++)
		{
			if(index_table.index[idle_pos].hash_index==0xFFFF)
				break;
		}
		if(idle_pos>=MAX_POINT_NUM)
		{
			Err_log("Make str text hash table error");
			exit(0);
		}
		index_table.index[idle_pos].hash_index= id;
		while(index_table.index[hash_val].hash_next!=0xFFFF)
		{
			pack_id= index_table.index[index_table.index[hash_val].hash_index].packet_id;
			ds_id= index_table.index[index_table.index[hash_val].hash_index].ds_id;
			offset= index_table.index[index_table.index[hash_val].hash_index].offset;			
			hash_val_name= pt_table.packet[pack_id-1].ds[ds_id-1].pt[offset].abb_dbname;

			if(!strcmp(id_name, hash_val_name))
				SendTraceMsg(0,0,"Repeated abb_dbname in define file: %s", id_name); 
			
			hash_val= index_table.index[hash_val].hash_next;
		}
		index_table.index[hash_val].hash_next= idle_pos;

		pack_id= index_table.index[index_table.index[hash_val].hash_index].packet_id;
		ds_id= index_table.index[index_table.index[hash_val].hash_index].ds_id;
		offset= index_table.index[index_table.index[hash_val].hash_index].offset;			
		hash_val_name= pt_table.packet[pack_id-1].ds[ds_id-1].pt[offset].abb_dbname;
		
		if(!strcmp(id_name, hash_val_name))
			SendTraceMsg(0,0,"Repeated abb_dbname in define file: %s", id_name); 
	}	
	
	return 0;		
}


int InitPtTable(void)
{
	FILE	*fp;
	char	filename[256], strbuf[256];
	int		line_no=0;
	
	sprintf(filename, "%s%s", SYS_DEFINE_PATH, RVIP_GCOM_PT_TABLE_FILE);
	if((fp=fopen(filename,"r"))==NULL)
	{
		SendTraceMsg(0, 0, "Fail to open %s", filename);
		return -1;
	}
	
	memset(&pt_table, 0, sizeof(pt_table));
	memset(&index_table, 0, sizeof(index_table));

	while( NULL != fgets(strbuf, sizeof(strbuf), fp) )
	{
		char	abb_dbname[256], dbname[256];
		int		packet_id, ds_id, offset;
		POINTER	pt;

		line_no++;

		if(strbuf[0]==0 || strbuf[0]=='!' || strbuf[0]=='#'|| strbuf[0]=='\n')
			continue;
		
		if(5!=sscanf(strbuf, "%s %d %d %d %s", abb_dbname, &packet_id, &ds_id, &offset, dbname))
		{
			SendTraceMsg(0,0,"line %d format error: %s", line_no, strbuf);
			continue;
		}

		if(packet_id<1 || packet_id>MAX_PACKET_NUM)
		{
			SendTraceMsg(0,0,"line %d packet id error: %d 1~%d", line_no, packet_id, MAX_PACKET_NUM);
			continue;
		}
		if(ds_id<1 || ds_id>DS_NUM_PER_PACKET)
		{
			SendTraceMsg(0,0,"line %d dataset id in packet error: %d 1~%d", line_no, ds_id, DS_NUM_PER_PACKET);
			continue;
		}
		if(offset<0 || offset>=PT_NUM_PER_DS)
		{
			SendTraceMsg(0,0,"line %d pt offset in DS id error: %d 0~%d", line_no, offset, PT_NUM_PER_DS-1);
			continue;
		}
		if(-1==GetPtIdByNameStr(dbname, &pt))
		{
			continue;
		}
		if(strlen(abb_dbname)>=MAX_ABB_DBNAME_SIZE)
		{
			SendTraceMsg(0,0,"line %d abb dbname too long: %s", line_no, abb_dbname);
			continue;
		}
		
		if(pt_table.packet[packet_id-1].ds[ds_id-1].pt[offset].abb_dbname[0])
		{
			SendTraceMsg(0,0,"line %d id repeated: %s", line_no, strbuf);
			continue;
		}

		strcpy(pt_table.packet[packet_id-1].ds[ds_id-1].pt[offset].abb_dbname, abb_dbname);
		strcpy(pt_table.packet[packet_id-1].ds[ds_id-1].pt[offset].dbname, dbname);
		pt_table.packet[packet_id-1].ds[ds_id-1].pt[offset].pt= pt;

		index_table.index[index_table.pt_num].packet_id= packet_id;
		index_table.index[index_table.pt_num].ds_id= ds_id;
		index_table.index[index_table.pt_num].offset= offset;
		index_table.pt_num++;

		if(index_table.pt_num>=MAX_POINT_NUM)
		{
			SendTraceMsg(0,0,"Too much point num %d: max %d", index_table.pt_num, MAX_POINT_NUM);
			break;
		}
	}

	fclose(fp);

	MakeEntryHashTable();
	
	SendTraceMsg(0,0,"defined total point num: %d\n", index_table.pt_num);

	return 0;
}


