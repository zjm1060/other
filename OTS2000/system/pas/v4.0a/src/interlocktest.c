/* interlocktest.c program
   cc -o interlocktest.dxe interlocktest.c interlockfn.o /DBLIB/dblib.dlb
*/
#define STRICT
#define DEBUG

#ifdef  SUN_OS
#include<unistd.h>
#endif
#include<stdio.h>


#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../pas/v4.0a/inc/interlockgen.h"


void	main()
{
	char	name_input[POINT_NAME_SIZE];

	if(InterLockFileLoad()==-1)
	{
		printf("InterLock.dat error!\n");
		exit(1);
	}
	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	for(;;)
	{
		printf("\nPlease Input the interlock point name:");
		if(gets(name_input)==NULL)
		{
			printf("Input null name,exit now!\n");
			break;
		}

		if(strlen(name_input)==0)
		{
			printf("CR received,exit now!\n");
			break;
		}
		printf("\t InterLock_Judge() return %d\n",InterLock_Judge(name_input));
	}

	UnMountDB();
	InterLockFree();
}
