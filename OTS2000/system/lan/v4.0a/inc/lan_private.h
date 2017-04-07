/************************************************************************
"lan_private.h"

abstract:
some private function definition such as lan_out()	
	
revision history:
2005.11.1	start to modify the code for H9000 V4.0		yyp
************************************************************************/
#ifndef	_LAN_PRIVATE
#define	_LAN_PRIVATE
#ifdef __cplusplus
extern "C" {
#endif
#define		IDLE_FLAG	0
#define		LOCK_FLAG	1
#define 	TCP_PORT 	2560
#define 	UDP_PORT_JK        2350 //¼à¿Ø¶Ë¿Ú
#define 	UDP_PORT_B_JK      2351 //¼à¿Ø¶Ë¿Ú
#define 	UDP_PORT        2450 //·ÂÕæ¶Ë¿Ú
#define 	UDP_PORT_B      2451 //·ÂÕæ¶Ë¿Ú
#define		MAX_PKG_NUM		65535					/* max package number for lan_out 	*/
#define		MAX_IDBUF_NUM	1200					/* max package id to be saved in buffer */			
#define		MAX_UDPRCV_BUF	1024000
#define		MAX_UDPSND_BUF	57344
#define		MAX_TCPRCV_BUF	57344
#define		MAX_TCPSND_BUF	57344

typedef struct								/****struct of package id for V4.0 added by yyp 05.12.12********/
{
	char src_id;
	unsigned short pkg_num;
}PKGID;

#ifdef MSWIN32
extern	HANDLE		sem_recv_id,sem_send_id,sem_plc_data_id,sem_net_cmd_id,sem_plc_net_id,sem_plc_send_id;
#endif
static		char *netname[] = {"NETA_", "NETB_"};

SOCKET 		tcp_open(unsigned short ,char,short);
int 		tcp_recv(SOCKET,	char *,short);
int		tcp_send(SOCKET, char *,int);
void		tcp_close(SOCKET);
int		tcp_init(unsigned short);
int		sig_pipe();
void		Timer(int );
void		sig_child(int );

SOCKET		udp_init(int port);
SOCKET		udp_open(char,short);
SOCKET		udp_init_grp(int port);
SOCKET		udp_open_grp(char,short);
void		udp_close();
int		udp_recv(SOCKET,char *,int);
int		udp_send(SOCKET,char *,int,short);
BOOL		uet_send();	 
BOOL		uet_recv(SOCKET);
BOOL	net_recv(SOCKET sock);
int		net_send();
int		udb_net(char* ,short);

int		NetSwitchCmd(LCUID); 
unsigned short	GetRandID();
BOOL		IsExistID(char src_id,unsigned short pkgid,PKGID *recvid);
int		get_app_name(enum app_code,char *log_filename);

#ifdef __cplusplus
}
#endif

#endif
