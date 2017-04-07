#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/mman.h>
#include	<fcntl.h>

#include "/DBINC/dmsstd.h"
#include "/DBINC/dmserr.h"
#include "/DBINC/dmsdef.h"
#include "/DBINC/dmscom.h"

extern DBS_GBL_COMMON  *dms_common_g;

char    strbuf[4][80];
int     strcout;


int	GetNumberByID( point )
POINTER		*point;
{
	int     i, start_ptr, end_ptr, tmp_nb;
	GLONG	no_group, no_record;
	extern  char    groupname[GROUP_ENTRY_NUMBER][GROUP_NAME_SIZE];

	no_group = -1;
	no_record = -1;

	tmp_nb = dms_common_g->Table_Union.TableEntry.group_num;
#ifdef  SUN
	Swap4Byte(&tmp_nb);
#endif
	for ( i=1; i<=tmp_nb; i++ )
	{
		if ( strcmp ( dms_common_g->Group_Union.GroupEntry[i].group_name,
			 groupname[point->dev_id]) == 0)
		{
			no_group = i;
			break;
		}
	}

	if ( no_group == -1)
	{
		printf ( "\nDBread:_GetNumberByID : %s No this group entry in the database file !!!" 
			,groupname[point->dev_id]);

		 printf ( "\nDBread:_GetNumberByID : dev=%d  point=%d   type=%d \n",

			point->dev_id,point->point_id,point->data_type );

		return (-1);
	}

#ifdef  SUN
	start_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off;
	Swap4Byte(&start_ptr);
	end_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num;
	Swap4Byte(&end_ptr);
	end_ptr += start_ptr;
#else
	start_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off;
	end_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off
			+ dms_common_g->Group_Union.GroupEntry[no_group].rec_num;
#endif


	for ( i=start_ptr; i<end_ptr; i++ ) 
	{
		if ( strcmp ( dms_common_g->Record_Union.RecordEntry[i].record_name,
			 recordname[point->data_type - 1]) == 0)
		{
			no_record = i;
			break;
		}
	}

	if ( no_record == -1)
	{
		printf ( "\nDBread__GetNumberByID : %s : No this record entry in the database file !!!",
			 recordname[point->data_type - 1]);

		printf ( "\nDBread:_GetNumberByID : dev=%d  point=%d   type=%d \n",

			point->dev_id,point->point_id,point->data_type );

		return (-1);
	}

	tmp_nb = dms_common_g->Record_Union.RecordEntry[no_record].ent_num;
#ifdef  SUN
	Swap4Byte(&tmp_nb);
#endif
	return ( tmp_nb );

}




int	GetNumberByName ( record_name )
char	*record_name;
{
	int     i, num, len, start_ptr, end_ptr, typeflag, tmp_nb;
	char	rec_name[80];
	GLONG	no_group, no_record;


	strcpy ( rec_name, record_name );
	len = strlen ( rec_name );
	for ( i=0; i<=len; i++ )
	{
		if ( rec_name[i] == '.')
			rec_name[i] = ' ';
	}

	GetStrings( rec_name );


	no_group = -1;
	no_record = -1;

	tmp_nb = dms_common_g->Table_Union.TableEntry.group_num;
#ifdef  SUN
	Swap4Byte(&tmp_nb);
#endif
	for ( i=1; i<=tmp_nb; i++ )
	{
		if ( strcmp ( dms_common_g->Group_Union.GroupEntry[i].group_name, strbuf[0]) == 0)
		{
			no_group = i;
			break;
		}
	}

	if ( no_group == -1)
	{
		printf ( "\nDBread_GetNumberByName: %s:No this group entry in the database file !!!",
			strbuf[0]);
		return (-1);
	}

#ifdef  SUN
	start_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off;
	Swap4Byte(&start_ptr);
	end_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num;
	Swap4Byte(&end_ptr);
	end_ptr += start_ptr;
#else
	start_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off;
	end_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off
			+ dms_common_g->Group_Union.GroupEntry[no_group].rec_num;
#endif

	for ( i=start_ptr; i<end_ptr; i++ ) 
	{
		if ( strcmp ( dms_common_g->Record_Union.RecordEntry[i].record_name, strbuf[1]) == 0)
		{
			no_record = i;
			break;
		}
	}

	if ( no_record == -1)
	{
		printf ( "\nDBread_GetNumberByName:%s: No this record entry in database file !!!",
			strbuf[1]);
		return (-1);
	}

	tmp_nb = dms_common_g->Record_Union.RecordEntry[no_record].ent_num;
#ifdef  SUN
	Swap4Byte(&tmp_nb);
#endif
	return ( tmp_nb );
}




int	GetRtuNumber ()
{
	int	group_nb;

	group_nb = dms_common_g->Table_Union.TableEntry.group_num;
#ifdef  SUN
	Swap4Byte(&group_nb);
#endif
	return ( group_nb );
}




void	GetRtuNameByID ( point, rtu_name )
char	*rtu_name;
POINTER		*point;
{
	GLONG	no_group;
	int	group_nb;

	no_group = point->dev_id;

	group_nb = dms_common_g->Table_Union.TableEntry.group_num;
#ifdef  SUN
	Swap4Byte(&group_nb);
#endif

	if (( no_group <= 0) & ( no_group > group_nb ) )
	{
		printf ( "\nDBread: No this group entry in the database file !!!" );
		strcpy ( rtu_name, "NO_RTU" );
	}
	else
	{
		strcpy ( rtu_name, dms_common_g->Group_Union.GroupEntry[point->dev_id].group_name );
	}


}




int	GetRecordNo ( point )
POINTER		*point;
{
	int     i, start_ptr, end_ptr, group_nb;
	GLONG	no_group, no_record, no_entry;
	extern  char    groupname[GROUP_ENTRY_NUMBER][GROUP_NAME_SIZE];


	no_group = -1;
	no_record = -1;

	group_nb = dms_common_g->Table_Union.TableEntry.group_num;
#ifdef  SUN
	Swap4Byte(&group_nb);
#endif
	for ( i=1; i<=group_nb; i++ )
	{
		if ( strcmp ( dms_common_g->Group_Union.GroupEntry[i].group_name,
			 groupname[point->dev_id]) == 0)
		{
			no_group = i;
			break;
		}
	}

	if ( no_group == -1)
	{
		printf ( "\nNo this group entry in the database file !!!" );
		return ( -1);
	}

#ifdef  SUN
	start_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off;
	Swap4Byte(&start_ptr);
	end_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num;
	Swap4Byte(&end_ptr);
	end_ptr += start_ptr;
#else
	start_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off;
	end_ptr = dms_common_g->Group_Union.GroupEntry[no_group].rec_num_off
			+ dms_common_g->Group_Union.GroupEntry[no_group].rec_num;
#endif

	for ( i=start_ptr; i<end_ptr; i++ ) 
	{
		if ( strcmp ( dms_common_g->Record_Union.RecordEntry[i].record_name,
			 recordname[point->data_type - 1]) == 0)
		{
			no_record = i;
			break;
		}
	}

	if ( no_record == -1)
	{
		printf ( "\nNo this record entry in relative group of the database file !!!" );
		return ( -1);
	}


	return ( no_record );

}
