
// yh must read and tell others!!!!

/*
	ReadRecvBuf
	created by yh 2003.6.25
*/
/*
	History:  ( the most recent record should be added first so one can see imediately
				the recent modification when open the file )

	modified by wdk 2003.6.26: rename unclear variable & subroutine names.
	modified by wdk 2002.2.26: rename unclear .	// this should be deleted.

*/

#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
#include "/home/ems/system/ots/SimuEngine/inc/SimuEngine_fun.h"

int FirstCmd=TRUE;

int	ReadCMD_FromNetcmdBuf ( short *ptr, char *buff );

/* pointer initial */

void	LAN_ReadPtrInit ( short *ptr )
{
	lan_init_all();
	*ptr = shm_net_cmd->head;
}		
	

/* read   recv_buffer when dp_type is COMM_RECORD */

int	ReadCMD_FromNetcmdBuf ( short *ptr, char *buff )
{
	short	shm_end;
	int i; 	

	shm_end	= *ptr ;

	while ( shm_end != shm_net_cmd->head )
	{
		if(shm_net_cmd->seg[shm_end].lan_head.dp_type != COMM_SIM)
		{
			shm_end ++ ;
			if ( shm_end >= MAX_NETCMD_SEG_NUM )
				shm_end = 0;
			continue;
		}
		memcpy(buff, &(shm_net_cmd->seg[shm_end]), 
			shm_net_cmd->seg[shm_end].lan_head.length + LANH_SIZE);
		printf("\nid=%02d: ",shm_end);
		for(i=0;i<shm_net_cmd->seg[shm_end].lan_head.length;i++)
		{
			printf("=%x",shm_net_cmd->seg[shm_end].net_cmd_buf.data[i]);
		}
		shm_end++;
		if ( shm_end >= MAX_NETCMD_SEG_NUM )
			shm_end = 0;
		*ptr	= shm_end;
		return ( TRUE );
	}
	*ptr	= shm_end;
	return ( FALSE );
} 
