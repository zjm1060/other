
#include	"../inc/os_type_def.h"


char	strbuf[5][128];


int	GetStrings ( char *bufptr, int lineNo )
{
	unsigned int j;
	int		m, position[6];
	char	*pointer, *ptr;
	char	str_buffer[256];

	static const char *dbInStringFormat = "%s";

	strbuf[0][0] = '\0';
	strbuf[1][0] = '\0';
	strbuf[2][0] = '\0';
	strbuf[3][0] = '\0';
	strbuf[4][0] = '\0';

	if( strlen(bufptr) >= sizeof(str_buffer) )
		strncpy(str_buffer, bufptr, sizeof(str_buffer));
	else
		strcpy(str_buffer, bufptr);

	for(j=0; j<strlen(str_buffer); j++)
	{
		if ( ( str_buffer[j] == '=') | ( str_buffer[j] == ',') | ( str_buffer[j] == '\t') )
			str_buffer[j] = ' ';

		if ( str_buffer[j] == 0x0d )
		{
			str_buffer[j] = '\0';
			break;
		}
	}
	
	m = 1;
	position[0] = 0;
	position[1] = -1;
	position[2] = -1;
	position[3] = -1;
	position[4] = -1;
	position[5] = -1;
	for(j=0; j<strlen(str_buffer)-1; j++ )
	{
		if ( ( str_buffer[j] == ' ')  & ( str_buffer[j+1] != ' ') )
		{
			position[m] = j+1;
			m++;
			if (m>5)
				break;
		}
	}
	
	for(j=0; j<5; j++)
	{
		if ( position[j] == -1)
			break;

		if ( str_buffer[0] == ' ')
		{
			if( position[j+1] == -1 )
				break;
			else
				pointer = str_buffer + position[j+1];
		}
		else
			pointer = str_buffer + position[j];

		if ( ( j > 0) & ( !strncmp(strbuf[j-1], "LONGNAME", 8)
						||!strncmp(strbuf[j-1], "DSP_FILE", 8) 
						||!strncmp(strbuf[j-1], "COMMENT", 7)
						||!strncmp(strbuf[j-1], "TERMINAL_NO", 11) ) )
		{
			pointer += 1;
			ptr = strchr(pointer, '\'');
			if( ptr != NULL )
			{
				*ptr= 0;
				strcpy(strbuf[j], pointer);
			}
			else
			{
/*				printf("\nString format error! lineNo: %d", lineNo); */
				return -1;
			}

		}
		
		else
			sscanf(pointer, dbInStringFormat, strbuf[j]);
	}

	return 0;

}
