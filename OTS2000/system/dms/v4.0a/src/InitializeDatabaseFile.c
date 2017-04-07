

#include	"../inc/os_type_def.h"
#include	"../inc/dms_structure_def.h"
#include	"../inc/dms_text_def.h"
#include	"../inc/dms_com.h"


int	InitializeDatabaseFile ( char *filename )
{
	static DBS_GBL_COMMON  dms_common_gen;
	FILE	*fp;
	int		ret;

	fp= fopen(filename, "w+");
	if(fp==(FILE*)NULL)
	{
		printf ( "\nOpen or create %s failed!", filename );
		return -1; 
	}

	memset(&dms_common_gen, 0x00, sizeof(dms_common_gen));
	ret= fwrite(&dms_common_gen, 1, sizeof(dms_common_gen), fp);
	fclose(fp);
	if( ret!=sizeof(dms_common_gen) )
	{
		printf ( "\nWrite to %s failed!\n", filename );
		return -1; 
	}
	
	return 0;
}


int	InitErrorFile(char *err_file)
{
	FILE	*fp_err;

	fp_err= fopen ( err_file, "w" );
	if ( fp_err == ( FILE*)NULL )
	{
		printf ( "\nFailed to create %s!\n", err_file );
		return -1;
	}
	fclose(fp_err);
	return 0;
}


