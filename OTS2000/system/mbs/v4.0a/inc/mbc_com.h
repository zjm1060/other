#define PLCDEBUG
#define MULTITHREAD
#define SOCK_TIMEOUT_S_MBS 5
#define MBPORT	502	
#define	SLAVE_ADDR	1
#define MASTER_ADDRESS 1
#define MAX_MB_AREA 6400
#define ANA_SEND_ADDR 1
#define POL_SEND_ADDR 3001
#define ANA_RECV_ADDR 3201
#define POL_RECV_ADDR 6201

#define PLC_SCAN_LEN        12
#define PLC_CMD_LEN			13
#define WR_PLC_ACK_LEN		12
#define	LINE_BUF_CHAR		256
#define MAX_ANA_SEND_NUM	1000
#define MAX_POL_SEND_NUM	100 * 16
#define MAX_ANA_RECV_NUM	1000
#define MAX_POL_RECV_NUM	100 * 16
#define MAX_MASTER_NUM		10
#define MAX_SLAVE_NUM		10
#define MAX_BUF_SIZE    SHM_DATA_SEG_SIZE
#define MBFRAMELEN    (MAX_SEND_SIZE*2+5)
#define MBFRAMELEN_RECV (MAX_RECV_SIZE*2+9)#define MAX_MB_AREA	6400


#define CONFIG_FILE			"config.def"
#define	HOMEPATH			"/home/ems/h9000/mbc/"
#define	MBC_FILE_KEY		"/home/ems/h9000/mbc"	
#define	MBS_STN_DEF_FILE	"/home/ems/h9000/mbc/mbs_station.def"
#define	MBM_STN_DEF_FILE	"/home/ems/h9000/mbc/mbm_station.def"

#define ANA_SEND_FILE		"ana_send.def"
#define ANA_RECV_FILE		"ana_recv.def"
#define POL_SEND_FILE		"pol_send.def"
#define POL_RECV_FILE		"pol_recv.def"

typedef struct
{	
	char	NameID[50];
	char	Ip[30];
	int		stnid;
	int		valid;
}MASTER_REC;

typedef struct
{	
	char	NameID[50];
	char	Ip[30];
	int		stnid;
	int		valid;
}SLAVE_REC;

typedef struct  
{
	//needed to be modified
	int tmp;
}MBS_CONFIG;

typedef	struct
{
	USHORT	number;
	char	name[MAX_POL_SEND_NUM][36];
	short	status[MAX_POL_SEND_NUM];
} POLDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_ANA_SEND_NUM][36];
	int	value[MAX_ANA_SEND_NUM];
} ANADEF;



typedef struct
{
	short mbarea[MAX_MB_AREA];
	ANADEF anas,anar;
	POLDEF pols,polr;
	int	newsock;
	int err_code;
}MBDATA;

typedef struct
{	
	unsigned char	dev_id;
	char		data_type;
	short		point_id;
	unsigned short	status;
	unsigned short	quality;
	float		fvalue;
	float		HI_limit;	  
	IOATTR		ioattr;
	short		year;
	char		month;
	char		day;
	char		hour;
	char		minute;
	char		second;
	short		ms;
	unsigned int    LongValue;
} DB_DATA;

typedef	struct
{
	short		bit0:1;
	short		bit1:1;
	short		bit2:1;
	short		bit3:1;
	short		bit4:1;
	short		bit5:1;
	short		bit6:1;
	short		bit7:1;
	short		bit8:1;
	short		bit9:1;
	short		bit10:1;
	short		bit11:1;
	short		bit12:1;
	short		bit13:1;
	short		bit14:1;
	short		bit15:1;
} BIT_SHORT;


void	PrintSendData(DWORD nbsend);
void	PrintRecvData(DWORD nbrecv);
int		DBread_by_name( char *name,  DB_DATA *db);
unsigned short	GetCharBits (unsigned short v,unsigned short start_bit,unsigned short bit_num );
int	SendPLC_MB(SOCKET , char *, int);
int RecvPLC_MB(SOCKET ,char *,int,int);
int WritePLC_MB(SOCKET , unsigned char , int , int , unsigned char *);
int ReadPLC_MB(SOCKET , unsigned char , int , int , unsigned char *);
int	CloseSock(SOCKET *p_sockplc);
int Swap4Str(unsigned char *buf,int wordlen);


char log_str[256];
#define INF 1
#define ERR 2
#define PLC_APP 1
