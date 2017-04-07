
#include	"../inc/os_type_def.h"
#include	"../inc/dms_com.h"

extern char err_file[];

int	PrintError(char *buf)
{
	FILE		*fp;
	extern int err_count;

	err_count++;
	fp= fopen ( err_file, "a+" );
	if(fp==(FILE*)NULL)
	{
		printf("\nFailed to open %s", err_file);
		return -1;
	}
	fprintf(fp, "\n%s", buf);
	fclose(fp);
	return 0;
}



