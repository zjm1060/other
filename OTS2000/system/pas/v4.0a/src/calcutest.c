/* calcutest.c program
   cc -o calcutest.dxe calcutest.c calcufn.o /DBLIB/dblib.dlb
*/

#define DEBUG

#ifdef  SUN_OS
#include<unistd.h>
#endif

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include "../../../pas/v4.0a/inc/calcugen.h"

extern	int	CalcuFileLoad(short i);
extern	void	Calcu_Judge(short i,int numb);
extern	void	Calcu_Dat(short i,int numd);
extern	void	CalcuFileFree(short i);
extern	void	CalcuFnts(short i);
extern	void	CalcuShowStruct(short i);

int  PdcCheck(int i)
{
	return(0);
}

void	main(int argc, char *argv[])
{
	short	i;

	if(argc<2)
		i=0;
	else
		i=atoi(argv[1]);
	if(i>1)
		i=1;

	if(CalcuFileLoad(i)==-1)
	{
		printf("Calcugen.dat error!\n");
		exit(1);
	}
	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}

#ifdef DEBUG
	CalcuShowStruct(i);
#endif
	CalcuFnts(i);

	lan_finish(0);
	UnMountDB();
	CalcuFileFree(i);
}
