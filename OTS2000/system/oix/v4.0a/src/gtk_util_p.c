
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include <gconvert.h>
extern  int  searchLableStr( char* indent ,char *outStr  )  ; 

void s_error(char    *problem_string)
{
	printf("%s\n", problem_string);
	exit(0);
}

int	Max(int x,int y)
{
	return (x>y? x:y);
}

int	Min(int x,int y)
{
	return (x<y? x:y);
}

void	BlankStripCopy (char	*dst, char	*src, int len)
{
	while ( (--len >= 0) && (src[len] == ' ') );

	while ( *src != '\0')
	{
		*dst++ = *src++;
	}
	*dst = '\0';
/*
	printf ("BlankStripCopy: End n= %d, Dst= %s\n", len, dst);
*/
}


void	BlankFillCopy (char    *dst,char    *src,int len)
{
	while ( *src && len-- > 1 )
		*(dst++) = *(src++);

	while (len-- > 1)
		*(dst++) = ' ';
	*dst = '\0';
/*
	printf ("BlankFillCopy: End n= %d, Dst= %s  xxx\n", len, dst);
*/
}


time_t get_sys_date(char* str)
{
	struct tm 	*timeptr;
	time_t 		ttt;
	
	ttt =time (NULL);
	timeptr = localtime(&ttt);

#ifdef ENGLISH_VER
        sprintf ( str,"%02d-%s-%04d",(*timeptr ).tm_mday,
                        mthstr[(*timeptr ).tm_mon], (*timeptr ).tm_year+1900);
#else
        sprintf ( str,"%04d-%02d-%02d",(*timeptr ).tm_year+1900,
                        (*timeptr ).tm_mon+1,(*timeptr ).tm_mday );
#endif
	
	return ttt;
}

